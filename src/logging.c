#include "woody.h"

bool _perror(const char *error)
{
	fprintf(stderr, "%s%-25s	%s%s%s",RED, "ERROR", RESET, error, "\n");
	return false;
}

bool _psuccess(const char *mess)
{
	fprintf(stderr, "%s%-25s	%s%s%s", GREEN, "SUCCESS", RESET, mess, "\n");
	return true;
}

void _plog(const char *mess)
{
	fprintf(stderr, "%s%-25s	%s%s%s", INFO, "INFO", RESET, mess, "\n");
}