// -*- C++ -*-
//
// Package:    SimHitAnalyzer
// Class:      SimHitAnalyzer
// 
/**\class SimHitAnalyzer SimHitAnalyzer.cc test/SimHitAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
// Modified and move, dk 4/14
// Original Author:  Tommaso Boccali
//         Created:  Tue Jul 26 08:47:57 CEST 2005
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#define HEPMC // look at generator output
#ifdef HEPMC
//#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "HepPDT/ParticleDataTable.hh"
#endif

using namespace std;
using namespace edm;

//
//
// class decleration
//

class SimHitAnalyzer : public edm::EDAnalyzer {
   public:
      explicit SimHitAnalyzer( const edm::ParameterSet& );
      ~SimHitAnalyzer();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
   private:
      // ----------member data ---------------------------
  std::string HepMCLabel;
  std::string SimTkLabel;
  std::string SimVtxLabel;
  std::string SimHitLabel;
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
SimHitAnalyzer::SimHitAnalyzer( const edm::ParameterSet& iConfig ):
  HepMCLabel(iConfig.getUntrackedParameter("moduleLabelMC",std::string("FlatRandomPtGunSource"))),
  SimTkLabel(iConfig.getUntrackedParameter("moduleLabelTk",std::string("g4SimHits"))),
  SimVtxLabel(iConfig.getUntrackedParameter("moduleLabelVtx",std::string("g4SimHits"))),
  SimHitLabel(iConfig.getUntrackedParameter("moduleLabelHit",std::string("g4SimHits")))
{
   //now do what ever initialization is needed
}


SimHitAnalyzer::~SimHitAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
SimHitAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   using namespace edm;
   const float PI = 3.1416;

   std::vector<PSimHit> theTrackerHits;
   std::vector<SimTrack> theSimTracks;
   std::vector<SimVertex> theSimVertexes;

   //   Handle<HepMCProduct> MCEvt;
   Handle<SimTrackContainer> SimTk;
   Handle<SimVertexContainer> SimVtx;
   Handle<PSimHitContainer> PixelBarrelHitsLowTof;
   Handle<PSimHitContainer> PixelBarrelHitsHighTof;
   Handle<PSimHitContainer> PixelEndcapHitsLowTof;
   Handle<PSimHitContainer> PixelEndcapHitsHighTof;
   Handle<PSimHitContainer> TIBHitsLowTof;
   Handle<PSimHitContainer> TIBHitsHighTof;
   Handle<PSimHitContainer> TIDHitsLowTof;
   Handle<PSimHitContainer> TIDHitsHighTof;
   Handle<PSimHitContainer> TOBHitsLowTof;
   Handle<PSimHitContainer> TOBHitsHighTof;
   Handle<PSimHitContainer> TECHitsLowTof;
   Handle<PSimHitContainer> TECHitsHighTof;


   //   iEvent.getByLabel(HepMCLabel, MCEvt);
   iEvent.getByLabel(SimTkLabel,SimTk);
   iEvent.getByLabel(SimVtxLabel,SimVtx);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelBarrelLowTof", PixelBarrelHitsLowTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelBarrelHighTof", PixelBarrelHitsHighTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelEndcapLowTof", PixelEndcapHitsLowTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelEndcapHighTof", PixelEndcapHitsHighTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTIBLowTof", TIBHitsLowTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTIBHighTof", TIBHitsHighTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTIDLowTof", TIDHitsLowTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTIDHighTof", TIDHitsHighTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTOBLowTof", TOBHitsLowTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTOBHighTof", TOBHitsHighTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTECLowTof", TECHitsLowTof);
   iEvent.getByLabel(SimHitLabel,"TrackerHitsTECHighTof", TECHitsHighTof);


   theSimTracks.insert(theSimTracks.end(),SimTk->begin(),SimTk->end());
   theSimVertexes.insert(theSimVertexes.end(),SimVtx->begin(),SimVtx->end());
   theTrackerHits.insert(theTrackerHits.end(), PixelBarrelHitsLowTof->begin(), PixelBarrelHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), PixelBarrelHitsHighTof->begin(), PixelBarrelHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), PixelEndcapHitsLowTof->begin(), PixelEndcapHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), PixelEndcapHitsHighTof->begin(), PixelEndcapHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TIBHitsLowTof->begin(), TIBHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TIBHitsHighTof->begin(), TIBHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TIDHitsLowTof->begin(), TIDHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TIDHitsHighTof->begin(), TIDHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TOBHitsLowTof->begin(), TOBHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TOBHitsHighTof->begin(), TOBHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TECHitsLowTof->begin(), TECHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TECHitsHighTof->begin(), TECHitsHighTof->end());


#ifdef HEPMC
   Handle< HepMCProduct > EvtHandle ;
   iEvent.getByLabel( "generator", EvtHandle ) ;
   const HepMC::GenEvent* myGenEvent = EvtHandle->GetEvent() ;
   
   //Hepmc::GenEvent * myGenEvent = new  HepMC::GenEvent(*(MCEvt->GetEvent()));
   
   int i=0;
   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();
	 p != myGenEvent->particles_end(); ++p, ++i ) {
     cout<< "Particle from MC = "<<i<<" type "<< abs((*p)->pdg_id())<<" "<< (*p)->status()
	 << " ,Pt/Eta = "<< (*p)->momentum().perp()<<" "<< (*p)->momentum().eta()<<endl;  
   }   
#endif // HEPMC

   const bool printTracks = true;
   if(printTracks) {
     int i=0;
     for (std::vector<SimTrack>::iterator isimtk = theSimTracks.begin();
	  isimtk != theSimTracks.end(); ++isimtk, ++i){
       cout<<"Track "<<i<<" genIndex "<<isimtk->genpartIndex() <<" Vtx index "<<isimtk->vertIndex()
	   <<" momentum Pt "<< sqrt(isimtk->momentum().perp2())<<" eta "<< isimtk->momentum().eta()<<endl;
     }
   }
   const bool printVertices = true;
   if(printVertices) {
     int i=0;
     for (std::vector<SimVertex>::iterator isimvtx = theSimVertexes.begin();
	  isimvtx != theSimVertexes.end(); ++isimvtx, ++i){
       cout<<"Vertex "<<i
	   <<" position  x = "<<isimvtx->position().x() <<" y = "<<isimvtx->position().y() 
	   <<" z = "<< isimvtx->position().z()
	   <<" index = "<< isimvtx->vertexId()<<" parent = "<< isimvtx->parentIndex()
	   <<" process = "<<isimvtx->processType() 
	   <<endl;
     }
   }

   const bool printsHits = true;
   if(printsHits) {
     std::map<unsigned int, std::vector<PSimHit>,std::less<unsigned int> > SimHitMap;
     for (std::vector<PSimHit>::iterator isim = theTrackerHits.begin();
	  isim != theTrackerHits.end(); ++isim){
       SimHitMap[(*isim).detUnitId()].push_back((*isim));

       int detid = isim->detUnitId();
       if(detid>=302000000 && detid<303000000) {  // only bpix
	 // SimHit information 
	 float eloss = (*isim).energyLoss() * 1000000/3.7;//convert GeV to ke 
	 //float tof = (*isim).timeOfFlight();
	 float p = (*isim).pabs();
	 float theta = (*isim).thetaAtEntry();
	 float beta = 0;  // beta is roughly like real theta
	 if(theta<PI/2.) beta=(PI/2.) - theta; //  
	 else            beta=theta - (PI/2.);
	 float eta = -log(tan(beta/2.));  // this is an approximation to the tracks eta

	 //float phi = (*isim).phiAtEntry();
	 int pid = ((*isim).particleType()); 
	 int tid = (*isim).trackId();
	 int procType = (*isim).processType();
	 if(pid==13) 
	   cout<<"BPix mu Simhit id "<<pid<<" track "<<tid<<" proc "<<procType<<" de "
	       <<eloss<<" p "<<p<<" theta "<<theta<<" beta "<<beta<<" eta "<<eta
	       <<" position  x = "<<isim->localPosition().x() 
	       <<" y = "<<isim->localPosition().y() <<" z = "<< isim->localPosition().z()<<endl;
       }


     }
   }

}

//define this as a plug-in
DEFINE_FWK_MODULE(SimHitAnalyzer);
