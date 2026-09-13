#include <stdio.h>

__attribute__((constructor))
static void before_main(void)
{
    printf("constructor ran before main\n");
}

__attribute__((destructor))
static void after_main(void)
{
    printf("destructor ran after main\n");
}

int main(void)
{
    printf("main running\n");
    return 0;
}
