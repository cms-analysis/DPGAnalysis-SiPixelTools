# validation 
import FWCore.ParameterSet.Config as cms
pixRecHitsValid = cms.EDAnalyzer("SiPixelRecHitsValid_pix",
    src = cms.InputTag("siPixelRecHits"),
    useTracks = cms.untracked.bool(False),
    tracks = cms.untracked.InputTag("generalTracks"),
    pixelSimLinkSrc = cms.InputTag("simSiPixelDigis"),
    outputFile = cms.untracked.string('pixelrechits.root'),
#    ROUList = cms.vstring('g4SimHitsTrackerHitsPixelBarrelLowTof', 
#        'g4SimHitsTrackerHitsPixelBarrelHighTof', 
#        'g4SimHitsTrackerHitsPixelEndcapLowTof', 
#        'g4SimHitsTrackerHitsPixelEndcapHighTof'),
     ROUList = cms.vstring(
        'TrackerHitsPixelBarrelLowTof', 
        'TrackerHitsPixelBarrelHighTof', 
        'TrackerHitsPixelEndcapLowTof', 
        'TrackerHitsPixelEndcapHighTof'),
    associatePixel = cms.bool(True),
    associateStrip = cms.bool(False),
    associateRecoTracks = cms.bool(False),
    verbose = cms.untracked.bool(False),
    muOnly = cms.untracked.bool(False),
    ptCut = cms.untracked.double(1.0)
)
