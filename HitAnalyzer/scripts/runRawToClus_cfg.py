import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("MyRawToClus",eras.Run2_2017)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
#process.load('CondCore.CondDB.CondDB_cfi')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.load('Configuration.StandardSequences.RawToDigi_cff')

# needed for pixel RecHits (TkPixelCPERecord)
process.load("Configuration.StandardSequences.Reconstruction_cff")

# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2017
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # from CMSSW927
#process.GlobalTag.globaltag = '92X_dataRun2_Prompt_v11' # Promot
#process.GlobalTag.globaltag = '94X_dataRun2_ReReco_EOY17_v2' # for RERECO
# 2018 express
#process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' # 
# 2018 re-reco 
#process.GlobalTag.globaltag = '111X_dataRun2_v3' # 
#process.GlobalTag.globaltag = '106X_dataRun2_v28' # 

# AUTO conditions 
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias	
#    HLTPaths = ['HLT_Physics*'],
#    HLTPaths = ['HLT_Random*'],
#    HLTPaths = ['HLT_ZeroBias*'],
    HLTPaths = ['HLT_ZeroBias_v*'],  # simple democratic ZB
#    HLTPaths = ['HLT_ZeroBias_part*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionInTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_LastCollisionInTrain_*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstBXAfterTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_IsolatedBunches_*'], # ok
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionAfterAbortGap_*'], # ok
#    HLTPaths = ['HLT_ZeroBias_BeamSpot_*'],
#
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
#    HLTPaths = ['HLT_PAZeroBias*'],
#    HLTPaths = ['HLT_PARandom*'],
#    HLTPaths = ['HLT_PAMinBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1Tech5_BPTX_PlusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech6_BPTX_MinusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech7_NoBPTX_v*'],
#
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

#myfilelist = cms.untracked.vstring()
#myfilelist.extend([
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/88FA64E4-D1A1-3748-B8C8-7D2C33053969.root",
#])

process.source = cms.Source("PoolSource",
    # fileNames =  myfilelist)
    fileNames =  cms.untracked.vstring(

#"file:/eos/cms/store/data/Run2018E/ZeroBias/RAW/v1/000/325/699/00000/FAAE70B9-D4C7-364D-8D80-C34BBB8D1AAC.root",
#"file:/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/tvami/forDanek/321178/FC2C31E9-C99E-E811-9C51-FA163EAC0A56.root",  # from Tamas OK 

"file:/work/kotlinski/DATA/RAW/325170/06590754-A60F-E24C-AFDE-4B01585C2B01.root", # copy from AAA
#"file:/work/kotlinski/DATA/RAW/325308/FDDD4BEE-4B1B-7E42-BD0E-E7FDF26FB466.root", # copy from eos

# HI cannot be accessed from T3
#"/store/hidata/HIRun2018A/HIMinimumBias0/RAW/v1/000/327/560/00000/D8ACD174-92CC-FC4A-A38E-8588FB558023.root",
#"/store/hidata/HIRun2018A/HIMinimumBias0/RAW/v1/000/327/560/00000/90CF1CEE-99C3-AE4F-9489-38CCB99609B2.root",

# cosmics 
#"/store/hidata/HIRun2018A/MinimumBias/RAW/v1/000/326/945/00000/7942C3E7-2287-FD41-93C4-56C05F187CA2.root",

#"/store/data/Run2018D/HIMinimumBias0/RAW/v1/000/325/112/00000/660F62BB-9932-D645-A4A4-0BBBDA3963E8.root",

#"/store/data/Run2018D/ZeroBias/RAW/v1/000/325/170/00000/06590754-A60F-E24C-AFDE-4B01585C2B01.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2EB3AEDA-747D-FB46-9583-BC1F1241CFE7.root",
 )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:1-325097:100') # overview


process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
       threshold = cms.untracked.string('ERROR')
#        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:clus.root'),
    #fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/clus/clus_zb_248025.root'),
    outputCommands = cms.untracked.vstring("drop *","keep *_*_*_MyRawToClus"), # 12.4MB per 100 events
    # save only pixel clusters 
    #outputCommands = cms.untracked.vstring("drop *","keep *_siPixelClustersPreSplitting_*_*"), # 5.6MB per 100 events
    #outputCommands = process.RECOEventContent.outputCommands,  # 4.9MB per 10 events 
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    splitLevel = cms.untracked.int32(0),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

# pixel local reco (RecHits) needs the GenError object,
# not yet in GT, add here:
# DB stuff 
useLocalDB = False
if useLocalDB :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
#        record = cms.string('SiPixelGainCalibrationRcd'),
#       record = cms.string('SiPixelGainCalibrationForHLTRcd'),
#        tag = cms.string('SiPixelGainCalibration_2018_v3') 
#        tag = cms.string('SiPixelGainCalibration_2018_v2_offline') 
#        tag = cms.string('SiPixelGainCalibration_2018_v1_offline') 
#        tag = cms.string('SiPixelGainCalibration_2018_v7') 
#        tag = cms.string('SiPixelGainCalibration_2018_v8') 
#        tag = cms.string('SiPixelGainCalibration_2018_v9') 
#        tag = cms.string('SiPixelGainCalibration_2018_v9_offline') 
#        tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
        tag = cms.string('SiPixelGainCalibration_2009runs_ScaledForVCal_hlt')
#        tag = cms.string('SiPixelGainCalibration_2009runs_prompt_v1_IOV_326851_scaled')
 	),
       ),
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_full.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v5_full.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_full.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_1337_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_offline_oldvcalHL.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v5_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v3_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v2_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v2_bugfix_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_phase1_ideal_v2.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v1_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v2_offline. db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v3_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v9_offline.db')
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 

