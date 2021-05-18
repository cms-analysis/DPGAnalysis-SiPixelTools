# dbs search --query='find file, file.numevents where run=172998 and site=caf.cern.ch and dataset=/MinimumBias/Run2011A-PromptReco-v6/RECO' > bla.txt

# 2012
#das_client.py --limit=10 --query='file dataset=/JetHT/Run2012D-22Jan2013-v1/RECO run=208686' # 
#das_client.py --limit=0 --query='file dataset=/MuOnia/Run2012D-22Jan2013-v1/AOD run=208686' # 

# Cosmics 
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Run2015A-v1/RAW run=250381' # Y
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Run2015A-PromptReco-v1/RECO run=248566' # NO
#das_client.py --limit=0 --query='file dataset=/Cosmics/Run2015A-PromptReco-v1/RECO run=248566' # NO

# Collisions 
# RAW
#das_client.py --limit=0 --query='file dataset=/ZeroBias1/Run2015A-v1/RAW run=247324' # YES

#RECO 
# A
#das_client.py --limit=0 --query='file dataset=/ExpressPhysics/Run2015A-Express-v1/FEVT run=246963' # YES
# most data sets are only on /tier0/store/data but DAS still lists them
#das_client.py --limit=0 --query='file dataset=/ZeroBias_0T/Run2015C-PromptReco-v2/RECO run=256004' # YES

# D
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015D-PromptReco-v3/RECO run=256729' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015D-PromptReco-v4/RECO run=260627' # YES


# Lumi
#das_client.py --limit=0 --query='file dataset=/AlCaLumiPixels/Run2015B-LumiPixels-PromptReco-v1/ALCARECO run=251643' # YES
#das_client.py --limit=0 --query='file dataset=/AlCaLumiPixels/Run2015C-LumiPixels-v1/RAW run=254319' # YES
#das_client.py --limit=0 --query='file dataset=/AlCaLumiPixels/Run2015D-LumiPixels-PromptReco-v4/ALCARECO run=260490' 

# 2015-A
# /eos/cms/store/data/Run2015A/MinimumBias/RECO/PromptReco-v1/000/246
#/eos/cms/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/246/908/00000

# 2015-B
#/eos/cms/store/data/Run2015B/ZeroBias/RECO/PromptReco-v1/000/251/643/00000
#/eos/cms/store/data/Run2015B/Commissioning/RECO/PromptReco-v1/000/251/643/00000

# 2016
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Commissioning2016-v1/RAW run=266277' # 
#das_client.py --limit=0 --query='file dataset=/Cosmics/Commissioning2016-v1/RAW run=266277' # 

#das_client.py --limit=0 --query='file dataset=/Cosmics/Commissioning2016-PromptReco-v1/RECO run=266277' # 
#das_client.py --limit=0 --query='file dataset=/Cosmics/Commissioning2016-PromptReco-v1/RECO run=266665' # 
#das_client.py --limit=0 --query='file dataset=/Cosmics/Commissioning2016-PromptReco-v1/RECO run=267594' # 

#das_client.py --limit=0 --query='file dataset=/ExpressPhysics/Run2016E-Express-v2/FEVT run=277069' # 
#das_client.py --limit=0 --query='file dataset=/ExpressPhysics/Run2016E-Express-v2/FEVT run=277148' # 

#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ExpressPhysics/Run2016F-Express-v1/FEVT run=278509' 
#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ExpressPhysics/Run2016G-Express-v1/FEVT run=278969' 
#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ExpressPhysics/Run2016G-Express-v1/FEVT run=278975' 
# 2017 
#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ExpressCosmics/Commissioning2017-Express-v1/FEVT run=294307' 
#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ExpressPhysics/Run2017A-Express-v1/FEVT run=294927' 
#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ZeroBias/Run2017C-PromptReco-v3/RECO run=300742' 
#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ExpressPhysics/Run2017E-Express-v1/FEVT run=303790' 
#/cvmfs/cms.cern.ch/common/das_client  --limit=0 --query='file dataset=/ExpressPhysics/Run2017F-Express-v1/FEVT run=305282' 

#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Run2017F-Express-v1/FEVT run=306432' 
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ZeroBias/Run2017F-*/RECO'  # works

# 2018
# MC
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/RelValQCD_Pt-20toInf_MuEnrichedPt15_13TeV/CMSSW_10_2_0_pre6-PU25ns_102X_upgrade2018_realistic_v10_HS-v1/GEN-SIM-RECO'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/TTbar_14TeV_TuneCP5_Pythia8/Run3Summer19DR-106X_mcRun3_2021_realistic_v3-v2/GEN-SIM-DIGI-RAW'
# gives
# /store/mc/Run3Summer19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/106X_mcRun3_2021_realistic_v3-v2/130000/F37978B0-7657-4845-B84A-918A4C91652E.root

#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/RelValTTbar_13/CMSSW_10_6_1_patch3-PU25ns_106X_upgrade2018_realistic_v6-v1/GEN-SIM-RECO'
#"/store/relval/CMSSW_10_6_1_patch3/RelValTTbar_13/GEN-SIM-RECO/PU25ns_106X_upgrade2018_realistic_v6-v1/20000/FF0A1ADF-8ECA-514A-A177-2723BCCABDE0.root",

#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Commissioning2018-Express-v1/FEVT run=312220' 
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Commissioning2018-Express-v1/FEVT run=313759'
 
# 2018 data 
# cosmics 
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Run2018B-Express-v1/FEVT run=317687'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Run2018B-Express-v1/FEVT run=318370'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Run2018D-Express-v1/FEVT run=324847'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Run2018D-Express-v1/FEVT run=325088'
#
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Commissioning2018-Express-v1/FEVT run=315188'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Run2018A-Express-v1/FEVT run=316505'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Run2018B-Express-v1/FEVT run=319104'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Run2018C-Express-v1/FEVT run=320500'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Run2018D-Express-v1/FEVT run=325097'
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Run2018E-Express-v1/FEVT run=325308'

#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/HIExpressPhysics/HIRun2018A-Express-v1/FEVT run=326384'


#dasgoclient -query='dataset dataset=/*/Run2018*/RAW site=T2_CH_CERN'  # all RAW at CERN
#dasgoclient -query='file dataset=/ZeroBias/Run2018E-v1/RAW site=T2_CH_CERN'  # all RAW at CERN
dasgoclient -query='file dataset=/HIMinimumBias0/HIRun2018A-v1/RAW run=327560'  # RAW HI

