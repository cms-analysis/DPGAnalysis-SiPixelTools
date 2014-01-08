# ######################################################################
# lxb8838.cern.ch
# /afs/cern.ch/user/u/ursl/pixel/CMSSW_4_2_3_patch2/src/DPGAnalysis/SiPixelTools/test/reco/165
# file list contains 188389 events
# mkPyFiles -t ../../pixelTree-XXXX.py -f 110526.165617 -s r28 -e 50000 -r
# ./pixelTree-r28-0165617-0002.py with 50000 events, skipEvents = 100000
# ######################################################################
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
import os,sys
import FWCore.ParameterSet.Config as cms
process = cms.Process("Demo")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_P_V42_AN4::All' 


# -- Input files
process.source = cms.Source(
    "PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
        '/store/data/Run2012D/MuOnia/RECO/PromptReco-v1/000/208/307/F282D666-F03C-E211-B25A-BCAEC5329703.root '
    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
    )


# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks'

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

# -- skimming
#process.PixelFilter = cms.EDFilter(
#	"TriggerResultsFilter",
#	triggerConditions = cms.vstring('HLT_ZeroBias_v?' )
#)
process.PixelFilter = cms.EDFilter("TriggerResultsFilter",
        triggerConditions = cms.vstring('HLT_ZeroBias_v*/2'),
	hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
	l1tResults = cms.InputTag( "" ),
	daqPartitions = cms.uint32( 1 ),
	l1tIgnoreMask = cms.bool( False ),
	l1techIgnorePrescales = cms.bool( True ),
	throw = cms.bool( True )
)
    

# -- the tree filler
##try:
rootFileName = 'data.root' 
##except KeyError:
##    rootFileName = 

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
    lumiSummaryLabel             = cms.untracked.InputTag('lumiProducer'),
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
    #process.PixelFilter*
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    )
