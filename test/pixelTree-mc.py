import os
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
process.GlobalTag.globaltag = "STARTUP3X_V8D::All"
# -- Input files
process.source = cms.Source(
    "PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
     'rfio:/castor/cern.ch/cms/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/E4B3A7BE-3AD7-DE11-9230-002618943939.root'

    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )


#process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
#process.load("Configuration.StandardSequences.Reconstruction_cff")


# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")




# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "pixel-mc.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(0),
    rootFileName           = cms.untracked.string(rootFileName),
    trajectoryInputLabel   = cms.untracked.InputTag('TrackRefitter'),
    muonCollectionLabel    = cms.untracked.InputTag('muons'),
    trackCollectionLabel   = cms.untracked.InputTag('generalTracks'),
    pixelClusterLabel      = cms.untracked.InputTag('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap       = cms.untracked.InputTag('hltL1GtObjectMap'), 
    HLTResultsLabel        = cms.untracked.InputTag('TriggerResults::HLT'),
    storeRecHit            = cms.untracked.int32(1)
    )






# -- Path
process.p = cms.Path(
    # filters:
    #process.fedInRunFilter*
    # standard reco sequence
#    process.RawToDigi*process.reconstructionCosmics*
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
    process.TrackRefitter*
    process.PixelTree
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'DEBUG'
#process.TrackerDigiGeometryESModule.applyAlignment = True
