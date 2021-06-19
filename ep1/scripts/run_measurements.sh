#! /bin/bash

set -o xtrace

MEASUREMENTS=10
ITERATIONS=10
INITIAL_SIZE=16
THREADS=(1 2 4 6 8 16 32)

SIZE=$INITIAL_SIZE

NAMES=('mandelbrot_seq' 'mandelbrot_pth' 'mandelbrot_omp')

make
mkdir results

NAME='mandelbrot_seq'
mkdir results/$NAME

for ((i=1; i<=$ITERATIONS; i++)); do
        perf stat -r $MEASUREMENTS ./$NAME -2.5 1.5 -2.0 2.0 $SIZE >> full.log 2>&1
        perf stat -r $MEASUREMENTS ./$NAME -0.8 -0.7 0.05 0.15 $SIZE >> seahorse.log 2>&1
        perf stat -r $MEASUREMENTS ./$NAME 0.175 0.375 -0.1 0.1 $SIZE >> elephant.log 2>&1
        perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012 0.554 0.754 $SIZE >> triple_spiral.log 2>&1
        SIZE=$(($SIZE * 2))
done

SIZE=$INITIAL_SIZE

mv *.log results/$NAME
rm output.ppm

for THREAD in ${THREADS[@]}; do
    NAME='mandelbrot_pth'
    mkdir results/$NAME
    mkdir results/$NAME/$THREAD
    for ((i=1; i<=$ITERATIONS; i++)); do
            perf stat -r $MEASUREMENTS ./$NAME -2.5 1.5 -2.0 2.0 $SIZE $THREAD >> full.log 2>&1
            perf stat -r $MEASUREMENTS ./$NAME -0.8 -0.7 0.05 0.15 $SIZE $THREAD >> seahorse.log 2>&1
            perf stat -r $MEASUREMENTS ./$NAME 0.175 0.375 -0.1 0.1 $SIZE $THREAD >> elephant.log 2>&1
            perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012 0.554 0.754 $SIZE $THREAD >> triple_spiral.log 2>&1
            SIZE=$(($SIZE * 2))
    done

    SIZE=$INITIAL_SIZE

    mv *.log results/$NAME/$THREAD
    rm output.ppm
done

SIZE=$INITIAL_SIZE


for THREAD in ${THREADS[@]}; do
    NAME='mandelbrot_omp'
    mkdir results/$NAME
    mkdir results/$NAME/$THREAD
    export OMP_NUM_THREADS=$THREAD
    for ((i=1; i<=$ITERATIONS; i++)); do
            perf stat -r $MEASUREMENTS ./$NAME -2.5 1.5 -2.0 2.0 $SIZE >> full.log 2>&1
            perf stat -r $MEASUREMENTS ./$NAME -0.8 -0.7 0.05 0.15 $SIZE >> seahorse.log 2>&1
            perf stat -r $MEASUREMENTS ./$NAME 0.175 0.375 -0.1 0.1 $SIZE >> elephant.log 2>&1
            perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012 0.554 0.754 $SIZE >> triple_spiral.log 2>&1
            SIZE=$(($SIZE * 2))
    done

    SIZE=$INITIAL_SIZE

    mv *.log results/$NAME/$THREAD
    rm output.ppm
done
