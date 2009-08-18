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
#process.load("Configuration.StandardSequences.MagneticField_cff") #for 0T runs
#process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff") #for 0T runs
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://FrontierInt/CMS_COND_30X_GLOBALTAG"
process.GlobalTag.globaltag = "CRAFT0831X_V1::All"

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

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
process.PixelNtuplizer_RealData.isCosmic = True
process.PixelNtuplizer_RealData.isSim = False
process.PixelNtuplizer_RealData.useAllPixel = False

# also run 3 times:
process.ctfNtuple = process.PixelNtuplizer_RealData.clone()
process.ctfNtuple.trajectoryInput = 'ctfRefitter'

process.rsNtuple = process.PixelNtuplizer_RealData.clone()
process.rsNtuple.trajectoryInput = 'rsRefitter'

process.cosmtfNtuple = process.PixelNtuplizer_RealData.clone()
process.cosmtfNtuple.trajectoryInput = 'cosmRefitter'

##
## configure output ntuple file using TFileService
##

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("SuperPointing_test_threealgos.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

##
## Configure input file
##
process.source = cms.Source("PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(

        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/76160DD7-FE73-DE11-92B6-001A92810AE0.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/6C615EAE-FE73-DE11-B169-001A92971B0E.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/7EA856DA-FE73-DE11-AFD7-0018F3D0962A.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/EE494DB9-FE73-DE11-A86F-0018F3D096BC.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/28A5C5B0-FE73-DE11-A3FB-001A928116C0.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/8A20AB9D-FE73-DE11-8E78-001BFCDBD154.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/92575CDF-FE73-DE11-8FC6-001A92971B8E.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/9CFEBBAB-FE73-DE11-9DFF-001A92971B8A.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0003/081ACD90-FE73-DE11-A83C-0018F3D09688.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0002/E4AEF4AC-FE73-DE11-BE61-001A92971B48.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0002/9833D7D2-FE73-DE11-8FFF-0018F3D0970E.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0002/CC35E3C2-FE73-DE11-964A-0018F3D096E0.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0000/E44428C2-9873-DE11-AB53-003048679228.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0002/30F0D9B7-FE73-DE11-B2A3-001A92971B04.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0002/3E4F14C5-FE73-DE11-8314-001A92971B48.root',
        '/store/skimming/Commissioning08/Cosmics/RAW-RECO/CRAFT0831X_V1_311_ReReco_FromSuperPointing_v1/0002/904AA096-FE73-DE11-AE25-0018F3D09708.root'
) )

# these drop commands are necessary to get rid of all HLT problems and DQM bulk
process.source.inputCommand = cms.untracked.vstring("drop *_*_*_FU"
                                                    ,"drop *_*_*_HLT",
                                                    "drop *_MEtoEDMConverter_*_*","drop *_lumiProducer_*_REPACKER"
                                                    )
##
## number of events
##
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

##
## executionpath
##
process.p = cms.Path(
    # filters:
    process.fedInRunFilter*
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
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True
