# Use cray C++ compiler
CC = CC
#compiler flag for OPENMP is not required for Cray Compiler- the default CC enables OPENMP
#please uncomment theis if you are compiling on gnu or intel c++ compilers
#CFLAGS = -fopenmp

Ex2Schedules:
	${CC}  Ex2Schedules.cpp -o Ex2Schedules.out
#lib:
#       ${CC} ${CFLAGS} lib.c -o lib

clean:
	rm -f Ex2Schedules.out

