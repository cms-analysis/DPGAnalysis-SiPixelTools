#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("cluTest",eras.Run2_2017)
                   
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2017
#process.GlobalTag.globaltag = '90X_dataRun2_Express_v4' # 
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # 
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v4' # for 926
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # for 927,9
#process.GlobalTag.globaltag = '94X_dataRun2_ReReco_EOY17_v2' # for 2017 rereco

# 2018
#process.GlobalTag.globaltag = '100X_dataRun2_Express_v2' # 
process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' # 

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

"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/526C97FB-0096-E811-BCB8-FA163E8BA02A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/80B971B5-0096-E811-8DA3-FA163E010862.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/94C35580-0196-E811-B71F-FA163EB33FD8.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/34899197-0296-E811-90DA-FA163E88A838.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/5C22EDED-0296-E811-B458-FA163E63AE40.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/D8480FA2-0796-E811-97B5-FA163E5469DB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/C6717CD2-0C96-E811-A739-FA163EE96767.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/70F5A96A-0F96-E811-B71A-FA163EF21D25.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/A6272A60-0E96-E811-9E6A-FA163E086F03.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/5C10946F-0F96-E811-870A-FA163E0B2F68.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/FAFD318B-1096-E811-AB69-A4BF01277823.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/70B0CBCD-1096-E811-B4FC-FA163EB9878F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/1EE27F67-1096-E811-9184-FA163ECFE6A5.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/02E19413-1296-E811-977B-FA163EB63606.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/C2F5CDE5-1396-E811-9053-FA163EE81CA0.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/AEF017AB-1396-E811-9C7F-FA163E11507C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/1ED28904-0196-E811-9FDD-FA163E6A7294.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/BAC9E869-0396-E811-8560-02163E013B89.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/8AEA2C56-0596-E811-93AB-FA163EA692A9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/712/00000/F2E2E7F6-0796-E811-902E-FA163E6788A6.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/BC7B85EB-6E94-E811-94AD-02163E01A000.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/AA0376C1-6E94-E811-8513-FA163EC4E221.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/94948D0F-7094-E811-8757-FA163EE68B78.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/5C985E06-7394-E811-96B0-FA163E333059.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/B877832D-7394-E811-BB89-02163E00B2C5.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/601F8434-7B94-E811-A573-02163E00BE22.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/E408C2FE-8294-E811-BD39-FA163EFCD9FA.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/FED82299-8494-E811-9DA2-FA163E1F3FE7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/F2986356-8794-E811-A469-FA163E746B64.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/4A8971B2-8994-E811-A5BB-FA163E3FD732.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/FC35D1FE-8D94-E811-B6C8-FA163E95D558.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/6EFABB98-9194-E811-B3E5-FA163E336245.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/D8F17278-8A94-E811-BECC-02163E013C00.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/B6BDE0B5-9494-E811-807D-FA163E0422A8.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/E0D9B458-9194-E811-9C7E-FA163E80F0F5.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/EA55D4B1-9294-E811-9EAB-FA163E89DAA9.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/4E164395-9494-E811-B140-FA163E8A2E19.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/0E0F9288-9494-E811-9501-FA163EB95E71.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/9AAECB1D-9594-E811-8A18-02163E019FF1.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/B8F8A6AA-9294-E811-8644-FA163E22141F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/320/569/00000/A04CFE44-9C94-E811-93B7-FA163E0CAA0D.root",

#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/500AA548-2684-E811-A161-FA163E5C2908.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/5CA8D8A5-2784-E811-9E89-FA163E01B3E3.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/366A1917-2584-E811-90F4-02163E01A0F3.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/B4CF5640-2684-E811-BCFC-FA163EB4CC5C.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/E6C31E52-2684-E811-9222-FA163EE7DE3E.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/C896D95A-2684-E811-8594-FA163E56C46A.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/6CFCE364-2684-E811-BFB9-FA163E12C02D.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/A8355E35-2584-E811-A4C1-FA163E158EBC.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/B2EFAC53-2684-E811-97A5-FA163ED0CAD6.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/38B1C9B6-2784-E811-9253-FA163E205A29.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/8A13A407-2584-E811-AF17-FA163ED62B5C.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/D6007F51-2684-E811-9637-02163E017F10.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/FCC9B38C-2684-E811-8EB7-FA163EF040BF.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/3C5C70CA-2784-E811-BC66-FA163EF040BF.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/C4175AF0-2984-E811-A3CF-FA163E4DEB7C.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/E2503824-2584-E811-A158-FA163E880BF2.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/8ACF7A2E-2584-E811-B868-FA163E79885E.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/0E4C6164-2684-E811-AF64-02163E01A0F1.root",
#"/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/460/00000/647CD960-2684-E811-AD81-02163E00AC97.root",

