#include <cs50.h>
#include <stdio.h>
#include <math.h>

int get_num_digits(long n);
int get_digit(long n, int i);
bool checksum(long n);
int main(void)
{
    long num = get_long("Number: ");
    int digits = get_num_digits(num);
    if (digits == 13)
    {
        if (get_digit(num, digits - 1) == 4)
        {
            if (checksum(num))
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else 
        {
            printf("INVALID\n");
        }
    }
    else if (digits == 15)
    {
        if (get_digit(num, digits - 1) == 3)
        {
            if (get_digit(num, digits - 2) == 4 || get_digit(num, digits - 2) == 7)
            {
                if (checksum(num))
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (digits == 16)
    {
        if (get_digit(num, digits - 1) == 4)
        {
            if (checksum(num))
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (get_digit(num, digits - 1) == 5)
        {
            if (get_digit(num, digits - 2) > 0 && get_digit(num, digits - 2) < 6)
            {
                if (checksum(num))
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

// performs the checksum
bool checksum(long n)
{
    int total = 0;
    // every other digit, starting from second to last
    for (int i = 1; i < get_num_digits(n); i += 2)
    {
        int x = get_digit(n, i);
        x *= 2;
        for (int j = 0; j < get_num_digits(x); j++)
        {
            total += get_digit(x, j);
        }
    }
    for (int i = 0; i < get_num_digits(n); i += 2)
    {
        int x = get_digit(n, i);
        total += x;
    }
    if (total % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// returns the number of digits
int get_num_digits(long n)
{
    int i = 0;
    while (pow(10, i) <= n)
    {
        i++;
    }
    return i;
}

// returns a particular digit 
// if h = 0, then it will return the ones place
// if h = 1 then it will return the tens place
// etc
int get_digit(long n, int h)
{
    for (int i = 0; i < h; i++)
    {
        n /= 10;
    }
    return n % 10;
}