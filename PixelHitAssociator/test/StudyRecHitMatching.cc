//
// StudyRecHitMatching.cc
// Created 6/7/06
// Make standalone, independent from the validation code. dk 3/14
// Add option for on-track hits 29/Oct/2016 Janos Karancsi
// Options:
// useSimHits_ = True - loop over all simHits, match to recHits
// useTracks_ = True - loop over tracks, get RecHits on a track, match to SimHits.
// both  = False - loop over all RecHits (from the RecHit container) and match to SimHits. 
//--------------------------------

#include "StudyRecHitMatching.h"

//#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
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

namespace {
  const float CloseMatch=0.0075; // 75um in cm
  const float Matched   =0.0175; // 175um

}
StudyRecHitMatching::StudyRecHitMatching(const ParameterSet& ps): 
  conf_(ps),
#ifdef PIXEL_ASSOCIATOR
#else
  trackerHitAssociatorConfig_(ps, consumesCollector() ),
#endif
  src_( ps.getParameter<edm::InputTag>( "src" ) ),
  useTracks_( ps.getUntrackedParameter<bool>( "useTracks", false ) ),
  tracks_( ps.getUntrackedParameter<edm::InputTag>( "tracks", edm::InputTag("generalTracks") ) )  {

  // Look over simhits as a starting point 
  useSimHits_= true; // for the moment just hardwire it 
  if(useSimHits_) useTracks_=false;

  if (useTracks_) {
    tTracks = consumes<TrajTrackAssociationCollection>( tracks_ );
  } else {
    tPixelRecHit = consumes<edmNew::DetSetVector<SiPixelRecHit>>( src_ );
  }

  tPixelSimHits = 
    consumes <PSimHitContainer> (edm::InputTag("g4SimHits","TrackerHitsPixelBarrelLowTof"));

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
//----------------------------------------------------------------------------------
StudyRecHitMatching::~StudyRecHitMatching() {
}

//----------------------------------------------------------------------------------
void StudyRecHitMatching::analyze(const edm::Event& e, const edm::EventSetup& es) {
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
  
  // Get simhits 
  Handle<PSimHitContainer> PixelSimHits;
  //Handle<PSimHitContainer> PixelHitsLowTof;
  //Handle<PSimHitContainer> PixelHitsHighTof;
  e.getByToken( tPixelSimHits ,PixelSimHits);
  const vector<PSimHit>& ism(*PixelSimHits); 

  //Get event setup
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get(geom); 
  const TrackerGeometry& theTracker(*geom);
  
  if(verbose_) cout<<" Call associator "<<endl;
#ifdef PIXEL_ASSOCIATOR
  //PixelHitAssociator associate( e); 
  ////PixelHitAssociator associate( e, conf_ ); 
#else
  TrackerHitAssociator associate( e, trackerHitAssociatorConfig_); 
#endif

  count1=count2=count3=count4=count5=count6=count9=count10=0;

  // There are 3 options 
  // Option 1 - loop over simhits, match to rec hits
  if(useSimHits_) {  // The SimHit loop

    const bool selectL1= true; // select only L1 simhits for the simHit loop
    int countSimHits=0;
    for(vector<PSimHit>::const_iterator isim = PixelSimHits->begin();
	isim != PixelSimHits->end(); ++isim) {
      
      // Det id
      DetId detId=DetId((*isim).detUnitId());
      unsigned int dettype=detId.det(); // for tracker=1
      unsigned int subid=detId.subdetId();// pixel=1
      unsigned int detid=detId.rawId(); // raw det id
      if(dettype!=1 && subid!=1) cout<<" error in det id "<<dettype<<" "<<subid<<endl;

      // Global variables 
      const PixelGeomDetUnit * theGeomDet = 
	dynamic_cast<const PixelGeomDetUnit*> ( theTracker.idToDet(detId) );
      //double detZ = theGeomDet->surface().position().z();    // module z position 
      //double detR = theGeomDet->surface().position().perp(); //        r
      //double detPhi = theGeomDet->surface().position().phi();//        phi	

      if(selectL1) {
	unsigned layerC=tTopo->pxbLayer(detid);
	if(layerC!=1) continue; // keep only bpix layer 1
      }
      count1++;

     // SimHit information 
     float eloss = (*isim).energyLoss() * 1000000/3.7;//convert GeV to ke 
     float tof = (*isim).timeOfFlight();
     float p = (*isim).pabs();
     float pt= (*isim).momentumAtEntry().perp();
     float theta = (*isim).thetaAtEntry();
     float phi = (*isim).phiAtEntry();
     int pid = ((*isim).particleType()); 
     int tid = (*isim).trackId();
     int procType = (*isim).processType(); // production mechanism, 0 for primary 

     if(procType!=0) continue; // skip non primary simhits 
     countSimHits++;
     
     float x = (*isim).entryPoint().x(); // width (row index, in col direction)
     float y = (*isim).entryPoint().y(); // length (col index, in row direction)
     float z = (*isim).entryPoint().z(); // thickness, + or -142.5um
     
     float x2 = (*isim).exitPoint().x();
     float y2 = (*isim).exitPoint().y();
     float z2 = (*isim).exitPoint().z();  //+- 142.5

     //float dz = abs(z-z2); // should be the sensor thickness 285um
     //bool moduleDirectionUp = ( z < z2 ); // for positive direction z2>z

     float xpos = (x+x2)/2.;
     float ypos = (y+y2)/2.;
     float zpos = (z+z2)/2.; // should be z=0

     // Select certain type of simHits 

     if(verbose_) cout<<"Simhit "<<countSimHits<<" module "<<detid<<" with simhit eloss= "<<eloss<<" tof "<<tof<<" p "
	 <<p<<" pid "<<pid<<" "<<pt<<" "<<phi<<" "<<theta<<", track/process "
	 <<tid<<"/"<<procType<<" pos "<<xpos<<" "<<ypos<<" "<<zpos
	 <<std::endl;


     // get rechits for this module 
     SiPixelRecHitCollection::const_iterator pixeldet = recHitColl->find(detId);
     if (pixeldet == recHitColl->end()) continue;


     if(verbose_) cout<<" pixel det, rechit size "<<pixeldet->size()<<endl;
     float distance0=9999.;
     float distance = matchToRecs(*isim, pixeldet, detId, theGeomDet,tTopo,-1.,-1.,-1.);

     int countRecHits=0;
     SiPixelRecHitCollection::DetSet::const_iterator pixeliter0 = NULL;
     SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixeldet->begin();
     for (; pixeliter != pixeldet->end(); pixeliter++) { // loop over rechits in this module 

       if ( !((pixeliter)->isValid()) ) continue;
     
       count2++;
       LocalPoint lp = pixeliter->localPosition();
       float rechit_x = lp.x();
       float rechit_y = lp.y();

       const DetId & rhdetId = (pixeliter)->geographicalId();
       unsigned int rhdetid = (rhdetId.rawId());
       
      if( detid != rhdetid ) {cout<<" dets do not agree "<<endl; continue;} 
      countRecHits++;
       // Get cluster 
       edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = 
	 pixeliter->cluster();
        
       float ch = (clust->charge())/1000.; // convert ke to electrons
       int size = clust->size();
       int sizeX = clust->sizeX();
       int sizeY = clust->sizeY();
       //float xClu = clust->x();
       //float yClu = clust->y();

       if(verbose_) 
	 cout<<"rechit "<<countRecHits<<" det "<<rhdetid<<" "<<rechit_x<<" "<<rechit_y<<" "<<ch<<" "<<size<<" "<<sizeX<<" "<<sizeY<<endl;

       float resX = (rechit_x - xpos);
       float resY = (rechit_y - ypos);
       float distance = sqrt(resX*resX + resY*resY);
       if(verbose_) cout<<" distance "<<distance<<endl;
       if(distance<distance0) {distance0=distance;pixeliter0=pixeliter;}

     } // loop ober rechits 

     if(verbose_) cout<<"best distance "<<distance0<<endl;
     if(distance0<9999.) {
       if (detId.subdetId() == PixelSubdetector::PixelBarrel)
	 fillBarrel(pixeliter0,*isim, detId, theGeomDet,tTopo,-1.,-1.,-1.,0);
       else if (detId.subdetId() == PixelSubdetector::PixelEndcap)
	 fillForward(pixeliter0, *isim, detId, theGeomDet,tTopo,-1.,-1.,-1.,0);
     }
     if(distance0<CloseMatch) count3++;
     if(distance0<Matched) count4++;
     hdist1->Fill(distance0);

    } // loop over simhits 

    
  // Option 2 - loop ober tracks and than rechits on tracks, match to simhits 
  } else if( useTracks_ && hTTAC.isValid()) { // rechits on tracks only


    // Tracks loop, look at on-track RecHits 
    const TrajTrackAssociationCollection ttac = *(hTTAC.product());
    if (verbose_) cout << "   hTTAC.isValid() " << ttac.size()<< endl;
    
    // Loop on traj-track pairs
    for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {
      //count1++;

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
      count1++;

      if (verbose_) cout<<" track "<<pt<<" "<<eta<<" "<<trackref->d0()<<" "<<trackref->dz()<<" "
			<<trackref->vx()<<" "<<trackref->vy()<<" "<<trackref->vz() <<endl;

      for(trackingRecHit_iterator irecHit = trackref->recHitsBegin(); // loop over rechits 
	  irecHit != trackref->recHitsEnd(); ++irecHit) {
	
	//cout<<(*irecHit)->type()<<endl;
	if( (*irecHit)->type() != 0 ) continue; // skip non valid hits 
	
	DetId detId = (*irecHit)->geographicalId();
	
	if (!( (detId.subdetId() == PixelSubdetector::PixelBarrel)||  // skip non-pixel hits
	       (detId.subdetId() == PixelSubdetector::PixelEndcap) )) continue;
	
	// select only layer 1
	// if(selectL1) {
	//   unsigned int detid=detId.rawId(); // raw det id
	//   unsigned layerC=tTopo->pxbLayer(detid);
	//   if(layerC!=1) continue; // keep only bpix layer 1
	// }

	const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTracker.idToDet(detId) );
	count2++;
	
	float distance =9999.;
#ifdef PIXEL_ASSOCIATOR
	// the way I access simhits works only for bpix so skip fpix
	if( (detId .subdetId() != PixelSubdetector::PixelBarrel) ) continue;

	distance = matchToSims(ism, (*irecHit), detId, theGeomDet,tTopo,pt,eta,phi);
	//float distance = matchToSims((*irecHit), detId, theGeomDet,tTopo,pt,eta,phi);
#else
       distance = matchToSims(associate, (*irecHit), detId, theGeomDet,tTopo,pt,eta,phi);
	//float distance = matchToSims(associate, (*irecHit), detId, theGeomDet,tTopo,pt,eta,phi);
#endif

	hdist1->Fill(distance);
	if(distance<CloseMatch) count3++;
	if(distance<Matched) count4++;
	
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


  // Option 3 : Loop over all recHits, match to simhits
  } else { // all rechits 


    float distance=9999.;
    //cout<<distance<<endl;
    //iterate over detunits
    for (TrackerGeometry::DetContainer::const_iterator it = geom->dets().begin(); 
     	 it != geom->dets().end(); it++) {

      DetId detId = ((*it)->geographicalId());
      
      if (!( (detId.subdetId() == PixelSubdetector::PixelBarrel)||   // select pixels
       	     (detId.subdetId() == PixelSubdetector::PixelEndcap) )) continue;
      
      const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTracker.idToDet(detId) ); 
      //if(verbose_) cout<<" pixel det "<<endl;
      
      SiPixelRecHitCollection::const_iterator pixeldet = recHitColl->find(detId);
      if (pixeldet == recHitColl->end()) continue;  // detunit has pixel rechits 
      if(verbose_) cout<<" pixel det "<<detId.rawId()<<" "<<pixeldet->size()<<endl;

      // select only layer 1
      // if(selectL1) {
      // 	unsigned int detid=detId.rawId(); // raw det id
      // 	unsigned layerC=tTopo->pxbLayer(detid);
      // 	if(layerC!=1) continue; // keep only bpix layer 1
      // }

      count1++;
      distance=9999.;
      //cout<<distance<<endl;
      //----Loop over rechits for this detId

      SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixeldet->begin();
      for (; pixeliter != pixeldet->end(); pixeliter++) { 
	count2++;
#ifdef PIXEL_ASSOCIATOR
	// the way I access simhits works only for bpix so skip fpix
	if( (detId .subdetId() != PixelSubdetector::PixelBarrel) ) continue;
       distance = matchToSims(ism, &(*pixeliter), detId, theGeomDet,tTopo,-1.,-1.,-1.);
       	//distance = matchToSims(&(*pixeliter), detId, theGeomDet,tTopo,-1.,-1.,-1.);
#else
       	distance = matchToSims(associate,&(*pixeliter), detId, theGeomDet,tTopo,-1.,-1.,-1.);
#endif
	if(distance<CloseMatch) count3++;
	if(distance<Matched)    count4++;
	hdist1->Fill(distance);
       	//cout<<distance<<endl;
      }

    } // <------ end detunit loop
    
  }  // use track?

  hcount1->Fill(count1);
  hcount2->Fill(count2);
  hcount3->Fill(count3);
  hcount4->Fill(count4);
  hcount5->Fill(count5);
  hcount6->Fill(count6);
  hcount9->Fill(count9);
  hcount10->Fill(count10);

}
//---------------------------------------------------------------------------------------------------------
// make a list of simhits from the same detunit as the rechit 
#ifdef PIXEL_ASSOCIATOR
std::vector<PSimHit> StudyRecHitMatching::associateHit(const std::vector<PSimHit>& ism,
						       //const TrackingRecHit* hit,
						       DetId detId //, const PixelGeomDetUnit* theGeomDet, 
						       //const TrackerTopology *tTopo
						       ) {
  std::vector<PSimHit> result; 

  for(vector<PSimHit>::const_iterator isim = ism.begin(); isim != ism.end(); ++isim) {    
    // Det id
    DetId smdetId=DetId((*isim).detUnitId());
    unsigned int smdetid=smdetId.rawId(); // raw det id
    if(smdetid == detId.rawId() ) result.push_back(*isim);  // select the right detunit 
  }

  return result;
}
// select the best matching simhit from the list
float StudyRecHitMatching::matchToSims(const  vector<PSimHit>& ism, const TrackingRecHit* hit, 
					   DetId detId, const PixelGeomDetUnit* theGeomDet, 
					   const TrackerTopology *tTopo,double pt, double eta, double phi) {

  float closest=9999., closest2=9999.;
  std::vector<PSimHit>::const_iterator closestit, closest2it;

  LocalPoint lp = hit->localPosition();
  float rechit_x = lp.x();
  float rechit_y = lp.y();
  //int countClose=0;

  // Get cluster 
  SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)hit)->cluster();
  float ch  = (*clust).charge()/1000.; // in kelec

  if(verbose_) cout<<" rechit "<<ch<<" "<<rechit_x<<" "<<rechit_y<<endl;

  // get simhits for this detunit 
  std::vector<PSimHit> matched = associateHit(ism,detId); // get the matched simhits
  if(verbose_) cout<<" simhits from this unit "<<matched.size()<<endl;

  if ( matched.empty() ) {
    cout<<" no match "<<detId.rawId()<<" "<<ch<<" "<<rechit_x<<" "<<rechit_y<<endl; 
    return closest;}

  count5 = matched.size();
  int count7=0;
  int count8=0;
  int majorProcess=-1., majorPid=-1, majorTrack=-1;

  //loop over sim hits and find closet
  for (std::vector<PSimHit>::const_iterator isim = matched.begin(); isim!=matched.end(); ++isim) {
    //cout<<(m->energyLoss())*1000000/3.7<<" ";
    //}
    //cout<<endl;
    //}
    //for(vector<PSimHit>::const_iterator isim = ism.begin(); isim != ism.end(); ++isim) {
    
    // Det id
    DetId smdetId=DetId(isim->detUnitId());
    unsigned int smdetid=smdetId.rawId(); // raw det id
    if(smdetid != detId.rawId() ) continue;  // select the right detunit 
    
    float sim_x1 = isim->entryPoint().x();
    float sim_x2 = isim->exitPoint().x();
    float sim_xpos = 0.5*(sim_x1+sim_x2);
    
    float sim_y1 = isim->entryPoint().y();
    float sim_y2 = isim->exitPoint().y();
    float sim_ypos = 0.5*(sim_y1+sim_y2);
    
    float x_res = fabs(sim_xpos - rechit_x);
    float y_res = fabs(sim_ypos - rechit_y);
      
    float dist = sqrt(x_res*x_res + y_res*y_res); // in cm
    hdist5->Fill(dist);

    if(verbose_) cout<<" simhit "<<dist<<" "<<(isim->energyLoss()*1000000/3.7)<<" "
	<<isim->particleType()<<" "<<isim->processType()<<" "
	<<isim->trackId()
	<<endl;

    if(dist<CloseMatch) count7++; // good fit, within 75um
    if(dist<Matched) { // matched, within 175um 
	count6++;
	count8++;
	// This is just to check if among several matching simhits there is one from the org.gen.
	if( isim->processType() == 0 ) {
	  majorProcess=0; majorPid=abs(isim->particleType()); majorTrack=isim->trackId();
	}
	hParticleType1->Fill(float(abs(isim->particleType())));
	hTrackId1->Fill(float(isim->trackId()));
	hProcessType1->Fill(float(isim->processType()));
    }

    if ( dist < closest ) {
      if(dist<Matched) {closest2 = closest;closest2it = closestit;} // 2nd best
      closest = dist;
      closestit = isim;
      if(majorProcess==-1) 
       	{majorProcess=isim->processType();majorPid=abs(isim->particleType()); majorTrack=isim->trackId();}
    } else if (dist<closest2) {      
      if(dist<Matched) {closest2 = dist;closest2it = isim;} // 2nd best
    }
    
  }

  
  if(verbose_) cout<<" closest "<<closest<<" "<<detId.rawId()<<endl;
  hcount7->Fill(float(count7));
  hcount8->Fill(float(count8));

  hParticleType3->Fill(float(majorPid));
  hTrackId3->Fill(float(majorTrack));
  hProcessType3->Fill(float(majorProcess));

  int status=0;  // only one simhit
  if(closest2<Matched) {
    int proc1 = closestit->processType();
    int proc2 = closest2it->processType();
    
    if(proc1==0 && proc2==0) status=1;  // both come from primary gen particles 
    else if( (proc1==0 && proc2==2) || (proc1==2 && proc2==0) ) status=2; // one is a delta ray
    else if( (proc1==0 && proc2>0)  || (proc1>0 && proc2==0)  ) status=3;  // one is something else 
    else if( (proc1>0 && proc2>0) ) {status=4;}  // both are secondaries 
    else status=5;
  }
  
  if (detId.subdetId() == PixelSubdetector::PixelBarrel)
    fillBarrel(hit, *closestit, detId, theGeomDet,tTopo,pt,eta,phi,status);
  else if (detId.subdetId() == PixelSubdetector::PixelEndcap)
    fillForward(hit, *closestit, detId, theGeomDet,tTopo,pt,eta,phi,status);
  
  return closest;
}

