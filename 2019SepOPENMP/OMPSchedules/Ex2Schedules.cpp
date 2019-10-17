//***************************************************************************************************************
//
//Akhila Prabhakaran OPENMP Course SERC May 2019
//
//Exercise 3: Exploring the Schedule Clause
//Duration: 5 minutes
//
//Login to the workshop machine 
//  ssh username@delta-cluster.serc.iisc.ernet.in
//  enter the password provided
//
//  pwd (print working directory)
//  mkdir username_2019MayOMP
//  cd username_2019MayOMP
//  
//  Compile and Run the following program to understand various schedule clauses
//  
//
//
//*******************************************************************************************************************

#include <omp.h>
#include <stdio.h>

//********************************************************************************************************************
//Static schedule with chunksize
//Each thread will execute a chunk of loop iterations
//for example if number of threads is 4 and `total iterations is 25, with schedule of chunksize 5
//0,1,2,3,4 will be processed by thread0
//5,6,7,8,9 will be processed by thread1
//10,11,12,13,14 by thread2
//15,16,17,18,19 by thread3
//20,21,22,23,24 by thread0
//********************************************************************************************************************
void ompLoopStaticChunkExample(int N)
{

 printf("schedule static chunksize = 5 \n");

 #pragma omp parallel
 {
   #pragma omp for schedule(static,5)
   for (int i=0; i<N; i++) {
      printf("Thread Number %d - loop iteration %d \n", omp_get_thread_num(), i);       
      // do something with i
      }
  }
}

//************************************************************************************************
//Static schedule with no  chunksize
//When no chunk-size is specified, OpenMP divides iterations into chunks 
//that are approximately equal in size and it distributes at most one chunk to each thread.
//************************************************************************************************
void ompLoopStaticExample(int N)
{

 printf("schedule static chunksize not specified \n");

 #pragma omp parallel
 {
   #pragma omp for schedule(static)
   for (int i=0; i<N; i++) {
      printf("Thread Number %d - loop iteration %d \n", omp_get_thread_num(), i);
      // do something with i
       }
 }
      //

}
//***********************************************************************************************************
/*GUIDED
 * If you specify a value for n, the iterations of a loop are divided into chunks 
 * such that the size of each successive chunk is exponentially decreasing. 
 * n specifies the size of the smallest chunk, except possibly the last. 
 * If you do not specify a value for n, the default value is 1.
 * The size of the initial chunk is proportional to CEILING(number_of_iterations / number_of_threads) iterations. 
 * Subsequent chunks are proportional to CEILING(number_of_iterations_remaining / number_of_threads) iterations. 
 * If n is greater than 1, each chunk must contain at least n consecutive iterations (except for the last chunk to be assigned, which can have fewer than n iterations.)
 * As each thread finishes a chunk, it dynamically obtains the next available chunk.
 * You can use guided scheduling in a situation in which 
 * multiple threads in a team might arrive at a DO work-sharing construct at varying times, 
 * and each iteration requires roughly the same amount of work. 
 * For example, if you have a DO loop preceded by one or more work-sharing SECTIONS or DO constructs with NOWAIT clauses, 
 * you can guarantee that no thread waits at the barrier longer than it takes another thread to execute its final iteration, or final k iterations if a chunk size of k is specified. 
 * The GUIDED schedule requires the fewest synchronizations of all the scheduling methods.

Iterations are dynamically assigned to threads in blocks as threads request them until no blocks remain to be assigned.
Similar to DYNAMIC (please write your own function code for this ) except that the block size decreases each time a parcel of work is given to a thread.
The size of the initial block is proportional to:
number_of_iterations / number_of_threads
Subsequent blocks are proportional to number_of_iterations_remaining / number_of_threads
The chunk parameter defines the minimum block size. The default chunk size is 1.
As seen in the output of the following function. Some threads perform a large numer of
iterations while others do not. This is a feature of Dynamic scheduling where
work is assigned to the free thread. If there are multiple free threads as in our
case where we are NOT doing any other work, we could observe an extremely random pattern of iteration space to threads.
 ***********************************************************************************************************
 */
void ompLoopGuidedChunkExample(int N)
{
 printf("schedule guided chunksize = %d \n", 5);
 #pragma omp parallel
 {
   #pragma omp for schedule(guided,5)
   for (int i=0; i<N; i++) {
      printf("Thread Number %d - loop iteration %d \n", omp_get_thread_num(), i);       
      // do something with i
      }
  }
}

//****************************************************************
//Guided schedule with no  chunksize
//****************************************************************
void ompLoopGuidedExample(int N)
{

 printf("schedule guided chunksize not specified \n");
 #pragma omp parallel
 {
   #pragma omp for schedule(guided)
   for (int i=0; i<N; i++) {
      printf("Thread Number %d - loop iteration %d \n", omp_get_thread_num(), i);
      // do something with i
       }
 }
      //

}


//---------------------------------------
//---------------------------------------
int main()
{
   ompLoopStaticChunkExample(25);
   ompLoopStaticExample(25);
   ompLoopGuidedExample(30);
   ompLoopGuidedChunkExample(30);
   return (0);
}

