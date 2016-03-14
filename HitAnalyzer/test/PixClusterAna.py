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
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v0' # for 254227
# 2016
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
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
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Physics_part*'],
#    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
#    HLTPaths = ['HLT_ZeroBias_*'],
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


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100000)
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

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/06F327B5-64E7-E511-A635-02163E014279.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/086E75D6-64E7-E511-8067-02163E011C5C.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/0A61E2D8-64E7-E511-BF26-02163E012078.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/10C7B0F3-64E7-E511-8716-02163E0138CC.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/1CD98AC4-64E7-E511-9583-02163E0145D8.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/1E74EAD6-64E7-E511-A290-02163E014520.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/22401FE6-64E7-E511-B53C-02163E01349B.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/2290A9F7-64E7-E511-8826-02163E01430D.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/242D2FFB-64E7-E511-946F-02163E011A8A.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/28CD9AFA-64E7-E511-A65A-02163E011939.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/2AE04DD8-64E7-E511-B09B-02163E011D29.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/2EF82CCE-64E7-E511-B2C0-02163E0144D7.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/3C3482D9-64E7-E511-9CA4-02163E014233.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/3C63F47D-65E7-E511-A706-02163E0135E8.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/3E1944BD-64E7-E511-8659-02163E0137A9.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/42A06494-64E7-E511-BA14-02163E011AA6.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/489EE3D4-64E7-E511-BB8D-02163E014431.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/4ECBFD45-65E7-E511-93B8-02163E01183D.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/50C916F6-64E7-E511-B33D-02163E011F42.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/52229BEE-64E7-E511-BD1F-02163E014225.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/5443D0E1-64E7-E511-9B0F-02163E01444F.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/54A6A3D3-64E7-E511-9249-02163E01340B.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/56840348-65E7-E511-8D97-02163E012725.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/5C0F49F9-64E7-E511-9DA1-02163E01352A.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/5C8F4BC7-64E7-E511-A3E8-02163E01383F.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/62E2C7EC-64E7-E511-B8D3-02163E011936.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/6662CBE2-64E7-E511-A9DE-02163E0145FA.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/68A7A79C-64E7-E511-8292-02163E014636.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/68D78E99-64E7-E511-9B80-02163E0143BF.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/6C26964D-65E7-E511-9CDB-02163E0137B2.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/740FA5A8-64E7-E511-907B-02163E012B2C.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/763B9BED-64E7-E511-8E50-02163E01454E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/765E735B-65E7-E511-8F52-02163E012B15.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/781CB488-64E7-E511-87A0-02163E013787.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/78760825-65E7-E511-A1A0-02163E011B7B.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/7AAEE9EB-64E7-E511-AD81-02163E0118E4.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/8A552888-64E7-E511-B8D6-02163E011E9A.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/8E766463-64E7-E511-B3FE-02163E014169.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/8EE37DDC-64E7-E511-A588-02163E012695.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/9858D717-65E7-E511-BA25-02163E01253F.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/9E79C5DA-69E7-E511-A602-02163E014266.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/A05A830C-65E7-E511-8BB3-02163E0141F2.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/AEB581BE-64E7-E511-9811-02163E011B04.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/B042B5F7-68E7-E511-B109-02163E0142E7.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/B07C39CF-64E7-E511-840C-02163E0119C1.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/B2A5F1AA-64E7-E511-9936-02163E01397E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/BA4EE2D1-64E7-E511-8C1E-02163E011DC2.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/BC6A6EDB-65E7-E511-B9AA-02163E01382B.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/C276C884-64E7-E511-B82D-02163E01446D.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/C4396A5E-65E7-E511-A9EB-02163E0136EA.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/CACE48E5-64E7-E511-A94F-02163E012250.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/D48B7FDD-64E7-E511-A307-02163E011822.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/DAF148DA-64E7-E511-9D4B-02163E0120CD.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/DC0B747E-65E7-E511-A7C2-02163E011CDA.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/E01F23DB-64E7-E511-AE25-02163E0146F7.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/E09CD9F2-64E7-E511-9AA7-02163E014179.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/F01B79D8-64E7-E511-BBAB-02163E013552.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/F096D6CD-64E7-E511-B2AE-02163E01208E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/F8FA2120-65E7-E511-9767-02163E011B50.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/FA640AD6-64E7-E511-8EC7-02163E012151.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/277/00000/FE0F43A3-64E7-E511-9CA4-02163E0134E1.root",
])


process.source = cms.Source("PoolSource",
 fileNames =  myfilelist
#  fileNames = cms.untracked.vstring(    
# muonia 
#"/store/data/Run2015A/MuOnia/RECO/PromptReco-v1/000/248/038/00000/22A90F7F-5A14-E511-AD96-02163E012BE3.root",
#single muon 
#"/store/data/Run2015A/SingleMuon/RECO/PromptReco-v1/000/248/038/00000/0C5FA27E-5A14-E511-8D85-02163E012385.root",
# charmonium
#"/store/data/Run2015A/Charmonium/RECO/PromptReco-v1/000/248/038/00000/FC099721-5A14-E511-A3D1-02163E012B9A.root",
#
#"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/248/025/00000/50B4C032-0314-E511-A718-02163E013569.root",
#"/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/248/025/00000/08590F92-0115-E511-9483-02163E011C17.root",
#"/store/data/Run2015A/ZeroBias/RECO/PromptReco-v1/000/248/025/00000/30A880F2-0414-E511-9E31-02163E0139A3.root",
#"/store/data/Run2015A/ZeroBias1/AOD/PromptReco-v1/000/248/025/00000/341760D5-5114-E511-A92E-02163E0129E4.root",

#"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/247/607/00000/681AB913-9110-E511-98DA-02163E014374.root",
#"/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/246/963/00000/00F51FE4-090C-E511-8F33-02163E014299.root",
#"/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/963/00000/0006E9B1-4A0A-E511-AD47-02163E0138BC.root",

#  "file:/afs/cern.ch/work/d/dkotlins/public/data/clus/clus_zb_248025.root"
#  "file:../scripts/clus.root"
# MC
#"/store/relval/CMSSW_7_1_0_pre7/RelValTTbar/GEN-SIM-RECO/PRE_STA71_V3-v1/00000/887DEA5B-5CD1-E311-BB97-002618943923.root",
#"/store/relval/CMSSW_7_1_0_pre7/RelValTTbar/GEN-SIM-RECO/PRE_STA71_V3-v1/00000/8EEC0F1F-9FD1-E311-966F-003048FFD76E.root",
## 2012, cosmics
# "/store/data/Commissioning12/MinimumBias/RECO/PromptReco-v1/000/186/791/CEA46376-7069-E111-B395-001D09F24D67.root",
# "/store/data/Commissioning12/Commissioning/RECO/PromptReco-v1/000/186/791/6EC3470C-6F69-E111-93CA-001D09F241B9.root",
# "/store/data/Commissioning12/Cosmics/RECO/PromptReco-v1/000/186/791/6A54D2A0-6D69-E111-ABA8-001D09F2441B.root",

#  )   # end the list "by-hand"

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('207469:0-207469:51')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('207477:76-207477:570')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('208686:73-208686:463')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('210534:24-210534:347')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana.root')
)


process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d) # for cosmics


