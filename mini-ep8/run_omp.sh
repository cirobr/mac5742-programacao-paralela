#!/bin/bash

MEASUREMENTS=10
NAME='bs_omp.o'

rm meas_omp.log
perf stat -r $MEASUREMENTS ./$NAME input output 2>meas_omp.log
chown ciro:ciro meas_omp.log
rm output
