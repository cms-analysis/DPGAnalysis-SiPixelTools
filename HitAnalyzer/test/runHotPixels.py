#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_L1Tech_BSC_minBias_threshold1_v*'],
    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_ZeroBias_v*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v*'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
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
    input = cms.untracked.int32(1000000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(                         

# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/0693E2CE-39A9-E411-88C1-02163E012827.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/06ADE7A1-38A9-E411-820A-02163E0118B2.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/06F032CC-39A9-E411-AEE7-02163E00FBF9.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/0824B5E3-37A9-E411-97DA-02163E01221E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/0A1A9BC7-39A9-E411-872F-02163E010547.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/0C4218E6-37A9-E411-9CEC-02163E0102C3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/0C536EA6-38A9-E411-B8A3-02163E010547.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/0E5F4B19-3AA9-E411-8EB5-02163E011D7B.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/0E7425F0-37A9-E411-85B8-02163E011DE0.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/10320AA0-38A9-E411-A09D-02163E011D9C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/1065359C-38A9-E411-BFEF-02163E01037D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/10DD6218-3AA9-E411-AC20-02163E011D48.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/1209039C-38A9-E411-B854-02163E010547.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/162429E9-37A9-E411-B78C-02163E01262E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/169BEEA1-38A9-E411-90FA-02163E0118B2.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/1CB664C8-39A9-E411-8FF9-02163E01250B.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/1CCDDE82-34A9-E411-B374-02163E0128F5.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/1E9844CD-39A9-E411-ACBD-02163E0102C3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/1EC07618-3AA9-E411-9C4E-02163E0121A6.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/2274C918-39A9-E411-89D3-02163E01232F.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/22A7E79D-3AA9-E411-8363-02163E012936.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/2445009C-38A9-E411-A84D-02163E010480.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/24B965A4-38A9-E411-864E-02163E0121CA.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/2A7895A0-38A9-E411-B799-02163E011D9C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/2EA3659C-38A9-E411-AD0E-02163E0118B1.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/2EA70B1A-39A9-E411-9E1A-02163E012103.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/3010769D-3AA9-E411-9A02-02163E0102DB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/30C3F5E5-37A9-E411-9BE2-02163E0105D1.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/3245E628-3AA9-E411-A6E3-02163E012B27.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/32777A99-3AA9-E411-8A37-02163E01232F.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/38F2E7A0-3AA9-E411-B36A-02163E011D0B.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/3C37E4CA-39A9-E411-A083-02163E012339.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/400FBF9C-38A9-E411-AE15-02163E012893.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/40BD8D9D-38A9-E411-B1AF-02163E0121FB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/40FA2982-34A9-E411-9C86-02163E0103B8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/4237059E-3AA9-E411-ACA2-02163E0127F6.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/42EE109A-3AA9-E411-849B-02163E012339.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/44A8AE86-3AA9-E411-B217-02163E0105DB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/4A24B7A1-3AA9-E411-AFC1-02163E012014.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/4C4FB7CF-39A9-E411-A415-02163E01219C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/4EBA48EC-37A9-E411-AC4C-02163E011CF3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/5010FB1B-3AA9-E411-96F7-02163E0123E5.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/503230EA-37A9-E411-9275-02163E012B31.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/52F455A0-38A9-E411-B380-02163E012295.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/568AC7E5-37A9-E411-BA44-02163E012611.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/582076AD-38A9-E411-AF61-02163E012565.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/5C0226FF-37A9-E411-AA34-02163E0129A8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/5CFD5C89-3AA9-E411-9C7C-02163E012B31.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/5EAAA0C4-39A9-E411-A6AF-02163E012555.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/64832CE8-37A9-E411-9C0B-02163E011D1D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/68A2F59B-38A9-E411-844E-02163E01033A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/74205317-3AA9-E411-98DD-02163E01040D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/76731A9D-38A9-E411-ABE8-02163E011CF3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/7676F0C7-39A9-E411-B54D-02163E01054C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/7CD46FD5-39A9-E411-8F89-02163E011834.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/821F97A4-38A9-E411-AA9F-02163E011CEB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/8230F39B-38A9-E411-BF54-02163E010494.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/82DE5DCD-39A9-E411-83F1-02163E011815.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/84851990-3AA9-E411-8A5E-02163E012613.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/84DBAFA0-38A9-E411-BE71-02163E00FE24.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/84F1A8C7-39A9-E411-B1E4-02163E01232F.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/8616F9C8-39A9-E411-9189-02163E012B31.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/864558A0-38A9-E411-8CA9-02163E012295.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/88EDDDE9-37A9-E411-8C71-02163E011843.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/8A363AE8-37A9-E411-A5C9-02163E011D1D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/8AA568CB-39A9-E411-BCC0-02163E0102BC.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/8E2523C9-39A9-E411-BC2E-02163E012966.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/8E426CA5-38A9-E411-AC77-02163E012390.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/905534C7-39A9-E411-9802-02163E012103.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/9057EA82-34A9-E411-BC16-02163E01219C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/92B1829C-3AA9-E411-AA77-02163E01262E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/9415C587-33A9-E411-9A81-02163E012B27.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/946AC896-3AA9-E411-AB89-02163E01033A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/94C7FE9B-38A9-E411-A713-02163E0122B8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/968382CC-39A9-E411-B6DE-02163E0120E1.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/9A688F8F-3AA9-E411-9317-02163E0105E1.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/9C66C4A2-38A9-E411-B508-02163E011CEB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/9CF28A9B-38A9-E411-B630-02163E0126BF.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/9E135DAE-3AA9-E411-B435-02163E0125FF.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/A2E7FC9A-3AA9-E411-85CE-02163E01262E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/A4BFACC6-39A9-E411-8528-02163E011D1E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/A84079AD-38A9-E411-9A04-02163E012565.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/AA65AC9C-3AA9-E411-997F-02163E012626.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/ACA0FECC-39A9-E411-A715-02163E010425.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/AEA4F99D-3AA9-E411-A614-02163E0124DC.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/B0A3C0C7-39A9-E411-8B24-02163E012103.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/B0CB43A8-3AA9-E411-B2A5-02163E01250C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/B4D0E99E-3AA9-E411-8363-02163E011806.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/B4D9D19B-38A9-E411-8549-02163E01037D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/B8070729-3AA9-E411-A628-02163E012B27.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/B825089D-38A9-E411-BD3A-02163E012893.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/BE6B1CA4-3AA9-E411-A387-02163E012225.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/BEFACAC6-39A9-E411-9BCE-02163E01232F.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/C05A5AC6-39A9-E411-A0EA-02163E01054C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/C241CBE6-37A9-E411-8FFD-02163E0124DC.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/C400779C-38A9-E411-BCEF-02163E01037D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/C40671C8-39A9-E411-B5DF-02163E01250B.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/C4752D92-3AA9-E411-8D5C-02163E01276C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/C4CB64DD-39A9-E411-A0C4-02163E0121E8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/C8E72BA2-38A9-E411-A92E-02163E0118B2.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/CA6ADE18-3AA9-E411-9F05-02163E01037D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/D2B6C7CC-39A9-E411-9749-02163E0105C8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/DA5138E3-37A9-E411-8DEE-02163E0120E1.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/DA59E1C6-39A9-E411-926D-02163E012626.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/DE5E6BCA-39A9-E411-B646-02163E012260.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/DEBC6FF4-39A9-E411-A422-02163E01228F.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/E084D3E6-37A9-E411-B1E2-02163E012611.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/E22C6B20-3AA9-E411-B540-02163E012893.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/E4C6C99D-3AA9-E411-8839-02163E01200B.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/E64595FE-37A9-E411-B8AF-02163E0129A8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/E8809AA0-3AA9-E411-938E-02163E00FD19.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/EAACB417-3AA9-E411-B944-02163E00FB4D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/EC5F0599-3AA9-E411-8192-02163E012827.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/F093F39A-3AA9-E411-9B60-02163E01219C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/F21CE6DE-37A9-E411-9B5E-02163E011CF3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/F44D31A1-38A9-E411-9DBD-02163E01203A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/F6EA1E18-3AA9-E411-AD74-02163E012295.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/F8752DE8-37A9-E411-A69A-02163E01232F.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/F8BC6D82-34A9-E411-9B24-02163E0122FA.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FC177D82-34A9-E411-8A1B-02163E0102BC.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FC9119CB-39A9-E411-814B-02163E012966.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FE2CD3C9-39A9-E411-96D3-02163E0105D1.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FE39BF98-3AA9-E411-9AED-02163E0122B8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FE6613A4-3AA9-E411-98BE-02163E01189A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FEA5C616-3AA9-E411-8348-02163E0122A3.root",

#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/4829BD7C-57B2-E411-BCC4-02163E012700.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/6C81712E-57B2-E411-A619-02163E012B6C.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/CCF57A73-57B2-E411-87FE-02163E012BC5.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/EA1A5074-57B2-E411-8418-02163E0124AC.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/ECCA0A2B-57B2-E411-8338-02163E0125D7.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/FADADC33-54B2-E411-8539-02163E0125D7.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/FE8E1D08-55B2-E411-AA42-02163E01181F.root",
 
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/235/849/00000/6401A801-98BD-E411-9A14-02163E012445.root",

"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/235/849/00000/26D73DA7-97BD-E411-964B-02163E0122AB.root",
"/store/data/Commissioning2015/MinimumBias/RAW/v1/000/235/849/00000/2CE7DF02-98BD-E411-BD4B-02163E0121D5.root",

# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/00C6027F-1AB7-E411-8725-02163E012515.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/0258D1D1-0FB7-E411-9E9C-02163E011DEA.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/12090AAD-58B7-E411-A6DB-02163E012A56.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/1889B049-21B7-E411-B584-02163E0126E7.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/18A5E189-61B7-E411-B76A-02163E011DE0.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/1ABCC95C-1AB7-E411-AF22-02163E011890.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/1E04C069-61B7-E411-848C-02163E01257D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/26C5C6B7-20B7-E411-A376-02163E011DF5.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/28140574-61B7-E411-BBCD-02163E011D10.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/2C190E58-61B7-E411-9159-02163E012BDD.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/2EAB8B5A-61B7-E411-BF8B-02163E0129F4.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/30F8547F-1AB7-E411-AFA9-02163E012183.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/3266F7D1-0FB7-E411-AF5B-02163E012859.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/345DB461-1AB7-E411-8704-02163E012340.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/36267AB9-1DB7-E411-8825-02163E01230A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/36BAE2AF-61B7-E411-A3C9-02163E0128AB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/38440589-61B7-E411-A67B-02163E011885.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/3A66B400-58B7-E411-8A2F-02163E012AD8.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/3AA12937-12B7-E411-8EDE-02163E011885.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/3C17E7B6-61B7-E411-BE1E-02163E011DDD.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/3CDC169F-1BB7-E411-9C48-02163E01275A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/4055F359-61B7-E411-BC14-02163E012700.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/42B15D69-61B7-E411-BF4D-02163E011CEE.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/4A3BCD58-61B7-E411-82A2-02163E0123AE.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/4ACCF670-61B7-E411-9955-02163E011D72.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/4C05AE6F-61B7-E411-BFE0-02163E012593.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/4C48D258-61B7-E411-8A8E-02163E0122AB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/52DA2B58-61B7-E411-987D-02163E01243C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/5416DC5A-61B7-E411-AF5E-02163E011D1C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/54E1EB4E-60B7-E411-A5E0-02163E012337.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/58315661-26B7-E411-AACB-02163E011DDD.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/583A9B95-62B7-E411-B57E-02163E011E00.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/5A3DDFCA-61B7-E411-AA56-02163E012B7E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/6C9BAAD7-0FB7-E411-8442-02163E01230A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/74293A5B-61B7-E411-A9BA-02163E012947.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/78A08E9B-1AB7-E411-BBF0-02163E01230A.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/7C50F769-61B7-E411-9B9C-02163E012057.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/848EA188-61B7-E411-BA75-02163E0122AB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/8ABC9D58-61B7-E411-8F3E-02163E012B05.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/8C139633-62B7-E411-8974-02163E011DD0.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/8E025F38-12B7-E411-8314-02163E012720.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/90E6976A-61B7-E411-BF0A-02163E012042.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/9627025F-61B7-E411-9BE3-02163E0118BA.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/A459F2D0-0FB7-E411-86B8-02163E0118AB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/A627AB74-1AB7-E411-9090-02163E011890.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/AA52A738-12B7-E411-B4E5-02163E01283D.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/AACCE689-61B7-E411-A7F8-02163E011DE0.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/AEB7C130-23B7-E411-AA12-02163E012362.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/B0F535A2-61B7-E411-8A56-02163E012BDD.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/B6FB765A-61B7-E411-BF1E-02163E012B8E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/C2970EC0-61B7-E411-BBEF-02163E011D9E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/C4FCF760-61B7-E411-B223-02163E012A05.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/C4FD7A4A-21B7-E411-8C26-02163E011DD2.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/C6E15B57-61B7-E411-AB0A-02163E012A56.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/D4C7FF5A-61B7-E411-B120-02163E011D1C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/D6A41F3C-1DB7-E411-BBDD-02163E011DB9.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/D88796F6-24B7-E411-9921-02163E0120DE.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/D8E9D158-61B7-E411-9A92-02163E01243C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/E80D5A32-62B7-E411-A8A0-02163E011DD0.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/F66D5E61-61B7-E411-8833-02163E012A44.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/650/00000/FAE5318A-61B7-E411-AFEF-02163E0120DE.root",


#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",

    )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205718:49-205718:734')


#process.out = cms.OutputModule("PoolOutputModule",
#    fileName =  cms.untracked.string('file:histos.root')
#)

#process.dumper = cms.EDAnalyzer("findHotPixels", 
process.dumper = cms.EDAnalyzer("FindHotPixelFromRaw", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#    InputLabel = cms.untracked.string('source'),
#   In 2012, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
#    InputLabel = cms.untracked.string('siPixelRawData'),
    CheckPixelOrder = cms.untracked.bool(False),
#   Fraction to define  noisy pixels 
    Fraction = cms.untracked.double(0.01)
)

#process.p = cms.Path(process.hltfilter*process.dumper)
process.p = cms.Path(process.dumper)

# process.ep = cms.EndPath(process.out)


