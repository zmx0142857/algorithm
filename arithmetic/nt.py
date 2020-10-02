#!/usr/bin/env python3

""" number theory tools """

import functools

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
@unsigned
def gcd(a, b):
    if b == 0:
        return a
    return gcd(b, a % b)

# 推广的辗转相除法
# 设 a, b 为非负整数, 返回最大公约数 d, 以及满足 d = a x + b y 的系数 x, y
@unsigned
def gcd_ext(a, b):
    if b == 0:
        return a, 1, 0
    d, x, y = gcd_ext(b, a % b)
    return d, y, x - (a//b)*y

# 设 n 为非负整数, 返回平方不超过 n 的最大正整数
@unsigned
def sqrt(n):
    x = 1
    while True:
        x = (x + n//x) >> 1
        if x**2 <= n:
            break
    # assert (x+1)**2 > n
    return x

# 设 n 是整数, 用试除法判断 n 是否为素数
@integer
def isprime1(n):
    # 2, 3 是素数
    if n <= 3:
        return n >= 2
    # 其余素数为 6k+-1 的形式
    if (n % 6) % 4 != 1:
        return False
    # 用 6k+-1 试除
    return all(n % (k-1) != 0 and n % (k+1) != 0 for k in range(6, sqrt(n), 6))

isprime = isprime1

# 标准分解
# 设 n 是正整数, 返回 n 的所有素因子及其次数
# factor(252) -> {2:2, 3:2, 7:1}
@positive
def factor(n):
    factors = {}
    def add(i):
        if i in factors:
            factors[i] += 1
        else:
            factors[i] = 1

    p = 2
    while p <= n:
        while n % p == 0:
            add(p)
            n //= p
        # 避免遇到大素数时效率低下
        if isprime(n):
            add(n)
            break
        p += 1
    return factors

# 设 n 是正整数, 返回 n 的所有因子, 次序从小到大
@positive
def factors1(n):
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
@positive
def factors2(n):
    d = factor(n)
    keys = [k for k in d]
    ret = []

    # 深度优先搜索
    def _factors(n, acc, i):
        if i == -1:
            ret.append(acc)
            return
        k = keys[i]
        save_acc = acc
        for j in range(d[k]+1):
            _factors(n//k, acc, i-1)
            acc *= k
        acc = save_acc

    _factors(n, 1, len(keys)-1)
    return ret

factors = factors1
