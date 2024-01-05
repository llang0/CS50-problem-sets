#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int size;
    do
    {
        size = get_int("Size: ");
    } while (size < 1 || size > 8);

    for (int i = size - 1; i >= 0; i--)
    {
        for (int j = 0; j < size; j++)
        {
            if (j < i)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("  ");
        for (int j = size - 1; j >= 0; j--)
        {
            if (j >= i)
            {
                printf("#");
            }
        }
        printf("\n");
    }
}