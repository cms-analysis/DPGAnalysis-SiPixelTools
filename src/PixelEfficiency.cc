// system include files
#include <memory>

// user include files
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h" 
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h" 
#include "FWCore/Framework/interface/EventSetup.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/UniformEngine/src/UniformMagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/UniformEngine/src/UniformMagneticFieldESProducer.h"
//class to take the pixel submodules
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
//class for the trajectories
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Alignment/CommonAlignment/interface/Utilities.h"

#include "TH1F.h"
#include "TFile.h"

using namespace std;
using namespace edm;
using namespace reco;

 const int kBPIX = PixelSubdetector::PixelBarrel;
 const int kFPIX = PixelSubdetector::PixelEndcap;

//
// class decleration
//

class PixelEfficiency : public edm::EDAnalyzer {
public:
  explicit PixelEfficiency(const edm::ParameterSet&);
  ~PixelEfficiency();
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

      // ----------member data ---------------------------
  std::string fOutputFileName; 
  edm::InputTag TkTag_ ;

  edm::InputTag trajectoryInput_;

  std::string fOutputFileName0T; 
  edm::InputTag TkTag0T_ ;

  TH1F*  histo;
  TH1F*  histLayer1;
  TH1F*  histLayer2;
  TH1F*  histLayer3;
  TH1F*  histEndcapPlus;
  TH1F*  histEndcapMinus;
  TH1F*  histBarrel;
  TH1F*  histEndcap;

  TH1F*  consistencyCheck;

  TH1F*  histInvalidRecHitCollection;
  TH1F*  residual;
  
  TFile* fOutputFile;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//The current directory (root) is not associated with a file. The object (Pt Ctf) has not been written.


//
// constructors and destructor
//
PixelEfficiency::PixelEfficiency(const edm::ParameterSet& iConfig) :
  fOutputFileName( iConfig.getUntrackedParameter<std::string>("HistOutFile",std::string("pixelEfficiency.root")) ), 
  TkTag_( iConfig.getParameter<edm::InputTag>("TkTag") ),
  fOutputFileName0T( iConfig.getUntrackedParameter<std::string>("HistOutFile0T",std::string("pixelEfficiency0T.root")) ), 
  TkTag0T_( iConfig.getParameter<edm::InputTag>("TkTag0T") ),
  trajectoryInput_( iConfig.getParameter<edm::InputTag>("trajectoryInput") )

{   
 //now do what ever initialization is needed
 std::cout<<"debug constructor"<<std::endl;
}


PixelEfficiency::~PixelEfficiency()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
PixelEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::cout<<"debug analyze"<<std::endl;

  using namespace edm;
  using namespace std;

  //skip 0T runs for reprocessing errors
  unsigned int runNumber=iEvent.id().run();

  if ( (runNumber>= 66893 && runNumber<=67085) ||
       (runNumber>=67264 && runNumber<=67432) ||
       (runNumber>=67676 && runNumber<=67777) ||
       (runNumber>=69536 && runNumber<=69671) ||
       (runNumber>=70088 && runNumber<=99999) ) return;

  // Get event setup (to get global transformation)
  edm::ESHandle<TrackerGeometry> geom;
  iSetup.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker( *geom );

