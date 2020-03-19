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
echo PWD: $PWD

MYPWD=$PWD
echo $MYPWD

export X509_USER_PROXY=/t3home/kotlinski/.x509up_u1373
# export X509_USER_PROXY=/tmp/x509up_u1373  # does not work, copy from this to myhome 

echo JOB_ID: $SLURM_JOB_ID 
echo $X509_USER_PROXY

# each worker node has local /scratch space to be used during job run
mkdir -p /scratch/$USER/${SLURM_JOB_ID}
sleep 10
#
cd /scratch/$USER/${SLURM_JOB_ID}
#cmsRun $MYPWD/SimsToClus.py
#cmsRun $MYPWD/SimsToRec.py
#cmsRun $MYPWD/RawToRec.py
#cmsRun $MYPWD/gen_sim.py
cmsRun $MYPWD/digitize.py
# 
pwd
ls 
cd $MYPWD
pwd
ls /scratch/$USER/${SLURM_JOB_ID}
#cp /scratch/$USER/${SLURM_JOB_ID}/simtorec.root $MYPWD/.
#cp /scratch/$USER/${SLURM_JOB_ID}/rawtoreco.root $MYPWD/.

# xrdcp -d 1 -f s.root root://t3se01.psi.ch:1094//store/user/kotlinski/MC/test/s.root  # is OK,
# xrdcp      -f s.root root://t3dcachedb.psi.ch:1094//pnfs/psi.ch/cms/trivcat/store/user/kotlinski/MC/test/. # OK
# xrdcp -f /scratch/$USER/${SLURM_JOB_ID}/s.root root://t3dcachedb.psi.ch:1094//pnfs/psi.ch/cms/trivcat/store/user/kotlinski/MC/test/s.root   # OK
#xrdcp -f /scratch/$USER/${SLURM_JOB_ID}/s.root root://t3se01.psi.ch:1094//store/user/kotlinski/MC/mu_pt100/simhits/simHits2_eta0p1.root # OK
xrdcp -f /scratch/$USER/${SLURM_JOB_ID}/d.root root://t3se01.psi.ch:1094//store/user/kotlinski/MC11/mu_pt100/raw/raw1.root # OK

#  rmdir  /scratch/$USER/${SLURM_JOB_ID}
rm -rf  /scratch/$USER/${SLURM_JOB_ID}
date


