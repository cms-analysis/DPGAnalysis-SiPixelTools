# Pixel gain calibration

Prepare your working directory with CMSSW

```
mkdir Pixels
cd Pixels
cmsrel CMSSW_9_2_10
cd CMSSW_9_2_10/src
cmsenv
git clone https://github.com/cms-analysis/DPGAnalysis-SiPixelTools
rm -r DPGAnalysis-SiPixelTools/PixelDBTools/ # packages which do not compile...
rm -r DPGAnalysis-SiPixelTools/PixelTriplets/ # packages which do not compile...
scram b -j 20
cd DPGAnalysis-SiPixelTools/GainCalibration/test
```

To run

```
./Run.sh -create 1591 /store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_1591  /store/group/dpg_tracker_pixel/comm_pixel/thaarres/
```
Creates a folder with a config file storing information about input/output folders and creates the job scripts for the submission (from the template)


```
./Run.sh -submit 1591 #submit
./Run.sh -resubmit 1591 #resubmit if fails (all jobs MUST finish!)
```