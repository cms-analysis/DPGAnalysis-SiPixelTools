import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("MyRawToClus",eras.Run2_2017)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
#process.load('CondCore.CondDB.CondDB_cfi')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.load('Configuration.StandardSequences.RawToDigi_cff')

# needed for pixel RecHits (TkPixelCPERecord)
process.load("Configuration.StandardSequences.Reconstruction_cff")

# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2017
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # fpr 90W 
process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # from CMSSW927
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
# 2016 cosmics 
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v3', '')

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
 fileNames =  cms.untracked.vstring(



"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v2/000/300/633/00000/0085677A-AC7B-E711-B37E-02163E01A1C6.root",

#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v2/000/300/233/00000/062F0768-5676-E711-B303-02163E014626.root",
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v2/000/300/226/00000/02BBC9F3-2F76-E711-B29A-02163E019C08.root",
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/300/155/00000/0052B596-8F75-E711-B704-02163E011C13.root",

#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/300/122/00000/001BAA8B-BA74-E711-9564-02163E01A3CB.root",
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/300/122/00000/02F7A4F5-E074-E711-AAF3-02163E0133FC.root",
#"/store/express/Run2017C/ExpressPhysics/FEVT/Express-v1/000/300/088/00000/003E6DDF-4974-E711-A723-02163E0133C2.root",


# time 
#"/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/902/00000/008557CF-BE51-E711-A29A-02163E01A4E0.root",
#"/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/901/00000/009184D1-BF51-E711-A86D-02163E0143CF.root",
#"/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/900/00000/2C83793D-BE51-E711-9E1F-02163E0138BA.root",
#"/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/899/00000/185FC0AB-BE51-E711-9C71-02163E012019.root",
###"/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/898/00000/44CD306E-BA51-E711-977A-02163E01351C.root",
#"/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/888/00000/D8232BCF-AB51-E711-8B13-02163E013978.root",

# ctrl reg sca 
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/795/00000/1A1C0A23-0851-E711-8692-02163E01472F.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/790/00000/02DC1127-0951-E711-851C-02163E0143F0.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/789/00000/06177BD3-0451-E711-ABDF-02163E0133C1.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/788/00000/A0F7FF84-0351-E711-8FEE-02163E013662.root",

# vibias_bus scan
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/879/00000/046EAD9E-6151-E711-B783-02163E012B0C.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/876/00000/00A4AF1A-5C51-E711-83A7-02163E0146E8.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/875/00000/0895EF79-5A51-E711-99A0-02163E0142F0.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/874/00000/1475EB30-5751-E711-8988-02163E014472.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/873/00000/0638B0A2-5751-E711-BB45-02163E012477.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/872/00000/0E4D488C-5951-E711-8FD1-02163E0120C8.root",

# threshold scan 
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/878/00000/026F3312-5F51-E711-81E7-02163E011C6D.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/877/00000/04ED8753-5D51-E711-8C13-02163E011BE7.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/871/00000/0A593904-5751-E711-AB41-02163E011B0E.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/870/00000/065E00C2-5851-E711-8C67-02163E01356F.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/869/00000/263C3C92-5351-E711-9FE6-02163E0142F9.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/868/00000/0607D84F-4F51-E711-9E94-02163E01338F.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/867/00000/02BC1FDC-4C51-E711-BE3E-02163E0128EB.root",

#
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/702/00000/FED4EA05-D64F-E711-82F9-02163E012A6B.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/664/00000/00F38C8D-A54F-E711-A312-02163E01472F.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/643/00000/08070285-5E4F-E711-BB56-02163E01476C.root",


# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/296/108/00000/22174A51-CB49-E711-8375-02163E01384C.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/296/075/00000/045D378C-9949-E711-A42A-02163E011F09.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/640/00000/029DD0A3-8645-E711-AE8C-02163E013978.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/636/00000/003BFB77-6E45-E711-AECD-02163E013407.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/426B4782-FC43-E711-9328-02163E011A76.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/F853E939-FF43-E711-A59D-02163E013479.root",


# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/294/929/00000/000E8FD5-1D40-E711-80C9-02163E01467E.root",

#"/store/data/Commissioning2016/Cosmics/RAW/v1/000/266/277/00000/02643438-E2E5-E511-A9AA-02163E013432.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/277/00000/0004807C-AFE5-E511-83C1-02163E011EAD.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",
 )
)

# for Raw2digi for data
process.siPixelDigis.InputLabel = 'rawDataCollector'
#process.siStripDigis.ProductLabel = 'rawDataCollector'

# for digi to clu
#process.siPixelClusters.src = 'siPixelDigis'

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
       threshold = cms.untracked.string('ERROR')
#        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:clus.root'),
    #fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/clus/clus_zb_248025.root'),
    outputCommands = cms.untracked.vstring("drop *","keep *_*_*_MyRawToClus"), # 12.4MB per 100 events
    # save only pixel clusters 
    #outputCommands = cms.untracked.vstring("drop *","keep *_siPixelClustersPreSplitting_*_*"), # 5.6MB per 100 events
    #outputCommands = process.RECOEventContent.outputCommands,  # 4.9MB per 10 events 
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    splitLevel = cms.untracked.int32(0),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

# pixel local reco (RecHits) needs the GenError object,
# not yet in GT, add here:
# DB stuff 
useLocalDB = False
if useLocalDB :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
#        record = cms.string('SiPixelGainCalibrationRcd'),
#        tag = cms.string('SiPixelGainCalibration_2017_v4')
        tag = cms.string('SiPixelGainCalibration_2017_v2_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v3_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v4_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v4_1337_offline')
        #tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
 	),
       ),
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_full.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_1337_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v3_offline.db')
     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v2_offline.db')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 

#process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siPixelDigis.IncludeErrors = True
process.siPixelDigis.Timing = False 
#process.siPixelDigis.UsePilotBlade = True 
process.siPixelDigis.UsePhase1 = True

# digis from raw (this is the default)
#process.siPixelClustersPreSplitting.src = 'siPixelDigis'
# modify clusterie parameters
#process.siPixelClustersPreSplitting.VCaltoElectronGain = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = -60
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = -60
process.siPixelClustersPreSplitting.SeedThreshold = 1000
process.siPixelClustersPreSplitting.ChannelThreshold = 2 #must be bigger than 1
process.siPixelClustersPreSplitting.ClusterThreshold = 1 # integer?
process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 1 # integer?


process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)


process.d = cms.EDAnalyzer("PixClusterAna",
#process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('test.root')
)

#process.p = cms.Path(process.siPixelDigis)
#process.p = cms.Path(process.siPixelDigis*process.a)
#process.p = cms.Path(process.siPixelDigis*process.siPixelClusters)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# suppress output file or not
#process.ep = cms.EndPath(process.out)
