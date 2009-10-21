// -*- C++ -*-
//
// Package:    FEDInRunFilter
// Class:      FEDInRunFilter
// 
/**\class FEDInRunFilter FEDInRunFilter.cc DPGAnalysis/SiPixelTools/src/FEDInRunFilter.cc

 Description: <Filter to select on good runs >

 Implementation:
    Class selects on if the pixel was in the run and if so also checks all feds are in.
*/
//
// Original Author:  Freya Blekman
//         Created:  Tue Mar  3 19:15:02 CET 2009
// $Id: FEDInRunFilter.cc,v 1.3 2009/03/04 16:41:12 fblekman Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
// the record file contains the DB definitions of RunInfo and RunSummary.
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"

#include <iostream>
//
// class declaration
//

class FEDInRunFilter : public edm::EDFilter {

public:
  explicit FEDInRunFilter(const edm::ParameterSet&);
  ~FEDInRunFilter();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  std::vector<int> feds_;

  uint32_t totbpixfeds_;
  uint32_t totfpixfeds_;
  uint32_t max_badfeds_bpix_;
  uint32_t max_badfeds_fpix_;
  uint32_t max_badfeds_;

  uint64_t bookkeeping_[40];
  uint64_t totevents_;
  uint64_t goodevents_;
  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FEDInRunFilter::FEDInRunFilter(const edm::ParameterSet& iConfig)
  :
  totbpixfeds_(32),
  totfpixfeds_(8),
  max_badfeds_bpix_(iConfig.getParameter<uint32_t>("maxBadFEDsBPIX")),
  max_badfeds_fpix_(iConfig.getParameter<uint32_t>("maxBadFEDsFPIX")),
  max_badfeds_(iConfig.getParameter<uint32_t>("maxBadFEDs"))
{
   //now do what ever initialization is needed
  totevents_=0;
  goodevents_=0;
  
  for(unsigned int ifed=0; ifed<totbpixfeds_+totfpixfeds_;++ifed)
    bookkeeping_[ifed]=0;
}


FEDInRunFilter::~FEDInRunFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
FEDInRunFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{    
  // run summary, keeps track of what detectors are 'in'
  //  edm::ESHandle<RunSummary> runSummary;
  //  std::cout<<"got eshandle"<<std::endl;
  //  iSetup.get<RunSummaryRcd>().get(runSummary);



  // first: check that the pixels were in:
  


  // run info, keeps track of what FEDs are in (amongst other things)
  
  // check for record
  edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("RunInfoRcd"));
  if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
    //record not found
    edm::LogError("FEDInRunFilter::filter()") <<"Record \"RunInfoRcd"<<"\" does not exist, returning false "<<std::endl;
    return false;
  }

  edm::ESHandle<RunInfo> runInfoHandle;
  //  std::cout<<"got eshandle"<<std::endl;
  iSetup.get<RunInfoRcd>().get(runInfoHandle);
  const RunInfo *runInfo = runInfoHandle.product();

  // very loud!
  //  runInfo->printAllValues();

  // then: check that all our feds are in:
  feds_= runInfo->m_fed_in;
  uint32_t nbpixfeds=0;
  uint32_t nfpixfeds=0;
  // loop over all, they come in random order, unfortunately.
  for(size_t fedindex=0; fedindex< feds_.size(); fedindex++){
    if((uint32_t)feds_[fedindex]>=totbpixfeds_+totfpixfeds_)
      continue;
    if((uint32_t)feds_[fedindex]<totbpixfeds_)
      nbpixfeds++;
    else if((uint32_t)feds_[fedindex]<totbpixfeds_+totfpixfeds_)
      nfpixfeds++;
    // keep track of number of passed events.
    bookkeeping_[feds_[fedindex]]++;
  }
  totevents_++;
  // use hard-coded values of the total numbers of FEDs (defined in constructor)
  edm::LogInfo("FEDInRunFilter::filter()") << "number of feds in run: BPIX " << nbpixfeds << ", FPIX " << nfpixfeds << std::endl;
  if(nbpixfeds-totbpixfeds_> max_badfeds_bpix_){
    //    std::cout << "number of feds in run: BPIX " << nbpixfeds << ", FPIX " << nfpixfeds << " returning false"<< std::endl;
    return false;
  }
  if(nfpixfeds-totfpixfeds_> max_badfeds_fpix_){
    //    std::cout << "number of feds in run: BPIX " << nbpixfeds << ", FPIX " << nfpixfeds << " returning false"<< std::endl;
    return false;
  }
  if(nfpixfeds+nbpixfeds-totbpixfeds_-totfpixfeds_>max_badfeds_){
    //    std::cout << "number of feds in run: BPIX " << nbpixfeds << ", FPIX " << nfpixfeds << " returning false"<< std::endl;
    return false;
  }
  goodevents_++;
  //  std::cout<< "number of feds in run: BPIX " << nbpixfeds << ", FPIX " << nfpixfeds  << " returning true" << std::endl;
  return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
FEDInRunFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
FEDInRunFilter::endJob() {

  std::stringstream buffer;
  buffer << "FEDInRunFilter: statistics: " << std::endl;
  buffer << "total number of events processed is : " << totevents_<< ", of which " << goodevents_ << " (" << 100*goodevents_/(double)totevents_ << "%) passed the following criteria:" << std::endl;
  buffer << "max number of bad FEDs in the entire Pixel detector:" << max_badfeds_ << std::endl;
  buffer << "max number of bad FEDs in the BPIX:" << max_badfeds_bpix_ << std::endl;
  buffer << "max number of bad FEDs in the FPIX:" << max_badfeds_fpix_ << std::endl;
  buffer<< "breakdown per FED: " << std::endl;
  for(unsigned int ifed=0; ifed<totbpixfeds_+totfpixfeds_;++ifed)
    buffer << "FED " << ifed << " was in " << bookkeeping_[ifed] << " (" << 100*bookkeeping_[ifed]/(double)totevents_<< "%) events." << std::endl;
  edm::LogInfo("FEDInRunFilter")<< buffer.str() ;
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(FEDInRunFilter);
