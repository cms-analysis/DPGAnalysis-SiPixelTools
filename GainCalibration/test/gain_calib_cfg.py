#! /usr/bin/env cmsRun
print ">>> %s start gain_calib.py %s"%('-'*16,'-'*15)
import os
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

# SETTINGS
from FWCore.ParameterSet.VarParsing import VarParsing
options   = VarParsing('analysis')
options.register('run',  323203, mytype=VarParsing.varType.int)
options.register('fed',  1200,   mytype=VarParsing.varType.int)
options.register('verb', int,    mytype=VarParsing.varType.int)
options.parseArguments()
fed       = options.fed
run       = options.run
verbosity = options.verb
era       = eras.Run2_2017
globaltag = '100X_dataRun2_Express_v2'
ext       = 'dmp'
sqlfile   = "siPixelVCal.db"
dmpfile   = "GainCalibration_%s_%s.%s"%(fed,run,ext)

# PRINT
print ">>> %-10s = '%s'"%('era',era)
print ">>> %-10s = '%s'"%('globaltag',globaltag)
print ">>> %-10s = '%s'"%('sqlfile',sqlfile)
print ">>> %-10s = '%s'"%('dmpfile',dmpfile)

# CHECK
if not os.path.isfile(sqlfile):
  raise IOError("VCal database object '%s' does not exist! cwd=%s"%(sqlfile,os.getcwd()))
if not os.path.isfile(dmpfile):
  raise IOError("Dump file '%s' does not exist! cwd=%s"%(dmpfile,os.getcwd()))

# SERVICES
process = cms.Process('PIXEL',era)
process.load('Configuration.StandardSequences.Services_cff')
process.TFileService = cms.Service('TFileService', fileName = cms.string('GainCalibration.root'))
process.MessageLogger = cms.Service('MessageLogger',
    destinations = cms.untracked.vstring('text_output'),
    text_output = cms.untracked.PSet(threshold=cms.untracked.string('WARNING')),
)

# GLOBAL TAG
# Phase0
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
# Phase1
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
process.GlobalTag.globaltag = globaltag

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# SLINK DATA --> DIGIS
lumiblocks = [cms.LuminosityBlockID(1,1)]
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
#process.source = cms.Source("PixelSLinkDataInputSource",
process.source = cms.Source("PixelDumpDataInputSource",
    fedid = cms.untracked.int32(-1),
    runNumber = cms.untracked.int32(500000),
    fileNames = cms.untracked.vstring("file:"+dmpfile),
    firstLuminosityBlock = cms.untracked.uint32(1),
    firstLuminosityBlockForEachRun = cms.untracked.VLuminosityBlockID(*lumiblocks),
)
process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.UsePhase1 = cms.bool(True)

# DIGIs --> CALIB DIGIs
process.load("DPGAnalysis-SiPixelTools.GainCalibration.SiPixelCalibDigiProducer_cfi")
# Instead of the old calib.dat file, configure these factors here
process.siPixelCalibDigis.Repeat = cms.int32(5)
process.siPixelCalibDigis.CalibMode = cms.string('GainCalibration')

