/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// maximum size of hash table
const int MAX = 65536;

// declare nodes for my linked lists
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// hash table
node* hashtable[MAX];

// dictionary word counter
int words;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(char *word)
{
    /***********
     *
     * 1. hash word
     * 2. loop through linked list checking for word
     * 3. if found return true else return false
     *
     ************/
    // hash word
    int index = hash(word);

    // declare a node pointer variable to "look through the list"
    node* look;
    look = hashtable[index];

    // loops through the linked list looking for a match
    while (look->next != NULL)
    {
        if (look->word == word)
            return true;
        else
            look = look->next;
    }

    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // opens dictionary
    FILE* dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        printf("Could not open dicionary.\n");
        return false;
    }

    // creates buffer of maximum length for fgets
    char buffer[LENGTH + 1];

    // loops through the file one word at a time
    while (fscanf(dict, "%s", buffer) != EOF){

        // create new node
        node *n = malloc(sizeof(*n));
        if (n ==NULL)
        {
            printf("out of memory");
            return false;
        }

        // copies buffer into the new node
        strcpy(n->word, buffer);

        // checks to see if hashtable placement is empty
        if (hashtable[hash(buffer)]==NULL)
            hashtable[hash(buffer)] = n;

        // handles hashtable conflicts
        else
        {
            // declare a temp pointer and points to the head of the linked list
            node* tmp;
            tmp = hashtable[hash(buffer)];

            // set the node pointer to the front of the linked list
            n->next = tmp;

            // points table pointer to the newest front of the list
            hashtable[hash(buffer)] = n;
        }

        // increases word count by one
        words++;
    }
    // close dictionary
    fclose(dict);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return false;
}

	/**************************
	* Hash Function
	*
	* I will use the sdbm hash function.
	* code from https://github.com/batanete/CHashTables/blob/master/hashtables.c
	* It is modified for use in my program.
	*
	**************************/
	unsigned int hash(char* key){
	//unsigned long res = 5381; //djb2
	unsigned int res = 0; // sdbm
    int c;

    while ((c = *key++))
		//res = ((res << 5) + res) + c; /* hash * 33 + c (djb2)*/
		res = c + (res << 6) + (res << 16) - res; //sdbm

	// makes sure hash value will fit into my hash table
	res = res % MAX;


	return res;
}
