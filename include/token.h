#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <string.h>

typedef enum TokenType
{
	TOKEN_WHITESPACE,
	TOKEN_ID,
	TOKEN_PROGRAM,
	TOKEN_VAR,
	TOKEN_ARRAY,
	TOKEN_OF,
	TOKEN_INTEGER,
	TOKEN_REAL,
	TOKEN_PROCEDURE,
	TOKEN_FUNCTION,
	TOKEN_BEGIN,
	TOKEN_END,
	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_ELSE,
	TOKEN_WHILE,
	TOKEN_DO,
	TOKEN_NOT,
	TOKEN_NUM,
	TOKEN_RELOP,
	TOKEN_MULOP,
	TOKEN_ADDOP,
	TOKEN_ASSIGNOP,
	TOKEN_SEMICOLON,
	TOKEN_COLON,
	TOKEN_COMMA,
	TOKEN_PERIOD,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_EOF,
	ERR_TOKEN_NOT_FOUND,
	TOKEN_LEXERR,
	TOKEN_EMPTY
} TokenType;

typedef struct Token
{
	TokenType type;
	int attribute;
} Token;

#define ATTRIBUTE_INT 999
#define ATTRIBUTE_REAL 998
#define ATTRIBUTE_LONGREAL 997
#define ATTRIBUTE_GE 996
#define ATTRIBUTE_LE 995
#define ATTRIBUTE_NE 994
#define ATTRIBUTE_AND 993
#define ATTRIBUTE_OR 992
#define ATTRIBUTE_DIV 991
#define ATTRIBUTE_MOD 990

#define RESERVED_WORD_DELIM "\t"

#define SYM_TABLE_START_ADDR 2000

typedef struct ReservedWord
{
	char *name;
	Token *token;
	struct ReservedWord *next;
} ReservedWord;

typedef struct SymbolTable
{
	char *symbol;
	struct SymbolTable *next;
} SymbolTable;

typedef struct MachineResult {
	char *lexeme;
	Token *token;
	char *f;
} MachineResult;

MachineResult *get_next_token (char *line, ReservedWord *reserved_words, SymbolTable *symbol_table);
ReservedWord *tokenize_reserved_word_str (char *line);
TokenType int_to_token_type (int id);
char *token_type_to_str (TokenType type);
char *attribute_to_str (int attr);

#endif