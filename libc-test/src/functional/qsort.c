#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

static int scmp(const void *a, const void *b)
{
	return strcmp(*(char **)a, *(char **)b);
}

static int icmp(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

static int ccmp(const void *a, const void *b)
{
	return *(char*)a - *(char*)b;
}

static int cmp64(const void *a, const void *b)
{
	const uint64_t *ua = a, *ub = b;
	return *ua < *ub ? -1 : *ua != *ub;
}

/* 26 items -- even */
static const char *s[] = {
	"Bob", "Alice", "John", "Ceres",
	"Helga", "Drepper", "Emeralda", "Zoran",
	"Momo", "Frank", "Pema", "Xavier",
	"Yeva", "Gedun", "Irina", "Nono",
	"Wiener", "Vincent", "Tsering", "Karnica",
	"Lulu", "Quincy", "Osama", "Riley",
	"Ursula", "Sam"
};
static const char *s_sorted[] = {
	"Alice", "Bob", "Ceres", "Drepper",
	"Emeralda", "Frank", "Gedun", "Helga",
	"Irina", "John", "Karnica", "Lulu",
	"Momo", "Nono", "Osama", "Pema",
	"Quincy", "Riley", "Sam", "Tsering",
	"Ursula", "Vincent", "Wiener", "Xavier",
	"Yeva", "Zoran"
};

/* 23 items -- odd, prime */
static int n[] = {
	879045, 394, 99405644, 33434, 232323, 4334, 5454,
	343, 45545, 454, 324, 22, 34344, 233, 45345, 343,
	848405, 3434, 3434344, 3535, 93994, 2230404, 4334
};
static int n_sorted[] = {
	22, 233, 324, 343, 343, 394, 454, 3434,
	3535, 4334, 4334, 5454, 33434, 34344, 45345, 45545,
	93994, 232323, 848405, 879045, 2230404, 3434344, 99405644
};

static void str_test(const char **a, const char **a_sorted, int len)
{
	int i;
	qsort(a, len, sizeof *a, scmp);
	for (i=0; i<len; i++) {
		if (strcmp(a[i], a_sorted[i]) != 0) {
			t_error("string sort failed at index %d\n", i);
			t_printf("\ti\tgot\twant\n");
			for (i=0; i<len; i++)
				t_printf("\t%d\t%s\t%s\n", i, a[i], a_sorted[i]);
			break;
		}
	}
}

static void int_test(int *a, int *a_sorted, int len)
{
	int i;
	qsort(a, len, sizeof *a, icmp);
	for (i=0; i<len; i++) {
		if (a[i] != a_sorted[i]) {
			t_error("integer sort failed at index %d\n", i);
			t_printf("\ti\tgot\twant\n");
			for (i=0; i<len; i++)
				t_printf("\t%d\t%d\t%d\n", i, a[i], a_sorted[i]);
			break;
		}
	}
}

static void uint64_gen(uint64_t *p, uint64_t *p_sorted, int n)
{
	int i;
	uint64_t r = 0;
	t_randseed(n);
	for (i = 0; i < n; i++) {
		r += t_randn(20);
		p[i] = r;
	}
	memcpy(p_sorted, p, n * sizeof *p);
	t_shuffle(p, n);
}

static void uint64_test(uint64_t *a, uint64_t *a_sorted, int len)
{
	int i;
	qsort(a, len, sizeof *a, cmp64);
	for (i=0; i<len; i++) {
		if (a[i] != a_sorted[i]) {
			t_error("uint64 sort failed at index %d\n", i);
			t_printf("\ti\tgot\twant\n");
			for (i=0; i<len; i++)
				t_printf("\t%d\t%" PRIu64 "\t%" PRIu64 "\n", i, a[i], a_sorted[i]);
			break;
		}
	}
}

#define T(a, a_sorted) do { \
	char p[] = a; \
	qsort(p, sizeof p - 1, 1, ccmp); \
	if (memcmp(p, a_sorted, sizeof p) != 0) { \
		t_error("character sort failed\n"); \
		t_printf("\tgot:  \"%s\"\n", p); \
		t_printf("\twant: \"%s\"\n", a_sorted); \
	} \
} while(0)

