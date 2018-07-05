import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process("Test",eras.Phase2)

process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D17_cff')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')



process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
#    lastRun = cms.untracked.uint32(1),
#    timetype = cms.string('runnumber'),
#    interval = cms.uint32(1),
    firstRun = cms.untracked.uint32(1)
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root")
                                   )

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag  import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

# DB stuff 
useLocalDB = True
if useLocalDB :
  process.DBReader = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(
		cms.PSet(
			record = cms.string("SiPixelLorentzAngleRcd"),
#			record = cms.string("SiPixelLorentzAngleSimRcd"),

			tag = cms.string("SiPixelLorentzAngle_phase2_v1")
#			tag = cms.string("SiPixelLorentzAngleSim_phase2_v1")

		),
	),

    connect = cms.string('sqlite_file:SiPixelLorentzAngle_phase2_v1.db')
    #connect = cms.string('sqlite_file:SiPixelLorentzAngleSim_phase2_v1.db')

  ) # end process
  process.es_prefer_DBReader = cms.ESPrefer("PoolDBESSource","DBReader")
#  end if

process.LorentzAngleReader = cms.EDAnalyzer("SiPixelLorentzAngleDBReader",
    printDebug = cms.untracked.bool(True),
#    label = cms.untracked.string("fromAlignment"),
    useSimRcd = cms.bool(False)                                    
)

#process.LorentzAngleSimReader = cms.EDAnalyzer("SiPixelLorentzAngleDBReader",
#    printDebug = cms.untracked.bool(False),
#    useSimRcd = cms.bool(True)                                             
#)

#process.p = cms.Path(process.LorentzAngleSimReader)
process.p = cms.Path(process.LorentzAngleReader)

#process.test = cms.EDAnalyzer( "SiPixelDetsPhase2")
#process.ana_seq = cms.Sequence(process.test)
#process.p = cms.Path(process.ana_seq)
