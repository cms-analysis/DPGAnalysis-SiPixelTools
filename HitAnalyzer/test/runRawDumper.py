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

"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/62D40E91-CB32-E811-8231-FA163E2D0F8B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/38E3030F-CE32-E811-A369-FA163EAE8BE0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/980DD043-CF32-E811-9A2C-FA163E010862.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/C4763265-CF32-E811-9D1C-FA163E6E568E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/0C817D37-CE32-E811-ADB6-FA163E5CE18A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/80D62C1A-D232-E811-9890-FA163E045E3F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/944600B7-D332-E811-BB49-FA163E3FA786.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/92AD4812-D532-E811-BE0B-FA163E967705.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/FC076D0A-D632-E811-B2A4-FA163EE782AD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/86E740C4-D432-E811-9B2E-FA163E5CE18A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/7A1516F8-D632-E811-9FFD-FA163EE92B1D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/3C5A8030-D832-E811-94FB-FA163EE92B1D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/56825D7D-DA32-E811-A6FC-02163E019FF9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/CC7E7385-DB32-E811-95A3-FA163E9A30FF.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/DC065D53-DC32-E811-97F9-02163E01A01D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/313/105/00000/0CD8BFCA-DD32-E811-9563-FA163E014056.root",

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


