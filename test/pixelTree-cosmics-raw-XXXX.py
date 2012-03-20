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

process = cms.Process('Demo')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# most likely, the files are only on castor, e.g.
# rfio:/castor/cern.ch/cms/store/data/Commissioning12/Cosmics/RAW/v1/000/186/996/5A0A9D0E-C768-E111-A5CF-001D09F25267.root

# -- Input files
# POOLSOURCE

# -- Input files
#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    REPLACEFILES
#    )
#    )

# Input source
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        )
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


#
# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# Other statements
process.GlobalTag.globaltag = 'GR_P_V28::All'

process.raw2digi_step = cms.Path(process.gctDigis
                                 +process.gtDigis
                                 +process.gtEvmDigis
                                 +process.siPixelDigis
                                 +process.siStripDigis
                                 +process.hcalDigis
                                 +process.muonCSCDigis
                                 +process.muonDTDigis
                                 +process.muonRPCDigis
                                 )

process.reconstruction_step = cms.Path(process.trackerCosmics
                                       *process.muonsLocalRecoCosmics
                                       *process.vertexrecoCosmics
                                       )

# ------------------
# -- the tree filler
# ------------------

# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'splittedTracksP5'

# -- RecHit production
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "pixelTree-cosmics-raw-XXXX.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(0),
    rootFileName                 = cms.untracked.string(rootFileName),
    type                         = cms.untracked.string('/Commissioning12/MinimumBias/RAW/'),
    globalTag                    = process.GlobalTag.globaltag,
    dumpAllEvents                = cms.untracked.int32(0),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
    muonCollectionLabel          = cms.untracked.InputTag('muons'),
    trajectoryInputLabel         = cms.untracked.InputTag('TrackRefitter::Demo'),
    trackCollectionLabel         = cms.untracked.InputTag('splittedTracksP5'),
    pixelClusterLabel            = cms.untracked.InputTag('siPixelClusters'),
    pixelRecHitLabel             = cms.untracked.InputTag('siPixelRecHits'),
    HLTProcessName               = cms.untracked.string('HLT'), 
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis'), 
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap'), 
    l1muGmtLabel                 = cms.untracked.InputTag('gtDigis'), 
    HLTResultsLabel              = cms.untracked.InputTag('TriggerResults::HLT')
    )

## -- read in other tags instead of default
#process.cablingReader = cms.ESSource(
#    "PoolDBESSource",
#    process.CondDBSetup,
#    toGet = cms.VPSet(cms.PSet(
#    record = cms.string('SiPixelFedCablingMapRcd'),
#    tag = cms.string('SiPixelFedCablingMap_v17'),
#    )),
#    connect = cms.string('sqlite_file:SiPixelFedCablingMap_v17.db')
#    )
#
#process.esprefer_DBReaders = cms.ESPrefer("PoolDBESSource", "cablingReader")


# -- Path
process.p = cms.Path(
    process.siPixelRecHits*
    process.TrackRefitter*
    process.PixelTree
    )



# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.p)

