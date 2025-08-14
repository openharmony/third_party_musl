# Extended backtrace test for stripped adlt image

The test checks correctness of 'lookup symbol' and 'backtrace address'
procedures for symbols and addresses of adlt image, which was stripped.
There are 3 libraries in the adlt image: backtrace-lib_a.so,
backtrace-lib_b.so, backtrace-lib_test.so.

## 'Ver' test:

1. Search for clibGetVerA and clibGetVerB symbols corresponding to
functions without parameters is performed by means of public interface function
dlsym(). Functions are called and the correct result is expected.
2. Public interface function dladdr() is applied to the addresses of clibGetVerA
and clibGetVerB symbols. The correct pathname of the original library is
expected. 

## 'MaxMin' test:

1. Search for clibMaxA and clibMinB symbols corresponding to functions with 2
parameters is performed by means of public interface function dlsym(). Functions
are called and the correct result is expected.
2. Public interface function dladdr() is applied to the addresses of symbols
clibMaxA and clibMinB. The correct pathname of the original library is expected.

