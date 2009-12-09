import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ----------------------------------------------------------------------
# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR09_P_V6::All"

# ----------------------------------------------------------------------
process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring(
    REPLACEFILES
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


# ----------------------------------------------------------------------
# -- PixelTree
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks'

try:
    rootFileName = os.environ["JOB"] + "-pixel.root"
except KeyError:
    rootFileName = "rfio:/castor/cern.ch/cms/store/group/tracker/pixel/PixelTree/test/pixelTree-XXXX.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(2),
    rootFileName           = cms.untracked.string(rootFileName),
    muonCollectionLabel    = cms.untracked.InputTag('muons'),
    trackCollectionLabel   = cms.untracked.InputTag('generalTracks'),
    trajectoryInputLabel   = cms.untracked.InputTag('TrackRefitter'),
    pixelClusterLabel      = cms.untracked.InputTag('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.InputTag("gtDigis"), 
    hltL1GtObjectMap       = cms.untracked.InputTag("hltL1GtObjectMap"), 
    HLTResultsLabel        = cms.untracked.InputTag("TriggerResults::HLT")
    )

# ----------------------------------------------------------------------
process.PixelFilter = cms.EDFilter(
    "SkimEvents",
    verbose                        = cms.untracked.int32(2),
    filterOnPrimaryVertex          = cms.untracked.int32(1),
    primaryVertexCollectionLabel   = cms.untracked.InputTag('offlinePrimaryVertices'),
    filterOnTracks                 = cms.untracked.int32(1),
    trackCollectionLabel           = cms.untracked.InputTag('generalTracks'),
    filterOnPixelCluster           = cms.untracked.int32(1),
    PixelClusterCollectionLabel    = cms.untracked.InputTag('siPixelClusters'),
    filterOnL1TechnicalTriggerBits = cms.untracked.int32(1),
    L1TechnicalTriggerBits         = cms.untracked.vint32(40, 41)
    
    )


# ----------------------------------------------------------------------
process.HepPDTESSource = cms.ESSource(
    "HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/particle.tbl')
    )
process.genParticles = cms.EDProducer(
    "GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("generator"),
    abortOnUnknownPDGCode = cms.untracked.bool(False)
    )


# ----------------------------------------------------------------------
process.genDump = cms.EDAnalyzer(
    "HFDumpGenerator",
    generatorCandidates = cms.untracked.string('genParticles'),
    generatorEvent = cms.untracked.string('generator')
    )


# ----------------------------------------------------------------------
try:
    rootFileName = os.environ["JOB"] + "-hfa.root"
except KeyError:
    rootFileName = "rfio:/castor/cern.ch/cms/store/group/tracker/pixel/PixelTree/test/hfa-XXXX.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(0),
    requireCand  =  cms.untracked.bool(True),
    fileName     = cms.untracked.string(rootFileName)
    )


# ----------------------------------------------------------------------
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.trkDump = cms.EDAnalyzer(
    "HFDumpTracks",
    verbose                = cms.untracked.int32(0),
    generatorEventLabel    = cms.untracked.InputTag('generator'),
    muonsLabel             = cms.untracked.InputTag("muons"),
    trackingParticlesLabel = cms.untracked.InputTag('trackingParticles'),
    associatorLabel        = cms.untracked.InputTag('TrackAssociatorByHits'),
    doTruthMatching        = cms.untracked.int32(0),
    tracksLabel            = cms.untracked.InputTag('generalTracks'),
    simTracksLabel         = cms.untracked.InputTag('allLayer1TrackCands')
    )

# ----------------------------------------------------------------------
process.stuffDump = cms.EDAnalyzer(
    "HFDumpStuff",
    verbose                  = cms.untracked.int32(0),
    PrimaryVertexLabel       = cms.untracked.InputTag("offlinePrimaryVertices"),
    PrimaryVertexTracksLabel = cms.untracked.InputTag("generalTracks")
    )

# ----------------------------------------------------------------------
process.muonDump = cms.EDAnalyzer(
    "HFDumpMuons",
    verbose         = cms.untracked.int32(0),
    muonsLabel      = cms.untracked.InputTag("muons"),
    doTruthMatching = cms.untracked.int32(0),
    )


