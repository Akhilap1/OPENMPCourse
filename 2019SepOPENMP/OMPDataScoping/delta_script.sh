#!/bin/sh
#PBS -N jobname
#PBS -l nodes=1:ppn=16:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /working/directory/path

g++ -fopenmp Ex3DataScoping.cpp  -o DataScoping.exe
#gfortran -fopenmp Ex3DataScoping.f95 -o DataScopingF.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`
 
./DataScoping.exe >  DataScoping.out
#./DataScopingF.exe >  DataScopingF.out

