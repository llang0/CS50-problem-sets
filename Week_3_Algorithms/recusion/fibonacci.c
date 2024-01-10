#include <cs50.h>
#include <stdio.h>

int fibonacci(int x);
int main(void)
{
    printf("Get the nth fibonacci number.\n");
    int n = get_int("n: ");
        
    printf("%i\n", fibonacci(n));
    // fibonacci(n);
}

int fibonacci(int x)
{
    if (x == 0)
    {
        // a[0] = 0;
        return 0;
    }
    else if (x == 1)
    {
        // a[0] = 0;
        // a[1] = 1; 
        return 1;
    }
    else
    {
        return fibonacci(x-1) + fibonacci(x-2);
    }
}