#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms

process = cms.Process("cluTest")

# needed for det-id
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Random_v*'],
    HLTPaths = ['HLT_ZeroBias_*'],
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
#    HLTPaths = ['HLT_PAZeroBias*'],
#    HLTPaths = ['HLT_PARandom*'],
#    HLTPaths = ['HLT_PAMinBias*'],
# Commissioning: HLT_L1_BptxXOR_BscMinBiasOR
#    HLTPaths = ['HLT_L1_BptxXOR_BscMinBiasOR'],
# Zero-Bias : HLT_L1_BPTX, HLT_L1_BPTX_PlusOnly, HLT_L1_BPTX_MinusOnly, HLT_ZeroBias
#    HLTPaths = ['HLT_L1_BPTX','HLT_ZeroBias','HLT_L1_BPTX_MinusOnly','HLT_L1_BPTX_PlusOnly'],
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelClusters'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
#        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
#  fileNames =  myfilelist
 fileNames = cms.untracked.vstring(    
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/1A57B62A-4328-E611-9D78-02163E01339F.root",

# for MC 
#  'file:../scripts/tracks.root'
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/clus/clus1.root'
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/rechits/rechits1.root'
#   'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_postls171.root'
#   'file:/afs/cern.ch/work/d/dkotlins/public/data/740_p9/clus/rechits_2012d_208307.root'
#   'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_mc71.root'

  )

)

# for data 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('206940:0-206940:1027')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('274314:97-274314:9999')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus.root')
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# Choose the global tag here:
# 2012
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag.globaltag = autoCond['run1_data']
#process.GlobalTag.globaltag = 'GR_R_73_V0A'
# 2015
#process.GlobalTag.globaltag = autoCpnd['run2_data']
# 2015
#process.GlobalTag.globaltag = 'GR_P_V49::All'
# 2016
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8' # for 272

process.analysis = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(True),
    src = cms.InputTag("siPixelClusters"),
#    src = cms.InputTag("siPixelClustersPreSplitting"),
)

# for data
process.p = cms.Path(process.hltfilter*process.analysis)
#process.p = cms.Path(process.analysis)

# for MC
#process.p = cms.Path(process.analysis)

# define an EndPath to analyze all other path results
#process.hltTrigReport = cms.EDAnalyzer( 'HLTrigReport',
#    HLTriggerResults = cms.InputTag( 'TriggerResults','','' )
#)
#process.HLTAnalyzerEndpath = cms.EndPath( process.hltTrigReport )
