#ifndef TOKENISER_H
#define TOKENISER_H

enum {
	TOK_ERR,
	TOK_EOL,
	TOK_PRINT,
	TOK_IF,
	TOK_THEN,
	TOK_GOTO,
	TOK_RETURN,
	TOK_CLEAR,
	TOK_LIST,
	TOK_RUN,
	TOK_END,
	TOK_LET,
	TOK_VAR,
	TOK_NUM,
	TOK_STRING,
	TOK_COMMA,
	TOK_GT,
	TOK_LT,
	TOK_EQ,
	TOK_ADD,
	TOK_SUB,
	TOK_MUL,
	TOK_DIV
};

void tokeniser_setup(char* line);
int tokeniser_next();
int tokeniser_number();
char* tokeniser_string();

#endif