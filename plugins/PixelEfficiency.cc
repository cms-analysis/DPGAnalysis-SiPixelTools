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
  TH1F*  validPerRun;
  TH1F*  invalidPerRun;
  TH1F*  inactivePerRun;
  
  TH1F*  validVsAlpha;
  TH1F*  invalidVsAlpha;
  TH1F*  validVsBeta;
  TH1F*  invalidVsBeta;
  //
  TH1F*  checkoutValidityFlag;
  TH1F*  checkoutTraj;
  //
  TH1F*  totalPerTrack;
  TH1F*  inactivePerTrack;
  TH1F*  missingPerTrack;
  TFile* fOutputFile;


  //"maps" for module analysis: <rawModuleID, counterOn[inactive,missing,valid]>
  vector< vector<int> > badModuleMap;  
  vector< vector<int> > goodModuleMap; //but interesting only in-active!!
  //TTree for module analysis
  TTree* tree;
  int idTree, isModuleBadTree, inactiveTree, missingTree, validTree, barrelTree;

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

  //skip 0T runs for reprocessing errors
  if (
  runNumber==69557 || runNumber==69559|| runNumber==69564|| runNumber==69572|| runNumber==69573|| runNumber==69587|| runNumber==69594||
  runNumber==69728|| runNumber==69743|| runNumber==70147|| runNumber==70170|| runNumber==70195|| runNumber==70344 || runNumber==70347 ||
  runNumber==70410|| runNumber==70411||
  runNumber==70412|| runNumber==70413|| runNumber==70414|| runNumber==70415|| runNumber==70416|| runNumber==70417|| runNumber==70421||
  runNumber==70454|| runNumber==70664|| runNumber==70674|| runNumber==70675 ||
       (runNumber>= 66951 && runNumber<=67085) ||
       (runNumber>=67264 && runNumber<=67432) ||
       (runNumber>=67676 && runNumber<=67777) ||
       (runNumber>=69536 && runNumber<=69671) ||
       (runNumber>=70196 && runNumber<=99999) ) return;

  //check carefully this 
  //last implemented list: from January reprocessing observation
  //int badRunList[numOfBadRuns]={66664,66706,66709,66733,66910,67139,67534,67544,67548,68124,69253,69256,69269,69276,69273,69310};
  //new list: from February ReRecoi TrackerPointing
  int badRunList[numOfBadRuns]={66733,64711,68124,69276,67548,67139,69256,67544};
                               
  badRun=false;
  for (int ll=0; ll<numOfBadRuns; ll++)
    {
    if (runNumber==badRunList[ll])
      {
      badRun=true;
      break;
      }
    }
  //make the analysis only on well known goodRuns (=not 100% inactive)
  //comment this line if you want to observe everything
  //if (!badRun) return;

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
           
       //****************************************
       //Tracks in Pixel volume
       
       if (inPixelVolume)
       {
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
       for (trackingRecHit_iterator iHit = tkIter->recHitsBegin(); iHit != tkIter->recHitsEnd(); ++iHit)
         {
	  
         //check if the recHit belong to Pixel otehrwise go to the next recHit
         int type =(*iHit)->geographicalId().subdetId();           
         if ( !( type==int(kBPIX)|| type==int(kFPIX)) ) continue;

	 //DEBUG THIS *&$$!!! FLAG
	 numPixHit++;
	 if ( (*iHit)->getType()==TrackingRecHit::inactive )  numInactiveHit++;
	 if ( (*iHit)->getType()==TrackingRecHit::missing )   numMissingHit++;
	 
         //run analysis
	 if ( ! ((*iHit)->isValid()) )  invalidPerRun->Fill(testLabel,1);
	 if (   ((*iHit)->isValid()) )  validPerRun->Fill(testLabel,1);
	 if ( (*iHit)->getType()==TrackingRecHit::inactive ) inactivePerRun->Fill(testLabel,1);
	 
	 //***********************************
	 //bad module list and type of invalid recHit
	 
	 //check if belonging to badmodule list
	 bool badModule=false;
         for(Parameters::iterator it = BadModuleList_.begin(); it != BadModuleList_.end(); ++it) {         
           if ( ((*iHit)->geographicalId().rawId())==(it->getParameter<uint32_t>("detid")) ) {badModule=true; break;}
	   }
	
	 //let's observe better invalid not missing recHits
         int specificInvalid = 0;  //something else than inactive or missing or bad
	 if ( ! ((*iHit)->isValid()) )
	   {
	   if ( (*iHit)->getType()==TrackingRecHit::inactive )     specificInvalid = 1;
	   if ( (*iHit)->getType()==TrackingRecHit::bad )          specificInvalid = 2;
	   if ( (*iHit)->getType()==TrackingRecHit::missing )      specificInvalid = 3;
	   }
	 if ( ( (*iHit)->isValid() && badModule ) )
	   {
	   specificInvalid = 4;
	   }
	 
	 if ( ! ((*iHit)->isValid()) ) histInvalidRecHitCollection->Fill(specificInvalid);
	 
	 //histWithBadmoduleList
	 //fill 4th bin when we have a valid recHit belonging to badModule list: we expect 0 !!!
	 if (specificInvalid == 4)                    histInvalidRecHitWithBadmoduleList->Fill(specificInvalid);
	 //the other 3 bins are filled for the invalid statistic and excluding bad module list
	 if ( (!((*iHit)->isValid()))&&(!(badModule)) )  histInvalidRecHitWithBadmoduleList->Fill(specificInvalid);
	 
	 int moduleRawId = (*iHit)->geographicalId().rawId();
	 //maps stuffs
	 //consider only good runs (ie not 100% inactive)
	 if (!badRun)
	 {
	 if (badModule)
	   {
	   bool founded=0;
           for (int it=0; it<badModuleMap.size(); it++)
	     {
	     if (badModuleMap[it][0]==moduleRawId)
	       {
	       founded=1;
	       if ( (*iHit)->getType()==TrackingRecHit::inactive ) (badModuleMap[it])[1]++;
	       if ( (*iHit)->getType()==TrackingRecHit::missing )  (badModuleMap[it])[2]++;
	       if ( (*iHit)->isValid() )                           (badModuleMap[it])[3]++;      
	       break;
	       }
	     }
           if (!founded)
             {
	     vector<int> aux;
	     aux.push_back(moduleRawId);
	     for (int j=1;j<=4; j++) aux.push_back(0);
	     badModuleMap.push_back(aux);
             if ( (*iHit)->getType()==TrackingRecHit::inactive ) (badModuleMap[badModuleMap.size()-1])[1]++;
	     if ( (*iHit)->getType()==TrackingRecHit::missing )  (badModuleMap[badModuleMap.size()-1])[2]++;
	     if ( (*iHit)->isValid() )                           (badModuleMap[badModuleMap.size()-1])[3]++;     
	     if ( type==int(kBPIX) ) (badModuleMap[badModuleMap.size()-1])[4]= 1;
	     if ( type==int(kFPIX) )  (badModuleMap[badModuleMap.size()-1])[4]= 2;
	     }
	   }
	 else
	   {
	   //if ( !((*iHit)->isValid()) )
	     {
	     bool founded=0;
             for (int it=0; it<goodModuleMap.size(); it++)
	       {
	       if (goodModuleMap[it][0]==moduleRawId)
	         {
	         founded=1;
	         if ( (*iHit)->getType()==TrackingRecHit::inactive ) (goodModuleMap[it])[1]++;
	         if ( (*iHit)->getType()==TrackingRecHit::missing )  (goodModuleMap[it])[2]++;
		 if ( (*iHit)->isValid() )                           (goodModuleMap[it])[3]++;
	         break;
	         }
	       }
             if (!founded)
               {
	       vector<int> aux;
	       aux.push_back(moduleRawId);
	       for (int j=1;j<=4; j++) aux.push_back(0);
	       goodModuleMap.push_back(aux);
               if ( (*iHit)->getType()==TrackingRecHit::inactive ) (goodModuleMap[goodModuleMap.size()-1])[1]++;
	       if ( (*iHit)->getType()==TrackingRecHit::missing )  (goodModuleMap[goodModuleMap.size()-1])[2]++;
	       if ( (*iHit)->isValid() )                           (goodModuleMap[goodModuleMap.size()-1])[3]++;
	       if ( type==int(kBPIX) ) (goodModuleMap[goodModuleMap.size()-1])[4]= 1;
	       if ( type==int(kFPIX) ) (goodModuleMap[goodModuleMap.size()-1])[4]= 2;
  	       }
	     }
	   }
	   }//end-if not badRun
	 
	 //***********************************
	 //Fill with valid/missing recHits
	 
	 //make statistic only on not badModule
	 if (badModule) continue;
	 
	 //operation done for all the Pixel recHit
         int filling = 0;
         if((*iHit)->isValid() ) filling=2;
         if (!((*iHit)->isValid()) ) filling=0;
	 if (!((*iHit)->isValid()) && (*iHit)->getType()==TrackingRecHit::missing) filling=1;	 
	 
	 histo->Fill(filling);
	 DetId detId = ((*iHit)->geographicalId());
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
	
       //no check on denominator: for sure at least 1 recHit in previous filling 
       totalPerTrack->Fill( numPixHit );
       inactivePerTrack->Fill( (numInactiveHit/numPixHit)*100.);
       missingPerTrack->Fill( (numMissingHit/numPixHit)*100.);
	 
       } //if-else inPixelVolume 
     } // end of loop on tracks
      
   
  //**************************************************************
  //residual (loop on traj)
  
  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
  iEvent.getByLabel(trajectoryInput_,trajTrackCollectionHandle);

