import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# DQM services
process.load("DQMServices.Core.DQM_cfg")

# Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

#fake conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
#process.GlobalTag.connect ="sqlite_file:/afs/cern.ch/user/m/malgeri/public/globtag/CRZT210_V1.db"
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRAFT_ALL_V9::All"
process.GlobalTag.globaltag = "COSMMC_22X_V1::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")




##
## Load and Configure TrackRefitter
##
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
#process.TrackRefitterP5.src = 'cosmictrackfinderP5'
process.TrackRefitterP5.src = 'cosmictrackfinderP5'
#'ALCARECOTkAlCosmicsCosmicTF0T'
process.TrackRefitterP5.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")


##
## Load and Configure OfflineValidation
##
process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")


process.load("DPGAnalysis.SiPixelTools.PixelNtuplizer_RealData_cfi")
process.PixelNtuplizer_RealData.OutputFile = 'COSMMC_22X_SuperPointing_ntupl_2.root'
process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'


process.source = cms.Source("PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_1.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_10.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_100.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_101.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_11.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_12.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_13.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_14.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_15.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_16.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_17.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_18.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_19.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_2.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_20.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_21.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_22.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_23.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_24.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_25.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_26.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_27.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_28.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_29.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_3.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_30.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_31.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_32.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_33.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_34.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_35.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_36.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_37.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_38.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_39.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_4.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_40.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_41.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_42.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_43.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_44.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_45.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_46.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_47.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_48.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_49.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_5.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_50.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_51.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_53.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_54.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_55.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_56.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_57.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_58.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_59.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_6.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_60.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_61.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_62.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_63.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_64.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_65.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_66.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_67.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_68.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_69.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_7.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_70.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_71.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_72.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_73.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_74.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_75.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_76.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_77.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_78.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_8.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_80.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_81.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_82.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_83.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_84.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_85.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_86.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_87.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_88.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_89.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_9.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_90.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_91.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_92.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_93.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_94.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_95.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_96.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_97.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_98.root',
'rfio:/castor/cern.ch/user/v/vesna/REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_99.root'


   
    ) )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.PixelNtuplizer_RealData)
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'Info'
process.TrackerDigiGeometryESModule.applyAlignment = True
