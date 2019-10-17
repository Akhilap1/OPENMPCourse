
#include <stdio.h>
#include <omp.h>
#include <ctime>

/*******************************************************************
///Author: Akhila Prabhakaran
// Exercise 6:  Returns the Nth number of the Fibonacci series
//
// The Fibonacci sequence is recursively defined as
//  F(0)=1,F(1)=1,F(n)=F(n−1)+F(n−2) for n≥2.
//  (1)F(0)=1,F(1)=1,F(n)=F(n−1)+F(n−2) for n≥2.
//
// N is initialised to an integer in main()
//
// Functions: 
// 1. long naiveSerialFib(int N) <= Serial function that conputes Fibonacci[N]
// The above is a naive function, because it uses recursion to repeatedly call fib[N-1] and fib[n-2]
// Most of the time the fib of a number is computed more than once. The code includes a counter that
// keeps track of how many times the fibonacci of a given integer is computed.
// This is stored in the global(shared) array repeatComputation.
// repeatComputation[i] tells how many times the fibonacci of "i" was computed, in the process of
// calculating fibonacci(N)
//
// 2. long optimalSerialFib(int N) <= Serial function that computes Fibonacci[N]
// This is an optimal function (not the bestest one, but much better than the naive function).
// A shared array (long* computedVals)  is implemented to store the computed value of Fibonacci[i] for 0<= i <= N
// Before computing Fibonacci[i] , the function checks if it has already been computed and reuses that value.
//
// By running the following program. it can be seen that the naive function has a very poor performance when compared
// to the optimal serial function.
//
// 3. long parallelNaiveFibWithTasks(int N) => The naive serial function is converted to a parallel function
// using openmp tasks. Performance is expected to be worse than the serial code because of too many tasks
// being created and too many repeated computations of Fibonacci for i < N
// Sometimes this even fails to return.(try N > 40)
//
// 4. long parallelFibWithTasks(int N) => improved version using an array of computed Fibonacci values
// Much better than function 3 in terms of performance. But has a lot of overhead for creation of tasks.
// Interestngly, the function also stores the number of Fibonacci computations done per Thread.
// loadbalancing in OPENMP is an important concept that drives improvement in performance.
//
// TODO:
// 1. Fix the number of threads  = number of processors (just let it default)
// 2. Run the optimal parallel and serial functions for various values of N (25 through 50)
// 3. Document the serial and parallel compute times.
// 4. Calculate speed up for parallel code for various N
// 5. Do you see any patterns in speedup?
// Can the parallel code be further optimised?
//   Hint:based on my experience, optimised serial code works well for N < 30. Parallel code works well for N>= 30
//   One could add an "if" clause in the #pragma omp task directive to see if they get an improvement in speedup
//////////////////////////////////////////////////////////////////
*/

int* repeatComputation;
long* computedVals;
int* iterationsPerThread;

/******************************************************************
The basic single-threaded recursive solution. 

******************************************************************/
long naiveSerialFib(int N)
{

  if (N >=2) {

     repeatComputation[N]++;
     return (naiveSerialFib(N-1) + naiveSerialFib(N-2));
  }
  else {
    repeatComputation[N]++;
  	return 1;
  }

}

/********************************************************************
The optimal single-threaded recursive solution. Where we store Fib[i]
once it is computed and reuse that value when there is an attempt to
compute it again
******************************************************************/
long optimalSerialFib(int N)
{

  if (N >=2) 
  {
 
     if (computedVals[N-1] == -1)
     {
        repeatComputation[N-1]++;
		computedVals[N-1] = optimalSerialFib(N-1);
     }
     if (computedVals[N-2] == -1)
     {
        repeatComputation[N-2]++;
		computedVals[N-2] = optimalSerialFib(N-2);
     }

     return (computedVals[N-1] + computedVals[N-2]);
  }
  else 
  {
    if (computedVals[N] == -1)
    {
	    repeatComputation[N]++;
        computedVals[N] = 1;

    }
    
  	return computedVals[N];
  }

}

/*********************************************************
The OpenMP parallel solution calls for two different ideas. 
First of all, we parallelize the recursion by using tasks
This computes the right solution, but, as in the naive single-threaded solution, 
it recomputes many of the intermediate values.
*********************************************************/
long parallelNaiveFibWithTasks(int N)
{

  long i, j;
  if (N>=2) {
#pragma omp task shared(i) firstprivate(N)
    i=parallelNaiveFibWithTasks(N-1);
#pragma omp task shared(j) firstprivate(N)
    j=parallelNaiveFibWithTasks(N-2);
#pragma omp taskwait
    return i+j;
  }
  else
  {
    return N;
  }


}

/******************************************************************
// Parallel Fibonacci with tasks - Work Sharing Construct
// Typical function that could be used for most recursive function calls
******************************************************************/
long parallelFibWithTasks(int N)
{
  long val1, val2;
  int CurrentThread = omp_get_thread_num();
  #pragma omp atomic
  iterationsPerThread[CurrentThread]++;


 if (N >=2) 
  {
 
     #pragma omp task shared(computedVals) firstprivate(N)
     if (computedVals[N-1] == -1)
     {
        val1 =parallelFibWithTasks(N-1);
        #pragma omp critical
		computedVals[N-1] = val1;
     }
     #pragma omp task shared(computedVals) firstprivate(N)
     if (computedVals[N-2] == -1)
     {
        val2= parallelFibWithTasks(N-2);
        #pragma omp critical
		computedVals[N-2] = val2;
     }
     #pragma omp taskwait
     return (computedVals[N-1] + computedVals[N-2]);
  }
  else 
  {
    if (computedVals[N] == -1)
    {
        #pragma omp critical
        computedVals[N] = 1;
    }
    
  	return computedVals[N];
  }
}

