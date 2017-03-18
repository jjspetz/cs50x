#include <cs50.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
    // declare variables
    long long cc_num = 0; // intialize credit card number
    char cc_string [50];
    int checksum = 0;
    char checksum_string1[10] = {0}; // part of number to be used in step one of checksum
    char checksum_string2[10] = {0}; // part of number to be added in step two of checksum


    // gets user input
    printf("Number:");
    cc_num = get_long_long();

    // turn the cc_num into a string
    sprintf(cc_string, "%lli", cc_num);

    int n = strlen(cc_string);
    // gets the numbers to be multiplied by two for checksum
    for (int i = n-2, j = 0; i >= 0; i -= 2)
    {
        checksum_string1[j] = cc_string[i];
        j++;
    }

    // calculates step one of the checksum
    for (int i = 0, m = strlen(checksum_string1); i < m; i++)
    {
        int num = (checksum_string1[i] - '0') * 2;
        checksum +=  num / 10 + num % 10;
    }

    // gets the remaining numbers to be used in the checksum
    for (int i = n-1, j = 0; i >= 0; i -= 2)
    {
        checksum_string2[j] = cc_string[i];
        j++;
    }

    // adds remaining digites to checksum
    for (int i = 0, m = strlen(checksum_string2); i < m; i++)
    {

        checksum +=  (checksum_string2[i] - '0');
    }

    // checks checksum
    if (checksum % 10 == 0)
    {
        if (cc_string[0] == '4')
        {
            printf("VISA\n");
        }
        if (cc_string[0] == '3')
        {
            switch (cc_string[1]) {
                case '4':
                case '7':
                    printf("AMEX\n");
            }
        }
        if (cc_string[0] == '5')
        {
            switch(cc_string[1]) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                    printf("MASTERCARD\n");
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
