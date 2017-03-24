#include <stdio.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file
    FILE *in = fopen(infile, "r");
    if (in == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // declares variables
    uint8_t buffer[512]; // needs 512 to handle one chunk
    char filename[8];
    int count = 0;
    FILE *img = NULL;

    // fread starts (this way should continue until it gets to EOF)
    while(fread(&buffer, 1, 512, in) == 512)
    {
        // checks for JPEG header
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
            {
                // creates file name for current jpg
                sprintf(filename, "%03i.jpg", count);

                // close last opened jpg
                if (count > 0) {
                    fclose(img);
                }

                // opens new jpeg file named "filename"
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    fclose(in);
                    fprintf(stderr, "Could not create %s.\n", filename);
                    return 3;
                }

                count++; // increases count for next found JPEG
            }

            // write jpeg 512 bytes at a time until new jpeg is found
            if (count > 0) // the if statement prevents writing before a jpeg is found
            {
                fwrite(&buffer, 1, 512, img);
            }
    }
    // close in
    fclose(in);

    // close last img
    fclose(img);

    // success
    return 0;
}
