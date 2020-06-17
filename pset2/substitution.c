#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>

// const char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

// int getLetterIndex()
// {

// }

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // get substitution
    string key = argv[1];
    // printf("key is: %s", key);
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;

    }

    char clean_key[26];

    const int keylen = strlen(key);

    // build the clean key (all lower)
    for (int c = 0; c < keylen; c++)
    {
        // printf("letter is : %i", key[c]);
        clean_key[c] = tolower(key[c]);
    }

    // check no invalid characters or duplicate characters
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Invalid key.\n");
            return 1;
        }
        int dupcount = 0;
        for (int j = 0; j < strlen(key); j++)
        {
            if (key[i] == key[j])
            {
                dupcount += 1;
            }
        }
        if (dupcount > 1) // duplicate if two occurrences
        {
            printf("Invalid key.\n");
            return 1;
        }


    }

    string pt = get_string("plaintext: ");
    char ct[strlen(pt)];

    const int ptlen = strlen(pt);

    char clean_pt[ptlen];
    char pt_copy[ptlen];
    strncpy(pt_copy, pt, ptlen);
    strncpy(clean_pt, pt, ptlen);

    char clean_ct[ptlen];
    strncpy(clean_ct, ct, ptlen);


    // printf("clean key is: %s", clean_key);
    // build the cleaned plaintext

    for (int c = 0; pt[c] != '\0'; c++)
    {
        char temp = pt_copy[c];
        clean_pt[c] = tolower(temp);
    }

    // build the cleaned ciphertext
    for (int c = 0; c < ptlen; c++)
    {
        char cipher_char;
        if (isalpha(clean_pt[c]))
        {
            int index = (int) clean_pt[c] - 97;
            cipher_char = clean_key[index];

        }
        else
        {
            cipher_char = pt_copy[c];
        }

        clean_ct[c] = cipher_char;
    }

    // convert back to proper cases
    for (int c = 0; c < ptlen; c++)
    {
        char cipher_char = clean_ct[c];
        char plain_char = pt_copy[c];
        if (isupper(plain_char))
        {
            cipher_char = toupper(cipher_char);

        }
        else if (islower(plain_char))
        {
            cipher_char = tolower(cipher_char);
        }

        clean_ct[c] = cipher_char;
    }
    // for (int i = 0; i < ptlen; i++)
    // {
    //     printf("cleanct is: %s\n", clean_ct);
    // }
    printf("ciphertext: %s\n", clean_ct);

}