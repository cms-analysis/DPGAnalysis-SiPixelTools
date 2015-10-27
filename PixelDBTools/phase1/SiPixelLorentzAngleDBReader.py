import FWCore.ParameterSet.Config as cms
process = cms.Process("Test")

#process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2017_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
#    lastRun = cms.untracked.uint32(1),
#    timetype = cms.string('runnumber'),
#    interval = cms.uint32(1),
#    firstRun = cms.untracked.uint32(1)
#    firstRun = cms.untracked.uint32(180000),
# iov 1
#    firstRun = cms.untracked.uint32(190000),
#    firstRun = cms.untracked.uint32(190703),
# iov 2
#    firstRun = cms.untracked.uint32(197000),
# iov 3
#    firstRun = cms.untracked.uint32(200000),
# iov 4 
#    firstRun = cms.untracked.uint32(202000),
# iov 5
#    firstRun = cms.untracked.uint32(204000),
# iov 6
#    firstRun = cms.untracked.uint32(206000),
# iov 7
#    firstRun = cms.untracked.uint32(208686),
#  2015, iov 1
    firstRun = cms.untracked.uint32(240000),
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
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.GlobalTag import GlobalTag
# works with condDB and condDBv2
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS1', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_73_V7', '')

#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_upgrade2017_design_v4', '')

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

# 			label = cms.untracked.string("fromAlignment"),
# 			label = cms.untracked.string("forWidth"),

#			tag = cms.string("SiPixelLorentzAngle_2015_v1")
#			tag = cms.string("SiPixelLorentzAngle_2015_v2")
#			tag = cms.string("SiPixelLorentzAngle_test") 

#			tag = cms.string("SiPixelLorentzAngle_v02_mc")

#			tag = cms.string("SiPixelLorentzAngle_forWidth_v0_mc")
#			tag = cms.string("SiPixelLorentzAngle_forWidth_v01")

#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1_mc")
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1")
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1_offline") 
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1_express") 
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1_hlt") 

                        # express/prompt, hlt
                        #tag = cms.string("SiPixelLorentzAngle_2009_v1_express")
                        #tag = cms.string("SiPixelLorentzAngle_2009_v1_hlt")
                        # offline
                        #tag = cms.string("SiPixelLorentzAngle_v00")
                        #tag = cms.string("SiPixelLorentzAngle_v01")
                        #tag = cms.string("SiPixelLorentzAngle_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v1_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v02_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v03_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v4_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v5_offline")
                        # in v4_offline
                        #tag = cms.string("SiPixelLorentzAngle_r194912_v1")
                        #tag = cms.string("SiPixelLorentzAngle_r196364_v1")
                        #tag = cms.string("SiPixelLorentzAngle_r200091_v2")
                        #tag = cms.string("SiPixelLorentzAngle_r202237_v2")
                        #tag = cms.string("SiPixelLorentzAngle_r203987_v2")
                        #tag = cms.string("SiPixelLorentzAngle_2012_IOV6")
                        #tag = cms.string("SiPixelLorentzAngle_2012_IOV7")
                        # in v5_offline
                        #tag = cms.string("SiPixelLorentzAngle_r194912_v2")
                        #tag = cms.string("SiPixelLorentzAngle_r196364_v2")
                        #tag = cms.string("SiPixelLorentzAngle_r200091_v2")
                        #tag = cms.string("SiPixelLorentzAngle_r202237_v2")
                        #tag = cms.string("SiPixelLorentzAngle_r203987_v2")
                        #tag = cms.string("SiPixelLorentzAngle_2012_IOV6")
                        #tag = cms.string("SiPixelLorentzAngle_2012_IOV7")
			#tag = cms.string("SiPixelLorentzAngle_test")
			#tag = cms.string("dupa")
                        # phase 1 MC
                        tag = cms.string("SiPixelLorentzAngle_phase1_mc_v1")
                        #tag = cms.string("SiPixelLorentzAngle_0_106_612_slhc1_mc")
                        #tag = cms.string("SiPixelLorentzAngle_0_106_612_slhc1_offline")

#                       tag = cms.string("SiPixelLorentzAngleSim_phase1_mc_v1")
#			tag = cms.string("SiPixelLorentzAngleSim_v02_mc")

		),
	),

#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_forWidth_v01.db')
#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_fromAlignment_v01.db')
#    connect = cms.string('sqlite_file:../../../../../DB/SiPixelLorentzAngle_2015_v1.db')
#    connect = cms.string('sqlite_file:../../../../../DB/SiPixelLorentzAngle_2015_v2.db')
#    connect = cms.string('sqlite_file:../test/SiPixelLorentzAngle_test.db')
#   MC 
#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_forWidth_v01_mc.db')
#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_fromAlignment_v01_mc.db')
#   MC phase1
#    connect = cms.string('sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db')
    connect = cms.string('sqlite_file:SiPixelLorentzAngle_phase1_mc_v1.db')
#    connect = cms.string('sqlite_file:SiPixelLorentzAngleSim_phase1_mc_v1.db')
#    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
#    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')

  ) # end process
  process.es_prefer_DBReader = cms.ESPrefer("PoolDBESSource","DBReader")

#  end if

process.LorentzAngleReader = cms.EDAnalyzer("SiPixelLorentzAngleDBReader",
    printDebug = cms.untracked.bool(True),
#    label = cms.untracked.string("fromAlignment"),
    useSimRcd = cms.bool(False)                                    
)

process.LorentzAngleSimReader = cms.EDAnalyzer("SiPixelLorentzAngleDBReader",
    printDebug = cms.untracked.bool(False),
    useSimRcd = cms.bool(True)
                                             
)

#process.p = cms.Path(process.LorentzAngleReader*process.LorentzAngleSimReader)
#process.p = cms.Path(process.LorentzAngleSimReader)
process.p = cms.Path(process.LorentzAngleReader)


