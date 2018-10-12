import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("SiPixelDets",eras.Run2_2017)

process.load("Configuration.Geometry.GeometryRecoDB_cff")

#Load the correct Magnetic Field
# Control the template selection either through the run number or by explicitly 
# using the specific mag field map
#process.load("Configuration.StandardSequences.MagneticField_0T_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load("Configuration.StandardSequences.MagneticField_cff") # same

# turn off alignment 
# (1) does not work  
#process.load('Geometry.TrackerGeometryBuilder.idealForDigiTrackerGeometryDB_cff')

# (2) 
#process.load('Geometry.TrackerGeometryBuilder.trackerGeometryDB_cfi')
# does not define trackerGeometryDB
# this does, needs false 
#from Geometry.TrackerGeometryBuilder.trackerGeometryDB_cfi import trackerGeometryDB
#trackerGeometryDB.applyAlignment = cms.bool(False)

# (3) DOES not  work
#process.load('RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi')
#from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import trackerGeometryDB
#trackerGeometryDB.applyAlignment = cms.bool(False)

# (4) works, needs false
#from Configuration.StandardSequences.GeometryDB_cff import trackerGeometryDB
#trackerGeometryDB.applyAlignment = cms.bool(False)


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')  # phase 0
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '') # phase 0
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '') # phase 1 WRONG in 92X
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '') # phase 1 OK
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '') # phase 1
#process.GlobalTag.globaltag = '92X_dataRun2_Express_v7' # 
#process.GlobalTag.globaltag = '101X_dataRun2_Express_v7' # 
# design & realistic give same module positions 


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


process.source = cms.Source("EmptySource",
   firstRun = cms.untracked.uint32(315000), #must be number from phase1 2017
)
 	      
# FileService is mandatory, as the following analyzer module 
# will want it, to create output histogram file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test.root")
)

# the analyzer itself - empty parameter set 
process.test = cms.EDAnalyzer( "SiPixelDets",
         phase1=cms.untracked.bool(True),   
#        NPartForHisto = cms.untracked.int32(100),
#        PtMaxForHisto = cms.untracked.double(200.0)
)

process.p1 = cms.Path( process.test )

