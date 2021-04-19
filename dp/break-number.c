#include <stdio.h>

int dp[1000][1000];
int count = 0;

// 输入: 100 100
// res: 190569292, count: 405050416
int rec(int m, int n) {
    ++count;
    if (m == 0 || n == 1) return 1;
    if (n > m) return rec(m, m);
    return rec(m, n-1) + rec(m-n, n);
}


// 输入: 100 100
// res: 190569292, count: 10000
int non_rec(int m, int n) {
    if (n > m) {
        n = m;
    }
    // 边界条件
    for (int j = 1; j <= n; ++j) {
        dp[0][j] = 1;
    }
    for (int i = 0; i <= m; ++i) {
        dp[i][1] = 1;
    }
    // 填表
    for (int i = 1; i <= m; ++i) {
        for (int j = 2; j <= n; ++j) {
            if (j > i) dp[i][j] = dp[i][i];
            else dp[i][j] = dp[i][j-1] + dp[i-j][j];
        }
    }
    count = m * n;
    return dp[m][n];
}

int main() {
    int m, n;
    while (scanf("%d %d", &m, &n) == 2) {
        count = 0;

        printf("res: %d, ", non_rec(m, n));
        //printf("res: %d, ", rec(m, n));

        printf("count: %d\n", count);
    }
    return 0;
}
