#include "parser.h"

int parse(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	parse_program(files, reserved_words, symbol_table);

	return match(TOKEN_EOF, files, reserved_words, symbol_table);

	// TODO need way to detect if a lexerr or synerr was raised
}

int match(TokenType t, ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(files, reserved_words, symbol_table);

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
	MachineResult *tok = get_next_token(files, reserved_words, symbol_table);

	switch (tok->token->type) {
	case TOKEN_PROGRAM:
		match(TOKEN_ID, files, reserved_words, symbol_table);
		match(TOKEN_LPAREN, files, reserved_words, symbol_table);
		parse_id_list(files, reserved_words, symbol_table);
		match(TOKEN_RPAREN, files, reserved_words, symbol_table);
		match(TOKEN_SEMICOLON, files, reserved_words, symbol_table);
		parse_program_(files, reserved_words, symbol_table);
	break;
	default:
		synerr((TokenType[]){TOKEN_PROGRAM}, 1, tok, files->listing);
	break;
	}
}


void parse_program_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{

}

void parse_id_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(files, reserved_words, symbol_table);

	switch (tok->token->type) {
	case TOKEN_ID:
		parse_id_list_(files, reserved_words, symbol_table);
	break;
	default:
		synerr((TokenType[]){TOKEN_ID}, 1, tok, files->listing);
	break;
	}
}

void parse_id_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table)
{
	MachineResult *tok = get_next_token(files, reserved_words, symbol_table);

	switch (tok->token->type) {
	case TOKEN_COMMA:
		match(TOKEN_ID, files, reserved_words, symbol_table);
		parse_id_list_(files, reserved_words, symbol_table);
	break;
	case TOKEN_RPAREN:
		// TODO
		// NOP
	break;
	default:
		synerr((TokenType[]){TOKEN_COMMA,TOKEN_RPAREN}, 2, tok, files->listing);
	break;
	}
}

void synerr(TokenType *expected, int len, MachineResult *found, FILE *out)
{
	FILE *out2 = out;
	if (out == NULL)
		out2 = stderr;

	fprintf (out2, "%-8sExpecting: ", "SYNERR" );
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