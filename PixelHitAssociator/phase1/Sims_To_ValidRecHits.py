##############################################################################
# start from simhits, do trackerlocal, compare sim and rec-hits
import FWCore.ParameterSet.Config as cms
process = cms.Process("TestValid")

# process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.Geometry.GeometryDB_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')

#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")

##process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
##process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('SimGeneral.MixingModule.mixNoPU_cfi')
# process.load("SimTracker.Configuration.SimTracker_cff")

process.load("SimG4Core.Configuration.SimG4Core_cff") # needed

# for strips 
#process.load("CalibTracker.SiStripESProducers.SiStripGainSimESProducer_cfi")

# clusterizers & rhs 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff") # needed

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '76X_upgrade2017_design_v8', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# mix replaced by the code below
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


process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/simhits/simHits1_eta1.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/simhits/simHits1.root',
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_72/simhits/simHits1.root',
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_72/simhits/simHits3.root',
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_72/simhits/simHits4.root',
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



process.o1 = cms.OutputModule("PoolOutputModule",
      outputCommands = cms.untracked.vstring('drop *','keep *_*_*_TestValid'),
#      outputCommands = cms.untracked.vstring('keep *_*_*_*'),
      fileName = cms.untracked.string('file:dummy.root')
)


# DB stuff 
# for digitisation

#LA
useLocalLASim = True
if useLocalLASim :
  process.LASimReader = cms.ESSource("PoolDBESSource",
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
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngleSim_phase1_mc_v1.db')
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  ) # end process
  process.lasimprefer = cms.ESPrefer("PoolDBESSource","LASimReader")
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
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelQuality_phase1_ideal.db')
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  )
  process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")
#  end if

# for reco 
# LA 
useLocalLA = True
if useLocalLA :
    process.LAReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
# LA
	record = cms.string("SiPixelLorentzAngleRcd"),
        tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
# cabling
#           record = cms.string('SiPixelFedCablingMapRcd'),
#           tag = cms.string('SiPixelFedCablingMap_v17')
# GenError
#          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
#          tag = cms.string('SiPixelGenErrorDBObject38T_v0_mc1')
#          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
 		),
 	),
     #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    ) # end process

    process.LAprefer = cms.ESPrefer("PoolDBESSource","LAReader")

    # now the forWidth LA
    process.LAWidthReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
	record = cms.string("SiPixelLorentzAngleRcd"),
        label = cms.untracked.string("forWidth"),
        tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
       ),
     ),
     #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')

    ) # end process

    process.LAWidthprefer = cms.ESPrefer("PoolDBESSource","LAWidthReader")

# endif

# Gain 
useLocalGain = True
if useLocalGain :
  process.GainsReader = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0),
       authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
        #tag = cms.string('SiPixelGainCalibration_phase1_ideal')
        #tag = cms.string('SiPixelGainCalibration_phase1_mc_v1')
        tag = cms.string('SiPixelGainCalibration_phase1_mc_v2')
        #tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
    )),
    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_mc_v2.db')
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_ideal_v2.db')
    #connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  ) # end process
  process.Gainprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if


# GenError
useLocalGenErr = True
if useLocalGenErr :
    process.GenErrReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
          record = cms.string('SiPixelGenErrorDBObjectRcd'),
          tag = cms.string('SiPixelGenErrorDBObject_phase1_38T_mc_v1')
 	 ),
 	),
#     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    ) # end process
    process.generrprefer = cms.ESPrefer("PoolDBESSource","GenErrReader")
# endif


process.g4SimHits.Generator.HepMCProductLabel = 'source'

# for direct digis
process.siPixelClustersPreSplitting.src = 'simSiPixelDigis' # for V5, direct
# process.siPixelClusters.src = 'mix'
# modify digitizer parameters
process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_BPix = 2000.0 
process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_BPix_L1 = 2000.0 
process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_FPix = 2000.0 

