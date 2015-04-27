import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("SiPixelGenErrorDBReaderTest")

process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
# process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
# process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryDB_cff")

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

testGlobalTag = True
if testGlobalTag :
 process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
 from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
# from Configuration.AlCa.GlobalTag import GlobalTag
# works with condDB and condDBv2
 process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# for local sqlite files
else:
    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
       process.CondDBSetup,
       toGet = cms.VPSet(
         cms.PSet(
           record = cms.string('SiPixelGenErrorDBObjectRcd'),
#           tag = cms.string('SiPixelGenErrorDBObject38TV10')
#           tag = cms.string('SiPixelGenErrorDBObject38Tv1')
           tag = cms.string('SiPixelGenErrorDBObject38Tv3')
         )),
         timetype = cms.string('runnumber'),
        #connect = cms.string('sqlite_file:../../../../../DB/siPixelGenErrors38T_v1_mc.db')
        #connect = cms.string('sqlite_file:../../../../../DB/siPixelGenErrors38T_2012_IOV7_v1.db')
        connect = cms.string('sqlite_file:../../../../../DB/siPixelGenErrors38T_IOV8a.db')
    )
    process.PoolDBESSource.DBParameters.authenticationPath='.'
    process.PoolDBESSource.DBParameters.messageLevel=0


process.reader = cms.EDAnalyzer("SiPixelGenErrorDBReader",
#                     siPixelGenErrorCalibrationLocation = cms.string("./"),
                     siPixelGenErrorCalibrationLocation = cms.string(""),
#Change to True if you would like a more detailed error output
#wantDetailedOutput = False
#Change to True if you would like to output the full GenError database object
#wantFullOutput = False
                     wantDetailedGenErrorDBErrorOutput = cms.bool(True),
                     wantFullGenErrorDBOutput = cms.bool(True)
                 )

process.p = cms.Path(process.reader)






