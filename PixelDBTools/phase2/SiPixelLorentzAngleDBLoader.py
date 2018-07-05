#  for phase2
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("SiPixelLorentzAngleLoader",eras.Phase2)

#process.load('Configuration.Geometry.GeometryExtended2023D23Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2023D23_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

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
                                          connect = cms.string("sqlite_file:SiPixelLorentzAngle_phase2_v1.db"),
#                                          connect = cms.string("sqlite_file:SiPixelLorentzAngleSim_phase2_v1.db"),
                                          toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelLorentzAngleRcd'),
            tag = cms.string('SiPixelLorentzAngle_phase2_v1')
            ),
        #        cms.PSet(
        #            record = cms.string('SiPixelLorentzAngleSimRcd'),
        #            tag = cms.string('SiPixelLorentzAngleSim_phase2_v1')
        #        ),
        )
                                          )

#FPix_300V_RNG1_PNL1 = 0.0805
#BPIX_LAYER1=0.0595
#BPIX_LAYER_2_MODULE_1_4=0.0765
#BPIX_LAYER_2_MODULE_5_8=0.0805
#BPIX_LAYER_3_MODULE_1_4=0.0864
#BPIX_LAYER_3_MODULE_5_8=0.0929
#BPIX_LAYER_4_MODULE_1_4=0.0961
#BPIX_LAYER_4_MODULE_5_8=0.1036

###### LORENTZ ANGLE OBJECT ######
process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngleDBLoader",
                                             # common input for all bpix/fpix
                                             bPixLorentzAnglePerTesla = cms.untracked.double(0.1),
                                             fPixLorentzAnglePerTesla = cms.untracked.double(0.1),
                                             # enter -9999 if individual input
                                             #bPixLorentzAnglePerTesla = cms.untracked.double(-9999.),
                                             #fPixLorentzAnglePerTesla = cms.untracked.double(-9999.),
                                             #in case of PSet (only works if above is -9999)
                                             # One common value for BPix for now
                                             BPixParameters = cms.untracked.VPSet(
        cms.PSet(angle = cms.double(0.1)),
        ),
                                             #   BPixParameters = cms.untracked.VPSet(
                                             #        cms.PSet(layer = cms.int32(1), angle = cms.double(BPIX_LAYER1)),
                                             #        cms.PSet(layer = cms.int32(2), module = cms.int32(1), angle = cms.double(BPIX_LAYER_2_MODULE_1_4)),
                                             #        cms.PSet(layer = cms.int32(3), module = cms.int32(8), angle = cms.double(BPIX_LAYER_3_MODULE_5_8)),
                                             #        cms.PSet(layer = cms.int32(4), module = cms.int32(1), angle = cms.double(BPIX_LAYER_4_MODULE_1_4)),
                                             #    ),
                                             FPixParameters = cms.untracked.VPSet(
        cms.PSet(ring = cms.int32(1), panel = cms.int32(1), angle = cms.double(0.1) ),
        ),
                                             
                                             # List of Exceptions
                                             ModuleParameters = cms.untracked.VPSet(
        cms.PSet( rawid=cms.uint32(352588804), angle=cms.double(0.1) ),
        ),
                                             #in case lorentz angle values for bpix should be read from file -> not implemented yet
                                             useFile = cms.bool(False),
                                             record = cms.untracked.string('SiPixelLorentzAngleRcd'),  
                                             #record = cms.untracked.string('SiPixelLorentzAngleSimRcd'),  
                                             fileName = cms.string('lorentzFit.txt')	
                                             )


process.p = cms.Path(
#    process.SiPixelLorentzAngleSim
    process.SiPixelLorentzAngle
)

