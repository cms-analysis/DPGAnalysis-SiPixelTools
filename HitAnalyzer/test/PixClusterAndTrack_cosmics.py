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
process.GlobalTag.globaltag = '100X_dataRun2_Express_v2' # 
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
#    HLTPaths = ['HLT_Random*'],
    HLTPaths = ['HLT_ZeroBias*'],
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
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/301/",
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist

  fileNames = cms.untracked.vstring(    

# mc
#"/store/relval/CMSSW_9_2_3/RelValNuGun/GEN-SIM-RECO/PUpmx25ns_92X_upgrade2017_realistic_v2_earlyBS2017-v1/10000/12995CE4-3851-E711-B4A6-0CC47A4D7602.root",
# "/store/relval/CMSSW_9_2_3/RelValNuGun/GEN-SIM-DIGI-RAW-HLTDEBUG/PUpmx25ns_92X_upgrade2017_realistic_v2_earlyBS2017-v1/10000/1ECCAF11-2E51-E711-AE03-0025905B8594.root",



#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/001D171B-F02B-E811-AE35-FA163E9F7EA5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/001FC020-D62B-E811-805F-FA163EF28A5C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/003B013D-D52B-E811-86E1-FA163E121244.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/00824F23-F02B-E811-A35A-FA163E3EF566.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/0097FE3B-F02B-E811-B24C-02163E019F73.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/00C364A9-F02B-E811-B966-FA163EB108A8.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/023D38D4-DF2B-E811-87E6-FA163EBDF68C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/0250DBFF-DA2B-E811-BE98-FA163E8CC404.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/0298AF23-F02B-E811-B785-FA163ED7B2EC.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/02B42FD0-FC2B-E811-8DA6-FA163EE7A704.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/02BE268B-D72B-E811-83D4-02163E01A000.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/004B757B-3B2B-E811-880A-FA163E5577DD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/0063B2F0-252B-E811-982E-FA163E3CC334.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00AE2312-202B-E811-AE91-FA163E6CDBB2.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00C46971-322B-E811-AEF1-FA163EFD7515.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00D62B56-3D2B-E811-ACAE-FA163EBF1F42.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00DBDBE7-282B-E811-9A94-02163E014C0D.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00E89BFA-252B-E811-957F-FA163EE347FC.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/0209221B-2A2B-E811-9807-FA163E83AA16.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/02159E91-3A2B-E811-A915-FA163E22408B.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/02370D59-282B-E811-8D77-FA163E3A5F0F.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/02018B3C-7229-E811-A0F9-FA163EAE0640.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/026F8CBA-7029-E811-B2A5-FA163EE7A704.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/02E5DB20-7729-E811-8188-FA163E34CDF1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/044D2DBB-7629-E811-B684-FA163E34CDF1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/044FD472-7029-E811-BABD-FA163E9E72CB.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/046682FA-7329-E811-9A27-FA163E9E72CB.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/04D7DA93-8429-E811-8833-FA163E5D2DA7.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/061FF441-7829-E811-9611-FA163E126B49.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/0A22EC82-8829-E811-BFBE-FA163EF4135C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/0AA0F41F-9229-E811-A339-FA163EDA9841.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/001FB648-E328-E811-A8FC-FA163E664BFC.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/003C44E8-D928-E811-B9BE-FA163E09D221.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/005EEE07-CB28-E811-BE2E-FA163ECA2FB5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/00BE0970-D328-E811-BFFB-FA163E71F02B.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/00DC315F-A828-E811-9844-FA163E734BE1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/020502DD-CF28-E811-BD98-FA163E1BF0F8.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/023153A7-AB28-E811-851D-FA163E32403C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/02ED1EE7-BB28-E811-9261-02163E0165AA.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001000000/02F99B7D-DD28-E811-845E-FA163EB8D19E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/0465AFC9-DD28-E811-BD4E-FA163E0A8836.root",


#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0066B415-FA27-E811-AEA9-FA163E96C3EF.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0067A8F6-0A28-E811-923C-FA163E47AC10.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/007CAB86-F427-E811-A7DF-FA163E2D5FC7.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/00C4D8C5-D627-E811-B30C-FA163E21BFF5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/023B9490-E127-E811-AA77-FA163E4C8771.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02A4554A-1628-E811-8606-FA163E8676B9.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02D079B3-1B28-E811-95CD-FA163E954ED5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02E03A82-0328-E811-88A9-FA163E265254.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0419114D-0928-E811-8491-FA163E8F5243.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/04AB8003-1F28-E811-8F50-FA163EF5D4C6.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/04FB3A78-F027-E811-8956-02163E017706.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/060A0B31-E327-E811-B57B-FA163E34CB74.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0610FAF1-D727-E811-9C22-FA163E93EDBE.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0616F9D0-D027-E811-B9DD-FA163EF225E6.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0618395D-D027-E811-B3F1-02163E00B478.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/04304C4B-5F27-E811-BCA5-FA163E447F8E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/0855D54E-5E27-E811-9622-FA163E2848B2.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/56FC28E0-6027-E811-9D59-FA163E447F8E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/9C57EB28-5F27-E811-A0F3-02163E013C39.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/A8469AB1-5E27-E811-B5AE-FA163E98F3FD.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0026B7C4-8926-E811-8C04-FA163E87F2DD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0862AF9C-7A26-E811-91EB-FA163E333059.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/08AFF0E8-8126-E811-A465-FA163E1414FD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0C672356-8026-E811-80A1-FA163E148FBE.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0CFE4FC2-8226-E811-85FE-FA163E148FBE.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/004D47A8-F524-E811-B57A-FA163E19119E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/00F4A829-F824-E811-8E46-FA163E0839FE.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/026C25BD-C324-E811-8C89-FA163E38DEFD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/02A7E24F-C024-E811-BF4E-FA163E7B0F38.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/02D4F400-0225-E811-8AFD-FA163EAEF82B.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/248286D7-2424-E811-B7EC-FA163E85C1BF.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/2E3FC689-2524-E811-B511-FA163E7947B1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/343980D7-2224-E811-A524-FA163E355A35.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/3646FED1-2224-E811-9502-FA163E8DF100.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/48FC8562-2424-E811-843E-FA163E445507.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/06BF6B46-2524-E811-83DE-FA163E5B4AF5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/10BA6E45-2324-E811-9ACE-FA163E355A35.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/1C63E23D-2524-E811-BA00-FA163EFDF28A.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/2043D215-2424-E811-8281-FA163E5DC871.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/22487555-1C24-E811-96C3-FA163EA09EAB.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/12CA4EF1-EF22-E811-8D04-FA163E125028.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/22593422-EF22-E811-88E1-FA163EFAF5BE.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/",
#"/store/data/Commissioning2018/MinimumBias/RAW/v1/000/311/321/00000/2040707D-FA22-E811-8373-FA163E9DE046.root",
#"/store/data/Commissioning2018/Cosmics/RAW/v1/000/311/321/00000/98BAF94A-ED22-E811-B8B9-FA163E45557F.root",


#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/301/465/00000/00D3F7BD-8A85-E711-918E-02163E01A37A.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/02DBAD4C-AE7C-E711-B44F-02163E019DD0.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/04BBD43B-0876-E711-8C3E-02163E0119F7.root",

#
  )   # end the list "by-hand"

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('300122:44-300122:9999')


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana.root')
)

process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    Normalise = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
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
#process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.hltfilter*process.d*process.c)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.c1*process.c2)
process.p = cms.Path(process.d*process.c)  # for mc 
#process.p = cms.Path(process.d) # for cosmics


