
import FWCore.ParameterSet.Config as cms

process = cms.Process("DQ")

#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
# process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
# tags for 74X
#process.GlobalTag.globaltag = 'FT_R_74_V15B' # for 251643
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v3' # 
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v2' # 
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v1' # only for 746
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v0' # for 251643
#process.GlobalTag.globaltag = '74X_dataRun2_PromptValidation_forPostTS2_v2' # for 251643
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'START53_V27::All'
#process.GlobalTag.globaltag = 'FT_R_53_V21::All'
# for data in V7
#process.GlobalTag.globaltag = "GR_R_62_V1::All"
# for simulations 
#process.GlobalTag.globaltag = "MC_70_V1::All"
#process.GlobalTag.globaltag = "START70_V1::All"
#process.GlobalTag.globaltag = "MC_71_V1::All"
#process.GlobalTag.globaltag = "MCRUN2_73_V7::All"

#process.GlobalTag.toGet = cms.VPSet(
#       cms.PSet(record = cms.string("TrackerAlignmentRcd"),
#                tag = cms.string("testTag"),
#                connect = cms.untracked.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRAC#KERALIGN/PayLoads/2015-07-15_PseudoPCLupdate/TkAlignment.db')
#               )
#      )

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


process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelRecHits'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#        threshold = cms.untracked.string('INFO')
        threshold = cms.untracked.string('WARNING')
#        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)


process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
# WithTrackAngle - PixelCPEGeneric
process.load('RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi')
# WithAngleAndTemplate - PixelCPETemplateReco
process.load('RecoTracker.TransientTrackingRecHit.TTRHBuilderWithTemplate_cfi')
# WithoutRefit - Fake (does not work)
#process.load('RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi')

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
# Get beamspot from DB
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))

myfilelist = cms.untracked.vstring()
myfilelist.extend([
#"/store/express/Run2015D/StreamExpress/ALCAPROMPT/PromptCalibProd-Express-v3/000/256/729/00000/0001F604-CE5D-E511-98EB-02163E0135A2.root",
])

process.source = cms.Source("PoolSource",
# fileNames =  myfilelist
	fileNames = cms.untracked.vstring(
"/store/data/Run2015C/DoubleMuon/RECO/PromptReco-v1/000/254/905/00000/0E69BA5C-B24B-E511-BEF4-02163E01478C.root",

#"/store/data/Run2012D/MinimumBias/RECO/PromptReco-v1/000/208/686/F60495B3-1E41-E211-BB7C-003048D3756A.root",
# my re-reco 
#    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_0.root',
# mc
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_73/tracks/tracks1_mc73_13.root'
	)
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('208686:73-208686:463')

# DB stuff 
useLocalDB = False
if useLocalDB :
# Frontier LA , GenErrors
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
# GenError
          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
#          tag = cms.string('SiPixelGenErrorDBObject38T_v0_mc1')
#          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
#          tag = cms.string('SiPixelGenErrorDBObject_38T_2015_v3_hltvalidation')
          tag = cms.string('SiPixelGenErrorDBObject0Tv3')
# LA
# 	   record = cms.string("SiPixelLorentzAngleRcd"),
# 	   label = cms.untracked.string("fromAlignment"),
# 	   label = cms.untracked.string("forWidth"),
# 	   tag = cms.string("SiPixelLorentzAngle_v02_mc")
# 	   tag = cms.string("SiPixelLorentzAngle_fromAlignment_v0_mc")
# 	   tag = cms.string("SiPixelLorentzAngle_forWidth_v0_mc")
#  	   tag = cms.string("SiPixelLorentzAngle_2015_v4")
#  	   tag = cms.string("SiPixelLorentzAngle_2015_v3_hltvalidation")
 	 ),
# 	 cms.PSet(
# 			record = cms.string("SiPixelLorentzAngleSimRcd"),
# 			tag = cms.string("test_LorentzAngle_Sim")
# 		 )
 	),
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/210915/SiPixelLorentzAngle_2015_v4.db')
     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/210915/SiPixelGenErrorDBObject_38T_2015_v4.db')
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
#     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# endif

# SQ_LITE Templates 
useLocalDBTemplate = True
if useLocalDBTemplate :
    process.DBReaderFrontier3 = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 		cms.PSet(
 			record = cms.string("SiPixelTemplateDBObjectRcd"),
# 			label = cms.untracked.string("fromAlignment"),
# 			tag = cms.string("SiPixelGenErrorDBObject38Tv1")
# 			tag = cms.string("SiPixelTemplateDBObject38TV10")
# 			tag = cms.string("SiPixelTemplateDBObject38Tv21")
# 			tag = cms.string("SiPixelTemplateDBObject_38T_2015_v3_hltvalidation")
# 			tag = cms.string("SiPixelTemplateDBObject_38T_2015_v4")
 			tag = cms.string("SiPixelTemplateDBObject38Tv3")
 		),
 	),
#     connect = cms.string('sqlite_file:siPixelTemplates38T.db')
#     connect = cms.string('sqlite_file:siPixelTemplates38T_2012_IOV7_v21.db')
     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/210915/SiPixelTemplateDBObject_38T_2015_v4.db')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
     ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier3")
# endif
 

process.Histos = cms.EDAnalyzer('PxlBPix',
# for official RECO
	triggerSource = cms.InputTag('TriggerResults::HLT'),
# For MC or my rereco
#	triggerSource = cms.InputTag(''),
# Using templates   
	ttrhBuilder = cms.string('WithAngleAndTemplate'),
# Using generic 
#	ttrhBuilder = cms.string('WithTrackAngle'),
#        singleParticleMC = cms.untracked.bool(True),
)

# values for LA, if 0 it is taken from DB 
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.098)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.09)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.double(0.098)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.double(0.058)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.double(0.0)

#process.PixelCPEGenericESProducer.useLAAlignmentOffsets = cms.bool(True)
#process.PixelCPEGenericESProducer.useLAWidthFromDB = cms.bool(True)

# use the LA correction from alignment in templates 
#process.templates.DoLorentz = cms.bool(True)

process.TFileService = cms.Service('TFileService',
	fileName = cms.string('job.root'),
)

#process.p = cms.Path(process.hltfilter*process.offlineBeamSpot*process.Histos)
process.p = cms.Path(process.offlineBeamSpot*process.Histos)
