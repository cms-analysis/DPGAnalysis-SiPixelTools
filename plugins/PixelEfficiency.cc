//try me
#include "DataFormats/GeometrySurface/interface/LocalError.h"
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
//#include "MagneticField/UniformEngine/src/UniformMagneticFieldESProducer.h"
//class to take the pixel submodules
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
//class for the trajectories
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Alignment/CommonAlignment/interface/Utilities.h"
//class to handle the pixelcluster
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>

using namespace std;
using namespace edm;
using namespace reco;

 const bool DEBUG = false;

 const int kBPIX = PixelSubdetector::PixelBarrel;
 const int kFPIX = PixelSubdetector::PixelEndcap;

 const int numOfBadRuns=16;

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

  typedef std::vector< edm::ParameterSet > Parameters;
  Parameters  BadModuleList_;

      // ----------member data ---------------------------
  std::string fOutputFileName; 
  edm::InputTag TkTag_ ;

  edm::InputTag trajectoryInput_;
  edm::InputTag pixelClusterInput_;

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
  TH1F*  consistencyCheckTraj;

  TH1F*  histInvalidRecHitCollection;
  TH1F*  histInvalidRecHitWithBadmoduleList;
  TH1F*  windowSearch;
  TH1F*  windowSearchSameModule;
  TH1F*  windowSearchBPix;
  TH1F*  windowSearchFPix;
  TH1F*  validPerRun;
  TH1F*  invalidPerRun;
  TH1F*  inactivePerRun;
  
  TH1F*  validVsAlpha;
  TH1F*  missingVsAlpha;
  TH1F*  validVsBeta;
  TH1F*  missingVsBeta;

  TH1F*  validVsAlphaBPix;
  TH1F*  missingVsAlphaBPix;
  TH1F*  validVsBetaBPix;
  TH1F*  missingVsBetaBPix;
  
  TH1F*  validVsAlphaFPix;
  TH1F*  missingVsAlphaFPix;
  TH1F*  validVsBetaFPix;
  TH1F*  missingVsBetaFPix;
  
  TH1F*  validVsLocalX;
  TH1F*  missingVsLocalX;
  TH1F*  validVsLocalY;
  TH1F*  missingVsLocalY;
  
  //
  TH1F*  checkoutValidityFlag;
  TH1F*  checkoutTraj;
  //
  TH1F*  totalPerTrack;
  TH1F*  inactivePerTrack;
  TH1F*  missingPerTrack;
  TFile* fOutputFile;

