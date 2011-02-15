
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
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "PixelNtuplizer_RealData.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "CondFormats/Alignment/interface/Definitions.h"
// SimDataFormats
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

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
#include <vector>

using namespace std;
using namespace edm;
using namespace reco;

PixelNtuplizer_RealData::PixelNtuplizer_RealData(edm::ParameterSet const& iConfig) : 
  conf_(iConfig),
  PixHitTree_(0),
  TrackTree_(0),
  maxsize_PixInfoStruct_(200)
{

}

// Virtual destructor needed.
PixelNtuplizer_RealData::~PixelNtuplizer_RealData() { }  

// End job: write and close the ntuple file
void PixelNtuplizer_RealData::endJob() 
{

}


void PixelNtuplizer_RealData::beginJob(const edm::EventSetup& es)
{
  es.get<TrackerDigiGeometryRecord>().get( tkGeom_ );
  es.get<IdealMagneticFieldRecord>().get(magneticField_);

  // put here whatever you want to do at the beginning of the job
  edm::Service<TFileService> fs;
  // a 'feature' of TFileService is that it does not like to add trees until a histogram is also created.
  dummyhist = fs->make<TH1D>("number_of_events","number of events",1,0,1);

  isCosmic = conf_.getParameter<bool>("isCosmic");
  if (isCosmic == true) readOffsets();

  isSim = conf_.getParameter<bool>("isSim");
  useAllPixel = conf_.getParameter<bool>("useAllPixel");

  // but after that it is ok to create a tree..
  PixHitTree_ = new TTree("PixNtuple", "Pixel hit analyzer ntuple");
  TrackTree_ = new TTree("TrackNtuple", "Counters filled every track");
  int bufsize = 64000;
  // Create one branch. If splitlevel is set, event is a superbranch
  // creating a sub branch for each data member of the Event object.
  //  std::cout << "Making evt branch:" << std::endl;
  PixHitTree_->Branch("evt", &evt_, "run/i:evtnum:lumiBlock:trackNumber:bunchCrossing/I:orbit", bufsize);
  //  std::cout << "Making det branch:" << std::endl;
  PixHitTree_->Branch("det", &det_, "thickness/F:cols/I:rows/I:layer/I:ladder/I:module/I:disk/I:blade/I:panel/I:plaquette/I:isflipped/I", bufsize);
  //  std::cout << "Making vertex branch:" << std::endl;
  PixHitTree_->Branch("vertex",   &vertex_,   "r/F:z", bufsize);
  //  std::cout << "Making clust branch:" << std::endl;
  PixHitTree_->Branch("Cluster", &clust_, "row/F:col:x:y:charge:normalized_charge:size/I:size_x:size_y:maxPixelCol:maxPixelRow:minPixelCol:minPixelRow:geoId/i:edgeHitX/I:edgeHitY:clust_alpha/F:clust_beta:n_neighbour_clust/I", bufsize);
  //  std::cout << "Making sim branch:" << std::endl;
  if(isSim == true) PixHitTree_->Branch("sim",   &sim_, "localX/F:localY/F:eloss/F:localPhi/F:localTheta/F:alpha/F:beta/F:charge/F:particleID/I:trackID/i:vertexID/I:entryPointLocalX/F:exitPointLocalX/F:entryPointLocalY/F:exitPointLocalY/F:entryPointLocalZ/F:exitPointLocalZ/F:entryPointRow/F:exitPointRow/F:entryPointColumn/F:exitPointColumn/F:entryPointGlobalX/F:exitPointGlobalX/F:entryPointGlobalY/F:exitPointGlobalY/F:entryPointGlobalZ/F:exitPointGlobalZ/F:processID/I:p/F:px/F:py/F:pz/F:globalEta/F:globalPhi/F:globalTheta/F", bufsize);
  //  std::cout << "Making pixinfo branch:" << std::endl;
  PixHitTree_->Branch("npix", &pixinfo_.npix, "npix/I", bufsize);
  PixHitTree_->Branch("hasOverFlow",&pixinfo_.hasOverFlow, "hasOverFlow/I",bufsize);
  PixHitTree_->Branch("rowpix", pixinfo_.row, "row[npix]/F", bufsize);
  PixHitTree_->Branch("colpix", pixinfo_.col, "col[npix]/F", bufsize);
  PixHitTree_->Branch("adc", pixinfo_.adc, "adc[npix]/F", bufsize);
  PixHitTree_->Branch("xpix", pixinfo_.x, "x[npix]/F", bufsize);
  PixHitTree_->Branch("ypix", pixinfo_.y, "y[npix]/F", bufsize);
  PixHitTree_->Branch("gxpix", pixinfo_.gx, "gx[npix]/F", bufsize);
  PixHitTree_->Branch("gypix", pixinfo_.gy, "gy[npix]/F", bufsize);
  PixHitTree_->Branch("gzpix", pixinfo_.gz, "gz[npix]/F", bufsize);

  if(useAllPixel == true){
    //    std::cout << "Making allpixinfo branch:" << std::endl;
    PixHitTree_->Branch("allpix_npix", &allpixinfo_.allpix_npix, "allpix_npix/I", bufsize);
    PixHitTree_->Branch("allpix_hasOverFlow",&allpixinfo_.allpix_hasOverFlow, "allpix_hasOverFlow/I",bufsize);
    PixHitTree_->Branch("allpixInfo",&allpixinfo_,"allpix_row[200]/F:allpix_col[200]/F:allpix_adc[200]/F",bufsize);
  }
  // std::cout << "Making allclustinfo branch:" << std::endl;
  PixHitTree_->Branch("allclust_hasOverFlow", &allclustinfo_.allclust_hasOverFlow, "allclust_hasOverFlow/I"                 , bufsize);
  PixHitTree_->Branch("n_allclust"          , &allclustinfo_.n_allclust         , "n_allclust/I"                           , bufsize);
  PixHitTree_->Branch("allclust_row"        , allclustinfo_.allclust_row        , "allclust_row[n_allclust]/F"        , bufsize);
  PixHitTree_->Branch("allclust_col"        , allclustinfo_.allclust_col        , "allclust_col[n_allclust]/F"        , bufsize);
  PixHitTree_->Branch("allclust_x"          , allclustinfo_.allclust_x          , "allclust_x[n_allclust]/F"          , bufsize);
  PixHitTree_->Branch("allclust_y"          , allclustinfo_.allclust_y          , "allclust_y[n_allclust]/F"          , bufsize);
  PixHitTree_->Branch("allclust_charge"     , allclustinfo_.allclust_charge     , "allclust_charge[n_allclust]/F"     , bufsize);
  PixHitTree_->Branch("allclust_nor_charge" , allclustinfo_.allclust_nor_charge , "allclust_nor_charge[n_allclust]/F" , bufsize);
  PixHitTree_->Branch("allclust_size"       , allclustinfo_.allclust_size       , "allclust_size[n_allclust]/F"       , bufsize);
  PixHitTree_->Branch("allclust_size_x"     , allclustinfo_.allclust_size_x     , "allclust_size_x[n_allclust]/F"     , bufsize);
  PixHitTree_->Branch("allclust_size_y"     , allclustinfo_.allclust_size_y     , "allclust_size_y[n_allclust]/F"     , bufsize);
  PixHitTree_->Branch("allclust_maxPixelCol", allclustinfo_.allclust_maxPixelCol, "allclust_maxPixelCol[n_allclust]/F", bufsize);
  PixHitTree_->Branch("allclust_maxPixelRow", allclustinfo_.allclust_maxPixelRow, "allclust_maxPixelRow[n_allclust]/F", bufsize);
  PixHitTree_->Branch("allclust_minPixelCol", allclustinfo_.allclust_minPixelCol, "allclust_minPixelCol[n_allclust]/F", bufsize);
  PixHitTree_->Branch("allclust_minPixelRow", allclustinfo_.allclust_minPixelRow, "allclust_minPixelRow[n_allclust]/F", bufsize);
  PixHitTree_->Branch("allclust_geoId"      , allclustinfo_.allclust_geoId      , "allclust_geoId[n_allclust]/F"      , bufsize);
  PixHitTree_->Branch("allclust_edgeHitX"   , allclustinfo_.allclust_edgeHitX   , "allclust_edgeHitX[n_allclust]/F"   , bufsize);
  PixHitTree_->Branch("allclust_edgeHitY"   , allclustinfo_.allclust_edgeHitY   , "allclust_edgeHitY[n_allclust]/F"   , bufsize);
  PixHitTree_->Branch("allclust_dist"       , allclustinfo_.allclust_dist       , "allclust_dist[n_allclust]/F"       , bufsize);

  //  std::cout << "Making muon branch:" << std::endl;
  if(isCosmic == true){
    PixHitTree_->Branch("MuonInfo",&muoninfo_,"timeAtIpInOut[2]/F:corrTimeAtIpInOut[2]/F:errorTime[2]/F:IsGlobalMuon[2]/F:IsStandAloneMuon[2]/F:IsTrackerMuon[2]/F:IsTimeValid[2]/F:HasGlobalTrack[2]/F:HasPixelHit[2]/F:trackpt[2]/F:tracketa[2]/F:trackphi[2]/F",bufsize);
    PixHitTree_->Branch("nMuon",&muoninfo_.nMuon,"nMuon/I",bufsize);
    PixHitTree_->Branch("nMuonHasOverFlow",&muoninfo_.HasOverFlow,"nMuonHasOverFlow/I",bufsize);
    PixHitTree_->Branch("Muon_meanTime",&muoninfo_.mean_Time,"Muon_meanTime/F",bufsize);
  }
  //  std::cout << "Making rechit branch:" << std::endl;
  PixHitTree_->Branch("RecHit", &rechit_, "localX/F:localY:globalX:globalY:globalZ:residualX:residualY:resErrX:resErrY:hit_errX:hit_errY:resXprime:resXprimeErr:clusterProb:probX:probY:qualWord/i:qBin/I:onEdge:badPixels:spansTwoROCs", bufsize);
  //  std::cout << "Making track branch:" << std::endl;
  PixHitTree_->Branch("track", &track_, "pt/F:p:px:py:pz:globalTheta:globalEta:globalPhi:localTheta:localPhi:chi2:ndof:foundHits/I:tracknum", bufsize);
  //  std::cout << "Making track only branch:" << std::endl;
  TrackTree_->Branch("TrackInfo", &trackonly_, "run/I:evtnum:tracknum:pixelTrack:NumPixelHits:NumStripHits:charge:chi2/F:ndof:theta:d0:dz:p:pt:px:py:pz:phi:eta:vx:vy:vz:muonT0:muondT0", bufsize);
  edm::LogInfo("PixelNuplizer_RealData") << "Made all branches." << std::endl;

}


