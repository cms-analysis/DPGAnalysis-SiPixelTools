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
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/8D623D9E-D1D2-2443-852D-7F29D0F90366.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/1185D59C-0A0D-3946-B2E5-E2897731A02B.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/9AB03B76-821F-4A46-A62C-11870F0F9F34.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/D4E27A68-CE45-AA43-814F-B9FFB97CA36C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/94491697-99F5-CC4F-97DB-4E506FED40A7.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/B109921A-4712-2E4D-9307-1F06CFAF802B.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/8627FDBF-B67C-9D49-961C-ED5D33B1378B.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/5BDDD23E-E8BE-5C44-A7DF-12538083FBC1.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/12B6FE36-7E73-1048-80F8-910AD6754884.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/98740235-C2D5-A749-9A01-4358AAFCE76A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/043015B8-CC99-F844-8FB6-33283FDEB4F9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/817B8E2A-04C6-7D44-BEAF-2F4D29A7C38A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/4647AFC5-8F91-564E-8B41-F67CDC9AB91F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/14E962BF-2193-C54F-9902-3DC3C18CAD9C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/AFFA9556-E204-BC47-8CF6-B151FA766A00.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/024B1158-065A-8D49-925A-F549934EE9BF.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/2FF32EFA-D39D-C44E-9C3D-C6D64CC0A7DE.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/4397C9DB-02E1-6545-9D88-6D87695E3909.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/89D5C4BC-0050-2444-B864-C6315C5BBC35.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/146337E1-1299-9446-B2BF-A9AE91A667AD.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/F34CC664-6F0A-8544-B0D3-86B46BA1E7D9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/5E8A33E8-725A-4D46-9E74-8DC5FC2553E6.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/618727CF-270A-2C40-9D2B-A25C74245DB1.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/410/00000/22B39490-06CA-9B4F-93B3-18802FE9D730.root",


    )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('324410:0-324410:168')

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


