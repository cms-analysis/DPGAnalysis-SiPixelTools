import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# DQM services
process.load("DQMServices.Core.DQM_cfg")

# Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "COSMMC_22X_V1::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0

<<<<<<< pixelNtuplizer_MC_cfg38T.py
# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

=======
# reconstruction sequence for Cosmics
# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

>>>>>>> 1.4

##
## Load and Configure TrackRefitter
##
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
# tracks available in cosmic reco: 'cosmictrackfinderP5', 'ctfWithMaterialTracksP5', 'rsWithMaterialTracksP5'
#process.TrackRefitterP5.src =  'cosmictrackfinderP5' #'ctfWithMaterialTracksP5'
# AlCa tracks:
#processlTrackRefitterP5.src = 'ALCARECOTkAlCosmicsCosmicTF0T'
#process.TrackRefitterP5.TrajectoryInEvent = True

# Instead, load all three track algorithms:
process.ctfRefitter = process.TrackRefitterP5.clone()
process.ctfRefitter.src = 'ctfWithMaterialTracksP5'
process.ctfRefitter.TrajectoryInEvent = True
process.rsRefitter = process.TrackRefitterP5.clone()
process.rsRefitter.src = 'rsWithMaterialTracksP5'
process.rsRefitter.TrajectoryInEvent = True
process.cosmRefitter = process.TrackRefitterP5.clone()
process.cosmRefitter.src = 'cosmictrackfinderP5'
process.cosmRefitter.TrajectoryInEvent = True


process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

##
## Load and Configure OfflineValidation
##
process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")

##
## Pixel event filters:
##

process.load("DPGAnalysis.SiPixelTools.muonTOF_cfi")
process.load("DPGAnalysis.SiPixelTools.FEDInRunFilter_cfi")
process.MuonTOFFilter_trackQuality.max_goodmuons = 2
process.MuonTOFFilter_trackQuality.max_timeError = 15
process.MuonTOFFilter_trackQuality.max_chi2_ndof = 15


##
##  Ntuplizer
##

process.load("DPGAnalysis.SiPixelTools.PixelNtuplizer_RealData_cfi")
<<<<<<< pixelNtuplizer_MC_cfg38T.py
process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'
=======
# also run 3 times:
process.ctfNtuple = process.PixelNtuplizer_RealData.clone()
process.ctfNtuple.trajectoryInput = 'ctfRefitter'

process.rsNtuple = process.PixelNtuplizer_RealData.clone()
process.rsNtuple.trajectoryInput = 'rsRefitter'
>>>>>>> 1.4

process.cosmtfNtuple = process.PixelNtuplizer_RealData.clone()
process.cosmtfNtuple.trajectoryInput = 'cosmRefitter'

<<<<<<< pixelNtuplizer_MC_cfg38T.py

##
## configure output ntuple file using TFileService
##

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("Reprocessed_SuperPointingMC_COSMMC_22X_V1_1_ntupl.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )




=======
#process.load("DPGAnalysis.SiPixelTools.PixelNtuplizer_RealData_cfi")
#process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'


##
## configure output ntuple file using TFileService
##

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("SuperPointing_test_threealgos_MC.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

##
## Configure input file
##
>>>>>>> 1.4
process.source = cms.Source("PoolSource",
<<<<<<< pixelNtuplizer_MC_cfg38T.py
      fileNames = cms.untracked.vstring(
     '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0006/26B79166-7C00-DE11-AA81-0030486790A0.root',
     'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_1.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_10.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_100.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_101.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_11.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_12.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_13.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_14.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_15.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_16.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_17.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_18.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_19.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_2.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_20.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_21.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_22.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_23.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_24.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_25.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_26.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_27.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_28.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_29.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_3.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_31.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_32.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_33.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_34.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_35.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_36.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_37.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_38.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_39.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_4.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_40.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_41.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_42.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_43.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_44.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_45.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_46.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_47.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_48.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_49.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_5.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_50.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_51.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_52.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_53.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_54.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_55.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_56.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_57.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_58.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_59.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_6.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_60.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_61.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_62.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_63.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_64.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_65.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_66.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_67.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_68.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_69.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_7.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_70.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_71.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_72.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_73.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_74.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_75.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_76.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_77.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_78.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_79.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_8.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_80.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_81.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_82.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_83.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_84.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_85.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_86.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_87.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_89.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_9.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_90.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_91.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_92.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_93.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_94.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_95.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_96.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_97.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_98.root',
'rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_99.root'

    

    ) )

=======
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
    "rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_1.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_10.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_100.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_101.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_11.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_12.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_13.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_14.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_15.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_16.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_17.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_18.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_19.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_2.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_20.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_21.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_22.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_23.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_24.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_25.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_26.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_27.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_28.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_29.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_3.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_31.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_32.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_33.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_34.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_35.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_36.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_37.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_38.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_39.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_4.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_40.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_41.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_42.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_43.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_44.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_45.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_46.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_47.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_48.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_49.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_5.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_50.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_51.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_52.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_53.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_54.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_55.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_56.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_57.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_58.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_59.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_6.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_60.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_61.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_62.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_63.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_64.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_65.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_66.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_67.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_68.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_69.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_7.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_70.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_71.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_72.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_73.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_74.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_75.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_76.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_77.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_78.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_79.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_8.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_80.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_81.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_82.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_83.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_84.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_85.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_86.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_87.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_89.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_9.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_90.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_91.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_92.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_93.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_94.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_95.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_96.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_97.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_98.root",
"rfio:/castor/cern.ch/user/v/vesna/NEW_REPROCESSING/Reprocessed_SuperPointingMC_COSMMC_22X_V1_99.root"

   
    )

                            )

# these drop commands are necessary to get rid of all HLT problems and DQM bulk
process.source.inputCommand = cms.untracked.vstring("drop *_*_*_FU"
                                                    ,"drop *_*_*_HLT",
                                                    "drop *_MEtoEDMConverter_*_*","drop *_lumiProducer_*_REPACKER"
                                                    )
##
## number of events
##
>>>>>>> 1.4
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

##
## executionpath
##
process.p = cms.Path(
    # filters:
#    process.fedInRunFilter*
    # standard reco sequence
#    process.RawToDigi*process.reconstructionCosmics*
    # more filters:
    process.MuonTOFFilter_trackQuality *
    # create rechits
    # (rechits are transient data members so are not saved in CMSSW .root files)
    process.offlineBeamSpot*
#   refitters for all tracking algos (thse are what actually create the rechits)
    process.ctfRefitter*
    process.rsRefitter*
    process.cosmRefitter*
    # run ntuplizers
    process.ctfNtuple*
    process.rsNtuple*
    process.cosmtfNtuple
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'Info'
process.TrackerDigiGeometryESModule.applyAlignment = True
