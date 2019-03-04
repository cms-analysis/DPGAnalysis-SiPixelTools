import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("PixelGainsDBReader",eras.Run2_2017)

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("CondTools.SiPixel.SiPixelGainCalibrationService_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# works with condDB and condDBv2
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_73_V7', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS1', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
# for phase1 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '') #same
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_design', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')
process.GlobalTag.globaltag = '103X_mc2017_design_IdealBS_v2' # mc 2017
#process.GlobalTag.globaltag = '103X_mc2017_realistic_v2' # mc 2017
#process.GlobalTag.globaltag = '103X_upgrade2018_design_v4' # mc 2018
#process.GlobalTag.globaltag = '103X_upgrade2018_realistic_v8' # mc 2018
# 2018
#process.GlobalTag.globaltag = '101X_dataRun2_Express_v8' # data 2018

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root")
                                   )


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.source = cms.Source("EmptySource",
# timetype = cms.string('runnumber'),
#    numberEventsInRun = cms.untracked.uint32(10),
#    select the IOV from the global tag
#    firstRun = cms.untracked.uint32(190000)  # iov1-2012
#    firstRun = cms.untracked.uint32(200000)  # iov2-2012
#    firstRun = cms.untracked.uint32(208000)  # iov3-2012
#    firstRun = cms.untracked.uint32(240000)  # iov1-2015
#    firstRun = cms.untracked.uint32(200000)  # v4-2017
#    firstRun = cms.untracked.uint32(304000)  # v6-2017
#    firstRun = cms.untracked.uint32(313000)  # iov1-2018 v1
#    firstRun = cms.untracked.uint32(319000)  # 2018 V2
#    firstRun = cms.untracked.uint32(319940)  # 2018 V3 (short)
#    firstRun = cms.untracked.uint32(320000)  # 2018 V4
#    firstRun = cms.untracked.uint32(323000)  # 2018 V6
#    firstRun = cms.untracked.uint32(324000)  # 2018 V7
    firstRun = cms.untracked.uint32(327000)  # 2018 V9
)

#process.Timing = cms.Service("Timing")

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(0)
#)

