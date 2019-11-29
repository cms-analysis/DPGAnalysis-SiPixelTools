
// SiPixelRecHitsValid.cc
// Description: see SiPixelRecHitsValid.h
// Author: Jason Shaev, JHU
// Created 6/7/06
//
// Make standalone, independent from the validation code. dk 3/14
// Add option for on-track hits 29/Oct/2016 Janos Karancsi
// Convert DAQ histos to simple root 11/19
//--------------------------------

#include "SiPixelRecHitsValid_pix.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include <math.h>

#include "CommonTools/UtilAlgos/interface/TFileService.h"

using namespace std;
using namespace edm;

SiPixelRecHitsValid_pix::SiPixelRecHitsValid_pix(const ParameterSet& ps): 
  conf_(ps),
  trackerHitAssociatorConfig_(ps, consumesCollector() ),
  src_( ps.getParameter<edm::InputTag>( "src" ) ),
  useTracks_( ps.getUntrackedParameter<bool>( "useTracks", false ) ),
  tracks_( ps.getUntrackedParameter<edm::InputTag>( "tracks", edm::InputTag("generalTracks") ) )  {
  if (useTracks_) {
    tTracks = consumes<TrajTrackAssociationCollection>( tracks_ );
  } else {
    tPixelRecHit = consumes<edmNew::DetSetVector<SiPixelRecHit>>( src_ );
  }

  outputFile_ = ps.getUntrackedParameter<string>("outputFile", "pixelrechitshisto.root");
  verbose_ = ps.getUntrackedParameter<bool>("verbose", false);
  muOnly_ = ps.getUntrackedParameter<bool>("muOnly", false);
  ptCut_ = ps.getUntrackedParameter<double>("ptCut", 1.0);

#ifdef PIXEL_ASSOCIATOR
  cout<<" Compare Sim-Rec pixel hits, using pixel hit associator "<<endl;
#else
  cout<<" Compare Sim-Rec pixel hits, using tracker hit associator "<<endl;
#endif
  cout<<" source = "<<(useTracks_? tracks_ : src_)<<endl; //dk

}

//----------------------------------------------------------------
SiPixelRecHitsValid_pix::~SiPixelRecHitsValid_pix() {
}

