
// File: PixelNtuplizer.cc
// Description: see PixelNtuplizer.h
// Authors: Petar Maksimovic, Jason Shaev, JHU...Vincenzo Chiochia, CERN
//
// G. Giurgiu, JHU:  - added the check that the associated simHit comes from 
// the generated particle (12/11/06)   
//  - this feature is enabled by     checkType_ = true    in the cfg file
//  - the particle code is given by    genType_
//                   - added nsimhits - number of simhits associated with 
//                                      a rechit 
//                   - added alpha and beta from cluster position (12/18/06)
//                   - make clust_.edgeHitX and clust_.edgeHitY of type int
//
//----------------------------------------------------------------------

#include "DPGAnalysis/SiPixelTools/interface/PixelNtuplizer.h"

// DataFormats
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h" 
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h" 
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/Ref.h"

// Old
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelTopologyBuilder.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

// SimDataFormats
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
// For ROOT
#include <TROOT.h>
// #include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>

// STD
#include <memory>
#include <string>
#include <iostream>

using namespace std;
using namespace edm;

PixelNtuplizer::PixelNtuplizer(edm::ParameterSet const& conf) : 
  conf_(conf),
  src_( conf.getParameter<edm::InputTag>( "src" ) ),
  tfile_(0), 
  t_(0), 
  checkType_(conf.getParameter<bool>("checkType") ), //do we check that the simHit associated with recHit is of the expected particle type ?
  genType_(conf.getParameter<int>("genType") ) // the type of particle that the simHit associated with recHits should be
{}

// Virtual destructor needed.
PixelNtuplizer::~PixelNtuplizer() { }  

// End job: write and close the ntuple file
void PixelNtuplizer::endJob() 
{
  std::cout << " PixelNtuplizer::endJob" << std::endl;
  tfile_->Write();
  tfile_->Close();
}

void PixelNtuplizer::beginJob(const edm::EventSetup& es)
{
  // put here whatever you want to do at the beginning of the job
  std::string outputFile = conf_.getParameter<std::string>("OutputFile");
  tfile_ = new TFile ( outputFile.c_str() , "RECREATE" );

  t_ = new TTree("PixNtuple", "Pixel hit analyzer ntuple");
  int bufsize = 64000;

  // Create one branch. If splitlevel is set, event is a superbranch
  // creating a sub branch for each data member of the Event object.  
  t_->Branch("evt", &evt_, "run/I:evtnum/I", bufsize);
  
  t_->Branch("det", &det_, "thickness/F:cols/I:rows/I:layer/I:ladder/I:module/I:disk/I:blade/I:panel/I:plaquette/I", bufsize);
  
  std::cout << "Making vertex branch:" << std::endl;
  t_->Branch("vertex",   &vertex_,   "num/I:r/F:z/F", bufsize);
  
  std::cout << "Making track branch:" << std::endl;
  t_->Branch("track", &track_, "eta/F:phi/F", bufsize);
  
  std::cout << "Making sim hit branch:" << std::endl;
  t_->Branch("sim",   &sim_, "x/F:y/F:px/F:py/F:pz/F:eloss/F:phi/F:theta/F:subdetid/I:isflipped/I:alpha/F:beta/F:PID/I:TID/i:x1/F:x2:y1:y2:z1:z2:row1:row2:col1:col2:gx1:gx2:gy1:gy2:gz1:gz2:proc/I:trk_eta/F:trk_phi/F:trk_eta2/F:trk_phi2/F", bufsize);
  
  std::cout << "Making cluster branch:" << std::endl;
  t_->Branch("clust", &clust_, "row/F:col/F:x/F:y/F:charge/F:size/I:size_x/I:size_y/I:maxPixelCol/I:maxPixelRow/I:minPixelCol/I:minPixelRow/I:geoId/i:edgeHitX/I:edgeHitY/I:clust_alpha/F:clust_beta/F", bufsize);
  
  std::cout << "Making pixinfo branch:" << std::endl;
  t_->Branch("npix", &pixinfo_.npix, "npix/I", bufsize);
  t_->Branch("rowpix", pixinfo_.row, "row[npix]/F", bufsize);
  t_->Branch("colpix", pixinfo_.col, "col[npix]/F", bufsize);
  t_->Branch("adc", pixinfo_.adc, "adc[npix]/F", bufsize);
  t_->Branch("xpix", pixinfo_.x, "x[npix]/F", bufsize);
  t_->Branch("ypix", pixinfo_.y, "y[npix]/F", bufsize);
  t_->Branch("gxpix", pixinfo_.gx, "gx[npix]/F", bufsize);
  t_->Branch("gypix", pixinfo_.gy, "gy[npix]/F", bufsize);
  t_->Branch("gzpix", pixinfo_.gz, "gz[npix]/F", bufsize);
  
  std::cout << "Making recHit branch:" << std::endl;
  t_->Branch("recHit", &recHit_, "x/F:y/F:xx/F:xy/F:yy/F:row/F:col/F:gx/F:gy/F:gz/F:probx/F:proby/F:nsimhit/I:binq/I", bufsize);

  std::cout << "Made all branches." << std::endl;
}

