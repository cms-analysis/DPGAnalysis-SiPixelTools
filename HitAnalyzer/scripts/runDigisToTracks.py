# produce pixel cluster & rechits from digia
# works directly or through raw
# 
#
##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("TrackTest")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# for data?
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')


# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

# for raw
#process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')

# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.load("RecoTracker.Configuration.RecoTracker_cff")

process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.MessageLogger = cms.Service("MessageLogger",
#    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    debugModules = cms.untracked.vstring('SiPixelRecHits'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
       threshold = cms.untracked.string('ERROR')
#        threshold = cms.untracked.string('WARNING')
#        threshold = cms.untracked.string('DEBUG')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)
# get the files from DBS:
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/digis_trk/digis1_postls171.root'
    'file:digis.root'
    )
)

# Choose the global tag here:
# process.GlobalTag.globaltag = 'MC_71_V7::All'
# process.GlobalTag.globaltag = 'POSTLS171_V1::All'
# process.GlobalTag.globaltag = 'PRE_MC_71_V2::All'
# for 73
process.GlobalTag.globaltag = "MCRUN1_73_V5::All" # OK for condDB

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
#        outputCommands = cms.untracked.vstring('drop *','keep *_*_*_TrackTest'),
        fileName = cms.untracked.string('file:tracks.root'),
#       fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks1_postls171.root'),
#    splitLevel = cms.untracked.int32(0),
#    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )

)

#process.Timing = cms.Service("Timing")
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

# My 
# modify clusterie parameters
#process.siPixelClusters.ClusterThreshold = 4000.0

# direct clusterization (no raw step)
# label of digis 
#process.siPixelClusters.src = 'mix'

# plus pixel clusters  (OK)
#process.p1 = cms.Path(process.siPixelClusters)

# plus pixel rechits (OK)
#process.p1 = cms.Path(process.pixeltrackerlocalreco)

# clusterize through raw (OK)
# for digi to raw 
# my old
#process.siPixelRawData.InputLabel = 'mix'
#process.SiStripDigiToRaw.InputModuleLabel = 'mix'
# my new 
process.siPixelRawData.InputLabel = 'simSiPixelDigis'
process.SiStripDigiToRaw.InputModuleLabel = 'simSiPixelDigis'
# for Raw2digi for simulations 
process.siPixelDigis.InputLabel = 'siPixelRawData'
process.siStripDigis.ProductLabel = 'SiStripDigiToRaw'
# for digi to clu
process.siPixelClusters.src = 'siPixelDigis'

# pixel only 
#process.p1 = cms.Path(process.siPixelRawData)
#process.p1 = cms.Path(process.siPixelRawData*process.siPixelDigis)
#process.p1 = cms.Path(process.siPixelRawData*process.siPixelDigis*process.pixeltrackerlocalreco)

# with strips ok
#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw)
#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis)

# runs ok
#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco)

# runs ok
#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot)

# runs ok
#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.MeasurementTrackerEvent)

# runs ok
#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.MeasurementTrackerEvent*process.siPixelClusterShapeCache*process.recopixelvertexing)

process.load("RecoTracker.IterativeTracking.iterativeTk_cff")

# copy the sequence below from  
# RecoTracker/IterativeTracking/python/iterativeTk_cff.py  - 71_pre7
process.myTracking = cms.Sequence(process.InitialStep*
                            process.DetachedTripletStep*
                            process.LowPtTripletStep*
                            process.PixelPairStep*
                            process.MixedTripletStep*
                            process.PixelLessStep*
                            process.TobTecStep*
                            process.earlyGeneralTracks*
                            # muonSeededStep*
                            process.preDuplicateMergingGeneralTracks*
                            process.generalTracksSequence*
                            process.ConvStep*
                            process.conversionStepTracks
                            )

# run full tracking
# trackingGlobalReco does not work, needs EarlyMuons for muon seeding.
# ckftracks & iterTracking does not work as well  (same problem).
process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.siPixelClusterShapeCache*process.recopixelvertexing*process.MeasurementTrackerEvent*process.myTracking*process.vertexreco)

#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.siPixelClusterShapeCache*process.recopixelvertexing*process.MeasurementTrackerEvent*process.iterTracking*process.vertexreco)


# unused 
# Path and EndPath definitions
#process.raw2digi_step = cms.Path(process.RawToDigi)
#process.reconstruction_step = cms.Path(process.reconstruction)
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

process.outpath = cms.EndPath(process.o1)
