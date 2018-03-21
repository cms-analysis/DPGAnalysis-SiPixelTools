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
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(1000)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('noise.root')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 


#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/0478B63F-E02C-E811-A298-FA163E010862.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/10EBDF2B-DD2C-E811-BC6D-FA163E4B6104.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/14650547-D82C-E811-A345-FA163E81F19A.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/1EA8D1A2-DB2C-E811-A719-FA163E4B6104.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/26E20010-DF2C-E811-8BAB-FA163E4B6104.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/2CE98CE8-DB2C-E811-81B6-FA163E510DF7.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/2E45AF64-DF2C-E811-97D3-FA163E06876F.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/30FFCD41-DC2C-E811-B9C8-FA163E1D6363.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/34281E65-DC2C-E811-993C-FA163E8ECDD2.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/302/00000/3601548A-D82C-E811-A3F7-FA163E7164EC.root",

"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/00097234-762C-E811-9C01-FA163E376AE7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/002FAD18-AE2C-E811-AE7F-FA163E595F01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/0068E3AB-BF2C-E811-A765-FA163E49E4DD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/00797185-882C-E811-A3AC-FA163ECCEC77.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/008A0155-8B2C-E811-BDAB-02163E0176B0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/028FFAB8-862C-E811-BCFE-FA163E024A59.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/02BCB4EF-772C-E811-9F81-FA163EFD5681.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/041065A5-CF2C-E811-B1ED-FA163E95BBB1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/0422A8DA-B82C-E811-9045-FA163E252608.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/301/00000/0430A381-952C-E811-8467-FA163E468C72.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/001D171B-F02B-E811-AE35-FA163E9F7EA5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/001FC020-D62B-E811-805F-FA163EF28A5C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/003B013D-D52B-E811-86E1-FA163E121244.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/00824F23-F02B-E811-A35A-FA163E3EF566.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/0097FE3B-F02B-E811-B24C-02163E019F73.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/00C364A9-F02B-E811-B966-FA163EB108A8.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/023D38D4-DF2B-E811-87E6-FA163EBDF68C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/0250DBFF-DA2B-E811-BE98-FA163E8CC404.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/0298AF23-F02B-E811-B785-FA163ED7B2EC.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/02B42FD0-FC2B-E811-8DA6-FA163EE7A704.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/220/00000/02BE268B-D72B-E811-83D4-02163E01A000.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/004B757B-3B2B-E811-880A-FA163E5577DD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/0063B2F0-252B-E811-982E-FA163E3CC334.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00AE2312-202B-E811-AE91-FA163E6CDBB2.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00C46971-322B-E811-AEF1-FA163EFD7515.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00D62B56-3D2B-E811-ACAE-FA163EBF1F42.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00DBDBE7-282B-E811-9A94-02163E014C0D.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/00E89BFA-252B-E811-957F-FA163EE347FC.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/0209221B-2A2B-E811-9807-FA163E83AA16.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/02159E91-3A2B-E811-A915-FA163E22408B.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/141/00000/02370D59-282B-E811-8D77-FA163E3A5F0F.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/02018B3C-7229-E811-A0F9-FA163EAE0640.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/026F8CBA-7029-E811-B2A5-FA163EE7A704.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/02E5DB20-7729-E811-8188-FA163E34CDF1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/044D2DBB-7629-E811-B684-FA163E34CDF1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/044FD472-7029-E811-BABD-FA163E9E72CB.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/046682FA-7329-E811-9A27-FA163E9E72CB.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/04D7DA93-8429-E811-8833-FA163E5D2DA7.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/061FF441-7829-E811-9611-FA163E126B49.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/0A22EC82-8829-E811-BFBE-FA163EF4135C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/100/00000/0AA0F41F-9229-E811-A339-FA163EDA9841.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/001FB648-E328-E811-A8FC-FA163E664BFC.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/003C44E8-D928-E811-B9BE-FA163E09D221.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/005EEE07-CB28-E811-BE2E-FA163ECA2FB5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/00BE0970-D328-E811-BFFB-FA163E71F02B.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/00DC315F-A828-E811-9844-FA163E734BE1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/020502DD-CF28-E811-BD98-FA163E1BF0F8.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/023153A7-AB28-E811-851D-FA163E32403C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/02ED1EE7-BB28-E811-9261-02163E0165AA.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001000000/02F99B7D-DD28-E811-845E-FA163EB8D19E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/312/001/00000/0465AFC9-DD28-E811-BD4E-FA163E0A8836.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0066B415-FA27-E811-AEA9-FA163E96C3EF.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0067A8F6-0A28-E811-923C-FA163E47AC10.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/007CAB86-F427-E811-A7DF-FA163E2D5FC7.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/00C4D8C5-D627-E811-B30C-FA163E21BFF5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/023B9490-E127-E811-AA77-FA163E4C8771.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02A4554A-1628-E811-8606-FA163E8676B9.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02D079B3-1B28-E811-95CD-FA163E954ED5.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/04304C4B-5F27-E811-BCA5-FA163E447F8E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/0855D54E-5E27-E811-9622-FA163E2848B2.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/56FC28E0-6027-E811-9D59-FA163E447F8E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/9C57EB28-5F27-E811-A0F3-02163E013C39.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/787/00000/A8469AB1-5E27-E811-B5AE-FA163E98F3FD.root",


