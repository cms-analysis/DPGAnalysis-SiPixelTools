import FWCore.ParameterSet.Config as cms

process = cms.Process("DQ")

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.Geometry.GeometryDB_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
process.GlobalTag= GlobalTag(process.GlobalTag,'76X_dataRun2_v15', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")

process.MessageLogger = cms.Service("MessageLogger",
                                    debugModules = cms.untracked.vstring('siPixelRecHits'),
                                    destinations = cms.untracked.vstring('cout'),
                                    cout = cms.untracked.PSet( threshold = cms.untracked.string('WARNING')
                                                               )
                                    )

# WithTrackAngle - PixelCPEGeneric                                                                    
process.load('RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi')

# WithAngleAndTemplate - PixelCPETemplateReco 
process.load('RecoTracker.TransientTrackingRecHit.TTRHBuilderWithTemplate_cfi')

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

# Number of events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

# Files to run on
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
        #Data
        #'/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/777/24F0F702-110B-E211-99C9-001D09F248F8.root',
        #'/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/777/440888DB-240B-E211-8398-003048D2BB58.root',
        #'/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/778/3A8126C6-E10A-E211-8FEE-003048D2C01E.root',
        #'/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/780/AEDB1CB4-DE0A-E211-A08F-5404A6388699.root',
        #MC
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_73/tracks/tracks1_mc73_13.root',
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_73/tracks/tracks2_mc73_13.root',
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_73/tracks/tracks3_mc73_13.root',
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_73/tracks/tracks4_mc73_13.root',
        
        #Data 2015
        "/store/data/Run2015C/DoubleMuon/RECO/PromptReco-v1/000/254/905/00000/0E69BA5C-B24B-E511-BEF4-02163E01478C.root"
        )
                            )


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
                # GenError                                                                                                              
                #record = cms.string('SiPixelGenErrorDBObjectRcd'),
                #tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
                record = cms.string('SiPixelLorentzAngleRcd'),
                tag = cms.string('SiPixelLorentzAngle_2015_v1')
                ),
            ),
                                            connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
                                            #connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
                                            ) # end process                                                                                                     
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# endif   



process.Histos = cms.EDAnalyzer('Pixel',
                             triggerSource = cms.InputTag(''), #for MC
                             ttrhBuilder = cms.string('WithTrackAngle'), #generic no template
                             doBPix = cms.bool(True),    # boolean to run BPIX residuals
                             doFPix = cms.bool(True),    # boolean to run FPIX residuals
)

process.TFileService = cms.Service('TFileService',
                                 fileName = cms.string('Pixeljob.root'),
                                 )

process.p = cms.Path(process.offlineBeamSpot*process.Histos)
