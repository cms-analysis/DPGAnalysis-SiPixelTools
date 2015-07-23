#
# Last update: new version for python
#
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("cluTest")
                   
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")


#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# 2015
#process.GlobalTag.globaltag = 'GR_E_V48'
#process.GlobalTag.globaltag = 'GR_P_V56' # works for 2469763
process.GlobalTag.globaltag = 'GR_P_V56' # for 247607
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
#    HLTPaths = ['HLT_Physics_v*'],
    HLTPaths = ['HLT_Physics_part*'],
#    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_L1SingleMuOpen_v*'],
#    HLTPaths = ['HLT_ZeroBias_*'],
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

# to select PhysicsBit
#process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
#process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

# i do not know what is this doing?
#triggerSelection = cms.EDFilter( "TriggerResultsFilter",
#    triggerConditions = cms.vstring(
#    'HLT_ZeroBias / 1' ),
#    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
#    l1tResults = cms.InputTag( "gtDigis" ),
#    l1tIgnoreMask = cms.bool( True ),
#    l1techIgnorePrescales = cms.bool( True ),
#    daqPartitions = cms.uint32( 1 ),
#    throw = cms.bool( True )
#)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelClusters'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

myfilelist = cms.untracked.vstring()
myfilelist.extend([
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/002557B6-570C-E511-A2F3-02163E0119F7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/007003F9-FA0B-E511-9081-02163E01184D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/007E331C-080C-E511-B07B-02163E011C5E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/008D5EE0-270C-E511-B891-02163E01392A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0090B513-090C-E511-9CC6-02163E01438B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/00CDAAD0-E70B-E511-B556-02163E01467A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/00D16F42-0C0C-E511-8E86-02163E01429D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/00F0A4AC-0C0C-E511-B198-02163E01379B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/02251459-050C-E511-9105-02163E014338.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/025E98CF-990C-E511-A794-02163E0134F5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0464ADD7-060C-E511-B610-02163E014303.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/04F13EAC-040C-E511-AD8D-02163E014384.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0649E164-270C-E511-8B12-02163E011A9D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/066F9E45-1D0C-E511-AC77-02163E01453C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0673A1C4-870C-E511-89F0-02163E014100.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/068D1E4C-A50C-E511-A3C9-02163E0142E6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/08073EB5-050C-E511-A7E9-02163E013656.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/080DEB45-F90B-E511-808B-02163E01474E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/085212A6-FF0B-E511-A087-02163E014482.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/08A4BE12-080C-E511-B36A-02163E01257B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0A03497B-770C-E511-B997-02163E0139A3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0A0B0CA3-EE0B-E511-A763-02163E01475D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0A1A9924-ED0D-E511-BC19-02163E01470D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0A62A3B1-000C-E511-BCED-02163E013604.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0C40FCCF-030C-E511-A076-02163E012925.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0E291BDD-0F0C-E511-ABEA-02163E01369D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/0EFCE18D-1A0C-E511-AA5F-02163E01257B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/101E761D-0F0C-E511-A95A-02163E0146D0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1022BACB-040C-E511-B18A-02163E014695.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1072E36D-020C-E511-A3A9-02163E011A48.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1091411D-030C-E511-9383-02163E0138C6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/10C01C08-1D0C-E511-A7B2-02163E0142BB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/10F06206-E50B-E511-B3C0-02163E014458.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/12822C57-360C-E511-A16D-02163E01287B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/12828C2B-110C-E511-AE5F-02163E0137E3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/12AC19A7-0B0C-E511-BA26-02163E012325.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/12D18B71-040C-E511-A82A-02163E0136D7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/12E63FDA-E60B-E511-9D42-02163E011D4A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/12F2194A-FA0B-E511-BAFA-02163E013985.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/14166562-4D0C-E511-95CC-02163E012808.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1440ACFC-FB0B-E511-8F8B-02163E0125CE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1850F3D3-500C-E511-8454-02163E011E08.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/18C143B8-0A0C-E511-A4DF-02163E012028.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1AB61B2C-040C-E511-892C-02163E012BE0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1AC99D54-140C-E511-982E-02163E0119DC.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1C22B095-620F-E511-B5A3-02163E011DC2.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1C41C92D-310C-E511-8061-02163E0136C3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1C6BF161-280C-E511-B057-02163E014467.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1CAC0FDE-E80B-E511-8255-02163E011DBC.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1EB637C2-0C0C-E511-BECD-02163E01446F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1EE5DBC9-180C-E511-AC3D-02163E0142B3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/1EF24308-0B0C-E511-8943-02163E0126CB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2051A3CC-170C-E511-AA19-02163E0142F0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/20B45C0E-0F0C-E511-98F0-02163E012925.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/20D71277-1D0C-E511-9CD4-02163E011B82.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/22187459-230C-E511-B4FA-02163E01429D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/22298B11-1B0C-E511-ACC4-02163E012028.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/22394422-0A0C-E511-B754-02163E014472.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2283112F-F80D-E511-AE69-02163E01392A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/22EB6F95-040C-E511-B55A-02163E012795.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2434BC06-ED0B-E511-AA96-02163E01199C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/243DF664-050C-E511-9DF9-02163E0125CE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/24EFDBAB-EA0B-E511-9587-02163E011C14.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/24F55BBA-3C0C-E511-86EC-02163E011C14.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/268F6802-ED0B-E511-9D63-02163E0138BC.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/26DD6056-200C-E511-8F2D-02163E0139A3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/26E5517A-080C-E511-9C28-02163E01344C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/28AE01F3-460C-E511-8792-02163E01184D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2A321A5E-880C-E511-8FA6-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2A3DCACA-1F0C-E511-ADCA-02163E01432F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2A75F412-F60D-E511-932C-02163E0119A5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2AB5093C-3D0C-E511-A7BA-02163E012028.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2AD1F496-EF0B-E511-9AEB-02163E01429C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2C40D2F7-8F0C-E511-ABCC-02163E013556.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2C938472-E80B-E511-96C6-02163E0145D2.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2E09FE5F-9A0C-E511-AA29-02163E0138F6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2E3A45AF-050C-E511-A2D4-02163E011865.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2E519C4D-2B0C-E511-B8B3-02163E011B55.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/2EBD8409-EE0B-E511-9BA7-02163E011C5E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3078B168-090C-E511-A43D-02163E01474E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/32017A84-170C-E511-8B7A-02163E013549.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3294C9E9-F00D-E511-9504-02163E0146F6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/32D7C202-0C0C-E511-A03C-02163E012124.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3460C1C5-E70B-E511-AF4D-02163E0143CB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/346F1DFF-BA0C-E511-9A9B-02163E01462A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3487AA37-150C-E511-9FBA-02163E0144DC.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/36360FEF-100C-E511-BFEB-02163E0142B3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3648B223-220C-E511-9678-02163E01184D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/36659347-000C-E511-B0CE-02163E0134DE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/36751BCD-FA0C-E511-BF9E-02163E011D25.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/388E1648-ED0B-E511-BA3B-02163E011911.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3A51B7CD-060C-E511-B3DC-02163E01429D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3A6ABE9D-FD0B-E511-A0C1-02163E014216.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3AA70F15-FC0C-E511-8782-02163E0125F6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3C035D1C-4F0C-E511-AAC6-02163E013653.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/3CA528C0-100C-E511-9750-02163E0146AD.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/40638756-1D0C-E511-8DD7-02163E014349.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/408284D6-210C-E511-9820-02163E0146AD.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/40A423F0-320C-E511-A051-02163E01379B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/40B192A2-E50C-E511-936D-02163E0144FD.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/40BF26CE-FA0B-E511-98D6-02163E01437C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4254524E-030C-E511-8160-02163E01380F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/427BBB07-230C-E511-8684-02163E01383E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/42CF3C09-1F0C-E511-9CA0-02163E012BAA.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/440D7D69-2A0C-E511-97A9-02163E01184D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/44604FBD-240C-E511-8FA7-02163E011C5E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/44B238EF-2A0C-E511-9472-02163E014216.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/46418560-0E0C-E511-AB8C-02163E01428C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/466328C6-FC0B-E511-A63B-02163E0142E4.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/46F5EA8F-420C-E511-8982-02163E01257B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/489CFA25-EF0B-E511-BD6C-02163E01180E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4C680172-2B0C-E511-9629-02163E013549.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4C83F057-090C-E511-8D2D-02163E011E07.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4C974CED-080C-E511-B960-02163E0138F2.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4CBE1259-550C-E511-B36A-02163E0119F7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4CF048BA-F00B-E511-8AF0-02163E0145C8.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4E545054-160C-E511-82E5-02163E012028.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4E678FBD-E70B-E511-9857-02163E011B6D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4E91C6C6-180C-E511-9252-02163E012808.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4EF3E8D3-FF0B-E511-8190-02163E013617.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/4EFC4CC2-120C-E511-9A0D-02163E01445E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/50EDF18D-090C-E511-9EFA-02163E014146.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/522194A7-EA0B-E511-BB53-02163E0136C3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5248D67A-170C-E511-B4CD-02163E014338.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/525626D1-3F0C-E511-BC05-02163E0138F3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/527194AE-1F0C-E511-829C-02163E011B82.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/52C86438-320C-E511-90B3-02163E013768.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/52DA494F-130E-E511-889D-02163E014496.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/52EC6A4D-340C-E511-88E2-02163E0144F1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/542E2D03-140C-E511-AC23-02163E01383E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/54809EB6-E80B-E511-9D26-02163E0142E4.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/548A1B8F-3D0C-E511-B8AE-02163E012808.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/54CF2D7F-820C-E511-8552-02163E014300.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5626DA04-520C-E511-937D-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/562D9C8C-F20B-E511-A921-02163E014169.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/56A4D106-6C0C-E511-9741-02163E01432C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/56B395E4-0B0C-E511-95BA-02163E0142F0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/56FF48A7-FE0B-E511-8109-02163E011D4A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/589F8ED0-420C-E511-806D-02163E0141F1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/58F8F443-3D0C-E511-B9E0-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5A8F6ADA-380C-E511-8F1B-02163E0143EB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5C23AAD2-330C-E511-8212-02163E012808.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5C38AF83-130C-E511-BDEE-02163E011C5E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5C45907A-E80B-E511-AF1A-02163E01184D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5C8DD2EF-100C-E511-BCDA-02163E011B82.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5E276293-230C-E511-A9FC-02163E011E07.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/5E3FC6D9-010C-E511-B8BD-02163E013653.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6060F6C2-0B0C-E511-9212-02163E01287B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6082B42C-EF0B-E511-8274-02163E0127ED.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6097D6D7-0C0C-E511-B807-02163E0144F5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6202A0A1-FD0B-E511-AE05-02163E0137E1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/620F8DEB-F00B-E511-AEE9-02163E0146D0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6239D172-3C0C-E511-8B67-02163E012069.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/62692064-710C-E511-8ACE-02163E0136A7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/62B476AA-E90B-E511-BA00-02163E0142F3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6456981A-0E0C-E511-90D3-02163E0145C5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/681DA3F5-EE0B-E511-999C-02163E011865.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/682027B1-F70B-E511-968F-02163E01365A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/684AF935-0D0C-E511-B56B-02163E014467.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6857D69A-E60B-E511-ACF3-02163E0138F3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/68C25863-FE0B-E511-9131-02163E014467.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6A41032C-190C-E511-988E-02163E014122.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6AC6302B-670C-E511-8205-02163E0140FA.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6AF3C233-810C-E511-9FE1-02163E014231.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6C93E80A-ED0B-E511-A128-02163E0142B3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6E57D0B7-0A0C-E511-B7D6-02163E014303.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6E88A2D4-210C-E511-96E6-02163E014338.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/6EA9EEEF-FD0B-E511-8FFC-02163E0143EB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7034C575-EC0B-E511-BC84-02163E01369F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/72114B38-0B0C-E511-B211-02163E0144F1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/72530E88-0B0C-E511-AC41-02163E014467.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/729E73B3-0C0C-E511-BB28-02163E0141D2.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/72C4667A-220C-E511-8266-02163E014218.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/743E8A91-EE0B-E511-8124-02163E014658.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/74D93098-7B0E-E511-8AE1-02163E01453F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/786B53A1-EE0B-E511-9BEF-02163E0142D7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/789EA5E0-3D0C-E511-84AA-02163E0144F1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/78F98E5D-090C-E511-A45D-02163E012914.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7A062CAC-220C-E511-B419-02163E0134D5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7A088438-F10B-E511-91BB-02163E012914.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7ABD14C5-410C-E511-B4F5-02163E013653.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7AFD0B58-8B0C-E511-AA58-02163E014100.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7C493DFC-EA0B-E511-B145-02163E0146A5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7C5A39E8-280C-E511-A00B-02163E0136C3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7C6E2B00-FC0B-E511-95B2-02163E01287B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7C818369-F00B-E511-AE1E-02163E01430E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7CDA067E-130C-E511-911B-02163E014695.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7E10485E-050C-E511-ADB7-02163E014303.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/7EAB2E37-0A0C-E511-A2EA-02163E011CEE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/801A2E12-050C-E511-BEE7-02163E014402.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/803C6B55-350C-E511-BD5B-02163E01257B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8203E10E-1F0C-E511-8B30-02163E011BDB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/82170232-0D0C-E511-B0DB-02163E01184D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/844CDD13-230C-E511-A750-02163E01287B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/84BB4569-1E0C-E511-BF34-02163E0121C5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/84EAD320-0E0C-E511-A501-02163E01369F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/861A3644-FC0B-E511-A987-02163E0144A3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8663CB2D-E90B-E511-8C74-02163E014467.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8673B0CF-100C-E511-A0DA-02163E0138F3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/868138D5-060C-E511-AE2C-02163E013913.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/86C2ACCE-3E0C-E511-BA6A-02163E012376.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8827A2DE-040C-E511-BC21-02163E013653.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/884E4227-E60B-E511-A73E-02163E011D4A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/88DDAAFE-070C-E511-947B-02163E01474E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8A1D4632-060C-E511-85CE-02163E0146D0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8A4040F8-FF0B-E511-851E-02163E011AF5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8A515DBA-460C-E511-B2AA-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8AAF3A3B-2B0C-E511-95F8-02163E014467.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8AD3438E-0A0C-E511-BE1B-02163E014482.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8C85ABE1-FF0B-E511-A9A6-02163E014111.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8C88FC57-4E0C-E511-8C72-02163E014374.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8CB7D2FD-340C-E511-8724-02163E011CFB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8E1DCE16-270C-E511-8D0E-02163E01383E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/8EF68509-FE0B-E511-9724-02163E014349.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/90ABC4D9-050C-E511-AB32-02163E01184D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/90C6CB3A-190C-E511-9AC8-02163E0146AD.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/90CCA54B-320C-E511-9855-02163E011CCC.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/90DFC289-280C-E511-B9B7-02163E0145C5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/92C5E6BD-F70B-E511-A47C-02163E01444C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/92E04C62-7C0C-E511-AAB8-02163E0136A7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/92F60D06-070C-E511-A981-02163E0137E3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/94263EA8-E90B-E511-8AC7-02163E011C1F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/94317632-100E-E511-8A8E-02163E0144F5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/94D79490-080C-E511-A586-02163E0126EB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/94DDC230-070C-E511-8C61-02163E011CFE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/967137B2-0C0C-E511-8164-02163E014146.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/96778275-E90B-E511-B43D-02163E0145D0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/967A78FE-070C-E511-8D72-02163E01429D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/96C178D5-FD0B-E511-AEB1-02163E014695.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/98031923-120C-E511-83B7-02163E014122.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/982F4EC4-0A0C-E511-8011-02163E0142F0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/983D6E5E-420C-E511-BFCA-02163E011C14.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9A57A8B0-120C-E511-9199-02163E014609.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9A5EDDF7-5C0C-E511-9752-02163E012556.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9AA40F14-9B0C-E511-B228-02163E0136A7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9AAA2686-1B0C-E511-98C8-02163E0121DA.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9ABA6EE0-EB0B-E511-91D1-02163E014146.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9AC1C02B-330C-E511-8A64-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9ACB256B-FA0E-E511-802E-02163E014736.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9AE10DD3-140C-E511-8EDC-02163E01392A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9E261D55-EF0B-E511-B303-02163E0119D0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9E29AC85-270C-E511-8E50-02163E0143AB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9E3F67B0-080C-E511-A5F0-02163E0134D5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9E439EB4-190C-E511-9490-02163E01392A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/9EE30EEE-100C-E511-B427-02163E0138BC.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A01C7B7D-0C0C-E511-9019-02163E011CFE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A0B704AE-0C0C-E511-A995-02163E014220.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A0C8639F-2D0C-E511-A8A5-02163E012AC1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A201CE15-210C-E511-AFFD-02163E01453C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A2030682-2A0C-E511-A056-02163E0138F8.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A4C52E11-090C-E511-A0BF-02163E0138F6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A614B537-010C-E511-B6E4-02163E013549.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A695867A-E90B-E511-A5B9-02163E012912.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A6FA484B-140C-E511-8223-02163E0141CE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A834A991-090C-E511-8B01-02163E013913.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A84CE69C-FE0B-E511-BAF9-02163E01380F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/A89A2041-090C-E511-A418-02163E012069.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/AAB6BA08-0C0C-E511-A2F7-02163E01460C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/AAFD74E5-960C-E511-8D32-02163E0134D9.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/AE910EC1-070C-E511-94E9-02163E014299.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B0435C8E-290C-E511-BE1E-02163E014485.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B0AD42F5-080C-E511-8212-02163E0139DE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B0BB6674-0C0C-E511-A7B3-02163E011804.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B0D64384-EB0B-E511-BCE2-02163E01220A.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B0EDECF0-FF0B-E511-9B73-02163E013604.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B2449B67-2C0C-E511-BC23-02163E011E08.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B4F0471C-3B0C-E511-B191-02163E014218.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B68535AF-190C-E511-95DE-02163E013617.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B6D9A13F-A00C-E511-A721-02163E013518.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B80E9A9D-2C0C-E511-96A5-02163E011DC6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B857D709-FE0B-E511-9955-02163E01196F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/B88AAA82-0B0C-E511-8F41-02163E013826.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/BAB436C7-180C-E511-BAAB-02163E01458C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/BAD97396-000C-E511-BADC-02163E0139C0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/BC051E38-090C-E511-8F9B-02163E014485.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/BC9C7D37-950C-E511-83E3-02163E014485.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/BCD53BC3-0D0C-E511-8228-02163E0142F3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/BCD6E4D0-0A0C-E511-B7D4-02163E014698.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/BEBBDC31-480C-E511-A4D2-02163E0138F3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C02ED64C-140C-E511-8D46-02163E01379B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C0BCCADE-FF0B-E511-82F0-02163E014122.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C277DBD1-A70C-E511-8108-02163E0141D2.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C45C2805-FB0B-E511-A79F-02163E013729.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C4BAC0A4-0A0C-E511-BFE7-02163E014189.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C4E2547C-170C-E511-9999-02163E0146A9.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C628C717-080C-E511-A996-02163E01380F.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C6CCC95F-0F0C-E511-94B2-02163E01428C.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C81BBC84-E30B-E511-B03D-02163E0133EB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C82468CB-050C-E511-AC6C-02163E014338.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C87A400C-FB0B-E511-8041-02163E014485.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/C8899946-530C-E511-B57B-02163E0126EB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CA10FF7D-1E0C-E511-B72F-02163E01287B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CA36CE9B-2E0C-E511-99BF-02163E0121DA.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CA7977E0-3E0C-E511-83E3-02163E0126EB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CAED087A-220C-E511-9F09-02163E012432.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CC5BFB30-3B0C-E511-90A7-02163E012355.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CCBE7A6B-0A0C-E511-B9B4-02163E011CFE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CCEF40A4-020C-E511-A917-02163E011911.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CE1562AF-080C-E511-A566-02163E0126A4.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CE353AE1-0B0C-E511-8442-02163E011A9D.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CE8A9914-2E0C-E511-A1B1-02163E012028.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/CEAFB417-310C-E511-9D9E-02163E013653.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D09305CF-060C-E511-AA0C-02163E0143AD.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D097E87F-EC0B-E511-835A-02163E013604.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D400718F-960C-E511-8B9A-02163E0142E6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D4B26DD6-F10B-E511-8973-02163E014341.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D619BEAB-350C-E511-BEFB-02163E0144F1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D630219C-070C-E511-95C0-02163E0142E4.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D67169E8-B80C-E511-82AB-02163E013768.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D6B84DD2-0D0C-E511-AD1F-02163E014111.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D6FAF8FC-220C-E511-9604-02163E0121C5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/D82799A3-520C-E511-B916-02163E012376.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/DA295407-F90B-E511-9861-02163E0135CA.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/DA3A270A-940C-E511-B324-02163E014100.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/DACC34DC-4A0C-E511-B480-02163E011A3E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/DCDDF63E-0E0C-E511-87BB-02163E0138B3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/DCE12E7A-0F0C-E511-BFAE-02163E0139DE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/DE69F4EF-2B0C-E511-A923-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E2433B13-390C-E511-8EC0-02163E01379B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E40135FB-1E0C-E511-AFBE-02163E0119C6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E41587C6-420C-E511-AD10-02163E012069.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E4284EBC-590C-E511-8CA5-02163E011B55.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E44EB180-390C-E511-A796-02163E0141F1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E693E30B-120C-E511-97CD-02163E013774.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E6ECBBBB-0D0C-E511-A56B-02163E013985.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E8202E28-260C-E511-9450-02163E012028.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E82F0163-0E0C-E511-BDC0-02163E0137E1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E8910FF0-240C-E511-AD0D-02163E01379B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E8957E15-050C-E511-BC3A-02163E0143B6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/E8CDDAD4-BA0C-E511-8BDA-02163E01447E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/EA30C6C1-E70B-E511-B7C0-02163E0142EE.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/EA930018-080C-E511-B698-02163E013826.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/EA93D351-0F0C-E511-98E0-02163E014338.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/EC5FACC8-0A0C-E511-9A7C-02163E012AC1.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/EC966D66-380C-E511-8E16-02163E011CFB.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/EE45ED3A-1F0C-E511-A8A5-02163E013768.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F0FA74F1-2E0C-E511-A9CB-02163E0138F3.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F2D58B72-E70B-E511-903F-02163E0143B6.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F2DB42DA-990C-E511-B770-02163E013826.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F4AFD858-230C-E511-8C57-02163E012028.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F4C27170-E70B-E511-B8F0-02163E0140FA.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F4C5B120-F40B-E511-B08C-02163E0119DC.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F4F3E89B-EB0B-E511-9354-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F618D412-0D0C-E511-9495-02163E011911.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F67424DF-390C-E511-A936-02163E01257B.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F6D13A66-1C0C-E511-A475-02163E0142F0.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F6EE0D74-230C-E511-815A-02163E011C5E.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F8BCA358-1A0C-E511-B8DA-02163E0144E5.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/F8D7EDC5-F70B-E511-BA63-02163E0141D2.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/FC323C97-2D0C-E511-8BFD-02163E014338.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/FC8C9C87-020F-E511-A1C6-02163E0145B7.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/FCC8A1ED-AD0C-E511-A8D1-02163E012553.root",
"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/246/963/00000/FE5DF91F-240C-E511-B27F-02163E0146AD.root",
])


