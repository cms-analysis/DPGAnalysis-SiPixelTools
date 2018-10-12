#
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("simTest",eras.Run2_2017)

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.MagneticField_cff')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('PixSimHitsTest'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.source = cms.Source("PoolSource",
    fileNames =  cms.untracked.vstring(
#    '/store/user/kotlinski/mu100_v74/simhits/simHits1.root',
    'file:../scripts/sim.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu/pt100_72/simhits/simHits1.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu/pt100_71_pre5/simhits/simHits2.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu/pt100_71_pre5/simhits/simHits3.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu/pt100_71_pre5/simhits/simHits4.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu/pt100_71_pre5/simhits/simHits5.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu/pt100_71_pre5/simhits/simHits6.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mb/13tev/simhits/simHits1.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mb/8tev/simhits/simHits1.root'
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('sim_histos.root')
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '') # phase 1 WRONG in 92X
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '') # phase 1 OK

# use the test from SiTracker
#process.analysis =  cms.EDAnalyzer("PixelSimHitsTest",
# or from DPGAnalysis 
process.analysis =  cms.EDAnalyzer("PixSimHitsTest",
	src = cms.string("g4SimHits"),
        mode = cms.untracked.string("bpix"),
	list = cms.string("TrackerHitsPixelBarrelLowTof"),
#	list = cms.string("TrackerHitsPixelBarrelHighTof"),
#        mode = cms.untracked.string("fpix"),
#	list = cms.string("TrackerHitsPixelEndcapLowTof"),
#	list = cms.string("TrackerHitsPixelEndcapHighTof"),
        Verbosity = cms.untracked.bool(True),
        phase1 = cms.untracked.bool(True),
)

process.p = cms.Path(process.analysis)

