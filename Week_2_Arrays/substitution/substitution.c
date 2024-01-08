#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char rotate(char c, string k);
int main(int argc, string argv[])
{
    // check for the right number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
    // check for valid key
    {
        string key = argv[1];
        // check for valitd key
        int key_length = strlen(key);
        // make sure key is 26 characters long
        if (key_length != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }

        // make sure key contains only alphabetic characters
        // and each letter is only seen once
        char seen[26];
        for (int i = 0; i < key_length; i++)
        {
            if (!isalpha(key[i]))
            {
                printf("Key may only contain letters.\n");
                return 1;
            }
            else
            {
                key[i] = toupper(key[i]);
                // for every seen character
                for (int j = 0; j < key_length; j++)
                {
                    // if this character is seen return 1
                    // otherwise add character to seen
                    if (key[i] == seen[j])
                    {
                        printf("Key must use each letter only once.\n");
                        return 1;
                    }
                }
                seen[strlen(seen)] = key[i];
            }
        }
    }

    // get key
    string key = argv[1];

    // get plaintext
    string plaintext = get_string("Plaintext: ");
    printf("ciphertext: ");

    // for each character in plaintext
    int length = strlen(plaintext);
    for (int i = 0; i < length; i++)
    {
        printf("%c", rotate(plaintext[i], key));
    }

    printf("\n");
}

char rotate(char c, string k)
{
    if (isalpha(c))
    {
        if (isupper(c))
        {
            c -= 65;
            char key_char = k[c];
            key_char = toupper(key_char);
            return key_char;
        }
        else 
        {
            c -= 97;
            char key_char = k[c];
            key_char = tolower(key_char);
            return key_char;
        }
    }
    else
    {
        return c;
    }
}
