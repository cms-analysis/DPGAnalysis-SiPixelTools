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
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
    fileNames = cms.untracked.vstring(                          

"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/00097234-762C-E811-9C01-FA163E376AE7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/002FAD18-AE2C-E811-AE7F-FA163E595F01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/0068E3AB-BF2C-E811-A765-FA163E49E4DD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/00797185-882C-E811-A3AC-FA163ECCEC77.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/008A0155-8B2C-E811-BDAB-02163E0176B0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/028FFAB8-862C-E811-BCFE-FA163E024A59.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/02BCB4EF-772C-E811-9F81-FA163EFD5681.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/041065A5-CF2C-E811-B1ED-FA163E95BBB1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/0422A8DA-B82C-E811-9045-FA163E252608.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/0430A381-952C-E811-8467-FA163E468C72.root",

# 2017
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v3/000/300/742/00000/00538B29-837C-E711-BCD7-02163E011F61.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/02DBAD4C-AE7C-E711-B44F-02163E019DD0.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/04985C0E-B17C-E711-8A86-02163E0146AC.root",

#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v2/000/300/233/00000/062F0768-5676-E711-B303-02163E014626.root",
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v2/000/300/226/00000/02BBC9F3-2F76-E711-B29A-02163E019C08.root",
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/300/155/00000/0052B596-8F75-E711-B704-02163E011C13.root",

#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/300/122/00000/001BAA8B-BA74-E711-9564-02163E01A3CB.root",
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/300/088/00000/003E6DDF-4974-E711-A723-02163E0133C2.root",

# "/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/299/479/00000/000AE42E-696D-E711-AC50-02163E0142C8.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/702/00000/FED4EA05-D64F-E711-82F9-02163E012A6B.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/664/00000/00F38C8D-A54F-E711-A312-02163E01472F.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/643/00000/08070285-5E4F-E711-BB56-02163E01476C.root",

   )

)



# for dump files 
#process.source = cms.Source("PixelSLinkDataInputSource",
#    fedid = cms.untracked.int32(-1),
#    runNumber = cms.untracked.int32(-1),
#    #fileNames = cms.untracked.vstring('file:../phase1/PixelAlive_1293_1018.dmp')
#    fileNames = cms.untracked.vstring(
#     #'/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_300/GainCalibration_1293_1015.dmp'
#     #'/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_300/GainCalibration_1200_1015.dmp'
#     '/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_300/GainCalibration_1300_300.dmp'
#    )
# )

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('278509:91-278509:9999')


process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2015 data, label = rawDataCollector, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),

# for MC
#    InputLabel = cms.untracked.string('siPixelRawData'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),

# for dump files 
#    InputLabel = cms.untracked.string('source'),
# old
#    InputLabel = cms.untracked.string('siPixelRawData'),
#    InputLabel = cms.untracked.string('source'),
#    InputLabel = cms.untracked.string("ALCARECOTkAlMinBias"), # does not work

    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(0),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


