import nt

# find a, b in sorted list L such that a + b = val
def sum2(L, val):
    i = 0
    j = len(L)-1
    while i <= j:
        s = L[i] + L[j]
        if s > val:
            j -= 1
        elif s < val:
            i += 1
        else:
            return L[i], L[j]

L = nt.primes(100)
for val in range(4, 101, 2):
    print(val, sum2(L, val))
