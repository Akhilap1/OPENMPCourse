#!/bin/sh
#PBS -N openmp_job
#PBS -l nodes=1:ppn=32:debug
#PBS -l walltime=02:00:00
#PBS -V

cd /home/proj/16/secpraba/2019MayOPENMP/OMPSchedules 
g++ -fopenmp Ex2Schedules.cpp  -o Schedules.exe
gfortran -fopenmp Ex2Schedules.f95  -o SchedulesF.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`
 
 ./Schedules.exe > Schedules.out
 ./SchedulesF.exe > SchedulesF.out

