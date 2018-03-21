import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("MyRawToTracks",eras.Run2_2017)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Services_cff")

process.load('Configuration.EventContent.EventContent_cff')

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# needed for pixel RecHits (TkPixelCPERecord)
#process.load("Configuration.StandardSequences.Reconstruction_cff")

# clusterizer 
#process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v9' # for 251643
# 2017
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v2' # fpr 90W 
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # from CMSSW927
# 2018
process.GlobalTag.globaltag = '100X_dataRun2_Express_v2' # 

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],  # not in ZB stream
#    HLTPaths = ['DST_Physics_v*'], useless 
#    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_ZeroBias*'],
    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))


myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0066B415-FA27-E811-AEA9-FA163E96C3EF.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0067A8F6-0A28-E811-923C-FA163E47AC10.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/007CAB86-F427-E811-A7DF-FA163E2D5FC7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/00C4D8C5-D627-E811-B30C-FA163E21BFF5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/023B9490-E127-E811-AA77-FA163E4C8771.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02A4554A-1628-E811-8606-FA163E8676B9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02D079B3-1B28-E811-95CD-FA163E954ED5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/02E03A82-0328-E811-88A9-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0419114D-0928-E811-8491-FA163E8F5243.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/04AB8003-1F28-E811-8F50-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/04FB3A78-F027-E811-8956-02163E017706.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/060A0B31-E327-E811-B57B-FA163E34CB74.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0610FAF1-D727-E811-9C22-FA163E93EDBE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0616F9D0-D027-E811-B9DD-FA163EF225E6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0618395D-D027-E811-B3F1-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/061B7ADC-1628-E811-BB50-FA163EC32BAB.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/065DE606-0728-E811-9FD1-FA163EFD7E8B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/06880E9E-0D28-E811-8C09-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0694C3CE-EE27-E811-8596-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/069D4CDF-FB27-E811-A321-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/06C326C3-D127-E811-A67F-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/06EF8ECE-F527-E811-99B7-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/08005ACB-E327-E811-9F27-FA163E9AD52A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/08ABCF50-E827-E811-AD0F-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/08BF3BA6-0D28-E811-905A-FA163E361D48.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0A042DFA-FE27-E811-917F-FA163EE07180.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0A3DE4A5-0D28-E811-B24E-FA163EF604E2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0A7BDE6C-D227-E811-A81C-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0AD70B90-E927-E811-9344-FA163E0D5E50.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0AEF6D8C-F027-E811-8898-FA163E03756B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0AF44D9E-EA27-E811-96FE-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0C5B06E2-DF27-E811-A03F-FA163E310D96.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0C6AC864-FA27-E811-B385-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0C6FCF89-0A28-E811-A359-FA163E3BBE9B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0C7B5DDF-F227-E811-AFB2-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0E72B869-D827-E811-8D2D-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0E849263-E827-E811-B42D-FA163E0D5E50.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0E8EFADC-E327-E811-9D11-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/0EB51FAF-1F28-E811-A698-FA163E658ACD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1003913A-D327-E811-82A4-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1028BE17-0328-E811-84CE-02163E019F19.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/10D8857C-E527-E811-B950-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1279EC77-1228-E811-B2D7-FA163E850905.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/127BCB31-D727-E811-9D52-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/128012CC-1328-E811-92E3-FA163EF604E2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1419A5E9-0428-E811-BFA5-FA163E47AC10.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/141F5D2B-1E28-E811-B35F-FA163E3FD732.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1432D48B-0B28-E811-B29E-FA163E8F5243.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1433AD0C-0628-E811-8827-FA163EFD7E8B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/147397D3-0028-E811-9BE1-FA163ED9B0D9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/149EDFE3-D227-E811-B172-FA163E32403C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/14A7CA1A-D527-E811-943D-FA163E46FB0D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/14ADA4D0-D027-E811-9E81-FA163ECC3DC1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/14C659E7-0428-E811-905F-FA163E53153A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/14C9F699-DF27-E811-9E6F-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/14D8D57E-D027-E811-B1FB-FA163EA437BA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/184C9892-0E28-E811-853C-FA163EE07180.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/18571D65-0128-E811-839F-FA163E523D0D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/18607033-FA27-E811-B4B0-02163E00B845.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/18CFCE53-DA27-E811-B9A0-FA163E93EDBE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/18EDB4C7-EE27-E811-8EAB-FA163E8E65F7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1A29AF80-D027-E811-8D7C-FA163E174C15.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1A3FE549-0B28-E811-8937-FA163EAFBB72.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1A43A01C-DE27-E811-970E-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1AB72217-F227-E811-BFB5-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1AD1F9D3-1128-E811-84DA-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1C19C6DF-1228-E811-B35D-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1C4F948A-E127-E811-A94E-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1C5F1B1E-E427-E811-BE55-FA163E65100B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1E4560D3-1528-E811-BEF6-FA163EF604E2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1E4CB2FB-F827-E811-B607-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1E4CBB67-E827-E811-B9E0-FA163EE76E1A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/1EE67768-0428-E811-915D-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2019AB17-D927-E811-82BB-FA163ECCC7CF.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2080D8A6-D327-E811-9325-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/20E8471D-D127-E811-822E-FA163EA437BA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2208C83C-F127-E811-873A-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/244C647B-0728-E811-8F5C-02163E00C568.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/24570837-FC27-E811-BDF8-FA163E48BD1C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/26428435-1C28-E811-BAEA-FA163E06735E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/264625D9-E227-E811-9DE1-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2685437F-0228-E811-9920-02163E01654C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/26B05881-F227-E811-8678-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/26BEA0FD-ED27-E811-BB08-FA163E9AD52A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/26C39990-1128-E811-A60C-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/26F5F0DA-0128-E811-A9FB-FA163E9E72CB.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2801670E-0428-E811-8210-02163E019F19.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/28178F01-FB27-E811-921A-02163E00B920.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/28484C6A-D627-E811-AA70-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2848CC4A-0228-E811-B002-FA163ECFACBA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2851BB80-E827-E811-970D-FA163E67C610.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/28ECB770-D427-E811-ADED-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2A15544D-1C28-E811-AD88-02163E00CE9C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2A27719E-F627-E811-BFA3-FA163E4C8771.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2A380EC9-E127-E811-B5BD-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2AEE7817-FA27-E811-B801-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2C079062-E627-E811-8ED6-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2C1EDD39-1428-E811-96B3-FA163E954ED5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2C29DEAE-1528-E811-A105-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2C5ADF1A-0928-E811-B74C-FA163E05ACAC.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2CD1FFE6-EB27-E811-B393-FA163ECC4A4E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2EA22A54-1728-E811-92A3-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2EA4A2E6-1928-E811-AF4E-FA163E7C6CDD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2ED11E90-1728-E811-9321-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/2EE1B2CB-1E28-E811-9762-FA163EC3545B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/302CA1E8-0728-E811-BB8E-FA163E517D33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/305ABED6-F627-E811-BB61-FA163E6009D1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/306AD51B-D127-E811-855A-FA163E0FEE8D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/306FC600-D327-E811-B484-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/30834618-F527-E811-A810-FA163E400416.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/32124184-1B28-E811-817A-02163E00F7A3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/321F03CC-0E28-E811-A868-FA163E3BBE9B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/34A34B9A-FE27-E811-8C4D-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/34BDD3ED-0328-E811-A5FF-FA163E53153A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/36BD71A0-ED27-E811-9DE9-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3801394F-FF27-E811-BF48-FA163E7245A6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/38171CC3-FB27-E811-832C-FA163ECFACBA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/38201F0B-F327-E811-AF6D-FA163E2A2CE2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3864700E-1428-E811-91F9-02163E00B4B0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/38D81A11-DF27-E811-A542-FA163E7DF37F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3A1B6183-D727-E811-B845-FA163E247CD3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3A21344E-F727-E811-BB15-FA163E03756B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3A2F8EE3-0328-E811-ADF5-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3A42E1C9-0728-E811-B967-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3A6693EE-DF27-E811-BF80-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3AFCFE8C-1728-E811-8ABC-FA163EDE9AD6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3C12B151-CF27-E811-B941-FA163E21BF4B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3C93907C-1628-E811-9839-FA163E5189D4.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3E1D18EB-0C28-E811-B03B-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3E27A3CC-FA27-E811-A69F-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3E57DA5E-1028-E811-8123-FA163E7052A5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3E8931AD-FC27-E811-B1A3-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3E9B6D74-1E28-E811-AD0E-FA163E5702B7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/3EEB6E43-E027-E811-8CD6-FA163E331F71.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/409092EA-0428-E811-AD1B-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/40A10DCC-E227-E811-9865-FA163E0E13DA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/42274AE9-1928-E811-AD6E-02163E015D35.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/424731D9-1328-E811-A309-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/427C68C4-DA27-E811-9BDB-02163E017C33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/42AA0062-F427-E811-88C3-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/44A19ADF-1F28-E811-901E-FA163E5702B7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/44A869BE-F727-E811-ABD0-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/44B5AB95-1228-E811-B10C-FA163E8F5243.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/44B7016B-1C28-E811-82A2-02163E00B997.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/44F42494-DE27-E811-805C-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/462166DF-0428-E811-A136-FA163EFD7E8B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/465D9B5E-0A28-E811-B24D-FA163E04A679.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/467537BE-1928-E811-AEA5-FA163EA0727F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/468D540B-D927-E811-9F8B-FA163E62D305.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4837E858-F627-E811-BAFC-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/486F1599-1928-E811-B8CC-FA163E14314A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/48AADE1D-FA27-E811-A420-FA163E623DCC.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/48B27616-0728-E811-AADD-FA163E3BBE9B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4A3FBE2A-1728-E811-8257-FA163E2FBE64.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4A7A6519-FB27-E811-98FB-FA163ECFACBA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4C249128-1E28-E811-94AF-FA163E7C6CDD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4C30461E-D127-E811-B252-FA163E024A59.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4C4AF75C-0D28-E811-AFB7-FA163E9CA336.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4CB3646E-0728-E811-92C7-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4E64F9C0-F427-E811-AF36-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4E6E6750-E827-E811-8298-FA163E0816C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4E6FB1DF-F427-E811-AED0-FA163E31557C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4E80424A-CF27-E811-B788-FA163EF0AD4F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4E9F0891-EE27-E811-BB96-FA163EA1233B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4ED534AE-F627-E811-A308-FA163EEB2BCA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/4EE63E89-D327-E811-AD8B-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5008526D-D127-E811-83FB-FA163E056376.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/503284F6-0428-E811-8985-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5061CF80-1E28-E811-8C06-02163E015D35.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/50848A01-E327-E811-9EF4-FA163E65100B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5091BBB3-1A28-E811-A222-FA163E4A0FB4.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/50D1A8E7-1828-E811-A1D6-FA163E53153A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/523D9EB7-1228-E811-8316-FA163E3BBE9B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/529219B6-0228-E811-91F8-02163E01538E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/52A0C291-1F28-E811-A3CB-FA163E850905.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/52AE64E6-1A28-E811-9170-FA163E523D0D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/54070BD2-F627-E811-BBDC-02163E017706.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/54443EAB-1B28-E811-9527-FA163E850905.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5487CB08-0228-E811-AD62-FA163E008DAE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/548C5835-E927-E811-86C8-FA163E523D0D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/54D1536C-E027-E811-81F8-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/54D71120-0928-E811-8C69-FA163E53153A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/54FC4BEE-DB27-E811-B628-FA163E24EC20.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/56A99A90-1D28-E811-8DA3-FA163E2FE61A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5816DFEC-1228-E811-A911-02163E00BAB6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5887ACC2-DB27-E811-8C7F-02163E017C33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/58D87253-E327-E811-B524-FA163EF890F0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5A464EB7-D427-E811-82F5-FA163EA437BA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5A572F5A-F427-E811-ADA6-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5A691D2F-F227-E811-9F3A-FA163E3E3877.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5A714794-E227-E811-82E5-02163E00B845.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5AC6BF9C-1B28-E811-9398-FA163EDE9AD6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5C073360-0628-E811-AE90-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5C7601C1-1228-E811-AEC9-FA163E472D18.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5CAE92E1-F627-E811-B494-FA163E3BBE9B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5CF67272-1F28-E811-98B4-FA163E2A7247.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5E018067-F727-E811-9AC3-FA163E5B9932.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5E065832-E627-E811-91C9-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5E78C1E0-F227-E811-B2D5-FA163E4E0CCE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5E8FAE9C-1928-E811-B8F8-FA163EC2044A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5EEAD574-E227-E811-A8F8-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/5EFAFEFB-FE27-E811-BA91-FA163EAFBB72.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/602C5A0D-E627-E811-BE32-FA163E1BF0F8.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/603BBB0E-0128-E811-9C87-02163E00B845.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6054289F-1728-E811-B679-02163E01654C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6059E001-EE27-E811-AFED-FA163E4E0CCE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/605FF627-0128-E811-9CDE-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/60720E6E-EF27-E811-B6BF-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/60DC4D92-1728-E811-829E-FA163EF604E2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/60E3F8A3-1728-E811-A937-FA163E279739.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/60FB9B7F-E527-E811-B3D8-FA163E70FEE2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6210656B-E227-E811-B5B9-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/62416FC2-0928-E811-97C9-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/62D61F95-F127-E811-BDA5-FA163E9AD52A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/62EFFDE0-1228-E811-93AC-FA163E954ED5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/64337D8F-F427-E811-AC2D-02163E00C568.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6446D1A3-FB27-E811-8341-FA163E623DCC.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/645C288F-DF27-E811-AA3B-FA163E331F71.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/64858519-D927-E811-B11D-02163E017C33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/64C5EF5A-1028-E811-B973-FA163EF604E2.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/64EE877E-E527-E811-BE20-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/66C0257D-F227-E811-A552-FA163E31557C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/66C0A57A-1F28-E811-9510-FA163E3FD732.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/66C0CEB7-D727-E811-AC10-02163E017C33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6809A097-F927-E811-9DD2-FA163EA437BA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6894DE3D-D027-E811-B7AB-02163E00B2CB.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/68FBD9DC-0A28-E811-9EC2-FA163E04A679.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6A156607-0128-E811-BBFE-FA163EDE64A3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6A640B36-E827-E811-A495-FA163E544B01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6A9EE044-FC27-E811-8F9B-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6AEE6146-F727-E811-AABF-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6C13DC26-F827-E811-8B1A-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6C2C2848-1728-E811-889E-FA163E5189D4.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6C4D611E-0028-E811-8D2E-FA163E361D48.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6CB7F850-2128-E811-BAC8-FA163E5702B7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6E2C5F3C-DB27-E811-8E91-02163E013CB1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6E5AD18D-D027-E811-AC03-FA163E36C42E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6E6C01CB-E227-E811-B06A-FA163EB2723B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6E83D4C5-D527-E811-A3E9-FA163E21BFF5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6E85FA91-0928-E811-BAE1-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6E946407-EE27-E811-9204-FA163EDE64A3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6E9BE489-DF27-E811-B8A6-FA163E893DB0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/6EE2E008-1828-E811-8955-02163E00BEF3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/70025808-EE27-E811-ACBA-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/70D399E7-FB27-E811-A812-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/70EF5933-E327-E811-8BF6-FA163E58D7A9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7240AB9C-E927-E811-ADBD-FA163EB8FF1D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/727D61FD-0328-E811-A85B-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/728F4CE0-E227-E811-9BC7-FA163E31557C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/72CEFFE0-DB27-E811-A29B-FA163E331F71.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/74013BFF-0628-E811-98BE-FA163E600A7A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7420D3E4-D827-E811-8CF7-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/74218E09-D527-E811-83F7-FA163E7DF37F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/744C94E0-DB27-E811-AECF-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7463F1D5-F927-E811-A738-FA163E472D18.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/74823701-1428-E811-BD7B-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/74B2C930-1828-E811-BE3A-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/74B4610A-1828-E811-A970-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/767659B6-E227-E811-8184-FA163E06735E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/76990FB4-ED27-E811-AF30-FA163E31557C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/76A8A960-E827-E811-AA06-FA163E523D0D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/76E05EDE-0428-E811-8B42-FA163E03756B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/78136F1F-DE27-E811-B291-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/786084EC-FE27-E811-AE61-02163E00B158.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/789D596D-F327-E811-859F-02163E00B845.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/78B00EB2-0428-E811-B933-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/78BEEFC4-D527-E811-A79F-FA163E8A6FB6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/78FC52D8-1B28-E811-8DF8-FA163E523D0D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7A494C00-0628-E811-B9F4-FA163E47AC10.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7A72D067-0528-E811-8400-02163E00C568.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7C55CC62-CF27-E811-B349-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7C607C5E-EB27-E811-B258-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7C692C10-1228-E811-AFE3-02163E017706.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7C9AE682-0428-E811-9615-02163E01654C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7CE6D907-FA27-E811-8C93-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7E6B0986-FC27-E811-BEE8-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7EBF0E9A-EE27-E811-AA45-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7EE5A746-E227-E811-A119-FA163ECC3DC1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/7EE679F6-1328-E811-8EFE-FA163E472D18.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/800EDBD1-1B28-E811-800D-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/80112176-DE27-E811-9AC9-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8061B537-0B28-E811-8E21-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/80AB04F3-0B28-E811-8F8B-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/80D4B4E2-FC27-E811-9415-02163E00B845.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/80DC5D95-EE27-E811-864A-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/80E4A6CD-0A28-E811-ADC0-FA163E53153A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8214E587-E527-E811-A034-FA163E8F5243.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/82209988-1028-E811-9746-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/829493E6-1628-E811-BC65-02163E01654C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/82990E3E-FE27-E811-AE00-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/829D678F-D227-E811-B9B7-FA163EF0AD4F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/82B29FEB-0F28-E811-B856-FA163E53153A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8471AED6-0E28-E811-B567-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/861A3EE9-0728-E811-BB0E-FA163EFD63BA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/862AC2F8-1D28-E811-937E-FA163E954ED5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/86420D9C-E527-E811-9868-FA163E2D5FC7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/86B71A79-DB27-E811-B48A-FA163EF8C4B1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/86D1F66E-F427-E811-89F9-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/86E8092B-D727-E811-8A6C-FA163E498180.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/88479D56-1B28-E811-907B-FA163E3E883C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/888745D5-E127-E811-B681-FA163E2D5FC7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/88D39EA7-E727-E811-ADEF-FA163E523D0D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/88F11290-D127-E811-82AA-FA163E2F9079.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8A86B03A-0628-E811-AA45-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8AE1DA61-EB27-E811-8CE2-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8C48078A-E227-E811-876A-FA163E67C610.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8CAA8EA0-F627-E811-8003-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8E1A27DA-E427-E811-9BBC-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8E30F875-D027-E811-B412-FA163EE54E99.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8E53FF9C-E827-E811-A346-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8E745EAC-1228-E811-9C2D-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8EA43BCA-DD27-E811-A354-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8EAC9CE2-1A28-E811-97A5-FA163E7C6CDD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8EBA1979-D127-E811-912D-FA163E83AE5D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/8ECB16D2-F127-E811-BB15-FA163E4533EE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/906069FF-D027-E811-9417-FA163E174C15.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/922A5C1A-0128-E811-90AA-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/926C0B54-F927-E811-9977-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/92B30754-0B28-E811-BE26-FA163E517D33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/92C38923-0928-E811-BDD9-FA163E801D16.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9403EA3F-0928-E811-B5E7-02163E00C568.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/94529553-1E28-E811-B19C-FA163E2A7247.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9487D38E-ED27-E811-BF94-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/94EB1013-FA27-E811-9B0A-FA163E30B142.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/961CA79F-EA27-E811-BA17-FA163E544B01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/96319F0B-D627-E811-B69B-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/964D145D-EE27-E811-A48F-FA163ECC3DC1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/964EC7FE-E327-E811-8DB3-FA163EB2723B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9662E399-0928-E811-862E-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/96682EB0-1028-E811-86EB-02163E00C90A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/966FF5B5-0528-E811-A64C-FA163E3BBE9B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/96C5F0A4-DE27-E811-9820-FA163E310D96.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9846E182-F627-E811-A57B-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9864E3E9-1E28-E811-A46A-02163E01A8CA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/986D0FF1-DD27-E811-A55B-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9886BDF8-0E28-E811-B54F-FA163E53153A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/98C063A9-F127-E811-9347-02163E017706.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/98E46096-FC27-E811-B741-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9A4EE428-EE27-E811-9FFB-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9A97F4AE-0E28-E811-AA2B-FA163E517D33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9AD7C82C-D727-E811-A67A-02163E00C16F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9AE076C7-1628-E811-BCB1-FA163E279739.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9AE88687-1128-E811-9259-FA163E8676B9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9C296BCB-0F28-E811-8A33-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9C74FF82-0A28-E811-B9DA-FA163EEC157D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9C91F90B-D927-E811-A0DE-FA163E93EDBE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9C978C60-EB27-E811-AD1B-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9CA3532D-F127-E811-8612-FA163ECFACBA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9CBFC502-E827-E811-A32C-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9CD35CFA-0D28-E811-8F4D-FA163E4CBD04.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9CFB79D6-0028-E811-BD39-02163E00B920.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9E095BD0-F227-E811-A8A6-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9E2CF502-FE27-E811-860F-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9ED667BE-2028-E811-8FDF-02163E01A95B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9EFF2994-0A28-E811-BAF9-FA163E6A6C62.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/9EFFEA99-D527-E811-AAC5-FA163ECCC7CF.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A0200E81-E427-E811-BB10-FA163E58D7A9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A08186BD-FB27-E811-B3C8-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A0A0F497-D027-E811-9ADA-FA163EC9DA88.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A0ACECCE-EA27-E811-A297-FA163EE76E1A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A0B04D3F-E827-E811-940E-FA163EB8FF1D.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A0BEA18E-E127-E811-BF12-FA163EFDD7AF.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A0C6179B-D427-E811-B60A-FA163E56CD46.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A0E346CF-2128-E811-82ED-02163E013EF4.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A25B121A-0028-E811-8FBE-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A28D10E6-D127-E811-BB0B-FA163E0FEB7E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A2B4C388-EB27-E811-B0BF-FA163E31557C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A45F45C8-1828-E811-B5E4-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A4817EFA-F927-E811-BCCE-FA163E361D48.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A48F73B8-D427-E811-8BD9-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A4E1CCE3-1B28-E811-9C48-02163E00C785.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A61A2861-0D28-E811-AE81-02163E015CD0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A61D31D4-ED27-E811-9627-02163E015DE3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A64A4CE5-DF27-E811-9606-FA163E893DB0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A692A284-F227-E811-93CD-FA163E2D5FC7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A698C8CA-EE27-E811-97F0-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A6B09EE7-1528-E811-9EF6-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A80C3629-DA27-E811-974A-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A80E603F-E827-E811-BA9D-FA163E472D18.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A872173C-E927-E811-8798-FA163EEB2BCA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A87C0AEF-1F28-E811-8921-FA163E7C6CDD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A87D813F-2028-E811-B949-02163E015D35.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A8886B3C-E327-E811-9433-FA163E0D5E50.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/A8B66329-1B28-E811-98AD-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AA09B59B-F327-E811-8CD3-FA163ECFACBA.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AA73125F-0428-E811-A585-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AA749D5F-0928-E811-8DB2-FA163E3BBE9B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AAD90E0D-FE27-E811-992A-02163E00B158.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AAE599BB-1728-E811-8A58-FA163EC32BAB.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AC3A3EEA-0628-E811-9CA0-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/ACD4D32B-E127-E811-9AF8-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AE5B91E8-F127-E811-9A03-FA163ED04110.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/AEF2873F-EF27-E811-BBFE-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B05E7278-E527-E811-ADF8-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B2815405-F227-E811-B833-FA163E4E0CCE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B29D2CF6-D527-E811-987A-02163E017C33.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B4237A70-DB27-E811-A6B3-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B48112D4-F427-E811-9CE1-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B61BA421-0B28-E811-8459-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B630D8F6-1E28-E811-885A-FA163E850905.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B8359579-D327-E811-B1A0-FA163EF0AD4F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B86F4470-FD27-E811-92C0-FA163E361D48.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/B8F04E53-1628-E811-A57F-FA163E9F91D6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BA92EBE6-1E28-E811-AEA7-02163E00BAB6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BC0D2E80-0C28-E811-A9D8-FA163E47AC10.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BC3AD41A-D127-E811-9112-FA163E21BF4B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BC5B68D0-ED27-E811-8C43-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BCE590E7-E727-E811-ABB3-FA163EF890F0.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BE3529DA-0528-E811-8745-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BE42F48B-F927-E811-9A20-FA163E03C693.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BEDD2AD8-E527-E811-B0EC-FA163E056376.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/BEF4225C-F327-E811-BEFB-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C019E408-E127-E811-B259-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C070C536-DA27-E811-8BD6-FA163EAD427A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C0FA7173-1628-E811-8559-FA163E47AC10.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C2353092-D427-E811-AEB8-FA163E61D2A6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C23E1066-0D28-E811-A45A-FA163E47AC10.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C257176A-DF27-E811-A764-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C29C7868-F427-E811-B864-FA163E962411.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C2B9837F-E227-E811-A52B-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C48AB720-E027-E811-9031-FA163EEA4C01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C4F682EF-F227-E811-8B60-FA163E6F382C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C60A6927-1E28-E811-9052-FA163E850905.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C62F4E67-CF27-E811-AD12-FA163E2D5FC7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C6591697-D027-E811-B906-FA163E2D5FC7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C6B44FA2-0D28-E811-9C6E-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/C806B43B-1B28-E811-B2FE-FA163EA0727F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CA03B1A8-DF27-E811-9769-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CA13C5C8-1828-E811-80B9-FA163E2FE61A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CA18ADF6-DA27-E811-A4CD-02163E00F6F1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CA333D79-0228-E811-8953-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CA3D9840-F927-E811-B948-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CA5E4556-CF27-E811-8136-FA163E32403C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CABC446F-EA27-E811-9302-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CAC6508E-D327-E811-878D-FA163E7DF37F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CAC7FDD6-1328-E811-AE6D-FA163E736E92.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CAE86999-0028-E811-A027-FA163E9AD52A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CC284B6F-1628-E811-A6F3-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CC29D9F3-0C28-E811-8753-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CC85D11B-1328-E811-948D-FA163E7052A5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CC937CEB-0728-E811-A02A-02163E01A098.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/CE653A87-1128-E811-B669-FA163E4CBD04.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D034D9EF-1A28-E811-9A3F-FA163EF1616A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D07CEC25-E927-E811-B761-FA163E67C610.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D0B25713-0028-E811-816F-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D21EECD1-1128-E811-AEC3-FA163E7052A5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D227BCC8-1928-E811-8B19-02163E00CE9C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D2599862-EF27-E811-848E-02163E015DE3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D27F9AA0-0128-E811-AE19-FA163E3C37D4.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D2C589B9-EA27-E811-9EEB-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D2FE437D-0128-E811-B366-02163E00AD01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D4759251-E327-E811-A8AF-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D6092F60-1528-E811-9466-FA163E9F91D6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D63A2493-EE27-E811-91C2-02163E015DE3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D6B12515-FE27-E811-82E6-FA163E8E65F7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D6CAB02D-0028-E811-B2DE-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D6D8CBC2-DD27-E811-89E2-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D82938F1-FE27-E811-AFF1-02163E017634.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D8339556-0F28-E811-9821-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D87E1B98-D427-E811-B178-FA163E21BF4B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D89E669F-D527-E811-B465-FA163E0339E1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D89F46CF-E727-E811-8F02-FA163E34CB74.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D8AB5F95-F727-E811-BFD6-02163E017634.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D8B5DFF2-E727-E811-B0B2-FA163E65100B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D8CFB515-0728-E811-9311-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D8F7ECAD-E227-E811-AAF6-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/D8FA0524-0F28-E811-84E7-FA163E4CBD04.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DA01DD3B-E027-E811-8407-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DA44B8DA-0428-E811-A2AA-FA163E8E65F7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DA5A0B17-1428-E811-A552-FA163E7052A5.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DA5F7D96-E127-E811-8148-FA163E498180.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DAA56D5A-1928-E811-B083-FA163ED88C48.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DAE7CDF2-0B28-E811-8D3C-FA163E47AC10.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DC30F82F-1828-E811-8135-02163E00B920.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DC719E0C-E627-E811-8079-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DC7F2A24-0728-E811-BDB5-FA163E985227.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DE12A38B-0A28-E811-99C8-FA163E8F5243.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DE172CC0-1028-E811-A711-FA163E4CBD04.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DE1A7B3C-F727-E811-B52F-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DE7C6766-CF27-E811-A3AB-FA163E0FEB7E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DECF3622-DD27-E811-AF34-FA163ECCC7CF.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/DEFBEB7A-E927-E811-9377-FA163E544B01.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E056B12D-EC27-E811-96B7-FA163EDE64A3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E06E0B6A-0128-E811-8A81-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E078829D-D427-E811-A5F4-FA163E310D96.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E0A99C49-F127-E811-A429-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E22FD744-F227-E811-B36B-FA163E4E0CCE.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E278F890-E127-E811-93A4-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E43ADC02-1828-E811-B4F1-02163E00CE9C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E47CD2E8-E527-E811-82C9-FA163E58D7A9.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E4B9367A-D627-E811-8B24-02163E00B478.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E4F8BBE9-0128-E811-B790-FA163EBDF1DC.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E6A304A4-ED27-E811-A43B-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E6ACA031-1028-E811-92F6-FA163EF5D4C6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E6DBACCA-D727-E811-A02E-02163E017C49.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E6EC1553-D527-E811-9C8C-FA163EA82112.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E82713AC-D327-E811-9D8B-FA163E310D96.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E84C85FF-E827-E811-A64F-FA163ECD532B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/E8B1C4CC-E527-E811-9A57-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EA46217B-F327-E811-AEDD-02163E00B159.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EA9F1843-E027-E811-A7FA-FA163E73968C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EABA4156-F727-E811-A124-FA163E2D5FC7.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EAC10462-D127-E811-BD16-02163E014B6E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EAD41780-E227-E811-8626-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EAD5500E-E327-E811-BCEE-02163E00BE39.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EAE4B5F3-1528-E811-9935-02163E019F19.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EC2A12C7-2028-E811-AA33-FA163E95E333.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EC2ED599-E127-E811-8328-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EC439813-EF27-E811-991A-FA163EDE64A3.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EC8A3EA8-1728-E811-92C4-02163E00C30A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EC904D3F-0228-E811-9F42-02163E01A124.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/ECDC2BD0-E227-E811-A75E-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EE156627-E427-E811-A8D0-FA163EFA0E12.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/EE39BF61-D727-E811-B8A8-FA163EBC586E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F02981E4-E127-E811-8E86-FA163E310D96.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F02DE412-D927-E811-A5CB-FA163EF8C4B1.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F0615D02-D327-E811-AF97-FA163EF0AD4F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F202349E-0D28-E811-8090-FA163EAFBB72.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F2AE4F4F-F427-E811-B11D-FA163E2AF044.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F4A74CE6-F127-E811-97FC-02163E00C568.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F4E61581-1228-E811-A302-02163E00CE9C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F6A8B860-DB27-E811-9671-FA163E331F71.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F6EBC20B-0628-E811-B3DE-02163E00B997.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F84EE117-FE27-E811-86B6-FA163E265254.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F8DE9F4A-D727-E811-9877-FA163E7DF37F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F8E1343F-FA27-E811-95F6-FA163ED1E1E6.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/F8F8FB50-0028-E811-A382-FA163EBCB8FD.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FA0F208A-FD27-E811-9E82-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FA106B51-DB27-E811-A2D0-FA163E24EC20.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FA1B938F-FB27-E811-A778-FA163EE60398.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FA6B92B9-1328-E811-9857-FA163EAFBB72.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FA9564E7-1B28-E811-B151-02163E017706.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FAB31860-F427-E811-8B8F-FA163E20A15A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FC8648C3-0428-E811-A3CD-FA163E903C4E.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FC8E9793-F627-E811-9D05-FA163E31557C.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FC959C09-1A28-E811-AE54-02163E01A95B.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FCFD02D9-EF27-E811-A1BC-FA163E9AD52A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FE7A9520-E227-E811-A1FA-FA163E9AD52A.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FE9EBF61-DA27-E811-9FA0-02163E00C16F.root",
"/store/express/Commissioning2018/ExpressCosmics/FEVT/Express-v1/000/311/879/00000/FEC49FA8-1928-E811-8975-FA163EEB9CD8.root",
])



