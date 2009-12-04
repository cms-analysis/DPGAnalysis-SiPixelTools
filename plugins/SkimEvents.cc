#include <memory>

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>
#include <DataFormats/VertexReco/interface/Vertex.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <iostream>

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

  int fNpv, fNtk, fNpx; 
  int fNfailed, fNpassed; 
  int fEvent; 

};

// ----------------------------------------------------------------------
SkimEvents::SkimEvents(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  filterOnPrimaryVertex(iConfig.getUntrackedParameter<int>("filterOnPrimaryVertex", 1)),
  fPrimaryVertexCollectionLabel(iConfig.getUntrackedParameter<InputTag>("PrimaryVertexCollectionLabel", edm::InputTag("offlinePrimaryVertices"))),
  filterOnTracks(iConfig.getUntrackedParameter<int>("filterOnTracks", 1)),
  fTrackCollectionLabel(iConfig.getUntrackedParameter<InputTag>("TrackCollectionLabel", edm::InputTag("generalTracks"))),
  filterOnPixelCluster(iConfig.getUntrackedParameter<int>("filterOnPixelCluster", 1)),
  fPixelClusterCollectionLabel(iConfig.getUntrackedParameter<InputTag>("PixelClusterCollectionLabel", edm::InputTag("siPixelClusters")))
{
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- SkimEvents constructor" << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  cout << "---  filterOnPrimaryVertex:           " << filterOnPrimaryVertex << endl;
  cout << "---  PrimaryVertexCollectionLabel:    " << fPrimaryVertexCollectionLabel << endl;
  cout << "---  filterOnTracks:                  " << filterOnTracks << endl;
  cout << "---  TrackCollectionLabel:            " << fTrackCollectionLabel << endl;
  cout << "---  filterOnPixelCluster:            " << filterOnPixelCluster << endl;
  cout << "---  PixelClusterCollectionLabel:     " << fPixelClusterCollectionLabel << endl;
  cout << "----------------------------------------------------------------------" << endl;

  fNpv = fNtk = fNpx = 0; 
  fEvent = fNfailed = fNpassed = 0; 
  
}


// ----------------------------------------------------------------------
SkimEvents::~SkimEvents() {

}



// ----------------------------------------------------------------------
bool SkimEvents::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {    
  bool result(false);
  ++fEvent; 

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
	 << " PV: " << (goodPv?1:0)
	 << " Tk: " << (goodTk?1:0)
	 << " Px: " << (goodPx?1:0)
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
	 << "passed events: " << fNpassed
	 << "failed events: " << fNfailed
	 << "fNpv: " << fNpv
	 << "fNtk: " << fNtk
	 << "fNpx: " << fNpx
	 << endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(SkimEvents);
