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
process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' # 
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
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/88FA64E4-D1A1-3748-B8C8-7D2C33053969.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/FE593DCD-2B82-F640-AD3C-CB5A174BF78A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/DD7E0C95-8CA3-194A-8ED8-24EC644C2607.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/624C895B-87DA-8C4D-97AE-55AB272FDB00.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/7D0AEB19-D47B-B84A-85E2-21FAADA64F91.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/2057CB2E-9752-E842-AFBB-0BB76FEDF2C9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/91D54AD5-599A-1949-BD88-E4C5DBF6748E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/A3B70477-88F5-A443-8A98-27877176D6D3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D99B54FD-0248-6C4B-AEA1-C201D5517F33.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/1510983E-F9D8-F84A-A99B-81043847BA9E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/74E120B4-A779-5141-92DB-C98F5D7FEC59.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/40B6CA7C-E3D6-1B4E-89E6-A488DC55344A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/0C6EB4BC-629F-C94A-B8A5-23E1FC2683FE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/1AAA7A73-1C19-284E-A2FC-5FB387504ACE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/24BAE196-F9EE-264E-B518-64701EEF2C37.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/0B8FE34C-A2A9-9D49-BD9E-35E87DDB8D59.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D09F9E69-0AE8-8446-88F8-138228AA9ABE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/3B4A63D7-1351-4247-9A9B-9661662E6E12.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/184E494C-63D8-2648-A020-5DCF2139CF32.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/57C37CA4-66B7-CE4B-8524-50ECB1F40B71.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/12C5F3BB-5255-E943-A492-E191CBAFFEBB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/A6CB12C6-0A03-6348-8FA7-C795F8DCB443.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/7EEAA057-5C58-254F-8AA4-817C2E3C543C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/6317D182-219B-294D-AC95-5BEBD5F9F07D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/1D6EDE5A-7B4F-6042-A26F-1DFBE069CCAE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/02419A7D-A844-6E4D-AADA-57D93D0D6E88.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/BE4E0668-60B3-4A47-82DF-66BFD95CAEF3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/5E90FACE-CB84-914A-A30B-8D0CCE1D39EF.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/58516B5D-07AC-3A47-ABA5-71BAF685E6D0.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/DBD69EF0-7F0E-704C-BD9D-2B17DFD8CFA7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D26EA913-8B01-9146-86BA-0E38FAEEA68C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/B42F8619-65D4-344C-9EFC-FB9AF80A439D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/18261A24-69A4-E648-B528-FF13836FBED3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/00A4A86C-D9C2-394B-A1D2-7BDE1351C3A7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/1BEFFA15-49A2-964F-B7EE-30CADD2CE4D3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/F1B9FEA2-0A5B-704A-BC4A-EF77E4FB1B28.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/1DC60BD7-7AF6-BE40-8850-38C02BABD49E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D3C1C231-F94E-3C41-A3CF-AE342A975261.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/9C69E5B7-C9A5-E74E-9D22-6387BBD23F7D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/81E265A6-7A19-694F-9F10-6AC882DBC829.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/33F2678E-8417-CA4F-BAE6-45FFABCA60AC.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/ECEBE2EF-D71B-884D-9650-2B7950EBEC29.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/689C57AA-B006-C64E-A710-2557693E2838.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D1C05BFB-A40E-4E48-ACF7-E6CB81DFC0BA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/2F35A7F5-25CF-3D49-AA53-55930B414D2F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/DF7C623F-8F44-AF41-9813-5F3DD81853F9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/F7C22AC4-1B42-BF4C-978E-3C33FCE1F079.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/88954DAF-D70A-284B-88EC-0042BADB26B2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/99FD8A0B-CC03-9F4D-8ECD-FB498D513113.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/7112D407-B8B3-B14F-BF04-75CCA8F83DB2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/C12ED27E-0E92-D848-BCBF-FC886DE274D2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/FBFC4F84-831F-C14E-93C9-6E0385972CF1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/2EB56FDB-F9D8-EC46-9AA5-C866300D214C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/27F9D5AA-0E9C-1C47-BD08-A9057CF75601.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D1F2FC50-9DA3-CA42-9A22-D67DA93B11A7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/5BF400D9-187B-3C4E-A683-303575AC55C9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/CA1A5E15-3D69-A140-97E4-572137A0ED18.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D7F7FED9-99F9-DF47-8479-EDC42130117A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/8304820A-8926-7F4C-A22F-8E57E63C0CE6.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/3579737F-EF09-B044-A7EA-48313D96C1A8.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/D4024119-DD3F-9D45-B4F8-707AE77C6764.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/356D8820-0288-5048-9B84-BB7E8B5D33C1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/64DDD165-1FE8-9F46-9B92-7587D7C96EEB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/4D8FBF7F-F1DF-BB41-81B6-2C7688C1B46C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/59EFF262-61A2-5846-84AF-FD572CABFACB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/A74B23DF-0A5E-8142-80BC-925F0D67CF03.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/097/00000/8AD727A3-FC77-614A-B4EC-902AFD1FC83A.root",
])

