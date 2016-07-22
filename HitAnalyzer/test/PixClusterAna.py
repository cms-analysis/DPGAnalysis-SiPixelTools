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
# 2015
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v0' # for 254227
# 2016
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8' # for 272
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


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
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
"/store/data/Run2016B/AlCaLumiPixels/ALCARECO/LumiPixels-PromptReco-v1/000/272/022/00000/1C85457B-D30F-E611-8EE6-02163E014695.root",
])

process.source = cms.Source("PoolSource",
 #fileNames =  myfilelist
  fileNames = cms.untracked.vstring(    

# 272022
#"/store/data/Run2016B/ZeroBias1/RECO/PromptReco-v1/000/272/022/00000/0014666A-D80F-E611-83FA-02163E011999.root",

# 272783
#"root://eoscms//eos/cms/tier0/store/data/Run2016B/ZeroBias/RECO/PromptReco-v1/000/272/783/00000/1C4AD70E-7916-E611-9B0D-02163E0140FF.root",
#"root://eoscms//eos/cms/tier0/store/data/Run2016B/ZeroBias1/RECO/PromptReco-v1/000/272/783/00000/02BD007E-6E16-E611-994B-02163E01468A.root",
#"/store/data/Run2016B/ZeroBias1/RECO/PromptReco-v1/000/272/783/00000/02BD007E-6E16-E611-994B-02163E01468A.root",

# 272798
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v1/000/272/798/00000/FEF97F57-8414-E611-AF3C-02163E0125B1.root/",

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

# 274314
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/00ABDB9B-0E28-E611-9406-02163E014602.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/04B93E10-4328-E611-A8CA-02163E0135D0.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/0AA8AD56-4328-E611-8758-02163E013975.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/0C60C3F9-4328-E611-9C46-02163E014340.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/1009B80A-4628-E611-9E6E-02163E014587.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/142DC033-1528-E611-AD43-02163E0137D5.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/14435014-4328-E611-B78C-02163E013713.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/14EC7CCA-4328-E611-A875-02163E01342C.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/16299419-4428-E611-9538-02163E01433F.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/1A57B62A-4328-E611-9D78-02163E01339F.root",

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


  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('273725:83-273725:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('274314:97-274314:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('277069:80-277069:9999')


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana.root')
)

process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d) # for cosmics


