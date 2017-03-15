#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void error_msg(void);

//global variables
int key_length = 0; // I want to calculate this once and use it accros scopes

int main(int argc, string argv[])
{
    // checks for input of a cipher key
    if (argc != 2)
    {
        error_msg();
        return 1;
    }
    if (argv[1]) // must check for argv before setting variable to avoid runtime error
    {
        key_length = strlen(argv[1]); // can't be accessed outside of the if statement.
        for (int i = 0; i < key_length; i++)
        {
            if (!isalpha(argv[1][i]))
            {
                error_msg();
                return 1;
            }
        }
    }

    // prompts user for plain text to be ciphered
    printf("plaintext: ");

    string p = get_string(); // user input is stored in var p


    // initilize more variables here
    int plaintext_length = strlen(p); // calculates length of the plain text string and saves it for further use
    char c[plaintext_length]; // initialize the cipher text var
    int key[key_length];

    // convert text key to an array of ints
    for (int i = 0; i < key_length; i++)
    {
        key[i] = toupper(argv[1][i]) - 65;
    }

    for (int i = 0, j = 0; i < plaintext_length; i++)
    {
        if (p[i] > 64 && p[i] < 91)
        {
            c[i] = (p[i] + key[j % key_length] - 65) % 26 + 65;
            j++;
        }
        else if (p[i] > 96 && p[i] < 123)
        {
            c[i] = (p[i] + key[j % key_length]- 97) % 26 + 97;
            j++;
        }
        else
        {
            c[i] = p[i];
        }
    }

    // outputs cipher text
    printf("ciphertext: ");
    for (int i = 0; i < plaintext_length; i++)
    {
        printf("%c", c[i]);
    }
    printf("\n");

    return 0; // final return: indicates program closed without error
}

void error_msg(void)
{
    printf("This program reqiures one additional alphabetic argument for a cipher key\n");
    printf("EXAMPLE: ./vigenere key\n");
}