//-------------------------------------------------------------------------------------
void SiPixelRecHitsValid_pix::analyze(const edm::Event& e, const edm::EventSetup& es) {
  double ptMin=1., etaMax=2.5;
  ptMin = ptCut_;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoHand;
  es.get<TrackerTopologyRcd>().get(tTopoHand);
  const TrackerTopology *tTopo=tTopoHand.product();

  // Check which phase we are in
  edm::ESHandle<TrackerGeometry> trackerGeometryHandle;
  es.get<TrackerDigiGeometryRecord>().get(trackerGeometryHandle);
  auto trackerGeometry = trackerGeometryHandle.product();
  phase_ = 
    trackerGeometry -> isThere(GeomDetEnumerators::P1PXB) &&
    trackerGeometry -> isThere(GeomDetEnumerators::P1PXEC);

  if ( ((int) e.id().event() % 1000 == 0) || verbose_ )
    cout << " Run = " << e.id().run() << " Event = " << e.id().event() << endl;
  
  //Get RecHits
  edm::Handle<SiPixelRecHitCollection> recHitColl;

  // Get tracks
  edm::Handle<TrajTrackAssociationCollection> hTTAC;
  if (useTracks_) {
    e.getByToken(tTracks, hTTAC);
  } else {
    //e.getByLabel( src_, recHitColl);
    e.getByToken(tPixelRecHit , recHitColl);
  }
  
  //Get event setup
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get(geom); 
  const TrackerGeometry& theTracker(*geom);
  
  if(verbose_) cout<<" Call associator "<<endl;
#ifdef PIXEL_ASSOCIATOR
  PixelHitAssociator associate( e); 
  //PixelHitAssociator associate( e, conf_ ); 
#else
  //TrackerHitAssociator associate( e);
  TrackerHitAssociator associate( e, trackerHitAssociatorConfig_); 
#endif

  //iterate over detunits
  for (TrackerGeometry::DetContainer::const_iterator it = geom->dets().begin(); it != geom->dets().end(); it++) {
    DetId detId = ((*it)->geographicalId());
    
    if (!( (detId.subdetId() == PixelSubdetector::PixelBarrel)||
	   (detId.subdetId() == PixelSubdetector::PixelEndcap) )) continue;
    
    const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTracker.idToDet(detId) );
    //if(verbose_) cout<<" pixel det "<<endl;
    
    if (useTracks_) {

      //if(verbose_) cout<<" use tracks "<<endl;

      if (hTTAC.isValid()) {

	const TrajTrackAssociationCollection ttac = *(hTTAC.product());
	if (verbose_) cout << "   hTTAC.isValid() " << ttac.size()<< endl;

	// Loop on traj-track pairs
	for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {
	  
	  if (verbose_) cout << "      TracjTrackAssociationCollection iterating" << endl;
	  reco::TrackRef trackref = it->val;
	  auto pt = trackref->pt();
	  auto eta = trackref->eta();

	  if (verbose_) cout<<" track "<<pt<<" "<<eta<<endl;
	  	  
        // fTkQuality[fTkN]= trackref->qualityMask(); // see: CMSSW/DataFormats/TrackReco/interface/TrackBase.h
        // fTkCharge[fTkN] = trackref->charge();
        // fTkChi2[fTkN]   = trackref->chi2();
        // fTkNdof[fTkN]   = trackref->ndof();
        // fTkPt[fTkN]     = trackref->pt();
        // fTkTheta[fTkN]  = trackref->theta();
        // fTkEta[fTkN]    = trackref->eta();
        // fTkPhi[fTkN]    = trackref->phi();
        // fTkD0[fTkN]     = trackref->d0();
        // fTkDz[fTkN]     = trackref->dz();
        // fTkVx[fTkN]     = trackref->vx();
        // fTkVy[fTkN]     = trackref->vy();
        // fTkVz[fTkN]     = trackref->vz();
        // fTkNHits[fTkN]  = trackref->hitPattern().numberOfValidHits();
        // fTkLHits[fTkN]  = trackref->hitPattern().numberOfLostHits(reco::HitPattern::TRACK_HITS);
        // fTkLHitsI[fTkN] = trackref->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS);  
        // fTkLHitsO[fTkN] = trackref->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_OUTER_HITS); 

	  // look at pt & eta 
	  if(pt<ptMin || abs(eta)>etaMax) continue; // skip tracks 

	  for(trackingRecHit_iterator irecHit = trackref->recHitsBegin();
	      irecHit != trackref->recHitsEnd(); ++irecHit) {

	    //cout<<(*irecHit)->type()<<endl;
	    if( (*irecHit)->type() != 0 ) continue; // skip non valid hits 

	    if (detId == (*irecHit)->geographicalId())
	      matchToSimHits(associate, (*irecHit), detId, theGeomDet,tTopo);
	  
	    //const edm::Ref<std::vector<Trajectory> > refTraj = it->key;
	    //std::vector<TrajectoryMeasurement> tmeasColl =refTraj->measurements();
	    //for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = refTraj->measurements().begin(); 
	    //     tmeasIt!=refTraj->measurements().end(); tmeasIt++) {   
	    //  if (!tmeasIt->updatedState().isValid()) continue; 
	    //  TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt->recHit();
	    //  if (detId == hit->geographicalId()) matchToSimHits(associate, &(*hit), detId, theGeomDet,tTopo);
	    //}
	  }
	}
      }
    } else {
      
      SiPixelRecHitCollection::const_iterator pixeldet = recHitColl->find(detId);
      if (pixeldet == recHitColl->end()) continue;
      if(verbose_) cout<<" pixel det "<<pixeldet->size()<<endl;
      //----Loop over rechits for this detId
      SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixeldet->begin();
      for (; pixeliter != pixeldet->end(); pixeliter++) matchToSimHits(associate, &(*pixeliter), detId, theGeomDet,tTopo);
    }
  } // <------ end detunit loop
}
//--------------------------------------------------------------------------------
#ifdef PIXEL_ASSOCIATOR
void SiPixelRecHitsValid_pix::matchToSimHits(const PixelHitAssociator& associate, const TrackingRecHit* hit, 
					     DetId detId, const PixelGeomDetUnit* theGeomDet, const TrackerTopology *tTopo) {
#else
void SiPixelRecHitsValid_pix::matchToSimHits(const TrackerHitAssociator& associate, const TrackingRecHit* hit, 
					     DetId detId, const PixelGeomDetUnit* theGeomDet, const TrackerTopology *tTopo) {
#endif


  std::vector<PSimHit> matched = associate.associateHit(*hit); // get the matched simhits
  
  if(verbose_) 
    cout<<" rechit "<<hit->localPosition().x()<<" "<<matched.size()<<endl;
  
  if ( !matched.empty() ) {
    float closest = 9999.9;
    std::vector<PSimHit>::const_iterator closestit = matched.begin();
    LocalPoint lp = hit->localPosition();
    float rechit_x = lp.x();
    float rechit_y = lp.y();
    
    
    //loop over sim hits and fill closet
    for (std::vector<PSimHit>::const_iterator m = matched.begin(); m<matched.end(); m++) {
      float sim_x1 = (*m).entryPoint().x();
      float sim_x2 = (*m).exitPoint().x();
      float sim_xpos = 0.5*(sim_x1+sim_x2);
      
      float sim_y1 = (*m).entryPoint().y();
      float sim_y2 = (*m).exitPoint().y();
      float sim_ypos = 0.5*(sim_y1+sim_y2);
      
      float x_res = fabs(sim_xpos - rechit_x);
      float y_res = fabs(sim_ypos - rechit_y);
      
      float dist = sqrt(x_res*x_res + y_res*y_res); // in cm
      
      if ( dist < closest ) {
	//closest = x_res;
	closest = dist;
	closestit = m;
        
	if(verbose_) 
	  std::cout<<" simhit "
		   <<(*m).pabs()<<" "
		   <<(*m).thetaAtEntry()<<" "
		   <<(*m).phiAtEntry()<<" "
		   <<(*m).particleType()<<" "
		   <<(*m).trackId()<<" "
		   <<(*m).momentumAtEntry()
		   <<std::endl;
	
      }
    } // end sim hit loop
    
    if(verbose_) cout<<" closest "<<closest<<" "<<detId.subdetId()<<endl;
    
    if (detId.subdetId() == PixelSubdetector::PixelBarrel)
      fillBarrel(hit, *closestit, detId, theGeomDet,tTopo);
    else if (detId.subdetId() == PixelSubdetector::PixelEndcap)
      fillForward(hit, *closestit, detId, theGeomDet,tTopo);
  } // end matched emtpy
}

//-------------------------------------------------------------------------------------
void SiPixelRecHitsValid_pix::fillBarrel(const TrackingRecHit* recHit, const PSimHit& simHit, 
				     DetId detId, const PixelGeomDetUnit* theGeomDet,
				     const TrackerTopology *tTopo) {
  const float cmtomicron = 10000.0; 
  const float PI = 3.1416;
  const int NumLayers = 4;
  bool PRINT = verbose_;

  float phi = simHit.phiAtEntry();
  float theta = simHit.thetaAtEntry(); // theta with respect to module coordinates 
  float beta = 0;  // beta is roughly like real theta
  if(theta<PI/2.) beta=(PI/2.) - theta; //  
  else            beta=theta - (PI/2.);
  float eta = -log(tan(beta/2.));  // this is an approximation to the tracks eta, only positive

  //float phiLocal = simHit.localDirection().phi(); // same as phi
  //float thetaLocal = simHit.localDirection().theta(); // same as theta

  //float pt  = simHit.momentumAtEntry().perp(); // crap, not real pt
  float p  = simHit.pabs();
  int pid = simHit.particleType();

  // skip secondaries
  if(muOnly_ && (abs(pid)!=13 || p<10.)) {if(PRINT) cout<<" skip "<<p<<" "<<pid<<endl; return;}

  unsigned int layer = tTopo->pxbLayer(detId);
  unsigned int module = tTopo->pxbModule(detId);
  // The inner ladder (in the smaller radius) are flipped
  // unsigned int ladder = tTopo->pxbLadder(detId);
  // bool odd_lad = ladder % 2;
  // bool inner = (phase_ ? layer == 4 : layer % 2) ? !odd_lad : odd_lad;
  // Determining flipped ladders automatically using surface
  float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
  float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
  bool flipped = (tmp2<tmp1);
  //std::cout<<"Flipped (Surface): "<<(tmp2<tmp1)<<" Flipped (Inner): "<<inner<<std::endl;
  if(PRINT) cout<<" layer "<<layer<<" eta "<<eta<<" phi "<<phi<<endl;

  // skip, for tests only
  //if( abs(eta)<0.1 || abs(eta)>1.0) return; // limit 
  //if( abs(phi)<1.3 || abs(phi)>1.9) return;  // limit to l2 acceptance

  if(PRINT) std::cout<<" simhit "
		     <<p<<" "
		     <<pid<<" "
		     <<phi<<" "
		     <<theta<<" "
		     <<simHit.trackId()<<" lay "
		     <<layer<<" mod "
		     <<module<<std::endl;

  hphi1->Fill(phi);
  htheta1->Fill(theta);
  hbeta1->Fill(beta);
  heta1->Fill(eta);

  LocalPoint lp = recHit->localPosition();
  float lp_y = lp.y();  
  float lp_x = lp.x();

  LocalError lerr = recHit->localPositionError();
  float lerr_x = sqrt(lerr.xx());
  float lerr_y = sqrt(lerr.yy());
  //cout<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<endl;

  
  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  float res_x = (lp.x() - sim_xpos)*cmtomicron;
  
  
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  float res_y = (lp.y() - sim_ypos)*cmtomicron;
  
  if(PRINT) // dk 
    cout<<detId.rawId()<<" "<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<" "
	<<sim_xpos<<" "<<sim_ypos<<" "<<res_x<<" "<<res_y
	<<endl;

  float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  float pull_y = ( lp_y - sim_ypos ) / lerr_y;

  recHitXResAllB->Fill(res_x);
  recHitYResAllB->Fill(res_y);
  recHitXPullAllB->Fill(pull_x);  
  recHitYPullAllB->Fill(pull_y);


  // as a function of layer
  recHitXResLayer[layer-1]->Fill(res_x); 
  recHitYResLayer[layer-1]->Fill(res_y);  
  recHitXErrorB[layer-1]->Fill(lerr_x); 
  recHitYErrorB[layer-1]->Fill(lerr_y);
    
  //get cluster
  SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)recHit)->cluster();
  int sizeX = (*clust).sizeX();
  int sizeY = (*clust).sizeY();

  if (flipped) {
    recHitXResFlippedL[layer-1]->Fill(res_x);
    if(module<5) recHitXResFlippedLmz[layer-1]->Fill(res_x); // split into -+z
    else         recHitXResFlippedLpz[layer-1]->Fill(res_x); // split into -+z
  } else {
    recHitXResNonFlippedL[layer-1]->Fill(res_x);
    if(module<5) recHitXResNonFlippedLmz[layer-1]->Fill(res_x); //split into -+z
    else         recHitXResNonFlippedLpz[layer-1]->Fill(res_x); //split into -+z
  } // end if

  // Eta dependence 
  recHitXResLayerP[layer-1]->Fill(eta,std::abs(res_x));
  recHitYResLayerP[layer-1]->Fill(eta,std::abs(res_y));

  // split into size  as a function of layer
  for (unsigned int i=0; i<NumLayers; i++) {
    if (layer == i+1)  { // select layer
	if(sizeX==1) {
	  recHitXResLayerP1[i]->Fill(eta,std::abs(res_x));
	  //if(i==0) recHitL1XResSize1->Fill(res_x);  // layer 1
	  //else if(i==1) recHitL2XResSize1->Fill(res_x); // layer 2
	} else if(sizeX==2) {
	  recHitXResLayerP2[i]->Fill(eta,std::abs(res_x));
	  //if(i==0) recHitL1XResSize2->Fill(res_x);
	  //else if(i==1) recHitL2XResSize2->Fill(res_x);
	} else if(sizeX>=3) {
	  recHitXResLayerP3[i]->Fill(eta,std::abs(res_x));
	  //if(i==0) recHitL1XResSize3->Fill(res_x);
	  //else if(i==1) recHitL2XResSize3->Fill(res_x);
	}
	
	if(sizeY==1)      recHitYResLayerP1[i]->Fill(eta,std::abs(res_y));
	else if(sizeY==2) recHitYResLayerP2[i]->Fill(eta,std::abs(res_y));
	else if(sizeY==3) recHitYResLayerP3[i]->Fill(eta,std::abs(res_y));
	else if(sizeY>=4) recHitYResLayerP4[i]->Fill(eta,std::abs(res_y));
     }
  }
  
