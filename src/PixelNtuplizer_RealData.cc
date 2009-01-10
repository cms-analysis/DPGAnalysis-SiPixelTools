// File: PixelNtuplizer_RealData.cc
// Description: Adapted tree structure from PixelNtuplizer package
//   		Adapted method for finding residual information from TrackerValidationVariables package
// Authors: Andrew York, Tennessee
//          Freya Blekman, Cornell
//
//
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "DPGAnalysis/SiPixelTools/interface/PixelNtuplizer_RealData.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"

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

PixelNtuplizer_RD::PixelNtuplizer_RD(edm::ParameterSet const& ps) : 
  conf_(ps),
  tfile_(0), 
  t_(0),
  maxsize_PixInfoStruct_(200)
{}

// Virtual destructor needed.
PixelNtuplizer_RD::~PixelNtuplizer_RD() { }  

// End job: write and close the ntuple file
void PixelNtuplizer_RD::endJob() 
{

  std::string outputFileName = conf_.getParameter<std::string>("OutputFile");
  std::cout << " PixelNtuplizer_RealData::endJob" << std::endl;
  tfile_->Write();
  tfile_->Close();

}


void PixelNtuplizer_RD::beginJob(const edm::EventSetup& es)
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

  std::cout << "Making rechit branch:" << std::endl;
  t_->Branch("RecHit", &rechit_, "localX/F:localY:globalX:globalY:globalZ:residualX:residualY:resErrX:resErrY:resXprime:resXprimeErr", bufsize);

  std::cout << "Making track branch:" << std::endl;
  t_->Branch("track", &track_, "pt/F:p:px:py:pz:globalTheta:globalEta:globalPhi:localTheta:localPhi:chi2:ndof:foundHits/I:tracknum", bufsize);

  //  std::cout << "Making tracker hit branch:" << std::endl;
  //  ts_->Branch("TrackerHit", &trackerhits_, "globalX/F:globalY:globalZ:run/I:evtnum:tracknum", bufsize);

  std::cout << "Making track only branch:" << std::endl;
  tt_->Branch("TrackInfo", &trackonly_, "run/I:evtnum:tracknum:pixelTrack:NumPixelHits:NumStripHits:charge:chi2/F:ndof:theta:d0:dz:p:pt:px:py:pz:phi:eta:vx:vy:vz", bufsize);
  
  std::cout << "Made all branches." << std::endl;

}


// Functions that get called by framework every event
void PixelNtuplizer_RD::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //std::cout << " here " << endl;
  int TrackNumber = 0;

  trackonly_.init();

  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
  //edm::Handle<std::vector<Trajectory> > trajCollectionHandle;
  iEvent.getByLabel(conf_.getParameter<std::string>("trajectoryInput"),trajTrackCollectionHandle);

  TrajectoryStateCombiner tsoscomb;
  int NbrTracks =  trajTrackCollectionHandle->size();
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

    fillTrackOnly(iEvent, pixelHits, stripHits, TrackNumber, track);
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
	        rechit_.resXprime = (res.x())*(phiorientation );
	        rechit_.resXprimeErr = errX;
		dZ = gROrZDirection.z() - gPModule.z();
		if(dR != -999) rechit_.residualY = dR;
		else if(dZ != -999) rechit_.residualY = res.y() * (dZ >=0.? +1 : -1) ;
		else rechit_.residualY = res.y();

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

void PixelNtuplizer_RD::fillTrackOnly(const edm::Event& E, int pixelHits, int stripHits, int TrackNumber, const Track& track)
{
  if(pixelHits > 0) trackonly_.pixelTrack = 1;
  trackonly_.tracknum = TrackNumber;
  trackonly_.run = E.id().run();
  trackonly_.evtnum = E.id().event();
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
}

void PixelNtuplizer_RD::fillEvt(const edm::Event& E,int NbrTracks)
{
  evt_.run = E.id().run();
  evt_.evtnum = E.id().event();
  evt_.nbrTracks = NbrTracks;
}

void PixelNtuplizer_RD::fillDet(const DetId &tofill, uint subdetid, const PixelGeomDetUnit* PixGeom)
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

void PixelNtuplizer_RD::fillVertex(const PixelGeomDetUnit* PixGeom)
{
  vertex_.z = PixGeom->surface().position().z();
  vertex_.r = PixGeom->surface().position().perp();
}

void PixelNtuplizer_RD::fillClust(const SiPixelCluster& matchIt, const RectangularPixelTopology* topol, const PixelGeomDetUnit* PixGeom, TrajectoryStateOnSurface& tsos) 
{
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

  clust_.normalized_charge = clust_.charge / (det_.thickness / cos( loctheta ) );
  if ( clust_.normalized_charge <=0 ) clust_.normalized_charge = -1 * clust_.normalized_charge;

}

void PixelNtuplizer_RD::fillPix(const SiPixelCluster & LocPix, const RectangularPixelTopology * topol, const PixelGeomDetUnit * PixGeom)
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

void PixelNtuplizer_RD::fillTrack(TrajectoryStateOnSurface& tsos,const Trajectory &traj, int TrackNumber) 
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


void PixelNtuplizer_RD::init() 
{
  evt_.init();
  det_.init();
  vertex_.init();
  clust_.init();
  pixinfo_.init();
  rechit_.init();
  track_.init();
}

void PixelNtuplizer_RD::EvtStruct::init()
{
  int dummy_int = -9999;

  run = dummy_int;
  evtnum = dummy_int;
}

void PixelNtuplizer_RD::DetStruct::init()
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

void PixelNtuplizer_RD::VertexStruct::init()
{
  float dummy_float = -9999.0;

  r = dummy_float;
  z = dummy_float;
 }

void PixelNtuplizer_RD::ClusterStruct::init()
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

void PixelNtuplizer_RD::PixInfoStruct::init()
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

void PixelNtuplizer_RD::RecHitStruct::init()
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
  resXprime = dummy_float;
  resXprimeErr = dummy_float;  
}

void PixelNtuplizer_RD::TrackStruct::init()
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
void PixelNtuplizer_RD::TrackerHitStruct::init()
{
  float dummy_float = -9999.0;

  globalX = dummy_float;
  globalY = dummy_float;
  globalZ = dummy_float;
} */

void PixelNtuplizer_RD::TrackOnlyStruct::init()
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
}

// define this as a plug-in
//
DEFINE_FWK_MODULE(PixelNtuplizer_RD);
