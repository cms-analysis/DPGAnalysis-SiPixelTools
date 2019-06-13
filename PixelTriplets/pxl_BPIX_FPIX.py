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
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
 
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/044C579F-7E7E-E711-B516-02163E0126FD.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/06066EA4-7E7E-E711-A454-02163E01A30E.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/0C862C9C-7E7E-E711-A925-02163E0127B8.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/0EACF186-7E7E-E711-BCB0-02163E01A34F.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/10792C8C-7E7E-E711-BB06-02163E014150.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/1498068F-7E7E-E711-AB1C-02163E01A1FE.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/16B3528A-7E7E-E711-B493-02163E019CC5.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/186B2A9E-7E7E-E711-9AD7-02163E014298.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/1EDC54AC-7E7E-E711-AFEF-02163E0139B6.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/20499197-7E7E-E711-B536-02163E01471F.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/241C9B84-7E7E-E711-82E5-02163E01A69D.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/42B695AE-7E7E-E711-A41B-02163E011800.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/4468C29B-7E7E-E711-8D22-02163E01459A.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/44EC1FB0-7E7E-E711-9709-02163E011F56.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/46556D9D-7E7E-E711-85A0-02163E01A37E.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/4E08AF9B-7E7E-E711-BA78-02163E019E02.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/54103189-7E7E-E711-959B-02163E01A273.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/56F57A8F-7E7E-E711-8FC7-02163E01A42B.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/58317A89-7E7E-E711-981D-02163E01A5DA.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/60BE7C8D-7E7E-E711-89E3-02163E019B54.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/6482BC8F-7E7E-E711-8D41-02163E019B21.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/6ED73490-7E7E-E711-B775-02163E01A2C2.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/6EFDB27C-7F7E-E711-A869-02163E0145AE.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/7256308A-7E7E-E711-95D7-02163E01A21D.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/765C708B-7E7E-E711-B8B6-02163E019E66.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/787837A1-7E7E-E711-BE7B-02163E012528.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/7A210A9C-7E7E-E711-894F-02163E014132.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/7C99589C-7E7E-E711-9F2A-02163E011DD7.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/7E57F7AB-7E7E-E711-BB8C-02163E01241A.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/7EA7B089-7E7E-E711-ACC4-02163E01A479.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/8034548E-7E7E-E711-BE9D-02163E01366B.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/8655F5CF-7E7E-E711-BB27-02163E0118B6.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/8ECF5D90-7E7E-E711-B378-02163E014408.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/9276C8AC-7E7E-E711-99A7-02163E01346A.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/92DBCC8E-7E7E-E711-B3C9-02163E01A66C.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/944DF19B-7E7E-E711-B0BF-02163E01192A.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/9454CD86-7E7E-E711-A6C3-02163E01A642.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/94919C01-7F7E-E711-BB7E-02163E014725.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/9E9557DE-7E7E-E711-9C4C-02163E011E6F.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/A4814086-7E7E-E711-8E83-02163E014330.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/AC9E6D8E-7E7E-E711-80D2-02163E0134F6.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/AE093A91-7E7E-E711-9656-02163E01A665.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/AE9AA1BB-7E7E-E711-9A6B-02163E014637.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/AEBF7383-7E7E-E711-93CA-02163E014163.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/B0671B91-7E7E-E711-B891-02163E01A4B0.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/BAC51287-7E7E-E711-951E-02163E019DA0.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/BCA2DC88-7E7E-E711-B02A-02163E0146AE.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/BE290299-7E7E-E711-97CA-02163E019E12.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/C68DB08D-7E7E-E711-95F8-02163E01A2B8.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/DADCA788-7E7E-E711-80E9-02163E019B52.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/E4371D95-7E7E-E711-92EB-02163E01A5E5.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/E4A13284-7E7E-E711-B030-02163E0145F5.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/E8329190-7E7E-E711-B3FC-02163E01A4DF.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/E86E9398-7E7E-E711-AD32-02163E01A708.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/EA7305A7-7E7E-E711-8BCF-02163E011838.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/EE6C0D84-7E7E-E711-9162-02163E019E14.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/F6FE917E-7E7E-E711-B7A2-02163E01A28B.root",
"/store/data/Run2017C/SingleMuon/RECO/PromptReco-v3/000/300/780/00000/FEA56B96-7E7E-E711-A6D8-02163E01A6AC.root",



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
process.GlobalTag = GlobalTag(process.GlobalTag, '92X_dataRun2_Prompt_v8', '')

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
process.BPixResolution_Template = cms.EDAnalyzer('Pxl',
    triggerSource = cms.InputTag('TriggerResults::HLT'),
    ttrhBuilder = cms.string('WithAngleAndTemplate')
)
process.BPixResolution_Generic = process.BPixResolution_Template.clone(
    ttrhBuilder = cms.string('WithTrackAngle')
)

# FPix Resolution
process.FPixResolution_Template = cms.EDAnalyzer('Pixel_phase1',
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
    #process.raw2digi_step,
    #process.L1Reco_step,
    #process.reconstruction_step,
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
