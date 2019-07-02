#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("cluTest")
                   
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2018

# 2017
#process.GlobalTag.globaltag = '90X_dataRun2_Express_v4' # 
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # 
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v4' # for 926
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # for 927,929
# 2018
#process.GlobalTag.globaltag = '100X_dataRun2_Express_v2' # 
process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' 
# 2016
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v9' # >=8010
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v10' # >=8014
#process.GlobalTag.globaltag = '80X_dataRun2_Express_v10' # >8010
#process.GlobalTag.globaltag = '80X_dataRun2_Express_v12' # >8014
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')


import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias	
#    HLTPaths = ['HLT_Physics*'],
    HLTPaths = ['HLT_Random*'],
#    HLTPaths = ['HLT_ZeroBias*'],
#    HLTPaths = ['HLT_ZeroBias_part*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionInTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_LastCollisionInTrain_*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstBXAfterTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_IsolatedBunches_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionAfterAbortGap_*'],
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
#    HLTPaths = ['HLT_PAZeroBias*'],
#    HLTPaths = ['HLT_PARandom*'],
#    HLTPaths = ['HLT_PAMinBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1Tech5_BPTX_PlusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech6_BPTX_MinusOnly_v*'],
#    HLTPaths = ['HLT_L1Tech7_NoBPTX_v*'],
#
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )


process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelClusters'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist )

  fileNames = cms.untracked.vstring(    

#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/088/00000/0189E695-D59D-3448-AD5B-AE4E5FCF2D1B.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/088/00000/ADC0A840-1436-F942-95C4-4565AA524BCB.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/088/00000/CC6FBCDF-DC08-FE40-8838-9ED9EF9ED95B.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/088/00000/4EC4CB38-0C6C-CC44-ADD8-3FF6D0831659.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/088/00000/8577D913-AC86-374B-9EA6-FC36D36B44B5.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/088/00000/9295A80F-8408-AD45-B2B1-2003D1CFD3BA.root",

"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/DB6CF1BD-F6DD-8549-B6A7-33BEF90DA6E5.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/EAEA538F-C843-014E-A963-A013783E1675.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/5D4EFCF8-70B7-F64F-9528-F2E6241967F3.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/60E93249-A569-B949-BB63-332EDBDF7208.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/D6F38C78-457D-6B42-9133-0603AAE99B39.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/1E9EE128-7246-FD49-8257-EA0E5A812D46.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/430884BC-941D-4A41-AB9C-664D727719B6.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/D268031A-9B52-154F-95C6-949F62A01982.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/97E625D4-EAAC-BF4C-B4B5-BEFDF8207C61.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/DF7CA664-BBBB-414C-9136-93F3EE9E55F3.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/590CEE26-2C08-6E43-B821-728E87A2F492.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/6DB5328C-7816-F040-80A5-889FAADC3B91.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/DFFD6AE4-B6FE-6C42-AE9C-55B64040035D.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/F779CC99-9A48-8340-A1A2-297019CD0B6E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/F30E9C1A-CE3A-B94E-9AF0-9A0A0A713E6E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/E0D06D69-10E6-7A49-9C1C-8080666545AB.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/BF242D08-2552-DC46-9576-C7A4183C9CA0.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/325/066/00000/4DE8676B-8D14-B147-9189-9DFD8C368C9E.root",


  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325088:0-325088:9')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana_cosmics.root')
)

process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    Normalise = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("ctfWithMaterialTracksP5"),
    # additional selections, e.g. select bx=1 -> (2,1)
    Select1 = cms.untracked.int32(0),  # select the cut type, 0 no cut
    Select2 = cms.untracked.int32(0),  # select the cut value   
)

process.d2 = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("ctfWithMaterialTracksP5"),
    # additional selections, e.g. select bx=1 -> (2,1)
    Select1 = cms.untracked.int32(2),  # select the cut type, 0 no cut
    Select2 = cms.untracked.int32(1),  # select the cut value   
)

process.clutest = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(True),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("siPixelClusters"),
#    src = cms.InputTag("siPixelClustersPreSplitting"),
)

process.c = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    Normalise = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# for collisions 
#    src = cms.InputTag("generalTracks"),
# for cosmics 
    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
# additional selections
    Select1 = cms.untracked.int32(0),  # select the cut type, o no cut
    Select2 = cms.untracked.int32(0),  # select the cut value   
)
process.c1 = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("generalTracks"),
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
    src = cms.InputTag("generalTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
# additional selections
    Select1 = cms.untracked.int32(14),  # select the cut type, o no cut
    Select2 = cms.untracked.int32(1),  # select the cut value   
)


#process.p = cms.Path(process.hltfilter*process.c)
process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.hltfilter*process.d*process.c)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.c1*process.c2)
#process.p = cms.Path(process.d*process.c)  # for mc 
#process.p = cms.Path(process.d) # for cosmics


