# produce pixel cluster & rechits from digia
# works directly or through raw
# 
#
##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("ClusTest")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Services_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
# to use aut0:
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use autoCond[*]
#from Configuration.AlCa.autoCond_condDBv2 import autoCond

# 2012
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag.globaltag = autoCond['run1_data']
#process.GlobalTag.globaltag = 'GR_R_73_V0A'
# 2015
#process.GlobalTag.globaltag = autoCpnd['run2_data']


# clusterizer 
#  already include in LocalTracker
#process.load("RecoLocalTracker.SiPixelClusterizer/SiPixelClusterizer_cfi")
# from 74X
#process.load("RecoLocalTracker.SiPixelClusterizer/SiPixelClusterizerPreSplitting_cfi")
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
# get the files from DBS:
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/digis/digis1.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/digis/digis2_postls171.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/digis/digis2_mc71.root'

  '/store/data/Run2012D/MinimumBias/RAW/v1/000/208/307/020716B4-CC3A-E211-B309-5404A63886B6.root',
  '/store/data/Run2012D/MinimumBias/RAW/v1/000/208/307/0492EC15-9E3A-E211-AF09-BCAEC5364C4C.root',
  '/store/data/Run2012D/MinimumBias/RAW/v1/000/208/307/04FD5338-BD3A-E211-B11A-003048F118C4.root',
  '/store/data/Run2012D/MinimumBias/RAW/v1/000/208/307/0642B1C0-C73A-E211-9F27-0030486780A8.root',
  '/store/data/Run2012D/MinimumBias/RAW/v1/000/208/307/0E4C8627-AC3A-E211-BB71-003048D2BCA2.root',

#  '/store/relval/CMSSW_7_1_0_pre8/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/PU_PRE_STA71_V4-v1/00000/06397C95-91E2-E311-963D-02163E00B776.root',



# 25ns flat pu=20-50
#   /store/mc/Spring14dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/Flat20to50_POSTLS170_V5-v1/00000/006EBBE4-98DC-E311-B64F-0025905A611E.root

# 50ns poisson pu=50
#   '/store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx50_POSTLS162_V2-v1/00000/00E707E5-0D75-E311-B109-003048678BAE.root',

# 25ns poisson pu=50
#   /store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU40bx25_POSTLS162_V2-v1/00000/00309507-AB75-E311-AB10-0025905A60B2.root

# 25ns poisson pu=20
#   /store/mc/Fall13dr/TT_Tune4C_13TeV-pythia8-tauola/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/001E7210-126D-E311-8D68-003048679182.root


  )
)


# DB stuff 
useLocalDB = False
if useLocalDB :
# Frontier LA 
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
# GenError
          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
#          tag = cms.string('SiPixelGenErrorDBObject38T_v0_mc1')
          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
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
     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
    ) # end process

# SQ_LITE GenError
    process.DBReaderFrontier2 = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 		cms.PSet(
 			record = cms.string("SiPixelGenErrorDBObjectRcd"),
# 			label = cms.untracked.string("fromAlignment"),
# 			tag = cms.string("SiPixelGenErrorDBObject38Tv1")
 			tag = cms.string("SiPixelGenErrorDBObject38TV10")
 		),
 	),
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db_old')
     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
   ) # end process
# endif
 
#process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
#process.myprefer2 = cms.ESPrefer("PoolDBESSource","DBReaderFrontier2")


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
#
# To use a test DB instead of the official pixel object DB tag: 
#process.customDead = cms.ESSource("PoolDBESSource", process.CondDBSetup, connect = cms.string('sqlite_file:/afs/cern.ch/user/v/vesna/Digitizer/dead_20100901.db'), toGet = cms.VPSet(cms.PSet(record = cms.string('SiPixelQualityRcd'), tag = cms.string('dead_20100901'))))
#process.es_prefer_customDead = cms.ESPrefer("PoolDBESSource","customDead")


process.o1 = cms.OutputModule("PoolOutputModule",
            outputCommands = cms.untracked.vstring('drop *','keep *_*_*_ClusTest'),
#            fileName = cms.untracked.string('file:clus.root')
#            fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_mc71.root')
            fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/740_p9/clus/rechits_2012d_208307.root')
)


# RAW
# clusterize through raw (OK)
# for Raw2digi for simulations 
#process.siPixelRawData.InputLabel = 'mix'
#process.siPixelDigis.InputLabel = 'siPixelRawData'
process.siPixelDigis.InputLabel = 'rawDataCollector'
#process.siPixelDigis.InputLabel = 'source'

# DIRECT
# direct clusterization (no raw step)
# label of digis 
#process.siPixelClusters.src = 'simSiPixelDigis'
# digis from raw (this is the default)
#process.siPixelClustersPreSplitting.src = 'siPixelDigis'
# modify clusterie parameters
#process.siPixelClustersPreSplitting.ClusterThreshold = 4000.0

# make digis only 
#process.p1 = cms.Path(process.siPixelDigis)
# plus pixel clusters  
#process.p1 = cms.Path(process.siPixelDigis*process.siPixelClustersPreSplitting)
# plus rechits
process.p1 = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)

process.outpath = cms.EndPath(process.o1)
