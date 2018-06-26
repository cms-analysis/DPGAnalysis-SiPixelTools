#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_L1Tech_BSC_minBias_threshold1_v*'],
    HLTPaths = ['HLT_Random_*'],
#    HLTPaths = ['HLT_ZeroBias_*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v*'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
# Alca
#    HLTPaths = ['AlCa_LumiPixels_Random_v*'],
#    HLTPaths = ['AlCa_LumiPixels_ZeroBias_v*'],
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
#    input = cms.untracked.int32(-1)
    input = cms.untracked.int32(100000)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('noise.root')
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/B24C5B53-8876-E811-AB5D-FA163ED7C351.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/A863B659-8876-E811-8393-FA163ECAA680.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/E034DBD8-8A76-E811-BABF-FA163ED7C351.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/02D7EE92-8A76-E811-ABD3-FA163EAE9992.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/C8489DF5-8C76-E811-8054-FA163E006575.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/0401786C-8D76-E811-B034-FA163E2F746A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/805B4CF7-8E76-E811-9D04-FA163E77A136.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/CED393D4-8F76-E811-B42C-FA163E7247B0.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/08ADD0CF-9476-E811-BB30-FA163E249C0F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/D6BCA30E-9676-E811-BAAA-FA163EEB9D73.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/CABD18CD-9576-E811-BE55-FA163EF80BCE.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/D434993B-9976-E811-800E-FA163E1E709D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/ACD2DD58-9976-E811-A036-FA163E5AEA3D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/26BACB12-8676-E811-8C53-FA163E832FBF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/70F97322-8676-E811-A5D5-FA163E8C2B72.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/E06C0822-8776-E811-8D5D-FA163E8C2B72.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/9A990331-8776-E811-BB62-FA163E5D53FA.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/7A6B0E60-8776-E811-8216-02163E019F71.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/6C3924CC-8B76-E811-B96A-FA163EC45139.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/F0DE93DD-8B76-E811-A4C4-FA163E006575.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/AC8A16DA-8B76-E811-AA07-FA163E194DBB.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/0A614DA4-8D76-E811-9E6A-02163E013E3C.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/86228D8D-9076-E811-9D49-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/BC19F891-9476-E811-BCA6-FA163EC53277.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/0A3829CB-9576-E811-AAC3-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/08866B2C-9776-E811-AA7A-FA163EC53277.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/12B47F53-9776-E811-8895-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/B0439A40-9976-E811-8646-FA163EC5BA5A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/CA0EB3ED-9876-E811-AC8F-FA163E937594.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/C8D56160-9976-E811-A0C4-FA163E12A3ED.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/BC207C98-9976-E811-B2FC-FA163E6BA0FF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/24D23AF6-9A76-E811-B873-FA163E35E850.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/20021DF2-9A76-E811-82C3-FA163E5AEA3D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/8A27ECDD-8776-E811-A421-FA163E6894DC.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/AE3532E8-8776-E811-B90E-FA163E2DF0C3.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/74BD8A4B-8876-E811-AB40-FA163E763540.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/A2314EFA-8776-E811-97BA-FA163E8C2B72.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2C0D4B92-8876-E811-9F84-FA163EA393DA.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/CC789C66-8976-E811-BF86-FA163EA393DA.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/82DE773C-8A76-E811-965D-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/587F965B-8A76-E811-8300-FA163E28A50A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/04553BCF-8B76-E811-8458-FA163E827615.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/202151A8-8C76-E811-9BEC-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/4C5D00F3-8E76-E811-B3B2-FA163EC45139.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/C4D71F58-9076-E811-83B1-FA163EAE9992.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/3CE0FE2D-9276-E811-A0C1-FA163EFDDA6B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2CD6F900-9376-E811-BD29-FA163E249C0F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/5E73DC53-9376-E811-8F6E-FA163E7247B0.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/1047435D-9376-E811-83B4-FA163E6F47B2.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/7ED52BFC-9676-E811-A9FB-FA163E937594.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/90D6AB23-8776-E811-8B92-FA163E9AEBCE.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/5CACC6C0-8776-E811-B6BB-FA163E9CAB46.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/B6C4A302-8876-E811-86D4-FA163E9AEBCE.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2E687B00-8876-E811-B680-FA163EA3C7AF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/3C7F67E7-8A76-E811-BF99-FA163E7D17FA.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/A80F5698-8F76-E811-8D5E-FA163EC45139.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/1A93BFBA-9076-E811-AF52-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/86740414-9176-E811-935C-FA163E7247B0.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/54D6498E-9276-E811-8F1D-FA163EAE9992.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/3ACE6E94-9376-E811-8D73-FA163ED7C351.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/F2A506A8-9476-E811-A288-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/48B84CA9-9476-E811-88A5-FA163EC45139.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/46A498D7-9476-E811-A564-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/AE0CBFAE-9476-E811-A82A-FA163E6F47B2.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/DAF09FCA-9676-E811-A037-FA163E1E709D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/9CD36643-9976-E811-A40A-FA163EC0AC06.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/30C64C3B-9976-E811-9F64-FA163EA4F94F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/AC3DC4EE-9876-E811-93B9-FA163EC318C1.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/348794F0-9A76-E811-8702-FA163E88012B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/8A68DA78-8676-E811-BBE6-FA163EEDDE6D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2064F8DA-8876-E811-8949-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/E41F9908-9076-E811-AC02-FA163EA5D5A9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/3C9A87B5-9076-E811-A5EC-FA163EE653E7.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/C6641D01-9276-E811-B1DD-FA163EA5D5A9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/A8B5011C-9276-E811-8E28-FA163EC45139.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/B211DD62-9B76-E811-93DD-FA163E9626F3.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/A2FBE8C2-9B76-E811-AB7E-FA163E6BA0FF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/503BEB89-8A76-E811-8EEC-FA163E006575.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/AC5A3E0B-8B76-E811-A401-FA163ECEFAE1.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/AE6E601E-8B76-E811-B4BD-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/1478DDEF-8B76-E811-8B41-FA163E4200C7.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/3863F89C-8C76-E811-AE90-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/98ED7BF2-8C76-E811-857C-FA163EA1368E.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/280AAA01-8F76-E811-952B-FA163E7247B0.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/BAB9A96A-8F76-E811-87E4-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/ACB99CF7-9076-E811-9CE2-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2AC27B3B-9276-E811-B843-FA163ED7C351.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/38393827-9276-E811-B9EF-FA163E8BC7EE.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/8A099D44-9876-E811-B1CD-FA163E6BA0FF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/562DB23E-9976-E811-92F6-FA163E5EAC11.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2C79B239-9A76-E811-8253-FA163E937594.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/6271636E-9A76-E811-B50A-FA163E6BA0FF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/FEC6154B-8876-E811-99D0-FA163EC7D185.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2CE20E71-8976-E811-96FF-FA163E6894DC.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/C217BD83-8976-E811-81BB-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/C2CA278E-8976-E811-8E1A-FA163E4200C7.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/90BB42BD-8A76-E811-A3B0-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/7E05DED4-8B76-E811-A5C5-FA163EFAF60B.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/A26E59B2-8C76-E811-8905-FA163E2F746A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/7A68454B-9076-E811-B0CE-FA163E53400A.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/388C4B3E-9276-E811-A74D-FA163E249C0F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/2242E7E8-9276-E811-9D42-FA163EA4F94F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/9466E7B9-9476-E811-9BBE-FA163EF80BCE.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/DA02BF11-9776-E811-BD24-FA163EA4F94F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/EC4B2429-9776-E811-AD3D-FA163EE10C3D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/F4D4C339-9776-E811-A08A-FA163EC318C1.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/62993037-8776-E811-BC2A-FA163E64DF51.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/40598535-8776-E811-9F31-FA163E4200C7.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/24DA8FA4-8A76-E811-97D6-FA163E4200C7.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/08A84807-8B76-E811-AE22-FA163EA1368E.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/E007BA36-8F76-E811-B0DB-FA163E1E709D.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/C077AD61-8F76-E811-89C5-FA163EB165D3.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/AAE8640D-9076-E811-B36F-02163E019F13.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/92B67401-9076-E811-99FD-02163E019EEF.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/BE8F6F3A-9376-E811-9958-FA163EA5D5A9.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/7EE5DCBE-9576-E811-AFB0-FA163E31691F.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/3ACDCED7-9676-E811-95B1-FA163EC53277.root",
"/store/express/Run2018B/ExpressCosmics/FEVT/Express-v1/000/318/370/00000/9EBA271F-9676-E811-AB51-FA163E37E0B0.root",
])

process.source = cms.Source("PoolSource",
 fileNames =  myfilelist
)
#    fileNames = cms.untracked.vstring( 
#    )
#)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205718:49-205718:734')

process.d = cms.EDAnalyzer("FindHotPixelFromRaw", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#    InputLabel = cms.untracked.string('source'),
#   In 2012, 2015, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),
#    InputLabel = cms.untracked.string('siPixelRawData'),
    CheckPixelOrder = cms.untracked.bool(False),
#   Fraction to define  noisy pixels 
    Fraction = cms.untracked.double(0.01),
    MAXFED = cms.untracked.int32(1293)  # bpix only 
    #MAXFED = cms.untracked.int32(1338)  # all  
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


