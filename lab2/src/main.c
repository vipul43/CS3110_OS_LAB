#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "matrixmath.h"
#include "transformations.h"
#include "ppm.h"


char *concatenate(char*inppath)
{
  int len1 = strlen(inppath);
  char *result = (char*)malloc(sizeof(char)*(len1+5));
  int flag = 0;
  int idx;
  if(inppath[len1-1] != 'm' || inppath[len1-2] != 'p' ||inppath[len1-3] != 'p' ||inppath[len1-4] != '.')
  {
    fprintf(stderr, "Invalid file path. Pls ensure .ppm extension\n");
    exit(1);
  }
  for(idx = len1-1; idx>=0; idx--)
  {
    if(inppath[idx] == '/')
    {
      flag = 1;
      break;
    }
  }
  int count;
  for(count = 0; count<idx+flag; count++)
    result[count] = inppath[count];
  result[count++] = 'o';
  result[count++] = 'u';
  result[count++] = 't';
  result[count++] = '/';
  while(count < len1+4)
  {
    result[count] = inppath[count-4];
    count++;
  }
  result[len1+4] = '\0';
  return result;
}



int main(int argc, char* argv[])
{
  if(argc <= 3)
  {
    fprintf(stderr, "Invalid arguments\n");
  }

  int inputType = atoi(argv[2]);
  for(int i = 3; i<argc-(inputType-1); i+=inputType)
  {
    char *inputpath = argv[i];
    char *outputpath;
    if(inputType == 2)
    {
      outputpath = argv[i+1];
    }
    else if(inputType == 1)
    {
      outputpath = concatenate(inputpath);
    }
    else
    {
      fprintf(stderr, "Invalid args: %d %s\n", inputType, argv[2]);
      return 1;
    }
    image inputimg = readPPM(inputpath);
    
    if(atoi(argv[1]) <= 1)
    {
      int centerRows = (inputimg->img->rows>=256)? 256:(inputimg->img->rows)/2;
      int centerCols = (inputimg->img->cols>=256)? 256:(inputimg->img->cols)/2;
  
      int flipResult = applyCenterFlipping(inputimg, centerRows, centerCols);
      if(flipResult != 0)
      {
        fprintf(stderr, "Error applying flipping transformation");
        return 1;
      }
    }
    
    if(atoi(argv[1])%2 == 0)
    {
      int blurResult = applyBlur(inputimg);
      if(blurResult != 0)
      {
	fprintf(stderr, "Error applying blur transformation");
	return 1;
      }
    }
  
    writePPM(inputimg, outputpath);
    if(inputType == 1)
      free(outputpath);
    freeimage(inputimg);
  }

  return 0;
}
