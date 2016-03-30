#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("Test")
                   
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v0' # for 254227
# 2016
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
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
    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
#    HLTPaths = ['HLT_ZeroBias_*'],
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

# POOLSOURCE
process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring()
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histos.root')
)

process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

process.a = cms.EDAnalyzer("FindHotPixelFromRaw", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#    InputLabel = cms.untracked.string('source'),
#   In 2012, 2015, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),
#    InputLabel = cms.untracked.string('siPixelRawData'),
    CheckPixelOrder = cms.untracked.bool(False),
#   Fraction to define  noisy pixels 
    Fraction = cms.untracked.double(0.01)
)


#process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d) # reco, cluster analysis 
process.p = cms.Path(process.hltfilter*process.a) # raw, noisy pixels


