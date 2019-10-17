//***************************************************************************************************************
//
//Akhila Prabhakaran OPENMP Course SERC May 2019
//
//Exercise 1: Familiarization with openmp pragmas, run time and environment variables, shared variables and barriers, master and single
//Duration: 20 -30 minutes
//
//Login to the workshop machine 
//  ssh username@delta-cluster.serc.iisc.ernet.in
//  enter the password provided
//
//  pwd (print working directory)
//  mkdir username_2019MayOMP
//  cd username_2019MayOMP
//use an editor like vi to write a program "MyHelloWorld.cpp" [vi MyHelloWorld.cpp]
//First write a serial program that prints "hello world!"
//Parallelise the program by making the following changes
//  1. add header file
//  2. add a parallel region using #pragma omp parallel outside the printf("Hello World!")
//  3. identify the number of processors in the machine using omp_get_num_procs() //Runtime function
//  4. print number of processors
//  5. print max number of threads (default) WITHOUT setting any environment variables using omp_get_max_threads() //runtime function
//     By default, most compilers will create 1 thread for each core.
//  6. Compile the program using g++ -fopenmp MyHelloWorld.cpp -o MyHelloWorld.exe
//  7. Run ./MyHelloWorld.exe and report the output
//
//Part B: Use of omp_get_thread_num() inside the parallel region
//  8. declare int tid = 0; before the parallel region (variable that holds the thread id of each thread in the parallel region)
//  9. Edit the code to include the "private clause" by changing #pragma omp parallel to 
//     #pragma omp parallel private (tid)
//  10.Inside the parallel region add the following code
//     id = omp_get_thread_num();
//  11.Change the printf or cout statement in the parallel region as follows
//     printf("Hello World from thread = %d\n", tid);
//  12. Compile and re-run the program. Run multiple times to see the changing order of tid.
//
//Part C:Use of environment variables to override the number of threads in code to something other than max threads printed in part B
//  13. bash: export OMP_NUM_THREADS=<number of threads to use>
//      csh or tcsh shell, enter: setenv OMP_NUM_THREADS <number of threads to use>
//  14. Now run the exe (NO COMPILATION Required as no code has changed here). Report the number of threads. Is it the same as the ENV Variable?
//
//Part D:Use the num_threads clause in #pragma omp parallel to override number of threads in Part C
//  15. Change #pragma omp parallel to #pragma omp parallel num_threads(10)   //ensure that 10 is different from the env variable in Part C and max threads in part B
//  16. Compile and run the program. Report results
//
//Part E:Use of #pragma omp master
//  17. inside the parallel region after the printf("hello world from thread =%d", tid); add the following
//      #pragma omp master //if (tid == 0) 
//	    {
//		  nthreads = omp_get_num_threads();
//		  printf("Number of threads inside parallel region= %d\n", nthreads);
//	    }
//   18. Compile and Run the program again and report results
//
//Part F: use of barrier
//   19. Add a #pragma omp barrier before the #pragma omp master
//   20. Compile, Run and Report results
//
//Part G: use of single
//   21: Change #pragma omp master to #pragma omp single & print the thread id too
//
//*******************************************************************************************************************
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
  int nthreads, tid;

  printf("Number of Processors = %d \n",omp_get_num_procs());
  printf("Max Number of Threads = %d \n",omp_get_max_threads());
  printf("Number of Threads outside parallel region = %d \n",omp_get_num_threads());
  /* Fork a team of threads giving them their own copies of variables */
  #pragma omp parallel num_threads(10) private(nthreads, tid)
  {

	  /* Obtain thread number */
	  tid = omp_get_thread_num();
	  printf("Hello World from thread = %d\n", tid);

      #pragma omp barrier

	  /* Only master thread does this */
	  #pragma omp master //if (tid == 0) 
	  {
		 nthreads = omp_get_num_threads();
		 printf("Number of threads inside parallel region= %d\n", nthreads);
	  }

  }  /* All threads join master thread and disband */

}

