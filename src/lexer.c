#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "token.h"
#include "machine.h"

void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
 
    *s = 0;
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf ("usage: %s reserved_words_file source_file output_dir\n", argv[0]);
		exit(1);
	}

	printf("-- Lexer begin --\n");

	FILE *f;
	char line[200];

	/* tokenize reserved words file */

	ReservedWord *head, *curr;
	head = NULL;

	f = fopen (argv[1], "r");

	if (f == NULL)
	{
		fprintf (stderr, "Can't open reserved words file!\n");
		exit(1);
	}

	printf("Parsing reserved words file.. ");

	while (fgets (line, 200, f) != NULL)
	{
		curr = tokenize_reserved_word_str (line);
		curr->next = head;
		head = curr;
	}

	ReservedWord *reserved_words = head;
	fclose(f);

	printf("ok\n");

	/* tokenize input source file */

	f = fopen (argv[2], "r");

	if (f == NULL)
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
	FILE *lf = fopen (listing_filename, "w");

	if (lf == NULL)
	{
		fprintf (stderr, "Can't create listing file at %s!\n", listing_filename);
		exit(1);
	}

	// open tokens file for writing
	char *tokens_filename = malloc(strlen(output_dir) + 9);
	sprintf(tokens_filename, "%s/tokens", output_dir);
	FILE *tf = fopen (tokens_filename, "w");

	if (tf == NULL)
	{
		fprintf (stderr, "Can't create tokens file at %s!\n", tokens_filename);
		exit(1);
	}

	// symbol table file header
	fprintf (tf, "%-10s%-20s%-20s%s\n", "Line No.", "Lexeme", "TOKEN-TYPE", "ATTRIBUTE");

	// open symbol table file for writing
	char *symtable_filename = malloc(strlen(output_dir) + 9);
	sprintf(symtable_filename, "%s/symtable", output_dir);
	FILE *sf = fopen (symtable_filename, "w");

	if (sf == NULL)
	{
		fprintf (stderr, "Can't create symbol table file at %s!\n", symtable_filename);
		exit(1);
	}

	// symbol table file header
	fprintf (sf, "%-5s%s\n", "Loc.", "ID");

	printf("Parsing source file.. ");

	int i = 1;
	int eof = 0;

	SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
	symbol_table->symbol = NULL;
	symbol_table->next = NULL;

	while (!eof)
	{
		if (line[0] == EOF)
			eof = 1;
		else
		{
			fgets (line, 72, f);

			// remove \n character
			chomp(line);

			// output line to listing file
			if (i > 1) fprintf (lf, "\n");
			fprintf (lf, "%-8d%s", i, line);
		}

		// tokenize line
		MachineResult *t = get_next_token(line, reserved_words, symbol_table);;
		while (t != NULL)
		{
			if (t->token->type != TOKEN_WHITESPACE)
			{
				// write token to tokens file
				fprintf (tf, "%-10d%-20s%-20s%-6d(%s)\n", i, t->lexeme, token_type_to_str(t->token->type), t->token->attribute, attribute_to_str(t->token->attribute));

				// output errors to listing file
				if (t->token->type == TOKEN_LEXERR)
					fprintf (lf, "\n%-8s%-30s%s", "LEXERR", attribute_to_str(t->token->attribute), t->lexeme);
			}

			t = get_next_token(line, reserved_words, symbol_table);
		}

		i++;

		if (feof(f))
		{
			line[0] = EOF;
			line[1] = '\0';
			i--;
		}				
	}

	fclose(lf);
	fclose(tf);

	// write ids to symbol table
	SymbolTable *s = symbol_table;
	i = 0;
	while (s != NULL && s->symbol != NULL)
	{
		fprintf (sf, "%-5d%s\n", i, s->symbol);

		i++;
		s = s->next;
	}

	fclose(sf);

	printf("ok\n");

	return(0);
}