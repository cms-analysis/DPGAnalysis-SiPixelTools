import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("SiPixelTemplateDBReaderTest")

process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("CalibTracker.SiPixelESProducers.SiPixelTemplateDBObjectESProducer_cfi")

#Load the correct Magnetic Field
# Control the template selection either through the run number or by explicitly 
# using the specific mag field map
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
#process.load("Configuration.StandardSequences.MagneticField_cff") # same


process.source = cms.Source("EmptySource",
#   firstRun = cms.untracked.uint32(1), # 
   firstRun = cms.untracked.uint32(258000), # for prompt 2015D 
#   firstRun = cms.untracked.uint32(256004), # 2015D @ 0T
#   firstRun = cms.untracked.uint32(257058), # 2015D @ 0T
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

#process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#process.GlobalTag.globaltag = "MC_70_V4::All"
process.GlobalTag.globaltag = "74X_dataRun2_Prompt_v3"

# use a local sqlite file instead of GT
testLocal = True
if testLocal :
  process.DBReader = cms.ESSource("PoolDBESSource",
     process.CondDBSetup,
        toGet = cms.VPSet(cms.PSet(
          label = cms.untracked.string('0T'),
          record = cms.string('SiPixelTemplateDBObjectRcd'),
#          tag = cms.string('SiPixelTemplateDBObject38Tv10')
          tag = cms.string('SiPixelTemplateDBObject0Tv10')
#          tag = cms.string('SiPixelTemplateDBObject_0T_2015_v1_hltvalidation')
#          tag = cms.string('SiPixelTemplateDBObject0Tv2_express')
#          tag = cms.string('SiPixelTemplateDBObject_0T_v3_offline')
        )),
        timetype = cms.string('runnumber'),
#     connect = cms.string('sqlite_file:../../../../../DB/310815/SiPixelTemplateDBObject_38T_2015_v3.db')
     connect = cms.string('sqlite_file:../../../../../DB/310815/SiPixelTemplateDBObject_0T_2015_v3.db')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    )
    #process.PoolDBESSource.DBParameters.authenticationPath='.'
    #process.PoolDBESSource.DBParameters.messageLevel=0
  process.es_prefer_DBReader = cms.ESPrefer("PoolDBESSource","DBReader")
# end if

process.reader = cms.EDAnalyzer("SiPixelTemplateDBReader",
                    siPixelTemplateCalibrationLocation = cms.string("."),
                    wantDetailedTemplateDBErrorOutput = cms.bool(False),
                    wantFullTemplateDBOutput = cms.bool(True),
                    useBFieldFromGlobalTag = cms.bool(True)
                   )

process.p = cms.Path(process.reader)