# DB stuff 
useLocalDB = True
if useLocalDB:
  process.GainsReader = cms.ESSource("PoolDBESSource",
  #    process.CondDBCommon,
    DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0),
       authenticationPath = cms.untracked.string('')
    ),
    toGet = cms.VPSet(
      cms.PSet(
       record = cms.string('SiPixelGainCalibrationOfflineRcd'),
#       record = cms.string('SiPixelGainCalibrationForHLTRcd'),
#       record = cms.string('SiPixelGainCalibrationRcd'),
#       record = cms.string('SiPixelGainCalibrationOfflineSimRcd'),  
#       record = cms.string('SiPixelGainCalibrationForHLTSimRcd'),  

#       Phase1-MC
       tag = cms.string('SiPixelGainCalibration_phase1_mc_v3')
#       tag = cms.string('SiPixelGainCalibration_phase1_mc_v2')
#       tag = cms.string('SiPixelGainCalibration_phase1_ideal_v2')
#       tag = cms.string('SiPixelGainCalibrationSim_phase1_ideal_v2')
#
#       tag = cms.string('SiPixelGainCalibration_r203368_offline')
#       tag = cms.string('SiPixelGainCalibration_r197749_offline')
#       tag = cms.string('SiPixelGainCalib_2009CollRuns_offline')
#       tag = cms.string('SiPixelGainCalibration_2016_v1_offline')
#       tag = cms.string('SiPixelGainCalibration_2016_v1_HLT')  # HLT
#       tag = cms.string('SiPixelGainCalibration_2016_v2_HLT')  # HLT
#       tag = cms.string('SiPixelGainCalibration_2017_v4')
#       tag = cms.string('SiPixelGainCalibration_2017_v1_offline')
#       tag = cms.string('SiPixelGainCalibration_2017_v4_offline')
#       tag = cms.string('SiPixelGainCalibration_2017_v4_1337_offline')
#       tag = cms.string('SiPixelGainCalibration_2017_v5') # for offline
#       tag = cms.string('SiPixelGainCalibration_2018_v1_offline')
#       tag = cms.string('SiPixelGainCalibration_2018_v2_offline')
#       tag = cms.string('SiPixelGainCalibration_2018_v3_offline') # the tag in sqlite is v1 for v3
#       tag = cms.string('SiPixelGainCalibration_2018_v4_offline') # the tag in sqlite 
#       tag = cms.string('SiPixelGainCalibration_2018_v44_offline') # test tag 
#       tag = cms.string('SiPixelGainCalibration_2018_v2_fine_offline') #  
#       tag = cms.string('SiPixelGainCalibration_2018_vv3full_offline') #  
#       tag = cms.string('SiPixelGainCalibration_2018_v4_fine_offline') #  
#       tag = cms.string('SiPixelGainCalibration_2018_v4full_offline') # tag for v6 
#       tag = cms.string('SiPixelGainCalibration_2018_v7_offline') # tag for v7
#       tag = cms.string('SiPixelGainCalibration_2018_v7') # tag for v7 in dev and prod
#       tag = cms.string('SiPixelGainCalibration_2018_v8_offline') # tag for v8 
#       tag = cms.string('SiPixelGainCalibration_2018_v8') # tag for v8 
#       tag = cms.string('SiPixelGainCalibration_2018_v9_offline') # tag for v9 
#       tag = cms.string('SiPixelGainCalibration_2018_v9') # tag for v9 
#       tag = cms.string('SiPixelGainCalibration_2018_v1')
#       tag = cms.string('SiPixelGainCalibration_2018_v2')
#       tag = cms.string('SiPixelGainCalibration_2018_v3') # v3 in db 
#       tag = cms.string('SiPixelGainCalibration_2018_v4')
#       tag = cms.string('SiPixelGainCalibration_2018_v5')
    )),
#     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#     connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
#     connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/SiPixelGainCalibration_phase1_mc_v2.db')
#     connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/SiPixelGainCalibration_phase1_ideal_v2.db')
#     connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/SiPixelGainCalibrationSim_phase1_ideal_v2.db')
     connect = cms.string('sqlite_file:/afs//cern.ch/work/d/dkotlins/public/DB/Gains/MC/gain_slope_0p15.db')
#     connect = cms.string('sqlite_file:gain.db')
#   2018
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v1_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v2_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v3_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v4_offline.db')
#   after granularity fix 
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v2_fine_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v3_fine_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v4_fine_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v6_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v7_offline.db')
#     connect = cms.string('sqlite_file:/afs/cern.ch/user/d/dkotlins/WORK/DB/Gains/SiPixelGainCalibration_2018_v8_offline.db')

#     connect = cms.string('sqlite_file:/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_2815552613/GainRun_2815552613/SiPixelGainCalibration_2018_v4fineRes_offline.db')
#     connect = cms.string('sqlite_file:/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_325585/GainRun_325585/SiPixelGainCalibration_2018_v9_offline.db')

  ) # end process
  # process.prefer("PoolDBESSource")
  process.myprefer = cms.ESPrefer("PoolDBESSource","GainsReader")
# end if

process.SiPixelGainsDBReader = cms.EDAnalyzer("SiPixelGainsDBReader",
    process.SiPixelGainCalibrationServiceParameters,
#    payloadType = cms.string('HLT'),
#    payloadType = cms.string('Full'),
    payloadType = cms.string('Offline'),
    useSimRcd = cms.bool(False),
#    useSimRcd = cms.bool(True),
    verbose = cms.bool(False),
    maxRangeDeadPixHist = cms.untracked.double(0.001)

)

process.p = cms.Path(process.SiPixelGainsDBReader)
