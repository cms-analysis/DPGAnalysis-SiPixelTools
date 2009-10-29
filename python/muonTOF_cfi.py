import FWCore.ParameterSet.Config as cms

MuonTOFFilter_trackQuality = cms.EDFilter("MuonTOFFilter_trackQuality",
    min_goodmuons = cms.int(1),                                    
    max_goodmuons = cms.int(3),
    min_time      = cms.double(-20),
    max_time      = cms.double(20),
    max_timeError = cms.double(10),
    min_trk_pt    = cms.double(4),
    max_chi2_ndof = cms.double(20)                                    
 )

