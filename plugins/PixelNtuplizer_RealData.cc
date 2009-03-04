// File: PixelNtuplizer_RealData.cc
// Description: Adapted tree structure from PixelNtuplizer package
//   		Adapted method for finding residual information from TrackerValidationVariables package
// Authors: Andrew York, Tennessee
//          Freya Blekman, Cornell
//
//
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "PixelNtuplizer_RealData.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "PhysicsTools/UtilAlgos/interface/PhysObjectMatcher.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"

// For ROOT
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

// STD
#include <memory>
#include <string>
#include <iostream>

using namespace std;
using namespace edm;
using namespace reco;

PixelNtuplizer_RealData::PixelNtuplizer_RealData(edm::ParameterSet const& iConfig) : 
  conf_(iConfig),
  tfile_(0), 
  t_(0),
  maxsize_PixInfoStruct_(200)
{
}

// Virtual destructor needed.
PixelNtuplizer_RealData::~PixelNtuplizer_RealData() { }  

// End job: write and close the ntuple file
void PixelNtuplizer_RealData::endJob() 
{

  std::string outputFileName = conf_.getParameter<std::string>("OutputFile");
  std::cout << " PixelNtuplizer_RealData::endJob" << std::endl;
  tfile_->Write();
  tfile_->Close();

}


