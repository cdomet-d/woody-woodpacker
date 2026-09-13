#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *path = getenv("PATH");
    printf("PATH = %s\n", path ? path : "(not set)");
    return 0;
}
