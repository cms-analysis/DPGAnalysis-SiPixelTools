#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],  # not in ZB stream
#    HLTPaths = ['DST_Physics_v*'], useless 
    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_ZeroBias*'],
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
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00030CAB-6C1E-E611-90F9-02163E0137A8.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/005C7BCB-471E-E611-89AB-02163E0141C1.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/0092140B-431E-E611-B190-02163E01399A.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/009B159B-581E-E611-90E1-02163E01265D.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C0B635-611E-E611-9B9C-02163E011CFC.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C82263-741E-E611-B7F7-02163E014147.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C8C916-561E-E611-8D0B-02163E0146A2.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00D87AA4-4D1E-E611-9895-02163E0127A7.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00ED8C57-4B1E-E611-AAE8-02163E0133C1.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/0203E153-6A1E-E611-B6ED-02163E011CAD.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/02132225-6B1E-E611-AD96-02163E014765.root",

# 273730
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/730/00000/00821BA0-381F-E611-BBC5-02163E0142C0.root",

# 274314
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/00ABDB9B-0E28-E611-9406-02163E014602.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/04B93E10-4328-E611-A8CA-02163E0135D0.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/0AA8AD56-4328-E611-8758-02163E013975.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/0C60C3F9-4328-E611-9C46-02163E014340.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/1009B80A-4628-E611-9E6E-02163E014587.root",

# 275828
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/002CE548-BE3B-E611-8A2D-02163E012602.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/0274712A-C03B-E611-8709-02163E0118C8.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/02E3DE60-BC3B-E611-B926-02163E014540.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/065E2CAB-BE3B-E611-9106-02163E0144CC.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/0EC5F068-BE3B-E611-B112-02163E014315.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/100984B7-B43B-E611-91B8-02163E011A7C.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/108204F4-BA3B-E611-8752-02163E014121.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/1815525F-BC3B-E611-8F2F-02163E011DF0.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/184260D7-B33B-E611-9159-02163E01394C.root",
#"/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/828/00000/1857DCC1-BA3B-E611-9185-02163E014518.root",

# 277069
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0004128B-E74D-E611-A955-FA163E7DD696.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0049EB23-EA4D-E611-902C-FA163E652810.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/00FBA566-F94D-E611-B793-FA163E7C989C.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0201B2FC-F94D-E611-9813-02163E01218E.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/022B0DA1-F34D-E611-905E-02163E0142D3.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/02827EA1-F34D-E611-BB0A-02163E014548.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0405AC07-EF4D-E611-AB30-02163E01449E.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/046D401F-F94D-E611-B1BF-02163E01474F.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0489977E-EA4D-E611-9DB3-FA163EC974A2.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/04BF9517-F54D-E611-B42B-02163E0144D3.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0639477C-F44D-E611-A5F4-02163E011A3F.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0642A9F2-F44D-E611-AE91-02163E0138C1.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/068EF33F-F34D-E611-87E6-02163E012726.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/06B59B88-F94D-E611-A175-02163E014146.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/06D39F44-E94D-E611-9A05-02163E0133B0.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0818E287-F94D-E611-BD33-02163E0144B0.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/08E6E065-EF4D-E611-8820-02163E01444C.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0A1FE33B-F04D-E611-B11F-02163E011B76.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0A57D891-F94D-E611-AB10-02163E01419D.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0CC25B50-F54D-E611-A641-FA163EA74264.root",

   )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('257487:50-257487:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('273725:83-273725:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('274314:97-274314:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('275828:69-275828:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277069:80-277069:9999')

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

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


