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
process.GlobalTag.globaltag = "CRAFT_ALL_V9::All"
#process.GlobalTag.globaltag = "COSMMC_21X_V1::All"
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
process.PixelNtuplizer_RealData.OutputFile = 'SuperPointing_AllCraft_V4_CosmicsCosmicTF0TTTree_dttof_MC.root'
process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'


process.source = cms.Source("PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
'/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V9_225_ReReco_FromTrackerPointing_v1/0000/0E025CF5-38F9-DD11-92D0-001D0967D526.root'

   
    ) )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

process.p = cms.Path(process.siPixelClusters*process.siPixelRecHits+process.siStripMatchedRecHits+process.tracksP5+process.CosmicMuonSeed+process.cosmicMuons+process.globalCosmicMuons+process.muons*process.offlineBeamSpot*process.TrackRefitterP5*process.PixelNtuplizer_RealData)
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'Info'
process.TrackerDigiGeometryESModule.applyAlignment = True
