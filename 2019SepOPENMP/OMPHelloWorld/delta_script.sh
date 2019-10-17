#!/bin/sh
#PBS -N jobname
#PBS -l nodes=1:ppn=16:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /working/directory/path

g++ -fopenmp Ex1HelloWorld.cpp -o HelloWorld.exe
#gfortran -fopenmp Ex1HelloWorld.cpp -o HelloWorldF.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`

./HelloWorld.exe > Helloworld.out
#./HelloWorldF.exe > HelloworldF.out

