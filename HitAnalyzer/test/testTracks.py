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
# 2017
process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # 
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
    input = cms.untracked.int32(10)
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


 "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/296/108/00000/22174A51-CB49-E711-8375-02163E01384C.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/296/075/00000/045D378C-9949-E711-A42A-02163E011F09.root",


# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/640/00000/029DD0A3-8645-E711-AE8C-02163E013978.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/636/00000/003BFB77-6E45-E711-AECD-02163E013407.root",


# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/436/00000/F60EF27A-FC43-E711-86F2-02163E019E38.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/426B4782-FC43-E711-9328-02163E011A76.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/F853E939-FF43-E711-A59D-02163E013479.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/454/00000/0E5832FA-1744-E711-A404-02163E01A6AA.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/458/00000/0A262760-1644-E711-926B-02163E01A1E0.root",



# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/381/00000/00034176-5043-E711-AA6F-02163E019C98.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/318/00000/06085100-B142-E711-A1D0-02163E01A1FA.root",

#    'file:tracks.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_73/tracks/tracks1_mc73_13.root',

# 272783
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/783/00000/008955C7-3A14-E611-99D8-02163E0133B7.root",
# 272798
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/798/00000/FEF97F57-8414-E611-AF3C-02163E0125B1.root",
# 273017
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/273/017/00000/005CCAB3-7617-E611-94AC-02163E01382D.root",
# 273158
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/158/00000/000FBA71-E317-E611-B784-02163E0141D0.root",
# 273725
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/725/00000/00030CAB-6C1E-E611-90F9-02163E0137A8.root",
# 277069
#"/store/express/Run2016E/ExpressPhysics/FEVT/Express-v2/000/277/069/00000/0004128B-E74D-E611-A955-FA163E7DD696.root",

    )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('260627:97-260627:1910')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('273725:83-273725:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277069:80-277069:9999')
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
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)



