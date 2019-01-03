##############################################################################
# start from simhits, do trackerlocal, compare sim and rec-hits
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("TestValid",eras.Run2_2017)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')

# process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.Geometry.GeometryDB_cff")
#process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# process.load("SimTracker.Configuration.SimTracker_cff")
process.load("SimG4Core.Configuration.SimG4Core_cff")


process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

# for strips 
process.load("CalibTracker.SiStripESProducers.SiStripGainSimESProducer_cfi")

# clusterizers & rhs 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# needed for pixel RecHits (templates?)
#process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco1.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw1.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw2.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw3.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw4.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw5.root',
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
# cabling
#           record = cms.string('SiPixelFedCablingMapRcd'),
#           tag = cms.string('SiPixelFedCablingMap_v17')
# for reco 
# LA 
useLocalLA = False
if useLocalLA :
    process.LAReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
	  record = cms.string("SiPixelLorentzAngleRcd"),
          tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
 		),
 	),
      #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
      connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
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
        tag = cms.string("SiPixelLorentzAngle_forWidth_phase1_mc_v1")
       ),
     ),
     #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_forWidth_phase1_mc_v1.db')
     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process

    process.LAWidthprefer = cms.ESPrefer("PoolDBESSource","LAWidthReader")

# endif

# Gain 
useLocalGain = False
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
        tag = cms.string('SiPixelGainCalibration_phase1_mc_v1')
    )),
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_ideal.db')
     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
  ) # end process
  process.Gainprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if


# GenError
useLocalGenErr = False
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
     #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGenErrorDBObject_phase1_38T_mc_v1.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.generrprefer = cms.ESPrefer("PoolDBESSource","GenErrReader")
# endif


process.g4SimHits.Generator.HepMCProductLabel = 'source'

# for direct digis
process.siPixelClustersPreSplitting.src = 'simSiPixelDigis' # for V5, direct
# process.siPixelClusters.src = 'mix'


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

process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
# raw2digi
#process.siPixelDigis.InputLabel = 'siPixelRawData'
process.siPixelDigis.InputLabel = 'rawDataCollector'


# Refitter
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")


# my rec-sim hit compare 
#  process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.SiPixelRecHitsValid_cff")
process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.StudyRecHitResolution_cff")

# not on track 
#process.pixRecHitsValid.outputFile="pixelrechitshisto.root"
process.pixRecHitsValid.verbose=False 
process.pixRecHitsValid.src="siPixelRecHitsPreSplitting"
#process.pixRecHitsValid.src="siPixelRecHits"
#process.pixRecHitsValid.associatePixel = True
#process.pixRecHitsValid.associateStrip = False
#process.pixRecHitsValid.associateRecoTracks = False

# on track 
process.pixRecHitsValid.useTracks = True
process.pixRecHitsValid.tracks = 'TrackRefitter'

process.TrackRefitter.src = "generalTracks"
process.TrackRefitter.TTRHBuilder = 'WithAngleAndTemplate'
#process.TrackRefitter.TTRHBuilder = 'WithTrackAngle'


#This process is to run the digitizer:
#process.p1 = cms.Path(process.simSiPixelDigis)
#process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco)
#process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco*process.pixRecHitsValid)

# does not work with my digis
#process.myAnalyzer_step = cms.Path(process.pixRecHitsValid)

# on track 
#process.p1 = cms.Path(process.RawToDigi*process.reconstruction*process.MeasurementTrackerEvent*process.TrackRefitter*process.pixRecHitsValid)
process.p1 = cms.Path(process.RawToDigi*process.reconstruction*process.TrackRefitter*process.pixRecHitsValid)
# all rechits 
#process.p1 = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.pixRecHitsValid)
#process.p1 = cms.Path(process.pixRecHitsValid)

# for RelVal GEN-SIM-DIGI-RAW-HLTDEBUG
#process.p1 = cms.Path(process.pixeltrackerlocalreco*process.pixRecHitsValid)

#process.outpath = cms.EndPath(process.o1)


