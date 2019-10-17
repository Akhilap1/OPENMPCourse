EXERCISE: OpenMP ComputePI Program

DELTA-CLUSTER LOGIN
LOGIN: ssh username@delta-cluster.serc.iisc.ernet.in
PASSWORD: enter the provided password 

TYRONE-CLUSTER LOGIN
LOGIN: ssh username@tyrone-cluster.serc.iisc.ernet.in
PASSWORD: enter the provided password

COMMAND TO RUN:
mkdir <username>_2019MayOMP
cd <username>_2019MayOMP
cd  ComputePI
pwd  #print working directory


SAMPLE PBS JOBSCRIPT:

Copy the below sample jobscript and paste it into the pbsjob.sh

vi pbsjob.sh
>press "i" for insert
>paste the below sample jobscript into the file.
>Press "Esc", then "Shift+Q" or "Shift+:" 
>save the file with "wq" and press "Enter Tab"

1. TYRONE-CLUSTER JOBSCRIPT
******************************************************
#!/bin/sh
#PBS -N openmp_job
#PBS -l nodes=1:ppn=32:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /mention/working/directory/path

g++ -fopenmp ComputePI.cpp  -o ComputePI.exe
#gfortran -fopenmp Ex4ComputePI.f95 -o ComputePIF.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`
 
./ComputePI.exe >  ComputePI.out
#./ComputePIF.exe >  ComputePIF.out

*******************************************************

2.DELTA-CLUSTER JOBSCRIPT
******************************************************
#!/bin/sh
#PBS -N openmp_job
#PBS -l nodes=1:ppn=16:custom
#PBS -l walltime=00:10:00
#PBS -V
cd /mention/working/directory/path

g++ -fopenmp ComputePI.cpp  -o ComputePI.exe
#gfortran -fopenmp Ex4ComputePI.f95 -o ComputePIF.exe

NPROCS=`wc -l < $PBS_NODEFILE`
HOSTS=`cat $PBS_NODEFILE | uniq | tr '\n' "," | sed 's|,$||'`

./ComputePI.exe >  ComputePI.out
#./ComputePIF.exe >  ComputePIF.out

*******************************************************

RUN THE PBS JOBSCRIPT:
qsub pbsjob.sh
qstat -a #to check the status of the job
qstat -r #to check the job is in running state

CHECK THE OUTPUT FILES:
Total 3 files will generate after job completion like below:
1. omputePI.out  #output file
2. openmp_job.o***** #output file
3. openmp_job.e***** #error file 


