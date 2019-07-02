##############################################################################
# start from simhits, digitze, digi-raw, raw->digi, reco, triplets 
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("MySimToTriplets",eras.Run2_2017)

process.load("Configuration.Geometry.GeometryDB_cff")

#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.Digi_cff')


# change the random seed
#process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#  mix = cms.PSet(
#    initialSeed = cms.untracked.uint32(123456789),
#    engineName = cms.untracked.string('TRandom3')
#  )
#)
#process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#     simMuonCSCDigis = cms.PSet(
#        initialSeed = cms.untracked.uint32(1234567),
#        engineName = cms.untracked.string('TRandom3')
#    ),
#     simSiPixelDigis = cms.PSet(
#        initialSeed = cms.untracked.uint32(1234567),
#        engineName = cms.untracked.string('TRandom3')
#   )
#)

# to digitze tracker only 
# process.digitisation_step = cms.Path(cms.SequencePlaceholder("mix")*process.trDigi*process.trackingParticles) 


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use for MC
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '') # for phase1, idealBS
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '') # for phase1 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '') # for phase1 "design"


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)


process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
  #'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/simhits/simHits1.root',

"/store/relval/CMSSW_9_4_0_pre3/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_94X_mc2017_design_IdealBS_v4-v1/10000/0E1F186E-1BBB-E711-AD29-0CC47A7C34A0.root",

#"/store/relval/CMSSW_9_4_0_pre3/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_94X_mc2017_realistic_v4_highPU_AVE50-v1/10000/06DBBF38-BEBA-E711-80DC-4C79BA1814BB.root",

  )
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('Pxl'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
#        threshold = cms.untracked.string('DEBUG')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.o1 = cms.OutputModule("PoolOutputModule",
      outputCommands = cms.untracked.vstring('drop *','keep *_*_*_MySimToTriplets'),
#      outputCommands = cms.untracked.vstring('keep *_*_*_*'),
      fileName = cms.untracked.string('file:dummy.root')
)

# Overwrite the DB objects with ones from the disk (sqlite) 
# for digitisation
#LA
useLocalLASim = False
if useLocalLASim :
  process.LASimReader = cms.ESSource("PoolDBESSource",
    DBParameters = 
      cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('')
      ),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string("SiPixelLorentzAngleSimRcd"),
        tag = cms.string("SiPixelLorentzAngleSim_phase1_mc_v1")
      ),
    ),
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngleSim_phase1_mc_v1.db')
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  ) # end process
  process.lasimprefer = cms.ESPrefer("PoolDBESSource","LASimReader")
#  end if

# Quality
useLocalQuality = False
if useLocalQuality :
  process.QualityReader = cms.ESSource("PoolDBESSource",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('SiPixelQuality_phase1_ideal')
    )),
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelQuality_phase1_ideal.db')
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  )
  process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")
#  end if

# for reco 
# LA 
useLocalLA = False
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
# cabling
#           record = cms.string('SiPixelFedCablingMapRcd'),
#           tag = cms.string('SiPixelFedCablingMap_v17')
# GenError
#          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
#          tag = cms.string('SiPixelGenErrorDBObject38T_v0_mc1')
#          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
 		),
 	),
     #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    ) # end process

    process.LAprefer = cms.ESPrefer("PoolDBESSource","LAReader")

    # now the forWidth LA
    process.LAWidthReader = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
       cms.PSet(
	record = cms.string("SiPixelLorentzAngleRcd"),
        label = cms.untracked.string("forWidth"),
        tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
       ),
     ),
     #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')

    ) # end process

    process.LAWidthprefer = cms.ESPrefer("PoolDBESSource","LAWidthReader")

# endif

# Gain 
useLocalGain = False
if useLocalGain :
  process.GainsReader = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0),
       authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string('SiPixelGainCalibrationOfflineRcd'),
        #tag = cms.string('SiPixelGainCalibration_phase1_ideal')
        tag = cms.string('SiPixelGainCalibration_phase1_mc_v1')
    )),
    #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_mc_v1.db')
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
  ) # end process
  process.Gainprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if


# overwite adjustable parameters 
#process.g4SimHits.Generator.HepMCProductLabel = 'source'
# for direct digis
#process.siPixelClustersPreSplitting.src = 'simSiPixelDigis' # for V5, direct
# process.siPixelClusters.src = 'mix'
# modify digitizer parameters
#process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_BPix = 4500.0  # 3500.
#process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_BPix_L1 = 4500.0  # 3500.
#process.simSiPixelDigis.digitizers.pixel.ThresholdInElectrons_FPix = 4500.0  # 3500.
#
#process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
#process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiency = cms.bool(False)
#
# use inefficiency from DB Gain calibration payload? OFF BY DEFAULT
#process.simSiPixelDigis.digitizers.pixel.useDB = cms.bool(False) 
# use LA from file 
#process.simSiPixelDigis.digitizers.pixel.LorentzAngle_DB = cms.bool(False)
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla_BPix = 0.100 
#process.simSiPixelDigis.digitizers.pixel.TanLorentzAnglePerTesla_FPix = 0.058 
# clus , label of digis 
#process.siPixelClustersPreSplitting.src = 'simSiPixelDigis'
#process.siPixelClustersPreSplitting.ClusterThreshold = 2000.
#process.siPixelClustersPreSplitting.ChannelThreshold = 500
#process.siPixelClustersPreSplitting.SeedThreshold = 1000
# set to false to ignore the gain calibration
#process.siPixelClustersPreSplitting.MissCalibrate = cms.untracked.bool(False)
# rechits 
#process.PixelCPEGenericESProducer.lAOffset = cms.double(0.100) 
#process.PixelCPEGenericESProducer.useLAWidthFromDB = cms.bool(False)
# read rechits
#process.analysis = cms.EDAnalyzer("ReadPixelRecHit",
#    Verbosity = cms.untracked.bool(False),
#    src = cms.InputTag("siPixelRecHits"),
#)


# Triplets 
process.triplets = cms.EDAnalyzer('Pxl',
# for official RECO
#       triggerSource = cms.InputTag('TriggerResults::HLT'),
# For MC or my rereco
        triggerSource = cms.InputTag(''),
# Using templates   
#       ttrhBuilder = cms.string('WithAngleAndTemplate'),
# Using generic 
        ttrhBuilder = cms.string('WithTrackAngle'),
        singleParticleMC = cms.untracked.bool(False),
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('test.root')
)

# testing modules 

#process.analysis = cms.EDAnalyzer("PixClusterTest",
#    Verbosity = cms.untracked.bool(False),
#    src = cms.InputTag("siPixelClustersPreSplitting"),
#)
process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Select1 = cms.untracked.int32(1),  # cut on the num of dets <4 skip, 0 means 4 default 
    Select2 = cms.untracked.int32(0),  # 6 no bptx, 0 no selection                               
)

process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7, from simHits 
#    src = cms.InputTag("mix"),
# from raw 
    src = cms.InputTag("siPixelDigis"),
)


#This process is to run the digitizer + clusterizer + analysis 
#process.p1 = cms.Path(process.pdigi*process.a)  # OK
#process.p1 = cms.Path(process.pdigi*process.SimL1Emulator*process.DigiToRaw*process.RawToDigi*process.a)

process.p1 = cms.Path(process.pdigi*process.SimL1Emulator*process.DigiToRaw*process.RawToDigi*process.reconstruction*process.triplets)

# to save the output data files 
#process.outpath = cms.EndPath(process.o1)


