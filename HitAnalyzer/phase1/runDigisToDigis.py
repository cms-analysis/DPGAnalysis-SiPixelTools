# produce pixel cluster & rechits from digia
# works directly or through raw
# 
# For phase1
##############################################################################

import FWCore.ParameterSet.Config as cms
process = cms.Process("DigiTest")

process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# clusterizer 
#process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

# for raw
process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
#process.load('Configuration.StandardSequences.DigiToRaw_cff')
#process.load('Configuration.StandardSequences.RawToDigi_cff')


# needed for pixel RecHits (templates?)
#process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '76X_upgrade2017_design_v8', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
#       threshold = cms.untracked.string('ERROR')
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)
# get the files from DBS:
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
#    'file:digis_100eve.root'
#    'file:digis.root'
    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_81/digis/digis1_pixonly.root'
  )
)

# a service to use root histos (keep if the analyser is run at the end)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
)

# Local DB stuff 
# cabling
#           record = cms.string('SiPixelFedCablingMapRcd'),
#           tag = cms.string('SiPixelFedCablingMap_v17')
# GenError
#          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
# LA (clusters do not need LA)
#useLocalLA = False
#if useLocalLA :
#    process.LAReader = cms.ESSource("PoolDBESSource",
#     DBParameters = cms.PSet(
#         messageLevel = cms.untracked.int32(0),
#         authenticationPath = cms.untracked.string('')
#     ),
#     toGet = cms.VPSet(
# 	cms.PSet(
#	 record = cms.string("SiPixelLorentzAngleRcd"),
#         tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
# 	),
#      ),
#      connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
#    ) # end process
#    process.LAprefer = cms.ESPrefer("PoolDBESSource","LAReader")
# endif

process.o1 = cms.OutputModule("PoolOutputModule",
          outputCommands = cms.untracked.vstring('drop *','keep *_*_*_DigiTest'),
#          fileName = cms.untracked.string('file:digis.root')
         fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_81/digis/digis1_l1roc.root')
)

# My 


# DIRECT
# direct clusterization (no raw step)
# label of digis 


process.analysis = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# my own pixel only digis (after digi step)
#    src = cms.InputTag("simSiPixelDigis"),
# after raw (my raw2digi)
    src = cms.InputTag("siPixelDigis"),
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_digis.root')
)  

# RAW
# digi2raw
process.siPixelRawData.UsePhase1 = cms.bool(True)
# raw2digi
process.siPixelDigis.InputLabel = 'siPixelRawData'
# for data
#process.siPixelDigis.InputLabel = 'source'
#process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siPixelDigis.IncludeErrors = True
process.siPixelDigis.Timing = False 
process.siPixelDigis.UsePhase1 = cms.bool(True)

# clusterize through raw (OK)
# for Raw2digi for simulations 
#process.siPixelRawData.InputLabel = 'mix'
#process.siPixelRawData.InputLabel = 'simSiPixelDigis'
#process.siPixelDigis.InputLabel = 'siPixelRawData'

#process.p1 = cms.Path(process.siPixelRawData)
#process.p1 = cms.Path(process.siPixelRawData*process.siPixelDigis)
process.p1 = cms.Path(process.siPixelRawData*process.siPixelDigis*process.analysis)

# for no output comment it out
process.outpath = cms.EndPath(process.o1)
