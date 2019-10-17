//***************************************************************************************************************
//
//Akhila Prabhakaran OPENMP Course SERC May 2019
//
//Exercise 3: Exploring Shared, Private, FirstPrivate and lastPrivate
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
//  Compile and Run the following program to understand various data scoping clauses
//  
//
//
//*******************************************************************************************************************

#include <omp.h>
#include <stdio.h>

//********************************************************************************************************************
//In a parallel region, any data declared outside it will be shared: 
//any thread using a variable  x will access the same memory location 
//associated with that variable.
//********************************************************************************************************************
void exampleSharedVar()
{

  int x = 5;
  printf("X is a shared varible for all threads \n");
  printf("X is initialised to %d before the #pragma omp parallel region \n", x);
  #pragma omp parallel
  {
    x = x+1;
    printf("X is updated by thread %d to value %d \n", omp_get_thread_num(), x);
 
  }
  printf("Value of X at the end of the parallel region is: %d \n", x);

  x = 0;
  printf("X is reset to %d before the #pragma omp parallel for region \n");
  #pragma omp parallel for
  for (int i = 0; i < 10; i++)
  {
    x = x+1;
    printf("X is updated by thread %d to value %d \n", omp_get_thread_num(), x);
 
  }
  printf("Value of X at the end of the parallel for region is: %d \n", x);

}

//******************************************************************************
//The first guideline is to always write parallel regions with the default(none) clause. 
//This forces the programmer to explicitly think about the data-sharing attributes of all variables.
//The second guideline is to declare private variables inside parallel regions whenever possible. 
//This guideline improves the readability of the code and makes it clearer.

//******************************************************************************
void examplePrivateVar()
{

  int x = 5;
  printf("X is a Private varible for all threads \n");
  printf("X is not explicitly initialised in the parallel region. Could result in bad data/garbage \n");
  printf("X is initialised to %d before the #pragma omp parallel region \n", x);
  #pragma omp parallel private(x)
  {
    x = x+1;
    printf("X is updated by thread %d to value %d \n", omp_get_thread_num(), x);
 
  }
  printf("Value of X at the end of the parallel region is: %d \n", x);

  x = 0;
  printf("X is reset to %d before the #pragma omp parallel for region \n");
  printf("X is not explicitly initialised in the parallel region. Could result in bad data/garbage \n");
  #pragma omp parallel for private(x)
  for (int i = 0; i < 10; i++)
  {
    x = x+1;
    printf("X is updated by thread %d to value %d \n", omp_get_thread_num(), x);
 
  }
  printf("Value of X at the end of the parallel for region is: %d \n", x);

}

//************************************************************************************************
//Statically allocated data, that is with a syntax like
//int array[100];
//integer,dimension(:) :: array(100}
//can be shared or private, depending on the clause you use.
//
//Dynamically allocated data, that is, created with malloc or allocate , can only be shared.
//************************************************************************************************
void staticVsDynamicData()
{
    int nthreads = omp_get_num_procs();    
	int arrayStatic[nthreads];    
    int *arrayDynamic =  new int[nthreads];

    for (int i=0; i < nthreads; i++)
    {
		arrayStatic[i] = 0;
		arrayDynamic[i] = -1;

    }
    printf("Shared static and dynamic llocated data");
    #pragma omp parallel shared(arrayStatic, arrayDynamic)
	{
  		int t = omp_get_thread_num();
  		arrayStatic[0] = t;
        arrayDynamic[0] = t;
    	printf("arrayStatic[%d] = %d \n", 0, arrayStatic[0]);
    	printf("arrayDynamic[%d] = %d \n", 0, arrayDynamic[0]);
	}

    printf("After the parallel region \n ......................\n");
    printf("arrayStatic[%d] = %d \n", 0, arrayStatic[0]);
    printf("arrayDynamic[%d] = %d \n", 0, arrayDynamic[0]);


    printf("Private static and dynamic llocated data");
    #pragma omp parallel private(arrayStatic, arrayDynamic)
	{
        arrayDynamic =  new int[nthreads];
  		int t = omp_get_thread_num();
  		arrayStatic[0] = t;
        arrayDynamic[0] = -2;
    	printf("arrayStatic[%d] = %d \n", 0, arrayStatic[0]);
    	printf("arrayDynamic[%d] = %d \n", 0, arrayDynamic[0]);
	}

    printf("After the parallel region \n ......................\n");
    printf("arrayStatic[%d] = %d \n", 0, arrayStatic[0]);
    printf("arrayDynamic[%d] = %d \n", 0, arrayDynamic[0]);


}

//****************************************************************
//Above, you saw that private variables are completely separate 
//from any variables by the same name in the surrounding scope. 
//However, there are two cases where you may want some storage association 
//between a private variable and a global counterpart.
//First of all, private variables are created with an undefined value.
// You can force their initialization with clause{firstprivate}.
// In the following function variable t behaves like a private variable, 
//except that it is initialized to the outside value.
//****************************************************************
void firstPrivate()
{
  int t=2;
#pragma omp parallel firstprivate(t)
  {
    t += omp_get_thread_num();
    printf("t is updated by thread %d to value %d \n", omp_get_thread_num(), t);
  }
  printf("Value of t after the parallel region is %d \n", t);
}

//****************************************************************
//Secondly, you may want a private value to be preserved to the 
//environment outside the parallel region. 
//This really only makes sense in one case, 
//where you preserve a private variable from the last iteration of a parallel loop, 
//or the last section in an sections construct. This is done with 
//clause{lastprivate}:
//NOTE: lastprivate works only only a omp parallel for OR omp sections
//Also note that the value is from the LAST iteration of the for loop, NOT from the thread that runs last
//****************************************************************
void lastPrivate()
{

  int t=2;
  int m= 0;
#pragma omp parallel for firstprivate(t) lastprivate(m)
  for (int i = 0; i < 10; i++)
  {
    t += omp_get_thread_num();
    m = i*i;
    printf("t is updated by thread %d to value %d \n", omp_get_thread_num(), t);
  }
  printf("Value of t after the parallel region is %d \n", t);
  printf("Value of m after the parallel region is %d \n", m);


#pragma omp parallel for shared(t) lastprivate(m)
  for (int i = 0; i < 10; i++)
  {
    t = i*i;
    m = i*i;
    printf("t is updated by thread %d to value %d \n", omp_get_thread_num(), t);
    printf("m is updated by thread %d to value %d \n", omp_get_thread_num(), m);
  }
  printf("Value of t after the parallel region is %d \n", t);
  printf("Value of m after the parallel region is %d \n", m);

}

//---------------------------------------
//---------------------------------------
int main()
{
   exampleSharedVar();
   examplePrivateVar();
   //staticVsDynamicData();
   firstPrivate();
   lastPrivate();
   return (0);
}

