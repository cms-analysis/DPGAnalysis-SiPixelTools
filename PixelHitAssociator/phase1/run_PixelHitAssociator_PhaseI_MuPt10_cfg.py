# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: -s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --evt_type SingleMuPt10_pythia8_cfi --conditions auto:phase1_2017_realistic --era Run2_2017 --geometry Extended2017 --fileout file:GENSIMRECO_MuPt10.root --python_filename=PhaseI_MuPt10_cfg.py --runUnscheduled -n 10
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2017_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('SingleMuPt10_pythia8_cfi nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:GENSIMRECO_MuPt10.root'),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

process.generator = cms.EDFilter("Pythia8PtGun",
    PGunParameters = cms.PSet(
        AddAntiParticle = cms.bool(True),
        MaxEta = cms.double(2.5),
        MaxPhi = cms.double(3.14159265359),
        MaxPt = cms.double(10.01),
        MinEta = cms.double(-2.5),
        MinPhi = cms.double(-3.14159265359),
        MinPt = cms.double(9.99),
        ParticleID = cms.vint32(-13)
    ),
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring()
    ),
    Verbosity = cms.untracked.int32(0),
    firstRun = cms.untracked.uint32(1),
    psethack = cms.string('single mu pt 10')
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 










# begin inserting configs
#------------------------------------------
#  Options - can be given from command line
#------------------------------------------
import FWCore.ParameterSet.VarParsing as opts

opt = opts.VarParsing ('analysis')

opt.register('globalTag',          '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Global Tag, Default="" which uses auto:run2_data')

opt.register('useClustersOnTrack', True,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'True: use RecHits of generalTracks, False: use siPixelRecHits')

