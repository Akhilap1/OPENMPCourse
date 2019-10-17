
# Use cray C++ compiler
CC = CC
#compiler flag for OPENMP is not required for Cray Compiler- the default CC enables OPENMP
#please uncomment theis if you are compiling on gnu or intel c++ compilers
#CFLAGS = -fopenmp

Ex4ComputePI:
	${CC}  Ex4ComputePI.cpp -o Ex4ComputePI.out
#lib:
#       ${CC} ${CFLAGS} lib.c -o lib

clean:
	 rm -f Ex4ComputePI.out

