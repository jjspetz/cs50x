/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <string.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // declare variables
    int left = 0;
    int right = n - 1;
    int middle = right + left / 2;

    if (n <= 0)
    {
        return false;
    }

    do
    {
        middle = right + left / 2;

        if (values[middle] < value)
        {
            left = middle + 1;
        }
        else if (values[middle] > value)
        {
            right = middle - 1;
        }
        else if (values[middle] == value)
        {
            return true;
        }
    } while (right >= left);

    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n) sorting algorithm

    // makes and initilizes counting array
    int MAX = 65536;
    int count_array[MAX];
    memset(count_array, 0, sizeof count_array); //used to initialize "variable-sized arrays"

    /* // This code won't work because the array is too large.
    for (int i = 0; i < MAX; i++)
    {
        int count_array[i] = 0;
    }
    */

    for (int i = 0; i < n; i++)
    {
        count_array[values[i]]++; // populates the counting array (maybe x=values[i] & count_array[x])
    }

    // rewrites over values the sorted numbers generated via count-array
    for (int i = 0, j = 0; i < MAX; i++)
    {
        while (count_array[i] > 0)
        {
            values[j] = i;
            j++;
            count_array[i]--;
        }
    }

    return;
}
