#include "woody.h"

bool _perror(char *error)
{
	fprintf(stderr, "%s%10s%s%s%s",RED, "[ ERROR ]	", RESET, error, "\n");
	return false;
}

bool _psuccess(char *mess)
{
	fprintf(stderr, "%s%10s%s%s%s", GREEN, "[ SUCCESS ]	", RESET, mess, "\n");
	return true;
}

void _plog(char *mess)
{
	fprintf(stderr, "%s%10s%s%s%s", INFO, "[ INFO ]	", RESET, mess, "\n");
}