/* a simple cat implementaion */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, const char **argv) {
	FILE *fp;
	char *line = NULL;
	size_t line_size = 0;
	ssize_t line_len;

	for (int i = 1; i < argc; i++) {
		if ((fp = fopen(argv[i], "r")) == NULL)
			return EXIT_FAILURE;
	
		while ((getline(&line, &line_size, fp)) != -1)
			fprintf(stdout, "%s", line);
		fclose(fp);
	}
	free(line);
	return 0; 
}
