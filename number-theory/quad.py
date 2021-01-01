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
    >>> float(Quad({5:1, 1:1}))
    3.23606797749979
    >>> Quad({3: Fraction(5, 2)}).inv()
    2/15√3
    >>> Quad({1:1, 2:1}).inv()
    -1+√2
    >>> b.inv()
    -2/5√2+1/5√3
    >>> Quad({1:1, 5:1}) * Quad({1:1, 5:-1})
    -4
    >>> 1 + sqrt(4)
    3
    >>> sqrt(4) + 2
    4
    >>> sqrt(25) - 3
    2
    >>> 1 / sqrt(4)
    1/2
    >>> 1 / sqrt(6)
    1/6√6
    >>> (1 + sqrt(2) + sqrt(3)).inv()
    1/2+1/4√2-1/4√6
    >>> a = Quad({1:1, 2:1, 3:1, 5:1, 7:1})
    >>> b = a.inv()
    >>> b.inv()
    1+√2+√3+√5+√7
    >>> a * b
    1
    """
    #s.replace(/([+-])?(\d+)\/(\d+)√?(\d+)?/g, ',$4:$1Fraction($2,$3)')
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
                if n == 1:
                    buf.append('1')
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
            coef = self.dict[n]
            if coef == 0:
                continue
            f = nt.factor(n) # long time!
            m = 1
            for p in f:
                if f[p] > 1:
                    coef *= p * (f[p] // 2)
                if f[p] % 2:
                    m *= p
            dic_add(buf, m, coef)
        self.dict = buf

    def __eq__(self, other):
        if not isinstance(other, Quad):
            try:
                other = Quad({1: Fraction(other)})
            except Exception:
                return NotImplemented
        if len(self.dict) != len(other.dict):
            return False
        for k in self.dict:
            if k not in other.dict:
                return False
            if self.dict[k] != other.dict[k]:
                return False
        return True

    def __ne__(self, other):
        return False

    def __float__(self):
        return sum(math.sqrt(n) * self.dict[n] for n in self.dict)

    # copy of self
    def __pos__(self):
        return Quad(self.dict.copy())

    copy = __pos__

    def __neg__(self):
        for n in self.dict:
            self.dict[n] = -self.dict[n]

    def __add__(self, other):
        if not isinstance(other, Quad):
            if other == 0:
                return self
            try:
                other = Quad({1: Fraction(other)})
            except Exception:
                return NotImplemented
        buf = {}
        for n in self.dict:
            dic_add(buf, n, self.dict[n])
        for n in other.dict:
            dic_add(buf, n, other.dict[n])
        return Quad(buf)

    def __sub__(self, other):
        return self + (-other)

    def __mul__(self, other):
        if not isinstance(other, Quad):
            if other == 1:
                return +self
            elif other == -1:
                return -self
            try:
                other = Quad({1: Fraction(other)})
            except Exception:
                return NotImplemented
        buf = {}
        for m in self.dict:
            for n in other.dict:
                dic_add(buf, m*n, self.dict[m] * other.dict[n])
        return Quad(buf)

    def __truediv__(self, other):
        if not isinstance(other, Quad):
            if other == 1:
                return +self
            elif other == -1:
                return -self
            elif other == 0:
                raise ZeroDivisionError
            try:
                return self / Fraction(other)
            except Exception:
                return NotImplemented
            return NotImplemented
        return self * other.inv()

    def __radd__(rhs, lhs):
        return rhs + lhs

    def __rsub__(rhs, lhs):
        return (-rhs) + lhs

    def __rmul__(rhs, lhs):
        return rhs * lhs

    def __rtruediv__(rhs, lhs):
        return rhs.inv() * lhs

    # 关于素数 p 的共轭: 将所有含因子 √p 的项符号取反
    def conj(self, p):
        buf = {}
        for k, v in self.dict.items():
            buf[k] = -v if k % p == 0 else v
        return Quad(buf)

    # 分母有理化
    # 运用平方差公式, 将含因子 √2 的放在一起, 不含的放在一起:
    # (a + c√3)^2 - (b√2 + d√6)^2
    # 就能消去因子 √2. 类似消去所有素因子 √p
    def inv(self, factor=1):
        keys = (k for k in self.dict)
        if len(self.dict) == 1:
            a = next(keys)
            return Quad({a: Fraction(1, self.dict[a] * a)}) * factor
        p = 1
        while p == 1:
            a = next(keys)
            p = nt.prime_of(a)
        conj = self.conj(p)
        #print(self * conj, '^-1 *', factor * conj)
        #input('<press enter>')
        return (self * conj).inv(factor * conj)

if __name__ == '__main__':
    import doctest
    doctest.testmod()
