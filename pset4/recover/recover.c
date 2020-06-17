#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <stdint.h>
typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    BYTE bytes[512];

    size_t nread;

    FILE *file = fopen(argv[1], "r");

    bool readingFile = false;
    int file_num = 0; // number of files encountered
    int block = 0;

    char name[50];
    sprintf(name, "%03d", file_num);

    char buf[50];
    snprintf(buf, sizeof(buf), "%s.jpg", name);
    // create new file
    FILE *jpgfile = fopen(buf, "w");
    while ((nread = fread(bytes, 1, 512, file)) > 0)
    {
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] <= 0xef && bytes[3] >= 0xe0))
        {
            if (readingFile) // encounter new jpg signature means new jpg file
            {
                printf("filenum: %i had %i bytes\n", file_num, block);
                block = 0;

                fclose(jpgfile);
                readingFile = false;
                file_num += 1;
                sprintf(name, "%03d", file_num);

                snprintf(buf, sizeof(buf), "%s.jpg", name);
                // create new file
                jpgfile = fopen(buf, "w");

            }
            readingFile = true;
        }
        if (readingFile)
        {
            fwrite(bytes, 1, 512, jpgfile);
            block += 1;
        }
    }

    // Close file
    fclose(file);
}