static void char_test(void)
{
	T("", "");
	T("1", "1");
	T("11", "11");
	T("12", "12");
	T("21", "12");
	T("111", "111");
	T("211", "112");
	T("121", "112");
	T("112", "112");
	T("221", "122");
	T("212", "122");
	T("122", "122");
	T("123", "123");
	T("132", "123");
	T("213", "123");
	T("231", "123");
	T("321", "123");
	T("312", "123");
	T("1423", "1234");
	T("51342", "12345");
	T("261435", "123456");
	T("4517263", "1234567");
	T("37245618", "12345678");
	T("812436597", "123456789");
	T("987654321", "123456789");
	T("321321321", "111222333");
	T("49735862185236174", "11223344556677889");
}

/* CVE-2026-40200 regression tests
 * https://www.openwall.com/lists/oss-security/2026/04/10/13/1
 *
 * Bug: pntz function in qsort.c could return 0 instead of 8*sizeof(size_t)
 * when p[0]==1 and p[1] has its low bit set, causing stack buffer overflow
 * in trinkle function's loop.
 *
 * Trigger condition: array size exceeding ~7 million elements (32nd Leonardo
 * number on 32-bit platforms). Since practical tests cannot allocate such
 * large arrays, we test key Leonardo numbers and boundary conditions that
 * stress the smoothsort algorithm's internal state management.
 */

/* Leonardo numbers used in smoothsort heap structure
 * L(1)=1, L(2)=1, L(3)=3, L(4)=5, L(5)=9, L(6)=15, L(7)=25, L(8)=41...
 * Recurrence: L(n) = L(n-1) + L(n-2) + 1
 */
static void test_leonardo_sizes(void)
{
    int i, j, len;
    int *arr;
    int leo_sizes[] = {1, 1, 3, 5, 9, 15, 25, 41, 67, 109, 177, 287, 465, 753,
                       1219, 1973, 3193, 5167, 8361, 13529, 21891, 35421};
    int num_sizes = sizeof(leo_sizes) / sizeof(leo_sizes[0]);

    for (j = 0; j < num_sizes; j++) {
        len = leo_sizes[j];
        arr = malloc(len * sizeof(int));
        if (!arr) {
            t_error("malloc failed for Leonardo size %d\n", len);
            continue;
        }

        /* Fill with reverse order to stress the algorithm */
        for (i = 0; i < len; i++) {
            arr[i] = len - i;
        }

        qsort(arr, len, sizeof(int), icmp);

        /* Verify sorted order */
        for (i = 0; i < len; i++) {
            if (arr[i] != i + 1) {
                t_error("Leonardo size %d sort failed at index %d: got %d, want %d\n",
                    len, i, arr[i], i + 1);
                break;
            }
        }

        free(arr);
    }
}

/* Powers of two minus one - these sizes stress boundary conditions
 * and may trigger specific internal states in the bit vector p[2]
 */
