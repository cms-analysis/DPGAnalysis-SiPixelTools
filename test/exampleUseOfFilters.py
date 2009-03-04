import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'Info'
process.MessageLogger.categories.extend(["MuonTOFFilter_trackQuality"])
process.MessageLogger.categories.extend(["FEDInRunFilter"])



# DQM services
process.load("DQMServices.Core.DQM_cfg")

# Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

#fake conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRAFT_ALL_V11::All"

process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

process.load("DPGAnalysis.SiPixelTools.muonTOF_cfi")
process.load("DPGAnalysis.SiPixelTools.FEDInRunFilter_cfi")
process.MuonTOFFilter_trackQuality.max_goodmuons = 2
process.MuonTOFFilter_trackQuality.max_timeError = 15
process.MuonTOFFilter_trackQuality.max_chi2_ndof = 15


process.source = cms.Source("PoolSource",

    fileNames = cms.untracked.vstring(
       '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0005/B8FB3273-5DFF-DD11-BEAB-00304875A7B5.root',
       '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0005/C0E4F880-5CFF-DD11-B561-0030487624FD.root',
       '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0005/EAA9AE47-5FFF-DD11-A966-001A92810ADE.root',
       '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0006/20993712-7C00-DE11-8BAA-003048678B5E.root',
       '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0006/26B79166-7C00-DE11-AA81-0030486790A0.root'


        )  )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

process.p = cms.Path(process.fedInRunFilter*process.MuonTOFFilter_trackQuality)


        