process.source = cms.Source("PoolSource",
 fileNames =  myfilelist
)


# fileNames =  cms.untracked.vstring('file:rawdata.root')
#fileNames =  cms.untracked.vstring(

#"root://eoscms//eos/cms/store/data/Run2015C/ZeroBias/RAW/v1/000/254/227/00000/FA244051-8141-E511-B22B-02163E014153.root",

# "/store/express/Run2017E/ExpressPhysics/FEVT/Express-v1/000/303/824/00000/02000C18-EBA0-E711-B8FB-02163E01A29A.root",

#"/store/express/Run2017D/ExpressPhysics/FEVT/Express-v1/000/302/131/00000/000109B7-6C8E-E711-89D1-02163E019CD2.root",

#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/193/00000/0E6E4ACA-4F5A-E611-97B5-FA163E1E4ACD.root",

# )
#   skipEvents = cms.untracked.uint32(5000)
#)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('303824:54-303824:999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('278193:77-278193:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('302131:34-302131:943')

# for Raw2digi for data
process.siPixelDigis.InputLabel = 'rawDataCollector'
process.siStripDigis.ProductLabel = 'rawDataCollector'

#process.siPixelClustersPreSplitting.SeedThreshold = 1000
#process.siPixelClustersPreSplitting.ChannelThreshold = 2 #must be bigger than 1
#process.siPixelClustersPreSplitting.ClusterThreshold = 1000    # integer?
#process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 1000 # integer?
#process.siPixelClustersPreSplitting.payloadType = cms.string('Full')


