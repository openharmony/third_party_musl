#include "functionalext.h"

extern int entry_for_ldso_relocation();

int main() {
    EXPECT_EQ("entry_for_ldso_relocation", 2, entry_for_ldso_relocation());
    return t_status;
}