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

"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/A942FEA1-0586-C441-A46E-19C5515F2498.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/E4421625-0AE4-5843-A581-4A56CC7778A9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/F939DEE3-697C-9B40-901C-007CBFB47193.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/5A6E7AC8-20DA-CC4E-8E1C-A456C1B97B5E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/78FE343D-51F7-8F44-9978-1D191BC6301A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/26AE7D86-8A08-FB43-A490-A3995AA7F604.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/9AE20490-133E-BB44-A9C7-3E8525A4073D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/65F84781-3A8E-D545-8E5E-CBC8BE987B89.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/04DFA5AA-290C-7744-ACA7-F7227C52D7D7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/8008A581-F2D2-B343-ABBC-F85B1F670F19.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/BB0C09D9-79E4-1C40-9DCB-EC5ECCE66AEE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/808C732E-0028-4F48-9396-E163DA9C4DA2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/72B21936-8F0B-C44F-A39E-D63A57478DC1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/A6AAA7F1-DB4F-B447-B43E-B9C269A5BC0F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/C0923D62-B80D-3C4C-ADA4-D6C94A2A46A7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/1F103BDD-9968-CE49-BD10-AB86A2F677C2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/0C6A3C31-2358-C543-880F-82DBA7EBBFF4.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/DBFB486D-DF18-FE44-9872-A42D1E736F64.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/BD85231F-68FB-7F4F-806D-6E6847CE1A91.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/A3C6E028-DD24-8B4B-8D32-FB7B043B0380.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/F8AC92DC-F513-CF4D-9756-C65434F6C94A.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/A49792CB-6CB4-E811-95C4-FA163EC32196.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/AC5233CC-6CB4-E811-82B1-FA163ED229B7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/0ED3F305-6DB4-E811-8B73-FA163E7F4F04.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/AC1537A0-6DB4-E811-8D41-FA163E533386.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/A24C539C-6EB4-E811-80DA-FA163E7CEFDF.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/B635FEE9-6EB4-E811-8C43-FA163EBB8D74.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/349EA9FD-6EB4-E811-BB0B-FA163E4B6F1D.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/2E3B975C-6DB4-E811-B2A8-FA163E6FA269.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/96EC336E-6EB4-E811-A63A-FA163EB0AF7F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/605/00000/2A2FADD7-6EB4-E811-8088-FA163E1409FE.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/2EED2132-6AB4-E811-84C3-FA163EE3D13F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/303F0A46-6AB4-E811-81BA-FA163EC32196.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/88910938-6AB4-E811-8945-FA163E337D5D.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/A639ED6F-6AB4-E811-A584-FA163EBDB555.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/121DBCA6-6AB4-E811-9285-FA163E7835F3.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/56A4BAFF-6AB4-E811-95D4-FA163E45DA89.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/CE8109B6-6AB4-E811-91E2-FA163EDAE846.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/84B1FFCF-6AB4-E811-9D29-FA163E165244.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/54207318-6BB4-E811-88DB-FA163E47C8BD.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/322/602/00000/9047E4D5-6AB4-E811-B3FE-FA163E49FA5E.root",

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

  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('319104:24-319104:185')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('318661:1-318661:999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('320569:57-320569:308')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('320712:54-320712:242')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('323470:38-323470:270')
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
process.p = cms.Path(process.hltfilter*process.d*process.c)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.c1*process.c2)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.d1*process.d2)

#process.p = cms.Path(process.d*process.c*process.c1*process.c2) # for mc
#process.p = cms.Path(process.d) # for cosmics