void PixelNtuplizer_RealData::beginJob(const edm::EventSetup& es)
{
  es.get<TrackerDigiGeometryRecord>().get( tkGeom_ );
  es.get<IdealMagneticFieldRecord>().get(magneticField_);

  // put here whatever you want to do at the beginning of the job
  std::string outputFile = conf_.getParameter<std::string>("OutputFile");
  tfile_ = new TFile ( outputFile.c_str() , "RECREATE" );

  t_ = new TTree("PixNtuple", "Pixel hit analyzer ntuple");
  //  ts_ = new TTree("StripNtuple", "Strip hit analyzer ntuple");
  tt_ = new TTree("TrackNtuple", "Counters filled every track");
  int bufsize = 64000;

  // Create one branch. If splitlevel is set, event is a superbranch
  // creating a sub branch for each data member of the Event object.
  t_->Branch("evt", &evt_, "run/I:evtnum:nbrTracks", bufsize);
  
  t_->Branch("det", &det_, "thickness/F:cols/I:rows/I:layer/I:ladder/I:module/I:disk/I:blade/I:panel/I:plaquette/I", bufsize);

  std::cout << "Making vertex branch:" << std::endl;
  t_->Branch("vertex",   &vertex_,   "r/F:z", bufsize);

  std::cout << "Making cluster branch:" << std::endl;
  t_->Branch("Cluster", &clust_, "row/F:col:x:y:charge:normalized_charge:size/I:size_x:size_y:maxPixelCol:maxPixelRow:minPixelCol:minPixelRow:geoId/i:edgeHitX/I:edgeHitY:clust_alpha/F:clust_beta:n_neighbour_clust/I", bufsize);
  
  std::cout << "Making pixinfo branch:" << std::endl;
  t_->Branch("npix", &pixinfo_.npix, "npix/I", bufsize);
  t_->Branch("hasOverFlow",&pixinfo_.hasOverFlow, "hasOverFlow/I",bufsize);
  t_->Branch("rowpix", pixinfo_.row, "row[npix]/F", bufsize);
  t_->Branch("colpix", pixinfo_.col, "col[npix]/F", bufsize);
  t_->Branch("adc", pixinfo_.adc, "adc[npix]/F", bufsize);
  t_->Branch("xpix", pixinfo_.x, "x[npix]/F", bufsize);
  t_->Branch("ypix", pixinfo_.y, "y[npix]/F", bufsize);
  t_->Branch("gxpix", pixinfo_.gx, "gx[npix]/F", bufsize);
  t_->Branch("gypix", pixinfo_.gy, "gy[npix]/F", bufsize);
  t_->Branch("gzpix", pixinfo_.gz, "gz[npix]/F", bufsize);

  std::cout << "Making allpixinfo branch:" << std::endl;
  t_->Branch("allpix_npix", &allpixinfo_.allpix_npix, "allpix_npix/I", bufsize);
  t_->Branch("allpix_hasOverFlow",&allpixinfo_.allpix_hasOverFlow, "allpix_hasOverFlow/I",bufsize);
  t_->Branch("allpixInfo",&allpixinfo_,"allpix_row[200]/F:allpix_col[200]/F:allpix_adc[200]/F",bufsize);

  // t_->Branch("allpix_npix", &allpixinfo_.allpix_npix, "allpix_npix/I", bufsize);
  //  t_->Branch("allpix_hasOverFlow",&allpixinfo_.allpix_hasOverFlow, "allpix_hasOverFlow/I",bufsize);
  //t_->Branch("allpix_rowpix", allpixinfo_.allpix_row, "allpix_row[allpix_npix]/F", bufsize);
  //t_->Branch("allpix_colpix", allpixinfo_.allpix_col, "allpix_col[allpix_npix]/F", bufsize);
  //t_->Branch("allpix_adc"   , allpixinfo_.allpix_adc, "allpix_adc[allpix_npix]/F", bufsize);

 std::cout << "Making allclustinfo branch:" << std::endl;

 //  t_->Branch("allclustInfo",&allclustinfo_,"allclust_row[100]/F:allclust_col[100]:allclust_x[100]:allclust_y[100]:allclust_charge[100]:allclust_nor_charge[100]:allclust_size[100]:allclust_size_x[100]:allclust_size_y[100]:allclust_maxPixelCol[100]:allclust_maxPixelRow[100]:allclust_minPixelCol[10000]:allclust_minPixelRow[100]:allclust_geoId[100]:allclust_edgeHitX[100]:allclust_edgeHitY[100]:allclust_dist[100]",bufsize);

  t_->Branch("allclust_hasOverFlow", &allclustinfo_.allclust_hasOverFlow, "allclust_hasOverFlow/I"                 , bufsize);
  t_->Branch("n_allclust"          , &allclustinfo_.n_allclust         , "n_allclust/I"                           , bufsize);
  
  
  t_->Branch("allclust_row"        , allclustinfo_.allclust_row        , "allclust_row[n_allclust]/F"        , bufsize);
  t_->Branch("allclust_col"        , allclustinfo_.allclust_col        , "allclust_col[n_allclust]/F"        , bufsize);
  t_->Branch("allclust_x"          , allclustinfo_.allclust_x          , "allclust_x[n_allclust]/F"          , bufsize);
  t_->Branch("allclust_y"          , allclustinfo_.allclust_y          , "allclust_y[n_allclust]/F"          , bufsize);
  t_->Branch("allclust_charge"     , allclustinfo_.allclust_charge     , "allclust_charge[n_allclust]/F"     , bufsize);
  t_->Branch("allclust_nor_charge" , allclustinfo_.allclust_nor_charge , "allclust_nor_charge[n_allclust]/F" , bufsize);
  t_->Branch("allclust_size"       , allclustinfo_.allclust_size       , "allclust_size[n_allclust]/F"       , bufsize);
  t_->Branch("allclust_size_x"     , allclustinfo_.allclust_size_x     , "allclust_size_x[n_allclust]/F"     , bufsize);
  t_->Branch("allclust_size_y"     , allclustinfo_.allclust_size_y     , "allclust_size_y[n_allclust]/F"     , bufsize);
  t_->Branch("allclust_maxPixelCol", allclustinfo_.allclust_maxPixelCol, "allclust_maxPixelCol[n_allclust]/F", bufsize);
  t_->Branch("allclust_maxPixelRow", allclustinfo_.allclust_maxPixelRow, "allclust_maxPixelRow[n_allclust]/F", bufsize);
  t_->Branch("allclust_minPixelCol", allclustinfo_.allclust_minPixelCol, "allclust_minPixelCol[n_allclust]/F", bufsize);
  t_->Branch("allclust_minPixelRow", allclustinfo_.allclust_minPixelRow, "allclust_minPixelRow[n_allclust]/F", bufsize);
  t_->Branch("allclust_geoId"      , allclustinfo_.allclust_geoId      , "allclust_geoId[n_allclust]/F"      , bufsize);
  t_->Branch("allclust_edgeHitX"   , allclustinfo_.allclust_edgeHitX   , "allclust_edgeHitX[n_allclust]/F"   , bufsize);
  t_->Branch("allclust_edgeHitY"   , allclustinfo_.allclust_edgeHitY   , "allclust_edgeHitY[n_allclust]/F"   , bufsize);
  t_->Branch("allclust_dist"       , allclustinfo_.allclust_dist       , "allclust_dist[n_allclust]/F"       , bufsize);

  



  //  t_->Branch("Cluster", &clust_, "row/F:col:x:y:charge:normalized_charge:size/I:size_x:size_y:maxPixelCol:maxPixelRow:minPixelCol:minPixelRow:geoId/i:edgeHitX/I:edgeHitY:clust_alpha/F:clust_beta", bufsize);

  std::cout << "Making muon branch:" << std::endl;
  t_->Branch("MuonInfo",&muoninfo_,"timeAtIpInOut[2]/F:errorTime[2]/F:IsGlobalMuon[2]/F:IsStandAloneMuon[2]/F:IsTrackerMuon[2]/F:IsTimeValid[2]/F:HasGlobalTrack[2]/F:HasPixelHit[2]/F:trackpt[2]/F:tracketa[2]/F:trackphi[2]/F",bufsize);
  t_->Branch("nMuon",&muoninfo_.nMuon,"nMuon/I",bufsize);
  t_->Branch("nMuonHasOverFlow",&muoninfo_.HasOverFlow,"nMuonHasOverFlow/I",bufsize);

  std::cout << "Making rechit branch:" << std::endl;
  t_->Branch("RecHit", &rechit_, "localX/F:localY:globalX:globalY:globalZ:residualX:residualY:resErrX:resErrY:hit_errX:hit_errY:resXprime:resXprimeErr", bufsize);

  std::cout << "Making track branch:" << std::endl;
  t_->Branch("track", &track_, "pt/F:p:px:py:pz:globalTheta:globalEta:globalPhi:localTheta:localPhi:chi2:ndof:foundHits/I:tracknum", bufsize);

  //  std::cout << "Making tracker hit branch:" << std::endl;
  //  ts_->Branch("TrackerHit", &trackerhits_, "globalX/F:globalY:globalZ:run/I:evtnum:tracknum", bufsize);

  std::cout << "Making track only branch:" << std::endl;
  tt_->Branch("TrackInfo", &trackonly_, "run/I:evtnum:tracknum:pixelTrack:NumPixelHits:NumStripHits:charge:chi2/F:ndof:theta:d0:dz:p:pt:px:py:pz:phi:eta:vx:vy:vz:muonT0:muondT0", bufsize);
  
  std::cout << "Made all branches." << std::endl;


  /*  ifstream f("test.txt");

  while(!f.eof())
    {
      int w0,s0,w1,s1;
      float b,r,p;
      
      f >>  w0 >> s0 >> w1 >> s1 >> p >>  b >> r;
      if (!f.good()) break;
      
      points[w0][s0][w1][s1] = p;
      bias[w0][s0][w1][s1] = b;
      rms[w0][s0][w1][s1] = r;
      std::cout << w0 << " " << s0 << " " << w1 << " "<< s1 <<" " << b << " " << p << " " << r <<  std::endl;
    }
  
  */
  


}


