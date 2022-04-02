// BUILD: clang -fsanitize=address %s -o %t
// CHECK: AddressSanitizer:Stack-buffer-overflow

int main(int argc, char *argv[])
{
    int array[100];
    return array[100];
}