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
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v4' # for 251643
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
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/000A8212-8284-E511-8BB1-02163E01184B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/003021E6-8E84-E511-AE88-02163E0140D9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/00429574-7A84-E511-8BE6-02163E014340.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/006ABCF6-7684-E511-8A61-02163E011A83.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/02CB057F-7984-E511-BDCD-02163E014184.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/06026368-7F84-E511-9505-02163E011B74.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/086F7562-8184-E511-AFCB-02163E014564.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/08D74E6F-7984-E511-B9E1-02163E011D6D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/08D9BBFB-7D84-E511-A537-02163E01434C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/0C507FFF-8684-E511-9E2C-02163E01472C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/0C5126B8-8A84-E511-A027-02163E01260E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/0E0C0847-8084-E511-A6E8-02163E01338D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/10453D98-8C84-E511-8ABA-02163E0144B1.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/10E94D2A-8784-E511-94DC-02163E014455.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/10FC5AA9-7484-E511-AC5A-02163E014340.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/12FC69ED-7984-E511-B44E-02163E012922.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1418C52E-7E84-E511-8EC2-02163E0118CC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/14CC2454-7784-E511-B820-02163E0146FD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/161AFBEC-8584-E511-ADC8-02163E01398D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/16DC67B0-7F84-E511-8E12-02163E013724.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1859AA56-8184-E511-930B-02163E011859.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/18D14996-8484-E511-8746-02163E0145B5.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1A876E91-7F84-E511-8348-02163E0134CD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1AA089C9-7784-E511-A66D-02163E014315.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1CFFFC70-7E84-E511-83C3-02163E01416C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1E19FC92-7884-E511-A6B8-02163E01274F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1E4FE22B-7884-E511-8DCA-02163E0146C0.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1E69A12A-8284-E511-A7C5-02163E01260D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/1ED7D189-7584-E511-ABB4-02163E0121E9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/201BD3B0-7584-E511-8CDD-02163E011B22.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/20978CA4-7E84-E511-9330-02163E0143E7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/245674A3-7A84-E511-A360-02163E014344.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/2492E046-7A84-E511-A3CA-02163E01460F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/24A1948C-8584-E511-800F-02163E011A0C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/262AC693-8584-E511-8D09-02163E01438B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/28023D0F-9184-E511-A937-02163E0139DF.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/285BC0CF-7684-E511-97A7-02163E014564.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/28D3F304-7884-E511-B4C2-02163E012424.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/28DC5ED9-8484-E511-BDF7-02163E011859.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/2A955BEB-7984-E511-B3AF-02163E012084.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/2AEC1D3B-8E84-E511-9ADD-02163E0127B9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/2C52EC9C-8284-E511-8DA2-02163E0133BD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/2E0F187C-8184-E511-983B-02163E013507.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/2E8C350A-7784-E511-8855-02163E013424.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/2ECE65B1-7B84-E511-AB9B-02163E014337.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/30FC3299-8C84-E511-B0C6-02163E0134CD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/3246059D-9284-E511-A1DE-02163E01352B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/32F1EC72-7A84-E511-A22B-02163E0143E8.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/34B69FBD-7984-E511-A33E-02163E011BFB.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/369159AB-7C84-E511-B9C0-02163E014244.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/36A782DB-8A84-E511-924A-02163E014707.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/36CE60B3-8A84-E511-91FD-02163E01372C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/38CF8037-8484-E511-974F-02163E014109.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/3A1BB7AB-7784-E511-B39D-02163E013526.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/3AFB5E1D-7E84-E511-8E82-02163E01471F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/3CF04974-8684-E511-97AE-02163E0140DF.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/3E474A72-7A84-E511-AAF5-02163E0133BD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/42062103-8784-E511-A417-02163E0137DD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/42331E40-7A84-E511-9AB0-02163E0118CC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/424BB5AB-7884-E511-B974-02163E01369C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/42598EB9-7984-E511-8E3D-02163E013452.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/44B26A42-8784-E511-AD3D-02163E0146E3.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4634128D-8984-E511-B5AC-02163E012062.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/46703F79-7A84-E511-92E6-02163E0141C9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/480BC2EF-7584-E511-BF6D-02163E011D6D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4877F8A4-8284-E511-8C44-02163E01453D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4A975C31-8484-E511-8671-02163E01434E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4AF9A05C-8F84-E511-919F-02163E01471C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4C096BB6-7784-E511-83DF-02163E0144EE.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4C2C7EC5-8784-E511-AB9E-02163E01385F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4C3C0403-7884-E511-B203-02163E0136BE.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4E26A30E-7884-E511-B20C-02163E0140FC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4E585F3C-8484-E511-8664-02163E01271D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/4EB0C683-7884-E511-8821-02163E0121E9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5099A97D-7984-E511-B353-02163E0143E7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/523AD1B8-8784-E511-998C-02163E0136E2.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5403EEF4-8184-E511-88BE-02163E014473.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/54AB680E-7884-E511-BDFC-02163E0136E2.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/54EDE102-7884-E511-8109-02163E014405.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5680359C-7884-E511-88E1-02163E01381F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5866A978-7C84-E511-94EB-02163E0118CC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/586E59B6-7784-E511-A10E-02163E01476E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/58B5920A-7684-E511-B0A9-02163E01446B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/58D888F9-7784-E511-AD70-02163E0143E8.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5C25EC08-9684-E511-B47B-02163E014243.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5CF7850C-7784-E511-AD7D-02163E014455.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5E030B6F-9584-E511-8625-02163E0143DC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5EDC03B6-8B84-E511-BF92-02163E0133D1.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/5EEDE018-8684-E511-A4C0-02163E011F49.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6007D213-8384-E511-BDC6-02163E012922.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6097447D-8584-E511-BF2C-02163E0144EE.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/60C73DAB-7E84-E511-9B44-02163E014278.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/621E5310-8884-E511-B574-02163E01477E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/62484700-8484-E511-B49F-02163E014557.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6481C933-8E84-E511-AE06-02163E0142B7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/649899C6-8284-E511-AB6F-02163E01469D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/64B02165-7684-E511-8BC1-02163E0142B7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/64B54CAA-8884-E511-AB6A-02163E0133BD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6694B05D-8184-E511-BC31-02163E0137FA.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/66A7F6FA-7684-E511-8B26-02163E014239.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6807A06C-8684-E511-A45D-02163E014235.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/682B23BB-8B84-E511-91D9-02163E01373A.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/682EB695-7F84-E511-AA8E-02163E012919.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/68D7D05E-7A84-E511-8231-02163E013836.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6ADCD1D6-7C84-E511-8518-02163E0133D0.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6C223421-8384-E511-B80B-02163E013599.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6CAC04C8-7784-E511-ADBB-02163E0143F4.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6CE55E9B-7A84-E511-B743-02163E0133F4.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/6CF82146-8B84-E511-8B27-02163E01452F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/706AC66D-7B84-E511-8E60-02163E013842.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/720BD0A8-7A84-E511-A05D-02163E01478E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/72547B9C-8384-E511-B4F6-02163E012764.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/727F2A86-7484-E511-9412-02163E014310.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/746E1E59-8584-E511-9CCA-02163E01450E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/7635CC7A-7484-E511-8B58-02163E01477E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/76908689-7884-E511-A339-02163E0137B9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/76950BB6-7B84-E511-92B6-02163E0145D7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/76CB356B-9384-E511-8E04-02163E011859.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/78AD1FDB-8284-E511-92CF-02163E0140DD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/78DABBFD-7D84-E511-9E12-02163E014648.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/7A299072-8984-E511-838B-02163E0121B8.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/7ACDCEFF-8784-E511-B2EE-02163E014431.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/7AEC9EC7-7B84-E511-8B43-02163E01464C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/7C079012-7D84-E511-A173-02163E011A71.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/7E260673-8F84-E511-B3FA-02163E0137A5.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/7ED131FD-8884-E511-AE5B-02163E01388E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/80C6F34E-8884-E511-9D91-02163E014770.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/80CFAFD0-7D84-E511-9E6C-02163E0145B5.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/82A4C8B1-7F84-E511-8E3E-02163E014253.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8441B55B-7784-E511-8164-02163E012845.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/84CD9C22-7484-E511-B970-02163E0135A1.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/866B22E8-7984-E511-9BF1-02163E014310.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/86E641FC-8984-E511-93EB-02163E012611.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/86F16430-9184-E511-8EF4-02163E014736.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/86FFB895-7E84-E511-A04A-02163E0145D7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8823E94F-7D84-E511-AE39-02163E011977.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8870C104-7184-E511-9A22-02163E013765.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/88A67B39-7E84-E511-860C-02163E0118CC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/88CE9CC7-7484-E511-BCE9-02163E0143DC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8A71CB58-8384-E511-B9E7-02163E01385F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8AC016EC-8C84-E511-B0C5-02163E014314.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8AD43706-7584-E511-B53D-02163E01356A.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8CD947FF-7884-E511-92CD-02163E01449F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8CDD86FD-8C84-E511-A0F8-02163E012611.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8E6BA0C7-9184-E511-8F1A-02163E0143C3.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8E7444FD-7C84-E511-9752-02163E013425.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8E79EADB-8484-E511-979C-02163E01448A.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8EAC8FE2-7884-E511-94CF-02163E014239.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/8ED06F7A-7D84-E511-BBA8-02163E01422E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/90B187A1-8884-E511-AE89-02163E013697.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/9253F15F-8F84-E511-9FFE-02163E011BCC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/9291AA6B-8284-E511-AAE0-02163E0143CF.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/92AB0C63-8784-E511-8BFA-02163E0134BE.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/94CBE394-8484-E511-8DF2-02163E014117.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/984AA40D-7884-E511-BA90-02163E0135A1.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/98A00ED4-7884-E511-AC13-02163E0142D6.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/9A3EE3C7-8884-E511-BF34-02163E01233D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/9A480BAB-8284-E511-8087-02163E014244.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/9AACA02F-8784-E511-8EE7-02163E01471C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/9EAE9948-8D84-E511-96E7-02163E01233D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A027E0C0-7B84-E511-83EE-02163E0143A3.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A2048565-7684-E511-9FFC-02163E013507.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A244E0B9-8B84-E511-BA60-02163E01432F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A4CB6FCD-7884-E511-BAC0-02163E0138FA.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A6524797-7684-E511-8939-02163E014281.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A664AEEB-8084-E511-AB3E-02163E0137B9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A69AF1B7-8B84-E511-8E18-02163E0133E0.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/A6A646B5-8C84-E511-8233-02163E0133B9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/AA2EE110-8384-E511-96A8-02163E01441D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/AA38E20B-7D84-E511-863C-02163E0138DF.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/AA467E19-8984-E511-8F81-02163E0143F7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/AA4DA4A3-8B84-E511-B703-02163E014251.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/AA628C05-9184-E511-BFD6-02163E013407.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/ACA6A1EC-7184-E511-8224-02163E011C1A.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/ACDA08FC-8584-E511-95E1-02163E0144A5.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B03D6CE8-7484-E511-8D63-02163E014310.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B088DC3A-7B84-E511-A097-02163E01398D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B27BD4E8-8A84-E511-9DF8-02163E014291.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B2936FB5-7E84-E511-A42C-02163E0141ED.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B46B88F2-7684-E511-99CA-02163E011B46.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B4DA1AD2-8B84-E511-92E0-02163E0139DF.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B4E512B9-7984-E511-AA64-02163E012455.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B86C31DF-8184-E511-BA50-02163E013407.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B87DCE02-7E84-E511-AE17-02163E01446B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B88167EC-7984-E511-9E37-02163E0134F7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/B8BAEE0E-7E84-E511-991B-02163E0145A1.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/BA6B189A-7584-E511-93C1-02163E0134F7.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/BA9E8C7C-7884-E511-A16B-02163E014164.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/BC701B07-7C84-E511-A5C3-02163E014351.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/BCDEDD50-7A84-E511-95D8-02163E014424.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/BCE20636-9A84-E511-BC78-02163E01271D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/BE434D97-7684-E511-96A4-02163E0143DC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/BEC4A5F2-8C84-E511-8405-02163E0140FC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C09EB27C-7984-E511-9FD2-02163E013550.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C0B1A262-8184-E511-947F-02163E01225C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C0D00A7B-8C84-E511-865C-02163E014685.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C20A9853-8684-E511-BD4C-02163E014483.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C23848EC-8C84-E511-ADF6-02163E01464C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C256BCE3-8F84-E511-8C7D-02163E014685.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C2B76B81-8284-E511-B68C-02163E014677.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C2CFEE26-7984-E511-BC20-02163E0137D5.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C8D5AFFE-7984-E511-9CA8-02163E01343E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C8DEFAEC-9184-E511-9969-02163E01419C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/C8FAD780-7D84-E511-AF38-02163E0140EC.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CA507925-8A84-E511-8153-02163E0143E8.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CAACC697-8784-E511-A683-02163E013541.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CC0E3F9A-8584-E511-9B0D-02163E013842.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CC14782A-8484-E511-8250-02163E01215B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CE385B07-8A84-E511-8F3C-02163E0134BE.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CE60CE28-7884-E511-AE25-02163E014670.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CE7601B0-7F84-E511-A1B6-02163E0135FF.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CE7A9501-8284-E511-BC77-02163E01339E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/CE9E992B-8A84-E511-8977-02163E0128D9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D0253F14-8C84-E511-8814-02163E014258.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D02B110A-7884-E511-B92F-02163E014340.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D07FC4D7-8984-E511-8A3F-02163E01420B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D0891593-7884-E511-9342-02163E014116.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D0E8050F-7B84-E511-B479-02163E0119D3.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D28984AF-8684-E511-AA46-02163E01372C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D4348DB7-8784-E511-9B3B-02163E01373A.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D45BE225-7584-E511-9E75-02163E014243.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D4AA9922-7784-E511-8700-02163E01430E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D4FB8165-7D84-E511-953C-02163E01467E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D6783BD9-8A84-E511-900D-02163E014281.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D6DA807F-7984-E511-B38A-02163E01288F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D8577EBF-7B84-E511-BF6A-02163E01439C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/D8C3FED6-8184-E511-9299-02163E014394.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DA21E051-7A84-E511-9B99-02163E013392.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DA6A97AA-7684-E511-AFB5-02163E01369C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DAF4B7E8-7E84-E511-9E93-02163E013765.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DC1EA5F2-8884-E511-B225-02163E0145B5.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DC31F00D-8384-E511-96F6-02163E0125D9.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DC39D0AD-8784-E511-938B-02163E012B63.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DE7010EA-8B84-E511-9E64-02163E014424.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DED129B6-7784-E511-8F0E-02163E013697.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/DEF7C1AC-7784-E511-900D-02163E012AA6.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E010C583-8284-E511-BAEC-02163E013765.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E0414B6A-7684-E511-8F19-02163E01424B.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E2D30B05-7884-E511-AF0D-02163E013599.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E2ED245D-8884-E511-8A0A-02163E0139AF.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E4125114-8D84-E511-8E28-02163E0136CE.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E6196893-7584-E511-B34A-02163E01369D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E66947A5-7584-E511-BFCC-02163E013724.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E6B80DF2-7784-E511-8C55-02163E0145D2.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E8927513-7784-E511-A5FF-02163E01420F.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/E8978877-8284-E511-817F-02163E0142CD.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/EA0FBCAD-7B84-E511-ADEF-02163E0143B8.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/EAD7B214-8884-E511-8DF9-02163E011A0C.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/EC6D7303-7884-E511-BF2F-02163E011B74.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/EE714CC1-8A84-E511-ACC8-02163E01446E.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F07CDEE5-8584-E511-8763-02163E012AB8.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F2AA79B2-8984-E511-A4D7-02163E014239.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F4542830-8484-E511-B99C-02163E01448A.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F471A169-8284-E511-8380-02163E014775.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F4C26FDD-8A84-E511-9F0D-02163E011859.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F636D022-8384-E511-BAAF-02163E0143E8.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F80056D1-7D84-E511-9D7D-02163E0145B5.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/F80D6078-7E84-E511-8E1E-02163E0129E1.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/FC06A64E-7E84-E511-AD60-02163E011D6D.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/FC781203-7884-E511-9A65-02163E012455.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/FC83EEE6-8484-E511-A2C0-02163E0121A0.root",
"/store/data/Run2015D/ZeroBias/RECO/PromptReco-v4/000/260/627/00000/FEBAA932-8184-E511-85AB-02163E0144F2.root",
])



process.source = cms.Source("PoolSource",
 fileNames =  myfilelist

# fileNames =  cms.untracked.vstring(
#    'file:tracks.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/tracks/tracks1.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks2_mc71.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks2_postls171.root'
#    )

)


process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('260627:97-260627:1910')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_tracks.root')
)


process.d = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("generalTracks"),
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),     
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
)

process.p = cms.Path(process.hltfilter*process.d)
#process.p = cms.Path(process.d)



