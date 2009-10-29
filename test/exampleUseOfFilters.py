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
process.GlobalTag.globaltag = "STARTUP3XY_V9::All"

# -- Input files
process.source = cms.Source(
    "PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
    "/store/relval/CMSSW_3_4_0_pre2/RelValZmumuJets_Pt_20_300_GEN/GEN-SIM-RECO/MC_3XY_V10_LowLumiPileUp-v1/0003/F68BF339-7FBE-DE11-BF9F-0018F3D096A2.root"
    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
    )

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'ctfWithMaterialTracksP5'
process.TrackRefitterP5.TrajectoryInEvent = True
#process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

# -- the tree filler
process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(0),
    rootFileName           = cms.untracked.string("pixelTree-test.root"),
    trajectoryInputLabel   = cms.untracked.string('TrackRefitterP5'),
    muonCollectionLabel    = cms.untracked.string('muons'),
    trackCollectionLabel   = cms.untracked.string('globalTracks'),
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
    #process.TrackRefitterP5*
    process.PixelTree
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 10
#process.MessageLogger.cerr.threshold = 'INFO'
#process.TrackerDigiGeometryESModule.applyAlignment = True
