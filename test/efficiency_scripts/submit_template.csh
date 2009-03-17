#!/bin/csh

source /afs/cern.ch/cms/sw/cmsset_default.csh
setenv storedir STOREDIR
setenv startdir `pwd`

cd ${storedir}
cmsenv
#setenv STAGE_SVCCLASS wan
setenv STAGE_SVCCLASS cmscaf
cp CFGFILE ${startdir}

cd ${startdir}
cmsRun CFGFILE
cp -fr *.root ${storedir}/.