bool PixelNtuplizer_RealData::isValidMuonAssoc(const edm::Event& iEvent){
 
  // get the ccmuons
    edm::Handle<MuonCollection> MuonHandle;
    iEvent.getByLabel("muons", MuonHandle);
    if ( !MuonHandle.isValid() ) {
      std::cout << "No Muon results for InputTag " << std::endl;
      return false;
    }

    // currently unused.
    //const MuonCollection & muoninfo  =  *MuonHandle.product();
   
    int count = 0;
    muoninfo_.init();
   
    int maxSize = MuonHandle->size();
    
    if(MuonHandle->size() > 2){
      muoninfo_.HasOverFlow = 1;
      maxSize = 2;
      muoninfo_.nMuon = 2;
    }
    else muoninfo_.nMuon = MuonHandle->size();

      int good_muons = 0;

      vector<float> timeAtIpInOut;
      vector<float> corrTimeAtIpInOut;
      vector<float> errorTime;

      timeAtIpInOut.clear();
      corrTimeAtIpInOut.clear();
      errorTime.clear();
   
      for(MuonCollection::const_iterator it = MuonHandle->begin(), itEnd = MuonHandle->end(); it!=itEnd;++it){
     
      if(count >= 2) return false;

      if(!it->globalTrack())muoninfo_.HasGlobalTrack[count] = 0;
      else muoninfo_.HasGlobalTrack[count] = 1;
   
      muoninfo_.IsGlobalMuon[count] = (float)it->isGlobalMuon();
      muoninfo_.IsStandAloneMuon[count] = (float)it->isStandAloneMuon();
      muoninfo_.IsTrackerMuon[count] = (float)it->isTrackerMuon();
      muoninfo_.IsTimeValid[count] = (float) it->isTimeValid();

      if(it->isTimeValid() == true){
	muoninfo_.timeAtIpInOut[count] = it->time().timeAtIpInOut;
	muoninfo_.corrTimeAtIpInOut[count] = correctedTime(*it);
	muoninfo_.errorTime[count] = it->time().timeAtIpInOutErr;
      }
      else {
	muoninfo_.timeAtIpInOut[count] = -9999;
	muoninfo_.corrTimeAtIpInOut[count] = -9999;
	muoninfo_.errorTime[count] = -9999;
      }

      if( it->isTimeValid() == true && it->time().timeAtIpInOutErr < 15){//only take muons with error < max_timeError
	//LogInfo("MuonTOFFilter_trackQuality") << " muon time " <<  it->time().timeAtIpInOut << "corrected Time " << correctedTime(*it) << std::endl;
	//if(correctedTime(*it) != -10000)cout << " muon time " <<  it->time().timeAtIpInOut << "corrected Time " << correctedTime(*it) << std::endl;
	  timeAtIpInOut.push_back( it->time().timeAtIpInOut);
	  corrTimeAtIpInOut.push_back(correctedTime(*it));
	  errorTime.push_back(it->time().timeAtIpInOutErr);
	  good_muons++;
      }
   
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

      muoninfo_.mean_Time = 0;
      float sumweight = 0;
    
      if(good_muons <= 2 && good_muons > 0){//only calc. time for events with right number of good muons

      for(unsigned int i = 0; i < timeAtIpInOut.size(); i++){
	muoninfo_.mean_Time =  muoninfo_.mean_Time + 1/(pow(errorTime[i],2))*timeAtIpInOut[i];
	sumweight = sumweight + 1/(pow(errorTime[i],2));
      }

      if(sumweight != 0) muoninfo_.mean_Time = muoninfo_.mean_Time/sumweight;
    }
    
      // cout << " time "<< 	muoninfo_.mean_Time << endl;
    return true;

}



// Functions that get called by framework every event
void PixelNtuplizer_RealData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 LogDebug("MuonTOFFilter_trackQuality") << " analyze " << std::endl;
  int TrackNumber = 0;
  dummyhist->Fill(0.5);
  maxsize_AllPixInfoStruct_ = 200;
  maxsize_AllClustInfoStruct_ = 100;
 
  for(int i = 0; i < (int)maxsize_AllPixInfoStruct_; i++) {
    allpixinfo_.allpix_row[i] = -999;
    allpixinfo_.allpix_col[i] = -999;
    allpixinfo_.allpix_adc[i] = -999;
  }

  init();

  trackonly_.init();
 
  // -- Does this belong into beginJob()?
  ESHandle<TrackerGeometry> TG;
  iSetup.get<TrackerDigiGeometryRecord>().get(TG);
  const TrackerGeometry* theTrackerGeometry = TG.product();
 

  // -- Track trajectory association map
  edm::Handle<TrajTrackAssociationCollection> hTTAC;
  iEvent.getByLabel(conf_.getParameter<std::string>("trajectoryInput"), hTTAC);
  
  std::set<SiPixelCluster> clusterSet;
  TrajectoryStateCombiner tsoscomb;

  if (hTTAC.isValid()) {
    int NbrTracks =  hTTAC->size();
    //cout << " nbr tracks " << NbrTracks << endl;
    const TrajTrackAssociationCollection ttac = *(hTTAC.product());
   
    // cout << "   hTTAC.isValid()" << endl;
    for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {

      TrackNumber++;
      int pixelHits = 0;
      int stripHits = 0;

      // cout << "      TracjTrackAssociationCollection iterating" << endl;
      const edm::Ref<std::vector<Trajectory> > refTraj = it->key;  
      reco::TrackRef trackref = it->val;

      std::vector<TrajectoryMeasurement> checkColl = refTraj->measurements();

      //count number of pixel and strip hits
      for(std::vector<TrajectoryMeasurement>::const_iterator checkTraj = checkColl.begin(), checkTrajEnd = checkColl.end();checkTraj != checkTrajEnd; ++checkTraj) {
	if(! checkTraj->updatedState().isValid()) continue;
	TransientTrackingRecHit::ConstRecHitPointer testhit = checkTraj->recHit();
	if(! testhit->isValid() || testhit->geographicalId().det() != DetId::Tracker ) continue;
	uint testSubDetID = (testhit->geographicalId().subdetId());
	if(testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) pixelHits++;
	else if (testSubDetID == StripSubdetector::TIB || testSubDetID == StripSubdetector::TOB ||
		 testSubDetID == StripSubdetector::TID || testSubDetID == StripSubdetector::TEC) stripHits++;
      }//end of for trajectory loop 

      fillTrackOnly(iEvent,iSetup, pixelHits, stripHits, TrackNumber, trackref);
    //++++++++++
      TrackTree_->Fill();
    //++++++++++


       // -- Clusters associated with a track
      std::vector<TrajectoryMeasurement> tmeasColl =refTraj->measurements();
      for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = tmeasColl.begin(); tmeasIt!=tmeasColl.end(); tmeasIt++){   
	if (!tmeasIt->updatedState().isValid()) continue; 
	TrajectoryStateOnSurface tsos = tsoscomb(tmeasIt->forwardPredictedState(), tmeasIt->backwardPredictedState());
	TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt->recHit();

	if (hit->geographicalId().det() != DetId::Tracker) {
	  continue; 
	} 

	const GeomDetUnit* detUnit = hit->detUnit();
	if (0 == detUnit) {
	  //cout << "-- PixelTree> Did not find hit->detUnit()" << endl;
	  continue;
	}
	double dR = -999, dZ = -999;
	//double dPhi = -999;


	bool barrel = DetId::DetId(hit->geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
	bool endcap = DetId::DetId(hit->geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
	if (!barrel && !endcap) {  
	  continue;
	}

	
	
	const Surface &surface = hit->detUnit()->surface();
	LocalPoint lPModule(0.,0.,0.), lPhiDirection(1.,0.,0.), lROrZDirection(0.,1.,0.);
	GlobalPoint gPModule       = surface.toGlobal(lPModule),
	  gPhiDirection  = surface.toGlobal(lPhiDirection),
	  gROrZDirection = surface.toGlobal(lROrZDirection);


	float phiorientation = deltaPhi(gPhiDirection.phi(), gPModule.phi()) >= 0 ? +1. : -1.;
	
	LocalTrajectoryParameters ltp = tsos.localParameters();
	LocalVector localDir = ltp.momentum()/ltp.momentum().mag();

	int DBlayer, DBladder, DBmodule, DBdisk, DBblade, DBpanel, DBdetid; 
	DBdetid = hit->geographicalId().rawId();
	if (barrel) {
	  bpixNames(DetId::DetId((*hit).geographicalId()), DBlayer, DBladder, DBmodule); 
	} else {
	  fpixNames(DetId::DetId((*hit).geographicalId()), DBdisk, DBblade, DBpanel, DBmodule); 
	}

	const DetId &hit_detId = hit->geographicalId();
	uint IntSubDetID = (hit_detId.subdetId());
	if (IntSubDetID == 0) continue;
	const TrackerGeometry &theTracker(*theTrackerGeometry);
	const PixelGeomDetUnit *theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(hit_detId) );
	if (theGeomDet == 0) {
	  continue; 	  // skip strip modules
	}

	// Flipped module?
	float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
	float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
	int isFlipped(0); 
	if (tmp2 < tmp1) isFlipped = 1;
	else isFlipped = 0;

	const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));

	// -- Missing hits
	if (hit->getType() == TrackingRecHit::missing)continue;



	const TrackingRecHit *persistentHit = hit->hit();
	if (persistentHit != 0) {
	  if (typeid(*persistentHit) != typeid(SiPixelRecHit))  continue;
	}
	const SiPixelRecHit *pixhit = dynamic_cast<const SiPixelRecHit*>(hit->hit());
	edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = (*pixhit).cluster();

	align::LocalVector res = tsos.localPosition() - hit->localPosition();
	LocalError err1 = tsos.localError().positionError();
	LocalError err2 = hit->localPositionError();
	float errX = std::sqrt( err1.xx() + err2.xx() );
	float errY = std::sqrt( err1.yy() + err2.yy() );
       

	if (clust.isNonnull()) {
	  muoninfo_.init();
	  init();
	  rechit_.localX = hit->localPosition().x();
	  rechit_.localY = hit->localPosition().y();
	  rechit_.globalX = hit->globalPosition().x();
	  rechit_.globalY = hit->globalPosition().y();
	  rechit_.globalZ = hit->globalPosition().z();
          rechit_.qualWord = pixhit->rawQualityWord();
          rechit_.qBin = pixhit->qBin();
	  if (pixhit->hasFilledProb() == true) {
            rechit_.clusterProb = pixhit->clusterProbability(0);
            rechit_.probX = pixhit->probabilityX();
            rechit_.probY = pixhit->probabilityY();
	  }
	  if (pixhit->isOnEdge() == true) {
	    rechit_.onEdge = 1;
	  } else rechit_.onEdge = 0;
	  if (pixhit->hasBadPixels() == true) {
	    rechit_.badPixels = 1;
	  } else rechit_.badPixels = 0;
	  if (pixhit->spansTwoROCs() == true) {
	    rechit_.spansTwoROCs = 1;
	  } else rechit_.spansTwoROCs = 0;
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
	  

	  if(isCosmic == true) isValidMuonAssoc(iEvent);

	  //  isOffTrackHits(iEvent,*clust, iSetup,topol,theGeomDet->geographicalId().rawId(),tsos);
	  

	  // get the contents
	  fillEvt(iEvent,NbrTracks);
	  fillDet(hit_detId, IntSubDetID, theGeomDet);
	  fillVertex(theGeomDet);
	  fillClust(*clust, topol, theGeomDet, tsos);
	  fillPix(*clust, topol, theGeomDet);
	  fillTrack(tsos, *refTraj, TrackNumber);

	  // fill simulated hits if applicable
	  if(isSim == true) {
	    edm::Handle<edm::SimTrackContainer> simtracks;
	    iEvent.getByLabel("g4SimHits", simtracks);
	    TrackerHitAssociator* associate = new TrackerHitAssociator(iEvent);
	    std::vector<PSimHit> matched;
	    matched.clear();
	    matched = associate->associateHit(*persistentHit);
	    LocalPoint lp = topol->localPosition(MeasurementPoint(clust->x(),clust->y()));
	    float rechitx = lp.x();
	    float rechity = lp.y();
	    
                  if ( !matched.empty() ) {
	            bool found_hit_from_generated_particle = false;

	            //---Loop over sim hits found on DetUnit, use closest to RecHit
	            float closest_dist = 99999.9;
	            std::vector<PSimHit>::const_iterator closest_simhit = matched.begin();
	            for (std::vector<PSimHit>::const_iterator m = matched.begin(); m < matched.end(); m++) {
 
		      float simhitx = 0.5 * ( (*m).entryPoint().x() + (*m).exitPoint().x() );
		      float simhity = 0.5 * ( (*m).entryPoint().y() + (*m).exitPoint().y() );
		      float x_res = simhitx - rechitx;
		      float y_res = simhity - rechity;
		      float dist = sqrt( x_res*x_res + y_res*y_res);
	
		      if ( dist < closest_dist ) {
		        closest_dist = dist;
		        closest_simhit = m;
		        found_hit_from_generated_particle = true;
		      } 
		    } // end sim hit loop over candidates

                    fillSim( closest_simhit, IntSubDetID, theGeomDet, topol, *(simtracks.product()) );

                  } //end if ( !matched.empty() )
                } // end if(isSim == true)

		

	  //++++++++++
	  PixHitTree_->Fill();
	  //++++++++++
	  

	}//check if cluster is nonnull
	

      }//IN TRAJECTORY LOOP


    }
  }
}  // end analyze function


