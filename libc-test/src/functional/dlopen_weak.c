#include "dlopen_weak_deps.h"

__attribute__((weak)) int TestFunction(int input)
{
	return input % 2;
}

int TestNumber(int input)
{
	return TestNumber2(input);
}
