#!/usr/bin/env python3

from math import pi, e

# 反转数字 i 的二进制表示, 如
# rev(1011) = 1101
def rev(i, n):
    ret = 0
    n >>= 1
    while n:
        ret <<= 1
        ret |= (i & 1)
        i >>= 1
        n >>= 1
    return ret

# 设 n 是 2 的幂
# 简记多项式 f(x) = sum_(0 <= k < n) a_k x^k 为 [a_0, ..., a_(n-1)].
# fft 返回 f 在 n 次单位根 ω_n^j = e^(2jπi/n) 处的值
# f_j = sum_(0 <= k < n) a_k ω_n^(jk)
# 如 fft([4, 3, 2, 1]) = [10, 2+2j, 2, 2-2j]
# 现假设 f 的次数是 2n-1, 定义多项式
# g = [a_0, a_2, a_4, ..., a_(2n-2)]
# h = [a_1, a_3, a_5, ..., a_(2n-1)]
# 则 f(x) = g(x^2) + x h(x^2), 特别
# f(ω_2n^j) = g(ω_2n^2j) + ω_2n^j h(ω_2n^2j)
#           = g(ω_n^j) + ω_2n^j h(ω_n^j)
# f(ω_2n^(n+j)) = g(ω_2n^(2n+2j)) + ω_2n^(n+j) h(ω_2n^(2n+2j))
#               = g(ω_2n^2j) - ω_2n^j h(ω_2n^2j)
#               = g(ω_n^j) - ω_2n^j h(ω_n^j)
# 从而将 fft(f) 转化为两个规模为原问题一半的问题 fft(g) 与 fft(h)
def fft(a, reverse=False):
    PI = -pi if reverse else pi
    n = len(a)
    f = [a[rev(i, n)] for i in range(n)]
    m = 1
    while m < n:
        omega = e**(PI*1j/m)
        for j in range(0, n, m << 1):
            omegak = 1
            for k in range(m):
                s = f[j + k]
                t = omegak * f[j + k + m]
                f[j + k], f[j + k + m] = s + t, s - t
                omegak *= omega
        m <<= 1
    return [x/n for x in f] if reverse else f

# 计算 n 维向量 a, b 的卷积, n 是 2 的幂
# a (*) b = fft^-1( fft(a) * fft(b) )
# [4, 3, 2, 1] (*) [4, 3, 2, 1] = [16, 24, 25, 20, 10, 4, 1, 0], 因此
# (x^3 + 2x^2 + 3x + 4)^2 = x^6 + 4x^5 + 10x^4 + 20x^3 + 25x^2 + 24x + 16
def convolution(a, b):
    a += [0 for x in a]
    if b is not a:
        b += [0 for x in b]
    return fft([t[0] * t[1] for t in zip(fft(a), fft(b))], True)

