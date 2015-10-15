import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("SiPixelTemplateDBReaderTest")

process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("CalibTracker.SiPixelESProducers.SiPixelTemplateDBObjectESProducer_cfi")

#Load the correct Magnetic Field
#magfieldCffStr = "38T"
magfieldCffStr = "0T"
process.load("Configuration.StandardSequences.MagneticField_"+magfieldCffStr+"_cff")


process.source = cms.Source("EmptySource",
#   firstRun = cms.untracked.uint32(1), # 
   firstRun = cms.untracked.uint32(258000), # for prompt 2015D 
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )


## Change to False if you do not want to test the global tag
testGlobalTag = False
if testGlobalTag :
    #process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
    #process.GlobalTag.globaltag = "MC_70_V4::All"
    process.GlobalTag.globaltag = "74X_dataRun2_Prompt_v3"


#Uncomment these two lines to get from the global tag
else:
    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
      process.CondDBSetup,
        toGet = cms.VPSet(cms.PSet(
          record = cms.string('SiPixelTemplateDBObjectRcd'),
          tag = cms.string('SiPixelTemplateDBObject0Tv10')
#          tag = cms.string('SiPixelTemplateDBObject38Tv10')
        )),
        timetype = cms.string('runnumber'),
#        connect = cms.string('sqlite_file:../../../../../DB/310815/SiPixelTemplateDBObject_38T_2015_v3.db')
        connect = cms.string('sqlite_file:../../../../../DB/310815/SiPixelTemplateDBObject_0T_2015_v3.db')
    )
    process.PoolDBESSource.DBParameters.authenticationPath='.'
    process.PoolDBESSource.DBParameters.messageLevel=0

process.reader = cms.EDAnalyzer("SiPixelTemplateDBReader",
                              siPixelTemplateCalibrationLocation = cms.string("."),
                              wantDetailedTemplateDBErrorOutput = cms.bool(False),
                              wantFullTemplateDBOutput = cms.bool(True),
                              TestGlobalTag = cms.bool(testGlobalTag)
                              )

process.p = cms.Path(process.reader)






