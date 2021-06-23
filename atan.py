from fractions import Fraction as F

def init(n, k):
    if k == 0:
        return 1 if n == 0 else 0
    if k == 1:
        return F(1, 2*n+1) if n % 2 == 0 else F(-1, 2*n+1)

L = [[init(n, k) for n in range(20)] for k in range(2)]

for k in range(2, 10):
    L.append([])
    for n in range(len(L[k-1])-1):
        L[k].append((L[k-2][n+1] - (2*k-3)*L[k-1][n+1])/(k-1)**2)

print('\n'.join(' '.join(str(el) for el in row) for row in L))