#else

// Tracker Associator
float StudyRecHitMatching::matchToSims(const TrackerHitAssociator& associate, const TrackingRecHit* hit, 
					   DetId detId, const PixelGeomDetUnit* theGeomDet, 
					   const TrackerTopology *tTopo,double pt, double eta, double phi) {

  std::vector<PSimHit> matched = associate.associateHit(*hit); // get the matched simhits
  
  if(verbose_) 
    cout<<" rechit det= "<<detId.rawId() <<" matched size "<<matched.size()<<endl;
  float closest = 9999.;
  
  //if(matched.size()>1) cout<<" matched size greater than 1 "<<matched.size()<<endl;
  hcount8->Fill(float(matched.size()));
  count9 += matched.size();

  if ( !matched.empty() ) {
    std::vector<PSimHit>::const_iterator closestit = matched.begin();
    LocalPoint lp = hit->localPosition();
    float rechit_x = lp.x();
    float rechit_y = lp.y();
    int countClose=0;
    int majorProcess=-1., majorPid=-1, majorTrack=-1;
    
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
      hdist5->Fill(dist);

      if(dist<Matched) {
	count6++;
	countClose++;
	// This is just to check if among several matching simhits there is one from the org.gen.
	if( (*m).processType() == 0 ) {
	  majorProcess=0; majorPid=abs((*m).particleType()); majorTrack=(*m).trackId();
	}
	hParticleType1->Fill(float(abs((*m).particleType())));
	hTrackId1->Fill(float((*m).trackId()));
	hProcessType1->Fill(float((*m).processType()));
      }

      if ( dist < closest ) {
	//closest = x_res;
	closest = dist;
	closestit = m;
	if(majorProcess==-1) 
	  {majorProcess=(*m).processType();majorPid=abs((*m).particleType()); majorTrack=(*m).trackId();}
	if(verbose_) 
	  std::cout<<" simhit pos= "<<sim_xpos<<" "<<sim_ypos
		   <<" p "<<(*m).pabs()<<" e "<<(*m).energyLoss()*1000000/3.7<<" "
		   <<(*m).thetaAtEntry()<<" "
		   <<(*m).phiAtEntry()<<" "
		   <<(*m).particleType()<<" "<<(*m).processType()<<" "
		   <<(*m).trackId()<<" "
		   <<(*m).momentumAtEntry()<<" "
		   <<pt<<" "<<eta<<" "<<phi
		   <<std::endl;
	
      }
    } // end sim hit loop
    
    if(verbose_) cout<<" closest "<<closest<<" "<<detId.rawId()<<endl;
    hcount7->Fill(float(countClose));
    hParticleType3->Fill(float(majorPid));
    hTrackId3->Fill(float(majorTrack));
    hProcessType3->Fill(float(majorProcess));

    if (detId.subdetId() == PixelSubdetector::PixelBarrel)
      fillBarrel(hit, *closestit, detId, theGeomDet,tTopo,pt,eta,phi,0);
    else if (detId.subdetId() == PixelSubdetector::PixelEndcap)
      fillForward(hit, *closestit, detId, theGeomDet,tTopo,pt,eta,phi,0);
  } // end matched emtpy

  return closest;
}
#endif

