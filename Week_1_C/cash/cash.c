#include <cs50.h>
#include <stdio.h>
int count_coins(int value, int total);
int main(void)
{
    int change;
    do
    {
        change = get_int("Change owed: ");
    } 
    while (change < 1);
    
    int num_coins = 0;  

    // count number of quarters
    while (change >= 25)
    {
        num_coins += 1;
        change -= 25;
    }

    // count number of dimes
    while (change >= 10)
    {
        num_coins += 1;
        change -= 10;
    }

    // count number of nickels
    while (change >= 5)
    {
        num_coins += 1;
        change -= 5;
    }

    // count number of pennies
    while (change >= 1)
    {
        num_coins += 1;
        change -= 1;
    }
    printf("%i\n", num_coins);
}