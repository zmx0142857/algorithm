#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# move a disk from a to b
def move(a, b):
    #print('%c -> %c' % (a, b))
    return '%c -> %c' % (a, b)

# move n disk from a to c by b
def hanoi(n, a, b, c):
    if n == 1:
        move(a, c)
    else:
        hanoi(n-1, a, c, b)
        move(a, c)
        hanoi(n-1, b, a, c)

# 13.440s
hanoi(24, 'a', 'b', 'c')
