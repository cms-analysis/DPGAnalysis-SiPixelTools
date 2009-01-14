#!/bin/bash

############### runPixelNtuplizer_RealData.csh
# created by Romain Rougny and Luca Mucibello
#
# usage: first time :   chmod u+x runPixelNtuplizer_RealData.csh inputFilesintoCFG.csh
#        then :         ./runPixelNtuplizer_RealData.csh DirectoryWhereTheJobsWillBeLaunched
#
# description:
# This script launches the Ntuplizer from a filelist. 


###########################################
# MODIFY THESE TO YOUR PREFERRED SETTINGS
###########################################

# number of input files per job
        Nfile=200 # has to be <255 !
	
# Filelist as input. The Syntax is:
# "file1.root"
# "file2.root"
        Filelist=filelist.txt
# Global Tag Name
	globaltagname="CRAFT_ALL_V4"
# fill out your preferred castor output directory here
	castoroutputdir="/castor/cern.ch/user/r/rougny"
# preferred batch queue. see bqueues -u myname for available queues.
	batchqueuename="cmscaf"


#################
# MAIN PROGRAMM
#################


DIR=$1
mydir=`pwd`/${DIR}

if [ ! -d ${DIR} ]; then 
  mkdir ${DIR}
else
  rm -fr ${DIR}/*
fi

./inputFilesintoCFG.csh $Nfile $Filelist

let i=0

for file in template*; do
let i=$i+1
echo Creating job $i
sed "s#MYGLOBALTAGNAME#$globaltagname#" < $file > temp1
sed "s#OUTPUTFILE#NTUPLIZER_${globaltagname}_JOB_$i.root#" < temp1 >  ${DIR}/pixelNtuplizer_${globaltagname}_cfg_$i.py
rm -f temp1 

sed "s#PYFILE#pixelNtuplizer_${globaltagname}_cfg_$i.py#" < batchscript_template > temp
sed "s#CURRENTDIR#${mydir}#" < temp > temp1
sed "s#CASTORDIR#$castoroutputdir#" < temp1 > ${DIR}/submit_$i.csh
rm -fr temp temp1

cd $DIR

bsub -q $batchqueuename -J job_$i < submit_$i.csh

cd ..
done

rm -f template*