#ifdef SPLIT_INTO_ETA
  // as a function of eta
  for (unsigned int i=0; i<25; i++) {
    float eta1=float(i)*0.1;
    float eta2=eta1+0.1;
    if(eta>=eta1 && eta<eta2) { 
      if (layer == 1) 
       	{recHitXResLayer1Eta[i]->Fill(res_x);recHitYResLayer1Eta[i]->Fill(res_y);}
      else if (layer == 2) 
       	{recHitXResLayer2Eta[i]->Fill(res_x);recHitYResLayer2Eta[i]->Fill(res_y);}
      else if (layer == 3) 
       	{recHitXResLayer3Eta[i]->Fill(res_x);recHitYResLayer3Eta[i]->Fill(res_y);}
      else if (layer == 4) 
       	{recHitXResLayer4Eta[i]->Fill(res_x);recHitYResLayer4Eta[i]->Fill(res_y);}
    } // eta 
  } // i (eta)
#endif  

#ifdef SPLIT_INTO_Z
  // Y resolution, fill module dependent info
  for (unsigned int i=0; i<8; i++) {
    if (module == i+1) {
      //float charge = (*clust).charge();
      if (layer == 1)      {recHitYResLayer1Modules[i]->Fill(res_y);}
      else if (layer == 2) {recHitYResLayer2Modules[i]->Fill(res_y);}
      else if (layer == 3) {recHitYResLayer3Modules[i]->Fill(res_y);} 
      else if (layer == 4) {recHitYResLayer3Modules[i]->Fill(res_y);} 
    } // if module 
  } // end for
