# produce pixel cluster & rechits from digia
# works directly or through raw
# 
#
##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("ClusTest")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
# 2017, cannot use this rag for dump files, picks up phase0 geom?
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # 

# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

# for raw
#process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')


# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

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

# read from dmp files 
#process.source = cms.Source("PixelSLinkDataInputSource",
process.source = cms.Source("PixelDumpDataInputSource",
    fedid = cms.untracked.int32(-1),
    runNumber = cms.untracked.int32(-1),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/d/dkotlins/WORK/dmp/PixelAlive_1200_1298.dmp')
#    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/d/dkotlins/WORK/dmp/PixelAlive_1200_1294.dmp')
#    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/d/dkotlins/WORK/dmp/PixelAlive_1200_1295.dmp')
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/d/dkotlins/WORK/dmp/PixelAlive_1200_1293.dmp')
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/d/dkotlins/WORK/dmp/PixelAlive_1200_1018.dmp')
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/d/dkotlins/WORK/dmp/SCurve_1200_1242.dmp')
#    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/d/dkotlins/WORK/gain/GainCalibration_1200_1092.dmp')
    )


## get raw root file 
#process.source = cms.Source("PoolSource",
#  fileNames = cms.untracked.vstring(
##    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/digis/digis1.root'
##  '/store/relval/CMSSW_7_1_0_pre8/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/PU_PRE_STA71_V4-v1/00000/06397C95-91E2-E311-963D-02163E00B776.root',
#  )
#)

# DB stuff 
useLocalDB = True
if useLocalDB :
# Frontier 
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
# Gain calib
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
        tag = cms.string('SiPixelGainCalibration_2017_v2_offline')
        #tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
# GenError
#          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
#          tag = cms.string('SiPixelGenErrorDBObject38T_v0_mc1')
#          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
# LA
# 			record = cms.string("SiPixelLorentzAngleRcd"),
# 			label = cms.untracked.string("fromAlignment"),
# 			label = cms.untracked.string("forWidth"),
# 			tag = cms.string("SiPixelLorentzAngle_v02_mc")
# 			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v0_mc")
# 			tag = cms.string("SiPixelLorentzAngle_forWidth_v0_mc")
 		),
# 		cms.PSet(
# 			record = cms.string("SiPixelLorentzAngleSimRcd"),
# 			tag = cms.string("test_LorentzAngle_Sim")
# 		)
 	),
#     connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
#     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
     connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_2017_v2_offline.db')
    ) # end process
# endif
 
process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")


#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
#    DBParameters = cms.PSet(
#        messageLevel = cms.untracked.int32(0),
#        authenticationPath = cms.untracked.string('')
#    ),
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('SiPixelQualityRcd'),
#        tag = cms.string('SiPixelBadModule_test')
#    )),
#    connect = cms.string('sqlite_file:test.db')
#)
# To use a test DB instead of the official pixel object DB tag: 
#process.customDead = cms.ESSource("PoolDBESSource", process.CondDBSetup, connect = cms.string('sqlite_file:/afs/cern.ch/user/v/vesna/Digitizer/dead_20100901.db'), toGet = cms.VPSet(cms.PSet(record = cms.string('SiPixelQualityRcd'), tag = cms.string('dead_20100901'))))
#process.es_prefer_customDead = cms.ESPrefer("PoolDBESSource","customDead")


process.o1 = cms.OutputModule("PoolOutputModule",
            outputCommands = cms.untracked.vstring('drop *','keep *_*_*_ClusTest'),
            fileName = cms.untracked.string('file:clus.root')
#            fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_mc71.root')

)

process.analysisRH = cms.EDAnalyzer("PixRecHitTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("siPixelRecHitsPreSplitting"),
)

process.analysisClu = cms.EDAnalyzer("PixClusterAna",
#process.analysisClu = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    #Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    #Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

process.analysisCluTest = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    #Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    #Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

process.analysisDig = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
)

# My 
# modify clusterie parameters
#process.siPixelClustersPreSplitting.ClusterThreshold = 4000.0
#process.siPixelClustersPreSplitting.SeedThreshold = 1000
#process.siPixelClustersPreSplitting.ChannelThreshold = 2 #must be bigger than 1
#process.siPixelClustersPreSplitting.ClusterThreshold = 1.0
# this is to see the charge in units of vcalHigh
process.siPixelClustersPreSplitting.VCaltoElectronGain = 125 #  47
process.siPixelClustersPreSplitting.VCaltoElectronOffset = 0 # -60


# RAW
# clusterize through raw (OK)
# for Raw2digi for simulations 
#process.siPixelDigis.InputLabel = 'siPixelRawData'
#process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.UsePhase1 = cms.bool(True)

# probably not needed (is default)
#process.siPixelClusters.src = 'siPixelDigis'


#process.p1 = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)
process.p1 = cms.Path(process.siPixelDigis*process.analysisDig*process.pixeltrackerlocalreco*process.analysisCluTest)
#process.p1 = cms.Path(process.siPixelDigis*process.analysisDig*process.pixeltrackerlocalreco*process.analysisClu*process.analysisRH)

#process.outpath = cms.EndPath(process.o1)
