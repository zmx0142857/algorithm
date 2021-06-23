# 方法一，做一个快乐的调包侠
def power_set1(nums):
    import itertools
    L = []
    for i in range(len(nums)+1):
        L.append(list(itertools.combinations(nums, i)))
    return [list(t) for t in itertools.chain(*L)]

# 方法二，做一个勇敢的轮子哥
def power_set2(nums):
    ret = []
    buf = []
    def rec(i):
        if i == len(nums):
            ret.append(buf.copy())
        else:
            j = len(buf) # 记住此时的长度
            # 情况一, 子集含有 nums[i]
            buf.append(nums[i])
            rec(i+1)
            # 情况二, 子集不含 nums[i]
            buf.pop(j) # 把刚才插入的值弹出
            rec(i+1)
    rec(0)
    return ret

print(power_set1([1, 2, 3]))
print(power_set2([1, 2, 3]))
