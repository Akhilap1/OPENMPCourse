
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
typedef double TYPE;
#define MAX_VAL 1000
#define MIN_VAL 1

//**********************************************************************
// Initialise a matrix to random real values between MIN_VAL and MAX_VAL
//**********************************************************************
TYPE** initMatrix(int rows, int cols)
{
	TYPE** matrix = new TYPE*[rows];
	for(int i = 0; i < rows; ++i)
    	matrix[i] = new TYPE[cols];
	//Random seed
	//srandom(time(0)+clock()+random());

	#pragma omp parallel for
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			matrix[i][j] = rand() % MAX_VAL + MIN_VAL;
		}
	}

	return matrix;

}
//**********************************************************************
// Initialise a vector to random real values between MIN_VAL and MAX_VAL
//**********************************************************************
TYPE* initVector(int size)
{
	TYPE* vector = new TYPE[size];
	#pragma omp parallel for
	for(int i=0; i<size; i++){
		vector[i] = rand() % MAX_VAL + MIN_VAL;
	}

	return vector;

}
//**********************************************************************
// Initialise a vector to zeroes
//**********************************************************************
TYPE* initVectorToZeroes(int size)
{
	TYPE* vector = new TYPE[size];
	#pragma omp parallel for
	for(int i=0; i<size; i++){
		vector[i] = 0.0;
	}

	return vector;

}
//**********************************************************************
// Initialise a matrix to zeroes
//**********************************************************************
TYPE** initMatrixToZeroes(int rows, int cols)
{
	TYPE** matrix = new TYPE*[rows];
	for(int i = 0; i < rows; ++i)
    	matrix[i] = new TYPE[cols];
	#pragma omp parallel for
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			matrix[i][j] = 0.0;
		}
	}

	return matrix;

}

//****************************************************************************80
//  Purpose:
//    MXVParallel_OPENMP computes y = A * x, using OpenMP parallel directives.
//  Parameters:
//    Input, int M, N, the number of rows and columns 
//    of the matrix.
//    Input, double A[M*N], the matrix.
//    Input, double X[N], the vector to be multiplied.
//    Output, double Y[M], the product vector.
//****************************************************************************80
void MXVParallel_OMPENP(int NROWS, int NCOLS, TYPE** A, TYPE* B, TYPE* C)
{
  int i;
  int j;
  double start_secs, end_secs;

 // printf("Starting OMP Parallel matrix -vector multiplication example\n");
  start_secs = omp_get_wtime();
# pragma omp parallel shared (NROWS,NCOLS,A,B,C) private ( i, j )
  {
    #pragma omp single
    {
      //printf("Number of Threads = %d \n",omp_get_num_threads());
	}
	# pragma omp for
	  for ( i = 0; i < NROWS; i++ )
	  {
		C[i] = (TYPE)0.0;
		for ( j = 0; j < NCOLS; j++ )
		{
		  C[i] = C[i] + A[i][j] * B[j];
		}
	  }
  }
  end_secs = omp_get_wtime ( );
  //printf("Matrix Multiplication for %d ROWS and %d COLS took %5.2f seconds\n", NROWS, NCOLS, end_secs - start_secs);

  return;
}


//****************************************************************************
//****************************************************************************
int main(int argc, char *argv[])
{
	int i;
	int NROWS; //Number of rows in matrix M
	int NCOLS; //Number of columns in matrix M
	double seconds, start_secs, end_secs;

    NROWS = atoi(argv[1]);
    NCOLS = atoi(argv[2]);

	//printf("Initializing matrices...\n");
	start_secs = omp_get_wtime ( );
	TYPE** A = initMatrix(NROWS, NCOLS);
	TYPE* B = initVector(NCOLS);
	TYPE* C = initVectorToZeroes(NROWS);

	end_secs = omp_get_wtime ( );
	printf("Matrix Intitialization took %5.2f seconds\n", end_secs - start_secs);
    start_secs = omp_get_wtime ( );
	MXVParallel_OMPENP(NROWS,NCOLS, A,B,C);
	end_secs = omp_get_wtime ( );
	printf("MXV_PARALLEL_OPENMP");
    printf("NROWS = %d  NCOLS= %d  TIME TAKEN = %f Seconds\n",NROWS,NCOLS, end_secs - start_secs);

    return(0);
}
