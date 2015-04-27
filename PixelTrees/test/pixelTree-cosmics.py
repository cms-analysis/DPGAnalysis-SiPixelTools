# ----------------------------------------------------------------------
# -- RECO py template file for dumping the PixelTree only
# ----------------------------------------------------------------------

import os
import FWCore.ParameterSet.Config as cms
process = cms.Process("Demo")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')


# -- Input files
# POOLSOURCE

# -- Input files
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        "/store/data/Commissioning2015/Cosmics/RECO/PromptReco-v1/000/236/351/00000/F67E9CEA-28C2-E411-A9F1-02163E011D8B.root"
    )
)

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'ctfWithMaterialTracksP5'
process.TrackRefitter.NavigationSchool=""

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "pixelTree-cosmics.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(0),
    rootFileName                 = cms.untracked.string(rootFileName),
    #type                         = cms.untracked.string(),
    globalTag                    = process.GlobalTag.globaltag,
    dumpAllEvents                = cms.untracked.int32(0),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
    muonCollectionLabel          = cms.untracked.InputTag('muons'),
    trajectoryInputLabel         = cms.untracked.InputTag('TrackRefitter::Demo'),
    trackCollectionLabel         = cms.untracked.InputTag('splittedTracksP5'),
    pixelClusterLabel            = cms.untracked.InputTag('siPixelClusters'),
    pixelRecHitLabel             = cms.untracked.InputTag('siPixelRecHits'),
    HLTProcessName               = cms.untracked.string('HLT'), 
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap'), 
    l1muGmtLabel                 = cms.untracked.InputTag('gtDigis'), 
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::HLT')
    )

# -- Path
process.p = cms.Path(
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    )
