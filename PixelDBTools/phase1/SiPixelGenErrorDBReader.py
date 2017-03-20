import FWCore.ParameterSet.Config as cms
import sys

from Configuration.StandardSequences.Eras import eras

process = cms.Process("SiPixelGenErrorDBReader",eras.Run2_2017)

process.load("CondCore.DBCommon.CondDBSetup_cfi") # needed for custom tags

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
# process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
# process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.GeometryIdeal_cff")
#process.load("Configuration.Geometry.GeometryDB_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')


process.source = cms.Source("EmptySource",
#    firstRun = cms.untracked.uint32(1), #  
#    firstRun = cms.untracked.uint32(210000), #  
#    firstRun = cms.untracked.uint32(238000), #  iov1 
#    firstRun = cms.untracked.uint32(239000), # iov2 in offline  
    firstRun = cms.untracked.uint32(258000), # iov2 in prompt
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

#testGlobalTag = False
#if testGlobalTag :
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.XMLFromDBSource.label = cms.string("Extended")
from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v3', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_75_V1A', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'FT_R_74_V15B', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, '75X_upgrade2017_design_v4', '')
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')


# for local sqlite files
testTag = True
if testTag :
#else:
#    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.DBReader = cms.ESSource("PoolDBESSource",
       process.CondDBSetup,
       toGet = cms.VPSet(
         cms.PSet(
           record = cms.string('SiPixelGenErrorDBObjectRcd'),
#           tag = cms.string('SiPixelGenErrorDBObject_38T_v1_offline')
#           tag = cms.string('SiPixelGenErrorDBObject_38T_v2_express')
#           tag = cms.string('SiPixelGenErrorDBObject_38T_2015_v1_hltvalidation')
#           tag = cms.string('SiPixelGenErrorDBObject_0T_2015_v1_hltvalidation')
#           tag = cms.string('SiPixelGenErrorsDBObject_38T_2015_v4')
#           tag = cms.string('SiPixelGenErrorDBObject38Tv1')
#           tag = cms.string('SiPixelGenErrorDBObject38Tv3')
#           tag = cms.string('SiPixelGenErrorDBObject0Tv10')
#           tag = cms.string('SiPixelGenErrorDBObject0Tv3')
#           tag = cms.string('SiPixelGenErrorDBObject_phase1_38T_2017_v1')
           tag = cms.string('SiPixelGenErrorDBObject_phase1_00T_2017_v1')
#           MC
#           tag = cms.string('SiPixelGenErrorDBObject38TV10')
#           tag = cms.string('SiPixelGenErrorDBObject_phase1_38T_mc_v1')
#           tag = cms.string('SiPixelGenErrorDBObject_phase1_38T_mc_v2')
         )),
        #timetype = cms.string('runnumber'),
        # DATA 
        #connect = cms.string('sqlite_file:../../../../../DB/siPixelGenErrors38T_2012_IOV7_v1.db')
        #connect = cms.string('sqlite_file:../../../../../DB/siPixelGenErrors38T_IOV8a.db')
#        connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/310815/siPixelGenErrorsNM0Tv10.db')
#        connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/210915/SiPixelGenErrorDBObject_38T_2015_v4.db')
#        connect = cms.string('sqlite_file:SiPixelGenErrorsDBObject_38T_2015_v4.db')
#        connect = cms.string('sqlite_file:SiPixelGenErrorsDBObject_38T_mc_v1.db')
        #connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
        #connect = cms.string('frontier://FrontierProd/CMS_COND_PIXEL_000')
        #connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
        #connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
        #connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
#        connect = cms.string('sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase1/2017_03_20/SiPixelGenErrorDBObject_phase1_38T_2017_v1.db')
        connect = cms.string('sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase1/2017_03_20/SiPixelGenErrorDBObject_phase1_00T_2017_v1.db')
        # MC
        #connect = cms.string('sqlite_file:../../../../../DB/siPixelGenErrors38T_v1_mc.db')
        #connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/phase1/SiPixelGenErrorDBObject_phase1_38T_mc_v1.db')
        #connect = cms.string('sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase1/2017_02_13/SiPixelGenErrorDBObject_phase1_38T_mc_v2.db')

    )
    #process.PoolDBESSource.DBParameters.authenticationPath='.'
    #process.PoolDBESSource.DBParameters.messageLevel=0

    process.es_prefer_DBReader = cms.ESPrefer("PoolDBESSource","DBReader")

#end if

process.reader = cms.EDAnalyzer("SiPixelGenErrorDBReader",
                     siPixelGenErrorCalibrationLocation = cms.string(""),
#Change to True if you would like a more detailed error output
#wantDetailedOutput = False
#Change to True if you would like to output the full GenError database object
#wantFullOutput = False
                     wantDetailedGenErrorDBErrorOutput = cms.bool(True),
                     wantFullGenErrorDBOutput = cms.bool(True)
                 )

process.p = cms.Path(process.reader)






