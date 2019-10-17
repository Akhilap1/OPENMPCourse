
# Use cray C++ compiler
CC = CC
#compiler flag for OPENMP is not required for Cray Compiler- the default CC enables OPENMP
#please uncomment theis if you are compiling on gnu or intel c++ compilers
#CFLAGS = -fopenmp

Ex1HelloWorld:
	${CC}  Ex1HelloWorld.cpp -o Ex1HelloWorld.out
#lib:
#       ${CC} ${CFLAGS} lib.c -o lib

clean:
	 rm -f Ex1HelloWorld.out

