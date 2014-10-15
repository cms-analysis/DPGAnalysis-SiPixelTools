import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")
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

#process.Timing = cms.Service("Timing")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_R_70_V1::All"

process.load("Configuration.StandardSequences.GeometryIdeal_cff")

# DB stuff 
useLocalDB = False
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
        tag = cms.string('SiPixelQuality_v03')
    )),
    connect = cms.string('sqlite_file:Quality_v03.db')
  )
  process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")
#  end if

process.BadModuleReader = cms.EDAnalyzer("SiPixelBadModuleDBReader",
    printDebug = cms.untracked.uint32(1),
    RcdName = cms.untracked.string("SiPixelQualityFromDbRcd") 
)

process.p = cms.Path(process.BadModuleReader)



