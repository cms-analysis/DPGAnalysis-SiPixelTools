import FWCore.ParameterSet.Config as cms

process = cms.Process("MyRawToDigi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
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
process.GlobalTag = GlobalTag(process.GlobalTag, '76X_upgrade2017_design_v8', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
fileNames =  cms.untracked.vstring(
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/24014201-5C13-E511-AEFB-02163E0139DC.root",
 'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/raw/raw1_pixonly.root'
# 'file:rawdata_100.root',
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
    )),
    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelFedCablingMap_phase1_v1.db')
    #connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  ) # end process
  process.prefer = cms.ESPrefer("PoolDBESSource","CablingReader")
# end if


process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
# for simultaions 
process.siPixelDigis.InputLabel = 'siPixelRawData'
# for data
#process.siPixelDigis.InputLabel = 'source'
#process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siPixelDigis.IncludeErrors = True
process.siPixelDigis.Timing = False 
process.siPixelDigis.UsePhase1 = cms.bool(True)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelDigis'),
    destinations = cms.untracked.vstring('log'),
    log = cms.untracked.PSet( threshold = cms.untracked.string('WARNING'))
    #log = cms.untracked.PSet( threshold = cms.untracked.string('DEBUG'))
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:digis.root'),
#    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/digis/digi_zb_248025.root'),
#    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/digis/digis_1k.root'),
    outputCommands = cms.untracked.vstring("drop *","keep *_siPixelDigis_*_*")
)


process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis.root')
)

#process.p = cms.Path(process.siPixelDigis)
process.p = cms.Path(process.siPixelDigis*process.a)
process.ep = cms.EndPath(process.out)
