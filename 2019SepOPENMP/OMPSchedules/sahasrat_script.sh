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

CC -fopenmp Ex2Schedules.cpp  -o Schedules.exe 

#COMPILATION OF FORTRAN CODE

#ftn -fopenmp Ex2Schedules.cpp  -o Schedules.exe

#EXECUTABLE FILE TO RUN 

 ./Schedules.exe > Schedules.out
# ./SchedulesF.exe > SchedulesF.out






