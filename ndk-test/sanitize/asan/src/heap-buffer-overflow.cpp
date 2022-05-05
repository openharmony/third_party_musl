// BUILD: clang -fsanitize=address %s -o %t
// CHECK: AddressSanitizer:Heap-buffer-overflow

int main(int argc, char *argv[])
{
    int *array = new int[100];
    int res = array[100];
    delete[] array;
    return res;
}