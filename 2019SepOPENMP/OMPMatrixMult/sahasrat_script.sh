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

CC -fopenmp  Ex5MM_Multiply.cpp -o MM_Multiply.exe

#COMPILATION OF FORTRAN CODE

#ftn -fopenmp Ex5MM_Multiply.cpp -o MM_MultiplyF.exe

#EXECUTABLE FILE TO RUN

./MM_Multiply.exe > MM_Multiply.out
#./MM_MultiplyF.exe > MM_MultiplyF.out




