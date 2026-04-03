#include "cla/cla.h"
#include <criterion/criterion.h>


Test(CLA_add_flag, __LINE__, .description="CLA_add_flag with a certain situation") {
    bool b = true;
    cr_assert(b);
}

Test(CLA_add_flag, __LINE__, .description="CLA_add_flag with another situation") {
    bool b = true;
    cr_assert(b);
}