#endif // SPLIT_INTO_Z

#ifdef LOOK_AT_ERRORS  
  // Look at errors
  LocalError lape = theGeomDet->localAlignmentError();
    if (lape.valid()) {
      float tmp11= 0.;
      if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
      float tmp12= sqrt(lape.xy())*1E4;
      float tmp13= 0.;
      if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
      bool tmp14=tmp2<tmp1;
      if( (layer) == 1) 
      	{recHitXAlignError1->Fill(tmp11); recHitYAlignError1->Fill(tmp13);}
      else if( (layer) == 2) 
      	{recHitXAlignError2->Fill(tmp11); recHitYAlignError2->Fill(tmp13);}
      else if( (layer) == 3) 
      	{recHitXAlignError3->Fill(tmp11); recHitYAlignError3->Fill(tmp13);}
      else if( (layer) == 4) 
      	{recHitXAlignError4->Fill(tmp11); recHitYAlignError4->Fill(tmp13);}
      else {cout<<" unknown layer "<< layer<<endl;}
    }  // if lape
#endif // LOOK_AT_ERRORS



}

int SiPixelRecHitsValid_pix::PhaseIBladeOfflineToOnline(const int& blade)
{
  int blade_online = -999;
  if(1  <= blade && blade < 6)  blade_online = 6  - blade; // 5 on 1st quarter
  if(6  <= blade && blade < 17) blade_online = 5  - blade; // 11 on 2nd half
  if(17 <= blade && blade < 23) blade_online = 28 - blade; // 6 on 4th quarter
  if(23 <= blade && blade < 31) blade_online = 31 - blade; // 8 on 1st quarter
  if(31 <= blade && blade < 48) blade_online = 30 - blade; // 17 on 2nd half
  if(48 <= blade && blade < 57) blade_online = 65 - blade; // 9 on 4th quarter
  return blade_online;
}

