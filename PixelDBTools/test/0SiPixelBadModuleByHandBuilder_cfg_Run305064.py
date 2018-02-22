import FWCore.ParameterSet.Config as cms
process = cms.Process("ICALIB")

process.load("Configuration.Geometry.GeometryReco_cff")
process.load("Geometry.CMSCommonData.cmsExtendedGeometry2017XML_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('')
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('sqlite_file:SiPixelQuality_Run305064.db'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('SiPixelQuality_Run305064')
    ))
)

process.prod = cms.EDAnalyzer("SiPixelBadModuleByHandBuilder",
    BadModuleList = cms.untracked.VPSet(
## # bad for SiPixelQuality_v36   
## #BPix_BpO_SEC2_LYR2_LDR3F_MOD2, TBM-A, rocs 0-7
##     cms.PSet(
##     errortype = cms.string('tbmA'),
##     detid = cms.uint32(302123800)
##     ),
## # bad for SiPixelQuality_v34      
## #BmO_Sec8_Lyr1_Ldr9F_Mod4 tbmB
##     cms.PSet(
##     errortype = cms.string('tbmB'),
##     detid = cms.uint32(302059012)
##     ), 
## # bad for SiPixelQuality_v33
## #BmO_SEC7_LYR2_LDR13_MOD3
##     cms.PSet(
##     errortype = cms.string('tbmA'),
##     detid = cms.uint32(302126344)
##     ), 

## # bad for SiPixelQuality_v32
## #BpO_Sec7_Lyr2_Ldr13F_Mod3
##     cms.PSet(
##     errortype = cms.string('tbmB'),
##     detid = cms.uint32(302126364)
##     ), 

## #still bad for v31
## #Lay1
## # BPix_BpO_SEC2_LYR1_LDR3_MOD2
##     cms.PSet(
##     errortype = cms.string('tbmA'),
##     detid = cms.uint32(302057496)
##     ), 

## #Lay2
## #Layer2
## #BPix_BmO_SEC3_LYR2LDR5_MOD4
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124292)
##     ), 

## #BPix_BmO_SEC3_LYR2LDR5_MOD3
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124296)
##     ), 
  
## #BPix_BmO_SEC3_LYR2LDR5_MOD2
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124300)
##     ), 
  
## #BPix_BmO_SEC3_LYR2LDR5_MOD1
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124304)
##     ), 
  

## #BPix_BmO_SEC3_LYR2LDR6_MOD4
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124548)
##     ), 

## #BPix_BmO_SEC3_LYR2LDR6_MOD3
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124552)
##     ), 
  
## #BPix_BmO_SEC3_LYR2LDR6_MOD2
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124556)
##     ), 
  
## #BPix_BmO_SEC3_LYR2LDR6_MOD1
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302124560)
##     ), 


## # BPix_BpO_SEC1_LYR2_LDR1H_MOD4 
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302123296) 
##     ), 

## # BPix_BpO_SEC8_LYR2_LDR16_MOD4
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302127136)
##     ), 
## # BPix_BmI_SEC1_LYR2_LDR1_MOD1
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302123024)
##     ),

## #Lay3
## # BPix_BpI_SEC1_LYR3_LDR3_MOD1
##     cms.PSet(
##     errortype = cms.string('whole'),
##     detid = cms.uint32(302188820) 
##     ), 

## #FPix_BpO_D1_BLD5_PNL2_PLQ2_ROC6 (352398856)
##     cms.PSet(
##     errortype = cms.string('none'),
##     detid = cms.uint32(352398856), 
##     badroclist = cms.vuint32(6)
##     ),

#---------------------------------------------------------------------------
),
    Record = cms.string('SiPixelQualityFromDbRcd'),
    SinceAppendMode = cms.bool(True),
    IOVMode = cms.string('Run'),
    printDebug = cms.untracked.bool(True),
    doStoreOnDB = cms.bool(True),
    ROCListFile = cms.untracked.string("DeadROC_offline305064.txt"),
)

#process.print = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path(process.prod)
#process.ep = cms.EndPath(process.print)


