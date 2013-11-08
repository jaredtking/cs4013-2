#include "check_token.h"

START_TEST (test_reserved_words)
{
	ReservedWord *word = tokenize_reserved_word_str ("program\t1000\t0");

	ck_assert(strcmp(word->name, "program") == 0);
	ck_assert(word->token != NULL);
	ck_assert(word->token->type == TOKEN_PROGRAM);
	ck_assert(word->token->attribute == 0);

	word = tokenize_reserved_word_str ("and\t1019\t100");

	ck_assert(strcmp(word->name, "and") == 0);
	ck_assert(word->token != NULL);
	ck_assert(word->token->type == TOKEN_MULOP);
	ck_assert(word->token->attribute == 100);
}
END_TEST

START_TEST (test_get_token)
{
	// build a reserved word list
	ReservedWord *reserved_words = tokenize_reserved_word_str ("program\t1000\t0");
	ReservedWord *next = tokenize_reserved_word_str ("begin\t1007\t0");
	ReservedWord *next2 = tokenize_reserved_word_str ("while\t1013\t0");
	reserved_words->next = next;
	next->next = next2;	

	// symbol table
	SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
	symbol_table->symbol = NULL;
	symbol_table->next = NULL;

	char *str1 = "program   test #@";

	// program
	MachineResult *res = get_next_token (str1, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_PROGRAM);
	ck_assert(res->token->attribute == 0);

	// whitespace
	res = get_next_token (str1, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_WHITESPACE);
	ck_assert(res->token->attribute == 0);

	// test
	res = get_next_token (str1, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_ID);
	ck_assert(res->token->attribute == SYM_TABLE_START_ADDR);

	// whitespace
	res = get_next_token (str1, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_WHITESPACE);
	ck_assert(res->token->attribute == 0);

	// #
	res = get_next_token (str1, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_LEXERR);
	ck_assert(res->token->attribute == MACHINE_ERR_NO_MATCH);

	// @
	res = get_next_token (str1, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_LEXERR);
	ck_assert(res->token->attribute == MACHINE_ERR_NO_MATCH);

	char *str2 = "12345678901	44.444444";

	// integer
	res = get_next_token (str2, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_LEXERR);
	ck_assert(res->token->attribute == MACHINE_ERR_INT_TOO_LONG);

	// whitespace
	res = get_next_token (str2, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_WHITESPACE);
	ck_assert(res->token->attribute == 0);

	// real
	res = get_next_token (str2, reserved_words, symbol_table);
	ck_assert(res != NULL);
	ck_assert(res->token->type == TOKEN_LEXERR);
	ck_assert(res->token->attribute == MACHINE_ERR_REAL_YY_TOO_LONG);
}
END_TEST

Suite * token_suite (void)
{
	Suite *s = suite_create ("Tokens");

	/* Core test case */
	TCase *tc_core = tcase_create ("Core");
	tcase_add_test (tc_core, test_reserved_words);
	tcase_add_test (tc_core, test_get_token);
	suite_add_tcase (s, tc_core);

	return s;
}