bool PixelNtuplizer_RealData::isOffTrackHits(const edm::Event& iEvent,const SiPixelCluster& clust, const edm::EventSetup & iSetup,const RectangularPixelTopology* topol, uint32_t geoId, TrajectoryStateOnSurface& tsos ){
  
  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord> ().get (pDD);
  //const TrackerGeometry* tracker = &(* pDD); currently unused

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
		      
   if ( detId.rawId() != geoId ) continue;
   else if ( detId.rawId() == geoId ) {
     // cout << "Found the det unit for this RecHit = " << (int)detId.rawId() << endl;
     found_det_unit = true;

     edm::Handle<SiPixelRecHitCollection> recHitColl;
     iEvent.getByLabel("siPixelRecHits", recHitColl );
			  
     SiPixelRecHitCollection::const_iterator dsmatch = recHitColl->find(detId);
     if (dsmatch == recHitColl->end()) continue;
     SiPixelRecHitCollection::DetSet pixelrechitRange = *dsmatch;
     SiPixelRecHitCollection::DetSet::const_iterator pixelrechitRangeIteratorBegin = pixelrechitRange.begin();
     SiPixelRecHitCollection::DetSet::const_iterator pixelrechitRangeIteratorEnd = pixelrechitRange.end();
     SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixelrechitRangeIteratorBegin;

     int n_clust = 0;
     int pixel_index = 0;
     for ( ; pixeliter != pixelrechitRangeIteratorEnd; ++pixeliter) {
       ++n_clust;
				      
       if ( (int)n_clust <= (int)maxsize_AllClustInfoStruct_ ) {
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
         1.0/pow( tan(clust_beta ), 2 ) + 1.0 ) );
					  
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
         allclustinfo_.allclust_dist[n_clust-1] = sqrt( (clust_x - allclustinfo_.allclust_x[n_clust-1])*
						      (clust_x - allclustinfo_.allclust_x[n_clust-1]) +
						      (clust_y - allclustinfo_.allclust_y[n_clust-1])*
						      (clust_y - allclustinfo_.allclust_y[n_clust-1]) );

         // std::cout << " distance " << allclustinfo_.allclust_dist[n_clust-1] << "clustx " << clust_x << " allclust dist " << allclustinfo_.allclust_x[n_clust-1] << std::endl;

         const std::vector<SiPixelCluster::Pixel>& pixvector = pixeliter->cluster()->pixels();
					    
         if(useAllPixel == true){
         for (int i=0; i<(int)pixvector.size(); ++i ) {
           if ( (int)pixel_index < (int)maxsize_AllPixInfoStruct_ ) {
	      ++pixel_index;
	      SiPixelCluster::Pixel holdpix = pixvector[i];
	      allpixinfo_.allpix_row[pixel_index-1] = holdpix.x;
	      allpixinfo_.allpix_col[pixel_index-1] = holdpix.y;
	      allpixinfo_.allpix_adc[pixel_index-1] = holdpix.adc;
           } 
           else allpixinfo_.allpix_hasOverFlow = 1;
        }
      }
    } // if ( n_clust <= maxsize_AllClustInfoStruct_ )
    else allclustinfo_.allclust_hasOverFlow = 1;
  } 

  //cout << "n_clust = " << n_clust << endl;
  clust_.n_neighbour_clust = n_clust;
  allclustinfo_.n_allclust = n_clust;
  if(useAllPixel == true)allpixinfo_.allpix_npix = pixel_index;
  break;
  } 
  else cout << "Not good !!!" << endl;
} 
		  			  
