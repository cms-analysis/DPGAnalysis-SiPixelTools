import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("Analysis",eras.Run2_2017)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")

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
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v9'  # for 251643
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
 "/store/express/Run2017E/ExpressPhysics/FEVT/Express-v1/000/303/824/00000/02000C18-EBA0-E711-B8FB-02163E01A29A.root",

 )
#   skipEvents = cms.untracked.uint32(5000)
)
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('303824:54-303824:999')

# for Raw2digi for data
process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siStripDigis.ProductLabel = 'rawDataCollector'

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
useLocalDBGain = True 
if useLocalDBGain:
    process.DBReaderGain = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
        #record = cms.string('SiPixelGainCalibrationRcd'),

        #tag = cms.string('SiPixelGainCalibration_2016_v1_offline')
        #tag = cms.string('SiPixelGainCalibration_2016_v2')
        #tag = cms.string('SiPixelGainCalibration_2016_v1_hltvalidation')
        tag = cms.string('SiPixelGainCalibration_2017_v6_offline')
        #tag = cms.string('SiPixelGainCalibration_2017_v1337') # full

 	),
       ),
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_full.db')
     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_offline.db')

#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderGain")
# end 

useLocalDBError = False
if useLocalDBError :
    process.DBReaderError = cms.ESSource("PoolDBESSource",
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
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderError")
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

#process.siPixelClustersPreSplitting.payloadType = cms.string('Full')

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

process.histos = cms.EDAnalyzer('Pxl',
# for official RECO
#	triggerSource = cms.InputTag('TriggerResults::HLT'),
# For MC or my rereco
	triggerSource = cms.InputTag(''),
# Using templates   
#	ttrhBuilder = cms.string('WithAngleAndTemplate'),
# Using generic 
	ttrhBuilder = cms.string('WithTrackAngle'),
        singleParticleMC = cms.untracked.bool(False),
)
process.TFileService = cms.Service('TFileService',
	fileName = cms.string('triplets.root'),
)

process.c = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("generalTracks::Analysis"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),                             
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
)


#process.p1 = cms.Path(process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.siPixelClusterShapeCache*process.recopixelvertexing*process.MeasurementTrackerEvent*process.myTracking*process.vertexreco)

# do not save tracks, call triplets  
process.p = cms.Path(process.hltfilter*process.RawToDigi*process.reconstruction*process.histos*process.c)


# Path and EndPath definitions
#process.raw2digi_step = cms.Path(process.RawToDigi)
#process.L1Reco_step = cms.Path(process.L1Reco)
#process.reconstruction_step = cms.Path(process.reconstruction)
#process.histos_step = cms.Path(process.histos)
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.RECOoutput_step = cms.EndPath(process.out)
# Schedule definition
# save tracks
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)
# do not save tracks, call triplets  
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.histos_step)

# customisation of the process.
# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
#from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 
#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
#process = customisePostLS1(process)

#process.ep = cms.EndPath(process.out)
