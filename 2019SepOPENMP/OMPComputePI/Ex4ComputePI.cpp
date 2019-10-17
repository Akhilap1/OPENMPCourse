//***************************************************************************************************************
//
//Akhila Prabhakaran OPENMP Course SERC May 2019
//
//Exercise 4: Simple example of numerical integration for computation of PI using openmp
//Duration: 20 minutes
//
//Login to the workshop machine 
//  ssh username@delta-cluster.serc.iisc.ernet.in
//  enter the password provided
//
//  pwd (print working directory)
//  mkdir username_2019MayOMP
//  cd username_2019MayOMP
//use an editor like vi to write a program "MyComputePI.cpp" [vi MyComputePI.cpp]
//1. First write a serial program that computes PI - based on the methodology discussed in the lecture
//   Alternatively, copy the program below and run the Serial function to compute PI
//2. Time the program and record the time for various (STEPS [100K, 500K, 1000K etc.])
//   use  double start = omp_get_wtime(); when the computation starts
//   use  double end = omp_get_wtime(); when computation ends 
//   double delta = end -start; 
//   print compute time in seconds using printf("PI = %.16g computed in %.4g seconds\n", pi, delta);
//3. Use Work Sharing Construct #pragma omp for to parallelize the for loop (Write a new function for parallel PI) using 4 threads
//   3A: Do NOT add a reduction clause. Try to see race conditions - check the value of PI returned.
//   3B: Use a syncronization construct #pragma omp atomic  to ensure that there are no race conditions. Compile and Run program and time the computation.
//       Is there any performance improvement in the parallel code? If NO, why?
//   3C: Use a reduction clause to reduce "sum", now check the accuracy and performance of the program. Report findings.
//4. Time the parallel program with reduction for 1,2,3,4,5,6,7,8,9,10 Threads for STEPS {100K, 500K, 1000K}
//5. Record the time and accuracy of PI for various threads and step_size.
//
//
//*******************************************************************************************************************

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_THREADS 8

static long steps = 10000000;
double step;

//####################################################################################################################
//
//
//
//####################################################################################################################
int serialPI()
{

    int i,j;
    double x;
    double pi, sum = 0.0;
    double start, delta;

    step = 1.0/(double) steps;
    printf("Running Serial code");

    sum = 0.0;
    start = omp_get_wtime();

    for (i=0; i < steps; i++) 
	{
        x = (i+0.5)*step;
        sum += 4.0 / (1.0+x*x); 
    } 

    pi = step * sum;
    delta = omp_get_wtime() - start;
    printf("PI = %.16g computed in %.4g seconds\n", pi, delta);
    
    return(0);



}
//####################################################################################################################
//
//
//3A; incorrect parallel PI where there is a race condition
//####################################################################################################################
int parallelPIRaceCondition()
{
    int i,j;
    double x;
    double pi, sum = 0.0;
    double start, delta;

    step = 1.0/(double) steps;
    for (j=1; j<= MAX_THREADS; j++) 
    {

        printf(" running on %d threads: ", j);

        // This is the beginning of a single PI computation
        omp_set_num_threads(j);

        sum = 0.0;
        double start = omp_get_wtime();


        #pragma omp parallel for private(x)
        for (i=0; i < steps; i++) 
		{
            x = (i+0.5)*step;
            sum += 4.0 / (1.0+x*x); 
        } 

        pi = step * sum;
        delta = omp_get_wtime() - start;
        printf("PI = %.16g computed in %.4g seconds\n", pi, delta);
    }

    return(0);
}
//####################################################################################################################
//
//
//
//####################################################################################################################
int parallelPI()
{
    int i,j;
    double x;
    long double pi, sum = 0.0;
    double start, delta;

    step = 1.0/(double) steps;
    for (j=1; j<= MAX_THREADS; j++) 
    {

        printf(" running on %d threads: ", j);

        // This is the beginning of a single PI computation
        omp_set_num_threads(j);

        sum = 0.0;
        double start = omp_get_wtime();


        #pragma omp parallel for reduction(+:sum) private(x)
        for (i=0; i < steps; i++) 
		{
            x = (i+0.5)*step;
            sum += 4.0 / (1.0+x*x); 
        } 

        pi = step * sum;
        delta = omp_get_wtime() - start;
        printf("PI = %.25g computed in %.4g seconds\n", pi, delta);
    }
    return(0);
}
//####################################################################################################################
//
//
//
//####################################################################################################################
int main (int argc, const char *argv[]) {
	
	printf("Serial computation of PI using N = %lu STEPS", steps);
	serialPI();
	printf("Parallel computation of PI using N = %lu STEPS [Race condition]", steps);
	parallelPIRaceCondition();
	printf("Parallel computation of PI using N = %lu STEPS [using reduction]", steps);
    parallelPI();
    return(0);
}


