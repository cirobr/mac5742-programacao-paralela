#!/bin/bash

MEASUREMENTS=10
NAME='bs'

rm meas.log
perf stat -r $MEASUREMENTS ./$NAME input output 2>meas.log
rm output