   //handle of the tracks 
   consistencyCheck->Fill(0); //fill for each event
   // Get Tracks
   Handle<reco::TrackCollection> trackCollection;
   // Loop over tracks
   try{
     iEvent.getByLabel(TkTag_,trackCollection);

     reco::TrackCollection::const_iterator tkIter;
     for( tkIter=trackCollection->begin(); tkIter!=trackCollection->end(); ++tkIter )
       {
       consistencyCheck->Fill(1); //1 = each track
       bool inPixelVolume = false;

       //precheck that the track is passing through pixels
       for (trackingRecHit_iterator iHit = tkIter->recHitsBegin(); iHit != tkIter->recHitsEnd(); ++iHit)
         {
	 int type =(*iHit)->geographicalId().subdetId();
         if (type==int(kBPIX)|| type==int(kFPIX))
	   {
           inPixelVolume = true;
	   break;
	   }
         }
             
       // loop on rechits
       if (inPixelVolume)
       {
       consistencyCheck->Fill(2);
       for (trackingRecHit_iterator iHit = tkIter->recHitsBegin(); iHit != tkIter->recHitsEnd(); ++iHit)
         {
         int filling = 0;
         int type =(*iHit)->geographicalId().subdetId();           
         if((*iHit)->isValid() ) filling=2;
         if (!((*iHit)->isValid()) ) filling=0;
	 if (!((*iHit)->isValid()) && (*iHit)->getType()==TrackingRecHit::missing) filling=1;
	 
	 //let's observe better invalid not missing recHits
         int specificInvalid = 0;  //something else than inactive or missing or bad
	 if ( ! ((*iHit)->isValid()) )
	   {
	   if ( (*iHit)->getType()==TrackingRecHit::inactive )     specificInvalid = 1;
	   if ( (*iHit)->getType()==TrackingRecHit::bad )          specificInvalid = 2;
	   if ( (*iHit)->getType()==TrackingRecHit::missing )      specificInvalid = 3;
	   }
	 	 
	 DetId detId;
         if (type==int(kBPIX)|| type==int(kFPIX))
	   {
	   detId = ((*iHit)->geographicalId());
	   histo->Fill(filling);
	   if ( ! ((*iHit)->isValid()) ) histInvalidRecHitCollection->Fill(specificInvalid);
	   }

         if (type==int(kBPIX))  
	   {
	   histBarrel->Fill(filling);
	   
	   PXBDetId pdetId = PXBDetId(detId);
           unsigned int layer=0;
	   layer=pdetId.layer();
	   if (layer==1) histLayer1->Fill(filling);
	   if (layer==2) histLayer2->Fill(filling);
	   if (layer==3) histLayer3->Fill(filling);
	   }//barrel
	 if (type==int(kFPIX))
	   {
	   histEndcap->Fill(filling);
	   
	   const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
	   if(theGeomDet->surface().position().z() < 0.0) histEndcapMinus->Fill(filling);
	   else                                           histEndcapPlus->Fill(filling);
	   }//endcaps
         } // end of loop on rechits
       } //if-else inPixelVolume 
     } // end of loop on tracks
          
  //handle of the trajectories
  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
  iEvent.getByLabel(trajectoryInput_,trajTrackCollectionHandle);

  TrajectoryStateCombiner tsoscomb;
  for(TrajTrackAssociationCollection::const_iterator it = trajTrackCollectionHandle->begin(), itEnd = trajTrackCollectionHandle->end(); it!=itEnd;++it){

    const Trajectory& traj  = *it->key;
    std::vector<TrajectoryMeasurement> tmColl = traj.measurements();
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	itTraj != itTrajEnd; ++itTraj) {
//are we in the pixels with valid stuffs?
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();

      uint testSubDetID = (testhit->geographicalId().subdetId());
      if(! (testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) ) continue;

      if(! testhit->isValid() ) {residual->Fill(-0.01);continue;}      
      if(! itTraj->updatedState().isValid()) {residual->Fill(-0.01);continue;}

      TrajectoryStateOnSurface tsos = tsoscomb( itTraj->forwardPredictedState(), itTraj->backwardPredictedState() );

     if(testSubDetID == 0 )  {residual->Fill(-0.01);continue;}
	
     align::LocalVector res = tsos.localPosition() - testhit->localPosition();
     residual->Fill( sqrt(pow(res.x(),2)+pow(res.y(),2)) );
   }
   }
   
   } catch ( cms::Exception& er ) {
     std::cout<<"caught std::exception "<<er.what()<<std::endl;
   } catch ( ... ) {
     std::cout<<" funny error " <<std::endl;
   }
   cout<<"debug end of analyze"<<endl;

}

// ------------ method called once each job just before starting event loop  ------------

void 
PixelEfficiency::beginJob(const edm::EventSetup&)
{
 std::cout<<"debug begin job"<<std::endl;

 fOutputFile = new TFile(fOutputFileName.c_str(), "RECREATE"); 
 
 histo = new TH1F("histo", "histo", 3, 0, 3);
 histLayer1 = new TH1F("histLayer1", "histLayer1", 3, 0, 3);
 histLayer2 = new TH1F("histLayer2", "histLayer2", 3, 0, 3);
 histLayer3 = new TH1F("histLayer3", "histLayer3", 3, 0, 3);
 histEndcapPlus = new TH1F("histEndcapPlus", "histEndcapPlus", 3, 0, 3);
 histEndcapMinus = new TH1F("histEndcapMinus", "histEndcapMinus", 3, 0, 3);
 histBarrel = new TH1F("histBarrel", "histBarrel", 3, 0, 3);
 histEndcap = new TH1F("histEndcap", "histEndcap", 3, 0, 3);

 histInvalidRecHitCollection = new TH1F("histInvalidRecHitCollection","histInvalidRecHitCollection",4,0,4);
 residual = new TH1F ("residual","residual",302, -0.02,3);
 
 consistencyCheck = new TH1F("consistencyCheck","consistencyCheck", 3, 0,3);

 std::cout<<"debug at the end of begin job"<<std::endl;
 
 //f = new TFile("checktkcollection.root","RECREATE");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PixelEfficiency::endJob() {
 
  std::cout<<"debug endjob"<<std::endl;

  using namespace std;

  fOutputFile->cd();

  histo->Write();  
  histLayer1->Write();  
  histLayer2->Write();  
  histLayer3->Write();  
  histEndcapPlus->Write();
  histEndcapMinus->Write();
  histBarrel->Write();  
  histEndcap->Write();  

  histInvalidRecHitCollection->Write();

  consistencyCheck->Write();
  residual->Write();

  fOutputFile->Write() ;
  fOutputFile->Close() ;

}

//define this as a plug-in
//DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(PixelEfficiency);
