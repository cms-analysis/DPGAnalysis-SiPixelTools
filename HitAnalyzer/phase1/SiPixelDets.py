import FWCore.ParameterSet.Config as cms
process = cms.Process("SiPixelDets")
# needed for det-id
#process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2017_cff')

# turn off alignment  (presently for phase1 there is no alignment correction)
#from Geometry.TrackerGeometryBuilder.trackerGeometryDB_cfi import trackerGeometryDB
#trackerGeometryDB.applyAlignment = cms.bool(False)


#Load the correct Magnetic Field
# Control the template selection either through the run number or by explicitly 
# using the specific mag field map
#process.load("Configuration.StandardSequences.MagneticField_0T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
#process.load("Configuration.StandardSequences.MagneticField_cff") # same

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '76X_upgrade2017_design_v8', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


process.source = cms.Source( "EmptySource",
)
	      
# FileService is mandatory, as the following analyzer module 
# will want it, to create output histogram file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test.root")
)

# the analyzer itself - empty parameter set 
process.test = cms.EDAnalyzer( "SiPixelDets",
#         phase1=cms.untracked.bool(True),                      
#        NPartForHisto = cms.untracked.int32(100),
#        PtMaxForHisto = cms.untracked.double(200.0)
)

process.p1 = cms.Path( process.test )

