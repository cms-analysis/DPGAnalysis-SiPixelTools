
##############################################################################
# start from simhits, do trackerlocal, compare sim and rec-hits
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("TestValid",eras.Run2_2017) # default 
#process = cms.Process("TestValid",eras.Run2_2018)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("SimG4Core.Configuration.SimG4Core_cff")
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

# Refitter
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_design', '') # crashes 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

#process.GlobalTag = GlobalTag(process.GlobalTag, '104X_mc2017_realistic_Candv1', '')

#process.GlobalTag.globaltag = '103X_mc2017_design_IdealBS_v2' # mc 2017
#process.GlobalTag.globaltag = '103X_mc2017_realistic_v2'      # mc 2017
#process.GlobalTag.globaltag = '103X_upgrade2018_design_v4'    # mc 2018
#process.GlobalTag.globaltag = '103X_upgrade2018_realistic_v8' # mc 2018
#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw1_thr1k.root',
  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw1.root',
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw2.root',
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw3.root',
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw4.root',
#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw5.root',

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_design_v4-v1/20000/193FB1CE-333D-E540-995E-1BA38BA1CE3C.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_realistic_v8-v1/20000/0D388168-67C8-A544-8A75-8A96D40C396B.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_mc2017_realistic_v2-v1/20000/222AF93E-CE66-6342-A266-E78F6FCDD8EC.root"

  )
)

# allow for events to have the same event number 
process.source.duplicateCheckMode=cms.untracked.string("noDuplicateCheck")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('pixRecHitsValid'),
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
      outputCommands = cms.untracked.vstring('drop *','keep *_*_*_TestValid'),
#      outputCommands = cms.untracked.vstring('keep *_*_*_*'),
      fileName = cms.untracked.string('file:dummy.root')
)


# DB stuff 
# cabling
#           record = cms.string('SiPixelFedCablingMapRcd'),
#           tag = cms.string('SiPixelFedCablingMap_v17')
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
	  record = cms.string("SiPixelLorentzAngleRcd"),
          tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
 		),
 	),
      #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
      connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
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
        tag = cms.string("SiPixelLorentzAngle_forWidth_phase1_mc_v1")
       ),
     ),
     #connect= cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_forWidth_phase1_mc_v1.db')
     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
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
        #tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
        tag = cms.string('SiPixelGainCalibration_phase1_mc_v2')
        #tag = cms.string('SiPixelGainCalibration_phase1_mc_v3') # to test gains 
    )),
    #connect = cms.string('sqlite_file:/afs/cern.ch/work/d/dkotlins/public/DB/Gains/MC/SiPixelGainCalibration_phase1_ideal_v2.db')
    connect = cms.string('sqlite_file:/afs/cern.ch/work/d/dkotlins/public/DB/Gains/MC/SiPixelGainCalibration_phase1_mc_v2.db')

#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_ped_5p4.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_ped_10.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_ped_15.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_ped_20.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p054.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p1.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p15.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p2.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p32.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p64.db')
#    connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p96.db')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
  ) # end process
  process.Gainprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if


# GenError
useLocalGenErr = False
if useLocalGenErr :
    process.GenErrReader = cms.ESSource("PoolDBESSource",
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
     #connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelGenErrorDBObject_phase1_38T_mc_v1.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
    ) # end process
    process.generrprefer = cms.ESPrefer("PoolDBESSource","GenErrReader")
# endif

# clus , label of digis 
#process.siPixelClustersPreSplitting.src = 'simSiPixelDigis' <-- not needed 

#process.siPixelClustersPreSplitting.VCaltoElectronGain = 94  # 47 default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = -120 # -60 default 
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 100  # 50 default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = -1340 # -670 default 

# do 1000,1500,2000,2500,3000,3500,4000,5000
#process.siPixelClustersPreSplitting.SeedThreshold = 3500 #1  def=1000
#process.siPixelClustersPreSplitting.ChannelThreshold = 3500 #2 must be bigger than 1, def=10
#process.siPixelClustersPreSplitting.ClusterThreshold = 10 # def =4000    # integer?
#process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 10 # def=2000 # integer?

