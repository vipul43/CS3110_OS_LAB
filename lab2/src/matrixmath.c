#include "matrixmath.h"
#include "matrix.h"
#include "transformations.h"
#include <stdlib.h>
#include <stdio.h>

int abs(int x)
{
  if(x >=0)
    return x;
  else
    return -1*x;
}

int multiply(matrix P, matrix A, matrix B) /*multiplies A and B and stores the result in P. Note that to call this function, you need to allocate space for P struct pointer using emptyMatrixStruct()*/
{
  if(A->cols != B->rows)
  {
    return 1;
  }
  
  int rows = A->rows;
  int cols = B->cols;
  int Acols = A->cols;
  P->rows = rows;
  P->cols = cols;
  
  if(matrixalloc(P) != 0) return 2;
  
  for(int i = 0; i<rows; i++)
    for (int j = 0; j<cols; j++)
      for(int k = 0; k<Acols; k++)
      {
	P->R[i][j] += (A->R[i][k] * B->R[k][j]);
	P->G[i][j] += (A->G[i][k] * B->G[k][j]);
	P->B[i][j] += (A->B[i][k] * B->B[k][j]);
      }
  
  return 0;
}

int subtract(matrix P, matrix A, matrix B)
{
  if(A->cols != B->cols || A->rows != B->rows)
  {
    return 1;
  }
  
  int rows = A->rows;
  int cols = A->cols;
  P->rows = rows;
  P->cols = cols;
  
  if(matrixalloc(P) != 0) return 2;
  
  for(int i = 0; i<rows; i++)
    for (int j = 0; j<cols; j++)
    {
      P->R[i][j] = (A->R[i][j] - B->R[i][j]);
      P->G[i][j] = (A->G[i][j] - B->G[i][j]);
      P->B[i][j] = (A->B[i][j] - B->B[i][j]);
    }
  
  return 0;
}

int add(matrix P, matrix A, matrix B)
{
  if(A->cols != B->cols || A->rows != B->rows)
  {
    return 1;
  }
  
  int rows = A->rows;
  int cols = A->cols;
  P->rows = rows;
  P->cols = cols;
  
  if(matrixalloc(P) != 0) return 2;
  
  for(int i = 0; i<rows; i++)
    for (int j = 0; j<cols; j++)
    {
      P->R[i][j] = (A->R[i][j] + B->R[i][j]);
      P->G[i][j] = (A->G[i][j] + B->G[i][j]);
      P->B[i][j] = (A->B[i][j] + B->B[i][j]);
    }
  
  return 0;
}

matrix flippedIdentity(int n)
{
  matrix fi = creatematrix(n,n);
  if(fi == NULL) return NULL;
  for(int i = 0; i<n; i++)
    for(int j = 0; j<n; j++)
      if(i+j == n-1)
      {
	fi->R[i][j] = 1;
	fi->G[i][j] = 1;
	fi->B[i][j] = 1;
      }
      else
      {
	fi->R[i][j] = 0;
	fi->G[i][j] = 0;
	fi->B[i][j] = 0;
      }
      
  return fi;
}

matrix ones(int rows, int cols)
{
  matrix result = creatematrix(rows,cols);
  if(result == NULL) return NULL;
  
  for(int i = 0; i<rows; i++)
    for(int j = 0; j<cols; j++)
    {
	result->R[i][j] = 1;
	result->G[i][j] = 1;
	result->B[i][j] = 1;
    }
  return result;
}

int scale(matrix input, float scalar)
{
  if(!input) return 1;
  for(int i = 0; i<input->rows; i++)
  {
    for(int j = 0; j<input->cols; j++)
    {
      float Rval = (input->R[i][j])*scalar;
      float Gval = (input->G[i][j])*scalar;
      float Bval = (input->B[i][j])*scalar;
      input->R[i][j] = (int)Rval;
      input->G[i][j] = (int)Gval;
      input->B[i][j] = (int)Bval;
    }
  }
  return 0;
}

int castUchar(matrix input)
{
  if(!input) return 1;
  for(int i = 0; i<input->rows; i++)
  {
    for(int j = 0; j<input->cols; j++)
    {
      unsigned char Rval = (input->R[i][j]);
      unsigned char Gval = (input->G[i][j]);
      unsigned char Bval = (input->B[i][j]);
      input->R[i][j] = Rval;
      input->G[i][j] = Gval;
      input->B[i][j] = Bval;
    }
  }
  return 0;
}

int scaleSeparately(matrix input, float rmul, float gmul, float bmul)
{
  if(!input) return 1;
  for(int i = 0; i<input->rows; i++)
  {
    for(int j = 0; j<input->cols; j++)
    {
      float Rval = (input->R[i][j])*rmul;
      float Gval = (input->G[i][j])*gmul;
      float Bval = (input->B[i][j])*bmul;
      input->R[i][j] = (int)Rval;
      input->G[i][j] = (int)Gval;
      input->B[i][j] = (int)Bval;
    }
  }
  return 0;
}

matrix convolution(matrix kernel, matrix input, int padVal)
{
  if(kernel->rows != kernel->cols)
  {
    fprintf(stderr, "Please give a square kernel matrix\n");
    return input;
  }
  int kerneldim = kernel->rows;
  matrix slidingWindow = paddedSlidingWindow(input, kerneldim, padVal);
  matrix flattenedKernel = flatten(kernel);
  matrix product = emptyMatrixStruct();
  int mulResult = multiply(product, flattenedKernel, slidingWindow);
  freematrix(slidingWindow);
  freematrix(flattenedKernel);
  if(mulResult != 0)
  {
    fprintf(stderr, "Convolution failed. Operation Aborted.");
    return input;
  }
  matrix conv = reshape(product, input->rows, input->cols);
  freematrix(product);
  return conv;
}
