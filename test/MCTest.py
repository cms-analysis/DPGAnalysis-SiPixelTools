# ----------------------------------------------------------------------
# -- RECO py template file for dumping the PixelTree only
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
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound'), wantSummary = cms.untracked.bool(True) )

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "START70_V3::All"

# -- Input files
# POOLSOURCE

# -- Input files
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	'/store/relval/CMSSW_7_0_0_pre8/RelValSingleMuPt10/GEN-SIM-RECO/START70_V1-v1/00000/5639F1A7-934A-E311-B7EC-0025905964BA.root'   )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
    )

# -- Trajectory producer
process.load('RecoTracker/MeasurementDet/MeasurementTrackerEventProducer_cfi')
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks'

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

# -- skimming
process.PixelFilter = cms.EDFilter(
    "SkimEvents",
    verbose                        = cms.untracked.int32(0),
    filterOnPrimaryVertex          = cms.untracked.int32(0),
    primaryVertexCollectionLabel   = cms.untracked.InputTag('offlinePrimaryVertices'),
    filterOnTracks                 = cms.untracked.int32(0),
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
    rootFileName = "pixelTree_MC.root"

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
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::HLT')
    )

# -- Path
process.p = cms.Path(
#    process.PixelFilter*
    process.siPixelRecHits*
    process.MeasurementTrackerEvent*
    process.TrackRefitter*
    process.PixelTree
    )
