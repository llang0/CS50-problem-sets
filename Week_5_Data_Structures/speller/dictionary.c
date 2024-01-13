// Implements a dictionary's functionality
#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 8192 * 4;

// Hash table
node *table[N];
int dict_size = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash word to obtain hash value
    int index = hash(word);

    // access the linked list at that hash value
    node *list = table[index];
    if (list == NULL)
    {
        return false;
    }
    else
    {
        // traverse linked list looking for that word
        node *ptr = list;
        while (ptr != NULL)
        {
            // Return true if the word is in the list
            int same = strcasecmp(word, ptr->word);
            if (same == 0)
            {
                return true;
            }
            else
            {
                ptr = ptr->next;
            }
        }
    }
    // otherwise return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    long total = 0;
    int l = strlen(word);
    for (int i = 0; i < l; i++)
    {
        long c = tolower(word[i]);
        c = c * c;
        total += c;
    }
    total %= N;
    return total;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // put a null pointer at every index of the hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // Open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Couldn't open dictionary.\n");
        return false;
    }
    // Read strings from file one at a time
    // create a buffer to hold word
    char *word = malloc(LENGTH * sizeof(char));
    // int n_words = 0;
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);
        n->next = NULL;

        // Hash word to obtain a hash value
        int index = hash(n->word);
        // Insert node into hash table at that location
        // if theres nothing at that index
        if (table[index] == NULL)
        {
            n->next = NULL;
            table[index] = n;
        }
        else
        {
            node *list = table[index];
            n->next = list;
            list = n;
            table[index] = list;
        }
        dict_size++;
    }
    free(word);
    fclose(dict);
    printf("Dictionary size: %i\n", size());
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *list = table[i];
        if (list != NULL)
        {
            node *ptr = list;
            node *tmp = ptr;

            while (ptr != NULL)
            {
                ptr = ptr->next;
                free(tmp);
                tmp = ptr;
            }
        }
    }
    return true;
}
