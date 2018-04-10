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
#    input = cms.untracked.int32(1000)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('noise.root')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/EA0EE7D9-943A-E811-BE36-02163E013DCD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/523385A7-973A-E811-BCB9-02163E00B53D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/1E14D2B7-973A-E811-ABD3-FA163EE655A1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/12CADCB2-973A-E811-B519-FA163E2BCBE1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/D6FEB8CD-983A-E811-B321-FA163E2BCBE1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/98667B60-A13A-E811-B138-02163E01762E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/6419ECE2-933A-E811-9FC4-FA163EA6A0C9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/FE3DA187-943A-E811-A62B-FA163ED596E6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/F03C830A-993A-E811-8DC5-FA163EF5C888.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/F80EBCFF-9A3A-E811-8149-FA163E355523.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/76B72AC9-9B3A-E811-9EA5-FA163E94A9FA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/34C7AA20-9C3A-E811-AD23-02163E0160B2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/6EF67AAA-9C3A-E811-BE8F-FA163E287A7D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/BA46975E-9E3A-E811-BCA0-FA163E355523.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/92774696-9F3A-E811-A6DB-FA163EB78729.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/70868229-A23A-E811-B017-FA163EB5448B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/1EF55363-A23A-E811-9D47-FA163E456877.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/759/00000/4C1B9186-A23A-E811-AEE7-FA163E427FAE.root",
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
    Fraction = cms.untracked.double(0.01),
    #MAXFED = cms.untracked.int32(1293)  # bpix only 
    MAXFED = cms.untracked.int32(1338)  # all  
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


