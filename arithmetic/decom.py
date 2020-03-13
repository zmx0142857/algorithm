# 设 n 是正整数, 返回 n 的所有素因子及其次数,
# 如 n = 252 时, 得到 {2:2, 3:2, 7:1}
def decom(n):
    d = {}
    for i in range(2,n+1):
        while n % i == 0:
            if i in d:
                d[i] += 1
            else:
                d[i] = 1
            n //= i
    return d

while True:
    try:
        n = int(input())
        print(decom(n))
    except EOFError:
        break
