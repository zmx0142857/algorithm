#!/usr/bin/env bash
time ./prime 1000000000 1 > prime.out1 &&
wc -w prime.out1 &&
time ./prime 1000000000 2 > prime.out2 &&
wc -w prime.out2 &&
diff prime.out1 prime.out2

#real	0m51.415s
#user	0m46.932s
#sys	0m1.168s
#50847534 prime.out1
#
#real	0m31.029s
#user	0m25.987s
#sys	0m1.304s
#50847534 prime.out2
