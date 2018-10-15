#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//define BYTE
typedef uint8_t  BYTE;

int main(int argc, char *argv[]){


    //ensure proper usage
    if (argc != 2){
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames, create buffer and variables
    char *infile = argv[1];
    BYTE block[511];
    char filename[8];
    int nextfile = 0;
    int filesfound = 0;
    FILE *outptr;

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }


    //read first infile's 512 Block
    int blocksizecheck = fread(block, 1, 512, inptr);

    //Repeat until find EOF (end of file)
    while(blocksizecheck > 0){

        //check if start new jpeg
        if (block[0] == 0xff &&
           block[1] == 0xd8 &&
           block[2] == 0xff &&
           (block[3] & 0xf0) == 0xe0){

                if (nextfile == 0){
                    //create a filename
                    sprintf(filename, "%03d.jpg", filesfound);

                    //create a outfile
                    outptr = fopen(filename, "w");

                    //Check if outfile can open
                    if (outptr == NULL){
                    fclose(inptr);
                    fprintf(stderr, "Could not create %s.\n", filename);
                    return 3;
                    }

                    //write a block of 512 Bytes in outfile
                    fwrite(block, 512, 1, outptr);
                    //read next block
                    nextfile = 1;
                }

                else if (nextfile == 1){

                    //close last outfile
                    fclose (outptr);

                    //add one in file found counter
                    filesfound++;

                    //create a filename
                    sprintf(filename, "%03d.jpg", filesfound);

                    //create a new outfile
                    outptr = fopen(filename, "w");

                    //Check if outfile can open
                    if (outptr == NULL){
                    fclose(inptr);
                    fprintf(stderr, "Could not create %s.\n", filename);
                    return 3;
                    }

                    //write a block of 512 Bytes in outfile
                    fwrite(block, 512, 1, outptr);
            }

        }

        else if(nextfile == 1){

            //write a block of 512 Bytes in outfile
            fwrite(block, 512, 1, outptr);
        }
    //read next block ok 512 Bytes
    blocksizecheck = fread(block, 1, 512, inptr);
    }

    //Close Files & main return 0
    fclose(inptr);
    fclose(outptr);
    return 0;
}

