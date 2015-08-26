#
import FWCore.ParameterSet.Config as cms

process = cms.Process("GenTest")

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.EventContent.EventContent_cff')

#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.GeometryIdeal_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')

process.load('Configuration.StandardSequences.Generator_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')

process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
 
process.load('GeneratorInterface.Core.genFilterSummary_cff')

process.load('Configuration.StandardSequences.SimIdeal_cff')
# process.load('Configuration/StandardSequences/Sim_cff')


###########
# what is this?
#process.load("SimTracker.Configuration.SimTracker_cff")

#process.load("Geometry.TrackerSimData.trackerSimGeometryXML_cfi")
#process.load("Geometry.CMSCommonData.cmsSimIdealGeometryXML_cfi")

#process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
#process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
#process.load("Configuration.StandardSequences.MagneticField_cff")
# include "MagneticField/Engine/data/volumeBasedMagneticField.cfi"

# GTs
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#process.GlobalTag.globaltag = 'MC_53_V15::All'
#process.GlobalTag.globaltag = 'DESIGN53_V15::All'
#process.GlobalTag.globaltag = 'START53_V15::All'
# ideal
#process.GlobalTag.globaltag = 'MC_72_V3::All'
#process.GlobalTag.globaltag = 'MC_72_V3'
# realistiv alignment and calibrations 
#process.GlobalTag.globaltag = 'START72_V3::All'
#process.GlobalTag.globaltag = 'DESIGN72_V5::All'

from Configuration.AlCa.autoCond_condDBv2 import autoCond
#process.GlobalTag.globaltag = autoCond['run2_design']
process.GlobalTag.globaltag = autoCond['run2_mc']

##############################
# magnetic field in solenoid #
##############################
#
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

#########################
# event vertex smearing #
#########################
#
#process.load("Configuration.StandardSequences.VtxSmearedGauss_cff")
#process.load("Configuration.StandardSequences.VtxSmearedBetafuncEarlyCollision_cff")
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
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
process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(100.1),
        MinPt = cms.double(99.9),
        PartID = cms.vint32(13,13,13,13),
        MaxEta = cms.double(3.0),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-3.0),
        MinPhi = cms.double(-3.14159265359) ## in radians
    ),
    Verbosity = cms.untracked.int32(1), ## set to 1 (or greater)  for printouts
    psethack = cms.string('4 muona at pt 100'),
    AddAntiParticle = cms.bool(False), # True makes two muons per event
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



###############################
# global output of simulation #
###############################
#
process.o1 = cms.OutputModule(
    "PoolOutputModule",
# definition of branches to keep or drop
    outputCommands = cms.untracked.vstring('keep *',
        'drop PCaloHits_*_*_*','drop *_*_MuonCSCHits_*',
        'drop *_*_MuonDTHits_*','drop *_*_MuonRPCHits_*',
        'drop *_*_MuonPLTHits_*','drop *_*_TotemHitsRP_*',
        'drop *_*_TotemHitsT1_*','drop *_*_TotemHitsT2Gem_*',
        'drop *_*_BCM1FHits_*','drop *_*_BSCHits_*',
        'drop *_*_FP420SlHits_*','drop *_*_PLTHits_*',
        'drop *_*_MuonGEMHits_*',
    ),
# definition of output file (full path)
    fileName = cms.untracked.string(
      'simHits1.root')
)
#
process.outpath = cms.EndPath(process.o1)

#process.p = cms.Path(process.generator*process.genParticles)
process.p = cms.Path(process.generator*process.VtxSmeared*process.genParticles*process.psim)

process.schedule = cms.Schedule(process.p,process.outpath)
