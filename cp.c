/* a re-implementation of the cp command */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char **line;
	long max_line;
}CONTENTS;

/* copys file and prints to new file */
static void copy_file (CONTENTS *c, const char **argv, long size)
{
	FILE *fp = fopen(argv[1], "r");
	char *line = NULL;
	size_t line_size = 0;
	ssize_t line_len;
	
	for (int i = 0; (line_len = getline(&line, &line_size, fp)) != -1; i++) {
		c->line[i] = strndup(line, size);
		c->max_line = i;
	}

	fclose(fp);	
	free(line);
}

/* creates a copy of the file passed at the cmd line */
static void create_copy (CONTENTS *c, const char **argv)
{
	FILE *fp = fopen(argv[2], "wb");
	
	for (int i = 0; i < c->max_line; i++)
		fprintf(fp, "%s", c->line[i]);
}

/* gets size of a file for memory */
static int get_size (const char **argv)
{
	FILE *fp;
	long size = 0;

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "No such file or directory!\n"); 
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	size = size + ftell(fp);
	fclose(fp);
	return size;
}

int main (int argc, const char **argv)
{
	long size = 0;
	if ((size = get_size(argv)) == -1)
		return EXIT_FAILURE;

	CONTENTS *c = malloc(sizeof(*c) + size);
	c->line = malloc(size);
	copy_file(c, argv, size);
	create_copy(c, argv);
	free(c);
	return EXIT_SUCCESS;
}
