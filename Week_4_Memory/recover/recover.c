
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    // Make sure there is only one command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILENAME\n");
        return 1;
    }

    // open the memory card
    FILE *card = fopen(argv[1], "r");
    // check that it opened successfully
    if (card == NULL)
    {
        printf("File %s can't be opened!\n", argv[1]);
        return 1;
    }

    // declare variables
    BYTE buffer[512];
    int jpeg_count = 0;
    FILE *image = NULL;
    char *filename = malloc(8 * sizeof(char));
    int c = 0;

    // while there is still data left to read, read chuncks of 512 bytes
    while (fread(&buffer, sizeof(buffer), 1, card) == 1)
    {
        // if the start of the chunck is a jpeg header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if a file is open
            if (image)
            {
                // close it
                fclose(image);
            }
            // open a new file
            sprintf(filename, "%03d.jpg", jpeg_count);
            image = fopen(filename, "w");
            if (image == NULL)
            {
                return 1;
            }
            // increment filename
            jpeg_count++;
            // write chunck to new file
            fwrite(&buffer, sizeof(buffer), 1, image);
        }
        // if the start is not a jpeg header
        else
        {
            // if there's a file open
            if (image)
            {
                // continue writing to that file
                fwrite(&buffer, sizeof(buffer), 1, image);
            }
        }
    }

    free(filename);
    fclose(card);
    fclose(image);
}
