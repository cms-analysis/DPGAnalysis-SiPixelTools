#
# rechits are not persisent anymore, so one should run one of the CPEs
# on clusters ot do the track fitting. 11/08 d.k.
#
import FWCore.ParameterSet.Config as cms
process = cms.Process("recHitsTest")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
# to use no All 
# Choose the global tag here:
# 2012
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag.globaltag = autoCond['run1_data']
#process.GlobalTag.globaltag = 'GR_R_73_V0A'
# 2015
#process.GlobalTag.globaltag = autoCpnd['run2_data']
# 2015
#process.GlobalTag.globaltag = 'GR_P_V49::All'
# 2016
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8' # for 272

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('ReadPixelRecHit'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)

process.source = cms.Source("PoolSource",
   fileNames =  cms.untracked.vstring(
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/rechits/rechits2_postls171.root'
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/314/00000/1A57B62A-4328-E611-9D78-02163E01339F.root",
   )
)

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('274314:97-274314:9999')

# a service to use root histos
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histos_rechits.root')
)

# read rechits
# from RecoLocalTracker
#process.analysis = cms.EDAnalyzer("ReadPixelRecHit",
# from DPGAnalysis-SiPixelTools
process.analysis = cms.EDAnalyzer("PixRecHitTest",
    Verbosity = cms.untracked.bool(True),
    src = cms.InputTag("siPixelRecHits"),
)

process.p = cms.Path(process.analysis)

# test the DB object, works
#process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff")
##process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
##process.load("CalibTracker.SiPixelESProducers.SiPixelFakeTemplateDBObjectESSource_cfi"
##process.load("CalibTracker.SiPixelESProducers.SiPixelFakeCPEGenericErrorParmESSource_cfi"
#process.test = cms.EDAnalyzer("CPEAccessTester",
##    PixelCPE = cms.string('PixelCPEGeneric'),
#    PixelCPE = cms.string('PixelCPETemplateReco'),
#)
#process.p = cms.Path(process.test)