# for digi to clu
#process.siPixelClusters.src = 'siPixelDigis'

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
#       threshold = cms.untracked.string('ERROR')
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)


# pixel local reco (RecHits) needs the GenError object,
# not yet in GT, add here:
# DB stuff 
useLocalDB = True
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
        tag = cms.string('SiPixelGainCalibration_2018_v1_offline') #in DB
#        tag = cms.string('SiPixelGainCalibration_2017_v2_bugfix') #in DB
#        tag = cms.string('SiPixelGainCalibration_2017_v2_bugfix_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v2_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v3_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v4_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v5_offline') # v4 fixed
#        tag = cms.string('SiPixelGainCalibration_2017_v6_offline')
#        tag = cms.string('SiPixelGainCalibration_2017_v4')
#        tag = cms.string('SiPixelGainCalibration_2017_v5')
#        tag = cms.string('SiPixelGainCalibration_2017_v6') # full tag & in DB
#        tag = cms.string('SiPixelGainCalibration_2017_v4_1337_offline')
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
     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v1_offline.db')

#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_phase1_ideal_v2.db')

#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 


useLocalDBError = False
if useLocalDBError :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
         record = cms.string('SiPixelGenErrorDBObjectRcd'),
# 	 tag = cms.string("SiPixelGenErrorDBObject38Tv1")
#        tag = cms.string('SiPixelGenErrorDBObject_38T_2012_IOV7_v1')
         tag = cms.string('SiPixelGenErrorDBObject_38T_v1_offline')
 	 ),
       ),
