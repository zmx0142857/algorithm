import nt

"""
运用 Pollard ρ 方法分解整数

首先生成数列 x_n:
选取种子 x_0 = 2, x_(n+1) = f(x_n) (mod n), 其中 f(x) = x^2 + 1
设 p | n, 寻找整数 x_i, x_j 满足 x_i = x_j (mod p) 且 x_i != x_j (mod n)
于是 (x_i - x_j, n) 是 n 的非平凡因子
65537 -> prime
9379 -> prime
65537*9379 -> 65537
32 -> boom!
"""
def pollard_rho(n):
    def f(x):
        ret = pow(x, 2, n) + 1
        return 0 if ret == n else ret
    x = 2
    y = 1
    cnt = 10000
    while x != y:# and cnt > 0:
        cnt -= 1
        x = f(x)
        y = f(y)
        g = nt.gcd(n, abs(x-y))
        #print('x = {}, y = {}, g = {}'.format(x, y, g))
        if 1 < g and g < n:
            #print('iteration: {}'.format(10000-cnt))
            return g
        x = f(x)
    #print('iteration: {}'.format(10000-cnt))
    if cnt == 0:
        return 'max iteration exceeded'
    return 'not found, maybe prime'

def factor(n, *, ret=None):
    if ret == None:
        ret = {}
    if nt.isprime(n):
        ret[n] = ret.get(n, 0) + 1
    elif n < 10000:
        ret = nt.factor(n, ret=ret)
    else:
        res = pollard_rho(n)
        if isinstance(res, int):
            ret = factor(res, ret=ret)
            ret = factor(n // res, ret=ret)
        else:
            print('sorry, ' + res)
    return ret
