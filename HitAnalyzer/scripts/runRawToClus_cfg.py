import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("MyRawToClus",eras.Run2_2017)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
#process.load('CondCore.CondDB.CondDB_cfi')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.load('Configuration.StandardSequences.RawToDigi_cff')

# needed for pixel RecHits (TkPixelCPERecord)
process.load("Configuration.StandardSequences.Reconstruction_cff")

# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2017
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # fpr 90W 
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # from CMSSW927
#process.GlobalTag.globaltag = '92X_dataRun2_Prompt_v11' # Promot
#process.GlobalTag.globaltag = '94X_dataRun2_ReReco_EOY17_v2' # for RERECO
# 2018
process.GlobalTag.globaltag = '101X_dataRun2_Express_v7' # 
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
    HLTPaths = ['HLT_ZeroBias_v*'],  # simple democratic ZB
#    HLTPaths = ['HLT_ZeroBias_part*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionInTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_LastCollisionInTrain_*'],  # empty
#    HLTPaths = ['HLT_ZeroBias_FirstBXAfterTrain_*'], # empty
#    HLTPaths = ['HLT_ZeroBias_IsolatedBunches_*'], # ok
#    HLTPaths = ['HLT_ZeroBias_FirstCollisionAfterAbortGap_*'], # ok
#    HLTPaths = ['HLT_ZeroBias_BeamSpot_*'],
#
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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

