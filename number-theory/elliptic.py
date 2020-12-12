primes = [2, 3, 5, 7, 11, 13, 17, 19, 23]

# 1. solve y^2 = x^3 - x (mod p)
test1 = lambda x, y, p: 1 if (pow(y, 2, p) + x) % p == pow(x, 3, p) else 0

def foo(p):
    for x in range(p):
        for y in range(p):
            if test1(x, y, p):
                print(x, y)

# [2, 3, 7, 7, 11, 7, 15, 19, 23]
print([
    sum(test1(x, y, p) for x in range(p) for y in range(p))
    for p in primes
])

# 2. expand x * prod_(k >= 0) (1-x^(4k))(1-x^(8k))
