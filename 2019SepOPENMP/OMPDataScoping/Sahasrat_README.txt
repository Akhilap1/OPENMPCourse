EXERCISE: OpenMP Datascoping Program

LOGIN: ssh username@sahasrat.serc.iisc.ernet.in
PASSWORD: enter the provided password 

COMMAND TO RUN:
mkdir <username>_2019MayOMP
cd <username>_2019MayOMP
cd  ComputePI
pwd  #print working directory

MODULE TO LOAD IN THE SAHASRAT
module swap PrgEnv-cray/6.0.4 PrgEnv-gnu/6.0.4
module load gcc/8.2.0
module load pbs

SAMPLE PBS JOBSCRIPT:

Copy the below sample jobscript and paste it into the pbsjob.sh

vi pbsjob.sh
>press "i" for insert
>paste the below sample jobscript into the file.
>Press "Esc", then "Shift+Q" or "Shift+:" 
>save the file with "wq" and press "Enter Tab"

1. SAHASRAT JOBSCRIPT
******************************************************
#!/bin/sh 
#This job should be redirected to idqueue
#PBS -N OMPHelloWorld_24threads
#PBS -l select=1:ncpus=24
#PBS -l walltime=00:02:00
#PBS -l place=scatter
#PBS -l accelerator_type="None"
#PBS -S /bin/sh@sdb -V 
export OMP_NUM_THREADS=24
. /opt/modules/default/init/sh
cd /home/proj/16/secpraba/2019MayOPENMP/OMPDataScoping
aprun -j 1 -n 1 -N 1 -d $OMP_NUM_THREADS ./Ex3DataScoping.out


#ftn -fopenmp Ex3DataScoping.f95 -o DataScopingF.exe

***************************************************** 

RUN THE PBS JOBSCRIPT:
qsub pbsjob.sh
qstat -a #to check the status of the job
qstat -r #to check the job is in running state

CHECK THE OUTPUT FILES:
Total 2 files will generate after job completion like below:
 jobscript*.o***** #output file
 jobscript*.e***** #error file 


