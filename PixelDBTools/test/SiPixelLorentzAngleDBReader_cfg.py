import FWCore.ParameterSet.Config as cms
process = cms.Process("Test")

process.load("Configuration.Geometry.GeometryRecoDB_cff") # works
#process.load("Configuration.StandardSequences.GeometryDB_cff") # works

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
#    firstRun = cms.untracked.uint32(240000), # for offline
#    firstRun = cms.untracked.uint32(250000), # for prompt 
# 2016, iov 1
    firstRun = cms.untracked.uint32(270000),
# 2016, iov 2
#    firstRun = cms.untracked.uint32(281500),
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
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v15', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v5', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_v11', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v0', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')


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

# use label when reading LA from sqlite files, to force the label
# 			label = cms.untracked.string("fromAlignment"),
# 			label = cms.untracked.string("forWidth"),

#			tag = cms.string("SiPixelLorentzAngle_2015_v1")
#			tag = cms.string("SiPixelLorentzAngle_2015_v2")
#			tag = cms.string("SiPixelLorentzAngle_2015_v3")
#			tag = cms.string("SiPixelLorentzAngle_2015_v4")
#			tag = cms.string("SiPixelLorentzAngle_2015_v2_hltvalidation")
#			tag = cms.string("SiPixelLorentzAngle_2016_v1")
#			tag = cms.string("SiPixelLorentzAngle_2016_v2")
#			tag = cms.string("SiPixelLorentzAngle_2016_v3")
			tag = cms.string("SiPixelLorentzAngle_2016_v4")
#			tag = cms.string("SiPixelLorentzAngle_test") 

#			tag = cms.string("SiPixelLorentzAngle_v02_mc")

#			tag = cms.string("SiPixelLorentzAngle_forWidth_v01_mc")
#			tag = cms.string("SiPixelLorentzAngle_forWidth_v01")
#			tag = cms.string("SiPixelLorentzAngle_forWidth_v2_offline") 
#			tag = cms.string("SiPixelLorentzAngle_forWidth_v1_express") 

#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1_mc")
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1")
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1_hlt") 
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v2_offline") 
#			tag = cms.string("SiPixelLorentzAngle_fromAlignment_v1_express") 

                        # express/prompt, hlt
                        #tag = cms.string("SiPixelLorentzAngle_2009_v1_express")
                        #tag = cms.string("SiPixelLorentzAngle_2009_v1_hlt")
                        # offline
                        #tag = cms.string("SiPixelLorentzAngle_v4_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v00")
                        #tag = cms.string("SiPixelLorentzAngle_v01")
                        #tag = cms.string("SiPixelLorentzAngle_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v1_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v02_offline")
                        #tag = cms.string("SiPixelLorentzAngle_v03_offline")
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
                        # phase 1
                        #tag = cms.string("SiPixelLorentzAngleSim_0_106_612_slhc1_mc")
                        #tag = cms.string("SiPixelLorentzAngle_0_106_612_slhc1_mc")
                        #tag = cms.string("SiPixelLorentzAngle_0_106_612_slhc1_offline")

#			tag = cms.string("SiPixelLorentzAngleSim_test")
#                       tag = cms.string("SiPixelLorentzAngleSim_0_106_612_slhc1_mc")
#			tag = cms.string("SiPixelLorentzAngleSim_v02_mc")

		),
	),

#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_2015_v4.db')
#    connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/MC/SiPixelLorentzAngle_forWidth_v01_mc.db')
#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_forWidth_v01.db')
#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_fromAlignment_v01_mc.db')

#    connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/SiPixelLorentzAngle_2015_v1.db')
#    connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/SiPixelLorentzAngle_2015_v2.db')
#    connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/310815/SiPixelLorentzAngle_2015_v3.db')
#    connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/210915/SiPixelLorentzAngle_2015_v4.db')
#    connect = cms.string('sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase0/2016_11_20/SiPixelLorentzAngle_2016_v1.db')
#    connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERCALIB/Pixels/PixelDB2016/2SiPixelLorentzAngle/SiPixelLorentzAngle_2016_v2.db')
#    connect = cms.string('sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase0/2016_11_20/SiPixelLorentzAngle_2016_v3.db')
    connect = cms.string('sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase0/2016_11_20/SiPixelLorentzAngle_2016_v4.db')
#    connect = cms.string('sqlite_file:SiPixelLorentzAngle_test.db')
#    connect = cms.string('sqlite_file:SiPixelLorentzAngleSim_test.db')

#    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PIXEL')
#    connect = cms.string('frontier://FrontierPrep/CMS_COND_PIXEL')
#    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#    connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
  ) # end process
  process.es_prefer_DBReader = cms.ESPrefer("PoolDBESSource","DBReader")

#  end if

process.LorentzAngleReader = cms.EDAnalyzer("SiPixelLorentzAngleDBReader",
    printDebug = cms.untracked.bool(False),
#   use label to read labeled object from DB
#    label = cms.untracked.string("fromAlignment"),
#    label = cms.untracked.string("forWidth"),
    useSimRcd = cms.bool(False)                                    
)

process.LorentzAngleSimReader = cms.EDAnalyzer("SiPixelLorentzAngleDBReader",
    printDebug = cms.untracked.bool(False),
    useSimRcd = cms.bool(True)
                                             
)

#process.p = cms.Path(process.LorentzAngleReader*process.LorentzAngleSimReader)
#process.p = cms.Path(process.LorentzAngleSimReader)
process.p = cms.Path(process.LorentzAngleReader)


