#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int before;
    int after;
    int year = 0;

    do
    {
        before = get_int("Start size: ");
    }
    while (before < 9);

    do
    {
        after = get_int("End size: ");
    }
    while (after < before);


    while (before < after)
    {
        before = before + (before / 3) - (before / 4);
        year++;
    }


    printf("Years: %i \n", year);

}