//test
TH1F* histoMethod2After;
TH1F* histoMethod2;
TH1F* histoMethod2AfterFPix;
TH1F* histoMethod2FPix;

  //"maps" for module analysis: <rawModuleID, counterOn[inactive,missing,valid]>
  vector< vector<int> > badModuleMap;  
  vector< vector<double> > goodModuleMap; //but interesting only in-active!!
  //TTree for module analysis
  TTree* tree;
  int idTree, isModuleBadTree, inactiveTree, missingTree, validTree, barrelTree, ladderTree, bladeTree, moduleInLadderTree;
  double globalXTree, globalYTree, globalZTree;

  bool badRun;  
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
  trajectoryInput_( iConfig.getParameter<edm::InputTag>("trajectoryInput") ),
  pixelClusterInput_( iConfig.getParameter<edm::InputTag>("pixelClusterInput") )
{   
 //now do what ever initialization is needed
 std::cout<<"debug constructor"<<std::endl;
 BadModuleList_ = iConfig.getUntrackedParameter<Parameters>("BadModuleList");

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
  if(DEBUG) std::cout<<"Starting Analyze"<<std::endl;

  using namespace edm;
  using namespace std;

  
  unsigned int runNumber=iEvent.id().run();
                
  badRun=false;
 
  // Get event setup (to get global transformation)
  edm::ESHandle<TrackerGeometry> geom;
  iSetup.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker( *geom );

   //handle of the tracks 
   consistencyCheck->Fill(0); //fill for each event
   
   // Loop over trajectories
   edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
   
  //**************************************************************
  //residual (loop on traj)
  

try{

 iEvent.getByLabel(trajectoryInput_,trajTrackCollectionHandle);

  edm::Handle< edmNew::DetSetVector<SiPixelCluster> > theClusters;
  iEvent.getByLabel(pixelClusterInput_, theClusters);

  const edmNew::DetSetVector<SiPixelCluster>& input = *theClusters;

  int NbrTracks =  trajTrackCollectionHandle->size();

  checkoutTraj->Fill(NbrTracks);

  TrajectoryStateCombiner tsoscomb;
  for(TrajTrackAssociationCollection::const_iterator it = trajTrackCollectionHandle->begin(),
      itEnd = trajTrackCollectionHandle->end(); it!=itEnd;++it){

    consistencyCheck->Fill(1); //1 = each track //*************

    const Trajectory& traj  = *it->key;
    std::vector<TrajectoryMeasurement> tmColl = traj.measurements();
      
    //here put the QUALITY CUTS over teh trajectories      
    //at least two intersections and 1 valid hit
    
    //************************ WE DONT CARE ANYMORE ABOUT TRACK SELECTION, ITS ON HIT SELECTION !!
    /*bool trajWithTwoPixIntersec  = false;
    int numOfPixIntersec=0;
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
        itTraj != itTrajEnd; ++itTraj) {
  
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
      uint testSubDetID = (testhit->geographicalId().subdetId());
      if( testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap ) numOfPixIntersec++;
      if (numOfPixIntersec==2) {trajWithTwoPixIntersec=true; break;}
      }
    bool trajWithAtLeastOneValid = false; 
    if (trajWithTwoPixIntersec)
      {
      for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	  itTraj != itTrajEnd; ++itTraj) {     
       	//check if belonging to badmodule list otherwise go to the next recHit
        TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
	bool badModule=false;
        for(Parameters::iterator it = BadModuleList_.begin(); it != BadModuleList_.end(); ++it) {         
          if ( ((testhit)->geographicalId().rawId())==(it->getParameter<uint32_t>("detid")) ) {badModule=true; break;}
	  }
        if (badModule) continue;
       
        uint testSubDetID = (testhit->geographicalId().subdetId());
        if (testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap)
          {
	  if( (*testhit).getType()== TrackingRecHit::valid )
	    {
	    trajWithAtLeastOneValid=true;
	    break;
	    }
	  }
        }
      }*/

    consistencyCheckTraj->Fill(0);//number of total trajectories


  //if ( !(trajWithAtLeastOneValid&&trajWithTwoPixIntersec) ) continue;

//*************
    consistencyCheck->Fill(2);

    //runNumber analysis
       char testLabel[10] ;
       sprintf (testLabel,"%d",runNumber) ;
       bool found=false;
       int setBin=190;
       for (int binSearch=1; binSearch<=200; binSearch++)
         {
	 if ( !strcmp(validPerRun->GetXaxis()->GetBinLabel(binSearch),"") ) {setBin=binSearch; break;}
	 else
	   {
	   if ( !strcmp(testLabel,validPerRun->GetXaxis()->GetBinLabel(binSearch)) ) {found=true; break;}
	   } 
	 }
	 
       if (!found)
         {
	 validPerRun->GetXaxis()->SetBinLabel(setBin,testLabel);
	 invalidPerRun->GetXaxis()->SetBinLabel(setBin,testLabel);
	 inactivePerRun->GetXaxis()->SetBinLabel(setBin,testLabel);
	 }
       
       float numPixHit=0;
       float numInactiveHit=0;
       float numMissingHit=0;
       
       //*************
       
    //precheck to count the BPix/FPix number of trajectories
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	itTraj != itTrajEnd; ++itTraj) {
	
	
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
      uint testSubDetID = (testhit->geographicalId().subdetId());
      if( (testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) ) 
        {
	consistencyCheckTraj->Fill(1);
	break;
	}
      }

    //precheck to count only the BPix number of trajectories
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	itTraj != itTrajEnd; ++itTraj) {
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
      uint testSubDetID = (testhit->geographicalId().subdetId());
      if( testSubDetID == PixelSubdetector::PixelBarrel ) 
        {
	consistencyCheckTraj->Fill(2);
	break;
	}
      }
     
    int numofhit=0;      
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	itTraj != itTrajEnd; ++itTraj) {

	numofhit++;
	
  
      //are we in the pixels with valid stuffs?
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
      //check if in PixelDetector
      uint testSubDetID = (testhit->geographicalId().subdetId());
      int type = testSubDetID;
      if(! (testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) ) continue;
      //check if belonging to badmodule list
      bool badModule=false;
      for(Parameters::iterator it = BadModuleList_.begin(); it != BadModuleList_.end(); ++it) {         
        if ( (testhit->geographicalId().rawId())==(it->getParameter<uint32_t>("detid")) ) {badModule=true; break;}
	}
      if (badModule) continue;
            	
	 int moduleRawId = (testhit)->geographicalId().rawId();

	 DetId detId = ((testhit)->geographicalId());
        
         unsigned int layer=0;
         unsigned int ladder=0;
         unsigned int disk=0;
         unsigned int blade=0;
	 unsigned int moduleInLadder=0;
	 if (type==int(kBPIX))
	   {PXBDetId pdetId = PXBDetId(detId); layer=pdetId.layer(); ladder=pdetId.ladder(); moduleInLadder=pdetId.module();}
	 if (type==int(kFPIX)){PXFDetId pfdetId = PXFDetId(detId); blade=pfdetId.blade(); disk=pfdetId.disk();}
	 double globalX = 0;
	 double globalY = 0;
	 double globalZ = 0;
         const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
         globalX = theGeomDet->surface().position().x();
         globalY = theGeomDet->surface().position().y();
         globalZ = theGeomDet->surface().position().z();
	
	 
	//********************* HERE IS THE CUT ON HITS !!
	int numofhitCut=0;
        int numOfOtherValid=0;  
	bool hasValidInUpperPix = false;
	bool hasValidInLowerPix = false;
	for(std::vector<TrajectoryMeasurement>::const_iterator itCut1 = tmColl.begin(), itCut1End = tmColl.end();
	itCut1 != itCut1End; ++itCut1) {
	  numofhitCut++;
	
	  //FPixTest;
	  
	  TransientTrackingRecHit::ConstRecHitPointer testhitCut = itCut1->recHit();
          uint testSubDetIDCut = (testhitCut->geographicalId().subdetId());
          int typeCut = testSubDetIDCut;
          if(! (testSubDetIDCut == PixelSubdetector::PixelBarrel || testSubDetIDCut == PixelSubdetector::PixelEndcap) ) continue;
         
	  //check if belonging to badmodule list
          bool badModuleCut=false;
          for(Parameters::iterator itCut = BadModuleList_.begin(); itCut != BadModuleList_.end(); ++itCut) {         
            if ( (testhitCut->geographicalId().rawId())==(itCut->getParameter<uint32_t>("detid")) ) {badModuleCut=true; break;}
	  }
          if (badModuleCut) continue;
           	
	   //int moduleRawIdCut = (testhitCut)->geographicalId().rawId();

	   DetId detIdCut = ((testhitCut)->geographicalId());
        
           unsigned int layerCut=99999;
           unsigned int diskCut=99999;
	   if (typeCut==int(kBPIX))
	     {PXBDetId pdetIdCut = PXBDetId(detIdCut); layerCut=pdetIdCut.layer();}
	   if (typeCut==int(kFPIX)){PXFDetId pfdetIdCut = PXFDetId(detIdCut); diskCut=pfdetIdCut.disk();}
	   double globalYCut = 0;
           const PixelGeomDetUnit* theGeomDetCut = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detIdCut) );
           globalYCut = theGeomDetCut->surface().position().y();
	   
	   if(numofhitCut!=numofhit && globalYCut>0 && layerCut!=layer && disk!=diskCut && (*testhitCut).getType()==TrackingRecHit::valid)
	     hasValidInUpperPix = true;
	   if(numofhitCut!=numofhit && globalYCut<0 && layerCut!=layer && disk!=diskCut && (*testhitCut).getType()==TrackingRecHit::valid)
	     hasValidInLowerPix = true;
		
	   if(numofhitCut!=numofhit && layerCut!=layer && disk!=diskCut && (*testhitCut).getType()==TrackingRecHit::valid)
	     numOfOtherValid++;  

	}//end of loop for trajmeas for cut
	
      
       //*******************************
       // DO THE ANALYSIS ONLY IF GOOD OTHER HIT ARE FOUND !!!!!!!!!!!
       
