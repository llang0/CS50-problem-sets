#include <cs50.h>
#include <stdio.h>

int main(void)
{   
    // get size from user input
    int size;
    do
    {
        size = get_int("Size: ");
    } 
    while (size < 1);
    
    // loop through rows
    for (int i = size - 1; i >= 0; i--)
    {
        // loop through columns in row
        for (int j = 0; j < size; j++)
        {
            if (i > j)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }
}