bool PixelNtuplizer_RealData::isValidMuonAssoc(const edm::Event& iEvent){

 
  // get the ccmuons
    edm::Handle<MuonCollection> MuonHandle;
    //  iEvent.getByLabel("GLBMuons", MuonHandle);
     iEvent.getByLabel("muons", MuonHandle);
    if ( !MuonHandle.isValid() ) {
      std::cout << "No Muon results for InputTag " << std::endl;
      return false;
    }

    // currently unused.
    //const MuonCollection & muoninfo  =  *MuonHandle.product();
   
    int count = 0;
    muoninfo_.init();
    //std::cout << "muon collection size " << MuonHandle->size() << std::endl;
    int maxSize = MuonHandle->size();
    
      if(MuonHandle->size() > 2){
      muoninfo_.HasOverFlow = 1;
      maxSize = 2;
      muoninfo_.nMuon = 2;
    }
    else muoninfo_.nMuon = MuonHandle->size();
      


    // std::cout << " muon size " << std::endl;
      for(MuonCollection::const_iterator it = MuonHandle->begin(), itEnd = MuonHandle->end(); it!=itEnd;++it){
     
      if(count > 1) return false;

      if(!it->globalTrack())muoninfo_.HasGlobalTrack[count] = 0;
      else muoninfo_.HasGlobalTrack[count] = 1;
   

      muoninfo_.IsGlobalMuon[count] = (float)it->isGlobalMuon();
     
      muoninfo_.IsStandAloneMuon[count] = (float)it->isStandAloneMuon();
     
      muoninfo_.IsTrackerMuon[count] = (float)it->isTrackerMuon();
    
      muoninfo_.IsTimeValid[count] = (float) it->isTimeValid();

      if(it->isTimeValid() == true){
	muoninfo_.timeAtIpInOut[count] = it->time().timeAtIpInOut;
	
	muoninfo_.errorTime[count] = it->time().timeAtIpInOutErr;
	//std::cout << muoninfo_.timeAtIpInOut[count] << " " << 	muoninfo_.errorTime[count] << " " << count << std::endl;
      }
      else {
	muoninfo_.timeAtIpInOut[count] = -9999;
	muoninfo_.errorTime[count] = -9999;

      }

      //  std::cout << " muon " << count << " time " << muoninfo_.timeAtIpInOut[count] << " error " << muoninfo_.errorTime[count] << std::endl;
           

      if(!it->globalTrack() == false){
	muoninfo_.trackpt[count] = it->globalTrack()->pt();
	muoninfo_.tracketa[count] = it->globalTrack()->eta();
	muoninfo_.trackphi[count] = it->globalTrack()->phi();
      }
      else {
	muoninfo_.trackpt[count] = -9999;
	muoninfo_.tracketa[count] = -9999;
	muoninfo_.trackphi[count] = -9999;
      }
      //std::cout << " has global track " << muoninfo_.HasGlobalTrack[count] <<std::endl;
 
      if(  !it->globalTrack() == false){ 

	bool isMuonPixelHit = false; 

	for(size_t hit = 0; hit < it->globalTrack()->recHitsSize();hit++){
	  //if hit is valid and in tracker say true
	  if(it->globalTrack()->recHit(hit)->isValid() == true && it->globalTrack()->recHit(hit)->geographicalId().det() == DetId::Tracker){
	    uint testSubDetID = it->globalTrack()->recHit(hit)->geographicalId().subdetId();
	    //if hit is in pixel detector say true
	    if(testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) isMuonPixelHit = true;
	  }
	}//end loop over muon hits
	muoninfo_.HasPixelHit[count] = (float)isMuonPixelHit;

      
      }//end asking if time valid and has global track
      else{
	muoninfo_.HasPixelHit[count] = false;

      }

      count++;
    } //end looping over muons
    
   
    

    return true;

}



