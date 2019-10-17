!******************************************************************************
!Author: Akhila Prabhakaran
! Exercise 5: Matrix Matrix Multiplication (With Solution)
! Time: 30 minutes
! The following code has a function that computes the product of 2 matrices A and B
! Function Name: MatrixMultSerial(int NRA, int NCA, int NCB)
! input: 
!	Number of Rows of Matrix A (NRA)
!	Number of Cols of Matrix A (NCA)
!	Number of Cols of Matrix B (NCB)
! Process: 
!	generates the 2 matrices A and B using random numbers
!    multiplies the 2 matrices to compute the product
! output:
!  Result of matrix multiplication is written to a file MatrixMultiplyOutput.txt
!  returns the time taken in seconds to complete Matrix Multiplication
!
!1. Compile and run the program and see the time taken for the serial code 
!     gfortran -fopenmp MatrixMultiplySchedule.f95 -o MMSchedule.exe
!2. Write a function to perform Matrix Multiplication in parallel (see MatrixMultWithOMPFOR(...))
!   using !$omp for with NO schedule clause
!   Which variables are private and which ones need to be shared - elaborate
!3. The function should have the same inputs as the serial function with an additional input
!   parameter - int numThreads. This will allow you to time the parallel code for various number of threads.
!4. outputs to the parallel function should be similar to the serial code
!  	Result of matrix multiplication is written to a file MatrixMultiplyOutput.txt
!  	returns the time taken in seconds to complete Matrix Multiplication
!5. In the main() function add code to do the following:
!	   For fixed matrices of size NRA x NCA and NCA x NCB
!      call the serialFunction to multiply matrices
!      for various number fo threads (2,3,4,5,6,....number of processors in machine)
!         call parallelFunction to multiply matrices
!         compute speedup(t) = Time Takes By Serial Code / Time taken by parallel code with t threads
!         print the speedup
!6. How does speedup change with increasing number of threads?
!7. What would you expect to happen if the number of threads was increased to more than the number of cores?
!8. Do you think there will be a change in the speed up if Schedule - chunk clause was used?
!9. Add a new function which is similar to the parallel function, but accepts as input a chunk size too
!10.  double MatrixMultWithScheduleChunk(int num_threads, int chunk_size, int NRA, int NCA, int NCB)
!11. add a static schedule with chunk size in the #pragma omp for
!12. Evaluate speedup for various chunk sizes.
!13. Discuss
!*****************************************************************************/

!***************************************************************************************
! Will take in rows and cols and return a matrix with random number bewteen 1 and 10
!***************************************************************************************
subroutine initMatrix( rows,  cols, matrix)
    implicit none
    integer, intent(in) :: rows, cols
    REAL, DIMENSION(rows,cols), intent(out):: matrix
    integer i,j
    real r
    call random_seed()
	!$omp parallel do
	do i = 1, rows
		do j = 1, cols
			 CALL RANDOM_NUMBER(r)
             matrix(i,j) = FLOOR(10*r)
		end do
	end do

end subroutine

!***************************************************************************************
! Will take in rows and cols and return a matrix with Zeroes
!***************************************************************************************
subroutine initMatrixToZeroes( rows,  cols, matrix)
    implicit none
    integer, intent(in) :: rows, cols
    REAL, DIMENSION(rows,cols), intent(out):: matrix
    integer i,j
    real r
	!$omp parallel do
	do i = 1, rows
		do j = 1, cols
             matrix(i,j) = 0
		end do
	end do

end subroutine

!*******************************************************************************************
! Parallel code for matrix multiplication using #pragma omp for with the default schedule
! where the iterations are divided equally among the various threads.
! Number of Threads is passed as a parameter to the function to allow us to evaluate speed up
! of parallel code for various number of threads.
! 
!*******************************************************************************************
subroutine MatrixMultWithOMPFOR(numThreads, NRA, NCA, NCB, time)

	integer , intent(in) ::NRA, NCB, NCA, numThreads
	double precision, intent(out) ::time
	integer	tid, nthreads, i, j, k, chunk, omp_get_thread_num, omp_get_num_threads
	double precision start_secs, end_secs, omp_get_wtime

	REAL, DIMENSION(NRA,NCA) :: A
	REAL, DIMENSION(NCA,NCB) :: B
	REAL, DIMENSION(NRA,NCB) :: C
	print *, "Starting matrix multiple example with ", numThreads, " threads"
	print *, "Initializing matrices..."
	start_secs = omp_get_wtime ( );

	call initMatrix(NRA, NCA, A);
	call initMatrix(NCA, NCB, B);
	call initMatrixToZeroes(NRA, NCB, C);

	end_secs = omp_get_wtime ( );
	print *, "Matrix Intitialization took ", end_secs - start_secs, "seconds "

	nthreads = numThreads

	call omp_set_num_threads(numThreads);

