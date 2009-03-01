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
process.GlobalTag.globaltag = "COSMMC_21X_V1::All"
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
process.PixelNtuplizer_RealData.OutputFile = 'Summer08_COSMMC_21X_V1_SuperPointing_v4_ntupl.root'
process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'


process.source = cms.Source("PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(

'/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/0E025CF5-38F9-DD11-92D0-001D0967D526.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/16305D0F-3DF9-DD11-A19B-001D0967D639.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/1EC6EB8A-35F9-DD11-881C-001D0967DE9F.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/4813F51E-3FF9-DD11-A6DE-0019B9E4AA47.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/5EA6E6A5-3BF9-DD11-9A6F-001D096B0E2C.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/6E877998-38F9-DD11-AE1E-0019B9E7CC47.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/A0E470C4-35F9-DD11-A3D7-0019B9E8B5DD.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/AA21A54B-33F9-DD11-A0D4-001D0967D689.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/C822973C-59F9-DD11-8B2A-001D0967D77E.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/DEE328AB-3BF9-DD11-A54C-0019B9E4FCA8.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/F4069DF4-3AF9-DD11-A8DF-001D0967D16B.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/02663257-53F9-DD11-9538-0019B9E7E112.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/0A4422A9-35F9-DD11-8BF7-0015178C4864.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/407FE63F-32F9-DD11-BB27-0019B9E4B150.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/66B49B09-4EF9-DD11-B721-001D0967D1E3.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/74AC0503-49F9-DD11-8B79-001D0967DCBA.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/76B16851-59F9-DD11-BDE5-0019B9E4FD75.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/905C6671-53F9-DD11-BE4A-001D0967CFB8.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/92053547-59F9-DD11-B8B4-0019B9E7133E.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0001/98554428-3BF9-DD11-9118-0015178C4D7C.root'
   
    ) )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.PixelNtuplizer_RealData)
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'Info'
process.TrackerDigiGeometryESModule.applyAlignment = True