#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/C8AEEAA6-E97D-E811-A439-FA163E29D251.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/D0DDB8D8-DF7D-E811-B8EA-FA163E2F5EF2.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/16BE4366-E27D-E811-9AA3-A4BF0127AF07.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/16AF8723-E67D-E811-8ADA-FA163E6716A8.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/FAC68D98-DD7D-E811-B573-FA163E4AE302.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/8A2F2AA2-DF7D-E811-9AE8-FA163EFF87FE.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/BA84AAA1-EE7D-E811-BCAB-FA163EB165D3.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/44965139-E27D-E811-92C6-FA163ED7F3BC.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/529746E9-E27D-E811-BC6A-02163E01A0AB.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/FE49AA77-E77D-E811-9586-FA163E816F3E.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/9E2B0E73-DE7D-E811-9842-FA163EA263D0.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/80B8785F-E67D-E811-8985-A4BF0112DB74.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/44F26F22-DE7D-E811-863F-FA163E11EAA8.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/DCF08FA1-DF7D-E811-BF5C-02163E019F9F.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/16DF60A8-DF7D-E811-A173-FA163E60CC95.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/319/104/00000/24E65732-E57D-E811-8F83-FA163E8FF902.root",

#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/14A74212-1979-E811-994B-FA163EDA67FA.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/9677CD0C-1979-E811-B77E-FA163E068260.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/64AF304F-1D79-E811-A8B4-FA163EC6DE18.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/4291E67E-1D79-E811-898F-02163E019F2A.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/00A6B11D-2279-E811-B39D-FA163EF29411.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/10E7D42B-2279-E811-BBC2-FA163EE59C15.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/306D0528-2279-E811-B9D0-FA163EFF0809.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/74EFA06B-2279-E811-98D2-FA163E529D85.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/42469B97-2279-E811-9D6A-FA163E2434A3.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/661/00000/986EEA41-2379-E811-8FF0-FA163ED05A75.root",

#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/069A7418-F868-E811-858C-FA163E034210.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/7E362915-FB68-E811-9D9B-FA163EA45FDA.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/54C8942B-FD68-E811-95FD-FA163E9CBD30.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/EA2AF28F-0169-E811-9E82-02163E01A085.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/F6BB77BA-0469-E811-A44D-02163E019FB9.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/EE3BDDB9-0469-E811-9527-FA163EB656C6.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/5A95CAD9-0469-E811-AE6E-FA163E66B10C.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/E2E60F38-0569-E811-92AF-FA163EB8310B.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/FCDEAAF4-0469-E811-84A5-FA163E9641D4.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/475/00000/88A545FC-0569-E811-B81D-FA163E614A5A.root",

#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/96A86B03-4B68-E811-8553-FA163EC07E76.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/7084EDA1-4C68-E811-805D-FA163ED532A6.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/D61B8C7B-5E68-E811-B7F8-FA163E3F84E6.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/F20A6597-6368-E811-A460-FA163EA4290B.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/76F3344D-6268-E811-BBA2-02163E018013.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/7A0800F8-6468-E811-9B14-FA163E2AF1D2.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/3E291922-6568-E811-9DA8-FA163ED62B5C.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/A21FDAF7-6468-E811-8501-FA163ED1245E.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/12B690A3-6568-E811-8C82-FA163EA9CF9F.root",
#"/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/317/434/00000/342A349A-6768-E811-935B-FA163E3F1A44.root",

# mc
#"/store/relval/CMSSW_9_2_3/RelValNuGun/GEN-SIM-RECO/PUpmx25ns_92X_upgrade2017_realistic_v2_earlyBS2017-v1/10000/12995CE4-3851-E711-B4A6-0CC47A4D7602.root",
# "/store/relval/CMSSW_9_2_3/RelValNuGun/GEN-SIM-DIGI-RAW-HLTDEBUG/PUpmx25ns_92X_upgrade2017_realistic_v2_earlyBS2017-v1/10000/1ECCAF11-2E51-E711-AE03-0025905B8594.root",

  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('319104:24-319104:185')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314472:55-314472:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315357:44-315357:872')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315488:42-315488:1126')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('317434:48-317434:271')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('317475:33-317475:89')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('318661:1-318661:999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('320569:57-320569:308')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('320712:54-320712:242')
#

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana.root')
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
    Select1 = cms.untracked.int32(0),  # select the cut type, 0 no cut
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


#process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.hltfilter*process.d*process.c)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.c1*process.c2)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.d1*process.d2)

process.p = cms.Path(process.d*process.c*process.c1*process.c2) # for mc
#process.p = cms.Path(process.d) # for cosmics

