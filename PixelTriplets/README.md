
# Pixel Triplet Code

Choose which input files to run over in pxl.py. If running on lxplus, you can for instance find GEN-SIM-RECO files in /eos/cms/store/relval/CMSSW_9_4_0/

```
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
"file:/eos/cms/store/relval/CMSSW_9_4_0/RelValSingleMuPt10/GEN-SIM-RECO/94X_mc2017_realistic_v10-v1/10000/1A0C459B-63CA-E711-A9A8-0CC47A4D76AA.root",
"file:/eos/cms/store/relval/CMSSW_9_4_0/RelValSingleMuPt10/GEN-SIM-RECO/94X_mc2017_realistic_v10-v1/10000/1A0C459B-63CA-E711-A9A8-0CC47A4D76AA.root",
"file:/eos/cms/store/relval/CMSSW_9_4_0/RelValSingleMuPt10/GEN-SIM-RECO/94X_mc2017_realistic_v10-v1/10000/2C1FD59F-63CA-E711-A75D-0CC47A4D7600.root",
"file:/eos/cms/store/relval/CMSSW_9_4_0/RelValSingleMuPt10/GEN-SIM-RECO/94X_mc2017_realistic_v10-v1/10000/DABB00E3-68CA-E711-BF46-0CC47A4D75F2.root",
"file:/eos/cms/store/relval/CMSSW_9_4_0/RelValSingleMuPt10/GEN-SIM-RECO/94X_mc2017_realistic_v10-v1/10000/DCA18BE8-68CA-E711-B3ED-0025905A48D8.root",
	)
)
```

Run using cmsRun

```
cmsRun pxl.py
```

Remember to set the appropriate Global Tag. This can be done from the command line

```
cmsRun pxl.py --GlobalTag auto:phase1_2017_realistic
```