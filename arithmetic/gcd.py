# 计算最大公约数 d, 同时求出系数 x, y 满足 a x + b y = d
# 比如 a = 75, b = 32:
# 11 = (75) - 2(32) = (1,0) - 2(0,1) = (1,-2)
# 10 = (32) - 2(11) = (0,1) - 2(1,-2) = (-2,5)
# 1 = (11) - (10) = (1,-2) - (-2,5) = (3,-7)
# 因此 1 = 3(75) - 7(32)
def gcd_coef(a, b):
    coef_a = (1, 0)
    coef_b = (0, 1)
    if a < 0:
        a = -a
        coef_a = (-1, 0)
    if b < 0:
        b = -b
        coef_b = (0, -1)
    while b:
        res = divmod(a, b)
        a, b = b, res[1]
        coef_a, coef_b = coef_b, (coef_a[0]-res[0]*coef_b[0], coef_a[1]-res[0]*coef_b[1])
    return (*coef_a, a)

while True:
    try:
        a, b = tuple(map(int, input().split()))
        t = gcd_coef(a, b)
        if (t[1] < 0):
            print('%d(%d) - %d(%d) = %d' % (t[0], a, -t[1], b, t[2]))
        else:
            print('%d(%d) + %d(%d) = %d' % (t[0], a, t[1], b, t[2]))
    except EOFError:
        break
