# The following comments couldn't be translated into the new config version:

# this input combined rejects values above suppressZeroAndPlateausInFitFrac of the full range (0-225) from the fit. This effectively rejects plateau values

# fraction of maximum value. if 0.05 the top 0.05 fraction of the gain curve will not be included in the fit 

# option to reject points with one entry or less (so only VCAL points with two or more adc hits are taken into account... Improves fit stability but means big errors in data might be ignored

# minimum chi2/NDOF at which histograms on pixel level are saved. Set to very high or -1 to disable.

# possiblity to save ALL gain curve histograms. Makes program slow and produces HUGE output files. 
# !!!Use with care!!!!

# Database Record name...

import FWCore.ParameterSet.Config as cms

#
#  sipixelgaincalibrationanalysis.cfi
#  CMSSW configuration init file for pixel gain calibrations in CMSSW>=180
#  Original Author:  Freya Blekman
#          Created:  November 15 2007  
#
#
siPixelGainCalibrationAnalysis = cms.EDAnalyzer("SiPixelGainCalibrationAnalysis",
    saveFile = cms.untracked.bool(True),
    maxChi2InHist = cms.untracked.double(50.0),
    savePixelLevelHists = cms.untracked.bool(False),
    saveAllHistograms = cms.untracked.bool(False),
    listOfDetIDs = cms.untracked.vuint32(),                                         
    # try to create database. 'true' setting for expert use only.
    writeDatabase = cms.untracked.bool(False),
    record = cms.string('SiPixelGainCalibrationRcd'),
    suppressPointsWithOneEntryOrLess = cms.untracked.bool(True),
    #parameters common to SiPixelOfflineCalibAnalysisBase 
    DetSetVectorSiPixelCalibDigiTag = cms.InputTag("siPixelCalibDigis"),
    outputFileName = cms.string('Pixel_DQM_Calibration.root'),
    suppressZeroAndPlateausInFitFrac = cms.untracked.double(0.2),
    suppressPlateauInFit = cms.untracked.bool(True),
    minChi2NDFforHistSave = cms.untracked.double(25.0),
    minChi2ProbforHistSave = cms.untracked.double(0.001),
    plateauSlopeMax = cms.untracked.double(1.0),
    appendDatabaseMode = cms.untracked.bool(False),
    prova = cms.string('Funziona'),
    vCalValues_Int = cms.vint32( 6, 8, 10, 12, 14, 15, 16, 17, 18, 21, 24, 28, 35, 42, 49, 56, 63, 70, 77, 84, 91, 98, 105, 112, 119, 126, 133, 140, 160),
    calibcols_Int = cms.vint32(  0, 13, 26, 39, 1, 14, 27, 40, 2, 15, 28, 41, 3, 16, 29, 42, 4, 17, 30, 43, 5, 18, 31, 44, 6, 19, 32, 45, 7, 20, 33, 46, 8, 21, 34, 47, 9, 22, 35, 48, 10, 23, 36, 49, 11, 24, 37, 50, 12, 25, 38, 51),
    calibrows_Int = cms.vint32( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79),
    Repeat = cms.int32(5), 
    CalibMode = cms.string('GainCalibration'),
    # the gain is defined as 1/slope of fit.
    # maxGainInHist fixes the range of the 1D gain summary plots to [0,maxGainInHist]]
    maxGainInHist = cms.untracked.double(25.),
    useVCALHIGH = cms.bool(True),
    # conversion factor to go from VCAL_HIGH to VCAL_LOW. Current best estimate: VCAL_HIGH = 7 * VCAL_LOW, which is encoded in the parameter below 
    # vcalHighToLowConversionFac = cms.double(7.0),
    vcalHighToLowConversionFac = cms.double(6.5),
    # vcalHighToLowConversionFac = cms.double(7.15),
    # use this mode if you want to analyze S-Curve data with the Gain analysis
    ignoreMode = cms.untracked.bool(False),
    phase1 = cms.untracked.bool(False)                                   
)

