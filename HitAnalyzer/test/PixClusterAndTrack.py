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
  

#"/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/308/00000/54B99B3A-989C-EF42-8265-B946EEE3F0D4.root",
#"/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/308/00000/6316F278-4532-A942-87E1-8FA7F351FAB4.root",
#"/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/308/00000/896BE210-B8EA-4545-8482-F8A201DDB83B.root",
#"/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/308/00000/97768502-3E51-DF42-B947-F01937EE4A4B.root",
#"/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/308/00000/9E5000BB-0D06-5F4F-B00E-0A2566789B7C.root",
#"/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/308/00000/DDD3C8D1-719A-C94D-9141-C46B5CBFD8DF.root",
#"/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/308/00000/DEF99CEF-27A7-474D-BA11-4450A9B82216.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2EB3AEDA-747D-FB46-9583-BC1F1241CFE7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A31A15DC-DC4F-9A48-A278-9816B5B0C23B.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/63FBC89A-3FAF-E642-AD8A-FAD8C4A4D139.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6B191B61-9EB9-1644-B271-87840E7F7315.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/50BA4D46-9AA5-7B44-BC45-71655453DD56.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F1ED7D7B-ABC5-214A-957A-DD92E752425F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/521A7618-FFF8-0C43-9189-F7AAF55637BE.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/09AA7E93-0472-5543-8069-B8317366D4DD.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F60236DA-AFC3-2541-9721-A9F7450F42D7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6D4FAFF9-115D-234E-8746-03A1B6136220.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/DF03B2C1-9A51-E540-BB6F-FDAE3B385E56.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/96684FC8-5AF9-0045-A088-58695B5FE879.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6544B394-ED7F-D54A-A0A3-D384FE0E42F7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/3CD78F7C-B80D-E348-A821-CD98F9084912.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/23F9FB24-E592-7C4B-B65D-53CC5556AC1F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C1CA8CFB-2AE3-384F-AAE6-FE73A3EC39C3.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C7DE23CA-4B92-764A-9DD6-4383F1E8199C.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/36EEBCC3-918F-924A-993B-C46E50909B2C.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C726877F-D225-514E-BAC7-DF3BD425D0EA.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/1735EA73-9AE3-2643-A624-592F78661977.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/7135945F-DD85-914B-82C6-A3FDB5764E1A.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F71296F4-8E93-5442-889C-2FE0BF70DA07.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E731F1A0-DBBC-0744-8A40-5DFC02C01500.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/00DAA8F6-1B23-C349-9660-D311315DA8B5.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/7194FBD2-5F3A-9A44-9D95-162C22E7F3F8.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/02848B0E-F7AD-534D-8059-00FCFF64A944.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/D8FCCA21-FE58-4744-9EAE-27AC62DB9F8B.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/B8B14525-7223-5243-AA1C-99C38FC9BF96.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/8ECC4BF2-43C7-E04B-89E7-69583DD3CF49.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/7307C674-918A-434D-89F3-9009FB70A341.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/BE255598-195A-8340-8698-D0311183F464.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/855ED9DE-7F0F-134F-A26F-8109F10EEC44.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/BA8937A9-16C5-3D4B-9798-A7193848A1DA.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/16F04289-9780-D443-BEB3-78A6A87247B9.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/F8411E08-AA79-6742-B026-AED84FB91B20.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/4FE9A7CA-371C-DF43-AA8A-2F44C991B4FA.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/DCB1BB6D-34B6-2442-91E9-0F3B66CC6CDC.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/4C4C2F4B-F3EE-744D-878B-1DA41E4B3E28.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/CF601D34-0446-704B-97A6-42D606A43C97.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/F90B0CA0-4BB8-3E48-8023-E94ABB98F10C.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/8E48968E-D1EC-7944-B5C4-67CBE7B0DB66.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/5057C704-2E23-024D-85B4-F05E01ACBFC7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/BF1B1EA9-3D93-3D48-8C5B-D57CBFC0F675.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/060C9EF6-9B47-A948-9E0E-08ACBC8AD5E4.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/0EB50C41-BDA0-7547-80BE-FD74B344ABA8.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/A2AD0CF2-E217-6042-9E3B-BC30EF80C03A.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/169/00000/7B7F2DC3-6E51-D743-AA9B-95D90D5112B4.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/F63EC611-F0CD-274F-A8F8-4B4A70E2BC95.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/38D09541-9D50-4643-95F1-5A131E8A9F1B.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/914E7E9B-9E3B-0E47-B768-00C4B2960CAF.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/979294AA-25D4-6D49-82D5-258DE59492C7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/96345D43-7292-D348-AE3C-9952490CF424.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/7F67424C-49F3-3645-99DD-D3215CF054B3.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/2E1474DA-640F-E544-8FCB-13DC93F14223.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/91B12C9C-3C30-134F-9058-9B4ED5DD086B.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/CC12C490-A51E-154E-90E9-F23ECE5C5E34.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/E959CF33-A54C-0549-90A3-5A27B36A2819.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/834DECF0-F427-AC45-BDB4-1E7A5098D55E.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/2276194E-E208-4A44-B706-7798CA749F6C.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/A9355AFC-45BC-CC48-A7C9-ECE7EABC8367.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/D599A38B-07A3-7D43-94E2-921513B2D9E1.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/AE5930CA-B8F3-6748-AB5F-47E050D99594.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/7754ACB5-E0F3-CE4C-B370-787553F3B076.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/C421C8AE-0C3F-8643-B7BD-F79B4B7C8A48.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/168/00000/9FF3FD5C-50D0-ED41-AF41-B50BC806346B.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/571/00000/704B6D2C-25AE-FA45-89E0-53DFA2EFD8EE.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/571/00000/ADD2C51B-0F71-0746-A779-A584D9739F12.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/571/00000/0977B209-7D86-7D4F-BCC4-9D7DE2FC8A12.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/571/00000/780D0158-A812-F24C-AC8A-905EE3ADB829.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/571/00000/392858E7-E299-7A45-AD67-D253E628721C.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/571/00000/605DCBA5-D437-B54A-8EAD-704DA5A4924A.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/4D91F356-7F84-B644-8EA2-A667CC1C3D9B.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/2AAE7B84-316C-7141-B89B-732F2C071294.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/905396B9-7685-CE44-B631-B398E571DD93.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/B194D4D3-D094-464B-A9E1-D385176174A1.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/FA0BAB37-F024-484F-B2CA-453F657F4F20.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/2C245079-75E4-8F43-BA46-1CE7AA7CEA8C.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/E32063D6-A592-1748-A676-290BF149653F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/8B6788D7-9CEB-C84E-BA74-DFB1E76853CC.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/5040BC50-798D-2B42-9693-7C0AFCCF961D.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/847C5378-421B-9340-B576-384E5AF7B47B.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/13A25B7C-2668-4C4C-9949-BA82629C40FE.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/D8B0D420-C156-1D41-89D5-7F5A3199AACC.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/33465C63-7472-4840-8CB3-4F2BD725F922.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/E1B2F040-245F-3148-A4FE-9CD3638C714E.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/9C396898-DDB9-454F-9F93-A06C09BC3AAF.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/7A6D70FB-AC50-AC47-8FBE-D702317BF9C0.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/C69EAC34-A5ED-3844-AB94-E44BEB59F181.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/4BB75EE0-8D62-414C-960D-294890925CFB.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/6F1D7E93-8658-4146-912F-8FB09DF095A0.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/01915A11-B199-094B-A9E1-3EA2D903E791.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/4714B28A-D233-3A4C-A6E2-40A49C1256D5.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/324/420/00000/9E77729E-450C-BF4C-9F97-1EA4047D05F7.root",

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/A942FEA1-0586-C441-A46E-19C5515F2498.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/E4421625-0AE4-5843-A581-4A56CC7778A9.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/F939DEE3-697C-9B40-901C-007CBFB47193.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/5A6E7AC8-20DA-CC4E-8E1C-A456C1B97B5E.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/78FE343D-51F7-8F44-9978-1D191BC6301A.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/26AE7D86-8A08-FB43-A490-A3995AA7F604.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/9AE20490-133E-BB44-A9C7-3E8525A4073D.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/65F84781-3A8E-D545-8E5E-CBC8BE987B89.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/04DFA5AA-290C-7744-ACA7-F7227C52D7D7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/8008A581-F2D2-B343-ABBC-F85B1F670F19.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/BB0C09D9-79E4-1C40-9DCB-EC5ECCE66AEE.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/808C732E-0028-4F48-9396-E163DA9C4DA2.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/72B21936-8F0B-C44F-A39E-D63A57478DC1.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/A6AAA7F1-DB4F-B447-B43E-B9C269A5BC0F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/C0923D62-B80D-3C4C-ADA4-D6C94A2A46A7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/1F103BDD-9968-CE49-BD10-AB86A2F677C2.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/0C6A3C31-2358-C543-880F-82DBA7EBBFF4.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/DBFB486D-DF18-FE44-9872-A42D1E736F64.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/BD85231F-68FB-7F4F-806D-6E6847CE1A91.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/A3C6E028-DD24-8B4B-8D32-FB7B043B0380.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/470/00000/F8AC92DC-F513-CF4D-9756-C65434F6C94A.root",

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

  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('319104:24-319104:185')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('318661:1-318661:999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('320569:57-320569:308')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('320712:54-320712:242')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('324571:1-324571:515')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:62-325159:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325308:43-325308:9999')


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

