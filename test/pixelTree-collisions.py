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
#process.GlobalTag.globaltag = "GR09_E_V6::All"
process.GlobalTag.globaltag = "FIRSTCOLL::All"

# -- Input files
process.source = cms.Source(
    "PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
#    "file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_GLOBAL/bit40or41skim.root"
    #'rfio:/castor/cern.ch/user/c/chiochia/09_beam_commissioning/BSCskim_123151_Express_bit40-41.root'
    #'rfio:/castor/cern.ch/user/c/chiochia/09_beam_commissioning/BSCskim_123615_Express_bit40-41_LS72-88.root'
    'file:/nfs/data6/CMS_DATA/BSCskim_123615_Express_bit40-41_LS72-88.root'
    #'rfio:/castor/cern.ch/user/c/chiochia/09_beam_commissioning/BSCskim_123596_Express_bit40-41_LS69-129.root'
    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

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
    rootFileName = "collisions_900GeV_ntuplizer.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(2),
    rootFileName           = cms.untracked.string(rootFileName),
    dumpAllEvents          = cms.untracked.int32(1),
    muonCollectionLabel    = cms.untracked.InputTag('muons'),
    trajectoryInputLabel   = cms.untracked.InputTag('TrackRefitter'),
    trackCollectionLabel   = cms.untracked.InputTag('generalTracks::EXPRESS'),
    pixelClusterLabel      = cms.untracked.InputTag('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.InputTag("gtDigis"),
    hltL1GtObjectMap       = cms.untracked.InputTag("hltL1GtObjectMap"),
    HLTResultsLabel        = cms.untracked.InputTag("TriggerResults::HLT"),
    storeRecHit        = cms.untracked.int32(1)
    )



    


# -- Path
process.p = cms.Path(
#    process.superPointingFilter*
    process.TrackRefitter*
    process.PixelTree
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'DEBUG'
#process.TrackerDigiGeometryESModule.applyAlignment = True
