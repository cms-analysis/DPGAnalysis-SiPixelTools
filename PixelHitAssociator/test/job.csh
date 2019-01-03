#!/bin/tcsh
echo $home

source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh

cd /afs/cern.ch/user/d/dkotlins/public/CMSSW/CMSSW_10_4_0_pre3/src

setenv SCRAM_ARCH slc6_amd64_gcc700

#source /afs/cern.ch/cms/sw/cmsset_default.csh
source /afs/cern.ch/project/gd/apps/cms/cmsset_default.csh  


# must be run frm the release area
eval `scram runtime -csh`

cd DPGAnalysis-SiPixelTools/PixelHitAssociator/test

#cmsRun Sims_To_ValidRecHits.py
cmsRun Raw_To_RecHitsValid.py




