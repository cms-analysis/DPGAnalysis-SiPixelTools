#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("T")

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

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
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
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
    HLTPaths = ['HLT_ZeroBias*'],
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

# to select PhysicsBit
#process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
#process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'


myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/data/Run2015B/ZeroBias/RECO/PromptReco-v1/000/251/643/00000/002E6E66-762C-E511-8242-02163E0137EA.root",
])



process.source = cms.Source("PoolSource",
 fileNames =  myfilelist

# fileNames =  cms.untracked.vstring(
#    'file:tracks.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/tracks/tracks1.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks2_mc71.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks2_postls171.root'
#    )

)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_tracks.root')
)


process.d = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("generalTracks"),
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),     
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
)

#process.p = cms.Path(process.hltPhysicsDeclared*process.hltfilter*process.d)
# process.p = cms.Path(process.hltPhysicsDeclared*process.d)
process.p = cms.Path(process.d)



