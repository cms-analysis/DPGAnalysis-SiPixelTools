# ----------------------------------------------------------------------
# -- py template file for dumping the PixelTree in MC
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
def getDataset(filename):
    "Interface with the DBS API to get the dataset used"
    from xml.dom.minidom import parseString
    from DBSAPI.dbsApi import DbsApi
    args = {}
    args['url']='http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet'
    args['version']='DBS_2_0_9'
    args['mode']='POST'
    api = DbsApi(args)
    data = api.executeQuery("find dataset where file="+filename)
    domresults = parseString(data)
    dbs = domresults.getElementsByTagName('dbs')
    result = dbs[0].getElementsByTagName('results')
    rows=result[0].getElementsByTagName('row')
    dataset=(rows[0].getElementsByTagName('dataset'))[0] #rows should have only one element and produce a one element array
    node=(dataset.childNodes)[0] #childNodes should be a one element array
    return str(node.data) #The output is in unicode, so it has to be translated through str

# ----------------------------------------------------------------------
import os
import FWCore.ParameterSet.Config as cms
process = cms.Process("Demo")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "START311_V1::All"

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)

# -- Input files
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0091/067E5F2D-DC35-E011-9F5C-001A92971B68.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/D41F4D5C-BF34-E011-98D6-0018F3D09624.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/D0A87085-BB34-E011-B220-001A928116EC.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/70AA1B57-B634-E011-BB22-00261894391D.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/68E2AE56-B734-E011-A686-002618943947.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/62FAF9D6-C034-E011-9903-0026189438A0.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/4A518384-B934-E011-8A84-0018F3D0965C.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/48C592F2-B534-E011-AC54-001A928116D2.root',
    '/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-RECO/START311_V1_64bit-v1/0088/38746552-BC34-E011-A7B5-002618943907.root'
    ),
    secondaryFileNames=cms.untracked.vstring(
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/FC8C43E8-B634-E011-B3E0-0018F3D096A2.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/F8F79F53-B534-E011-BB0B-0018F3D095EC.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/EAB2B177-BE34-E011-A8C0-0018F3D09624.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/D6953F82-BB34-E011-B0E4-0018F3D096D8.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/D61B09D7-C134-E011-A064-001A928116EC.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/D007FCCE-B934-E011-9525-00248C0BE012.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/C82F9D51-BC34-E011-AC83-0018F3D096E0.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/B656B597-B734-E011-B03B-001A92971B92.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/AAA5C5EF-B634-E011-8452-00261894391D.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/AA2846E0-BD34-E011-8E35-002618943907.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/9838E1EF-BF34-E011-8038-001A92971AA8.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/8CF7DBF5-BB34-E011-8C22-002618943899.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/84F49851-B834-E011-815E-001A92971BB2.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/7EA76D76-C134-E011-97CE-001A92811730.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/4C9E3BEF-B534-E011-8D46-001A92971BB2.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/32EA42F1-BB34-E011-B2B1-001A92971AA4.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/2A3AFEDF-B534-E011-AF81-00261894396D.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/22F46586-B934-E011-98E4-001A92971B96.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/0A959F4C-B834-E011-94A5-002618FDA28E.root',
'/store/relval/CMSSW_3_11_1/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/START311_V1_64bit-v1/0088/00552EEC-BF34-E011-8744-001A92810A98.root'
    ) 
    )

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks'

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

# -- skimming
process.PixelFilter = cms.EDFilter(
    "SkimEvents",
    verbose                        = cms.untracked.int32(0),
    filterOnPrimaryVertex          = cms.untracked.int32(1),
    primaryVertexCollectionLabel   = cms.untracked.InputTag('offlinePrimaryVertices'),
    filterOnTracks                 = cms.untracked.int32(1),
    trackCollectionLabel           = cms.untracked.InputTag('generalTracks'),
    filterOnPixelCluster           = cms.untracked.int32(1),
    PixelClusterCollectionLabel    = cms.untracked.InputTag('siPixelClusters'),
    filterOnL1TechnicalTriggerBits = cms.untracked.int32(0),
    L1TechnicalTriggerBits         = cms.untracked.vint32(40, 41)
    )


# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "pixel-mc-XXXX.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(0),
    rootFileName                 = cms.untracked.string(rootFileName),
    type                         = cms.untracked.string(getDataset(process.source.fileNames[0])),
    globalTag                    = process.GlobalTag.globaltag,
    dumpAllEvents                = cms.untracked.int32(0),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
    muonCollectionLabel          = cms.untracked.InputTag('muons'),
    trajectoryInputLabel         = cms.untracked.InputTag('TrackRefitter::Demo'),
    trackCollectionLabel         = cms.untracked.InputTag('generalTracks'),
    pixelClusterLabel            = cms.untracked.InputTag('siPixelClusters'),
    pixelRecHitLabel             = cms.untracked.InputTag('siPixelRecHits'),
    HLTProcessName               = cms.untracked.string('HLT'), 
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap'), 
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::HLT'),
    accessSimHitInfo               = cms.untracked.bool( True )
    )

process.load("SimGeneral.MixingModule.mixNoPU_cfi")
#process.p0 = cms.Path(process.mix)

# -- Path
process.p = cms.Path(
    process.mix*
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    )


