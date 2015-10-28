import FWCore.ParameterSet.Config as cms
process = cms.Process("Test")

process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2017_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
# for phase1 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_upgrade2017_design_v4', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
    #lastRun = cms.untracked.uint32(85000),
    #timetype = cms.string('runnumber'),
    #interval = cms.uint32(1)
    firstRun = cms.untracked.uint32(208000),
)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root")
                                   )


process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    destinations = cms.untracked.vstring('cout')
)

# DB stuff 
useLocalDB = True
if useLocalDB :
  process.QualityReader = cms.ESSource("PoolDBESSource",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('')
    ),
    # timetype = cms.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
#        tag = cms.string('SiPixelQuality_test')
        tag = cms.string('SiPixelQuality_phase1_ideal')
#        tag = cms.string('SiPixelQuality_v03')
    )),
#    connect = cms.string('sqlite_file:SiPixelQuality_test.db')
    connect = cms.string('sqlite_file:SiPixelQuality_phase1_ideal.db')
  )
  process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")
#  end if

process.BadModuleReader = cms.EDAnalyzer("SiPixelBadModuleDBReader",
    printDebug = cms.untracked.uint32(1),
    RcdName = cms.untracked.string("SiPixelQualityFromDbRcd") 
)

process.p = cms.Path(process.BadModuleReader)