//------------------------------------------------------------------------------------------

float StudyRecHitMatching::matchToRecs(const PSimHit& simHit,SiPixelRecHitCollection::const_iterator pixeldet,
				       DetId detId, const PixelGeomDetUnit* theGeomDet, 
				       const TrackerTopology *tTopo,double ptT,double etaT ,double phiT) {

  const float cmtomicron = 10000.0; 
  float distance1=9999., distance2=9999.;
  const float MATCH_LIMIT=1000.; // 1.mm=1000um

  // Simhit
  float sim_x1 = simHit.entryPoint().x() * cmtomicron;
  float sim_x2 = simHit.exitPoint().x()  * cmtomicron;
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  float sim_y1 = simHit.entryPoint().y() * cmtomicron;
  float sim_y2 = simHit.exitPoint().y()  * cmtomicron;
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  float sim_lenx = abs(sim_x1-sim_x2)/100.; // convert to pitch units
  float sim_leny = abs(sim_y1-sim_y2)/150.;

  if(verbose_) cout<<"simhit "
		   <<" position "<<sim_xpos<<" "<<sim_ypos
		   <<" entry/exit position x "<<sim_x1<<" "<<sim_x2<<" y "<<sim_y1<<" "<<sim_y2
		   <<" length x "<<sim_lenx<<" y "<<sim_leny
		   <<std::endl;

  //RecHit loop 
  // get rechits for this module 
  if(verbose_) cout<<" pixel det, rechit size "<<pixeldet->size()<<endl;

  int countRecHits=0, countMatched=0;
  SiPixelRecHitCollection::DetSet::const_iterator pixeliter1 = NULL, pixeliter2=NULL;
  SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixeldet->begin();
  for (; pixeliter != pixeldet->end(); pixeliter++) { // loop over rechits in this module 
    
    if ( !((pixeliter)->isValid()) ) continue;
    
    LocalPoint lp = pixeliter->localPosition();
    float rechit_x = lp.x() * cmtomicron;
    float rechit_y = lp.y() * cmtomicron;
    
    const DetId & rhdetId = (pixeliter)->geographicalId();
    unsigned int rhdetid = (rhdetId.rawId());
    
    countRecHits++;
    // Get cluster 
    edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = 
      pixeliter->cluster();
    
    float ch = (clust->charge())/1000.; // convert ke to electrons
    int size = clust->size();
    int sizeX = clust->sizeX();
    int sizeY = clust->sizeY();
    //float xClu = clust->x();
    //float yClu = clust->y();
    
    if(verbose_) 
      cout<<"rechit "<<countRecHits<<" det "<<rhdetid<<" "<<rechit_x<<" "<<rechit_y<<" "<<ch<<" "<<size<<" "<<sizeX<<" "<<sizeY<<endl;
    
    float resX = (rechit_x - sim_xpos);
    float resY = (rechit_y - sim_ypos);
    float distance = sqrt(resX*resX + resY*resY);
    if(distance<MATCH_LIMIT) countMatched++;
    if(verbose_) cout<<" distance "<<distance<<endl;
    if(distance<distance1) {  // find the 1st best 
      distance2=distance1; pixeliter2=pixeliter1;
      distance1=distance;  pixeliter1=pixeliter;
    } else if(distance<distance2) { // find the 2nd best
      distance2=distance; pixeliter2=pixeliter;
    }
             
  } // loop ober rechits 

  // Do the split cluster analysis only for at least 2 matching rechits
  if(countMatched>1) {

    float p  = simHit.pabs();
    int pid = abs(simHit.particleType());
    int process = simHit.processType();
    

    if(verbose_ || 1) cout<<"simhit p "
			  <<p<<" pid "<<pid<<" proc "<<process
			  <<" position "<<sim_xpos<<" "<<sim_ypos
			  <<" entry/exit position x "<<sim_x1<<" "<<sim_x2<<" y "<<sim_y1<<" "<<sim_y2
			  <<" length x "<<sim_lenx<<" y "<<sim_leny
			  <<endl;

    // 1st rechit
    LocalPoint lp = pixeliter1->localPosition();
    float rechit_x1 = lp.x() * cmtomicron/100.;
    float rechit_y1 = lp.y() * cmtomicron/150.;
    // Get cluster 
    edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = 
      pixeliter1->cluster();    
    float ch1 = (clust->charge())/1000.; // convert ke to electrons
    int size1 = clust->size();
    int sizeX1 = clust->sizeX();
    int sizeY1 = clust->sizeY();
    int maxPixelCol1 = clust->maxPixelCol();  //y
    int minPixelCol1 = clust->minPixelCol(); //y
    int maxPixelRow1 = clust->maxPixelRow();  //x
    int minPixelRow1 = clust->minPixelRow(); //x
    cout<<"RecHit1 : "<<distance1<<" "<<rechit_x1<<" "<<rechit_y1<<" "<<sizeX1<<" "<<sizeY1<<" "<<size1<<" "
	<<minPixelRow1<<" "<<maxPixelRow1<<" "
	<<minPixelCol1<<" "<<maxPixelCol1
	<<endl;
    // 2nd rechit
    lp = pixeliter2->localPosition();
    float rechit_x2 = lp.x() * cmtomicron/100.;
    float rechit_y2 = lp.y() * cmtomicron/150.;

    edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust2 = pixeliter2->cluster();    
    float ch2 = (clust2->charge())/1000.; // convert ke to electrons
    int size2 = clust2->size();
    int sizeX2 = clust2->sizeX();
    int sizeY2 = clust2->sizeY();
    int maxPixelCol2 = clust2->maxPixelCol();  //y
    int minPixelCol2 = clust2->minPixelCol(); //y
    int maxPixelRow2 = clust2->maxPixelRow();  //x
    int minPixelRow2 = clust2->minPixelRow(); //x
    cout<<"RecHit2 : "<<distance2<<" "<<rechit_x2<<" "<<rechit_y2<<" "<<sizeX2<<" "<<sizeY2<<" "<<size2<<" "
	<<minPixelRow2<<" "<<maxPixelRow2<<" "
	<<minPixelCol2<<" "<<maxPixelCol2<<" "
	<<endl;


    hdist6->Fill( (distance1) );
    hdist7->Fill( (distance2) );

    hdist8->Fill( abs(rechit_y1 - rechit_y2));
    hdist9->Fill( abs(rechit_x1 - rechit_x2));

    hdist10->Fill( (sim_leny - sizeY1));
    hdist11->Fill( (sim_leny - sizeY2));
    hdist12->Fill( (sim_lenx - sizeX1));
    hdist13->Fill( (sim_lenx - sizeX2));


    //check the distance 
    int disx = min( abs(maxPixelRow1-minPixelRow2)-1, abs(maxPixelRow2-minPixelRow1)-1);
    int disy = min( abs(maxPixelCol1-minPixelCol2)-1, abs(maxPixelCol2-minPixelCol1)-1);
    int lenx = max( abs(maxPixelRow1-minPixelRow2)+1, abs(maxPixelRow2-minPixelRow1)+1);
    int leny = max( abs(maxPixelCol1-minPixelCol2)+1, abs(maxPixelCol2-minPixelCol1)+1);
    cout<<" dist rechits "<<disx<<" "<<disy<<endl;
    if(disx<0) disx=0;
    if(disy<0) disy=0;
    cout<<" dist rechits "<<disx<<" "<<disy<<endl;

    hdist14->Fill(disx);
    hdist15->Fill(disy);

    float sizeRecHitY = sizeY1 + sizeY2 + disy;
    float sizeRecHitX = sizeX1 + sizeX2 + disx;  
    cout<<" length "<<lenx<<" "<<leny<<" "<<sizeRecHitX<<" "<<sizeRecHitY<<endl;

    hdist16->Fill( (sim_leny - sizeRecHitY));
    hdist17->Fill( (sim_lenx - sizeRecHitX));
    hdist18->Fill( (sim_leny - leny));
    hdist19->Fill( (sim_lenx - lenx));


    if(disy>0 && disy<3) hdist20->Fill( (sim_leny - lenx));
    if(disx>0 && disx<2) hdist21->Fill( (sim_lenx - leny));

    if( sim_leny>2 && (disy>0 && disy<3) && disx<1) {
      float tmp1 = (sim_leny - sizeRecHitY);
      float tmp2 = (sim_leny - leny);
      float tmp3 = abs(rechit_y1 - rechit_y2);
      float tmp4 = abs(rechit_x1 - rechit_x2);
      cout<<" candidate for a broken cluster in y "<<tmp1<<" "<<tmp2<<" "<<tmp3<<" "<<tmp4<<endl;
      hdist22->Fill(tmp1);
      hdist23->Fill(tmp2);
      hdist24->Fill(tmp3);
      hdist25->Fill(tmp4);

      count10++; count11++;
    }


  }

  cout<<" best distance "<<distance1<<" "<<countMatched<<" "<<pixeliter1<<endl;
  return distance1;
}

