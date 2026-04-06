#include "cla/cla.h"
#include <ctype.h>           // isalnum
#include <errno.h>           // errno
#include <stdio.h>           // fprintf, stderr
#include <stdlib.h>          // calloc, exit
#include <string.h>          // strerror


enum key_type {
    KEY_TYPE_ERR = 0,
    KEY_TYPE_REQUIRED,
    KEY_TYPE_OPTIONAL,
    KEY_TYPE_FLAG,
    KEY_TYPE_COUNT,
};

enum token_type {
    TOKEN_TYPE_ERR = 0,
    TOKEN_TYPE_REQUIRED = KEY_TYPE_REQUIRED,
    TOKEN_TYPE_OPTIONAL = KEY_TYPE_OPTIONAL,
    TOKEN_TYPE_FLAG = KEY_TYPE_FLAG,
    TOKEN_TYPE_POSITIONAL,
    TOKEN_TYPE_EXENAME,
    TOKEN_TYPE_VALUE,
    TOKEN_TYPE_COUNT,
};

struct key {
    char * name;
    char * alias;
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

// implementation of opaque type from the API
struct cla {
    bool isfinal;
    bool help_requested;
    struct keys keys;
    int npositionals;
    struct tokens tokens;
};


static void add_key (struct cla * cla, const char * name, const char * alias, enum key_type type);
static void assert_alias_is_compliant (const char * alias);
static void assert_alias_isnt_duplicate (const struct cla * self, const char * alias);
static void assert_alias_isnt_h (const char * alias);
static void assert_arguments_have_been_parsed (const struct cla * self);
static void assert_arguments_have_not_been_parsed (const struct cla * self);
static void assert_is_named (const char * name, const char * alias);
static void assert_key_exists (int ikey, const char * name);
static void assert_key_is_of_type (const struct cla * self, int ikey, const char * name, enum key_type type);
static void assert_key_is_used (const struct cla * self, int ikey, const char * name);
static void assert_name_is_compliant (const char * name);
static void assert_name_isnt_duplicate (const struct cla * self, const char * name);
static void assert_name_isnt_help (const char * name);
static void assert_required_keys_are_present (const struct cla * self);
static void assert_self_is_not_nullptr (const struct cla * self);
static void assert_token_not_repeated (const struct cla * self, enum key_type type);
static int find_key_by_name (const struct cla * self, const char * name);


static void add_key (struct cla * self, const char * name, const char * alias, enum key_type type) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_not_been_parsed(self);
    assert_is_named(name, alias);
    assert_alias_isnt_h(alias);
    assert_alias_is_compliant(alias);
    assert_alias_isnt_duplicate(self, alias);
    assert_name_isnt_help(name);
    assert_name_is_compliant(name);
    assert_name_isnt_duplicate(self, name);

    // if the keys.items array is full, double its capacity, and zero-initialize the new positions
    if (self->keys.len >= self->keys.cap) {
        errno = 0;
        self->keys.items = reallocarray(self->keys.items, self->keys.cap * 2, sizeof(struct key));
        if (self->keys.items == nullptr) {
            const int code = 1;
            fprintf(stderr, "%s\nERROR %d: Could not allocate memory for growing keys array, aborting.\n", strerror(errno), code);
            exit(code);
        }
        self->keys.cap *= 2;
        for (int i = self->keys.len; i < self->keys.cap; i++) {
            self->keys.items[i] = (struct key){};
        }
    }

    // define the struct key that will hold the new key
    int * i = &self->keys.len;
    self->keys.items[*i] = (struct key) {
        .name = nullptr,
        .alias = nullptr,
        .noccurrences = 0,
        .type = type,
    };

    // if the new key has a name, set some memory aside for it and copy name into it
    if (name != nullptr) {
        char * tmp = calloc(strlen(name) + 1, sizeof(char));
        if (tmp == nullptr) {
            const int code = 2;
            fprintf(stderr, "%s\nERROR %d: Could not allocate memory for name %d, aborting.\n", strerror(errno), code, *i);
            exit(code);
        }
        self->keys.items[*i].name = tmp;
        strcpy(self->keys.items[*i].name, name);
    }

    // if the new key has a alias, set some memory aside for it and copy alias into it
    if (alias != nullptr) {
        char * tmp = calloc(strlen(alias) + 1, sizeof(char));
        if (tmp == nullptr) {
            const int code  = 3;
            fprintf(stderr, "%s\nERROR %d: Could not allocate memory for alias %d, aborting.\n", strerror(errno), code, *i);
            exit(code);
        }
        self->keys.items[*i].alias = tmp;
        strcpy(self->keys.items[*i].alias, alias);
    }

