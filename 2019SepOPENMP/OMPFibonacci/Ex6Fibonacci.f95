!*******************************************************************
!/Author: Akhila Prabhakaran
! Exercise 6:  Returns the Nth number of the Fibonacci series
!
! The Fibonacci sequence is recursively defined as
!  F(0)=1,F(1)=1,F(n)=F(n−1)+F(n−2) for n≥2.
!  (1)F(0)=1,F(1)=1,F(n)=F(n−1)+F(n−2) for n≥2.
!
! N is initialised to an integer in main()
!
! Functions: 
! 1. long naiveSerialFib(int N) <= Serial function that conputes Fibonacci[N]
! The above is a naive function, because it uses recursion to repeatedly call fib[N-1] and fib[n-2]
! Most of the time the fib of a number is computed more than once. The code includes a counter that
! keeps track of how many times the fibonacci of a given integer is computed.
! This is stored in the global(shared) array repeatComputation.
! repeatComputation[i] tells how many times the fibonacci of "i" was computed, in the process of
! calculating fibonacci(N)
!
! 2. long optimalSerialFib(int N) <= Serial function that computes Fibonacci[N]
! This is an optimal function (not the bestest one, but much better than the naive function).
! A shared array (long* computedVals)  is implemented to store the computed value of Fibonacci[i] for 0<= i <= N
! Before computing Fibonacci[i] , the function checks if it has already been computed and reuses that value.
!
! By running the following program. it can be seen that the naive function has a very poor performance when compared
! to the optimal serial function.
!
! 3. long parallelNaiveFibWithTasks(int N) => The naive serial function is converted to a parallel function
! using openmp tasks. Performance is expected to be worse than the serial code because of too many tasks
! being created and too many repeated computations of Fibonacci for i < N
! Sometimes this even fails to return.(try N > 40)
!
! 4. long parallelFibWithTasks(int N) => improved version using an array of computed Fibonacci values
! Much better than function 3 in terms of performance. But has a lot of overhead for creation of tasks.
! Interestngly, the function also stores the number of Fibonacci computations done per Thread.
! loadbalancing in OPENMP is an important concept that drives improvement in performance.
!
! TODO:
! 1. Fix the number of threads  = number of processors (just let it default)
! 2. Run the optimal parallel and serial functions for various values of N (25 through 50)
! 3. Document the serial and parallel compute times.
! 4. Calculate speed up for parallel code for various N
! 5. Do you see any patterns in speedup?
! Can the parallel code be further optimised?
!   Hint:based on my experience, optimised serial code works well for N < 30. Parallel code works well for N>= 30
!   One could add an "if" clause in the #pragma omp task directive to see if they get an improvement in speedup
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/



!*****************************************************************
!--Calculate the Fibonacci number of the given integer.
!The basic single-threaded recursive solution. 
!******************************************************************/
recursive function fibonacci(n, A, B) result (fib)
    integer, intent(in) :: n   !< If n <= 0 then 0 is assumed.
    integer, dimension(n), intent(out) :: A
    integer, dimension(n), intent(out) :: B
    integer  :: fib !< Fibonacci number
    
    if (n > 0) then
    	B(n) = B(n)+1;
    end if
    if (n < 3) then
      fib = n
    else
      fib = fibonacci(n - 1, A, B) + fibonacci(n - 2, A, B)
    endif
end function fibonacci

!********************************************************************
!The optimal single-threaded recursive solution. Where we store Fib[i]
!once it is computed and reuse that value when there is an attempt to
!compute it again
!******************************************************************
recursive function optimalSerialFib(N, computedVals, repeatComputation) result (fib)
    integer, intent(in) :: N   !< If n <= 0 then 0 is assumed.
    integer, dimension(N), intent(out) :: computedVals
    integer, dimension(N), intent(out) :: repeatComputation
    integer  :: fib !< Fibonacci number
    
    if (N > 0) then
    	repeatComputation(N) = repeatComputation(N)+1;
    end if

    if (N < 3) then
      if (computedVals(N) == -1) then
        computedVals(N) = N
      end if
      fib = N
    else
		if (computedVals(N-1) == -1) then
			computedVals(N-1) = optimalSerialFib(N-1, computedVals, repeatComputation);
		end if
     	if (computedVals(N-2) == -1) then
			computedVals(N-2) = optimalSerialFib(N-2,  computedVals, repeatComputation);
     	end if
      fib =computedVals(N-1) + computedVals(N-2)
    endif
