
This project consists of a set of OpenMP coding exercise used for my introductory OPENMP workshop at the Indian Institte of Science (Bangalore), Supercomputing Center. 
These workshops are frequently conducted. Code for the first revision is from my 5th workshop and the most recent one in September 2019.
Programs are primarily written in C or C++. An attempt has been made to write the same programs in fortran too, but that is still incomplete. I hope to complete it before the next workshop or when I find enough time.

Exercises include serial and parallel code as functions in the same program. Each folder in this project is given a meaningful name following the prefix "OMP" and should be self contained with a lot of comments and explanation in the beginning of the program.

Code base here illustrates the following in Serial and Parallel. 
HelloWorld
Fobonacci Series
Compute PI
Matrix Multiplication
Mandelbrot Set 
OpenMP Schedules
OpenMP Data Scoping

Included along with the code is a makefile and a jobscript for PBS Scheduler on SahasraT (http://www.serc.iisc.ac.in/supercomputer/for-traditional-hpc-simulations-sahasrat/).Some folders might also contan a jobscript for the torque scheduler on other small HPC clusters namely delta or tyronne (http://www.serc.iisc.ac.in/batch-compute-servers/delta-cluster/)
All code written here is for demonstration purposes for getting a hands on into OpenMP. These cannot be used for production runs. One could use the same code as a starting point and modify it appropriately for their needs. For novices, it is also important to understand that parallel systems come with their own proprietary linear algebra and math librarues that sould be the first choice for matrix multiplication functions.

Acknowledgment:Most programs are adopted from the ones written by John Burkardt - https://people.sc.fsu.edu/~jburkardt/

