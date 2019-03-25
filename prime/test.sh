#!/usr/bin/env bash
time ./prime 10000000 1 > prime.out1 &&
wc -w prime.out1 &&
time ./prime 10000000 2 > prime.out2 &&
wc -w prime.out2 &&
diff prime.out1 prime.out2
