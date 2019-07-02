#
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("simTest",eras.Run2_2017)

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.MagneticField_cff')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

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
#    'file:../scripts/sim.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu_phase1/pt100/simhits/simHits1.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu_phase1/pt100/simhits/simHits1_neutron1gev.root'
    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu_phase1/pt100/simhits/simHits1_neutron10mev.root'

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_design_v4-v1/20000/193FB1CE-333D-E540-995E-1BA38BA1CE3C.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_realistic_v8-v1/20000/0D388168-67C8-A544-8A75-8A96D40C396B.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_mc2017_realistic_v2-v1/20000/222AF93E-CE66-6342-A266-E78F6FCDD8EC.root"

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

