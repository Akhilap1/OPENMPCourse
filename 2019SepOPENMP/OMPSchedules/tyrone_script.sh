#!/bin/sh
#PBS -N jobname
#PBS -l nodes=1:ppn=32:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /working/directory/path

g++ -fopenmp Ex2Schedules.cpp  -o Schedules.exe
gfortran -fopenmp Ex2Schedules.f95  -o SchedulesF.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`

 ./Schedules.exe > Schedules.out
 ./SchedulesF.exe > SchedulesF.out

