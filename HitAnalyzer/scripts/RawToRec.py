
##############################################################################
# start from simhits, do trackerlocal, compare sim and rec-hits
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
#process = cms.Process("myRawToReco",eras.Run2_2017) # default 
process = cms.Process("myRawToReco",eras.Run3)

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

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_design', '') # for Run3
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '') # for Run3
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2023_realistic', '') # for Run3
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2024_realistic', '') # for Run3

#process.GlobalTag.globaltag = '106X_upgrade2018_realistic_v9'
#process.GlobalTag.globaltag = '110X_mcRun3_2021_realistic_v3'
#process.GlobalTag.globaltag = '110X_mcRun3_2023_realistic_v3'
#process.GlobalTag.globaltag = '110X_mcRun3_2024_realistic_v3'

#process.GlobalTag.globaltag = '106X_mcRun3_2023_realistic_Candidate_2019_10_11_19_29_18' #1200e
#process.GlobalTag.globaltag = '106X_mcRun3_2023_realistic_Candidate_2019_10_11_19_43_54' #2000e

#process.GlobalTag.globaltag = '111X_mcRun3_2021_design_newPixelGain_v1'
#process.GlobalTag.globaltag = '111X_mcRun3_2021_realistic_newPixelGain_v1'
#process.GlobalTag.globaltag = '111X_mcRun3_2023_realistic_newPixelGain_v1'
#process.GlobalTag.globaltag = '111X_mcRun3_2024_realistic_newPixelGain_v1'

#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
 '/store/user/kotlinski/MC11/mu_pt100/raw/raw1.root',

# Run3 MC
#"/store/mc/Run3Summer19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/106X_mcRun3_2021_realistic_v3-v2/130000/FF1B6F7B-55F6-2A42-A85B-654E59172A1E.root",
#"/store/mc/Run3Summer19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/106X_mcRun3_2023_realistic_v3-v2/50000/FF819EB4-7243-1640-9F3A-21553B718CA9.root",
#"/store/mc/Run3Summer19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/106X_mcRun3_2024_realistic_v4-v2/270000/C0FD668E-2D90-4F4A-A607-9E8FF11B0A9F.root",
#"/store/mc/Run3Summer19DRPremix/SingleNeutrino/GEN-SIM-RECO/2021ScenarioNZSRECO_106X_mcRun3_2021_realistic_v3-v2/270000/FF710887-FA64-F348-B870-C1A578545138.root",

# 2018
#"/store/relval/CMSSW_10_6_1_patch3/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2018_realistic_v6-v1/20000/FE7FDB90-CC19-244B-B7CE-DDEB9500B5CB.root",
#"/store/relval/CMSSW_10_6_1_patch3/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2018_realistic_v6-v1/20000/FE65C915-9009-C742-B10C-95A57202AE41.root",
#"/store/relval/CMSSW_10_6_1_patch3/RelValTTbar_13/GEN-SIM-RECO/PU25ns_106X_upgrade2018_realistic_v6-v1/20000/FF0A1ADF-8ECA-514A-A177-2723BCCABDE0.root",
#" /store/relval/CMSSW_10_6_1_patch3/RelValTTbar_13/GEN-SIM-RECO/PU25ns_106X_upgrade2018_realistic_v6-v1/20000/FBD352C0-28D6-9049-B3E9-C0DFF01658C8.root",


# mb used in 2/2019
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/F0FF4DFA-613B-4749-BBC3-FB29AD691FA5.root"
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_design_v4-v1/20000/C9CE0381-F732-4A4F-A263-D445DF99148C.root"
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_realistic_v8-v1/20000/"  #new

# PU
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
        #tag = cms.string('SiPixelGainCalibration_phase1_mc_v2')
        tag = cms.string('SiPixelGainCalibration_phase1_mc_v3') # gains with vcal
    )),
#    connect = cms.string('sqlite_file:/t3home/kotlinski/CMSSW/CMSSW_11_1_0_pre4/src/DPGAnalysis-SiPixelTools/PixelDBTools/test/gain_mc_v3.db')
#    connect = cms.string('sqlite_file:/t3home/kotlinski/CMSSW/CMSSW_11_1_0_pre4/src/DPGAnalysis-SiPixelTools/PixelDBTools/test/gain_mc_v2.db')
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
     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
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

#process.siPixelClustersPreSplitting.VCaltoElectronGain = 1  # 47 default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = 0 # -60 default 
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 1  # 50 default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = 0 # -670 default 

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


process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# after the digitizer 
#    src = cms.InputTag("mix"),
# after raw
    src = cms.InputTag("siPixelDigis"),
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



process.TFileService = cms.Service("TFileService",
    fileName = cms.string('rawtoreco.root')
)

# reconstruct all 
process.p1 = cms.Path(process.RawToDigi*process.a*process.reconstruction*process.d*process.c)
#process.p1 = cms.Path(process.r*process.RawToDigi*process.a*process.reconstruction*process.d*process.c)

#process.outpath = cms.EndPath(process.o1)


