import FWCore.ParameterSet.Config as cms
import shlex, shutil, getpass
import os.path

db_file = "SiPixelDynamicInefficiency_dcol0p8.db"

tag = 'SiPixelDynamicInefficiency_dcol0p8'

Phase = 1 # If testing Phase 0, run this in 80X, There's no way I can configure this to work with Phase 0 in 81X

from Configuration.StandardSequences.Eras import eras

if Phase == 0:
    process = cms.Process("SiPixelInclusiveBuilder",eras.Run2_25ns)
    process.load("Configuration.StandardSequences.GeometryDB_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
elif Phase == 1:
    process = cms.Process("SiPixelInclusiveBuilder",eras.Run2_2017)
    process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
    process.load('Configuration.StandardSequences.GeometrySimDB_cff')
    process.XMLFromDBSource.label = cms.string("Extended")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    from Configuration.AlCa.GlobalTag import GlobalTag
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_design', '')

process.load("FWCore.MessageService.MessageLogger_cfi")

print "GlobalTag = "+str(process.GlobalTag.globaltag)

process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.source = cms.Source("EmptyIOVSource",
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    timetype = cms.string('runnumber'),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

#to get the user running the process
user = getpass.getuser()
print '\n-> Uploading as user %s into file %s\n' % (user, db_file)
if os.path.isfile(db_file):
    shutil.move(db_file, "siPixelDynamicInefficiency_old.db")

##### DATABASE CONNNECTION AND INPUT TAGS ######
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('.'),
        connectionRetrialPeriod = cms.untracked.int32(10),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(1),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(0),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False)
   ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('sqlite_file:' + db_file),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelDynamicInefficiencyRcd'),
        tag = cms.string(tag)
    ))
)

###### DYNAMIC INEFFICIENCY OBJECT ######
if Phase == 0:
    process.SiPixelDynamicInefficiencyDBLoader = cms.EDAnalyzer("SiPixelDynamicInefficiencyDB",
        thePixelGeomFactors = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.double(0.999)
            ),
        ),
        theColGeomFactors = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.double(0.999)
            ),
        ),
        theChipGeomFactors = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.double(0.999)
            ),
        ),
        thePUEfficiency = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                layer = cms.uint32(1),
                factor = cms.vdouble(1, -0.00131768, -0.00105849),
            ),
            cms.PSet(
                det = cms.string("bpix"),
                layer = cms.uint32(2),
                factor = cms.vdouble(1, -0.00206698),
            ),
            cms.PSet(
                det = cms.string("bpix"),
                layer = cms.uint32(3),
                factor = cms.vdouble(1, -0.0011765),
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.vdouble(1),
            ),
        ),
        theInstLumiScaleFactor = cms.untracked.double(0.32743),
    )
elif Phase == 1:
    process.SiPixelDynamicInefficiencyDBLoader = cms.EDAnalyzer("SiPixelDynamicInefficiencyDBLoader",
        thePixelGeomFactors = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.double(1)
            ),
        ),
        theColGeomFactors = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                layer = cms.uint32(1),
                factor = cms.double(0.8)
            ),
            cms.PSet(
                det = cms.string("bpix"),
                layer = cms.uint32(2),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("bpix"),
                layer = cms.uint32(3),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("bpix"),
                layer = cms.uint32(4),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.double(1)
            ),
        ),
        theChipGeomFactors = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                factor = cms.double(1)
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.double(1)
            ),
        ),
        thePUEfficiency = cms.untracked.VPSet(
            cms.PSet(
                det = cms.string("bpix"),
                factor = cms.vdouble(1),
            ),
            cms.PSet(
                det = cms.string("fpix"),
                factor = cms.vdouble(1),
            ),
        ),
        theInstLumiScaleFactor = cms.untracked.double(0),
    )

# InstLumiScaleFactor
#mb_xsec = 71.3
#l_lhc = 27
#c = 300000
#nbx = 2076
#l_lhc / c
#-->
#instlumi/<pile-up> [10e27 cm-2s-1/pu] = nbx * c /l_lhc / mb_xsec = 0.3235 [10e33 cm-2s-1/pile-up]

process.p = cms.Path(
    process.SiPixelDynamicInefficiencyDBLoader
)

