CC = icc

##dynamic, maximum optimization, for intel scalable cpu only, better precision and reproducibility
CFLAGS_avx512 = -std=c11 -Wall -xCore-AVX512 -qopt-zmm-usage=high -O3 -ip -ipo -finline-functions -unroll-aggressive -fp-speculation=safe -mp1 -fp-model precise -fimf-precision=high -qno-opt-dynamic-align -fimf-use-svml=true -fma -qopenmp -qopenmp-simd -vec -simd -qopt-report=5 -qopt-report-phase=all #-qopt-report-file=stdout

CFLAGS_xhost = -std=c11 -Wall -xHost -O3 -ip -ipo -finline-functions -unroll-aggressive -fp-speculation=safe -mp1 -fp-model precise -fimf-precision=high -qno-opt-dynamic-align -fimf-use-svml=true -fma -qopenmp -qopenmp-simd -vec -simd -qopt-report=5 -qopt-report-phase=all #-qopt-report-file=stdout


avx512:
		$(CC) $(CFLAGS_avx512) Test_C_CPU_Performance.c -o Test_C_CPU_Performance
		$(CC) $(CFLAGS_avx512) Test_C_CPU-RAM_Performance.c -o Test_C_CPU-RAM_Performance
		$(CC) $(CFLAGS_avx512) Test_C_RAM_Performance.c -o Test_C_RAM_Performance

xhost:
		$(CC) $(CFLAGS_xhost) Test_C_CPU_Performance.c -o Test_C_CPU_Performance
		$(CC) $(CFLAGS_xhost) Test_C_CPU-RAM_Performance.c -o Test_C_CPU-RAM_Performance
		$(CC) $(CFLAGS_xhost) Test_C_RAM_Performance.c -o Test_C_RAM_Performance

clean:
		rm -f ./*.optrpt
		rm -f ./Test_C_CPU_Performance
		rm -f ./Test_C_CPU-RAM_Performance
		rm -f ./Test_C_RAM_Performance

debug:		
		$(CC) $(CFLAGS_xhost) -E Test_C_CPU_Performance.c -o Test_C_CPU_Performance


## "-E" = Causes the preprocessor to send output to stdout.
##Description
##This option causes the preprocessor to send output to stdout . Compilation stops when the files have beenpreprocessed.
##When you specify this option, the compiler's preprocessor expands your source module and writes the result
##to stdout . The preprocessed source contains #line directives, which the compiler uses to determine thesource file and line number.
