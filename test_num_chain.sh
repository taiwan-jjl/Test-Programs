#!/bin/bash
#
##########

rm -f ./Test_C_CPU_Performance.out
for((i=1;i<40;i++))
do
    echo "# of chained FMA is: ${i}" >> ./Test_C_CPU_Performance.out
    sed -i "8c const int n_chained_fmas = ${i}; // Must be tuned for architectures here and in blocks (R) and in (E)" ./Test_C_CPU_Performance.c
    make clean xhost
    ./Test_C_CPU_Performance >> ./Test_C_CPU_Performance.out
done

rm -f ./Test_C_CPU-RAM_Performance.out
for((i=1;i<40;i++))
do
    echo "# of chained FMA is: ${i}" >> ./Test_C_CPU-RAM_Performance.out
    sed -i "8c const int n_chained_fmas = ${i}; // Must be tuned for architectures here and in blocks (R) and in (E)" ./Test_C_CPU-RAM_Performance.c
    make clean xhost
    ./Test_C_CPU-RAM_Performance >> ./Test_C_CPU-RAM_Performance.out
done