process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiency = cms.bool(False)
#
# tell the digitize it is phase1
process.simSiPixelDigis.digitizers.pixel.NumPixelBarrel = 4 
process.simSiPixelDigis.digitizers.pixel.NumPixelEndcap = 3 
process.simSiPixelDigis.digitizers.pixel.thePixelColEfficiency_BPix1 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelColEfficiency_BPix2 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelColEfficiency_BPix3 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelColEfficiency_BPix4 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelEfficiency_BPix1 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelEfficiency_BPix2 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelEfficiency_BPix3 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelEfficiency_BPix4 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelChipEfficiency_BPix1 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelChipEfficiency_BPix2 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelChipEfficiency_BPix3 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelChipEfficiency_BPix4 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelColEfficiency_FPix1 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelColEfficiency_FPix2 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelColEfficiency_FPix3 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelEfficiency_FPix1 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelEfficiency_FPix2 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelEfficiency_FPix3 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelChipEfficiency_FPix1 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelChipEfficiency_FPix2 = cms.double(1.0)
process.simSiPixelDigis.digitizers.pixel.thePixelChipEfficiency_FPix3 = cms.double(1.0)

# gain parameters
process.simSiPixelDigis.digitizers.pixel.FPix_SignalResponse_p0 = cms.double(0.00174)
process.simSiPixelDigis.digitizers.pixel.FPix_SignalResponse_p1 = cms.double(0.711)
process.simSiPixelDigis.digitizers.pixel.FPix_SignalResponse_p2 = cms.double(203.)
process.simSiPixelDigis.digitizers.pixel.FPix_SignalResponse_p3 = cms.double(148.)
process.simSiPixelDigis.digitizers.pixel.BPix_SignalResponse_p0 = cms.double(0.00174)
process.simSiPixelDigis.digitizers.pixel.BPix_SignalResponse_p1 = cms.double(0.711)
process.simSiPixelDigis.digitizers.pixel.BPix_SignalResponse_p2 = cms.double(203.)
process.simSiPixelDigis.digitizers.pixel.BPix_SignalResponse_p3 = cms.double(148.)

# use inefficiency from DB Gain calibration payload? OFF BY DEFAULT
#process.simSiPixelDigis.digitizers.pixel.useDB = cms.bool(False) 
# use LA from file 
#process.simSiPixelDigis.digitizers.pixel.LorentzAngle_DB = cms.bool(False)
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla_BPix = 0.106 
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla=FPix = 0.106 

# clus , label of digis 
process.siPixelClustersPreSplitting.src = 'simSiPixelDigis'
#process.siPixelClustersPreSplitting.ClusterThreshold = 4000.
# set to false to ignore the gain calibration
#process.siPixelClustersPreSplitting.MissCalibrate = cms.untracked.bool(False)
# rechits
# force generic CPE to skip GenErrors. For parameters used by the producer us the producer process
#process.PixelCPEGenericESProducer.UseErrorsFromTemplates = cms.bool(False)
#process.PixelCPEGenericESProducer.LoadTemplatesFromDB = cms.bool(False)
#process.PixelCPEGenericESProducer.TruncatePixelCharge = cms.bool(False)

#process.PixelCPEGenericESProducer.useLAAlignmentOffsets = cms.bool(True)
#process.PixelCPEGenericESProducer.useLAWidthFromDB = cms.bool(True)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.098)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.09)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.double(0.098)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.double(0.058)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.double(0.0)

# to run rechit "official" validation
#process.load("Validation.TrackerRecHits.trackerRecHitsValidation_cff")
#process.load("Validation.TrackerRecHits.SiPixelRecHitsValid_cfi")

# my rec-sim hit compare 
process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.SiPixelRecHitsValid_cff")

process.pixRecHitsValid.outputFile="pixelsimrechitshistos.root"
#process.pixRecHitsValid.verbose=True
process.pixRecHitsValid.src="siPixelRecHitsPreSplitting"
#process.pixRecHitsValid.associatePixel = True
#process.pixRecHitsValid.associateStrip = False
#process.pixRecHitsValid.associateRecoTracks = False

#This process is to run the digitizer:
#process.p1 = cms.Path(process.simSiPixelDigis)
#process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco)
process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco*process.pixRecHitsValid)

# for RelVal GEN-SIM-DIGI-RAW-HLTDEBUG
#process.p1 = cms.Path(process.pixeltrackerlocalreco*process.pixRecHitsValid)
#process.outpath = cms.EndPath(process.o1)
