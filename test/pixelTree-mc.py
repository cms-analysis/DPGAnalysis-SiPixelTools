import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "STARTUP31X_V7::All"

# -- Input files
process.source = cms.Source(
    "PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
 "/store/mc/Summer09/TkCosmics38T/RAW-RECO/STARTUP31X_V3_SuperPointing-v1/0005/C8D3D828-05B4-DE11-AFBA-001EC9AA9996.root"
    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'ctfWithMaterialTracksP5'
process.TrackRefitterP5.TrajectoryInEvent = True
#process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

# -- the tree filler
process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(0),
    rootFileName           = cms.untracked.string("pixelTree-mc.root"),
    trajectoryInputLabel   = cms.untracked.string('TrackRefitterP5'),
    muonCollectionLabel    = cms.untracked.string('muons'),
    trackCollectionLabel   = cms.untracked.string('TrackRefitterP5'),
    pixelClusterLabel      = cms.untracked.string('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.string("gtDigis"), 
    hltL1GtObjectMap       = cms.untracked.InputTag("hltL1GtObjectMap"), 
    HLTResultsLabel        = cms.untracked.InputTag("TriggerResults::HLT")
    )



# -- Path
process.p = cms.Path(
    # filters:
    #process.fedInRunFilter*
    # standard reco sequence
    # process.RawToDigi*process.reconstructionCosmics*
    #process.trackReconstruction*
    # more filters:
    #process.MuonTOFFilter_trackQuality *
    # create rechits
    # (rechits are transient data members so are not saved in CMSSW .root files)
    #    process.offlineBeamSpot*
    #   refitters for all tracking algos (thse are what actually create the rechits)
    #    process.ctfRefitter*
    #    process.rsRefitter*
    #    process.cosmRefitter*
    # run ntuplizers
    process.TrackRefitterP5*
    process.PixelTree
    )

#process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = 'INFO'
#process.TrackerDigiGeometryESModule.applyAlignment = True
