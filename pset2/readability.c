#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>


int main(void)
{
    string text = get_string("Text: ");
    long length = strlen(text);


    long sen = 0;
    long wor = 0;
    long let = 0;
    // loop through and get letters, words, sentences
    for (int c = 0; text[c] != '\0'; c++)
    {
        char ch = tolower(text[c]); // convert to lower case

        if isalnum(ch)
        {
            let += 1;
        }
        else if isspace(ch)
        {
            wor += 1;
        }
        else if (ch == '!' || ch == '?' || ch == '.')
        {
            sen += 1;
            if (c == length - 1)
            {
                wor += 1;
            }
        }
    }
    // printf("Words: %li\n Letters: %li\n Sentences: %li\n", wor,let,sen);

    // convert sen to sen/100 wor, etc.
    double letwor = (double) let / (double) wor * 100;
    double senwor = (double) sen / (double) wor * 100;

    // compute coleman liau
    long cl = roundl(0.0588 * letwor - 0.296 * senwor - 15.8);
    if (cl >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (cl < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %li\n", cl);
    }




}

