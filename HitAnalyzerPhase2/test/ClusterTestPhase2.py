# Imports
import FWCore.ParameterSet.Config as cms

# Create a new CMS process
process = cms.Process('cluTest')

# Import all the necessary files
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
#process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')


# Number of events (-1 = all)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input file
process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(

"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/DA232E7D-EDCF-E711-B0B3-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/16C5AE1E-E9CF-E711-A218-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/F429ADE4-E6CF-E711-AA8E-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/4661E716-E7CF-E711-AF34-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/549EB99C-E9CF-E711-9123-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/AC401BD1-EACF-E711-ADBD-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/B28963B7-EACF-E711-B0FF-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/BCD1196E-EACF-E711-A6F1-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/143A37A6-E8CF-E711-9A95-0242AC130002.root",
"/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt10/GEN-SIM-RECO/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/9422CC46-EBCF-E711-8186-0242AC130002.root",

 )
)

# Output
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('file:clusters.root')
)

# DEBUG
#process.MessageLogger = cms.Service('MessageLogger',
#	debugModules = cms.untracked.vstring('siPhase2Clusters'),
#	destinations = cms.untracked.vstring('cout'),
#	cout = cms.untracked.PSet(
#		threshold = cms.untracked.string('ERROR')
#	)
#)

# Analyzer
process.analysis = cms.EDAnalyzer('PixClusterTestPhase2',
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("siPixelClusters")
    #src = cms.InputTag("siPixelClustersPreSplitting")
)

# Processes to run
process.p = cms.Path(process.analysis)
