#
import FWCore.ParameterSet.Config as cms

process = cms.Process("d")

import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# accept if 'path_1' succeeds
process.hltfilter = hlt.hltHighLevel.clone(
# Min-Bias
#    HLTPaths = ['HLT_Physics_v*'],
#    HLTPaths = ['HLT_Random_v*'],
#    HLTPaths = ['HLT_ZeroBias*'],
#    HLTPaths = ['HLT_L1Tech54_ZeroBias*'],
# Commissioning:
#    HLTPaths = ['HLT_L1_Interbunch_BSC_v*'],
#    HLTPaths = ['HLT_L1_PreCollisions_v1'],
#    HLTPaths = ['HLT_BeamGas_BSC_v*'],
#    HLTPaths = ['HLT_BeamGas_HF_v*'],
# LumiPixels
#    HLTPaths = ['AlCa_LumiPixels_Random_v*'],
#    HLTPaths = ['AlCa_LumiPixels_ZeroBias_v*'],
#    HLTPaths = ['AlCa_LumiPixels_v*'],
    
# examples
#    HLTPaths = ['p*'],
#    HLTPaths = ['path_?'],
    andOr = True,  # False = and, True=or
    throw = False
    )

# process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('dumper'),
    destinations = cms.untracked.vstring('cout'),
#    destinations = cms.untracked.vstring("log","cout"),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
#    log = cms.untracked.PSet(
#        threshold = cms.untracked.string('DEBUG')
#    )
)
#process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = 'Debug'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('raw.root')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(                          
#    "file:../../../../../CMSSW_7_1_3/src/DPGAnalysis-SiPixelTools/HitAnalyzer/test/raw.root"
#    "file:/afs/cern.ch/work/d/dkotlins/public/MC/mu/pt100_71_pre7/raw/raw2.root"

"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/027EE06B-D8CE-E411-8E83-02163E012A80.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/02D738CA-D7CE-E411-8500-02163E012576.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/06013B3A-D8CE-E411-9F80-02163E0125E2.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/2E60760C-E9CE-E411-86DA-02163E0134B2.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/3A79D614-D9CE-E411-A8A9-02163E01216F.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/46931501-D8CE-E411-A9C7-02163E0126C4.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/487A5EC5-D7CE-E411-87E1-02163E0126FD.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/5000E87B-CDCE-E411-8CA3-02163E012391.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/540BBBB6-D8CE-E411-AEA9-02163E01208D.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/60C1DEC6-D8CE-E411-8CE8-02163E01275B.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/66D05AFF-CDCE-E411-B0BF-02163E01295E.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/688A18FE-D7CE-E411-A055-02163E012260.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/74F348CC-D7CE-E411-B535-02163E012A8E.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/76FF043B-D1CE-E411-A419-02163E012732.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/7CA73FDB-E8CE-E411-A786-02163E0134D6.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/8889C149-D8CE-E411-BD20-02163E0128D6.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/8ED40C4E-D8CE-E411-BB99-02163E011D41.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/90E6CFDB-CDCE-E411-81A0-02163E01216F.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/9444CE71-CDCE-E411-8C08-02163E0122AB.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/9A94DCD8-E8CE-E411-B713-02163E013678.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/A0637634-D8CE-E411-A7B2-02163E012B87.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/A20BB75E-CDCE-E411-9000-02163E0125D2.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/A41AA0E6-D7CE-E411-A559-02163E0123FC.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/B202BDF9-E8CE-E411-93F5-02163E012732.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/B60B04DA-D7CE-E411-9094-02163E0122B4.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/C4E62452-D8CE-E411-B5C1-02163E011D09.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/DAE96C69-D8CE-E411-B52E-02163E012014.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/DC25E5E8-E8CE-E411-A292-02163E0133C6.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/E8DD4BCA-D7CE-E411-B5C4-02163E011D1A.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/EA312B1F-CBCE-E411-B834-02163E011D09.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/F0DBB9C4-D7CE-E411-879D-02163E011CD9.root",
"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/512/00000/FA0864CF-D7CE-E411-A984-02163E012495.root",


#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/176/00000/50824D8B-D3CC-E411-B5D9-02163E0120E0.root",

# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/02D921F4-97CC-E411-AF2A-02163E012146.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/049E09FF-97CC-E411-A019-02163E012A1F.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/068D1EC7-97CC-E411-9648-02163E012A74.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/0808DD58-98CC-E411-9C67-02163E012010.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/0854BC0F-98CC-E411-9624-02163E0127FE.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/0CA31F22-98CC-E411-A665-02163E0123B3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/20A6C636-98CC-E411-A803-02163E012293.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/22E34104-98CC-E411-B69F-02163E012076.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/268517F7-97CC-E411-9DF8-02163E0120E0.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/26DC14EE-97CC-E411-BF40-02163E012445.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/307F03EE-97CC-E411-855F-02163E012BD7.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/36DBFDE6-97CC-E411-BA14-02163E0123F2.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/38F62FED-97CC-E411-B479-02163E0123EA.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/44D2FAEE-97CC-E411-9A05-02163E011D5E.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/48457FF8-97CC-E411-A2E2-02163E012B27.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/48846DEE-97CC-E411-B38A-02163E0124BE.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/4ADC300D-98CC-E411-8A1E-02163E012BBF.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/5025E8ED-97CC-E411-A856-02163E011866.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/507A04F5-97CC-E411-9230-02163E011D0C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/509CBAF4-97CC-E411-8944-02163E0128EE.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/5A53B15A-98CC-E411-9470-02163E012493.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/5EB46FF1-97CC-E411-A3AE-02163E0127CD.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/7430AAED-97CC-E411-89ED-02163E0123B9.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/7C522ADF-97CC-E411-A0BC-02163E012B7C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/7ECFEC06-98CC-E411-8EE7-02163E012524.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/7ED0E0EE-97CC-E411-B99F-02163E012952.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/842F5B0A-98CC-E411-AF15-02163E012B7C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/86F2D201-64CC-E411-918F-02163E0128DC.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/8C3BE6ED-97CC-E411-94B3-02163E012785.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/8EA85DD0-97CC-E411-AAB1-02163E011827.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/90F505F2-97CC-E411-B248-02163E012ABD.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/96D7D3D2-97CC-E411-902F-02163E011876.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/96FC1DFA-97CC-E411-BFC2-02163E012B45.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/981492CB-97CC-E411-A721-02163E0121BF.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/A619C2F6-97CC-E411-9827-02163E01208C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/B078FEEC-97CC-E411-829C-02163E011866.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/B269FC0A-98CC-E411-A585-02163E012271.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/B8E028EE-97CC-E411-B6FD-02163E0118DB.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/C27904ED-97CC-E411-8371-02163E012033.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/CA156FED-97CC-E411-B6C2-02163E0127B3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/CE7A0BED-97CC-E411-8819-02163E01203C.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/CE7CDFDA-97CC-E411-9FAE-02163E0128EC.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/D876EFEE-97CC-E411-AB77-02163E012785.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/DEC533FB-97CC-E411-A230-02163E012637.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/E2BCEB29-98CC-E411-88C0-02163E0124D3.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/E683D3DB-97CC-E411-B1F0-02163E0118BF.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/E8053893-98CC-E411-A641-02163E012408.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/E820CB7D-64CC-E411-A239-02163E0129F6.root",
# "/store/data/Commissioning2015/Cosmics/RAW/v1/000/238/081/00000/F65124F3-97CC-E411-8B31-02163E012BB8.root",


#  "/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/238/00000/FEA5C616-3AA9-E411-8348-02163E0122A3.root",

#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/4829BD7C-57B2-E411-BCC4-02163E012700.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/6C81712E-57B2-E411-A619-02163E012B6C.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/CCF57A73-57B2-E411-87FE-02163E012BC5.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/EA1A5074-57B2-E411-8418-02163E0124AC.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/ECCA0A2B-57B2-E411-8338-02163E0125D7.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/FADADC33-54B2-E411-8539-02163E0125D7.root",
#"/store/data/Commissioning2015/Cosmics/RAW/v1/000/233/886/00000/FE8E1D08-55B2-E411-AA42-02163E01181F.root",


#  "/store/data/Commissioning2015/Cosmics/RAW/v1/000/234/026/00000/029FB859-68B2-E411-8145-02163E0122AB.root",

# fill 2576
##    "rfio:/castor/cern.ch/cms/store/data/Run2012A/LP_ZeroBias/RAW/v1/000/193/092/16131715-E893-E111-8CDB-001D09F27003.root",
#  "rfio:/castor/cern.ch/cms/store/data/Run2012D/MinimumBias/RAW/v1/000/205/718/02285033-FD1B-E211-8F74-001D09F295FB. 
    )

)

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('191271:55-191271:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('191718:30-191718:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('194912:52-194912:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('194912:52-194912:330 ')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195099:61-195099:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195109:85-195109:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('195841:73-195841:100','195841:116-195841:143')
# 195774 OK from LS=0
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('198609:47-198609:112')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('201657:77-201657:9999')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205217:0-205217:323')
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('205718:49-205718:734')

process.d = cms.EDAnalyzer("SiPixelRawDump", 
    Timing = cms.untracked.bool(False),
    IncludeErrors = cms.untracked.bool(True),
#   In 2012, label = rawDataCollector, extension = _LHC                                
    InputLabel = cms.untracked.string('rawDataCollector'),
# for MC
#    InputLabel = cms.untracked.string('siPixelRawData'),
#   For PixelLumi stream                           
#    InputLabel = cms.untracked.string('hltFEDSelectorLumiPixels'),
# old
#    InputLabel = cms.untracked.string('siPixelRawData'),
#    InputLabel = cms.untracked.string('source'),
    CheckPixelOrder = cms.untracked.bool(False),
# 0 - nothing, 1 - error , 2- data, 3-headers, 4-hex
    Verbosity = cms.untracked.int32(0),
# threshold, print fed/channel num of errors if tot_errors > events * PrintThreshold, default 0,001 
    PrintThreshold = cms.untracked.double(0.001)
)

# process.p = cms.Path(process.hltfilter*process.d)
process.p = cms.Path(process.d)

# process.ep = cms.EndPath(process.out)


