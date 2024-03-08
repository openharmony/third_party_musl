__attribute__((weak)) int TestFunction(int input)
{
	return input % 5;
}

int TestNumber2(int input)
{
	return TestFunction(input) == 2;
}
