import FWCore.ParameterSet.Config as cms

process = cms.Process("MyRawToDigi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
# fileNames =  cms.untracked.vstring('file:rawdata.root')
fileNames =  cms.untracked.vstring(
"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/24014201-5C13-E511-AEFB-02163E0139DC.root",
"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/2EE61F25-5C13-E511-9608-02163E013944.root",
"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/E855CA8B-B812-E511-A0FD-02163E013430.root",
"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/EC4F0147-5413-E511-9817-02163E013899.root",

#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/208/686/A88F66A0-393F-E211-9287-002481E0D524.root",
#    "file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/raw/raw2.root"
 )
)

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

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelDigis'),
    destinations = cms.untracked.vstring('r2d'),
    r2d = cms.untracked.PSet( threshold = cms.untracked.string('WARNING'))
)

process.out = cms.OutputModule("PoolOutputModule",
#    fileName =  cms.untracked.string('file:digis.root'),
    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/digis/digi_zb_248025.root'),
#    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/digis/digis_1k.root'),
    outputCommands = cms.untracked.vstring("drop *","keep *_siPixelDigis_*_*")
)


process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(False),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis_raw_zb_248025.root')
)

#process.p = cms.Path(process.siPixelDigis)
process.p = cms.Path(process.siPixelDigis*process.a)
process.ep = cms.EndPath(process.out)
