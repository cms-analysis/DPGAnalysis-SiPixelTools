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
    input = cms.untracked.int32(100)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('raw.root')
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/0060618D-2850-E611-AEB8-02163E01375C.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/00BCD7DD-1950-E611-8522-FA163E13172F.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/00E3D746-1150-E611-AE9D-02163E01462B.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/020EBDBF-1250-E611-90A5-02163E011B8B.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/0273015B-1E50-E611-B7F2-02163E0137F3.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/028C4B20-2350-E611-8133-02163E013625.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/02EB69ED-1950-E611-9FFF-FA163ED73630.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/02FB17F3-2850-E611-97ED-02163E0121A0.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/043B1B81-2350-E611-8A71-02163E013650.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/04982CD7-2250-E611-924E-02163E011863.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/0606A256-2250-E611-9E59-02163E013526.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/060E8C20-0250-E611-AA27-02163E01247E.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/064935B5-1A50-E611-8311-02163E01374E.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/06709141-1A50-E611-9FDA-02163E011AD9.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/068C0F1A-1150-E611-B4EB-FA163E0C4781.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/148/00000/06957A3B-0D50-E611-95DB-02163E014517.root",
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
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0004128B-E74D-E611-A955-FA163E7DD696.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0049EB23-EA4D-E611-902C-FA163E652810.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/00FBA566-F94D-E611-B793-FA163E7C989C.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0201B2FC-F94D-E611-9813-02163E01218E.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/022B0DA1-F34D-E611-905E-02163E0142D3.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/02827EA1-F34D-E611-BB0A-02163E014548.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0405AC07-EF4D-E611-AB30-02163E01449E.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/046D401F-F94D-E611-B1BF-02163E01474F.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0489977E-EA4D-E611-9DB3-FA163EC974A2.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/04BF9517-F54D-E611-B42B-02163E0144D3.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0639477C-F44D-E611-A5F4-02163E011A3F.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0642A9F2-F44D-E611-AE91-02163E0138C1.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/068EF33F-F34D-E611-87E6-02163E012726.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/06B59B88-F94D-E611-A175-02163E014146.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/06D39F44-E94D-E611-9A05-02163E0133B0.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0818E287-F94D-E611-BD33-02163E0144B0.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/08E6E065-EF4D-E611-8820-02163E01444C.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0A1FE33B-F04D-E611-B11F-02163E011B76.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0A57D891-F94D-E611-AB10-02163E01419D.root",
# "/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0CC25B50-F54D-E611-A641-FA163EA74264.root",

# 277087
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0014F268-DC4E-E611-AC39-02163E0141E7.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0096FD8F-C34E-E611-88C6-FA163E808E33.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/00B50BAD-CE4E-E611-9B51-02163E0133A7.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/00BB4D52-C84E-E611-B37B-FA163E0FB798.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0230860F-D44E-E611-B235-02163E012A56.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0286D58A-AE4E-E611-8550-FA163E0AD718.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0484E306-CF4E-E611-B348-FA163E7CF9DE.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/04D84910-CA4E-E611-AF26-02163E01198A.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/04DBB221-D44E-E611-A78F-02163E0133A7.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/04DF1C91-BF4E-E611-A983-FA163E77A215.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0656C096-B14E-E611-B118-02163E0138B0.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/06CC1475-D14E-E611-A246-02163E012330.root"
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/080D58E2-B54E-E611-B12F-02163E013736.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/081B3554-D14E-E611-ABB8-02163E011CD3.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/08246E9C-BF4E-E611-9520-02163E01352C.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/082E7F8A-B14E-E611-88FA-02163E011A28.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/08864B7E-BC4E-E611-AF6C-02163E01357C.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/088AE68E-C64E-E611-811C-02163E014226.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/08F563EE-D34E-E611-84A5-02163E01200A.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0A1CAD74-D14E-E611-9184-02163E0143E6.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0A71202D-D84E-E611-9A29-FA163E50BF62.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0AA4F4A4-AE4E-E611-A03D-FA163E5259C4.root",
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/087/00000/0AC69253-D14E-E611-8341-FA163EB695A1.root",

#278193
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/04476347-4F5A-E611-AF5B-FA163EB8F285.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/063A87B8-515A-E611-9AF6-02163E0136A1.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/06EAFD59-525A-E611-BB68-02163E012626.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/084F25A3-525A-E611-9D33-02163E011CFE.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/08649C3B-425A-E611-BC0E-FA163EF071BF.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/08690E6C-545A-E611-A200-FA163EDD08C4.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/08DBAFB4-575A-E611-8BB2-FA163EDB8A5C.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/0E35562B-535A-E611-BFAF-02163E0134BC.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/0E5D88A4-525A-E611-BBFE-FA163E7939D9.root",
#"root://eoscms//eos/cms/tier0/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/0E6E4ACA-4F5A-E611-97B5-FA163E1E4ACD.root",

# 278509
#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/509/00000/000D37E3-915E-E611-A2CB-02163E011AAE.root",
#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/509/00000/002B0EDB-905E-E611-989E-FA163E721B2A.root",
#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/509/00000/0079B580-8C5E-E611-B895-02163E012797.root",
#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/509/00000/008E5634-915E-E611-9F52-02163E0140F7.root",
#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/509/00000/0094ACA0-915E-E611-9D06-02163E0143E4.root",

#"/store/data/Run2016F/ZeroBias/ALCARECO/TkAlMinBias-PromptReco-v1/000/278/509/00000/18648660-5460-E611-9CDB-FA163ED6B29A.root",

   )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('257487:50-257487:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('273725:83-273725:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('274314:97-274314:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('275828:69-275828:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277069:80-277069:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277148:83-277148:867')  # all
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277148:460-277148:560')  # part 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('278193:77-278193:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('278509:91-278509:9999')


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
#    InputLabel = cms.untracked.string("ALCARECOTkAlMinBias"), # does not work
    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(3),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


