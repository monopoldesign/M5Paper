#ifndef BASIC_PARSER_H
#define BASIC_PARSER_H

// ******************************************************************************
// Includes
// ******************************************************************************
#include <Arduino.h>
#include "../epdgui/epdgui.h"

// ******************************************************************************
// Prototypes
// ******************************************************************************
void ubasic_init(const char *program, M5EPD_Canvas *canvas);
void ubasic_run();
int ubasic_finished();
void line_statement();
void ubasic_end();
void statement(bool action);

void end_statement(bool action);
void print_statement(bool action);

void gosub_statement(bool action);
void return_statement(bool action);

void for_statement(bool action);
void next_statement(bool action);

void if_statement(bool action);
void then_statement();
void else_statement();

void let_statement(bool action);
void update_statement(bool action);
void circle_statement(bool action);
void rect_statement(bool action);
void button_statement(bool action);

void jump_linenum(int linenum);
void jump_linenum_slow(int linenum);

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
void ubasic_set_stringvar(int varnum, String value);
String ubasic_get_stringvar(int varnum);

void button_pressed(epdgui_args_vector_t &args);

// ******************************************************************************
// Definitions
// ******************************************************************************
struct for_state
{
	int line_after_for;
	int for_variable;
	int to;
	int step;
};

// ******************************************************************************
// Global Variables
// ******************************************************************************
extern EPDGUI_Button *_button;

#endif
