#include "parser.h"

int parse(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	parse_program(PARSER_DATA);

	return match(TOKEN_EOF, PARSER_DATA);

	// TODO need way to detect if a lexerr or synerr was raised
}

int match(TokenType t, ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NONE);

	if (tok->token->type == t) {
		if (t == TOKEN_EOF)
			return RESULT_OK;
	} else {
		synerr((TokenType[]){t}, 1, tok, files->listing);
		return RESULT_SYNERR;
	}

	return RESULT_OK;
}

void parse_program(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_PROGRAM:
		match(TOKEN_PROGRAM, PARSER_DATA);
		match(TOKEN_ID, PARSER_DATA);
		match(TOKEN_LPAREN, PARSER_DATA);
		parse_id_list(PARSER_DATA);
		match(TOKEN_RPAREN, PARSER_DATA);
		match(TOKEN_SEMICOLON, PARSER_DATA);
		parse_program_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_PROGRAM}, 1, tok, files->listing);
	break;
	}
}

void parse_program_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_VAR:
		parse_declarations(PARSER_DATA);
		parse_program__(PARSER_DATA);
	break;
	case TOKEN_FUNCTION:
		parse_subprogram_declarations(PARSER_DATA);
		parse_compound_statement(PARSER_DATA);
		match(TOKEN_PERIOD, PARSER_DATA);
	break;
	case TOKEN_BEGIN:
		parse_compound_statement(PARSER_DATA);
		match(TOKEN_PERIOD, PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_VAR,TOKEN_FUNCTION,TOKEN_BEGIN}, 3, tok, files->listing);
	break;
	}
}

void parse_program__(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_FUNCTION:
		parse_subprogram_declarations(PARSER_DATA);
		parse_compound_statement(PARSER_DATA);
		match(TOKEN_PERIOD, PARSER_DATA);
	break;
	case TOKEN_BEGIN:
		parse_compound_statement(PARSER_DATA);
		match(TOKEN_PERIOD, PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_FUNCTION,TOKEN_BEGIN}, 2, tok, files->listing);
	break;
	}
}

void parse_id_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
		match(TOKEN_ID, PARSER_DATA);
		parse_id_list_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID}, 1, tok, files->listing);
	break;
	}
}

void parse_id_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_COMMA:
		match(TOKEN_COMMA, PARSER_DATA);
		match(TOKEN_ID, PARSER_DATA);
		parse_id_list_(PARSER_DATA);
	break;
	case TOKEN_RPAREN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_COMMA,TOKEN_RPAREN}, 2, tok, files->listing);
	break;
	}
}

void parse_declarations(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_VAR:
		match(TOKEN_VAR, PARSER_DATA);
		match(TOKEN_ID, PARSER_DATA);
		match(TOKEN_COLON, PARSER_DATA);
		parse_type(PARSER_DATA);
		match(TOKEN_SEMICOLON, PARSER_DATA);
		parse_declarations_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_VAR}, 1, tok, files->listing);
	break;
	}
}

void parse_declarations_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_VAR:
		match(TOKEN_VAR, PARSER_DATA);
		match(TOKEN_ID, PARSER_DATA);
		match(TOKEN_COLON, PARSER_DATA);
		parse_type(PARSER_DATA);
		match(TOKEN_SEMICOLON, PARSER_DATA);
		parse_declarations_(PARSER_DATA);
	break;
	case TOKEN_FUNCTION:
	case TOKEN_BEGIN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_VAR, TOKEN_FUNCTION, TOKEN_BEGIN}, 3, tok, files->listing);
	break;
	}
}

void parse_type(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ARRAY:
		match(TOKEN_ARRAY, PARSER_DATA);
		match(TOKEN_LBRACKET, PARSER_DATA);
		match(TOKEN_NUM, PARSER_DATA);
		match(TOKEN_PERIOD, PARSER_DATA);
		match(TOKEN_PERIOD, PARSER_DATA);
		match(TOKEN_RBRACKET, PARSER_DATA);
		match(TOKEN_NUM, PARSER_DATA);
		match(TOKEN_OF, PARSER_DATA);
	case TOKEN_INTEGER:
	case TOKEN_REAL:
		parse_std_type(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_INTEGER, TOKEN_REAL, TOKEN_ARRAY}, 3, tok, files->listing);
	break;
	}
}

void parse_std_type(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_INTEGER:
		match(TOKEN_INTEGER, PARSER_DATA);
	break;
	case TOKEN_REAL:
		match(TOKEN_REAL, PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_INTEGER, TOKEN_REAL}, 2, tok, files->listing);
	break;
	}
}