//--------------------------------------------------------------------------------------------

void StudyRecHitMatching::fillBarrel(const TrackingRecHit* recHit, const PSimHit& simHit, 
				     DetId detId, const PixelGeomDetUnit* theGeomDet,
				     const TrackerTopology *tTopo, double ptT, double etaT, double phiT,int simStatus) {
  const float cmtomicron = 10000.0; 
  //const bool muOnly = true;
  const float PI = 3.1416;
  //const int NumLayers = 4;
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

  //if( (phi) != abs(phiH)) cout<<" phi not same "<<phi<<" "<<phiH<<endl; 

  //float pt  = simHit.momentumAtEntry().perp(); // crap, not real pt
  float p  = simHit.pabs();
  int pid = abs(simHit.particleType());
  int process = simHit.processType();

  // cout<<"pid "<<pid<<endl;

  hParticleType2->Fill(float((pid)));
  hTrackId2->Fill(float(simHit.trackId()));
  hProcessType2->Fill(float(process));

  hdist2->Fill(0.001);
  // skip secondaries
  if(muOnly_ && ( (pid) != 13 || p<10.) ) 
    {if(PRINT||(pid)==13 ) cout<<" skip "<<p<<" "<<pid<<endl; return;}


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
  if(flipped) phi = phi - (PI/2.);
  else        phi = (PI/2.) - phi;

  //std::cout<<"Flipped (Surface): "<<(tmp2<tmp1)<<" Flipped (Inner): "<<inner<<std::endl;
  if(PRINT) cout<<" det "<<detId.rawId() <<" layer "<<layer<<" eta "<<eta<<" phi "<<phi<<" "<<p<<endl;

  // Simhit
  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  float sim_lenx = abs(sim_x1-sim_x2);
  float sim_leny = abs(sim_y1-sim_y2);

  // RecHit
  LocalPoint lp = recHit->localPosition();
  float lp_y = lp.y();  
  float lp_x = lp.x();

  GlobalPoint gp = recHit->globalPosition();
  float gp_z = abs(gp.z());

  LocalError lerr = recHit->localPositionError();
  float lerr_x = sqrt(lerr.xx());
  float lerr_y = sqrt(lerr.yy());
  //cout<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<endl;
  float res_x = (lp_x - sim_xpos)*cmtomicron;
  float res_y = (lp_y - sim_ypos)*cmtomicron;
  //float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  //float pull_y = ( lp_y - sim_ypos ) / lerr_y;
  
  //get cluster
  SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)recHit)->cluster();
  int sizeX = (*clust).sizeX();
  int sizeY = (*clust).sizeY();
  //int maxPixelRow = clust->maxPixelRow();
  int maxPixelCol = clust->maxPixelCol();  //y
  //int minPixelRow = clust->minPixelRow(); //x
  int minPixelCol = clust->minPixelCol(); //y


  float distance = sqrt(res_x*res_x + res_y*res_y)/cmtomicron; // in cm
  hdist3->Fill(distance);
  if(distance<Matched) hdist4->Fill(distance);  // matched within 100um

  // skip, for tests only
  //if( abs(eta)<0.1 || abs(eta)>1.0) return; // limit 
  //if( abs(phi)<1.3 || abs(phi)>1.9) return;  // limit to l2 acceptance

  //bool negZ = false;
  //if(module<5) negZ=true;
  

  if(PRINT) {
    cout<<"simhit p= "
	<<p<<" pid "
	<<pid<<" eloss "<<simHit.energyLoss()*1000000/3.7
	<<", track/process "
	<<simHit.trackId()<<"/"<<simHit.processType()
	<<" lay "<<layer<<" mod "<<module
	<<" track pt/eta/phi "<<ptT<<"/"<<etaT<<"/"<<phiT
	<<" position "<<sim_xpos<<" "<<sim_ypos
	<<" entry/exit position x "<<sim_x1<<" "<<sim_x2<<" y "<<sim_y1<<" "<<sim_y2
	<<" length x "<<sim_lenx<<" y "<<sim_leny
	<<std::endl;

    cout<<" rechit "<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y
	<<" residual "<<res_x<<" "<<res_y<<" dist "<<distance
	<<endl;
  }

  if(layer==1) {
    htest1->Fill(eta,phi);
    hphi1->Fill(phi);
    htheta1->Fill(theta);
    hbeta1->Fill(beta);
    heta1->Fill(eta);
    htest4->Fill(etaH,etaT);
    htest5->Fill(phiH,phiT);
    hParticleType4->Fill(float((pid)));
    hTrackId4->Fill(float(simHit.trackId()));
    hProcessType4->Fill(float(process));
  } else if(layer==2) {
    htest2->Fill(eta,phi);
    hphi2->Fill(phi);
    htheta2->Fill(theta);
    hbeta2->Fill(beta);
    heta2->Fill(eta);
  } else if(layer==3) {
    heta3->Fill(eta);
  } else if(layer==4) {
    heta4->Fill(eta);
    hParticleType5->Fill(float((pid)));
    hTrackId5->Fill(float(simHit.trackId()));
    hProcessType5->Fill(float(process));
  }

  float lpx = lp_x, simx=sim_xpos;
  if(flipped) {lpx=-lpx; simx=-simx;} // flip the corrdinate for flipped ladders

  if(layer==1) {

    float r1=0, r2=0;
    hz1->Fill(gp_z);
    if( (pid==2212)||(pid==321)||(pid==211)||(pid==13)) {hz1_1->Fill(gp_z);r1=1.;}
    else if( (pid==11) ) {hz1_2->Fill(gp_z);r1=2.;}
    else if( (pid==22) ) {hz1_3->Fill(gp_z);r1=3.;}
    else if( (pid==2112)){hz1_4->Fill(gp_z);r1=4.;}
    else {hz1_5->Fill(gp_z); r1=5.;}
    
    if( (process==0) ) {hz1_11->Fill(gp_z); r2=1;}
    else if( (process==201) ) {hz1_12->Fill(gp_z); r2=2.;}
    else if( (process==2) )   {hz1_13->Fill(gp_z); r2=3.;}
    else if( (process==121) ) {hz1_14->Fill(gp_z); r2=4.;}
    else if( (process==151) || (process==111)) {hz1_15->Fill(gp_z); r2=5.;}
    else if( (process==4)||(process==3)||(process==12)||(process==13)||(process==14)) 
      {hz1_16->Fill(gp_z); r2=6.;}
    else  {hz1_17->Fill(gp_z); r2=7.;}

    htest6->Fill(r1,r2);

    if     ( (simStatus==0) ) hz1_21->Fill(gp_z);
    else if( (simStatus==1) ) hz1_22->Fill(gp_z);
    else if( (simStatus==2) ) hz1_23->Fill(gp_z);
    else if( (simStatus==3) ) hz1_24->Fill(gp_z);
    else if( (simStatus==4) ) hz1_25->Fill(gp_z);
    else                      hz1_26->Fill(gp_z);

  }


    // Some test histos 
    if(layer==1) {
      phiPerDet1->Fill(phi);
      cluXPerDet1->Fill(lpx);cluYPerDet1->Fill(lp_y);
      //simsXPerDet1->Fill(simx);simsYPerDet1->Fill(sim_ypos);
      //if(flipped) cluXPerDetFL1->Fill(lp_x);
      //else        cluXPerDetNF1->Fill(lp_x);
      htest3->Fill(phi,lpx);
      //htest4->Fill(simx,lpx);

    }
 
  // as a function of layer
  recHitXResLayer[layer-1]->Fill(res_x); 
  recHitYResLayer[layer-1]->Fill(res_y);  

  if(layer==1) {
    if     ( (simStatus==0) ) recHitXResB1->Fill(res_x);
    else if( (simStatus==1) ) recHitXResB2->Fill(res_x);
    else if( (simStatus==2) ) recHitXResB3->Fill(res_x);
    else if( (simStatus==3) ) recHitXResB4->Fill(res_x);
    else if( (simStatus==4) ) recHitXResB5->Fill(res_x);
    else                      recHitXResB6->Fill(res_x);
  }


}
//-----------------------------------------------------------------------------
int StudyRecHitMatching::PhaseIBladeOfflineToOnline(const int& blade)
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
void StudyRecHitMatching::fillForward(const TrackingRecHit* recHit, const PSimHit & simHit, 
				      DetId detId,const PixelGeomDetUnit * theGeomDet,
				      const TrackerTopology *tTopo, double ptT, double etaT, double phiT, int simStatus) {
  const float cmtomicron = 10000.0;

  //int rows = theGeomDet->specificTopology().nrows();
  //int cols = theGeomDet->specificTopology().ncolumns();
  
  LocalPoint lp = recHit->localPosition();
  float lp_x = lp.x();
  float lp_y = lp.y();
  
  //LocalError lerr = recHit->localPositionError();
  //float lerr_x = sqrt(lerr.xx());
  //float lerr_y = sqrt(lerr.yy());

  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  
  //float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  //float pull_y = ( lp_y - sim_ypos ) / lerr_y;

  float res_x = (lp_x - sim_xpos)*cmtomicron;  
  float res_y = (lp_y - sim_ypos)*cmtomicron;

  // integrated fpix results 
  recHitXResAllF->Fill(res_x);
  recHitYResAllF->Fill(res_y);  
  
#ifdef NOT

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

  //int iii=0;
  //cout<<(iii++)<<endl;

  if (!quick_) {
    recHitXResRingSidePanel[ring-1][side-1][panel-1]->Fill(res_x);
    recHitYResRingSidePanel[ring-1][side-1][panel-1]->Fill(res_y);
    recHitXResRingSide[ring-1][side-1]->Fill(res_x);
    recHitYResRingPanel[ring-1][panel-1]->Fill(res_y);
  }

  //cout<<(iii++)<<endl;

  if (side==1) {
    recHitXResNegZF->Fill(res_x);
    recHitYResNegZF->Fill(res_y);
  } else if (side==2) {
    recHitXResPosZF->Fill(res_x);
    recHitYResPosZF->Fill(res_y);  
  }

  //cout<<(iii++)<<endl;

  // get cluster
  //SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)recHit)->cluster();
  //int sizeX = (*clust).sizeX(), sizeY = (*clust).sizeY();
  //float charge = (*clust).charge();
  // if(disk==1) {
  //   if(!quick_) {
  //     clustXSizeDisk1Plaquettes[0]->Fill(sizeX);
  //     clustYSizeDisk1Plaquettes[0]->Fill(sizeY);
  //     clustChargeDisk1Plaquettes[0]->Fill(charge);
  //   }
  // } else if (disk==2) {
  //   if(!quick_) {
  //     clustXSizeDisk2Plaquettes[0]->Fill(sizeX);	
  //     clustYSizeDisk2Plaquettes[0]->Fill(sizeY);
  //     clustChargeDisk2Plaquettes[0]->Fill(charge);
  //   }
  // } else if (disk==3) {
  //   if(!quick_) {
  //     clustXSizeDisk3Plaquettes[0]->Fill(sizeX);	
  //     clustYSizeDisk3Plaquettes[0]->Fill(sizeY);	
  //     clustChargeDisk3Plaquettes[0]->Fill(charge);
  //   } 
  // }

  //cout<<(iii++)<<endl;

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

    ///cout<<(iii++)<<endl;

    if(!quick_) {
      recHitXPullDisk1Plaquettes[0]->Fill(pull_x);
      recHitYPullDisk1Plaquettes[0]->Fill(pull_y);
    }
    
    //cout<<(iii++)<<endl;


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

    //cout<<(iii++)<<endl;

    if(!quick_) {
      recHitXPullDisk2Plaquettes[0]->Fill(pull_x);
      recHitYPullDisk2Plaquettes[0]->Fill(pull_y);
    }


  } else if (disk==3) {

    //cout<<(iii++)<<endl;

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

    if(!quick_) {
      recHitXPullDisk3Plaquettes[0]->Fill(pull_x);
      recHitYPullDisk3Plaquettes[0]->Fill(pull_y);
    }
    //cout<<(iii++)<<endl;


  } // end disk 


  ///cout<<(iii++)<<endl;

  if(quick_) return; // skip the rest

  recHitXErrorAllF->Fill(lerr_x);
  recHitYErrorAllF->Fill(lerr_y);
  recHitXPullAllF->Fill(pull_x);
  recHitYPullAllF->Fill(pull_y);

  // if (rows == 80)       recHitXPlaquetteSize1->Fill(lp_x);
  // else if (rows == 160) recHitXPlaquetteSize2->Fill(lp_x);  
  // if (cols == 104) 
  //   {
  //     recHitYPlaquetteSize2->Fill(lp_y);
  //   } 
  // else if (cols == 156) 
  //   {
  //     recHitYPlaquetteSize3->Fill(lp_y);
  //   } 
  // else if (cols == 208) 
  //   {
  //     recHitYPlaquetteSize4->Fill(lp_y);
  //   } 
  // else if (cols == 260) 
  //   {
  //     recHitYPlaquetteSize5->Fill(lp_y);
  //   }
  
  LocalError lape = theGeomDet->localAlignmentError();
  if (lape.valid()) {
    float tmp11= 0.;
    if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
    //float tmp12= sqrt(lape.xy())*1E4;
    float tmp13= 0.;
    if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
    //if( (tTopo->pxfDisk(detId)) == 1)  { // disk 1
    if( (tTopo->pxfSide(detId)) == 1)  // -z
      {recHitXAlignError5->Fill(tmp11); recHitYAlignError5->Fill(tmp13);}
    else if( (tTopo->pxfSide(detId)) == 2)  // +z
      {recHitXAlignError6->Fill(tmp11); recHitYAlignError6->Fill(tmp13);}
    //} else if( (tTopo->pxfDisk(detId)) == 2) {  // disk 2
    // if( (tTopo->pxfSide(detId)) == 1)  
    //	{recHitXAlignError6->Fill(tmp11); recHitYAlignError6->Fill(tmp13);}
    //else if( (tTopo->pxfSide(detId)) == 2)  
    //	{recHitXAlignError7->Fill(tmp11); recHitYAlignError7->Fill(tmp13);}
    //}
    //else {cout<<" unkown disk "<<tTopo->pxfDisk(detId)<<endl;}
    //if(tmp11>0.) cout<<tTopo->pxfDisk(detId)<<" "
    //		     <<tTopo->pxfSide(detId)<<" "<<tTopo->pxfBlade(detId)<<" "
    //		     <<tTopo->pxfPanel(detId)<<" "<<tTopo->pxfModule(detId)<<" "
    //		     <<rows<<" "<<cols<<" "
    //		     <<tmp11<<" "<<tmp13<<endl;

    float tmp14=float(tTopo->pxfBlade(detId));
    if( (tTopo->pxfDisk(detId)) == 2) tmp14 += 50.;
    if( (tTopo->pxfSide(detId)) == 2) tmp14 += 25.; 
    test->Fill(tmp14,tmp13);

  } // if lape 

