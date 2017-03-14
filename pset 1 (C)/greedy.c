#include <cs50.h>
#include <stdio.h>
#include <math.h>

int get_change(void);
int reduce(int change);

int main(void)
{
    printf("%i\n", reduce(get_change()));
}

int get_change(void)
{
    printf("How much change is owed? ");
    float change = get_float();

    while (true) {
        if (change < 0)
        {
            printf("How much change is owed? ");
            change = get_float();
        } else { break; }
    }
    change = change * 100;
    return round(change);
}

int reduce (int change)
{
    int coins = 0;

    while (change >= 25)
    {
        change = change - 25;
        coins++;
    }
    while (change >= 10)
    {
        change = change - 10;
        coins++;
    }
    while (change >= 5)
    {
        change = change - 5;
        coins++;
    }
    while (change > 0)
    {
        change = change - 1;
        coins++;
    }
    return coins;
}
