import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("PixelLABuilder",eras.Run2_2017)
#process = cms.Process("PixelLABuilder",eras.Run3)
#import os
import shlex, shutil, getpass

tag = 'test'

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("ERROR"))

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# works with condDB and condDBv2
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_73_V7', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS1', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
# for phase1 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '') #same
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_design', '') # for Run3
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '') # for Run3
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2023_realistic', '') # for Run3
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2024_realistic', '') # for Run3

print process.GlobalTag.globaltag


process.source = cms.Source("EmptyIOVSource",
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    timetype = cms.string('runnumber'),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

#to get the user running the process
#user = getpass.getuser()
# sqlit file name 
file = tag+".db"
sqlfile = "sqlite_file:" + file
print '\n-> Uploading into file %s, i.e. %s\n' % (file, sqlfile)

##### DATABASE CONNNECTION AND INPUT TAGS ######
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('.'),
        connectionRetrialPeriod = cms.untracked.int32(10),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(1),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(0),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False)
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string(sqlfile),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelLorentzAngleRcd'),
            tag = cms.string(tag)
        ),
###        cms.PSet(
###            record = cms.string('SiPixelLorentzAngleSimRcd'),
###            tag = cms.string('SiPixelLorentzAngleSim_v1')
###        ),
    )
)

L1_mZ      = 0.0836
L1_pZ      = 0.0874
L2_mZ      = 0.0837
L2_pZ      = 0.0881
L3_mZ      = 0.0817
L3_pZ      = 0.0853
L4_mZ      = 0.0817
L4_pZ      = 0.0853
#L1_new     = 0.0878
FPix_plq1  = 0.0742
FPix_plq2  = 0.0704
FPix_plq34 = 0.0721

###### LORENTZ ANGLE OBJECT ######
process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngleDBLoader",
    magneticField = cms.double(3.8),
    #in case of PSet
    BPixParameters = cms.untracked.VPSet(
        # Parameters that can be given: layer [1-4], ladder [1-N], module [1-8], side [1-2]
        cms.PSet(layer = cms.int32(1), side = cms.int32(1), angle = cms.double(L1_mZ)),
        cms.PSet(layer = cms.int32(1), side = cms.int32(2), angle = cms.double(L1_pZ)),
        cms.PSet(layer = cms.int32(2), side = cms.int32(1), angle = cms.double(L2_mZ)),
        cms.PSet(layer = cms.int32(2), side = cms.int32(2), angle = cms.double(L2_pZ)),
        cms.PSet(layer = cms.int32(3), side = cms.int32(1), angle = cms.double(L3_mZ)),
        cms.PSet(layer = cms.int32(3), side = cms.int32(2), angle = cms.double(L3_pZ)),
        cms.PSet(layer = cms.int32(4), side = cms.int32(1), angle = cms.double(L4_mZ)),
        cms.PSet(layer = cms.int32(4), side = cms.int32(2), angle = cms.double(L4_pZ)),
    ),
    FPixParameters = cms.untracked.VPSet(
        # Parameters that can be given: side [1-2], disk [1-3], blade [1-N], panel [1-2], plq [1-4], HVgroup [1-2] 
        cms.PSet(plq = cms.int32(1), angle = cms.double(FPix_plq1)),
        cms.PSet(plq = cms.int32(2), angle = cms.double(FPix_plq2)),
        cms.PSet(plq = cms.int32(3), angle = cms.double(FPix_plq34)),
        cms.PSet(plq = cms.int32(4), angle = cms.double(FPix_plq34)),
    ),
    # If any module is specified below, it will be filled with given value instead
    ModuleParameters = cms.untracked.VPSet(
        # for exmple L1 new modules
        #cms.PSet(rawid = cms.uint32(302056472), angle = cms.double(L1_new)),
    ),

    #in case lorentz angle values for bpix should be read from file -> not implemented yet
    useFile = cms.bool(False),
    record = cms.untracked.string('SiPixelLorentzAngleRcd'),  
    fileName = cms.string('lorentzFit.txt')	
)

process.SiPixelLorentzAngleSim = cms.EDAnalyzer("SiPixelLorentzAngleDB",
    magneticField = cms.double(3.8),
    #in case lorentz angle values for bpix should be read from file -> not implemented yet
    useFile = cms.bool(False),
    record = cms.untracked.string('SiPixelLorentzAngleSimRcd'),
    fileName = cms.string('lorentzFit.txt')	
)

process.p = cms.Path(
#    process.SiPixelLorentzAngleSim*
    process.SiPixelLorentzAngle
    )
