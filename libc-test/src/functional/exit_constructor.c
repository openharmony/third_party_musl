#include <stdlib.h>
#include "test.h"

__attribute__((constructor))
void Ctor()
{
	exit(t_status);
}

int main()
{
	return t_status;
}