! ***************************************************************************************************************
! Akhila Prabhakaran OPENMP Course SER! May 2019
! Exercise 3: Exploring the Schedule Clause
! Duration: 5 minutes
! Login to the workshop machine 
!  ssh username@delta-cluster.serc.iisc.ernet.in
!  enter the password provided
!  pwd (print working directory)
!  mkdir username_2019MayOMP
!  cd username_2019MayOMP
!  
!  Compile and Run the following program to understand various schedule clauses
!  
! *******************************************************************************************************************

! ********************************************************************************************************************
! Stati! schedule with chunksize
! Each thread will execute a chunk of loop iterations
! for example if number of threads is 4 and `total iterations is 25, with schedule of chunksize 5
! 0,1,2,3,4 will be processed by thread0
! 5,6,7,8,9 will be processed by thread1
! 10,11,12,13,14 by thread2
! 15,16,17,18,19 by thread3
! 20,21,22,23,24 by thread0
! ********************************************************************************************************************
	SUBROUTINE ompLoopStaticChunkExample(N)
     INTEGER TID ,OMP_GET_NUM_THREADS, OMP_GET_THREAD_NUM
     TID = 0
	 PRINT *, "schedule static chunksize = 5"
!$OMP PARALLEL PRIVATE(TID)
   PRINT *, "Number of threads=", OMP_GET_NUM_THREADS()
!$OMP DO SCHEDULE(STATIC, 5) 
	   	DO I = 1, N
        	TID = OMP_GET_THREAD_NUM()
      		PRINT *, "Thread Number",TID, " - loop iteration", I
	! do something with i
		END DO
!$OMP END DO
!$OMP END PARALLEL

	END SUBROUTINE ompLoopStaticChunkExample
! ************************************************************************************************
! Stati! schedule with no  chunksize
! When no chunk-size is specified, OpenMP divides iterations into chunks 
! that are approximately equal in size and it distributes at most one chunk to each thread.
! ************************************************************************************************
	SUBROUTINE ompLoopStaticExample (N)
    INTEGER TID, OMP_GET_NUM_THREADS, OMP_GET_THREAD_NUM
	TID = 0
 	PRINT *,"schedule static chunksize not specified"
!$OMP PARALLEL PRIVATE(TID)
   PRINT *, "Number of threads=", OMP_GET_NUM_THREADS()
!$OMP DO SCHEDULE(STATIC)
   	DO i = 1, N
      TID = OMP_GET_THREAD_NUM()
      PRINT *, "Thread Number", TID, " - loop iteration", i
! do something with i
    END DO
!$OMP END DO
!$OMP END PARALLEL
	END SUBROUTINE ompLoopStaticExample
! ***********************************************************************************************************
! GUIDED
! * If you specify a value for n, the iterations of a loop are divided into chunks 
! * such that the size of each successive chunk is exponentially decreasing. 
! * n specifies the size of the smallest chunk, except possibly the last. 
! * If you do not specify a value for n, the default value is 1.
! * The size of the initial chunk is proportional to CEILING(number_of_iterations / number_of_threads) iterations. 
! * Subsequent chunks are proportional to CEILING(number_of_iterations_remaining / number_of_threads) iterations. 
! * If n is greater than 1, each chunk must contain at least n consecutive iterations (except for the last chunk to be assigned, which can have fewer than n iterations.)
! * As each thread finishes a chunk, it dynamically obtains the next available chunk.
! * You can use guided scheduling in a situation in which 
! * multiple threads in a team might arrive at a DO work-sharing construct at varying times, 
! * and each iteration requires roughly the same amount of work. 
! * For example, if you have a DO loop preceded by one or more work-sharing SECTIONS or DO constructs with NOWAIT clauses, 
! * you can guarantee that no thread waits at the barrier longer than it takes another thread to execute its final iteration, or final k iterations if a chunk size of k is specified. 
! * The GUIDED schedule requires the fewest synchronizations of all the scheduling methods.
! ***********************************************************************************************************

 SUBROUTINE ompLoopGuidedChunkExample (N)
    INTEGER TID, OMP_GET_NUM_THREADS, OMP_GET_THREAD_NUM
 PRINT *, "schedule guided chunksize = 5"
 !$OMP PARALLEL
   PRINT *, "Number of threads=", OMP_GET_NUM_THREADS()
   !$OMP DO SCHEDULE(GUIDED,5)
   DO i = 1, N
      PRINT *, "Thread Number", OMP_GET_THREAD_NUM(), " - loop iteration", i
! do something with i
   END DO
   !$OMP END DO
 !$OMP END PARALLEL
 END SUBROUTINE ompLoopGuidedChunkExample

! ****************************************************************
! Guided schedule with no  chunksize
! ****************************************************************
	SUBROUTINE ompLoopGuidedExample (N)
    INTEGER TID, OMP_GET_NUM_THREADS, OMP_GET_THREAD_NUM
 		PRINT *, "schedule guided chunksize not specified"
		!$OMP PARALLEL
	   !$OMP DO SCHEDULE(GUIDED)
	   DO i = 1, N
	      PRINT *, "Thread Number", OMP_GET_THREAD_NUM(), " - loop iteration", i
	   ENDDO
! do something with i
	   !$OMP END DO
	!$OMP END PARALLEL

	END SUBROUTINE ompLoopGuidedExample


! ******************************************************************
! MAIN PROGRAM
! ******************************************************************
	PROGRAM MAIN

      INTEGER NTHREADS, TID, OMP_GET_NUM_THREADS, OMP_GET_THREAD_NUM, NUM_PROCS, MAX_THREADS
        CALL ompLoopStaticExample(10)
        CALL ompLoopStaticChunkExample(10)
		CALL ompLoopGuidedExample(10)
        CALL ompLoopGuidedChunkExample(10)
	END PROGRAM 