process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
  fileNames = cms.untracked.vstring(    
# muonia 
#"/store/data/Run2015A/MuOnia/RECO/PromptReco-v1/000/248/038/00000/22A90F7F-5A14-E511-AD96-02163E012BE3.root",
#single muon 
#"/store/data/Run2015A/SingleMuon/RECO/PromptReco-v1/000/248/038/00000/0C5FA27E-5A14-E511-8D85-02163E012385.root",
# charmonium
#"/store/data/Run2015A/Charmonium/RECO/PromptReco-v1/000/248/038/00000/FC099721-5A14-E511-A3D1-02163E012B9A.root",
#
#"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/248/025/00000/50B4C032-0314-E511-A718-02163E013569.root",
#"/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/248/025/00000/08590F92-0115-E511-9483-02163E011C17.root",
#"/store/data/Run2015A/ZeroBias/RECO/PromptReco-v1/000/248/025/00000/30A880F2-0414-E511-9E31-02163E0139A3.root",
"/store/data/Run2015A/ZeroBias1/AOD/PromptReco-v1/000/248/025/00000/341760D5-5114-E511-A92E-02163E0129E4.root",

#"/store/data/Run2015A/Commissioning/RECO/PromptReco-v1/000/247/607/00000/681AB913-9110-E511-98DA-02163E014374.root",
#"/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/246/963/00000/00F51FE4-090C-E511-8F33-02163E014299.root",
#"/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/963/00000/0006E9B1-4A0A-E511-AD47-02163E0138BC.root",

#  "file:/afs/cern.ch/work/d/dkotlins/public/data/clus/clus_zb_248025.root"
#  "file:../scripts/clus.root"
# MC
#"/store/relval/CMSSW_7_1_0_pre7/RelValTTbar/GEN-SIM-RECO/PRE_STA71_V3-v1/00000/887DEA5B-5CD1-E311-BB97-002618943923.root",
#"/store/relval/CMSSW_7_1_0_pre7/RelValTTbar/GEN-SIM-RECO/PRE_STA71_V3-v1/00000/8EEC0F1F-9FD1-E311-966F-003048FFD76E.root",
## 2012, cosmics
# "/store/data/Commissioning12/MinimumBias/RECO/PromptReco-v1/000/186/791/CEA46376-7069-E111-B395-001D09F24D67.root",
# "/store/data/Commissioning12/Commissioning/RECO/PromptReco-v1/000/186/791/6EC3470C-6F69-E111-93CA-001D09F241B9.root",
# "/store/data/Commissioning12/Cosmics/RECO/PromptReco-v1/000/186/791/6A54D2A0-6D69-E111-ABA8-001D09F2441B.root",

  )   # end the list "by-hand"
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124230:26-124230:9999','124030:2-124030:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('207469:0-207469:51')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('207477:76-207477:570')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('208686:73-208686:463')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('210534:24-210534:347')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('clus_ana.root')
)


process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

#process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)


# define an EndPath to analyze all other path results
#process.hltTrigReport = cms.EDAnalyzer( 'HLTrigReport',
#    HLTriggerResults = cms.InputTag( 'TriggerResults','','' )
#)
#process.HLTAnalyzerEndpath = cms.EndPath( process.hltTrigReport )
