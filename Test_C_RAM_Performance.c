//unfinished, 20190109
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <omp.h>

const size_t n_trials = 10000000; // Enough to keep cores busy for a while and observe a steady state
const size_t n_chained_fmas = 10; // Must be tuned for architectures here and in blocks (R) and in (E)
const size_t VECTOR_WIDTH = 8;
double time_difference=0.0;
struct timespec start, end;	//C11 only, new method to measure time.
const size_t align_length = VECTOR_WIDTH*sizeof(double);
//const int align_length = sizeof(double);


int main() {

    double *fa = aligned_alloc(align_length, n_trials*VECTOR_WIDTH*n_chained_fmas*sizeof(double));
    srand(time(NULL));

    timespec_get(&start, 1/*TIME_UTC*/);

    //#pragma omp parallel for
    for(size_t i = 0; i < n_trials; i++){
        //#pragma omp simd simdlen(VECTOR_WIDTH) aligned(fa:align_length)
        for(size_t j = 0; j < VECTOR_WIDTH; j++){
            #pragma unroll(n_chained_fmas)
            for(size_t k = 0; k < VECTOR_WIDTH*n_chained_fmas; k+=VECTOR_WIDTH){
                fa[i*VECTOR_WIDTH*n_chained_fmas + k+j] = (double)rand();
            }
        }
    }

    timespec_get(&end, 1/*TIME_UTC*/);
    time_difference = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1.0e9;
    printf("initialization time = %.6f s\n", time_difference);

    register double temp=0.0;
    timespec_get(&start, 1/*TIME_UTC*/);

    #pragma unroll(10)
    for(int l = 0; l < 10; l++){
    #pragma omp parallel for reduction(+:temp)
    #pragma nounroll // Prevents automatic unrolling by compiler to avoid skewed benchmarks
    for(size_t i = 0; i < n_trials; i++){
        #pragma omp simd simdlen(VECTOR_WIDTH) aligned(fa:align_length) reduction(+:temp)
        for(size_t j = 0; j < VECTOR_WIDTH; j++){
            #pragma unroll(n_chained_fmas)
            for(size_t k = 0; k < VECTOR_WIDTH*n_chained_fmas; k+=VECTOR_WIDTH){
                temp = temp + fa[i*VECTOR_WIDTH*n_chained_fmas + k+j];
            }
        }
    }}

    timespec_get(&end, 1/*TIME_UTC*/);
    time_difference = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1.0e9;
    printf("\n%f\n", temp);

    double gflops = 1.0e-9*(double)VECTOR_WIDTH*(double)n_trials*(double)n_chained_fmas*10.0;
    printf("Chained FMAs=%lu, vector width=%lu, GFLOPs=%.1f, time=%.6f s, performance=%.1f GFLOP/s\n", n_chained_fmas, VECTOR_WIDTH, gflops, time_difference, gflops/time_difference);


return 0;
}