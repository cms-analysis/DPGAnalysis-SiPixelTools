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
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '75X_upgrade2017_design_v4', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

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
#    connect = cms.string("sqlite_file:SiPixelLorentzAngle_phase1_mc_v2.db"),
    connect = cms.string("sqlite_file:SiPixelLorentzAngle_phase1_2017_v3.db"),
#    connect = cms.string("sqlite_file:SiPixelLorentzAngleSim_phase1_mc_v1.db"),
#    connect = cms.string("sqlite_file:SiPixelLorentzAngleSim_phase1_mc_v2.db"),
#    connect = cms.string("sqlite_file:SiPixelLorentzAngleSim_phase1_2017_v1_TESTONLY.db"),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelLorentzAngleRcd'),
#            tag = cms.string('SiPixelLorentzAngle_phase1_mc_v1')
#            tag = cms.string('SiPixelLorentzAngle_phase1_mc_v2')
            tag = cms.string('SiPixelLorentzAngle_phase1_2017_v3')
#	     tag = cms.string("SiPixelLorentzAngle_fromAlignment_v01_mc")	
#	     tag = cms.string("SiPixelLorentzAngle_fromAlignment_v01")	
#	     tag = cms.string("SiPixelLorentzAngle_forWidth_v01_mc")
#	     tag = cms.string("SiPixelLorentzAngle_forWidth_v01")
        ),
#        cms.PSet(
#            record = cms.string('SiPixelLorentzAngleSimRcd'),
#            tag = cms.string('SiPixelLorentzAngleSim_phase1_mc_v1')
#            tag = cms.string('SiPixelLorentzAngleSim_phase1_mc_v2')
#            tag = cms.string('SiPixelLorentzAngleSim_phase1_2017_v1_TESTONLY')
#        ),
       )
)

FPix_300V_RNG1_PNL1 = 0.0809
FPix_300V_RNG1_PNL2 = 0.0799
FPix_300V_RNG2_PNL1 = 0.0782
FPix_300V_RNG2_PNL2 = 0.0777

BPIX_LAYER1=0.1036
BPIX_LAYER_2_4_MODULE_1_4=0.1318
BPIX_LAYER_2_4_MODULE_5_8=0.1449

###### LORENTZ ANGLE OBJECT ######
process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngleDBLoader",
    # common input for all bpix/fpix
#    bPixLorentzAnglePerTesla = cms.untracked.double(0.122),
#    fPixLorentzAnglePerTesla = cms.untracked.double(0.122),
    # enter -9999 if individual input
    bPixLorentzAnglePerTesla = cms.untracked.double(-9999.),
    fPixLorentzAnglePerTesla = cms.untracked.double(-9999.),

    #in case of PSet (only works if above is -9999)
    # One common value for BPix for now
#   BPixParameters = cms.untracked.VPSet(
#        cms.PSet(
#            angle = cms.double(0.115)
#        ),
#    ),
   BPixParameters = cms.untracked.VPSet(
        cms.PSet(layer = cms.int32(1), angle = cms.double(BPIX_LAYER1)),

        cms.PSet(layer = cms.int32(2), module = cms.int32(1), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(2), module = cms.int32(2), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(2), module = cms.int32(3), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(2), module = cms.int32(4), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(2), module = cms.int32(5), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(2), module = cms.int32(6), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(2), module = cms.int32(7), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(2), module = cms.int32(8), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),

        cms.PSet(layer = cms.int32(3), module = cms.int32(1), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(3), module = cms.int32(2), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(3), module = cms.int32(3), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(3), module = cms.int32(4), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(3), module = cms.int32(5), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(3), module = cms.int32(6), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(3), module = cms.int32(7), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(3), module = cms.int32(8), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),

        cms.PSet(layer = cms.int32(4), module = cms.int32(1), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(4), module = cms.int32(2), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(4), module = cms.int32(3), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(4), module = cms.int32(4), angle = cms.double(BPIX_LAYER_2_4_MODULE_1_4)),
        cms.PSet(layer = cms.int32(4), module = cms.int32(5), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(4), module = cms.int32(6), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(4), module = cms.int32(7), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        cms.PSet(layer = cms.int32(4), module = cms.int32(8), angle = cms.double(BPIX_LAYER_2_4_MODULE_5_8)),
        
    ),

    FPixParameters = cms.untracked.VPSet(
        # Normal 150V FPix modules
        cms.PSet(
            ring = cms.int32(1),
            panel = cms.int32(1),
            angle = cms.double(0.118)
        ),
        cms.PSet(
            ring = cms.int32(1),
            panel = cms.int32(2),
            angle = cms.double(0.117)
        ),
        cms.PSet(
            ring = cms.int32(2),
            angle = cms.double(0.114)
        ),
    ),
    # List of Exceptions
    ModuleParameters = cms.untracked.VPSet(
        # Special 300V FPix modules are written first (so they don't get overwritten with the default 150V values)
        # Ring  1
        # Panel 1
        cms.PSet( rawid=cms.uint32(352588804), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352592900), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352596996), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352601092), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352605188), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352609284), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352658436), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352662532), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352666628), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352670724), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(352674820), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(344749060), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(344753156), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(344757252), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(344781828), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(344785924), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        cms.PSet( rawid=cms.uint32(344790020), angle=cms.double(FPix_300V_RNG1_PNL1) ),
        # Panel 2
        cms.PSet( rawid=cms.uint32(352589828), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352593924), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352598020), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352602116), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352606212), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352610308), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352659460), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352663556), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352667652), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352671748), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(352675844), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(344750084), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(344754180), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(344758276), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(344782852), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(344786948), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        cms.PSet( rawid=cms.uint32(344791044), angle=cms.double(FPix_300V_RNG1_PNL2) ),
        # Ring  2
        # Panel 1
        cms.PSet( rawid=cms.uint32(344851460), angle=cms.double(FPix_300V_RNG2_PNL1) ),
        cms.PSet( rawid=cms.uint32(344855556), angle=cms.double(FPix_300V_RNG2_PNL1) ),
        cms.PSet( rawid=cms.uint32(344859652), angle=cms.double(FPix_300V_RNG2_PNL1) ),
        cms.PSet( rawid=cms.uint32(344863748), angle=cms.double(FPix_300V_RNG2_PNL1) ),
        # Panel 2
        cms.PSet( rawid=cms.uint32(344852484), angle=cms.double(FPix_300V_RNG2_PNL2) ),
        cms.PSet( rawid=cms.uint32(344856580), angle=cms.double(FPix_300V_RNG2_PNL2) ),
        cms.PSet( rawid=cms.uint32(344860676), angle=cms.double(FPix_300V_RNG2_PNL2) ),
    ),
    #in case lorentz angle values for bpix should be read from file -> not implemented yet
    useFile = cms.bool(False),
    record = cms.untracked.string('SiPixelLorentzAngleRcd'),  
    #record = cms.untracked.string('SiPixelLorentzAngleSimRcd'),  
    fileName = cms.string('lorentzFit.txt')	
)


process.p = cms.Path(
    process.SiPixelLorentzAngle
    )