// Functions that get called by framework every event
void PixelNtuplizer_RealData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // std::cout << " here " << endl;
  int TrackNumber = 0;
  maxsize_AllPixInfoStruct_ = 200;
  maxsize_AllClustInfoStruct_ = 100;

 
  for(int i = 0; i < maxsize_AllPixInfoStruct_; i++){
    allpixinfo_.allpix_row[i] = -999;
    allpixinfo_.allpix_col[i] = -999;
    allpixinfo_.allpix_adc[i] = -999;
  }

	init();

  trackonly_.init();

  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
  //edm::Handle<std::vector<Trajectory> > trajCollectionHandle;
  iEvent.getByLabel(conf_.getParameter<std::string>("trajectoryInput"),trajTrackCollectionHandle);

  TrajectoryStateCombiner tsoscomb;
  int NbrTracks =  trajTrackCollectionHandle->size();
  // std::cout << " track measurements " << trajTrackCollectionHandle->size()  << std::endl;


  for(TrajTrackAssociationCollection::const_iterator it = trajTrackCollectionHandle->begin(), itEnd = trajTrackCollectionHandle->end(); it!=itEnd;++it){

    TrackNumber++;
    int pixelHits = 0;
    int stripHits = 0;
    const Track&      track = *it->val;
    const Trajectory& traj  = *it->key;
    
  

    std::vector<TrajectoryMeasurement> checkColl = traj.measurements();
    for(std::vector<TrajectoryMeasurement>::const_iterator checkTraj = checkColl.begin(), checkTrajEnd = checkColl.end();
	checkTraj != checkTrajEnd; ++checkTraj) {

      if(! checkTraj->updatedState().isValid()) continue;
      TransientTrackingRecHit::ConstRecHitPointer testhit = checkTraj->recHit();
      if(! testhit->isValid() || testhit->geographicalId().det() != DetId::Tracker ) continue;
      uint testSubDetID = (testhit->geographicalId().subdetId());
      if(testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) pixelHits++;
      else if (testSubDetID == StripSubdetector::TIB || testSubDetID == StripSubdetector::TOB ||
	       testSubDetID == StripSubdetector::TID || testSubDetID == StripSubdetector::TEC) stripHits++;

    }

    fillTrackOnly(iEvent,iSetup, pixelHits, stripHits, TrackNumber, track);
    //++++++++++
    tt_->Fill();
    //++++++++++

    // std::cout << " pixelHits " << pixelHits << std::endl;

    if (pixelHits == 0) continue;

    std::vector<TrajectoryMeasurement> tmColl = traj.measurements();
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	itTraj != itTrajEnd; ++itTraj) {

      if(! itTraj->updatedState().isValid()) continue;

      TrajectoryStateOnSurface tsos = tsoscomb( itTraj->forwardPredictedState(), itTraj->backwardPredictedState() );
      TransientTrackingRecHit::ConstRecHitPointer hit = itTraj->recHit();
      if(! hit->isValid() || hit->geographicalId().det() != DetId::Tracker ) {
	continue; 
      } else {
	const DetId & hit_detId = hit->geographicalId();
	//	uint IntRawDetID = (hit_detId.rawId()); // currently unused
	uint IntSubDetID = (hit_detId.subdetId());
	
	if(IntSubDetID == 0 )
	  continue;
	
	align::LocalVector res = tsos.localPosition() - hit->localPosition();

	LocalError err1 = tsos.localError().positionError();
	LocalError err2 = hit->localPositionError();
	
	float errX = std::sqrt( err1.xx() + err2.xx() );
	float errY = std::sqrt( err1.yy() + err2.yy() );
	
	//std::cout << "Residual x/y " << res.x() << '/' << res.y() 
	// << ", Error x/y " << errX << '/' << errY;		

	// begin partly copied from Tifanalyser 

	const GeomDetUnit* detUnit = hit->detUnit();
	double dPhi = -999, dR = -999, dZ = -999, phiorientation = -999;
	//double R = 0.; // currently unused
	//double origintointersect = 0.; // currently unused

	if(detUnit) {
	  const Surface& surface = hit->detUnit()->surface();
	  LocalPoint lPModule(0.,0.,0.), lPhiDirection(1.,0.,0.), lROrZDirection(0.,1.,0.);
	  GlobalPoint gPModule       = surface.toGlobal(lPModule),
	    gPhiDirection  = surface.toGlobal(lPhiDirection),
	    gROrZDirection = surface.toGlobal(lROrZDirection);
	  phiorientation = deltaPhi(gPhiDirection.phi(),gPModule.phi()) >= 0 ? +1. : -1.;

	  dPhi = tsos.globalPosition().phi() - hit->globalPosition().phi();
	  
	  if(IntSubDetID == PixelSubdetector::PixelBarrel || IntSubDetID == PixelSubdetector::PixelEndcap) {

            const TrackerGeometry& theTracker(*tkGeom_);

	    const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(hit_detId) );

            const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));

            // get the enclosed persistent hit
            const TrackingRecHit *persistentHit = hit->hit();
            // check if it's not null, and if it's a valid pixel hit
            if ((persistentHit != 0) && (typeid(*persistentHit) == typeid(SiPixelRecHit))) {
              // tell the C++ compiler that the hit is a pixel hit
              const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>( hit->hit() );
              // get the edm::Ref to the cluster
              edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = (*pixhit).cluster();
              //  check if the ref is not null
              if (clust.isNonnull()) {

		init();
		rechit_.localX = hit->localPosition().x();
		rechit_.localY = hit->localPosition().y();
		rechit_.globalX = hit->globalPosition().x();
		rechit_.globalY = hit->globalPosition().y();
		rechit_.globalZ = hit->globalPosition().z();
		rechit_.residualX = res.x();
		rechit_.resErrX = errX;
		rechit_.resErrY = errY;
		rechit_.hit_errX = sqrt(err2.xx());
		rechit_.hit_errY = sqrt(err2.yy());
	        rechit_.resXprime = (res.x())*(phiorientation );
	        rechit_.resXprimeErr = errX;
		dZ = gROrZDirection.z() - gPModule.z();
		if(dR != -999) rechit_.residualY = dR;
		else if(dZ != -999) rechit_.residualY = res.y() * (dZ >=0.? +1 : -1) ;
		else rechit_.residualY = res.y();

		isValidMuonAssoc(iEvent);

		isOffTrackHits(iEvent,*clust, iSetup,topol,theGeomDet->geographicalId().rawId(),tsos);


                // get the contents
                fillEvt(iEvent,NbrTracks);
                fillDet(hit_detId, IntSubDetID, theGeomDet);
		fillVertex(theGeomDet);
                fillClust(*clust, topol, theGeomDet, tsos);
                fillPix(*clust, topol, theGeomDet);
                fillTrack( tsos, traj, TrackNumber);
                //fillRecHit(pixeliter, topol, theGeomDet);

      	        //++++++++++
	        t_->Fill();
	        //++++++++++

              } // end if(cluster is valid)
            } // end if(hit is pixel hit)

	  } else if (IntSubDetID == StripSubdetector::TIB || IntSubDetID == StripSubdetector::TOB ||
		     IntSubDetID == StripSubdetector::TID || IntSubDetID == StripSubdetector::TEC) {

	    //fillTrackerHits(iEvent,hit);
	    //++++++++++
	    //ts_->Fill();
	    //++++++++++

	  } else {
	    //std::cout << "@SUB=PixelNtuplizer_RealData::fillHitQuantities" 
	    //  << "No valid tracker subdetector " << IntSubDetID;
	    rechit_.resXprime = -999;
	  }  // if-else to differentiate pixel hits vs tracker hits	  
	}  // end if(good detUnit)
      }  // end else (if-else to screen out invalid hits)
    }  // end loop over trajectory measurements (rec hits) 
  }  // end loop over trajectories
 }  // end analyze function