void parse_subprogram_declarations(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_FUNCTION:
		parse_subprogram_declaration(PARSER_DATA);
		match(TOKEN_SEMICOLON, PARSER_DATA);
		parse_subprogram_declarations_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_FUNCTION}, 1, tok, files->listing);
	break;
	}
}

void parse_subprogram_declarations_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_FUNCTION:
		parse_subprogram_declaration(PARSER_DATA);
		match(TOKEN_SEMICOLON, PARSER_DATA);
		parse_subprogram_declarations_(PARSER_DATA);
	break;
	case TOKEN_BEGIN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_FUNCTION,TOKEN_BEGIN}, 2, tok, files->listing);
	break;
	}
}

void parse_subprogram_declaration(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_FUNCTION:
		parse_subprogram_head(PARSER_DATA);
		parse_subprogram_declaration_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_FUNCTION}, 1, tok, files->listing);
	break;
	}
}

void parse_subprogram_declaration_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_VAR:
		parse_declarations(PARSER_DATA);
		parse_subprogram_declaration__(PARSER_DATA);
	break;
	case TOKEN_FUNCTION:
		parse_subprogram_declarations(PARSER_DATA);
		parse_compound_statement(PARSER_DATA);
	break;
	case TOKEN_BEGIN:
		parse_compound_statement(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_VAR,TOKEN_FUNCTION,TOKEN_BEGIN}, 3, tok, files->listing);
	break;
	}
}

void parse_subprogram_declaration__(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_FUNCTION:
		parse_subprogram_declarations(PARSER_DATA);
		parse_compound_statement(PARSER_DATA);
	break;
	case TOKEN_BEGIN:
		parse_compound_statement(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_FUNCTION,TOKEN_BEGIN}, 2, tok, files->listing);
	break;
	}
}

void parse_subprogram_head(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_FUNCTION:
		match(TOKEN_FUNCTION, PARSER_DATA);
		match(TOKEN_ID, PARSER_DATA);
		parse_subprogram_head_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_FUNCTION}, 1, tok, files->listing);
	break;
	}
}

void parse_subprogram_head_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_LPAREN:
		parse_arguments(PARSER_DATA);
		match(TOKEN_COLON, PARSER_DATA);
		parse_std_type(PARSER_DATA);
		match(TOKEN_SEMICOLON, PARSER_DATA);
	break;
	case TOKEN_COLON:
		match(TOKEN_COLON, PARSER_DATA);
		parse_std_type(PARSER_DATA);
		match(TOKEN_SEMICOLON, PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_LPAREN,TOKEN_COLON}, 2, tok, files->listing);
	break;
	}
}

void parse_arguments(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_LPAREN:
		match(TOKEN_LPAREN, PARSER_DATA);
		parse_param_list(PARSER_DATA);
		match(TOKEN_RPAREN, PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_LPAREN}, 1, tok, files->listing);
	break;
	}
}

void parse_param_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
		match(TOKEN_ID, PARSER_DATA);
		match(TOKEN_COLON, PARSER_DATA);
		parse_type(PARSER_DATA);
		parse_param_list_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID}, 1, tok, files->listing);
	break;
	}
}

void parse_param_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_SEMICOLON:
		match(TOKEN_SEMICOLON, PARSER_DATA);
		match(TOKEN_ID, PARSER_DATA);
		match(TOKEN_COLON, PARSER_DATA);
		parse_type(PARSER_DATA);
		parse_param_list_(PARSER_DATA);
	break;
	case TOKEN_RPAREN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_COLON,TOKEN_RPAREN}, 2, tok, files->listing);
	break;
	}
}

void parse_compound_statement(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_BEGIN:
		match(TOKEN_BEGIN, PARSER_DATA);
		parse_compound_statement_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_BEGIN}, 1, tok, files->listing);
	break;
	}
}

void parse_compound_statement_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
	case TOKEN_BEGIN:
	case TOKEN_IF:
	case TOKEN_WHILE:
		parse_optional_statements(PARSER_DATA);
	case TOKEN_END:
		match(TOKEN_END, PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_BEGIN,TOKEN_IF,TOKEN_WHILE,TOKEN_END}, 5, tok, files->listing);
	break;
	}		
}

void parse_optional_statements(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
	case TOKEN_BEGIN:
	case TOKEN_IF:
	case TOKEN_WHILE:
		parse_statement_list(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_BEGIN,TOKEN_IF,TOKEN_WHILE}, 4, tok, files->listing);
	break;
	}
}

void parse_statement_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
	case TOKEN_BEGIN:
	case TOKEN_IF:
	case TOKEN_WHILE:
		parse_statement(PARSER_DATA);
		parse_statement_list_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_BEGIN,TOKEN_IF,TOKEN_WHILE}, 4, tok, files->listing);
	break;
	}
}