opt.register('useTemplates',       True,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Only for On-track clusters! True: use Template reco, False: use Generic reco')

opt.register('saveRECO',           False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Option to keep GEN-SIM-RECO')

opt.register('useRECO',            False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Option to use GEN-SIM-RECO')

opt.register('RECOFileName',  '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Name of the histograms file')

opt.register('outputFileName',      '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Name of the histograms file')

opt.register('useLocalLASim',      False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test LA (SIM) conditions locally (prep/prod database or sqlite file')

opt.register('useLocalQuality',    False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test Quality conditions locally (prep/prod database or sqlite file')

opt.register('useLocalLA',         False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test LA (RECO) conditions locally (prep/prod database or sqlite file')

opt.register('useLocalGain',       False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test Gain conditions locally (prep/prod database or sqlite file')

opt.register('useLocalGenErr',     False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test GenError conditions locally (prep/prod database or sqlite file')

### Events to process: 'maxEvents' is already registered by the framework
opt.setDefault('maxEvents', 1000)

# Proceed with settings from command line
opt.parseArguments()

process.maxEvents.input = opt.maxEvents
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Set some default options based on others
if opt.useClustersOnTrack:
	if opt.useTemplates:
		process.initialStepTracks.TTRHBuilder = 'WithAngleAndTemplate'
		if opt.RECOFileName == '': opt.RECOFileName = 'file:GENSIMRECO_MuPt10_TemplateReco_'+str(opt.maxEvents)+'.root'
	else:
		process.initialStepTracks.TTRHBuilder = 'WithTrackAngle'
		if opt.RECOFileName == '': opt.RECOFileName = 'file:GENSIMRECO_MuPt10_GenericReco_'+str(opt.maxEvents)+'.root'
else:
	if opt.RECOFileName == '': opt.RECOFileName = 'file:GENSIMRECO_MuPt10_GenericReco_'+str(opt.maxEvents)+'.root'

# Add Input file in case using it
if opt.useRECO:
	process.setName_("TEST")
	process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
		# This is the file you create with saveRECO, by default
		opt.RECOFileName # Use previously saved RECO as input
		))

#________________________________________________________________________
#                        Main Analysis Module

# Other statements
#process.mix.digitizers = cms.PSet(process.theDigitizersValid)
# modify digitizer parameters
#process.simSiPixelDigis.pixel.ThresholdInElectrons_BPix = 3500.0 
# disable dyn. ineff.
#process.mix.digitizers.pixel.AddPixelInefficiencyFromPython = cms.bool(False)
#process.mix.digitizers.pixel.NumPixelBarrel = cms.int32(4)
#process.mix.digitizers.pixel.NumPixelEndcap = cms.int32(3)

# Load and confiugre the plugin you want to use
process.load("DPGAnalysis-SiPixelTools.PixelHitAssociator.SiPixelRecHitsValid_cff")

# Specify inputs/outputs
if opt.useClustersOnTrack:
	process.pixRecHitsValid.useTracks = True
	#process.pixRecHitsValid.tracks="initialStepTracks" # first tracker step
	process.pixRecHitsValid.tracks="generalTracks"
	if opt.useTemplates:
		if opt.outputFileName == '': opt.outputFileName = 'SimToRecHitHistos_TemplateReco_OnTrackHits_'+str(opt.maxEvents)+'.root'
	else:
		if opt.outputFileName == '': opt.outputFileName = 'SimToRecHitHistos_GenericReco_OnTrackHits_'+str(opt.maxEvents)+'.root'
else:
	process.pixRecHitsValid.useTracks = False
	#process.pixRecHitsValid.src="siPixelRecHitsPreSplitting"
	process.pixRecHitsValid.src="siPixelRecHits"
	if opt.outputFileName == '': opt.outputFileName = 'SimToRecHitHistos_GenericReco_siPixelRecHits_'+str(opt.maxEvents)+'.root'

# Some other options
process.pixRecHitsValid.outputFile = opt.outputFileName
# showing defaults
#process.pixRecHitsValid.verbose=False
#process.pixRecHitsValid.associatePixel = True
#process.pixRecHitsValid.associateStrip = False
#process.pixRecHitsValid.associateRecoTracks = False

# myAnalyzer Path
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = "generalTracks"
process.pixRecHitsValid.tracks = 'TrackRefitter'
process.myAnalyzer_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitter*process.pixRecHitsValid)

#________________________________________________________________________
#                        DataBase Stuff

# Print settings
print "Using options: "
if opt.globalTag == '':
    print "  globalTag (auto:phase1_2017_realistic) = "+str(process.GlobalTag.globaltag)
else:
    if "auto:" in opt.globalTag:
	process.GlobalTag = GlobalTag(process.GlobalTag, opt.globalTag, '')
	print "  globalTag ("+opt.globalTag+") = "+str(process.GlobalTag.globaltag)
    else:
	process.GlobalTag.globaltag = opt.globalTag
	print "  globalTag (manually chosen)            = "+str(process.GlobalTag.globaltag)
print "  useClustersOnTrack                     = "+str(opt.useClustersOnTrack)
print "  useTemplates                           = "+str(opt.useTemplates)
print "  saveRECO                               = "+str(opt.saveRECO)
print "  useRECO                                = "+str(opt.useRECO)
print "  RECOFileName                           = "+str(opt.RECOFileName)
print "  outputFileName                         = "+str(opt.outputFileName)
print "  maxEvents                              = "+str(opt.maxEvents)
print "  useLocalLASim                          = "+str(opt.useLocalLASim)
print "  useLocalQuality                        = "+str(opt.useLocalQuality)
print "  useLocalLA                             = "+str(opt.useLocalLA)
print "  useLocalGain                           = "+str(opt.useLocalGain)
print "  useLocalGenErr                         = "+str(opt.useLocalGenErr)

# Test Local DB conditions
# LA (SIM)
LASim_tag       = "SiPixelLorentzAngleSim_phase1_mc_v1"
#LASim_db        = 'sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngleSim_phase1_mc_v1.db'
LASim_db        = 'frontier://FrontierPrep/CMS_CONDITIONS'
#LASim_db        = 'frontier://FrontierProd/CMS_CONDITIONS'

# Quality
Qua_tag         = 'SiPixelQuality_phase1_ideal'
#Qua_db          = 'sqlite_file:../../../../../DB/phase1/SiPixelQuality_phase1_ideal.db'
Qua_db          = 'frontier://FrontierPrep/CMS_CONDITIONS'
#Qua_db          = 'frontier://FrontierProd/CMS_CONDITIONS'

# LA (RECO)
LA_tag          = 'SiPixelLorentzAngle_phase1_mc_v1'
LA_Width_tag    = 'SiPixelLorentzAngle_phase1_mc_v1'
#LA_db           = 'sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db'
#LA_db           = 'frontier://FrontierPrep/CMS_CONDITIONS'
LA_db           = 'frontier://FrontierProd/CMS_CONDITIONS'
#LA_Width_db     = 'sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db'
#LA_Width_db     = 'frontier://FrontierPrep/CMS_CONDITIONS'
LA_Width_db     = 'frontier://FrontierProd/CMS_CONDITIONS'

# Gains
#Gain_tag        = 'SiPixelGainCalibration_phase1_mc_v1'
Gain_tag        = 'SiPixelGainCalibration_phase1_mc_v2'
#Gain_tag        = 'SiPixelGainCalibration_phase1_ideal'
#Gain_tag        = 'SiPixelGainCalibration_phase1_ideal_v2'
Gain_db         = 'sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_mc_v2.db'
#Gain_db         = 'sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_ideal_v2.db'
#Gain_db         = 'frontier://FrontierPrep/CMS_CONDITIONS'
#Gain_db         = 'frontier://FrontierProd/CMS_CONDITIONS'

# GenErrors
GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_38T_mc_v1'
#GenErr_db       = 'sqlite_file:siPixelGenErrors38T.db'
#GenErr_db       = 'frontier://FrontierPrep/CMS_COND_PIXEL'
#GenErr_db       = 'frontier://FrontierPrep/CMS_CONDITIONS'
GenErr_db       = 'frontier://FrontierProd/CMS_CONDITIONS'

#LA
if opt.useLocalLASim :
	process.LASimReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string("SiPixelLorentzAngleSimRcd"),
			tag = cms.string(LASim_tag))),
		connect = cms.string(LASim_db))
	process.lasimprefer = cms.ESPrefer("PoolDBESSource","LASimReader")

# Quality
if opt.useLocalQuality :
	process.QualityReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('SiPixelQualityFromDbRcd'),
			tag = cms.string('SiPixelQuality_phase1_ideal'))),
		connect = cms.string(Qua_tag))
	process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")

# for reco
# LA 
if opt.useLocalLA :
	process.LAReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string("SiPixelLorentzAngleRcd"),
			tag = cms.string(LA_tag))),
		connect = cms.string(LA_db))
	process.LAprefer = cms.ESPrefer("PoolDBESSource","LAReader")
	# now the forWidth LA
	process.LAWidthReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string("SiPixelLorentzAngleRcd"),
			label = cms.untracked.string("forWidth"),
			tag = cms.string(LA_Width_tag))),
		connect = cms.string(LA_Width_db))
	process.LAWidthprefer = cms.ESPrefer("PoolDBESSource","LAWidthReader")

