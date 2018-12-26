#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

const int n_trials = 2000000; // Enough to keep cores busy for a while and observe a steady state
const int flops_per_calc = 2; // Multiply + add = 2 instructions
const int n_chained_fmas = 20; // Must be tuned for architectures here and in blocks (R) and in (E)
const int VECTOR_WIDTH = 8;
double time_difference=0.0;
struct timespec start, end;	//C11 only, new method to measure time.
const int align_length = VECTOR_WIDTH*sizeof(double);
//const int align_length = sizeof(double);


int main() {

    #pragma omp parallel
    {
        register double *fa = aligned_alloc(align_length, n_trials*VECTOR_WIDTH*n_chained_fmas*sizeof(double));
        register double *fb = aligned_alloc(align_length, VECTOR_WIDTH*sizeof(double));
        register double *fc = aligned_alloc(align_length, VECTOR_WIDTH*sizeof(double));

        #pragma omp simd simdlen(VECTOR_WIDTH) aligned(fa:align_length)
        for(int i = 0; i < n_trials*VECTOR_WIDTH*n_chained_fmas; i++){
            fa[i] = 0.0;
        }
        #pragma omp simd simdlen(VECTOR_WIDTH) aligned(fb, fc:align_length)
        for(int i = 0; i < VECTOR_WIDTH; i++){
            fb[i] = 0.5;
            fc[i] = 1.0;
        }

        #pragma omp master
        {
            timespec_get(&start, 1/*TIME_UTC*/);
        }

        #pragma unroll(10)
        for(int l = 0; l < 10; l++){
        #pragma nounroll // Prevents automatic unrolling by compiler to avoid skewed benchmarks
        for(int i = 0; i < n_trials; i++){
            #pragma omp simd simdlen(VECTOR_WIDTH) aligned(fa, fb, fc:align_length)
            for(int j = 0; j < VECTOR_WIDTH; j++){
                #pragma unroll(n_chained_fmas)
                for(int k = 0; k < VECTOR_WIDTH*n_chained_fmas; k+=VECTOR_WIDTH){
                    fa[i*VECTOR_WIDTH*n_chained_fmas + k+j] = fa[i*VECTOR_WIDTH*n_chained_fmas + k+j]*fb[j] + fc[j];
                    //fa[k+j] = fa[k+j]*fb[j] + fc[j];
                }
            }
        }}

        #pragma omp master
        {
            timespec_get(&end, 1/*TIME_UTC*/);
            time_difference = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1.0e9;
            //printf("The time difference is %.9f\n", time_difference);
            printf("The number of threads is %i\n", omp_get_max_threads());
        }

    }

    const double gflops = 1.0e-9*(double)VECTOR_WIDTH*(double)n_trials*(double)flops_per_calc*(double)omp_get_max_threads()*(double)n_chained_fmas*10.0;
    printf("Chained FMAs=%d, vector width=%d, GFLOPs=%.1f, time=%.6f s, performance=%.1f GFLOP/s\n", n_chained_fmas, VECTOR_WIDTH, gflops, time_difference, gflops/time_difference);


return 0;
}