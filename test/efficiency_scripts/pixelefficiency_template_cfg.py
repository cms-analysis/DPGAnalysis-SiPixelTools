import FWCore.ParameterSet.Config as cms

process = cms.Process("PixelEfficiency")

# Messaging
process.load("FWCore.MessageService.MessageLogger_cfi")

# DQM services
#process.load("DQMServices.Core.DQM_cfg")

# DB Configuration
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")
# Magnetic Field
process.load("Configuration.StandardSequences.MagneticField_38T_cff")#with MF
#process.load("Configuration.StandardSequences.MagneticField_cff")#0T
#process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")#0T

# reconstruction sequence for Cosmics
#process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
#process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
#process.MeasurementTracker.pixelClusterProducer = cms.string('')


# Global Tag
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG" # FOR 21X REPRO
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_31X_GLOBALTAG" # FOR 31X REPRO

#process.GlobalTag.globaltag = "CRAFT_ALL_V4::All"
########tag behind for MC dataset
#process.GlobalTag.globaltag = 'COSMMC_21X_V1::All'
#process.GlobalTag.globaltag = 'COSMMC_22X_V1::All'
#process.GlobalTag.globaltag = "CRAFT_ALL_V11::All"
######## TAG FOR 31X REPROCESSING
#process.GlobalTag.globaltag = "CRAFT09_R_V3::All"
#process.GlobalTag.globaltag = "GR09_31X_V6P::All"
#process.GlobalTag.globaltag = "CRAFT0831X_V1::All"
#process.GlobalTag.globaltag = "CRAFT09_R_V4::All"
#process.GlobalTag.globaltag = "CRAFT08_R_V1::All"
process.GlobalTag.globaltag = "GR09_P_V7::All"

#process.test = cms.ESSource("PoolDBESSource",
#                                        DBParameters = cms.PSet(
#                                           messageLevel = cms.untracked.int32(0),
#					   #authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
#                                           authenticationPath = cms.untracked.string('0')
#                                        ),
#                                        connect = cms.string("sqlite_file:/afs/cern.ch/user/m/mucib/public/prova.db"),
#                                        toGet = cms.VPSet(cms.PSet(record = cms.string("SiPixelGainCalibrationOfflineRcd"),
#                                                                   tag = cms.string("GainCalib_TEST_offline"))
#                                                          )
#                                        )
#process.es_prefer_test = cms.ESPrefer("PoolDBESSource","test")

## Load and Configure OfflineValidation
process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NUMOFEVENTS) )

process.source = cms.Source("PoolSource",
   #comment this line when ALCARECO
   #inputCommands = cms.untracked.vstring('keep *',"drop *_*_*_FU"),
   #lastRun = cms.untracked.uint32(109624),
   #timetype = cms.string('runnumber'),
   #firstRun = cms.untracked.uint32(109011),
   #interval = cms.uint32(1),

#replace 'myfile.root' with the source file you want to use
     fileNames = cms.untracked.vstring(    
STRINGTOCHANGE   
    )
)

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'TRACKINPUTTAG'
process.TrackRefitterP5.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

process.load("DPGAnalysis.SiPixelTools.muonTOF_cfi")
process.load("DPGAnalysis.SiPixelTools.FEDInRunFilter_cfi")
#process.MuonTOFFilter_trackQuality.max_goodmuons = 2
#process.MuonTOFFilter_trackQuality.max_timeError = 15
#process.MuonTOFFilter_trackQuality.max_chi2_ndof = 15

