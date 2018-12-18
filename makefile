CC = icc

##dynamic, maximum optimization, for intel scalable cpu only, better precision and reproducibility
CFLAGS_avx512 = -std=c11 -Wall -xHost -xCore-AVX512 -qopt-zmm-usage=high -O3 -ip -ipo -finline-functions -unroll-aggressive -fp-speculation=safe -mp1 -fp-model precise -fimf-precision=high -qno-opt-dynamic-align -fimf-use-svml=true -fma -qopenmp -qopenmp-simd -vec -simd -qopt-report=5 -qopt-report-phase=all #-qopt-report-file=stdout

CFLAGS_xhost = -std=c11 -Wall -xHost -O3 -ip -ipo -finline-functions -unroll-aggressive -fp-speculation=safe -mp1 -fp-model precise -fimf-precision=high -qno-opt-dynamic-align -fimf-use-svml=true -fma -qopenmp -qopenmp-simd -vec -simd -qopt-report=5 -qopt-report-phase=all #-qopt-report-file=stdout


avx512:
		$(CC) $(CFLAGS_avx512) Test_C_CPU_Performance.c -o Test_C_CPU_Performance
		$(CC) $(CFLAGS_avx512) Test_C_CPU-RAM_Performance.c -o Test_C_CPU-RAM_Performance

xhost:
		$(CC) $(CFLAGS_xhost) Test_C_CPU_Performance.c -o Test_C_CPU_Performance
		$(CC) $(CFLAGS_xhost) Test_C_CPU-RAM_Performance.c -o Test_C_CPU-RAM_Performance

clean:
		rm -f ./*.optrpt
		rm -f ./Test_C_CPU_Performance
		rm -f ./Test_C_CPU-RAM_Performance