# for Raw2digi for data
process.siPixelDigis.InputLabel = 'rawDataCollector'  # normal p-p
#process.siPixelDigis.InputLabel = 'rawDataRepacker'  # HI
#process.siStripDigis.ProductLabel = 'rawDataCollector'
#process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.IncludeErrors = True
process.siPixelDigis.Timing = False 
#process.siPixelDigis.UsePilotBlade = True 
process.siPixelDigis.UsePhase1 = True

# for digi to clu
#process.siPixelClusters.src = 'siPixelDigis'
# digis from raw (this is the default)
#process.siPixelClustersPreSplitting.src = 'siPixelDigis'
# modify clusterie parameters
#process.siPixelClustersPreSplitting.VCaltoElectronGain = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = -60
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = -60
#process.siPixelClustersPreSplitting.VCaltoElectronGain = 1  # for Run3 
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = 0
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 1  # 
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = 0
#process.siPixelClustersPreSplitting.SeedThreshold = 10 #  def=1000
#process.siPixelClustersPreSplitting.ChannelThreshold = 10 # must be bigger than 1, def=10
#process.siPixelClustersPreSplitting.ClusterThreshold = 10 # def =4000    # integer?
#process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 10 # def=2000 # integer?

# To select full granularity gain calibration 
# process.siPixelClustersPreSplitting.payloadType = cms.string('Full')


process.a = cms.EDAnalyzer("PixDigisTest",
                           Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)


process.d = cms.EDAnalyzer("PixClusterAna",
#process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Tracks = cms.InputTag(""), # ni tracks, only local reco
    #Tracks = cms.InputTag("generalTracks"),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)
process.d_cosm = cms.EDAnalyzer("PixClusterAna",
#process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Tracks = cms.InputTag(""),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis_clus.root')
)

#process.p = cms.Path(process.siPixelDigis)
#process.p = cms.Path(process.siPixelDigis*process.a)
#process.p = cms.Path(process.siPixelDigis*process.siPixelClusters)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# for random cosmics 
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d_cosm)

# for HI
process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# suppress output file or not
#process.ep = cms.EndPath(process.out)
