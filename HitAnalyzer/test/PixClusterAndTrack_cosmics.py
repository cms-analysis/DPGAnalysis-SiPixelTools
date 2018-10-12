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
#process.GlobalTag.globaltag = '100X_dataRun2_Express_v2' # 
process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' 
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
    HLTPaths = ['HLT_Random*'],
#    HLTPaths = ['HLT_ZeroBias*'],
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
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist )

  fileNames = cms.untracked.vstring(    
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/6DC681B8-12C2-534B-922C-568CAFEFBA23.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/44ED9E8D-F71B-4540-9DA9-A476E705CADE.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/ED612143-02C5-C742-A268-D003C747F9C2.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/350D164E-3069-F04E-9F19-C0A454843DCB.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/AAB64A7D-0F39-FC45-BA97-45D28FC8E3CC.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/302034FE-E606-904A-A8E0-4CCA4A251C8A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/3ACA86EB-0D8D-4244-A5A2-39884AB523A9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E14E1734-7B44-0249-A375-E2439155D574.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/F086480A-5AB6-A14C-908A-91CD5C255C24.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/2027DE44-C425-F24B-A84B-3F0F95C270FC.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E059BE35-1B40-9A40-8CC0-EF9F64B0216C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/A5AA5CEA-D219-3B49-997E-E0E8409E123E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/CCD4B54B-7D9D-854E-9AF0-9609F3919316.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/A852B0F3-5435-9341-9023-B0FEE829EDF2.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/617D0DC5-580F-8141-94B5-C5CA46A8C7E9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/2BAED97D-1E7E-A847-A927-639A481D0F3A.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/B6B7D160-B11A-F948-8D58-74F9D0064E4F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/FDD637D3-29CA-BD4A-A2B0-9C76145D89F4.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/79CB1294-16A9-BA4A-A4BC-B53DD52A2C4F.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/0282F21A-F0AF-F04E-B9E1-B8D07E81269C.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/90C23117-F8C0-7041-98E2-2830101292E9.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/EC806BEB-271F-B84A-9AE0-D940E9B9DB8E.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/7BDAE626-99D5-6449-8155-DF4B9F4B61DF.root",
"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/324/344/00000/E65A59C7-C58B-7E4B-8EEA-30F39C4E1B8E.root",


#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/7A2080A3-C458-A84F-83D9-4396299C4673.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/316A4552-038C-9246-919B-FF693C985662.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/4E6AF211-CFE5-CF4C-9888-8C9DE1F228BC.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/42F90AE0-D6EC-BB42-8A50-8BD595676F32.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/5CD9472F-DD37-414E-BCFC-0E700EB83218.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/F128B5FF-D9F6-3E4C-A500-FBD84891086D.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/B00430A7-3B82-E446-A668-0F8B12FE1824.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/066A3A77-F591-F241-8FE4-F55404539634.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/A4F33470-B61D-DE4B-BD85-7D2D45A69F3A.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/2DA15AAE-3A4E-DC4F-8CF3-0DDDDB804308.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/FC28DBC2-A680-EC40-BA8F-366A68FD3267.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/6347ED7C-C5EF-6944-A6B0-693B28C7C1F2.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/B37312FA-BEF9-714D-8E70-20DC42371BEC.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/43438277-5181-D84C-B333-136B4C3A5163.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/0B5F2CA0-9268-274D-93CB-42D3B657DA3F.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/9771F512-A00C-C045-81E3-8F1522178847.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/14D1B3FA-1A35-F145-B840-BA1B43DD1762.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/7A3DF481-67EA-5740-A30F-991E9F06ADE5.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/77DC5FC1-B924-8140-9F7F-DD51A260EB62.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/397E0803-DD06-BE43-A5F6-2C1F202921FB.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/FA8B269B-0E0A-4841-9ED2-2DD818C03D62.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/103ECD27-36CC-5745-AED1-0DB8C14B4BA1.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/6A436618-B0C6-BD42-8EEF-1D9D23071B84.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/644/00000/E6D0AC65-3678-B448-97BF-F9464B1ECCD0.root",

#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/0D30D270-A239-D046-A6E1-8AD5F3AA4110.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/247AB0A4-454C-BE41-96A9-226D7E63FD90.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/4A839A35-EDDA-274C-A241-9ACC395ED296.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/CEBD277A-F59B-544F-A7E9-B4732020805B.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/0CA3F082-EE4B-8F4C-88FB-8188E9C4EA9F.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/F12894D4-A297-5346-A194-01FCB3251C99.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/47C9786F-B34C-6A46-AE2A-218A42AE0622.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/1B679F56-14CE-E24A-94F0-3E9862B66BEA.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/15136893-0289-CE40-ACC3-2FD13AC05367.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/A0983D8D-A25A-2845-A40B-24FF521D9B08.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/14DACE1D-BADA-0341-B24E-46490922D83D.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/319C6BE7-F4C1-8A43-BD4A-A87817A1F4FA.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/59D49DA8-C56C-E84F-894C-BAF23AC13F04.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/556877A7-8444-AF4E-B178-9885047DE6ED.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/7B49BDD8-4A84-D343-A2B3-4D4482635BFA.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/6BBBF575-D7AD-BF4A-9893-B6646285D0EA.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/40B0A645-15BA-DE4D-A823-FF8D6750F62A.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/4073BF14-C9D5-B54B-92D4-A3E233F1B476.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/D5FAD90D-C468-4941-BD35-9F08B96B2D35.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/EAF8E3E6-9F43-4C45-A62E-2E1A737BAC4D.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/103/00000/B92AC319-40CF-4C46-81ED-4016393E88FF.root",

#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/43CBF804-F0B2-F64B-A54C-C61EA6CFCFD6.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/7842DC98-E4EB-A849-BA2A-D715D56FF679.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/FD2680A5-73E9-E648-AF36-20813FDA6070.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/FA719C3D-D094-FC47-8A14-901D0BD0F3DF.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/2D11CA56-0EE1-4D4D-9402-5C4B70609781.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/3A76CC1D-D575-124C-AA15-C13636314768.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/CF91D6C2-32E0-7C49-AF10-B98C715A2D84.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/566011E3-11D4-EA43-B5A3-DE81338CFE93.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/FE93632D-8AC8-B443-B3B3-2F4B22FABE1C.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/1819BD6A-2B5E-D242-9929-F7B11709E813.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/CAD8F295-5F33-674F-9D2C-FFD2EF2C51B0.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/63C657B0-A53D-0E43-A770-82EC4411FE0A.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/D87AF147-BBDD-5A4D-BCE2-2CCBA9C61A9B.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/2F8E6DB2-C4E3-A741-ABF8-08A25E5DAD2E.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/4901DE6B-1CAB-6E46-A792-E2E65790F368.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/58BF4C50-D76F-1541-8491-D254D1B03919.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/8DC3C417-5C15-4840-8659-E2080EDBF90B.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/4AF0E3A5-87F6-6F45-9B9B-23E38FC2A1B3.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/1FA6BE18-2107-B84E-989C-7BF55338980D.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/8C2BF39A-60FB-EC49-832C-78427B6B0579.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/323/003/00000/BE62E1D4-3D99-E349-BE50-FDF3F80516D0.root",

#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/F0FAFAAB-A0B5-E811-AF1E-02163E019F09.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/309BD2A2-A1B5-E811-A4E9-FA163E35B744.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/288EBA29-A2B5-E811-9C29-FA163EDE04F2.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/88C42BBB-A4B5-E811-B021-FA163EE347FC.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/8CF1E339-A1B5-E811-88EC-02163E01A047.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/2C555906-A2B5-E811-BFEA-FA163E771C26.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/048AE234-A1B5-E811-A2BA-FA163EE347FC.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/1EBF3C03-A2B5-E811-A805-FA163E2EA36F.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/8EFEB404-A1B5-E811-AB3F-FA163E669861.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/8E1B6F63-A2B5-E811-B158-FA163E669861.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/0E74B93B-A2B5-E811-A453-FA163E8CA0FA.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/8828A8C0-A0B5-E811-8D08-FA163E70B547.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/702349BA-A0B5-E811-92E8-FA163E35B744.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/B877785C-A1B5-E811-BCA2-FA163EDE04F2.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/0A6385D0-A1B5-E811-A7D0-02163E019F09.root",
#"/store/express/Run2018D/ExpressCosmics/FEVT/Express-v1/000/322/673/00000/AC6DB1D9-A1B5-E811-BC15-FA163EE347FC.root",

  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('324344:0-324344:9999')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana_cosmics.root')
)

process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    Normalise = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("ctfWithMaterialTracksP5"),
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
    Tracks = cms.InputTag("ctfWithMaterialTracksP5"),
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
process.p = cms.Path(process.hltfilter*process.d*process.c)
#process.p = cms.Path(process.hltfilter*process.d*process.c*process.c1*process.c2)
#process.p = cms.Path(process.d*process.c)  # for mc 
#process.p = cms.Path(process.d) # for cosmics


