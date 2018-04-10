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
