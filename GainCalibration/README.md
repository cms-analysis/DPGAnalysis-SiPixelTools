# Pixel gain calibration

## Installation
Prepare your working directory with CMSSW
```
mkdir -p ~/public/CMSSW/GainCalibrations/
cd ~/public/CMSSW/GainCalibrations/

source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc820
cmsrel CMSSW_11_1_2
cd CMSSW_11_1_2
cmsenv
git clone https://github.com/cms-analysis/DPGAnalysis-SiPixelTools
rm -r DPGAnalysis-SiPixelTools/PixelDBTools/     # not needed, and does not compile...
rm -r DPGAnalysis-SiPixelTools/PixelTriplets/    # not needed, and does not compile...
rm -r DPGAnalysis-SiPixelTools/HitAnalyzerPhase2 # not needed, and does not compile...
scram b -j 20
cd DPGAnalysis-SiPixelTools/GainCalibration/test
```

## Submission
To launch the gain calibration process, you need to use [`test/run.py`](test/run.py) script,
which will submit one job for each FED to the HTCondor batch system.
First, prepare the job directory with
```
./run.py create RUNNUMBER -i INPUTDIR -o OUTPUTDIR
```
For example,
```
./run.py create 2381 -i /store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_2381 -o /store/group/dpg_tracker_pixel/comm_pixel/$USER/
```
To run from and store to default `Phase1/Run_*` directory, which is what you will do for any "real" gain calibration, simply do
```
./run.py create 2381
```
This creates a folder with a config file storing information about input/output folders
and creates the job scripts for the submission (from the template).
If you want to run over BPIX only, use the `--BPix-only` option
```
./run.py create 2381 --BPix-only
```

## Status & resubmission
Submit your jobs
```
./run.py submit 2381
```
You can check the status of your jobs with `condor_q`, or with
```
./run.py status 2381
```
If jobs fail, please use
```
./run.py resubmit 2381
```
One or more specific FED jobs can be submitted with `--fed`.

## Finalizing
Now, hadd the output
```
./run.py hadd 2381
```
and create a summary pdf
```
./run.py summary 2381
```
Then create a tar file with the output and print some output to be posted at the TWiki (https://twiki.cern.ch/twiki/bin/viewauth/CMS/SiPixelGainCalibrationDoc)
```
./run.py twiki 2381
```
The last step is to produce the payload (a database object)
```
./run.py payload 2381 YEAR VERSION
```
where YEAR is the year the calibration was taken and VERSION is the number of payloads produced in that year.
After this is done, please notify the following people of the location of the database object
Currently (this list might change)
Danek Kotlinksi <danek.kotlinskiATpsi.ch>
Tamás Vámi <vami.tamasATwigner.mta.hu>
Tanja Susa <Tatjana.SusaATcern.ch>

## Local run
If you want to run the gain calibration on a single FED, please use [`test/gain_calib_cfg.py`](test/gain_calib_cfg.py)
For example, to run a test job, try
```
cp /eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_323203/GainCalibration_1200_323203.dmp ./
cmsRun gain_calib_cfg.py run=323203 fed=1200
```

## VCal database object
To update the VCal database object, please see the [TWiki](https://twiki.cern.ch/twiki/bin/view/CMS/SiPixelGainCalibrationDoc#VCal_database_object).
Basically:
```
export SCRAM_ARCH=slc7_amd64_gcc820
mkdir pixels
cd pixels
cmsrel CMSSW_11_1_2
cd CMSSW_11_1_2/src
cmsenv
git cms-addpkg CondTools/SiPixel
scram b -j8
cd CondTools/SiPixel/test

# edit hardcoded VCal -> #electrons slope/offset SiPixelVCalDB_cfg.py
cmsRun SiPixelVCalDB_cfg.py
```

## Contact
If you have issues with running the gain calibration code or `run.py`, please contact
Lars Noehte <lars.noehteATpsi.ch> and
Izaak Neutelings <izaak.neutelingsATuzh.ch>.

