// This program assumes the input will be a string only containing letters and spaces!

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
    char patt = ' ';
    string s = get_string(); // gets a string from the user. Assumes input is a valid name.

    printf("%c", toupper(s[0]));

    for (int i = 0, n = strlen(s); i < n; i++) // interates through the rest of the string
    {
        /* if the i'th place of sting is a space and the next character is neither a space nor the end of the string
         it prints the next character (s[i+1]) */
        if (s[i] == patt && s[i+1] != patt && s[i+1] != '\0')
        {
            printf("%c", toupper(s[i+1]));
        }
    }
    printf("\n");
}
