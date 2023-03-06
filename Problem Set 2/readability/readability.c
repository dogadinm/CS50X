#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    int count_letters, count_words, count_sentences;

    string s = get_string("Texts: ");
    count_letters = 0;
    count_words = 1;
    count_sentences = 0;

    for (int i = 0 ; i < strlen(s); i++)
    {
        if (isalpha(s[i]))
        {
            count_letters++;
        }

        if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            count_sentences++;
        }
    }

    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ' ')
        {
            count_words++;
        }
    }


    float L = (count_letters / (float) count_words) * 100;
    float S = (count_sentences / (float) count_words) * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }


}