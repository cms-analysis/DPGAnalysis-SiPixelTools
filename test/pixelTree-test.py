import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")


# -- DQM services ???? Hangs VERY long 
#process.load("DQMServices.Core.DQM_cfg")


# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")


# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://FrontierInt/CMS_COND_31X_GLOBALTAG"
process.GlobalTag.globaltag = "GR09_31X_V5P::All"


# -- This is for re-reconstructing with a test gain calibration
process.test = cms.ESSource("PoolDBESSource",
                                        DBParameters = cms.PSet(
                                           messageLevel = cms.untracked.int32(0),
                                          #authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
                                           authenticationPath = cms.untracked.string('0')
                                        ),
                                        connect = cms.string("sqlite_file:/afs/cern.ch/user/u/ursl/scratch0/prova.db"),
                                        toGet = cms.VPSet(cms.PSet(record = cms.string("SiPixelGainCalibrationOfflineRcd"),
                                                                   tag = cms.string("GainCalib_TEST_offline"))
                                                          )
                                        )
process.es_prefer_test = cms.ESPrefer("PoolDBESSource","test")


# -- Reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")


# -- Load and Configure TrackRefitter
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
# tracks available in cosmic reco: 'cosmictrackfinderP5', 'ctfWithMaterialTracksP5', 'rsWithMaterialTracksP5'
#process.TrackRefitterP5.src =  'cosmictrackfinderP5' #'ctfWithMaterialTracksP5'
# AlCa tracks:
#processlTrackRefitterP5.src = 'ALCARECOTkAlCosmicsCosmicTF0T'
#process.TrackRefitterP5.TrajectoryInEvent = True

# Instead, load all three track algorithms:
#process.ctfRefitter = process.TrackRefitterP5.clone()
#process.ctfRefitter.src = 'ctfWithMaterialTracksP5'
#process.ctfRefitter.TrajectoryInEvent = True
#process.rsRefitter = process.TrackRefitterP5.clone()
#process.rsRefitter.src = 'rsWithMaterialTracksP5'
#process.rsRefitter.TrajectoryInEvent = True
#process.cosmRefitter = process.TrackRefitterP5.clone()
#process.cosmRefitter.src = 'cosmictrackfinderP5'
#process.cosmRefitter.TrajectoryInEvent = True

#process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")


# -- Load and Configure OfflineValidation ????
process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")


# -- Pixel event filters:
process.load("DPGAnalysis.SiPixelTools.muonTOF_cfi")
process.load("DPGAnalysis.SiPixelTools.FEDInRunFilter_cfi")
process.MuonTOFFilter_trackQuality.max_goodmuons = 20
process.MuonTOFFilter_trackQuality.max_timeError = 15
process.MuonTOFFilter_trackQuality.max_chi2_ndof = 15


# -- the tree filler
process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    rootFileName          = cms.untracked.string("/afs/cern.ch/user/u/ursl/scratch0/root/pixelTree-test.root"),
    muonCollectionLabel   = cms.untracked.string('muons'),
    trajectoryInputLabel  = cms.untracked.string('ctfWithMaterialTracksP5'),
    trackCollectionLabel  = cms.untracked.string('ctfWithMaterialTracksP5'),
    pixelClusterLabel     = cms.untracked.string('siPixelClusters')
    )


# -- Input files
process.source = cms.Source("PoolSource",
                            # replace with your files
                            #lastRun = cms.untracked.uint32(64789),
                            #timetype = cms.string('runnumber'),
                            #firstRun = cms.untracked.uint32(64108),
                            #interval = cms.uint32(1),
                            fileNames = cms.untracked.vstring(
    "/store/data/CRAFT09/Cosmics/RAW-RECO/GR09_31X_V5P_SuperPointing-332_v4/0013/56C3BA64-648C-DE11-855F-0018F3D095F0.root"
    )
                            )

# these drop commands are necessary to get rid of all HLT problems and DQM bulk
process.source.inputCommand = cms.untracked.vstring("drop *_*_*_FU",
                                                    "drop *_MEtoEDMConverter_*_*","drop *_lumiProducer_*_REPACKER"
                                                    )
# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -- Path
process.p = cms.Path(
    # filters:
    #process.fedInRunFilter*
    # standard reco sequence
    process.RawToDigi*process.reconstructionCosmics*
    #process.trackReconstruction*
    # more filters:
    #process.MuonTOFFilter_trackQuality *
    # create rechits
    # (rechits are transient data members so are not saved in CMSSW .root files)
    process.offlineBeamSpot*
    #   refitters for all tracking algos (thse are what actually create the rechits)
    #    process.ctfRefitter*
    #    process.rsRefitter*
    #    process.cosmRefitter*
    # run ntuplizers
    process.PixelTree
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True
