#!/bin/sh
#PBS -N jobname
#PBS -l nodes=1:ppn=32:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /working/directory/path

g++ -fopenmp Ex6Fibonacci.cpp  -o Fibonacci.exe
#gfortran -fopenmp Ex6Fibonacci.f95 -o Fibonaccif95.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`

./Fibonacci.exe >  Fibonacci.out
#./Fibonaccif95.exe >  Fibonaccif95.out