#endif // NOT

}
//--------------------------------------------------------------------
void StudyRecHitMatching::beginJob() {
  
  count11=0;

  // Histos
  // BPIX 
  Char_t histo[200], title[200];
  edm::Service<TFileService> fs;

  // ---------------------------------------------------------------
  // All histograms that depend on plaquette number have 7 indexes.
  // The first 4 (0-3) correspond to Panel 1 plaquettes 1-4.
  // The last 3 (4-6) correspond to Panel 2 plaquettes 1-3.
  // ---------------------------------------------------------------
  //const int NumLayers=4;

  hcount1 = fs->make<TH1D>("hcount1","tracks or detunits",1000,-0.5,4999.5);
  hcount2 = fs->make<TH1D>("hcount2","rechits",2000,-0.5,1999.5);
  hcount3 = fs->make<TH1D>("hcount3","simhits <75um",2000,-0.5,1999.5);
  hcount4 = fs->make<TH1D>("hcount4","simhits <175um",2000,-0.5,1999.5);
  hcount5 = fs->make<TH1D>("hcount5","num simhits in the same det",2000,-0.5,1999.5);
  hcount6 = fs->make<TH1D>("hcount6","simhits <175um from rechits",2000,-0.5,1999.5);
  hcount7 = fs->make<TH1D>("hcount7","num simhits <75um",20,-0.5,19.5);
  hcount8 = fs->make<TH1D>("hcount8","num simhits <175um",20,-0.5,19.5);
  hcount9 = fs->make<TH1D>("hcount9","all simhits",2000,-0.5,1999.5);
  hcount10 = fs->make<TH1D>("hcount10","broken clusters",1000,-0.5,999.5);
  hcount11 = fs->make<TH1D>("hcount11","broken clusters",2000,-0.5,1999.5);
  
  hdist1 = fs->make<TH1D>("hdist1","matched",100,0.,0.1);
  hdist2 = fs->make<TH1D>("hdist2","number of bpix matched rechits",100,0.,0.1);
  hdist3 = fs->make<TH1D>("hdist3","selected matched",100,0.,0.1);
  hdist4 = fs->make<TH1D>("hdist4","selected match, close",100,0.,0.1);

  hdist6  = fs->make<TH1D>("hdist6","1st match",100,0.,1000.); // in um
  hdist7  = fs->make<TH1D>("hdist7","2nd match",100,0.,1000.);
  hdist8  = fs->make<TH1D>("hdist8", "matched",1000,-10.,10.);
  hdist9  = fs->make<TH1D>("hdist9", "matched",1000,-10.,10.);
  hdist10 = fs->make<TH1D>("hdist10","matched",1000,-10.,10.);
  hdist11 = fs->make<TH1D>("hdist11","matched",1000,-10.,10.);
  hdist12 = fs->make<TH1D>("hdist12","matched",1000,-10.,10.);
  hdist13 = fs->make<TH1D>("hdist13","matched",1000,-10.,10.);
  hdist14 = fs->make<TH1D>("hdist14","matched",1000,-10.,10.);
  hdist15 = fs->make<TH1D>("hdist15","matched",1000,-10.,10.);
  hdist16 = fs->make<TH1D>("hdist16","matched",1000,-10.,10.);
  hdist17 = fs->make<TH1D>("hdist17","matched",1000,-10.,10.);
  hdist18 = fs->make<TH1D>("hdist18","matched",1000,-10.,10.);
  hdist19 = fs->make<TH1D>("hdist19","matched",1000,-10.,10.);
  hdist20 = fs->make<TH1D>("hdist20","matched",1000,-10.,10.);
  hdist21 = fs->make<TH1D>("hdist21","matched",1000,-10.,10.);
  hdist22 = fs->make<TH1D>("hdist22","matched",1000,-10.,10.);
  hdist23 = fs->make<TH1D>("hdist23","matched",1000,-10.,10.);
  hdist24 = fs->make<TH1D>("hdist24","matched",1000,-10.,10.);
  hdist25 = fs->make<TH1D>("hdist25","matched",1000,-10.,10.);


  hParticleType1 = fs->make<TH1D>("hParticleType1","partcile type close",2500,0.,2500.);
  hTrackId1 = fs->make<TH1D>("hTrackId1","trackid close",1000,0.,1000.);
  hProcessType1 = fs->make<TH1D>("hProcessType1","process type close",500,0.,500.);
  hParticleType2 = fs->make<TH1D>("hParticleType2","partcile type best",2500,0.,2500.);
  hTrackId2 = fs->make<TH1D>("hTrackId2","trackid best",1000,0.,1000.);
  hProcessType2 = fs->make<TH1D>("hProcessType2","process type best",500,0.,500.);
  hParticleType3 = fs->make<TH1D>("hParticleType3","partcile type major",2500,0.,2500.);
  hTrackId3 = fs->make<TH1D>("hTrackId3","trackid major",1000,0.,1000.);
  hProcessType3 = fs->make<TH1D>("hProcessType3","process type major",500,0.,500.);
  hParticleType4 = fs->make<TH1D>("hParticleType4","partcile type major",2500,0.,2500.);
  hTrackId4 = fs->make<TH1D>("hTrackId4","trackid major",1000,0.,1000.);
  hProcessType4 = fs->make<TH1D>("hProcessType4","process type major",500,0.,500.);
  hParticleType5 = fs->make<TH1D>("hParticleType5","partcile type major",2500,0.,2500.);
  hTrackId5 = fs->make<TH1D>("hTrackId5","trackid major",1000,0.,1000.);
  hProcessType5 = fs->make<TH1D>("hProcessType5","process type major",500,0.,500.);





  //  if(!quick_) {
  hphiTrack = fs->make<TH1D>("hphiTrack","track phi",70,-3.5,3.5);
  hetaTrack = fs->make<TH1D>("hetaTrack","track eta",50,-2.5,2.5);
  hptTrack  = fs->make<TH1D>("hptTrack","track pt",150,0.0,150.);
  
  hphi1 = fs->make<TH1D>("hphi1","phi1",70,-3.5,3.5);
  htheta1 = fs->make<TH1D>("htheta1","theta1",70,-3.5,3.5);
  hbeta1  = fs->make<TH1D>("beta1","beta1",70,-3.5,3.5);
  
  hphi2 = fs->make<TH1D>("hphi2","phi2",70,-3.5,3.5);
  htheta2 = fs->make<TH1D>("htheta2","theta2",70,-3.5,3.5);
  hbeta2  = fs->make<TH1D>("beta2","beta2",70,-3.5,3.5);
  
  heta1 = fs->make<TH1D>("heta1","eta",60,-3.0,3.0);
  heta2 = fs->make<TH1D>("heta2","eta",50,-2.5,2.5);
  heta3 = fs->make<TH1D>("heta3","eta",50,-2.5,2.5);
  heta4 = fs->make<TH1D>("heta4","eta",50,-2.5,2.5);

  hz1 = fs->make<TH1D>("hz1","gz layer 1",26,0.,26.);

  hz1_1 = fs->make<TH1D>("hz1_1","gz pid group1",26,0.,26.);
  hz1_2 = fs->make<TH1D>("hz1_2","gz pid group2",26,0.,26.);
  hz1_3 = fs->make<TH1D>("hz1_3","gz pid group3",26,0.,26.);
  hz1_4 = fs->make<TH1D>("hz1_4","gz pid group4",26,0.,26.);
  hz1_5 = fs->make<TH1D>("hz1_5","eta pid group5",26,0.,26.);

  hz1_11 = fs->make<TH1D>("hz1_11","gz process group1",26,0.,26.);
  hz1_12 = fs->make<TH1D>("hz1_12","gz process group2",26,0.,26.);
  hz1_13 = fs->make<TH1D>("hz1_13","gz process group3",26,0.,26.);
  hz1_14 = fs->make<TH1D>("hz1_14","gz process group4",26,0.,26.);
  hz1_15 = fs->make<TH1D>("hz1_15","gz process group5",26,0.,26.);
  hz1_16 = fs->make<TH1D>("hz1_16","gz process group6",26,0.,26.);
  hz1_17 = fs->make<TH1D>("hz1_17","gz process group7",26,0.,26.);

  hz1_21 = fs->make<TH1D>("hz1_21","gz process0 ",26,0.,26.);
  hz1_22 = fs->make<TH1D>("hz1_22","gz process1",26,0.,26.);
  hz1_23 = fs->make<TH1D>("hz1_23","gz process2",26,0.,26.);
  hz1_24 = fs->make<TH1D>("hz1_24","gz process3",26,0.,26.);
  hz1_25 = fs->make<TH1D>("hz1_25","gz process4",26,0.,26.);
  hz1_26 = fs->make<TH1D>("hz1_26","gz process3",26,0.,26.);
  
  htest1 = fs->make<TH2F>("htest1","test1",50,0.0,2.5,70,-3.5,3.5);
  htest2 = fs->make<TH2F>("htest2","test2",50,0.0,2.5,70,-3.5,3.5);
  htest3 = fs->make<TH2F>("htest3","test3",120,-0.3,0.3,162,-0.81,0.81);
  htest4 = fs->make<TH2F>("htest4","eta T vs eta H",50,-2.5,2.5,50,-2.5,2.5);
  htest5 = fs->make<TH2F>("htest5","phi T vs phi H",70,-3.5,3.5,70,-3.5,3.5);
  htest6 = fs->make<TH2F>("htest6","test6",10,0.0,10.0,10,0.,10.);
  
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
  }


  // Test histos 
  phiPerDet1 = fs->make<TH1D>("phiPerDet1","clus vs phi-det1",100,-0.4,0.4);
  //phiPerDet2 = fs->make<TH1D>("phiPerDet2","clus vs phi-det2",100,-0.4,0.4);
  //phiPerDet3 = fs->make<TH1D>("phiPerDet3","clus vs phi-det3",100,-0.4,0.4);
  //phiPerDet4 = fs->make<TH1D>("phiPerDet4","clus vs phi-det4",100,-0.4,0.4);
  
  cluXPerDet1 = fs->make<TH1D>("cluXPerDet1","clus vs x-det1",162,-0.81,0.81);
  cluYPerDet1 = fs->make<TH1D>("cluYPerDet1","clus vs y-det1",100,-3.2,3.2);
  //simsXPerDet1 = fs->make<TH1D>("simsXPerDet1","sims vs x-det1",162,-0.81,0.81);
  //simsYPerDet1 = fs->make<TH1D>("simsYPerDet1","sims vs y-det1",100,-3.2,3.2);
  
  recHitXResB1 = fs->make<TH1D>("recHitXResB1","resX, L1, simStatus0", 100, -200., 200.);
  recHitXResB2 = fs->make<TH1D>("recHitXResB2","resX, L1, simStatus1", 100, -200., 200.);
  recHitXResB3 = fs->make<TH1D>("recHitXResB3","resX, L1, simStatus2", 100, -200., 200.);
  recHitXResB4 = fs->make<TH1D>("recHitXResB4","resX, L1, simStatus3", 100, -200., 200.);
  recHitXResB5 = fs->make<TH1D>("recHitXResB5","resX, L1, simStatus4", 100, -200., 200.);
  recHitXResB6 = fs->make<TH1D>("recHitXResB6","resX, L1, simStatus5", 100, -200., 200.);
  
