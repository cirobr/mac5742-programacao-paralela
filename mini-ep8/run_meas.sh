#!/bin/bash

MEASUREMENTS=10
NAME='bs.o'

rm meas.log
perf stat -r $MEASUREMENTS ./$NAME input output 2>meas.log
chown ciro:ciro meas.log
rm output