end function optimalSerialFib

!*********************************************************
!The OpenMP parallel solution calls for two different ideas. 
!First of all, we parallelize the recursion by using tasks
!This computes the right solution, but, as in the naive single-threaded solution, 
!it recomputes many of the intermediate values.
!*********************************************************/
recursive function parallelNaiveFibWithTasks(N, numThreads, iterationsPerThread) result (fib)

   integer, intent(in) :: N, numThreads   !< If n <= 0 then 0 is assumed.
   integer, dimension(numThreads), intent(out) :: iterationsPerThread
   integer  :: fib !< Fibonacci number
   integer :: val1, val2, CurrentThread, omp_get_thread_num

  !$omp critical
    CurrentThread = omp_get_thread_num();
    !print *, "Current Thread Num", CurrentThread
    !!print *, "NumThreads", numThreads
    iterationsPerThread(CurrentThread+1) = iterationsPerThread(CurrentThread+1)+ 1 
    !print *, iterationsPerThread(CurrentThread+1) , " Tasks processed by Thread ", CurrentThread
   !$omp end critical
  if (N>2) then
	  !$omp task shared(val1, iterationsPerThread) firstprivate(N)
		val1=parallelNaiveFibWithTasks(N-1, numThreads, iterationsPerThread);
	  !$omp end task
	  !$omp task shared(val2, iterationsPerThread) firstprivate(N)
		val2=parallelNaiveFibWithTasks(N-2, numThreads, iterationsPerThread);
	  !$omp end task
	  !$omp taskwait
	  fib = val1 + val2
  else
    fib = N;
  end if

end function
!******************************************************************
! Parallel Fibonacci with tasks - Work Sharing Construct
! Typical function that could be used for most recursive function calls
!******************************************************************/
recursive function parallelFibWithTasks(N, numThreads, computedVals, repeatComputation, iterationsPerThread) result (fib)
   integer, intent(in) :: N, numThreads   !< If n <= 0 then 0 is assumed.
   integer, dimension(N), intent(out) :: computedVals
   integer, dimension(N), intent(out) :: repeatComputation
   integer, dimension(numThreads), intent(out) :: iterationsPerThread
   integer  :: fib !< Fibonacci number
   integer :: val1, val2, CurrentThread, omp_get_thread_num
  !$omp critical
   CurrentThread = omp_get_thread_num();
    !print *, "Current Thread Num", CurrentThread
    !!print *, "NumThreads", numThreads
    iterationsPerThread(CurrentThread+1) = iterationsPerThread(CurrentThread+1)+ 1 
   !$omp end critical
 if (N > 2) then
     !omp task shared(computedVals, iterationsPerThread) firstprivate(N)
     if (computedVals(N-1) == -1) then
        val1 =parallelFibWithTasks(N-1, numThreads, computedVals, repeatComputation, iterationsPerThread)
        !omp critical
		computedVals(N-1) = val1
     end if
     !omp end task
     !omp task shared(computedVals, iterationsPerThread) firstprivate(N)
     if (computedVals(N-2) == -1) then
        val2= parallelFibWithTasks(N-2, numThreads, computedVals, repeatComputation, iterationsPerThread)
        !omp critical
		computedVals(N-2) = val2
     end if
     !omp end task
     !omp taskwait
     fib =  computedVals(N-1)+ computedVals(N-2)
  else 
    if (computedVals(N) == -1) then
        !$omp critical
        computedVals(N) = N;
        !$omp end critical
    end if
  	fib = computedVals(N)
  end if