// if ( !found_det_unit )cout << "Did not find the det unit of this RecHit !!!!!!!!!!!!!!!!!!!!!!!!!! " << endl;
			  			  
return true;

}


void PixelNtuplizer_RealData::fillTrackOnly(const edm::Event& iEvent, const edm::EventSetup& iSetup, int pixelHits, int stripHits, int TrackNumber, const TrackRef& track)
{
  if(pixelHits > 0) trackonly_.pixelTrack = 1;
  trackonly_.tracknum = TrackNumber;
  trackonly_.run = iEvent.id().run();
  trackonly_.evtnum = iEvent.id().event();
  trackonly_.NumPixelHits = pixelHits;
  trackonly_.NumStripHits = stripHits;
  trackonly_.chi2 = track->chi2();
  trackonly_.ndof = track->ndof();
  trackonly_.charge = track->charge();
  trackonly_.theta = track->theta();
  trackonly_.d0 = track->d0();
  trackonly_.dz = track->dz();
  trackonly_.p = track->p();
  trackonly_.pt = track->pt();
  trackonly_.px = track->px();
  trackonly_.py = track->py();
  trackonly_.pz = track->pz();
  trackonly_.phi = track->phi();
  trackonly_.eta = track->eta();
  trackonly_.vx = track->vx();
  trackonly_.vy = track->vy();
  trackonly_.vz = track->vz();

  if(isCosmic == true){
    if(muoninfo_.errorTime[0]<0)
      return;
    trackonly_.muonT0=muoninfo_.timeAtIpInOut[0];
    trackonly_.muondT0=muoninfo_.errorTime[0];
    //  std::cout <<  trackonly_.muonT0 << " " << trackonly_.muondT0 << std::endl;
    if(muoninfo_.errorTime[1]<0)
      return;
    if(muoninfo_.errorTime[1]<muoninfo_.errorTime[0]){ // time [1] more accurate than [0]
      trackonly_.muonT0=muoninfo_.timeAtIpInOut[1];
      trackonly_.muondT0=muoninfo_.errorTime[1];
    }
  }

  //  std::cout<< "* "  <<  trackonly_.muonT0 << " " << trackonly_.muondT0 << std::endl;
  return;
}


