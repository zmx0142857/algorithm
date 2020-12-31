#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Provides operations on quadratic numbers x_1 + x_2 √2 + ... + x_n √n"""

__author__ = 'Clarence Zhuo'

from fractions import Fraction # 支持 Quad 和有理数的混合运算
import nt, math

def dic_add(dic, key, value):
    dic[key] = dic.get(key, 0) + value

def sqrt(n):
    if not isinstance(n, int):
        raise TypeError('expecting non-negative int')
    if n < 0:
        raise ValueError('expecting non-negative int')
    return Quad({n:1})

class Quad(object):
    """
    Doctest:
    >>> float(Quad({5:1, 1:1}))
    3.23606797749979
    >>> a = Quad({1: 1, 12: 1, 6: -1})
    >>> a
    1+2√3-√6
    >>> b = Quad({2: -2, 3: -1})
    >>> b
    -2√2-√3
    >>> a + b
    1-2√2+√3-√6
    >>> a * b
    -6+√2+3√3-4√6
    """
    def __init__(self, *args):
        if len(args) == 1 and isinstance(args[0], dict):
            self.dict = args[0]
        self.simplify()

    def __str__(self):
        buf = []
        for n in sorted(self.dict):
            coef = self.dict[n]
            if coef > 0 and len(buf) > 0:
                buf.append('+')
            if coef == -1:
                buf.append('-')
            elif coef != 1 or n == 1:
                buf.append(coef)
            if n > 1:
                buf.append('√')
                buf.append(n)
        return ''.join(str(x) for x in buf)

    __repr__ = __str__

    # eliminate zero items
    # cancel out squares
    def simplify(self):
        buf = {}
        for n in self.dict:
            if n == 0:
                continue
            coef = self.dict[n]
            f = nt.factor(n) # long time!
            m = 1
            for p in f:
                if f[p] > 1:
                    coef *= p * (f[p] // 2)
                if f[p] % 2:
                    m *= p
            dic_add(buf, m, coef)
        self.dict = buf

    def __float__(self):
        return sum(math.sqrt(n) * self.dict[n] for n in self.dict)

    def __neg__(self):
        for n in self.dict:
            self.dict[n] = -self.dict[n]

    def __add__(self, other):
        if not isinstance(other, Quad):
            return NotImplemented
        buf = {}
        for n in self.dict:
            dic_add(buf, n, self.dict[n])
        for n in other.dict:
            dic_add(buf, n, other.dict[n])
        return Quad(buf)

    def __sub__(self, other):
        if not isinstance(other, Quad):
            return NotImplemented
        return self + (-other)

    def __mul__(self, other):
        if not isinstance(other, Quad):
            return NotImplemented
        buf = {}
        for m in self.dict:
            for n in other.dict:
                dic_add(buf, m*n, self.dict[m] * other.dict[n])
        return Quad(buf)

    # 分母有理化
    # 运用平方差公式, 将含因子 √2 的放在一起, 不含的放在一起:
    # (a + c√3)^2 - (b√2 + d√6)^2
    # 就能消去因子 √2
    def __div__(self, other):
        if not isinstance(other, Quad):
            return NotImplemented
        return NotImplemented

if __name__ == '__main__':
    import doctest
    doctest.testmod()
