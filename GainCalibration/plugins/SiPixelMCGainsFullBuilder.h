#ifndef CondTools_SiPixel_SiPixelMCGainsFullBuilder_H
#define CondTools_SiPixel_SiPixelMCGainsFullBuilder_H
// -*- C++ -*-
//
// Package:    SiPixelMCGainsFullBuilder
// Class:      SiPixelMCGainsFullBuilder
//
/**\class SiPixelMCGainsFullBuilder SiPixelMCGainsFullBuilder.h SiPixel/test/SiPixelMCGainsFullBuilder.h

 Description: Test analyzer for writing pixel calibration in the DB

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Vincenzo CHIOCHIA
//         Created:  Tue Oct 17 17:40:56 CEST 2006
// $Id: SiPixelMCGainsFullBuilder.h,v 1.9 2009/05/28 22:12:54 dlange Exp $
//
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibration.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationService.h"
#include "CondFormats/SiPixelObjects/interface/PixelIndices.h"
#include <string>

  class SiPixelMCGainsFullBuilder : public edm::EDAnalyzer {
  public:
    explicit SiPixelMCGainsFullBuilder(const edm::ParameterSet& iConfig);

    ~SiPixelMCGainsFullBuilder(){};
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();
    bool loadFromFile();

  private:
    edm::ParameterSet conf_;
    bool appendMode_;
    SiPixelGainCalibration* SiPixelGainCalibration_;
    SiPixelGainCalibrationService SiPixelGainCalibrationService_;
    std::string recordName_;

    double meanPed_;
    double rmsPed_;
    double meanGain_;
    double rmsGain_;
    double secondRocRowGainOffset_;
    double secondRocRowPedOffset_;
    int numberOfModules_;
    bool fromFile_;
    std::string fileName_;

    double electronsPerVcal_;
    double electronsPerVcal_Offset_;
    double electronsPerVcal_L1_;
    double electronsPerVcal_L1_Offset_;

    // Internal class
    class CalParameters {
    public:
      float p0;
      float p1;
    };
    // Map for storing calibration constants
    std::map<int, CalParameters, std::less<int> > calmap_;
    PixelIndices* pIndexConverter_;  // Pointer to the index converter
  };

#endif
