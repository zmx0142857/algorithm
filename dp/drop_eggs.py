# f 层楼, e 个蛋, 寻找鸡蛋刚好摔碎的临界楼层, 求最少次数
# 鸡蛋没有摔碎的话, 可以再利用

# 动态规划: 即, 填表法
# dp(f, 1) = f
# dp(1, e) = 1
# dp(f, e) = 1 + min(max(dp(k-1, e-1), dp(f-k, e)) for k=1..f)
def drop_eggs(floors, eggs):
    dp = [[1 for j in range(eggs+1)] for i in range(floors+1)]
    for i in range(1, floors+1):
        dp[i][1] = i
    for i in range(2, floors+1):
        for j in range(2, eggs+1):
            dp[i][j] = 1 + min(
                # 从第 k 层扔鸡蛋. 碎与不碎未知, 应该取最大
                max(dp[k-1][j-1], dp[i-k][j])
                for k in range(1, i)
            )
    return dp[floors][eggs]

while True:
    try:
        f, e = tuple(map(eval, input().split()))
        print(drop_eggs(f, e))
    except EOFError:
        break
