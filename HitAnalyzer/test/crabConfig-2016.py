from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = '2016Cosm'
config.General.workArea        = 'crab_data'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'prompt-Run2016.py'

# for RECO
config.Data.inputDataset  = '/Cosmics/Commissioning2016-PromptReco-v1/RECO'
# for RAW
#config.Data.inputDataset  = '/MinimumBias/Commissioning2016-v1/RAW'
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_ZeroTesla_25ns_JSON_MuonPhys.txt'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
#config.Data.splitting     = 'FileBased'
config.Data.unitsPerJob   = 2000
#config.Data.totalUnits    = 10
config.Data.runRange      = '267594-267594'
#config.Data.runRange      = '267767-267767'
config.Data.outLFNDirBase = '/store/user/dkotlins/'
config.Data.publication   = False
config.Site.storageSite = 'T2_CH_CSCS'
