#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    FILE *raw = NULL;
    BYTE buffer[512];
    int count = 0;
    char filename[8];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }



    while (fread(buffer, sizeof(BYTE), 512, file))
    {
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            if (raw != NULL)
            {
                fclose(raw);
            }
            sprintf(filename, "%03d.jpg", count++);
            raw = fopen(filename, "w");
        }

        if (raw != NULL)
        {
            fwrite(buffer, sizeof(BYTE), 512, raw);
        }
    }

    if (raw != NULL)
    {
        fclose(raw);
    }
    fclose(file);

    return 0;
}