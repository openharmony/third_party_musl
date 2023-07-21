#include <stdlib.h>
#include "test.h"

__attribute__((constructor)) 
void B_ctor() {
	exit(t_status); 
}

int main() {
	return t_status;  
}