    // increment keys.len
    (*i)++;
}


static void assert_alias_is_compliant (const char * alias) {
    if (alias == nullptr) return;
    if (strnlen(alias, 3) != 2) {
        const int code = 4;
        fprintf(stderr, "ERROR %d: alias \"%s\" should be length 2, aborting.\n", code, alias);
        exit(code);
    }
    if (alias[0] != '-') {
        const int code = 5;
        fprintf(stderr, "ERROR %d: alias \"%s\" should start with \'-\', aborting.\n", code, alias);
        exit(code);
    }
    if (!isalnum(alias[1])) {
        const int code = 6;
        fprintf(stderr, "ERROR %d: alias \"%s\" character at index 1 should "
                        "be [0-9a-zA-Z], aborting.\n", code, alias);
        exit(code);
    }
}


static void assert_alias_isnt_duplicate (const struct cla * self, const char * alias) {
    assert_self_is_not_nullptr(self);
    if (alias == nullptr) return;
    if (find_key_by_name(self, alias) != -1) {
        const int code = 7;
        fprintf(stderr, "ERROR %d: alias \"%s\" already exists, aborting.\n", code, alias);
        exit(code);
    }
}


static void assert_alias_isnt_h (const char * alias) {
    if (alias == nullptr) return;
    if (strcmp(alias, "-h") == 0) {
        const int code = 32;
        fprintf(stderr, "ERROR %d: Adding illegal alias \"%s\" would override accessing help, aborting.\n", code, alias);
        exit(code);
    }
}


static void assert_arguments_have_been_parsed (const struct cla * self) {
    assert_self_is_not_nullptr(self);
    if (self->isfinal == false) {
        const int code = 8;
        fprintf(stderr, "ERROR %d: arguments haven't been parsed yet, aborting.\n", code);
        exit(code);
    }
}


static void assert_arguments_have_not_been_parsed (const struct cla * self) {
    assert_self_is_not_nullptr(self);
    if (self->isfinal == true) {
        const int code = 9;
        fprintf(stderr, "ERROR %d: arguments have already been parsed, aborting.\n", code);
        exit(code);
    }
}


static void assert_is_named (const char * name, const char * alias) {
    if (name == nullptr && alias == nullptr) {
        const int code = 10;
        fprintf(stderr, "ERROR %d: Can't add an unnamed option, aborting.\n", code);
        exit(code);
    }
}


static void assert_key_exists (int ikey, const char * name) {
    if (ikey == -1) {
        const int code = 11;
        fprintf(stderr, "ERROR %d: Unknown argument name '%s', aborting.\n", code, name);
        exit(code);
    }
}


static void assert_key_is_of_type (const struct cla * self, int ikey, const char * name, enum key_type type) {
    assert_self_is_not_nullptr(self);
    const char * key_type_names[KEY_TYPE_COUNT] = {
        [KEY_TYPE_REQUIRED] = "a required argument",
        [KEY_TYPE_OPTIONAL] = "an optional argument",
        [KEY_TYPE_FLAG] = "a flag",
    };
    if (self->keys.items[ikey].type != type) {
        const int code = 12;
        fprintf(stderr, "ERROR %d: Key '%s' is not %s, aborting.\n", code, name, key_type_names[type]);
        exit(code);
    }
}


static void assert_key_is_used (const struct cla * self, int ikey, const char * name) {
    assert_self_is_not_nullptr(self);
    if (self->keys.items[ikey].noccurrences == 0) {
        const int code = 13;
        fprintf(stderr, "ERROR %d: '%s' is valid but it hasn't been used, aborting.\n", code, name);
        exit(code);
    }
}


static void assert_name_is_compliant (const char * name) {
    if (name == nullptr) return;
    if (strnlen(name, 4) <= 3) {
        const int code = 14;
        fprintf(stderr, "ERROR %d: name \"%s\" should be at least 4 characters, aborting.\n", code, name);
        exit(code);
    }
    if (strnlen(name, 65) > 64) {
        const int code = 15;
        fprintf(stderr, "ERROR %d: name \"%s\" should be at most 64 characters, aborting.\n", code, name);
        exit(code);
    }
    if (name[0] != '-' || name[1] != '-') {
        const int code = 17;
        fprintf(stderr, "ERROR %d: name \"%s\" should start with \"--\", aborting.\n", code, name);
        exit(code);
    }
    for (int i = 2; i < (int) strlen(name); i++) {
        if (!isalnum(name[i])) {
            const int code = 18;
            fprintf(stderr, "ERROR %d: name \"%s\" character at index %d should "
                            "be [0-9a-zA-Z], aborting.\n", code, name, i);
            exit(code);
        }
    }
}


