# produce pixel cluster & rechits from digia
# 
# To go from pixel clusters to rechits
#
##############################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("RecHitTest")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
# select one of these
#process.load("Configuration.Geometry.GeometryIdeal_cff")     # works for MC
#process.load("Configuration.StandardSequences.Geometry_cff") # works with MC & data
process.load("Configuration.StandardSequences.GeometryRecoDB_cff") # works for MC & data

process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# clusterizer & rechits 
#process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# or use this
process.load("Configuration.StandardSequences.Reconstruction_cff")


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
        threshold = cms.untracked.string('WARNING')
#       threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)
# get the files from DBS:
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
# 'file:tracks.root'
# 'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/tracks/tracks2_mc71.root'
# 'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_mc71.root'

# '/store/relval/CMSSW_7_3_3/RelValMinBias_13/GEN-SIM-RECO/MCRUN2_73_V11-v1/00000/28F9E4CD-E0C3-E411-9656-0025905A6082.root'

# "/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/238/721/00000/005ABD91-D6D0-E411-A46C-02163E01241E.root",

 "/store/data/Run2012C/MinimumBias/RECO/22Jan2013-v1/20020/2C3E2BB3-4173-E211-8417-003048F0117C.root",

# "/store/data/Run2012D/MinimumBias/RECO/PromptReco-v1/000/208/686/F60495B3-1E41-E211-BB7C-003048D3756A.root",
##"/store/data/Run2012D/MinimumBias/RECO/PromptReco-v1/000/208/686/F2BA6B22-2C41-E211-9D7A-003048D2BED6.root",
## "/store/data/Run2012D/MinimumBias/RECO/PromptReco-v1/000/208/686/E4E6B318-2041-E211-B351-001D09F29114.root",

  )
)

# a service to use root histos
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
)

# Choose the global tag here:
# old 70&71 MC tags
#process.GlobalTag.globaltag = "MC_70_V1::All"
#process.GlobalTag.globaltag = "START70_V1::All"
#process.GlobalTag.globaltag = "POSTLS170_V5::All"
#process.GlobalTag.globaltag = "START71_V1::All"
#process.GlobalTag.globaltag = "MC_71_V1::All"
#process.GlobalTag.globaltag = "POSTLS171_V1::All"
#process.GlobalTag.globaltag = "PRE_MC_71_V2::All"
# for 73X
#process.GlobalTag.globaltag = "MCRUN1_73_V5::All" # OK for condDB

# for data 2012
process.GlobalTag.globaltag = "GR_R_73_V2A::All"

# for data 2015
#process.GlobalTag.globaltag = "GR_R_73_V3A::All"

# DB stuff 
# GenError
useLocalDB = False
if useLocalDB :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
 	 ),
 	),
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# endif

useLocalDB2 = False
if useLocalDB2 :
    process.DBReaderFrontier2 = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 		cms.PSet(
 			record = cms.string("SiPixelTemplateDBObjectRcd"),
 			tag = cms.string("SiPixelTemplateDBObject38TV10")
# 			tag = cms.string("SiPixelTemplateDBObject38Tv21")
 		),
 	),
     connect = cms.string('sqlite_file:siPixelTemplates38T.db')
#     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
    ) # end process
    process.myprefer2 = cms.ESPrefer("PoolDBESSource","DBReaderFrontier2")
# endif
 
#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
#    DBParameters = cms.PSet(
#        messageLevel = cms.untracked.int32(0),
#        authenticationPath = cms.untracked.string('')
#    ),
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('SiPixelQualityRcd'),
#        tag = cms.string('SiPixelBadModule_test')
#    )),
#    connect = cms.string('sqlite_file:test.db')
#)
# To use a test DB instead of the official pixel object DB tag: 
#process.customDead = cms.ESSource("PoolDBESSource", process.CondDBSetup, connect = cms.string('sqlite_file:/afs/cern.ch/user/v/vesna/Digitizer/dead_20100901.db'), toGet = cms.VPSet(cms.PSet(record = cms.string('SiPixelQualityRcd'), tag = cms.string('dead_20100901'))))
#process.es_prefer_customDead = cms.ESPrefer("PoolDBESSource","customDead")


process.o1 = cms.OutputModule("PoolOutputModule",
                              outputCommands = cms.untracked.vstring('drop *','keep *_*_*_RecHitTest'),
            fileName = cms.untracked.string('file:rechits.root')
#            fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_mc71.root')
)

# My 
# modify clusterie parameters (just an example)
#process.siPixelClusters.ClusterThreshold = 4000.0

# read rechits
process.analysis = cms.EDAnalyzer("ReadPixelRecHit",
    Verbosity = cms.untracked.bool(True),
    src = cms.InputTag("siPixelRecHits"),
)

# only rechits
#process.p1 = cms.Path(process.siPixelRecHits)
process.p1 = cms.Path(process.siPixelRecHits*process.analysis)

# save output 
#process.outpath = cms.EndPath(process.o1)
