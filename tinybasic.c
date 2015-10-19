
#include "tokeniser.h"
#include "variable.h"
#include <stdio.h>
#include <string.h>

int token;

int parsefactor() {
	if (token == TOK_NUM) {
		int lhs = tokeniser_number();
		token = tokeniser_next();
		return lhs;
	} else if (token == TOK_VAR) {
		char* varName = tokeniser_string();
		token = tokeniser_next();
		return getvariable(varName);
	} else {
		printf("invalid expression\n");
		token = TOK_ERR;
		return 0;
	}
}

int parseterm() {
	int lhs = parsefactor();
	if (token == TOK_ERR) {
		return 0;
	}
	if (token == TOK_MUL) {
		token = tokeniser_next();
		int rhs = parsefactor();
		if (token == TOK_ERR) {
			return 0;
		}
		return lhs * rhs;
	}
	if (token == TOK_DIV) {
		token = tokeniser_next();
		int rhs = parsefactor();
		if (token == TOK_ERR) {
			return 0;
		}
		return lhs / rhs;
	}
	return lhs;
}

int parseexpression() {
	int invert = 0;
	switch (token) {
		case TOK_ADD:
			token = tokeniser_next();
			break;
		case TOK_SUB:
			token = tokeniser_next();
			invert = 1;
			break;
	}
	int lhs = parseterm();
	if (invert) {
		lhs = -lhs;
	}
	if (token == TOK_ADD) {
		token = tokeniser_next();
		int rhs = parseterm();
		if (token == TOK_ERR) {
			return 0;
		}
		return lhs + rhs;
	}
	if (token == TOK_SUB) {
		token = tokeniser_next();
		int rhs = parseterm();
		if (token == TOK_ERR) {
			return 0;
		}
		return lhs - rhs;
	}
	return lhs;
}

int returnstack[16];
int returnstacktop = 0;

int runline(char* line, int* pc) {
	tokeniser_setup(line);
	token = tokeniser_next();
	switch (token) {
		case TOK_PRINT:// STRING|EXPR (, STRING|EXPR)
			do {
				token = tokeniser_next();
				if (token == TOK_EOL || token == TOK_ERR) {
					break;
				}
				if (token == TOK_STRING) {
					printf("%s ", tokeniser_string());
					token = tokeniser_next();
				} else {
					printf("%d ", parseexpression());
				}
				if (token != TOK_COMMA && token != TOK_EOL) {
					printf("expected ','\n");
					return 0;
				}
			}
			while (token != TOK_EOL && token != TOK_ERR);
			printf("\n");
			break;
		case TOK_LET:// VAR = EXPR
			token = tokeniser_next();
			if (token != TOK_VAR) {
				printf("expected variable name\n");
				return 0;
			}
			char* varName = tokeniser_string();
			token = tokeniser_next();
			if (token != TOK_EQ) {
				printf("expected = after variable name\n");
				return 0;
			}
			token = tokeniser_next();
			int val = parseexpression();
			if (token == TOK_ERR) {
				return 0;
			}
			setvariable(varName, token);
			break;
	}
	(*pc)++;
	return 1;
}

char* programPointers[256];

//if/then/else, for/next, let, goto, gosub, print
void runprogram(char* program) {
	int i = 0;
	for (char* line = strtok(program, "\n"); line != NULL && i < 128; line = strtok(NULL, "\n"), i++) {
		programPointers[i] = line;
	}
	if (i >= 256) {
		printf("program too long!\n");
		return;
	}
	programPointers[i++] = NULL;

	int pc = 0;
	runline(programPointers[pc], &pc);
}