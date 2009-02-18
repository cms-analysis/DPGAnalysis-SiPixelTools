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
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"

#process.GlobalTag.globaltag = "CRAFT_ALL_V4::All"
#tag behind for MC dataset
process.GlobalTag.globaltag = "COSMMC_21X_V1"
#process.GlobalTag.globaltag = "CRAFT_ALL_V9::All"

#Do the reconstruction with new tag
#process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NUMOFEVENTS) )

process.source = cms.Source("PoolSource",
   #comment this line when ALCARECO
   #inputCommands = cms.untracked.vstring('keep *',"drop *_*_*_FU"),

#replace 'myfile.root' with the source file you want to use
     fileNames = cms.untracked.vstring(    
STRINGTOCHANGE   
    )
)

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'TRACKINPUTTAG'
process.TrackRefitterP5.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")


process.checkCosmicTF = cms.EDAnalyzer('PixelEfficiency',
                TkTag = cms.InputTag("TRACKINPUTTAG"),
		TkTag0T = cms.InputTag("TRACKINPUTTAG0T"),	

                trajectoryInput = cms.InputTag('TrackRefitterP5'),
		
		pixelClusterInput=cms.InputTag("siPixelClusters"),
		
                HistOutFile =cms.untracked.string('ROOTFILE'),
		HistOutFile0T =cms.untracked.string('ROOTFILE0T'),
#list from /CMSSW/CondTools/SiPixel/test/SiPixelBadModuleByHandBuilder_cfg.py    
    BadModuleList = cms.untracked.VPSet(cms.PSet(
        errortype = cms.string('whole'),
        detid = cms.uint32(302197784)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302195232)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302123296)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302127136)
        ), 
        cms.PSet(
            errortype = cms.string('tbmA'),
            detid = cms.uint32(302125076)
        ), 
        cms.PSet(
            errortype = cms.string('tbmB'),
            detid = cms.uint32(302126364)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302188552)
        ), 
        cms.PSet(
            errortype = cms.string('tbmA'),
            detid = cms.uint32(302121992)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(302126596)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344074500)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344074504)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344074508)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344074512)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344074756)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344074760)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344074764)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344075524)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344075528)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344075532)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344075536)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344075780)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344075784)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344075788)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076548)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076552)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076556)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076556)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076560)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076804)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076808)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344076812)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344005128)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344020236)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344020240)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344020488)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344020492)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344019212)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344019216)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344019464)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344019468)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344018188)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344018192)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344018440)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344018444)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344014340)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344014344)
        ), 
        cms.PSet(
            errortype = cms.string('whole'),
            detid = cms.uint32(344014348)
        ))		
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'Info'

#process.p = cms.Path(process.trackerCosmics*process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
