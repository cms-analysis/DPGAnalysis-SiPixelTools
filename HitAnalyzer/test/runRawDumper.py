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
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
    fileNames = cms.untracked.vstring(                          
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/44D191BB-BD6D-E811-8DBD-02163E01A000.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/205DC1C3-BE6D-E811-818A-FA163ED11D1B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/525E3ED6-BF6D-E811-A906-FA163E1E2BC2.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/C8B92F09-C06D-E811-B426-FA163EAE8BE0.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/026B6704-C06D-E811-81BD-FA163EE67851.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/18F09008-C56D-E811-9B8F-FA163EB85DCA.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/2EE1E466-C56D-E811-8A2D-FA163EFDB548.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/3C7B8E7A-C96D-E811-8533-FA163E1B4187.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/EE74FB7F-C96D-E811-8829-FA163E4B6F1D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/50D2737F-C96D-E811-83D2-02163E019F1A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/502EF9FB-CA6D-E811-BDC9-FA163EB0FD9C.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/E25C1CD2-CA6D-E811-A30A-FA163EE73BB9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/5A02B3E6-CA6D-E811-A6FA-FA163E949132.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/4E58B463-BC6D-E811-A1E3-FA163EB85ED6.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/204EE610-C56D-E811-BCE9-FA163E39C36B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/945D140A-C56D-E811-82D7-FA163E8B3219.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/1680D4EF-CA6D-E811-BD1A-FA163EB00D2E.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/D8B7B820-CB6D-E811-A42B-FA163EE3D42B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/6EE3AAFE-CA6D-E811-A3C0-FA163EE14499.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B0C5BE52-BC6D-E811-AE29-FA163E1BC813.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/462322D5-BF6D-E811-AA74-FA163E11A4DA.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/90E16754-C56D-E811-8717-FA163E95A69B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/CE88A87A-C96D-E811-9225-02163E017F56.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B2DBA265-BC6D-E811-9BA6-FA163E07A318.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/E4D9C693-BC6D-E811-BB1D-FA163E4EB847.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/6250038F-BD6D-E811-A156-FA163ECCF441.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/5E2587D9-BE6D-E811-AEA8-FA163EA2F461.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/665D330B-BF6D-E811-AE2B-FA163E390CE2.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/0A51C8EC-BF6D-E811-9E2B-FA163E7D17FA.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/FE228591-C56D-E811-9769-FA163E5FA5D4.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/88BB7C78-C96D-E811-9405-FA163EA348D9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/C866347B-C96D-E811-A142-FA163E7D61D9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B091F7C4-BD6D-E811-96B6-FA163EE1E03C.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/D4386C0A-C56D-E811-B30E-FA163E5FB578.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/AC30B816-C56D-E811-B7EE-FA163EE3824E.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/D0302D73-C56D-E811-8A20-FA163E700015.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/EC49ECBF-C56D-E811-8A47-FA163EA2F461.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/36CF1C80-C96D-E811-958F-FA163E0D11E3.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/2809BF7F-C96D-E811-AE8C-FA163E6F67BD.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/1EF5D2E7-CA6D-E811-9ABB-FA163EEE65D6.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B65EE293-BD6D-E811-86D1-FA163E0AFD9D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/10474CD8-BD6D-E811-ADA6-FA163E014A3A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/088556AB-BE6D-E811-8E20-FA163E4E616A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/E6C0F202-C06D-E811-9762-FA163ED1FB83.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B4578781-C56D-E811-82A6-FA163E209BDB.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/6A144A7A-C96D-E811-A6F7-FA163E786D96.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/FE1C0979-C96D-E811-9D2F-FA163E8E087D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/465EB679-C96D-E811-8BF7-FA163E5EA53C.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/0A3A629C-C96D-E811-9149-FA163E700015.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/1263C17F-C96D-E811-BF32-02163E01A048.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B8B4BE80-C96D-E811-83F3-FA163E2BE632.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/0E465981-C96D-E811-BD90-FA163E57A90D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/5A416FB9-CA6D-E811-9A19-FA163E5E2D2A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/4C300C18-CB6D-E811-87AB-FA163E92384D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/C0EB5274-BC6D-E811-9F83-FA163E8ED26E.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/2C47127A-BC6D-E811-83D4-02163E01A14B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/BEE2379E-BD6D-E811-9C53-FA163EB08204.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/58FE87BA-BE6D-E811-A8A8-02163E01A142.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/885CE0E7-BE6D-E811-B640-FA163EA87350.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/C47EB512-C56D-E811-B19D-02163E01A035.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/76A63072-C56D-E811-AB4A-FA163EF4F5A9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/FCDF2E6E-C56D-E811-9A71-FA163E1C5C83.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/D8DC428B-C56D-E811-BADB-FA163E67B66F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/FAAB1E79-C96D-E811-94DD-FA163ECD895B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B604EE80-C96D-E811-85DB-FA163E48B3F0.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/767F807C-C96D-E811-8107-FA163EDE01A4.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/8E849D97-C96D-E811-9703-FA163EC2044A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/9442F6BF-CA6D-E811-B97D-A4BF0112F728.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/6AF4AE01-CB6D-E811-B34F-FA163E92A483.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/68AC9E56-BC6D-E811-B3B0-FA163E36ECB4.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/B278049C-BE6D-E811-B54D-FA163EB28C2F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/7298A7D6-BF6D-E811-B628-FA163ECC735A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/5A58C336-BF6D-E811-88D7-FA163E5FA5D4.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/6682EF0F-C56D-E811-A27E-FA163E2D36A9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/209408A5-C86D-E811-8C1F-02163E019F92.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/FE6CE682-C96D-E811-B3DB-FA163E94CEF3.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/317/687/00000/8AF365FE-CA6D-E811-BCA6-FA163E30F0B0.root",

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