static void assert_name_isnt_duplicate (const struct cla * self, const char * name) {
    assert_self_is_not_nullptr(self);
    if (name == nullptr) return;
    if (find_key_by_name(self, name) != -1) {
        const int code = 19;
        fprintf(stderr, "ERROR %d: name \"%s\" already exists, aborting.\n", code, name);
        exit(code);
    }
}


static void assert_name_isnt_help (const char * name) {
    if (name == nullptr) return;
    if (strcmp(name, "--help") == 0) {
        const int code = 33;
        fprintf(stderr, "ERROR %d: Adding illegal name \"%s\" would override accessing help, aborting.\n", code, name);
        exit(code);
    }
}


static void assert_no_help_requested (const struct cla * self) {
    assert_self_is_not_nullptr(self);
    if (self->help_requested) {
        const int code = 34;
        fprintf(stderr, "ERROR %d: Unhandled help request, aborting.\n", code);
        exit(code);
    }
}


static void assert_required_keys_are_present (const struct cla * self) {
    assert_self_is_not_nullptr(self);
    for (struct key * key = &self->keys.items[0];
        key < &self->keys.items[self->keys.len];
        key++) {

        if (key->type != KEY_TYPE_REQUIRED) continue;

        bool found = false;
        for (struct token * token = &self->tokens.items[0];
            token < &self->tokens.items[self->tokens.len];
            token++) {

            if (token->type != TOKEN_TYPE_REQUIRED) continue;

            bool token_str_matches_key_alias = key->alias != nullptr && strcmp(token->str, key->alias) == 0;
            bool token_str_matches_key_name = key->name != nullptr && strcmp(token->str, key->name) == 0;

            if (token_str_matches_key_alias || token_str_matches_key_name) {
                found = true;
                break;
            }
        }
        if (found == false) {
            const int code = 20;
            if (key->name != nullptr && key->alias != nullptr) {
                fprintf(stderr, "ERROR %d: Required key '%s/%s' not found, aborting.\n", code, key->alias, key->name);
            } else if (key->name != nullptr) {
                fprintf(stderr, "ERROR %d: Required key '%s' not found, aborting.\n", code, key->name);
            } else {
                fprintf(stderr, "ERROR %d: Required key '%s' not found, aborting.\n", code, key->alias);
            }
            exit(code);
        }
    }
}


static void assert_self_is_not_nullptr (const struct cla * self) {
    if (self == nullptr) {
        const int code = 21;
        fprintf(stderr, "ERROR %d: struct cla * can't be used uninitialized, aborting.\n", code);
        exit(code);
    }
}

static void assert_token_not_repeated (const struct cla * self, enum key_type type) {
    assert_self_is_not_nullptr(self);
    for (struct key * key = &self->keys.items[0];
         key < &self->keys.items[self->keys.len];
         key++) {

        if (key->type == type && key->noccurrences > 1) {
            const int code = 22;
            if (key->name != nullptr && key->alias != nullptr) {
                fprintf(stderr, "ERROR %d: Found multiple usages of key '%s/%s', aborting.\n", code, key->alias, key->name);
            } else if (key->name != nullptr) {
                fprintf(stderr, "ERROR %d: Found multiple usages of key '%s', aborting.\n", code, key->name);
            } else {
                fprintf(stderr, "ERROR %d: Found multiple usages of key '%s', aborting.\n", code, key->alias);
            }
            exit(code);
        }
    }
}


void CLA_add_flag (struct cla * self, const char * name, const char * alias) {
    add_key(self, name, alias, KEY_TYPE_FLAG);
}


void CLA_add_optional (struct cla * self, const char * name, const char * alias) {
    add_key(self, name, alias, KEY_TYPE_OPTIONAL);
}


void CLA_add_positionals (struct cla * self, int npositionals) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_not_been_parsed(self);
    self->npositionals = npositionals;
}


void CLA_add_required (struct cla * self, const char * name, const char * alias) {
    add_key(self, name, alias, KEY_TYPE_REQUIRED);
}


