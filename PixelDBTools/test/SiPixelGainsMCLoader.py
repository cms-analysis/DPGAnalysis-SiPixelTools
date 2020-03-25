import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
#process = cms.Process("SiPixelGainsBuilder")
process = cms.Process("SiPixelGainsBuilder",eras.Run3)

process.load("Configuration.Geometry.GeometryRecoDB_cff")
#process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2017_cff')

process.load("CondTools.SiPixel.SiPixelGainCalibrationService_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    ),
    destinations = cms.untracked.vstring('cout')
)

#process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '')

process.source = cms.Source("EmptyIOVSource",
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    timetype = cms.string('runnumber'),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb'),
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
    connect = cms.string('sqlite_file:gain.db'),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelGainCalibrationRcd'),
            tag = cms.string('SiPixelGainCalibration_phase1_mc_v3')
        ), 
        cms.PSet(
            record = cms.string('SiPixelGainCalibrationOfflineRcd'),
            #tag = cms.string('SiPixelGainCalibration_phase1_ideal_v3')
            tag = cms.string('SiPixelGainCalibration_phase1_mc_v3')
        ), 
        cms.PSet(
            record = cms.string('SiPixelGainCalibrationForHLTRcd'),
            #tag = cms.string('SiPixelGainCalibration_hlt_phase1_ideal')
            tag = cms.string('SiPixelGainCalibration_hlt_phase1_mc_v3')
        ),
        cms.PSet(
            record = cms.string('SiPixelGainCalibrationOfflineSimRcd'),
            tag = cms.string('SiPixelGainCalibrationSim_phase1_ideal_v3')
        ), 
        cms.PSet(
            record = cms.string('SiPixelGainCalibrationForHLTSimRcd'),
            tag = cms.string('SiPixelGainCalibrationSim_hlt_phase1_ideal_v3')
        ))
)

# offline reco , can be ideal or mc
process.SiPixelGainsOfflineBuilder = cms.EDAnalyzer("SiPixelCondObjOfflineBuilder",
    process.SiPixelGainCalibrationServiceParameters,
    numberOfModules = cms.int32(2000),
    appendMode = cms.untracked.bool(False),
                                                    meanGain = cms.double(3.19),
                                                    meanPed = cms.double(16.5),
    # realistic (mc)
                                                    rmsGain = cms.double(0.054),
                                                    rmsPed = cms.double(5.4),
    # ideal payload
    #rmsGain = cms.double(0.0),
    #rmsPed = cms.double(0.0),
    # separate input for the FPIX. If not entered the default values are used.
                                                    meanGainFPix = cms.untracked.double(3.19),
                                                    meanPedFPix = cms.untracked.double(16.5),
    # realistic (mc) 
                                                    rmsGainFPix = cms.untracked.double(0.054),
                                                    rmsPedFPix = cms.untracked.double(5.4),
    # ideal 
    #rmsGainFPix = cms.untracked.double(0.0),
    #rmsPedFPix = cms.untracked.double(0.0),
    record = cms.string('SiPixelGainCalibrationOfflineRcd'),
    fromFile = cms.bool(False),
    fileName = cms.string(''),
    secondRocRowGainOffset = cms.double(0.0),
    secondRocRowPedOffset = cms.double(0.0),
    deadFraction = cms.double(0.0),
    noisyFraction = cms.double(0.0),
#    ElectronsPerVcal = cms.untracked.double(1.),
#    ElectronsPerVcal_Offset = cms.untracked.double(0.),
#    ElectronsPerVcal_L1 = cms.untracked.double(1.),
#    ElectronsPerVcal_L1_Offset = cms.untracked.double(0.)
    ElectronsPerVcal = cms.untracked.double(47.),
    ElectronsPerVcal_Offset = cms.untracked.double(-60.),
    ElectronsPerVcal_L1 = cms.untracked.double(50.),
    ElectronsPerVcal_L1_Offset = cms.untracked.double(-670.)
)

# Full payload 
process.SiPixelGainsBuilder = cms.EDAnalyzer("SiPixelCondObjBuilder",
    process.SiPixelGainCalibrationServiceParameters,
    numberOfModules = cms.int32(2000),
    appendMode = cms.untracked.bool(False),
                                                    meanGain = cms.double(3.19),
                                                    meanPed = cms.double(16.5),
    # realistic (mc)
                                                    rmsGain = cms.double(0.054),
                                                    rmsPed = cms.double(5.4),
    # ideal payload
    #rmsGain = cms.double(0.0),
    #rmsPed = cms.double(0.0),
    # separate input for the FPIX. If not entered the default values are used.
                                                    meanGainFPix = cms.untracked.double(3.19),
                                                    meanPedFPix = cms.untracked.double(16.5),
    # realistic (mc) 
                                                    rmsGainFPix = cms.untracked.double(0.054),
                                                    rmsPedFPix = cms.untracked.double(5.4),
    # ideal 
    #rmsGainFPix = cms.untracked.double(0.0),
    #rmsPedFPix = cms.untracked.double(0.0),
    record = cms.string('SiPixelGainCalibrationRcd'),
    fromFile = cms.bool(False),
    fileName = cms.string(''),
    secondRocRowGainOffset = cms.double(0.0),
    secondRocRowPedOffset = cms.double(0.0),
    deadFraction = cms.double(0.0),
    noisyFraction = cms.double(0.0),
#    ElectronsPerVcal = cms.untracked.double(1.),
#    ElectronsPerVcal_Offset = cms.untracked.double(0.),
#    ElectronsPerVcal_L1 = cms.untracked.double(1.),
#    ElectronsPerVcal_L1_Offset = cms.untracked.double(0.)
    ElectronsPerVcal = cms.untracked.double(47.),
    ElectronsPerVcal_Offset = cms.untracked.double(-60.),
    ElectronsPerVcal_L1 = cms.untracked.double(50.),
    ElectronsPerVcal_L1_Offset = cms.untracked.double(-670.)
)

