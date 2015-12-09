# produce pixel cluster & rechits from digia
# works directly or through raw
# 
# Fro phase1
##############################################################################
import FWCore.ParameterSet.Config as cms

process = cms.Process("RecHitTest")
 
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')

#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# clusterizer 
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

# needed for pixel RecHits (for TkPixelCPERecord))
process.load("Configuration.StandardSequences.Reconstruction_cff")

# for raw
#process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
#process.load('Configuration.StandardSequences.DigiToRaw_cff')
#process.load('Configuration.StandardSequences.RawToDigi_cff')


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '76X_upgrade2017_design_v8', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('SiPixelClusterizer'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
#       threshold = cms.untracked.string('INFO')
#       threshold = cms.untracked.string('ERROR')
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)
# get the files from DBS:
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
#    'file:digis_1k_GT.root'
#    'file:digis.root'
    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/digis/digis1_pixonly.root'
  )
)

# a service to use root histos (keep if the analyser is run at the end)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('rechits_histo.root')
)

# Local DB stuff 
# cabling
#           record = cms.string('SiPixelFedCablingMapRcd'),
#           tag = cms.string('SiPixelFedCablingMap_v17')
# GenError
useLocalGenErrors = True
if useLocalGenErrors :
    process.GenErrorReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
          record = cms.string('SiPixelGenErrorDBObjectRcd'),
          tag = cms.string('SiPixelGenErrorDBObject_phase1_38T_mc_v1')
       ),
      ),
      #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGenErrorDBObject_phase1_38T_mc_v1.db')
      #connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS') #NEW
      connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS') #NEW
    ) # end process
    process.GenErrorprefer = cms.ESPrefer("PoolDBESSource","GenErrorReader")
# endif 

# LA 
useLocalLA = True
if useLocalLA :
    process.LAReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
# LA
	record = cms.string("SiPixelLorentzAngleRcd"),
        tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
 		),
 	),
      #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
      #connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS') #NEW
      connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS') #NEW
    ) # end process

    process.LAprefer = cms.ESPrefer("PoolDBESSource","LAReader")

    process.LAWidthReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
	record = cms.string("SiPixelLorentzAngleRcd"),
        label = cms.untracked.string("forWidth"),
        tag = cms.string("SiPixelLorentzAngle_forWidth_phase1_mc_v1")
       ),
     ),
     #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_forWidth_phase1_mc_v1.db')
     #connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS') #NEW
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS') #NEW
    ) # end process

    process.LAWidthprefer = cms.ESPrefer("PoolDBESSource","LAWidthReader")

# endif

# Gain 
useLocalGain = True
if useLocalGain :
  process.GainsReader = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0),
       authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
        tag = cms.string('SiPixelGainCalibration_phase1_ideal')
        #tag = cms.string('SiPixelGainCalibration_phase1_mc_v1')
    )),
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_mc_v1.db')
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_ideal.db')
    #connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS') #NEW
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS') #NEW
  ) # end process
  process.Gainprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if

process.o1 = cms.OutputModule("PoolOutputModule",
          outputCommands = cms.untracked.vstring('drop *','keep *_*_*_RecHitTest'),
#          fileName = cms.untracked.string('file:rechits.root')
         fileName = cms.untracked.string('file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/clus/rechits1_pixonly.root')
)

# My 
# modify clusterie parameters
#process.siPixelClusters.ClusterThreshold = 4000.0

# DIRECT
# direct clusterization (no raw step)
# label of digis 
process.siPixelClustersPreSplitting.src = 'simSiPixelDigis'
#process.siPixelClustersPreSplitting.ClusterThreshold = 4000.
# set to false to ignore the gain calibration
#process.siPixelClustersPreSplitting.MissCalibrate = cms.untracked.bool(False)
# force generic CPE to skip GenErrors. For parameters used by the producer us the producer process
#process.PixelCPEGenericESProducer.UseErrorsFromTemplates = cms.bool(False)
#process.PixelCPEGenericESProducer.LoadTemplatesFromDB = cms.bool(False)
#process.PixelCPEGenericESProducer.TruncatePixelCharge = cms.bool(False)

#process.PixelCPEGenericESProducer.useLAAlignmentOffsets = cms.bool(True)
#process.PixelCPEGenericESProducer.useLAWidthFromDB = cms.bool(True)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.098)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.09)
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.double(0.098)
#process.PixelCPEGenericESProducer.lAWidthBPix = cms.double(0.0)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.double(0.058)
#process.PixelCPEGenericESProducer.lAWidthFPix = cms.double(0.0)

# read rechits
process.analysis = cms.EDAnalyzer("PixRecHitTest",
    Verbosity = cms.untracked.bool(False),
#    src = cms.InputTag("siPixelRecHits"),
    src = cms.InputTag("siPixelRecHitsPreSplitting"),
)

# To look at recHits one has to run the analysis directly.
# rechits will not be stored correctly in the output file 
# because they are not made persistant.

# pixel clusters 
#process.p1 = cms.Path(process.siPixelClustersPreSplitting)
# pixel clusters & rechits
#process.p1 = cms.Path(process.pixeltrackerlocalreco)
# plus analysis
process.p1 = cms.Path(process.pixeltrackerlocalreco*process.analysis)

# RAW
# clusterize through raw (OK)
# for Raw2digi for simulations 
#process.siPixelRawData.InputLabel = 'mix'
#process.siPixelRawData.InputLabel = 'simSiPixelDigis'
#process.siPixelDigis.InputLabel = 'siPixelRawData'
#process.siStripDigis.ProductLabel = 'SiStripDigiToRaw'
#process.siPixelClusters.src = 'siPixelDigis'

#process.p1 = cms.Path(process.siPixelRawData)
#process.p1 = cms.Path(process.siPixelRawData*process.siPixelDigis)
#process.p1 = cms.Path(process.siPixelRawData*process.siPixelDigis*process.siPixelClusters*process.analysis)

# for no output comment it out
#process.outpath = cms.EndPath(process.o1)
