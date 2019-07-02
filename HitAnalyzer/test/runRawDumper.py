#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],  # not in ZB stream
#    HLTPaths = ['DST_Physics_v*'], useless 
#    HLTPaths = ['HLT_Random_v*'],
    HLTPaths = ['HLT_ZeroBias_v*'],
#    HLTPaths = ['HLT_L1Tech54_ZeroBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v1'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
# LumiPixels
#    HLTPaths = ['AlCa_LumiPixels_Random_v*'],
#    HLTPaths = ['AlCa_LumiPixels_ZeroBias_v*'],
#    HLTPaths = ['AlCa_LumiPixels_v*'],
    
# examples
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

# process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('dumper'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)
#process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = 'Debug'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('raw.root')
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2EB3AEDA-747D-FB46-9583-BC1F1241CFE7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A31A15DC-DC4F-9A48-A278-9816B5B0C23B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/63FBC89A-3FAF-E642-AD8A-FAD8C4A4D139.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6B191B61-9EB9-1644-B271-87840E7F7315.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/50BA4D46-9AA5-7B44-BC45-71655453DD56.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F1ED7D7B-ABC5-214A-957A-DD92E752425F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/521A7618-FFF8-0C43-9189-F7AAF55637BE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/09AA7E93-0472-5543-8069-B8317366D4DD.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F60236DA-AFC3-2541-9721-A9F7450F42D7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6D4FAFF9-115D-234E-8746-03A1B6136220.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/DF03B2C1-9A51-E540-BB6F-FDAE3B385E56.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/96684FC8-5AF9-0045-A088-58695B5FE879.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6544B394-ED7F-D54A-A0A3-D384FE0E42F7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/3CD78F7C-B80D-E348-A821-CD98F9084912.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/23F9FB24-E592-7C4B-B65D-53CC5556AC1F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C1CA8CFB-2AE3-384F-AAE6-FE73A3EC39C3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C7DE23CA-4B92-764A-9DD6-4383F1E8199C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/36EEBCC3-918F-924A-993B-C46E50909B2C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C726877F-D225-514E-BAC7-DF3BD425D0EA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/1735EA73-9AE3-2643-A624-592F78661977.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/7135945F-DD85-914B-82C6-A3FDB5764E1A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F71296F4-8E93-5442-889C-2FE0BF70DA07.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E731F1A0-DBBC-0744-8A40-5DFC02C01500.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/00DAA8F6-1B23-C349-9660-D311315DA8B5.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/8CEB0826-245A-4548-91A6-E09B60806914.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/023839B8-0861-2741-B688-E617719B9BE3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/DEE69472-E936-4941-9CE2-09E54DA254A2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/05D52435-6EDD-C64F-BE98-0B407EF2BE19.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/CF7E8EA9-069A-3246-B141-38A9CD68EF13.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9774ED50-A6BA-C04E-BDBA-8343A5A5D903.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/77DCC177-6393-6F4D-AF94-F5C864C10F6C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/14B19DB0-6861-914E-AA34-EBAD9D69FACA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/8EE90046-538D-0648-8521-237C60DEFFF1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4B2AB5C0-63A2-8F46-B837-C77FC375E08F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C5D21F11-9078-BE47-8658-45CC70B529C6.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/0730B24C-EEF6-324B-AECF-EEDF1062F1FA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/DF44DD66-2633-5549-96A2-F383944B811B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/0F9C68D0-4B6E-3F4F-9227-F8686F62F652.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/8447E01D-90B1-7448-86FD-3DF793CB7857.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FE97328A-CC0E-0848-AD55-916D22945B3A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/EC2B37FC-1186-7640-A2B5-A7995745C7CE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9ECEA824-BF9D-A245-8631-E78A766599B4.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/BD7D5C03-D560-9645-A4B2-4A72E79BA77E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C340E849-9364-D046-A1B1-1A02DBB11519.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/B1E70FEA-C3C9-6949-9622-9E8079790D26.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/212A6F00-1A1B-8942-9E3D-D8D2C5C2115A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/0930CDB0-26BB-4C45-8AD0-88B2181957E1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E6BF5B26-6361-9044-B563-D6A36691C6A1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/D9A8753F-86D8-054E-90C3-7E984A88ADC9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E76B7DCB-93A2-7944-BAFE-B868764D26D2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/89D5C8BC-5AD0-3D41-86FA-8B624A6B9A48.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/B1B590B1-5194-3E48-9BFB-03E1814865AA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2B15A7CF-E11D-8E41-A115-D4403ED194AF.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/25E60925-1051-2F44-A708-8376AD16EC1C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/CE2BEE62-B6A7-D94D-AB2D-2FD7C5FBC949.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A336A28E-6E99-9447-A58B-637F4FA04E2A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/7FF9816D-3CA3-5E42-A5E0-4554FD68B3E3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/95B99E7E-430E-8948-9B07-7609F33CBD45.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E6F58793-0922-9344-BD77-D5749A50ECF5.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/0830049A-EAD6-7A4B-BC4F-FA11BE694D9D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/18D9546C-418F-5E4D-95F2-3BF1761E2E96.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FBD3F675-4822-394E-8E45-43A1BFD85766.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/618FEC57-F1DC-8B40-9AAD-4FF46C4949F8.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C881F766-16CC-5D4A-9D9B-7EAD5AB21B54.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/CBED9BB9-CAF6-064C-9E6A-3F12B58BEB5A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/03432562-7090-3F49-869F-3F96D49F122E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FC115FAD-7B0B-2B42-BB59-4599B94F5F32.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/B10354B4-B2D0-0847-98FD-5B47771C358A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/EDA53C77-375A-214C-B97E-6D1820B3ECF9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9F0ADEB5-202A-7041-AB86-4D97F6A44096.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/688A41CF-F286-A549-BFFE-7C480310CC31.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FC07FA28-89F5-664B-93AD-9282F6B71AF9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/09991C99-F358-CF41-BE39-B3043DFA2320.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/891A1521-F3AC-5041-B6D6-575CB9E714AF.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/5DDCC706-49E6-094D-BAE7-CBACC689A6B4.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/B54DD909-1CCA-AD4F-A3DE-E4D202C06AE8.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/99ED25D0-4B5C-C746-B771-E20B2F09DDB2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/80006621-13DD-2143-81F0-357DC183426B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/12135E2B-1841-DC4B-9164-FDDCD2DBC3C5.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/47D3621F-2C6F-114D-B80C-E46770674DA8.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/699710DF-9785-4B48-830D-DA9E29EE53AA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A44F13B6-595F-A243-A676-5DC5448BBE4B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F58E6267-CAEB-F046-B618-B99B4F15B524.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C6D492DF-0913-5B47-9C32-3BC3259551A6.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6ED75221-EAEB-4C40-B577-9DB4B951182C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/DDF2C65C-BA54-F548-81E4-BAF04B35C97F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4DF61693-8C8A-4046-BAD5-4CE58BA3E0F1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/1CBE6421-1CB4-3847-8544-B3D4AA9F5EC7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/BEEF1D77-93CD-4E44-B295-21CE72C15052.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/CAB3B1A5-43C3-A74B-B037-D91D0EA1DE4A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/EE14E84D-D897-EE48-82A8-3E7EE5F52CE1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4C331B82-1191-2E4D-A512-8F54DF04682C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/AB7B5315-9F2F-7848-B1EB-AAB2CD8B47D4.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C0EE3B84-42E3-5840-A0F9-2387122EA91A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/5FCBA179-8190-4644-8D77-9D4480EA421F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/8385A33A-9BFF-C748-9567-287EF3CAC695.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/8A413BB0-4AD2-DA41-8B33-8A215D7D4546.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/1845427E-F9B5-6045-9AC4-7FED79D21D66.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F01B343A-5E79-2448-817B-239EECC1AF0E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/23A72028-AE01-7143-8A61-48010516F4BE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/220A1E48-AEBF-1A40-846C-EB2EFBBA7942.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A379CC9F-5C24-7446-846F-4A7BB76F1475.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/D570E2AC-917B-BC4A-8C4D-01A39AC8B2FB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/D9078056-653B-4948-8C77-5ECA855E954B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/730DCCF1-7629-DC4D-9C3E-CEB43DB19CE2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/68647B0E-0B18-824A-BF1A-02737EB02702.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2F4E3F9E-3077-D049-B61C-07DFD9D3E61C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4A615BEE-6CBD-F54D-8DF6-A4898EB67FFA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9BBE2AE1-2446-DF44-83CA-597B812FCE44.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/089377A2-0B9B-7B4A-AE37-7DD525FB7530.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/5DACDDB6-ABAD-2646-B043-3D976A25743D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/38720BDA-3DEB-0F4C-AF94-377EC2A7E239.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F846FEC3-6668-1046-BC6F-676029C028DB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/19BF36AA-422D-C548-AC87-D8155ACF6BA4.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/EFD5A467-8D8B-3C4E-B480-0F0FE75D5943.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/92F5790D-C2D6-3D43-A69E-197800FA4B8E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9CA69C38-64B5-7340-80C1-527125AF8F52.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/61B946D0-CE68-7746-BE2B-05887C437C1F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/5AF7C3E7-0482-3A41-8744-F122F99A6F69.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/8C2E5374-8FC4-904B-BCFD-EE3A622C3AA9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E60C6B74-6483-954F-866A-95F53B71F820.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/7B03D04D-2328-284B-87E0-C6A5A5414121.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/0F8BAC3F-3A6B-724E-8F67-7BAF4CD3FCA8.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/90DB36AD-A2EC-ED41-8EF6-126C03EA992C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/15871358-7C29-4A40-88A8-27FA91B6B172.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/21679CA5-0213-8347-B16C-F7857F431A13.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F00A85C3-93E7-5A44-85C1-4110198C02A2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/831E42F8-AF7E-754D-B964-F4561C405B36.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9B9D0B98-6A30-FA4F-98E0-B7F589476694.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E39946E0-FA1E-4540-8A3D-BC37C435C553.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/08092BFA-82EC-364D-AED5-D6601FFF33BE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E2B58FB3-B53D-4840-A2A8-A7529C0F2D05.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/468D123E-BA7F-364F-8AE4-E8AEDBD2815A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F954A485-5389-7F49-958C-854963CC6BCC.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A0D4F70C-C285-984F-841C-87622BF9725D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/064EB18E-562A-C34E-8A63-89221C178145.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/611FB056-CB12-7747-8050-81EEB2AD4B47.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/01A5A6B0-C6DD-DE48-86A9-FC74E6441C2F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/AA621C95-F58F-CD4C-BBEA-80C8AA001B1F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A7B12AE5-C17A-584A-B7E0-733C19DB47BC.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/78DA8E11-2E89-6B48-BA45-C7C382B063FD.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9EF9A7E9-058E-AE49-A5C6-8F3B6FF256CF.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FC7E038F-836B-364C-A194-17C063C1FAC0.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/24B3A0B3-4DA5-AB40-82AA-170F1083A372.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/41E54134-C84A-3549-8BBE-E852C33DCDAC.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4238CACF-A259-0243-B640-58D8019F060E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2DAA6F61-FF72-734A-AD2F-B20C925ADCFE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/28A2124F-4F26-5748-909E-391A9BA327AE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/0ABEBE6F-F39F-1742-A19C-FBF92D0935ED.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/47613D63-E277-244C-8727-B071490B09B3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/40A195F7-0DE1-FA40-B5EA-99ADB123B1C3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/D9CDFBF6-0733-E848-AAD3-429C2066B75B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FB9AB32F-B56C-2E41-8C19-57547AD1F93D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/48AD1517-8D37-4F45-969D-B958CF5C9939.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/3BB5286D-0FD2-184B-A21C-5BF67D09660E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/EB13A678-CDB1-8E46-B642-D13C31A9FB86.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/EF014F73-6EA2-5C42-95E0-C72EDADB882A.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9EE6BEE2-D3E0-8740-8260-41B496FC6CD5.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/5C154C0D-521D-4148-B99E-945206C972C9.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/971EFD89-FBD6-B54D-90AB-D7AA5DA72A12.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/512B9254-CBFC-A646-B6E3-9B3066A2FFB6.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/29834234-C32B-1E40-9916-9BCB8544284C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E2EB9E5B-F2CD-0948-AF57-D9073F279B5B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E2DC8F28-1C68-E34D-967A-5CB26706C227.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FC4512D4-F3BE-4C46-8F4F-FD944A55F41B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/CE51C3FD-C5D8-7844-BC19-99002A0D32D2.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C2CCD73A-619D-4145-B01E-2BD2988C1ED1.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F64AF8D3-B865-B840-A696-ECA158CBA344.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4B0C61DD-48AC-BD42-AC4D-B1A456AB5805.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/0BF0A3C4-1DDF-134B-8C7E-5B3DDCD4E0AC.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/1C890724-C0AE-4B4A-B80A-3B668C41CEF3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A60760CA-8E34-ED43-A407-88ACCB51A237.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/BE2FD257-12A6-DF45-AE65-93594CFC0728.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/7D957B75-E56D-9243-87B1-1353AB2917B6.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/31C5B5B8-4E7B-7546-9BF0-26B7612DAD49.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FDB89FE0-29D3-FF4A-B236-9ABBB67B7804.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/94FC28EF-0374-CC48-8D78-6A2147C39F49.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/EEF87550-CD6A-1B4D-A687-3AFA0451FB81.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6119EE50-831A-4B4C-B393-435AAF9D48DD.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/B86CB859-3705-9647-8845-08E50EC376CE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/3713941B-BC3C-AF47-A24A-15DE99B85FDA.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/246D8603-86E4-4D4B-88CD-29449926B7C0.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/422BCDC2-880D-A540-85B8-C52FCB879A89.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/8AF5F3F6-F0BB-7548-B50E-9FC41FA23FA4.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/DDFD3A4B-B27F-9B45-8CAB-DCEFD6764831.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/5BE12AA8-F066-1D43-800E-A6E280BBA73D.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/DCC68DA5-0DBE-1E43-9054-CE0C132703BC.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4DE4BFB8-D625-DE40-A63D-B4724A70665B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A4CFCC7A-3735-E34A-A38C-189B1BC80D7C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/7668B58E-ABA0-4C44-84C5-FF34C9917D04.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/495CB38E-DCAF-4046-831B-DC17BC8891DD.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4718D251-DA54-CC48-A096-C5DEE7E8473C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/59F28633-C161-D44D-B423-6F240B894E85.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/F113FF4C-EAA8-944A-9F34-3A3695EAC003.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/1AEF4126-CC7A-934F-81F6-172E332141FD.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/6DF6C6D2-6EF3-0341-817D-2798D125458C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/97319252-0F81-8541-9B7C-77D9AFCF17BE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FEA844E6-918E-B44A-AC86-C0EF754396F7.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/AF14728E-FA14-B349-BBA5-A99BAD473E9C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/C2E7D200-BE75-114A-8415-E9ABAB2A7F47.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E1306E0C-EA33-BE4F-99D5-E63B215D81F8.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/CED88335-5756-2C40-8429-97D8341BDBAB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2428E5FB-AA26-5D47-860F-8CBD1367B0EE.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E76D911F-1BF2-3940-8173-612A339D4E0C.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/BAFF804E-0CF0-CC48-AF78-263556B35DE0.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/BBBCCA8F-81A0-B047-BFE9-3A5E4C772C33.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/00504A91-2F7A-B448-BBFA-24A5E3ED34C3.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/97B480A6-36C1-BE41-ACE2-0DB1A54252E6.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/50455413-860B-EA43-B305-43CBAEEF1E9B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A061C10F-886A-E54F-9D64-9CEFFB51E18E.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2141757F-A37F-5443-90A3-0BD58637EF33.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/E4F70444-8E26-D54A-8332-3A324FA60613.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/1EB17F9E-C0A7-9441-AADE-B5972C496E18.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9897BE21-47D3-B142-9184-C30A74E012E5.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/D66C4985-74A2-9A4B-BC98-840DEB73926F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9A0D31B1-C296-0345-9D78-F4E010E00613.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/BA5A79DE-0955-BD42-B025-BB108ABDECA5.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/FFA20A47-3DE3-424D-88F4-FBFA4B145400.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/5AFA0DDC-5DD5-B645-A919-4A74F5705FA4.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A981E45F-C98B-0E45-80F1-DBDCD93456AB.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/4B7A40F5-5C06-094D-9DF1-0AF4ED3C4B3F.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/2D7429C7-F90B-7B4D-A9B0-496A899E72FF.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/90A76980-6467-954D-A2D1-E96F8ABC1737.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/9FF1D040-2C7A-C34A-9DC8-FABBD581D073.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/3F3E0031-0DB8-B04C-98CB-00A2CBF90F64.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/A5D4CC47-24DA-474C-8D81-4120ADED8D5B.root",
"/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/325/159/00000/00ECBB9C-C5B6-A74B-8777-716972C948F1.root",
])

