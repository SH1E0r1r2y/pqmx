/*
 * Some external references to auto-generated assembly.
 */

#include <hal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define STATE_SIZE 200
#define REPEAT 100
#define REPEAT_MEDIAN 100
#define X25519_SIZE 8


/* Assembly Functions */
/* Reference */
extern void fe25519_add_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sub_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_mul_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sqr_wrap(uint32_t *out);
extern void curve25519_scalarmult(uint32_t *out, uint32_t *a, uint32_t *b);
/* Slothy */
extern void fe25519_add_opt_m7_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sub_opt_m7_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_mul_opt_m7_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sqr_opt_m7_wrap(uint32_t *out);

typedef struct {
    char name[100];
    uint64_t cycles;
} benchmark_result;

benchmark_result results[100];
int benchmark_cnt = 0;

static void add_benchmark_results(char *name, uint64_t cycles){
    if(benchmark_cnt == 100) return;

    results[benchmark_cnt].cycles = cycles;
    strncpy(results[benchmark_cnt].name, name, 100);
    benchmark_cnt++;
}

static int cmp_uint32_t(const void *p1, const void *p2)
{
    return (int)((*((const uint32_t *)p1)) - (*((const uint32_t *)p2)));
}

#define MAKE_BENCH_X25519(var, func)                          \
    int bench_x25519_##var()                                             \
    {                                                                    \
        debug_printf("Benchmarking function %s\n", #func);               \
        uint32_t t1, t2;                                                 \
        uint32_t cycles[REPEAT_MEDIAN];                                  \
        uint32_t out[X25519_SIZE] __attribute__((aligned(16))) = {0};    \
        uint32_t a[X25519_SIZE] __attribute__((aligned(16))) = {0};    \
        uint32_t b[X25519_SIZE] __attribute__((aligned(16))) = {0};    \
                                                                         \
        /* Warm up */                                                    \
        (func)(out, a, b);                                                 \
                                                                         \
        for (size_t cnt_median = 0; cnt_median < REPEAT_MEDIAN; cnt_median++) { \
            t1 = hal_get_time();                                         \
            for (size_t cnt = 0; cnt < REPEAT; cnt++) {                  \
                (func)(out, a, b);                                         \
            }                                                            \
            t2 = hal_get_time();                                         \
            cycles[cnt_median] = (t2 - t1) / REPEAT;                     \
                                                                         \
        }                                                                \
                                                                         \
        qsort(cycles, REPEAT_MEDIAN, sizeof(uint32_t), cmp_uint32_t);      \
        debug_printf("%s repeat %d, %d cycles\n", #func,                   \
                     REPEAT * REPEAT_MEDIAN,                               \
                     cycles[REPEAT_MEDIAN / 2]);                           \
        add_benchmark_results(#var, (cycles[REPEAT_MEDIAN >> 1]));         \
        return 0;                                                          \
    }
#define MAKE_BENCH_X25519_single(var, func)                          \
    int bench_x25519_##var()                                             \
    {                                                                    \
        debug_printf("Benchmarking function %s\n", #func);               \
        uint32_t t1, t2;                                                 \
        uint32_t cycles[REPEAT_MEDIAN];                                  \
        uint32_t out[X25519_SIZE] __attribute__((aligned(16))) = {0};    \
                                                                         \
        /* Warm up */                                                    \
        (func)(out);                                                     \
                                                                         \
        for (size_t cnt_median = 0; cnt_median < REPEAT_MEDIAN; cnt_median++) { \
            t1 = hal_get_time();                                         \
            for (size_t cnt = 0; cnt < REPEAT; cnt++) {                  \
                (func)(out);                                         \
            }                                                            \
            t2 = hal_get_time();                                         \
            cycles[cnt_median] = (t2 - t1) / REPEAT;                     \
        }                                                                \
                                                                         \
        qsort(cycles, REPEAT_MEDIAN, sizeof(uint32_t), cmp_uint32_t);      \
        debug_printf("%s repeat %d, %d cycles\n", #func,                   \
                     REPEAT * REPEAT_MEDIAN,                               \
                     cycles[REPEAT_MEDIAN / 2]);                           \
        add_benchmark_results(#var, (cycles[REPEAT_MEDIAN >> 1]));         \
        return 0;                                                          \
    }

MAKE_BENCH_X25519(fe25519_add, fe25519_add_wrap)
MAKE_BENCH_X25519(fe25519_sub, fe25519_sub_wrap)
MAKE_BENCH_X25519(fe25519_mul, fe25519_mul_wrap)
MAKE_BENCH_X25519_single(fe25519_sqr, fe25519_sqr_wrap)


MAKE_BENCH_X25519(fe25519_add_opt_m7,fe25519_add_opt_m7_wrap)
MAKE_BENCH_X25519(fe25519_sub_opt_m7,fe25519_sub_opt_m7_wrap)
MAKE_BENCH_X25519(fe25519_mul_opt_m7,fe25519_mul_opt_m7_wrap)
MAKE_BENCH_X25519_single(fe25519_sqr_opt_m7,fe25519_sqr_opt_m7_wrap)

#define MAKE_TEST_X25519(var, func, ref_func)                         \
    int test_x25519_##var()                                           \
    {                                                                 \
        debug_printf("25519 Correctness Test for " #func);            \
        uint32_t out_ref[X25519_SIZE] __attribute__((aligned(16))) = {0}; \
        uint32_t out_test[X25519_SIZE] __attribute__((aligned(16))) = {0}; \
        uint32_t a[X25519_SIZE] __attribute__((aligned(16))) = {0};   \
        uint32_t b[X25519_SIZE] __attribute__((aligned(16))) = {0};   \
                                                                      \
        /* Initialize inputs */                                       \
        for (size_t i = 0; i < X25519_SIZE; i++) {                    \
            a[i] = 1234;                                              \
            b[i] = 5678;                                              \
        }                                                             \
                                                                      \
        /* Reference function */                                      \
        ref_func(out_ref, a, b);                                      \
        /* Test function */                                           \
        func(out_test, a, b);                                         \
                                                                      \
        /* Compare outputs */                                         \
        int err = 0;                                                  \
        for (size_t i = 0; i < X25519_SIZE; i++) {                    \
            if (out_ref[i] != out_test[i]) {                          \
                debug_printf("Mismatch at index %zu: ref=%u, test=%u\n", \
                             i, out_ref[i], out_test[i]);             \
                err = 1;                                              \
            }                                                         \
        }                                                             \
                                                                      \
        if (err) {                                                    \
            debug_printf("Test FAILED for " #func "\n");              \
            return 1;                                                 \
        }                                                             \
        debug_printf("Test PASSED for " #func "\n");                  \
        return 0;                                                     \
    }
#define MAKE_TEST_X25519_single(var, func, ref_func)                  \
    int test_x25519_##var()                                           \
    {                                                                 \
        debug_printf("25519 Correctness Test for " #func);            \
        uint32_t out_ref[X25519_SIZE] __attribute__((aligned(16))) = {0}; \
        uint32_t out_test[X25519_SIZE] __attribute__((aligned(16))) = {0}; \
        for (size_t i = 0; i < X25519_SIZE; i++) {                    \
            out_ref[i] = rand();                                      \
        }                                                             \
        memcpy(out_test, out_ref, sizeof(out_ref));                   \
        ref_func(out_ref);                                            \
        func(out_test);                                               \
                                                                      \
        /* Compare outputs */                                         \
        int err = 0;                                                  \
        for (size_t i = 0; i < X25519_SIZE; i++) {                    \
            if (out_ref[i] != out_test[i]) {                          \
                debug_printf("Mismatch at index %zu: ref=%u, test=%u\n", \
                             i, out_ref[i], out_test[i]);             \
                err = 1;                                              \
            }                                                         \
        }                                                             \
                                                                      \
        if (err) {                                                    \
            debug_printf("Test FAILED for " #func "\n");              \
            return 1;                                                 \
        }                                                             \
        debug_printf("Test PASSED for " #func "\n");                  \
        return 0;                                                     \
    }

MAKE_TEST_X25519(fe25519_add, fe25519_add_wrap, fe25519_add_opt_m7_wrap)
// MAKE_TEST_X25519(fe25519_sub, fe25519_sub_wrap, fe25519_sub_opt_m7_wrap)
// MAKE_TEST_X25519(fe25519_mul, fe25519_mul_wrap, fe25519_mul_opt_m7_wrap)
// MAKE_TEST_X25519_single(fe25519_sqr, fe25519_sqr_wrap, fe25519_sqr_opt_m7_wrap)

int main(void)
{
    debug_test_start("====================== x25519 new");

    // /* Test correctness */
    if (test_x25519_fe25519_add() != 0) {
        debug_printf("add?");
        return 1;
     }
    // if (test_x25519_fe25519_sub() != 0) {
    //     debug_printf("sub?");
    //     return 1;
    // }
    // if (test_x25519_fe25519_mul() != 0) {
    //     debug_printf("mul?");
    //     return 1;
    // }
    // if (test_x25519_fe25519_sqr() != 0) {
    //     debug_printf("sqr?");
    //     return 1;
    // }


//     /* Bench */
    bench_x25519_fe25519_add();
    bench_x25519_fe25519_sub();
    bench_x25519_fe25519_mul();
    //bench_x25519_fe25519_sqr();

    
    bench_x25519_fe25519_add_opt_m7();
    bench_x25519_fe25519_sub_opt_m7();
    //bench_x25519_fe25519_mul_opt_m7();
    //bench_x25519_fe25519_sqr_opt_m7();

    debug_printf("======================\n");
    debug_printf("ALL GOOD!\n");
    return 0;
}
