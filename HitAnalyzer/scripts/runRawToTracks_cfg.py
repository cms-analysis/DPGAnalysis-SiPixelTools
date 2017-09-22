import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("MyRawToTracks",eras.Run2_2017)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")

process.load('Configuration.EventContent.EventContent_cff')

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# needed for pixel RecHits (TkPixelCPERecord)
#process.load("Configuration.StandardSequences.Reconstruction_cff")

# clusterizer 
#process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v9' # for 251643
# 2017
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # fpr 90W 
process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # from CMSSW927

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],  # not in ZB stream
#    HLTPaths = ['DST_Physics_v*'], useless 
#    HLTPaths = ['HLT_Random_v*'],
    HLTPaths = ['HLT_ZeroBias*'],
#    HLTPaths = ['HLT_L1Tech54_ZeroBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v1'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
# LumiPixels
#    HLTPaths = ['AlCa_LumiPixels_Random_v*'],
#    HLTPaths = ['AlCa_LumiPixels_ZeroBias_v*'],
#    HLTPaths = ['AlCa_LumiPixels_v*'],
    
# examples
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
# fileNames =  cms.untracked.vstring('file:rawdata.root')
fileNames =  cms.untracked.vstring(
#"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/FA244051-8141-E511-B22B-02163E014153.root",


"/store/express/Run2017D/ExpressPhysics/FEVT/Express-v1/000/302/131/00000/000109B7-6C8E-E711-89D1-02163E019CD2.root",

#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/0E6E4ACA-4F5A-E611-97B5-FA163E1E4ACD.root",

 )
#   skipEvents = cms.untracked.uint32(5000)
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('278193:77-278193:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('302131:34-302131:943')

# for Raw2digi for data
process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siStripDigis.ProductLabel = 'rawDataCollector'

process.siPixelClustersPreSplitting.SeedThreshold = 1000
process.siPixelClustersPreSplitting.ChannelThreshold = 2 #must be bigger than 1
process.siPixelClustersPreSplitting.ClusterThreshold = 1000    # integer?
process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 1000 # integer?
#process.siPixelClustersPreSplitting.payloadType = cms.string('Full')


# for digi to clu
#process.siPixelClusters.src = 'siPixelDigis'

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
#       threshold = cms.untracked.string('ERROR')
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

# pixel local reco (RecHits) needs the GenError object,
# not yet in GT, add here:
# DB stuff 
useLocalDBGain = False
if useLocalDBGain :
    process.DBReaderGain = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
        #tag = cms.string('SiPixelGainCalibration_2016_v1_offline')
        tag = cms.string('SiPixelGainCalibration_2016_v2_offline')
        #tag = cms.string('SiPixelGainCalibration_2016_v1_hltvalidation')
 	),
       ),
#     connect = cms.string('sqlite_file:../../../../../DB/Gains/SiPixelGainCalibration_2016_v1_offline.db')
     connect = cms.string('sqlite_file:../../../../../DB/Gains/SiPixelGainCalibration_2016_v2_offline.db')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderGain")
# end 


useLocalDBError = False
if useLocalDBError :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
         record = cms.string('SiPixelGenErrorDBObjectRcd'),
# 	 tag = cms.string("SiPixelGenErrorDBObject38Tv1")
#        tag = cms.string('SiPixelGenErrorDBObject_38T_2012_IOV7_v1')
         tag = cms.string('SiPixelGenErrorDBObject_38T_v1_offline')
 	 ),
       ),
#     connect = cms.string('sqlite_file:siPixelGenErrors38T_2012_IOV7_v1.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
#     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
     connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 



process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:reco.root'),
#    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/tracks_1_0.root'),
    #outputCommands = cms.untracked.vstring("drop *","keep *_*_*_MyRawToClus") # 13.1MB per 10 events
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,  # 4.9MB per 10 events 
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)


process.c = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("generalTracks::MyRawToTracks"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),                             
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
)

process.c1 = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("generalTracks::MyRawToTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
# additional selections
    Select1 = cms.untracked.int32(13),  # select the cut type, o no cut
    Select2 = cms.untracked.int32(1),  # select the cut value   
)
process.c2 = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("generalTracks::MyRawToTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
# additional selections
    Select1 = cms.untracked.int32(14),  # select the cut type, o no cut
    Select2 = cms.untracked.int32(1),  # select the cut value   
)




process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_tracks.root')
)


# copy the sequence below from  
# RecoTracker/IterativeTracking/python/iterativeTk_cff.py  - 71_pre7
process.myTracking = cms.Sequence(process.InitialStep*
                            process.DetachedTripletStep*
                            process.LowPtTripletStep*
                            process.PixelPairStep*
                            process.MixedTripletStep*
                            process.PixelLessStep*
                            process.TobTecStep*
                            process.earlyGeneralTracks*
                            # muonSeededStep*
                            process.preDuplicateMergingGeneralTracks*
                            process.generalTracksSequence*
                            process.ConvStep*
                            process.conversionStepTracks
                            )

#process.p = cms.Path(process.siPixelDigis)
#process.p = cms.Path(process.siPixelDigis*process.siPixelClusters)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)

#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis)
# crash on strip clusters, calibration records missing? works with the 620 tag
#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco)

#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot)
#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.recopixelvertexing)
#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.recopixelvertexing*process.MeasurementTrackerEvent)
# trackingGlobalReco, ckftracks, iterTracking - 
#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.recopixelvertexing*process.MeasurementTrackerEvent*process.myTracking)

#process.p1 = cms.Path(process.hltfilter*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.siPixelClusterShapeCache*process.recopixelvertexing*process.MeasurementTrackerEvent*process.myTracking*process.vertexreco)

#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.siPixelClusterShapeCache*process.recopixelvertexing*process.MeasurementTrackerEvent*process.myTracking*process.vertexreco)


# Path and EndPath definitions
#process.raw2digi_step = cms.Path(process.RawToDigi)
#process.L1Reco_step = cms.Path(process.L1Reco)
#process.reconstruction_step = cms.Path(process.reconstruction)
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.RECOoutput_step = cms.EndPath(process.out)
#process.d_step = cms.Path(process.d)
#process.filter_step = cms.Path(process.hltfilter)

# Schedule definition
# producing root files with tracks
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)
# producing track histos 
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.d_step)
#process.schedule = cms.Schedule(process.filter_step,process.raw2digi_step,process.reconstruction_step,process.d_step)

process.p = cms.Path(process.hltfilter*process.RawToDigi*process.reconstruction*process.c*process.c1*process.c2)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
#from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
#process = customisePostLS1(process)


#process.ep = cms.EndPath(process.out)