process.source = cms.Source("PoolSource",
 fileNames =  myfilelist
#    fileNames = cms.untracked.vstring(                          
#
#
#   )
)

# for dump files 
#process.source = cms.Source("PixelSLinkDataInputSource",
#    fedid = cms.untracked.int32(-1),
#    runNumber = cms.untracked.int32(-1),
#    #fileNames = cms.untracked.vstring('file:../phase1/PixelAlive_1293_1018.dmp')
#    fileNames = cms.untracked.vstring(
#     #'/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_300/GainCalibration_1293_1015.dmp'
#     #'/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_300/GainCalibration_1200_1015.dmp'
#     '/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_300/GainCalibration_1300_300.dmp'
#    )
# )

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:1-325159:47')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:1-325159:34')  # no collisions, no HV
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:35-325159:46') # collisions no HV 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:47-325159:61') # nonstable lumi
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:62-325159:999')

process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2015 data, label = rawDataCollector, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),

# for MC
#    InputLabel = cms.untracked.string('siPixelRawData'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),

# for dump files 
#    InputLabel = cms.untracked.string('source'),
# old
#    InputLabel = cms.untracked.string('siPixelRawData'),
#    InputLabel = cms.untracked.string('source'),
#    InputLabel = cms.untracked.string("ALCARECOTkAlMinBias"), # does not work

    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(0),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


