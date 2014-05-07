
import FWCore.ParameterSet.Config as cms

process = cms.Process("DQ")

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'START53_V27::All'
#process.GlobalTag.globaltag = 'FT_R_53_V21::All'
# for data in V7
#process.GlobalTag.globaltag = "GR_R_62_V1::All"
# for simulations 
#process.GlobalTag.globaltag = "MC_70_V1::All"
#process.GlobalTag.globaltag = "START70_V1::All"
#process.GlobalTag.globaltag = "PRE_MC_71_V2::All"
#process.GlobalTag.globaltag = "MC_71_V1::All"
#process.GlobalTag.globaltag = "START71_V1::All"
#process.GlobalTag.globaltag = "POSTLS170_V5::All"
#process.GlobalTag.globaltag = "POSTLS171_V1::All"
process.GlobalTag.globaltag = "PRE_LS171_V3::All"

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
# process.load("Configuration.StandardSequences.MagneticField_38T_cff")


process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelRecHits'),
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



# WithTrackAngle - PixelCPEGeneric
process.load('RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi')
# WithAngleAndTemplate - PixelCPETemplateReco
process.load('RecoTracker.TransientTrackingRecHit.TTRHBuilderWithTemplate_cfi')
# WithoutRefit - Fake (does not work)
#process.load('RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi')

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
# Get beamspot from DB
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

#from CalibTracker.Configuration.Common.PoolDBESSource_cfi import poolDBESSource
#import CalibTracker.Configuration.Common.PoolDBESSource_cfi

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
# "/store/data/Run2012D/MinimumBias/RECO/PromptReco-v1/000/208/686/F60495B3-1E41-E211-BB7C-003048D3756A.root",
# my re-reco 
#    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_0.root',
#    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_1.root',
#    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_2.root',
#    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_3.root',

# mc
    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/tracks/tracks1.root'

	)
)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('208686:73-208686:463')

# DB stuff 
useLocalDB = False
if useLocalDB :
# Frontier LA 
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 		cms.PSet(
 			record = cms.string("SiPixelLorentzAngleRcd"),
 			label = cms.untracked.string("fromAlignment"),
# 			label = cms.untracked.string("forWidth"),
# 			tag = cms.string("SiPixelLorentzAngle_v02_mc")
 			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v0_mc")
# 			tag = cms.string("SiPixelLorentzAngle_forWidth_v0_mc")
 		),
# 		cms.PSet(
# 			record = cms.string("SiPixelLorentzAngleSimRcd"),
# 			tag = cms.string("test_LorentzAngle_Sim")
# 		)
 	),
     connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
    ) # end process
# SQ_LITE GenError
    process.DBReaderFrontier2 = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 		cms.PSet(
 			record = cms.string("SiPixelGenErrorDBObjectRcd"),
# 			label = cms.untracked.string("fromAlignment"),
# 			tag = cms.string("SiPixelGenErrorDBObject38Tv1")
 			tag = cms.string("SiPixelGenErrorDBObject38TV10")
 		),
 	),
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db_old')
     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
   ) # end process
# endif
 
#process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
#process.myprefer2 = cms.ESPrefer("PoolDBESSource","DBReaderFrontier2")

process.Histos = cms.EDAnalyzer('Pxl',
# for official RECO
#	triggerSource = cms.InputTag('TriggerResults::HLT'),
# For MC or my rereco
	triggerSource = cms.InputTag(''),
# Using templates   
#	ttrhBuilder = cms.string('WithAngleAndTemplate'),
# Using generic 
	ttrhBuilder = cms.string('WithTrackAngle'),
        singleParticleMC = cms.untracked.bool(True),
)

# values for LA, if 0 it is taken from DB 
#process.PixelCPEGenericESProducer.lAOffset = cms.untracked.double(0.098)
#process.PixelCPEGenericESProducer.lAOffset = cms.untracked.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.untracked.double(0.098)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.untracked.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.untracked.double(0.058)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.untracked.double(0.0)

#process.PixelCPEGenericESProducer.useLAAlignmentOffsets = cms.bool(True)
#process.PixelCPEGenericESProducer.useLAWidthFromDB = cms.bool(True)


# use the LA correction from alignment in templates 
#process.templates.DoLorentz = cms.bool(True)

process.TFileService = cms.Service('TFileService',
	fileName = cms.string('job.root'),
)

process.p = cms.Path(process.offlineBeamSpot*process.Histos)