// Functions that gets called by framework every event
void PixelNtuplizer::analyze(const edm::Event& e, const edm::EventSetup& es)
{
  const bool PRINT = false;

  using namespace edm;
  //std::string rechitProducer = conf_.getParameter<std::string>("RecHitProducer");

  // Get event setup (to get global transformation)
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);

  //--- Fetch Pixel RecHits
  //std::string recHitCollLabel = conf_.getUntrackedParameter<std::string>("RecHitCollLabel","pixRecHitConverter");
  edm::Handle<SiPixelRecHitCollection> recHitColl;
  e.getByLabel( src_, recHitColl);
  
  if ( PRINT )
    std::cout <<" FOUND " 
	      << const_cast<SiPixelRecHitCollection*>(recHitColl.product())->size()
	      << " Pixel RecHits" << std::endl;
  
  //--- Get the simtracks for matching
  Handle<edm::SimTrackContainer> simtracks;
  e.getByLabel("g4SimHits", simtracks);

  TrackerHitAssociator associate( e, conf_ );

  //-----Iterate over detunits
  for (TrackerGeometry::DetContainer::const_iterator it = geom->dets().begin(); it != geom->dets().end(); it++) 
    {
      //--- Since TTree::Fill() will simply take a snapshot of what's 
      //--- in the memory, we need to re-initialize the cache used for 
      //--- each of the branches.
      //init();
      
      DetId detId = ((*it)->geographicalId());
      unsigned int subid = detId.subdetId();
      if ( !((subid==1) || (subid==2)) ) 
	continue; // end subid if
      
      SiPixelRecHitCollection::range pixelrechitRange = (recHitColl.product())->get(detId);
      SiPixelRecHitCollection::const_iterator pixelrechitRangeIteratorBegin = pixelrechitRange.first;
      SiPixelRecHitCollection::const_iterator pixelrechitRangeIteratorEnd = pixelrechitRange.second;
      SiPixelRecHitCollection::const_iterator pixeliter = pixelrechitRangeIteratorBegin;
      std::vector<PSimHit> matched;
      
      //----Loop over rechits for this detId
      for ( ; pixeliter != pixelrechitRangeIteratorEnd; ++pixeliter) 
	{
	  init();

	  LocalPoint lp = pixeliter->localPosition();
	  float rechitx = lp.x();
	  float rechity = lp.y();
	  
	  matched.clear();
	  matched = associate.associateHit(*pixeliter);
		  
	  if ( !matched.empty() )
	    {
	      bool found_hit_from_generated_particle = false;

	      //---Loop over sim hits, fill closest
	      float closest_dist = 99999.9;
	      std::vector<PSimHit>::const_iterator closest_simhit = matched.begin();
	      
	      for (std::vector<PSimHit>::const_iterator m = matched.begin(); m < matched.end(); m++) 
		{
		  if ( checkType_ )
		    {
		      int pid = (*m).particleType();
		      if ( abs(pid) != genType_ )
			continue;
		    } 
		  
		  float simhitx = 0.5 * ( (*m).entryPoint().x() + (*m).exitPoint().x() );
		  float simhity = 0.5 * ( (*m).entryPoint().y() + (*m).exitPoint().y() );

		  float x_res = simhitx - rechitx;
		  float y_res = simhity - rechity;
		  
		  float dist = sqrt( x_res*x_res + y_res*y_res );		  
	
		  if ( dist < closest_dist ) 
		    {
		      closest_dist = dist;
		      closest_simhit = m;
		      found_hit_from_generated_particle = true;
		    } 
		} // end sim hit loop
	      
	      // If this recHit does not have any simHit with the same particleType as the particles generated
	      // ignore it as most probably comes from delta rays.
	      if ( checkType_ && !found_hit_from_generated_particle )
		continue; 
		
	      const PixelGeomDetUnit* theGeomDet =
		dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
	      vertex_.z = theGeomDet->surface().position().z();
	      vertex_.r = theGeomDet->surface().position().perp();
	      
	      det_.thickness = theGeomDet->specificSurface().bounds().thickness();
	      det_.cols = theGeomDet->specificTopology().ncolumns();
	      det_.rows = theGeomDet->specificTopology().nrows();

	      recHit_.nsimhit = (int)matched.size();
	      
	      const RectangularPixelTopology * topol = 
		dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	      
	      fillSim( closest_simhit, subid, theGeomDet, topol, *(simtracks.product()) );
	      
	      fillTrack( *(simtracks.product()) );
	
	      fillRecHit(pixeliter, topol, theGeomDet);

	      //cout << "recHit_.probx = " << recHit_.probx << endl;
	      //cout << "recHit_.proby = " << recHit_.proby << endl;
	      //cout << "recHit_.binq = " << recHit_.binq << endl;
	      	      
	      fillEvt(e);
	      
	      fillDet(detId, subid);
	      
	      edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = pixeliter->cluster();

	      fillPix(*clust, topol, theGeomDet);
	      
	      fillClust(*clust, topol, theGeomDet);
	      	    
	      //++++++++++
	      t_->Fill();
	      //++++++++++
	      
	    } // end matched if
	  
	} // end rechit loop	
      
    } //end det loop
  
} // end analyze function

