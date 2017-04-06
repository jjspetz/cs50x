/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>

#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    
    
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    //opens text and checks if null
    char *text = (argc == 3) ? argv[2] : argv[1];
    FILE *fp = fopen(text, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 0;
    }
    // if the text passed error check finds the size of text
    int back = ftell(fp);
    fseek(fp, 0, SEEK_END);
    unsigned int sz = ftell(fp);
    fseek(fp, back, SEEK_SET);
    return sz;
    
    fclose(fp);
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return false;
}


// This is the hash function I will use. It is the public-domain sdbm function
    static unsigned long
    sdbm(str)
    unsigned char *str; // this string is from load
    {
        unsigned long hash = 0;
        int c;

        while (c == *str++) {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }
        return hash;
    }