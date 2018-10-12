import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("Analysis",eras.Run2_2017)

# run the input file through the end;
# for a limited number of events, replace -1 with the desired number 
#
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#process.load( "SimGeneral.HepPDTESSource.pythiapdt_cfi" )

process.source = cms.Source( "PoolSource",
                             fileNames = cms.untracked.vstring(
			     'file:../scripts/sim2.root'
#			     'file:$home/work/MC/mu/pt100_pre10/simhits/simHits1.root'
#			     'file:$home/work/MC/mu/pt100_71_pre5/simhits/simHits1.root'
#			     'file:$home/work/MC/mu/pt100_71_pre5/simhits/simHits1_eta0p1.root'
			     )
                           )
	      
# FileService is mandatory, as the following analyzer module 
# will want it, to create output histogram file
# 
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("simAna.root")
)

# the analyzer itself - empty parameter set 
#
process.BasicTest = cms.EDAnalyzer( "SimHitAnalyzer",
#        NPartForHisto = cms.untracked.int32(100),
#        PtMaxForHisto = cms.untracked.double(200.0)
)

process.p1 = cms.Path( process.BasicTest )

