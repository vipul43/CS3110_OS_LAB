#ifndef MATRIX
#define MATRIX

typedef struct matrix
{
  unsigned int rows;
  unsigned int cols;
  int **R;
  int **G;
  int **B;
}* matrix;

int matrixalloc(matrix);
matrix creatematrix(int rows, int cols);
matrix emptyMatrixStruct();
void freematrix(matrix);
matrix paddedSlidingWindow(matrix, int, int);
matrix flatten(matrix);
matrix reshape(matrix, int rows, int cols);
int fillCenter(matrix, matrix);
int swapCenter(matrix, matrix);
#endif