//different tightness in the cuts 
       if (numOfOtherValid<1) continue;
//         if(! (hasValidInLowerPix && hasValidInUpperPix) ) continue;


      
      //**********
      
      //THIS IS ONLY TO CHECK THE DEFINITION OF INACTIVE-FLAG!!!
	 if ( (*testhit).getType()==TrackingRecHit::inactive )  numInactiveHit++;
	 if ( (*testhit).getType()==TrackingRecHit::missing )   numMissingHit++;
	  
	 //***********************************
	 //type of invalid recHiT
	
	 //let's observe better invalid not missing recHits
         int specificInvalid = 0;  //something else than inactive or missing or bad
	 if ( ! ((*testhit).isValid()) )
	   {
	   if ( (*testhit).getType()==TrackingRecHit::inactive )     specificInvalid = 1;
	   if ( (*testhit).getType()==TrackingRecHit::bad )          specificInvalid = 2;
	   if ( (*testhit).getType()==TrackingRecHit::missing )      specificInvalid = 3;
	   }
	 if ( ( (*testhit).isValid() && badModule ) )
	   {
	   specificInvalid = 4;
	   }
	 
	 if ( ! ((*testhit).isValid()) ) histInvalidRecHitCollection->Fill(specificInvalid);
	 
	 //histWithBadmoduleList
	 //fill 4th bin when we have a valid recHit belonging to badModule list: we expect 0 !!!
	 if (specificInvalid == 4)                    histInvalidRecHitWithBadmoduleList->Fill(specificInvalid);
	 //the other 3 bins are filled for the invalid statistic and excluding bad module list
	 if ( (!((*testhit).isValid()))&&(!(badModule)) )  histInvalidRecHitWithBadmoduleList->Fill(specificInvalid);
	 
	 
      
      bool founded=0;
             for (int it=0; it<goodModuleMap.size(); it++)
	       {
	       if (goodModuleMap[it][0]==moduleRawId)
	         {
	         founded=1;
	         if ( (*testhit).getType()==TrackingRecHit::inactive ) (goodModuleMap[it])[1]++;
	         if ( (*testhit).getType()==TrackingRecHit::missing )  (goodModuleMap[it])[2]++;
		 if ( (*testhit).isValid() )                           (goodModuleMap[it])[3]++;
	         break;
	         }
	       }
             if (!founded)
               {
	       vector<double> aux;
	       aux.push_back(moduleRawId);
	       for (int j=1;j<=10; j++) aux.push_back(0);
	       goodModuleMap.push_back(aux);
               if ( (*testhit).getType()==TrackingRecHit::inactive ) (goodModuleMap[goodModuleMap.size()-1])[1]++;
	       if ( (*testhit).getType()==TrackingRecHit::missing )  (goodModuleMap[goodModuleMap.size()-1])[2]++;
	       if ( (*testhit).isValid() )                           (goodModuleMap[goodModuleMap.size()-1])[3]++;
	       if ( type==int(kBPIX) )
	         {
		 (goodModuleMap[goodModuleMap.size()-1])[4]= 1;
		 (goodModuleMap[goodModuleMap.size()-1])[5]= ladder;
		 (goodModuleMap[goodModuleMap.size()-1])[6]= 0;
		 (goodModuleMap[goodModuleMap.size()-1])[7]= moduleInLadder;
		 }
	       if ( type==int(kFPIX) ) 
	         {
		 (goodModuleMap[goodModuleMap.size()-1])[4]= 2;
		 (goodModuleMap[goodModuleMap.size()-1])[5]= 0;
		 (goodModuleMap[goodModuleMap.size()-1])[6]= blade;
		 (goodModuleMap[goodModuleMap.size()-1])[7]= moduleInLadder;  //by now it's ZERO for BPix
		 }
	       (goodModuleMap[goodModuleMap.size()-1])[8]=globalX;       
	       (goodModuleMap[goodModuleMap.size()-1])[9]=globalY;       	       
	       (goodModuleMap[goodModuleMap.size()-1])[10]=globalZ;       	       
  	       }
	       
	       
	     if ( ! ((*testhit).isValid()) )  invalidPerRun->Fill(testLabel,1);
	 if (   ((*testhit).isValid()) )  validPerRun->Fill(testLabel,1);
	 if ( (*testhit).getType()==TrackingRecHit::inactive ) inactivePerRun->Fill(testLabel,1);
	 
	 //operation done for all the Pixel recHit
         int filling = 0;
         if((*testhit).isValid() ) filling=2;
         if (!((*testhit).isValid()) ) filling=0;
	 if (!((*testhit).isValid()) && (*testhit).getType()==TrackingRecHit::missing) filling=1;	 
	 
	 histo->Fill(filling);

         if (type==int(kBPIX))  
	   {
	   histBarrel->Fill(filling);
	   
	   
	   if (layer==1) histLayer1->Fill(filling);
	   if (layer==2) histLayer2->Fill(filling);
	   if (layer==3) histLayer3->Fill(filling);
	   }//barrel
	 if (type==int(kFPIX))
	   {
	   histEndcap->Fill(filling);
	   
	   if(theGeomDet->surface().position().z() < 0.0) histEndcapMinus->Fill(filling);
	   else                                           histEndcapPlus->Fill(filling);
	   }
      //**********

      TrajectoryStateOnSurface tsos = tsoscomb( itTraj->forwardPredictedState(), itTraj->backwardPredictedState() );


      //**********************************
      //angular analysis of efficiency
      LocalTrajectoryParameters ltp = tsos.localParameters();
      LocalVector localDir = ltp.momentum()/ltp.momentum().mag();

      float locx = localDir.x();
      float locy = localDir.y();
      float locz = localDir.z();

      float alpha = atan2( locz, locx );
      if ((*testhit).getType()== TrackingRecHit::valid)  validVsAlpha->Fill( alpha );
      if ((*testhit).getType()== TrackingRecHit::missing)   missingVsAlpha->Fill( alpha);
      float  beta = atan2( locz, locy );
      if ((*testhit).getType()== TrackingRecHit::valid)  validVsBeta->Fill( beta );
      if ((*testhit).getType()== TrackingRecHit::missing)   missingVsBeta->Fill( beta);
      
      if(testSubDetID == PixelSubdetector::PixelBarrel)
        {
        float alpha = atan2( locz, locx );
        if ((*testhit).getType()== TrackingRecHit::valid)  validVsAlphaBPix->Fill( alpha );
        if ((*testhit).getType()== TrackingRecHit::missing)   missingVsAlphaBPix->Fill( alpha);
        float  beta = atan2( locz, locy );
        if ((*testhit).getType()== TrackingRecHit::valid)  validVsBetaBPix->Fill( beta );
        if ((*testhit).getType()== TrackingRecHit::missing)   missingVsBetaBPix->Fill( beta);
        }
      if(testSubDetID == PixelSubdetector::PixelEndcap)
        {
        float alpha = atan2( locz, locx );
        if ((*testhit).getType()== TrackingRecHit::valid)  validVsAlphaFPix->Fill( alpha );
        if ((*testhit).getType()== TrackingRecHit::missing)   missingVsAlphaFPix->Fill( alpha);
        float  beta = atan2( locz, locy );
        if ((*testhit).getType()== TrackingRecHit::valid)  validVsBetaFPix->Fill( beta );
        if ((*testhit).getType()== TrackingRecHit::missing)   missingVsBetaFPix->Fill( beta);
        }
	
      if ( (testSubDetID==kBPIX) )             
        {
      //**********************************
      //window searching analysis

      DetId hitDetId = (testhit->geographicalId());
      PXBDetId hitPdetId = PXBDetId(hitDetId);
      unsigned int hitLayer=0;
      hitLayer=hitPdetId.layer();        
/*
//test of existence of PROPER validity-flag handling trajectories
//make statistic both on valid/invalid!
if ( (*testhit).getType()==TrackingRecHit::inactive ) checkoutValidityFlag->Fill(0);
else if ((*testhit).getType()==TrackingRecHit::missing) checkoutValidityFlag->Fill(1);
else  if ((*testhit).getType()==TrackingRecHit::valid) checkoutValidityFlag->Fill(2);
else checkoutValidityFlag->Fill(3);
*/
//test
      if ((*testhit).getType()==TrackingRecHit::missing) histoMethod2->Fill(0);
      if ((*testhit).getType()==TrackingRecHit::valid)   histoMethod2->Fill(1);
//
      if ( (*testhit).getType()!=TrackingRecHit::inactive)
        {
	double minDistance=999999;
	bool sameModule=false;
	for (edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter = input.begin(); DSViter != input.end(); DSViter++)
	  {
	  unsigned int ClusterId = DSViter->id();
          DetId ClusterDetId(ClusterId);
	  PXBDetId pdetId = PXBDetId(ClusterDetId);
          unsigned int clusterLayer=0;
	  clusterLayer=pdetId.layer();
	  if ( clusterLayer != hitLayer) continue;
	  unsigned int detidaux = DSViter->detId();
          DetId detIdObject( detidaux );  
          //you don't use this last line
	  const GeomDetUnit * genericDet = geom->idToDetUnit( detIdObject );
		  
          edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
          edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
          	  
	  for(edmNew::DetSet<SiPixelCluster>::const_iterator clustIt=begin; clustIt!=end;++clustIt)
            {
            const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detIdObject) );
	    const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	    LocalPoint lpclust = topol->localPosition(MeasurementPoint((*clustIt).x(),(*clustIt).y()));
	    GlobalPoint GPclust =  theGeomDet->surface().toGlobal(Local3DPoint(lpclust.x(),lpclust.y(),lpclust.z()));
            double deltaX,deltaY,deltaZ;
	    deltaX= GPclust.x()- tsos.globalPosition().x();
	    deltaY= GPclust.y()- tsos.globalPosition().y();
	    deltaZ= GPclust.z()- tsos.globalPosition().z();
	    double distance;
	    distance = sqrt( pow(deltaZ,2)+pow(deltaX,2)+pow(deltaY,2) );
	    
	    if (distance<minDistance) 
	      {
	      //check same module
	      if (testhit->geographicalId().rawId()==theGeomDet->geographicalId().rawId()) sameModule=true;
	      else sameModule=false;
	      
	      minDistance=distance;
	      }
	    }//end-for first cluster loop	   
	  }//end-for second cluster loop 

	if (minDistance>0.5)
	  {
          missingVsLocalX->Fill(tsos.localPosition().x());
          missingVsLocalY->Fill(tsos.localPosition().y());
	  //test
	  histoMethod2After->Fill(0);
	  }
	else
	  {
	  validVsLocalX->Fill(tsos.localPosition().x());	  
	  validVsLocalY->Fill(tsos.localPosition().y());
	  //test
	  histoMethod2After->Fill(1);
	  }
	
	if (sameModule) windowSearchSameModule->Fill(minDistance);
	windowSearch->Fill(minDistance); 
	windowSearchBPix->Fill(minDistance); 
	}//end-if "work-on-valid-&-missing"    
      }//end-if method 2 for barrel

      if ( (testSubDetID==kFPIX) )             
        {
      //**********************************
      //window searching analysis FPix

      DetId hitDetId = (testhit->geographicalId());
      PXFDetId hitPdetId = PXFDetId(hitDetId);
      unsigned int hitDisk=0;
      hitDisk=hitPdetId.disk();        

      if ((*testhit).getType()==TrackingRecHit::missing) histoMethod2FPix->Fill(0);
      if ((*testhit).getType()==TrackingRecHit::valid)   histoMethod2FPix->Fill(1);

      if ( (*testhit).getType()!=TrackingRecHit::inactive)
        {
	double minDistance=999999;
	bool sameModule=false;
	for (edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter = input.begin(); DSViter != input.end(); DSViter++)
	  {
	  unsigned int ClusterId = DSViter->id();
          DetId ClusterDetId(ClusterId);
	  PXFDetId pdetId = PXFDetId(ClusterDetId);
          unsigned int clusterDisk=0;
	  clusterDisk=pdetId.disk();
	  if ( clusterDisk != hitDisk) continue;	  
	  unsigned int detidaux = DSViter->detId();
          DetId detIdObject( detidaux );  
          const GeomDetUnit * genericDet = geom->idToDetUnit( detIdObject );
		  
          edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
          edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
          	  
	  for(edmNew::DetSet<SiPixelCluster>::const_iterator clustIt=begin; clustIt!=end;++clustIt)
            {
            const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detIdObject) );
	    const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	    LocalPoint lpclust = topol->localPosition(MeasurementPoint((*clustIt).x(),(*clustIt).y()));
	    GlobalPoint GPclust =  theGeomDet->surface().toGlobal(Local3DPoint(lpclust.x(),lpclust.y(),lpclust.z()));
            double deltaX,deltaY,deltaZ;
	    deltaX= GPclust.x()- tsos.globalPosition().x();
	    deltaY= GPclust.y()- tsos.globalPosition().y();
	    deltaZ= GPclust.z()- tsos.globalPosition().z();
	    double distance;
	    distance = sqrt( pow(deltaZ,2)+pow(deltaX,2)+pow(deltaY,2) );
	    if (distance<minDistance) 
	      {
      	      //check same module
	      if (testhit->geographicalId().rawId()==theGeomDet->geographicalId().rawId()) sameModule=true;
	      else sameModule=false;
	      //??LocalError errClust = (*clustIt).localError().positionError();
              //??LocalError err=(*clustIt).localPositionError();
              //??LocalError leclust = topol->localError().positionError(MeasurementPoint((*clustIt).x(),(*clustIt).y()));

	      minDistance=distance;
	      }
	    }//end-for first cluster loop	   
	  }//end-for second cluster loop 

