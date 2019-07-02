import FWCore.ParameterSet.Config as cms
process = cms.Process("PixelGainsDBReader")
process.load("FWCore.MessageService.MessageLogger_cfi")

#process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2017_cff')


process.load("CondTools.SiPixel.SiPixelGainCalibrationService_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# works with condDB and condDBv2
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_73_V7', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS1', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
# for phase1 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '') #same
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_upgrade2017_design_v4', '')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root")
                                   )


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.source = cms.Source("EmptySource",
# timetype = cms.string('runnumber'),
#    numberEventsInRun = cms.untracked.uint32(10),
#    select the IOV from the global tag
#    firstRun = cms.untracked.uint32(190000)  # iov1-2012
#    firstRun = cms.untracked.uint32(200000)  # iov2-2012
#    firstRun = cms.untracked.uint32(208000)  # iov3-2012
    firstRun = cms.untracked.uint32(240000)  # iov1-2015
)

#process.Timing = cms.Service("Timing")

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(0)
#)

# DB stuff 
useLocalDB = True
if useLocalDB :
  process.GainsReader = cms.ESSource("PoolDBESSource",
  #    process.CondDBCommon,
   # BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0),
       authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(
      cms.PSet(
#       record = cms.string('SiPixelGainCalibrationForHLTRcd'),
       record = cms.string('SiPixelGainCalibrationOfflineRcd'),
#       record = cms.string('SiPixelGainCalibrationOfflineSimRcd'),  
#       record = cms.string('SiPixelGainCalibrationForHLTSimRcd'),  
#
#       tag = cms.string('SiPixelGainCalibration_r203368_offline')
#       tag = cms.string('SiPixelGainCalibration_r197749_offline')
#       tag = cms.string('SiPixelGainCalib_2009CollRuns_offline')
#       Phase1-MC
#       tag = cms.string('SiPixelGainCalibration_phase1_mc_v1')
#       tag = cms.string('SiPixelGainCalibration_phase1_ideal')
#       tag = cms.string('SiPixelGainCalibrationSim_phase1_ideal')
#       tag = cms.string('SiPixelGainCalibration_hlt_phase1_mc_v1')
#       tag = cms.string('SiPixelGainCalibration_hlt_phase1_ideal')
#       tag = cms.string('SiPixelGainCalibrationSim_hlt_phase1_ideal')
    )),
#    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
    connect = cms.string('sqlite_file:SiPixelGainCalibrationSim_phase1_ideal.db')

#    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_mc_v1.db')
#    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_ideal.db')
#    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibrationSim_phase1_ideal.db') 
  ) # end process
  # process.prefer("PoolDBESSource")
  process.myprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if

process.SiPixelGainsDBReader = cms.EDAnalyzer("SiPixelGainsDBReader",
    process.SiPixelGainCalibrationServiceParameters,
#    payloadType = cms.string('HLT'),
    payloadType = cms.string('Offline'),
#    useSimRcd = cms.bool(False),
    useSimRcd = cms.bool(True),
    maxRangeDeadPixHist = cms.untracked.double(0.001)

)

process.p = cms.Path(process.SiPixelGainsDBReader)
