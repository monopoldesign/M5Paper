#ifndef BASIC_PARSER_H
#define BASIC_PARSER_H

// ******************************************************************************
// Includes
// ******************************************************************************
#include <Arduino.h>

// ******************************************************************************
// Prototypes
// ******************************************************************************
void ubasic_init(const char *program, M5EPD_Canvas *canvas);
void ubasic_run();
int ubasic_finished();

void line_statement();
void end_statement();
void print_statement();
void gosub_statement();
void return_statement();
void for_statement();
void next_statement();
void let_statement();
void update_statement();
void circle_statement();
void rect_statement();

void jump_linenum(int linenum);
void jump_linenum_slow(int linenum);
void statement();
int varfactor();
int factor();
int term();
int expr();
void accept(int token);
void index_add(int linenum, const char *sourcepos);
void index_free();
const char* index_find(int linenum);

void ubasic_set_variable(int varnum, int value);
int ubasic_get_variable(int varnum);

// ******************************************************************************
// Definitions
// ******************************************************************************
struct for_state
{
	int line_after_for;
	int for_variable;
	int to;
};

// ******************************************************************************
// Global Variables
// ******************************************************************************

#endif
