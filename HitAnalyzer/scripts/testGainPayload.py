import FWCore.ParameterSet.Config as cms

process = cms.Process("MyRawToClus")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
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
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
# 2016 cosmics 
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Express_v0', '')


import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias	
#    HLTPaths = ['HLT_Physics*'],
#    HLTPaths = ['HLT_Random*'],
    HLTPaths = ['HLT_ZeroBias*'],
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

process.source = cms.Source("PoolSource",
 fileNames =  cms.untracked.vstring(

#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/04BF9517-F54D-E611-B42B-02163E0144D3.root"

"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0014F268-DC4E-E611-AC39-02163E0141E7.root"

#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/0E6E4ACA-4F5A-E611-97B5-FA163E1E4ACD.root",

 )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277069:80-277069:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277087:196-277087:1192')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('278193:77-278193:9999')

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
#       threshold = cms.untracked.string('ERROR')
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.out = cms.OutputModule("PoolOutputModule",
    #fileName =  cms.untracked.string('file:clus.root'),
    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/clus/clus_exp_zb_278193.root'),
    #outputCommands = cms.untracked.vstring("drop *","keep *_*_*_MyRawToClus"), # 12.4MB per 100 events
    # save only pixel clusters 
    outputCommands = cms.untracked.vstring("drop *","keep *_siPixelClustersPreSplitting_*_*"), # 5.6MB per 100 events
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
useLocalDBError = False
if useLocalDBError :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
        tag = cms.string('SiPixelGainCalibration_2016_v1_offline')
        #tag = cms.string('SiPixelGainCalibration_2016_v2_offline')
        #tag = cms.string('SiPixelGainCalibration_2016_v1_hltvalidation')
 	),
       ),
     connect = cms.string('sqlite_file:../../../../../DB/Gains/SiPixelGainCalibration_2016_v1_offline.db')
#     connect = cms.string('sqlite_file:../../../../../DB/Gains/SiPixelGainCalibration_2016_v2_offline.db')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 

process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(False),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)


process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(False),
# sim in V7
#    src = cms.InputTag("mix"),
# MC after raw->digi
#    src = cms.InputTag("simSiPixelDigis"),
    src = cms.InputTag("siPixelDigis"),
)


#process.d = cms.EDAnalyzer("PixClusterAna",
process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    #Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    #Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('test.root')
)

#process.p = cms.Path(process.siPixelDigis)
#process.p = cms.Path(process.siPixelDigis*process.a)
#process.p = cms.Path(process.siPixelDigis*process.siPixelClusters)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.d*process.a)

# suppress output file or not
#process.ep = cms.EndPath(process.out)