void parse_statement_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_SEMICOLON:
		match(TOKEN_SEMICOLON, PARSER_DATA);
		parse_statement(PARSER_DATA);
		parse_statement_list_(PARSER_DATA);
	break;
	case TOKEN_END:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_SEMICOLON,TOKEN_END}, 2, tok, files->listing);
	break;
	}
}

void parse_statement(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
		parse_var(PARSER_DATA);
		match(TOKEN_ASSIGNOP, PARSER_DATA);
		parse_expr(PARSER_DATA);
	break;
	case TOKEN_BEGIN:
		parse_compound_statement(PARSER_DATA);
	break;
	case TOKEN_IF:
		match(TOKEN_IF, PARSER_DATA);
		parse_expr(PARSER_DATA);
		match(TOKEN_THEN, PARSER_DATA);
		parse_statement(PARSER_DATA);
		parse_statement_(PARSER_DATA);
	break;
	case TOKEN_WHILE:
		match(TOKEN_WHILE, PARSER_DATA);
		parse_expr(PARSER_DATA);
		match(TOKEN_DO, PARSER_DATA);
		parse_statement(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_BEGIN,TOKEN_IF,TOKEN_WHILE}, 4, tok, files->listing);
	break;
	}
}

void parse_statement_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ELSE:
		tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NONE);
		if (tok->token->type == TOKEN_ELSE) {
			parse_statement(PARSER_DATA);
		} else  {
			// NOP
		}
	break;
	case TOKEN_SEMICOLON:
	case TOKEN_END:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_ELSE,TOKEN_SEMICOLON,TOKEN_END}, 4, tok, files->listing);
	break;
	}
}

void parse_var(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
		match(TOKEN_ID, PARSER_DATA);
		parse_var_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID}, 1, tok, files->listing);
	break;
	}
}

void parse_var_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_LBRACKET:
		match(TOKEN_LBRACKET, PARSER_DATA);
		parse_expr(PARSER_DATA);
		match(TOKEN_RBRACKET, PARSER_DATA);
	break;
	case TOKEN_ASSIGNOP:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_LBRACKET,TOKEN_ASSIGNOP}, 2, tok, files->listing);
	break;
	}
}

void parse_expr_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
	case TOKEN_NUM:
	case TOKEN_LPAREN:
	case TOKEN_NOT:
	case TOKEN_ADDOP:
		parse_expr(PARSER_DATA);
		parse_expr_list_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_NUM,TOKEN_LPAREN,TOKEN_NOT,TOKEN_ADDOP}, 5, tok, files->listing);
	break;
	}
}

void parse_expr_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_COMMA:
		match(TOKEN_COMMA, PARSER_DATA);
		parse_expr(PARSER_DATA);
		parse_expr_list_(PARSER_DATA);
	break;
	case TOKEN_RPAREN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_COMMA,TOKEN_RPAREN}, 2, tok, files->listing);
	break;
	}
}

void parse_expr(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
	case TOKEN_NUM:
	case TOKEN_LPAREN:
	case TOKEN_NOT:
	case TOKEN_ADDOP:
		parse_simple_expr(PARSER_DATA);
		parse_expr_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_NUM,TOKEN_LPAREN,TOKEN_NOT,TOKEN_ADDOP}, 5, tok, files->listing);
	break;
	}
}

void parse_expr_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_RELOP:
		match(TOKEN_RELOP, PARSER_DATA);
		parse_simple_expr(PARSER_DATA);
	break;
	case TOKEN_SEMICOLON:
	case TOKEN_END:
	case TOKEN_ELSE:
	case TOKEN_THEN:
	case TOKEN_DO:
	case TOKEN_RBRACKET:
	case TOKEN_COMMA:
	case TOKEN_RPAREN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_RELOP,TOKEN_SEMICOLON,TOKEN_END,TOKEN_ELSE,TOKEN_THEN,TOKEN_DO,TOKEN_RBRACKET,TOKEN_COMMA,TOKEN_RPAREN}, 9, tok, files->listing);
	break;
	}
}

void parse_simple_expr(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
	case TOKEN_NUM:
	case TOKEN_LPAREN:
	case TOKEN_NOT:
	case TOKEN_ADDOP:
		parse_term(PARSER_DATA);
		parse_simple_expr_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_NUM,TOKEN_LPAREN,TOKEN_NOT,TOKEN_ADDOP}, 5, tok, files->listing);
	break;
	}
}

void parse_simple_expr_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ADDOP:
		match(TOKEN_ADDOP, PARSER_DATA);
		parse_term(PARSER_DATA);
		parse_simple_expr_(PARSER_DATA);
	break;
	case TOKEN_RELOP:
	case TOKEN_SEMICOLON:
	case TOKEN_END:
	case TOKEN_ELSE:
	case TOKEN_THEN:
	case TOKEN_DO:
	case TOKEN_RBRACKET:
	case TOKEN_COMMA:
	case TOKEN_RPAREN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_RELOP,TOKEN_SEMICOLON,TOKEN_END,TOKEN_ELSE,TOKEN_THEN,TOKEN_DO,TOKEN_RBRACKET,TOKEN_COMMA,TOKEN_RPAREN}, 10, tok, files->listing);
	break;
	}
}