process.source = cms.Source("PoolSource",
 fileNames =  myfilelist
)

#fileNames =  cms.untracked.vstring(

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

#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/F8754875-47F3-D04F-A103-10E7E1E04472.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/96DD83FC-C015-5A4A-807D-5348485FDA93.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/2519D64C-B2FF-D44C-B265-ADE20A6E089D.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/17BF074E-CE40-3444-80F4-6D9D9CB76F8F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/2D229E12-D898-6540-B23F-34FFB43B95E2.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/15B76681-98A5-A44C-B0A9-2D48E48E003B.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/75DAD820-319A-8D4D-AD08-6992879B029A.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/DC5F3038-5CE4-A845-9E7B-064A505F1AA9.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/8E544E10-FDCA-224E-AB5C-C07718FEF219.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/E2DD548D-FDA4-5A42-A2AA-BC820F871BAD.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/F1A492F9-86A5-A048-8AF5-4CB015AAC9D0.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/EBF0DB6D-DABB-9341-99B1-296CFB547DC7.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/4648A804-CFCF-E14E-AD22-64077F6E5448.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/FCF4F91A-6201-A14C-A78D-AFB4FF066817.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/7D9FD273-9C8B-FD4A-B16F-61216BB1F3DB.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/D8A80C8C-9247-744C-BD23-1C3734BE5452.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/B1D40AD9-1060-0D40-B8E7-44B8E5C0F4C0.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/70297A28-2B4C-BA4D-AB97-0B6087808539.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/DEECFAEC-0850-4044-9560-EB45724B9E1F.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/DA85F331-9F45-1B4F-85F4-0E2CD3A26EFF.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/9B501EC5-0B2F-0C4D-AA97-4B2D4FB48E58.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/F436BEC4-7F7A-DF4F-8974-7E2B62EA8C19.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/0D7CE635-2C1E-A745-909E-7D7D07EA2300.root",
#"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/170/00000/B2A13090-ED78-BE45-83B4-FDA4DF2238FD.root",

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

# )
#)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('324847:3-324847:999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:50-314890:60')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:500-314890:510')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('314890:1300-314890:1310')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315357:44-315357:872')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('315488:42-315488:1126')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('317434:48-317434:271')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('317475:33-317475:89')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:62-325159:9999')


#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:1-325097:27') # no hv  no collisions 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:28-325097:36') # no hv collisions  
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:37-325097:58') # hv on 
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:59-325097:9999') # stable lumi 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:1-325097:100') # overview


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
#        tag = cms.string('SiPixelGainCalibration_2018_v3') 
#        tag = cms.string('SiPixelGainCalibration_2018_v2_offline') 
#        tag = cms.string('SiPixelGainCalibration_2018_v1_offline') 
#        tag = cms.string('SiPixelGainCalibration_2018_v7') 
        tag = cms.string('SiPixelGainCalibration_2018_v8') 
#        tag = cms.string('SiPixelGainCalibration_2018_v9') 
#        tag = cms.string('SiPixelGainCalibration_2018_v9_offline') 
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
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v1_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v2_offline. db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v3_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v9_offline.db')
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
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
process.siPixelClustersPreSplitting.SeedThreshold = 10 #  def=1000
#process.siPixelClustersPreSplitting.ChannelThreshold = 10 # must be bigger than 1, def=10
#process.siPixelClustersPreSplitting.ClusterThreshold = 10 # def =4000    # integer?
process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 10 # def=2000 # integer?

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
    Tracks = cms.InputTag("generalTracks"),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)
process.d_cosm = cms.EDAnalyzer("PixClusterAna",
#process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Tracks = cms.InputTag(""),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis_clus.root')
)

#process.p = cms.Path(process.siPixelDigis)
#process.p = cms.Path(process.siPixelDigis*process.a)
#process.p = cms.Path(process.siPixelDigis*process.siPixelClusters)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco)
#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.d)
process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# for random cosmics 
#process.p = cms.Path(process.hltfilter*process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d_cosm)

#process.p = cms.Path(process.siPixelDigis*process.pixeltrackerlocalreco*process.a*process.d)

# suppress output file or not
#process.ep = cms.EndPath(process.out)
