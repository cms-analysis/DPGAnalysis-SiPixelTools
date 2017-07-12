# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: RECO -s RAW2DIGI,L1Reco,RECO --data --scenario pp --conditions 92X_dataRun2_Prompt_v4 --era Run2_2017 --process NTUPLE --eventcontent RECO --datatier RECO --filein /store/data/Run2017B/SingleMuon/RAW/v1/000/297/664/00000/9EBD2895-645C-E711-A9A5-02163E019BBE.root --python_filename=run_Resolution_ReReco_Data_92X_cfg.py --runUnscheduled -n 10
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
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2017B/SingleMuon/RAW/v1/000/297/664/00000/9EBD2895-645C-E711-A9A5-02163E019BBE.root'),
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
process.GlobalTag = GlobalTag(process.GlobalTag, '92X_dataRun2_Prompt_v4', '')

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

# Modify conditions
# Scenario 1a - Current conditions in Prompt (No VCal calibration)
#process.GlobalTag.toGet = cms.VPSet(
#    cms.PSet(record = cms.string('SiPixelGenErrorDBObjectRcd'),
#         tag = cms.string('SiPixelGenErrorDBObject_phase1_38T_2017_v5_hltvalidation'),
#         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")),
#    cms.PSet(record = cms.string('SiPixelTemplateDBObjectRcd'),
#         tag = cms.string('SiPixelTemplateDBObject_phase1_38T_2017_v5_hltvalidation'),
#         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")),
#    cms.PSet(record = cms.string('SiPixelGainCalibrationOfflineRcd'),
#         tag = cms.string('SiPixelGainCalibration_2017_v3'),
#         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")),
#    cms.PSet(record = cms.string('TrackerAlignmentRcd'),
#         tag = cms.string('TrackerAlignment_from06Jul2017ReReco_v0'),
#         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"))
#)

# Scenario 1b - Proposed conditions for 926 (New VCal calibration) 
process.GlobalTag.toGet = cms.VPSet(
    cms.PSet(record = cms.string('SiPixelGenErrorDBObjectRcd'),
         tag = cms.string('SiPixelGenErrorDBObject_phase1_38T_2017_v6'),
         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")),
    cms.PSet(record = cms.string('SiPixelTemplateDBObjectRcd'),
         tag = cms.string('SiPixelTemplateDBObject_phase1_38T_2017_v6'),
         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")),
    cms.PSet(record = cms.string('SiPixelGainCalibrationOfflineRcd'),
         tag = cms.string('SiPixelGainCalibration_2017_v3'),
         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")),
    cms.PSet(record = cms.string('TrackerAlignmentRcd'),
         tag = cms.string('TrackerAlignment_from06Jul2017ReReco_v0'),
         connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"))
)

# Refitter
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitter)

#------------------------------------------
#  Define your Analyzer(s) here
#------------------------------------------

# BPix Resolution
process.BPixResolution_Template = cms.EDAnalyzer('Pxl',
    triggerSource = cms.InputTag('TriggerResults::HLT'),
    ttrhBuilder = cms.string('WithAngleAndTemplate')
)
process.BPixResolution_Generic = process.BPixResolution_Template.clone(
    ttrhBuilder = cms.string('WithTrackAngle')
)

# FPix Resolution
process.FPixResolution_Template = cms.EDAnalyzer('Pixel',
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
    process.raw2digi_step,
    process.L1Reco_step,
    process.reconstruction_step,
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