int CLA_count_flag (const struct cla * self, const char * name) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    assert_no_help_requested(self);
    int ikey = find_key_by_name(self, name);
    assert_key_exists(ikey, name);
    assert_key_is_of_type(self, ikey, name, KEY_TYPE_FLAG);
    return self->keys.items[ikey].noccurrences;
}


struct cla * CLA_create (void) {
    errno = 0;
    struct cla * self = calloc(1, sizeof(struct cla));
    if (self == nullptr) {
        const int code = 23;
        fprintf(stderr, "%s\nERROR %d: Could not allocate memory for instance of \"struct cla\", aborting.\n", strerror(errno), code);
        exit(code);
    }

    int cap = 8;
    errno = 0;
    self->keys = (struct keys) {
        .cap = cap,
        .items = calloc(cap, sizeof(struct key)),
        .len = 0,
    };
    if (self->keys.items == nullptr) {
        const int code = 24;
        fprintf(stderr, "%s\nERROR %d: Could not allocate memory for keys array, aborting.\n", strerror(errno), code);
        exit(code);
    }

    return self;
}


void CLA_destroy (struct cla ** self) {
    // deallocate dynamic memory set aside for storing key.alias for all keys
    for (int i = 0; i < (*self)->keys.len; i++) {
        free((*self)->keys.items[i].alias);
        (*self)->keys.items[i].alias = nullptr;
    }

    // deallocate dynamic memory set aside for storing key.name for all keys
    for (int i = 0; i < (*self)->keys.len; i++) {
        free((*self)->keys.items[i].name);
        (*self)->keys.items[i].name = nullptr;
    }

    // deallocate dynamic memory set aside for storing array of keys
    free((*self)->keys.items);
    (*self)->keys.items = nullptr;

    // deallocate dynamic memory set aside for storing token.str for all tokens
    for (int i = 0; i < (*self)->tokens.len; i++) {
        free((*self)->tokens.items[i].str);
        (*self)->tokens.items[i].str = nullptr;
    }

    // deallocate dynamic memory set aside for storing array of tokens
    free((*self)->tokens.items);
    (*self)->tokens.items = nullptr;

    // deallocate dynamic memory set aside for storing self
    free(*self);
    *self = nullptr;
}


const char * CLA_get_exename (const struct cla * self) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    return self->tokens.items[0].str;
}


const char * CLA_get_value_optional (const struct cla * self, const char * name) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    assert_no_help_requested(self);
    int ikey = find_key_by_name(self, name);
    assert_key_exists(ikey, name);
    assert_key_is_of_type(self, ikey, name, KEY_TYPE_OPTIONAL);
    assert_key_is_used(self, ikey, name);
    for (int itoken = 0; itoken < self->tokens.len; itoken++) {
        if (self->tokens.items[itoken].ikey == ikey) {
            return self->tokens.items[itoken + 1].str;
        }
    }
    return nullptr;
}


const char * CLA_get_value_positional (const struct cla * self, int ipos) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    assert_no_help_requested(self);
    if (ipos < 0) {
        const int code = 25;
        fprintf(stderr, "ERROR %d: Can't use a negative index to retrieve a positional argument, aborting.\n", code);
        exit(code);
    }
    if (ipos >= self->npositionals) {
        const int code = 26;
        fprintf(stderr, "ERROR %d: Requested positional argument does not exist, aborting.\n", code);
        exit(code);
    }
    int i = self->tokens.len - self->npositionals + ipos;
    return self->tokens.items[i].str;
}


const char * CLA_get_value_required (const struct cla * self, const char * name) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    assert_no_help_requested(self);
    int ikey = find_key_by_name(self, name);
    assert_key_exists(ikey, name);
    assert_key_is_of_type(self, ikey, name, KEY_TYPE_REQUIRED);
    assert_key_is_used(self, ikey, name);
    for (int itoken = 0; itoken < self->tokens.len; itoken++) {
        if (self->tokens.items[itoken].ikey == ikey) {
            return self->tokens.items[itoken + 1].str;
        }
    }
    const int code = 27;
    fprintf(stderr, "ERROR %d: Something went wrong retrieving the value of '%s', aborting.\n", code, name);
    exit(code);
}


bool CLA_has_flag (const struct cla * self, const char * name) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    assert_no_help_requested(self);
    int ikey = find_key_by_name(self, name);
    assert_key_exists(ikey, name);
    assert_key_is_of_type(self, ikey, name, KEY_TYPE_FLAG);
    return CLA_count_flag(self, name) > 0;
}


