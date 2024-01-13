#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int length = 45;
const int N = 8192;
int table[N];

int hash(char *string);
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./count_words FILENAME\n");
    }

    FILE *f = fopen(argv[1], "r");
    char *buffer = malloc(length * sizeof(char));

    for (int i = 0; i < N; i++)
    {
        table[i] = 0;
    }

    // add each hashed word to a linked list
    int word_count = 0;
    int largest = 0;

    while (fscanf(f, "%s", buffer) != EOF)
    {
        unsigned int h = hash(buffer);
        table[h]++;
        word_count++;

        if (h > largest)
        {
            largest = h;
        }
    }
    int longest = 0;
    for (int i = 0; i < N; i++)
    {
        printf("%i ", table[i]);
        if (table[i] > longest)
        {
            longest = table[i];
        }
        if (i % 10 == 0)
        {
            printf("\n");
        }
    }

    free(buffer);
    fclose(f);

    printf("\n");
    printf("\n");
    printf("Total words: %i\n", word_count);
    printf("Largest hash: %i\n", largest);
    printf("Longest linked list: %i\n", longest);
    return 0;
}

int hash(char *word)
{
    long total = 0;
    int l = strlen(word);
    for (int i = 0; i < l; i++)
    {
        long c = tolower(word[i]);
        c = c*c;
        total += c;
    }
    total %= N;
    return total;
}
