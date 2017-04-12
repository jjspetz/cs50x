/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>

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
    // declare linked list variables
    node* n;
    node* t;
    
    // opens dictionary
    FILE* dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        printf("Could not open dicionary.\n");
        return false;
    }
    
    // creates buffer of maximum length for fgets
    char word[LENGTH + 1];
    
    // loops through the file one word at a time
    while (fscanf(dict, "%s", word) != EOF){
        
        // create new node
        node* n = malloc(sizeof(node));
        if (n ==NULL)
        {
            printf("out of memory");
            return false;
        }
        
        // copies buffer into the new node
        strcpy(n->word, buffer);
        
        // checks to make sure there is a place to link n-node to
        if (hashtable[hash(word)] != NULL)
            n->next = hashtable[hash(word)];
            
        // set t to n
        t = n;
        
        // point hash table to head of linked list
        if (hashtable[hash(word)] == NULL)
            hashtable[hash(word)] = n;
    }

    
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

	/**************************
	* Hash Function
	* 
	* I will use the sdbm hash function.
	* code from https://github.com/batanete/CHashTables/blob/master/hashtables.c
	* It is modified for use in my program.
	* 
	**************************/
	unsigned long hash(char* key){
	//unsigned long res = 5381; //djb2
	unsigned long res = 0; // sdbm
    int c;

    while ((c = *key++))
		//res = ((res << 5) + res) + c; /* hash * 33 + c (djb2)*/
		res = c + (res << 6) + (res << 16) - res; //sdbm
		
	// makes sure hash value will fit into my hash table
	res = res % MAX;
	
	
	return res;
}