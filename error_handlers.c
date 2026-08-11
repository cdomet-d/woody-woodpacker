#include "woody.h"

void print_error(char *error) 
{
	fprintf(stderr, "%s %s%s", "ERROR: ", error, "\n");
}