//test
	if (minDistance>0.5)  histoMethod2After->Fill(0);
	else                  histoMethod2After->Fill(1);
	  
	if (sameModule) windowSearchSameModule->Fill(minDistance);
	windowSearchFPix->Fill(minDistance); 
	windowSearch->Fill(minDistance); 
	}//end-if "work-on-valid-&-missing"    
      }//end-if method 2 for forward

      }//end-for trajMeasurements 
      
      //**********
       totalPerTrack->Fill( numPixHit );
       inactivePerTrack->Fill( (numInactiveHit/numPixHit)*100.);
       missingPerTrack->Fill( (numMissingHit/numPixHit)*100.);
       //****************
	     
    }//end-for of Trajectories
}catch ( ... ) {}

   if(DEBUG) cout<<"End of Analyze"<<endl;

}

// ------------ method called once each job just before starting event loop  ------------

void 
PixelEfficiency::beginJob(const edm::EventSetup&)
{
 if(DEBUG) std::cout<<"Begin job"<<std::endl;

 fOutputFile = new TFile(fOutputFileName.c_str(), "RECREATE"); 

//test
 histoMethod2After = new TH1F("histoMethod2After","histoMethod2After",2,0,2);
 histoMethod2 = new TH1F("histoMethod2","histoMethod2",2,0,2);
 histoMethod2AfterFPix = new TH1F("histoMethod2AfterFPix","histoMethod2AfterFPix",2,0,2);
 histoMethod2FPix = new TH1F("histoMethod2FPix","histoMethod2FPix",2,0,2);
 
 
 histo = new TH1F("histo", "histo", 3, 0, 3);
 histLayer1 = new TH1F("histLayer1", "histLayer1", 3, 0, 3);
 histLayer2 = new TH1F("histLayer2", "histLayer2", 3, 0, 3);
 histLayer3 = new TH1F("histLayer3", "histLayer3", 3, 0, 3);
 histEndcapPlus = new TH1F("histEndcapPlus", "histEndcapPlus", 3, 0, 3);
 histEndcapMinus = new TH1F("histEndcapMinus", "histEndcapMinus", 3, 0, 3);
 histBarrel = new TH1F("histBarrel", "histBarrel", 3, 0, 3);
 histEndcap = new TH1F("histEndcap", "histEndcap", 3, 0, 3);

 histInvalidRecHitCollection = new TH1F("histInvalidRecHitCollection","histInvalidRecHitCollection",5,0,5);
 histInvalidRecHitWithBadmoduleList = new TH1F("histInvalidRecHitWithBadmoduleList","histInvalidRecHitWithBadmoduleList",5,0,5);
 
 consistencyCheck = new TH1F("consistencyCheck","consistencyCheck", 3, 0,3);
 consistencyCheckTraj = new TH1F("consistencyCheckTraj","consistencyCheckTraj", 3, 0,3);

 windowSearch = new TH1F("windowSearch","windowSearch",500,0,0.50);
 windowSearchSameModule = new TH1F("windowSearchSameModule","windowSearchSameModule",500,0,0.50);
 windowSearchBPix = new TH1F("windowSearchBPix","windowSearchBPix",500,0,0.50);
 windowSearchFPix = new TH1F("windowSearchFPix","windowSearchFPix",500,0,0.50);
 validPerRun = new TH1F("validPerRun","validPerRun",200,0,200);
 invalidPerRun = new TH1F("invalidPerRun","invalidPerRun",200,0,200);
 inactivePerRun = new TH1F("inactivePerRun","inactivePerRun",200,0,200);
 
//
 validVsAlpha = new TH1F("validVsAlpha","validVsAlpha",200,-3.5,3.5);
 missingVsAlpha = new TH1F("missingVsAlpha","missingVsAlpha",200,-3.5,3.5);
 validVsBeta = new TH1F("validVsBeta","validVsBeta",200,-3.5,3.5);
 missingVsBeta = new TH1F("missingVsBeta","missingVsBeta",200,-3.5,3.5);

 validVsAlphaBPix = new TH1F("validVsAlphaBPix","validVsAlphaBPix",200,-3.5,3.5);
 missingVsAlphaBPix = new TH1F("missingVsAlphaBPix","missingVsAlphaBPix",200,-3.5,3.5);
 validVsBetaBPix = new TH1F("validVsBetaBPix","validVsBetaBPix",200,-3.5,3.5);
 missingVsBetaBPix = new TH1F("missingVsBetaBPix","missingVsBetaBPix",200,-3.5,3.5);

 validVsAlphaFPix = new TH1F("validVsAlphaFPix","validVsAlphaFPix",200,-3.5,3.5);
 missingVsAlphaFPix = new TH1F("missingVsAlphaFPix","missingVsAlphaFPix",200,-3.5,3.5);
 validVsBetaFPix = new TH1F("validVsBetaFPix","validVsBetaFPix",200,-3.5,3.5);
 missingVsBetaFPix = new TH1F("missingVsBetaFPix","missingVsBetaFPix",200,-3.5,3.5);

 validVsLocalX = new TH1F("validVsLocalX","validVsLocalX",100,-1.5,1.5);
 missingVsLocalX = new TH1F("missingVsLocalX","missingVsLocalX",100,-1.5,1.5);
 validVsLocalY = new TH1F("validVsLocalY","validVsLocalY",100,-4.,4.);
 missingVsLocalY = new TH1F("missingVsLocalY","missingVsLocalY",100,-4.,4.);

 //
 checkoutValidityFlag = new TH1F("checkoutValidityFlag","checkoutValidityFlag", 4, 0,4);
 checkoutTraj = new TH1F("checkoutTraj","checkoutTraj",10,0,10);
 //
 totalPerTrack= new TH1F ("totalPerTrack","totalPerTrack",20,0,20);
 inactivePerTrack= new TH1F ("inactivePerTrack","inactivePerTrack",101,0,101);
 missingPerTrack= new TH1F ("missingPerTrack","missingPerTrack",101,0,101);


 tree = new TTree("moduleAnalysis","moduleAnalysis");
 tree->Branch("id",&idTree,"id/I");
 tree->Branch("isModuleBad",&isModuleBadTree,"isModuleBad/I");
 tree->Branch("inactive",&inactiveTree,"inactive/I");
 tree->Branch("missing",&missingTree,"missing/I");
 tree->Branch("valid",&validTree,"valid/I");
 tree->Branch("isBarrelModule",&barrelTree,"isBarrelModule/I");
 tree->Branch("ladder",&ladderTree,"ladder/I");
 tree->Branch("blade",&bladeTree,"blade/I");
 tree->Branch("moduleInLadder",&moduleInLadderTree,"moduleInLadder/I");
 tree->Branch("globalX",&globalXTree,"globalX/D");
 tree->Branch("globalY",&globalYTree,"globalY/D");
 tree->Branch("globalZ",&globalZTree,"globalZ/D");

 
 if(DEBUG) std::cout<<"End of begin job"<<std::endl;
 
 //f = new TFile("checktkcollection.root","RECREATE");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PixelEfficiency::endJob() {
 
  if(DEBUG) std::cout<<"End job"<<std::endl;

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
  histInvalidRecHitWithBadmoduleList->Write();

  consistencyCheck->Write();
  consistencyCheckTraj->Write();

  windowSearch->Write();
  windowSearchSameModule->Write();
  windowSearchBPix->Write();
  windowSearchFPix->Write();

  validVsAlpha->Write();
  missingVsAlpha->Write();
  validVsBeta->Write();
  missingVsBeta->Write();
  validVsAlphaBPix->Write();
  missingVsAlphaBPix->Write();
  validVsBetaBPix->Write();
  missingVsBetaBPix->Write();
  validVsAlphaFPix->Write();
  missingVsAlphaFPix->Write();
  validVsBetaFPix->Write();
  missingVsBetaFPix->Write();

  validVsLocalX->Write();
  missingVsLocalX->Write();
  validVsLocalY->Write();
  missingVsLocalY->Write();
  
  validPerRun->LabelsDeflate("X");
  validPerRun->Write();
  invalidPerRun->LabelsDeflate("X");
  invalidPerRun->Write();
  inactivePerRun->LabelsDeflate("X");
  inactivePerRun->Write();
  
  //
  checkoutValidityFlag->Write();
  checkoutTraj->Write();
  //
  totalPerTrack->Write();
  inactivePerTrack->Write();
  missingPerTrack->Write();

//test
histoMethod2After->Write();
histoMethod2->Write();  
histoMethod2AfterFPix->Write();
histoMethod2FPix->Write();  
  
    isModuleBadTree=1;
    for (int l=0; l<badModuleMap.size(); l++)
      {
      idTree=(badModuleMap[l])[0];inactiveTree=(badModuleMap[l])[1]; missingTree=(badModuleMap[l])[2]; validTree=(badModuleMap[l])[3];
      barrelTree=(badModuleMap[l])[4] ;
      tree->Fill();
      }
    isModuleBadTree=0;
    //validTree=-10;
    for (int l=0; l<goodModuleMap.size(); l++)
      {
      idTree=(goodModuleMap[l])[0];inactiveTree=(goodModuleMap[l])[1]; missingTree=(goodModuleMap[l])[2]; validTree=(goodModuleMap[l])[3];
      barrelTree=(goodModuleMap[l])[4] ; ladderTree=(goodModuleMap[l])[5]; bladeTree=(goodModuleMap[l])[6];
      moduleInLadderTree=(goodModuleMap[l])[7];
      globalXTree=(goodModuleMap[l])[8]; globalYTree=(goodModuleMap[l])[9]; globalZTree=(goodModuleMap[l])[10];
      tree->Fill();
      }
    tree->Write();
   
  fOutputFile->Write() ;
  fOutputFile->Close() ;

}

//define this as a plug-in
//DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(PixelEfficiency);
