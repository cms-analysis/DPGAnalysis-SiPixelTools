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

# data 2016
#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/265/510/00000/02474E86-4BDC-E511-8222-02163E01364A.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/150/00000/1A844C25-14E5-E511-B9B5-02163E013471.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/277/00000/0004807C-AFE5-E511-83C1-02163E011EAD.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/267/594/00000/0AD270D1-B8F0-E511-9A98-02163E01347C.root",

"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/267/594/00000/F6344265-C8F0-E511-81B3-02163E0122B6.root",

#"/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/277/00000/0004807C-AFE5-E511-83C1-02163E011EAD.root",

#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/411/00000/2227CD91-A61F-E511-8ABC-02163E011D1C.root",
#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/411/00000/D2A60B28-A71F-E511-9950-02163E012AEE.root",

#"/store/data/Run2015A/MinimumBias/RAW/v1/000/248/566/00000/FE2A5536-B617-E511-8FB5-02163E0138F9.root",
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


