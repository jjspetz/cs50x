/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

// hash table declaration
node* hashtable[MAX];

// dictionary word counter
int words;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    /***********
     *
     * 1. hash word
     * 2. loop through linked list checking for word
     * 3. if found return true else return false
     *
     ************/

    // declare useful variables
    int word_len = strlen(word);
    char checkword[word_len+1];

    // changes word to lower case and writes it into checkword variable
    for (int i=0; i< word_len; ++i)
        checkword[i] = tolower(word[i]);

    // adds word break to end of checkword
    checkword[word_len] = '\0';

    // looks up place in hashtable
    node* look;
    look = hashtable[hash(checkword)];

    // loops through linked list if any
    while(look)
    {
        // strcmp compares a string and returns 0 if same
        if (strcmp(look->word, checkword) == 0)
        {
            return true;
        }
        // continues through linked list if first value is not the same
        else
            look = look->next;
    }
    // returns false to indicate the word was not found in the dictionary
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

    // hash table initilization
    for (int i = 0; i < MAX; i++)
    hashtable[i] = NULL;


    // loops through the file one word at a time
    while (fscanf(dict, "%s", buffer) != EOF){

        // create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("out of memory");
            return false;
        }

        // copies buffer into the new node
        strcpy(n->word, buffer);
        n->next = NULL;

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
    // declare a node pointer variable to "look through the list" and to clear list as it goes
    node* look;
    node* clear;

    // loops through the hashtable
    for (int i = 0; i < MAX; i++)
    {
        // looks at current hashtable index
        look = hashtable[i];

        // loops through the linked list freeing as it goes
        while (look)
        {
            clear = look;
            look = look->next;
            free(clear);
        }
    }

    return true;
}

	/**************************
	* Hash Function
	*
	* I will use the djb2 hash function.
	* code from https://github.com/batanete/CHashTables/blob/master/hashtables.c
	* It is modified for use in my program.
	*
	**************************/
	unsigned int hash(const char* key){
	unsigned int res = 5381; //djb2
	//unsigned int res = 0; // sdbm
    int c;

    while ((c = *key++))
		res = ((res << 5) + res) + c; /* hash * 33 + c (djb2)*/
		//res = c + (res << 6) + (res << 16) - res; //sdbm

	// makes sure hash value will fit into my hash table
	res = res % MAX;


	return res;
}
