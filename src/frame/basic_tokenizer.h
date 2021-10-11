#ifndef BASIC_TOKENIZER_H
#define BASIC_TOKENIZER_H

// ******************************************************************************
// Includes
// ******************************************************************************

// ******************************************************************************
// Prototypes
// ******************************************************************************
void tokenizer_init(const char *program);
int tokenizer_num();
int tokenizer_variable_num();
const char* tokenizer_pos();
void tokenizer_goto(const char *program);
int tokenizer_token();
void tokenizer_next();
void tokenizer_string(char *dest, int len);
int tokenizer_finished();

// ******************************************************************************
// Definitions
// ******************************************************************************
enum Token
{
	TOKENIZER_ERROR,
	TOKENIZER_ENDOFINPUT,
	TOKENIZER_NUMBER,
	TOKENIZER_STRING,
	TOKENIZER_VARIABLE,
	TOKENIZER_LET,
	TOKENIZER_PRINT,
	TOKENIZER_IF,
	TOKENIZER_THEN,
	TOKENIZER_ELSE,
	TOKENIZER_FOR,
	TOKENIZER_TO,
	TOKENIZER_STEP,
	TOKENIZER_NEXT,
	TOKENIZER_GOTO,
	TOKENIZER_GOSUB,
	TOKENIZER_RETURN,
	TOKENIZER_CALL,
	TOKENIZER_REM,
	TOKENIZER_UPDATE,
	TOKENIZER_CIRCLE,
	TOKENIZER_RECT,
	TOKENIZER_RANDOM,
	TOKENIZER_END,
	TOKENIZER_COMMA,
	TOKENIZER_SEMICOLON,
	TOKENIZER_PLUS,
	TOKENIZER_MINUS,
	TOKENIZER_AND,
	TOKENIZER_OR,
	TOKENIZER_ASTR,
	TOKENIZER_SLASH,
	TOKENIZER_MOD,
	TOKENIZER_HASH,
	TOKENIZER_LEFTPAREN,
	TOKENIZER_RIGHTPAREN,
	TOKENIZER_LT,
	TOKENIZER_GT,
	TOKENIZER_EQ,
	TOKENIZER_CR
};

// ******************************************************************************
// Global Variables
// ******************************************************************************

#endif