void parse_term(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
	case TOKEN_NUM:
	case TOKEN_LPAREN:
	case TOKEN_NOT:
		parse_factor(PARSER_DATA);
		parse_term_(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_NUM,TOKEN_LPAREN,TOKEN_NOT}, 4, tok, files->listing);
	break;
	}
}

void parse_term_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_MULOP:
		match(TOKEN_MULOP, PARSER_DATA);
		parse_factor(PARSER_DATA);
		parse_term_(PARSER_DATA);
	break;
	case TOKEN_ADDOP:
	case TOKEN_RELOP:
	case TOKEN_SEMICOLON:
	case TOKEN_END:
	case TOKEN_ELSE:
	case TOKEN_THEN:
	case TOKEN_DO:
	case TOKEN_RBRACKET:
	case TOKEN_COMMA:
	case TOKEN_RPAREN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_MULOP,TOKEN_ADDOP,TOKEN_RELOP,TOKEN_SEMICOLON,TOKEN_END,TOKEN_ELSE,TOKEN_THEN,TOKEN_DO,TOKEN_RBRACKET,TOKEN_COMMA,TOKEN_RPAREN}, 11, tok, files->listing);
	break;
	}	
}

void parse_factor(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ID:
		match(TOKEN_ID, PARSER_DATA);
		parse_factor_(PARSER_DATA);
	break;
	case TOKEN_NUM:
		match(TOKEN_NUM, PARSER_DATA);
	break;
	case TOKEN_LPAREN:
		match(TOKEN_LPAREN, PARSER_DATA);
		parse_expr(PARSER_DATA);
		match(TOKEN_RPAREN, PARSER_DATA);
	break;
	case TOKEN_NOT:
		match(TOKEN_NOT, PARSER_DATA);
		parse_factor(PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID,TOKEN_NUM,TOKEN_LPAREN,TOKEN_NOT}, 4, tok, files->listing);
	break;
	}
}

void parse_factor_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_LPAREN:
		match(TOKEN_LPAREN, PARSER_DATA);
		parse_expr_list(PARSER_DATA);
		match(TOKEN_RPAREN, PARSER_DATA);
	break;
	case TOKEN_LBRACKET:
		match(TOKEN_LBRACKET, PARSER_DATA);
		parse_expr(PARSER_DATA);
		match(TOKEN_RBRACKET, PARSER_DATA);
	break;
	case TOKEN_MULOP:
	case TOKEN_ADDOP:
	case TOKEN_RELOP:
	case TOKEN_SEMICOLON:
	case TOKEN_END:
	case TOKEN_ELSE:
	case TOKEN_THEN:
	case TOKEN_DO:
	case TOKEN_RBRACKET:
	case TOKEN_COMMA:
	case TOKEN_RPAREN:
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_LPAREN,TOKEN_LBRACKET,TOKEN_MULOP,TOKEN_ADDOP,TOKEN_RELOP,TOKEN_SEMICOLON,TOKEN_END,TOKEN_ELSE,TOKEN_THEN,TOKEN_DO,TOKEN_RBRACKET,TOKEN_COMMA,TOKEN_RPAREN}, 13, tok, files->listing);
	break;
	}
}

void parse_sign(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(PARSER_DATA, TOKEN_OPTION_NOP);

	switch (tok->token->type) {
	case TOKEN_ADDOP:
		match(TOKEN_ADDOP, PARSER_DATA);
	break;
	default:
		synerr((TokenType[]){TOKEN_ADDOP}, 1, tok, files->listing);
	break;
	}
}

void synerr(TokenType *expected, int len, MachineResult *found, FILE *out)
{
	FILE *out2 = out;
	if (out == NULL)
		out2 = stderr;

	fprintf (out2, "%-8sExpecting ", "SYNERR" );
	for (int i = 0; i < len; i++) {
		fprintf (out2, "\"%s\"", token_type_to_str(expected[i]));
		if (i < len - 1) {
			if (len > 2)
				fprintf (out2, ", ");
			if (i == len - 2)
				fprintf (out2, " or ");
		}
	}
	fprintf (out2, " but received \"%s\"", token_type_to_str(found->token->type));

	if (out == NULL)
		fprintf (out2, " on line %d", found->line_no);
	
	fprintf (out2, "\n");

	if (out != NULL)
		synerr(expected, len, found, NULL);
}