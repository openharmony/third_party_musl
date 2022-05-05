#include <arm_neon.h>

uint32x2_t test_vadd_u32(uint32x2_t a, uint32x2_t b) {
    return vadd_u32(a,b);
}

float16x8_t test_vld1q_f16(float16_t const *a) {
    return vld1q_f16(a);
}

int main(){}