end function parallelFibWithTasks
!*************************************************************************************
!
!
!*************************************************************************************
program MAIN
  integer N, i, FIB, fibonacci, optimalSerialFib, parallelFibWithTasks, NUM_THREADS, omp_get_num_threads,parallelNaiveFibWithTasks
  double precision starttime, endtime, omp_get_wtime
  integer, allocatable :: repeatComputation(:) 
  integer, allocatable :: computedVals(:)
  integer, allocatable :: iterationsPerThread(:)

  starttime = 0
  endtime = 0
  i = 0
  N = 25

  allocate(repeatComputation(N))
  allocate(computedVals(N))
  !***********************************************************************
  ! Naive Serial Code
  !***********************************************************************
  !Some initialization  
  do i = 1,N
    computedVals(i) = -1;
	repeatComputation(i) = 0;
  end do
  !always time the code
  starttime = omp_get_wtime();
  
  !function call
  FIB = fibonacci(N,computedVals, repeatComputation )
  
  !time again
  endtime = omp_get_wtime();
  print *, "Fibonacci[", N, "] = ", FIB
  print *, "Time taken to compute Serial naive FIB[", N, "] is ", endtime-starttime, " seconds "

  do i = 1,N
    print*, i, " was computed ", repeatComputation(i), " times "
  end do

  !***********************************************************************
  ! Optimal Serial Code
  !***********************************************************************
  !Some initialization  
  do i = 1,N
    computedVals(i) = -1;
	repeatComputation(i) = 0;
  end do
  !always time the code
  starttime = omp_get_wtime();
  
  !function call
  FIB = optimalSerialFib(N,computedVals, repeatComputation);
  
 !time again
  endtime = omp_get_wtime();
  print *, "Fibonacci[", N, "] = ", FIB
  print *, "Time taken to compute Optimal Serial FIB[", N, "] is ", endtime-starttime, " seconds "

  do i = 1,N
    print*, i, " was computed ", repeatComputation(i), " times "
  end do

  !***********************************************************************
  !  OMENMP Parallel Region - optimal parallel code
  !***********************************************************************
  print *, "OPENMP Tasks for Fibonacci computation - naive implementation"
  !$omp parallel
	  !**********************************************************************
	  ! Parallel region begins here
      ! We do not know how many threads are spwaned - this will be taken from
      ! environment variables. We will just let it default.
      ! Alternatively, the user could change or control this before the parallel region
	  !**********************************************************************'

      NUM_THREADS = omp_get_num_threads()
      print * , "Starting OPENMp Parallel Region with " , NUM_THREADS, " Threads "

      !Intilialization********************************
      !intialize just once - use single or master here
      !$omp master
		  allocate(iterationsPerThread(NUM_THREADS))
		  !Some initialization  
		  do i = 1,N
			computedVals(i) = -1
			repeatComputation(i) = 0
		  end do
          do i = 1, NUM_THREADS
            iterationsPerThread(i) =0	    
 		  end do
		  !Call to parallel fibonacci recursive function also just once
		  starttime = omp_get_wtime();
          FIB = parallelNaiveFibWithTasks(N, NUM_THREADS,  iterationsPerThread)
		  endtime = omp_get_wtime();
		  print *, "Fibonacci[", N, "] = ", FIB
		  print *, "Time taken to compute parallel FIB[", N, "] using tasks is ", endtime-starttime," seconds "
		  do i = 1, NUM_THREADS
    		 print *, "Thread ", i, " processed ", iterationsPerThread(i)," TASKS "
 		  end do
      !$omp end master
   !$omp end parallel

  !***********************************************************************
  !  OMENMP Parallel Region - optimal parallel code
  !***********************************************************************
  print *, "OPENMP Tasks for Fibonacci computation - improved implementation"
  !$omp parallel
	  !**********************************************************************
	  ! Parallel region begins here
      ! We do not know how many threads are spwaned - this will be taken from
      ! environment variables. We will just let it default.
      ! Alternatively, the user could change or control this before the parallel region
	  !**********************************************************************'

      NUM_THREADS = omp_get_num_threads()
      print * , "Starting OPENMp Parallel Region with " , NUM_THREADS, " Threads "

      !Intilialization********************************
      !intialize just once - use single or master here
      !$omp master
		  !allocate(iterationsPerThread(NUM_THREADS))
		  !Some initialization  
		  do i = 1,N
			computedVals(i) = -1
			repeatComputation(i) = 0
		  end do
          do i = 1, NUM_THREADS
            iterationsPerThread(i) =0	    
 		  end do
		  !Call to parallel fibonacci recursive function also just once
		  starttime = omp_get_wtime();
          FIB = parallelFibWithTasks(N, NUM_THREADS, computedVals, repeatComputation, iterationsPerThread)
		  endtime = omp_get_wtime();
		  print *, "Fibonacci[", N, "] = ", FIB
		  print *, "Time taken to compute parallel FIB[", N, "] using tasks is ", endtime-starttime," seconds "
		  do i = 1, NUM_THREADS
    		 print *, "Thread ", i, " processed ", iterationsPerThread(i)," TASKS "
 		  end do
      !$omp end master
   !$omp end parallel
end program
