! ***************************************************************************************************************
! 
! Akhila Prabhakaran OPENMP Course SERC May 2019
! 
! Exercise 4: Simple example of numerical integration for computation of PI using openmp
! Duration: 20 minutes
! 
! Login to the workshop machine 
!   ssh username@delta-cluster.serc.iisc.ernet.in
!   enter the password provided
! 
!   pwd (print working directory)
!   mkdir username_2019MayOMP
!   cd username_2019MayOMP
! use an editor like vi to write a program "MyComputePI.cpp" [vi MyComputePI.cpp]
! 1. First write a serial program that computes PI - based on the methodology discussed in the lecture
!    Alternatively, copy the program below and run the Serial function to compute PI
! 2. Time the program and record the time for various (STEPS [100K, 500K, 1000K etc.])
!    use  double start = omp_get_wtime(); when the computation starts
!    use  double end = omp_get_wtime(); when computation ends 
!    double delta = end -start; 
!    print compute time in seconds using printf("PI = %.16g computed in %.4g seconds\n", pi, delta);
! 3. Use Work Sharing Construct #pragma omp for to parallelize the for loop (Write a new function for parallel PI) using 4 threads
!    3A: Do NOT add a reduction clause. Try to see race conditions - check the value of PI returned.
!    3B: Use a syncronization construct #pragma omp atomic  to ensure that there are no race conditions. Compile and Run program and time the computation.
!        Is there any performance improvement in the parallel code? If NO, why?
!    3C: Use a reduction clause to reduce "sum", now check the accuracy and performance of the program. Report findings.
! 4. Time the parallel program with reduction for 1,2,3,4,5,6,7,8,9,10 Threads for STEPS {100K, 500K, 1000K}
! 5. Record the time and accuracy of PI for various threads and step_size.
! 
! 
! *******************************************************************************************************************


! ####################################################################################################################
! 
! 
! 
! ####################################################################################################################
subroutine serialPI()
    integer	steps
    integer i,j
    real x
    real pi, total
    double precision start, delta, omp_get_wtime
    total = 0.0
	steps = 10000000
    step = 1.0/steps;
    print *, "Running Serial code"

    total = 0.0;
    start = omp_get_wtime();

    do i = 1, steps
        x = (i+0.5)*step;
        total = total + 4.0 / (1.0+x*x); 
 	end do 

    pi = step * total;
    delta = omp_get_wtime() - start;
    print *, "PI = ", pi, " computed in ", delta, " seconds"
    
end subroutine
! ####################################################################################################################
! 
! 
! 3A; incorrect parallel PI where there is a race condition
! ####################################################################################################################
subroutine parallelPIRaceCondition()
    integer	steps , omp_get_num_procs, MAX_THREADS
    double precision omp_get_wtime
    integer i,j
    real x
    real pi, total
    double precision start, delta
    total = 0.0
	steps = 10000000
    step = 1.0/steps;
    MAX_THREADS = omp_get_num_procs()
    do j = 1, MAX_THREADS 

        print *, "--------------------------------"
        print *, " running on ", j , " threads: "

        !  This is the beginning of a single PI computation
       call omp_set_num_threads(j);

        total = 0.0;
        start = omp_get_wtime();


        !$omp parallel do private(x)
        do i = 1, steps 
            x = (i+0.5)*step;
            total = total+ 4.0 / (1.0+x*x); 
 		end do
        !$omp end parallel do
        pi = step * total;
        delta = omp_get_wtime() - start;
        print *, "PI = ", pi, " computed in ", delta, " seconds "
    end do
end subroutine
! ####################################################################################################################
! 
! 
! 
! ####################################################################################################################
subroutine parallelPI( )
    integer	steps, MAX_THREADS, omp_get_num_procs
    double precision omp_get_wtime
    integer i,j
    real x
    real pi, total
    double precision start, delta

    MAX_THREADS = omp_get_num_procs()
	steps = 10000000
    total= 0.0
    step = 1.0/steps;
    do j = 1, MAX_THREADS
        print *, "--------------------------------"
        print *, " running on ", j , " threads: "

        !  This is the beginning of a single PI computation
        call omp_set_num_threads(j);

        total = 0.0;
         start = omp_get_wtime();


        !$omp parallel do reduction(+:total) private(x)
         do i=1,steps
            x = (i+0.5)*step;
            total = total + 4.0 / (1.0+x*x); 
 		end do
        !$omp end parallel do
        pi = step * total;
        delta = omp_get_wtime() - start;
        print *, "PI = ", pi, " computed in ", delta, " seconds "
	end do 
end subroutine
! ####################################################################################################################
! 
! 
! 
! ####################################################################################################################
program main 
	use omp_lib
implicit none
	integer steps
	real step
	steps = 1000000

	print *, "Serial computation of PI using N = ", steps, " STEPS"
	call serialPI();
    print *, "#################################################################"
	print *, "Parallel computation of PI using N = ", steps, " STEPS [Race condition]"
	call parallelPIRaceCondition();
    print *, "#################################################################"
	print *, "Parallel computation of PI using N = ", steps, "STEPS [using reduction]"
    call parallelPI();
end program 