!Spawn a parallel region explicitly scoping all variables ***/
!$omp parallel shared(a,b,c,nthreads) private(tid,i,j,k)
	tid = omp_get_thread_num();
	if (tid == 0) then
		nthreads = omp_get_num_threads();
		print *, "Starting matrix multiply example with " , nthreads, " threads "
	end if
	!$omp master
  		print *, "Matrix Intitialization took ", initTime, " seconds "
	!$omp end master
!Do matrix multiply sharing iterations on outer loop ***/
!Display who does which iterations for demonstration purposes ***/

	start_secs = omp_get_wtime ( );
	!$omp do
    do i = 1, NRA    
	  do j = 1, NCB
		do k= 1,NCA
	     C(i,j) = C(i,j) + A(i,k) * B(k,j)
        end do
	  end do
 	end do
    !$omp end do
!$omp end parallel   

     end_secs = omp_get_wtime ( );
   print *, "Matrix Multiplication took ",  end_secs - start_secs, " seconds"
! Print results to file
   open(1, file = 'MatrixMultiplyOutput.dat', status = 'unknown')  
   write(1,*) 	"******************************************************"
   write(1,*)   "Result Matrix:"
   do i=1,NRA
     do j = 1, NCB
      write(1,"(F10.2)", advance ='no') C(i,j)   
     end do
   end do  
   
   close(1) 
   print *, "******************************************************"

   time = end_secs-start_secs

end subroutine


!***************************************************************************************
! Using a Schedule clause with chunksize for threads in the DO loop
!
!***************************************************************************************
subroutine MatrixMultWithScheduleChunk(numThreads, chunk_size, NRA, NCA, NCB, time)
	integer , intent(in) ::NRA, NCB, NCA, numThreads, chunk_size
	double precision, intent(out) ::time
	integer	tid, nthreads, i, j, k, chunk, omp_get_thread_num, omp_get_num_threads
	double precision start_secs, end_secs, omp_get_wtime

	REAL, DIMENSION(NRA,NCA) :: A
	REAL, DIMENSION(NCA,NCB) :: B
	REAL, DIMENSION(NRA,NCB) :: C
	print *, "Starting matrix multiple example with ", numThreads, " threads"
	print *, "Initializing matrices..."
	start_secs = omp_get_wtime ( );

	call initMatrix(NRA, NCA, A);
	call initMatrix(NCA, NCB, B);
	call initMatrixToZeroes(NRA, NCB, C);

	end_secs = omp_get_wtime ( );
	print *, "Matrix Intitialization took ", end_secs - start_secs, "seconds "

	nthreads = numThreads

	call omp_set_num_threads(numThreads);

!Spawn a parallel region explicitly scoping all variables ***/
!$omp parallel shared(a,b,c,nthreads) private(tid,i,j,k)
	tid = omp_get_thread_num();
	if (tid == 0) then
		nthreads = omp_get_num_threads();
		print *, "Starting matrix multiply example with " , nthreads, " threads "
	end if
	!$omp master
  		print *, "Matrix Intitialization took ", initTime, " seconds "
	!$omp end master
!Do matrix multiply sharing iterations on outer loop ***/
!Display who does which iterations for demonstration purposes ***/

	start_secs = omp_get_wtime ( );
	!$omp do SCHEDULE(static, chunk_size)
    do i = 1, NRA    
	  do j = 1, NCB
		do k= 1,NCA
	     C(i,j) = C(i,j) + A(i,k) * B(k,j)
        end do
	  end do
 	end do
    !$omp end do 
!$omp end parallel   

     end_secs = omp_get_wtime ( );
   print *, "Matrix Multiplication took ",  end_secs - start_secs, " seconds"
