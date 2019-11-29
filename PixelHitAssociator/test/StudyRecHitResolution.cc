//
// StudyRecHitResolution.cc
// Created 6/7/06
// Make standalone, independent from the validation code. dk 3/14
// Add option for on-track hits 29/Oct/2016 Janos Karancsi
//--------------------------------

#include "StudyRecHitResolution.h"

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
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <math.h>

using namespace std;
using namespace edm;

//#define DO_SIMHITS

//---------------------------------------------------------------------------------
StudyRecHitResolution::StudyRecHitResolution(const ParameterSet& ps): 
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
//-------------------------------------------------------------------------------
StudyRecHitResolution::~StudyRecHitResolution() {
}
//------------------------------------------------------------------------------
void StudyRecHitResolution::analyze(const edm::Event& e, const edm::EventSetup& es) {
  double etaMax=2.5;

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


  //cout << " Run = " << e.id().run() << " Event = " << e.id().event() << " "<<geom->dets().size()<<" "<<hTTAC->size()<<endl;

  count1=count2=count3=count4=count5=count6=count9=0;

  if( useTracks_ && hTTAC.isValid()) { // rechits on tracks only

    const TrajTrackAssociationCollection ttac = *(hTTAC.product());
    if (verbose_) cout << "   hTTAC.isValid() " << ttac.size()<< endl;
    
    // Loop on traj-track pairs
    for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {
      count1++;

      if (verbose_) cout << "      TracjTrackAssociationCollection iterating" << endl;
      reco::TrackRef trackref = it->val;
      auto pt = trackref->pt();
      auto eta = trackref->eta();
      auto phi = trackref->phi();
      
      
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
      hptTrack->Fill(pt);
      hetaTrack->Fill(eta);
      hphiTrack->Fill(phi);

      if(pt<ptCut_ || abs(eta)>etaMax) continue; // skip tracks 
      count2++;
      if (verbose_) cout<<" track "<<pt<<" "<<eta<<" "<<trackref->d0()<<" "<<trackref->dz()<<" "
			<<trackref->vx()<<" "<<trackref->vy()<<" "<<trackref->vz() <<endl;

      for(trackingRecHit_iterator irecHit = trackref->recHitsBegin(); // loop over rechits 
	  irecHit != trackref->recHitsEnd(); ++irecHit) {
	
	//cout<<(*irecHit)->type()<<endl;
	if( (*irecHit)->type() != 0 ) continue; // skip non valid hits 
	
	DetId detId = (*irecHit)->geographicalId();
	
	if (!( (detId.subdetId() == PixelSubdetector::PixelBarrel)||  // skip non-pixel hits
	       (detId.subdetId() == PixelSubdetector::PixelEndcap) )) continue;
	
	const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTracker.idToDet(detId) );
	count3++;
	
	float distance = matchToSims(associate, (*irecHit), detId, theGeomDet,tTopo,pt,eta,phi);
	//hdist1->Fill(distance);
	if(distance<0.01) count4++;
	
	//const edm::Ref<std::vector<Trajectory> > refTraj = it->key;
	//std::vector<TrajectoryMeasurement> tmeasColl =refTraj->measurements();
	//for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = refTraj->measurements().begin(); 
	//     tmeasIt!=refTraj->measurements().end(); tmeasIt++) {   
	//  if (!tmeasIt->updatedState().isValid()) continue; 
	//  TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt->recHit();
	//  if (detId == hit->geographicalId()) matchToSims(associate, &(*hit), detId, theGeomDet,tTopo);
	//}
      } // loop rechits
    }  // loop trajectories

  } else { // all rechits 

#ifdef DO_SIMHITS


#else

    float distance=9999.;
    //cout<<distance<<endl;
    //iterate over detunits
    for (TrackerGeometry::DetContainer::const_iterator it = geom->dets().begin(); 
     	 it != geom->dets().end(); it++) {

      DetId detId = ((*it)->geographicalId());
      count1++;
      
      if (!( (detId.subdetId() == PixelSubdetector::PixelBarrel)||
       	     (detId.subdetId() == PixelSubdetector::PixelEndcap) )) continue;
      
      const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTracker.idToDet(detId) ); 
      //if(verbose_) cout<<" pixel det "<<endl;
      
      SiPixelRecHitCollection::const_iterator pixeldet = recHitColl->find(detId);
      if (pixeldet == recHitColl->end()) continue;
      count2++;
      if(verbose_) cout<<" pixel det "<<pixeldet->size()<<endl;
      distance=9999.;
      //cout<<distance<<endl;
      //----Loop over rechits for this detId

      SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixeldet->begin();
      for (; pixeliter != pixeldet->end(); pixeliter++) { 
	count3++;
       	distance = matchToSims(associate, &(*pixeliter), detId, theGeomDet,tTopo,-1.,-1.,-1.);
	if(distance<0.01) count4++;
	//hdist1->Fill(distance);
       	//cout<<distance<<endl;
      }

    } // <------ end detunit loop
#endif
    
  }  // use track?

#ifdef NOT
  hcount1->Fill(count1);
  hcount2->Fill(count2);
  hcount3->Fill(count3);
  hcount4->Fill(count4);
  hcount5->Fill(count5);
  hcount6->Fill(count6);
  hcount9->Fill(count9);
#endif

}

//--------------------------------------------------------------------------------
#ifdef PIXEL_ASSOCIATOR

