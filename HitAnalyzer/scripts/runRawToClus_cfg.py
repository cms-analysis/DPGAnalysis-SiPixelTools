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
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # fpr 90W 
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # from CMSSW927
#process.GlobalTag.globaltag = '92X_dataRun2_Prompt_v11' # Promot
#process.GlobalTag.globaltag = '94X_dataRun2_ReReco_EOY17_v2' # for RERECO
# 2018
process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' # 
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias	
#    HLTPaths = ['HLT_Physics*'],
    HLTPaths = ['HLT_Random*'],
#    HLTPaths = ['HLT_ZeroBias*'],
#    HLTPaths = ['HLT_ZeroBias_v*'],  # simple democratic ZB
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

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
#)
fileNames =  cms.untracked.vstring(

"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/6DC681B8-12C2-534B-922C-568CAFEFBA23.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/44ED9E8D-F71B-4540-9DA9-A476E705CADE.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/ED612143-02C5-C742-A268-D003C747F9C2.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/350D164E-3069-F04E-9F19-C0A454843DCB.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/AAB64A7D-0F39-FC45-BA97-45D28FC8E3CC.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/302034FE-E606-904A-A8E0-4CCA4A251C8A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/3ACA86EB-0D8D-4244-A5A2-39884AB523A9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E14E1734-7B44-0249-A375-E2439155D574.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/F086480A-5AB6-A14C-908A-91CD5C255C24.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/2027DE44-C425-F24B-A84B-3F0F95C270FC.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E059BE35-1B40-9A40-8CC0-EF9F64B0216C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/A5AA5CEA-D219-3B49-997E-E0E8409E123E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/CCD4B54B-7D9D-854E-9AF0-9609F3919316.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/A852B0F3-5435-9341-9023-B0FEE829EDF2.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/617D0DC5-580F-8141-94B5-C5CA46A8C7E9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/2BAED97D-1E7E-A847-A927-639A481D0F3A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/B6B7D160-B11A-F948-8D58-74F9D0064E4F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/FDD637D3-29CA-BD4A-A2B0-9C76145D89F4.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/79CB1294-16A9-BA4A-A4BC-B53DD52A2C4F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/0282F21A-F0AF-F04E-B9E1-B8D07E81269C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/90C23117-F8C0-7041-98E2-2830101292E9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/EC806BEB-271F-B84A-9AE0-D940E9B9DB8E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/7BDAE626-99D5-6449-8155-DF4B9F4B61DF.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E65A59C7-C58B-7E4B-8EEA-30F39C4E1B8E.root",

 )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:49-314890:1433')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:50-314890:60')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:500-314890:510')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:1300-314890:1310')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315357:44-315357:872')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315488:42-315488:1126')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('317434:48-317434:271')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('317475:33-317475:89')


# for Raw2digi for data
process.siPixelDigis.InputLabel = 'rawDataCollector'
#process.siStripDigis.ProductLabel = 'rawDataCollector'

# for digi to clu
#process.siPixelClusters.src = 'siPixelDigis'

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
#        tag = cms.string('SiPixelGainCalibration_2018_v3') 
#        tag = cms.string('SiPixelGainCalibration_2018_v2_offline') 
        tag = cms.string('SiPixelGainCalibration_2018_v1_offline') 
#        tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
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
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v2_offline.db')
     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v3_offline.db')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 

#process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siPixelDigis.IncludeErrors = True
process.siPixelDigis.Timing = False 
#process.siPixelDigis.UsePilotBlade = True 
process.siPixelDigis.UsePhase1 = True

# digis from raw (this is the default)
#process.siPixelClustersPreSplitting.src = 'siPixelDigis'
# modify clusterie parameters
#process.siPixelClustersPreSplitting.VCaltoElectronGain = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = -60
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = -60
#process.siPixelClustersPreSplitting.SeedThreshold = 10 #  def=1000
#process.siPixelClustersPreSplitting.ChannelThreshold = 10 #must be bigger than 1
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
    Tracks = cms.InputTag("generalTracks"),
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
process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# for random cosmics 
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d_cosm)

#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# suppress output file or not
#process.ep = cms.EndPath(process.out)
