#!/bin/sh
#PBS -N openmp_job
#PBS -l nodes=1:ppn=32:debug
#PBS -l walltime=02:00:00
#PBS -V
cd /home/proj/16/secpraba/2019MayOPENMP/PerfTool

g++ -fopenmp MXVPerfTool.cpp  -o MXV.exe

#gfortran -fopenmp Ex4ComputePI.f95 -o ComputePI.exe
NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`

perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./MXV.exe 1000 1000 >  MXV.out
perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./MXV.exe 10000 100 >> MXV.out
perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./MXV.exe 100000 10 >> MXV.out
perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./MXV.exe 10 100000 >> MXV.out
perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./MXV.exe 100 10000 >> MXV.out
 

