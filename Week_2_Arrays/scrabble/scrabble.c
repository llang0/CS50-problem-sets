#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int get_points(string str);
// includes a 0 at index 27 so you can assign 0 points for throw-away chars
const int Points[27] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10, 0};
// T stands for Transposition
// subtract T from the ascii index of an uppercase character to get its
// corresponding index in Points
const int T = 65;
int main(void)
{
    // get word from player one and immediately convert to points
    int player1 = get_points(get_string("Player 1: "));
    // get word from player two and immediately convert to points
    int player2 = get_points(get_string("Player 2: "));

    if (player1 > player2)
    {
        printf("Player 1 wins!\n");
    }
    else if (player1 < player2)
    {
        printf("Player 2 wins!\n");
    }
    else 
    {
        printf("Tie!\n");
    }
}

// get points value for string
int get_points(string str)
{
    int total = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        // convert ascii value to index in Points array
        char c = toupper(str[i]);
        c -= T;

        // if c is not a letter set index to 26 (index for throw-aways)
        if (c < 0 || c > 25)
        {
            c = 26;
        }

        // get points value for corresponding index
        int value = Points[c];
        // add points value to total
        total += value;
    }
    return total;
}