void PixelNtuplizer_RealData::fillEvt(const edm::Event& iEvent,int NbrTracks)
{
  evt_.run = iEvent.id().run();
  evt_.evtnum = iEvent.id().event();
  evt_.lumiBlock = iEvent.luminosityBlock();
  evt_.trackNumber = NbrTracks;
  evt_.bunchCrossing = iEvent.bunchCrossing();
  evt_.orbit = iEvent.orbitNumber();
}


void PixelNtuplizer_RealData::fillDet(const DetId &tofill, uint subdetid, const PixelGeomDetUnit* PixGeom)
{
  if (subdetid==1) {
      det_.layer  = PXBDetId::PXBDetId(tofill).layer();
      det_.ladder = PXBDetId::PXBDetId(tofill).ladder();
    // convert to online ladder numbering convention 
    if (det_.layer==1) {
           if (det_.ladder<=5&&det_.ladder>=1)   det_.ladder = 6-det_.ladder;
      else if (det_.ladder<=15&&det_.ladder>=6)  det_.ladder = 5-det_.ladder;
      else if (det_.ladder<=20&&det_.ladder>=16) det_.ladder = 26-det_.ladder;
    } else if (det_.layer==2) {
           if (det_.ladder<=8&&det_.ladder>=1)   det_.ladder = 9-det_.ladder;
      else if (det_.ladder<=24&&det_.ladder>=9)  det_.ladder = 8-det_.ladder;
      else if (det_.ladder<=32&&det_.ladder>=25) det_.ladder = 41-det_.ladder;
    }else if (det_.layer==3) {
           if (det_.ladder<=11&&det_.ladder>=1)  det_.ladder = 12-det_.ladder;
      else if (det_.ladder<=33&&det_.ladder>=12) det_.ladder = 11-det_.ladder;
      else if (det_.ladder<=44&&det_.ladder>=34) det_.ladder = 56-det_.ladder;
    }
      det_.module = PXBDetId::PXBDetId(tofill).module();
    // convert to online module numbering convention
    if (det_.module<=4) det_.module = det_.module-5;
    else det_.module = det_.module-4;
  } 
  else {
      det_.disk      =  PXFDetId::PXFDetId(tofill).disk();
      det_.blade     =  PXFDetId::PXFDetId(tofill).blade();
      // convert to online blade numbering convention
           if(det_.blade<=6&&det_.blade>=1)   det_.blade = 7-det_.blade;
      else if(det_.blade<=18&&det_.blade>=7)  det_.blade = 6-det_.blade;
      else if(det_.blade<=24&&det_.blade>=19) det_.blade = 31-det_.blade;
      det_.panel     =  PXFDetId::PXFDetId(tofill).panel();
      det_.plaquette =  PXFDetId::PXFDetId(tofill).module();
  }


  //cout << " layer " << det_.layer << " disk "<< det_.disk << endl;

  det_.thickness = PixGeom->specificSurface().bounds().thickness();
  det_.cols = PixGeom->specificTopology().ncolumns();
  det_.rows = PixGeom->specificTopology().nrows();

  // Does the module have a flipped local Y axis?
  float tmp1 = PixGeom->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
  float tmp2 = PixGeom->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
  if ( tmp2<tmp1 ) det_.isflipped = 1;
  else det_.isflipped = 0;

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


void PixelNtuplizer_RealData::fillSim(std::vector<PSimHit>::const_iterator isim, unsigned int subid, const PixelGeomDetUnit * theGeomDet, 
                                      const RectangularPixelTopology * topol, const edm::SimTrackContainer& trks) 
{
  sim_.entryPointLocalX = (*isim).entryPoint().x(); // width (row index, in col direction)
  sim_.entryPointLocalY = (*isim).entryPoint().y(); // length (col index, in row direction) 
  sim_.entryPointLocalZ = (*isim).entryPoint().z(); 
  sim_.exitPointLocalX = (*isim).exitPoint().x();
  sim_.exitPointLocalY = (*isim).exitPoint().y();
  sim_.exitPointLocalZ = (*isim).exitPoint().z();
  GlobalPoint GP1 = theGeomDet->surface().toGlobal(Local3DPoint((*isim).entryPoint().x(),(*isim).entryPoint().y(),(*isim).entryPoint().z()));
  GlobalPoint GP2 = theGeomDet->surface().toGlobal(Local3DPoint((*isim).exitPoint().x(),(*isim).exitPoint().y(),(*isim).exitPoint().z()));
  sim_.entryPointGlobalX = GP1.x();
  sim_.exitPointGlobalX = GP2.x();
  sim_.entryPointGlobalY = GP1.y();
  sim_.exitPointGlobalY = GP2.y();
  sim_.entryPointGlobalZ = GP1.z();
  sim_.exitPointGlobalZ = GP2.z();
  
  MeasurementPoint mp1 = topol->measurementPosition(LocalPoint((*isim).entryPoint().x(),(*isim).entryPoint().y(),(*isim).entryPoint().z()));
  MeasurementPoint mp2 = topol->measurementPosition(LocalPoint((*isim).exitPoint().x(),(*isim).exitPoint().y(), (*isim).exitPoint().z()));
  sim_.entryPointRow = mp1.x();
  sim_.entryPointColumn = mp1.y();
  sim_.exitPointRow = mp2.x();
  sim_.exitPointColumn = mp2.y();
  
  float sim_xpos = 0.5*(sim_.entryPointLocalX+sim_.exitPointLocalX);
  float sim_ypos = 0.5*(sim_.entryPointLocalY+sim_.exitPointLocalY);         
  sim_.localX = sim_xpos;
  sim_.localY = sim_ypos;
  sim_.eloss = (*isim).energyLoss();
  sim_.localPhi   = (*isim).phiAtEntry();
  sim_.localTheta = (*isim).thetaAtEntry();
  sim_.particleID = (*isim).particleType();
  sim_.trackID = (*isim).trackId();
  
  //--- Fill alpha and beta -- more useful for exploring the residuals...
  float localPx = (*isim).momentumAtEntry().x();
  float localPy = (*isim).momentumAtEntry().y();
  float localPz = (*isim).momentumAtEntry().z();
  sim_.beta  = atan2(localPz, localPy);
  sim_.alpha = atan2(localPz, localPx);
  sim_.processID = (int)isim->processType();

  SimTrackContainer::const_iterator trksiter;
  for (trksiter = trks.begin(); trksiter != trks.end(); trksiter++) 
    {
/*      if ( trksiter->trackId() == (sim_.TID - 1) ) 
	{
	  sim_.trk_eta = trksiter->momentum().eta();
	  sim_.trk_phi = trksiter->momentum().phi();
	} */
      if ( trksiter->trackId() == sim_.trackID ) 
	{
          sim_.p = trksiter->momentum().mag();
          sim_.px = trksiter->momentum().x();
          sim_.py = trksiter->momentum().y();
          sim_.pz = trksiter->momentum().z();
	  sim_.globalEta = trksiter->momentum().eta();
          sim_.globalPhi = trksiter->momentum().phi();
          sim_.globalTheta = trksiter->momentum().theta();
          sim_.charge = trksiter->charge();
          sim_.vertexID = trksiter->vertIndex();
	}
    }
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
  sim_.init();
  muoninfo_.init();
  allclustinfo_.init();
  allpixinfo_.init();
    
}

void PixelNtuplizer_RealData::EvtStruct::init()
{
  int dummy_int = -9999;
  uint32_t dummy_uint = 0;

  run = dummy_uint;
  evtnum = dummy_uint;
  lumiBlock = dummy_uint;
  trackNumber = dummy_uint;
  bunchCrossing = dummy_int;
  orbit = dummy_int;
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
  isflipped = dummy_int;
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
  uint32_t dummy_uint = 9999;
  
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
  int dummy_int = -9999;
  uint32_t dummy_uint = 9999;

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
  clusterProb = dummy_float;
  probX = dummy_float;
  probY = dummy_float;
  qualWord = dummy_uint;
  qBin = dummy_int;
  onEdge = dummy_int;
  badPixels = dummy_int;
  spansTwoROCs = dummy_int;
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

void PixelNtuplizer_RealData::SimStruct::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;
  uint32_t dummy_uint = 9999;
  
  localX = dummy_float;
  localY = dummy_float;
  eloss = dummy_float;
  localPhi = dummy_float;
  localTheta = dummy_float;
  alpha = dummy_float;
  beta = dummy_float;
  charge = dummy_float;
  entryPointLocalX = entryPointLocalY = entryPointLocalZ = dummy_float;
  exitPointLocalX = exitPointLocalY = exitPointLocalZ = dummy_float;
  entryPointRow = entryPointColumn = exitPointRow = exitPointColumn = dummy_float;
  entryPointGlobalX = entryPointGlobalY = entryPointGlobalZ = dummy_float;
  exitPointGlobalX = exitPointGlobalY = exitPointGlobalZ = dummy_float;
  p = px = py = pz = dummy_float;
  globalEta = dummy_float;
  globalPhi = dummy_float;
  globalTheta = dummy_float;
  
  particleID = dummy_int;
  vertexID = dummy_int;
  processID = dummy_int;
  
  trackID = dummy_uint;

}

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
  muondT0 = dummy_float;
}


