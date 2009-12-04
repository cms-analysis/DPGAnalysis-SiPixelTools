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
    #    "rfio:/castor/cern.ch/user/c/chiochia/09_beam_commissioning/BSCskim_123151_Express.root"
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/DC4465B0-F6DD-DE11-A7F1-000423D6B444.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/DA95AB4C-F7DD-DE11-B1D4-000423DD2F34.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/C6F6B54F-F7DD-DE11-B5C1-000423D6CA02.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/C499DCD4-FADD-DE11-9D82-001D09F28D4A.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/B4C44A4F-F7DD-DE11-8896-0019B9F709A4.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/B2EFCDAB-F8DD-DE11-B1A5-001D09F2532F.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/A40088D3-FADD-DE11-9EA6-001D09F290BF.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/A027B44B-F7DD-DE11-948A-000423D6B48C.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/7207EA6B-F4DD-DE11-9568-001D09F248F8.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/70DAA1AE-F6DD-DE11-8FC2-000423D99896.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/623BF14C-F7DD-DE11-81BF-000423D6CA42.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/5A8BD223-01DE-DE11-BF9C-0030486733D8.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/5A10EBA7-F8DD-DE11-9C78-001D09F254CE.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/4204294E-F7DD-DE11-AE9E-001D09F29114.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/3E6ADAAE-F6DD-DE11-87D6-000423D6B5C4.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/3AEBF014-F8DD-DE11-9DBA-001D09F25041.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/3630CC5B-F9DD-DE11-8460-001D09F242EF.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/345F157F-FBDD-DE11-83BE-001D09F27003.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/32F5F8A5-F8DD-DE11-B4CA-001D09F25456.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/14BFD3B2-F6DD-DE11-BD02-0030487C5CFA.root",
    "/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/0E45A7CE-F5DD-DE11-9B2E-001617E30CC8.root"
    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks::EXPRESS'

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
    rootFileName = "/afs/cern.ch/user/u/ursl/scratch0/pixel-123151-ExpressPhysics2.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(0),
    rootFileName           = cms.untracked.string(rootFileName),
    dumpAllEvents          = cms.untracked.int32(0),
    muonCollectionLabel    = cms.untracked.InputTag('muons'),
    trajectoryInputLabel   = cms.untracked.InputTag('TrackRefitter'),
    trackCollectionLabel   = cms.untracked.InputTag('generalTracks::EXPRESS'),
    pixelClusterLabel      = cms.untracked.InputTag('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap       = cms.untracked.InputTag('hltL1GtObjectMap'), 
    HLTResultsLabel        = cms.untracked.InputTag('TriggerResults::HLT')
    )



# -- Path
process.p = cms.Path(
#    process.superPointingFilter*
    process.TrackRefitter*
    process.PixelTree
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = 'INFO'
#process.TrackerDigiGeometryESModule.applyAlignment = True
