##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("Digitize")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource", 
  fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu_phase1/pt100_76/simhits/simHits1.root'
#    'file:simHits_100eve.root'
#    'file:sims.root'
  ),
  dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)


# add sqlite DBs
#LA
useLocalLA = False
if useLocalLA :
  process.LAReader = cms.ESSource("PoolDBESSource",
    DBParameters = 
      cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('')
      ),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string("SiPixelLorentzAngleSimRcd"),
        tag = cms.string("SiPixelLorentzAngleSim_phase1_mc_v1")
      ),
    ),
    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngleSim_phase1_mc_v1.db')
  ) # end process
  process.es_prefer_DBReader = cms.ESPrefer("PoolDBESSource","LAReader")
#  end if

# Quality
useLocalQuality = False
if useLocalQuality :
  process.QualityReader = cms.ESSource("PoolDBESSource",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('SiPixelQuality_phase1_ideal')
    )),
    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelQuality_phase1_ideal.db')
  )
  process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")
#  end if

#very strange effect, if I use 
#process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
# the output is 50% bigger than with this
process.output = cms.OutputModule("PoolOutputModule",
     dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(1048576),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    #outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Test'),
    splitLevel = cms.untracked.int32(0),
#    fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/digis/digis1.root')
    fileName = cms.untracked.string('file:digis.root')

)

# Other statements
process.mix.digitizers = cms.PSet(process.theDigitizersValid)

# modify digitizer parameters
#process.simSiPixelDigis.pixel.ThresholdInElectrons_BPix = 3500.0 
# disable dyn. ineff.
process.mix.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.output_step = cms.EndPath(process.output)
#process.output_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
#process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.endjob_step,process.output_step)
process.schedule = cms.Schedule(process.digitisation_step,process.endjob_step,process.output_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2017 

#call to customisation function cust_2017 imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2017(process)

# End of customisation functions

#process.g4SimHits.Generator.HepMCProductLabel = 'source'
#This process is to run the digitizer, pixel gitizer is now clled by the mix module
#process.p1 = cms.Path(process.simSiPixelDigis)
#process.outpath = cms.EndPath(process.o1)


