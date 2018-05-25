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
process.GlobalTag.globaltag = '101X_dataRun2_Express_v7' # 

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

"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/3EFD24FC-434D-E811-B07B-FA163E390CE2.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/8AAB9738-454D-E811-876B-FA163ED26202.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/58A83A0D-494D-E811-B5B6-FA163EB68CF3.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/DA1BFC38-484D-E811-A5BE-02163E01A083.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/68E10058-494D-E811-84E5-FA163E23B3D2.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/8254EB82-4C4D-E811-AC43-FA163E6B94A5.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/FAD64631-484D-E811-A732-FA163E03FB38.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/CCCD2D53-494D-E811-9E32-FA163EC36191.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/52B66769-494D-E811-B36C-FA163E7FE60F.root",
"/store/express/Run2018A/ExpressPhysics/FEVT/Express-v1/000/315/509/00000/BA5BEAB1-484D-E811-9EF0-FA163ED5E288.root",

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

#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/FC0276D2-0A46-E811-AB5F-FA163EF10B4F.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/FA2064E6-0A46-E811-A763-FA163EBA050E.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/2094D2F7-0A46-E811-8514-FA163EC92774.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/643AE5AE-0B46-E811-9B1C-FA163EB07E7A.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/76E518FA-0B46-E811-AAFE-FA163EC92774.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/22B9D6EE-0B46-E811-86B1-FA163E57FDA9.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/D457E9F0-0A46-E811-B353-FA163E6AD357.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/0C40B963-0C46-E811-A52E-02163E017F44.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/CE87D501-0C46-E811-ADE8-FA163E980E92.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/860/00000/EECCD14B-0B46-E811-A9B6-FA163EA4AD14.root",

#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/162C0656-3844-E811-89FB-FA163E60EB64.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/B4202F54-3844-E811-A709-02163E019F84.root"#,
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/A239181D-3E44-E811-8CC8-FA163ECF8DB3.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/28441B25-3E44-E811-BC54-FA163E09BE42.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/EC8F963A-3E44-E811-B5E0-FA163E126126.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/D200BA2E-3E44-E811-BD0A-02163E0176D5.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/88884529-3E44-E811-A83F-FA163EFC186C.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/B8FE061C-3E44-E811-9301-FA163EEC73C2.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/58874C35-3E44-E811-BF1A-FA163EC4DE33.root",
#"/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/755/00000/9811B322-3E44-E811-BE87-FA163E848E20.root",

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

# mc
#"/store/relval/CMSSW_9_2_3/RelValNuGun/GEN-SIM-RECO/PUpmx25ns_92X_upgrade2017_realistic_v2_earlyBS2017-v1/10000/12995CE4-3851-E711-B4A6-0CC47A4D7602.root",
# "/store/relval/CMSSW_9_2_3/RelValNuGun/GEN-SIM-DIGI-RAW-HLTDEBUG/PUpmx25ns_92X_upgrade2017_realistic_v2_earlyBS2017-v1/10000/1ECCAF11-2E51-E711-AE03-0025905B8594.root",

  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('307076:50-307076:90')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314472:55-314472:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315357:44-315357:872')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315488:42-315488:1126')


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
process.p = cms.Path(process.hltfilter*process.d*process.c*process.c1*process.c2)

#process.p = cms.Path(process.d*process.c*process.c1*process.c2) # for mc
#process.p = cms.Path(process.d) # for cosmics

