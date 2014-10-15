import FWCore.ParameterSet.Config as cms

process = cms.Process("PixelDBReader")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Geometry.TrackerGeometryBuilder.idealForDigiTrackerGeometry_cff")

process.load("CondTools.SiPixel.SiPixelGainCalibrationService_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_R_71_V7::All"
#process.GlobalTag.globaltag = "GR_P_V43::All"

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
    firstRun = cms.untracked.uint32(190000)
#    firstRun = cms.untracked.uint32(200000)
#    firstRun = cms.untracked.uint32(208000)
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
       #SiPixelGainCalibrationForHLTSimRcd  
       #SiPixelGainCalibrationOfflineSimRcd 
#       tag = cms.string('SiPixelGainCalibration_r203368_offline')
#       tag = cms.string('SiPixelGainCalibration_r197749_offline')
#       tag = cms.string('SiPixelGainCalib_2009CollRuns_offline')
#       tag = cms.string('IOV1')
       tag = cms.string('test')
    )),
#    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
#    connect = cms.string('sqlite_file:gain_iov1.db')
    connect = cms.string('sqlite_file:gains.db')
  ) # end process
  # process.prefer("PoolDBESSource")
  process.myprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if

process.SiPixelGainsDBReader = cms.EDAnalyzer("SiPixelGainsDBReader",
    process.SiPixelGainCalibrationServiceParameters,
#    payloadType = cms.string('HLT'),
    payloadType = cms.string('Offline'),
    useSimRcd = cms.bool(False),
    maxRangeDeadPixHist = cms.untracked.double(0.001)

)

process.p = cms.Path(process.SiPixelGainsDBReader)
#process.ep = cms.EndPath(process.print)
