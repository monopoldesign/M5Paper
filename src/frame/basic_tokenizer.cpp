//
//  tokenizer.c
//  Basic
//
//  Created by Mario Volkel on 05/07/2021.
//

// ******************************************************************************
// Includes
// ******************************************************************************
#include <Arduino.h>
#include "basic_tokenizer.h"

// ******************************************************************************
// Prototypes
// ******************************************************************************
static int get_next_token(void);
static int singlechar(void);

// ******************************************************************************
// Definitions
// ******************************************************************************
struct keyword_token
{
	const char *keyword;
	int token;
};

const struct keyword_token keywords[] =
{
	{"let", TOKENIZER_LET},
	{"print", TOKENIZER_PRINT},
	{"goto", TOKENIZER_GOTO},
	{"if", TOKENIZER_IF},
	{"then", TOKENIZER_THEN},
	{"else", TOKENIZER_ELSE},
	{"for", TOKENIZER_FOR},
	{"to", TOKENIZER_TO},
	{"step", TOKENIZER_STEP},
	{"next", TOKENIZER_NEXT},
	{"goto", TOKENIZER_GOTO},
	{"gosub", TOKENIZER_GOSUB},
	{"return", TOKENIZER_RETURN},
	{"call", TOKENIZER_CALL},
	{"rem", TOKENIZER_REM},
	{"update", TOKENIZER_UPDATE},
	{"circle", TOKENIZER_CIRCLE},
	{"rect", TOKENIZER_RECT},
	{"rnd", TOKENIZER_RANDOM},
	{"button", TOKENIZER_BUTTON},
	{"waitbutton", TOKENIZER_WAITBUTTON},
	{"but()", TOKENIZER_BUTTONID},
	{"cls", TOKENIZER_CLS},
	{"end", TOKENIZER_END},
	{NULL, TOKENIZER_ERROR}
};

#define MAX_NUMLEN 6

// ******************************************************************************
// Global Variables
// ******************************************************************************
const char *ptr, *nextptr;
int current_token = TOKENIZER_ERROR;

// ******************************************************************************
// Functions
// ******************************************************************************

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void tokenizer_init(const char *program)
{
	tokenizer_goto(program);
	current_token = get_next_token();
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void tokenizer_goto(const char *program)
{
	ptr = program;
	current_token = get_next_token();
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int tokenizer_finished()
{
	if ((*ptr == 0) || (current_token == TOKENIZER_ENDOFINPUT))
		return 1;
	else
		return 0;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int tokenizer_num()
{
	return atoi(ptr);
}


// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int tokenizer_variable_num()
{
	return *ptr - 'a';
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
const char* tokenizer_pos()
{
	return ptr;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int tokenizer_token()
{
	return current_token;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void tokenizer_next()
{
	if (tokenizer_finished())
		return;

	ptr = nextptr;

	while(*ptr == ' ')
	{
		++ptr;
	}
	current_token = get_next_token();

	if (current_token == TOKENIZER_REM)
	{
		while (!(*nextptr == '\n' || tokenizer_finished()))
		{
			++nextptr;
		}

		if (*nextptr == '\n')
			++nextptr;

		tokenizer_next();
	}
	return;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void tokenizer_string(char *dest, int len)
{
	char *string_end;
	long string_len;

	if (tokenizer_token() != TOKENIZER_STRING)
		return;

	string_end = strchr(ptr + 1, '"');

	if (string_end == NULL)
		return;

	string_len = string_end - ptr - 1;

	if (len < string_len)
		string_len = len;

	memcpy(dest, ptr + 1, string_len);
	dest[string_len] = 0;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int get_next_token()
{
	struct keyword_token const *kt;
	int i;

	if (*ptr == 0)
		return TOKENIZER_ENDOFINPUT;

	if (isdigit(*ptr))
	{
		for (i = 0; i < MAX_NUMLEN; ++i)
		{
			if(!isdigit(ptr[i]))
			{
				if (i > 0)
				{
					nextptr = ptr + i;
					return TOKENIZER_NUMBER;
				}
				else
					return TOKENIZER_ERROR;
			}

			if (!isdigit(ptr[i]))
				return TOKENIZER_ERROR;
		}

		return TOKENIZER_ERROR;

	}
	else if (singlechar())
	{
		nextptr = ptr + 1;
		return singlechar();
	}
	else if(*ptr == '"')
	{
		nextptr = ptr;
		do
		{
			++nextptr;
		}
		while (*nextptr != '"');

		++nextptr;
		return TOKENIZER_STRING;
	}
	else
	{
		for (kt = keywords; kt->keyword != NULL; ++kt)
		{
			if (strncmp(ptr, kt->keyword, strlen(kt->keyword)) == 0)
			{
				nextptr = ptr + strlen(kt->keyword);
				return kt->token;
			}
		}
	}

	if (*ptr >= 'a' && *ptr <= 'z')
	{
		nextptr = ptr + 1;
		return TOKENIZER_VARIABLE;
	}

	return TOKENIZER_ERROR;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
int singlechar()
{
	if(*ptr == '\n')
		return TOKENIZER_CR;
	else if (*ptr == ',')
		return TOKENIZER_COMMA;
	else if (*ptr == ';')
		return TOKENIZER_SEMICOLON;
	else if (*ptr == '+')
		return TOKENIZER_PLUS;
	else if (*ptr == '-')
		return TOKENIZER_MINUS;
	else if (*ptr == '&')
		return TOKENIZER_AND;
	else if (*ptr == '|')
		return TOKENIZER_OR;
	else if (*ptr == '*')
		return TOKENIZER_ASTR;
	else if (*ptr == '/')
		return TOKENIZER_SLASH;
	else if (*ptr == '%')
		return TOKENIZER_MOD;
	else if (*ptr == '(')
		return TOKENIZER_LEFTPAREN;
	else if (*ptr == '#')
		return TOKENIZER_HASH;
	else if (*ptr == ')')
		return TOKENIZER_RIGHTPAREN;
	else if (*ptr == '<')
		return TOKENIZER_LT;
	else if (*ptr == '>')
		return TOKENIZER_GT;
	else if (*ptr == '=')
		return TOKENIZER_EQ;
	else if (*ptr == '$')
		return TOKENIZER_DOLLAR;

	return TOKENIZER_ERROR;
}
