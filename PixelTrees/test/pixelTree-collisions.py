import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound'), wantSummary = cms.untracked.bool(True) )


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

# -- Input files
process.source = cms.Source(
    "PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
	"/store/relval/CMSSW_7_4_0_pre2/MinimumBias/RECO/GR_R_73_V0A_RelVal_mb2012D-v1/00000/FE3DCAC0-F584-E411-A9CE-02163E00FACD.root"
    )
    )
# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
    )

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks'
process.TrackRefitter.NavigationSchool = ""
# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

# -- skimming
process.superPointingFilter = cms.EDFilter(
    "HLTMuonPointingFilter",
    SALabel = cms.string("generalTracks"),
    PropagatorName = cms.string("SteppingHelixPropagatorAny"),
    radius = cms.double(10.0),
    maxZ = cms.double(50.0)
    )

# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "pixelTree_data.root"


process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(0),
    rootFileName                 = cms.untracked.string(rootFileName),
    #type                         = cms.untracked.string(getDataset(process.source.fileNames[0])),
    globalTag                    = process.GlobalTag.globaltag,
    dumpAllEvents                = cms.untracked.int32(0),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
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
#    process.superPointingFilter*
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    )

#process.TrackerDigiGeometryESModule.applyAlignment = True
