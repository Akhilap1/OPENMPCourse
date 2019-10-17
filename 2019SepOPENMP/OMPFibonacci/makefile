
# Use cray C++ compiler
CC = CC
#compiler flag for OPENMP is not required for Cray Compiler- the default CC enables OPENMP
#please uncomment theis if you are compiling on gnu or intel c++ compilers
#CFLAGS = -fopenmp

Ex6Fibonacci:
	${CC}  Ex6Fibonacci.cpp -o Ex6Fibonacci.out -target-cpu=x86-64
#lib:
#       ${CC} ${CFLAGS} lib.c -o lib

clean:
	 rm -f Ex6Fibonacci.out

