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

// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// For ROOT
#include <TROOT.h>
//#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2.h>

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
  void beginRun();
  void beginJob();
  void endJob();
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
private:
      // ----------member data ---------------------------
  std::string HepMCLabel;
  std::string SimTkLabel;
  std::string SimVtxLabel;
  std::string SimHitLabel;

  edm::EDGetTokenT<PSimHitContainer> tPixelSimHits;
  edm::EDGetTokenT<SimTrackContainer> tSimTracks;
  edm::EDGetTokenT<SimVertexContainer> tSimVertexs;
  edm::EDGetTokenT<HepMCProduct> tHepMC;
  
  // histograms 
  TH1D *heta,*heta1,*heta2,*heta3,*heta4,*heta5,*heta6,*heta7; 
  TH1D *heta11,*heta12,*heta13,*heta14; 
  TH1D *hpid,*hpid1,*hpid2,*hpid3,*hpid4,*hpid5,*hpid6,*hpid7,*hpid8,*hpid9; 
  TH1D *hpid10,*hpid11,*hpid12,*hpid13,*hpid14;
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
  edm::InputTag tag("g4SimHits","TrackerHitsPixelBarrelLowTof");   // for the ByToken
  //edm::InputTag tag("g4SimHits","TrackerHitsPixelBarrelHighTof");   // for the ByToken
  //edm::InputTag tag("g4SimHits","TrackerHitsPixelEndcapLowTof");   // for the ByToken
  //edm::InputTag tag("g4SimHits","TrackerHitsPixelEndcapHighTof");   // for the ByToken
  tPixelSimHits = consumes <PSimHitContainer> (tag);

  edm::InputTag tag1("g4SimHits","");   // for the ByToken
  tSimTracks = consumes <SimTrackContainer> (tag1);
  edm::InputTag tag2("g4SimHits","");   // for the ByToken
  tSimVertexs = consumes <SimVertexContainer> (tag2);

  edm::InputTag tag0("generatorSmeared","");   // for the ByToken
  tHepMC = consumes <HepMCProduct> (tag0);

}


