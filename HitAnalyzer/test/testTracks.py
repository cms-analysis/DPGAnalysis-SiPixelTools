#
# Last update: new version for python
#
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("T",eras.Run2_2017)

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# MC
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '') # no misalignment 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '') # with misalignment 
# 2017
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v4' # for 251643
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8' # for 273158
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
       
process.MessageLogger = cms.Service("MessageLogger",
#    debugModules = cms.untracked.vstring('TestPixTracks'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#   HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_ZeroBias_v*'],
#     HLTPaths = ['HLT_L1Tech54_ZeroBias*'],
#     HLTPaths = ['HLT_L1Tech53_MB*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v*'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
 fileNames =  cms.untracked.vstring(
     'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco2.root',

#    'file:tracks.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_73/tracks/tracks1_mc73_13.root',

# 272783
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/783/00000/008955C7-3A14-E611-99D8-02163E0133B7.root",
# 272798
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/798/00000/FEF97F57-8414-E611-AF3C-02163E0125B1.root",

    )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277087:196-277087:1192')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_tracks.root')
)


process.d = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("generalTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
    Select1 = cms.untracked.int32(11),  # select the cut type
    Select2 = cms.untracked.int32(4),  # select the cut value   
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)



