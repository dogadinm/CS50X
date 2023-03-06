#include <cs50.h>
#include <stdio.h>

int main(void)

{
    int Height, Row, Free, Colum;

    // Checking for an input number
    do
    {
        Height = get_int("Height:");
    }
    while (Height < 1 || Height > 8);

    // print row
    for (Row = 0; Row < Height; Row++)
    {
        for (Free = Height - 1 ; Free > Row;  Free--)
        {
            printf(" ");
        }

        // print colume
        for (Colum = 0; Colum <= Row; Colum++)
        {
            printf("#");
        }
        printf("\n");
    }
}