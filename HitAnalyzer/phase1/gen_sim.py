#
import FWCore.ParameterSet.Config as cms

process = cms.Process("GenTest")

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')

process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2017_cff')


process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')

#process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('IOMC.EventVertexGenerators.VtxSmearedGauss_cfi')

process.load('Configuration.StandardSequences.SimIdeal_cff')

# GTs
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#from Configuration.AlCa.autoCond_condDBv2 import autoCond
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag.globaltag = autoCond['run2_design']
#process.GlobalTag.globaltag = autoCond['run2_mc']
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
)

# Init random numbers - 1st file    
process.RandomNumberGeneratorService.generator.initialSeed = 1
# next files 
#process.RandomNumberGeneratorService.generator.initialSeed = 2
#process.RandomNumberGeneratorService.generator.initialSeed = 3
#process.RandomNumberGeneratorService.generator.initialSeed = 4
#process.RandomNumberGeneratorService.generator.initialSeed = 5



###########################################
# list of possible events to be generated #
###########################################
#
# single muons 
#process.generator = cms.EDProducer("FlatRandomPtGunProducer",
process.generator = cms.EDFilter("Pythia8PtGun",
    PGunParameters = cms.PSet(
        AddAntiParticle = cms.bool(True), # True makes two muons per event
        MaxPt = cms.double(100.1),
        MinPt = cms.double(99.9),
        ParticleID = cms.vint32(13,13),
        MaxEta = cms.double(1.0), # 2.5
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-1.0), # -2.5
        MinPhi = cms.double(-3.14159265359) ## in radians
    ),
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring()
    ),
    Verbosity = cms.untracked.int32(1), ## set to 1 (or greater)  for printouts
    # firstRun = cms.untracked.uint32(1),
    psethack = cms.string('4 muona at pt 100')
)

## # choose particle
#process.generator.PGunParameters.PartID[0] = 13
## # example: for 4 muons to test with vertex
## #process.generator.PGunParameters.PartID = cms.untracked.vint32(13,-13,13,-13)
## # example: for opposite sign back-to-back dimuon pairs set to True
## # define limits for Pt
#process.generator.PGunParameters.MinPt = 40.0
#process.generator.PGunParameters.MaxPt = 50.0
## # define limits for Pseudorapidity
#process.generator.PGunParameters.MinEta = -3
#process.generator.PGunParameters.MaxEta = 3

# select run & lumi section 
# 1st file 
process.source.firstRun = cms.untracked.uint32(1)
process.source.firstEvent = cms.untracked.uint32(1)
process.source.firstLuminosityBlock = cms.untracked.uint32(1)
# next files 
#process.source.firstRun = cms.untracked.uint32(2)
#process.source.firstEvent = cms.untracked.uint32(10001)
#process.source.firstLuminosityBlock = cms.untracked.uint32(1000)
#process.source.firstRun = cms.untracked.uint32(N)
#process.source.firstEvent = cms.untracked.uint32(N)
#process.source.firstLuminosityBlock = cms.untracked.uint32(N)


#process.MessageLogger = cms.Service("MessageLogger",
#    debugModules = cms.untracked.vstring('PixelDigisTest'),
#    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
#    cout = cms.untracked.PSet(
#        threshold = cms.untracked.string('ERROR')
#    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
#)

# Output definition

process.output = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string(
     #'simHits.root'
     '/afs/cern.ch/user/d/dkotlins/work/MC/mu_phase1/pt100_76/simhits/simHits1_eta1_novtx.root'
#     '/afs/cern.ch/user/d/dkotlins/work/MC/mu_phase1/pt100_76/simhits/simHits1_eta1.root'
#     '/afs/cern.ch/user/d/dkotlins/work/MC/mu_phase1/pt100_76/simhits/simHits1.root'
#     '/afs/cern.ch/user/d/dkotlins/work/MC/mu_phase1/pt100_76/simhits/simHits2.root'
    ),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)


###############################
# global output of simulation #
###############################
#
#process.o1 = cms.OutputModule(
#    "PoolOutputModule",
## definition of branches to keep or drop
#    outputCommands = cms.untracked.vstring('keep *',
#        'drop PCaloHits_*_*_*','drop *_*_MuonCSCHits_*',
#        'drop *_*_MuonDTHits_*','drop *_*_MuonRPCHits_*',
#        'drop *_*_MuonPLTHits_*','drop *_*_TotemHitsRP_*',
#        'drop *_*_TotemHitsT1_*','drop *_*_TotemHitsT2Gem_*',
#        'drop *_*_BCM1FHits_*','drop *_*_BSCHits_*',
#        'drop *_*_FP420SlHits_*','drop *_*_PLTHits_*',
#        'drop *_*_MuonGEMHits_*',
#    ),
# definition of output file (full path)
#    fileName = cms.untracked.string(
#      'simHits.root')
#      '/afs/cern.ch/user/d/dkotlins/work/MC/mu/pt100_72/simhits/simHits1.root')
#)
#


process.genstepfilter.triggerConditions=cms.vstring("generation_step")
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.output_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process
.simulation_step,process.endjob_step,process.output_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2017 

#call to customisation function cust_2017 imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2017(process)

# End of customisation functions

#process.outpath = cms.EndPath(process.o1)
#process.p = cms.Path(process.generator*process.VtxSmeared*process.genParticles*process.psim)
#process.schedule = cms.Schedule(process.p,process.outpath)
