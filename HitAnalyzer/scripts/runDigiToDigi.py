import FWCore.ParameterSet.Config as cms
process = cms.Process("DigiToRaw1")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
# for simultaions 
#process.siPixelRawData.InputLabel = 'siPixelDigis'
process.siPixelDigis.InputLabel = 'siPixelRawData'
# for data
#process.siPixelDigis.InputLabel = 'source'
#process.siPixelDigis.InputLabel = 'rawDataCollector'
#process.siPixelDigis.IncludeErrors = True
#process.siPixelDigis.Timing = False 

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource", 
  fileNames =  cms.untracked.vstring(
  'file:/afs//cern.ch/work/d/dkotlins/public/MC/mu/pt100_74/digis/digis1.root'
 )
)

# Cabling
useLocal = False
if useLocal:
  process.CablingReader = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0),
       authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string('SiPixelFedCablingMapRcd'),
        tag = cms.string('SiPixelFedCablingMap_V19')
    )),
    connect = cms.string('sqlite_file:../../../../../DB/Connectivity/SiPixelFedCablingMap_V19.db')
    #connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  ) # end process
  process.prefer = cms.ESPrefer("PoolDBESSource","CablingReader")
# end if


process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelRawData'),
    destinations = cms.untracked.vstring('log'),
    log = cms.untracked.PSet( threshold = cms.untracked.string('WARNING'))
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:digitFromRaw.root'),
   #outputCommands = cms.untracked.vstring("drop *","keep *_siPixelRawData_*_*")
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
    fileName = cms.string('digis_histos.root')
)


#process.p = cms.Path(process.siPixelRawData)
#process.p = cms.Path(process.siPixelRawData*process.siPixelDigis)
process.p = cms.Path(process.siPixelRawData*process.siPixelDigis*process.a)
process.ep = cms.EndPath(process.out)


