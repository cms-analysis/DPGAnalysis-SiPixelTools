import FWCore.ParameterSet.Config as cms

PixelNtuplizer_RealData = cms.EDFilter("PixelNtuplizer_RealData",
    trajectoryInput = cms.string('TrackRefitter'),
    useAllPixel = cms.bool(False),
# should be moved to TFileService.                                       
    OutputFile = cms.string('TTreeFile.root') 
)