bool PixelNtuplizer_RealData::isOffTrackHits(const edm::Event& iEvent,const SiPixelCluster& clust, const edm::EventSetup & iSetup,const RectangularPixelTopology* topol, uint32_t geoId, TrajectoryStateOnSurface& tsos ){
  
  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord> ().get (pDD);
  const TrackerGeometry* tracker = &(* pDD);

  allpixinfo_.allpix_hasOverFlow = 0;
  allclustinfo_.allclust_hasOverFlow = 0;

  allclustinfo_.n_allclust = 0;
  allpixinfo_.allpix_npix = 0;

  // calculate alpha and beta from cluster position
  LocalTrajectoryParameters ltp = tsos.localParameters();
  LocalVector localDir = ltp.momentum()/ltp.momentum().mag();

  float locx = localDir.x();
  float locy = localDir.y();
  float locz = localDir.z();
  //float loctheta = localDir.theta(); // currently unused

  float clust_alpha = atan2( locz, locx );
  float clust_beta = atan2( locz, locy );

  LocalPoint lpc = topol->localPosition(MeasurementPoint(clust.x(),clust.y()));
  float clust_x = lpc.x();
  float clust_y = lpc.y();

  if(clust_x > 1000) cout << " not valid x too large "<< clust_x << endl;



 //-----Iterate over detunits
			  //cout << endl << "Start search for det unit..." << endl;
			  bool found_det_unit = false;
			  for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++) 
			    {
			      DetId detId = ( (*it)->geographicalId() );
			      if ( (int)detId.subdetId() != (int)PixelSubdetector::PixelBarrel && 
				   (int)detId.subdetId() != (int)PixelSubdetector::PixelEndcap )
				{
				  // cout << "Not a pixel detector. Skip it." << endl;
				  continue;
				}
			      
			      if ( detId.rawId() != geoId )
				continue;
			      else if ( detId.rawId() == geoId ) 
				{
				  // cout << "Found the det unit for this RecHit = " << (int)detId.rawId() << endl;
				  found_det_unit = true;

				  edm::Handle<SiPixelRecHitCollection> recHitColl;
				  iEvent.getByLabel( "siPixelRecHits", recHitColl );
				  
				  SiPixelRecHitCollection::range pixelrechitRange = (recHitColl.product())->get(detId);
				  SiPixelRecHitCollection::const_iterator pixelrechitRangeIteratorBegin = pixelrechitRange.first;
				  SiPixelRecHitCollection::const_iterator pixelrechitRangeIteratorEnd = pixelrechitRange.second;
				  SiPixelRecHitCollection::const_iterator pixeliter = pixelrechitRangeIteratorBegin;
				  				
				  int n_clust = 0;
				   int pixel_index = 0;
				  for ( ; pixeliter != pixelrechitRangeIteratorEnd; ++pixeliter) 
				    {
				      ++n_clust;
				      
				      if ( (int)n_clust <= (int)maxsize_AllClustInfoStruct_ )
					{
					  //	  cout << "row " << pixeliter->cluster()->x() << endl;

					  allclustinfo_.allclust_row[n_clust-1] = pixeliter->cluster()->x();
					  allclustinfo_.allclust_col[n_clust-1] = pixeliter->cluster()->y();
					  
					  LocalPoint lp = topol->localPosition( MeasurementPoint(allclustinfo_.allclust_row[n_clust-1], 
												 allclustinfo_.allclust_col[n_clust-1]) );
					  
					  allclustinfo_.allclust_x[n_clust-1] = lp.x();          
					  allclustinfo_.allclust_y[n_clust-1] = lp.y();          
					  
					  allclustinfo_.allclust_charge[n_clust-1] = pixeliter->cluster()->charge()/1000.0;     
					  allclustinfo_.allclust_nor_charge[n_clust-1] = pixeliter->cluster()->charge() * 
					    sqrt( 1.0 / ( 1.0/pow( tan(clust_alpha), 2 ) + 
							  1.0/pow( tan(clust_beta ), 2 ) + 
							  1.0 )
						  );
					  
				          allclustinfo_.allclust_size[n_clust-1] = (float)pixeliter->cluster()->size();       
				          allclustinfo_.allclust_size_x[n_clust-1] = (float)pixeliter->cluster()->sizeX();     
					  allclustinfo_.allclust_size_y[n_clust-1] = (float)pixeliter->cluster()->sizeY();     
					  allclustinfo_.allclust_maxPixelCol[n_clust-1] = (float)pixeliter->cluster()->maxPixelCol();
					  allclustinfo_.allclust_maxPixelRow[n_clust-1] = (float)pixeliter->cluster()->maxPixelRow();
					  allclustinfo_.allclust_minPixelCol[n_clust-1] = (float)pixeliter->cluster()->minPixelCol();
					  allclustinfo_.allclust_minPixelRow[n_clust-1] = (float)pixeliter->cluster()->minPixelRow();
					  allclustinfo_.allclust_geoId[n_clust-1] = (float)detId.rawId();      
					  allclustinfo_.allclust_edgeHitX[n_clust-1] = (float)(int)(topol->isItEdgePixelInX(clust.minPixelRow()) || 
											     topol->isItEdgePixelInX(clust.maxPixelRow()));
					  allclustinfo_.allclust_edgeHitY[n_clust-1] = (float)(int)(topol->isItEdgePixelInY(clust.minPixelCol()) || 
											     topol->isItEdgePixelInY(clust.maxPixelCol()));
					  
					  
					 
					  allclustinfo_.allclust_dist[n_clust-1] 
					    = sqrt( (clust_x - allclustinfo_.allclust_x[n_clust-1])*
						    (clust_x - allclustinfo_.allclust_x[n_clust-1]) +
						    (clust_y - allclustinfo_.allclust_y[n_clust-1])*
						    (clust_y - allclustinfo_.allclust_y[n_clust-1]) );

					  // std::cout << " distance " << allclustinfo_.allclust_dist[n_clust-1] << "clustx " << clust_x << " allclust dist " << allclustinfo_.allclust_x[n_clust-1] << std::endl;


					    const std::vector<SiPixelCluster::Pixel>& pixvector = pixeliter->cluster()->pixels();
					  
					    

					    for (int i=0; i<(int)pixvector.size(); ++i )
					    {
					      if ( (int)pixel_index < (int)maxsize_AllPixInfoStruct_ )
						{
						  ++pixel_index;
						  SiPixelCluster::Pixel holdpix = pixvector[i];
						  allpixinfo_.allpix_row[pixel_index-1] = holdpix.x;
						  allpixinfo_.allpix_col[pixel_index-1] = holdpix.y;
						  allpixinfo_.allpix_adc[pixel_index-1] = holdpix.adc;
						}
					      else
						allpixinfo_.allpix_hasOverFlow = 1;
					      
					    }

					  
					
					  
					
					} // if ( n_clust <= maxsize_AllClustInfoStruct_ )
				      else
					allclustinfo_.allclust_hasOverFlow = 1;
				      
				    } 

			

				  //cout << "n_clust = " << n_clust << endl;
				  clust_.n_neighbour_clust = n_clust;

				  allclustinfo_.n_allclust = n_clust;
				  allpixinfo_.allpix_npix = pixel_index;

				  break;
				  
				} 
			      else cout << "Fuck this !!!" << endl;
			      
			    } 
			  			  
			  // if ( !found_det_unit )cout << "Did not find the det unit of this RecHit !!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;

			  return true;

}



