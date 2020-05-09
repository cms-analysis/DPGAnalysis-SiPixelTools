import os
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

# SETTINGS
from FWCore.ParameterSet.VarParsing import VarParsing
options    = VarParsing('analysis')
options.register('run',         -1, mytype=VarParsing.varType.int)
options.register('year',        -1, mytype=VarParsing.varType.int)
options.register('dbversion',    0, mytype=VarParsing.varType.int)
options.register('phase',        1, mytype=VarParsing.varType.int)
options.register('gain', 'offline', mytype=VarParsing.varType.string)
options.register('outdir',      "", mytype=VarParsing.varType.string)
options.register('file',      None, mytype=VarParsing.varType.string)
options.parseArguments()
phase      = 1
gaincalDB  = options.gain
era        = eras.Run2_2017
run        = options.run if options.run>0 else 300000
year       = options.year
dbversion  = options.dbversion
outdir     = options.outdir
if run>0 and year>0:
  infile   = "GainCalibration.root"
  dbfile   = "SiPixelGainCalibration_%s_v%s_%s.db"%(year,dbversion,gaincalDB)
  rootfile = "SiPixelGainCalibration_%s_v%s_%s.root"%(year,dbversion,gaincalDB)
else:
  infile   = "GainCalibration_v2.root"
  #infile   = "GainCalibration_v3_vcal.root"
  dbfile   = "gains.db"
  rootfile = "gains.root"
if options.file:
  infile   = options.file
if outdir:
  if not os.path.exists(outdir):
    os.makedirs(outdir)
  dbfile   = os.path.join(outdir,dbfile)
  rootfile = os.path.join(outdir,rootfile)
if ':' not in infile:
  infile   = 'file:'+infile
if ':' not in dbfile:
  dbfile   = 'sqlite_file:'+dbfile

# PRINT
#print ">>> %-10s = '%s'"%('era',era)
print ">>> %-10s = %s"%('run',run)
print ">>> %-10s = %s"%('year',year)
print ">>> %-10s = %s"%('dbversion',dbversion)
print ">>> %-10s = '%s'"%('infile',infile)
print ">>> %-10s = '%s'"%('dbfile',dbfile)
print ">>> %-10s = '%s'"%('rootfile',rootfile)
print ">>> %-10s = '%s'"%('gaincalDB',gaincalDB)

## CHECK
#if not os.path.isfile(sqlfile):
#  raise IOError("VCal database object '%s' does not exist! cwd=%s"%(sqlfile,os.getcwd()))

# PROCESS
process = cms.Process("PixelGainDB",era)
if phase==0:
  process.load("Configuration.StandardSequences.GeometryDB_cff")
  process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
  from Configuration.AlCa.autoCond_condDBv2 import autoCond
  process.GlobalTag.globaltag = autoCond['run2_design']
elif phase==1:
  process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
  process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
  from Configuration.AlCa.GlobalTag import GlobalTag
  process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
  #process.GlobalTag.globaltag = '100X_dataRun2_Express_v2'#'92X_dataRun2_Express_v7'
  #process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
process.source = cms.Source("EmptyIOVSource",                            
    #lastRun = cms.untracked.uint32(1),
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(run),
    lastValue = cms.uint64(run),
    interval = cms.uint64(1)
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

# GAIN CALIBRATION
if gaincalDB.lower()=='offline':
  process.gaincalDB = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
      inputrootfile = cms.untracked.string(infile),
      record = cms.untracked.string('SiPixelGainCalibrationOfflineRcd'),
      useMeanWhenEmpty = cms.untracked.bool(True),     # True-fills ALL modules
      #useMeanWhenEmpty = cms.untracked.bool(False),   # True-fills ALL modules
      appendMode = cms.untracked.bool(False),          # adds to the old *.db file
      badChi2Prob = cms.untracked.double(0.000000001), # not used
      pedlow = cms.untracked.double(-100.0),
      pedhigh = cms.untracked.double(250.0),
      pedmax = cms.untracked.double(250.0),
      #gainmax = cms.untracked.double(6.0)  # for vcal not-included (Run1&2)  
      gainmax = cms.untracked.double(500.0) # for vcal included (Run3)  
  )
elif gaincalDB.lower()=='full':
  process.gaincalDB = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
      inputrootfile = cms.untracked.string(infile),
      record = cms.untracked.string('SiPixelGainCalibrationRcd'),
      useMeanWhenEmpty = cms.untracked.bool(True),
      appendMode = cms.untracked.bool(False),      # adds to the old *.db file
      badChi2Prob = cms.untracked.double(0.00001), # not used
      pedlow = cms.untracked.double(-100.0),
      pedhigh = cms.untracked.double(250.0),
      pedmax = cms.untracked.double(250.0),
      #gainmax = cms.untracked.double(6.0)  # for vcal not-included (Run1&2)  
      gainmax = cms.untracked.double(500.0) # for vcal included (Run3)  
  )
elif gaincalDB.lower()=='hlt':
  process.gaincalDB = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
      inputrootfile = cms.untracked.string(infile),
      record = cms.untracked.string('SiPixelGainCalibrationForHLTRcd'),
      useMeanWhenEmpty = cms.untracked.bool(True),
      appendMode = cms.untracked.bool(False),      # adds to the old *.db file
      badChi2Prob = cms.untracked.double(0.00001), # not used
      pedlow = cms.untracked.double(-100.0),
      pedhigh = cms.untracked.double(250.0),
      pedmax = cms.untracked.double(250.0),
      #gainmax = cms.untracked.double(6.0)  # for vcal not-included (Run1&2)
      gainmax = cms.untracked.double(500.0) # for vcal included (Run3)
  )
else:
  raise IOError("Did not recognize '%s' gain calibration!"%(gaincalDB))

# OUTPUT
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(rootfile)
)
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(10),
        authenticationPath = cms.untracked.string('.')
    ),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelGainCalibrationOfflineRcd'),
            #tag = cms.string('GainCalib_offline_v2')
            tag = cms.string('GainCalib_offline_v3')
        )
    ),
    connect = cms.string(dbfile)
)

# PATH
process.p = cms.Path(process.gaincalDB)
