#include "cla/cla.h"
#include <ctype.h>           // isalpha
#include <errno.h>           // errno
#include <stdio.h>           // fprintf, stderr
#include <stdlib.h>          // calloc, exit
#include <string.h>          // strerror


enum key_type {
    KEY_TYPE_ERR = 0,
    KEY_TYPE_REQUIRED,
    KEY_TYPE_OPTIONAL,
    KEY_TYPE_FLAG,
};

enum token_type {
    TOKEN_TYPE_ERR = 0,
    TOKEN_TYPE_REQUIRED = KEY_TYPE_REQUIRED,
    TOKEN_TYPE_OPTIONAL = KEY_TYPE_OPTIONAL,
    TOKEN_TYPE_FLAG = KEY_TYPE_FLAG,
    TOKEN_TYPE_POSITIONAL,
    TOKEN_TYPE_EXENAME,
    TOKEN_TYPE_VALUE,
};

struct key {
    const char * longname;
    const char * shortname;
    int noccurrences;
    enum key_type type;
};

struct keys {
    int cap;
    int len;
    struct key * items;
};

struct token {
    char * str;
    int ikey;
    enum token_type type;
};

struct tokens {
    int cap;
    int len;
    struct token * items;
};

// command line arguments
struct cla {
    bool isfinal;
    struct keys keys;
    int npositionals;
    struct tokens tokens;
};


static void add_key (struct cla * cla, const char * longname, const char * shortname, enum key_type type);
static void assert_is_named (const char * longname, const char * shortname);
static void assert_longname_is_compliant (const char * longname);
static void assert_longname_isnt_duplicate (const struct cla * self, const char * longname);
static void assert_required_keys_are_present (const struct cla * self);
static void assert_shortname_is_compliant (const char * shortname);
static void assert_shortname_isnt_duplicate (const struct cla * self, const char * shortname);
static void assert_token_not_repeated (struct cla * self, enum key_type type);
static int find_key_by_name (const struct cla * self, const char * name);


static void add_key (struct cla * self, const char * longname, const char * shortname, enum key_type type) {

    fprintf(stderr, "// TODO: consider managing your own memory for shortname and longname\n");

    assert_is_named(longname, shortname);
    assert_shortname_is_compliant(shortname);
    assert_shortname_isnt_duplicate(self, shortname);
    assert_longname_is_compliant(longname);
    assert_longname_isnt_duplicate(self, longname);

    if (self->keys.len >= self->keys.cap) {
        fprintf(stderr, "ERROR: Can't add any more keys, aborting.\n");
        exit(EXIT_FAILURE);
    }

    int * i = &self->keys.len;
    self->keys.items[*i] = (struct key) {
        .longname = longname,
        .shortname = shortname,
        .noccurrences = 0,
        .type = type,
    };
    (*i)++;
}


static void assert_is_named (const char * longname, const char * shortname) {
    if (longname == nullptr && shortname == nullptr) {
        fprintf(stderr, "ERROR: Can't add an unnamed option, aborting.\n");
        exit(EXIT_FAILURE);
    }
}


static void assert_longname_is_compliant (const char * longname) {
    if (longname == nullptr) return;
    if (strnlen(longname, 4) <= 3) {
        fprintf(stderr, "ERROR: longname \"%s\" should be at least 4 characters, aborting.\n", longname);
        exit(EXIT_FAILURE);
    }
    if (strnlen(longname, 65) > 64) {
        fprintf(stderr, "ERROR: longname \"%s\" should be at most 64 characters, aborting.\n", longname);
        exit(EXIT_FAILURE);
    }
    if (longname[0] != '-') {
        fprintf(stderr, "ERROR: longname \"%s\" should start with \"--\", aborting.\n", longname);
        exit(EXIT_FAILURE);
    }
    if (longname[1] != '-') {
        fprintf(stderr, "ERROR: longname \"%s\" should start with \"--\", aborting.\n", longname);
        exit(EXIT_FAILURE);
    }
    if (!isalpha(longname[2])) {
        fprintf(stderr, "ERROR: longname \"%s\" character at index 2 should\n"
                        "be [a-zA-Z], aborting.\n", longname);
        exit(EXIT_FAILURE);
    }
}


