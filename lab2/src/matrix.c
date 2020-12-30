#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

matrix emptyMatrixStruct() //allocates space for a matrix struct and returns pointer to it.
{
  matrix res = (matrix)malloc(sizeof(struct matrix));
  if(!res) return NULL;
  return res;
}

int matrixalloc(matrix X)
{
  if(X == NULL) return 1;
  if(X->rows <= 0 || X->cols <=0)
    return 2;
  int rows = X->rows;
  int cols = X->cols;
  X->R = (int**)malloc(sizeof(int*)*rows);
  if(!(X->R)) return 2;
  X->G = (int**)malloc(sizeof(int*)*rows);
  if(!(X->G)) return 2;
  X->B = (int**)malloc(sizeof(int*)*rows);
  if(!(X->B)) return 2;
  for(int i = 0; i<rows; i++)
  {
    X->R[i] = (int*)malloc(sizeof(int)*cols);
    if(!(X->R[i])) return 2;
    X->G[i] = (int*)malloc(sizeof(int)*cols);
    if(!(X->G[i])) return 2;
    X->B[i] = (int*)malloc(sizeof(int)*cols);
    if(!(X->B[i])) return 2;
  }
  for(int i=0; i<rows; i++)
  {
    for(int j=0; j<cols; j++)
    {
      X->R[i][j]=0;
      X->G[i][j]=0;
      X->B[i][j]=0;
    }
  }
  return 0;
}

matrix creatematrix(int rows,int cols)
{
  matrix res = emptyMatrixStruct();
  if(!res) return NULL;
  res->rows = rows;
  res->cols = cols;
  if(matrixalloc(res) != 0) return NULL;
  return res;
}


matrix paddedSlidingWindow(matrix input, int windowSize, int padVal)
{
  if(!input) return input;
  int padding = windowSize/2;
  if(padding < 0 || input == NULL)
  {
    fprintf(stderr, "Incorrect inputs. Terminating.");
    return input;
  }
  int rows = input->rows;
  int cols = input->cols;
  
  matrix result = creatematrix(windowSize*windowSize, rows*cols);
  if(!result) return NULL;
  
  int resultColIdx = 0;
  for(int i = 0; i<rows; i++)
  {
    for (int j = 0; j<cols; j++)
    {
      int resultRowIdx = 0;
      for(int rowIdx = i-padding; rowIdx <= i+padding; rowIdx++)
      {
	for(int colIdx = j-padding; colIdx <= j+padding; colIdx++)
	{
	  int valueR = padVal;
	  int valueG = padVal;
	  int valueB = padVal;
	  if((rowIdx>=0 && rowIdx < rows) && (colIdx >= 0 && colIdx < cols))
	  {
	    valueR = input->R[rowIdx][colIdx];
	    valueG = input->G[rowIdx][colIdx];
	    valueB = input->B[rowIdx][colIdx];
	  }
	  result->R[resultRowIdx][resultColIdx] = valueR;
	  result->G[resultRowIdx][resultColIdx] = valueG;
	  result->B[resultRowIdx][resultColIdx] = valueB;

	  resultRowIdx++; 
	}
      }
      resultColIdx++;
    }
  }

  if(resultColIdx != rows*cols)
  {
    freematrix(result);
    fprintf(stderr,"Error in generating sliding window matrix. Aborting operation.\n");
    return input;
  }

  return result;
  
}

matrix flatten(matrix input)
{
  if(!input)
  {
    fprintf(stderr, "No matrix given. Aborted.\n");
    return input;
  }
  
  int rows = input->rows;
  int cols = input->cols;

  matrix result = creatematrix(1, rows*cols);

  int idx = 0;
  for(int i = 0; i<rows; i++)
  {
    for(int j = 0; j<cols; j++)
    {
      result->R[0][idx] = input->R[i][j];
      result->G[0][idx] = input->G[i][j];
      result->B[0][idx] = input->B[i][j];
      idx++;
    }
  }
  if(idx != result->cols)
  {
    freematrix(result);
    fprintf(stderr, "Error in flattening matrix. Aboring.");
    return input;
  }
  return result;
}

matrix reshape(matrix input, int rows, int cols)
{
  if(!input)
  {
    fprintf(stderr, "No matrix given. Aborted.\n");
    return input;
  }
  
  if(input->rows != 1 || rows*cols != input->cols)
  {
    fprintf(stderr, "Incorrect input. Aborted operation.\n");
    return input;
  }

  matrix result = creatematrix(rows, cols);
  int idx = 0;
  for(int i = 0; i<rows; i++)
  {
    for(int j = 0; j<cols; j++)
    {
      result->R[i][j] = input->R[0][idx];
      result->G[i][j] = input->G[0][idx];
      result->B[i][j] = input->B[0][idx];
      idx++;
    }
  }

  if(idx != input->cols)
  {
    fprintf(stderr, "Some error occurred. Aborting.\n");
    freematrix(result);
    return input;
  }
  return result;
}

int swapCenter(matrix img, matrix center)
{
  int cenRows = center->rows;
  int cenCols = center->cols;
  int rows = img->rows;
  int cols = img->cols;
  if(cenRows > rows || cenCols >cols)
    return 1;
  int rowStart = (rows-cenRows)/2;
  int colStart = (cols-cenCols)/2;
  int rowEnd = rowStart+cenRows;
  int colEnd = colStart+cenCols;
  int rowIdx = 0;
  int colIdx = 0;
  for(int i = rowStart; i<rowEnd; i++)
  {
    colIdx = 0;
    for(int j = colStart; j<colEnd; j++)
    {
      img->R[i][j] = center->R[rowIdx][colIdx];
      img->G[i][j] = center->G[rowIdx][colIdx];
      img->B[i][j] = center->B[rowIdx][colIdx];
      colIdx++;
    }
    rowIdx++;
  }
  if(rowIdx == cenRows && colIdx == cenCols)
    return 0;
  else return 2;
}

int fillCenter(matrix img, matrix center)
{
  int cenRows = center->rows;
  int cenCols = center->cols;
  int rows = img->rows;
  int cols = img->cols;
  if(cenRows > rows || cenCols >cols)
    return 1;
  int rowStart = (rows-cenRows)/2;
  int colStart = (cols-cenCols)/2;
  int rowEnd = rowStart+cenRows;
  int colEnd = colStart+cenCols;
  int rowIdx = 0;
  int colIdx = 0;
  for(int i = rowStart; i<rowEnd; i++)
  {
    colIdx = 0;
    for(int j = colStart; j<colEnd; j++)
    {
      center->R[rowIdx][colIdx] = img->R[i][j];
      center->G[rowIdx][colIdx] = img->G[i][j];
      center->B[rowIdx][colIdx] = img->B[i][j];
      colIdx++;
    }
    rowIdx++;
  }
  if(rowIdx == cenRows && colIdx == cenCols)
    return 0;
  else return 2;
}

void freematrix(matrix X)
{
  if(X == NULL) return;
  for(int i = 0; i<X->rows; i++)
  {
    if(X->R[i] != NULL) free(X->R[i]);
    if(X->G[i] != NULL) free(X->G[i]);
    if(X->B[i] != NULL) free(X->B[i]);
  }
  if(X->R != NULL) free(X->R);
  if(X->G != NULL) free(X->G);
  if(X->B != NULL) free(X->B);
  free(X);
}
