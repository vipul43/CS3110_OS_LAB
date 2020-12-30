#ifndef PPM
#define PPM
#include <stdio.h>
#include "matrix.h"

struct image
{
  unsigned int height, width;
  int maxval;
  matrix img;
};

typedef struct image* image;

void errmessage(char *);
void ignoreSpaces(FILE*);
void skipComments(FILE*);
image readPPM(char *);
int writePPM(image, char *);
void freeimage(image);

#endif
