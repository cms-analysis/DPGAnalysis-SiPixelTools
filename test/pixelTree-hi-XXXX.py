# ----------------------------------------------------------------------
# -- RECO py template file for dumping the PixelTree only
# ----------------------------------------------------------------------
import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR10_P_V12::All"

# -- Input files
# POOLSOURCE

# -- Input files
#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    REPLACEFILES
#    )
#    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks'

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

# -- skimming
process.PixelFilter = cms.EDFilter(
    "SkimEvents",
    verbose                        = cms.untracked.int32(0),
    filterOnPrimaryVertex          = cms.untracked.int32(0),
    primaryVertexCollectionLabel   = cms.untracked.InputTag('offlinePrimaryVertices'),
    filterOnTracks                 = cms.untracked.int32(0),
    trackCollectionLabel           = cms.untracked.InputTag('generalTracks'),
    filterOnPixelCluster           = cms.untracked.int32(1),
    PixelClusterCollectionLabel    = cms.untracked.InputTag('siPixelClusters'),
    filterOnL1TechnicalTriggerBits = cms.untracked.int32(0),
    L1TechnicalTriggerBits         = cms.untracked.vint32(40, 41)
    )

# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "rfio:/castor/cern.ch/cms/store/group/tracker/pixel/PixelTree/reco/pixelTree-XXXX.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(0),
    rootFileName                 = cms.untracked.string(rootFileName),
    dumpAllEvents                = cms.untracked.int32(0),
#    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('hiSelectedVertex'),
    muonCollectionLabel          = cms.untracked.InputTag('muons'),
    trajectoryInputLabel         = cms.untracked.InputTag('TrackRefitter::Demo'),
    trackCollectionLabel         = cms.untracked.InputTag('generalTracks'),
    pixelClusterLabel            = cms.untracked.InputTag('siPixelClusters'),
    pixelRecHitLabel             = cms.untracked.InputTag('siPixelRecHits'),
    HLTProcessName               = cms.untracked.string('HLT'), 
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap'), 
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::HLT')
    )

# -- Path
process.p = cms.Path(
#    process.PixelFilter*
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    )
