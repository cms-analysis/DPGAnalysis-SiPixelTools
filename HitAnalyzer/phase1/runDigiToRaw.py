import FWCore.ParameterSet.Config as cms
process = cms.Process("DigiToRaw1")

process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '76X_upgrade2017_design_v8', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_81/digis/digis1_pixonly.root'
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
        tag = cms.string('SiPixelFedCablingMap_phase1_v1')
    )),
    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelFedCablingMap_phase1_v1.db')
    #connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  ) # end process
  process.prefer = cms.ESPrefer("PoolDBESSource","CablingReader")
# end if


process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
process.siPixelRawData.UsePhase1 = cms.bool(True)
#process.siPixelRawData.InputLabel = 'siPixelDigis'
# for raw
#process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
#process.load('Configuration.StandardSequences.DigiToRaw_cff')
#process.load('Configuration.StandardSequences.RawToDigi_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelRawData'),
    #debugModules = cms.untracked.vstring('*'),
    destinations = cms.untracked.vstring('log'),
    log = cms.untracked.PSet( threshold = cms.untracked.string('WARNING'))
    #log = cms.untracked.PSet( threshold = cms.untracked.string('DEBUG'))
    #log = cms.untracked.PSet( threshold = cms.untracked.string('INFO'))
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string(
    #'file:rawdata.root'
    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_81/raw/raw1_l1roc.root'
    ),
    outputCommands = cms.untracked.vstring("drop *","keep *_siPixelRawData_*_*")
)

process.p = cms.Path(process.siPixelRawData)
process.ep = cms.EndPath(process.out)