// ------------------------------------------------------------------------------
void SiPixelRecHitsValid_pix::fillForward(const TrackingRecHit* recHit, const PSimHit & simHit, 
				      DetId detId,const PixelGeomDetUnit * theGeomDet,
				      const TrackerTopology *tTopo) {
  const float cmtomicron = 10000.0;

  //int rows = theGeomDet->specificTopology().nrows();
  //int cols = theGeomDet->specificTopology().ncolumns();
  
  LocalPoint lp = recHit->localPosition();
  float lp_x = lp.x();
  float lp_y = lp.y();
  
  LocalError lerr = recHit->localPositionError();
  float lerr_x = sqrt(lerr.xx());
  float lerr_y = sqrt(lerr.yy());

  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  
  float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  float pull_y = ( lp_y - sim_ypos ) / lerr_y;

  float res_x = (lp.x() - sim_xpos)*cmtomicron;  
  float res_y = (lp.y() - sim_ypos)*cmtomicron;

  // integrated fpix results 
  recHitXResAllF->Fill(res_x);
  recHitYResAllF->Fill(res_y);  
  recHitXPullAllF->Fill(pull_x);
  recHitYPullAllF->Fill(pull_y);  
  recHitXErrorAllF->Fill(lerr_x);
  recHitYErrorAllF->Fill(lerr_y);

  // spliy into disks etc.
  int disk = tTopo->pxfDisk(detId);
  int panel = tTopo->pxfPanel(detId);      // Phase 1: Forward 1, Backward 2
  int side = tTopo->pxfSide(detId);
  //int module = tTopo->pxfModule(detId);  // Phase 1: Always 1
  // Phase 1 specific
  int blade  = tTopo->pxfBlade(detId);     // Phase 1: Inner blades 1-22, Outer blades 23-56
  int ring = 1 + int(blade>22);               // Phase 1: Inner: 1, Outer: 2
  //int phase1_online_blade = PhaseIBladeOfflineToOnline(blade); // Phase 1: Ring1 +-1-11, Ring2 +-1-17
  //int xside = 1 + (phase1_online_blade>0); // Phase 1: -X 1, +X 2

  recHitXResRingSide[ring-1][side-1]->Fill(res_x);
  recHitYResRingPanel[ring-1][panel-1]->Fill(res_y);
#ifdef SPLIT_INTO_PANELS
  recHitXResRingSidePanel[ring-1][side-1][panel-1]->Fill(res_x);
  recHitYResRingSidePanel[ring-1][side-1][panel-1]->Fill(res_y);
#endif

  if (side==1) {
    recHitXResNegZF->Fill(res_x);
    recHitYResNegZF->Fill(res_y);
  } else if (side==2) {
    recHitXResPosZF->Fill(res_x);
    recHitYResPosZF->Fill(res_y);  
  }

  // get cluster
  SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)recHit)->cluster();
  //int sizeX = (*clust).sizeX(), sizeY = (*clust).sizeY();
  //float charge = (*clust).charge();

  if(disk==1) {
    recHitXResDisk1[0]->Fill(res_x);
    recHitYResDisk1[0]->Fill(res_y);
    if(side==1) {  // -z 
      recHitXResDisk1[1]->Fill(res_x);
      recHitYResDisk1[1]->Fill(res_y);
      if(panel==1) {  // panel 1 
	recHitXResDisk1[3]->Fill(res_x);
	recHitYResDisk1[3]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk1[4]->Fill(res_x);
	recHitYResDisk1[4]->Fill(res_y);
      }
    } else { //+z
      recHitXResDisk1[2]->Fill(res_x);
      recHitYResDisk1[2]->Fill(res_y);
      if(panel==1) {  // panel 1 
	recHitXResDisk1[5]->Fill(res_x);
	recHitYResDisk1[5]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk1[6]->Fill(res_x);
	recHitYResDisk1[6]->Fill(res_y);
      }
    }

  } else if (disk==2) {

    recHitXResDisk2[0]->Fill(res_x);
    recHitYResDisk2[0]->Fill(res_y);
    if(side==1) {  // -z 
      recHitXResDisk2[1]->Fill(res_x);
      recHitYResDisk2[1]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk2[3]->Fill(res_x);
	recHitYResDisk2[3]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk2[4]->Fill(res_x);
	recHitYResDisk2[4]->Fill(res_y);
      }
    } else { //+z
      recHitXResDisk2[2]->Fill(res_x);
      recHitYResDisk2[2]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk2[5]->Fill(res_x);
	recHitYResDisk2[5]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk2[6]->Fill(res_x);
	recHitYResDisk2[6]->Fill(res_y);
      }
    }

  } else if (disk==3) {

    recHitXResDisk3[0]->Fill(res_x);
    recHitYResDisk3[0]->Fill(res_y);
    if(side==1) {  // -z 
      recHitXResDisk3[1]->Fill(res_x);
      recHitYResDisk3[1]->Fill(res_y);
      if(panel==1) {  // panel 1 
	recHitXResDisk3[3]->Fill(res_x);
	recHitYResDisk3[3]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk3[4]->Fill(res_x);
	recHitYResDisk3[4]->Fill(res_y);
      }
    } else { //+z
      recHitXResDisk3[2]->Fill(res_x);
      recHitYResDisk3[2]->Fill(res_y);
      if(panel==1) {  // panel 1 
	recHitXResDisk3[5]->Fill(res_x);
	recHitYResDisk3[5]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk3[6]->Fill(res_x);
	recHitYResDisk3[6]->Fill(res_y);
      }
    }

  } // end disk 


#ifdef LOOK_AT_ERRORS  
  LocalError lape = theGeomDet->localAlignmentError();
  if (lape.valid()) {
    float tmp11= 0.;
    if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
    float tmp12= sqrt(lape.xy())*1E4;
    float tmp13= 0.;
    if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
    if( (tTopo->pxfDisk(detId)) == 1)  { // disk 1
    	{recHitXAlignError5->Fill(tmp11); recHitYAlignError5->Fill(tmp13);}
    } else if( (tTopo->pxfDisk(detId)) == 2) {  // disk 2
    	{recHitXAlignError6->Fill(tmp11); recHitYAlignError6->Fill(tmp13);}
    } else if( (tTopo->pxfDisk(detId)) == 3) {  // disk 3
    	{recHitXAlignError7->Fill(tmp11); recHitYAlignError7->Fill(tmp13);}
    }
    else {cout<<" unkown disk "<<tTopo->pxfDisk(detId)<<endl;}

  } // if lape 
#endif // LOOK_AT_ERRORS    

}

