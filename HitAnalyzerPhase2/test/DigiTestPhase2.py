import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('digiTest',eras.Phase2)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#process.MessageLogger = cms.Service("MessageLogger",
#    debugModules = cms.untracked.vstring('siPixelRawData'),
#    destinations = cms.untracked.vstring("cout"),
#    cout = cms.untracked.PSet(
#        threshold = cms.untracked.string('ERROR')
#    )
#)
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')
# list of files

process.source = cms.Source("PoolSource",
    fileNames =  cms.untracked.vstring(
#        'file:step2_DIGI.root'
 "/store/relval/CMSSW_10_0_0_pre1/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW/PU25ns_94X_upgrade2023_realistic_v2_2023D17PU200-v1/10000/B8E397EE-E2CF-E711-929E-0242AC130002.root",
      )
)
# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step1 nevts:1'),
    name = cms.untracked.string('Applications')
)
# Output definition
process.DQMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.DQMEventContent.outputCommands,
    fileName = cms.untracked.string('step1_DigiTest.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# FileService is mandatory, as the following analyzer module 
# will want it, to create output histogram file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test.root")
)

process.test = cms.EDAnalyzer( "PixDigisTestPhase2",
#         phase1=cms.untracked.bool(True),   
#        NPartForHisto = cms.untracked.int32(100),
#        PtMaxForHisto = cms.untracked.double(200.0)
    Verbosity = cms.untracked.bool(False),
#    phase1 = cms.untracked.bool(False),
# sim in V7
#    src = cms.InputTag("mix"),
# MC after raw->digi
    src = cms.InputTag("simSiPixelDigis","Pixel"),
#    src = cms.InputTag("siPixelDigis"),

   #InnerPixelDigiSource   = cms.InputTag("simSiPixelDigis","Pixel"),
   #OuterTrackerDigiSource = cms.InputTag("mix", "Tracker"),
   #GeometryType = cms.string('idealForDigi'),
)
process.ana_seq = cms.Sequence(process.test)

#process.load('SimTracker.SiPhase2Digitizer.Phase2TrackerMonitorDigi_cff')
#process.load('SimTracker.SiPhase2Digitizer.Phase2TrackerValidateDigi_cff')
#process.digiana_seq = cms.Sequence(process.pixDigiMon * process.pixDigiValid)


process.load('DQMServices.Components.DQMEventInfo_cfi')
process.dqmEnv.subSystemFolder = cms.untracked.string('Ph2TkDigi')
process.dqm_comm = cms.Sequence(process.dqmEnv)

# Path and EndPath definitions
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.DQMoutput_step = cms.EndPath(process.DQMoutput)

#process.digi_step = cms.Sequence(process.siPixelRawData*process.siPixelDigis)
#process.p = cms.Path(process.digiana_seq * process.dqm_comm )
process.p = cms.Path(process.ana_seq)
