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
    input = cms.untracked.int32(10)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('raw.root')
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/00A76FFA-0C82-E511-B441-02163E01450F.root",
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
    fileNames = cms.untracked.vstring(                          
#    "file:../../../../../CMSSW_7_1_3/src/DPGAnalysis-SiPixelTools/HitAnalyzer/test/raw.root"
#    "file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/raw/raw2.root"
 "/store/express/HIRun2015/HIExpressPhysics/FEVT/Express-v1/000/262/548/00000/FCBD965E-6293-E511-B9A2-02163E01463B.root",

# "/store/data/Run2015D/ZeroBias/RAW/v1/000/258/655/00000/0C7D4CCE-886E-E511-A789-02163E0142E2.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/258/655/00000/",
# "/store/data/Run2015D/ZeroBias/RAW/v1/000/258/655/00000/F8BA24BF-866E-E511-BC31-02163E013890.root",

# "/store/data/Run2015D/Cosmics/RAW-RECO/CosmicSP-PromptReco-v4/000/258/415/00000/B66364BE-F56D-E511-A301-02163E0138F6.root",

# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/257/396/00000/30C0E789-EA62-E511-8051-02163E013514.root",
#"root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/257/396/00000/846DCB9F-EA62-E511-9CF3-02163E01423D.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/257/396/00000/CAC46AAE-EA62-E511-984D-02163E0118D2.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/257/396/00000/F216E889-EA62-E511-97A2-02163E0142FE.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/257/396/00000/F481C387-EA62-E511-99D9-02163E011C68.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/257/396/00000/FCAF068C-EA62-E511-91B6-02163E0134C0.root",
# "root://eoscms//eos/cms/tier0/store/data/Run2015D/ZeroBias/RAW/v1/000/257/396/00000/FEEE6F91-EA62-E511-B8AB-02163E0139B6.root",

# "root://eoscms//eos/cms/tier0/store/data/Run2015D/Cosmics/RAW/v1/000/256/856/00000/68555315-165F-E511-A279-02163E014228.root",

# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/0230C488-1543-E511-A00F-02163E01437A.root",
#"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/FA244051-8141-E511-B22B-02163E014153.root",
#"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/F4E6E4DE-8841-E511-922B-02163E014125.root",
#"/store/data/Run2015B/Cosmics/RAW/v1/000/253/106/00000/98FDF449-DA37-E511-A847-02163E0124BA.root",
#"/store/data/Run2015B/MinimumBias/RAW/v1/000/253/215/00000/04B30F2F-A839-E511-B855-02163E013717.root",
#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/411/00000/2227CD91-A61F-E511-8ABC-02163E011D1C.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/24014201-5C13-E511-AEFB-02163E0139DC.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/00C00730-A00D-E511-8273-02163E014565.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/026/00000/029FB859-68B2-E411-8145-02163E0122AB.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/027EE06B-D8CE-E411-8E83-02163E012A80.root",
# fill 2576
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

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('254227:20-254227:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('254227:21-254227:21')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('257487:50-257487:9999')

process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2015 data, label = rawDataCollector, extension = _LHC                                
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
    Verbosity = cms.untracked.int32(1),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


