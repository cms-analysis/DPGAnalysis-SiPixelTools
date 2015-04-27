#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms

process = cms.Process("cluTest")

#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.Geometry.GeometryIdeal_cff")
# needed for det-id
process.load("Configuration.Geometry.GeometryRecoDB_cff")


process.load("Configuration.StandardSequences.MagneticField_38T_cff")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_ZeroBias_*'],
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

# to select PhysicsBit
#process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
#process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

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
#  "/store/data/Run2012C/MinimumBias/RECO/22Jan2013-v1/20020/2C3E2BB3-4173-E211-8417-003048F0117C.root",
#  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/FEFCE925-8EB2-E411-A251-02163E011D70.root",

# for MC 
#  'file:../scripts/tracks.root'
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/clus/clus1.root'
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/rechits/rechits1.root'
#  'file:../../../../../CMSSW_7_0_0_pre8/src/EventFilter/SiPixelRawToDigi/test/digis.root'
#   'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_postls171.root'
   'file:/afs/cern.ch/work/d/dkotlins/public/data/740_p9/clus/rechits_2012d_208307.root'
#   'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_mc71.root'
#  "/store/relval/CMSSW_7_4_0_pre9/RelValBsToMuMu_13/GEN-SIM-RECO/MCRUN2_74_V7-v1/00000/4C1C6AAA-31D4-E411-AD53-0025905A48EC.root",
#  "/store/relval/CMSSW_7_4_0_pre9/RelValBsToMuMu_13/GEN-SIM-RECO/MCRUN2_74_V7-v1/00000/DC26A1A1-31D4-E411-987E-003048FFCBFC.root",

  )

)

# for data 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('206940:0-206940:1027')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus.root')
)

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 

# Choose the global tag here:
# 2012
#process.GlobalTag.globaltag = 'GR_P_V40::All'
# MC 2013
# process.GlobalTag.globaltag = 'MC_70_V1::All'
# DATA 2014
#process.GlobalTag.globaltag = 'PRE_R_71_V3::All'
# MC 2014
#process.GlobalTag.globaltag = 'PRE_STA71_V4::All'
# for MC-2015 73X
#process.GlobalTag.globaltag = "MCRUN1_73_V5::All" # OK for condDB
# for MC-2015 74X
#process.GlobalTag.globaltag = "MCRUN2_74_V7" 

# 2012
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag.globaltag = autoCond['run1_data']
#process.GlobalTag.globaltag = 'GR_R_73_V0A'
# 2015
#process.GlobalTag.globaltag = autoCpnd['run2_data']
# 2015
#process.GlobalTag.globaltag = 'GR_P_V49::All'


process.analysis = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
#    src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
)

# for data
#process.p = cms.Path(process.hltPhysicsDeclared*process.hltfilter*process.analysis)
#process.p = cms.Path(process.hltfilter*process.analysis)
process.p = cms.Path(process.analysis)

# for MC
#process.p = cms.Path(process.analysis)

# define an EndPath to analyze all other path results
#process.hltTrigReport = cms.EDAnalyzer( 'HLTrigReport',
#    HLTriggerResults = cms.InputTag( 'TriggerResults','','' )
#)
#process.HLTAnalyzerEndpath = cms.EndPath( process.hltTrigReport )
