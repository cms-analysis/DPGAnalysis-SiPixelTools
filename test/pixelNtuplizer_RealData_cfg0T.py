import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# DQM services
process.load("DQMServices.Core.DQM_cfg")

# Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

#fake conditions
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
#process.GlobalTag.connect ="sqlite_file:/afs/cern.ch/user/m/malgeri/public/globtag/CRZT210_V1.db"
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRAFT_V3P::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0


##
## Load and Configure TrackRefitter
##
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'cosmictrackfinderP5'
process.TrackRefitterP5.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")


##
## Load and Configure OfflineValidation
##
process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")


process.load("DPGAnalysis.SiPixelTools.pixelNtuplizer_RealData_cfi")
process.PixelNtuplizer_RealData.OutputFile = 'W41cosmictrackfinderP5TTree.root'
process.PixelNtuplizer_RealData.trajectoryInput = 'TrackRefitterP5'


process.source = cms.Source("PoolSource",
    #replace with your files
    #lastRun = cms.untracked.uint32(64768),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64119),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(        
        
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/2069CE90-AC95-DD11-8A33-001D0967D021.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/34239878-C895-DD11-B2E6-0019B9E712A3.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5C3CFB31-CA95-DD11-88A9-001D0967DE63.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5E168AAF-AF95-DD11-8D7E-001D0967CFC7.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/7E30BE58-C595-DD11-ADCD-001D0967E048.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/9C84C713-BC95-DD11-8E87-001D0967DB7A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/D25E8F11-B995-DD11-82C5-001125C48F70.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/DA8A6EF8-C395-DD11-89C5-001D096908D8.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/E2E06128-8995-DD11-8A0A-001D0967DF17.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/E8ACA634-CA95-DD11-9530-001D0967BFAE.root',
        
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/D6DF887A-A995-DD11-95B0-001125C47504.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/0631CE4A-C295-DD11-8629-0019B9E712A3.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5419BC56-A695-DD11-8AA4-001D0967512F.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5EEAA333-B695-DD11-9846-001D0968E1F8.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/8C315BEC-B295-DD11-8A70-001D0967D558.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/94A915E2-D795-DD11-B746-001D0967DAA3.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/16E504F8-0096-DD11-BC6B-0019B9E7B929.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/18D98C42-CA95-DD11-B776-001D0967D634.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/38CAFE45-8F95-DD11-B4D1-001D0967DBB6.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/3E29CFEE-A195-DD11-9B7E-0019B9E4AC6E.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/96AFD7B2-8895-DD11-8829-0019B9E4B1C8.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/0A187518-C795-DD11-9497-001D0968F2F6.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/3034D012-BC95-DD11-B14D-001D0967D986.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/441322A1-AC95-DD11-A786-0019B9E4FD07.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/90604FED-B795-DD11-B440-001D0967D63E.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/9EE9E962-B195-DD11-8A19-001D0967D9E5.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/B8B9C835-CA95-DD11-B2FD-001D0967CF95.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/CCCE7D2B-BF95-DD11-9236-001D0967D03A.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/000D980F-EA95-DD11-96F3-001D0967DF67.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/2EF30313-BC95-DD11-B5B2-001D096B0F08.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/42BB689F-CB95-DD11-9020-0019B9E5016C.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/44BFEF11-EF95-DD11-8581-001D0967DA76.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5C840D28-F795-DD11-9907-001D0967D28D.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5E151CC4-E695-DD11-A18A-0019B9E4AA47.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/7C3CD92F-0096-DD11-820D-0019B9E4FC71.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/A80FAAA9-FC95-DD11-85DD-0019B9E4FDC5.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/D249BCCD-DF95-DD11-9D34-001D0967DA76.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/E0DB2FEF-D795-DD11-B632-001D0967D571.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/128467A0-CE95-DD11-B971-001D0967D517.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/1EFE80FE-DA95-DD11-B129-001D0967D99A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/70708D41-C295-DD11-A1D9-001D0967E048.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/78852329-A095-DD11-A221-001D0967DA3A.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/06D45365-B195-DD11-B56B-001D0967DE4A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/3838B0B9-BA95-DD11-B293-001D0967DC7E.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/4044BDDA-9695-DD11-ABD3-001D096B0F08.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/406EE64B-CD95-DD11-B63E-001D0967D639.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/4EAF84CA-1596-DD11-9766-001D0967DCAB.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/ACB0E446-8F95-DD11-A6AC-001D0967DA49.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/B8D9B236-AB95-DD11-A815-0019B9E49386.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/CE6AEA7B-B495-DD11-B01B-001D096B0BDE.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/0A1C0D80-F595-DD11-8929-0019B9E4FBFE.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/16638F27-BF95-DD11-84F4-001D096908D8.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/2E861482-EA95-DD11-9F20-0019B9E49148.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/2EBD6E86-C895-DD11-9D36-001D0967CFA9.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/36F3B905-E395-DD11-9802-001D0967DF21.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/387274EC-A795-DD11-BC64-0019B9E4FD43.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5A66F20F-0496-DD11-AF16-0019B9E4FDC5.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/5E60AEBD-BA95-DD11-99EC-0019B9E7CF3A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/6C23316B-C895-DD11-A81C-001D0968E086.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/7A5C8F77-E995-DD11-AF32-001125C49178.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/80C0AFD3-B295-DD11-A514-001D0967DF67.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/84F03BCD-DF95-DD11-BB75-001D0967DF21.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/A4FA6A19-C795-DD11-9113-001D0967D1E3.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/C2743920-0496-DD11-B3D5-0019B9E7B929.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/D297D7C9-D495-DD11-BB38-001D0967D454.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/D676526B-D095-DD11-A372-0019B9E7DFA0.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/DC9E04E9-E595-DD11-B9FC-001D0967DF67.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/DE771695-E395-DD11-B5D2-001D096B0E2C.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/547582B2-AF95-DD11-91D5-0019B9E48D55.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/865062BD-B795-DD11-BA76-001D0967D19D.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/CC7F86BA-B795-DD11-BFA1-001D0967D0DA.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/04669BEA-EF95-DD11-AC11-001D0967D7DD.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/0CBB2F48-0096-DD11-B3DD-001D0968E1E9.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/0CCCA59D-D395-DD11-BE83-0019B9E712A3.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/24F0ED6E-D395-DD11-923C-001D0967DF2B.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/36083293-C895-DD11-B999-0019B9E4AD9A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/3AC78ACE-DF95-DD11-BFED-001D0967D454.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/42345849-CD95-DD11-A3DB-0019B9E4FDDE.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/4669E85E-D095-DD11-AD45-001D0967D53F.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/609C8771-A995-DD11-933A-001D0967DFB2.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/68E1C567-0996-DD11-8F47-001D0967DADF.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/7A0E808E-CB95-DD11-BBCA-001D0967DA71.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/88BC2EC4-B295-DD11-B162-001D0967DCF1.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/906CA019-E395-DD11-ABFC-0019B9E712A3.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/9CFB56E2-D795-DD11-8DC9-001D0967D517.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/AA2C6D6E-D395-DD11-AB4C-001D0969096E.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/B8DD80AA-D395-DD11-84E7-0019B9E4FDBB.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/EA705B05-ED95-DD11-8BFA-0019B9E4FBFE.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/F283C503-C495-DD11-91D2-0019B9E4FF5A.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/629C9BCF-0796-DD11-B0A7-001D0967DF3F.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/6ECB2CCD-D495-DD11-9132-001D096B0E2C.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/00E902F4-D795-DD11-9128-001D0967D3D2.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/02C858B8-DC95-DD11-BA58-001D096B0E2C.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/06EB7DE7-CF95-DD11-BD60-001D0967D03A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/188CBFBF-EE95-DD11-8D38-0019B9E713FC.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/6856C60B-EB95-DD11-AFD9-0019B9E4FDC5.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/6A9D4304-E695-DD11-A011-0019B9E48DA0.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/6AB30E3D-C295-DD11-8FDA-001D0967DFB7.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/6CCFF06E-EA95-DD11-87A8-001D0967DADF.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/74F07538-E195-DD11-9EB1-001D096760DE.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/7CC01A90-EE95-DD11-91D9-001D0967DADF.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/80CF9301-D895-DD11-99A7-0019B9E7DD74.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/8ADD482C-C795-DD11-8031-001D0967D99A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/AA41F7A7-F595-DD11-81BA-0019B9E4FDC5.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/AEB17455-ED95-DD11-B5D1-001D0967DAAD.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/BE437397-FB95-DD11-ABD4-0019B9E7B929.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/C42562A8-AF95-DD11-8EFF-001D0967D6B1.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/DE1E6EB1-AF95-DD11-AA16-001D0967D337.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/448E0BED-A195-DD11-ADE5-001125C48FF2.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/664E5306-A595-DD11-B5F9-001D0967CEF5.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/D023F964-B195-DD11-AB17-001D0967DC92.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/DA99107F-C895-DD11-9B12-0019B9E7B96F.root',

        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/9AA417E8-A195-DD11-97E7-0019B9E4FC1C.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/9EA9F4CA-A195-DD11-AAD6-001D0967DA3A.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/B292DDF6-0796-DD11-8BCC-001D0968E1E9.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/B80C127D-F595-DD11-AC67-0019B9E713FC.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRUZET4_V6P_v1/0006/E2FB8CC2-B295-DD11-B4F3-001D0967DFB7.root'

    ) )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitterP5*process.PixelNtuplizer_RealData)
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'Info'
process.TrackerDigiGeometryESModule.applyAlignment = True
