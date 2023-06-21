// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int number = 26;
unsigned int word_count;

// Hash table
node *table[number];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    node *pointer = table[hash(word)];

    while (pointer != 0)
    {
        if (strcasecmp(word, pointer->word) == 0)
        {
            return true;
        }
        pointer = pointer->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int total = 0;
    for (int i = 0 ; i < strlen(word); i++)
    {
        total += tolower(word[i]);
    }

    return total % number;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        printf("Cant open %s\n", dictionary);
        return false;
    }
    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *memory = malloc(sizeof(node));

        if (memory == NULL)
        {
            return false;
        }

        strcpy(memory->word, word);
        memory->next = table[hash(word)];
        table[hash(word)] = memory;
        word_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < number; i++)
    {
        node *pointer = table[i];
        while (pointer != NULL)
        {
            node *tmp = pointer;
            pointer = pointer->next;
            free(tmp);
        }
        if (pointer == NULL && i == number - 1)
        {
            return true;
        }
    }
    return false;
}
