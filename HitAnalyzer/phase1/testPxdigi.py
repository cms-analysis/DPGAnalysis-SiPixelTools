#
import FWCore.ParameterSet.Config as cms

process = cms.Process("digiTest")

#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2017_cff')
process.load('Configuration.Geometry.GeometryExtended2017NewFPixReco_cff')

process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')

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
#    'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_76/digis/digis1.root'
     'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_81/digis/digis1_pixonly.root'
#     'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100_81/digis/digis1_fromraw.root'
    #'file:digis.root'
    #'file:digis_fromraw.root'
    )
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo_digis.root')
)
  
process.analysis = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# my own pixel only digis (after digi step)
    src = cms.InputTag("simSiPixelDigis"),
# after raw (my raw2digi)
#    src = cms.InputTag("siPixelDigis"),
)

process.p = cms.Path(process.analysis)