# VCAL VALUES
process.siPixelCalibDigis.vCalValues_Int =  cms.vint32(
     6,  8, 10, 12, 14, 15, 16,  17,  18,  21,  24,  28,  35,  42, 49,
    56, 63, 70, 77, 84, 91, 98, 105, 112, 119, 126, 133, 140, 160
)
# --> have to add -1 (as a separator) after every 3rd column
# process.siPixelCalibDigis.calibcols_Int = cms.vint32(
#      0, 13, 26, -1,
#     39,  1, 14, -1,
#     27, 40,  2, -1,
#     15, 28, 41, -1,
#      3, 16, 29, -1,
#     42,  4, 17, -1,
#     30, 43,  5, -1,
#     18, 31, 44, -1,
#      6, 19, 32, -1,
#     45,  7, 20, -1,
#     33, 46,  8, -1,
#     21, 34, 47, -1,
#      9, 22, 35, -1,
#     48, 10, 23, -1,
#     36, 49, 11, -1,
#     24, 37, 50, -1,
#     12, 25, 38, -1,
#     51,         -1,
#)
process.siPixelCalibDigis.calibcols_Int = cms.vint32(
    0  , 4  , 8  , 12 , 16 , 20 , -1,
    24 , 28 , 32 , 36 , 40 , 44 , -1,
    48 , 1  , 5  , 9  , 13 , 17 , -1,
    21 , 25 , 29 , 33 , 37 , 41 , -1,
    45 , 49 , 2  , 6  , 10 , 14 , -1,
    18 , 22 , 26 , 30 , 34 , 38 , -1,
    42 , 46 , 50 , 3  , 7  , 11 , -1,
    15 , 19 , 23 , 27 , 31 , 35 , -1,
    39 , 43 , 47 , 51, -1,  
)
# -1 as a separator after every row
process.siPixelCalibDigis.calibrows_Int = cms.vint32(
     0, -1,  1, -1,  2, -1,  3, -1,  4, -1,  5, -1,  6, -1,  7, -1,  8, -1,  9, -1,
    10, -1, 11, -1, 12, -1, 13, -1, 14, -1, 15, -1, 16, -1, 17, -1, 18, -1, 19, -1,
    20, -1, 21, -1, 22, -1, 23, -1, 24, -1, 25, -1, 26, -1, 27, -1, 28, -1, 29, -1,
    30, -1, 31, -1, 32, -1, 33, -1, 34, -1, 35, -1, 36, -1, 37, -1, 38, -1, 39, -1,
    40, -1, 41, -1, 42, -1, 43, -1, 44, -1, 45, -1, 46, -1, 47, -1, 48, -1, 49, -1,
    50, -1, 51, -1, 52, -1, 53, -1, 54, -1, 55, -1, 56, -1, 57, -1, 58, -1, 59, -1,
    60, -1, 61, -1, 62, -1, 63, -1, 64, -1, 65, -1, 66, -1, 67, -1, 68, -1, 69, -1,
    70, -1, 71, -1, 72, -1, 73, -1, 74, -1, 75, -1, 76, -1, 77, -1, 78, -1, 79, -1,
)

# SET CALIB
process.load("DPGAnalysis-SiPixelTools.GainCalibration.SiPixelGainCalibrationAnalysis_cfi")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.siPixelGainCalibrationAnalysis.saveFile = False
process.siPixelGainCalibrationAnalysis.savePixelLevelHists = True 
process.siPixelGainCalibrationAnalysis.vCalValues_Int = process.siPixelCalibDigis.vCalValues_Int
process.siPixelGainCalibrationAnalysis.calibcols_Int = process.siPixelCalibDigis.calibcols_Int
process.siPixelGainCalibrationAnalysis.calibrows_Int = process.siPixelCalibDigis.calibrows_Int
process.siPixelGainCalibrationAnalysis.Repeat = process.siPixelCalibDigis.Repeat
process.siPixelGainCalibrationAnalysis.CalibMode = process.siPixelCalibDigis.CalibMode
process.siPixelGainCalibrationAnalysis.phase1 = True
if verbosity>=1:
  process.siPixelGainCalibrationAnalysis.writeSummary = True

# VCAL DB
process.VCalReader = cms.ESSource("PoolDBESSource",
    #BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('')
    ),
    connect = cms.string("sqlite_file:"+sqlfile),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string("SiPixelVCalRcd"),
            tag = cms.string("SiPixelVCal_v1")
        ),
    ),
)
process.myprefer = cms.ESPrefer("PoolDBESSource","VCalReader")

# PATH
process.p = cms.Path(process.siPixelDigis * process.siPixelCalibDigis * process.siPixelGainCalibrationAnalysis )

print ">>> %s done gain_calib.py %s"%('-'*16,'-'*16)
