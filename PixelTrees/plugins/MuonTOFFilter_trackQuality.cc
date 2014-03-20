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
// $Id: MuonTOFFilter_trackQuality.cc,v 1.6 2009/03/04 16:41:12 fblekman Exp $
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

#include "DataFormats/TrackReco/interface/Track.h"

#include <vector>

//
// class declaration
//
using namespace edm;
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

  int min_goodmuons;
  int max_goodmuons;
  double  min_time;
  double  max_time;
  double max_timeError;
  double min_trk_pt;
  double max_chi2_ndof;

  vector<double> timeAtIpInOut;
  vector<double> errorTime;


  int count_good_events;
  int count_all_events;
  int notenough_good_muons;
  int outOfTime_events;
  
  int all_muons;
  int muon_timeErrorTooLarge;
  int muon_noValidTrack;
  int muon_noValidTimeInformation;
  int muon_ptTooSmall;
  int muon_chi2TooLarge;



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
MuonTOFFilter_trackQuality::MuonTOFFilter_trackQuality(const edm::ParameterSet& iConfig)
{
  min_goodmuons = iConfig.getParameter<int>("min_goodmuons");
  max_goodmuons = iConfig.getParameter<int>("max_goodmuons");
  min_time = iConfig.getParameter<double>("min_time");
  max_time = iConfig.getParameter<double>("max_time");
  max_timeError = iConfig.getParameter<double>("max_timeError");
  min_trk_pt = iConfig.getParameter<double>("min_trk_pt");
  max_chi2_ndof = iConfig.getParameter<double>("max_chi2_ndof");
  

  
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

   count_all_events++;

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

    LogDebug("MuonTOFFilter_trackQuality") << " new event " << std::endl;
    
    for(MuonCollection::const_iterator it = MuonHandle->begin(), itEnd = MuonHandle->end(); it!=itEnd;++it){
      all_muons++;
      
      /*
      if(!it->track() == false){
	if(it->track()->chi2()/it->track()->ndof() >  max_chi2_ndof) {
	  LogDebug("MuonTOFFilter_trackQuality") << " muon rejected because chi1/ndof to large! chi2/ndof " <<it->track()->chi2()/it->track()->ndof()   << " required chi2/ndof " << max_chi2_ndof  << " chi2 " <<it->track()->chi2() << " ndof " << it->track()->ndof() <<  std::endl;
	  muon_chi2TooLarge++;
	  continue;
	}
	if(it->track()->pt() < min_trk_pt){
	  LogDebug("MuonTOFFilter_trackQuality") << " muon rejected because pt to small! pt " << it->track()->pt() << " required pt " << min_trk_pt<< std::endl;
	  muon_ptTooSmall++;
	  continue;
	}
      }
      else {
	LogDebug("MuonTOFFilter_trackQuality") << " muon has no valid track attached, reject ! " << std::endl;
	muon_noValidTrack++;
	continue;
      }
      */

      if(it->isTimeValid() == true){//check if muon has a valid time information

	if(it->time().timeAtIpInOutErr < max_timeError){//only take muons with error < max_timeError
	  LogDebug("MuonTOFFilter_trackQuality") << " muon time " <<  it->time().timeAtIpInOut << std::endl;
	  timeAtIpInOut.push_back( it->time().timeAtIpInOut);
	  errorTime.push_back(it->time().timeAtIpInOutErr);
	  good_muons++;
	}
	else {
	  LogDebug("MuonTOFFilter_trackQuality") << "error on muon time is too large, " << it->time().timeAtIpInOutErr  << " maximum error " <<  max_timeError << " reject ! " << std::endl;
	  muon_timeErrorTooLarge++;
	}
      }
      else {
       LogDebug("MuonTOFFilter_trackQuality") << "muon has no valid time information reject " << std::endl;
	muon_noValidTimeInformation++;
      }
    }//end of for loop

    if(good_muons < min_goodmuons) {
      LogDebug("MuonTOFFilter_trackQuality") << " event rejected contains not enough good muon! number of muons " << good_muons << " minimum number of good muons " << min_goodmuons << " all muons " << MuonHandle->size() << std::endl;
      notenough_good_muons++;
      return false;
    }

    

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
      LogDebug("MuonTOFFilter_trackQuality") << " event rejected because it contains to many good muons! Number muons " << good_muons << " max_number muons " <<  max_goodmuons << std::endl;
      return false;
    }

    

    //if mean time < -4 ns return false
    if(mean_Time < min_time || mean_Time > max_time) {
      LogDebug("MuonTOFFilter_trackQuality") << " event rejected because mean_time is " << mean_Time << " error " << sqrt(1/sumweight) << " min_time " <<  min_time << " max_time " << max_time <<  std::endl;
      
      outOfTime_events++;

      return false;
    }
    LogDebug("MuonTOFFilter_trackQuality") << " mean time " << mean_Time << endl;  


    count_good_events++;
    return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MuonTOFFilter_trackQuality::beginJob(const edm::EventSetup&)
{
  //counters for events
  count_good_events = 0;
  count_all_events = 0;
  notenough_good_muons = 0;
  outOfTime_events = 0;


  //counter for muons in event
  all_muons = 0;
  muon_timeErrorTooLarge = 0;
  muon_noValidTrack = 0;
  muon_noValidTimeInformation = 0;
  muon_ptTooSmall = 0;
  muon_chi2TooLarge = 0;

}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuonTOFFilter_trackQuality::endJob() {

  std::stringstream buffer;
  buffer << "MuonTOFFilter_trackQuality: statistics: " << std::endl;
  buffer << " all events " << count_all_events << " good events " << count_good_events << " percentage " << (float)count_good_events/count_all_events << std::endl;
  buffer << " events rejected because not enough good muons " << notenough_good_muons << std::endl;
  buffer << " events rejected because out of time " << outOfTime_events <<endl;
  buffer << " all muons " << all_muons << " ~muons/event " << (float)all_muons/count_all_events << std::endl;
  buffer << " percentage of muons rejected because: time error too large " << (float)muon_timeErrorTooLarge/all_muons << std::endl;
  buffer << " percentage of muons rejected because: no valid track " << (float)muon_noValidTrack/all_muons << std::endl;
  buffer << " percentage of muons rejected because: no valid time information " << (float)muon_noValidTimeInformation/all_muons << std::endl;
  buffer << " percentage of muons rejected because: pt too small " << (float)muon_ptTooSmall/all_muons << std::endl;
  buffer << " percentage of muons rejected because: chi2/ndof too large " << (float)muon_chi2TooLarge/all_muons << std::endl;

  edm::LogInfo("MuonTOFFilter_trackQuality") << buffer.str();

}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonTOFFilter_trackQuality);
