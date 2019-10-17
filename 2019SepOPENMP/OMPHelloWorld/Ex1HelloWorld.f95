!******************************************************************************
! Akhila Prabhakaran OPENMP Course SER! May 2019
!
! Exercise 1: Familiarization with openmp pragmas, run time and environment variables, shared variables and barriers, master and single
! Duration: 20 -30 minutes
!
! Login to the workshop machine 
!  ssh username@delta-cluster.serc.iisc.ernet.in
!  enter the password provided
!
!  pwd (print working directory)
!  mkdir username_2019MayOMP
!  cd username_2019MayOMP
! use an editor like vi to write a program "MyHelloWorld.f" [vi MyHelloWorld.f]
! First write a serial program that prints "hello world!"
! Parallelise the program by making the following changes
!  1. add header file
!  2. add a parallel region using !$OMP PARALLEL outside the PRINT statement
!  3. identify the number of processors in the machine using OMP_get_num_procs() Fortran Runtime function
!  4. print number of processors
!  5. print max number of threads (default) WITHOUT setting any environment variables using OMP_get_max_threads() Fortran runtime function
!     By default, most compilers will create 1 thread for each core.
!  6. Compile the program using gfortran -fopenmp MyHelloWorld.f -o MyHelloWorld.exe
!  7. Run ./MyHelloWorld.exe and report the output
!
! Part B: Use of omp_get_thread_num() inside the parallel region
!  8. declare int tid = 0; before the parallel region (variable that holds the thread id of each thread in the parallel region)
!  9. Edit the code to include the "private clause" by changing !$OMP PARALLEL to 
!     !$OMP PARALLEL PRIVATE (TID)
!  10.Inside the parallel region add the following code
!     TID = omp_get_thread_num();
!  11.Change the printf or cout statement in the parallel region as follows
!     print *, "Hello World from thread =", TID
!  12. Compile and re-run the program. Run multiple times to see the changing order of tid.
!
! Part C:Use of environment variables to override the number of threads in code to something other than max threads printed in part B
!  13. bash: export OMP_NUM_THREADS=<number of threads to use>
!      csh or tcsh shell, enter: setenv OMP_NUM_THREADS <number of threads to use>
!  14. Now run the exe (NO COMPILATION Required as no code has changed here). Report the number of threads. Is it the same as the ENV Variable?
!
! Part D:Use the num_threads clause in !$OMP PARALLEL  to override number of threads in Part C
!  15. Change !$OMP PARALLEL  to !$OMP PARALLEL NUM_THREADS(10)   
!  ensure that 10 is different from the env variable in Part ! and max threads in part B
!  16. Compile and run the program. Report results
!
! Part E:Use of #pragma omp master
!  17. inside the parallel region after the printf("hello world from thread =%d", tid); add the following
!      !$OMP MASTER
!		  nthreads = omp_get_num_threads();
!		  printf("Number of threads inside parallel region= %d\n", nthreads);
!      !$OMP END MASTER
!   18. Compile and Run the program again and report results
! Part F: use of barrier
!   19. Add a !$OMP BARRIER before the #pragma omp master
!   20. Compile, Run and Report results
! Part G: use of single
!   21: Change !$OMP MASTER to #!$OMP SINGLE & print the thread id too
!*****************************************************************************

      PROGRAM HELLO
     
      INTEGER NTHREADS, TID, OMP_GET_NUM_THREADS, OMP_GET_THREAD_NUM, OMP_GET_NUM_PROCS, OMP_GET_MAX_THREADS, NUM_PROCS, MAX_THREADS

	  TID = 0
      NUM_PROCS = omp_get_num_procs()
      MAX_THREADS = omp_get_max_threads()
   	  PRINT *, "Number of Processors = ", NUM_PROCS
      PRINT *, "Max Number of Threads = ",MAX_THREADS
  	  PRINT *, "Number of Threads outside parallel region = ",OMP_GET_NUM_THREADS()

!     Fork a team of threads giving them their own copies of variables
!$OMP PARALLEL PRIVATE(NTHREADS, TID)

!     Obtain thread number
      TID = OMP_GET_THREAD_NUM()
      PRINT *, 'Hello World from thread = ', TID

! --------- Only master thread does this
! --------- !$OMP MASTER
      IF (TID ==0) THEN
        NTHREADS = OMP_GET_NUM_THREADS()
        PRINT *, 'Number of threads inside parallel region= ', NTHREADS
      END IF
! --------- !$OMP END MASTER

!     All threads join master thread and disband
!$OMP END PARALLEL

      END PROGRAM


