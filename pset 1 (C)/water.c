#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("How many minutes do you shower?: ");
    int min = get_int();
    printf("You use approximately %i bottles of water while showering.\n", min * 12);

}
