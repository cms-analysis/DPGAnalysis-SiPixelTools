import FWCore.ParameterSet.Config as cms

process = cms.Process("PixelGainDB")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.autoCond_condDBv2 import autoCond
process.GlobalTag.globaltag = autoCond['run2_design']

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.TFileService = cms.Service("TFileService", fileName = cms.string('/tmp/rougny/histos.root') )

process.gainDBOffline = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
    inputrootfile = cms.untracked.string('file:///tmp/rougny/test.root'),
    record = cms.untracked.string('SiPixelGainCalibrationOfflineRcd'),
    useMeanWhenEmpty = cms.untracked.bool(True),
    badChi2Prob = cms.untracked.double(0.00001)                                       
    )

process.gainDBHLT = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
    inputrootfile = cms.untracked.string('file:///tmp/rougny/test.root'),
    record = cms.untracked.string('SiPixelGainCalibrationForHLTRcd'),
    useMeanWhenEmpty = cms.untracked.bool(True),  
    badChi2Prob = cms.untracked.double(0.00001)                             
    )

process.source = cms.Source("EmptyIOVSource",                            
    #lastRun = cms.untracked.uint32(1),
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
    )

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(10),
        authenticationPath = cms.untracked.string('.')
        ),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelGainCalibrationOfflineRcd'),
            tag = cms.string('GainCalib_TEST_offline')
            )
        ),
    connect = cms.string('sqlite_file:prova.db')
)

process.p = cms.Path(process.gainDBOffline)

