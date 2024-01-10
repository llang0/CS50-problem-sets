/// user recursopm to create a factorial function
#include <cs50.h>
#include <stdio.h>

int factorial(int x);
int main(void)
{
    int n = get_int("n: ");

    printf("%i\n", factorial(n));
}

int factorial(int x)
{
    if (x==1)
    {
        return 1;
    }
    else
    {
        return x * factorial(x-1);
    }
}