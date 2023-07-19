#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

static int ls_open (const char *arg, bool hidden)
{
	struct dirent *de;
	DIR *dr;
	
	if ((dr = opendir(arg)) == NULL) {
		fprintf(stderr, "No such directory!\n");		
		return -1;
	}
	
	while ((de = readdir(dr)) != NULL) {
		if (hidden == false && de->d_name[0] != '.')
			fprintf(stdout, "%s\n", de->d_name);
		else if (hidden == true)
			fprintf(stdout, "%s\n", de->d_name);
	}

	closedir(dr);
	return 0;
}

int main (int argc, const char **argv)
{
	bool hidden = false;
	int i = 1;

	if (argv[1] == NULL)
		ls_open(".", hidden);
	else if (argv[1][0] == '-' && argv[1][1] == 'a' && argv[1][2] == NULL) {
		hidden == true;	
		i++;
	}
	
	for (i; i < argc; i++)
 		if ((ls_open(argv[i], hidden)) != 0)
			return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
