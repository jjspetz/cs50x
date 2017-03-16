#include <cs50.h>
#include <stdio.h>

void print_pyramid(int height);
int get_height(void);

int main(void)
{

    print_pyramid(get_height());
}

int get_height(void)
{
    printf("Height: ");
    int height = get_int();

    while (true) {
        if (height > 23 || height < 0)
        {
            printf("Height: ");
            height = get_int();
        } else { break; }
    }
    return height;
}

void print_pyramid(int height)
{
    for (int i=1; i<= height; i++)
    {
        for (int j = 0; j < height - i; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}
