#!/bin/sh
echo $home

source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh

#rfdir /castor/cern.ch/cms/store/data

setenv SCRAM_ARCH slc6_amd64_gcc491
#cd /afs/cern.ch/work/g/grauco/CMSSW_7_4_6/src
cd /afs/cern.ch/work/c/cgalloni/PXL_GIT/CMSSW_8_0_20/src/DPGAnalysis-SiPixelTools/PixelTriplets

#source /afs/cern.ch/cms/sw/cmsset_default.csh
source /afs/cern.ch/project/gd/apps/cms/cmsset_default.sh   # this works with gcc462

# must be run frm the release area
eval `scramv1 runtime -sh`

#cd DPGAnalysis/PixelTriplets
pwd


#cmsRun pxl_257969_Template.py
cmsRun pxl.py
