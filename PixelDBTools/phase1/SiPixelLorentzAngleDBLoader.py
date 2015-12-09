#
import FWCore.ParameterSet.Config as cms
process = cms.Process("SiPixelLorentzAngleLoader")

#process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2017_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '75X_upgrade2017_design_v4', '')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("ERROR"))

process.source = cms.Source("EmptyIOVSource",
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    timetype = cms.string('runnumber'),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

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
#    connect = cms.string("sqlite_file:SiPixelLorentzAngle_phase1_mc_v1.db"),
    connect = cms.string("sqlite_file:SiPixelLorentzAngleSim_phase1_mc_v1.db"),
    toPut = cms.VPSet(
#        cms.PSet(
#            record = cms.string('SiPixelLorentzAngleRcd'),
#            tag = cms.string('SiPixelLorentzAngle_phase1_mc_v1')
#	     tag = cms.string("SiPixelLorentzAngle_fromAlignment_v01_mc")	
#	     tag = cms.string("SiPixelLorentzAngle_fromAlignment_v01")	
#	     tag = cms.string("SiPixelLorentzAngle_forWidth_v01_mc")
#	     tag = cms.string("SiPixelLorentzAngle_forWidth_v01")
#        ),
        cms.PSet(
            record = cms.string('SiPixelLorentzAngleSimRcd'),
            tag = cms.string('SiPixelLorentzAngleSim_phase1_mc_v1')
        ),
       )
)


###### LORENTZ ANGLE OBJECT ######
process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngleDBLoader",
# common input for all rings 
    bPixLorentzAnglePerTesla = cms.untracked.double(0.098),
    fPixLorentzAnglePerTesla = cms.untracked.double(0.058),
# enter -9999 if individual input for rings 
#    bPixLorentzAnglePerTesla = cms.untracked.double(-9999.),
#    fPixLorentzAnglePerTesla = cms.untracked.double(-9999.),

    #in case of PSet
    BPixParameters = cms.untracked.VPSet(
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(1),
            angle = cms.double(0.0948)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(2),
            angle = cms.double(0.0948)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(3),
            angle = cms.double(0.0948)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(4),
            angle = cms.double(0.0948)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(5),
            angle = cms.double(0.0964)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(6),
            angle = cms.double(0.0964)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(7),
            angle = cms.double(0.0964)
        ),
        cms.PSet(
            layer = cms.uint32(1),
            module = cms.uint32(8),
            angle = cms.double(0.0964)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(1),
            angle = cms.double(0.0916)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(2),
            angle = cms.double(0.0916)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(3),
            angle = cms.double(0.0916)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(4),
            angle = cms.double(0.0916)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(5),
            angle = cms.double(0.0931)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(6),
            angle = cms.double(0.0931)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(7),
            angle = cms.double(0.0931)
        ),
        cms.PSet(
            layer = cms.uint32(2),
            module = cms.uint32(8),
            angle = cms.double(0.0931)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(1),
            angle = cms.double(0.0920)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(2),
            angle = cms.double(0.0920)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(3),
            angle = cms.double(0.0920)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(4),
            angle = cms.double(0.0920)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(5),
            angle = cms.double(0.0935)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(6),
            angle = cms.double(0.0935)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(7),
            angle = cms.double(0.0935)
        ),
        cms.PSet(
            layer = cms.uint32(3),
            module = cms.uint32(8),
            angle = cms.double(0.0935)
        ),
    ),
    FPixParameters = cms.untracked.VPSet(
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.081)
        ),
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.081)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.081)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(1),
            angle = cms.double(0.081)
        ),
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.081)
        ),
        cms.PSet(
            side = cms.uint32(1),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.081)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(1),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.081)
        ),
        cms.PSet(
            side = cms.uint32(2),
            disk = cms.uint32(2),
            HVgroup = cms.uint32(2),
            angle = cms.double(0.081)
        ),
    ),
   ModuleParameters = cms.untracked.VPSet(
        cms.PSet(
            #rawid = cms.uint32(0),
            #angle = cms.double(0.0)
        ),
   ),
    #in case lorentz angle values for bpix should be read from file -> not implemented yet
    useFile = cms.bool(False),
    record = cms.untracked.string('SiPixelLorentzAngleRcd'),  
    fileName = cms.string('lorentzFit.txt')	
)

process.SiPixelLorentzAngleSim = cms.EDAnalyzer("SiPixelLorentzAngleDBLoader",
    bPixLorentzAnglePerTesla = cms.untracked.double(0.098),
    fPixLorentzAnglePerTesla = cms.untracked.double(0.058),
    #in case lorentz angle values for bpix should be read from file -> not implemented yet
    useFile = cms.bool(False),
    record = cms.untracked.string('SiPixelLorentzAngleSimRcd'),
    fileName = cms.string('lorentzFit.txt'),	
    #in case of PSet
    BPixParameters = cms.untracked.VPSet(
        cms.PSet(
            layer = cms.uint32(0),
            module = cms.uint32(0),
            angle = cms.double(0.0)
        ),
    ),
    FPixParameters = cms.untracked.VPSet(
        cms.PSet(
            side = cms.uint32(0),
            disk = cms.uint32(0),
            HVgroup = cms.uint32(0),
            angle = cms.double(0.0)
        ),
    ),
   ModuleParameters = cms.untracked.VPSet(
        cms.PSet(
            #rawid = cms.uint32(0),
            #angle = cms.double(0.0)
        ),
   ),

)


process.p = cms.Path(
    process.SiPixelLorentzAngleSim
#    process.SiPixelLorentzAngle
    )

