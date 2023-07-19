/* word count improved aka wim */
/* TODO fix the word counting part */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 1024

/*
	wc = word count
	cc = character count
	tc = tab count
	fc = form feed count
	nc = newline count
	vc = vertical tab count
	bc = blank count
	lc = line count
	rc = return carrige count
*/
typedef struct {
	long wc;
	long cc;
	long tc;
	long fc;
	long nc;
	long vc;
	long bc;
	long lc;
	long rc;
	char *buffer[MAX];
}DATA;

/* Program stuff like files */
typedef struct {
	FILE *stream;
	char *file;
	char *line;
	size_t line_size;
	ssize_t line_len;
}PROG;

/*
	cmd line args
	0 is used to represent null
	1 is used to represent true
	w - word count
	n - newline
	v - vertical tab
*/
typedef struct {
	int w;
	int n;
	int v; 
}ARGS;

static int file_open (PROG *p)
{
	if ((p->stream = fopen(p->file, "r")) == NULL)
		return 1;

	return 0;
}

static void cleanup (PROG *p, DATA *d)
{
	free(p);
	free(d);
}

/*
	gets passed line as arg
	scans char by char from the buffer
	checks what the char in buffer is
	increments accordingly
*/
static void get_count (DATA *d, int line)
{
	int c;
	for (int i = 0; d->buffer[line][i] != '\0'; i++) {
		switch(d->buffer[line][i]) {
		case ' ':
			d->bc++;	
		break;
		case '\t':
			d->tc++;
		break;
		case '\v':
			d->vc++;	
		break;
		case '\n':
			d->nc++;
		break;
		case '\f':
			d->fc++;
		break;
		case '\r':
			d->rc++;
		break;
		default:
                        for (int j = i; (c = isspace(d->buffer[line][j+1])) == 0; j++)
                                ;
                        d->cc++;
		break;
		}
	}
}

/*
	reads the file, splits it into lines,
	puts them lines into buffer
	exits
*/
static void file_read (PROG *p, DATA *d)
{
	p->line = NULL;
	p->line_size = 0;

	for (int i = 0; (p->line_len = getline(&p->line, &p->line_size, p->stream)) != -1; i++) {
		d->buffer[i] = strndup(p->line, MAX);	
		d->lc++;
	}

	fclose(p->stream);
	free(p->line);
}

/* 	
	gets the size of a file 
	in order to allocate enough memory
*/
static int get_size (int argc, char **argv)
{
	FILE *fp;
	long size = 0;
	
	for (int i = 1; i < argc; i++) {
		if ((fp = fopen(argv[i], "r")) == NULL) {
			fclose(fp);
			return -1;
		}

		fseek(fp, 0L, SEEK_END);	
		size = size + ftell(fp);
		fclose(fp);
	}

	return size;
}

static void print_results (PROG *p, DATA *d)
{
	printf("%s:\n", p->file);
	printf("Words = %ld\nCharacters = %ld\nLines = %ld\nNew Line's = %ld\nTabs = %ld\nVertical Tabs = %ld\nBlanks = %ld\n\a",
		d->wc, d->cc, d->lc, d->nc, d->tc, d->vc, d->bc);
}

static int get_args (int argc, char **argv)
{
	/* no args */
	if (argv[1][0] != '-')
		return 0;

	if (argc < 3)
		return -1;

	ARGS *a = malloc(sizeof(*a));
        /* TODO not the best system but somewhat works for now */	
	for (int i = 1; i < argc; i++) {
                switch (argv[i][0]) {
                case '-':
		        switch (argv[i][1]) {
		        case 'w':
		        	a->w = 1;
		        break;
		        case 'n':
		        	a->n = 1;
	        	break;
	        	default:
	        		return -1;
                        break;
                break;
                        }
       
                }
        }
        return 1;
}

int main (int argc, char **argv)
{
	long size;
	int args;

	if (argc < 2) {
		printf("Nothing to do ...\n");
		return 0;
	}
		
	if ((args = get_args(argc, argv)) == -1) {
		printf("No such arg\n");
		return 1;
	}

	if ((size = get_size(argc, argv)) == -1) {
		printf("No such file or directory\n");
		return 1;
	}

	DATA *d = malloc(sizeof(*d) + size);
	PROG *p = malloc(sizeof(*p) + size);
	
	p->file = strndup(argv[1], MAX);

	file_open(p);	
	file_read(p, d);

	for (int i = 0; i < d->lc; i++)
		get_count(d, i);

	print_results(p, d);
	cleanup(p, d);
	return 0;
}
