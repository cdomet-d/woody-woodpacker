#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *buf = malloc(64);
    if (!buf)
        return 1;
    strcpy(buf, "allocated on the heap");
    printf("%s\n", buf);
    free(buf);
    return 0;
}
