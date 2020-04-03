import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("PixelGainDB",eras.Run2_2017)

phase=1
if phase==0:
    process.load("Configuration.StandardSequences.GeometryDB_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
    from Configuration.AlCa.autoCond_condDBv2 import autoCond
    process.GlobalTag.globaltag = autoCond['run2_design']
elif phase==1:
    process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    from Configuration.AlCa.GlobalTag import GlobalTag
    #process.GlobalTag.globaltag = '100X_dataRun2_Express_v2'#'92X_dataRun2_Express_v7'
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
    #process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.TFileService = cms.Service("TFileService", fileName = cms.string('gains.root') )

process.gainDBOffline = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
    inputrootfile = cms.untracked.string('file:GainCalibration_v2.root'),
    record = cms.untracked.string('SiPixelGainCalibrationOfflineRcd'),
    useMeanWhenEmpty = cms.untracked.bool(False), # True-fills ALL modules
    appendMode = cms.untracked.bool(False), # adds to the old *.db file
    badChi2Prob = cms.untracked.double(0.000000001), # not used
    pedlow = cms.untracked.double(-100.0),
    pedhigh = cms.untracked.double(250.0),
    pedmax = cms.untracked.double(250.0)                                     
    )

process.gainDBOfflineFull = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
    inputrootfile = cms.untracked.string('file:GainCalibration_v2.root'),
    record = cms.untracked.string('SiPixelGainCalibrationRcd'),
    useMeanWhenEmpty = cms.untracked.bool(True),
    appendMode = cms.untracked.bool(False), # adds to the old *.db file
    badChi2Prob = cms.untracked.double(0.00001), # not used
    pedlow = cms.untracked.double(-100.0),
    pedhigh = cms.untracked.double(250.0),
    pedmax = cms.untracked.double(250.0)                                       
    )

process.gainDBHLT = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
    inputrootfile = cms.untracked.string('file:GainCalibration_v2.root'),
    record = cms.untracked.string('SiPixelGainCalibrationForHLTRcd'),
    useMeanWhenEmpty = cms.untracked.bool(True),  
    appendMode = cms.untracked.bool(False), # adds to the old *.db file
    badChi2Prob = cms.untracked.double(0.00001), # not used
    pedlow = cms.untracked.double(-100.0),
    pedhigh = cms.untracked.double(250.0),
    pedmax = cms.untracked.double(250.0)                             
    )

process.source = cms.Source("EmptyIOVSource",                            
    #lastRun = cms.untracked.uint32(1),
    timetype = cms.string('runnumber'),
                            firstValue = cms.uint64(300000),
                            lastValue = cms.uint64(300000),
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
            tag = cms.string('GainCalib_offline_v2')
            )
        ),
    connect = cms.string('sqlite_file:gains.db')
)

process.p = cms.Path(process.gainDBOffline)

