#!/bin/bash
source /afs/cern.ch/cms/cmsset_default.sh
function peval { echo -e ">>> $@"; eval "$@"; }

mydir=CFGDIR
indir=INDIR
storedir=STOREDIR
startdir=T2_TMP_DIR/gain_calib_NUM
mkdir -p $startdir
cd $mydir

echo "Setting the environment ..."
eval `scramv1 runtime -sh`

cp gain_calib_NUM_cfg.py $startdir/
cd $startdir
echo -e "************************"
echo -e "  => ls: \n`ls`"
echo -e "************************\n\n"

echo -e "Copying files from storage to local ..."
file=GainCalibration_NUM_RUN.EXT
echo "(T2_CP $indir/$file $file)"
T2_CP T2_PREFIX$indir/$file $file

#convfile="vcal-irradiation-factors.txt"
#echo "(cp $mydir/../$convfile $convfile)"
#cp $mydir/../$convfile $convfile
sqlite="siPixelVCal.db"
echo "(cp $mydir/../$sqlite $sqlite)"
cp $mydir/../$sqlite $sqlite
echo -e "************************"
echo -e "  => ls: \n`ls`"
echo -e "************************\n\n"

echo -e "Running CMSSW job:"
cmsRun gain_calib_NUM_cfg.py
cat *.log

echo -e "Copying output to pnfs:"
echo "(T2_OUT_CP "GainCalibration.root" ${storedir}/NUM.root)"
#remove leading "/" to make eos cp work
if [ ${storedir:0:1} = "/" ]; then storedir=${storedir:1};fi
T2_OUT_CP GainCalibration.root T2_PREFIX${storedir}/NUM.root

echo -e "end ... \n\n\n"

cp *.log ${mydir}/JOB_NUM/
cp *.txt ${mydir}/JOB_NUM/
cd $mydir
rm -fr $startdir
