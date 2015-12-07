# Read RecHits
##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("RecHitRead")
 
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')

#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

# for raw
#process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
#process.load('Configuration.StandardSequences.DigiToRaw_cff')
#process.load('Configuration.StandardSequences.RawToDigi_cff')


# needed for pixel RecHits 
#process.load("Configuration.StandardSequences.Reconstruction_cff")


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
#       threshold = cms.untracked.string('ERROR')
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:rechits.root'
  )
)

# a service to use root histos (keep if the analyser is run at the end)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
)

# read rechits
process.analysis = cms.EDAnalyzer("PixRecHitTest",
    Verbosity = cms.untracked.bool(True),
#    src = cms.InputTag("siPixelRecHits"),
    src = cms.InputTag("siPixelRecHitsPreSplitting"),
)
process.p1 = cms.Path(process.analysis)