void PixelNtuplizer_RealData::readOffsets()
{

  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 15; j++){
      spoints[i][j] =  -1000;
      sbias[i][j]   =  -1000;
      srms[i][j]    =  -1000;
    }
  }

  ifstream f1("muons_input-singleOffset.txt");

  while(!f1.eof())
  {
   int w0,s0;
   float b,r,p;

   f1 >>  w0 >> s0 >> p >>  b >> r;
   if (!f1.good()) break;

   spoints[w0][s0] = p;
   sbias[w0][s0] = b;
   srms[w0][s0] = r;
   std::cout << " muon offsets " << w0 << " " << s0 << " " << b << " " << p << " " << r <<  std::endl;
  }
}


void PixelNtuplizer_RealData::sectorAndWheel(const  reco::Muon & muon0 ,int & w0,int & s0 )
{
 w0=0;
 s0=0;
 for(trackingRecHit_iterator match = muon0.bestTrack()->recHitsBegin() ; match != muon0.bestTrack()->recHitsEnd() ; ++match)
 {
  DetId did=(*match)->geographicalId() ;
  if(did.det() == 2 && did.subdetId() == MuonSubdetId::DT)
  {
    DTChamberId * id =  new DTChamberId(did);
    w0=id->wheel();
    s0=id->sector();
 
    delete id;
    break;
  }
 }

}


