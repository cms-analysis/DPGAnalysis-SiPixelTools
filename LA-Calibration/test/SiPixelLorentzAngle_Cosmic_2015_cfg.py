import FWCore.ParameterSet.Config as cms

process = cms.Process("LA")

process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# check for the correct tag on https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = "GR_R_73_V0A::All" 


#process.load("RecoTracker.Configuration.RecoTracker_cff")

from RecoVertex.BeamSpotProducer.BeamSpot_cff import *
process.offlineBeamSpot = offlineBeamSpot 
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

from RecoTracker.TrackProducer.TrackRefitters_cff import *
process.CosmicTFRefit = RecoTracker.TrackProducer.TrackRefitterP5_cfi.TrackRefitterP5.clone()
process.CosmicTFRefit.src = 'cosmictrackfinderP5'

rootFileName = "LorentzTree_MuOnia.root"
fitFileName = "LorentzFit_MuOnia.txt"

process.lorentzAngle = cms.EDAnalyzer("PixelLorentzAngle",
	src = cms.string("CosmicTFRefit"),
	fileName = cms.string(rootFileName),
	fileNameFit	= cms.string(fitFileName),
	binsDepth	= cms.int32(50),
	binsDrift =	cms.int32(200),
        # generally used cuts:
	ptMin = cms.double(3.0),#default is 3.0
	#in case of MC set this to true to save the simhits (does not work currently, Mixing Module needs to be included correctly)
	simData = cms.bool(False),
  	normChi2Max = cms.double(2),#default is 2
	clustSizeYMin = cms.int32(4),# default is 4
	residualMax = cms.double(0.005),#default is 0.005
	clustChargeMax = cms.double(120000) #default is 120000
)

#process.myout = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('LA_CMSSW.root')
#)

process.p = cms.Path(process.offlineBeamSpot)
		    #*process.CosmicTFRefit
		    #*process.lorentzAngle)

# uncomment this if you want to write out the new CMSSW root file (very large)
# process.outpath = cms.EndPath(process.myout)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
	#put here the sample you want to use
    fileNames = cms.untracked.vstring(
#FILENAMES
	#"file:/STORE/WZbb/LATrees/04B31BFB-3C2A-E211-A847-003048D374CA.root",
	#"file:/STORE/WZbb/LATrees/08528856-312A-E211-BC8C-5404A63886A5.root"
	#"file:/STORE/WZbb/LATrees/08528856-312A-E211-BC8C-5404A63886A5.root"
	"/store/relval/CMSSW_7_4_0_pre2/Cosmics/RECO/GR_R_73_V0A_RelVal_cos2011A-v1/00000/F4B1D62D-1C85-E411-BAE5-02163E00E6C8.root"	


	),   
#   skipEvents = cms.untracked.uint32(100) 
)

