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
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015C-v1/RAW run=254319' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias_0T/Run2015C-v1/RAW run=256004' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015D-v1/RAW run=256676' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015D-v1/RAW run=258655' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015D-v1/RAW run=260627' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015D-v1/RAW run=260627' # YES


#RECO 
# A
#das_client.py --limit=0 --query='file dataset=/ExpressPhysics/Run2015A-Express-v1/FEVT run=246963' # YES
#das_client.py --limit=0 --query='file dataset=/MinimumBias/Run2015A-PromptReco-v1/RECO run=247607' # NO
#das_client.py --limit=0 --query='file dataset=/Commissioning/Run2015A-PromptReco-v1/RECO run=248025' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias1/Run2015A-PromptReco-v1/RECO run=248025' # YES
# B
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015B-PromptReco-v1/RECO run=251643' # YES
#das_client.py --limit=0 --query='file dataset=/Commissioning/Run2015B-PromptReco-v1/RECO run=251643' # YES
# C
#das_client.py --limit=0 --query='file dataset=/Commissioning/Run2015C-PromptReco-v1/RECO run=254227' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015C-PromptReco-v1/RECO run=254227' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015C-PromptReco-v1/RECO run=254790' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015C-PromptReco-v1/RECO run=254608' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015C-PromptReco-v1/RECO run=254833' # YES
#das_client.py --limit=0 --query='file dataset=/ZeroBias/Run2015C-PromptReco-v1/RECO run=254905' # YES
#das_client.py --limit=0 --query='file dataset=/HLTPhysics/Run2015C-PromptReco-v1/RECO run=254227' # YES
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
#/eos/cms/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/908/00000
#/eos/cms/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/963
#/eos/cms/store/data/Run2015A/MinimumBias/RAW/v1/000/246
#/eos/cms/store/data/Run2015A/AlCaLumiPixels/RAW/v1/000/246
#/eos/cms/store/data/Run2015A/Commissioning/RAW/v1/000/246/  <---
#/eos/cms/store/data/Run2015A/ZeroBias1/RAW/v1/000/246/ <---


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
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Commissioning2018-Express-v1/FEVT run=312220' 
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressCosmics/Commissioning2018-Express-v1/FEVT run=313759'
 
# 2018 data 
#/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Commissioning2018-Express-v1/FEVT run=315188'
/cvmfs/cms.cern.ch/common/dasgoclient  --limit=0 --query='file dataset=/ExpressPhysics/Run2018A-Express-v1/FEVT run=316766'


