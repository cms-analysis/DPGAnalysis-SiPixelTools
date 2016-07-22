#
# Last update: new version for python
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("T")

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v4' # for 251643
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8' # for 273158
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

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
 fileNames =  cms.untracked.vstring(
#    'file:tracks.root'

# 272783
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/783/00000/008955C7-3A14-E611-99D8-02163E0133B7.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/783/00000/068192D9-3814-E611-9731-02163E01412C.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/783/00000/084B665B-3514-E611-B9A1-02163E01396B.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/783/00000/0A4809DE-3D14-E611-84C3-02163E01387F.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/783/00000/0CF6C89E-3F14-E611-A33E-02163E01184A.root",

# 272798
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/798/00000/FEF97F57-8414-E611-AF3C-02163E0125B1.root",

# 273017
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/273/017/00000/005CCAB3-7617-E611-94AC-02163E01382D.root",

# 273158
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/000FBA71-E317-E611-B784-02163E0141D0.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/004A4494-E217-E611-8698-02163E011D03.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/008B2ED1-FC17-E611-BE33-02163E01361A.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/0094F401-E417-E611-B3A9-02163E01355D.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/00C7F40C-F617-E611-9134-02163E01394C.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/024956C1-FC17-E611-B286-02163E011BBE.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/028A7ADE-FC17-E611-A0AD-02163E01450C.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/02EE2460-F417-E611-B6AB-02163E0145EA.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/02F7E720-F417-E611-96D6-02163E0133FE.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/04074738-FD17-E611-83DA-02163E0138A8.root",

# 273725
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00030CAB-6C1E-E611-90F9-02163E0137A8.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/005C7BCB-471E-E611-89AB-02163E0141C1.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/0092140B-431E-E611-B190-02163E01399A.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/009B159B-581E-E611-90E1-02163E01265D.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C0B635-611E-E611-9B9C-02163E011CFC.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C82263-741E-E611-B7F7-02163E014147.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00C8C916-561E-E611-8D0B-02163E0146A2.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00D87AA4-4D1E-E611-9895-02163E0127A7.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00ED8C57-4B1E-E611-AAE8-02163E0133C1.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/0203E153-6A1E-E611-B6ED-02163E011CAD.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/02132225-6B1E-E611-AD96-02163E014765.root",
# 277069
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0004128B-E74D-E611-A955-FA163E7DD696.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0049EB23-EA4D-E611-902C-FA163E652810.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/00FBA566-F94D-E611-B793-FA163E7C989C.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0201B2FC-F94D-E611-9813-02163E01218E.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/022B0DA1-F34D-E611-905E-02163E0142D3.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/02827EA1-F34D-E611-BB0A-02163E014548.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0405AC07-EF4D-E611-AB30-02163E01449E.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/046D401F-F94D-E611-B1BF-02163E01474F.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0489977E-EA4D-E611-9DB3-FA163EC974A2.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/04BF9517-F54D-E611-B42B-02163E0144D3.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0639477C-F44D-E611-A5F4-02163E011A3F.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0642A9F2-F44D-E611-AE91-02163E0138C1.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/068EF33F-F34D-E611-87E6-02163E012726.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/06B59B88-F94D-E611-A175-02163E014146.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/06D39F44-E94D-E611-9A05-02163E0133B0.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0818E287-F94D-E611-BD33-02163E0144B0.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/08E6E065-EF4D-E611-8820-02163E01444C.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0A1FE33B-F04D-E611-B11F-02163E011B76.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0A57D891-F94D-E611-AB10-02163E01419D.root",
"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0CC25B50-F54D-E611-A641-FA163EA74264.root",

    )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('260627:97-260627:1910')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('273725:83-273725:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277069:80-277069:9999')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_tracks.root')
)


process.d = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("generalTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)



