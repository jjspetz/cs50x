#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, string argv[])
{
    // checks for input of a cipher key
    if (argc != 2)
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }

    // prompts user for plain text to be ciphered
    printf("plaintext: ");

    string p = get_string(); //user input is stored in var p

    // initilize variables here
    int n = strlen(p); //calculates length of the plain text string and saves it for further use
    char c[n]; //initialize the cipher text var
    int key = atoi(argv[1]);

    // loops through the plain text and creates cipher text
    for (int i = 0; i < n; i++)
    {
        if (p[i] > 64 && p[i] < 91)
        {
            c[i] = (p[i] + key - 65) % 26 + 65;
        }
        else if (p[i] > 96 && p[i] < 123)
        {
            c[i] = (p[i] + key - 97) % 26 + 97;
        }
        else
        {
            c[i] = p[i];
        }
    }

    // outputs cipher text
    printf("ciphertext: ");
    for (int i = 0; i < n; i++)
    {
        printf("%c", c[i]);
    }
    printf("\n");
}
