# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: RECO -s RAW2DIGI,L1Reco,RECO --data --scenario pp --conditions 92X_dataRun2_Prompt_forTier0Replay_PixelLocalReco_TkAl_newVCal_v2 --era Run2_2017 --process NTUPLE --eventcontent RECO --datatier RECO --filein /store/data/Tier0_REPLAY_vocms015/ZeroBias/RECO/PromptReco-v141/00000/6658E50E-9A65-E711-A4F9-02163E011C17.root --python_filename=run_Resolution_ReReco_Data_92X_cfg.py --runUnscheduled -n 10
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('NTUPLE',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(16)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/044C579F-7E7E-E711-B516-02163E0126FD.root",
#"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/044C579F-7E7E-E711-B516-02163E0126FD.root",
#'/store/express/Run2018E/ExpressPhysics/FEVT/Express-v1/000/325/421/00000/2E50CCBF-9043-9447-8008-8950E7DB2FF2.root',
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/020/00000/04CED47A-B152-E711-AF94-02163E012BB1.root",
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/101/00000/FAED7D95-0D54-E711-BD0C-02163E014169.root",
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/101/00000/FA2D4A91-BF53-E711-9042-02163E0140ED.root",
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/101/00000/F6A63CD1-0D54-E711-9E22-02163E014416.root",
"/store/relval/CMSSW_10_6_0_pre4/ZeroBias/RECO/106X_dataRun2_tracker2017_v1_RelVal_zb2017B-v2/20000/F8F9DFBE-89CF-E84A-BFBF-34D112ACBBB9.root"
#"/store/user/tvami/Monitoring/SingleMuon/crab_crab_2016pp_RECORun281727_v3/190220_103737/0000/RAW2DIGI_L1Reco_RECO_6.root"
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/101/00000/F4D24CD1-BF53-E711-9703-02163E0119D2.root",
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/101/00000/F4D10111-BD53-E711-9BC4-02163E0144EE.root",
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/101/00000/F468EB03-BD53-E711-A093-02163E01194B.root",
#"/store/data/Run2017B/ZeroBias/RAW/v1/000/297/101/00000/F43B15A7-0D54-E711-84C2-02163E011AEE.root",
#'/store/express/Run2018C/ExpressPhysics/FEVT/Express-v1/000/319/450/00000/2455990D-0384-E811-AF79-FA163E2091AE.root',

),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('RECO nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('RECO_RAW2DIGI_L1Reco_RECO.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_tracker2017_v1', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Insert resolution stuff here

# Refitter
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitter)

#------------------------------------------
#  Define your Analyzer(s) here
#------------------------------------------

# BPix Resolution
process.BPixResolution_Template = cms.EDAnalyzer('Pixel_BPix_phase1',
    triggerSource = cms.InputTag('TriggerResults::HLT'),
    ttrhBuilder = cms.string('WithAngleAndTemplate')
)
process.BPixResolution_Generic = process.BPixResolution_Template.clone(
    ttrhBuilder = cms.string('WithTrackAngle')
)

# FPix Resolution
process.FPixResolution_Template = cms.EDAnalyzer('Pixel_FPix_phase1',
    triggerSource = cms.InputTag('TriggerResults::HLT'),
    ttrhBuilder = cms.string('WithAngleAndTemplate'),
    doBPix = cms.bool(False),
    doFPix = cms.bool(True)
)
process.FPixResolution_Generic = process.FPixResolution_Template.clone(
    ttrhBuilder = cms.string('WithTrackAngle')
)

# TFileService used for both BPix/FPix resolution
process.TFileService = cms.Service('TFileService',
    fileName = cms.string("Resolution.root"),
)

# Paths
process.BPixResolution_step = cms.Path(process.BPixResolution_Template*process.BPixResolution_Generic)
process.FPixResolution_step = cms.Path(process.FPixResolution_Template*process.FPixResolution_Generic)

# Schedule definition
process.schedule = cms.Schedule(
#    process.raw2digi_step,
#    process.L1Reco_step,
#    process.reconstruction_step,
    process.TrackRefitter_step,
    process.FPixResolution_step,
    process.BPixResolution_step
    )

# end of insert to cmsDriver script

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