void PixelNtuplizer::fillRecHit(SiPixelRecHitCollection::const_iterator pixeliter,
				const RectangularPixelTopology * topol, const PixelGeomDetUnit * PixGeom) 
{
  LocalPoint lp = pixeliter->localPosition();
  LocalError le = pixeliter->localPositionError();
  
  recHit_.x = lp.x();
  recHit_.y = lp.y();
  recHit_.xx = le.xx();
  recHit_.xy = le.xy();
  recHit_.yy = le.yy();
  MeasurementPoint mp = topol->measurementPosition(LocalPoint(recHit_.x, recHit_.y));
  recHit_.row = mp.x();
  recHit_.col = mp.y();
  GlobalPoint GP = PixGeom->surface().toGlobal(Local3DPoint(lp.x(),lp.y(),lp.z()));
  recHit_.gx = GP.x();
  recHit_.gy = GP.y();
  recHit_.gz = GP.z();
  
  //recHit_.probx = pixeliter->getProbX();
  //recHit_.proby = pixeliter->getProbY();
  //recHit_.binq  = pixeliter->getBinQ();

  // cout << "recHit_.probx = " << recHit_.probx << endl;
  //cout << "recHit_.proby = " << recHit_.proby << endl;
  //cout << "recHit_.binq = " << recHit_.binq << endl;

}

