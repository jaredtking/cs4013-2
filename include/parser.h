#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "token.h"

int parse(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
int match(TokenType t, ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);

void parse_program(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_program_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_program__(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_id_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_id_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_declarations(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_declarations_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_type(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_std_type(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_subprogram_declarations(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_subprogram_declarations_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_subprogram_declaration(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_subprogram_declaration_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_subprogram_declaration__(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_subprogram_head(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_subprogram_head_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_arguments(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_param_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_param_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_compound_statement(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_compound_statement_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_optional_statements(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_statement_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_statement_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_statement(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_statement_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_var(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_var_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_expr_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_expr_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_expr(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_expr_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_simple_expr(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_simple_expr_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_term(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_term_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_factor(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_factor_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_sign(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);

void synerr(TokenType *expected, int len, MachineResult *found, FILE *out);

#endif