void PixelNtuplizer_RealData::fillTrackOnly(const edm::Event& iEvent, const edm::EventSetup& iSetup, int pixelHits, int stripHits, int TrackNumber, const Track& track)
{
  if(pixelHits > 0) trackonly_.pixelTrack = 1;
  trackonly_.tracknum = TrackNumber;
  trackonly_.run = iEvent.id().run();
  trackonly_.evtnum = iEvent.id().event();
  trackonly_.NumPixelHits = pixelHits;
  trackonly_.NumStripHits = stripHits;
  trackonly_.chi2 = track.chi2();
  trackonly_.ndof = track.ndof();
  trackonly_.charge = track.charge();
  trackonly_.theta = track.theta();
  trackonly_.d0 = track.d0();
  trackonly_.dz = track.dz();
  trackonly_.p = track.p();
  trackonly_.pt = track.pt();
  trackonly_.px = track.px();
  trackonly_.py = track.py();
  trackonly_.pz = track.pz();
  trackonly_.phi = track.phi();
  trackonly_.eta = track.eta();
  trackonly_.vx = track.vx();
  trackonly_.vy = track.vy();
  trackonly_.vz = track.vz();

  trackonly_.muonT0=-1000;
  trackonly_.muondT0=0;
  if(muoninfo_.nMuon==0)
    return;
  trackonly_.muonT0=muoninfo_.timeAtIpInOut[0];
  trackonly_.muondT0=muoninfo_.errorTime[0];
  if(muoninfo_.nMuon==1)
    return;
  if(muoninfo_.errorTime[1]< muoninfo_.errorTime[2]){
    trackonly_.muonT0=muoninfo_.timeAtIpInOut[0];
    trackonly_.muondT0=muoninfo_.errorTime[0];
  }
  
  // load trackassociation from event, get muon out and read timing

  // TO DO
  
}

