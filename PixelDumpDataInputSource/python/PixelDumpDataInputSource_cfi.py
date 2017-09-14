import FWCore.ParameterSet.Config as cms

source = cms.Source("PixelDumpDataInputSource",
    runNumber = cms.untracked.int32(-1),
    fileNames = cms.untracked.vstring('file:PixelAlive_070106d.dmp'),
    fedid = cms.untracked.int32(-1)
)


