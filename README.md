
# DPGAnalysis-SiPixelTools

Prepare your working directory with whatever CMSSW version you want to use (branch "master" is currently on 94X)

```
mkdir PixelAnalysis
cd PixelAnalysis
cmsrel CMSSW_9_4_0
cd CMSSW_9_4_0/src
cmsenv
```

Fork DPGAnalysis-SiPixelTools from https://github.com/cms-analysis/DPGAnalysis-SiPixelTools and checkout the code

```
export GITUSER=`git config user.github`
git clone https://github.com/${GITUSER}/DPGAnalysis-SiPixelTools PixelTools
cd PixelTools
git remote add cms-analysis https://github.com/cms-analysis/DPGAnalysis-SiPixelTools.git
git fetch cms-analysis
git checkout -b 9_4_0 cms-analysis/9_4_0
cd **myPackage**(see below)
scram b -j 20
```

**myPackage** can be any of the repositories below, containing code to calibrate & test pixels:

- PixelTriplets - measure hit resolution in bpix using the triplet method.

- PixelTrees - make pixel trees used for testing and calibration of resonstructin

- PixelHitAsociator - a customised version of the SimHit-RecHit comparion, used for resolution testing.

- HitAnalyser - various codes to test pixel simHits, digis, clusters and recHits.

- LA-Calibration - code to calibrate the LA from data.

- GainCalibration - code to run the gain calibartion to obtain pedestals and (offsets) gains (slopes) per pixel.

- PixelDBTools - various test programs to monitor the content of pixel DB payloads.