void PixelNtuplizer::fillSim(std::vector<PSimHit>::const_iterator isim, unsigned int subid, const PixelGeomDetUnit * theGeomDet, 
                             const RectangularPixelTopology * topol, const edm::SimTrackContainer& trks) 
{
  sim_.x1 = (*isim).entryPoint().x(); // width (row index, in col direction)
  sim_.y1 = (*isim).entryPoint().y(); // length (col index, in row direction) 
  sim_.z1 = (*isim).entryPoint().z(); 
  sim_.x2 = (*isim).exitPoint().x();
  sim_.y2 = (*isim).exitPoint().y();
  sim_.z2 = (*isim).exitPoint().z();
  GlobalPoint GP1 = theGeomDet->surface().toGlobal(Local3DPoint((*isim).entryPoint().x(),(*isim).entryPoint().y(),(*isim).entryPoint().z()));
  GlobalPoint GP2 = theGeomDet->surface().toGlobal(Local3DPoint((*isim).exitPoint().x(),(*isim).exitPoint().y(),(*isim).exitPoint().z()));
  sim_.gx1 = GP1.x();
  sim_.gx2 = GP2.x();
  sim_.gy1 = GP1.y();
  sim_.gy2 = GP2.y();
  sim_.gz1 = GP1.z();
  sim_.gz2 = GP2.z();
  
  MeasurementPoint mp1 =
    topol->measurementPosition(LocalPoint((*isim).entryPoint().x(),(*isim).entryPoint().y(),(*isim).entryPoint().z()));
  MeasurementPoint mp2 =
    topol->measurementPosition(LocalPoint((*isim).exitPoint().x(),(*isim).exitPoint().y(), (*isim).exitPoint().z()));
  sim_.row1 = mp1.x();
  sim_.col1 = mp1.y();
  sim_.row2 = mp2.x();
  sim_.col2 = mp2.y();
  
  float sim_xpos = 0.5*(sim_.x1+sim_.x2);
  float sim_ypos = 0.5*(sim_.y1+sim_.y2);         
  sim_.x     = sim_xpos;
  sim_.y     = sim_ypos;
  
  sim_.px  = (*isim).momentumAtEntry().x();
  sim_.py  = (*isim).momentumAtEntry().y();
  sim_.pz  = (*isim).momentumAtEntry().z();
  sim_.eloss = (*isim).energyLoss();
  sim_.phi   = (*isim).phiAtEntry();
  sim_.theta = (*isim).thetaAtEntry();
  sim_.PID = (*isim).particleType();
  sim_.TID = (*isim).trackId();
  sim_.subdetid = subid;
  
  //--- Fill alpha and beta -- more useful for exploring the residuals...
  sim_.beta  = atan2(sim_.pz, sim_.py);
  sim_.alpha = atan2(sim_.pz, sim_.px);
  
  // Is flipped ?
  float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
  float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
  //cout << " 1: " << tmp1 << " 2: " << tmp2 << endl;
  if ( tmp2<tmp1 ) sim_.isflipped = 1;
  else sim_.isflipped = 0;

  sim_.proc = (int)isim->processType();


  SimTrackContainer::const_iterator trksiter;
  for (trksiter = trks.begin(); trksiter != trks.end(); trksiter++) 
    {
      if ( trksiter->trackId() == (sim_.TID - 1) ) 
	{
	  sim_.trk_eta = trksiter->momentum().eta();
	  sim_.trk_phi = trksiter->momentum().phi();
	}
      if ( trksiter->trackId() == sim_.TID ) 
	{
	  sim_.trk_eta2 = trksiter->momentum().eta();
	  sim_.trk_phi2 = trksiter->momentum().phi();
	}
      
    }


}

void PixelNtuplizer::fillDet(DetId &tofill, int subdetid)
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
      unsigned int side = PXFDetId::PXFDetId(tofill).side();
      if (side==1) det_.disk = - det_.disk; 
    } 
}

