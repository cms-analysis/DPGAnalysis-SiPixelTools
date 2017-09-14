#ifndef CondTools_SiPixel_SiPixelGainsDBReader_H
#define CondTools_SiPixel_SiPixelGainsDBReader_H
// -*- C++ -*-
//
// Package:    SiPixelGainsDBReader
// Class:      SiPixelGainsDBReader
// 
/**\class SiPixelGainsDBReader SiPixelGainsDBReader.h SiPixel/test/SiPixelGainsDBReader.h

 Description: Test analyzer for reading pixel calibration from the DB

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Vincenzo CHIOCHIA
//         Created:  Tue Oct 17 17:40:56 CEST 2006
// $Id: SiPixelGainsDBReader.h,v 1.7 2009/05/28 22:12:55 dlange Exp $
//
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationServiceBase.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

class SiPixelGainsDBReader : public edm::EDAnalyzer {

public:

  explicit SiPixelGainsDBReader( const edm::ParameterSet& iConfig);

  ~SiPixelGainsDBReader(){};
  virtual void beginJob();
  virtual void analyze(const edm::Event& , const edm::EventSetup& );
  virtual void endJob() ;

private:

  edm::ParameterSet conf_;
  edm::ESHandle<TrackerGeometry> tkgeom;
  SiPixelGainCalibrationServiceBase *SiPixelGainCalibrationService_;
  bool PRINT;

  std::map< uint32_t, TH1F* >  _TH1F_Pedestals_m;
  std::map< uint32_t, TH1F* >  _TH1F_Gains_m;
  std::map< uint32_t, double >  _deadfrac_m;
  std::map< uint32_t, double >  _noisyfrac_m;
  
  TH1F *_TH1F_Dead_sum;
  TH1F *_TH1F_Noisy_sum;
  TH1F *_TH1F_Gains_sum;
  TH1F *_TH1F_Pedestals_sum; 
  TH1F *_TH1F_Dead_all; 
  TH1F *_TH1F_Noisy_all;
  TH1F *_TH1F_Gains_all;
  TH1F *_TH1F_Pedestals_all;
  TH1F *_TH1F_Gains_bpix;
  TH1F *_TH1F_Gains_fpix;
  TH1F *_TH1F_Pedestals_bpix;
  TH1F *_TH1F_Pedestals_fpix;
  TH1D *GainsL1, *GainsL2, *GainsL3, *GainsL4, *PedsL1,*PedsL2,*PedsL3,*PedsL4; 
  TH1D *ADCL1, *ADCL2, *ADCL3, *ADCL4, *VcalL1,*VcalL2,*VcalL3,*VcalL4; 
  TH1D *GainsSumL1, *GainsSumL2, *GainsSumL3, *GainsSumL4, *PedsSumL1,*PedsSumL2,*PedsSumL3,*PedsSumL4; 
  TH1D *ADCSumL1, *ADCSumL2, *ADCSumL3, *ADCSumL4, *VcalSumL1,*VcalSumL2,*VcalSumL3,*VcalSumL4; 
};
#endif
