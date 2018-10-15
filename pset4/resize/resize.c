/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4 || *argv[1] > 100 || *argv[1] < 0)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames & resize factor
    int resizeFactor = *argv[1] - 48;
    char *infile = argv[2];
    char *outfile = argv[3];

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

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //Calculate new BITMAPFILEHEADER & BITMAPINFOHEADER

    BITMAPINFOHEADER bio;
    bio = bi;
    bio.biWidth = bi.biWidth * resizeFactor;
    bio.biHeight = bi.biHeight * resizeFactor;

    //Padding for outfile
    int paddingout = (4 - (bio.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bio.biSizeImage =((sizeof(RGBTRIPLE) * bio.biWidth) + paddingout) * abs(bio.biHeight);

    BITMAPFILEHEADER bfo;
    bfo = bf;
    bfo.bfSize = bio.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfo, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bio, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        //position reference to re-copy the line
        int precopy = ftell(inptr);

        for (int n = 0; n < resizeFactor; n++){
            //reposition of cursor for line re-copy
                fseek(inptr, precopy, SEEK_SET);

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
                {

                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    // write RGB triple to outfile
                    //iterates over pixel for outputfile
                    for (int l = 0; l < resizeFactor; l++){
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
                 // then add padding in outfile if need it
                for (int m = 0; m < paddingout; m++){
                fputc(0x00, outptr);
                }

            }


        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