void 
PixelNtuplizer::fillClust(const SiPixelCluster& matchIt, const RectangularPixelTopology* topol, const PixelGeomDetUnit* PixGeom) 
{
  //const vector<Pixel>  = clustIt->pixels();
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
  
  clust_.geoId = matchIt.geographicalId();
  
  //clust_.edgeHitX = matchIt.edgeHitX();  // Old methods
  //clust_.edgeHitY = matchIt.edgeHitY(); 
  // Replace with the topology methods
  // edge method moved to topologi class
  clust_.edgeHitX = (int) ( topol->isItEdgePixelInX( clust_.minPixelRow ) || topol->isItEdgePixelInX( clust_.maxPixelRow ) );
  clust_.edgeHitY = (int) ( topol->isItEdgePixelInY( clust_.minPixelCol ) || topol->isItEdgePixelInY( clust_.maxPixelCol ) );

  // calculate alpha and beta from cluster position

  // get cluster center of gravity (of charge)
  float xcenter = matchIt.x();
  float ycenter = matchIt.y();
  
  // get the cluster position in local coordinates (cm) 
  LocalPoint mylp = topol->localPosition( MeasurementPoint(xcenter, ycenter) );

  // get the cluster position in global coordinates (cm)
  GlobalPoint gp = PixGeom->surface().toGlobal( mylp );
  float gp_mod = sqrt( gp.x()*gp.x() + gp.y()*gp.y() + gp.z()*gp.z() );

  // normalize
  float gpx = gp.x()/gp_mod;
  float gpy = gp.y()/gp_mod;
  float gpz = gp.z()/gp_mod;

  // make a global vector out of the global point; this vector will point from the 
  // origin of the detector to the cluster
  GlobalVector gv(gpx, gpy, gpz);

  // make local unit vector along local X axis
  const Local3DVector lvx(1.0, 0.0, 0.0);

  // get the unit X vector in global coordinates/
  GlobalVector gvx = PixGeom->surface().toGlobal( lvx );

  // make local unit vector along local Y axis
  const Local3DVector lvy(0.0, 1.0, 0.0);

  // get the unit Y vector in global coordinates
  GlobalVector gvy = PixGeom->surface().toGlobal( lvy );
   
  // make local unit vector along local Z axis
  const Local3DVector lvz(0.0, 0.0, 1.0);

  // get the unit Z vector in global coordinates
  GlobalVector gvz = PixGeom->surface().toGlobal( lvz );
    
  // calculate the components of gv (the unit vector pointing to the cluster) 
  // in the local coordinate system given by the basis {gvx, gvy, gvz}
  // note that both gv and the basis {gvx, gvy, gvz} are given in global coordinates
  float gv_dot_gvx = gv.x()*gvx.x() + gv.y()*gvx.y() + gv.z()*gvx.z();
  float gv_dot_gvy = gv.x()*gvy.x() + gv.y()*gvy.y() + gv.z()*gvy.z();
  float gv_dot_gvz = gv.x()*gvz.x() + gv.y()*gvz.y() + gv.z()*gvz.z();

  // calculate angles
  clust_.clust_alpha = atan2( gv_dot_gvz, gv_dot_gvx );
  clust_.clust_beta  = atan2( gv_dot_gvz, gv_dot_gvy );

}

void
PixelNtuplizer::fillPix(const SiPixelCluster & LocPix, 
			const RectangularPixelTopology * topol, const PixelGeomDetUnit * PixGeom)
{
  const std::vector<SiPixelCluster::Pixel>& pixvector = LocPix.pixels();
  for ( ; pixinfo_.npix < (int)pixvector.size(); ++pixinfo_.npix)
    {
      SiPixelCluster::Pixel holdpix = pixvector[pixinfo_.npix];
      pixinfo_.row[pixinfo_.npix] = holdpix.x;
      pixinfo_.col[pixinfo_.npix] = holdpix.y;
      pixinfo_.adc[pixinfo_.npix] = holdpix.adc;
      LocalPoint lp = topol->localPosition(MeasurementPoint(holdpix.x, holdpix.y));
      pixinfo_.x[pixinfo_.npix] = lp.x();
      pixinfo_.y[pixinfo_.npix]= lp.y();
      GlobalPoint GP =  PixGeom->surface().toGlobal(Local3DPoint(lp.x(),lp.y(),lp.z()));
      pixinfo_.gx[pixinfo_.npix] = GP.x();	
      pixinfo_.gy[pixinfo_.npix]= GP.y();
      pixinfo_.gz[pixinfo_.npix]= GP.z();
    }
}

