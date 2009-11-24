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
process.GlobalTag.globaltag = "GR09_P_V6::All"

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
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/F6E6E5CD-60D8-DE11-A93B-0019B9F705A3.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/CECE0A1C-60D8-DE11-8B50-001D09F26C5C.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/C4F34BCE-60D8-DE11-BA15-0019B9F581C9.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/7C08033A-62D8-DE11-A81E-001D09F2A49C.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/66828337-62D8-DE11-A665-001D09F24024.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/64194B3A-62D8-DE11-9730-001D09F27067.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/4259728C-61D8-DE11-BE08-000423D6A6F4.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/3290CD36-62D8-DE11-B029-001D09F24DDF.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/2E6A82EE-60D8-DE11-B655-001D09F2423B.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/245448AF-5ED8-DE11-9E67-001D09F2527B.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/2288321E-60D8-DE11-B57D-001D09F2AF1E.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/1653DF8D-61D8-DE11-A283-00304879FBB2.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/122/314/04E779D1-60D8-DE11-9811-001D09F24498.root"
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
    SALabel = cms.string("globalTracks"),
    PropagatorName = cms.string("SteppingHelixPropagatorAny"),
    radius = cms.double(10.0),
    maxZ = cms.double(50.0)
    )




# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "pixel-collision.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(1),
    rootFileName           = cms.untracked.string(rootFileName),
    dumpAllEvents          = cms.untracked.int32(1),
    muonCollectionLabel    = cms.untracked.string('muons'),
    trajectoryInputLabel   = cms.untracked.string('TrackRefitter'),
    trackCollectionLabel   = cms.untracked.string('generalTracks'),
    pixelClusterLabel      = cms.untracked.string('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.string("gtDigis"), 
    hltL1GtObjectMap       = cms.untracked.InputTag("hltL1GtObjectMap"), 
    HLTResultsLabel        = cms.untracked.InputTag("TriggerResults::HLT")
    )



# -- Path
process.p = cms.Path(
    process.superPointingFilter*
    process.TrackRefitter*
    process.PixelTree
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = 'INFO'
#process.TrackerDigiGeometryESModule.applyAlignment = True
