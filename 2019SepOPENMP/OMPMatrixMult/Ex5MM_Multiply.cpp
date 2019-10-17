/******************************************************************************
/Author: Akhila Prabhakaran
// Exercise 5: Matrix Matrix Multiplication (With Solution)
// Time: 30 minutes
// The following code has a function that computes the product of 2 matrices A and B
// Function Name: MatrixMultSerial(int NRA, int NCA, int NCB)
// input: 
//	Number of Rows of Matrix A (NRA)
//	Number of Cols of Matrix A (NCA)
//	Number of Cols of Matrix B (NCB)
// Process: 
	generates the 2 matrices A and B using random numbers
    multiplies the 2 matrices to compute the product
// output:
//  Result of matrix multiplication is written to a file MatrixMultiplyOutput.txt
//  returns the time taken in seconds to complete Matrix Multiplication
//
//1. Compile and run the program and see the time taken for the serial code 
     g++ -fopenmp MatrixMultiplySchedule.c -o MMSchedule.exe
//2. Write a function to perform Matrix Multiplication in parallel (see MatrixMultWithOMPFOR(...))
//   using #pragma omp for with NO schedule clause
//   Which variables are private and which ones need to be shared - elaborate
//3. The function should have the same inputs as the serial function with an additional input
//   parameter - int numThreads. This will allow you to time the parallel code for various number of threads.
//4. outputs to the parallel function should be similar to the serial code
//  	Result of matrix multiplication is written to a file MatrixMultiplyOutput.txt
//  	returns the time taken in seconds to complete Matrix Multiplication
//5. In the main() function add code to do the following:
//	   For fixed matrices of size NRA x NCA and NCA x NCB
//      call the serialFunction to multiply matrices
//      for various number fo threads (2,3,4,5,6,....number of processors in machine)
//         call parallelFunction to multiply matrices
//         compute speedup(t) = Time Takes By Serial Code / Time taken by parallel code with t threads
//         print the speedup
//6. How does speedup change with increasing number of threads?
//7. What would you expect to happen if the number of threads was increased to more than the number of cores?
//8. Do you think there will be a change in the speed up if Schedule - chunk clause was used?
//9. Add a new function which is similar to the parallel function, but accepts as input a chunk size too
//10.  double MatrixMultWithScheduleChunk(int num_threads, int chunk_size, int NRA, int NCA, int NCB)
//11. add a static schedule with chunk size in the #pragma omp for
//12. Evaluate speedup for various chunk sizes.
//13. Discuss
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
typedef double TYPE;
#define MAX_VAL 1000
#define MIN_VAL 1


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
//***************************************************************************************
// Serial code for multiplying 2 matrices of size NRA x NCA and NCA X NCB
// Will return the time taken to multiply the matrices and will save the result matrix into 
// a file
//***************************************************************************************
double MatrixMultSerial(int NRA, int NCA, int NCB)
{

	int	tid, nthreads, i, j, k, chunk;
	double start_secs, end_secs;

    printf("Starting matrix multiple example with 1 thread\n");
	printf("Initializing matrices...\n");
    start_secs = omp_get_wtime ( );

    TYPE** a = initMatrix(NRA, NCA);
    TYPE** b = initMatrix(NCA, NCB);
    TYPE** c = initMatrix(NRA, NCB);

   end_secs = omp_get_wtime ( );
   printf("Matrix Intitialization took %5.2f seconds\n", end_secs - start_secs);

   /*** Do matrix multiply sharing iterations on outer loop ***/
   /*** Display who does which iterations for demonstration purposes ***/

   start_secs = omp_get_wtime ( );
	  for (int i=0; i<NRA; i++)    
		{
		 for(int j=0; j<NCB; j++)       
		   for (int k=0; k<NCA; k++)
		    c[i][j] += a[i][k] * b[k][j];
		}


      end_secs = omp_get_wtime ( );
     printf("Matrix Multiplication took %5.2f seconds\n", end_secs - start_secs);
	
    /*** Print results ***/
    FILE *fptr = fopen("MatrixMultiplyOutput.txt", "w"); 
    if (fptr == NULL) 
    { 
        printf("Could not open file"); 
        return 0; 
    } 
  
	fprintf(fptr, "******************************************************\n");
	fprintf(fptr, "Result Matrix:\n");
	for (int i=0; i<NRA; i++)
	{
	  for (int j=0; j<NCB; j++) 
		fprintf(fptr, "%6.2f   ", c[i][j]);
	  fprintf(fptr, "\n"); 
	}
    fclose(fptr); 	
    printf("******************************************************\n");

    return(end_secs-start_secs);
}

