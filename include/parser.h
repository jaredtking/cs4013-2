#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "token.h"

#define PARSER_DATA files, reserved_words, symbol_table

int parse(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
int match(TokenType t, ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);

void parse_program(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_program_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_id_list(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);
void parse_id_list_(ParserFiles *files, ReservedWord *reserved_words, SymbolTable *symbol_table);

void synerr(TokenType *expected, int len, MachineResult *found, FILE *out);

#endif