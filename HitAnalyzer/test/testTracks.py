#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("T")

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v4' # for 251643
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v3' # for 266277
# AUTO conditions 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
       
process.MessageLogger = cms.Service("MessageLogger",
#    debugModules = cms.untracked.vstring('TestPixTracks'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#   HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Random_v*'],
    HLTPaths = ['HLT_ZeroBias*'],
#     HLTPaths = ['HLT_L1Tech54_ZeroBias*'],
#     HLTPaths = ['HLT_L1Tech53_MB*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v*'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

# to select PhysicsBit
#process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
#process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/02220AF5-D8E9-E511-B238-02163E01224D.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/02478F81-D9E9-E511-9474-02163E013459.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/02B44568-D9E9-E511-BEAE-02163E0138EE.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/0432CA4A-D9E9-E511-9D90-02163E01352A.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/04FE84A1-D8E9-E511-8EF5-02163E014336.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/0686DC74-D8E9-E511-AE12-02163E0127AD.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/08B2912C-D9E9-E511-A90B-02163E013507.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/0A7596D6-D8E9-E511-B28F-02163E014261.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/0AF4088A-D9E9-E511-B17A-02163E014788.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/10905063-DAE9-E511-9FD7-02163E013886.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/12119628-D9E9-E511-B899-02163E011D09.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/142E2516-D9E9-E511-8422-02163E0144C8.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/20F0C050-DAE9-E511-A120-02163E0143FE.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/24A9A898-D9E9-E511-9FBF-02163E011CE4.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/261220D0-D8E9-E511-928A-02163E0144BD.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/26786C08-DAE9-E511-9735-02163E013559.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/30319180-D9E9-E511-A398-02163E013477.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/3059386B-D9E9-E511-AA6C-02163E014334.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/323CF743-DAE9-E511-BFCF-02163E014408.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/32614A86-D9E9-E511-9C92-02163E0117FF.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/3403DABC-D8E9-E511-82A9-02163E013667.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/364CFABA-D8E9-E511-B9BD-02163E0146DA.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/3ABD4E2D-D9E9-E511-B68D-02163E01381E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/3AD56EB9-D8E9-E511-B965-02163E0134DD.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/3AF0707D-D9E9-E511-B09E-02163E0142F8.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/3C35C73B-D8E9-E511-95A8-02163E013961.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/3E7CD1EB-D9E9-E511-BA7A-02163E0145FA.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/40206D32-D9E9-E511-8F9A-02163E01456D.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/4049DF00-D9E9-E511-969B-02163E014776.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/488A5A50-D9E9-E511-92D1-02163E011DC2.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/48EE3773-D9E9-E511-B138-02163E0134B6.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/4C92C0FC-D8E9-E511-ABA3-02163E011D09.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/502E338F-D9E9-E511-BE4E-02163E011CE4.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/54D6F8BE-D8E9-E511-82C6-02163E01430D.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/5AADACF3-DBE9-E511-B871-02163E013805.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/5C84C7DF-D8E9-E511-859A-02163E0141A9.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/5E029BBE-D8E9-E511-9B58-02163E0142EE.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/608A31DA-D8E9-E511-B606-02163E012BD2.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/64CC3F8F-D9E9-E511-BAEC-02163E01449E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/64E551D7-D9E9-E511-A5F0-02163E013575.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/669406B3-D9E9-E511-BCF5-02163E014734.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/74060F21-D9E9-E511-BB9D-02163E01416F.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/7433BF5B-DAE9-E511-AA00-02163E0139A6.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/76A66B45-D9E9-E511-BAA2-02163E01456D.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/7810A400-D9E9-E511-ABFF-02163E014684.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/7C6C0661-D9E9-E511-B1C3-02163E013575.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/7C6DE33C-D9E9-E511-9C8F-02163E01352A.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/7E7E6C6E-D9E9-E511-BA08-02163E01357E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/825CA6F3-D8E9-E511-B374-02163E014533.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/88B61731-D9E9-E511-9F2A-02163E013825.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/8AD25DDB-D9E9-E511-806F-02163E013521.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/8C15EE97-D9E9-E511-9BDE-02163E012062.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/90FBAC5D-D9E9-E511-A6DC-02163E0146EC.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/922702F2-DAE9-E511-A8EA-02163E0145F5.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/960ED797-D9E9-E511-8AF5-02163E012B0F.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/964E2A5A-D9E9-E511-8E31-02163E014635.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/989E2602-D9E9-E511-9AEF-02163E0141AB.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/9A6DF183-D9E9-E511-930E-02163E011829.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/9C8362AA-D9E9-E511-80E5-02163E0142DC.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/9E7E8869-D9E9-E511-A562-02163E0142C5.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/9EE7EEA8-D9E9-E511-9BDD-02163E0144FF.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/A46B1431-D9E9-E511-8449-02163E014407.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/A4F9C7DB-D8E9-E511-B00A-02163E0140D7.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/A637C002-D9E9-E511-BC91-02163E01422F.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/A63F3E59-D9E9-E511-9984-02163E012B71.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/A8B552F0-D8E9-E511-AD20-02163E01470A.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/B463F871-D9E9-E511-9F4C-02163E0133B2.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/B4BD0CEB-D8E9-E511-B898-02163E01395E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/B83A8131-D9E9-E511-BC6F-02163E0118BB.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/BA5BD7EA-D8E9-E511-98E2-02163E013611.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/BE472BF2-D8E9-E511-8254-02163E0117FF.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/C09705E9-DDE9-E511-8326-02163E013575.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/C804ADEA-D8E9-E511-9C45-02163E012B71.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/CA4B1689-D8E9-E511-ABFB-02163E013432.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/CA9875B3-D9E9-E511-A41B-02163E014691.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/D26121DD-D8E9-E511-8698-02163E01419C.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/D62E0984-D8E9-E511-8FF8-02163E0128DB.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/D64C5D5C-D9E9-E511-86B2-02163E0133CC.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/DA507A05-D9E9-E511-94F9-02163E01449E.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/DAC5EC40-D9E9-E511-AA89-02163E013825.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E044815E-D9E9-E511-A3B8-02163E0136EB.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E21C4B4D-D8E9-E511-A4BF-02163E014447.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E2214999-D9E9-E511-810C-02163E013817.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E2C0E18E-D9E9-E511-B187-02163E013559.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E46DDC62-D9E9-E511-98CE-02163E011F11.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E616FA9A-D8E9-E511-BA27-02163E0142F9.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E64BDCC4-D8E9-E511-9BBB-02163E01249B.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E85F0B66-D9E9-E511-AACF-02163E013952.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/E888B348-DAE9-E511-8E51-02163E0117FF.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/ECCA58FA-D8E9-E511-A696-02163E013559.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/EE678AE3-D8E9-E511-AD0F-02163E013391.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/F0600568-D9E9-E511-92F1-02163E0129C3.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/F431F183-D9E9-E511-AB2A-02163E011829.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/F4ABF12C-D9E9-E511-BA66-02163E014510.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/F6123184-D9E9-E511-BC02-02163E013815.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/F8688258-D9E9-E511-BF15-02163E014100.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/FE156C34-D9E9-E511-8C85-02163E0144DC.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/FEB8D531-D9E9-E511-BF77-02163E013449.root",
"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/FED7F10E-DAE9-E511-BDE0-02163E014408.root",
])

process.source = cms.Source("PoolSource",
 fileNames =  myfilelist
# fileNames =  cms.untracked.vstring(
#    'file:tracks.root'
#"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/267/767/00000/0A50CE9E-3AF3-E511-8A5E-02163E012A04.root",
#"/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/267/767/00000/649C9422-45F3-E511-8AB7-02163E013807.root",

# "/store/data/Commissioning2016/Cosmics/RECO/PromptReco-v1/000/266/665/00000/02220AF5-D8E9-E511-B238-02163E01224D.root",
#    )
)


#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('260627:97-260627:1910')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_tracks.root')
)


process.d = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
#    src = cms.InputTag("generalTracks"),
# for cosmics 
    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)



