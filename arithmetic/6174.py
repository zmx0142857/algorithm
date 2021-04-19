def n2d(n):
    ret = []
    for i in range(4):
        ret.append(n % 10)
        n //= 10
    return list(reversed(ret))

def d2n(digits):
    ret = 0
    for d in digits:
        ret *= 10
        ret += d
    return ret

def parent(n):
    digits = n2d(n)
    minN = d2n(sorted(digits))
    maxN = d2n(sorted(digits, reverse=True))
    return maxN - minN

# init tree from parent pointers
N = 10000
first_child = [0 for i in range(N)]
next_sibling = [0 for i in range(N)]
for n in range(1, N):
    p = parent(n)
    if p == n:
        continue
    child = first_child[p]
    if not child:
        first_child[p] = n
        #print('first_child[{}] = {}'.format(p, n))
    else:
        while next_sibling[child]:
            child = next_sibling[child]
        next_sibling[child] = n
        #print('next_sibling[{}] = {}'.format(child, n))
    #input()

# print tree
def preorder(n, depth):
    if n:
        print('    ' * depth + str(n))
        n = first_child[n]
        while n:
            preorder(n, depth+1)
            n = next_sibling[n]

preorder(6174, 0)
