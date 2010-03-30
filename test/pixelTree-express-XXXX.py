# ----------------------------------------------------------------------
# -- EXPRESS py template file for dumping the PixelTree only
# ----------------------------------------------------------------------
import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR10_P_V4::All"

# -- Input files
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
    REPLACEFILES
    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks::EXPRESS'

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
process.siPixelRecHits.src = 'siPixelClusters::EXPRESS'

# -- skimming
process.PixelFilter = cms.EDFilter(
    "SkimEvents",
    verbose                        = cms.untracked.int32(0),
    filterOnPrimaryVertex          = cms.untracked.int32(1),
    primaryVertexCollectionLabel   = cms.untracked.InputTag('offlinePrimaryVertices::EXPRESS'),
    filterOnTracks                 = cms.untracked.int32(1),
    trackCollectionLabel           = cms.untracked.InputTag('generalTracks::EXPRESS'),
    filterOnPixelCluster           = cms.untracked.int32(1),
    PixelClusterCollectionLabel    = cms.untracked.InputTag('siPixelClusters::EXPRESS'),
    filterOnL1TechnicalTriggerBits = cms.untracked.int32(1),
    L1TechnicalTriggerBits         = cms.untracked.vint32(40, 41)
    )

# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "rfio:/castor/cern.ch/cms/store/group/tracker/pixel/PixelTree/express/r23/pixelTree-ExpressPhysics-XXXX.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(0),
    rootFileName                 = cms.untracked.string(rootFileName),
    dumpAllEvents                = cms.untracked.int32(0),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices::EXPRESS'),
    muonCollectionLabel          = cms.untracked.InputTag('muons'),
    trajectoryInputLabel         = cms.untracked.InputTag('TrackRefitter::Demo'),
    trackCollectionLabel         = cms.untracked.InputTag('generalTracks::EXPRESS'),
    pixelClusterLabel            = cms.untracked.InputTag('siPixelClusters::EXPRESS'),
    pixelRecHitLabel             = cms.untracked.InputTag('siPixelRecHits::Demo'),
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis::EXPRESS'), 
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap::HLT'), 
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::EXPRESS')
    )

# -- Path
process.p = cms.Path(
#    process.PixelFilter* 
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    )
