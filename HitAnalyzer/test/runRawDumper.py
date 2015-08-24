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

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(                          
#    "file:../../../../../CMSSW_7_1_3/src/DPGAnalysis-SiPixelTools/HitAnalyzer/test/raw.root"
#    "file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/raw/raw2.root"
#

"root://eoscms//eos/cms/tier0/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/FA244051-8141-E511-B22B-02163E014153.root",
"root://eoscms//eos/cms/tier0/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/F4E6E4DE-8841-E511-922B-02163E014125.root",
"root://eoscms//eos/cms/tier0/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/F41B36B8-8841-E511-B34E-02163E01437D.root",
"root://eoscms//eos/cms/tier0/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/F0C1F208-9341-E511-B9B1-02163E0145C7.root",
"root://eoscms//eos/cms/tier0/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/EEF00C97-8141-E511-9148-02163E0142D9.root",

#"root://eoscms//eos/cms/tier0/store/data/Run2015C/HLTPhysics/RAW/v1/000/254/227/00000/FE743A63-9941-E511-80CC-02163E01437A.root",
#"root://eoscms//eos/cms/tier0/store/data/Run2015C/Charmonium/RAW/v1/000/254/227/00000/B6FC5492-8841-E511-9CF7-02163E01444F.root",
#"root://eoscms//eos/cms/tier0/store/data/Run2015C/Charmonium/RAW/v1/000/254/227/00000/88DDB326-8841-E511-A4C2-02163E014595.root",


#"/store/data/Run2015B/Cosmics/RAW/v1/000/253/106/00000/98FDF449-DA37-E511-A847-02163E0124BA.root",

#"/store/data/Run2015B/MinimumBias/RAW/v1/000/253/215/00000/04B30F2F-A839-E511-B855-02163E013717.root",
#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/411/00000/2227CD91-A61F-E511-8ABC-02163E011D1C.root",
#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/411/00000/D2A60B28-A71F-E511-9950-02163E012AEE.root",

#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/381/00000/0EE60482-6A1F-E511-BF44-02163E013515.root",

#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/24014201-5C13-E511-AEFB-02163E0139DC.root",
#"/store/data/Run2015A/ZeroBias/RAW/v1/000/248/025/00000/3C78C42B-5C13-E511-9ED3-02163E012385.root",

#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/00C00730-A00D-E511-8273-02163E014565.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/125871B2-6E0D-E511-9212-02163E01365F.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/1A482360-530D-E511-B661-02163E0145CC.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/1AC17670-580D-E511-8AC6-02163E0143D5.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/2C480D5F-530D-E511-825C-02163E014157.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/2CF1716F-580D-E511-BA9F-02163E011BAC.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/2EFEC6AE-6E0D-E511-9652-02163E0138F2.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/42833732-A00D-E511-8298-02163E013529.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/44A2FFFD-810D-E511-963E-02163E0141F9.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/46100729-7B0D-E511-97F5-02163E0145B1.root",
##"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/464E4136-A00D-E511-BAB5-02163E011DD9.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/5607B9FA-810D-E511-9B1A-02163E013604.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/58ED3A7D-8B0D-E511-91C7-02163E013891.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/5CA06E70-580D-E511-8F4E-02163E0143A6.root",
##"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/60ED9B22-820D-E511-8773-02163E0144B2.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/664BB65F-530D-E511-B9CA-02163E011EB6.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/72942CAB-6E0D-E511-8A8E-02163E0126FB.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/82333907-3E0D-E511-8859-02163E013500.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/824DCFFE-810D-E511-8E53-02163E01475E.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/82635EB3-6E0D-E511-B610-02163E012123.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/92067870-580D-E511-940D-02163E013891.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/98225731-A00D-E511-A3C0-02163E01257B.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/A6341D61-530D-E511-A31E-02163E01453F.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/A652AB5F-530D-E511-9343-02163E0143D5.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/AE399D6F-580D-E511-ACD2-02163E011953.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/AECE2070-580D-E511-BEF2-02163E014157.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/B2A12AA6-6E0D-E511-BC45-02163E013942.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/B89B6337-590D-E511-A0DD-02163E0146A5.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/C09FB11D-820D-E511-A8D4-02163E0128A1.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/C8C8F002-820D-E511-9D25-02163E011BB0.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/D2B03870-580D-E511-ADCF-02163E011EB6.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/D462C230-A00D-E511-B3D0-02163E0140E0.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/D62BF4A6-6E0D-E511-B2D8-02163E012927.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/E6F7D3A9-6E0D-E511-8DAA-02163E0146EE.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/EA87EA60-530D-E511-AA8F-02163E013891.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/EEE0AD55-6B0D-E511-8495-02163E0146AC.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/F28EE92F-7B0D-E511-91B8-02163E01458C.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/F2C3A0A5-6E0D-E511-A26E-02163E0138BC.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/F686589F-A00D-E511-A3E2-02163E0145E7.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/FABFBF61-530D-E511-B20B-02163E013653.root",
#

#  "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/026/00000/029FB859-68B2-E411-8145-02163E0122AB.root",


#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/027EE06B-D8CE-E411-8E83-02163E012A80.root",
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
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('254227:20-254227:9999')

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

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