static void assert_longname_isnt_duplicate (const struct cla * self, const char * longname) {
    if (longname == nullptr) return;
    for (int i = 0; i < self->keys.len; i++) {
        if (strcmp(self->keys.items[i].longname, longname) == 0) {
            fprintf(stderr, "ERROR: longname \"%s\" already exists, aborting.\n", longname);
            exit(EXIT_FAILURE);
        }
    }
}


static void assert_required_keys_are_present (const struct cla * self) {
    for (struct key * key = &self->keys.items[0];
        key < &self->keys.items[self->keys.len];
        key++) {

        if (key->type != KEY_TYPE_REQUIRED) continue;

        bool found = false;
        for (struct token * token = &self->tokens.items[0];
            token < &self->tokens.items[self->tokens.len];
            token++) {

            if (token->type != TOKEN_TYPE_REQUIRED) continue;

            bool token_str_matches_key_shortname = key->shortname != nullptr && strcmp(token->str, key->shortname) == 0;
            bool token_str_matches_key_longname = key->longname != nullptr && strcmp(token->str, key->longname) == 0;

            if (token_str_matches_key_shortname || token_str_matches_key_longname) {
                found = true;
                break;
            }
        }
        if (found == false) {
            if (key->longname != nullptr && key->shortname != nullptr) {
                fprintf(stderr, "ERROR: Required key '%s/%s' not found%s, aborting.\n",
                        key->shortname, key->longname,
                        self->npositionals > 0 ? " or not all positionals have been defined" : "");
                exit(EXIT_FAILURE);
            } else if (key->longname != nullptr) {
                fprintf(stderr, "ERROR: Required key '%s' not found%s, aborting.\n", key->longname,
                        self->npositionals > 0 ? " or not all positionals have been defined" : "");
                exit(EXIT_FAILURE);
            } else {
                fprintf(stderr, "ERROR: Required key '%s' not found%s, aborting.\n", key->shortname,
                        self->npositionals > 0 ? " or not all positionals have been defined" : "");
                exit(EXIT_FAILURE);
            }
        }
    }
}


static void assert_shortname_is_compliant (const char * shortname) {
    if (shortname == nullptr) return;
    if (strnlen(shortname, 3) != 2) {
        fprintf(stderr, "ERROR: shortname \"%s\" should be length 2, aborting.\n", shortname);
        exit(EXIT_FAILURE);
    }
    if (shortname[0] != '-') {
        fprintf(stderr, "ERROR: shortname \"%s\" should start with \'-\', aborting.\n", shortname);
        exit(EXIT_FAILURE);
    }
    if (!isalpha(shortname[1])) {
        fprintf(stderr, "ERROR: shortname \"%s\" character at index 1 should\n"
                        "be [a-zA-Z], aborting.\n", shortname);
        exit(EXIT_FAILURE);
    }
}


static void assert_shortname_isnt_duplicate (const struct cla * self, const char * shortname) {
    if (shortname == nullptr) return;
    for (int i = 0; i < self->keys.len; i++) {
        if (strcmp(self->keys.items[i].shortname, shortname) == 0) {
            fprintf(stderr, "ERROR: shortname \"%s\" already exists, aborting.\n", shortname);
            exit(EXIT_FAILURE);
        }
    }
}


static void assert_token_not_repeated (struct cla * self, enum key_type type) {
    for (struct key * key = &self->keys.items[0];
         key < &self->keys.items[self->keys.len];
         key++) {

        if (key->type == type && key->noccurrences > 1) {
            if (key->longname != nullptr && key->shortname != nullptr) {
                fprintf(stderr, "ERROR: Found multiple usages of key '%s/%s', aborting.\n", key->shortname, key->longname);
            } else if (key->longname != nullptr) {
                fprintf(stderr, "ERROR: Found multiple usages of key '%s', aborting.\n", key->longname);
            } else {
                fprintf(stderr, "ERROR: Found multiple usages of key '%s', aborting.\n", key->shortname);
            }
            exit(EXIT_FAILURE);
        }
    }
}


void CLA_add_flag (struct cla * self, const char * longname, const char * shortname) {
    add_key(self, longname, shortname, KEY_TYPE_FLAG);
}


