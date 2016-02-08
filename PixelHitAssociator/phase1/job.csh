#!/bin/tcsh
echo $home

#source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh
cd /afs/cern.ch/user/d/dkotlins/public/CMSSW/CMSSW_7_6_0_pre7/src

# for V7
setenv SCRAM_ARCH slc6_amd64_gcc491

# must be run frm the release area
eval `scram runtime -csh`

cd DPGAnalysis-SiPixelTools/PixelHitAssociator/phase1 

#cmsRun DigisToRecHitsValid.py
#cmsRun Sims_To_ValidRecHits.py
cmsRun runSimitsToRecHitsValid_customlhc.py




