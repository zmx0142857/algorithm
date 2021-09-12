""" examples
>>> c = Cycle(1, 2, 3)
>>> c
Cycle {1: 2, 2: 3, 3: 1}
>>> [c[1], c[2], c[3]]
[2, 3, 1]
>>> _factor({ 1: 2, 2: 3, 3: 1 })
[(1, 2, 3)]
>>> p = Perm({ 1: 2, 2: 3, 3: 1, 4: 5, 5: 4 })
>>> p.factor()
'(1 2 3) (4 5)'
>>> p
Perm {1: 2, 2: 3, 3: 1, 4: 5, 5: 4}
>>> p.inv()
Perm {2: 1, 3: 2, 1: 3, 5: 4, 4: 5}
>>> p.ord()
6
>>> c.conj(p)
Perm {1: 2, 2: 3, 3: 1}
>>> Perm({ 'a': 'b', 'b': 'c', 'c': 'a', 'd': 'e', 'e': 'd' }).factor()
'(a b c) (d e)'
>>> c**(-2)
Perm {1: 2, 2: 3, 3: 1}
>>> (c**3).factor()
'()'
>>> Cycle(1, 2) * Cycle(3, 4) * Cycle(1, 3) * Cycle(2, 4) != Cycle(1, 3) * Cycle(2, 4) * Cycle(1, 2) * Cycle(3, 4)
False
>>> Cycle()
Cycle {}
"""

import math

# 置换 Permutation
class Perm:
    def __init__(self, perm: dict):
        # 约去 1 -> 1 映射
        self.perm = dict(i for i in perm.items() if i[0] != i[1])
        self.cycles = None

    def __str__(self):
        return f'{self.__class__.__name__} {self.perm}'

    __repr__ = __str__

    def __getitem__(self, x: int):
        return self.perm.get(x, x)

    def factor(self):
        if not self.cycles:
            self.cycles = _factor(self.perm.copy())
        return str(self.cycles)\
            .replace("'", '')\
            .replace('[]', '()')\
            .replace('[', '')\
            .replace(']', '')\
            .replace(',', '')

    def __eq__(self, other):
        return self.perm == other.perm

    def __mul__(self, other):
        keys = set(self.perm.keys()).union(set(other.perm.keys()))
        ret = {}
        for k in keys:
            ret[k] = self[other[k]]
        return Perm(ret)

    # 逆
    def inv(self):
        ret = dict((v, k) for (k, v) in self.perm.items())
        return Perm(ret)

    # 快速幂
    def __pow__(self, n):
        a = self
        if n < 0:
            a = self.inv()
            n = -n
        ret = Perm({}) # 单位元
        mask = 1 << n.bit_length()-1
        while mask:
            ret *= ret
            if n & mask:
                ret *= a
            mask >>= 1
        return ret

    # 共轭
    def conj(self, other):
        return other * self * other.inv()

    # 换位子
    def commutor(self, other):
        return self * other * self.inv() * other.inv()

    # 各个轮换的阶的最小公倍数
    def ord(self):
        if not self.cycles:
            self.factor()
        return math.lcm(*(len(c) for c in self.cycles))

# 轮换
class Cycle(Perm):
    def __init__(self, *arr):
        self.cycles = [tuple(arr)]
        self.perm = {}
        if len(arr):
            k = arr[0]
            for v in arr[1:]:
                self.perm[k] = v
                k = v
            self.perm[arr[-1]] = arr[0]

# 分解为不相交轮换
def _factor(perm: dict):
    factors = []
    while len(perm) > 1:
        k0 = next(iter(perm.keys()))
        cyc = [k0]
        k = perm.pop(k0)
        while k != k0:
            cyc.append(k)
            k = perm.pop(k)
        factors.append(tuple(cyc))
    if len(perm) == 1:
        factors.append(tuple(perm.keys()))
    return factors

if __name__ == '__main__':
    import doctest
    doctest.testmod()
