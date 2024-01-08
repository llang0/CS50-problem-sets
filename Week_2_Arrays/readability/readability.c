#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int main(void)
{
    // Prompt the user for some text
    string input = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(input);
    int words = count_words(input);
    int sentences = count_sentences(input);

    // Compute the Coleman-Liau index
    float l = ((float) letters / words) * 100.0;
    float s = ((float) sentences / words) * 100.0;

    float index = 0.0588 * l - 0.296 * s - 15.8;
    index = round(index);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else 
    {
        printf("Grade %i\n", (int) index);
    }
}

// count number of letters
int count_letters(string text)
{
    int length = strlen(text);
    int n = 0;
    // Return the number of letters in text
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            n++;
        }
    }
    return n;
}

// count the number of words, determined by how many spaces
int count_words(string text)
{
    int length = strlen(text);
    int n = 1;
    for (int i = 0; i < length; i++)
    {
        if (isspace(text[i]))
        {
            n++;
        }
    }
    return n;
}
// count number of sentances
// determined by how many ending punctuation
int count_sentences(string text)
{
    int length = strlen(text);
    int n = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            n++;
        }
    }
    return n;
}