# Gain 
if opt.useLocalGain :
	process.GainsReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('SiPixelGainCalibrationOfflineRcd'),
			tag = cms.string(Gain_tag))),
		connect = cms.string(Gain_db))
	process.Gainprefer = cms.ESPrefer("PoolDBESSource","GainsReader")

# GenError
if opt.useLocalGenErr :
	process.GenErrReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('SiPixelGenErrorDBObjectRcd'),
			tag = cms.string(GenErr_tag))),
		connect = cms.string(GenErr_db))
	process.generrprefer = cms.ESPrefer("PoolDBESSource","GenErrReader")

#---------------------------
#  Schedule
#---------------------------

# Modify Schedule
if opt.useRECO:
	process.schedule = cms.Schedule(process.myAnalyzer_step)
else:
	if not opt.saveRECO:
		process.schedule.remove(process.RECOSIMoutput_step)
	else:
		process.RECOSIMoutput.fileName = opt.RECOFileName
		# Additionally, save the pixel sim hits and digis too
		process.RECOSIMoutput.outputCommands.extend(('keep *_g4SimHits_*Pixel*_*','keep *_simSiPixelDigis_*_*','drop *_mix_simSiPixelDigis*_*'))
	# Remove unnecessary steps and add Analyzer in the end of the chain
	process.schedule.remove(process.endjob_step)
	process.schedule.remove(process.genfiltersummary_step)
	process.schedule.append(process.myAnalyzer_step)
# End of inserted code
















#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
