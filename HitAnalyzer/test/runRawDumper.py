#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],  # not in ZB stream
#    HLTPaths = ['DST_Physics_v*'], useless 
#    HLTPaths = ['HLT_Random_v*'],
    HLTPaths = ['HLT_ZeroBias*'],
#    HLTPaths = ['HLT_L1Tech54_ZeroBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v1'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
# LumiPixels
#    HLTPaths = ['AlCa_LumiPixels_Random_v*'],
#    HLTPaths = ['AlCa_LumiPixels_ZeroBias_v*'],
#    HLTPaths = ['AlCa_LumiPixels_v*'],
    
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
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('raw.root')
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/0097F016-0C09-E611-AA06-02163E011AE6.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/00C1560E-0C09-E611-AC8C-02163E014593.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/04AA83EC-FF08-E611-BA1D-02163E0140D5.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/0AB10C4B-0609-E611-9727-02163E011FE5.root",
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
    fileNames = cms.untracked.vstring(                          
# data 2016
#"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/0097F016-0C09-E611-AA06-02163E011AE6.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/265/510/00000/02474E86-4BDC-E511-8222-02163E01364A.root",
#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/150/00000/1A844C25-14E5-E511-B9B5-02163E013471.root",
#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/277/00000/0004807C-AFE5-E511-83C1-02163E011EAD.root",

# "/store/data/Run2015D/ZeroBias/RAW/v1/000/258/655/00000/0C7D4CCE-886E-E511-A789-02163E0142E2.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/258/655/00000/",

# 273725
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00030CAB-6C1E-E611-90F9-02163E0137A8.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/005C7BCB-471E-E611-89AB-02163E0141C1.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/0092140B-431E-E611-B190-02163E01399A.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/009B159B-581E-E611-90E1-02163E01265D.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C0B635-611E-E611-9B9C-02163E011CFC.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C82263-741E-E611-B7F7-02163E014147.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C8C916-561E-E611-8D0B-02163E0146A2.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00D87AA4-4D1E-E611-9895-02163E0127A7.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00ED8C57-4B1E-E611-AAE8-02163E0133C1.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/0203E153-6A1E-E611-B6ED-02163E011CAD.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/02132225-6B1E-E611-AD96-02163E014765.root",

# 273730
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/730/00000/00821BA0-381F-E611-BBC5-02163E0142C0.root",

   )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('257487:50-257487:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('273725:83-273725:9999')

process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2015 data, label = rawDataCollector, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
# for MC
#    InputLabel = cms.untracked.string('siPixelRawData'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),
# old
#    InputLabel = cms.untracked.string('siPixelRawData'),
#    InputLabel = cms.untracked.string('source'),
    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(1),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


