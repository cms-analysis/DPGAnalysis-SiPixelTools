import FWCore.ParameterSet.Config as cms
process = cms.Process("DigiToRaw1")

# process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2017NewFPixReco_cff')
#  2017NewFPix  *or* cmsExtendedGeometry2017newXML_cfi.py.

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
useLocal = True
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
#        tag = cms.string('SiPixelFedCablingMap_phase1_v2')
    )),
    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelFedCablingMap_phase1_v1.db')
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelFedCablingMap_phase1_v2.db')
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
    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_81/raw/raw1_wrongformat.root'
    ),
    outputCommands = cms.untracked.vstring("drop *","keep *_siPixelRawData_*_*")
)

process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2015 data, label = rawDataCollector, extension = _LHC                                
#    InputLabel = cms.untracked.string('rawDataCollector'),
# for MC
    InputLabel = cms.untracked.string('siPixelRawData'),
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
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('raw.root')
)

process.p = cms.Path(process.siPixelRawData*process.d)
process.ep = cms.EndPath(process.out)



