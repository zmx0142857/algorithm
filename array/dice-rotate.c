#include <stdio.h>

char dice[7] = "123456"; // 0 ~ 5 分别表示骰子的 左 右 前 后 上 下
int idx[6][4] = {
	{0, 4, 1, 5},
	{5, 1, 4, 0},
	{2, 4, 3, 5},
	{5, 3, 4, 2},
	{0, 3, 1, 2},
	{2, 1, 3, 0}
};

// n 的含义
#define LEFT 0
#define RIGHT 1
#define FRONT 2
#define BACK 3
#define ACW 4
#define CW 5
void roll(int n)
{
	int tmp = dice[idx[n][0]];
	dice[idx[n][0]] = dice[idx[n][1]];
	dice[idx[n][1]] = dice[idx[n][2]];
	dice[idx[n][2]] = dice[idx[n][3]];
	dice[idx[n][3]] = tmp;
}

int main()
{
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n') {
		switch (ch) {
			case 'L': roll(LEFT); break;
			case 'R': roll(RIGHT); break;
			case 'F': roll(FRONT); break;
			case 'B': roll(BACK); break;
			case 'A': roll(ACW); break;
			case 'C': roll(CW); break;
		}
	}
	puts(dice);
	return 0;
}
