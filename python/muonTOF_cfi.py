import FWCore.ParameterSet.Config as cms

MuonTOFFilter_trackQuality = cms.EDFilter("MuonTOFFilter_trackQuality",
    max_goodmuons = cms.double(2),
    min_time      = cms.double(-20),
    max_time      = cms.double(20),
    max_timeError = cms.double(10)                         
 )

