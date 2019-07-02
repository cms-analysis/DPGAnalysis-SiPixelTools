from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'MC'
config.General.workArea        = 'crab_mc'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName  = 'Analysis'
#config.JobType.pluginName  = 'PrivateMC'
#config.JobType.psetName    = 'SimToRec_crab.py'
config.JobType.psetName    = 'Sims_To_ValidRecHits_crab.py'
#config.JobType.psetName    = 'RawToRec_crab.py'
#config.JobType.psetName    = 'Raw_To_RecHitsValid_crab.py'
config.JobType.maxMemoryMB = 3000
#config.JobType.outputFiles = ['pixelrechits.root']
config.JobType.outputFiles = ['simtorec.root','pixelrechits.root']

# for MC
#config.Data.inputDataset  = '/RelValTTbar_13/CMSSW_10_4_0-PU25ns_103X_upgrade2018_design_v4-v1/GEN-SIM-DIGI-RAW'
#config.Data.inputDataset  = '/RelValTTbar_13/CMSSW_10_4_0-PU25ns_103X_upgrade2018_realistic_v8-v1/GEN-SIM-DIGI-RAW'
#config.Data.inputDataset  = '/RelValTTbar_13/CMSSW_10_4_0-PU25ns_103X_mc2017_realistic_v2-v1/GEN-SIM-DIGI-RAW'

#config.Data.inputDataset  = '/RelValMinBias_13/CMSSW_10_4_0-103X_upgrade2018_design_v4-v1/GEN-SIM'
config.Data.inputDataset  = '/RelValMinBias_13/CMSSW_10_4_0-103X_upgrade2018_realistic_v8-v1/GEN-SIM'

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_ZeroTesla_25ns_JSON_MuonPhys.txt'
config.Data.inputDBS      = 'global'
#config.Data.splitting     = 'LumiBased'
#config.Data.splitting     = 'EventAwareLumiBased'
config.Data.splitting     = 'FileBased'
#config.Data.splitting     = 'EventBased'
#config.Data.splitting     = 'Automatic'
config.Data.unitsPerJob   = 1
config.Data.totalUnits    = 1
#config.Data.runRange      = '326384-326384'
config.Data.outLFNDirBase = '/store/user/dkotlins/'
config.Data.publication   = False

config.Site.storageSite = 'T2_CH_CSCS'