! Print results to file
   open(1, file = 'MatrixMultiplyOutput.dat', status = 'unknown')  
   write(1,*) 	"******************************************************"
   write(1,*)   "Result Matrix:"
   do i=1,NRA
     do j = 1, NCB
      write(1,"(F10.2)", advance ='no') C(i,j)   
     end do
   end do  
   
   close(1) 
   print *, "******************************************************"

   time = end_secs-start_secs
end subroutine


!***************************************************************************************
! Serial code for multiplying 2 matrices of size NRA x NCA and NCA X NCB
! Will return the time taken to multiply the matrices and will save the result matrix into 
! a file
!***************************************************************************************
subroutine MatrixMultSerial(NRA, NCA, NCB, time)
    integer , intent(in) ::NRA, NCB, NCA
    double precision, intent(out) ::time
	integer	tid, nthreads, i, j, k, chunk
	double precision start_secs, end_secs, omp_get_wtime

    REAL, DIMENSION(NRA,NCA) :: A
    REAL, DIMENSION(NCA,NCB) :: B
    REAL, DIMENSION(NRA,NCB) :: C
    print *, "Starting matrix multiple example with 1 thread"
	print *, "Initializing matrices..."
    start_secs = omp_get_wtime ( );

    call initMatrix(NRA, NCA, A);
    call initMatrix(NCA, NCB, B);
    call initMatrixToZeroes(NRA, NCB, C);

   end_secs = omp_get_wtime ( );
   print *, "Matrix Intitialization took ", end_secs - start_secs, "seconds "

   ! Do matrix multiply sharing iterations on outer loop ***/
   ! Display who does which iterations for demonstration purposes ***/

   start_secs = omp_get_wtime ( );
   do i=1,NRA    
		 do j = 1, NCB
		   do k = 1, NCA
		    C(i,j) = C(i,j) +  A(i,k) * B(k,j)
		   end do
         end do
   end do
   end_secs = omp_get_wtime ( );
   print *, "Matrix Multiplication took ",  end_secs - start_secs, " seconds"
	
    ! Print results to file
   open(1, file = 'MatrixMultiplyOutput.dat', status = 'unknown')  
   write(1,*) 	"******************************************************"
   write(1,*)   "Result Matrix:"
   do i=1,NRA
     do j = 1, NCB
      write(1,"(F10.2)", advance ='no') C(i,j)   
     end do
   end do  
   
   close(1) 
   print *, "******************************************************"

   time = end_secs-start_secs
end subroutine

!##########################################################################
!
!
!##########################################################################
program main
   REAL, DIMENSION(1000,500) :: A
   integer NRA, NCA, NCB
   double precision serialTime, parallelTime
   NRA = 1000
   NCA = 500
   NCB = 1000
   !call initMatrix(NRA, NCA, A)
   !call initMatrix(NCA, NCB, B)
   !call initMatrixToZeroes(NRA, NCB, C)
   call MatrixMultSerial(NRA,NCA, NCB, serialTime)


   print *, "******************************************************"
   print *, "Parallel Code #pragma omp for....."
   do i = 4,  32, 2
      print *,"Number of Threads =", i
      call MatrixMultWithOMPFOR(i, NRA, NCA, NCB, parallelTime);
      speedup =serialTime/parallelTime;
      print *, "SpeedUp with ", i, " Threads = ", serialTime, "/", parallelTime, " = ", speedup
      print *, "******************************************************"
   end do   
   print *, "Parallel Code #pragma omp for schedule(chunk = 5)....."
   do i = 4,  32, 2
      print *,"Number of Threads =", i
      call MatrixMultWithScheduleChunk(i,5, NRA, NCA, NCB, parallelTime);
      speedup = serialTime/parallelTime;
      print *, "SpeedUp with ", i, " Threads = ", serialTime, "/", parallelTime, " = ", speedup
      print *, "******************************************************"
   end do
   print *, "Parallel Code #pragma omp for schedule(chunk = 10)....."
   do i = 4,  32, 2
      print *,"Number of Threads =", i
      call MatrixMultWithScheduleChunk(i,10, NRA, NCA, NCB, parallelTime);
      print *, "SpeedUp with ", i, " Threads = ", serialTime, "/", parallelTime, " = ", speedup
      print *, "******************************************************"
   end do

end program 