#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0026B7C4-8926-E811-8C04-FA163E87F2DD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0862AF9C-7A26-E811-91EB-FA163E333059.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/08AFF0E8-8126-E811-A465-FA163E1414FD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0C672356-8026-E811-80A1-FA163E148FBE.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/668/00000/0CFE4FC2-8226-E811-85FE-FA163E148FBE.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/532/00000/000072CE-AA25-E811-8D88-02163E0147AF.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/532/00000/00311845-6C25-E811-AE1A-FA163EAEF82B.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/532/00000/02089336-A425-E811-8E85-02163E019EBE.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/532/00000/0231CC26-A725-E811-9161-FA163EC08B4C.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/532/00000/0269325F-6725-E811-B12D-FA163E126B49.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/004D47A8-F524-E811-B57A-FA163E19119E.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/00F4A829-F824-E811-8E46-FA163E0839FE.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/026C25BD-C324-E811-8C89-FA163E38DEFD.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/02A7E24F-C024-E811-BF4E-FA163E7B0F38.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/518/00000/02D4F400-0225-E811-8AFD-FA163EAEF82B.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/248286D7-2424-E811-B7EC-FA163E85C1BF.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/2E3FC689-2524-E811-B511-FA163E7947B1.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/343980D7-2224-E811-A524-FA163E355A35.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/3646FED1-2224-E811-9502-FA163E8DF100.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/447/00000/48FC8562-2424-E811-843E-FA163E445507.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/06BF6B46-2524-E811-83DE-FA163E5B4AF5.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/10BA6E45-2324-E811-9ACE-FA163E355A35.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/1C63E23D-2524-E811-BA00-FA163EFDF28A.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/2043D215-2424-E811-8281-FA163E5DC871.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/446/00000/22487555-1C24-E811-96C3-FA163EA09EAB.root",

#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/12CA4EF1-EF22-E811-8D04-FA163E125028.root",
#"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/321/00000/22593422-EF22-E811-88E1-FA163EFAF5BE.root",

#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/301/465/00000/00D3F7BD-8A85-E711-918E-02163E01A37A.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/02DBAD4C-AE7C-E711-B44F-02163E019DD0.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/04985C0E-B17C-E711-8A86-02163E0146AC.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/0A141D71-B17C-E711-991B-02163E019E22.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/0E5A4992-B07C-E711-953F-02163E01A20D.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/0E9C9D97-B07C-E711-851A-02163E01A5EB.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/12A4804B-AE7C-E711-BB91-02163E014505.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/1A38FCB6-B27C-E711-BC22-02163E01A2D8.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/3079B880-B27C-E711-8917-02163E012510.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/30CA51CC-B37C-E711-AE4A-02163E011DD4.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v3/000/300/773/00000/3E248E7C-AF7C-E711-BA93-02163E0134FB.root",

#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/04BBD43B-0876-E711-8C3E-02163E0119F7.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/04CC776E-0376-E711-850D-02163E0141D5.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/0624BE8A-0A76-E711-9D53-02163E011825.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/086C8B4F-0676-E711-8081-02163E01A20D.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/1C743D35-1676-E711-B5A3-02163E013412.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/1EF32E71-0376-E711-B413-02163E011B72.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/206382F8-0676-E711-B222-02163E011D78.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/22A3A271-0376-E711-9918-02163E013734.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/24C3FE4A-0676-E711-8961-02163E01A5FB.root",
#"/store/express/Run2017C/ExpressCosmics/FEVT/Express-v1/000/300/211/00000/288C0969-0676-E711-A54D-02163E01384A.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v2/000/296/702/00000/FED4EA05-D64F-E711-82F9-02163E012A6B.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/426B4782-FC43-E711-9328-02163E011A76.root",
# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/439/00000/F853E939-FF43-E711-A59D-02163E013479.root",

# "/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/209/00000/005E61C9-D341-E711-BEAE-02163E019C9F.root",

# cosmics  2017
#"/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/294/307/00000/00ABDA7D-733B-E711-A08B-02163E01440D.root",

#"/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/291/877/00000/0C01F638-9923-E711-A733-02163E014409.root",
#"/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/292/080/00000/0014512B-9325-E711-8981-02163E01A28A.root",

#"root://eoscms//eos/cms/tier0/store/data/Commissioning2016/MinimumBias/RAW/v1/000/267/594/00000/0AD270D1-B8F0-E511-9A98-02163E01347C.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/217/2EF61B7D-F216-E211-98C3-001D09F28D54.root",

    )
)

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
    Fraction = cms.untracked.double(0.01)
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


