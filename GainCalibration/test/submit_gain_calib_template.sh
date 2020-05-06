#! /bin/bash
function peval { echo -e ">>> $@"; eval "$@"; }

echo -e "Starting job at ..." `date`
DATE_START=`date +%s`
[ -z "$1" ] && echo ">>> FED is not given as input!" && exit 1

run=$RUN
fed=$1
ext='dmp'
rundir=$RUNDIR
indir=$INDIR
outdir=$OUTDIR
workdir="$TMPDIR/job_GainCalib_${run}_${fed}"
logdir="${rundir}/job_${fed}"
cpcmd="xrdcp -f --nopbar"
cpurl="root://eoscms.cern.ch//eos/cms/"

# remove leading "/" to make eos cp work
if [ ${outdir:0:1} = "/" ]; then outdir=${outdir:1}; fi

# FILES
script="gain_calib_cfg.py"
infile="GainCalibration_${fed}_${run}.${ext}"
sqlite="siPixelVCal.db" # VCal DB
outfile="GainCalibration.root"

# ENVIRONMENT
echo ">>> Setting the environment..."
peval "source /afs/cern.ch/cms/cmsset_default.sh"
peval "cd $rundir"
peval "eval `scramv1 runtime -sh`"

# WORKDIR
peval "mkdir -p $workdir"
peval "cd $workdir"

# RETRIEVE SCRIPT, CALIBRATION & VCAL DB
echo ">>> Copying input files from storage to local ..."
peval "$cpcmd $cpurl$indir/$infile $infile"
peval "cp $rundir/../$sqlite $sqlite"
peval "cp $rundir/$script $script"
echo "************************"
peval "ls $rundir"
echo "************************"
peval "ls"
echo -e "************************\n"

# RUN SCRIPT
echo ">>> Running CMSSW job:"
peval "cmsRun $script run=$run fed=$fed"
peval "cat *.log"

# COPY BACK RESULT
echo ">>> Copying output to pnfs:"
peval "$cpcmd $outfile $cpurl${outdir}/$fed.root"
peval "mkdir $logdir"
peval "cp *.log $logdir/"
peval "cp *.txt $logdir/"

# CLEAN
cd $rundir
peval "rm -fr $workdir"

# DONE
echo -e ">>> Job done... \n\n"
DATE_END=`date +%s`
RUNTIME=$((DATE_END-DATE_START))
printf ">>> Wallclock running time: %02d:%02d:%02d" "$(( $RUNTIME / 3600 ))" "$(( $RUNTIME % 3600 /60 ))" "$(( $RUNTIME % 60 ))"
