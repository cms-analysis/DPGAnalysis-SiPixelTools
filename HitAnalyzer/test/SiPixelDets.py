import FWCore.ParameterSet.Config as cms

process = cms.Process("SiPixelDets")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_R_70_V1::All"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

# needed for geometry->detunits
process.load("Configuration.StandardSequences.Geometry_cff")

process.source = cms.Source( "EmptySource",
#process.source = cms.Source( "PoolSource",
#                             fileNames = cms.untracked.vstring(
#			     'file:$home/work/MC/mu/pt100_71_pre5/simhits/simHits1.root'
#			     )
)
	      
# FileService is mandatory, as the following analyzer module 
# will want it, to create output histogram file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test.root")
)

# the analyzer itself - empty parameter set 
process.test = cms.EDAnalyzer( "SiPixelDets",
#        NPartForHisto = cms.untracked.int32(100),
#        PtMaxForHisto = cms.untracked.double(200.0)
)

process.p1 = cms.Path( process.test )

