from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'ReReco_PHruns_Phase0VCal_withFPix'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_Resolution_ReReco_Data_92X_Phase0VCal_cfg.py'
#config.JobType.pyCfgParams = []
#config.JobType.inputFiles = []
#config.JobType.disableAutomaticOutputCollection = True
#config.JobType.outputFiles = ['Resolution.root']

config.section_('Data')
config.Data.inputDataset = '/SingleMuon/Run2017B-v1/RAW'
config.Data.runRange = '297664,297671'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 2000
config.Data.outLFNDirBase = '/store/group/dpg_tracker_pixel/comm_pixel/Resolution/Phase1'
config.Data.outputDatasetTag = 'PH_Runs-CMSSW_9_2_5_patch2-92X_dataRun2_Prompt_v4-Phase0VCalCalib-RERECO'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
