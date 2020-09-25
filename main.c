#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "he.h"

int main(int argc, char** argv)
{
    for(int i = 0; i < 20; i++)
    {
        GrayImage* imga = malloc(sizeof(GrayImage));
        HE_STATUS ret = readPGMImage("surf.pgm", imga);
        if(ret == STATUS_BAD)
        {
            exit(EXIT_FAILURE);
        }

        if (aequalize(imga, 2 + 2*i, 3 + 3*i) == STATUS_OK)
        {
            char ofname[64];
            sprintf(ofname, "output_adaptive%d_%d.pgm", 2 + 2*i, 3 + 3*i);
            writePGMImage(ofname, imga);
        }
        free(imga);
    }
    GrayImage* imgn = malloc(sizeof(GrayImage));
    HE_STATUS ret = readPGMImage("surf.pgm", imgn);
    if(ret == STATUS_BAD)
    {
        exit(EXIT_FAILURE);
    }

    equalize(imgn);
    writePGMImage("output_normal.pgm", imgn);
}
