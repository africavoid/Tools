/* random hexadecimal program */

#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

int file_hex (int argc, char **argv)
{
	FILE *fp;
	char *line = NULL;
	size_t line_size = 0;
	ssize_t line_len;

	for (int i = 2; i < argc; i++) {
		if ((fp = fopen(argv[i], "r")) == NULL)
			return -1;
		while ((line_len = getline(&line, &line_size, fp)) != -1)
			fprintf(stdout, "0x%X\n", &line);
		fclose(fp);
	}

	free(line);
	return 0;
}

/* prints x amount of random hexadecimal numbers */
void random_hex (int x)
{
	int rand_num;
	for (int i = 0; i < x; i++) { 
		rand_num = rand();
		fprintf(stdout, "0x%X\n", rand_num);
	}
}

int main (int argc, char **argv)
{
	long num[MAX];

	if (argc < 2) {
		fprintf(stderr, "No args supplied!\n");
		return EXIT_FAILURE;
	}
	
	if (argv[1][0] == '-' && argv[1][1] == 'f' && argc >= 3) {
		if ((file_hex(argc, argv)) == -1)
			return EXIT_FAILURE;
	} else if (argv[1][0] == '-' && argv[1][1] == 'r' && argc >= 3) {
		random_hex(atoi(argv[2]));
	} else {
		for (int i = 1; i < argc; i++) {
			num[i-1] = atoi(argv[i]);
			fprintf(stdout, "0x%X\n",  num[i-1]);
		}
	}
	
	return EXIT_SUCCESS;
}