#ifdef NOT 
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
	
	if(i==0) {
	  cluSizeXVsPhi1 = dbe_->bookProfile("cluSizeXVsPhi1","clus sizeX vs phi layer1",1620,-0.81,0.81,0.,100.," ");

	  size1_mz_f = fs->make<TH1D>("size1_mz_f", "size1 -z flip",162,-0.81,0.81);
	  size1_mz_nf= fs->make<TH1D>("size1_mz_nf","size1 -z nfli",162,-0.81,0.81);
	  size1_pz_f = fs->make<TH1D>("size1_pz_f", "size1 +z flip",162,-0.81,0.81);
	  size1_pz_nf= fs->make<TH1D>("size1_pz_nf","size1 +z nfli",162,-0.81,0.81);
	  size2_mz_f = fs->make<TH1D>("size2_mz_f", "size2 -z flip",162,-0.81,0.81);
	  size2_mz_nf= fs->make<TH1D>("size2_mz_nf","size2 -z nfli",162,-0.81,0.81);
	  size2_pz_f = fs->make<TH1D>("size2_pz_f", "size2 +z flip",162,-0.81,0.81);
	  size2_pz_nf= fs->make<TH1D>("size2_pz_nf","size2 +z nfli",162,-0.81,0.81);
	  size3_mz_f = fs->make<TH1D>("size3_mz_f", "size3 -z flip",162,-0.81,0.81);
	  size3_mz_nf= fs->make<TH1D>("size3_mz_nf","size3 -z nfli",162,-0.81,0.81);
	  size3_pz_f = fs->make<TH1D>("size3_pz_f", "size3 +z flip",162,-0.81,0.81);
	  size3_pz_nf= fs->make<TH1D>("size3_pz_nf","size3 +z nfli",162,-0.81,0.81);
	}
      } // end i, layers
    
