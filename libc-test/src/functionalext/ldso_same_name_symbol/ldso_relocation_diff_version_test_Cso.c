extern int test_reloc_a_so();
extern int ldso_reality_return_value();

int entry_for_ldso_relocation()
{
    return ldso_reality_return_value();
}

int just_test_for_a_so()
{
    return test_reloc_a_so();
}