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
  t_->Branch("Cluster", &clust_, "row/F:col:x:y:charge:normalized_charge:size/I:size_x:size_y:maxPixelCol:maxPixelRow:minPixelCol:minPixelRow:geoId/i:edgeHitX/I:edgeHitY:clust_alpha/F:clust_beta", bufsize);
  
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

  t_->Branch("nmuon",&muoninfo_.nmuon,"nmuon/I",bufsize);
 t_->Branch("muoninfo_HasOverFlow",&muoninfo_.HasOverFlow,"muoninfo_HasOverFlow/bool",bufsize);
 t_->Branch("muoninfo_IsGlobalMuon",muoninfo_.IsGlobalMuon,"muoninfo_IsGlobalMuon[nmuon]/bool",bufsize);
 t_->Branch("muoninfo_IsStandAloneMuon",muoninfo_.IsStandAloneMuon,"muoninfo_IsStandAloneMuon[nmuon]/bool",bufsize);
 t_->Branch("muoninfo_IsTrackerMuon",muoninfo_.IsTrackerMuon,"muoninfo_IsTrackerMuon[nmuon]/bool",bufsize);
 t_->Branch("muoninfo_HasGlobalTrack",muoninfo_.HasGlobalTrack,"muoninfo_HasGlobalTrack[nmuon]/bool",bufsize);
  t_->Branch("muoninfo_HasPixelHit",muoninfo_.HasPixelHit,"muoninfo_HasPixelHit[nmuon]/bool",bufsize);
  t_->Branch("muoninfo_IsTimeValid",muoninfo_.IsTimeValid,"muoninfo_IsTimeValid[nmuon]/bool",bufsize);
  t_->Branch("muoninfo_timeAtIpInOut",muoninfo_.timeAtIpInOut, "muoninfo_timeAtIpInOut[nmuon]/F",bufsize);
  t_->Branch("muoninfo_errorTime",muoninfo_.errorTime, "muoninfo_errorTime[nmuon]/F",bufsize);
  // t_->Branch("muoninfo_momentumDiff",muoninfo_.momentumDiff, "muoninfo_momentumDiff[nmuon]/F",bufsize);
   t_->Branch("muoninfo_trackpt",muoninfo_.trackpt, "muoninfo_trackpt[nmuon]/F",bufsize);
    t_->Branch("muoninfo_tracketa",muoninfo_.tracketa, "muoninfo_tracketa[nmuon]/F",bufsize);
   t_->Branch("muoninfo_trackphi",muoninfo_.trackphi, "muoninfo_trackphi[nmuon]/F",bufsize);

  std::cout << "Making rechit branch:" << std::endl;
  t_->Branch("RecHit", &rechit_, "localX/F:localY:globalX:globalY:globalZ:residualX:residualY:resErrX:resErrY:hit_errX:hit_errY:resXprime:resXprimeErr", bufsize);

  std::cout << "Making track branch:" << std::endl;
  t_->Branch("track", &track_, "pt/F:p:px:py:pz:globalTheta:globalEta:globalPhi:localTheta:localPhi:chi2:ndof:foundHits/I:tracknum", bufsize);

  //  std::cout << "Making tracker hit branch:" << std::endl;
  //  ts_->Branch("TrackerHit", &trackerhits_, "globalX/F:globalY:globalZ:run/I:evtnum:tracknum", bufsize);

  std::cout << "Making track only branch:" << std::endl;
  tt_->Branch("TrackInfo", &trackonly_, "run/I:evtnum:tracknum:pixelTrack:NumPixelHits:NumStripHits:charge:chi2/F:ndof:theta:d0:dz:p:pt:px:py:pz:phi:eta:vx:vy:vz:muonT0", bufsize);
  
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


