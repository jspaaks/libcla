#ifndef CLA_CLA_H_INCLUDED
#define CLA_CLA_H_INCLUDED

// forward declaration of abstract data type `struct cla` holding
// information about the command line arguments
typedef struct cla Cla;
struct cla;

void CLA_add_flag (struct cla * self, const char * longname, const char * shortname);
void CLA_add_optional (struct cla * self, const char * longname, const char * shortname);
void CLA_add_positionals (struct cla * self, int npositionals);
void CLA_add_required (struct cla * self, const char * longname, const char * shortname);
int CLA_count_flag (const struct cla * self, const char * name);
struct cla * CLA_create (void);
void CLA_destroy (struct cla ** self);
const char * CLA_get_optional_value (const struct cla * self, const char * name);
const char * CLA_get_positional_value (const struct cla * self, int ipos);
const char * CLA_get_required_value (const struct cla * self, const char * name);
void CLA_parse (struct cla * self, int argc, const char * argv[]);
void CLA_print_parse_result (const struct cla * self);
bool CLA_has_flag (const struct cla * self, const char * name);

#endif
