from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'Run298653_Tier0_REPLAY_withFPix'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_Resolution_RECO_Data_92X_cfg.py'
#config.JobType.pyCfgParams = []
#config.JobType.inputFiles = []
#config.JobType.disableAutomaticOutputCollection = True
#config.JobType.outputFiles = ['Resolution.root']

config.section_('Data')
config.Data.inputDataset = '/ZeroBias/Tier0_REPLAY_vocms015-PromptReco-v141/RECO'
config.Data.runRange = '298653'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 10000
config.Data.outLFNDirBase = '/store/group/dpg_tracker_pixel/comm_pixel/Resolution/Phase1'
config.Data.outputDatasetTag = 'Run298653-CMSSW_9_2_6-92X_dataRun2_Prompt_forTier0Replay_PixelLocalReco_TkAl_newVCal_v2-Phase1VCalCalib-Tier0_REPLAY'
config.Data.ignoreLocality = True

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