void PixelNtuplizer_RealData::fillEvt(const edm::Event& iEvent,int NbrTracks)
{
  evt_.run = iEvent.id().run();
  evt_.evtnum = iEvent.id().event();
  evt_.nbrTracks = NbrTracks;
}

void PixelNtuplizer_RealData::fillDet(const DetId &tofill, uint subdetid, const PixelGeomDetUnit* PixGeom)
{
  if (subdetid==1) 
    {
      det_.layer  = PXBDetId::PXBDetId(tofill).layer();
      det_.ladder = PXBDetId::PXBDetId(tofill).ladder();
      det_.module = PXBDetId::PXBDetId(tofill).module();
    } 
  else
    {
      det_.disk      =  PXFDetId::PXFDetId(tofill).disk();
      det_.blade     =  PXFDetId::PXFDetId(tofill).blade();
      det_.panel     =  PXFDetId::PXFDetId(tofill).panel();
      det_.plaquette =  PXFDetId::PXFDetId(tofill).module();
      
      //Following Danek's advice...
      //      unsigned int side = PXFDetId::PXFDetId(tofill).side();
      //      if (side==1) det_.disk = - det_.disk; 
    }
  det_.thickness = PixGeom->specificSurface().bounds().thickness();
  det_.cols = PixGeom->specificTopology().ncolumns();
  det_.rows = PixGeom->specificTopology().nrows();
}

void PixelNtuplizer_RealData::fillVertex(const PixelGeomDetUnit* PixGeom)
{
  vertex_.z = PixGeom->surface().position().z();
  vertex_.r = PixGeom->surface().position().perp();
}

void PixelNtuplizer_RealData::fillClust(const SiPixelCluster& matchIt, const RectangularPixelTopology* topol, const PixelGeomDetUnit* PixGeom, TrajectoryStateOnSurface& tsos) 
{//std::cout << " clustcharge "<< (matchIt.charge())/1000.0 << std::endl;
  clust_.charge = (matchIt.charge())/1000.0; // convert electrons to kilo-electrons
  clust_.size = matchIt.size();
  clust_.size_x = matchIt.sizeX();
  clust_.size_y = matchIt.sizeY();
  clust_.row = matchIt.x();
  clust_.col = matchIt.y();
  
  LocalPoint lp = topol->localPosition(MeasurementPoint(clust_.row,clust_.col));
  clust_.x = lp.x();
  clust_.y = lp.y();
  
  clust_.maxPixelCol = matchIt.maxPixelCol();
  clust_.maxPixelRow = matchIt.maxPixelRow();
  clust_.minPixelCol = matchIt.minPixelCol();
  clust_.minPixelRow = matchIt.minPixelRow();
  
  clust_.geoId = PixGeom->geographicalId().rawId();

  // Replace with the topology methods
  // edge method moved to topologi class
  clust_.edgeHitX = (int) ( topol->isItEdgePixelInX( clust_.minPixelRow ) || topol->isItEdgePixelInX( clust_.maxPixelRow ) );
  clust_.edgeHitY = (int) ( topol->isItEdgePixelInY( clust_.minPixelCol ) || topol->isItEdgePixelInY( clust_.maxPixelCol ) );

  // calculate alpha and beta from cluster position
  LocalTrajectoryParameters ltp = tsos.localParameters();
  LocalVector localDir = ltp.momentum()/ltp.momentum().mag();

  float locx = localDir.x();
  float locy = localDir.y();
  float locz = localDir.z();
  //float loctheta = localDir.theta(); // currently unused

  clust_.clust_alpha = atan2( locz, locx );
  clust_.clust_beta = atan2( locz, locy );

  clust_.normalized_charge = clust_.charge*sqrt(1.0/(1.0/pow(tan(clust_.clust_alpha),2)+1.0/pow(tan(clust_.clust_beta),2)+1.0));

}

void PixelNtuplizer_RealData::fillPix(const SiPixelCluster & LocPix, const RectangularPixelTopology * topol, const PixelGeomDetUnit * PixGeom)
{
  const std::vector<SiPixelCluster::Pixel>& pixvector = LocPix.pixels();
  if(pixvector.size()>maxsize_PixInfoStruct_)
    pixinfo_.hasOverFlow=1;
  for ( ; (size_t)pixinfo_.npix <pixvector.size() &&   (size_t)pixinfo_.npix< maxsize_PixInfoStruct_; ++pixinfo_.npix)
    {
     
      SiPixelCluster::Pixel holdpix = pixvector[pixinfo_.npix];
      pixinfo_.row[pixinfo_.npix] = holdpix.x;
      pixinfo_.col[pixinfo_.npix] = holdpix.y;
      pixinfo_.adc[pixinfo_.npix] = holdpix.adc;
      LocalPoint lp = topol->localPosition(MeasurementPoint(holdpix.x, holdpix.y));
      pixinfo_.x[pixinfo_.npix] = lp.x();
      pixinfo_.y[pixinfo_.npix] = lp.y();
      GlobalPoint GP =  PixGeom->surface().toGlobal(Local3DPoint(lp.x(),lp.y(),lp.z()));
      pixinfo_.gx[pixinfo_.npix] = GP.x();	
      pixinfo_.gy[pixinfo_.npix] = GP.y();
      pixinfo_.gz[pixinfo_.npix] = GP.z();
    }
}

