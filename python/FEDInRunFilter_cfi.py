import FWCore.ParameterSet.Config as cms

fedInRunFilter = cms.EDFilter("FEDInRunFilter",
                              maxBadFEDsBPIX = cms.uint32(0),
                              maxBadFEDsFPIX = cms.uint32(0),
                              maxBadFEDs = cms.uint32(0)
 )

