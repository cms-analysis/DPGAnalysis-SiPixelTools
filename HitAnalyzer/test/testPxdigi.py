#
import FWCore.ParameterSet.Config as cms
process = cms.Process("digiTest")

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")
# process.load("Configuration.StandardSequences.Services_cff")


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '') # no misalignment 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '') # with misalignment 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag.globaltag = '80X_dataRun2_Express_v10' # >8010


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('PixelDigisTest'),
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
#    'file:../scripts/digis4.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw2.root'
#    '/store/user/kotlinski/mu100_v74/digis/digis1.root'

# there are no digis in express
#"/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/509/00000/0094ACA0-915E-E611-9D06-02163E0143E4.root",

# mb used in 2/2019
#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/F0FF4DFA-613B-4749-BBC3-FB29AD691FA5.root"

#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_design_v4-v1/20000/C9CE0381-F732-4A4F-A263-D445DF99148C.root"

#"/store/relval/CMSSW_10_4_0/RelValMinBias_13/GEN-SIM/103X_upgrade2018_realistic_v8-v1/20000/"  #new



# PU
"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_design_v4-v1/20000/193FB1CE-333D-E540-995E-1BA38BA1CE3C.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_upgrade2018_realistic_v8-v1/20000/0D388168-67C8-A544-8A75-8A96D40C396B.root"

#"/store/relval/CMSSW_10_4_0/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_103X_mc2017_realistic_v2-v1/20000/222AF93E-CE66-6342-A266-E78F6FCDD8EC.root"



    )
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis.root')
)

  
process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(False),
# sim in V7, from sims
    src = cms.InputTag("mix"),
# MC after raw->digi
#    src = cms.InputTag("siPixelDigis"),
#####    src = cms.InputTag("simSiPixelDigis"),
)

process.p = cms.Path(process.a)

