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
#    HLTPaths = ['HLT_ZeroBias_v*'],
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


"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/6DC681B8-12C2-534B-922C-568CAFEFBA23.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/44ED9E8D-F71B-4540-9DA9-A476E705CADE.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/ED612143-02C5-C742-A268-D003C747F9C2.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/350D164E-3069-F04E-9F19-C0A454843DCB.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/AAB64A7D-0F39-FC45-BA97-45D28FC8E3CC.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/302034FE-E606-904A-A8E0-4CCA4A251C8A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/3ACA86EB-0D8D-4244-A5A2-39884AB523A9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E14E1734-7B44-0249-A375-E2439155D574.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/F086480A-5AB6-A14C-908A-91CD5C255C24.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/2027DE44-C425-F24B-A84B-3F0F95C270FC.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E059BE35-1B40-9A40-8CC0-EF9F64B0216C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/A5AA5CEA-D219-3B49-997E-E0E8409E123E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/CCD4B54B-7D9D-854E-9AF0-9609F3919316.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/A852B0F3-5435-9341-9023-B0FEE829EDF2.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/617D0DC5-580F-8141-94B5-C5CA46A8C7E9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/2BAED97D-1E7E-A847-A927-639A481D0F3A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/B6B7D160-B11A-F948-8D58-74F9D0064E4F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/FDD637D3-29CA-BD4A-A2B0-9C76145D89F4.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/79CB1294-16A9-BA4A-A4BC-B53DD52A2C4F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/0282F21A-F0AF-F04E-B9E1-B8D07E81269C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/90C23117-F8C0-7041-98E2-2830101292E9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/EC806BEB-271F-B84A-9AE0-D940E9B9DB8E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/7BDAE626-99D5-6449-8155-DF4B9F4B61DF.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E65A59C7-C58B-7E4B-8EEA-30F39C4E1B8E.root",

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

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


