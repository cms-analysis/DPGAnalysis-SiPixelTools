import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# DQM services
process.load("DQMServices.Core.DQM_cfg")

# Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# conditions
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.GlobalTag.globaltag = "FIRSTCOLL::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

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
                                   fileName = cms.string("test.root"),
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
    'rfio:/castor/cern.ch/user/c/chiochia/09_beam_commissioning/BSCskim_123151_Express.root'
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/FA9D48E2-14DE-DE11-BB11-001D09F291D2.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/F0604FD7-19DE-DE11-9A21-003048D37560.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/E4184AB2-0BDE-DE11-8B34-001D09F25208.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/DA87351C-01DE-DE11-B84D-0030487A18A4.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/D0985A13-15DE-DE11-81D9-001D09F29524.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/BC27D9E4-1CDE-DE11-B7DC-001D09F2A690.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/6ADC6A1B-01DE-DE11-8FBD-00304879FA4A.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/68216223-19DE-DE11-85D3-0030487A18A4.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/54F707ED-0FDE-DE11-B8E3-000423D6CA02.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/46BC540A-17DE-DE11-B913-003048D2BED6.root',
#        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/104086E4-1CDE-DE11-BE78-001D09F291D7.root'

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
    input = cms.untracked.int32(100) )

##
## executionpath
##
process.p = cms.Path(
    # mixing module (only for sim data)
#    process.mix*
    # filters:
#    process.fedInRunFilter*
    # standard reco sequence
    process.RawToDigi*
    process.reconstruction*
    # more filters:
#    process.MuonTOFFilter_trackQuality *
    # create rechits
    # (rechits are transient data members so are not saved in CMSSW .root files)
    process.offlineBeamSpot*
#   refitters for all tracking algos (thse are what actually create the rechits)
    process.ckfRefitter*
    process.rsRefitter*
    # run ntuplizers
    process.ckfNtuple*
    process.rsNtuple
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True