float StudyRecHitResolution::matchToSims(const PixelHitAssociator& associate, const TrackingRecHit* hit, 
					   DetId detId, const PixelGeomDetUnit* theGeomDet, 
					   const TrackerTopology *tTopo,double pt, double eta, double phi) {

#else

float StudyRecHitResolution::matchToSims(const TrackerHitAssociator& associate, const TrackingRecHit* hit, 
					   DetId detId, const PixelGeomDetUnit* theGeomDet, 
					   const TrackerTopology *tTopo,double pt, double eta, double phi) {

#endif


  std::vector<PSimHit> matched = associate.associateHit(*hit); // get the matched simhits
  
  if(verbose_) 
    cout<<" rechit "<<hit->localPosition().x()<<" "<<matched.size()<<endl;
  float closest = 9999.;
  
  //if(matched.size()>1) cout<<" matched size greater than 1 "<<matched.size()<<endl;
  //hcount8->Fill(float(matched.size()));
  count9 += matched.size();

  if ( !matched.empty() ) {
    std::vector<PSimHit>::const_iterator closestit = matched.begin();
    LocalPoint lp = hit->localPosition();
    float rechit_x = lp.x();
    float rechit_y = lp.y();
    int countClose=0;
    
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
      count5++;
      if(dist<0.01) {count6++;countClose++;}

      if ( dist < closest ) {
	//closest = x_res;
	closest = dist;
	closestit = m;
	if(verbose_) 
	  std::cout<<" simhit "
		   <<(*m).pabs()<<" "
		   <<(*m).thetaAtEntry()<<" "
		   <<(*m).phiAtEntry()<<" "
		   <<(*m).particleType()<<" "<<(*m).processType()<<" "
		   <<(*m).trackId()<<" "
		   <<(*m).momentumAtEntry()<<" "
		   <<pt<<" "<<eta<<" "<<phi
		   <<std::endl;
	
      }
    } // end sim hit loop
    
    if(verbose_) cout<<" closest "<<closest<<" "<<detId.subdetId()<<endl;
    //hcount7->Fill(float(countClose));

    if (detId.subdetId() == PixelSubdetector::PixelBarrel)
      fillBarrel(hit, *closestit, detId, theGeomDet,tTopo,pt,eta,phi);
    else if (detId.subdetId() == PixelSubdetector::PixelEndcap)
      fillForward(hit, *closestit, detId, theGeomDet,tTopo,pt,eta,phi);
  } // end matched emtpy

  return closest;
}


