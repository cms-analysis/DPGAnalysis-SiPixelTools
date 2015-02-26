#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms

process = cms.Process("cluTest")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Random_v*'],
    HLTPaths = ['HLT_ZeroBias_*'],
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
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
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

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(    

#  "/store/data/Commissioning2015/Cosmics/RECO/PromptReco-v1/000/233/238/00000/FCD10A5A-8DA9-E411-98F3-02163E0121B3.root",

  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/FEFCE925-8EB2-E411-A251-02163E011D70.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/00097CE4-A2B2-E411-8FEB-02163E011D8A.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/0049B70A-8EB2-E411-8979-02163E011D1C.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/004DEB4A-9DB2-E411-AB38-02163E011D06.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/00D04605-A5B2-E411-A496-02163E0129F4.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/00D21ADC-A2B2-E411-8861-02163E011D57.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/02035AA7-A6B2-E411-B294-02163E011885.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/021D699B-9DB2-E411-8154-02163E011DB4.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/027D5566-A4B2-E411-B4F1-02163E0121AD.root",
  "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/029BFD10-8EB2-E411-8415-02163E012593.root",


# for MC 
#  'file:clus.root'
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/clus/clus1.root'
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/rechits/rechits1.root'
#  'file:../../../../../CMSSW_7_0_0_pre8/src/EventFilter/SiPixelRawToDigi/test/digis.root'
#   'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_postls171.root'
#   'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_mc71.root'


  )
)

# for data 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('206940:0-206940:1027')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# Choose the global tag here:
# 2012
#process.GlobalTag.globaltag = 'GR_P_V40::All'
# MC 2013
# process.GlobalTag.globaltag = 'MC_70_V1::All'
# DATA 2014
#process.GlobalTag.globaltag = 'PRE_R_71_V3::All'
# MC 2014
#process.GlobalTag.globaltag = 'PRE_STA71_V4::All'
# 2015
process.GlobalTag.globaltag = 'GR_P_V49::All'

process.analysis = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("siPixelClusters"),
)

# for data
#process.p = cms.Path(process.hltPhysicsDeclared*process.hltfilter*process.analysis)
#process.p = cms.Path(process.hltPhysicsDeclared*process.analysis)
# for MC
process.p = cms.Path(process.analysis)

# define an EndPath to analyze all other path results
#process.hltTrigReport = cms.EDAnalyzer( 'HLTrigReport',
#    HLTriggerResults = cms.InputTag( 'TriggerResults','','' )
#)
#process.HLTAnalyzerEndpath = cms.EndPath( process.hltTrigReport )
