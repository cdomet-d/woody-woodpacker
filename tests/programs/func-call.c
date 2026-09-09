#include <stdio.h>

int factorial(int n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n)
{
    int a = 0, b = 1, tmp;

    for (int i = 0; i < n; i++)
    {
        tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

int main(void)
{
    for (int i = 1; i <= 5; i++)
        printf("factorial(%d) = %d\n", i, factorial(i));
    for (int i = 0; i < 5; i++)
        printf("fibonacci(%d) = %d\n", i, fibonacci(i));
    return 0;
}
