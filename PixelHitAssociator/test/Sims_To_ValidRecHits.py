##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("TestValid")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
# process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# process.load("SimTracker.Configuration.SimTracker_cff")
process.load("SimG4Core.Configuration.SimG4Core_cff")

# for strips 
process.load("CalibTracker.SiStripESProducers.SiStripGainSimESProducer_cfi")

# clusterizers & rhs 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff")


# process.load("SimGeneral.MixingModule.mixNoPU_cfi")

from SimGeneral.MixingModule.aliases_cfi import * 
from SimGeneral.MixingModule.mixObjects_cfi import *
# from SimGeneral.MixingModule.digitizers_cfi import *
from SimGeneral.MixingModule.pixelDigitizer_cfi import *
from SimGeneral.MixingModule.stripDigitizer_cfi import *
from SimGeneral.MixingModule.trackingTruthProducer_cfi import *


process.simSiPixelDigis = cms.EDProducer("MixingModule",
#    digitizers = cms.PSet(theDigitizers),
#    digitizers = cms.PSet(
#      mergedtruth = cms.PSet(
#            trackingParticles
#      )
#    ),

  digitizers = cms.PSet(
   pixel = cms.PSet(
    pixelDigitizer
   ),
#  strip = cms.PSet(
#    stripDigitizer
#  ),
  ),

#theDigitizersValid = cms.PSet(
#  pixel = cms.PSet(
#    pixelDigitizer
#  ),
#  strip = cms.PSet(
#    stripDigitizer
#  ),
#  ecal = cms.PSet(
#    ecalDigitizer
#  ),
#  hcal = cms.PSet(
#    hcalDigitizer
#  ),
#  castor = cms.PSet(
#    castorDigitizer
#  ),
#  mergedtruth = cms.PSet(
#    trackingParticles
#  )
#),


    LabelPlayback = cms.string(' '),
    maxBunch = cms.int32(3),
    minBunch = cms.int32(-5), ## in terms of 25 ns

    bunchspace = cms.int32(25),
    mixProdStep1 = cms.bool(False),
    mixProdStep2 = cms.bool(False),

    playback = cms.untracked.bool(False),
    useCurrentProcessOnly = cms.bool(False),
    mixObjects = cms.PSet(
        mixTracks = cms.PSet(
            mixSimTracks
        ),
        mixVertices = cms.PSet(
            mixSimVertices
        ),
        mixSH = cms.PSet(
#            mixPixSimHits
# mixPixSimHits = cms.PSet(
    input = cms.VInputTag(cms.InputTag("g4SimHits","TrackerHitsPixelBarrelHighTof"), 
                          cms.InputTag("g4SimHits","TrackerHitsPixelBarrelLowTof"),
                          cms.InputTag("g4SimHits","TrackerHitsPixelEndcapHighTof"), 
                          cms.InputTag("g4SimHits","TrackerHitsPixelEndcapLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTECHighTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTECLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTIBHighTof"),
#                          cms.InputTag("g4SimHits","TrackerHitsTIBLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTIDHighTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTIDLowTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTOBHighTof"), 
#                          cms.InputTag("g4SimHits","TrackerHitsTOBLowTof")
    ),
    type = cms.string('PSimHit'),
    subdets = cms.vstring(
        'TrackerHitsPixelBarrelHighTof',
        'TrackerHitsPixelBarrelLowTof',
        'TrackerHitsPixelEndcapHighTof',
        'TrackerHitsPixelEndcapLowTof',
#        'TrackerHitsTECHighTof',
#        'TrackerHitsTECLowTof',
#        'TrackerHitsTIBHighTof',
#        'TrackerHitsTIBLowTof',
#        'TrackerHitsTIDHighTof',
#        'TrackerHitsTIDLowTof',
#        'TrackerHitsTOBHighTof',
#        'TrackerHitsTOBLowTof'
    ),
    crossingFrames = cms.untracked.vstring(),
#        'MuonCSCHits',
#        'MuonDTHits',
#        'MuonRPCHits'),
#)   
        ),
        mixHepMC = cms.PSet(
            mixHepMCProducts
        )
    )
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#     simMuonCSCDigis = cms.PSet(
#        initialSeed = cms.untracked.uint32(1234567),
#        engineName = cms.untracked.string('TRandom3')
#    ),
     simSiPixelDigis = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('TRandom3')
   )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre5/simhits/simHits2.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre5/simhits/simHits1.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre5/simhits/simHits3.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre5/simhits/simHits4.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre5/simhits/simHits5.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre5/simhits/simHits6.root',