#endif // NOT

    // FPix 

    //RecHit X resolution all plaquettes
    recHitXResAllF = fs->make<TH1D>("RecHit_xres_f_All", "RecHit X Res All in Forward", 100, -200., 200.);
    //recHitXResPosZF = fs->make<TH1D>("RecHit_xres_f_posZ", "RecHit X Res All in Forward Disks +Z", 100, -200., 200.);
    //  recHitXResNegZF = fs->make<TH1D>("RecHit_xres_f_negZ", "RecHit X Res All in Forward Disks -Z", 100, -200., 200.);
    
    //RecHit Y resolution all plaquettes
    recHitYResAllF = fs->make<TH1D>("RecHit_yres_f_All", "RecHit Y Res All in Forward", 100, -200., 200.);
    //    recHitYResPosZF = fs->make<TH1D>("RecHit_yres_f_posZ", "RecHit Y Res All in Forward Disks +Z", 100, -200., 200.);
    //  recHitYResNegZF = fs->make<TH1D>("RecHit_yres_f_negZ", "RecHit Y Res All in Forward Disks -Z", 100, -200., 200.);
    

}
//-------------------------------------------------------------------------------
void StudyRecHitMatching::endJob() {
  cout<<"endJob "<<count11<<endl;
}


//define this as a plug-in
DEFINE_FWK_MODULE(StudyRecHitMatching);
