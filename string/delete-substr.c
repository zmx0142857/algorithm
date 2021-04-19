#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* 依次将 p[0] 和主串的下标 0, 1, 2, ... 对齐进行串的匹配,
 * 直到匹配成功或尝试过所有偏移量而失败为止. 时间复杂度为 O(s.len*p.len).
 */
void del(char *s, char *p)
{
    int i = 0, j = 0;
    int m = strlen(p);
    int read = 0;
    while (s[i]) {
        if (tolower(s[i]) != tolower(p[j])) { // 匹配失败
            i -= j - 1; // i 回退, 注意 j == 0 时 i 实际上在变大
            j = 0;
        } else if (j == m-1) { // 匹配成功, s[i-j..i] == p[0..m-1]
            while (read < i-j) {
                putchar(s[read++]);
            }
            ++i;
            j = 0;
            read = i; // 匹配成功的部分跳过不打印
        } else {
            ++i, ++j;
        }
    }
    puts(s + read);
}

int main()
{
    char str[100], sub[100];
    while (fgets(sub, 100, stdin) && fgets(str, 100, stdin)) {
        str[strlen(str)-1] = 0; // 把读入的 \n 换成 \0
        sub[strlen(sub)-1] = 0;
        del(str, sub);
    }
    return 0;
}