myfilelist = cms.untracked.vstring()
myfilelist.extend([
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
#)
fileNames =  cms.untracked.vstring(

# 2018
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/C6A89B55-674E-E811-97C8-FA163EC3589F.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/D804AFA5-6D4E-E811-A7C0-FA163E0422A8.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/6C83C7E8-6D4E-E811-A1E2-FA163EFABB07.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/FAB3BEA6-6D4E-E811-9C4D-FA163E0D9E97.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/B64D8002-6E4E-E811-8945-FA163EAE6D6D.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/24429B99-6D4E-E811-8A95-FA163E73E806.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/AC118FF3-6D4E-E811-827E-FA163E9ECA9A.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/56439E1E-714E-E811-9B66-FA163E0972CE.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/20094BF8-6E4E-E811-B36F-FA163EB8644D.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/642/00000/14D88AC9-6D4E-E811-841B-FA163EA2F461.root",

#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/3EFD24FC-434D-E811-B07B-FA163E390CE2.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/8AAB9738-454D-E811-876B-FA163ED26202.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/58A83A0D-494D-E811-B5B6-FA163EB68CF3.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/DA1BFC38-484D-E811-A5BE-02163E01A083.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/68E10058-494D-E811-84E5-FA163E23B3D2.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/8254EB82-4C4D-E811-AC43-FA163E6B94A5.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/FAD64631-484D-E811-A732-FA163E03FB38.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/CCCD2D53-494D-E811-9E32-FA163EC36191.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/52B66769-494D-E811-B36C-FA163E7FE60F.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/BA5BEAB1-484D-E811-9EF0-FA163ED5E288.root",

#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/C290907E-AD4C-E811-A7D7-FA163E1A07C9.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/EEABE79D-B34C-E811-808F-FA163E08198F.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/784E7F34-B84C-E811-9669-FA163E9626F3.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/72EA65A4-BC4C-E811-85B2-02163E01A012.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/86BE40D5-BE4C-E811-BE36-FA163E34040E.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/10D85837-BF4C-E811-A164-FA163EF96190.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/288F333D-BF4C-E811-8E68-FA163E5B522C.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/A49C6A34-C14C-E811-BAFC-FA163EF76148.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/3ECF472B-C14C-E811-84A3-FA163E456877.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/488/00000/CEE2F834-C24C-E811-8E63-FA163E04EEFE.root",

#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/5AEB971A-1E4B-E811-B71A-FA163EA3B75E.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/82DDE0C5-1E4B-E811-B501-FA163E015631.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/A843137B-284B-E811-98BC-FA163E721D55.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/92FF6C71-2D4B-E811-8085-FA163EDCB9C5.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/E8BD76F2-334B-E811-9004-FA163E395E49.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/DEAF63E2-324B-E811-9F22-FA163E478CA6.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/34C683CD-334B-E811-AB5B-FA163E924F37.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/62EB56CA-334B-E811-9B56-FA163EC861A6.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/5EF0C235-334B-E811-9CD6-FA163E147082.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/357/00000/B2996EE1-334B-E811-BF87-FA163E9AAC67.root",

#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/68404DFF-E049-E811-A97E-FA163ED782E1.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/D0F9505F-DE49-E811-9D3A-FA163E893DB0.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/A2FCF3CB-E049-E811-BFE6-FA163EE0AD35.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/EE71594E-E249-E811-A85B-FA163EB7A2BD.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/F4F2BE3F-E149-E811-AD70-02163E017FEC.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/1E136F95-E649-E811-ABD4-FA163ED213E0.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/A849BA9B-DE49-E811-BC6B-FA163E47E71E.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/26BE65AD-DE49-E811-A10E-02163E01A08D.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/926C6898-E049-E811-87B5-FA163EB7A2BD.root",
#"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/265/00000/8237B3D8-E049-E811-AF4A-FA163E956980.root",

#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/88C50189-FE48-E811-B7DA-FA163E421A08.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/78E66B76-FF48-E811-A31B-02163E01A029.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/AEBD1870-0049-E811-8CE6-FA163ECDEF72.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/6434CB95-0049-E811-9D0E-FA163E76C639.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/1A3046D7-0049-E811-BD68-FA163ED479D8.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/103C337A-0049-E811-AACF-FA163EE9EDC1.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/C8FD2BC1-0049-E811-93A5-FA163E3EE95B.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/06384292-0049-E811-B465-FA163EDF7DB7.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/E6E15116-0149-E811-8D61-FA163E7F5F92.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/315/188/00000/7C794AE3-0349-E811-92F8-FA163E39C0DF.root",

#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/A82B59D1-8343-E811-9CA4-FA163E58A668.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/A0D6A094-8043-E811-A12E-FA163E5C47D8.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/66401921-8343-E811-AB6E-02163E0150B6.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/3E2EBEEC-8343-E811-8E31-FA163E87271F.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/2AAE793E-8343-E811-9A54-02163E016545.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/E82B8DDA-8343-E811-B360-FA163E48FFBD.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/CE92FB27-8043-E811-9D67-FA163E9D9A43.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/CA3C8246-8043-E811-BBFB-02163E01A099.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/E050CADD-8043-E811-B34D-FA163E81FC4F.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/18966911-8143-E811-9EB7-FA163E785EB9.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/10ABC636-8343-E811-9982-FA163E7F17BA.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/657/00000/72A65D83-8343-E811-8731-02163E014A2B.root",

#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/5868F906-7B43-E811-88C3-FA163E0EE1E4.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/EA5BE871-8243-E811-96BE-FA163E21820E.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/28D22B6F-8343-E811-8A67-02163E00BFE8.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/2ADD3C14-8443-E811-BCED-FA163E21820E.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/EA751AC7-8143-E811-8AF7-FA163E37BDDC.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/2881BA1D-7E43-E811-920D-FA163E9D9A43.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/B0E2D00A-8243-E811-9906-FA163E87271F.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/C83BAD63-8143-E811-8777-FA163E78EE04.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/50B1D96E-8343-E811-9558-02163E00B12E.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/5406C58B-8143-E811-B27E-FA163EA5B62B.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/656/00000/1AABEE29-8143-E811-9AFF-FA163EF45B09.root",

#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/001885E8-6142-E811-BBE6-FA163EFCFF2C.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/502F5F29-6242-E811-851A-FA163E5043E5.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/C81CB4EC-6142-E811-9ED7-FA163EE8287C.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/DA54BA5A-6242-E811-A95B-FA163E2FECF2.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/E6E11EB7-6242-E811-B44F-FA163E464EF2.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/BC3CF299-6242-E811-8402-FA163E621846.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/76DB3D1D-6342-E811-883F-FA163E95EB41.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/C04A0D3C-6642-E811-93DE-FA163ECD65AF.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/F4592DC4-6642-E811-B10D-FA163E71CF10.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/515/00000/40A0026B-6742-E811-A5D0-02163E01762E.root",

#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/DA67BAAF-5942-E811-B3F8-FA163E3A5F0F.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/847EA9C3-5942-E811-A7EF-FA163EF52681.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/E4E11920-5B42-E811-BC0B-FA163E3E5302.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/9206B125-5A42-E811-8710-02163E014B85.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/521EF1BE-5842-E811-AACF-02163E01600A.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/241ECF20-5B42-E811-9A3E-FA163EECD9DC.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/4EB2E456-5B42-E811-87D8-FA163EA6B7A8.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/DA69211D-5B42-E811-B89C-02163E01A097.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/242A2654-5B42-E811-BE0F-FA163E3C297E.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/507/00000/92AEB812-5E42-E811-8403-FA163E2807E5.root",

"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/560F5A99-5C42-E811-ACE6-FA163ED141EE.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/5AFD0C2A-5942-E811-8A5B-FA163EA392AD.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/D27D394B-5942-E811-AE87-FA163ED06402.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/66421161-5942-E811-9325-FA163E2B0F48.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/54B47F7E-5942-E811-9E7A-FA163E3EF566.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/2A5DA723-5942-E811-9B5A-02163E01317C.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/662A8960-5942-E811-B738-FA163E3E5302.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/880FC835-6142-E811-90B2-02163E012DD8.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/2A2EEB3B-5942-E811-8B87-FA163ED36C09.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/D2125A27-5942-E811-BBA9-FA163E29DCBF.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/FC6A1B7F-5942-E811-9833-FA163E81E25F.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/BEA6FC36-5942-E811-B685-FA163E27F8CE.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/28F90535-5942-E811-98BE-FA163E840CCB.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/62B05F67-5942-E811-8FD2-FA163E6FAA82.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/5875CC2B-5942-E811-9E6A-FA163E25A55A.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/C4DB3045-5942-E811-80B7-02163E0147E8.root",
"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/506/00000/8EF42437-5942-E811-B5C6-FA163EFA1AD6.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/266/277/00000/0004807C-AFE5-E511-83C1-02163E011EAD.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",

 )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:49-314890:1433')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:50-314890:60')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:500-314890:510')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:1300-314890:1310')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315357:44-315357:872')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315488:42-315488:1126')


