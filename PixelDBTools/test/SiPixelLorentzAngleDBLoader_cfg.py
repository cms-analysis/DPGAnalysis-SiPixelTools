#import os
import shlex, shutil, getpass
#import subprocess

#tag = 'SiPixelLorentzAngle_2015_v3'
#tag = 'SiPixelLorentzAngle_2015_v4'
#tag = 'SiPixelLorentzAngle_v1'
#tag = 'SiPixelLorentzAngle_2016_v3'
tag = 'SiPixelLorentzAngle_2016_v4'

import FWCore.ParameterSet.Config as cms

process = cms.Process("SiPixelLABuilder")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.autoCond_condDBv2 import autoCond
#process.GlobalTag.globaltag = autoCond['run2_design']
process.GlobalTag.globaltag = autoCond['run2_data']
print process.GlobalTag.globaltag

process.load("Configuration.StandardSequences.GeometryDB_cff")

#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("CalibTracker.Configuration.TrackerAlignment.TrackerAlignment_Fake_cff")
#process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
#process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

#process.load("CondTools.SiPixel.SiPixelGainCalibrationService_cfi")
#process.load("CondCore.DBCommon.CondDBCommon_cfi")

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
user = getpass.getuser()

#try:
#    user = os.environ["USER"]
#except KeyError:
#    user = subprocess.call('whoami')
#    # user = commands.getoutput('whoami')
 
#file = "/tmp/" + user + "/SiPixelLorentzAngle.db"
file = tag+".db"
sqlfile = "sqlite_file:" + file
print '\n-> Uploading as user %s into file %s, i.e. %s\n' % (user, file, sqlfile)

#standard python libraries instead of spawn processes
#shutil.move("siPixelLorentzAngle.db", "siPixelLorentzAngle_old.db")
#subprocess.call(["/bin/cp", "siPixelLorentzAngle.db", file])
#subprocess.call(["/bin/mv", "siPixelLorentzAngle.db", "siPixelLorentzAngle.db"])

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


#> cat /afs/cern.ch/user/j/jkarancs/public/DB/Phase0/2016_11_20/xml/generror_IOV14/IOV14_LAs.txt
#IOV 14 (Autumn 2016)
#
#
#BPix LA[layer][module]
#
#    bpla[0][0] = 0.0836;
#    bpla[0][1] = 0.0836;
#    bpla[0][2] = 0.0836;
#    bpla[0][3] = 0.0836;
#    bpla[0][4] = 0.0874;
#    bpla[0][5] = 0.0874;
#    bpla[0][6] = 0.0874;
#    bpla[0][7] = 0.0874;
#    bpla[1][0] = 0.0837;
#    bpla[1][1] = 0.0837;
#    bpla[1][2] = 0.0837;
#    bpla[1][3] = 0.0837;
#    bpla[1][4] = 0.0881;
#    bpla[1][5] = 0.0881;
#    bpla[1][6] = 0.0881;
#    bpla[1][7] = 0.0881;
#    bpla[2][0] = 0.0817;
#    bpla[2][1] = 0.0817;
#    bpla[2][2] = 0.0817;
#    bpla[2][3] = 0.0817;
#    bpla[2][4] = 0.0853;
#    bpla[2][5] = 0.0853;
#    bpla[2][6] = 0.0853;
#    bpla[2][7] = 0.0853;
#
#BPix new modules [layers 1,2,3]
#
#   bpnla[0] = 0.0878;
#   bpnla[1] = 0.0867;
#   bpnla[2] = 0.0859;
#
#
#
#FPix [plaquettes 1, 2, 3-4]
#
#plaquettes 1 = 0.0742
#plaquettes 2 = 0.0704
#plaquettes 3-4 = 0.0721

L1_mZ      = 0.0836
L1_pZ      = 0.0874
L2_mZ      = 0.0837
L2_pZ      = 0.0881
L3_mZ      = 0.0817
L3_pZ      = 0.0853
L1_new     = 0.0878
L2_new     = 0.0867
L3_new     = 0.0859
FPix_plq1  = 0.0742
FPix_plq2  = 0.0704
FPix_plq34 = 0.0721

###### LORENTZ ANGLE OBJECT ######
#process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngleDB",
process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngleDBLoader",
    magneticField = cms.double(3.8),
    #in case of PSet
    BPixParameters = cms.untracked.VPSet(
        # Parameters that can be given: layer [1-4], ladder [1-N], module [1-8], side [1-2]
        cms.PSet(layer = cms.uint32(1), side = cms.uint32(1), angle = cms.double(L1_mZ)),
        cms.PSet(layer = cms.uint32(1), side = cms.uint32(2), angle = cms.double(L1_pZ)),
        cms.PSet(layer = cms.uint32(2), side = cms.uint32(1), angle = cms.double(L2_mZ)),
        cms.PSet(layer = cms.uint32(2), side = cms.uint32(2), angle = cms.double(L2_pZ)),
        cms.PSet(layer = cms.uint32(3), side = cms.uint32(1), angle = cms.double(L3_mZ)),
        cms.PSet(layer = cms.uint32(3), side = cms.uint32(2), angle = cms.double(L3_pZ)),
    ),
    FPixParameters = cms.untracked.VPSet(
        # Parameters that can be given: side [1-2], disk [1-3], blade [1-N], panel [1-2], plq [1-4], HVgroup [1-2] 
        cms.PSet(plq = cms.uint32(1), angle = cms.double(FPix_plq1)),
        cms.PSet(plq = cms.uint32(2), angle = cms.double(FPix_plq2)),
        cms.PSet(plq = cms.uint32(3), angle = cms.double(FPix_plq34)),
        cms.PSet(plq = cms.uint32(4), angle = cms.double(FPix_plq34)),
    ),
    # If any module is specified below, it will be filled with given value instead
    ModuleParameters = cms.untracked.VPSet(
        # L1 new modules
        cms.PSet(rawid = cms.uint32(302056472), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302056476), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302056212), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302055700), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302055708), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302060308), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302060312), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302059800), angle = cms.double(L1_new)),
        cms.PSet(rawid = cms.uint32(302059548), angle = cms.double(L1_new)),
        # L2 new modules
        cms.PSet(rawid = cms.uint32(302123040), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302122772), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302122776), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302122516), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302122264), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302122272), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302122008), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302121752), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302121496), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302121240), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302121244), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302128920), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302128924), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302129176), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302129180), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302129184), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302128404), angle = cms.double(L2_new)),
        cms.PSet(rawid = cms.uint32(302128408), angle = cms.double(L2_new)),
        # L3 new modules
        cms.PSet(rawid = cms.uint32(302189088), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302188820), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302188832), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302188052), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302187552), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302197784), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302197532), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302197536), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302197016), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302196244), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302195232), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302188824), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302186772), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302186784), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302121992), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302188552), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302187280), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302186768), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302186764), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302186756), angle = cms.double(L3_new)),
        cms.PSet(rawid = cms.uint32(302197516), angle = cms.double(L3_new)),
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
