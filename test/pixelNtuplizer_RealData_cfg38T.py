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
process.GlobalTag.globaltag = "CRAFT_ALL_V11::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0

# reconstruction sequence for Cosmics
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
# also run 3 times:
process.ctfNtuple = process.PixelNtuplizer_RealData.clone()
process.ctfNtuple.trajectoryInput = 'ctfRefitter'

process.rsNtuple = process.PixelNtuplizer_RealData.clone()
process.rsNtuple.trajectoryInput = 'rsRefitter'

process.cosmtfNtuple = process.PixelNtuplizer_RealData.clone()
process.cosmtfNtuple.trajectoryInput = 'cosmRefitter'

#process.load("DPGAnalysis.SiPixelTools.PixelNtuplizer_RealData_cfi")
#process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'


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
    '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0006/26B79166-7C00-DE11-AA81-0030486790A0.root'

   
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
process.MessageLogger.cerr.threshold = 'Info'
process.TrackerDigiGeometryESModule.applyAlignment = True
