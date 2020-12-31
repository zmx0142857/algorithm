#!/usr/bin/env python3

""" number theory tools """

import functools
from fractions import Fraction

def arg_checker(code):
    def decorator(fn):
        @functools.wraps(fn)
        def wrapper(*args, **kw):
            # 对 kw 不作检查
            for a in args:
                if not isinstance(a, int):
                    raise TypeError('expecting int, got %s' % type(a))
                if code == 0 and a < 0:
                    raise ValueError('expecting non-negative int, got %s' % a)
                elif code == 1 and a < 1:
                    raise ValueError('expecting positive int, got %s' % a)
            return fn(*args, **kw)
        return wrapper
    return decorator

integer = arg_checker(-1)
unsigned = arg_checker(0)
positive = arg_checker(1)

# 辗转相除法
# 设 a, b 为非负整数, 返回最大公约数
# @unsigned
def gcd(a, b):
    def _gcd(a, b):
        if b == 0:
            return a
        return _gcd(b, a % b)
    return _gcd(a, b)

# 推广的辗转相除法
# 设 a, b 为非负整数, 返回最大公约数 d, 以及满足 d = a x + b y 的系数 x, y
# @unsigned
def gcd_ext(a, b):
    def _gcd_ext(a, b):
        if b == 0:
            return a, 1, 0
        d, x, y = _gcd_ext(b, a % b)
        return d, y, x - (a//b)*y
    return _gcd_ext(a, b)

# 设 n 为非负整数, 返回平方不超过 n 的最大正整数
# @unsigned
def sqrt(n):
    x = 1
    while True:
        x = (x + n//x) >> 1
        if x**2 <= n:
            break
    # assert (x+1)**2 > n
    return x

# 判断平方数
# @integer
def issquare(n):
    if isinstance(n, Fraction):
        return issquare(n.denominator) and issquare(n.numerator)
    if n < 0:
        return False
    if n % 32 not in {0, 1, 4, 9, 16, 25, 17}: # 49 = 32 + 17
        return False
    r = sqrt(n)
    return r*r == n

"""素性检测"""

# 设 n 是整数, 用试除法判断 n 是否为素数
# @integer
def isprime1(n):
    # 2, 3 是素数
    if n <= 3:
        return n >= 2
    # 其余素数为 6k+-1 的形式
    if (n % 6) % 4 != 1:
        return False
    # 用 6k+-1 试除
    return all(n % (k-1) != 0 and n % (k+1) != 0 for k in range(6, sqrt(n), 6))

# Lucas-Lehmer 判定法
# 判断梅森数 2^p-1 是否为素数的高效算法
# [ismersenne(2**p-1) for p in [2, 3, 5, 7, 13, 17, 19, 31, 67, 127, 257]]
def ismersenne(n):
    p = int.bit_length(n)
    if n == 3:
        return True
    if not isprime1(p):
        return False
    r = 4
    for i in range(p-2):
        r = (pow(r, 2, n) - 2) % n
    return r == 0

def isprime(n):
    # n 是 2^m-1 的形式
    if n + 1 == 1 << int.bit_length(n):
        return ismersenne(n)
    else:
        return isprime1(n)

"""素数序列"""

# Eratosthenes 筛法
def eratosthenes_sieve(n, i, flag, ret):
    if not flag[i]:
        ret.append(i)
        j = i*i
        while j <= n:
           flag[j] = True
           j += i

# Euler 筛法
def euler_sieve(n, i, flag, ret):
    if not flag[i]:
        ret.append(i)
    for j in range(n):
        m = i * ret[j]
        if m > n:
            break
        flag[m] = True
        if i % ret[j] == 0:
            break

# 返回不超过 n 的所有素数
# @positive
def primes(n, sieve=None):
    # init
    if n < 2:
        return False
    flag = [False for i in range(n+1)]
    ret = []
    # default sieve method
    if sieve == None:
        sieve = euler_sieve
    # let's go!
    for i in range(2, n+1):
        sieve(n, i, flag, ret)
    return ret

"""因子分解"""

# 标准分解
# 设 n 是正整数, 返回 n 的所有素因子及其次数
# factor(252) -> {2:2, 3:2, 7:1}
# @positive
def factor1(n, *, factors=None):
    if factors == None:
        factors = {}
    p = 2
    while p <= n:
        while n % p == 0:
            factors[p] = factors.get(p, 0) + 1
            n //= p
        # 避免遇到大素数时效率低下
        if isprime(n):
            factors[n] = factors.get(n, 0) + 1
            break
        p += 1
    return factors

"""
运用 Pollard ρ 方法寻找 n 的非平凡因子

首先生成数列 x_n:
选取种子 x_0 = 2, x_(n+1) = f(x_n) (mod n), 其中 f(x) = x^2 + 1
设 p | n, 寻找整数 x_i, x_j 满足 x_i = x_j (mod p) 且 x_i != x_j (mod n)
于是 (x_i - x_j, n) 是 n 的非平凡因子
65537 -> prime
9379 -> prime
65537*9379 -> 65537
32 -> boom!
"""
# @positive
def pollard_rho(n):
    f = lambda x: (pow(x, 2, n) + 1) % n
    x, y = 2, 1
    while x != y:
        x, y = f(x), f(y)
        g = gcd(n, abs(x-y))
        if 1 < g and g < n:
            return g
        x = f(x)
    return 'not found, maybe prime'

# 试试看！
# 2**67-1 = 193707721 * 761838257287
# 33599569701452818993 = 2976811763 * 11287099211 很吃力...
# @positive
def factor2(n, *, factors=None):
    if factors == None:
        factors = {}
    if isprime(n):
        factors[n] = factors.get(n, 0) + 1
    elif n < 10000:
        factors = factor1(n, factors=factors)
    else:
        res = pollard_rho(n)
        if isinstance(res, int):
            factors = factor2(res, factors=factors)
            factors = factor2(n // res, factors=factors)
        else:
            print('sorry, ' + res)
    return factors

factor_dict = {}
# cached
def factor(n, *, factors=None):
    if factors == None:
        factors = {}
    if n in factor_dict:
        for p in factor_dict[n]:
            factors[p] = factors.get(p, 0) + factor_dict[n][p]
        return factors
    ret = factor2(n, factors=factors)
    factor_dict[n] = ret.copy()
    return ret

# 设 n 是正整数, 返回 n 的所有因子, 次序从小到大
# @positive
def all_factors1(n):
    lo = []
    hi = []
    for d in range(1, sqrt(n)+1):
        if n % d == 0:
            lo.append(d)
            hi.append(n//d)
    if lo[-1] == hi[-1]:
        return lo[:-1] + list(reversed(hi))
    return lo + list(reversed(hi))

# 设 n 是正整数, 返回 n 的所有因子, 不保证次序从小到大
# @positive
def all_factors2(n):
    d = factor(n)
    keys = [k for k in d]
    ret = []

    def dfs(n, acc, i):
        if i == -1:
            ret.append(acc)
            return
        k = keys[i]
        save_acc = acc
        for j in range(d[k]+1):
            dfs(n//k, acc, i-1)
            acc *= k
        acc = save_acc

    dfs(n, 1, len(keys)-1)
    return ret

all_factors = all_factors1

# 累乘
def prod(iterable):
    mul = lambda x, y: x * y
    return functools.reduce(mul, iterable, 1)

# 求积性函数的值
# @positive
def mul_func(f, n):
    factors = n if isinstance(n, dict) else factor(n)
    return prod(f(p, a) for p, a in factors.items())

# 求 Euler 函数的值
def phi(n):
    f = lambda p, a: p**(a-1)*(p-1)
    return mul_func(f, n)
