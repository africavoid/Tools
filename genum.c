/* test for rand */
/* CPU benchmark possibly ? */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	long random_number, loopc;
	
	if (argc < 2 || argc > 2) {
		printf("ERROR: Too many/little arguments supplied!\nNeeds 1 argument of type int\n");
		return 1;
	}
	
	for (int i = 1; i < argc; i++)
		loopc = atoi(argv[i]);

	for (long i = 0; i < loopc; i++) {
		random_number = rand();
		printf("%d\t", random_number); 
	}
	
	printf("\nDONE!\n\a");
	return 0;
}
