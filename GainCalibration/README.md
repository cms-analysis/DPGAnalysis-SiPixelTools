# Pixel gain calibration

Prepare your working directory with CMSSW

```
mkdir Pixels
cd Pixels
cmsrel CMSSW_10_0_3
cd CMSSW_10_0_3/src
cmsenv
git clone https://github.com/cms-analysis/DPGAnalysis-SiPixelTools
rm -r DPGAnalysis-SiPixelTools/PixelDBTools/ # packages which do not compile...
rm -r DPGAnalysis-SiPixelTools/PixelTriplets/ # packages which do not compile...
scram b -j 20
cd DPGAnalysis-SiPixelTools/GainCalibration/test
```

To run, the syntax is ./Run.sh -create 1591 inputfolder outputfolder, e.g

```
./Run.sh -create 1591 /store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_1591  /store/group/dpg_tracker_pixel/comm_pixel/thaarres/
```
To run from and store to default Phase1/Run_* directory, simply do
```
./Run.sh -create 1591
```

This creates a folder with a config file storing information about input/output folders and creates the job scripts for the submission (from the template).
If you want to run over BPIX only, change the module input array here in L622 or Run.sh

```
./Run.sh -submit 1591 #submit
./Run.sh -resubmit 1591 #resubmit if fails (all jobs MUST finish!)
```

You can check the status of your jobs with
```
bjobs | grep cmscaf1nw
```