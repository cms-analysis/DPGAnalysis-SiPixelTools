import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "GR09_E_V6::All"
process.GlobalTag.globaltag = "GR09_E_V8::All"

# -- Input files
process.source = cms.Source(
    "PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(

  '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/FE660B03-69E8-DE11-93F9-0019B9F72BAA.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/F2D1D72A-6BE8-DE11-A557-001D09F23A20.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/EC3C4DA9-67E8-DE11-A0C3-001D09F24934.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/E8C42B2A-6BE8-DE11-A36D-001D09F251FE.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/E6A991ED-6DE8-DE11-BA77-000423D94E70.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/E64455DE-64E8-DE11-9F6A-001D09F29321.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/DC39D802-69E8-DE11-9DCC-001D09F242EA.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/D8291FA9-67E8-DE11-899A-001D09F29538.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/D68F3E7A-63E8-DE11-A50C-001D09F29321.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/D4C291F7-66E8-DE11-A2C0-001D09F2525D.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/D25708F8-66E8-DE11-80A3-001D09F24DDF.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/CCAE67A4-6EE8-DE11-8C6F-001D09F24FEC.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/CAE73419-64E8-DE11-B6AA-001D09F25401.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/CA2A533D-66E8-DE11-8226-000423D9863C.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/C8F9D038-66E8-DE11-9492-001D09F252E9.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/C014435C-68E8-DE11-82CA-001D09F2527B.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/BE25A003-69E8-DE11-93BC-001D09F24493.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/BA1A63A4-6EE8-DE11-AE97-001D09F24303.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/B8FDDE3B-66E8-DE11-A009-000423D992A4.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/B21250A4-6EE8-DE11-BF8C-0019B9F70468.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/B0D8A32B-6BE8-DE11-80EC-001D09F29597.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/B04A6138-66E8-DE11-AE0B-001D09F25217.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/AE3C6FD2-69E8-DE11-8441-001D09F24EE3.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/AA960086-65E8-DE11-9220-001D09F2532F.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/A4C23F2A-6BE8-DE11-9A5A-001D09F231B0.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/A2365202-69E8-DE11-ABE9-001D09F2B2CF.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/A0470D7A-63E8-DE11-9F3E-001D09F29114.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/9C6837D7-69E8-DE11-A441-001D09F2545B.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/9813251B-64E8-DE11-9F76-001D09F2447F.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/94F87E3D-66E8-DE11-B005-001D09F24F1F.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/94F24AF4-66E8-DE11-B5AF-001617C3B778.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/8C7361A0-6CE8-DE11-9F67-000423D94C68.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/8AE33CA2-6CE8-DE11-B636-000423D99A8E.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/84C7355C-68E8-DE11-BDF1-001D09F2514F.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/84987E2B-6BE8-DE11-96C4-001D09F2462D.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/7ABC478A-6AE8-DE11-98E3-000423D992DC.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/78BF087A-63E8-DE11-A8E5-0019B9F72BAA.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/6EAC9C02-69E8-DE11-94EF-001D09F28F25.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/6404B7A3-6EE8-DE11-9F44-001D09F2423B.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/5067AE59-68E8-DE11-80FC-001617C3B6DE.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/4C8CD884-65E8-DE11-B515-001D09F231B0.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/44D5F288-6AE8-DE11-8D37-000423D99CEE.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/42BD7017-64E8-DE11-AA1E-001D09F24FBA.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/3CDEA82B-6BE8-DE11-A225-000423D6CA02.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/3A77C22A-6BE8-DE11-81A5-001D09F29533.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/38133B43-6DE8-DE11-B260-0019B9F72D71.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/2E11E19F-6CE8-DE11-A9EB-001617E30CE8.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/2C8D3B7A-63E8-DE11-9B56-001D09F291D2.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/26FC33CF-6BE8-DE11-8C83-0019DB29C5FC.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/26DC301D-64E8-DE11-A230-001D09F251BD.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/221E9B55-6FE8-DE11-BDDD-001D09F28D54.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/20463F19-64E8-DE11-99EF-001D09F231C9.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/1C8EF2EF-6DE8-DE11-B151-001D09F232B9.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/1AED53A1-62E8-DE11-B18D-001617C3B6CE.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/168B85DD-64E8-DE11-9C53-001D09F2910A.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/167C3F43-6DE8-DE11-BF89-001D09F28EA3.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/165F6E17-64E8-DE11-815E-001D09F24D4E.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/12156943-6DE8-DE11-BB2E-001D09F25041.root',
        '/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/124/120/04582C18-64E8-DE11-82C3-001D09F2546F.root'


    

    )
    )

# -- number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
    )

#process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
#process.load("Configuration.StandardSequences.Reconstruction_cff")


# -- Trajectory producer
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

# -- skimming
process.superPointingFilter = cms.EDFilter(
    "HLTMuonPointingFilter",
    SALabel = cms.string("generalTracks"),
    PropagatorName = cms.string("SteppingHelixPropagatorAny"),
    radius = cms.double(10.0),
    maxZ = cms.double(50.0)
    )




# -- the tree filler
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "collisions_900GeV_ntuplizer.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(0),
    rootFileName           = cms.untracked.string(rootFileName),
    dumpAllEvents          = cms.untracked.int32(1),
    muonCollectionLabel    = cms.untracked.InputTag('muons'),
    trajectoryInputLabel   = cms.untracked.InputTag('TrackRefitter'),
    trackCollectionLabel   = cms.untracked.InputTag('generalTracks'),
    pixelClusterLabel      = cms.untracked.InputTag('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.InputTag("gtDigis"),
    hltL1GtObjectMap       = cms.untracked.InputTag("hltL1GtObjectMap"),
    HLTResultsLabel        = cms.untracked.InputTag("TriggerResults::HLT"),
    storeRecHit            = cms.untracked.int32(1)
    )



    


# -- Path
process.p = cms.Path(
#    process.superPointingFilter*
    process.TrackRefitter*
    process.PixelTree
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'DEBUG'
#process.TrackerDigiGeometryESModule.applyAlignment = True
