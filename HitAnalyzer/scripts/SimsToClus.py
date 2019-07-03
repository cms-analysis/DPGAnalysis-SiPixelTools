##############################################################################
# start from simhits, do trackerlocal, compare sim and rec-hits
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("Test",eras.Run2_2017)

process.load("Configuration.Geometry.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# process.load("SimTracker.Configuration.SimTracker_cff")
process.load("SimG4Core.Configuration.SimG4Core_cff")

process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')

# for strips 
process.load("CalibTracker.SiStripESProducers.SiStripGainSimESProducer_cfi")

# clusterizers & rhs 
#process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff")

# Refitter
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')

process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')


process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
 '/store/user/kotlinski/MC/mu_pt100/simhits/simHits1.root'
# '/store/user/kotlinski/MC/mu_pt100/simhits/simHits1_2018design.root'

# '/store/user/kotlinski/MC/n/simhits/simHits_n_100MeV.root'
# '/store/user/kotlinski/MC/n/simhits/simHits_n_10MeV.root'
# '/store/user/kotlinski/MC/n/simhits/simHits_n_1GeV.root'
# '/store/user/kotlinski/MC/g/simhits/simHits_g_0p1MeV.root'
# '/store/user/kotlinski/MC/g/simhits/simHits_g_1MeV.root'
# '/store/user/kotlinski/MC/g/simhits/simHits_g_10MeV.root'
# '/store/user/kotlinski/MC/g/simhits/simHits_g_100MeV.root'

# mb
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/F0FF4DFA-613B-4749-BBC3-FB29AD691FA5.root"
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_design_v4-v1/20000/C9CE0381-F732-4A4F-A263-D445DF99148C.root"
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_realistic_v8-v1/20000/E0E79FD8-5A76-6145-9816-B7BBC5E79AB1.root" 

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
# GenError
useLocalDB = False
if useLocalDB :
    process.DBReaderFrontier = cms.ESSource("PoolDBESSource",
     DBParameters = cms.PSet(
         messageLevel = cms.untracked.int32(0),
         authenticationPath = cms.untracked.string('')
     ),
     toGet = cms.VPSet(
 	 cms.PSet(
          record = cms.string('SiPixelGenErrorDBObjectRcd'),
#          tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#          tag = cms.string('SiPixelGenErrorDBObject38TV10')
          tag = cms.string('SiPixelGenErrorDBObject_38T_v1_mc')
 	 ),
 	),
#     connect = cms.string('sqlite_file:siPixelGenErrors38T.db')
#     connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
     connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
    ) # end process
    process.myprefer = cms.ESPrefer("PoolDBESSource","DBReaderFrontier")
# endif


process.g4SimHits.Generator.HepMCProductLabel = 'source'

# for direct digis
#process.siPixelClustersPreSplitting.src = 'simSiPixelDigis' # for V5, direct
# process.siPixelClusters.src = 'mix'

# Additional output definition
# modify digitizer parameters
#process.mix.digitizers.pixel.TofUpperCut = 9999.5  # 12.5
#process.mix.digitizers.pixel.TofLowerCut =-9999.5  # -12.5
process.mix.digitizers.pixel.ThresholdInElectrons_FPix    = cms.double(2000.0)
process.mix.digitizers.pixel.ThresholdInElectrons_BPix    = cms.double(2000.0)
process.mix.digitizers.pixel.ThresholdInElectrons_BPix_L1 = cms.double(2000.0)
process.mix.digitizers.pixel.ThresholdInElectrons_BPix_L2 = cms.double(2000.0)
process.mix.digitizers.pixel.ThresholdSmearing_FPix    = cms.double(1.0)
process.mix.digitizers.pixel.ThresholdSmearing_BPix    = cms.double(1.0)
process.mix.digitizers.pixel.ThresholdSmearing_BPix_L1 = cms.double(1.0)
process.mix.digitizers.pixel.ThresholdSmearing_BPix_L2 = cms.double(1.0)

#process.simSiPixelDigis.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
# use inefficiency from DB Gain calibration payload?
#process.simSiPixelDigis.digitizers.pixel.useDB = cms.bool(False) 
# use LA from file 
process.mix.digitizers.pixel.LorentzAngle_DB = cms.bool(False)
process.mix.digitizers.pixel.TanLorentzAnglePerTesla_BPix = 0.011  # 0.1 
process.mix.digitizers.pixel.TanLorentzAnglePerTesla_FPix = 0.011  # 0.1

# modify clusterie parameters
#process.siPixelClustersPreSplitting.VCaltoElectronGain = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = -60
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = -60
process.siPixelClustersPreSplitting.SeedThreshold = 1000 # def
process.siPixelClustersPreSplitting.ChannelThreshold = 10 #def, must be bigger than 1
process.siPixelClustersPreSplitting.ClusterThreshold = 4000    # def, integer
process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 2000 # def, integer

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
# use the test from SiTracker
process.bf =  cms.EDAnalyzer("PixSimHitsTest",
                             src = cms.string("g4SimHits"),
                             #mode = cms.untracked.string("bpix"),
                             #list = cms.string("TrackerHitsPixelBarrelLowTof"),
                             #list = cms.string("TrackerHitsPixelBarrelHighTof"),
                             mode = cms.untracked.string("fpix"),
                             list = cms.string("TrackerHitsPixelEndcapLowTof"),
                             #list = cms.string("TrackerHitsPixelEndcapHighTof"),
                             Verbosity = cms.untracked.bool(False),
                             phase1 = cms.untracked.bool(True),
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
    fileName = cms.string('simtorec.root')
)


# go through raw
#process.p1 = cms.Path(process.b*process.bf)

#process.p1 = cms.Path(process.b*process.bf*process.pdigi_valid*process.a0)

#process.p1 = cms.Path(process.b*process.bf*process.pdigi_valid*process.SimL1Emulator*process.DigiToRaw*process.RawToDigi*process.a)

process.p1 = cms.Path(process.b*process.bf*process.pdigi_valid*process.SimL1Emulator*process.DigiToRaw*process.RawToDigi*process.a*process.reconstruction*process.d*process.c)

#process.outpath = cms.EndPath(process.o1)