SimHitAnalyzer::~SimHitAnalyzer() {
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//
void SimHitAnalyzer::beginRun() {}
void SimHitAnalyzer::beginJob() {
  edm::Service<TFileService> fs;

  heta = fs->make<TH1D>( "heta", "eta", 30,0.,3.);
  heta1 = fs->make<TH1D>( "heta1", "eta", 30,0.,3.);
  heta2 = fs->make<TH1D>( "heta2", "eta", 30,0.,3.);
  heta3 = fs->make<TH1D>( "heta3", "eta", 30,0.,3.);
  heta4 = fs->make<TH1D>( "heta4", "eta", 30,0.,3.);
  heta5 = fs->make<TH1D>( "heta5", "eta", 30,0.,3.);
  heta6 = fs->make<TH1D>( "heta6", "eta", 30,0.,3.);
  heta11 = fs->make<TH1D>( "heta11", "eta", 30,0.,3.);
  heta12 = fs->make<TH1D>( "heta12", "eta", 30,0.,3.);
  heta13 = fs->make<TH1D>( "heta13", "eta", 30,0.,3.);
  heta14 = fs->make<TH1D>( "heta14", "eta", 30,0.,3.);

  hpid   = fs->make<TH1D>( "hpid",  "pid",2500,0.,2500.);
  hpid1  = fs->make<TH1D>( "hpid1", "pid",2500,0.,2500.);
  hpid2  = fs->make<TH1D>( "hpid2", "pid",2500,0.,2500.);
  hpid3  = fs->make<TH1D>( "hpid3", "pid",2500,0.,2500.);
  hpid4  = fs->make<TH1D>( "hpid4", "pid",2500,0.,2500.);
  hpid5  = fs->make<TH1D>( "hpid5", "pid",2500,0.,2500.);
  hpid6  = fs->make<TH1D>( "hpid6", "pid",2500,0.,2500.);
  hpid7  = fs->make<TH1D>( "hpid7", "pid",2500,0.,2500.);
  hpid8  = fs->make<TH1D>( "hpid8", "pid",2500,0.,2500.);
  hpid9  = fs->make<TH1D>( "hpid9", "pid",2500,0.,2500.);
  hpid10 = fs->make<TH1D>( "hpid10", "pid",2500,0.,2500.);
  hpid11 = fs->make<TH1D>( "hpid11", "pid",2500,0.,2500.);
  hpid12 = fs->make<TH1D>( "hpid12", "pid",2500,0.,2500.);

}
void SimHitAnalyzer::endJob() {}





// ------------ method called to produce the data  ------------
void
SimHitAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   using namespace edm;
   const float PI = 3.1416;
   const bool print = false;

   std::vector<PSimHit> theTrackerHits;
   std::vector<SimTrack> theSimTracks;
   std::vector<SimVertex> theSimVertexes;

   Handle<HepMCProduct> MCEvt;
   //   iEvent.getByLabel(HepMCLabel, MCEvt);
   iEvent.getByToken(tHepMC, MCEvt);

   Handle<SimTrackContainer> SimTk;
   //iEvent.getByLabel(SimTkLabel,SimTk); // old way 
   iEvent.getByToken( tSimTracks ,SimTk); // 
   theSimTracks.insert(theSimTracks.end(),SimTk->begin(),SimTk->end());

   Handle<SimVertexContainer> SimVtx;
   //iEvent.getByLabel(SimVtxLabel,SimVtx);
   iEvent.getByToken( tSimVertexs ,SimVtx);
   theSimVertexes.insert(theSimVertexes.end(),SimVtx->begin(),SimVtx->end());

   Handle<PSimHitContainer> PixelBarrelHitsLowTof;
   //Handle<PSimHitContainer> PixelBarrelHitsHighTof;
   //Handle<PSimHitContainer> PixelEndcapHitsLowTof;
   //Handle<PSimHitContainer> PixelEndcapHitsHighTof;
   //Handle<PSimHitContainer> TIBHitsLowTof;
   //Handle<PSimHitContainer> TIBHitsHighTof;
   //Handle<PSimHitContainer> TIDHitsLowTof;
   //Handle<PSimHitContainer> TIDHitsHighTof;
   //Handle<PSimHitContainer> TOBHitsLowTof;
   //Handle<PSimHitContainer> TOBHitsHighTof;
   //Handle<PSimHitContainer> TECHitsLowTof;
   //Handle<PSimHitContainer> TECHitsHighTof;
   iEvent.getByToken( tPixelSimHits ,PixelBarrelHitsLowTof);

   //iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelBarrelLowTof", PixelBarrelHitsLowTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelBarrelHighTof", PixelBarrelHitsHighTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelEndcapLowTof", PixelEndcapHitsLowTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsPixelEndcapHighTof", PixelEndcapHitsHighTof);

   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTIBLowTof", TIBHitsLowTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTIBHighTof", TIBHitsHighTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTIDLowTof", TIDHitsLowTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTIDHighTof", TIDHitsHighTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTOBLowTof", TOBHitsLowTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTOBHighTof", TOBHitsHighTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTECLowTof", TECHitsLowTof);
   //iEvent.getByLabel(SimHitLabel,"TrackerHitsTECHighTof", TECHitsHighTof);

   theTrackerHits.insert(theTrackerHits.end(), PixelBarrelHitsLowTof->begin(), PixelBarrelHitsLowTof->end()); 
   //theTrackerHits.insert(theTrackerHits.end(), PixelBarrelHitsHighTof->begin(), PixelBarrelHitsHighTof->end());
   //theTrackerHits.insert(theTrackerHits.end(), PixelEndcapHitsLowTof->begin(), PixelEndcapHitsLowTof->end()); 
   //theTrackerHits.insert(theTrackerHits.end(), PixelEndcapHitsHighTof->begin(), PixelEndcapHitsHighTof->end());

   //theTrackerHits.insert(theTrackerHits.end(), TIBHitsLowTof->begin(), TIBHitsLowTof->end()); 
   //theTrackerHits.insert(theTrackerHits.end(), TIBHitsHighTof->begin(), TIBHitsHighTof->end());
   //theTrackerHits.insert(theTrackerHits.end(), TIDHitsLowTof->begin(), TIDHitsLowTof->end()); 
   //theTrackerHits.insert(theTrackerHits.end(), TIDHitsHighTof->begin(), TIDHitsHighTof->end());
   //theTrackerHits.insert(theTrackerHits.end(), TOBHitsLowTof->begin(), TOBHitsLowTof->end()); 
   //theTrackerHits.insert(theTrackerHits.end(), TOBHitsHighTof->begin(), TOBHitsHighTof->end());
   //theTrackerHits.insert(theTrackerHits.end(), TECHitsLowTof->begin(), TECHitsLowTof->end()); 
   //theTrackerHits.insert(theTrackerHits.end(), TECHitsHighTof->begin(), TECHitsHighTof->end());


#ifdef HEPMC
   const bool printGenParticles = false;
   if(printGenParticles) {
     const HepMC::GenEvent* myGenEvent = MCEvt->GetEvent() ;
     if(print) cout<<" Print HepMC "<<endl;
     int i=0;
     for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();
	   p != myGenEvent->particles_end(); ++p, ++i ) {
       if(print) 
	 cout<< "Particle from MC = "<<(i+1)<<" type "<< abs((*p)->pdg_id())<<" status "<< (*p)->status()
	     << " ,Pt/Eta = "<< (*p)->momentum().perp()<<" "<< (*p)->momentum().eta()<<endl;  
     }   
   }
