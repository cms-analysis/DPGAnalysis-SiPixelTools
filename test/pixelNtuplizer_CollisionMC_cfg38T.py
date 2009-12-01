import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# DQM services
process.load("DQMServices.Core.DQM_cfg")

# Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# conditions
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
#process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "STARTUP3X_V8D::All"
#process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0

# reconstruction sequence for Cosmics
#process.load("Configuration.StandardSequences.RawToDigi_cff")
#process.load("Configuration.StandardSequences.Reconstruction_cff")

##
## Load and Configure TrackRefitter
##
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

# Instead, load all track algorithms:
process.ckfRefitter = process.TrackRefitter.clone()
process.ckfRefitter.src = 'generalTracks'
process.ckfRefitter.TrajectoryInEvent = True

process.rsRefitter = process.TrackRefitter.clone()
process.rsRefitter.src = 'rsWithMaterialTracks'
process.rsRefitter.TrajectoryInEvent = True

#process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

##
## Load and Configure OfflineValidation
##
#process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")

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
process.PixelNtuplizer_RealData.isCosmic = False
process.PixelNtuplizer_RealData.isSim = False
process.PixelNtuplizer_RealData.useAllPixel = False

# also run 3 times:
process.ckfNtuple = process.PixelNtuplizer_RealData.clone()
process.ckfNtuple.trajectoryInput = 'ckfRefitter'

process.rsNtuple = process.PixelNtuplizer_RealData.clone()
process.rsNtuple.trajectoryInput = 'rsRefitter'

##
## configure output ntuple file using TFileService
##

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("900GeVMC_Cluster.root"),
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
  
  '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/E4B3A7BE-3AD7-DE11-9230-002618943939.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/E4590360-4CD7-DE11-8CB4-002618943896.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/AAE638A2-3BD7-DE11-B6A8-002618943944.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/A0AFB73F-38D7-DE11-82F3-0026189438B4.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/702846CC-38D7-DE11-8C11-0026189438E2.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/5A2ACAFC-47D7-DE11-9A9F-002618943935.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0004/BEDB1206-36D7-DE11-9B65-002354EF3BE4.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0004/72A7CB53-23D7-DE11-A681-00261894389E.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0004/2A33F2AE-17D7-DE11-9668-003048679030.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/FE95B543-06D7-DE11-B3A1-00261894385A.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/F2C002D3-0FD7-DE11-A0D8-003048678D78.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/AC9866AB-0FD7-DE11-9446-0018F3D0966C.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/9AF119CE-0BD7-DE11-BC1D-003048678B86.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/8A220D47-06D7-DE11-A3A2-002618FDA279.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/80995B54-06D7-DE11-A444-002354EF3BE2.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/7A31312F-0AD7-DE11-9A93-0018F3D095F6.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/76DF1D47-06D7-DE11-9745-002618943869.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/741A424E-06D7-DE11-AAD2-0026189438DB.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/72887158-06D7-DE11-8131-0026189438D9.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/58B6624E-06D7-DE11-ABE2-002618943906.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/484003D0-0FD7-DE11-8A90-003048678B88.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0003/46ED4064-06D7-DE11-A7E4-001A92810ADC.root'
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
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000) )

##
## executionpath
##
process.p = cms.Path(
    # mixing module (only for sim data)
   # process.mix*
    # filters:
#    process.fedInRunFilter*
    # standard reco sequence
  #  process.RawToDigi*process.reconstruction_withRS*
    # more filters:
#    process.MuonTOFFilter_trackQuality *
    # create rechits
    # (rechits are transient data members so are not saved in CMSSW .root files)
   # process.offlineBeamSpot*
#   refitters for all tracking algos (thse are what actually create the rechits)
    process.ckfRefitter*
  #  process.rsRefitter*
    # run ntuplizers
    process.ckfNtuple
  #  process.rsNtuple
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True
