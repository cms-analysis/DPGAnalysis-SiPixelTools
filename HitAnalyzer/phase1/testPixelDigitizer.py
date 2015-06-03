##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')

#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

process.load("Configuration.StandardSequences.Services_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

# from v7
#process.load("SimGeneral.MixingModule.pixelDigitizer_cfi")
# process.load("SimTracker.Configuration.SimTracker_cff")
process.load("SimG4Core.Configuration.SimG4Core_cff")

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
       'file:/afs/cern.ch/work/d/dkotlins/public//MC/mu_phase1/pt100_75/simhits/simHits1.root'
#       'file:simHits.root'
  )
)


process.o1 = cms.OutputModule("PoolOutputModule",
            outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Test'),
      fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_75/digis/digis1.root')
#      fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mb/13tev/digis/digis1.root')
#      fileName = cms.untracked.string('file:digis.root')
)


# add sqlite DBs
#LA
useLocalLA = True
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
useLocalQuality = True
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

# Redefine parameters 
process.g4SimHits.Generator.HepMCProductLabel = 'source'

# modify digitizer parameters
# with DB
# disable dyn. ineff.
process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
#
# NO DB case 
# use the simple linear response 
#process.simSiPixelDigis.digitizers.pixel.MissCalibrate = cms.bool(False) 
##process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_BPix = 3500.0 
#process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
# use inefficiency from DB Gain calibration payload? NO
#process.simSiPixelDigis.digitizers.pixel.useDB = cms.bool(False) 
# do not kill modules 
#process.simSiPixelDigis.digitizers.pixel.killModules = cms.bool(False) 
#process.simSiPixelDigis.digitizers.pixel.DeadModules_DB = cms.bool(False) 
# use LA from file 
#process.simSiPixelDigis.digitizers.pixel.LorentzAngle_DB = cms.bool(False)
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla_BPix = 0.10 
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla_FPix = 0.06 

# signal phase1 detector
process.simSiPixelDigis.digitizers.pixel.NumPixelBarrel = 4 
process.simSiPixelDigis.digitizers.pixel.NumPixelEndcap = 3 


#This process is to run the digitizer, pixel gitizer is now clled by the mix module
process.p1 = cms.Path(process.simSiPixelDigis)

process.outpath = cms.EndPath(process.o1)