# set to false to ignore the gain calibration
#process.siPixelClustersPreSplitting.MissCalibrate = cms.untracked.bool(False)
# process.siPixelClusters.src = 'mix' <-- no needed 

# rechits
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

# to run rechit "official" validation
#process.load("Validation.TrackerRecHits.trackerRecHitsValidation_cff")
#process.load("Validation.TrackerRecHits.SiPixelRecHitsValid_cfi")

# raw2digi
#process.siPixelDigis.InputLabel = 'siPixelRawData'    <--- not needed
#process.siPixelDigis.InputLabel = 'rawDataCollector'  <--- not needed 

# my rec-sim hit compare 
#  process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.SiPixelRecHitsValid_cff")
process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.StudyRecHitResolution_cff")

# not on track 
#process.pixRecHitsValid.outputFile="pixelrechitshisto.root"
process.pixRecHitsValid.verbose=False 
process.pixRecHitsValid.muOnly=False 
process.pixRecHitsValid.ptCut=1. # 10.
#process.pixRecHitsValid.src="siPixelRecHitsPreSplitting"
process.pixRecHitsValid.src="siPixelRecHits"
#process.pixRecHitsValid.associatePixel = True
#process.pixRecHitsValid.associateStrip = False
#process.pixRecHitsValid.associateRecoTracks = False

# on track 
process.pixRecHitsValid.useTracks = True
process.pixRecHitsValid.tracks = 'TrackRefitter'

process.TrackRefitter.src = "generalTracks"
process.TrackRefitter.TTRHBuilder = 'WithAngleAndTemplate'
#process.TrackRefitter.TTRHBuilder = 'WithTrackAngle'

process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("generalTracks"),
    # additional selections, e.g. select bx=1 -> (2,1)
    Select1 = cms.untracked.int32(9998),  # select the cut type, 0 no cut
    Select2 = cms.untracked.int32(0),  # select the cut value   
)

process.c = cms.EDAnalyzer("PixClustersWithTracks",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    src = cms.InputTag("generalTracks"),
# for cosmics 
#    src = cms.InputTag("ctfWithMaterialTracksP5"),
#     PrimaryVertexLabel = cms.untracked.InputTag("offlinePrimaryVertices"),
#     trajectoryInput = cms.string("TrackRefitterP5")
#     trajectoryInput = cms.string('cosmictrackfinderP5')
# additional selections
    Select1 = cms.untracked.int32(9998),  # select the cut type, o no cut
    Select2 = cms.untracked.int32(0),  # select the cut value   
)

process.r = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2015 data, label = rawDataCollector, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
# for MC
#    InputLabel = cms.untracked.string('siPixelRawData'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),
# for dump files 
#    InputLabel = cms.untracked.string('source'),
# old
#    InputLabel = cms.untracked.string('siPixelRawData'),
#    InputLabel = cms.untracked.string('source'),
#    InputLabel = cms.untracked.string("ALCARECOTkAlMinBias"), # does not work
    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(0),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# after the digitizer 
#    src = cms.InputTag("mix"),
# after raw
    src = cms.InputTag("siPixelDigis"),
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('rawtorec.root')
)


#This process is to run the digitizer:
#process.p1 = cms.Path(process.simSiPixelDigis)
#process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco)
#process.p1 = cms.Path(process.simSiPixelDigis*process.pixeltrackerlocalreco*process.pixRecHitsValid)

# does not work with my digis
#process.myAnalyzer_step = cms.Path(process.pixRecHitsValid)

# on track 
####process.p1 = cms.Path(process.RawToDigi*process.reconstruction*process.MeasurementTrackerEvent*process.TrackRefitter*process.pixRecHitsValid)
process.p1 = cms.Path(process.RawToDigi*process.a*process.reconstruction*process.TrackRefitter*process.d*process.c*process.pixRecHitsValid)
#process.p1 = cms.Path(process.RawToDigi*process.reconstruction*process.TrackRefitter*process.pixRecHitsValid*process.c)
# all rechits 
#process.p1 = cms.Path(process.RawToDigi*process.reconstruction*process.pixRecHitsValid)  # works from raw with presplitted & normal rechits

#process.outpath = cms.EndPath(process.o1)