#     connect = cms.string('sqlite_file:siPixelGenErrors38T_2012_IOV7_v1.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
#     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
     connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# end 



process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:reco.root'),
#    fileName =  cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/tracks_1_0.root'),
    #outputCommands = cms.untracked.vstring("drop *","keep *_*_*_MyRawToClus") # 13.1MB per 10 events
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,  # 4.9MB per 10 events 
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)


process.c = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("generalTracks::MyRawToTracks"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),                             
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
)

process.c1 = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("generalTracks::MyRawToTracks"),
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
    src = cms.InputTag("generalTracks::MyRawToTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
# additional selections
    Select1 = cms.untracked.int32(14),  # select the cut type, o no cut
    Select2 = cms.untracked.int32(1),  # select the cut value   
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_tracks.root')
)


# copy the sequence below from  
# RecoTracker/IterativeTracking/python/iterativeTk_cff.py  - 71_pre7
process.myTracking = cms.Sequence(process.InitialStep*
                            process.DetachedTripletStep*
                            process.LowPtTripletStep*
                            process.PixelPairStep*
                            process.MixedTripletStep*
                            process.PixelLessStep*
                            process.TobTecStep*
                            process.earlyGeneralTracks*
                            # muonSeededStep*
                            process.preDuplicateMergingGeneralTracks*
                            process.generalTracksSequence*
                            process.ConvStep*
                            process.conversionStepTracks
                            )


#process.p = cms.Path(process.hltfilter*process.RawToDigi*process.reconstruction*process.c*process.c1*process.c2)
process.p = cms.Path(process.hltfilter*process.RawToDigi*process.reconstruction*process.c)

#process.ep = cms.EndPath(process.out)
