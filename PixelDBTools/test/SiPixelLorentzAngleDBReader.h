#ifndef SiPixelLorentzAngleDBReader_H
#define SiPixelLorentzAngleDBReader_H

// system include files
//#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH2F.h"


//
//
// class decleration
//
  class SiPixelLorentzAngleDBReader : public edm::EDAnalyzer {

  public:
    explicit SiPixelLorentzAngleDBReader( const edm::ParameterSet& );
    ~SiPixelLorentzAngleDBReader();
  
    void analyze( const edm::Event&, const edm::EventSetup& );

  private:
    bool printdebug_;
    TH1F* LorentzAngleBarrel_;
    TH1F* LorentzAngleForward_;
    TH1F *LABPixL1_[8], *LABPixL2_[8], *LABPixL3_[8];

    bool useSimRcd_;
  };

#endif
