#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool only_digits(string);
char rotate(char p, int k);
int main(int argc, string argv[])
{
    // make sure it has just one command line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        if (!only_digits(argv[1]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // convert argument from string to int
    int key = atoi(argv[1]);

    // get plaintext
    string plaintext = get_string("Plaintext: ");
    printf("Ciphertext: ");

    // for each character in plaintext
    int length = strlen(plaintext);
    for (int i = 0; i < length; i++)
    {
        // rotate
        printf("%c", rotate(plaintext[i], key));
    }

    printf("\n");
}

bool only_digits(string s)
{
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (s[i] < 48 || s[i] > 57)
        {
            return false;
        }
    }
    return true;
}

char rotate(char p, int k)
{
    
    if (isalpha(p))
    {
        if (isupper(p))
        {
            // subtract value of 'A'
            p -= 65;
            // add key
            p += k;
            // wrap around
            p %= 26;
            // convert back
            p += 65; 
        }
        else
        {
            // subtract value of 'a'
            p -= 97;
            // add key
            p += k;
            // wrap around
            p %= 26;
            // convert back
            p += 97; 
        }
        return p;
    }
    return p;
}