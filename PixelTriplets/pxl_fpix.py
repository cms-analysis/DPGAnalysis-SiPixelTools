import FWCore.ParameterSet.Config as cms

process = cms.Process("DQ")

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

#process.GlobalTag.globaltag = "MCRUN2_73_V7::All"
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = "PRE_R_72_V11A"

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
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

# Files to run on
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
        # "/store/data/Run2012D/MinimumBias/RECO/PromptReco-v1/000/208/686/F60495B3-1E41-E211-BB7C-003048D3756A.root",                             # my re-reco                                                                                                                               #    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_0.root',                                                              #    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_1.root',                                                              #    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_2.root',                                                              #    'file:/afs/cern.ch/work/d/dkotlins/public/data/tracks/r208686_1_3.root',
        # mc                                                                                                                                       #    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100/tracks/tracks1.root'
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks2_mc71.root'
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks1_postls171.root', 
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks2_postls171.root',
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks3_postls171.root',
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks4_postls171.root',
        #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks5_postls171.root',
        ##'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks6_postls171.root',
        #'file:/afs/cern.ch/work/v/vlambert/public/Pixel/MC/tracks1_postls171.root',
        #'file:/afs/cern.ch/work/v/vlambert/public/Pixel/MC/tracks2_postls171.root'
        #'/store/relval/CMSSW_7_1_0_pre7/SingleMu/RECO/PRE_R_71_V2_RelVal_mu2012D-v1/00000/7052578E-B0D0-E311-9698-0026189437FD.root' #old data
        #'/store/data/Commissioning2015/Cosmics/RECO/PromptReco-v1/000/238/721/00000/728A34D5-6DD2-E411-B424-02163E0133D9.root'        
        '/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/777/24F0F702-110B-E211-99C9-001D09F248F8.root',
        '/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/777/440888DB-240B-E211-8398-003048D2BB58.root',
        '/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/778/3A8126C6-E10A-E211-8FEE-003048D2C01E.root',
        '/store/data/Run2012D/SingleMu/RECO/PromptReco-v1/000/203/780/AEDB1CB4-DE0A-E211-A08F-5404A6388699.root',
        )
                            )


useLocalDB = True
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



process.fpix = cms.EDAnalyzer('PxlFPix',
                              triggerSource = cms.InputTag(''), #for MC
                              ttrhBuilder = cms.string('WithTrackAngle'), #generic no template
)

process.TFileService = cms.Service('TFileService',
                                 fileName = cms.string('/afs/cern.ch/work/v/vlambert/fpixjob_newtag3.root'),
                                 )

process.p = cms.Path(process.offlineBeamSpot*process.fpix)
