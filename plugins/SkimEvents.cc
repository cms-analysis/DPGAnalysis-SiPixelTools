#include <memory>

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>
#include <DataFormats/VertexReco/interface/Vertex.h>
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace edm;
using namespace reco;

class SkimEvents : public EDFilter {
public:
  explicit SkimEvents(const ParameterSet&);
  ~SkimEvents();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void endJob() ;
  virtual bool filter(Event&, const EventSetup&);

  int          fVerbose;

  int          filterOnPrimaryVertex; 
  InputTag     fPrimaryVertexCollectionLabel;

  int          filterOnTracks; 
  InputTag     fTrackCollectionLabel;

  int          filterOnPixelCluster; 
  InputTag     fPixelClusterCollectionLabel;

  InputTag     fL1GTReadoutRecordLabel; 
  int          filterOnL1TriggerBits, filterOnL1TechnicalTriggerBits, filterOnHLTriggerBits;

  int fNpv, fNtk, fNpx, fL1T, fL1TT, fHLT; 
  int fNfailed, fNpassed; 
  int fEvent; 

  vector<int> fL1TriggerBits, fL1TechnicalTriggerBits, fHLTriggerBits;
};

// ----------------------------------------------------------------------
SkimEvents::SkimEvents(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  filterOnPrimaryVertex(iConfig.getUntrackedParameter<int>("filterOnPrimaryVertex", 1)),
  fPrimaryVertexCollectionLabel(iConfig.getUntrackedParameter<InputTag>("PrimaryVertexCollectionLabel", edm::InputTag("offlinePrimaryVertices"))),
  filterOnTracks(iConfig.getUntrackedParameter<int>("filterOnTracks", 1)),
  fTrackCollectionLabel(iConfig.getUntrackedParameter<InputTag>("TrackCollectionLabel", edm::InputTag("generalTracks"))),
  filterOnPixelCluster(iConfig.getUntrackedParameter<int>("filterOnPixelCluster", 1)),
  fPixelClusterCollectionLabel(iConfig.getUntrackedParameter<InputTag>("PixelClusterCollectionLabel", edm::InputTag("siPixelClusters"))),
  fL1GTReadoutRecordLabel(iConfig.getUntrackedParameter<InputTag>("L1GTReadoutRecordLabel", edm::InputTag("gtDigis"))),
  filterOnL1TriggerBits(iConfig.getUntrackedParameter<int>("filterOnL1TriggerBits", 0)),
  filterOnL1TechnicalTriggerBits(iConfig.getUntrackedParameter<int>("filterOnL1TechnicalTriggerBits", 0)),
  filterOnHLTriggerBits(iConfig.getUntrackedParameter<int>("filterOnHLTriggerBits", 0))
{
  vector<int> defa; defa.push_back(0); 
  fL1TriggerBits          = iConfig.getUntrackedParameter<vector<int> >("L1TriggerBits", defa);
  fL1TechnicalTriggerBits = iConfig.getUntrackedParameter<vector<int> >("L1TechnicalTriggerBits", defa);
  fHLTriggerBits          = iConfig.getUntrackedParameter<vector<int> >("HLTriggerBits", defa);

  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- SkimEvents constructor" << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  cout << "---  filterOnPrimaryVertex:           " << filterOnPrimaryVertex << endl;
  cout << "---  PrimaryVertexCollectionLabel:    " << fPrimaryVertexCollectionLabel << endl;
  cout << "---  filterOnTracks:                  " << filterOnTracks << endl;
  cout << "---  TrackCollectionLabel:            " << fTrackCollectionLabel << endl;
  cout << "---  filterOnPixelCluster:            " << filterOnPixelCluster << endl;
  cout << "---  PixelClusterCollectionLabel:     " << fPixelClusterCollectionLabel << endl;
  cout << "---  L1GTReadoutRecordLabel:          " << fL1GTReadoutRecordLabel << endl;
  cout << "---  filterOnL1TriggerBits:           " << filterOnL1TriggerBits << endl;
  cout << "---  filterOnL1TechnicalTriggerBits:  " << filterOnL1TechnicalTriggerBits << endl;
  cout << "---  filterOnHLTriggerBits:           " << filterOnHLTriggerBits << endl;
  cout << "---  fL1TriggerBits:                  "; 
  for (unsigned int i = 0; i < fL1TriggerBits.size(); ++i) cout << fL1TriggerBits[i] << "  "; 
  cout << endl;
  cout << "---  fL1TechnicalTriggerBits:         "; 
  for (unsigned int i = 0; i < fL1TechnicalTriggerBits.size(); ++i) cout << fL1TechnicalTriggerBits[i] << "  "; 
  cout << endl;
  cout << "---  fL1TriggerBits:                  "; 
  for (unsigned int i = 0; i < fHLTriggerBits.size(); ++i) cout << fHLTriggerBits[i] << "  "; 
  cout << endl;
  cout << "----------------------------------------------------------------------" << endl;

  fNpv = fNtk = fNpx = fL1T = fL1TT = fHLT = 0; 
  fEvent = fNfailed = fNpassed = 0; 
  
}


// ----------------------------------------------------------------------
SkimEvents::~SkimEvents() {

}



// ----------------------------------------------------------------------
bool SkimEvents::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {    
  bool result(false);
  ++fEvent; 

  int goodL1Technical(0); 
  if (filterOnL1TechnicalTriggerBits || filterOnL1TriggerBits) {
    Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
    iEvent.getByLabel(fL1GTReadoutRecordLabel,L1GTRR);
    
    if (filterOnL1TechnicalTriggerBits) {
      for (unsigned int iTrig = 0; iTrig < fL1TechnicalTriggerBits.size(); ++iTrig) {
	goodL1Technical = L1GTRR->technicalTriggerWord()[fL1TechnicalTriggerBits[iTrig]]; 
	if (goodL1Technical) {
	  if (fVerbose > 1) cout << "==>SkimEvents> trigger bit " << fL1TechnicalTriggerBits[iTrig] << " set" << endl;
	  goodL1Technical = fL1TechnicalTriggerBits[iTrig];
	  result = true; 
	  break;
	} 
      }
    }
    
    int l1flag(0); 
    if (filterOnL1TriggerBits) {
      for (unsigned int iTrig = 0; iTrig < fL1TriggerBits.size(); ++iTrig) {
	l1flag = L1GTRR->decisionWord()[fL1TriggerBits[iTrig]]; 
	if (l1flag) result = true; 
	break;
      }
    }

    // -- when filtering on trigger bits, skip rest if untriggered event
    if (!result) {
      if (fVerbose > 1) cout << "==>SkimEvents> trigger bits not set, skipping event" << endl;
      ++fNfailed;
      return result; 
    }
      
  }

  

//   if (filterOnHLTriggerBits) {
//     Handle<TriggerResults> hHLTresults;
//     bool hltF = true;
//     try {
//       iEvent.getByLabel(fHLTResultsLabel, hHLTresults);
//     } catch (cms::Exception &ex) {
//       //    cout << ex.explainSelf() << endl;
//       cout << "==>SkimEvents> Triggerresults  " << fHLTResultsLabel << " not found " << endl;
//       hltF = false;
//     }
    
//     if (hltF && hHLTresults.isValid()) {
//       TriggerNames triggerNames;
//       triggerNames.init(*hHLTresults);
//       fHLT = hHLTresults->accept();
//       if (fVerbose > 5) cout << "hHLTresults->size() = " << hHLTresults->size() << " and HLT accept = " << fHLT << endl;
      
//       vector<string>  hlNames;
//       hlNames = triggerNames.triggerNames();
      
//       int hltacc(0), hltrun(0), hlterr(0), itrig(0); 
//       for (unsigned int iTrig = 0; iTrig < hlNames.size(); ++iTrig) {
// 	hltacc = hHLTresults->accept(iTrig); 
//       }
//     }
//   }


  edm::Handle<reco::VertexCollection> hVertices;
  int goodVertices(-1);
  try{ 
    iEvent.getByLabel(fPrimaryVertexCollectionLabel, hVertices);
    const reco::VertexCollection vertices = *(hVertices.product());
    goodVertices = 0; 
    for (unsigned int i = 0; i < vertices.size(); ++i) {
      if (vertices[i].isFake()) {
      } else {
	++goodVertices;
      }
    }
  } catch (cms::Exception &ex) {
    if (fVerbose > 1) cout << "No Vertex collection with label " << fPrimaryVertexCollectionLabel << endl;
  }

  edm::Handle<reco::TrackCollection> hTracks;
  int goodTracks(-1);
  try{ 
    iEvent.getByLabel(fTrackCollectionLabel, hTracks);
    goodTracks = (*(hTracks.product())).size();
  } catch (cms::Exception &ex) {
    if (fVerbose > 1) cout << "No Track collection with label " << fTrackCollectionLabel << endl;
  }

  int goodPixel(-1);
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > hPixelClusters;
  try{
    iEvent.getByLabel(fPixelClusterCollectionLabel, hPixelClusters);
    const edmNew::DetSetVector<SiPixelCluster> clustColl = *(hPixelClusters.product());
    goodPixel = clustColl.size(); 
  } catch (cms::Exception &ex) {
    if (fVerbose > 1) cout << "No SiPixelCluster collection with label " << fPixelClusterCollectionLabel << endl;
  } 

  bool goodPv = (goodVertices >= filterOnPrimaryVertex);
  if ((filterOnPrimaryVertex > 0) && goodPv) {
    result = true;
    ++fNpv;
  }

  bool goodTk = (goodTracks >= filterOnTracks);
  if ((filterOnTracks > 0) && goodTk) {
    result = true;
    ++fNtk;
  }

  bool goodPx = (goodPixel >= filterOnPixelCluster);
  if ((filterOnPixelCluster > 0) && goodPx) {
    result = true;
    ++fNpx;
  }

  if (fVerbose > 1) {
    char line[20]; 
    sprintf(line, "%7d", fEvent);
    cout << "SkimEvents: " << line
	 << " result: " << (result? "true ":"false")
	 << " PV: " << (goodPv?goodVertices:0)
	 << " Tk: " << (goodTk?goodTracks:0)
	 << " Px: " << (goodPx?goodPixel:0)
	 << " L1TT: " << (goodL1Technical?goodL1Technical:0)
	 << endl;
  }

  if (result) {
    ++fNpassed; 
  } else {
    ++fNfailed;
  }
  
  return result;
}


// --------------------------------------------------------------------------------
void  SkimEvents::beginJob(const edm::EventSetup&) {
}


// --------------------------------------------------------------------------------
void  SkimEvents::endJob() {

    cout << "SkimEvents: " 
	 << " passed events: " << fNpassed
	 << " failed events: " << fNfailed
	 << " fNpv: " << fNpv
	 << " fNtk: " << fNtk
	 << " fNpx: " << fNpx
	 << endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(SkimEvents);