static void test_pow2_minus_one(void)
{
    int i, j, len;
    int *arr;
    int sizes[] = {3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (j = 0; j < num_sizes; j++) {
        len = sizes[j];
        arr = malloc(len * sizeof(int));
        if (!arr) {
            t_error("malloc failed for size %d\n", len);
            continue;
        }

        /* Reverse order input */
        for (i = 0; i < len; i++) {
            arr[i] = len - i;
        }

        qsort(arr, len, sizeof(int), icmp);

        /* Check sorted */
        for (i = 0; i < len; i++) {
            if (arr[i] != i + 1) {
                t_error("pow2-1 size %d sort failed at index %d\n", len, i);
                break;
            }
        }

        free(arr);
    }
}

/* Test partially sorted arrays - smoothsort is adaptive
 * and should handle nearly-sorted input efficiently
 */
static void test_partial_sorted(void)
{
    int i, j, len, k;
    int *arr;
    int sizes[] = {15, 31, 63, 127, 255, 511, 1023};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (j = 0; j < num_sizes; j++) {
        len = sizes[j];
        arr = malloc(len * sizeof(int));
        if (!arr) {
            t_error("malloc failed for size %d\n", len);
            continue;
        }

        /* Start with sorted array */
        for (i = 0; i < len; i++) {
            arr[i] = i;
        }

        /* Make a few random swaps */
        for (k = 0; k < 5; k++) {
            int idx1 = (k * 7) % len;
            int idx2 = (k * 13 + 5) % len;
            int tmp = arr[idx1];
            arr[idx1] = arr[idx2];
            arr[idx2] = tmp;
        }

        qsort(arr, len, sizeof(int), icmp);

        /* Verify */
        for (i = 0; i < len; i++) {
            if (arr[i] != i) {
                t_error("partial sorted test size %d failed at index %d\n", len, i);
                break;
            }
        }

        free(arr);
    }
}

/* Test with duplicate elements to stress comparison stability */
static void test_duplicates(void)
{
    int arr1[] = {5, 2, 2, 5, 5, 2, 5, 2, 5, 2};
    int expected1[] = {2, 2, 2, 2, 2, 5, 5, 5, 5, 5};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int i;

    qsort(arr1, n1, sizeof(int), icmp);
    for (i = 0; i < n1; i++) {
        if (arr1[i] != expected1[i]) {
            t_error("duplicate test 1 failed at index %d: got %d, want %d\n",
                i, arr1[i], expected1[i]);
            break;
        }
    }

    /* Larger duplicate test */
    int arr2[] = {1, 3, 3, 3, 1, 1, 3, 1, 3, 3, 1, 3, 1, 1, 3};
    int expected2[] = {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    qsort(arr2, n2, sizeof(int), icmp);
    for (i = 0; i < n2; i++) {
        if (arr2[i] != expected2[i]) {
            t_error("duplicate test 2 failed at index %d: got %d, want %d\n",
                i, arr2[i], expected2[i]);
            break;
        }
    }
}

/* Test boundary sizes: 1, 2 elements */
static void test_small_sizes(void)
{
    int arr1[] = {42};
    int arr2[] = {2, 1};
    int arr3[] = {1, 2};
    int arr4[] = {1, 1};

    qsort(arr1, 1, sizeof(int), icmp);
    if (arr1[0] != 42) {
        t_error("single element test failed: got %d, want 42\n", arr1[0]);
    }

    qsort(arr2, 2, sizeof(int), icmp);
    if (arr2[0] != 1 || arr2[1] != 2) {
        t_error("two elements reverse test failed\n");
    }

    qsort(arr3, 2, sizeof(int), icmp);
    if (arr3[0] != 1 || arr3[1] != 2) {
        t_error("two elements sorted test failed\n");
    }

    qsort(arr4, 2, sizeof(int), icmp);
    if (arr4[0] != 1 || arr4[1] != 1) {
        t_error("two equal elements test failed\n");
    }
}

/* Test sizes around Leonardo boundaries to stress internal state transitions
 * These sizes may trigger specific bit patterns in the p[2] vector
 */
static void test_leonardo_boundaries(void)
{
    int i, len;
    int *arr;
    /* Sizes just above and below key Leonardo numbers */
    int boundary_sizes[] = {
        /* Around L(5)=9 */
        7, 8, 9, 10, 11,
        /* Around L(6)=15 */
        13, 14, 15, 16, 17,
        /* Around L(7)=25 */
        23, 24, 25, 26, 27,
        /* Around L(8)=41 */
        39, 40, 41, 42, 43,
        /* Around L(9)=67 */
        65, 66, 67, 68, 69,
        /* Around L(10)=109 */
        107, 108, 109, 110, 111
    };
    int num_sizes = sizeof(boundary_sizes) / sizeof(boundary_sizes[0]);
    int j;

    for (j = 0; j < num_sizes; j++) {
        len = boundary_sizes[j];
        arr = malloc(len * sizeof(int));
        if (!arr) {
            t_error("malloc failed for boundary size %d\n", len);
            continue;
        }

        for (i = 0; i < len; i++) {
            arr[i] = len - i;
        }

        qsort(arr, len, sizeof(int), icmp);

        for (i = 0; i < len; i++) {
            if (arr[i] != i + 1) {
                t_error("boundary size %d sort failed at index %d\n", len, i);
                break;
            }
        }

        free(arr);
    }
}

int main(void)
{
	int i;

	str_test(s, s_sorted, sizeof s/sizeof*s);
	int_test(n, n_sorted, sizeof n/sizeof*n);
	char_test();
	for (i = 1023; i<=1026; i++) {
		uint64_t p[1026], p_sorted[1026];
		uint64_gen(p, p_sorted, i);
		uint64_test(p, p_sorted, i);
	}

    test_small_sizes();
    test_duplicates();
    test_leonardo_sizes();
    test_pow2_minus_one();
    test_partial_sorted();
    test_leonardo_boundaries();

	return t_status;
}
