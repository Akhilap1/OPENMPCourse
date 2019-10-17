#!/bin/sh

#PBS -l accelerator_type="None"
#PBS -N Job_name
#PBS -l walltime=00:10:00
#PBS -q R204573
#PBS -l place=scatter
#PBS -S /bin/sh@sdb -V 
. /opt/modules/default/init/sh


cd /working/directory/path

#COMPILATION OF CPP CODE

CC -fopenmp Ex6Fibonacci.cpp  -o Fibonacci.exe

#COMPILATION OF FORTRAN CODE

#ftn -fopenmp Ex6Fibonacci.f95 -o Fibonaccif95.exe

#EXECUTABLE FILE TO RUN 

./Fibonacci.exe >  Fibonacci.out
#./Fibonaccif95.exe >  Fibonaccif95.out




