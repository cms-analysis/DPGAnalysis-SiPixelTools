import FWCore.ParameterSet.Config as cms
import sys
process = cms.Process("SiPixelTemplateDBReaderTest")

process.load("CondCore.CondDB.CondDB_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")

process.load("CalibTracker.SiPixelESProducers.SiPixelTemplateDBObjectESProducer_cfi")

#Load the correct Magnetic Field
# Control the template selection either through the run number or by explicitly 
# using the specific mag field map
#process.load("Configuration.StandardSequences.MagneticField_0T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
#process.load("Configuration.StandardSequences.MagneticField_cff") # same


process.source = cms.Source("EmptySource",
#   firstRun = cms.untracked.uint32(1), # 
#   firstRun = cms.untracked.uint32(258000), # 2015 IOV 3,  for prompt 2015D 
#   firstRun = cms.untracked.uint32(255000), # 2015 IOV 2,  for prompt 
#   firstRun = cms.untracked.uint32(252000), # 2015 IOV 1,  for prompt (from end of 2012) 
   firstRun = cms.untracked.uint32(258000), # 2015 IOV 5,  rereco 2015D 
#   firstRun = cms.untracked.uint32(256004), # 2015D @ 0T
#   firstRun = cms.untracked.uint32(257058), # 2015D @ 0T
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# works with condDB and condDBv2
# process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v3', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_75_V1A', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'FT_R_74_V15B', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# use a local sqlite file instead of GT
testLocal = True
if testLocal :
  process.DBReader = cms.ESSource("PoolDBESSource",
        toGet = cms.VPSet(cms.PSet(
          label = cms.untracked.string('0T'),
          record = cms.string('SiPixelTemplateDBObjectRcd'),
          tag = cms.string('SiPixelTemplateDBObject3p8Tv8')
#          tag = cms.string('SiPixelTemplateDBObject0Tv10')
#          tag = cms.string('SiPixelTemplateDBObject_0T_2015_v1_hltvalidation')
#          tag = cms.string('SiPixelTemplateDBObject0Tv2_express')
#          tag = cms.string('SiPixelTemplateDBObject_0T_v3_offline')
#          tag = cms.string('SiPixelTemplateDBObject_38T_2016_v1')
        )),
        timetype = cms.string('runnumber'),
#     connect = cms.string('sqlite_file:../../../../../DB/310815/SiPixelTemplateDBObject_38T_2015_v3.db')
     connect = cms.string('sqlite_file:../../../../../DB/90516/SiPixelTemplateDBObject_38T_2016_v1.db')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    )
    #process.PoolDBESSource.DBParameters.authenticationPath='.'
    #process.PoolDBESSource.DBParameters.messageLevel=0
  process.es_prefer_DBReader = cms.ESPrefer("PoolDBESSource","DBReader")
# end if

process.reader = cms.EDAnalyzer("SiPixelTemplateDBReader",
                    siPixelTemplateCalibrationLocation = cms.string(""),
                    wantDetailedTemplateDBErrorOutput = cms.bool(False),
                    wantFullTemplateDBOutput = cms.bool(True),
                    useBFieldFromGlobalTag = cms.bool(True)
                   )

process.p = cms.Path(process.reader)






