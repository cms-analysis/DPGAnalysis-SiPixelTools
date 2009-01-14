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
process.GlobalTag.globaltag = "COSMMC_21X_V1::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0


##
## Load and Configure TrackRefitter
##
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'cosmictrackfinderP5'
process.TrackRefitterP5.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")


##
## Load and Configure OfflineValidation
##
process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")



process.load("DPGAnalysis.SiPixelTools.pixelNtuplizer_RealData_cfi")
process.PixelNtuplizer_RealData.OutputFile = 'MCRECOTkAlCosmicsCosmicTF38TTTree.root'
process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'


process.source = cms.Source("PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring("rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/68A55B21-01CC-DD11-9EDD-0019B9E7CC38.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/6AD4BFD9-03CC-DD11-BE65-001D0967DD73.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/700C0EF4-02CC-DD11-BA9A-0019B9E4FCDF.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/72AC7BD4-03CC-DD11-8F02-001D0967CFB8.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/780CD34C-00CC-DD11-8028-001D0967D0FD.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/789A73F0-46CC-DD11-8A31-0019B9E4FF5F.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/7A6F8E45-00CC-DD11-A3B1-001D0967C64E.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/7A74B621-01CC-DD11-8379-0019B9E4ACA0.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/7ED4A805-03CC-DD11-B1E5-001D0967DAF3.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/80C77E0F-01CC-DD11-87A9-001D0967D2E7.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/82A87FD3-03CC-DD11-9DB2-001D0967CFE5.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/82CEED3A-03CC-DD11-BB99-0019B9E71474.root",
"rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/82ED6150-00CC-DD11-B6F9-00145EDD75D9.root"


    ) )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.PixelNtuplizer_RealData)
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'Info'
process.TrackerDigiGeometryESModule.applyAlignment = True
