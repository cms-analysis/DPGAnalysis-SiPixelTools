import FWCore.ParameterSet.Config as cms

process = cms.Process("PixelEfficiency")

# Messaging
process.load("FWCore.MessageService.MessageLogger_cfi")
# DB Configuration
process.load("CondCore.DBCommon.CondDBSetup_cfi")
# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")
# Magnetic Field
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
#process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")

# Global Tag
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"   #modifythismodifythismodifythismodifythis
process.GlobalTag.globaltag = "CRAFT_ALL_V4::All"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NUMOFEVENTS) )

process.source = cms.Source("PoolSource",
#replace 'myfile.root' with the source file you want to use
     fileNames = cms.untracked.vstring(
STRINGTOCHANGE   
    )
)

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'ALCARECOTkAlCosmicsCosmicTF'
process.TrackRefitterP5.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")


process.checkCosmicTF = cms.EDAnalyzer('PixelEfficiency',
                TkTag = cms.InputTag("TRACKINPUTTAG"),
		TkTag0T = cms.InputTag("TRACKINPUTTAG0T"),	

                trajectoryInput = cms.InputTag('TrackRefitterP5'),
		
                HistOutFile =cms.untracked.string('ROOTFILE'),
		HistOutFile0T =cms.untracked.string('ROOTFILE0T')
)

#process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'Info'

process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
