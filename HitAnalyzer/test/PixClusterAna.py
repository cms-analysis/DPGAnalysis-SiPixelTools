#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("cluTest")
                   
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")


#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v0' # for 254227
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')


import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias	
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Physics_part*'],
#    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
    HLTPaths = ['HLT_ZeroBias_*'],
#    HLTPaths = ['HLT_PAZeroBias*'],
#    HLTPaths = ['HLT_PARandom*'],
#    HLTPaths = ['HLT_PAMinBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1Tech5_BPTX_PlusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech6_BPTX_MinusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech7_NoBPTX_v*'],
#
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

# to select PhysicsBit
#process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
#process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

# i do not know what is this doing?
#triggerSelection = cms.EDFilter( "TriggerResultsFilter",
#    triggerConditions = cms.vstring(
#    'HLT_ZeroBias / 1' ),
#    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
#    l1tResults = cms.InputTag( "gtDigis" ),
#    l1tIgnoreMask = cms.bool( True ),
#    l1techIgnorePrescales = cms.bool( True ),
#    daqPartitions = cms.uint32( 1 ),
#    throw = cms.bool( True )
#)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelClusters'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])


process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
  fileNames = cms.untracked.vstring(    
# muonia 
"/store/data/Run2015A/MuOnia/RECO/PromptReco-v1/000/248/038/00000/22A90F7F-5A14-E511-AD96-02163E012BE3.root",
#single muon 
#"/store/data/Run2015A/SingleMuon/RECO/PromptReco-v1/000/248/038/00000/0C5FA27E-5A14-E511-8D85-02163E012385.root",
# charmonium
#"/store/data/Run2015A/Charmonium/RECO/PromptReco-v1/000/248/038/00000/FC099721-5A14-E511-A3D1-02163E012B9A.root",
#
#"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/248/025/00000/50B4C032-0314-E511-A718-02163E013569.root",
#"/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/248/025/00000/08590F92-0115-E511-9483-02163E011C17.root",
#"/store/data/Run2015A/ZeroBias/RECO/PromptReco-v1/000/248/025/00000/30A880F2-0414-E511-9E31-02163E0139A3.root",
#"/store/data/Run2015A/ZeroBias1/AOD/PromptReco-v1/000/248/025/00000/341760D5-5114-E511-A92E-02163E0129E4.root",

#"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/247/607/00000/681AB913-9110-E511-98DA-02163E014374.root",
#"/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/246/963/00000/00F51FE4-090C-E511-8F33-02163E014299.root",
#"/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/963/00000/0006E9B1-4A0A-E511-AD47-02163E0138BC.root",

#  "file:/afs/cern.ch/work/d/dkotlins/public/data/clus/clus_zb_248025.root"
#  "file:../scripts/clus.root"
# MC
#"/store/relval/CMSSW_7_1_0_pre7/RelValTTbar/GEN-SIM-RECO/PRE_STA71_V3-v1/00000/887DEA5B-5CD1-E311-BB97-002618943923.root",
#"/store/relval/CMSSW_7_1_0_pre7/RelValTTbar/GEN-SIM-RECO/PRE_STA71_V3-v1/00000/8EEC0F1F-9FD1-E311-966F-003048FFD76E.root",
## 2012, cosmics
# "/store/data/Commissioning12/MinimumBias/RECO/PromptReco-v1/000/186/791/CEA46376-7069-E111-B395-001D09F24D67.root",
# "/store/data/Commissioning12/Commissioning/RECO/PromptReco-v1/000/186/791/6EC3470C-6F69-E111-93CA-001D09F241B9.root",
# "/store/data/Commissioning12/Cosmics/RECO/PromptReco-v1/000/186/791/6A54D2A0-6D69-E111-ABA8-001D09F2441B.root",

  )   # end the list "by-hand"

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('207469:0-207469:51')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('207477:76-207477:570')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('208686:73-208686:463')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('210534:24-210534:347')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana.root')
)


process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)


# define an EndPath to analyze all other path results
#process.hltTrigReport = cms.EDAnalyzer( 'HLTrigReport',
#    HLTriggerResults = cms.InputTag( 'TriggerResults','','' )
#)
#process.HLTAnalyzerEndpath = cms.EndPath( process.hltTrigReport )