# ----------------------------------------------------------------------
process.triggerDump = cms.EDAnalyzer(
    "HFDumpTrigger",
    verbose                 = cms.untracked.int32(0),
    L1GTReadoutRecordLabel  = cms.untracked.string("gtDigis"), 
    hltL1GtObjectMap        = cms.untracked.InputTag("hltL1GtObjectMap"), 
    L1MuonsLabel            = cms.untracked.InputTag("hltL1extraParticles"), 
    HLTResultsLabel         = cms.untracked.InputTag("TriggerResults::HLT"), 
    TriggerEventLabel       = cms.untracked.InputTag("hltTriggerSummaryAOD::HLT"), 
    hltLabel                = cms.untracked.InputTag("TriggerResults::HLT"), 
    )

# ----------------------------------------------------------------------
process.bmtDump = cms.EDAnalyzer(
    "HFMuonAndTrack",
    verbose       = cms.untracked.int32(0), 
    muonsLabel    = cms.untracked.InputTag("muons"),
    tracksLabel   = cms.untracked.InputTag('generalTracks'),
    muonPt        = cms.untracked.double(1.0),
    trackPt       = cms.untracked.double(0.5),
    type          = cms.untracked.int32(1300), 
    massLow       = cms.untracked.double(1.5), 
    massHigh      = cms.untracked.double(11.0)
    )

# ----------------------------------------------------------------------
process.bmmDump = cms.EDAnalyzer(
    "HFDimuons",
    verbose      = cms.untracked.int32(0), 
    muonsLabel   = cms.untracked.InputTag("muons"),
    tracksLabel  = cms.untracked.InputTag('generalTracks'),
    muonPt       = cms.untracked.double(1.0),
    type         = cms.untracked.int32(531), 
    massLow      = cms.untracked.double(4.0), 
    massHigh     = cms.untracked.double(6.0)
    )

# ----------------------------------------------------------------------
process.bupsikpDump = cms.EDAnalyzer(
    "HFBu2JpsiKp",
    verbose      = cms.untracked.int32(0), 
    muonsLabel   = cms.untracked.InputTag("muons"),
    tracksLabel  = cms.untracked.InputTag('generalTracks'),
    muonPt       = cms.untracked.double(1.0),
    trackPt      = cms.untracked.double(0.1),
    type         = cms.untracked.int32(100521) 
    )

# ----------------------------------------------------------------------
process.bdpsikstarDump = cms.EDAnalyzer(
    "HFBd2JpsiKstar",
    verbose      = cms.untracked.int32(0), 
    muonsLabel   = cms.untracked.InputTag("muons"),
    tracksLabel  = cms.untracked.InputTag('generalTracks'),
    muonPt       = cms.untracked.double(1.0),
    kaonPt       = cms.untracked.double(0.1),
    pionPt       = cms.untracked.double(0.1),
    type         = cms.untracked.int32(100511) 
    )

# ----------------------------------------------------------------------
process.bspsiphiDump = cms.EDAnalyzer(
    "HFBs2JpsiPhi",
    verbose      = cms.untracked.int32(0), 
    muonsLabel   = cms.untracked.InputTag("muons"),
    tracksLabel  = cms.untracked.InputTag('generalTracks'),
    muonPt       = cms.untracked.double(1.0),
    trackPt      = cms.untracked.double(0.1),
    type         = cms.untracked.int32(100531) 
    )

# ----------------------------------------------------------------------
process.b2muD0Dump = cms.EDAnalyzer(
    "HFB2muD0",
    verbose      = cms.untracked.int32(0), 
    muonsLabel   = cms.untracked.InputTag("muons"),
    tracksLabel  = cms.untracked.InputTag('generalTracks'),
    muonPt       = cms.untracked.double(1.0),
    trackPt      = cms.untracked.double(0.1),
    deltaR       = cms.untracked.double(1.5),
    deltaMD0     = cms.untracked.double(0.3),
    deltaMDs     = cms.untracked.double(0.3)
    )

# ----------------------------------------------------------------------
process.p = cms.Path(
    process.PixelFilter* 
    process.TrackRefitter*
    process.PixelTree*
#    process.genParticles* 
#    process.genDump*
    process.trkDump*
    process.stuffDump*
    process.muonDump*
    process.triggerDump*
    process.bmmDump*
    process.bmtDump*
    process.bupsikpDump*
    process.bdpsikstarDump*
    process.bspsiphiDump*
    process.b2muD0Dump*
    process.tree
)




