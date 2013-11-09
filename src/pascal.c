#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "parser.h"

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf (stdout, "usage: %s reserved_words_file source_file output_dir\n", argv[0]);
		exit(1);
	}

	fprintf(stdout, "-- Lexer begin --\n");

	FILE *f;

	/* tokenize reserved words file */

	char line[200];
	ReservedWord *head, *curr;
	head = NULL;

	f = fopen (argv[1], "r");

	if (f == NULL)
	{
		fprintf (stderr, "Can't open reserved words file!\n");
		exit(1);
	}

	fprintf(stdout, "Parsing reserved words file.. ");

	while (fgets (line, 200, f) != NULL)
	{
		curr = tokenize_reserved_word_str (line);
		curr->next = head;
		head = curr;
	}

	ReservedWord *reserved_words = head;
	fclose(f);

	fprintf(stdout,"ok\n");

	ParserFiles *files = (ParserFiles *)malloc(sizeof(ParserFiles));

	/* tokenize input source file */

	files->source = fopen (argv[2], "r");

	if (files->source == NULL)
	{
		fprintf (stderr, "Can't open source file!\n");
		exit(1);
	}

	char *output_dir = malloc(strlen(argv[3]));
	strcpy (output_dir, argv[3]);

	// ensure the output directory is present  
	struct stat fileStat;
	if (stat(output_dir, &fileStat) < 0)
	{
		mode_t process_mask = umask(0);
		mkdir(output_dir, S_IRWXU | S_IRWXG | S_IRWXO);
		umask(process_mask);

		if (stat(output_dir, &fileStat) < 0)
		{
			fprintf (stderr, "Output directory does not exist!\n");
			exit(1);
		}
	}

	// strip trailing "/" from output dir
	if (strcmp (&output_dir[strlen(output_dir)-1], "\\") == 0)
		output_dir[strlen(output_dir)-1] = 0;

	// open listing file for writing
	char *listing_filename = malloc(strlen(output_dir) + 9);
	sprintf(listing_filename, "%s/listing", output_dir);
	files->listing = fopen (listing_filename, "w");

	if (files->listing == NULL)
	{
		fprintf (stderr, "Can't create listing file at %s!\n", listing_filename);
		exit(1);
	}

	// open tokens file for writing
	char *tokens_filename = malloc(strlen(output_dir) + 9);
	sprintf(tokens_filename, "%s/tokens", output_dir);
	files->tokens = fopen (tokens_filename, "w");

	if (files->tokens == NULL)
	{
		fprintf (stderr, "Can't create tokens file at %s!\n", tokens_filename);
		exit(1);
	}

	// token file header
	fprintf (files->tokens, "%-10s%-20s%-20s%s\n", "Line No.", "Lexeme", "TOKEN-TYPE", "ATTRIBUTE");

	// initalize symbol table
	SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
	symbol_table->symbol = NULL;
	symbol_table->next = NULL;

	fprintf(stdout, "Parsing source file..\n");

	int result = parse(files, reserved_words, symbol_table);

	fclose(files->source);
	fclose(files->listing);
	fclose(files->tokens);

	// open symbol table file for writing
	char *symtable_filename = malloc(strlen(output_dir) + 9);
	sprintf(symtable_filename, "%s/symtable", output_dir);
	f = fopen (symtable_filename, "w");

	if (f == NULL)
	{
		fprintf (stderr, "Can't create symbol table file at %s!\n", symtable_filename);
		exit(1);
	}

	// symbol table file header
	fprintf (f, "%-5s%s\n", "Loc.", "ID");

	// write ids to symbol table
	SymbolTable *s = symbol_table;
	int i = 0;
	while (s != NULL && s->symbol != NULL)
	{
		fprintf (f, "%-5d%s\n", i, s->symbol);

		i++;
		s = s->next;
	}

	fclose(f);

	if (result == 0)
		fprintf(stdout, "ok\n");

	return(0);
}