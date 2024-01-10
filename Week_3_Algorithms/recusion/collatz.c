#include <cs50.h>
#include <stdio.h>

int collatz(int x, int c);
int main(void)
{
    int n = get_int("n: ");
    int counter = 0;

    collatz(n, counter);

}

int collatz(int x, int c)
{
    // base case
    if (x==1)
    {
        printf("\n");
        printf("%i steps\n", c);
        // printf("%i\n", c);
        return 1;
    }
    //recursive case 1, even number
    else if (x%2 == 0)
    {
        c++;
        printf("%i ", x);
        return collatz(x/2, c);
    }
    else
    {
        c++;
        printf("%i ", x);
        return collatz(3*x + 1, c);
    }
}