# gen-sim
# '/store/relval/CMSSW_7_0_0_pre8/RelValSingleMuPt100/GEN-SIM/START70_V2_RR-v7/00000/B464EA42-2B59-E311-A2C1-0025905964C2.root',
# '/store/relval/CMSSW_7_0_0_pre10/RelValSingleMuPt100/GEN-SIM-RECO/START70_V3-v1/00000/908DD48F-1466-E311-BEBE-0025905A48F0.root',
# '/store/relval/CMSSW_7_0_0_pre10/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/START70_V3-v1/00000/EAA29CCF-8965-E311-A43D-0025905A6084.root',
  )
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('pixRecHitsValid'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
#        threshold = cms.untracked.string('DEBUG')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

# Choose the global tag here:
# for v7.0
#process.GlobalTag.globaltag = "MC_70_V1::All"
#process.GlobalTag.globaltag = "MC_71_V1::All"

#process.GlobalTag.globaltag = "PRE_MC_71_V2::All"
#process.GlobalTag.globaltag = "PRE_STA71_V3::All"

#process.GlobalTag.globaltag = "START70_V1::All"
#process.GlobalTag.globaltag = "START71_V1::All"
process.GlobalTag.globaltag = "START72_V3::All"


#process.GlobalTag.globaltag = "POSTLS170_V4::All"
#process.GlobalTag.globaltag = "POSTLS171_V1::All"
#process.GlobalTag.globaltag = "PRE_LS171_V3::All"



process.o1 = cms.OutputModule("PoolOutputModule",
      outputCommands = cms.untracked.vstring('drop *','keep *_*_*_TestValid'),
#      outputCommands = cms.untracked.vstring('keep *_*_*_*'),
      fileName = cms.untracked.string('file:dummy.root')
)


# DB stuff 
# GenError
useLocalDB = False
if useLocalDB :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
 	 ),
 	),
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# endif


process.g4SimHits.Generator.HepMCProductLabel = 'source'

# for direct digis
process.siPixelClusters.src = 'simSiPixelDigis' # for V5, direct
# process.siPixelClusters.src = 'mix'
# modify digitizer parameters
#process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_BPix = 3500.0 
process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
# use inefficiency from DB Gain calibration payload?
process.simSiPixelDigis.digitizers.pixel.useDB = cms.bool(False) 
# use LA from file 
#process.simSiPixelDigis.digitizers.pixel.LorentzAngle_DB = cms.bool(False)
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla_BPix = 0.106 
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla=FPix = 0.106 

# to run rechit validation
#process.load("Validation.TrackerRecHits.trackerRecHitsValidation_cff")
#process.load("Validation.TrackerRecHits.SiPixelRecHitsValid_cfi")

# my rec-sim hit compare 
process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.SiPixelRecHitsValid_cff")

#process.pixRecHitsValid.outputFile="pixelrechitshisto.root"
#process.pixRecHitsValid.verbose=True
#process.pixRecHitsValid.src="siPixelRecHits"
#process.pixRecHitsValid.associatePixel = True
#process.pixRecHitsValid.associateStrip = False
#process.pixRecHitsValid.associateRecoTracks = False

#This process is to run the digitizer:
#process.p1 = cms.Path(process.mix)
#process.p1 = cms.Path(process.mix*process.pixeltrackerlocalreco)
process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco*process.pixRecHitsValid)

# for RelVal GEN-SIM-DIGI-RAW-HLTDEBUG
#process.p1 = cms.Path(process.pixeltrackerlocalreco*process.pixRecHitsValid)

#process.outpath = cms.EndPath(process.o1)


