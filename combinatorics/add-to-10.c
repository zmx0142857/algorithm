#include <stdio.h>

typedef char bool;
const bool true = 1;
const bool false = 0;

#define ROUNDS 30
#define OUTPUTS 2147483647

const size_t L = 0;
const size_t R = 1;
const size_t A = 1; // A goes first
const size_t B = 0;
char game[ROUNDS][2][2] = {
	{ {1, 1}, {1, 1} }
};
size_t output_cnt = 0;
size_t max_depth_reached = 0;
size_t begin = 0;

bool game_over(size_t depth)
{
	if (game[depth][A][L] >= 10 && game[depth][A][R] >= 10)
		return 'A';
	else if (game[depth][B][L] >= 10 && game[depth][B][R] >= 10)
		return 'B';
	return false;
}

void output(size_t depth)
{
	/*
	for (size_t i = 0; i < begin; ++i)
		printf("     ");
	for (size_t i = begin; i <= depth; ++i)
		printf("%c%c%c%c ",
				game[i][A][L] >= 10 ? 'x' : game[i][A][L] + '0',
				game[i][A][R] >= 10 ? 'x' : game[i][A][R] + '0',
				game[i][B][L] >= 10 ? 'x' : game[i][B][L] + '0',
				game[i][B][R] >= 10 ? 'x' : game[i][B][R] + '0');
				*/
	if (depth == ROUNDS) {
		//printf("maximum depth reached.");
		++max_depth_reached;
	}
	//printf("<- %c\n", game_over(depth));
	++output_cnt;
}

void permutate(size_t depth)
{
	if (depth < ROUNDS) {
		// copy status
		game[depth][A][L] = game[depth-1][A][L];
		game[depth][A][R] = game[depth-1][A][R];
		game[depth][B][L] = game[depth-1][B][L];
		game[depth][B][R] = game[depth-1][B][R];
		// make change
		size_t player = depth % 2;
		size_t opponent = 1 - player;
		for (size_t i = 0; i < 2; ++i) {
			for (size_t j = 0; j < 2; ++j) {
				if (game[depth][player][i] >= 10
					|| game[depth][opponent][j] >= 10)
					continue;
				game[depth][player][i] += game[depth][opponent][j];
				if (game_over(depth)) {
					if (output_cnt == OUTPUTS)
						return;
					output(depth);
					begin = depth;
				} else {
					permutate(depth+1);
				}
				game[depth][player][i] -= game[depth][opponent][j];
			}
		}
	} else {
		if (output_cnt == OUTPUTS)
			return;
		output(depth);
		begin = depth;
	}
	--begin;
}

int main()
{
	/*
	for (size_t i = 0; i < ROUNDS/2; ++i)
		printf("B    A    ");
	puts("");
	*/
	permutate(1);
	if (output_cnt == OUTPUTS)
		printf("maximum output (%d) reached.\n", OUTPUTS);
	else
		printf("output %ld lines.\n", output_cnt);
	printf("max_depth_reached = %ld.\n", max_depth_reached);
	return 0;
}
