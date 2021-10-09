// ******************************************************************************
// Includes
// ******************************************************************************
#include <Arduino.h>
#include <M5EPD.h>

#include "basic_parser.h"
#include "basic_tokenizer.h"

// ******************************************************************************
// Definitions
// ******************************************************************************

// ******************************************************************************
// Global Variables
// ******************************************************************************
const char *program_ptr;

#define MAX_STRINGLEN 40
char string[MAX_STRINGLEN];

#define MAX_GOSUB_STACK_DEPTH 10
int16_t gosub_stack[MAX_GOSUB_STACK_DEPTH];
int16_t gosub_stack_ptr;

#define MAX_FOR_STACK_DEPTH 4
struct for_state for_stack[MAX_FOR_STACK_DEPTH];
int16_t for_stack_ptr;

struct line_index
{
	int line_number;
	const char *program_text_position;
	struct line_index *next;
};
struct line_index *line_index_head = NULL;
struct line_index *line_index_current = NULL;

#define MAX_VARNUM 26
int16_t variables[MAX_VARNUM];

uint8_t ended;
M5EPD_Canvas *canvas_basic;

uint8_t _size;
int16_t _margin_left, _margin_right, _margin_top, _margin_bottom;

// ******************************************************************************
// Functions
// ******************************************************************************

