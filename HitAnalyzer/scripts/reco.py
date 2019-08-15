

import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process("MyRECO",eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '') # no misalignment 
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '') # with misalignment 

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#       '/store/user/kotlinski/MC/mu_pt100/raw/raw1_eta0p1.root'
#       '/store/user/kotlinski/MC/mu_pt100/raw/raw2_eta0p1.root'
       '/store/user/kotlinski/MC/mu_pt100/raw/raw3_eta0p1.root'
#       '/store/user/kotlinski/MC/mu_pt100/raw/raw4_eta0p1.root'
#       '/store/user/kotlinski/MC/mu_pt100/raw/raw5_eta0p1.root'

        #'file:digis4.root'
#       'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw1_thr1k.root'
#       'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw1.root'
#       'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw2.root'
#       'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw3.root'
#       'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw4.root'
#       'file:/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/raw/raw5.root'
        ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:9000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string(
        'r.root'
#        '/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco1_thr1kclu10.root'
#        '/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco1_clu10.root'
#        '/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco1.root'
#        '/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco2.root'
#        '/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco3.root'
#        '/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco4.root'
#        '/afs/cern.ch/work/d/dkotlins/public/MC/mu_phase1/pt100/reco/reco5.root'
    ),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Parameter changes 
#process.siPixelClustersPreSplitting.VCaltoElectronGain = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset = -60
#process.siPixelClustersPreSplitting.VCaltoElectronGain_L1 = 47  # default
#process.siPixelClustersPreSplitting.VCaltoElectronOffset_L1 = -60
process.siPixelClustersPreSplitting.SeedThreshold = 10 #  def=1000
process.siPixelClustersPreSplitting.ChannelThreshold = 2 # must be bigger than 1, def=10
process.siPixelClustersPreSplitting.ClusterThreshold = 10 # def =4000    # integer?
process.siPixelClustersPreSplitting.ClusterThreshold_L1 = 10 # def=2000 # integer?

process.a = cms.EDAnalyzer("PixDigisTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
# sim in V7
#    src = cms.InputTag("mix"),
# old default
    src = cms.InputTag("siPixelDigis"),
)

process.d = cms.EDAnalyzer("PixClusterAna",
#process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Tracks = cms.InputTag("generalTracks"),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)
process.d_cosm = cms.EDAnalyzer("PixClusterAna",
#process.d = cms.EDAnalyzer("PixClusterTest",
    Verbosity = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(True),
    #src = cms.InputTag("siPixelClusters"),
    src = cms.InputTag("siPixelClustersPreSplitting"),
    Tracks = cms.InputTag(""),
    Select1 = cms.untracked.int32(0),  # cut  
    Select2 = cms.untracked.int32(0),  # value     
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('digis_clus.root')
)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.a_step = cms.Path(process.a)
process.d_step = cms.Path(process.d)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step,process.a_step,process.d_step)

#process.schedule = cms.Schedule(process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

# customisation of the process.
# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
#from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 
#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
#process = customisePostLS1(process)
# End of customisation functions