//*******************************************************************************************
// Parallel code for matrix multiplication using #pragma omp for with the default schedule
// where the iterations are divided equally among the various threads.
// Number of Threads is passed as a parameter to the function to allow us to evaluate speed up
// of parallel code for various number of threads.
// 
//*******************************************************************************************
double MatrixMultWithOMPFOR(int num_threads, int NRA, int NCA, int NCB)
{

	int	tid, nthreads, i, j, k, chunk;
	double start_secs, end_secs;


	printf("Initializing matrices...\n");
    start_secs = omp_get_wtime ( );

    TYPE** a = initMatrix(NRA, NCA);
    TYPE** b = initMatrix(NCA, NCB);
    TYPE** c = initMatrix(NRA, NCB);

    end_secs = omp_get_wtime ( );
    double initTime = end_secs - start_secs;

    omp_set_num_threads(num_threads);

	/*** Spawn a parallel region explicitly scoping all variables ***/
	#pragma omp parallel shared(a,b,c,nthreads) private(tid,i,j,k)
	  {
	  tid = omp_get_thread_num();
	  if (tid == 0)
		{
  		  nthreads = omp_get_num_threads();
		  printf("Starting matrix multiple example with %d threads\n",nthreads);
		}

      #pragma omp master
      printf("Matrix Intitialization took %5.2f seconds\n",initTime);

	  /*** Do matrix multiply sharing iterations on outer loop ***/
	  /*** Display who does which iterations for demonstration purposes ***/

      start_secs = omp_get_wtime ( );
	//  printf("Thread %d starting matrix multiply...\n",tid);
	  #pragma omp for 
	  for (i=0; i<NRA; i++)    
		{
	//	printf("Thread=%d did row=%d\n",tid,i);
		for(j=0; j<NCB; j++)       
		  for (k=0; k<NCA; k++)
		    c[i][j] += a[i][k] * b[k][j];
		}
	  }   /*** End of parallel region ***/

      end_secs = omp_get_wtime ( );
     printf("Matrix Multiplication took %5.2f seconds\n", end_secs - start_secs);
	/*** Print results to file ***/

    
    FILE *fptr = fopen("MatrixMultiplyOutput.txt", "w"); 
    if (fptr == NULL) 
    { 
        printf("Could not open file"); 
        return 0; 
    } 
  
	fprintf(fptr, "******************************************************\n");
	fprintf(fptr, "Result Matrix:\n");
	for (i=0; i<NRA; i++)
	{
	  for (j=0; j<NCB; j++) 
		fprintf(fptr, "%6.2f   ", c[i][j]);
	  fprintf(fptr, "\n"); 
	}
    fclose(fptr); 


	//printf ("Done.\n");
    return(end_secs-start_secs);

}

