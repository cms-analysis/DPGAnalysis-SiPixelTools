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
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# process.load("SimTracker.Configuration.SimTracker_cff")
process.load("SimG4Core.Configuration.SimG4Core_cff")

# for strips 
process.load("CalibTracker.SiStripESProducers.SiStripGainSimESProducer_cfi")

# clusterizers & rhs 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
  #'file:../../HitAnalyzer/phase1/digis.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/digis/digis1.root',
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
useLocalLA = True
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
        tag = cms.string('SiPixelGainCalibration_phase1_mc_v1')
    )),
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_ideal.db')
     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
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

# my rec-sim hit compare 
process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.SiPixelRecHitsValid_cff")

#process.pixRecHitsValid.outputFile="pixelrechitshisto.root"
#process.pixRecHitsValid.verbose=True
process.pixRecHitsValid.src="siPixelRecHitsPreSplitting"
#process.pixRecHitsValid.associatePixel = True
#process.pixRecHitsValid.associateStrip = False
#process.pixRecHitsValid.associateRecoTracks = False

#This process is to run the digitizer:
#process.p1 = cms.Path(process.simSiPixelDigis)
#process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco)
#process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco*process.pixRecHitsValid)

# does not work with my digis
process.p1 = cms.Path(process.pixeltrackerlocalreco*process.pixRecHitsValid)

# for RelVal GEN-SIM-DIGI-RAW-HLTDEBUG
#process.p1 = cms.Path(process.pixeltrackerlocalreco*process.pixRecHitsValid)

#process.outpath = cms.EndPath(process.o1)


