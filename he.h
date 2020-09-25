#ifndef HE_H
#define HE_H
typedef struct PGMImage
{
    int w;
    int h;
    int levels;
    unsigned char** data;
} GrayImage;

typedef enum
{
    STATUS_OK = 0,
    STATUS_BAD = 1
} HE_STATUS;

HE_STATUS equalize(GrayImage* img);
HE_STATUS aequalize(GrayImage* img, unsigned int subx, unsigned int suby);
HE_STATUS readPGMImage(char *fileName, GrayImage* image);
HE_STATUS writePGMImage(char* filename, GrayImage* image);

#endif //HE_H
