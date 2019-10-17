! ***************************************************************************************************************
! 
! Akhila Prabhakaran OPENMP Course SERC May 2019
! 
! Exercise 3: Exploring Shared, Private, FirstPrivate and lastPrivate
! Duration: 5 minutes
! 
! Login to the workshop machine 
!   ssh username@delta-cluster.serc.iisc.ernet.in
!   enter the password provided
! 
!   pwd (print working directory)
!   mkdir username_2019MayOMP
!   cd username_2019MayOMP
!   
!   Compile and Run the following program to understand various data scoping clauses
!   
! 
! 
! *******************************************************************************************************************
! ********************************************************************************************************************
! In a parallel region, any data declared outside it will be shared: 
! any thread using a variable  x will access the same memory location 
! associated with that variable.
! ********************************************************************************************************************
SUBROUTINE exampleSharedVar()
  integer x, omp_get_thread_num, omp_get_num_threads
  x = 5
  PRINT *, "X is a shared varible for all threads"
  PRINT *, "X is initialised to %d before the #pragma omp parallel region", x
  !$omp parallel
    x = x+1;
    print *, "X is updated by thread",  omp_get_thread_num(), "to value ", x
  !$omp end parallel 
  print *, "Value of X at the end of the parallel region is:", x

  x = 0;
  print *, "X is reset to ", x, "before the #pragma omp parallel for region "
  !$omp parallel do
   do i = 1, 10
    x = x+1;
    print *, "X is updated by thread ", omp_get_thread_num()," to value ", x
  end do
!$omp end parallel do

  print *, "Value of X at the end of the parallel for region is:", x

END SUBROUTINE
! ******************************************************************************
! The first guideline is to always write parallel regions with the default(none) clause. 
! This forces the programmer to explicitly think about the data-sharing attributes of all variables.
! The second guideline is to declare private variables inside parallel regions whenever possible. 
! This guideline improves the readability of the code and makes it clearer.

! ******************************************************************************
SUBROUTINE examplePrivateVar()
  integer x, omp_get_thread_num, omp_get_num_threads

  x = 5
  print *, "X is a Private varible for all threads"
  print *, "X is not explicitly initialised in the parallel region. Could result in bad data/garbage"
  print *, "X is initialised to ", x ,"before the #pragma omp parallel region " 
  !$omp parallel private(x)
    x = x+1;
    print *, "X is updated by thread ", omp_get_thread_num(), "to value %d ", x
 
  !$omp end parallel
  print *, "Value of X at the end of the parallel region is: ", x

  x = 0;
  print *, "X is reset to ", x , "before the #pragma omp parallel for region "
  print *, "X is not explicitly initialised in the parallel region. Could result in bad data/garbage"

  !$omp parallel do private(x)
  do i = 1, 10
    x = x+1;
    print *, "X is updated by thread ", omp_get_thread_num()," to value ", x
  end do
!$omp end parallel do

  print *, "Value of X at the end of the parallel for region is:", x

END SUBROUTINE


! ****************************************************************
! Above, you saw that private variables are completely separate 
! from any variables by the same name in the surrounding scope. 
! However, there are two cases where you may want some storage association 
! between a private variable and a global counterpart.
! First of all, private variables are created with an undefined value.
!  You can force their initialization with clause{firstprivate}.
!  In the following SUBROUTINE variable t behaves like a private variable, 
! except that it is initialized to the outside value.
! ****************************************************************
SUBROUTINE firstPrivate()
  integer t, omp_get_thread_num, omp_get_num_threads
  t = 2
!$omp parallel firstprivate(t)
    t = t+ omp_get_thread_num();
    print *, "t is updated by thread ", omp_get_thread_num(), "to value ",  t
!$omp end parallel
  print *, "Value of t after the parallel region is ", t
end SUBROUTINE

! ****************************************************************
! Secondly, you may want a private value to be preserved to the 
! environment outside the parallel region. 
! This really only makes sense in one case, 
! where you preserve a private variable from the last iteration of a parallel loop, 
! or the last section in an sections construct. This is done with 
! clause{lastprivate}:
! NOTE: lastprivate works only only a omp parallel for OR omp sections
! Also note that the value is from the LAST iteration of the for loop, NOT from the thread that runs last
! ****************************************************************
SUBROUTINE lastPrivate()
  integer t, m,  omp_get_thread_num, omp_get_num_threads
  t=2
  m= 0
!$omp parallel do firstprivate(t) lastprivate(m)
  do i = 1, 10
    t = t+ omp_get_thread_num();
    m = i*i;
    print *, "t is updated by thread ", omp_get_thread_num(), "to value ",  t
  end do
  print *, "Value of t after the parallel region is ", t
  print *, "Value of m after the parallel region is ", m


!$omp parallel do shared(t) lastprivate(m)
  do i= 1, 10
    t = i*i;
    m = i*i;
    print *, "t is updated by thread ", omp_get_thread_num(), "to value ",  t
    print *, "m is updated by thread ", omp_get_thread_num(), "to value ",  m
  end do
!$omp end parallel do

  print *, "Value of t after the parallel region is ", t
  print *, "Value of m after the parallel region is ", m

end SUBROUTINE

! ---------------------------------------
! ---------------------------------------
program main
    call exampleSharedVar();
    call examplePrivateVar();
    call firstPrivate();
    call lastPrivate();
end program
