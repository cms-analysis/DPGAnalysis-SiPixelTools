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
    input = cms.untracked.int32(1000000)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('noise.root')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/029F1C01-52D1-E411-B6F2-02163E0133A3.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/0420C753-5ED1-E411-99FE-02163E0118C7.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/0C7DEE12-54D1-E411-8AD3-02163E012850.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/20A25B7E-65D1-E411-A4DF-02163E0127AF.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/285D8D30-64D1-E411-80C8-02163E011D73.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/2AF4488B-5AD1-E411-BF23-02163E01235C.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/32C8D19D-60D1-E411-B7A0-02163E011804.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/3A4E7160-5CD1-E411-BE9C-02163E011DB6.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/3AA134BF-50D1-E411-A939-02163E011DC7.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/3EE0E454-5ED1-E411-8EA9-02163E01275B.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/46AFA25B-54D1-E411-B39D-02163E011830.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/54663361-58D1-E411-BF4F-02163E0136B2.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/6A6EFB6A-5ED1-E411-B51F-02163E0123FC.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/6E1E0881-55D1-E411-A46D-02163E0134A2.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/7438DEB6-65D1-E411-85C6-02163E012452.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/782860B9-65D1-E411-90C0-02163E012669.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/7C709F55-5CD1-E411-B742-02163E01260B.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/8ABC60E5-60D1-E411-9891-02163E0128AC.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/8AF7FBDB-57D1-E411-9BC1-02163E012186.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/94FABBA2-5AD1-E411-A1A1-02163E01298B.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/988D30EA-57D1-E411-A1F6-02163E012087.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/AC7E5631-59D1-E411-ACC6-02163E013698.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/B0636CDB-5AD1-E411-91DD-02163E011DE2.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/B4B2C5F5-57D1-E411-A2E9-02163E011DE2.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/CA459CDF-57D1-E411-B2A7-02163E012201.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/CA7FCE74-65D1-E411-AA7A-02163E01235C.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/CE16416E-5ED1-E411-93C9-02163E01225F.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/DAA0385C-65D1-E411-96E9-02163E0123B9.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/F2CC8AE3-60D1-E411-A481-02163E011D13.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/238/751/00000/FE523C70-5ED1-E411-8E41-02163E01241E.root",

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
#   In 2012, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
#    InputLabel = cms.untracked.string('siPixelRawData'),
    CheckPixelOrder = cms.untracked.bool(False),
#   Fraction to define  noisy pixels 
    Fraction = cms.untracked.double(0.0001)
)

#process.p = cms.Path(process.hltfilter*process.dumper)
process.p = cms.Path(process.dumper)

# process.ep = cms.EndPath(process.out)


