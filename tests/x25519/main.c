/*
 * Some external references to auto-generated assembly.
 */

#include <hal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define STATE_SIZE 2
#define REPEAT 1
#define REPEAT_MEDIAN 1


/* Assembly Functions */
/* Reference, bit interlaved */
extern void fe25519_add_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sub_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_mul_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sqr_wrap(uint32_t *out, uint32_t *a, uint32_t *b);
/* To be tested, bit interlaved */
// extern void KeccakF1600_StatePermute_old(void*);
// /* M7 optimized code by Alexandre Adomnicai */
// extern void KeccakP1600_Permute_24rounds(void*);

/* Slothy */
extern void fe25519_add_opt_m7(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sub_opt_m7(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_mul_opt_m7(uint32_t *out, uint32_t *a, uint32_t *b);
extern void fe25519_sqr_opt_m7(uint32_t *out, uint32_t *a, uint32_t *b);

typedef struct {
    char name[100];
    uint64_t cycles;
} benchmark_result;

benchmark_result results[100];
int benchmark_cnt = 0;
uint32_t out[8] = {0};
uint32_t a[8] = {1, 2, 3, 4, 5, 6, 7, 8}; 
uint32_t b[8] = {8, 7, 6, 5, 4, 3, 2, 1}; 

// static void add_benchmark_results(char *name, uint64_t cycles){
//     if(benchmark_cnt == 100) return;

//     results[benchmark_cnt].cycles = cycles;
//     strncpy(results[benchmark_cnt].name, name, 100);
//     benchmark_cnt++;
// }

// static void dump_benchmarks_tex(void){
//     for(int i=0;i<benchmark_cnt;i++){

//         debug_printf("\\DefineVar{%s_%s}{%llu}", DEVICE, results[i].name, results[i].cycles);
//     }
// }

static int cmp_uint64_t(const void *a, const void *b)
{
    return (int)((*((const uint64_t *)a)) - (*((const uint64_t *)b)));
}

#define MAKE_BENCH_X25519(var, func,out,a,b)                                         \
    int bench_x25519_##var()                                                 \
    {                                                                         \
        debug_printf("X25519 Benchmark for " #func);                         \
        uint64_t t2, t1;                                                      \
        uint64_t cycles[REPEAT_MEDIAN];                                       \
        uint8_t state[STATE_SIZE] __attribute__((aligned(16)));               \
        for (uint32_t i = 0; i < STATE_SIZE; i++)                             \
        {                                                                     \
            state[i] = rand();                                                \
        }                                                                     \
                                                                              \
        debug_printf("X25519 Benchmark for " #func);                         \
        /* Warm up */                                                         \
        (func)(out,a,b);                                                        \
        debug_printf("X25519 Benchmark for " #func);                         \
        for (size_t cnt_median = 0; cnt_median < REPEAT_MEDIAN; cnt_median++) \
        {                                                                     \
            t1 = hal_get_time();                                              \
            for (size_t cnt = 0; cnt < REPEAT; cnt++)                         \
                (func)(out,a,b);                                                \
            t2 = hal_get_time();                                              \
            cycles[cnt_median] = (t2 - t1) / REPEAT;                          \
        }                                                                     \
        qsort(cycles, REPEAT_MEDIAN, sizeof(uint64_t), cmp_uint64_t);         \
        debug_printf(#func " repeat %d, %d",                                  \
                     REPEAT *REPEAT_MEDIAN, (cycles[REPEAT_MEDIAN >> 1]));    \
                                                                              \
        return (0);                                                           \
    }

MAKE_BENCH_X25519(fe25519_add, fe25519_add_wrap,out,a,b)
MAKE_BENCH_X25519(fe25519_sub, fe25519_sub_wrap,out,a,b)
MAKE_BENCH_X25519(fe25519_mul, fe25519_mul_wrap,out,a,b)
MAKE_BENCH_X25519(fe25519_sqr, fe25519_sqr_wrap,out,a,b)


MAKE_BENCH_X25519(fe25519_add_opt_m7,fe25519_add_opt_m7,out,a,b)
MAKE_BENCH_X25519(fe25519_sub_opt_m7,fe25519_sub_opt_m7,out,a,b)
MAKE_BENCH_X25519(fe25519_mul_opt_m7,fe25519_mul_opt_m7,out,a,b)
MAKE_BENCH_X25519(fe25519_sqr_opt_m7,fe25519_sqr_opt_m7,out,a,b)


// MAKE_TEST_KECCAKF(pqm4, KeccakF1600_StatePermute_old, KeccakF1600_StatePermute_pqm4)
// MAKE_TEST_KECCAKF(m7, KeccakP1600_Permute_24rounds, KeccakF1600_StatePermute_pqm4)

// MAKE_TEST_KECCAKF(old_opt_m7, KeccakF1600_StatePermute_old_opt_m7, KeccakF1600_StatePermute_pqm4)

int main(void)
{
    debug_test_start( "x25519 new" );

    /* Test correctness */
    // if(test_keccakf_pqm4() != 0){
    //     return 1;
    // }

    // if(test_keccakf_m7()) {
    //     return 1;
    // }

    // if(test_keccakf_old_opt_m7()) {
    //     return 1;
    // }

    /* Bench */
    bench_x25519_fe25519_add();
    bench_x25519_fe25519_sub();
    bench_x25519_fe25519_mul();
    bench_x25519_fe25519_sqr();

    
    bench_x25519_fe25519_add_opt_m7();
    bench_x25519_fe25519_sub_opt_m7();
    bench_x25519_fe25519_mul_opt_m7();
    bench_x25519_fe25519_sqr_opt_m7();

    debug_printf("======================" );
    //dump_benchmarks_tex();
    debug_printf("======================\n" );

    //debug_test_ok();
    debug_printf( "ALL GOOD!\n" );
    return 0;
}
