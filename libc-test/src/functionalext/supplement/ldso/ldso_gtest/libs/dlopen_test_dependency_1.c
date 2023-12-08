__attribute__((weak)) int DlopenTestDependencyImpl(void)
{
    return 0;
}

int DlopenTestDependency(void)
{
    return DlopenTestDependencyImpl();
}