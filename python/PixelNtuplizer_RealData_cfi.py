import FWCore.ParameterSet.Config as cms

PixelNtuplizer_RealData = cms.EDFilter("PixelNtuplizer_RealData",
    trajectoryInput = cms.string('TrackRefitter'),
# should be moved to TFileService.                                       
    OutputFile = cms.string('TTreeFile.root') 
)

