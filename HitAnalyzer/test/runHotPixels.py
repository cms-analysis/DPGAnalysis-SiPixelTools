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
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('noise.root')
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist )
    fileNames = cms.untracked.vstring( 
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/E0500075-85F0-9448-9C5A-D73B5BE24F48.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/1DF822DF-BBD1-6A4D-AE46-75F38E222934.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/DE456DC5-BE67-0945-B957-B283B2DB3A13.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/34E3AEE6-FCDA-DA47-B9AC-8FF54C2C7AB4.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/04F8BE50-F85F-D649-AE1D-FCE64148D5FA.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/F95C5F72-B963-734F-8F5A-5FB74D071E17.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/5F9C3063-D4BE-F545-897D-7323C0A7DD44.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/71A9EB03-7C8E-B149-A4CC-063D046426A8.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/B97A932E-D6D3-7540-9A97-14AD266A18D9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/34E7E08A-9821-B743-8B59-1536357490CD.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/24587F65-7FDB-914F-A208-32C2757F0B37.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/5E53120E-5A86-DE49-8A56-701D4B0F1300.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/9D3FF135-090C-4F42-BB76-D9907D7E121C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/39CA6E94-EB55-C647-8DAB-0C3025B6CAF1.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/C4ECD544-517F-E64F-9BAB-D58AB97BB14F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/29A49B12-801D-B649-B913-86D0F2563052.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/D4A07703-649E-9B43-BD7B-DFC3B6D70DFF.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/64FCB3D3-D244-534A-ACDD-7E23B0317BD8.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/D707EB15-DE30-BB42-ACFB-DF9C521A902B.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/FC2EA487-3589-3344-8A54-81EEF26F134B.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/F4425038-D9E3-334F-B0E8-C8CFE4E7C22B.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/6A032568-3B63-3542-B0C4-434FDBBBAEA9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/A837AA60-479C-D743-AF1D-CFF6A15AD449.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/481/00000/8F262917-44C5-9347-A085-295863F6DF2C.root",

    )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('324410:0-324410:168')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('324276:48-324276:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('324075:0-324075:307')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('323829:0-323829:455')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('323481:38-323481:311')

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
    Fraction = cms.untracked.double(0.05),
    MAXFED = cms.untracked.int32(1293)  # bpix only 
    #MAXFED = cms.untracked.int32(1338)  # all  
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


