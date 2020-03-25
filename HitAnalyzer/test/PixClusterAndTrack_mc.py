#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
#process = cms.Process("cluTest",eras.Run2_2017)
process = cms.Process("cluTest",eras.Run3)
                   
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# 
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_design', '') # for Run3
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '') # for Run3
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2023_realistic', '') # for Run3
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2024_realistic', '') # for Run3



#process.GlobalTag.globaltag = '106X_upgrade2018_realistic_v9'
#process.GlobalTag.globaltag = '110X_mcRun3_2021_realistic_v3'
#process.GlobalTag.globaltag = '110X_mcRun3_2023_realistic_v3'
#process.GlobalTag.globaltag = '110X_mcRun3_2024_realistic_v3'

#process.GlobalTag.globaltag = '110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00' # 2000e
#process.GlobalTag.globaltag = '110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44' # 2000e
#process.GlobalTag.globaltag = '110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03' # 2000e


import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias	
#    HLTPaths = ['HLT_Physics*'],
#    HLTPaths = ['HLT_Random*'],
#    HLTPaths = ['HLT_ZeroBias*'],
    HLTPaths = ['HLT_ZeroBias_v*'],
#    HLTPaths = ['HLT_ZeroBias_part*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionInTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_LastCollisionInTrain_*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstBXAfterTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_IsolatedBunches_*'], # ok
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionAfterAbortGap_*'], # ok
#    HLTPaths = ['HLT_ZeroBias_BeamSpot_*'],
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
#    destinations = cms.untracked.vstring('cout'),
    destinations = cms.untracked.vstring("log","cout"),
    log = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
#        threshold = cms.untracked.string('DEBUG')
    ),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    )
)


# ----------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
#)

  fileNames = cms.untracked.vstring(    

    '/store/user/kotlinski/MC11/mu_pt100/reco/reco1.root',

# new run3 MC
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1099.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1098.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1097.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1096.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1095.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1094.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1093.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1092.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1091.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1090.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1089.root',


# 2023
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1092.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1091.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1090.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1089.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1088.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1087.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1086.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1085.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1084.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1083.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1082.root',

# 2024
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1098.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1097.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1096.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1095.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1094.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1093.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1092.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1091.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1090.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1089.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1088.root',

# 2018
#"/store/relval/CMSSW_10_6_1_patch3/RelValTTbar_13/GEN-SIM-RECO/PU25ns_106X_upgrade2018_realistic_v6-v1/20000/FF0A1ADF-8ECA-514A-A177-2723BCCABDE0.root",


  )   # end the list "by-hand"
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana_mc.root')
)

process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("generalTracks"),
    # additional selections, e.g. select bx=1 -> (2,1)
                           Select1 = cms.untracked.int32(9998),  # select the cut type, 0 no cut
    Select2 = cms.untracked.int32(0),  # select the cut value   
)

process.d1 = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("generalTracks"),
    # additional selections, e.g. select bx=1 -> (2,1)
    Select1 = cms.untracked.int32(103),  # select events with no pvs
    Select2 = cms.untracked.int32(0),  # select the cut value   
)
process.d2 = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("generalTracks"),
    # additional selections, e.g. select bx=1 -> (2,1)
    Select1 = cms.untracked.int32(101),  # select events with no tracks
    Select2 = cms.untracked.int32(0),  # select the cut value   
)

process.clutest = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(True),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("siPixelClusters"),
#    src = cms.InputTag("siPixelClustersPreSplitting"),
)

process.c = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("generalTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
# additional selections
                           Select1 = cms.untracked.int32(9998),  # select the cut type, o no cut
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
                            Select1 = cms.untracked.int32(9998),  # select the cut type, o no cut
                            Select2 = cms.untracked.int32(0),  # select the cut value   
)


#process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.hltfilter*process.d*process.c)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.c1*process.c2)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.d1*process.d2)

process.p = cms.Path(process.d*process.c) # for mc
#process.p = cms.Path(process.c) # for mc
#process.p = cms.Path(process.d) # for cosmics

