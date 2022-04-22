//CHECK: AddressSanitizer:Global-buffer-overflow

int array[100];

int main(int argc, char *argv[]) {
    return array[100];
}