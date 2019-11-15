##############################################################################
# start from simhits, do trackerlocal, compare sim and rec-hits
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
#process = cms.Process("TestValid",eras.Run2_2017)
process = cms.Process("TestValid",eras.Run3)

process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Services_cff")
#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.load('Configuration.StandardSequences.Digi_cff')
#process.load('Configuration.StandardSequences.SimL1Emulator_cff')
#process.load('Configuration.StandardSequences.DigiToRaw_cff')
#process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# process.load("SimTracker.Configuration.SimTracker_cff")
#process.load("SimG4Core.Configuration.SimG4Core_cff")

process.load('Configuration.EventContent.EventContent_cff')
#process.load('SimGeneral.MixingModule.mixNoPU_cfi')

# for strips 
#process.load("CalibTracker.SiStripESProducers.SiStripGainSimESProducer_cfi")

# clusterizers & rhs 
#process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# needed for pixel RecHits (templates?)
#process.load("Configuration.StandardSequences.Reconstruction_cff")

# Refitter
#process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

process.GlobalTag.globaltag = '110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00' # 2000e
#process.GlobalTag.globaltag = '110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44' # 2000e
#process.GlobalTag.globaltag = '110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03' # 2000e


process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(

# new run3 MC
'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2021_realistic_Candidate_2019_10_31_12_09_00_th2000_3/TTbar/TTbar_RECOSIM_2021/191113_211045/0001/Run3_2021_RECOSIM_1099.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2023_realistic_Candidate_2019_11_04_22_24_44_th2000/TTbar/TTbar_RECOSIM_2023/191113_133540/0001/Run3_2023_RECOSIM_1092.root',
#'/store/user/tsusa/TTbar/Run3_RECOSIM-110X_mcRun3_2024_realistic_Candidate_2019_10_31_12_18_03_th2000/TTbar/TTbar_RECOSIM_2024/191113_133404/0001/Run3_2024_RECOSIM_1098.root',

#  'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/simhits/simHits1.root',
#"/store/user/leac/MinBias/MC_2017_UL_ReRECO_more/190130_162421/0000/test_sim_2.root",
# mb used in 2/2019
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/F0FF4DFA-613B-4749-BBC3-FB29AD691FA5.root"
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_design_v4-v1/20000/C9CE0381-F732-4A4F-A263-D445DF99148C.root"
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_realistic_v8-v1/20000/"  #new
# PU (GEN-SIM-DIGI-RAW)  used in 2/2019
#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_design_v4-v1/20000/193FB1CE-333D-E540-995E-1BA38BA1CE3C.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_realistic_v8-v1/20000/0D388168-67C8-A544-8A75-8A96D40C396B.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_mc2017_realistic_v2-v1/20000/222AF93E-CE66-6342-A266-E78F6FCDD8EC.root"

# new 
# MB (GEN-SIM-DIGI-RAW) 
#/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/103X_mc2017_realistic_v2-v1
#/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/103X_upgrade2018_design_v4-v1
#/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/103X_upgrade2018_realistic_v8-v1


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


#process.g4SimHits.Generator.HepMCProductLabel = 'source'


# use the test from SiTracker
process.b =  cms.EDAnalyzer("PixSimHitsTest",
                                   src = cms.string("g4SimHits"),
        mode = cms.untracked.string("bpix"),
        list = cms.string("TrackerHitsPixelBarrelLowTof"),
        #list = cms.string("TrackerHitsPixelBarrelHighTof"),
#        mode = cms.untracked.string("fpix"),
                                   #list = cms.string("TrackerHitsPixelEndcapLowTof"),
                                   #list = cms.string("TrackerHitsPixelEndcapHighTof"),
        Verbosity = cms.untracked.bool(False),
        phase1 = cms.untracked.bool(True),
)


process.d = cms.EDAnalyzer("PixClusterAna",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClustersForLumi"),   # from the lumi stream
    src = cms.InputTag("siPixelClusters"),
    #src = cms.InputTag("siPixelClustersPreSplitting"),
    #src = cms.InputTag("ALCARECOTkAlMinBias"), # ALCARECO
    Tracks = cms.InputTag("generalTracks"),
    # additional selections, e.g. select bx=1 -> (2,1)
    Select1 = cms.untracked.int32(9998),  # select the cut type, 0 no cut,9998 for mc
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
    Select1 = cms.untracked.int32(9998),  # select the cut type, 0 no cut,9998 for mc
    Select2 = cms.untracked.int32(0),  # select the cut value   
)


process.a0 = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# after the digitizer 
    src = cms.InputTag("mix"),
# after raw
#    src = cms.InputTag("siPixelDigis"),
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
    fileName = cms.string('inspectAll.root')
)

# all
#process.p1 = cms.Path(process.b*process.a0*process.r*process.a*process.d*process.c)
# RECO 
process.p1 = cms.Path(process.d*process.c)

#process.outpath = cms.EndPath(process.o1)