// ------------------------------------------------------------------------------
// main()
// ------------------------------------------------------------------------------
//int main(int argc, const char * argv[])
//{
//	ubasic_init(program2);
//
//	do
//	{
//		line_statement();
//	}
//	while (!ubasic_finished());
//
//	return 0;
//}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void ubasic_init(const char *program, M5EPD_Canvas *canvas)
{
	canvas_basic = canvas;

	_size = 26;
	_margin_left = 8;
	_margin_right = 8;
	_margin_top = 8;
	_margin_bottom = 8;

	canvas_basic->setTextSize(_size);
	canvas_basic->setTextArea(_margin_left, _margin_top, 520 - _margin_left - _margin_right, ((8 * 60) + 18) - _margin_top - _margin_bottom);

	program_ptr = program;
	for_stack_ptr = gosub_stack_ptr = 0;
	index_free();
	tokenizer_init(program);
	ended = 0;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void ubasic_run()
{
	line_statement();
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int ubasic_finished()
{
	if (ended || tokenizer_finished())
		return 1;

	return 0;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void line_statement()
{
	index_add(tokenizer_num(), tokenizer_pos());
	accept(TOKENIZER_NUMBER);
	statement();
	return;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void end_statement()
{
	accept(TOKENIZER_END);
	ended = 1;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void print_statement()
{
	uint8_t semi = 0;

	accept(TOKENIZER_PRINT);
	do
	{
		if (tokenizer_token() == TOKENIZER_STRING)
		{
			tokenizer_string(string, sizeof(string));
			canvas_basic->print(string);
			tokenizer_next();
		}
		else if (tokenizer_token() == TOKENIZER_COMMA)
		{
			canvas_basic->print(",");
			tokenizer_next();
		}
		else if (tokenizer_token() == TOKENIZER_SEMICOLON)
		{
			Serial.println("Semicolon found!");
			semi = 1;
			tokenizer_next();
		}
		else if (tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER)
			canvas_basic->print(expr());
		else
			break;
	}
	while(tokenizer_token() != TOKENIZER_CR && tokenizer_token() != TOKENIZER_ENDOFINPUT);

	if (!semi)
		canvas_basic->println();

	tokenizer_next();
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void gosub_statement()
{
	int linenum;
	accept(TOKENIZER_GOSUB);
	linenum = tokenizer_num();
	accept(TOKENIZER_NUMBER);
	accept(TOKENIZER_CR);
	if (gosub_stack_ptr < MAX_GOSUB_STACK_DEPTH)
	{
		gosub_stack[gosub_stack_ptr] = tokenizer_num();
		gosub_stack_ptr++;
		jump_linenum(linenum);
	}
	else
	{
		//printf("GOSUB ERROR!\n");
		Serial.println("GOSUB ERROR!");
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void return_statement()
{
	accept(TOKENIZER_RETURN);
	if (gosub_stack_ptr > 0)
	{
		gosub_stack_ptr--;
		jump_linenum(gosub_stack[gosub_stack_ptr]);
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void for_statement()
{
	int for_variable, to;

	accept(TOKENIZER_FOR);
	for_variable = tokenizer_variable_num();
	accept(TOKENIZER_VARIABLE);
	accept(TOKENIZER_EQ);
	ubasic_set_variable(for_variable, expr());
	accept(TOKENIZER_TO);
	to = expr();
	accept(TOKENIZER_CR);

	if (for_stack_ptr < MAX_FOR_STACK_DEPTH)
	{
		for_stack[for_stack_ptr].line_after_for = tokenizer_num();
		for_stack[for_stack_ptr].for_variable = for_variable;
		for_stack[for_stack_ptr].to = to;
		for_stack_ptr++;
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void next_statement()
{
	int var;

	accept(TOKENIZER_NEXT);
	var = tokenizer_variable_num();
	accept(TOKENIZER_VARIABLE);

	if (for_stack_ptr > 0 && var == for_stack[for_stack_ptr - 1].for_variable)
	{
		ubasic_set_variable(var, ubasic_get_variable(var) + 1);

		if (ubasic_get_variable(var) <= for_stack[for_stack_ptr - 1].to)
				jump_linenum(for_stack[for_stack_ptr - 1].line_after_for);
		else
		{
			for_stack_ptr--;
			accept(TOKENIZER_CR);
		}
	}
	else
		accept(TOKENIZER_CR);
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void let_statement()
{
  int var;

  var = tokenizer_variable_num();

  accept(TOKENIZER_VARIABLE);
  accept(TOKENIZER_EQ);
  ubasic_set_variable(var, expr());
  accept(TOKENIZER_CR);
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void update_statement()
{
	canvas_basic->pushCanvas(10, 90, UPDATE_MODE_GC16);
	accept(TOKENIZER_UPDATE);

	if (tokenizer_token() == TOKENIZER_CR)
		accept(TOKENIZER_CR);
	else if (tokenizer_token() == TOKENIZER_ENDOFINPUT)
		ended = 1;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void circle_statement()
{
	uint16_t x = 0, y = 0, r = 0, c = 0;

	accept(TOKENIZER_CIRCLE);

	// get x
	if (tokenizer_token() == TOKENIZER_NUMBER || tokenizer_token() == TOKENIZER_VARIABLE)
		x = expr();

	accept(TOKENIZER_COMMA);

	// get y
	if (tokenizer_token() == TOKENIZER_NUMBER || tokenizer_token() == TOKENIZER_VARIABLE)
		y = expr();

	accept(TOKENIZER_COMMA);

	// get radius
	if (tokenizer_token() == TOKENIZER_NUMBER || tokenizer_token() == TOKENIZER_VARIABLE)
		r = expr();

	accept(TOKENIZER_COMMA);

	// get color
	if (tokenizer_token() == TOKENIZER_NUMBER || tokenizer_token() == TOKENIZER_VARIABLE)
		c = expr();

	if (tokenizer_token() == TOKENIZER_CR)
		accept(TOKENIZER_CR);
	else if (tokenizer_token() == TOKENIZER_ENDOFINPUT)
		ended = 1;

	canvas_basic->drawCircle(x, y, r, c);
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void jump_linenum(int linenum)
{
	const char *pos = index_find(linenum);
	if (pos != NULL)
	{
		//printf("going to line %02d\n", linenum);
		tokenizer_goto(pos);
	}
	else
	{
		/* We'll try to find a yet-unindexed line to jump to. */
		//printf("jump_linenum: %02d Calling jump_linenum_slow.\n", linenum);
		jump_linenum_slow(linenum);
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void jump_linenum_slow(int linenum)
{
	tokenizer_init(program_ptr);
	while (tokenizer_num() != linenum)
	{
		do
		{
			do
			{
				tokenizer_next();
			}
			while (tokenizer_token() != TOKENIZER_CR && tokenizer_token() != TOKENIZER_ENDOFINPUT);

			if (tokenizer_token() == TOKENIZER_CR)
			{
				tokenizer_next();
			}
		}
		while(tokenizer_token() != TOKENIZER_NUMBER);
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void statement()
{
	int token;

	token = tokenizer_token();

	switch(token)
	{
		case TOKENIZER_PRINT:
			print_statement();
			break;
		case TOKENIZER_GOSUB:
			gosub_statement();
			break;
		case TOKENIZER_RETURN:
			return_statement();
			break;
		case TOKENIZER_FOR:
			for_statement();
			break;
		case TOKENIZER_NEXT:
			next_statement();
			break;
		case TOKENIZER_UPDATE:
			update_statement();
			break;
		case TOKENIZER_CIRCLE:
			circle_statement();
			break;
		case TOKENIZER_END:
			end_statement();
			break;
		case TOKENIZER_LET:
			accept(TOKENIZER_LET);
			/* Fall through. */
		case TOKENIZER_VARIABLE:
			let_statement();
			break;
		default:
			//printf("UNKNOWN TOKEN\n");
			Serial.println("UNKNOWN TOKEN");
			ended = 1;
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int varfactor()
{
	int r;
	r = ubasic_get_variable(tokenizer_variable_num());
	accept(TOKENIZER_VARIABLE);
	return r;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int factor()
{
	int r;

	switch(tokenizer_token())
	{
			case TOKENIZER_NUMBER:
				r = tokenizer_num();
				accept(TOKENIZER_NUMBER);
				break;
			case TOKENIZER_LEFTPAREN:
				accept(TOKENIZER_LEFTPAREN);
				r = expr();
				accept(TOKENIZER_RIGHTPAREN);
				break;
			default:
				r = varfactor();
				break;
	}
	return r;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int term()
{
	int f1, f2;
	int op;

	f1 = factor();
	op = tokenizer_token();

	while (op == TOKENIZER_ASTR || op == TOKENIZER_SLASH || op == TOKENIZER_MOD)
	{
		tokenizer_next();
		f2 = factor();

		switch (op)
		{
			case TOKENIZER_ASTR:
				f1 = f1 * f2;
				break;
			case TOKENIZER_SLASH:
				f1 = f1 / f2;
				break;
			case TOKENIZER_MOD:
				f1 = f1 % f2;
				break;
		}

		op = tokenizer_token();
	}
	return f1;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int expr()
{
	int t1, t2;
	int op;

	t1 = term();
	op = tokenizer_token();

	while (op == TOKENIZER_PLUS || op == TOKENIZER_MINUS || op == TOKENIZER_AND || op == TOKENIZER_OR)
	{
		tokenizer_next();
		t2 = term();

		switch(op)
		{
			case TOKENIZER_PLUS:
				t1 = t1 + t2;
				break;
			case TOKENIZER_MINUS:
				t1 = t1 - t2;
				break;
			case TOKENIZER_AND:
				t1 = t1 & t2;
				break;
			case TOKENIZER_OR:
				t1 = t1 | t2;
				break;
		}

		op = tokenizer_token();
	}
	return t1;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void accept(int token)
{
	if (token != tokenizer_token())
	{
		String error = "SYNTAX ERROR ON LINE" + String(line_index_current->line_number);
		canvas_basic->println(error);
		canvas_basic->pushCanvas(10, 90, UPDATE_MODE_GC16);
		ended = 1;
	}
	else
		tokenizer_next();
}


// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void index_add(int linenum, const char *sourcepos)
{
	if(line_index_head != NULL && index_find(linenum))
		return;

	struct line_index *new_lidx;

	new_lidx = (struct line_index *)malloc(sizeof(struct line_index));
	new_lidx->line_number = linenum;
	new_lidx->program_text_position = sourcepos;
	new_lidx->next = NULL;

	if(line_index_head != NULL)
	{
		line_index_current->next = new_lidx;
		line_index_current = line_index_current->next;
	}
	else
	{
		line_index_current = new_lidx;
		line_index_head = line_index_current;
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void index_free()
{
	if (line_index_head != NULL)
	{
		line_index_current = line_index_head;
		do
		{
			line_index_head = line_index_current;
			line_index_current = line_index_current->next;
			free(line_index_head);
		}
		while (line_index_current != NULL);

		line_index_head = NULL;
	}
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
const char* index_find(int linenum)
{
	struct line_index *lidx;
	lidx = line_index_head;

	while (lidx != NULL && lidx->line_number != linenum)
	{
		lidx = lidx->next;
	}
	if (lidx != NULL && lidx->line_number == linenum)
	{
		return lidx->program_text_position;
	}

	return NULL;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void ubasic_set_variable(int varnum, int value)
{
	if (varnum >= 0 && varnum <= MAX_VARNUM)
		variables[varnum] = value;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int ubasic_get_variable(int varnum)
{
	if(varnum >= 0 && varnum <= MAX_VARNUM)
		return variables[varnum];

	return 0;
}