//-----------------------------------------------------------------
void SiPixelRecHitsValid_pix::beginJob() {
  cout << "beginJob" <<endl;

  edm::Service<TFileService> fs;

  hphi1 =   fs->make<TH1D>("hphi1","phi1",70,-3.5,3.5);
  htheta1 = fs->make<TH1D>("htheta1","theta1",70,-3.5,3.5);
  hbeta1  = fs->make<TH1D>("beta1","beta1",70,-3.5,3.5);
  heta1 = fs->make<TH1D>("heta1","eta",50,-2.5,2.5);

  // ---------------------------------------------------------------
  // All histograms that depend on plaquette number have 7 indexes.
  // The first 4 (0-3) correspond to Panel 1 plaquettes 1-4.
  // The last 3 (4-6) correspond to Panel 2 plaquettes 1-3.
  // ---------------------------------------------------------------
  const int NumLayers=4;
  Char_t histo[200];

  //RecHit Resolution all barrel hits
  recHitXResAllB = fs->make<TH1D>("recHitXResAllB","RecHit X Res All Modules in Barrel", 100, -200., 200.);
  recHitYResAllB = fs->make<TH1D>("recHitYResAllB","RecHit Y Res All Modules in Barrel", 100, -200., 200.);
  recHitXPullAllB = fs->make<TH1D>("recHitXPullAllB", 
				   "RecHit X Pull All Modules in Barrel"        , 100, -10.0, 10.0);
  recHitYPullAllB = fs->make<TH1D>("recHitYPullAllB", 
				   "RecHit Y Pull All Modules in Barrel"        , 100, -10.0, 10.0);


  for (int i=0; i<NumLayers; i++) {  // per layer histos
    //RecHit X resolution per layer
    sprintf(histo, "recHitXResLayer%d", i+1);
    recHitXResLayer[i] = fs->make<TH1D>(histo, "RecHit XRes by Layer", 100, -200., 200.);
    
    //RecHit Y resolution per layer
    sprintf(histo, "recHitYResLayer%d", i+1);
    recHitYResLayer[i] = fs->make<TH1D>(histo, "RecHit YRes by Layer", 100, -200., 200.);
    
    //RecHit X resolution per layer Profile 
    sprintf(histo, "recHitXResLayerP%d", i+1);
    recHitXResLayerP[i] = fs->make<TProfile>(histo, "RecHit XRes by Layer", 25, 0., 2.5,0.,1000.," ");
    
    sprintf(histo, "recHitXResLayerP1%d", i+1);
    recHitXResLayerP1[i] = fs->make<TProfile>(histo, "RecHit XRes by Layer, size 1", 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitXResLayerP2%d", i+1);
    recHitXResLayerP2[i] = fs->make<TProfile>(histo, "RecHit XRes by Layer, size 2", 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitXResLayerP3%d", i+1);
    recHitXResLayerP3[i] = fs->make<TProfile>(histo, "RecHit XRes by Layer, size 3", 25, 0., 2.5,0.,1000.," ");
    
    //RecHit Y resolution per layer Profile 
    sprintf(histo, "recHitYResLayerP%d", i+1);
    recHitYResLayerP[i] = fs->make<TProfile>(histo, "RecHit YRes by Layer", 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP1%d", i+1);
    recHitYResLayerP1[i] = fs->make<TProfile>(histo, "RecHit YRes by Layer, size 1", 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP2%d", i+1);
    recHitYResLayerP2[i] = fs->make<TProfile>(histo, "RecHit YRes by Layer, size 2", 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP3%d", i+1);
    recHitYResLayerP3[i] = fs->make<TProfile>(histo, "RecHit YRes by Layer, size 3", 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP4%d", i+1);
    recHitYResLayerP4[i] = fs->make<TProfile>(histo, "RecHit YRes by Layer, size 4", 25, 0., 2.5,0.,1000.," ");
       
    //RecHit X resolution for flipped ladders by layer
    sprintf(histo, "recHitXResFlippedL%d", i+1);
    recHitXResFlippedL[i] = fs->make<TH1D>(histo, "RecHit XRes Flipped Ladders by Layer",100,-200.,200.);
      
    //RecHit X resolution for unflipped ladders by layer
    sprintf(histo, "recHitXResNonFlippedLadderl%d", i+1);
    recHitXResNonFlippedL[i] = fs->make<TH1D>(histo, "RecHit XRes NonFlipped Ladders by Layer", 100, -200., 200.);

    // Same plots for -Z/+Z
    sprintf(histo, "recHitXResFlippedLmz%d", i+1);
    recHitXResFlippedLmz[i] = fs->make<TH1D>(histo, "RecHit XRes Flipped Ladders by Layer, -Z", 100, -200., 200.);
    sprintf(histo, "recHitXResNonFlippedmz%d", i+1);
    recHitXResNonFlippedLmz[i] = fs->make<TH1D>(histo, "RecHit XRes NonFlipped Ladders by Layer, -Z", 100, -200., 200.);
    sprintf(histo, "recHitXResFlippedLpz%d", i+1);
    recHitXResFlippedLpz[i] = fs->make<TH1D>(histo, "RecHit XRes Flipped Ladders by Layer, +Z", 100, -200., 200.);
    sprintf(histo, "recHitXResNonFlippedpz%d", i+1);
    recHitXResNonFlippedLpz[i] = fs->make<TH1D>(histo, "RecHit XRes NonFlipped Ladders by Layer, +Z", 100, -200., 200.);

    // Errors 
    sprintf(histo, "recHitXErrorB%d", i+1);
    recHitXErrorB[i] = fs->make<TH1D>(histo,"RecHit X Error in Barrel by Layer", 100, 0, 0.1);
    sprintf(histo, "recHitYErrorB%d", i+1);
    recHitYErrorB[i] = fs->make<TH1D>(histo,"RecHit Y Error in Barrel by Layer", 100, 0, 0.1);

  } // end for layers 

#ifdef SPLIT_INTO_Z
  //RecHit Y resolutions for layers by module for barrel
  for (int i=0; i<8; i++) {
    sprintf(histo, "recHitYResLayer1Module%d", i+1);
    recHitYResLayer1Modules[i] = fs->make<TH1D>(histo, "RecHit YRes Layer1 by module", 100, -200., 200.);
    sprintf(histo, "recHitYResLayer2Module%d", i+1);
    recHitYResLayer2Modules[i] = fs->make<TH1D>(histo, "RecHit YRes Layer2 by module", 100, -200., 200.);
    sprintf(histo, "recHitYResLayer3Module%d", i+1);
    recHitYResLayer3Modules[i] = fs->make<TH1D>(histo, "RecHit YRes Layer3 by module", 100, -200., 200.); 
    sprintf(histo, "recHitYResLayer4Module%d", i+1);
    recHitYResLayer4Modules[i] = fs->make<TH1D>(histo, "RecHit YRes Layer4 by module", 100, -200., 200.); 
  } // end for
#endif  // split_into_z
  

#ifdef LOOK_AT_ERRORS
  // alignment errors
  recHitXAlignError1 = 
    fs->make<TH1D>("recHitXAlignError1","RecHit X  Alignment errors bpix 1", 100, 0., 100.);
  recHitXAlignError2 = 
    fs->make<TH1D>("recHitXAlignError2","RecHit X  Alignment errors bpix 2", 100, 0., 100.);
  recHitXAlignError3 = 
    fs->make<TH1D>("recHitXAlignError3","RecHit X  Alignment errors bpix 3", 100, 0., 100.);
  recHitXAlignError4 = 
    fs->make<TH1D>("recHitXAlignError4","RecHit X  Alignment errors bpix 4", 100, 0., 100.);
  recHitYAlignError1 = 
    fs->make<TH1D>("recHitYAlignError1","RecHit Y  Alignment errors bpix 1", 100, 0., 100.);
  recHitYAlignError2 = 
    fs->make<TH1D>("recHitYAlignError2","RecHit Y  Alignment errors bpix 2", 100, 0., 100.);
  recHitYAlignError3 = 
    fs->make<TH1D>("recHitYAlignError3","RecHit Y  Alignment errors bpix 3", 100, 0., 100.);
  recHitYAlignError4 = 
    fs->make<TH1D>("recHitYAlignError4","RecHit Y  Alignment errors bpix 4", 100, 0., 100.);
#endif // LOOK_AT_ERRORS
      
#ifdef SPLIT_INTO_ETA
  // eta plots
  for (int j=0; j<25; j++) {
    //RecHit X resolution per layer
    sprintf(histo, "recHitXResLayer1Eta%d", j+1);
    recHitXResLayer1Eta[j] = fs->make<TH1D>(histo, "RecHit XRes Layer1, eta", 100, -200., 200.);
    sprintf(histo, "recHitXResLayer2Eta%d", j+1);
    recHitXResLayer2Eta[j] = fs->make<TH1D>(histo, "RecHit XRes Layer2, eta", 100, -200., 200.);
    sprintf(histo, "recHitXResLayer3Eta%d", j+1);
    recHitXResLayer3Eta[j] = fs->make<TH1D>(histo, "RecHit XRes Layer3, eta", 100, -200., 200.);
    sprintf(histo, "recHitXResLayer4Eta%d", j+1);
    recHitXResLayer4Eta[j] = fs->make<TH1D>(histo, "RecHit XRes Layer4, eta", 100, -200., 200.);
    
    //RecHit Y resolution per layer
    sprintf(histo, "recHitYResLayer1Eta%d", j+1);
    recHitYResLayer1Eta[j] = fs->make<TH1D>(histo, "RecHit YRes Layer1, eta", 100, -200., 200.);
    sprintf(histo, "recHitYResLayer2Eta%d", j+1);
    recHitYResLayer2Eta[j] = fs->make<TH1D>(histo, "RecHit YRes Layer2, eta", 100, -200., 200.);
    sprintf(histo, "recHitYResLayer3Eta%d", j+1);
    recHitYResLayer3Eta[j] = fs->make<TH1D>(histo, "RecHit YRes Layer3, eta", 100, -200., 200.);
    sprintf(histo, "recHitYResLayer4Eta%d", j+1);
    recHitYResLayer4Eta[j] = fs->make<TH1D>(histo, "RecHit YRes Layer4, eta", 100, -200., 200.);
  }
#endif //SPLIT_INTO_ETA


  // special test histos for layer 1
  //     recHitL1XResSize1    = fs->make<TH1D>("recHitL1XSize1",      "XRes size 1 L1", 100, -200., 200.);   
  //     recHitL1XResSize2    = fs->make<TH1D>("recHitL1XSize2",      "XRes size 2 L1", 100, -200., 200.);   
  //     recHitL1XResSize3    = fs->make<TH1D>("recHitL1XSize3",      "XRes size 3 L1", 100, -200., 200.);   
  //     recHitL2XResSize1    = fs->make<TH1D>("recHitL2XSize1",      "XRes size 1 L2", 100, -200., 200.);   
  //     recHitL2XResSize2    = fs->make<TH1D>("recHitL2XSize2",      "XRes size 2 L2", 100, -200., 200.);   
  //     recHitL2XResSize3    = fs->make<TH1D>("recHitL2XSize3",      "XRes size 3 L2", 100, -200., 200.);   


  // FPix 
  //RecHit resolution all disks
  recHitXResAllF = fs->make<TH1D>("recHitXResAllF", "RecHit X Res All in Forward", 100, -200., 200.);
  recHitXResPosZF = fs->make<TH1D>("recHitXResPosZF", "RecHit X Res All in Forward Disks +Z", 100, -200., 200.);
  recHitXResNegZF = fs->make<TH1D>("recHitXResNegZF", "RecHit X Res All in Forward Disks -Z", 100, -200., 200.);
  recHitYResAllF = fs->make<TH1D>("recHitYResAllF", "RecHit Y Res All in Forward", 100, -200., 200.);
  recHitYResPosZF = fs->make<TH1D>("recHitYResPosZ", "RecHit Y Res All in Forward Disks +Z", 100, -200., 200.);
  recHitYResNegZF = fs->make<TH1D>("recHitYResNegZ", "RecHit Y Res All in Forward Disks -Z", 100, -200., 200.);
  recHitXPullAllF = fs->make<TH1D>("recHitXPullAllF", "RecHit X Pull All in Forward", 100, -10.0, 10.0);  
  recHitYPullAllF = fs->make<TH1D>("recHitYPullAllF", "RecHit Y Pull All in Forward", 100, -10.0, 10.0);
  recHitXErrorAllF = fs->make<TH1D>("recHitXErrorAllF", "RecHit X Error All in Forward", 100, 0.0, 0.1);  
  recHitYErrorAllF = fs->make<TH1D>("recHitYErrorAllF", "RecHit Y Error All in Forward", 100, 0.0, 0.1);

  // histograms per disk, side, panel, ring  
  for (int i=0; i<7; i++) { // 0 all, 1 -z, 2 +z, 3 panel 1, 4 panel 2, 5 ring 1, 6 ring 2
    //X resolution for Disk1 
    sprintf(histo, "recHitXResDisk1%d", i+1);
    recHitXResDisk1[i] = fs->make<TH1D>(histo, "RecHit XRes Disk1", 100, -200., 200.); 
    //X resolution for Disk2 
    sprintf(histo, "recHitXResDisk2%d", i+1);
    recHitXResDisk2[i] = fs->make<TH1D>(histo, "RecHit XRes Disk2", 100, -200., 200.);  
    //X resolution for Disk=3 
    sprintf(histo, "recHitXResDisk3%d", i+1);
    recHitXResDisk3[i] = fs->make<TH1D>(histo, "RecHit XRes Disk3", 100, -200., 200.);  
    
    //Y resolution for Disk1
    sprintf(histo, "recHitYResDisk1%d", i+1);
    recHitYResDisk1[i] = fs->make<TH1D>(histo, "RecHit YRes Disk1", 100, -200., 200.);
    //Y resolution for Disk2 
    sprintf(histo, "recHitYResDisk2%d", i+1);
    recHitYResDisk2[i] = fs->make<TH1D>(histo, "RecHit YRes Disk2", 100, -200., 200.);
    //Y resolution for Disk3 
    sprintf(histo, "recHitYResDisk3%d", i+1);
    recHitYResDisk3[i] = fs->make<TH1D>(histo, "RecHit YRes Disk3", 100, -200., 200.);
  }

  // jk 18 Feb 2017
  for (int ring=0; ring<2; ring++) {
#ifdef SPLIT_INTO_PANELS    
    for (int side=0; side<2; side++) for (int panel=0; panel<2; panel++) {
  	sprintf(histo, "RecHitXResRing%dSide%dPanel%d", ring+1, side+1, panel+1);
  	recHitXResRingSidePanel[ring][side][panel] = fs->make<TH1D>(histo, histo, 100, -200., 200.); 
  	sprintf(histo, "RecHitYResRing%dSide%dPanel%d", ring+1, side+1, panel+1);
  	recHitYResRingSidePanel[ring][side][panel] = fs->make<TH1D>(histo, histo, 100, -200., 200.);
      }
#endif
    for (int side=0; side<2; side++) {
      sprintf(histo, "recHitXResRing%dSide%d", ring+1, side+1);
      recHitXResRingSide[ring][side] = fs->make<TH1D>(histo, histo, 100, -200., 200.); 
    }
    for (int panel=0; panel<2; panel++) {
      sprintf(histo, "RecHitYResRing%dPanel%d", ring+1, panel+1);
      recHitYResRingPanel[ring][panel] = fs->make<TH1D>(histo, histo, 100, -200., 200.);
    }
  }
  
#ifdef LOOK_AT_ERRORS
    recHitXAlignError5 = 
      fs->make<TH1D>("RecHitXAlignError5","RecHit X  Alignment errors fpix 1", 100, 0., 100.);
    recHitXAlignError6 = 
      fs->make<TH1D>("RecHitXAlignError6","RecHit X  Alignment errors fpix 2", 100, 0., 100.);
    recHitYAlignError7 = 
      fs->make<TH1D>("RecHitYAlignError7","RecHit Y  Alignment errors fpix 3", 100, 0., 100.);
#endif

}
//----------------------------------------------------------------
void SiPixelRecHitsValid_pix::endJob() {
  // if ( outputFile_.size() != 0 && dbe_ ) {
  //   cout<<" Save file "<<endl;
  //   dbe_->save(outputFile_);
  // } else {
  //   cout<<" not saved "<<outputFile_.size()<<endl;
  // }
}


//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelRecHitsValid_pix);