//***************************************************************************************
//
//***************************************************************************************
double MatrixMultWithScheduleChunk(int num_threads, int chunk_size, int NRA, int NCA, int NCB)
{

	int	tid, nthreads, i, j, k, chunk;
	double start_secs, end_secs;

	printf("Initializing matrices...\n");
    start_secs = omp_get_wtime ( );

    TYPE** a = initMatrix(NRA, NCA);
    TYPE** b = initMatrix(NCA, NCB);
    TYPE** c = initMatrix(NRA, NCB);

    end_secs = omp_get_wtime ( );
    double initTime =end_secs - start_secs;
	chunk = chunk_size;        /* set loop iteration chunk size */
    omp_set_num_threads(num_threads);

	/*** Spawn a parallel region explicitly scoping all variables ***/
	#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
	  {
	  tid = omp_get_thread_num();
	  if (tid == 0)
		{
		nthreads = omp_get_num_threads();
		printf("Starting matrix multiple example with %d threads\n",nthreads);

		}

         #pragma omp master
         printf("Matrix Intitialization took %5.2f seconds\n", initTime);

	  /*** Do matrix multiply sharing iterations on outer loop ***/
	  /*** Display who does which iterations for demonstration purposes ***/

      start_secs = omp_get_wtime ( );
	//  printf("Thread %d starting matrix multiply...\n",tid);
	  #pragma omp for schedule (static, chunk)
	  for (i=0; i<NRA; i++)    
		{
	//	printf("Thread=%d did row=%d\n",tid,i);
		for(j=0; j<NCB; j++)       
		  for (k=0; k<NCA; k++)
		    c[i][j] += a[i][k] * b[k][j];
		}
	  }   /*** End of parallel region ***/

      end_secs = omp_get_wtime ( );
     double time_taken = end_secs - start_secs;
     printf("Matrix Multiplication took %5.2f seconds\n", time_taken);
	/*** Print results to file ***/

    
    FILE *fptr = fopen("MatrixMultiplyOutput.txt", "w"); 
    if (fptr == NULL) 
    { 
        printf("Could not open file"); 
        return 0; 
    } 
  
	fprintf(fptr, "******************************************************\n");
	fprintf(fptr, "Result Matrix:\n");
	for (i=0; i<NRA; i++)
	{
	  for (j=0; j<NCB; j++) 
		fprintf(fptr, "%6.2f   ", c[i][j]);
	  fprintf(fptr, "\n"); 
	}
    fclose(fptr); 

	printf("******************************************************\n");
	//printf ("Done.\n");
    return(time_taken);

}

//*******************************************************************************
//
//
//*******************************************************************************
int main () 
{
    //cout << "You have entered " << argc 
    //     << " arguments:" << "\n"; 
  
    //for (int i = 0; i < argc; ++i) 
    //    cout << argv[i] << "\n"; 
  
  
   int NRA =1000;                 /* number of rows in matrix A */
   int NCA =1000;                /* number of columns in matrix A */
   int NCB =500;                /* number of columns in matrix B */
   double parallelTime, serialTime, speedup;

   printf("Matrix Multiplication  [%d X %d] * [%d X %d] \n", NRA, NCA,NCA, NCB );
   printf("Serial Code......");
   serialTime = MatrixMultSerial(NRA, NCA, NCB);
   printf("******************************************************\n");
   printf("Parallel Code #pragma omp for.....");
   for (int i = 4; i < 32; i=i+2)
   {
      printf("Number of Threads %d \n", i);
      parallelTime = MatrixMultWithOMPFOR(i, NRA, NCA, NCB);
      speedup =serialTime/parallelTime;
      printf("SpeedUp with %d Threads = %f/%f = %f \n", i, serialTime, parallelTime, speedup);
      printf("******************************************************\n");
   }
   
   printf("Parallel Code #pragma omp for schedule(chunk = 5).....");
   for (int i = 4; i < 32; i=i+2)
   {
      printf("Number of Threads %d \n", i);
      parallelTime = MatrixMultWithScheduleChunk(i,5, NRA, NCA, NCB);
      speedup = serialTime/parallelTime;
      printf("SpeedUp with %d Threads = %6.2f \n", i, speedup);
      printf("******************************************************\n");

   }
   printf("Parallel Code #pragma omp for schedule(chunk = 10).....");
   for (int i = 4; i < 32; i=i+2)
   {
      printf("Number of Threads %d \n", i);
      parallelTime = MatrixMultWithScheduleChunk(i,10, NRA, NCA, NCB);
      printf("SpeedUp with %d Threads = %6.2f \n", i, serialTime/parallelTime);
      printf("******************************************************\n");

   }

   return 0; 
}
