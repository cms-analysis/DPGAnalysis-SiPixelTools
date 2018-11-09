#
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("i",eras.Run2_2017)

# add 
process.load("Configuration.Geometry.GeometryRecoDB_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' # 

process.MessageLogger = cms.Service("MessageLogger",
     debugModules = cms.untracked.vstring('dumper'),
     destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
     cout = cms.untracked.PSet(
#         threshold = cms.untracked.string('DEBUG')
         threshold = cms.untracked.string('WARNING')
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
#    fileNames = cms.untracked.vstring(                          
#    fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/d/dkotlins/public/digis.root'))
#    fileNames = cms.untracked.vstring('file:../scripts/digis.root'))
#
#
#   )
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325159:1-325159:47')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:1-325097:28') # no hv  no collisions 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:29-325097:38') # no hv collisions  
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:39-325097:57') # hv on 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:58-325097:9999') # stable lumi 
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('325097:1-325097:100') # overview


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('fed_errors.root')
)

process.dumper = cms.EDAnalyzer("PixelFedErrorDumper", 
    Verbosity = cms.untracked.bool(False),
    InputLabel = cms.untracked.string('siPixelDigis'),
#    src = cms.InputTag("siPixelDigis"),
)

process.p = cms.Path(process.dumper)


