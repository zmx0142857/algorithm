#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from random import randint
with open('in', 'w') as fin:
    for i in range(100000):
        fin.write('%d ' % randint(1, 100000))
    fin.write('\n')
