#include <stdio.h>

#define ROWS 100
#define COLS 100
#define WALL '#'
#define EMPTY '.'
#define START 'S'
#define GOAL 'G'
#define VISITED 'V'
char map[ROWS][COLS];
char cpy[ROWS][COLS];
int rows = 10;
int cols = 10;
int rstart, cstart, rgoal, cgoal;

void maze_scan(const char *filename)
{
	FILE *fin = fopen(filename, "r");
	if (!fin)
		return;
	rows = 0;
	cols = 0;
	char c;
	int j = 0;
	while ((c = fgetc(fin)) != EOF) {
		if (c == '\n') {
			j = 0;
			++rows;
		} else {
			map[rows][j++] = c;
			if (j > cols)
				cols = j;
			if (c == 'S') {
				rstart = rows;
				cstart = j-1;
			} else if (c == 'G') {
				rgoal = rows;
				cgoal = j-1;
			}
		}
	}
	fclose(fin);
}

void maze_print(char map[][COLS])
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			switch (map[i][j]) {
				case WALL: printf("\033[7m  \033[0m"); break;
				case EMPTY: printf("  "); break;
				case START: printf("ST"); break;
				case GOAL: printf("GL"); break;
				case VISITED: printf("  "); break;
				default: printf("%c%c", map[i][j], map[i][j]);
			}
		}
		putchar('\n');
	}
}

#define SIZE ROWS*COLS
typedef int Dir;
Dir stack[SIZE];
char go[4];
int dr[4];
int dc[4];

typedef char bool;
const bool true = 1;
const bool false = 0;
bool found = false;

void dfs(int top, int row, int col)
{
	for (Dir dir = 0; dir < 4; ++dir) {
		row += dr[dir];
		col += dc[dir];
		if (cpy[row][col] == EMPTY) {
			cpy[row][col] = VISITED;
			stack[top] = dir;
			dfs(top+1, row, col);
		} else if (cpy[row][col] == GOAL) {
			stack[top] = dir;
			found = true;
		}
		row -= dr[dir];
		col -= dc[dir];
		if (found) {
			if (top)
				cpy[row][col] = go[stack[top]];
			break;
		}
	}
}

int rqueue[SIZE];
int cqueue[SIZE];
int head = 0, tail = 0;

void enqueue(int row, int col)
{
	rqueue[tail] = row;
	cqueue[tail] = col;
	if (++tail == SIZE)
		tail = 0;
}

void dequeue(int *row, int *col)
{
	*row = rqueue[head];
	*col = cqueue[head];
	if (++head == SIZE)
		head = 0;
}

void bfs(int row, int col)
{
	enqueue(row, col);
	while (head != tail) {
		int r, c;
		dequeue(&r, &c);
		for (Dir dir = 0; dir < 4; ++dir) {
			r += dr[dir];
			c += dc[dir];
			if (map[r][c] == EMPTY) {
				map[r][c] = go[dir ^ 2]; // reverse direction
				enqueue(r, c);
			} else if (map[r][c] == START) {
				r -= dr[dir];
				c -= dc[dir];
				while (map[r][c] != GOAL) {
					cpy[r][c] = map[r][c];
					switch (cpy[r][c]) {
						case 'v': ++r; break;
						case '>': ++c; break;
						case '^': --r; break;
						case '<': --c; break;
					}
				}
				return;
			}
			r -= dr[dir];
			c -= dc[dir];
		}
	}
}

void maze_solve(const char *method,
		int row, int col, const char *preference)
{
	for (Dir i = 0; i < 4; ++i) {
		go[i] = preference[i];
		switch (go[i]) {
			case 'v': dr[i] = 1; dc[i] = 0; break;
			case '>': dr[i] = 0; dc[i] = 1; break;
			case '^': dr[i] = -1; dc[i] = 0; break;
			case '<': dr[i] = 0; dc[i] = -1; break;
		}
	}
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			cpy[i][j] = map[i][j];
	if (method[0] == 'd')
		dfs(0, row, col); // search from start (row, col)
	else if (method[0] == 'b')
		bfs(row, col); // search from goal (row, col)
	maze_print(cpy);
}

int main()
{
	maze_scan("maze.in");
	puts("dfs:");
	maze_solve("dfs", rstart, cstart, "v>^<");
	puts("bfs:");
	maze_solve("bfs", rgoal, cgoal, "v>^<");
	return 0;
}