try{
  edm::Handle< edmNew::DetSetVector<SiPixelCluster> > theClusters;
  iEvent.getByLabel(pixelClusterInput_, theClusters);
  const edmNew::DetSetVector<SiPixelCluster>& input = *theClusters;

  int NbrTracks =  trajTrackCollectionHandle->size();
  checkoutTraj->Fill(NbrTracks);

  TrajectoryStateCombiner tsoscomb;
  for(TrajTrackAssociationCollection::const_iterator it = trajTrackCollectionHandle->begin(),
      itEnd = trajTrackCollectionHandle->end(); it!=itEnd;++it){
    
    consistencyCheckTraj->Fill(0);//number of total trajectories
    const Trajectory& traj  = *it->key;
    std::vector<TrajectoryMeasurement> tmColl = traj.measurements();

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
          
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	itTraj != itTrajEnd; ++itTraj) {
  
      //are we in the pixels with valid stuffs?
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
      //check if in PixelDetector
      uint testSubDetID = (testhit->geographicalId().subdetId());
      if(! (testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) ) continue;
      //check if belonging to badmodule list
      bool badModule=false;
      for(Parameters::iterator it = BadModuleList_.begin(); it != BadModuleList_.end(); ++it) {         
        if ( (testhit->geographicalId().rawId())==(it->getParameter<uint32_t>("detid")) ) {badModule=true; break;}
	}
      if (badModule) continue;

      TrajectoryStateOnSurface tsos = tsoscomb( itTraj->forwardPredictedState(), itTraj->backwardPredictedState() );


      if (! (testSubDetID==kBPIX) ) continue;             
      //**********************************
      //eta-angular analysis of efficiency
      LocalTrajectoryParameters ltp = tsos.localParameters();
      LocalVector localDir = ltp.momentum()/ltp.momentum().mag();

      float locx = localDir.x();
      float locy = localDir.y();
      float locz = localDir.z();

      float alpha = atan2( locz, locx );
      if ((*testhit).getType()== TrackingRecHit::valid)  validVsAlpha->Fill( alpha );
      if ((*testhit).getType()== TrackingRecHit::missing)   invalidVsAlpha->Fill( alpha);

      float  beta = atan2( locz, locy );
      if ((*testhit).getType()== TrackingRecHit::valid)  validVsBeta->Fill( beta );
      if ((*testhit).getType()== TrackingRecHit::missing)   invalidVsBeta->Fill( beta);

      //**********************************
      //window searching analysis

      DetId hitDetId = (testhit->geographicalId());
      PXBDetId hitPdetId = PXBDetId(hitDetId);
      unsigned int hitLayer=0;
      hitLayer=hitPdetId.layer();        

//test of existence of PROPER validity-flag handling trajectories
//make statistic both on valid/invalid!
if ( (*testhit).getType()==TrackingRecHit::inactive ) checkoutValidityFlag->Fill(0);
else if ((*testhit).getType()==TrackingRecHit::missing) checkoutValidityFlag->Fill(1);
else  if ((*testhit).getType()==TrackingRecHit::valid) checkoutValidityFlag->Fill(2);
else checkoutValidityFlag->Fill(3);

      if ( (*testhit).getType()==TrackingRecHit::missing)
        {
        //set this better in case of no founded cluster at all...
	double minDistance=999999;
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
	    if (distance<minDistance) minDistance=distance;
	    }//end-for first cluster loop	   
	  }//end-for second cluster loop 
        //std::cout<<"previous of filling"<<std::endl;
	windowSearch->Fill(minDistance); 
	}//end-if "work-on-not-valid"    
      }//end-for trajMeasurements      
    }//end-for of Trajectories
}catch ( ... ) {}
   
   } catch ( cms::Exception& er ) {
     std::cout<<"caught cicciburicci std::exception "<<er.what()<<std::endl;
   } catch ( ... ) {
     std::cout<<" funny error " <<std::endl;
   }

   if(DEBUG) cout<<"End of Analyze"<<endl;

}

