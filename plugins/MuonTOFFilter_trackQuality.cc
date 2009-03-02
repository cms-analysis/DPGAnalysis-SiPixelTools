// -*- C++ -*-
//
// Package:    MuonTOFFilter
// Class:      MuonTOFFilter
// 
/**\class MuonTOFFilter MuonTOFFilter.cc MuonTOFFilter/MuonTOFFilter/src/MuonTOFFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Tanja ROMMERSKIRCHEN
//         Created:  Wed Feb 25 11:59:48 CET 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"

#include <vector>

//
// class declaration
//

using namespace std;
using namespace reco;

class MuonTOFFilter_trackQuality : public edm::EDFilter {
   public:
      explicit MuonTOFFilter_trackQuality(const edm::ParameterSet&);
      ~MuonTOFFilter_trackQuality();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//


  int max_goodmuons;
  double  min_time;
  double  max_time;
  double max_timeError;

vector<double> timeAtIpInOut;
vector<double> errorTime;


//
// constructors and destructor
//
MuonTOFFilter_trackQuality::MuonTOFFilter_trackQuality(const edm::ParameterSet& iConfig)
{
  max_goodmuons = iConfig.getParameter<double>("max_goodmuons");
  min_time = iConfig.getParameter<double>("min_time");
  max_time = iConfig.getParameter<double>("max_time");
  max_timeError = iConfig.getParameter<double>("max_timeError");
  

  
  errorTime[max_goodmuons];

}


MuonTOFFilter_trackQuality::~MuonTOFFilter_trackQuality()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
MuonTOFFilter_trackQuality::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

  // get the ccmuons
    edm::Handle<MuonCollection> MuonHandle;
     iEvent.getByLabel("muons", MuonHandle);
    if ( !MuonHandle.isValid() ) {
      edm::LogWarning("MuonTOFFilter_trackQuality") << " muon collection not valid  " << std::endl;
      return false;
    }

    
    int good_muons = 0;

    timeAtIpInOut.clear();
    errorTime.clear();

    for(MuonCollection::const_iterator it = MuonHandle->begin(), itEnd = MuonHandle->end(); it!=itEnd;++it){
     

      if(it->isTimeValid() == true){//check if muon has a valid time information

	if(it->time().timeAtIpInOutErr < max_timeError){//only take muons with error < max_timeError

	  timeAtIpInOut.push_back( it->time().timeAtIpInOut);
	  errorTime.push_back(it->time().timeAtIpInOutErr);
	  good_muons++;

	}

      }
    }//end of for loop

    //  for(unsigned int i = 0; i < timeAtIpInOut.size(); i++){
    //    edm::LogVerbatim("MuonTOFFilter_trackQuality") << " time " << i  << " timeAtIpInOut " << timeAtIpInOut[i] << " error " << errorTime[i] << std::endl;
    //  }


    Float_t mean_Time = 0;
    Float_t sumweight = 0;

    //caclulate mean time
    if(good_muons <= max_goodmuons){//only calc. time for events with right number of good muons

      for(unsigned int i = 0; i < timeAtIpInOut.size(); i++){
	mean_Time =  mean_Time + 1/(pow(errorTime[i],2))*timeAtIpInOut[i];
	sumweight = sumweight + 1/(pow(errorTime[i],2));
      }

      if(sumweight != 0) mean_Time = mean_Time/sumweight;
    }
    else{
      edm::LogVerbatim("MuonTOFFilter_trackQuality") << " number muons " << good_muons << " max_number muons " <<  max_goodmuons << std::endl;
    }

    

    //if mean time < -4 ns return false
    if(mean_Time < min_time || mean_Time > max_time) {
      edm::LogVerbatim("MuonTOFFilter_trackQuality") << " event rejected because mean_time is " << mean_Time << " error " << sqrt(1/sumweight) << " min_time " <<  min_time << " max_time " << max_time <<  std::endl;
      return false;
    }
    edm::LogVerbatim("MuonTOFFilter_trackQuality") << " mean time " << mean_Time << endl;  

    return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MuonTOFFilter_trackQuality::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuonTOFFilter_trackQuality::endJob() {



}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonTOFFilter_trackQuality);
