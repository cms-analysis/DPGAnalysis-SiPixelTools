import FWCore.ParameterSet.Config as cms

process = cms.Process("MyRawToDigi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
# 2017
process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # 
# 2016
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8' # for 272
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias	
#    HLTPaths = ['HLT_Physics*'],
#    HLTPaths = ['HLT_Random*'],
    HLTPaths = ['HLT_ZeroBias*'],
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
#    HLTPaths = ['HLT_PAZeroBias*'],
#    HLTPaths = ['HLT_PARandom*'],
#    HLTPaths = ['HLT_PAMinBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1Tech5_BPTX_PlusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech6_BPTX_MinusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech7_NoBPTX_v*'],
#
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
 # fileNames =  cms.untracked.vstring('file:rawdata.root')
 fileNames =  cms.untracked.vstring(

# high Vibias  250
"/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/671/00000/1665877E-B15C-E711-8FC7-02163E0133E1.root",

#"/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/494/00000/221DC7DD-1E5A-E711-A76A-02163E011BB6.root",
#"/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/495/00000/003591BD-1A5A-E711-AF2A-02163E01A508.root",
#"/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/496/00000/04813659-1B5A-E711-9DE9-02163E0143BD.root",
#"/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/499/00000/0E628ABD-225A-E711-8B40-02163E01A5E2.root",
#"/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/503/00000/0024EBBF-385A-E711-BF25-02163E019E77.root",


# ctrl reg sca 
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/795/00000/1A1C0A23-0851-E711-8692-02163E01472F.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/790/00000/02DC1127-0951-E711-851C-02163E0143F0.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/789/00000/06177BD3-0451-E711-ABDF-02163E0133C1.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/788/00000/A0F7FF84-0351-E711-8FEE-02163E013662.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/702/00000/FED4EA05-D64F-E711-82F9-02163E012A6B.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/664/00000/00F38C8D-A54F-E711-A312-02163E01472F.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/643/00000/08070285-5E4F-E711-BB56-02163E01476C.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/296/116/00000/00187318-DE49-E711-883F-02163E019DD2.root",


# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/296/108/00000/22174A51-CB49-E711-8375-02163E01384C.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/296/075/00000/045D378C-9949-E711-A42A-02163E011F09.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/640/00000/029DD0A3-8645-E711-AE8C-02163E013978.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/636/00000/003BFB77-6E45-E711-AECD-02163E013407.root",


# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/426B4782-FC43-E711-9328-02163E011A76.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/F853E939-FF43-E711-A59D-02163E013479.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/381/00000/00034176-5043-E711-AA6F-02163E019C98.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/318/00000/06085100-B142-E711-A1D0-02163E01A1FA.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/209/00000/005E61C9-D341-E711-BEAE-02163E019C9F.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/128/00000/2E861F6A-1F41-E711-BEEC-02163E01A32B.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/294/929/00000/000E8FD5-1D40-E711-80C9-02163E01467E.root",


# 273725
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00030CAB-6C1E-E611-90F9-02163E0137A8.root",
# 273730
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/730/00000/00821BA0-381F-E611-BBC5-02163E0142C0.root",



#    "file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/raw/raw2.root"
 )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('273725:83-273725:9999')

# Cabling
#  include "CalibTracker/Configuration/data/Tracker_FakeConditions.cff"
#process.load("CalibTracker.Configuration.SiPixel_FakeConditions_cff")
#process.load("CalibTracker.Configuration.SiPixelCabling.SiPixelCabling_SQLite_cff")
#process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRAFT_V3P::All"
#process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
#process.siPixelCabling.connect = 'sqlite_file:cabling.db'
#process.siPixelCabling.toGet = cms.VPSet(cms.PSet(
#    record = cms.string('SiPixelFedCablingMapRcd'),
#    tag = cms.string('SiPixelFedCablingMap_v14')
#))


process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
# for simultaions 
#process.siPixelDigis.InputLabel = 'siPixelRawData'
# for data
#process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siPixelDigis.IncludeErrors = True
process.siPixelDigis.Timing = False 
#process.siPixelDigis.UsePilotBlade = True 
process.siPixelDigis.UsePhase1 = True 

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelDigis'),
    destinations = cms.untracked.vstring('r2d'),
#    r2d = cms.untracked.PSet( threshold = cms.untracked.string('DEBUG'))
#    r2d = cms.untracked.PSet( threshold = cms.untracked.string('INFO'))
    r2d = cms.untracked.PSet( threshold = cms.untracked.string('WARNING'))
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:digis.root'),
#    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/digis/digi_zb_248025.root'),
    outputCommands = cms.untracked.vstring("drop *","keep *_siPixelDigis_*_*")
)


process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
    Select1 = cms.untracked.int32(0),  # select the cut type, o no cut
    Select2 = cms.untracked.int32(0),  # select the cut value   
)

process.d = cms.EDAnalyzer("PixelFedErrorDumper", 
    Verbosity = cms.untracked.bool(False),
    InputLabel = cms.untracked.string('siPixelDigis'),
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis_histo.root')
)

#process.p = cms.Path(process.siPixelDigis)
process.p = cms.Path(process.siPixelDigis*process.a)
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.a*process.d)
# disable data write to disk 
#process.ep = cms.EndPath(process.out)
