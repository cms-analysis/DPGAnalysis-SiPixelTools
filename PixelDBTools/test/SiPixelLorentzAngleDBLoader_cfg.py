#import os
import shlex, shutil, getpass
#import subprocess

import FWCore.ParameterSet.Config as cms

process = cms.Process("SiPixelInclusiveBuilder")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("CalibTracker.Configuration.TrackerAlignment.TrackerAlignment_Fake_cff")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("CondTools.SiPixel.SiPixelGainCalibrationService_cfi")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

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

#try:
#    user = os.environ["USER"]
#except KeyError:
#    user = subprocess.call('whoami')
#    # user = commands.getoutput('whoami')
 
file = "prova.db"
sqlfile = "sqlite_file:" + file
print '\n-> Uploading into file %s, i.e. %s\n' % (file, sqlfile)

#standard python libraries instead of spawn processes
#shutil.move("prova.db", "prova_old.db")

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
            tag = cms.string('SiPixelLorentzAngle_test')
#	     tag = cms.string("SiPixelLorentzAngle_fromAlignment_v01_mc")	
#	     tag = cms.string("SiPixelLorentzAngle_fromAlignment_v01")	
#	     tag = cms.string("SiPixelLorentzAngle_forWidth_v01_mc")
#	     tag = cms.string("SiPixelLorentzAngle_forWidth_v01")
        ),
###        cms.PSet(
###            record = cms.string('SiPixelLorentzAngleSimRcd'),
###            tag = cms.string('SiPixelLorentzAngleSim_v01')
###        ),
       )
)


###### LORENTZ ANGLE OBJECT ######
process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngleDBLoader",
#    magneticField = cms.double(3.8),
#    bPixLorentzAnglePerTesla = cms.double(-9999.),
#    fPixLorentzAnglePerTesla = cms.double(-9999.),
#    bPixLorentzAnglePerTesla = cms.double(0.10),
#    fPixLorentzAnglePerTesla = cms.double(0.06),
    bPixLorentzAnglePerTesla = cms.double(0.05),
    fPixLorentzAnglePerTesla = cms.double(0.03),
    #in case of PSet
    BPixParameters = cms.untracked.VPSet(
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(1),
            angle = cms.double(0.09103)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(2),
            angle = cms.double(0.09103)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(3),
            angle = cms.double(0.09103)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(4),
            angle = cms.double(0.09103)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(5),
            angle = cms.double(0.09574)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(6),
            angle = cms.double(0.09574)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(7),
            angle = cms.double(0.09574)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(8),
            angle = cms.double(0.09574)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(1),
            angle = cms.double(0.09415)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(2),
            angle = cms.double(0.09415)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(3),
            angle = cms.double(0.09415)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(4),
            angle = cms.double(0.09415)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(5),
            angle = cms.double(0.09955)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(6),
            angle = cms.double(0.09955)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(7),
            angle = cms.double(0.09955)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(8),
            angle = cms.double(0.09955)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(1),
            angle = cms.double(0.09541)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(2),
            angle = cms.double(0.09541)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(3),
            angle = cms.double(0.09541)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(4),
            angle = cms.double(0.09541)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(5),
            angle = cms.double(0.10121)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(6),
            angle = cms.double(0.10121)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(7),
            angle = cms.double(0.10121)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(8),
            angle = cms.double(0.10121)
        ),
    ),
    FPixParameters = cms.untracked.VPSet(
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.06404)
        ),
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.06404)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.06404)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.06404)
        ),
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.06404)
        ),
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.06404)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.06404)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.06404)
        ),
    ),
    #in case lorentz angle values for bpix should be read from file -> not implemented yet
    useFile = cms.bool(False),
    record = cms.untracked.string('SiPixelLorentzAngleRcd'),  
    fileName = cms.string('lorentzFit.txt')	
)

process.SiPixelLorentzAngleSim = cms.EDAnalyzer("SiPixelLorentzAngleDB",
    magneticField = cms.double(3.8),
    #bPixLorentzAnglePerTesla = cms.double(0.106),
    #fPixLorentzAnglePerTesla = cms.double(0.091),
    #in case lorentz angle values for bpix should be read from file -> not implemented yet
    useFile = cms.bool(False),
    record = cms.untracked.string('SiPixelLorentzAngleSimRcd'),
    fileName = cms.string('lorentzFit.txt')	
)




process.p = cms.Path(
#    process.SiPixelLorentzAngleSim*
    process.SiPixelLorentzAngle
    )

