import FWCore.ParameterSet.Config as cms

process = cms.Process("LA")

process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# check for the correct tag on https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = "MCRUN2_73_V7::All"

process.load("RecoTracker.Configuration.RecoTracker_cff")
from RecoVertex.BeamSpotProducer.BeamSpot_cff import *
process.offlineBeamSpot = offlineBeamSpot 

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

#process.load('RecoTracker/MeasurementDet/MeasurementTrackerEventProducer_cfi')
process.load("RecoTracker/TrackProducer/TrackRefitters_cff")
process.TrackRefitter.NavigationSchool = ""
# put here the tag of the tracks you want to use
# alcareco samples have special names for the tracks, in normal reco samples generalTracks can be used
process.TrackRefitter.src = "generalTracks"
process.TrackRefitter.TrajectoryInEvent = True
process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.categories.append('HLTrigReport')
#process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound'), wantSummary = cms.untracked.bool(True) )

process.lorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngle",
	src = cms.InputTag("TrackRefitter"),
	fileName = cms.string("lorentzangle.root"),
	fileNameFit	= cms.string("lorentzFit.txt"),
	binsDepth	= cms.int32(50),
	binsDrift =	cms.int32(200),
	ptMin = cms.double(3),
	#in case of MC set this to true to save the simhits (does not work currently, Mixing Module needs to be included correctly)
	simData = cms.bool(False),
  	normChi2Max = cms.double(2),	
	clustSizeYMin = cms.int32(2),
	residualMax = cms.double(0.01),
	clustChargeMax = cms.double(120000)
)

process.myout = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('LA_CMSSW.root')
)

process.p = cms.Path(
		process.siPixelRecHits*
		process.offlineBeamSpot*
		#process.MeasurementTrackerEvent*
		process.TrackRefitter*
		process.lorentzAngle)

# uncomment this if you want to write out the new CMSSW root file (very large)
# process.outpath = cms.EndPath(process.myout)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
	#put here the sample you want to use
    fileNames = cms.untracked.vstring(
	"/store/relval/CMSSW_7_4_0_pre2/RelValSingleMuPt100_UP15/GEN-SIM-RECO/MCRUN2_73_V7-v1/00000/5A994D3A-C984-E411-9744-02163E00E910.root"
    #put your source file here
	  # ' '
	),   
#   skipEvents = cms.untracked.uint32(100) 
)
