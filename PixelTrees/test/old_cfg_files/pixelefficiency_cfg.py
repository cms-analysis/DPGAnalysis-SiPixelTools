import FWCore.ParameterSet.Config as cms
#import process
#f=open("dump.py","w")
#print >> f,process.dumpPython()

process = cms.Process("PixelEfficiency")
#process.load("Alignment.OfflineValidation.DataSet_Craft_Run66604_66904_cff")

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
  #process.GlobalTag.globaltag = "CRUZET3_V5::All"
  #process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NUMOFEVENTS) )

process.source = cms.Source("PoolSource",
#replace 'myfile.root' with the source file you want to use
     fileNames = cms.untracked.vstring(
   "rfio:/castor/cern.ch/cms/store/data/Commissioning08/Cosmics/ALCARECO/CRAFT_ALL_V4_StreamALCARECOTkAlCosmicsHLT_step3_AlcaReco-v2/0051/6AD4BFD9-03CC-DD11-BE65-001D0967DD73.root",
  
#badCRUZET    "rfio:///castor/cern.ch/cms/store/data/CRUZET3/Cosmics/RECO/CRUZET3_V5_SuperPointing_v1/0000/0039C4CC-3265-DD11-8755-003048726C85.root"
#runningCRAFT	"rfio:///castor/cern.ch/cms/store/data/Commissioning08/Cosmics/RECO/v1/000/068/094/54F3D904-90A6-DD11-9102-000423D174FE.root",
#localtest 'file:CRAFT_V3P_V4.root'
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

#provato a fare hide process.p = cms.Path(process.offlineBeamSpot*process.CosmicTFRefit*process.checkCosmicTF)
process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.checkCosmicTF)