# for Raw2digi for data
process.siPixelDigis.InputLabel = 'rawDataCollector'
#process.siStripDigis.ProductLabel = 'rawDataCollector'

# for digi to clu
#process.siPixelClusters.src = 'siPixelDigis'

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
       threshold = cms.untracked.string('ERROR')
#        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:clus.root'),
    #fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/clus/clus_zb_248025.root'),
    outputCommands = cms.untracked.vstring("drop *","keep *_*_*_MyRawToClus"), # 12.4MB per 100 events
    # save only pixel clusters 
    #outputCommands = cms.untracked.vstring("drop *","keep *_siPixelClustersPreSplitting_*_*"), # 5.6MB per 100 events
    #outputCommands = process.RECOEventContent.outputCommands,  # 4.9MB per 10 events 
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    splitLevel = cms.untracked.int32(0),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

# pixel local reco (RecHits) needs the GenError object,
# not yet in GT, add here:
# DB stuff 
useLocalDB = False
if useLocalDB :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
#        record = cms.string('SiPixelGainCalibrationRcd'),
        tag = cms.string('SiPixelGainCalibration_2018_v1_offline') 
#        tag = cms.string('SiPixelGainCalibration_2017_v2_bugfix') #in DB
#        tag = cms.string('SiPixelGainCalibration_2017_v2_bugfix_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v2_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v3_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v4_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v5_offline') # v4 fixed
#        tag = cms.string('SiPixelGainCalibration_2017_v6_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v4')
#        tag = cms.string('SiPixelGainCalibration_2017_v5')
#        tag = cms.string('SiPixelGainCalibration_2017_v6') # full tag & in DB
#        tag = cms.string('SiPixelGainCalibration_2017_v4_1337_offline')
#        tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
 	),
       ),
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_full.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v5_full.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_full.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_1337_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_offline_oldvcalHL.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v6_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v5_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v4_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v3_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v2_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2017_v2_bugfix_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_phase1_ideal_v2.db')
     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v1_offline.db')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 

#process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siPixelDigis.IncludeErrors = True
process.siPixelDigis.Timing = False 
#process.siPixelDigis.UsePilotBlade = True 
process.siPixelDigis.UsePhase1 = True

# digis from raw (this is the default)
#process.siPixelClustersPreSplitting.src = 'siPixelDigis'
# modify clusterie parameters
#process.siPixelClustersPreSplitting.VCaltoElectronGain = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = -60
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = -60
#process.siPixelClustersPreSplitting.SeedThreshold = 10 #  def=1000
#process.siPixelClustersPreSplitting.ChannelThreshold = 10 #must be bigger than 1
#process.siPixelClustersPreSplitting.ClusterThreshold = 10 # def =4000    # integer?
#process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 10 # def=2000 # integer?

# To select full granularity gain calibration 
# process.siPixelClustersPreSplitting.payloadType = cms.string('Full')


process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)


process.d = cms.EDAnalyzer("PixClusterAna",
#process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis_clus_old.root')
)

#process.p = cms.Path(process.siPixelDigis)
#process.p = cms.Path(process.siPixelDigis*process.a)
#process.p = cms.Path(process.siPixelDigis*process.siPixelClusters)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# suppress output file or not
#process.ep = cms.EndPath(process.out)
