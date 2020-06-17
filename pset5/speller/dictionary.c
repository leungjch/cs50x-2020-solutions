// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "dictionary.h"
#include <math.h>
#include <string.h>
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// 515 is the maximum of the hash index from the biggest word 'pneumonoultramicroscopicsilicovolcanoconiosis'
const unsigned int N = 515;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char cpy[strlen(word) + 1];
    strcpy(cpy, word);
    // convert word to lowercase
    for (int i = 0; word[i] != '\0'; i++)
    {
        cpy[i] = tolower(word[i]);
    }

    // get word hash
    int hash_index = hash(cpy);
    // printf("%s has hash %i\n", word, hash_index);
    // check index of hash table
    // if no element at index, no match

    if (table[hash_index] == NULL)
    {
        return false;
    }
    // else, loop through linked list in index and check word with items
    else
    {
        for (node *tmp = table[hash_index]; tmp != NULL; tmp = tmp->next)
        {
            if (strcmp(cpy, tmp->word) == 0)
            {
                // printf("searching at %s", tmp->word);
                return true;
            }
        }
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Simple hash function:
    // loop through word (converted to lowercase) and get sum of ascii values minus 'a' (97)
    // sum = hash index
    int sum = 0;
    // char *word = "pneumonoultramicroscopicsilicovolcanoconiosis"; // test max
    for (int i = 0; word[i] != '\0' && word[i] != '\n'; i++)
    {
        sum += tolower(word[i]) - 97;
    }

    return abs(sum) % N;
    // return 0;

}
#define MAXCHAR 100

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Read file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) // check null pointer
    {
        return false;
    }

    // printf("Initializing hash table\n");
    // initialize the hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;

    }
    // Loop through elements in dictionary, 1 line at a time
    // build hash
    char str[MAXCHAR];
    while (fgets(str, MAXCHAR, file) != NULL)
    {
        str[strlen(str) - 1] = '\0';
        // printf("Dictionary item is: %s \n", str);
        int hash_index = hash(str);

        // assign a place in the hash table for the word
        // check if node is null (first element)
        if (table[hash_index] == NULL)
        {
            // insert first element
            node *x = malloc(sizeof(node));
            x->next = NULL;
            strcpy(x->word, str);
            table[hash_index] = x;
        }
        // if collision, then add a new element at the end of the list
        else
        {
            // printf("%s has hashee: %i\n", str, hash_index);

            // printf("Collision at %i index with word %s", hash_index, str);
            // loop through until at end of linked list
            node *tmp = table[hash_index];

            while (tmp->next != NULL)
            {
                // printf("we are at %s\n", tmp->word);
                tmp = tmp->next;
            }

            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                printf("Not enough memory\n");
                return 1;
            }
            n->next = NULL;
            strcpy(n->word, str);
            tmp->next = n;


        }

    }
    // printf("Done loading dictionary\n");
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int n = 0;
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        while (tmp != NULL)
        {
            tmp = tmp->next;
            n += 1;
            // printf("n at %s is now:%i\n", tmp->word, n);
        }
    }
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return n;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // loop through hash table
    for (int i = 0; i < N; i++)
    {
        node *list = table[i];
        while (list != NULL)
        {
            node *tmp = list->next;
            free(list);
            list = tmp;
        }
        free(list);

    }
    return true;

}
