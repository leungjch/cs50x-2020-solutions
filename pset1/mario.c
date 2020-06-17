#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n = -1;
    do
    {
        n = get_int("Height: ");
    }
    while (n <= 0 || n > 8);
    // Print n lines
    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            printf(" ");
        }

        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("  ");

        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}