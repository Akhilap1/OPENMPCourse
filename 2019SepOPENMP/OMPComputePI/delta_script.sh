#!/bin/sh
#PBS -N jobname
#PBS -l nodes=1:ppn=16:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /working/directory/path

g++ -fopenmp Ex4ComputePI.cpp  -o ComputePI.exe
#gfortran -fopenmp Ex4ComputePI.f95 -o ComputePIF.exe

#gfortran -fopenmp Ex4ComputePI.f95 -o ComputePI.exe
NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`
 
 ./ComputePI.exe >  ComputePI.out
#./ComputePIf98.exe >  ComputePIf98.out
