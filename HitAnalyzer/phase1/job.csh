#!/bin/tcsh
echo $home

source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh

#rfdir /castor/cern.ch/cms/store/data

setenv SCRAM_ARCH slc6_amd64_gcc493
cd /afs/cern.ch/user/d/dkotlins/public/CMSSW/CMSSW_7_6_0_pre7/src

#source /afs/cern.ch/cms/sw/cmsset_default.csh
source /afs/cern.ch/project/gd/apps/cms/cmsset_default.csh   # this works with gcc462

# must be run frm the release area
eval `scram runtime -csh`

cd DPGAnalysis-SiPixelTools/HitAnalyzer/phase1
pwd

# whole CMS 
#cmsRun gen_sim.py
#cmsRun testDigitizer.py

# pixel only  
#cmsRun testPixelDigitizer.py
#cmsRun runDigisToClus.py
#cmsRun runDigisToRecHits.py
cmsRun SimsToClus.py

# pixels & strips 
#cmsRun Digitze_Pixels_And_Strips.py
#cmsRun runPixelRH.py
#cmsRun runPixelRawToRecHitsAna.py
#cmsRun runPixelRawToRecHits.py
#cmsRun runClusToTracks.py
#cmsRun runClusToRecHits.py
#cmsRun runClusToRecHits_data.py


 




