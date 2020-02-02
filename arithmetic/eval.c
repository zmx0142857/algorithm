#include <stdio.h>

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define LPAREN 4
#define RPAREN 5
#define EOL 6
typedef char Precede;
Precede precede[7][7] = {
	{'>', '>', '<', '<', '<', '>', '>'},
	{'>', '>', '<', '<', '<', '>', '>'},
	{'>', '>', '>', '>', '<', '>', '>'},
	{'>', '>', '>', '>', '<', '>', '>'},
	{'<', '<', '<', '<', '<', '=', 'N'},
	{'>', '>', '>', '>', 'N', '>', '>'},
	{'<', '<', '<', '<', '<', 'N', '='},
};

#define STACK_SIZE 100
int operator[STACK_SIZE], optop = 0;
int operand[STACK_SIZE], numtop = 0;

#define OP 0
#define NUM 1
typedef struct {
	char type;
	int val;
} Token;
Token token[STACK_SIZE] = {
	{NUM, 3}, {OP, MUL}, {OP, LPAREN}, {NUM, 7}, {OP, SUB}, {NUM, 2}, {OP, RPAREN}, {OP, EOL}  // 3 * ( 7 - 2 )
};
int totop = 0;

int calculate(int theta, int a, int b)
{
	if (theta == ADD) {
		return a + b;
	} else if (theta == SUB) {
		return a - b;
	} else if (theta == MUL) {
		return a * b;
	} else if (theta == DIV) {
		if (b != 0)
			return a / b;
		else {
			puts("division by zero!");
			return 0;
		}
	}
	puts("invalid operator");
	return 0;
}

void eval()
{
	operator[optop++] = EOL;
	Token t = token[totop++];
	while (optop) {
		if (t.type == NUM) {
			operand[numtop++] = t.val;
			t = token[totop++];
		} else {
			Precede prec = precede[operator[optop-1]][t.val];
			if (prec == '<') {
				operator[optop++] = t.val;
				t = token[totop++];
			} else if (prec == '=') {
				--optop;
				t = token[totop++];
			} else if (prec == '>') {
				int theta = operator[--optop];
				int b = operand[--numtop];
				int a = operand[--numtop];
				int res = calculate(theta, a, b);
				operand[numtop++] = res;
			} else {
				puts("error");
				return;
			}
		}
	}
	printf("%d\n", operand[numtop-1]);
}

int main()
{
	eval();
	return 0;
}