// ------------ method called once each job just before starting event loop  ------------

void 
PixelEfficiency::beginJob(const edm::EventSetup&)
{
 if(DEBUG) std::cout<<"Begin job"<<std::endl;

 fOutputFile = new TFile(fOutputFileName.c_str(), "RECREATE"); 
 
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

 windowSearch = new TH1F("windowSearch","windowSearch",100,0,0.10);
 validPerRun = new TH1F("validPerRun","validPerRun",200,0,200);
 invalidPerRun = new TH1F("invalidPerRun","invalidPerRun",200,0,200);
 inactivePerRun = new TH1F("inactivePerRun","inactivePerRun",200,0,200);
 
 validVsAlpha = new TH1F("validVsAlpha","validVsAlpha",200,-3.5,3.5);
 invalidVsAlpha = new TH1F("invalidVsAlpha","invalidVsAlpha",200,-3.5,3.5);
 validVsBeta = new TH1F("validVsBeta","validVsBeta",200,-3.5,3.5);
 invalidVsBeta = new TH1F("invalidVsBeta","invalidVsBeta",200,-3.5,3.5);

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
  validVsAlpha->Write();
  invalidVsAlpha->Write();
  validVsBeta->Write();
  invalidVsBeta->Write();
  
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
      barrelTree=(goodModuleMap[l])[4] ;
      tree->Fill();
      }
    tree->Write();
   
  fOutputFile->Write() ;
  fOutputFile->Close() ;

}

//define this as a plug-in
//DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(PixelEfficiency);