process.checkCosmicTF = cms.EDAnalyzer('PixelEfficiency',
                TkTag = cms.InputTag("TRACKINPUTTAG"),
		TkTag0T = cms.InputTag("TRACKINPUTTAG0T"),	

                trajectoryInput = cms.InputTag('TrackRefitterP5'),
		
		pixelClusterInput=cms.InputTag("siPixelClusters"),
		
                HistOutFile =cms.untracked.string('ROOTFILE'),
		HistOutFile0T =cms.untracked.string('ROOTFILE0T'),
		
		skip0TRuns           = cms.untracked.bool(False),#for CRAFT08 only
		keep0TRuns           = cms.untracked.bool(False),
		keep38TRuns          = cms.untracked.bool(False),
		keepOnlyOneTrackEvts = cms.untracked.bool(False),
		skipBadModules       = cms.untracked.bool(False),
  		
		########### MUON CUT ###############
		#peak for muon cut (-8 CRAFT08 ; 10 MC08 ; -20=-11-9 CRAFT09)
		peak_MuonCut = cms.untracked.double(-8.),
		#window for muon cut (5 CRAFT08 ; 5 MC08 ; 2.5 CRAFT09)
		window_MuonCut = cms.untracked.double(5),
		#max muon time error for muon cut
		timeErr_MuonCut = cms.untracked.double(10),
		#min muon ndof for muon cut
		timeNdof_MuonCut = cms.untracked.double(25),
		
		########### EDGE CUT ###############
		#nSigma for edge cut (1 CRAFT08 ; 1 MC08 ; 1 CRAFT09)
		nSigma_EdgeCut = cms.untracked.double(1.),
		
		########### TACK CUT ###############
		#min number of hits on the track
		minHits_TrackCut = cms.untracked.double(6),
		#quality of the track: none (ie no quality cut), loose,good,tight
		quality_TrackCut = cms.untracked.string('none'),
		
		
		ListOfCuts = cms.untracked.PSet(
		  pT_cut        = cms.bool(False),
		  edge_cut      = cms.bool(False),
		  telescope_cut = cms.bool(False),
		  muon_cut      = cms.bool(False),
		  loose_cut     = cms.bool(False),
		  track_cut     = cms.bool(False)
		),
		

    BadModuleList = cms.untracked.VPSet(
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302060044)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302197784)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302195232)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302123296)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302127136)), 
        cms.PSet(
            errortype = cms.string('tbmA'),
            detid = cms.uint32(302125076)), 
        cms.PSet(
            errortype = cms.string('tbmB'),
            detid = cms.uint32(302126364)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302188552)), 
        cms.PSet(
            errortype = cms.string('tbmA'),
            detid = cms.uint32(302121992)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302126596)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344014340)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344014344)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344014348)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344019460)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344019464)), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344019468)), 

	)		
)		
process.load("DPGAnalysis.SiPixelTools.EventFilter_forTimingRun_cfi")
process.timingRunFilter.maxOrbit = 99999999999
process.timingRunFilter.minOrbit = 21600000
	
#list from /CMSSW/CondTools/SiPixel/test/SiPixelBadModuleByHandBuilder_cfg.py    
# new list from same file under CMSSW
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'INFO'#32X version
#process.MessageLogger.cerr.threshold = 'Info' #22X version

#RECONSTRUCTION FROM DQM
#process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
#process.siPixelDigis.InputLabel = 'source'
#process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")
#process.load("EventFilter.SiStripRawToDigi.SiStripRawToDigis_standard_cff")
#process.siStripDigis.ProductLabel = 'source'
#process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi")
#process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi")
#process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi")
#process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi")
#process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi")
#process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
#process.load("RecoPixelVertexing.Configuration.RecoPixelVertexing_cff")
#process.load("RecoTracker.Configuration.RecoTrackerP5_cff")
#process.Reco = cms.Sequence(process.siPixelDigis*process.siPixelClusters)
#process.RecoStrips = cms.Sequence(process.siStripDigis*process.siStripClusters)
#process.siPixelLocalReco = cms.Sequence(process.siPixelRecHits)
#process.siStripLocalReco = cms.Sequence(process.siStripMatchedRecHits)
#process.trackerReco = cms.Sequence(process.Reco*process.RecoStrips)
#process.trackerLocalReco = cms.Sequence(process.siPixelLocalReco*process.siStripLocalReco)
#process.trackReconstruction = cms.Sequence(process.trackerReco*process.trackerLocalReco*process.offlineBeamSpot*process.recopixelvertexing*process.tracksP5) 

#process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
process.p = cms.Path(process.timingRunFilter*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
#process.p = cms.Path(process.RawToDigi*process.reconstructionCosmics*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
#process.p = cms.Path(process.RawToDigi*process.reconstructionCosmics*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
#process.p = cms.Path(process.RawToDigi*process.reconstructionCosmics*process.fedInRunFilter*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
#process.p = cms.Path(process.trackReconstruction*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
#process.p = cms.Path(process.trackerCosmics*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
#process.p = cms.Path(process.fedInRunFilter*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
#process.p = cms.Path(process.fedInRunFilter*process.MuonTOFFilter_trackQuality*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)

    
process.TrackerDigiGeometryESModule.applyAlignment = True
