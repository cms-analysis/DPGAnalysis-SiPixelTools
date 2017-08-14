#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_L1Tech_BSC_minBias_threshold1_v*'],
    HLTPaths = ['HLT_Random_*'],
#    HLTPaths = ['HLT_ZeroBias_*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v*'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
# Alca
#    HLTPaths = ['AlCa_LumiPixels_Random_v*'],
#    HLTPaths = ['AlCa_LumiPixels_ZeroBias_v*'],
# examples
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )


# process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('dumper'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

#process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = 'Debug'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(10)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('noise.root')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 

"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/04BBD43B-0876-E711-8C3E-02163E0119F7.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/04CC776E-0376-E711-850D-02163E0141D5.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/0624BE8A-0A76-E711-9D53-02163E011825.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/086C8B4F-0676-E711-8081-02163E01A20D.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/1C743D35-1676-E711-B5A3-02163E013412.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/1EF32E71-0376-E711-B413-02163E011B72.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/206382F8-0676-E711-B222-02163E011D78.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/22A3A271-0376-E711-9918-02163E013734.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/24C3FE4A-0676-E711-8961-02163E01A5FB.root",
"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/288C0969-0676-E711-A54D-02163E01384A.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/702/00000/FED4EA05-D64F-E711-82F9-02163E012A6B.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/426B4782-FC43-E711-9328-02163E011A76.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/F853E939-FF43-E711-A59D-02163E013479.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/209/00000/005E61C9-D341-E711-BEAE-02163E019C9F.root",

# cosmics  2017
#"/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/294/307/00000/00ABDA7D-733B-E711-A08B-02163E01440D.root",

#"/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/291/877/00000/0C01F638-9923-E711-A733-02163E014409.root",
#"/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/292/080/00000/0014512B-9325-E711-8981-02163E01A28A.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/267/594/00000/0AD270D1-B8F0-E511-9A98-02163E01347C.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",

    )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205718:49-205718:734')


process.d = cms.EDAnalyzer("FindHotPixelFromRaw", 
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
    Fraction = cms.untracked.double(0.02)
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.dumper)

# process.ep = cms.EndPath(process.out)