void 
PixelNtuplizer::fillTrack(const edm::SimTrackContainer& trks) 
{
  SimTrackContainer::const_iterator trksiter;
  for (trksiter = trks.begin(); trksiter != trks.end(); trksiter++) 
    {
      if ( trksiter->trackId() == (sim_.TID - 1) ) 
	{
	  track_.eta = trksiter->momentum().eta();
	  track_.phi = trksiter->momentum().phi();
	}
    }
  //track_.eta = trks[sim_.TID].momentum().eta();
  //track_.phi = trks[sim_.TID].momentum().phi();
}

void
PixelNtuplizer::fillEvt(const edm::Event& E)
{
  evt_.run = E.id().run();
  evt_.evtnum = E.id().event();
}

void PixelNtuplizer::init() 
{
  evt_.init();
  det_.init();
  vertex_.init();
  clust_.init();
  sim_.init();
  track_.init();
  pixinfo_.init();
  recHit_.init();
}

void PixelNtuplizer::evt::init()
{
  int dummy_int = -9999;
  run = dummy_int;
  evtnum = dummy_int;
}

void PixelNtuplizer::Det::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;
  
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

void PixelNtuplizer::vertex::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;

  num = dummy_int;
  r = dummy_float;
  z = dummy_float;
 }

void PixelNtuplizer::track::init()
{
  float dummy_float = -9999.0;

  eta = dummy_float;
  phi = dummy_float;
}

void PixelNtuplizer::sim::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;
  
  x = dummy_float;
  y = dummy_float;
  px = dummy_float;
  py = dummy_float;
  pz = dummy_float;
  eloss = dummy_float;
  phi = dummy_float;
  theta = dummy_float;
  isflipped = dummy_int;
  x1 = x2 = y1 = y2 = z1 = z2 = dummy_float;
  row1 = dummy_float;
  row2 = dummy_float;
  col1 = dummy_float;
  col2 = dummy_float;
  gx1 = dummy_float;
  gx2 = dummy_float;
  gy1 = dummy_float;
  gy2 = dummy_float;
  gz1 = dummy_float;
  gz2 = dummy_float;
  
  proc = dummy_int;
  trk_eta = dummy_float;
  trk_phi = dummy_float;
  trk_eta2 = dummy_float;
  trk_phi2 = dummy_float;

}

void PixelNtuplizer::clust::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;
  unsigned int dummy_uint = 9999;
  
  charge = dummy_float; // convert ke to electrons
  size = dummy_int;
  size_x = dummy_int;
  size_y = dummy_int;
  row = dummy_float;
  col = dummy_float;
  x = dummy_float;
  y = dummy_float;
  maxPixelCol = dummy_int;
  maxPixelRow = dummy_int;
  minPixelCol = dummy_int;
  minPixelRow = dummy_int;
  
  geoId = dummy_uint;
  edgeHitX = dummy_int;
  edgeHitY = dummy_int;

  clust_alpha = dummy_float;
  clust_beta  = dummy_float; 
}

void PixelNtuplizer::pixinfo::init()
{
  npix = 0;
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

void PixelNtuplizer::RecHit::init()
{
  float dummy_float = -9999.0;
  int dummy_int = -9999;
  
  x = dummy_float;
  y = dummy_float;
  xx = dummy_float;
  xy = dummy_float; 
  yy = dummy_float;
  row = dummy_float;
  col = dummy_float;
  gx = dummy_float;
  gy = dummy_float;
  gz = dummy_float;

  nsimhit = dummy_int;

  probx = dummy_float;
  proby = dummy_float;
  binq = dummy_int;
}
