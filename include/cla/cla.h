#ifndef CLA_CLA_H_INCLUDED
#define CLA_CLA_H_INCLUDED

#include <stdio.h>

// forward declaration of abstract data type `struct cla` holding
// information about the command line arguments
typedef struct cla Cla;
struct cla;

void CLA_add_flag (struct cla * self, const char * name, const char * alias);
void CLA_add_optional (struct cla * self, const char * name, const char * alias);
void CLA_add_positionals (struct cla * self, int npositionals);
void CLA_add_required (struct cla * self, const char * name, const char * alias);
int CLA_count_flag (const struct cla * self, const char * name);
struct cla * CLA_create (void);
void CLA_destroy (struct cla ** self);
const char * CLA_get_value_optional (const struct cla * self, const char * name);
const char * CLA_get_value_positional (const struct cla * self, int ipos);
const char * CLA_get_value_required (const struct cla * self, const char * name);
bool CLA_has_flag (const struct cla * self, const char * name);
bool CLA_has_optional (const struct cla * self, const char * name);
void CLA_parse (struct cla * self, int argc, const char * argv[]);
void CLA_parsed_as (const struct cla * self, FILE * stream);

#endif
