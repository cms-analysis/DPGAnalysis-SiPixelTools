#
import FWCore.ParameterSet.Config as cms
process = cms.Process("digiTest")

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# process.load("Configuration.StandardSequences.Services_cff")
# process.load("Configuration.StandardSequences.Services_cff")


#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#from Configuration.AlCa.GlobalTag import GlobalTag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run1_data', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')
#process.GlobalTag.globaltag = 'GR_P_V56' # for 247607

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
    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_74/digis/digis1.root'
#    'file:/afs/cern.ch/work/d/dkotlins/public/data/digis/digi_zb_248025.root'
#    '/store/user/kotlinski/mu100_v74/digis/digis1.root'
    )
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis.root')
)

  
process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(False),
# sim in V7
#    src = cms.InputTag("mix"),
# MC after raw->digi
    src = cms.InputTag("simSiPixelDigis"),
#    src = cms.InputTag("siPixelDigis"),
)

process.p = cms.Path(process.a)

