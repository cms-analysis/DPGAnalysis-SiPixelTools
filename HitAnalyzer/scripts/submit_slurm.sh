#!/bin/bash
# 
#SBATCH -p wn
#SBATCH --time 04:00:00
#SBATCH -w t3wn60
#SBATCH -e cn-test.err 
#SBATCH -o cn-test.out  # replace default slurm-SLURM_JOB_ID.out

echo HOME: $HOME 
echo USER: $USER 
echo SLURM_JOB_ID: $SLURM_JOB_ID
echo HOSTNAME: $HOSTNAME

# each worker node has local /scratch space to be used during job run
mkdir -p /scratch/$USER/${SLURM_JOB_ID}
sleep 10
#
cmsRun SimsToClus.py
# here comes a computation
rmdir  /scratch/$USER/${SLURM_JOB_ID}
date