#endif // HEPMC

   const bool printTracks = true;
   if(printTracks) {
     int i=0;
     for (std::vector<SimTrack>::iterator isimtk = theSimTracks.begin();
	  isimtk != theSimTracks.end(); ++isimtk, ++i){

       auto pid = abs(isimtk->type());
       auto p = sqrt(isimtk->momentum().Vect().Mag2());
       auto pt =  sqrt(isimtk->momentum().Vect().Perp2());
       auto eta = abs(isimtk->momentum().eta());
       auto ch = isimtk->charge();
       auto gen = isimtk->genpartIndex();
      if(print) 
	cout<<"Track "<<(i+1)<<" genIndex "<<gen<<" Vtx index "<<isimtk->vertIndex()
	   <<" type "<<pid<<" ch "<<ch 
	   <<" p " << p<<" pt "<< pt<<" eta "<< eta
	   <<endl;

      hpid->Fill(float(pid));
      if(abs(eta)>3.0) continue; // skip
      hpid1->Fill(float(pid));

      if(ch==0) { // neutrals
	hpid2->Fill(float(pid));
	if(p<0.0001) continue; // skip netrals below 100keV
	hpid3->Fill(float(pid));
      } else { // charged
	hpid4->Fill(float(pid));
	if(pt<0.100) continue; // skip chartge below 100MeV
	hpid5->Fill(float(pid));
      }

      hpid6->Fill(float(pid));
      heta->Fill(eta);

      if( (pid==13) ||  (pid==211) ||  (pid==321) ||  (pid==2212) ) heta1->Fill(eta); // charged 
      else if( (pid==11) ) heta2->Fill(eta);   // e
      else if( (pid==22) ) heta3->Fill(eta);   // gamma
      else if( (pid==2112) ) heta4->Fill(eta); // n
      else if( (pid==111) ) heta5->Fill(eta);  //pi0
      else  heta6->Fill(eta);  // left 

      if(gen>-1) { // primary 
	hpid7->Fill(float(pid));
	if(ch==0) {heta11->Fill(eta);hpid8->Fill(float(pid));}
	else      {heta12->Fill(eta);hpid9->Fill(float(pid));}
      } else {  // secondary 
	hpid10->Fill(float(pid));
	if(ch==0) {heta13->Fill(eta);hpid11->Fill(float(pid));}
	else      {heta14->Fill(eta);hpid12->Fill(float(pid));}
      }

     }
   }


   const bool printVertices = false;
   if(printVertices) {
     int i=0;
     for (std::vector<SimVertex>::iterator isimvtx = theSimVertexes.begin();
	  isimvtx != theSimVertexes.end(); ++isimvtx, ++i){
       if(print) cout<<"Vertex "<<i
	   <<" position  x = "<<isimvtx->position().x() <<" y = "<<isimvtx->position().y() 
	   <<" z = "<< isimvtx->position().z()
	   <<" vertex index = "<< isimvtx->vertexId()<<" parent = "<< isimvtx->parentIndex()
	   <<" process = "<<isimvtx->processType() 
	   <<endl;
     }
   }

   const bool printsHits = false;
   if(printsHits) {
     if(print) cout<<" SimHits "<<theTrackerHits.size()<<endl;
     std::map<unsigned int, std::vector<PSimHit>,std::less<unsigned int> > SimHitMap;
     for (std::vector<PSimHit>::iterator isim = theTrackerHits.begin();
	  isim != theTrackerHits.end(); ++isim){
       SimHitMap[(*isim).detUnitId()].push_back((*isim));

       int detid = isim->detUnitId();
       //if(detid>=302000000 && detid<303000000) {  // only bpix
       if(1) {  // all
	 // SimHit information 
	 float eloss = (*isim).energyLoss() * 1000000/3.7;//convert GeV to ke 
	 //float tof = (*isim).timeOfFlight();
	 float p = (*isim).pabs();
	 //float pt = (*isim).momentumAtEntry().perp();
	 float theta = (*isim).thetaAtEntry();
	 float beta = 0;  // beta is roughly like real theta
	 if(theta<PI/2.) beta=(PI/2.) - theta; //  
	 else            beta=theta - (PI/2.);
	 float eta = -log(tan(beta/2.));  // this is an approximation to the tracks eta

	 //float phi = (*isim).phiAtEntry();
	 int pid = ((*isim).particleType()); 
	 int tid = (*isim).trackId();
	 int procType = (*isim).processType();
	 //cout<<pid<<endl;
	 //if(pid==13) 
	 if(print) cout<<"Simhit id "<<pid<<" track "<<tid<<" proc "<<procType<<" de "
	       <<eloss<<" p "<<p<<" theta "<<theta
	       <<" position  x = "<<isim->localPosition().x() 
	       <<" y = "<<isim->localPosition().y() <<" z = "<< isim->localPosition().z()<<endl;
       }


     }
   }

}

//define this as a plug-in
DEFINE_FWK_MODULE(SimHitAnalyzer);
