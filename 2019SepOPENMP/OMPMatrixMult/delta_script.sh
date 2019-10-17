#!/bin/sh
#PBS -N jobname
#PBS -l nodes=1:ppn=16:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /working/directory/path

g++ -fopenmp Ex5MM_Multiply.cpp -o MM_Multiply.exe
gfortran -fopenmp Ex5MM_Multiply.cpp -o MM_MultiplyF.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`

 ./MM_Multiply.exe > MM_Multiply.out
 ./MM_MultiplyF.exe > MM_MultiplyF.out