/******************************************************************
Main
*******************************************************************/
int main()
{
  int N = 35;
  double start = 0;
  double end = 0;
  int i = 0;

  repeatComputation = new int[N];
  computedVals = new long[N];
  
  //***********************************************************************
  // Naive Serial Code
  //***********************************************************************
  //Some initialization  
  for (i =0; i < N; i++)
  {
    computedVals[i] = -1;
	repeatComputation[i] = 0;
  }
  //always time the code
  start = omp_get_wtime();
  
  //function call
  long FIB = naiveSerialFib(N);
  
  //time again
  end = omp_get_wtime();
  printf("Fibonacci[%d] = %lu \n", N, FIB);
  printf("Time taken to compute Serial naive FIB[%d] is %f seconds \n", N, end-start);

  for (i = 0; i < N; i++)
    printf("%d was computed %d times \n", i, repeatComputation[i]);


  //***********************************************************************
  // Optimal Serial Code
  //***********************************************************************
  //Some initialization  
  for (i =0; i < N; i++)
  {
    computedVals[i] = -1;
	repeatComputation[i] = 0;
  }
  //always time the code
  start = omp_get_wtime();
  
  //function call
  FIB = optimalSerialFib(N);
  
  //time again
  end = omp_get_wtime();
  printf("Fibonacci[%d] = %lu \n", N, FIB);
  printf("Time taken to compute Serial optimised FIB[%d] is %f seconds \n", N, end-start);

  for (i = 0; i < N; i++)
    printf("%d was computed %d times \n", i, repeatComputation[i]);


  //***********************************************************************
  //  OMENMP Parallel Region - naive parallel code
  //***********************************************************************
  printf("OPENMP Tasks for Fibonacci computation - naive implementation \n");
  #pragma omp parallel
  {
	  //**********************************************************************
	  // Parallel region begins here
      // We do not know how many threads are spwaned - this will be taken from
      // environment variables. We will just let it default.
      // Alternatively, the user could change or control this before the parallel region
	  //**********************************************************************'
	  int NUM_THREADS =  omp_get_num_threads();
      printf("Starting OPENMP Parallel Region with %d Threads \n", NUM_THREADS);

      //Intilialization********************************
      //intialize just once - use single or master here
      #pragma omp master
      {
		  iterationsPerThread = new int[NUM_THREADS];
		  //Some initialization  
		  for (i =0; i < N; i++)
		  {
			computedVals[i] = -1;
			repeatComputation[i] = 0;
		  }
		  for (i - 0; i < NUM_THREADS; i++)
		  {
			iterationsPerThread[i] =0;		    
		  }
		  //Call to parallel fibonacci recursive function also just once
		  start = omp_get_wtime();
          FIB = parallelNaiveFibWithTasks(N);
		  end = omp_get_wtime();
		  printf("Fibonacci[%d] = %lu \n", N, FIB);
		  printf("Time taken to compute parallel FIB[%d] using tasks is %f seconds \n", N, end-start);
		  for (i = 0; i < NUM_THREADS; i++)
    		 printf("Thread %d processed %d TASKS \n", i, iterationsPerThread[i]);
 
      }
    
  }


  printf("OPENMP Tasks for Fibonacci computation - improved implementation \n");
  //***********************************************************************
  //  OMENMP Parallel Region - optimal parallel code
  //***********************************************************************
  #pragma omp parallel
  {
	  //**********************************************************************
	  // Parallel region begins here
      // We do not know how many threads are spwaned - this will be taken from
      // environment variables. We will just let it default.
      // Alternatively, the user could change or control this before the parallel region
	  //**********************************************************************'
	  int NUM_THREADS =  omp_get_num_threads();
      printf("Starting OPENMp Parallel Region with %d Threads \n", NUM_THREADS);

      //Intilialization********************************
      //intialize just once - use single or master here
      #pragma omp master
      {
		  iterationsPerThread = new int[NUM_THREADS];
		  //Some initialization  
		  for (i =0; i < N; i++)
		  {
			computedVals[i] = -1;
			repeatComputation[i] = 0;
		  }
		  for (i - 0; i < NUM_THREADS; i++)
		  {
			iterationsPerThread[i] =0;		    
		  }
		  //Call to parallel fibonacci recursive function also just once
		  start = omp_get_wtime();
          FIB = parallelFibWithTasks(N);
		  end = omp_get_wtime();
		  printf("Fibonacci[%d] = %lu \n", N, FIB);
		  printf("Time taken to compute parallel FIB[%d] using tasks is %f seconds \n", N, end-start);
		  for (i = 0; i < NUM_THREADS; i++)
    		 printf("Thread %d processed %d TASKS \n", i, iterationsPerThread[i]);
 
      }
    
  }

  return(0);
}

