# ----------------------------------------------------------------------
# -- ALCARECO py template file for dumping the PixelTree only
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
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_E_V13::All"

# -- Input files
# POOLSOURCE

# -- Input files
#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    REPLACEFILES
#    )
#    )

# -- Trajectory producer
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'ALCARECOTkAlMinBias'

# -- Beamspot producer
process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi")
process.offlinePrimaryVertices.TrackLabel = "ALCARECOTkAlMinBias"

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
process.siPixelRecHits.src = "ALCARECOTkAlMinBias"

# -- Trigger information printout
process.hltrep = cms.EDAnalyzer(
    "HLTrigReport",
    HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
    )


process.load("L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi")
process.l1trep = cms.EDAnalyzer(
    "L1GtTrigReport",
    UseL1GlobalTriggerRecord = cms.bool( False ),
    L1GtRecordInputTag = cms.InputTag( "gtDigis" )
    )

# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "pixelTree-XXXX.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(0),
    rootFileName                 = cms.untracked.string(rootFileName),
    type                         = cms.untracked.string(getDataset(process.source.fileNames[0])),
    globalTag                    = process.GlobalTag.globaltag,
    dumpAllEvents                = cms.untracked.int32(1),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
    muonCollectionLabel          = cms.untracked.InputTag('muons'),
    trajectoryInputLabel         = cms.untracked.InputTag('TrackRefitter::Demo'),
    trackCollectionLabel         = cms.untracked.InputTag('ALCARECOTkAlMinBias'),
    pixelClusterLabel            = cms.untracked.InputTag('ALCARECOTkAlMinBias'),
    pixelRecHitLabel             = cms.untracked.InputTag('siPixelRecHits'),
    HLTProcessName               = cms.untracked.string('HLT'), 
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap'), 
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::HLT')
    )

# -- Path
process.p = cms.Path(
#    process.PixelFilter*
    process.offlineBeamSpot*
    process.offlinePrimaryVertices*
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    *process.hltrep*process.l1trep
    )
