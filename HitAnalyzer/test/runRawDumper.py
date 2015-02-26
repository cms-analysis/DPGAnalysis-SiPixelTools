#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Random_v*'],
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

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(                          
#    "file:../../../../../CMSSW_7_1_3/src/DPGAnalysis-SiPixelTools/HitAnalyzer/test/raw.root"
#    "file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/raw/raw2.root"

#

"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/00240D79-6DBB-E411-82BC-02163E012B43.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/0A42EC30-6DBB-E411-85FD-02163E012A56.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/2A6FB538-6DBB-E411-8C26-02163E012A88.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/40368531-6DBB-E411-9B0F-02163E01284F.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/42D15032-6DBB-E411-A803-02163E012B81.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/549A7B31-6DBB-E411-962B-02163E01181B.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/6EDD6832-6DBB-E411-9D63-02163E011CEE.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/7E4F0E33-6DBB-E411-94C4-02163E0123B3.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/9C7E9130-6DBB-E411-9F57-02163E011D8F.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/B0690E32-6DBB-E411-85C0-02163E011CD6.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/BA7B3D32-6DBB-E411-966A-02163E01221B.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/346/00000/F23C6E30-6DBB-E411-9B9C-02163E0126F3.root",


#  "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FEA5C616-3AA9-E411-8348-02163E0122A3.root",

#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/4829BD7C-57B2-E411-BCC4-02163E012700.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/6C81712E-57B2-E411-A619-02163E012B6C.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/CCF57A73-57B2-E411-87FE-02163E012BC5.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/EA1A5074-57B2-E411-8418-02163E0124AC.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/ECCA0A2B-57B2-E411-8338-02163E0125D7.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/FADADC33-54B2-E411-8539-02163E0125D7.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/FE8E1D08-55B2-E411-AA42-02163E01181F.root",


#  "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/026/00000/029FB859-68B2-E411-8145-02163E0122AB.root",

# fill 2576
##    "rfio:/castor/cern.ch/cms/store/data/Run2012A/LP_ZeroBias/RAW/v1/000/193/092/16131715-E893-E111-8CDB-001D09F27003.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/718/02285033-FD1B-E211-8F74-001D09F295FB. 
    )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('191271:55-191271:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('191718:30-191718:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('194912:52-194912:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('194912:52-194912:330 ')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195099:61-195099:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195109:85-195109:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195841:73-195841:100','195841:116-195841:143')
# 195774 OK from LS=0
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('198609:47-198609:112')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('201657:77-201657:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205718:49-205718:734')

process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2012, label = rawDataCollector, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
# for MC
#    InputLabel = cms.untracked.string('siPixelRawData'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),
# old
#    InputLabel = cms.untracked.string('siPixelRawData'),
#    InputLabel = cms.untracked.string('source'),
    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(0),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

# process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


