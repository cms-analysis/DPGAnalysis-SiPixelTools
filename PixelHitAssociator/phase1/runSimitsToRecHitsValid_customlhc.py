##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("validate")

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

# clusterizers & rhs 
#process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff") # needed



process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

process.MessageLogger = cms.Service("MessageLogger",
    #debugModules = cms.untracked.vstring('Digitize'),
    debugModules = cms.untracked.vstring('PixelDigitizer'),
    destinations = cms.untracked.vstring('cout'),
    #destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    )
    #log = cms.untracked.PSet(
    #    threshold = cms.untracked.string('DEBUG')
    #)
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
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
    connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS') 
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
    connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
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
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
#     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    ) # end process
    process.generrprefer = cms.ESPrefer("PoolDBESSource","GenErrReader")
# endif




#very strange effect, if I use 
process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
# the output is 50% bigger than with this
#process.output = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(1048576),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    #outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Test'),
    splitLevel = cms.untracked.int32(0),
    fileName = cms.untracked.string('file:tmp.root')
)

# Other statements
process.mix.digitizers = cms.PSet(process.theDigitizersValid)

# modify digitizer parameters
#process.simSiPixelDigis.pixel.ThresholdInElectrons_BPix = 3500.0 
# disable dyn. ineff.
process.mix.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
#process.mix.digitizers.pixel.NumPixelBarrel = cms.int32(4)
#process.mix.digitizers.pixel.NumPixelEndcap = cms.int32(3)


# to run rechit "official" validation
#process.load("Validation.TrackerRecHits.trackerRecHitsValidation_cff")
#process.load("Validation.TrackerRecHits.SiPixelRecHitsValid_cfi")

# my rec-sim hit compare 
process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.SiPixelRecHitsValid_cff")

process.pixRecHitsValid.outputFile="pixelsimrechitshistos.root"
process.pixRecHitsValid.verbose=False
#process.pixRecHitsValid.src="siPixelRecHitsPreSplitting"
process.pixRecHitsValid.src="siPixelRecHits"
#process.pixRecHitsValid.associatePixel = True
#process.pixRecHitsValid.associateStrip = False
#process.pixRecHitsValid.associateRecoTracks = False


# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.pixeltrackerlocalreco_step = cms.Path(process.pixeltrackerlocalreco)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.valid_step = cms.Path(process.pixRecHitsValid)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.output_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
#process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.endjob_step,process.output_step)
#process.schedule = cms.Schedule(process.digitisation_step,process.endjob_step,process.output_step)
# if the enjob is there histos are NOT stored
#process.schedule = cms.Schedule(process.digitisation_step,process.pixeltrackerlocalreco_step,process.valid_step,process.endjob_step)
# get rid of endjob to have the histos
process.schedule = cms.Schedule(process.digitisation_step,process.pixeltrackerlocalreco_step,process.valid_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2017 
#from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 
#from SLHCUpgradeSimulations.Configuration.customise_mixing import customise_pixelMixing_PU
#from SLHCUpgradeSimulations.Configuration.customise_mixing import customise_NoCrossing_PU
#call to customisation function cust_2017 imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2017(process)
#process = customisePostLS1(process)
#process=customise_pixelMixing_PU(process)
#process=customise_NoCrossing_PU(process)

# End of customisation functions

#process.g4SimHits.Generator.HepMCProductLabel = 'source'
#This process is to run the digitizer, pixel gitizer is now clled by the mix module
#process.p1 = cms.Path(process.simSiPixelDigis)
#process.outpath = cms.EndPath(process.o1)


