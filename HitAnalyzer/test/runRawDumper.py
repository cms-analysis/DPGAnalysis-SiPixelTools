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
    HLTPaths = ['HLT_ZeroBias*'],
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

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(                          
#    "file:../../../../../CMSSW_7_1_3/src/DPGAnalysis-SiPixelTools/HitAnalyzer/test/raw.root"
#    "file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/raw/raw2.root"
#

# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/0230C488-1543-E511-A00F-02163E01437A.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/08AFCD36-1543-E511-B16D-02163E014683.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/08C79394-1543-E511-B57A-02163E0144BE.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/101D8C89-1543-E511-BEDC-02163E01437A.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/14C27628-1543-E511-A801-02163E01193F.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/1682B635-1543-E511-A3FB-02163E012560.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/18C5A54C-1543-E511-9E85-02163E014511.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/1EDE2D5A-1543-E511-B04B-02163E012560.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/2817BB89-1543-E511-9D8F-02163E01421E.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/28F02E97-1543-E511-A665-02163E014489.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/34FF693C-1543-E511-B86D-02163E014127.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/3E28112B-1543-E511-8384-02163E014318.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/3EF76A6B-1543-E511-931A-02163E0141DB.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/401A517E-1543-E511-85E3-02163E01421E.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/465B1E4C-1543-E511-9605-02163E011836.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/4AB7EFEA-1243-E511-88F8-02163E0146A6.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/505A3C84-1543-E511-8E46-02163E014221.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/5C460951-1543-E511-B605-02163E014411.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/5EFE5A6D-1543-E511-8459-02163E01180D.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/64AEF34B-1543-E511-A875-02163E014683.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/6CE11A5D-1543-E511-88AB-02163E014463.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/74C90B40-1543-E511-8893-02163E01421E.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/761B9288-1543-E511-A939-02163E012560.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/7E340734-1543-E511-867B-02163E0143BD.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/7E8A9356-1543-E511-A81A-02163E014416.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/82AB6861-1543-E511-9926-02163E014511.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/84CC1F64-1543-E511-B9EE-02163E01457C.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/84F71098-1543-E511-B2BF-02163E014416.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/92BC146C-1543-E511-A96C-02163E0137C1.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/96A4A64D-1543-E511-A1E0-02163E014411.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/98B8592A-1543-E511-B021-02163E0143A9.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/98BDC74D-1543-E511-826B-02163E01193F.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/9C60526C-1543-E511-A3FD-02163E014416.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/9E44A529-1543-E511-BA5B-02163E01412D.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/9EB45E55-1543-E511-9AD4-02163E014300.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/A03B062A-1543-E511-8134-02163E0129D9.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/A60A6F69-1543-E511-AD92-02163E015541.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/A696DC47-1543-E511-8019-02163E01431C.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/A82EB75A-1543-E511-B0BA-02163E014330.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/A8354C5A-1543-E511-A9C0-02163E0146A6.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/AC647C56-1543-E511-8FCA-02163E0146DD.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/B0F612AB-1643-E511-AE7B-02163E01465E.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/B299DF76-1543-E511-8FF7-02163E01181F.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/B4B9E9F7-1243-E511-9F42-02163E01181F.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/BAD59F36-1543-E511-89CD-02163E01430A.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/BCDCA42D-1543-E511-8FC0-02163E014330.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/C04E1425-1543-E511-8C5E-02163E01181F.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/C6056760-1543-E511-8346-02163E0143BD.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/CACCD55D-1543-E511-A1B9-02163E01180D.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/D0B3546A-1543-E511-BE8A-02163E014511.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/D28D7C8E-1543-E511-B6DD-02163E0123D2.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/D8863D97-1543-E511-B72C-02163E0129D9.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/DA1B613C-1543-E511-9AED-02163E014127.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/E2D96963-1543-E511-BDC9-02163E01418F.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/E40A7552-1543-E511-8655-02163E0141DB.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/F214D854-1543-E511-9741-02163E0146E2.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/F8EC0972-1543-E511-AA2D-02163E014138.root",
# "/store/data/Run2015C/ZeroBias/RAW/v1/000/254/319/00000/FCD79B91-1543-E511-8870-02163E0141DB.root",

"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/FA244051-8141-E511-B22B-02163E014153.root",

#"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/F4E6E4DE-8841-E511-922B-02163E014125.root",
#"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/F41B36B8-8841-E511-B34E-02163E01437D.root",
#"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/F0C1F208-9341-E511-B9B1-02163E0145C7.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/EEF00C97-8141-E511-9148-02163E0142D9.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/ECF8EBC7-8E41-E511-A682-02163E01423D.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/E899C7E4-8A41-E511-B621-02163E01476B.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/E6FF9DF7-A141-E511-840C-02163E0141F0.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/DEB4F9F8-A141-E511-82BF-02163E0155F9.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/D6EF3550-9541-E511-9BC4-02163E0142D9.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/0CD01E7F-9941-E511-B9D3-02163E0144AB.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/0E530F10-9341-E511-8009-02163E0142F7.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/107D4650-9541-E511-8743-02163E0144AD.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/10E500B5-8B41-E511-AB45-02163E014130.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/161E5FF7-A141-E511-8D07-02163E01458E.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/204DFED0-8B41-E511-AA23-02163E014389.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/20D01050-9541-E511-B114-02163E014730.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/245ACD07-9341-E511-A83B-02163E01476B.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/2A1AC4EE-8A41-E511-A0A1-02163E014595.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/2CD2C951-9541-E511-80C6-02163E01458E.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/2E7A4DF4-A141-E511-951A-02163E014125.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/3A1AA34F-9541-E511-AB74-02163E0145DF.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/40A3D16C-9241-E511-BEBE-02163E014595.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/40F88F53-7F41-E511-A2FA-02163E0144DB.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/42B3394F-8141-E511-A935-02163E01451A.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/48712308-9341-E511-A778-02163E014525.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/4AD0414F-8141-E511-8A60-02163E01437D.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/4C815F51-8141-E511-B856-02163E014153.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/50610BC3-8E41-E511-B336-02163E0142F7.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/74216909-A241-E511-878B-02163E015670.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/788BB650-9541-E511-A4E5-02163E01412A.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/8073A406-9341-E511-BDF5-02163E014390.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/8090D06D-9241-E511-A314-02163E0155D6.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/9018FDF8-A141-E511-ADEA-02163E01476B.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/90A11A5A-8141-E511-AD3E-02163E0144DB.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/94066E2C-A241-E511-B5AF-02163E015670.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/96FAB3EE-8A41-E511-98BC-02163E014595.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/A015E4EE-8A41-E511-A930-02163E0142F7.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/A27832D1-8B41-E511-B8FD-02163E014389.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/AA2EAB6B-9241-E511-BD06-02163E014603.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/C4A130F9-A141-E511-BB78-02163E01422D.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/CC859EC6-8E41-E511-9EE9-02163E01422D.root",
# "root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/D2C0D27D-9941-E511-B592-02163E01422D.root",


#"root://eoscms//eos/cms/store/data/Run2015C/HLTPhysics/RAW/v1/000/254/227/00000/FE743A63-9941-E511-80CC-02163E01437A.root",
#"root://eoscms//eos/cms/store/data/Run2015C/Charmonium/RAW/v1/000/254/227/00000/B6FC5492-8841-E511-9CF7-02163E01444F.root",
#"root://eoscms//eos/cms/store/data/Run2015C/Charmonium/RAW/v1/000/254/227/00000/88DDB326-8841-E511-A4C2-02163E014595.root",


#"/store/data/Run2015B/Cosmics/RAW/v1/000/253/106/00000/98FDF449-DA37-E511-A847-02163E0124BA.root",

#"/store/data/Run2015B/MinimumBias/RAW/v1/000/253/215/00000/04B30F2F-A839-E511-B855-02163E013717.root",
#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/411/00000/2227CD91-A61F-E511-8ABC-02163E011D1C.root",
#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/411/00000/D2A60B28-A71F-E511-9950-02163E012AEE.root",

#"/store/data/Run2015A/MinimumBias/RAW/v1/000/250/381/00000/0EE60482-6A1F-E511-BF44-02163E013515.root",

#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/248/025/00000/24014201-5C13-E511-AEFB-02163E0139DC.root",
#"/store/data/Run2015A/ZeroBias/RAW/v1/000/248/025/00000/3C78C42B-5C13-E511-9ED3-02163E012385.root",

#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/00C00730-A00D-E511-8273-02163E014565.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/125871B2-6E0D-E511-9212-02163E01365F.root",
#"/store/data/Run2015A/ZeroBias1/RAW/v1/000/247/324/00000/1A482360-530D-E511-B661-02163E0145CC.root",

#  "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/026/00000/029FB859-68B2-E411-8145-02163E0122AB.root",


#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/027EE06B-D8CE-E411-8E83-02163E012A80.root",
# fill 2576
##    "rfio:/castor/cern.ch/cms/store/data/Run2012A/LP_ZeroBias/RAW/v1/000/193/092/16131715-E893-E111-8CDB-001D09F27003.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/718/02285033-FD1B-E211-8F74-001D09F295FB. 

    )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('191271:55-191271:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('191718:30-191718:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('194912:52-194912:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('194912:52-194912:330 ')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195099:61-195099:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195109:85-195109:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195841:73-195841:100','195841:116-195841:143')
# 195774 OK from LS=0
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('198609:47-198609:112')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('201657:77-201657:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205718:49-205718:734')

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('254227:20-254227:9999')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('254227:21-254227:21')

process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2015 data, label = rawDataCollector, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
# for MC
#    InputLabel = cms.untracked.string('siPixelRawData'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),
# old
#    InputLabel = cms.untracked.string('siPixelRawData'),
#    InputLabel = cms.untracked.string('source'),
    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(0),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