# offline sim
process.SiPixelGainsOfflineSimBuilder = cms.EDAnalyzer("SiPixelCondObjOfflineBuilder",
    process.SiPixelGainCalibrationServiceParameters,
    numberOfModules = cms.int32(2000),
    appendMode = cms.untracked.bool(False),
    meanGain = cms.double(3.48),
    meanPed = cms.double(25.3),
    rmsGain = cms.double(0.0),
    rmsPed = cms.double(0.0),
    # separate input for the FPIX. If not entered the default values are used.
    meanGainFPix = cms.untracked.double(2.90),
    rmsGainFPix = cms.untracked.double(0.0),
    meanPedFPix = cms.untracked.double(27.7),
    rmsPedFPix = cms.untracked.double(0.0),
    record = cms.string('SiPixelGainCalibrationOfflineSimRcd'),
    fromFile = cms.bool(False),
    fileName = cms.string(''),
    secondRocRowGainOffset = cms.double(0.0),
    secondRocRowPedOffset = cms.double(0.0),
    deadFraction = cms.double(0.0),
    noisyFraction = cms.double(0.0)                                                       
)

# hlt reco, can be ideal or not (mc)
process.SiPixelGainsForHLTBuilder = cms.EDAnalyzer("SiPixelCondObjForHLTBuilder",
    process.SiPixelGainCalibrationServiceParameters,
    numberOfModules = cms.int32(2000),
    appendMode = cms.untracked.bool(False),
                                                   meanGain = cms.double(3.19),
                                                   meanPed = cms.double(16.5),
    # realistic (mc)
                                                   rmsGain = cms.double(0.054),
                                                   rmsPed = cms.double(5.4),
    # ideal 
    #rmsGain = cms.double(0.0),
    #rmsPed = cms.double(0.0),
    # separate input for the FPIX. If not entered the default values are used.
                                                   meanGainFPix = cms.untracked.double(3.19),
                                                   meanPedFPix = cms.untracked.double(16.5),
    #realistic (mc)
                                                   rmsGainFPix = cms.untracked.double(0.054),
                                                   rmsPedFPix = cms.untracked.double(5.4),
    # ideal 
    #rmsGainFPix = cms.untracked.double(0.0),
    #rmsPedFPix = cms.untracked.double(0.0),
    record = cms.string('SiPixelGainCalibrationForHLTRcd'),
    fromFile = cms.bool(False),
    fileName = cms.string(''),
    secondRocRowGainOffset = cms.double(0.0),
    secondRocRowPedOffset = cms.double(0.0),
    deadFraction = cms.double(0.0),
    noisyFraction = cms.double(0.0),
#    ElectronsPerVcal = cms.untracked.double(1.),
#    ElectronsPerVcal_Offset = cms.untracked.double(0.),
#    ElectronsPerVcal_L1 = cms.untracked.double(1.),
#    ElectronsPerVcal_L1_Offset = cms.untracked.double(0.)
    ElectronsPerVcal = cms.untracked.double(47.),
    ElectronsPerVcal_Offset = cms.untracked.double(-60.),
    ElectronsPerVcal_L1 = cms.untracked.double(50.),
    ElectronsPerVcal_L1_Offset = cms.untracked.double(-670.)
)

# hlt sim
process.SiPixelGainsForHLTSimBuilder = cms.EDAnalyzer("SiPixelCondObjForHLTBuilder",
    process.SiPixelGainCalibrationServiceParameters,
    numberOfModules = cms.int32(2000),
    appendMode = cms.untracked.bool(False),
    meanGain = cms.double(3.48),
    rmsGain = cms.double(0.0),
    meanPed = cms.double(25.3),
    rmsPed = cms.double(0.0),
    # separate input for the FPIX. If not entered the default values are used.
    meanGainFPix = cms.untracked.double(2.90),
    rmsGainFPix = cms.untracked.double(0.0),
    meanPedFPix = cms.untracked.double(27.7),
    rmsPedFPix = cms.untracked.double(0.0),
    record = cms.string('SiPixelGainCalibrationForHLTSimRcd'),
    fromFile = cms.bool(False),
    fileName = cms.string(''),
    secondRocRowGainOffset = cms.double(0.0),
    secondRocRowPedOffset = cms.double(0.0),
    deadFraction = cms.double(0.0),
    noisyFraction = cms.double(0.0)                                                      
)

process.p = cms.Path(process.SiPixelGainsOfflineBuilder)
#process.p = cms.Path(process.SiPixelGainsForHLTBuilder)
#process.p = cms.Path(process.SiPixelGainsBuilder)
#process.p = cms.Path(process.SiPixelGainsOfflineBuilder*process.SiPixelGainsForHLTBuilder)

#process.p = cms.Path(process.SiPixelGainsOfflineSimBuilder)
#process.p = cms.Path(process.SiPixelGainsForHLTSimBuilder)
#process.p = cms.Path(process.SiPixelGainsOfflineSimBuilder*process.SiPixelGainsForHLTSimBuilder)


