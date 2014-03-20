#!/bin/bash

#source /afs/cern.ch/cms/sw/cmsset_default.sh
export SCRAM_ARCH slc5_amd64_gcc462
cd PATH 
eval `scramv1 runtime -sh`
#cd $TOP
cd -
cmsRun 
cmsStage -f XXXX  DESTINATION/XXXX
rm XXXX 
