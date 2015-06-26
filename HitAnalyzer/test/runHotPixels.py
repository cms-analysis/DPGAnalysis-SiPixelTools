#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_L1Tech_BSC_minBias_threshold1_v*'],
    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_ZeroBias_v*'],
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

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/08C785B8-6118-E511-968A-02163E01190E.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/0A70F9C1-6118-E511-813C-02163E0141BC.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/0CB1A6BD-6118-E511-8764-02163E01374A.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/0EC23CCC-AA17-E511-B927-02163E01433C.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/16AA697F-7B17-E511-BE37-02163E011B15.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/1C1952AC-AA17-E511-A1F7-02163E011AAE.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/22F306B8-AA17-E511-A6AD-02163E0138D3.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/24730200-9017-E511-AB5A-02163E0118F6.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/260A8728-B817-E511-9FC8-02163E01186A.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/26688B1F-8717-E511-8EE8-02163E01438B.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/2CE1E2BD-6118-E511-B862-02163E013924.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/30E152B8-6118-E511-B40C-02163E01187D.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/38A569DB-7517-E511-8CFB-02163E013391.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/3C1E88BC-6118-E511-820B-02163E011B61.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/3C21E665-9917-E511-995D-02163E013700.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/42D26FBB-6118-E511-B7D3-02163E0134D1.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/448C89C2-6118-E511-BAA2-02163E0136C3.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/48A77B19-8C17-E511-9DD5-02163E0138C6.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/4E35F803-9B17-E511-B18F-02163E0138D3.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/547057C2-AA17-E511-BF90-02163E011BA7.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/5AB0AEE8-8717-E511-8EAB-02163E01392A.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/5C65E9B7-6118-E511-A964-02163E011BAC.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/60B751B8-6118-E511-9DD7-02163E014459.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/6641853B-8F17-E511-AFFF-02163E011DB6.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/682DC6B6-6118-E511-BA98-02163E011B79.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/70B61C72-8317-E511-8116-02163E013446.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/72D313AC-8817-E511-A02A-02163E011BA0.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/781040BD-6118-E511-949B-02163E0136C2.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/845BC1C6-6118-E511-8419-02163E011E9F.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/86489ABB-AA17-E511-82AB-02163E011B79.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/86C884B7-6118-E511-AA73-02163E0142B5.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/8C1DAFC2-BD17-E511-9A72-02163E014118.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/8E54A448-B917-E511-9D7A-02163E0134EB.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/904714CA-8717-E511-AE5D-02163E012444.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/9048AFB7-6118-E511-8FD8-02163E013642.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/90871332-8117-E511-A7ED-02163E0139A3.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/94E664BA-6118-E511-AB14-02163E011B58.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/96049F4F-B417-E511-BF62-02163E01394E.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/986D0CDB-7317-E511-B721-02163E013772.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/98CE6EEC-8F17-E511-94BA-02163E013844.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/9A1ACC16-B017-E511-9AC1-02163E01434F.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/9A723698-5218-E511-BD86-02163E01433C.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/9E2CFEBC-6118-E511-888A-02163E01358B.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/A0C2031B-7117-E511-8715-02163E01356D.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/A242A6AF-B217-E511-90D6-02163E01392B.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/A8A38A4E-B417-E511-AB34-02163E01361A.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/AA48A4B7-6118-E511-9525-02163E01441F.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/AEACEBB7-6118-E511-A785-02163E012973.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/B00E03DC-9F17-E511-B0DD-02163E013985.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/B031DFDE-9817-E511-9549-02163E0139DE.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/B0B8E9B7-9C17-E511-A842-02163E01261E.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/B41126B8-6118-E511-92B3-02163E0119CE.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/BE37EB38-AB17-E511-A658-02163E0136C2.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/C2B6081B-B017-E511-B23D-02163E01392B.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/C8A2DCB6-6118-E511-B585-02163E013833.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/CA27C3B7-6118-E511-915C-02163E013835.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/CAB09FFE-6F17-E511-B987-02163E011D4A.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/CACA702F-6218-E511-94D5-02163E0141BC.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/D4469EBD-6118-E511-A4E3-02163E013502.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/D69787DA-9517-E511-BD50-02163E011E4B.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/D8F33DE6-9817-E511-8B6D-02163E011B58.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/DA26C5B5-6118-E511-8E55-02163E012A22.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/DEEBA279-7F17-E511-84F0-02163E011EDB.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/E411309F-7817-E511-9F4A-02163E0133DC.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/E4DB54B7-6118-E511-8942-02163E011BD8.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/E84EF39C-7617-E511-A8FC-02163E013970.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/EC9964B0-BB17-E511-B493-02163E011929.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/EEC47FF2-7C17-E511-9C3B-02163E0138D0.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/F0530D94-5218-E511-A2BE-02163E011B79.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/F27086A9-AA17-E511-95CD-02163E01441F.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/F47AD6B8-6118-E511-9F7E-02163E0138BE.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/FC1B9CB8-6118-E511-B098-02163E0133E6.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/FC53F236-9217-E511-BA60-02163E013463.root",
"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/FE2A5536-B617-E511-8FB5-02163E0138F9.root",


#"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/607/00000/20B0B95A-4418-E511-94BB-02163E012973.root",
#"/store/data/Run2015A/Cosmics/RAW/v1/000/248/607/00000/EEAC196C-4418-E511-B6A7-02163E014118.root",


#"/store/data/Run2015A/AlCaLumiPixels/RAW/v1/000/247/324/00000/3CF692E4-0C0D-E511-B3EE-02163E012A7A.root",
#"/store/data/Run2015A/AlCaLumiPixels/RAW/v1/000/247/324/00000/42176018-080D-E511-A3D4-02163E011E08.root",
#"/store/data/Run2015A/AlCaLumiPixels/RAW/v1/000/247/324/00000/482293ED-0C0D-E511-92AD-02163E0143E9.root",
#"/store/data/Run2015A/AlCaLumiPixels/RAW/v1/000/247/324/00000/D237CDCC-170D-E511-ABCB-02163E01432C.root",

#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/FABFBF61-530D-E511-B20B-02163E013653.root",

#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/029F1C01-52D1-E411-B6F2-02163E0133A3.root",
#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/0420C753-5ED1-E411-99FE-02163E0118C7.root",
#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/0C7DEE12-54D1-E411-8AD3-02163E012850.root",
#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/20A25B7E-65D1-E411-A4DF-02163E0127AF.root",
#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/285D8D30-64D1-E411-80C8-02163E011D73.root",
#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/2AF4488B-5AD1-E411-BF23-02163E01235C.root",
#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/32C8D19D-60D1-E411-B7A0-02163E011804.root",
#"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/3A4E7160-5CD1-E411-BE9C-02163E011DB6.root",

#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",

    )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205718:49-205718:734')


#process.out = cms.OutputModule("PoolOutputModule",
#    fileName =  cms.untracked.string('file:histos.root')
#)

#process.dumper = cms.EDAnalyzer("findHotPixels", 
process.dumper = cms.EDAnalyzer("FindHotPixelFromRaw", 
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
    Fraction = cms.untracked.double(0.01)
)

process.p = cms.Path(process.hltfilter*process.dumper)
#process.p = cms.Path(process.dumper)

# process.ep = cms.EndPath(process.out)


