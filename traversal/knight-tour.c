#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <string.h> // memset

typedef char bool;
const bool true = 1;
const bool false = 0;

int n;				// scale of board
bool **board;
int *x, *y;			// solution vector
bool found;			// already found a solution?

// possible offsets
const int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
const int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

void clear_board()
{
	for (int i = 0; i < n; ++i)
		memset(board[i], false, sizeof(bool)*n);
}

void set_board(int depth, int xi, int yi)
{
	x[depth] = xi;
	y[depth] = yi;
	board[xi][yi] = true;
}

void permutate(int depth)
{
	if (found) return;

	if (depth < n*n) {
		for (int i = 0; i < 8; ++i) {
			int xi = x[depth-1] + dx[i];
			int yi = y[depth-1] + dy[i];
			bool is_valid = (
				0 <= xi && xi < n &&
				0 <= yi && yi < n &&
				!board[xi][yi]
			);
			if (is_valid) {
				set_board(depth, xi, yi);
				permutate(depth+1);
			}
		}
	} else {
		printf("(%d, %d)", x[0], y[0]);
		for (int i = 1; i < n*n; ++i)
			printf(" -> (%d, %d)", x[i], y[i]);
		puts("");
		found = true;
	}
	board[x[depth-1]][y[depth-1]] = false;
}

void naive_knight_tour()
{
	found = false;
	if (n < 1) {
		puts("n >= 1 is required");
		return;
	}

	// init board
	board = (bool**)malloc(sizeof(bool*)*n);
	for (int i = 0; i < n; ++i)
		board[i] = (bool*)malloc(sizeof(bool)*n);
	x = (int*)malloc(sizeof(int)*n*n);
	y = (int*)malloc(sizeof(int)*n*n);

	// init start pointer
	int half = (n % 2 == 0 ? n / 2 : n / 2 + 1);
	for (int i = 0; i < half; ++i) {
		for (int j = 0; j < half; ++j) {
			clear_board();
			set_board(0, i, j);
			permutate(1);
		}
	}

	// free memory
	free(x);
	free(y);
	for (int i = 0; i < n; ++i)
		free(board[i]);
	free(board);
}

int main()
{
	printf("n = ");
	while (scanf("%d", &n) == 1) {
		naive_knight_tour(n);
		printf("n = ");
	}
	puts("");
	return 0;
}