void StudyRecHitResolution::fillBarrel(const TrackingRecHit* recHit, const PSimHit& simHit, 
				     DetId detId, const PixelGeomDetUnit* theGeomDet,
				       const TrackerTopology *tTopo, double ptT, double etaT, double phiT) {
  const float cmtomicron = 10000.0; 
  //const bool muOnly = true;
  const float PI = 3.1416;
  bool PRINT = verbose_;

  float phiH = simHit.phiAtEntry();
  float theta = simHit.thetaAtEntry(); // theta with respect to module coordinates 
  float beta = 0;  // beta is roughly like real theta
  if(theta<PI/2.) beta=(PI/2.) - theta; //  
  else            beta=theta - (PI/2.);
  float etaH = -log(tan(beta/2.));  // this is an approximation to the tracks eta, only positive

  // use track eta for histos
  float eta = abs(etaT);
  if(ptT==-1.) eta=abs(etaH); // use hit eta if tracks not used  

  float phi = abs(simHit.localDirection().phi()); // same as phi, use this 

  //float thetaLocal = simHit.localDirection().theta(); // same as theta

  if( (phi) != abs(phiH)) cout<<" phi not same "<<phi<<" "<<phiH<<endl; 

  //float pt  = simHit.momentumAtEntry().perp(); // crap, not real pt
  float p  = simHit.pabs();
  int pid = abs(simHit.particleType());
  //int process = simHit.processType();

  if(verbose_) cout<<"pid "<<pid<<" "<<eta<<endl;

  //hParticleType2->Fill(float((pid)));
  //hTrackId2->Fill(float(simHit.trackId()));
  //hProcessType2->Fill(float(process));

  // skip secondaries
  if(muOnly_ && ( (pid) != 13 || p<10.) ) 
    {if(PRINT||(pid)==13 ) cout<<" skip "<<p<<" "<<pid<<endl; return;}


  unsigned int layer = tTopo->pxbLayer(detId);
  unsigned int module = tTopo->pxbModule(detId);
  bool negZ = false;
  if(module<5) negZ=true;

  // The inner ladder (in the smaller radius) are flipped
  // unsigned int ladder = tTopo->pxbLadder(detId);
  // bool odd_lad = ladder % 2;
  // bool inner = (phase_ ? layer == 4 : layer % 2) ? !odd_lad : odd_lad;
  // Determining flipped ladders automatically using surface
  float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
  float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
  bool flipped = (tmp2<tmp1);
  if(flipped) phi = phi - (PI/2.);
  else        phi = (PI/2.) - phi;

  //std::cout<<"Flipped (Surface): "<<(tmp2<tmp1)<<" Flipped (Inner): "<<inner<<std::endl;
  if(PRINT) cout<<" layer "<<layer<<" eta "<<eta<<" phi "<<phi<<" "<<negZ<<endl;


  // skip, for tests only
  //if( abs(eta)<0.1 || abs(eta)>1.0) return; // limit 
  //if( abs(phi)<1.3 || abs(phi)>1.9) return;  // limit to l2 acceptance

  if(PRINT) std::cout<<" closest simhit "
		     <<p<<" pid "
		     <<pid<<" "
		     <<phi<<" "<<theta<<" "<<eta<<", track/process "
		     <<simHit.trackId()<<"/"<<simHit.processType()
		     <<" lay "<<layer<<" mod "<<module
		     <<" track pt/eta/phi "<<ptT<<"/"<<etaT<<"/"<<phiT
		     <<std::endl;


  LocalPoint lp = recHit->localPosition();
  float lp_y = lp.y();  
  float lp_x = lp.x();

  //GlobalPoint gp = recHit->globalPosition();
  //float gp_z = abs(gp.z());  // global position 

  LocalError lerr = recHit->localPositionError();
  float lerr_x = sqrt(lerr.xx());
  float lerr_y = sqrt(lerr.yy());
  //cout<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<endl;

  
  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  float res_x = (lp_x - sim_xpos)*cmtomicron;
  
  
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  float res_y = (lp_y - sim_ypos)*cmtomicron;
  
  //float distance = sqrt(res_x*res_x + res_y*res_y)/cmtomicron; // in cm
  //hdist3->Fill(distance);
  //if(distance<0.01) hdist4->Fill(distance);  // matched within 100um

  if(PRINT)  
    cout<<" det "<<detId.rawId()<<" "<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<" "
	<<sim_xpos<<" "<<sim_ypos<<" "<<res_x<<" "<<res_y
	<<" "<<sim_x1<<" "<<sim_x2
	<<endl;

  float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  float pull_y = ( lp_y - sim_ypos ) / lerr_y;

  float lpx = lp_x, simx=sim_xpos;
  if(flipped) {lpx=-lpx; simx=-simx;} // flip the corrdinate for flipped ladders

  recHitXResAllB->Fill(res_x);
  recHitYResAllB->Fill(res_y);
  recHitXPullAllB->Fill(pull_x);  
  recHitYPullAllB->Fill(pull_y);
  recHitXErrorAllB->Fill(lerr_x);  
  recHitYErrorAllB->Fill(lerr_y);
    
  if (flipped) {
    recHitXResFlippedLadderLayers[layer-1]->Fill(res_x);
    recHitXResFlippedLadderLayersSide[layer-1][module>4]->Fill(res_x);
  } else {
    recHitXResNonFlippedLadderLayers[layer-1]->Fill(res_x);
    recHitXResNonFlippedLadderLayersSide[layer-1][module>4]->Fill(res_x);
  } // end if

  // as a function of layer
  recHitXResLayer[layer-1]->Fill(res_x); 
  recHitYResLayer[layer-1]->Fill(res_y);

  //get cluster
  SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)recHit)->cluster();
  int sizeX = (*clust).sizeX();

  // tests only for layer 1
  if(0 && layer==1) { // switch off 

    //   if       (sizeX==1) {
    //     if     (negZ && flipped) size1_mz_f->Fill((lp_x)); 
    //     else if(negZ &&!flipped) size1_mz_nf->Fill((lp_x));
    //     else if(!negZ&& flipped) size1_pz_f->Fill((lp_x)); 
    //     else if(!negZ&&!flipped) size1_pz_nf->Fill((lp_x)); 
    //   } else if(sizeX==2) {
    //     if     (negZ && flipped) size2_mz_f->Fill((lp_x)); 
    //     else if(negZ &&!flipped) size2_mz_nf->Fill((lp_x));
    //     else if(!negZ&& flipped) size2_pz_f->Fill((lp_x)); 
    //     else if(!negZ&&!flipped) size2_pz_nf->Fill((lp_x)); 
    //   } else if(sizeX==3) {
    //     if     (negZ && flipped) size3_mz_f->Fill((lp_x)); 
    //     else if(negZ &&!flipped) size3_mz_nf->Fill((lp_x));
    //     else if(!negZ&& flipped) size3_pz_f->Fill((lp_x)); 
    //     else if(!negZ&&!flipped) size3_pz_nf->Fill((lp_x)); 
    //   }
    
    cluSizeXVsPhi1->Fill((lpx),float(sizeX)); // all size 
    //if     (negZ && flipped) clusizeX21VsPhi->Fill((lp_x),float(sizeX)); // all size 
    //else if(negZ &&!flipped) clusizeX22VsPhi->Fill((lp_x),float(sizeX)); // all size 
    //else if(!negZ&& flipped) clusizeX23VsPhi->Fill((lp_x),float(sizeX)); // all size 
    //else if(!negZ&&!flipped) clusizeX24VsPhi->Fill((lp_x),float(sizeX)); // all size 
    
    //} else if(layer==2) {
  } // layer 1 only 

  // eta plots 
  recHitXResLayerP[layer-1]->Fill(eta,std::abs(res_x)); // all size 
  recHitYResLayerP[layer-1]->Fill(eta,std::abs(res_y));

  // phi plots 
  recHitXResVsPhiP[layer-1]->Fill((lpx),std::abs(res_x)); // all size 
  clusizeXVsX[layer-1]->Fill((lpx),float(sizeX)); // profile size vs x, all size 
  clusizeXVsPhi[layer-1]->Fill(lpx); // 1D x, all size 

  if(sizeX==1) {   // size 1
    
    recHitXResLayerP1[layer-1]->Fill(eta,std::abs(res_x)); 
    recHitXResVsPhiP1[layer-1]->Fill((lpx),std::abs(res_x));  
    clusizeX1VsPhi[layer-1]->Fill((lpx));  
    if     (layer==1) recHitL1XResSize1->Fill(res_x);  // layer 1
    else if(layer==2) recHitL2XResSize1->Fill(res_x);
    else if(layer==3) recHitL3XResSize1->Fill(res_x);
    else if(layer==4) recHitL4XResSize1->Fill(res_x);

  } else if(sizeX==2) {
    recHitXResLayerP2[layer-1]->Fill(eta,std::abs(res_x));
    recHitXResVsPhiP2[layer-1]->Fill((lpx),std::abs(res_x));  
    clusizeX2VsPhi[layer-1]->Fill((lpx));  
    if     (layer==1) recHitL1XResSize2->Fill(res_x);
    else if(layer==2) recHitL2XResSize2->Fill(res_x);
    else if(layer==3) recHitL3XResSize2->Fill(res_x);
    else if(layer==4) recHitL4XResSize2->Fill(res_x);

  } else if(sizeX>=3) {
    recHitXResLayerP3[layer-1]->Fill(eta,std::abs(res_x));
    recHitXResVsPhiP3[layer-1]->Fill((lpx),std::abs(res_x));  
    clusizeX3VsPhi[layer-1]->Fill((lpx));  
    if     (layer==1) recHitL1XResSize3->Fill(res_x);
    else if(layer==2) recHitL2XResSize3->Fill(res_x);
    else if(layer==3) recHitL3XResSize3->Fill(res_x);
    else if(layer==4) recHitL4XResSize3->Fill(res_x);
  }
  
  int sizeY = (*clust).sizeY();
  if(sizeY==1)      recHitYResLayerP1[layer-1]->Fill(eta,std::abs(res_y)); // size 1
  else if(sizeY==2) recHitYResLayerP2[layer-1]->Fill(eta,std::abs(res_y));
  else if(sizeY==3) recHitYResLayerP3[layer-1]->Fill(eta,std::abs(res_y));
  else if(sizeY>=4) recHitYResLayerP4[layer-1]->Fill(eta,std::abs(res_y));
  
  // as a function of eta
  for (unsigned int i=0; i<25; i++) {
    float eta1=float(i)*0.1;
    float eta2=eta1+0.1;
    if(eta>=eta1 && eta<eta2) {
      if      (layer == 1) {recHitXResLayer1Eta[i]->Fill(res_x);recHitYResLayer1Eta[i]->Fill(res_y);}
      else if (layer == 2) {recHitXResLayer2Eta[i]->Fill(res_x);recHitYResLayer2Eta[i]->Fill(res_y);}
      else if (layer == 3) {recHitXResLayer3Eta[i]->Fill(res_x);recHitYResLayer3Eta[i]->Fill(res_y);}
      else if (layer == 4) {recHitXResLayer4Eta[i]->Fill(res_x);recHitYResLayer4Eta[i]->Fill(res_y);}
    } // eta 
  } // i (eta)
  
  // fill module dependent info, z dependence 
  for (unsigned int i=0; i<8; i++) {
    if (tTopo->pxbModule(detId) == i+1) {      
      if (layer == 1) {
	recHitYResLayer1Modules[i]->Fill(res_y);
      } else if (layer == 2)  {
	recHitYResLayer2Modules[i]->Fill(res_y);
      } else if (layer == 3) {
	recHitYResLayer3Modules[i]->Fill(res_y);
      } else if (layer == 4) {
	recHitYResLayer4Modules[i]->Fill(res_y);
      } // end if
    } // end if
  } // end for

}
//-------------------------------------------------------------------------
int StudyRecHitResolution::PhaseIBladeOfflineToOnline(const int& blade) {
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
void StudyRecHitResolution::fillForward(const TrackingRecHit* recHit, const PSimHit & simHit, 
				      DetId detId,const PixelGeomDetUnit * theGeomDet,
				      const TrackerTopology *tTopo, double ptT, double etaT, double phiT) {
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
  recHitXErrorAllF->Fill(lerr_x);
  recHitYErrorAllF->Fill(lerr_y);
  recHitXPullAllF->Fill(pull_x);
  recHitYPullAllF->Fill(pull_y);

#ifdef NOT // skip for the moment 

  // spliy into disks etc.
  int disk = tTopo->pxfDisk(detId);
  int panel = tTopo->pxfPanel(detId);      // Phase 1: Forward 1, Backward 2
  int side = tTopo->pxfSide(detId);
  //int module = tTopo->pxfModule(detId);  // Phase 1: Always 1
  // Phase 1 specific
  int blade  = tTopo->pxfBlade(detId);     // Phase 1: Inner blades 1-22, Outer blades 23-56
  int ring = 1 + (blade>22);               // Phase 1: Inner: 1, Outer: 2
  //int phase1_online_blade = PhaseIBladeOfflineToOnline(blade); // Phase 1: Ring1 +-1-11, Ring2 +-1-17
  //int xside = 1 + (phase1_online_blade>0); // Phase 1: -X 1, +X 2


  recHitXResRingSidePanel[ring-1][side-1][panel-1]->Fill(res_x);
  recHitYResRingSidePanel[ring-1][side-1][panel-1]->Fill(res_y);
  recHitXResRingSide[ring-1][side-1]->Fill(res_x);
  recHitYResRingPanel[ring-1][panel-1]->Fill(res_y);

  if (side==1) {
    recHitXResNegZF->Fill(res_x);
    recHitYResNegZF->Fill(res_y);
  } else if (side==2) {
    recHitXResPosZF->Fill(res_x);
    recHitYResPosZF->Fill(res_y);  
  }

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
    
#endif  // NOT 

}


void StudyRecHitResolution::endJob() {

}


void StudyRecHitResolution::beginJob() {
  
  edm::Service<TFileService> fs;

  // BPIX 
  Char_t histo[200], title[200];

  // ---------------------------------------------------------------
  // All histograms that depend on plaquette number have 7 indexes.
  // The first 4 (0-3) correspond to Panel 1 plaquettes 1-4.
  // The last 3 (4-6) correspond to Panel 2 plaquettes 1-3.
  // ---------------------------------------------------------------

  hphiTrack = fs->make<TH1D>("hphiTrack","track phi",70,-3.5,3.5);
  hetaTrack = fs->make<TH1D>("hetaTrack","track eta",50,-2.5,2.5);
  hptTrack  = fs->make<TH1D>("hptTrack","track pt",150,0.0,150.);

  // BPix 
  recHitXResAllB = fs->make<TH1D>("recHitXResAllB","RecHit X Res All Modules in Barrel", 100, -200., 200.);
  recHitYResAllB = fs->make<TH1D>("recHitYResAllB","RecHit Y Res All Modules in Barrel", 100, -200., 200.);
  recHitXPullAllB = fs->make<TH1D>("recHitXPullAllB", 
				   "RecHit X Pull All Modules in Barrel"        , 100, -10.0, 10.0);
  recHitYPullAllB = fs->make<TH1D>("recHitYPullAllB", 
				   "RecHit Y Pull All Modules in Barrel"        , 100, -10.0, 10.0);
  recHitXErrorAllB = fs->make<TH1D>("recHitXErrorAllB", "RecHit X Error All in Barrel", 100, 0.0, 0.1);  
  recHitYErrorAllB = fs->make<TH1D>("recHitYErrorAllB", "RecHit Y Error All in Barrel", 100, 0.0, 0.1);


  // For each layer 
  for (int i=0; i<4; i++) {
    //RecHit X resolution per layer
    sprintf(histo, "recHitXResLayer%d", i+1);
    sprintf(title, "RecHit XRes Layer %d", i+1);
    recHitXResLayer[i] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    
    //RecHit Y resolution per layer
    sprintf(histo, "recHitYResLayer%d", i+1);
    sprintf(title, "RecHit YRes Layer %d", i+1);
    recHitYResLayer[i] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    
    //RecHit X resolution per layer Profile vs eta
    sprintf(histo, "recHitXResLayerP%d", i+1);
    sprintf(title, "RecHit XRes Layer %d", i+1);
    recHitXResLayerP[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitXResLayerP1%d", i+1);
    sprintf(title, "RecHit XRes Layer %d, size 1", i+1);
    recHitXResLayerP1[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitXResLayerP2%d", i+1);
    sprintf(title, "RecHit XRes Layer %d, size 2", i+1);
    recHitXResLayerP2[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    sprintf(title, "recHitXResLayer %d, size 3", i+1);
    sprintf(histo, "RecHit_XRes_LayerP3%d", i+1);
    recHitXResLayerP3[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    
    //RecHit Y resolution per layer Profile vs eta
    sprintf(histo, "recHitYResLayerP%d", i+1);
    sprintf(title, "RecHit YRes Layer %d", i+1);
    recHitYResLayerP[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP1%d", i+1);
    sprintf(title, "RecHit YRes Layer %d, size 1", i+1);
    recHitYResLayerP1[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP2%d", i+1);
    sprintf(title, "RecHit YRes Layer %d, size 2", i+1);
    recHitYResLayerP2[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP3%d", i+1);
    sprintf(title, "RecHit YRes Layer %d, size 3", i+1);
    recHitYResLayerP3[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    sprintf(histo, "recHitYResLayerP4%d", i+1);
    sprintf(title, "RecHit YRes Layer %d, size 4", i+1);
    recHitYResLayerP4[i] = fs->make<TProfile>(histo,title, 25, 0., 2.5,0.,1000.," ");
    
    //RecHit X resolution per layer Profile vs phi 
    sprintf(histo, "recHitXResVsPhi_%dP", i+1);
    sprintf(title, "RecHit XRes Vs Phi Layer %d", i+1);
    recHitXResVsPhiP[i] = fs->make<TProfile>(histo,title, 162, -0.81,0.81,0.,100.," ");
    sprintf(histo, "recHitXResVsPhi_%dP1", i+1);
    sprintf(title, "RecHit XRes Vs Phi Layer %d size 1", i+1);
    recHitXResVsPhiP1[i] = fs->make<TProfile>(histo,title, 162, -0.81,0.81,0.,100.," ");
    sprintf(histo, "recHitXResVsPhi_%dP2", i+1);
    sprintf(title, "RecHit XRes Vs Phi Layer %d size 2", i+1);
    recHitXResVsPhiP2[i] = fs->make<TProfile>(histo,title, 162, -0.81,0.81,0.,100.," ");
    sprintf(histo, "recHitXResVsPhi_%dP3", i+1);
    sprintf(title, "RecHit XRes Vs Phi Layer %d size 3", i+1);
    recHitXResVsPhiP3[i] = fs->make<TProfile>(histo,title, 162, -0.81,0.81,0.,100.," ");
    
    //RecHit X resolution for flipped ladders by layer
    sprintf(histo, "recHit_XRes_FlippedLadder_Layer%d", i+1);
    sprintf(title, "RecHit XRes FlippedLadder Layer%d", i+1);
    recHitXResFlippedLadderLayers[i] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    
    //RecHit X resolution for unflipped ladders by layer
    sprintf(histo, "recHitXResNonFlippedLadderLayer%d", i+1);
    sprintf(title, "RecHit XRes NonFlippedLadder Layer%d", i+1);
    recHitXResNonFlippedLadderLayers[i] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    // Same plots for -Z/+Z
    sprintf(histo, "recHitXResFlippedLadderLayer%d_mZ", i+1);
    sprintf(title, "RecHit XRes FlippedLadder Layer%d -Z", i+1);
    recHitXResFlippedLadderLayersSide[i][0] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    sprintf(histo, "recHitXResNonFlippedLadderLayer%d_mZ", i+1);
    sprintf(title, "RecHit XRes NonFlippedLadder Layer%d -Z", i+1);
    recHitXResNonFlippedLadderLayersSide[i][0] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    sprintf(histo, "recHitXResFlippedLadder_Layer%d_pZ", i+1);
    sprintf(title, "RecHit XRes FlippedLadderLayer%d +Z", i+1);
    recHitXResFlippedLadderLayersSide[i][1] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    sprintf(histo, "recHitXResNonFlippedLadderLayer%d_pZ", i+1);
    sprintf(title, "RecHit XRes NonFlippedLadder Layer%d +Z", i+1);
    recHitXResNonFlippedLadderLayersSide[i][1] = fs->make<TH1D>(histo,title, 100, -200., 200.);
    
    //RecHit Y resolutions for layers by module for barrel
    for (int ii=0; ii<8; ii++) {
      //Rec Hit Y resolution by module for Layer1
      sprintf(histo, "recHitYResLayer%dModule%d",i+1, ii+1);
      sprintf(title, "RecHit YRes Layer%d z-Module%d",i+1, ii+1);
      if     (i==0) recHitYResLayer1Modules[ii] = fs->make<TH1D>(histo,title, 100, -200., 200.);
      else if(i==1) recHitYResLayer2Modules[ii] = fs->make<TH1D>(histo,title, 100, -200., 200.);
      else if(i==2) recHitYResLayer3Modules[ii] = fs->make<TH1D>(histo,title, 100, -200., 200.);
      else if(i==3) recHitYResLayer4Modules[ii] = fs->make<TH1D>(histo,title, 100, -200., 200.);	  
    } // end for ii, modules 
    
    // size versus phi
    sprintf(histo, "clusizeXVsX%d", i+1);
    sprintf(title, "Clu sizeX Vs X Layer %d", i+1);
    clusizeXVsX[i] = fs->make<TProfile>(histo,title, 162, -0.81,0.81,0.,100.," ");
    
    sprintf(histo, "clusizeXVsPhi%d", i+1);
    sprintf(title, "clu sizeX Vs Phi Layer %d", i+1);
    clusizeXVsPhi[i] = fs->make<TH1D>(histo,title, 162, -0.81,0.81);  // alll sizes 
    
    sprintf(histo, "clusizeX1VsPhi%d", i+1);
    sprintf(title, "clu sizeX Vs Phi Layer %d size 1", i+1);
    clusizeX1VsPhi[i] = fs->make<TH1D>(histo,title, 162, -0.81,0.81); // size 1
    sprintf(histo, "clusizeX2VsPhi%d", i+1);
    sprintf(title, "clu sizeX Vs Phi Layer %d size 2", i+1);
    clusizeX2VsPhi[i] = fs->make<TH1D>(histo,title, 162, -0.81,0.81);  // size 2
    sprintf(histo, "clusizeX3VsPhi%d", i+1);
    sprintf(title, "clu sizeX Vs Phi Layer %d size 3", i+1);
    clusizeX3VsPhi[i] = fs->make<TH1D>(histo,title, 162, -0.81,0.81);  //size 3
    
  } // end i, layers
  
  //Only for layer 1
  if(0) {  // switch off
      cluSizeXVsPhi1 = fs->make<TProfile>("cluSizeXVsPhi1","clus sizeX vs phi layer1",1620,-0.81,0.81,0.,100.," ");
      
      // size1_mz_f = fs->make<TH1D>("size1_mz_f", "size1 -z flip",162,-0.81,0.81);
      // size1_mz_nf= fs->make<TH1D>("size1_mz_nf","size1 -z nfli",162,-0.81,0.81);
      // size1_pz_f = fs->make<TH1D>("size1_pz_f", "size1 +z flip",162,-0.81,0.81);
      // size1_pz_nf= fs->make<TH1D>("size1_pz_nf","size1 +z nfli",162,-0.81,0.81);
      // size2_mz_f = fs->make<TH1D>("size2_mz_f", "size2 -z flip",162,-0.81,0.81);
      // size2_mz_nf= fs->make<TH1D>("size2_mz_nf","size2 -z nfli",162,-0.81,0.81);
      // size2_pz_f = fs->make<TH1D>("size2_pz_f", "size2 +z flip",162,-0.81,0.81);
      // size2_pz_nf= fs->make<TH1D>("size2_pz_nf","size2 +z nfli",162,-0.81,0.81);
      // size3_mz_f = fs->make<TH1D>("size3_mz_f", "size3 -z flip",162,-0.81,0.81);
      // size3_mz_nf= fs->make<TH1D>("size3_mz_nf","size3 -z nfli",162,-0.81,0.81);
      // size3_pz_f = fs->make<TH1D>("size3_pz_f", "size3 +z flip",162,-0.81,0.81);
      // size3_pz_nf= fs->make<TH1D>("size3_pz_nf","size3 +z nfli",162,-0.81,0.81);
  }

  // hz1 = fs->make<TH1D>("hz1","gz layer 1",26,0.,26.);
  // hz1_1 = fs->make<TH1D>("hz1_1","gz pid group1",26,0.,26.);
  // hz1_2 = fs->make<TH1D>("hz1_2","gz pid group2",26,0.,26.);
  // hz1_3 = fs->make<TH1D>("hz1_3","gz pid group3",26,0.,26.);
  // hz1_4 = fs->make<TH1D>("hz1_4","gz pid group4",26,0.,26.);
  // hz1_5 = fs->make<TH1D>("hz1_5","eta pid group5",26,0.,26.);

  // hz1_11 = fs->make<TH1D>("hz1_11","gz process group1",26,0.,26.);
  // hz1_12 = fs->make<TH1D>("hz1_12","gz process group2",26,0.,26.);
  // hz1_13 = fs->make<TH1D>("hz1_13","gz process group3",26,0.,26.);
  // hz1_14 = fs->make<TH1D>("hz1_14","gz process group4",26,0.,26.);
  // hz1_15 = fs->make<TH1D>("hz1_15","gz process group5",26,0.,26.);
  // hz1_16 = fs->make<TH1D>("hz1_16","gz process group6",26,0.,26.);
  // hz1_17 = fs->make<TH1D>("hz1_17","gz process group7",26,0.,26.);
    
  // hcount1 = fs->make<TH1D>("hcount1","tracks (or detunits)",2000,-0.5,1999.5);
  // hcount2 = fs->make<TH1D>("hcount2","selected tracks",2000,-0.5,1999.5);
  // hcount3 = fs->make<TH1D>("hcount3","rechits",2000,-0.5,1999.5);
  // hcount4 = fs->make<TH1D>("hcount4","rechits <100um",2000,-0.5,1999.5);
  // hcount5 = fs->make<TH1D>("hcount5","simhits compared",2000,-0.5,1999.5);
  // hcount6 = fs->make<TH1D>("hcount6","simhits <100um",2000,-0.5,1999.5);
  // hcount7 = fs->make<TH1D>("hcount7","close simhits per rechit",20,-0.5,19.5);
  // hcount8 = fs->make<TH1D>("hcount8","matched simhits per rechit",20,-0.5,19.5);
  // hcount9 = fs->make<TH1D>("hcount9","all simhits",2000,-0.5,1995.5);
  
  // hdist1 = fs->make<TH1D>("hdist1","matched",100,0.,0.1);
  // hdist2 = fs->make<TH1D>("hdist2","number of bpix matched rechits",100,0.,0.1);
  // hdist3 = fs->make<TH1D>("hdist3","selected matched",100,0.,0.1);
  // hdist4 = fs->make<TH1D>("hdist4","selected match, close",100,0.,0.1);

  // hParticleType1 = fs->make<TH1D>("hParticleType1","partcile type close",2500,0.,2500.);
  // hTrackId1 = fs->make<TH1D>("hTrackId1","trackid close",1000,0.,1000.);
  // hProcessType1 = fs->make<TH1D>("hProcessType1","process type close",500,0.,500.);
  // hParticleType2 = fs->make<TH1D>("hParticleType2","partcile type best",2500,0.,2500.);
  // hTrackId2 = fs->make<TH1D>("hTrackId2","trackid best",1000,0.,1000.);
  // hProcessType2 = fs->make<TH1D>("hProcessType2","process type best",500,0.,500.);

      // special histos for layer 1
  recHitL1XResSize1    = fs->make<TH1D>("recHitL1XSize1","XRes size 1 L1", 100, -200., 200.);   
  recHitL1XResSize2    = fs->make<TH1D>("recHitL1XSize2","XRes size 2 L1", 100, -200., 200.);   
  recHitL1XResSize3    = fs->make<TH1D>("recHitL1XSize3","XRes size 3 L1", 100, -200., 200.);   
  recHitL2XResSize1    = fs->make<TH1D>("recHitL2XSize1","XRes size 1 L2", 100, -200., 200.);   
  recHitL2XResSize2    = fs->make<TH1D>("recHitL2XSize2","XRes size 2 L2", 100, -200., 200.);   
  recHitL2XResSize3    = fs->make<TH1D>("recHitL2XSize3","XRes size 3 L2", 100, -200., 200.);   
  recHitL3XResSize1    = fs->make<TH1D>("recHitL3XSize1","XRes size 1 L3", 100, -200., 200.);   
  recHitL3XResSize2    = fs->make<TH1D>("recHitL3XSize2","XRes size 2 L3", 100, -200., 200.);   
  recHitL3XResSize3    = fs->make<TH1D>("recHitL3XSize3","XRes size 3 L3", 100, -200., 200.);   
  recHitL4XResSize1    = fs->make<TH1D>("recHitL4XSize1","XRes size 1 L4", 100, -200., 200.);   
  recHitL4XResSize2    = fs->make<TH1D>("recHitL4XSize2","XRes size 2 L4", 100, -200., 200.);   
  recHitL4XResSize3    = fs->make<TH1D>("recHitL4XSize3","XRes size 3 L4", 100, -200., 200.);   
  
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
  }  // j, eta
  
#ifdef NOT
    recHitXPullAllB = fs->make<TH1D>("recHitXPullAllB"       , 
				   "RecHit X Pull All Modules in Barrel"        , 100, -10.0, 10.0);
    recHitYPullAllB = fs->make<TH1D>("recHitYPullAllB"       , 
				   "RecHit Y Pull All Modules in Barrel"        , 100, -10.0, 10.0);
    //recHitXErrorAllB = fs->make<TH1D>("RecHit_xerr_b_All"       , 
    //				 "RecHit X Error All Modules in Barrel"        , 100, 0, 0.1);
    //recHitYErrorAllB = fs->make<TH1D>("RecHit_yerr_b_All"       , 
    //				 "RecHit Y Error All Modules in Barrel"        , 100, 0, 0.1);

    recHitXError1B = fs->make<TH1D>("recHitXError1B"       , 
				  "RecHit X Error in Barrel Layer 1"        , 100, 0, 0.1);
    recHitYError1B = fs->make<TH1D>("recHitYError1B"       , 
				  "RecHit Y Error in Barrel Layer 1"        , 100, 0, 0.1);
    recHitXError2B = fs->make<TH1D>("recHitXError2B"       , 
				  "RecHit X Error in Barrel Layer 2"        , 100, 0, 0.1);
    recHitYError2B = fs->make<TH1D>("recHitYError2B"       , 
				  "RecHit Y Error in Barrel Layer 2"        , 100, 0, 0.1);
    recHitXError3B = fs->make<TH1D>("recHitXError3B"       , 
				  "RecHit X Error in Barrel Layer 3"        , 100, 0, 0.1);
    recHitYError3B = fs->make<TH1D>("recHitYError3B"       , 
				  "RecHit Y Error in Barrel Layer 3"        , 100, 0, 0.1);
    recHitXError4B = fs->make<TH1D>("recHitXError4B"       , 
				  "RecHit X Error in Barrel Layer 4"        , 100, 0, 0.1);
    recHitYError4B = fs->make<TH1D>("recHitYError4B"       , 
				  "RecHit Y Error in Barrel Layer 4"        , 100, 0, 0.1);
#endif
    

    // FPix 
    //RecHit X resolution all plaquettes
    recHitXResAllF = fs->make<TH1D>("recHitXResAllF", "RecHit X Res All in Forward", 100, -200., 200.);
    recHitXResPosZF = fs->make<TH1D>("recHitXResPosZF", "RecHit X Res All in Forward Disks +Z", 100, -200., 200.);
    recHitXResNegZF = fs->make<TH1D>("recHitXResNegZF", "RecHit X Res All in Forward Disks -Z", 100, -200., 200.);
    
    //RecHit Y resolution all plaquettes
    recHitYResAllF = fs->make<TH1D>("recHitYResAllF", "RecHit Y Res All in Forward", 100, -200., 200.);
    recHitYResPosZF = fs->make<TH1D>("recHitYResPosZF", "RecHit Y Res All in Forward Disks +Z", 100, -200., 200.);
    recHitYResNegZF = fs->make<TH1D>("recHitYResNegZF", "RecHit Y Res All in Forward Disks -Z", 100, -200., 200.);

    recHitXPullAllF = fs->make<TH1D>("recHitXPullAllF", "RecHit X Pull All in Forward", 100, -10.0, 10.0);  
    recHitYPullAllF = fs->make<TH1D>("recHitYPullAllF", "RecHit Y Pull All in Forward", 100, -10.0, 10.0);
    recHitXErrorAllF = fs->make<TH1D>("recHitXErrorAllF", "RecHit X Error All in Forward", 100, 0.0, 0.1);  
    recHitYErrorAllF = fs->make<TH1D>("recHitYErrorAllF", "RecHit Y Error All in Forward", 100, 0.0, 0.1);

#ifdef NOT

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
      for (int side=0; side<2; side++) for (int panel=0; panel<2; panel++) {
	sprintf(histo, "recHitXResRing%dSide%d_Panel%d", ring+1, side+1, panel+1);
	recHitXResRingSidePanel[ring][side][panel] = fs->make<TH1D>(histo, histo, 100, -200., 200.); 
	sprintf(histo, "recHitYResRing%dSide%d_Panel%d", ring+1, side+1, panel+1);
	recHitYResRingSidePanel[ring][side][panel] = fs->make<TH1D>(histo, histo, 100, -200., 200.);
      }
      for (int side=0; side<2; side++) {
	sprintf(histo, "recHitXResRing%dSide%d", ring+1, side+1);
	recHitXResRingSide[ring][side] = fs->make<TH1D>(histo, histo, 100, -200., 200.); 
      }
      for (int panel=0; panel<2; panel++) {
	sprintf(histo, "recHitYResRing%dPanel%d", ring+1, panel+1);
	recHitYResRingPanel[ring][panel] = fs->make<TH1D>(histo, histo, 100, -200., 200.);
      }
    }

  //RecHit X distribution for plaquette with x-size 1 in forward
  // recHitXPlaquetteSize1 = fs->make<TH1D>("RecHit_x_Plaquette_xsize1", "RecHit X Distribution for plaquette x-size1", 100, -2., 2.);
  
  // //RecHit X distribution for plaquette with x-size 2 in forward
  // recHitXPlaquetteSize2 = fs->make<TH1D>("RecHit_x_Plaquette_xsize2", "RecHit X Distribution for plaquette x-size2", 100, -2., 2.);
  
  // //RecHit Y distribution for plaquette with y-size 2 in forward
  // recHitYPlaquetteSize2 = fs->make<TH1D>("RecHit_y_Plaquette_ysize2", "RecHit Y Distribution for plaquette y-size2", 100, -4., 4.);
  
  // //RecHit Y distribution for plaquette with y-size 3 in forward
  // recHitYPlaquetteSize3 = fs->make<TH1D>("RecHit_y_Plaquette_ysize3", "RecHit Y Distribution for plaquette y-size3", 100, -4., 4.);
  
  // //RecHit Y distribution for plaquette with y-size 4 in forward
  // recHitYPlaquetteSize4 = fs->make<TH1D>("RecHit_y_Plaquette_ysize4", "RecHit Y Distribution for plaquette y-size4", 100, -4., 4.);
  
  // //RecHit Y distribution for plaquette with y-size 5 in forward
  // recHitYPlaquetteSize5 = fs->make<TH1D>("RecHit_y_Plaquette_ysize5", "RecHit Y Distribution for plaquette y-size5", 100, -4., 4.);
  
  //X and Y resolutions for both disks by plaquette in forward

    
    recHitXPullAllF = fs->make<TH1D>("recHitXPullAllF", "RecHit X Pull All in Forward", 100, -10.0, 10.0);  
    recHitYPullAllF = fs->make<TH1D>("recHitYPullAllF", "RecHit Y Pull All in Forward", 100, -10.0, 10.0);
    recHitXErrorAllF = fs->make<TH1D>("recHitXErrorAllF", "RecHit X Error All in Forward", 100, 0.0, 0.1);  
    recHitYErrorAllF = fs->make<TH1D>("recHitYErrorAllF", "RecHit Y Error All in Forward", 100, 0.0, 0.1);
    
    for (int i=0; i<7; i++) {
      sprintf(histo, "recHitXPullDisk1Plaquette%d", i+1);
      recHitXPullDisk1Plaquettes[i] = fs->make<TH1D>(histo, "RecHit XPull Disk1 by plaquette", 100, -10.0, 10.0); 
      sprintf(histo, "recHitXPullDisk2Plaquette%d", i+1);
      recHitXPullDisk2Plaquettes[i] = fs->make<TH1D>(histo, "RecHit XPull Disk2 by plaquette", 100, -10.0, 10.0);  
      sprintf(histo, "recHitXPullDisk3Plaquette%d", i+1);
      recHitXPullDisk3Plaquettes[i] = fs->make<TH1D>(histo, "RecHit XPull Disk3 by plaquette", 100, -10.0, 10.0);  
      
      sprintf(histo, "recHitYPullDisk1Plaquette%d", i+1);
      recHitYPullDisk1Plaquettes[i] = fs->make<TH1D>(histo, "RecHit YPull Disk1 by plaquette", 100, -10.0, 10.0);      
      sprintf(histo, "recHitYPullDisk2Plaquette%d", i+1);
      recHitYPullDisk2Plaquettes[i] = fs->make<TH1D>(histo, "RecHit YPull Disk2 by plaquette", 100, -10.0, 10.0);
      sprintf(histo, "recHitYPullDisk3Plaquette%d", i+1);
      recHitYPullDisk3Plaquettes[i] = fs->make<TH1D>(histo, "RecHit YPull Disk3 by plaquette", 100, -10.0, 10.0);
    }
    
#endif //NOT
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(StudyRecHitResolution);
