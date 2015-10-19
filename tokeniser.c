#include "tokeniser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* nextToken;
char* currentToken;

void tokeniser_setup(char* line) {
	nextToken = strtok(line, " \n");
	currentToken = NULL;
}

typedef struct {
	char* command;
	int token;
} CommandInfo;
CommandInfo commands[] = {
	{ "print", TOK_PRINT }
};

int tokeniser_next() {
	currentToken = nextToken;
	if (currentToken == NULL) {
		return TOK_EOL;
	}
	if (currentToken[0] == '"') {
		int len = strlen(currentToken);
		if (currentToken[len-1] == ',') {
			nextToken = &currentToken[len-1];
			len--;
		} else {
			nextToken = strtok(NULL, " \n");
		}
		if (len < 2 || currentToken[len-1] != '"') {
			printf("expected '\"' not %s\n", &currentToken[len-1]);
			return TOK_ERR;
		}
		currentToken[len-1] = '\0';
		currentToken++;
		return TOK_STRING;
	}
	nextToken = strtok(NULL, " \n");
	if (currentToken[0] >= '0' && currentToken[0] <= '9') {
		return TOK_NUM;
	} 
	switch (currentToken[0] ) {
		case '=':
			return TOK_EQ;
		case '>':
			return TOK_GT;
		case '<':
			return TOK_LT;
		case '+':
			return TOK_ADD;
		case '-':
			return TOK_SUB;
		case '/':
			return TOK_DIV;
		case '*':
			return TOK_MUL;
		case ',':
			return TOK_COMMA;
	}
	for (int i = 0; i<(sizeof(commands)/sizeof(commands[0])); i++) {
		if (strcmp(commands[i].command, currentToken) == 0) {
			return commands[i].token;
		}
	}
	if (currentToken[0] >= 'a' && currentToken[0] <= 'z') {
		return TOK_VAR;
	}

	printf("unknown syntax: %s\n", currentToken);
	return TOK_ERR;
}

int tokeniser_number() {
	return atoi(currentToken);
}

char* tokeniser_string() {
	return currentToken;
}
