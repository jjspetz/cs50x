/**
 * resizes a BMP
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // cheks if the inputed n is an integer between 1 and 100
    if (n > 101 || n < 1)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\nWhere n is an integer from 1 to 100\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

     // read infile's BITMAPFILEHEADER makes 2 copies one to read to and one to write to
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPFILEHEADER bf_new;
    bf_new = bf;

    // read infile's BITMAPINFOHEADER makes 2 copies one to read to and one to write to
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    BITMAPINFOHEADER bi_new;
    bi_new = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // MODIFY the NEW BITMAPFILEHEADER
    bi_new.biWidth = bi.biWidth * n;
    bi_new.biHeight = bi.biHeight * n;

    // determine padding for scanlines
    int padding_old = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_new = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi_new.biSizeImage = ((sizeof(RGBTRIPLE) * bi_new.biWidth) + padding_new) * abs(bi_new.biHeight);
    bf_new.bfSize = bi_new.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

    // delares an array of structs and a count to fill up the array
    RGBTRIPLE row[bi_new.biWidth];
    int count = 0;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // copies scanline row so it can be added n times for height
            for (int k = 0; k < n; k++)
            {
                row[count] = triple;
                count++;
            }
        }

        // skip over padding, if any
        fseek(inptr, padding_old, SEEK_CUR);


        // writes scanline n times
        for (int m = 0; m < n; m++)
        {
            for (int x = 0; x < bi_new.biWidth; x++)
            {
                fwrite(&row[x], sizeof(RGBTRIPLE), 1, outptr);
            }
            // adds new padding to outfile
            for (int l = 0; l < padding_new; l++)
            {
                    fputc(0x00, outptr);
            }
        }

        count = 0; // reinitilize count for next pass through loop
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