void PixelNtuplizer_RealData::fillTrack(TrajectoryStateOnSurface& tsos,const Trajectory &traj, int TrackNumber) 
{
  track_.pt = tsos.globalMomentum().perp();
  track_.p = tsos.globalMomentum().mag();
  track_.px = tsos.globalMomentum().x();
  track_.py = tsos.globalMomentum().y();
  track_.pz = tsos.globalMomentum().z();
  track_.globalPhi = tsos.globalDirection().phi();
  track_.globalEta = tsos.globalDirection().eta();
  track_.globalTheta = tsos.globalDirection().theta();
  track_.localPhi = tsos.localDirection().phi();
  track_.localTheta = tsos.localDirection().theta();
  track_.chi2 = traj.chiSquared();
  track_.ndof = traj.ndof();
  track_.foundHits = traj.foundHits();
  track_.tracknum = TrackNumber;

}


void PixelNtuplizer_RealData::init() 
{
  evt_.init();
  det_.init();
  vertex_.init();
  clust_.init();
  pixinfo_.init();
  rechit_.init();
  track_.init();
  muoninfo_.init();
  allclustinfo_.init();
  allpixinfo_.init();
    
}

void PixelNtuplizer_RealData::EvtStruct::init()
{
  int dummy_int = -9999;

  run = dummy_int;
  evtnum = dummy_int;
}

void PixelNtuplizer_RealData::DetStruct::init()
{
  int dummy_int = -9999;
  float dummy_float = -9999.0;
 
  thickness = dummy_float; 
  cols  = dummy_int;
  rows  = dummy_int;
  layer = dummy_int;
  ladder = dummy_int;
  module = dummy_int;
  disk = dummy_int;
  blade = dummy_int;
  panel = dummy_int;
  plaquette = dummy_int;
}

void PixelNtuplizer_RealData::VertexStruct::init()
{
  float dummy_float = -9999.0;

  r = dummy_float;
  z = dummy_float;
 }

void PixelNtuplizer_RealData::ClusterStruct::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;
  unsigned int dummy_uint = 9999;
  
  row = dummy_float;
  col = dummy_float;
  x = dummy_float;
  y = dummy_float;
  charge = dummy_float;
  normalized_charge = dummy_float;
  size = dummy_int;
  size_x = dummy_int;
  size_y = dummy_int;
  maxPixelCol = dummy_int;
  maxPixelRow = dummy_int;
  minPixelCol = dummy_int;
  minPixelRow = dummy_int;
  geoId = dummy_uint;
  edgeHitX = dummy_int;
  edgeHitY = dummy_int;    
  clust_alpha = dummy_float;
  clust_beta = dummy_float;
  n_neighbour_clust = dummy_int;
}

void PixelNtuplizer_RealData::PixInfoStruct::init()
{
  npix = 0;
  hasOverFlow = 0;
 
} 





void PixelNtuplizer_RealData::RecHitStruct::init()
{
  float dummy_float = -9999.0;

  localX = dummy_float;
  localY = dummy_float;
  globalX = dummy_float;
  globalY = dummy_float;
  globalZ = dummy_float;
  residualX = dummy_float;
  residualY = dummy_float;
  resErrX = dummy_float;
  resErrY = dummy_float;
  hit_errX = dummy_float;
  hit_errY = dummy_float;
  resXprime = dummy_float;
  resXprimeErr = dummy_float;  
}

void PixelNtuplizer_RealData::TrackStruct::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;

  p = dummy_float;
  pt = dummy_float; 
  px = dummy_float;
  py = dummy_float;
  pz = dummy_float;   
  globalTheta = dummy_float;
  globalEta = dummy_float;
  globalPhi = dummy_float;
  localTheta = dummy_float;
  localPhi = dummy_float;
  chi2 = dummy_float;
  ndof = dummy_int;
  tracknum = dummy_int;
  foundHits = dummy_int;
}
/*
void PixelNtuplizer_RealData::TrackerHitStruct::init()
{
  float dummy_float = -9999.0;

  globalX = dummy_float;
  globalY = dummy_float;
  globalZ = dummy_float;
} */

void PixelNtuplizer_RealData::TrackOnlyStruct::init()
{
  int dummy_int = -9999;
  float dummy_float = -9999.0;

  run = dummy_int;
  evtnum = dummy_int;
  tracknum = dummy_int;
  pixelTrack = 0;
  NumPixelHits = dummy_int;
  NumStripHits = dummy_int;
  chi2 = dummy_float;
  ndof = dummy_float;
  charge = dummy_int;
  theta = dummy_float;
  d0 = dummy_float;
  dz = dummy_float;
  p = dummy_float;
  pt = dummy_float;
  px = dummy_float;
  py = dummy_float;
  pz = dummy_float;
  phi = dummy_float;
  eta = dummy_float;
  vx = dummy_float;
  vy = dummy_float;
  vz = dummy_float;
  muonT0 = dummy_float;
}

// define this as a plug-in
//
DEFINE_FWK_MODULE(PixelNtuplizer_RealData);
