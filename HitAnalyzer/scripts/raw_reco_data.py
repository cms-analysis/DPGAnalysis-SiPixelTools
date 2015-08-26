import FWCore.ParameterSet.Config as cms

process = cms.Process("MyRawToTracks")

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


#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v0' # for 251643

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
"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/FA244051-8141-E511-B22B-02163E014153.root",

#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/208/686/A88F66A0-393F-E211-9287-002481E0D524.root",


 )
#   skipEvents = cms.untracked.uint32(5000)
)

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('254227:21-254227:21')


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
    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/reco/reco.root'),
#    fileName =  cms.untracked.string('file:reco.root'),
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
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)


#process.ep = cms.EndPath(process.out)