bool PixelNtuplizer_RealData::isValidMuonAssoc(const edm::Event& iEvent,const Track& track, int TrackNumber){

 
  // get the ccmuons
    edm::Handle<MuonCollection> MuonHandle;
    //  iEvent.getByLabel("GLBMuons", MuonHandle);
     iEvent.getByLabel("muons", MuonHandle);
    if ( !MuonHandle.isValid() ) {
      std::cout << "No Muon results for InputTag " << std::endl;
      return false;
    }

    const MuonCollection & muoninfo  =  *MuonHandle.product();
   
    int count = 0;
    muoninfo_.HasOverFlow = false;

    //std::cout << "muon collection size " << MuonHandle->size() << std::endl;
    int maxSize = MuonHandle->size();
    
      if(MuonHandle->size() > 2){
      muoninfo_.HasOverFlow = true;
      maxSize = 2;
      muoninfo_.nmuon = 2;
    }
    else muoninfo_.nmuon = MuonHandle->size();
    

      /*  muoninfo_.nmuon = 0;

    if(MuonHandle->size() != 2)return false;

    if(muoninfo[0].isTimeValid() != true || muoninfo[1].isTimeValid() != true)return false; 

    if( muoninfo[0].bestTrack()->hitPattern().numberOfValidMuonDTHits() < 25 ) return false;
    if( muoninfo[1].bestTrack()->hitPattern().numberOfValidMuonDTHits() < 25 ) return false;


    float t0 =  muoninfo[0].time().timeAtIpInOut;
    float t1 =  muoninfo[1].time().timeAtIpInOut;

    int w0=0,s0=0,w1=0,s1=0; //wheels and sectors
    DTChamberId * id;


    for(trackingRecHit_iterator match = muoninfo[0].bestTrack()->recHitsBegin() ; match != muoninfo[0].bestTrack()->recHitsEnd() ; ++match)
       {
	 DetId did=(*match)->geographicalId() ;
	 if(did.det() == 2 && did.subdetId() == MuonSubdetId::DT)
	   {
	     id =  new DTChamberId(did);
	     w0=id->wheel();
	     s0=id->sector();
	     delete id;
	     break;
	   }
       }


     
    for(trackingRecHit_iterator match = muoninfo[1].bestTrack()->recHitsBegin() ; match != muoninfo[1].bestTrack()->recHitsEnd() ; ++match)
      {
	DetId did=(*match)->geographicalId() ;
	if(did.det() == 2 && did.subdetId() == MuonSubdetId::DT)
	  {
	    id =  new DTChamberId(did);
	    w1=id->wheel();
	    s1=id->sector();
	    delete id;
	    break;
	  }
      }

    if(s0 ==0 || s1 ==0)
      {//no segment of muon chamber hit. Strange!
	cout << "EEEEEEEEERRRRRRRRROOOOOORRRRRRRRRRRRR" << endl;
	return false;
      }
  
    muoninfo_.nmuon = 2;  


    // cout << " points " << points[w0+2][s0][w1+2][s1] << endl;
    if(points[w0+2][s0][w1+2][s1]>=50){//found combination more than 50 times
      //  cout << " time 0 " << t0 << " corr time 1 " << t1-bias[w0+2][s0][w1+2][s1] << endl;
      muoninfo_.timeAtIpInOut[0] = t0;
      muoninfo_.timeAtIpInOut[1] = t1-bias[w0+2][s0][w1+2][s1];

      muoninfo_.errorTime[0] = muoninfo[0].time().timeAtIpInOutErr;
      muoninfo_.errorTime[1] = muoninfo[1].time().timeAtIpInOutErr; 

      muoninfo_.momentumDiff[0]=(muoninfo[0].momentum() - muoninfo[1].momentum()).r();
      muoninfo_.momentumDiff[1]=(muoninfo[0].momentum() - muoninfo[1].momentum()).r();

    }
    else {
      muoninfo_.timeAtIpInOut[0] = -9999;
      muoninfo_.timeAtIpInOut[1] = -9999;

      muoninfo_.errorTime[0] = -9999;
      muoninfo_.errorTime[1] = -9999; 

      muoninfo_.momentumDiff[0]=-9999;
      muoninfo_.momentumDiff[1]=-9999;


    }
      */

    // std::cout << " muon size " << std::endl;
      for(MuonCollection::const_iterator it = MuonHandle->begin(), itEnd = MuonHandle->end(); it!=itEnd;++it){
     
      if(count > 1) return false;

      if(!it->globalTrack())muoninfo_.HasGlobalTrack[count] = false;
      else muoninfo_.HasGlobalTrack[count] = true;
   

      muoninfo_.IsGlobalMuon[count] = it->isGlobalMuon();
     
      muoninfo_.IsStandAloneMuon[count] = it->isStandAloneMuon();
     
      muoninfo_.IsTrackerMuon[count] = it->isTrackerMuon();
    
      muoninfo_.IsTimeValid[count] = it->isTimeValid();

      if(it->isTimeValid() == true){
	muoninfo_.timeAtIpInOut[count] = it->time().timeAtIpInOut;
	muoninfo_.errorTime[count] = it->time().timeAtIpInOutErr;
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
	muoninfo_.HasPixelHit[count] = isMuonPixelHit;

      
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

  trackonly_.init();

  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
  //edm::Handle<std::vector<Trajectory> > trajCollectionHandle;
  iEvent.getByLabel(conf_.getParameter<std::string>("trajectoryInput"),trajTrackCollectionHandle);

  TrajectoryStateCombiner tsoscomb;
  int NbrTracks =  trajTrackCollectionHandle->size();
  //std::cout << " track measurements " << trajTrackCollectionHandle->size()  << std::endl;


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
	uint IntRawDetID = (hit_detId.rawId());
	uint IntSubDetID = (hit_detId.subdetId());
	
	if(IntSubDetID == 0 )
	  continue;
	
	align::LocalVector res = tsos.localPosition() - hit->localPosition();

	LocalError err1 = tsos.localError().positionError();
	LocalError err2 = hit->localPositionError();
	
	float errX = std::sqrt( err1.xx() + err2.xx() );
	float errY = std::sqrt( err1.yy() + err2.yy() );
	
	//std::cout << "Residual x/y " << res.x() << '/' << res.y() 
	//  << ", Error x/y " << errX << '/' << errY;		

	// begin partly copied from Tifanalyser 

	const GeomDetUnit* detUnit = hit->detUnit();
	double dPhi = -999, dR = -999, dZ = -999, phiorientation = -999;
	double R = 0.;
	double origintointersect = 0.;	

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

		isValidMuonAssoc(iEvent,track, TrackNumber);
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
  // associate muon with track and then fill muon T0. For now: set to zero.
  trackonly_.muonT0=0;
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
  float loctheta = localDir.theta();

  clust_.clust_alpha = atan2( locz, locx );
  clust_.clust_beta = atan2( locz, locy );

  clust_.normalized_charge = clust_.charge*sqrt(1.0/(1.0/pow(tan(clust_.clust_alpha),2)+1.0/pow(tan(clust_.clust_beta),2)+1.0));

}

void PixelNtuplizer_RealData::fillPix(const SiPixelCluster & LocPix, const RectangularPixelTopology * topol, const PixelGeomDetUnit * PixGeom)
{
  const std::vector<SiPixelCluster::Pixel>& pixvector = LocPix.pixels();
  if(pixvector.size()>maxsize_PixInfoStruct_)
    pixinfo_.hasOverFlow=1;
  for ( ; pixinfo_.npix < (int)pixvector.size() && pixinfo_.npix<maxsize_PixInfoStruct_; ++pixinfo_.npix)
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
}

void PixelNtuplizer_RealData::PixInfoStruct::init()
{
  npix = 0;
  hasOverFlow = 0;
  /* for(int i = 0; i != maxpix; ++i)
     {
     row[i] = dummy_float;
     col[i] = dummy_float;
     adc[i] = dummy_float;
     x[i] = dummy_float;
     y[i] = dummy_float;
     gx[i] = dummy_float;
     gy[i] = dummy_float;
     gz[i] = dummy_float;
     }
  */
} 



void PixelNtuplizer_RealData::MuonInfoStruct::init()
{
  nmuon = 0;
  HasOverFlow = false;
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
