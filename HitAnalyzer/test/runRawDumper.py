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
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/0E1EB3EB-1B09-E611-8E98-02163E0141B5.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/0E8F788A-0A09-E611-8B1F-02163E011F57.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/10458EAC-0B09-E611-AC3F-02163E012B1F.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/14C7732E-0509-E611-86E4-02163E014115.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/1671F670-FD08-E611-A79B-02163E012950.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/1A009A12-0C09-E611-BB4E-02163E0125FE.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/1A4AA82E-0609-E611-9D59-02163E014308.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/1AB012CC-0B09-E611-91AF-02163E01442F.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/1AC6E6A1-0A09-E611-BE16-02163E0144F0.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/1C372FF8-1B09-E611-9A0B-02163E013906.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/1CB90251-0909-E611-955E-02163E014126.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/24A5A20A-0C09-E611-A5E8-02163E01203B.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/28F02A50-1C09-E611-BEEB-02163E014111.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/2A047BCA-FF08-E611-9102-02163E01413C.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/327DC411-0C09-E611-84F9-02163E01341C.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/364B89DE-1B09-E611-A40E-02163E013489.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/40148DE3-0309-E611-99FA-02163E014115.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/468DE272-0909-E611-B121-02163E014260.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/482B336B-FD08-E611-81BC-02163E014150.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/48C0355B-FD08-E611-A3D8-02163E01182E.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/4ACCF76C-FD08-E611-8EDB-02163E012833.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/50BDAE3B-1C09-E611-814E-02163E013429.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/52B54C5C-0909-E611-BBF5-02163E01389A.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/548953E9-0D09-E611-B917-02163E011CA3.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/566B7265-0C09-E611-AB46-02163E0133B8.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/5A4C8C8B-0A09-E611-94D9-02163E012B1F.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/5E37BFE4-1B09-E611-8895-02163E0145E5.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/5E7B5353-FD08-E611-8DB8-02163E014641.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/605B783A-0509-E611-80BE-02163E011CDA.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/64CE3022-0C09-E611-9417-02163E01345A.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/66B78F9D-0A09-E611-9EA8-02163E01202A.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/66E9F01C-0C09-E611-AEF6-02163E013705.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/68604072-FD08-E611-97DC-02163E01210E.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/6C6802E6-0B09-E611-B4B0-02163E012259.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/6C7FED60-FD08-E611-A974-02163E013606.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/6E3756CF-1B09-E611-8590-02163E0140D5.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/72427391-0A09-E611-BE7F-02163E011DB6.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/801356DD-0B09-E611-96CB-02163E0120F6.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/809F7C13-0A09-E611-AFFD-02163E014115.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/82715BBE-0B09-E611-B71A-02163E013653.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/884CE587-0909-E611-A327-02163E0120F6.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/88526ABC-0D09-E611-8AC7-02163E014182.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/8C166857-FD08-E611-8AED-02163E01476A.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/9044E3C4-F608-E611-B1CD-02163E01436C.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/904909FD-0B09-E611-AEB0-02163E01387B.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/964F6B07-0C09-E611-8E1B-02163E013575.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/96AD15D7-0B09-E611-B4A7-02163E0144F0.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/96D95CDE-0909-E611-909B-02163E013745.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/98340914-0C09-E611-9744-02163E01435A.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/9C2F48D0-1B09-E611-894A-02163E01475F.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/9EE691B7-0909-E611-8D95-02163E0146A9.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/A2A1D59C-F408-E611-BF7A-02163E01442F.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/A4B3FD97-F408-E611-95C1-02163E013537.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/AC183634-1C09-E611-ACF9-02163E0144F4.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/AC3F1810-0A09-E611-AEF3-02163E011F57.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/B03198C8-0D09-E611-ACF6-02163E01391C.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/B213B7B1-0D09-E611-B0F0-02163E0144F0.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/B2D9D833-0309-E611-BA11-02163E011A01.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/B44A03CE-1B09-E611-950D-02163E0125FE.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/B6720ECC-0B09-E611-B3B5-02163E013745.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/B6D6EE97-0A09-E611-B5AA-02163E011FB7.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/BAA363D6-0D09-E611-8D90-02163E013906.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/BE842797-1C09-E611-AC6B-02163E011CA3.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/C2225A19-0C09-E611-8C2D-02163E01376A.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/C2AAD143-0509-E611-A61C-02163E0133B8.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/C8128EB6-0D09-E611-A1B9-02163E0143BC.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/C83CFED0-0D09-E611-9DA2-02163E011DB6.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/CA77A3DF-0D09-E611-8761-02163E0140DF.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/CAFDCE09-0A09-E611-8DA1-02163E01436C.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/CEB091D6-FF08-E611-ADAF-02163E012417.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/D036CB0C-1C09-E611-BF0C-02163E0135C0.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/D81829CD-FF08-E611-ADB6-02163E014330.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/DE182658-0309-E611-B1CB-02163E0140F8.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/DEC40415-0C09-E611-A8F9-02163E01349D.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/E88C20F7-0909-E611-8F75-02163E0146D8.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/EA7446DF-1B09-E611-BEAC-02163E01397C.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/EC7971F7-0D09-E611-B973-02163E013959.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/EE7764EA-FF08-E611-8C41-02163E013745.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/EEFAF0C7-0D09-E611-929A-02163E014308.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/F064DAB1-0A09-E611-8EF9-02163E0144F4.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/F0E7A1E6-F408-E611-B4D1-02163E0141B5.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/F4A239BD-0D09-E611-B8B4-02163E012132.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/F4E3C7FD-1B09-E611-A98B-02163E01236F.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/F69DC2DD-1B09-E611-B80F-02163E0146B0.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/F6E5ED86-0909-E611-AF34-02163E0120F6.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/F8F20A4C-0909-E611-9754-02163E011DF9.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/FA0A897C-0909-E611-AB41-02163E011E2A.root",
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/FCD7D454-FD08-E611-85A9-02163E01351F.root",
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
    fileNames = cms.untracked.vstring(                          
# data 2016
"/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/056/00000/0097F016-0C09-E611-AA06-02163E011AE6.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/265/510/00000/02474E86-4BDC-E511-8222-02163E01364A.root",
#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/150/00000/1A844C25-14E5-E511-B9B5-02163E013471.root",
#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/277/00000/0004807C-AFE5-E511-83C1-02163E011EAD.root",

# "/store/data/Run2015D/ZeroBias/RAW/v1/000/258/655/00000/0C7D4CCE-886E-E511-A789-02163E0142E2.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/258/655/00000/",


   )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('257487:50-257487:9999')

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
    Verbosity = cms.untracked.int32(0),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