float PixelNtuplizer_RealData::correctedTime(const  reco::Muon & aMuon)
{
// readOffsets() should be called in beginJob to initialized the offset maps
 int s0,w0;
 sectorAndWheel(aMuon,w0,s0);
 float time = -10000;
  
 
 if(spoints[w0+2][s0]>20) time = aMuon.time().timeAtIpInOut-sbias[w0+2][s0];
 
 
 return time;
}


// ----------------------------------------------------------------------
void PixelNtuplizer_RealData::bpixNames(const DetId &pID, int &DBlayer, int &DBladder, int &DBmodule) {
  PixelBarrelName::Shell DBshell = PixelBarrelName::PixelBarrelName(pID).shell();
  DBlayer  = PixelBarrelName::PixelBarrelName(pID).layerName();
  DBladder = PixelBarrelName::PixelBarrelName(pID).ladderName();
  DBmodule = PixelBarrelName::PixelBarrelName(pID).moduleName();
  
  if (DBshell == PixelBarrelName::mO) {
    DBladder *= -1;
    DBmodule *= -1;
  }
  
  if (DBshell == PixelBarrelName::mI) {
    DBmodule *= -1;
  }
  
  if (DBshell == PixelBarrelName::pO) {
    DBladder *= -1;
  }

}

// ----------------------------------------------------------------------
void PixelNtuplizer_RealData::fpixNames(const DetId &pID, int &DBdisk, int &DBblade, int &DBpanel, int &DBmodule) {

  PixelEndcapName::HalfCylinder DBside = PixelEndcapName::PixelEndcapName(pID).halfCylinder();
  DBdisk   = PixelEndcapName::PixelEndcapName(pID).diskName();
  DBblade  = PixelEndcapName::PixelEndcapName(pID).bladeName();
  DBpanel  = PixelEndcapName::PixelEndcapName(pID).pannelName();
  DBmodule = PixelEndcapName::PixelEndcapName(pID).plaquetteName();
  
  if (DBside == PixelEndcapName::mO) {
    DBdisk   *= -1; 
    DBblade  *= -1;
    DBpanel  *= -1;
    DBmodule *= -1;
  }
  
  if (DBside == PixelEndcapName::mI) {
    DBdisk   *= -1; 
  }
  
  if (DBside == PixelEndcapName::pO) {
    DBblade  *= -1;
    DBpanel  *= -1;
    DBmodule *= -1;
  }

}



// define this as a plug-in
//
DEFINE_FWK_MODULE(PixelNtuplizer_RealData);