void CLA_add_optional (struct cla * self, const char * longname, const char * shortname) {
    add_key(self, longname, shortname, KEY_TYPE_OPTIONAL);
}


void CLA_add_positionals (struct cla * self, int npositionals) {
    self->npositionals = npositionals;
}


void CLA_add_required (struct cla * self, const char * longname, const char * shortname) {
    add_key(self, longname, shortname, KEY_TYPE_REQUIRED);
}


int CLA_count_flag (const struct cla * self, const char * name) {
    if (self->isfinal == false) {
        fprintf(stderr, "ERROR: arguments haven't been parsed yet, aborting.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "// TODO: review logic\n");

    for (struct token * token = &self->tokens.items[0];
         token < &self->tokens.items[self->tokens.len];
         token++) {

        if (token->type != TOKEN_TYPE_FLAG) continue;

        struct key * key = &self->keys.items[token->ikey];
        bool token_str_matches_key_shortname = key->shortname != nullptr && strcmp(token->str, key->shortname) == 0;
        bool token_str_matches_key_longname = key->longname != nullptr && strcmp(token->str, key->longname) == 0;

        if (token_str_matches_key_shortname || token_str_matches_key_longname) {
            return key->noccurrences;
        }
    }
    return 0;
}


struct cla * CLA_create (void) {
    fprintf(stderr, "// TODO: memory growing when adding more than 10 keys\n");

    errno = 0;
    struct cla * self = calloc(1, sizeof(struct cla));
    if (self == nullptr) {
        fprintf(stderr, "%s\nERROR: Could not allocate memory for instance of \"struct cla\", aborting.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int cap = 10;
    errno = 0;
    self->keys = (struct keys) {
        .cap = cap,
        .items = calloc(cap, sizeof(struct key)),
        .len = 0,
    };
    if (self->keys.items == nullptr) {
        fprintf(stderr, "%s\nERROR: Could not allocate memory for keys array, aborting.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return self;
}


void CLA_destroy (struct cla ** self) {

    free((*self)->keys.items);
    (*self)->keys.items = nullptr;

    for (int i = 0; i < (*self)->tokens.len; i++) {
        free((*self)->tokens.items[i].str);
        (*self)->tokens.items[i].str = nullptr;
    }

    free((*self)->tokens.items);
    (*self)->tokens.items = nullptr;

    free(*self);
    *self = nullptr;
}


const char * CLA_get_optional_value (const struct cla * self, const char * name) {
    if (self->isfinal == false) {
        fprintf(stderr, "ERROR: arguments haven't been parsed yet, aborting.\n");
        exit(EXIT_FAILURE);
    }
    int ikey = find_key_by_name(self, name);
    for (int itoken = 0; itoken < self->tokens.len; itoken++) {
        if (self->tokens.items[itoken].ikey == ikey) {
            return self->tokens.items[itoken + 1].str;
        }
    }
    return nullptr;
}


const char * CLA_get_positional_value (const struct cla * self, int ipos) {
    if (ipos < 0) {
        fprintf(stderr, "ERROR: Can't use a negative index to retrieve a positional argument, aborting.\n");
        exit(EXIT_FAILURE);
    }
    if (ipos >= self->npositionals) {
        fprintf(stderr, "ERROR: Requested positional argument does not exist, aborting.\n");
        exit(EXIT_FAILURE);
    }
    int i = self->tokens.len - self->npositionals + ipos;
    return self->tokens.items[i].str;
}


const char * CLA_get_required_value (const struct cla * self, const char * name) {
    const char * value = CLA_get_optional_value(self, name);
    if (value == nullptr) {
        fprintf(stderr, "ERROR: Required argument '%s' not found, aborting.\n", name);
        exit(EXIT_FAILURE);
    }
    return value;
}


void CLA_parse (struct cla * self, int argc, const char * argv[]) {

    errno = 0;
    self->tokens = (struct tokens) {
        .cap = argc,
        .items = calloc(argc, sizeof(struct token)),
        .len = argc,
    };
    if (self->tokens.items == nullptr) {
        fprintf(stderr, "%s\nERROR: Could not allocate memory for token array, aborting.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // copy contents of argv into self->tokens
    for (int i = 0; i < argc; i++) {
        int len = strlen(argv[i]) + 1;
        self->tokens.items[i].str = calloc(len, sizeof(char *));
        if (self->tokens.items[i].str == nullptr) {
            fprintf(stderr, "%s\nError allocating memory for token string, aborting.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        strcpy(self->tokens.items[i].str, argv[i]);
    }

    // the first token is always the executable name
    self->tokens.items[0].type = TOKEN_TYPE_EXENAME;
    self->tokens.items[0].ikey = -1;

    // iterate over the non-positionals: flags, requireds, optionals
    int ipos0 = argc - self->npositionals;
    if (ipos0 < 1) {
        fprintf(stderr, "ERROR: Expected at least %d positional arguments, aborting.\n", self->npositionals);
        exit(EXIT_FAILURE);
    }
    for (int itoken = 1; itoken < ipos0; itoken++) {

        // shorthand notation
        struct token * previous_token = &self->tokens.items[itoken - 1];
        struct token * token = &self->tokens.items[itoken];

        // see if the previous token requires an associated value 
        if (previous_token->type == TOKEN_TYPE_REQUIRED || previous_token->type == TOKEN_TYPE_OPTIONAL) {
            token->type = TOKEN_TYPE_VALUE;
            token->ikey = -1;
            continue;
        }

        // if there is a key whose longname or shortname matches
        // token's str, assign key->type to token->type
        int ikey = find_key_by_name(self, token->str);
        if (ikey == -1) {
            fprintf(stderr, "ERROR: Found unknown option '%s', aborting.\n", self->tokens.items[itoken].str);
            exit(EXIT_FAILURE);
        }
        token->type = (enum token_type) self->keys.items[ikey].type;
        token->ikey = ikey;

        // keep track of how many times the key was used
        self->keys.items[ikey].noccurrences++;

        if (itoken == ipos0 - 1) {
            if (token->type == TOKEN_TYPE_REQUIRED || token->type == TOKEN_TYPE_OPTIONAL) {
                fprintf(stderr, "ERROR: '%s' requires a value but none given%s, aborting.\n",
                        token->str, self->npositionals > 0 ? " or not all positionals have been defined" : "");
                exit(EXIT_FAILURE);
            }
        }
    }

    // assign positional type to the last self->npositionals tokens
    for (int itoken = ipos0; itoken < argc; itoken++) {
        self->tokens.items[itoken].type = TOKEN_TYPE_POSITIONAL;
        self->tokens.items[itoken].ikey = -1;
    }

    assert_required_keys_are_present(self);
    assert_token_not_repeated(self, KEY_TYPE_REQUIRED);
    assert_token_not_repeated(self, KEY_TYPE_OPTIONAL);

    self->isfinal = true;
}


void CLA_print_parse_result (const struct cla * self) {
    const char * typenames[] = {
        [TOKEN_TYPE_ERR] = "error",
        [TOKEN_TYPE_REQUIRED] = "required",
        [TOKEN_TYPE_OPTIONAL] = "optional",
        [TOKEN_TYPE_FLAG] = "flag",
        [TOKEN_TYPE_POSITIONAL] = "positional",
        [TOKEN_TYPE_EXENAME] = "exename",
        [TOKEN_TYPE_VALUE] = "value",
    };
    fprintf(stdout, "Command line input parsed as:\n");
    for (int itoken = 0; itoken < self->tokens.len; itoken++) {
        struct token * token = &self->tokens.items[itoken];
        fprintf(stdout, "%-10s %s\n", typenames[token->type], token->str);
    }
    fprintf(stdout, "\n");
}


static int find_key_by_name (const struct cla * self, const char * name) {
    if (name == nullptr) return -1;
    for (int ikey = 0; ikey < self->keys.len; ikey++) {
        struct key * key = &self->keys.items[ikey];
        bool a = (key->shortname != nullptr) && (strcmp(key->shortname, name) == 0);
        bool b = (key->longname != nullptr) && (strcmp(key->longname, name) == 0);
        if (a || b) return ikey;
    }
    return -1;
}


bool CLA_has_flag (const struct cla * self, const char * name) {
    return CLA_count_flag(self, name) > 0;
}
