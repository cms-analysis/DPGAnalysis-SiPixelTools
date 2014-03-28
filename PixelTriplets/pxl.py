
import FWCore.ParameterSet.Config as cms

process = cms.Process("DQ")

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'START53_V27::All'
#process.GlobalTag.globaltag = 'FT_R_53_V21::All'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi')
process.load('RecoTracker.TransientTrackingRecHit.TTRHBuilderWithTemplate_cfi')
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
# Get beamspot from DB
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#from CalibTracker.Configuration.Common.PoolDBESSource_cfi import poolDBESSource
#import CalibTracker.Configuration.Common.PoolDBESSource_cfi

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
		'/store/user/aburgmei/embedding/20121126-DYJetsToLL-RECOSkim/job_0_skimGenZmumuWithinAcc_RECOSim.root'
	)
)

process.Histos = cms.EDAnalyzer('Pxl',
	triggerSource = cms.InputTag('TriggerResults::HLT'),
	ttrhBuilder = cms.string('WithAngleAndTemplate'),
)

process.TFileService = cms.Service('TFileService',
	fileName = cms.string('job.root'),
)

process.p = cms.Path(process.offlineBeamSpot*process.Histos)