bool CLA_has_optional (const struct cla * self, const char * name) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    assert_no_help_requested(self);
    int ikey = find_key_by_name(self, name);
    assert_key_exists(ikey, name);
    assert_key_is_of_type(self, ikey, name, KEY_TYPE_OPTIONAL);
    return self->keys.items[ikey].noccurrences > 0;
}


bool CLA_help_requested (struct cla * self) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    return self->help_requested;
}


void CLA_parse (struct cla * self, int argc, const char * argv[]) {
    assert_self_is_not_nullptr(self);

    // allocate space for token array self->tokens
    errno = 0;
    self->tokens = (struct tokens) {
        .cap = argc,
        .items = calloc(argc, sizeof(struct token)),
        .len = argc,
    };
    if (self->tokens.items == nullptr) {
        const int code = 28;
        fprintf(stderr, "%s\nERROR %d: Could not allocate memory for token array, aborting.\n", strerror(errno), code);
        exit(code);
    }

    // copy contents of argv into self->tokens
    for (int i = 0; i < argc; i++) {
        int len = strlen(argv[i]) + 1;
        self->tokens.items[i].str = calloc(len, sizeof(char *));
        if (self->tokens.items[i].str == nullptr) {
            const int code = 29;
            fprintf(stderr, "%s\nERROR %d: allocating memory for token string, aborting.\n", strerror(errno), code);
            exit(code);
        }
        strcpy(self->tokens.items[i].str, argv[i]);
    }

    // the first token is always the executable name
    self->tokens.items[0].type = TOKEN_TYPE_EXENAME;
    self->tokens.items[0].ikey = -1;

    // forgo further parsing if the user merely asked for help
    if (argc == 2) {
        bool a = strcmp(argv[1], "-h") == 0;
        bool b = strcmp(argv[1], "--help") == 0;
        if (a || b) {
            self->help_requested = true;
            self->isfinal = true;
            return;
        }
    }

    // iterate over the non-positionals: flags, requireds, optionals
    int ipos0 = argc - self->npositionals;
    if (ipos0 < 1) {
        const int code = 30;
        fprintf(stderr,
                "ERROR %d: Expected at least %d positional argument%s, aborting.\n",
                code, self->npositionals, self->npositionals == 1 ? "" : "s");
        exit(code);
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

        // if there is a key whose name or alias matches
        // token's str, assign key->type to token->type
        int ikey = find_key_by_name(self, token->str);
        assert_key_exists(ikey, token->str);
        token->type = (enum token_type) self->keys.items[ikey].type;
        token->ikey = ikey;

        // keep track of how many times the key was used
        self->keys.items[ikey].noccurrences++;

        if (itoken == ipos0 - 1) {
            if (token->type == TOKEN_TYPE_REQUIRED || token->type == TOKEN_TYPE_OPTIONAL) {
                const int code = 31;
                fprintf(stderr,
                        "ERROR %d: '%s' requires a value but none given%s, aborting.\n",
                        code, token->str, self->npositionals > 0 ? " or not all positionals have been defined" : "");
                exit(code);
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


void CLA_parsed_as (const struct cla * self, FILE * stream) {
    assert_self_is_not_nullptr(self);
    assert_arguments_have_been_parsed(self);
    assert_no_help_requested(self);
    const char * typenames[] = {
        [TOKEN_TYPE_ERR] = "error",
        [TOKEN_TYPE_REQUIRED] = "required",
        [TOKEN_TYPE_OPTIONAL] = "optional",
        [TOKEN_TYPE_FLAG] = "flag",
        [TOKEN_TYPE_POSITIONAL] = "positional",
        [TOKEN_TYPE_EXENAME] = "exename",
        [TOKEN_TYPE_VALUE] = "value",
    };
    fprintf(stream, "User-provided arguments were parsed as follows:\n");
    for (int itoken = 0; itoken < self->tokens.len; itoken++) {
        struct token * token = &self->tokens.items[itoken];
        fprintf(stream, "%-10s %s\n", typenames[token->type], token->str);
    }
}


static int find_key_by_name (const struct cla * self, const char * name) {
    assert_self_is_not_nullptr(self);
    if (name == nullptr) return -1;
    for (int ikey = 0; ikey < self->keys.len; ikey++) {
        struct key * key = &self->keys.items[ikey];
        bool a = (key->alias != nullptr) && (strcmp(key->alias, name) == 0);
        bool b = (key->name != nullptr) && (strcmp(key->name, name) == 0);
        if (a || b) return ikey;
    }
    return -1;
}
