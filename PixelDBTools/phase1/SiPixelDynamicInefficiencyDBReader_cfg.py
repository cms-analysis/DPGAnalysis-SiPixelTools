import FWCore.ParameterSet.Config as cms

db_file = "siPixelDynamicInefficiency_test.db"

tag = 'SiPixelDynamicInefficiency_test'

Phase = 1

process = cms.Process("Test")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource",
    firstRun = cms.untracked.uint32(1)
)

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
if Phase == 0:
    process.load("Configuration.StandardSequences.GeometryDB_cff")
    from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
elif Phase == 1:
    process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
    process.load('Configuration.StandardSequences.GeometrySimDB_cff')
    process.XMLFromDBSource.label = cms.string("Extended")
    from Configuration.AlCa.GlobalTag import GlobalTag
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

#In case you of conditions missing, or if you want to test a specific GT
#process.GlobalTag.globaltag = 'PRE_DES72_V6'

print "GlobalTag = "+str(process.GlobalTag.globaltag)

from CondCore.DBCommon.CondDBSetup_cfi import *
process.DynIneffDBSource = cms.ESSource("PoolDBESSource",
    CondDBSetup,
    connect = cms.string('sqlite_file:' + db_file),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelDynamicInefficiencyRcd'),
        tag = cms.string(tag)
    ))
)
process.es_prefer_DynIneffDBSource = cms.ESPrefer("PoolDBESSource","DynIneffDBSource")

if Phase == 0:
    process.DynamicInefficiencyDBReader = cms.EDAnalyzer("SiPixelDynamicInefficiencyDBReader",
        Phase = cms.int32(Phase),
        thePixelColEfficiency_BPix1 = cms.double(1),
        thePixelColEfficiency_BPix2 = cms.double(1),
        thePixelColEfficiency_BPix3 = cms.double(1),
        thePixelColEfficiency_FPix1 = cms.double(0.999),
        thePixelColEfficiency_FPix2 = cms.double(0.999),
        thePixelEfficiency_BPix1 = cms.double(1),
        thePixelEfficiency_BPix2 = cms.double(1),
        thePixelEfficiency_BPix3 = cms.double(1),
        thePixelEfficiency_FPix1 = cms.double(0.999),
        thePixelEfficiency_FPix2 = cms.double(0.999),
        thePixelChipEfficiency_BPix1 = cms.double(1),
        thePixelChipEfficiency_BPix2 = cms.double(1),
        thePixelChipEfficiency_BPix3 = cms.double(1),
        thePixelChipEfficiency_FPix1 = cms.double(0.999),
        thePixelChipEfficiency_FPix2 = cms.double(0.999),
        theLadderEfficiency_BPix1 = cms.vdouble( [1]*20 ),
        theLadderEfficiency_BPix2 = cms.vdouble( [1]*32 ),
        theLadderEfficiency_BPix3 = cms.vdouble( [1]*44 ),
        theModuleEfficiency_BPix1 = cms.vdouble( [1]*4 ),
        theModuleEfficiency_BPix2 = cms.vdouble( [1]*4 ),
        theModuleEfficiency_BPix3 = cms.vdouble( [1]*4 ),
        theInnerEfficiency_FPix1 = cms.double(1),
        theInnerEfficiency_FPix2 = cms.double(1),
        theOuterEfficiency_FPix1 = cms.double(1),
        theOuterEfficiency_FPix2 = cms.double(1),
        thePUEfficiency_BPix1 = cms.vdouble(1, -0.00467063, -0.000466703),
        thePUEfficiency_BPix2 = cms.vdouble(1, -0.00245103),
        thePUEfficiency_BPix3 = cms.vdouble(1, -0.00132013),
        thePUEfficiency_FPix_Inner = cms.vdouble(1),
        thePUEfficiency_FPix_Outer = cms.vdouble(1),
        theInstLumiScaleFactor = cms.double(0.3273),
    )
elif Phase == 1:
    process.DynamicInefficiencyDBReader = cms.EDAnalyzer("SiPixelDynamicInefficiencyDBReader",
        Phase = cms.int32(Phase),
        thePixelColEfficiency_BPix1 = cms.double(1),
        thePixelColEfficiency_BPix2 = cms.double(1),
        thePixelColEfficiency_BPix3 = cms.double(1),
        thePixelColEfficiency_BPix4 = cms.double(1),
        thePixelColEfficiency_FPix1 = cms.double(1),
        thePixelColEfficiency_FPix2 = cms.double(1),
        thePixelColEfficiency_FPix3 = cms.double(1),
        thePixelEfficiency_BPix1 = cms.double(1),
        thePixelEfficiency_BPix2 = cms.double(1),
        thePixelEfficiency_BPix3 = cms.double(1),
        thePixelEfficiency_BPix4 = cms.double(1),
        thePixelEfficiency_FPix1 = cms.double(1),
        thePixelEfficiency_FPix2 = cms.double(1),
        thePixelEfficiency_FPix3 = cms.double(1),
        thePixelChipEfficiency_BPix1 = cms.double(0.5),
        thePixelChipEfficiency_BPix2 = cms.double(0.5),
        thePixelChipEfficiency_BPix3 = cms.double(0.5),
        thePixelChipEfficiency_BPix4 = cms.double(0.5),
        thePixelChipEfficiency_FPix1 = cms.double(0.5),
        thePixelChipEfficiency_FPix2 = cms.double(0.5),
        thePixelChipEfficiency_FPix3 = cms.double(0.5),
        theInnerEfficiency_FPix1 = cms.double(1),
        theInnerEfficiency_FPix2 = cms.double(1),
        theInnerEfficiency_FPix3 = cms.double(1),
        theOuterEfficiency_FPix1 = cms.double(1),
        theOuterEfficiency_FPix2 = cms.double(1),
        theOuterEfficiency_FPix3 = cms.double(1),
        thePUEfficiency_BPix1 = cms.vdouble(1),
        thePUEfficiency_BPix2 = cms.vdouble(1),
        thePUEfficiency_BPix3 = cms.vdouble(1),
        thePUEfficiency_BPix4 = cms.vdouble(1),
        thePUEfficiency_FPix1 = cms.vdouble(1),
        thePUEfficiency_FPix2 = cms.vdouble(1),
        thePUEfficiency_FPix3 = cms.vdouble(1),
        theInstLumiScaleFactor = cms.double(0),
    )

process.p = cms.Path(process.DynamicInefficiencyDBReader)
