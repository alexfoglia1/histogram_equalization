#include "he.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HE_STATUS readPGMImage(char *fileName, GrayImage* image)
{
    FILE * fp = NULL;
    char version[100];
    int levels = 0;
    unsigned int w = 0, h = 0;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned char pixdat = 0;
    if(fileName == NULL)
    {
        fprintf(stderr, "Bad file name=%s\n", fileName );
        return STATUS_BAD;
    }

    fp = fopen(fileName, "rb");
    if(fp == NULL)
    {
        fprintf(stderr, "Cannot open file = %s\n", fileName );
        return STATUS_BAD;
    }

    fscanf(fp, "%s",version);
    fscanf(fp, "%d %d", &w, &h);
    fscanf(fp, "%d", &levels);

    image->w = w;
    image->h = h;
    image->levels = levels;
    image->data = malloc(w * h);
    for(i = 0; i < h ; i++)
    {
        image->data[i] = malloc(w);
        for(j = 0; j < w; j++)
        {
            fscanf(fp, "%hhu", &pixdat);
            image->data[i][j] = pixdat;
        }
    }
    fclose(fp);

    return STATUS_OK;
}



void do_equalize(GrayImage* img, unsigned int* hist)
{
    double cdf[img->levels + 1];
    cdf[0] = hist[0];
    for(int i = 1; i < img->levels + 1; i++)
    {
        cdf[i] = cdf[i - 1] + hist[i];
    }
    for(int i = 0; i < img->h; i++)
    {
        for(int j = 0; j < img->w; j++)
        {
            unsigned char v = img->data[i][j];
            double cdfv_cdfmin = cdf[v] - cdf[0];
            double mn_cdfmin = img->w * img->h - cdf[0];
            double ratio = cdfv_cdfmin / mn_cdfmin;
            hist[v] = (int)(ratio * (img->levels - 1));
        }
    }

    for(int i = 0; i < img->h; i++)
    {
        for(int j = 0; j < img->w; j++)
        {
            img->data[i][j] = hist[img->data[i][j]];
        }
    }
}

void make_histogram(GrayImage* img, unsigned int* hist)
{
    memset(hist, 0x00, (img->levels + 1) * sizeof(unsigned int));
    for(int i = 0; i < img->h; i++)
    {
        for(int j = 0; j < img->w; j++)
        {
            unsigned char glvl = img->data[i][j];
            hist[glvl] += 1;
        }
    }
}

HE_STATUS equalize(GrayImage* img)
{
    unsigned int hist[img->levels + 1];
    make_histogram(img, hist);
    do_equalize(img, hist);
    return STATUS_OK;
}

HE_STATUS writePGMImage(char* filename, GrayImage* image)
{
    FILE* test = fopen(filename, "w");
    fprintf(test, "%s\n", "P2");
    fprintf(test, "%d ",  image->w);
    fprintf(test, "%d\n", image->h);
    fprintf(test, "%d\n", image->levels);
    for(int i = 0; i < image->h; i++)
    {
        for(int j = 0; j < image->w; j++)
        {
            fprintf(test, "%hhu ", image->data[i][j]);
        }
        fprintf(test, "%s", "\n");
    }
    fclose(test);
}

HE_STATUS aequalize(GrayImage* img, unsigned int subx, unsigned int suby)
{
    if(img->w % subx)
    {
        printf("image width %d not multiple of %d\n", img->w, subx);
        return STATUS_BAD;
    }
    if(img->h % suby)
    {
        printf("image height %d not multiple of %d\n", img->h, suby);
        return STATUS_BAD;
    }

    int dx = img->w / subx;
    int dy = img->h / suby;
    printf("subimages of %dx%d pixels\n", dx, dy);

    int orig_row = 0;
    int orig_col = 0;
    for(orig_row = 0; orig_row < img->h; orig_row += dy)
    {
        for(orig_col = 0; orig_col < img->w; orig_col += dx)
        {
            GrayImage* sub = malloc(sizeof(GrayImage));
            sub->h = img->h/suby;
            sub->w = img->w/subx;
            sub->levels = img->levels;
            sub->data = malloc(sub->h * sub->w);
            for(int j = 0; j < sub->h; j++)
            {
                sub->data[j] = malloc(sub->w);
                for(int k = 0; k < sub->w; k++)
                {
                    unsigned char glvl = img->data[orig_row + j][orig_col + k];
                    sub->data[j][k] = glvl;
                }
            }

            equalize(sub);
            for(int j = 0; j < sub->h; j++)
            {
                for(int k = 0; k < sub->w; k++)
                {
                    img->data[orig_row + j][orig_col + k] = sub->data[j][k];
                }
            }

            /*
            char ofname[64];
            sprintf(ofname, "output_sub_%d_%d.pgm", orig_row, orig_col);
            writePGMImage(ofname, sub);
            */

            free(sub);
        }
    }

    return STATUS_OK;
}
