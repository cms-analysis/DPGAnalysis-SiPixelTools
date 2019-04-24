// File: PixelHitAssociator.cc
// Modified for pixel only direct simhits (no crossing frame) d.k. 2/14
 
#include <memory>
#include <string>
#include <vector>

#include "DPGAnalysis-SiPixelTools/PixelHitAssociator/interface/PixelHitAssociator.h"

//--- for Geometry:
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

//for accumulate
#include <numeric>
#include <iostream>

using namespace std;
using namespace edm;

//
// Constructor 
//
PixelHitAssociator::PixelHitAssociator(const edm::Event& e)  : 
  //doPixel_( true ),
  //doStrip_( false ), 
  //doTrackAssoc_( false ), 
  myEvent_(e) { 

  trackerContainers.clear();
  trackerContainers.push_back("TrackerHitsPixelBarrelLowTof");
  trackerContainers.push_back("TrackerHitsPixelBarrelHighTof");
  trackerContainers.push_back("TrackerHitsPixelEndcapLowTof");
  trackerContainers.push_back("TrackerHitsPixelEndcapHighTof");
    
  SimHitMap.clear();

  for(auto const& trackerContainer : trackerContainers) {     
    //cout<<" "<<trackerContainer<<endl;
      edm::Handle<std::vector<PSimHit> > simHits;
      edm::InputTag tag("g4SimHits", trackerContainer);
      e.getByLabel(tag, simHits);

      int i=0;
      for (std::vector<PSimHit>::const_iterator isim=simHits->begin(); 
	   isim!= simHits->end();isim++) {
	i++;
	//std::cout<<i<<" "<<(*isim).detUnitId()<<std::endl;
	SimHitMap[(*isim).detUnitId()].push_back((*isim));
      }
    } // for loop
    
  //std::cout<<" size "<<SimHitMap.size()<<std::endl;

  e.getByLabel("simSiPixelDigis", pixeldigisimlink);
  //e.getByLabel("mix", pixeldigisimlink);
  
}

//
// Constructor with configurables
// Modified to work with simple PSimHit containers and not the crossing bullshit
PixelHitAssociator::PixelHitAssociator(const edm::Event& e, const edm::ParameterSet& conf)  : 
  //doPixel_( conf.getParameter<bool>("associatePixel") ),
  //doStrip_( conf.getParameter<bool>("associateStrip") ),
  //doTrackAssoc_( conf.getParameter<bool>("associateRecoTracks") ),
  myEvent_(e) {
  
  //cout<<" from constructor "<<doPixel_<<" "<<doStrip_<<" "<<doTrackAssoc_<<endl;

  //if(!doTrackAssoc_) {
    
    trackerContainers.clear();
    trackerContainers = conf.getParameter<std::vector<std::string> >("ROUList");
    
    SimHitMap.clear();

    for(auto const& trackerContainer : trackerContainers) {     
      //cout<<" "<<trackerContainer<<endl;
      edm::Handle<std::vector<PSimHit> > simHits;
      edm::InputTag tag("g4SimHits", trackerContainer);
      e.getByLabel(tag, simHits);

      //cout<<simHits->size()<<endl;
      int i=0;
      for (std::vector<PSimHit>::const_iterator isim=simHits->begin(); 
	   isim!= simHits->end();isim++) {
	i++;
	//std::cout<<i<<" "<<(*isim).detUnitId()<<std::endl;
	SimHitMap[(*isim).detUnitId()].push_back((*isim));
      }

      //std::cout<<" simhit size "<<SimHitMap.size()<<std::endl;

    } // for loop
    
//}

  e.getByLabel("simSiPixelDigis", pixeldigisimlink);
  //e.getByLabel("mix", pixeldigisimlink);
  
}


std::vector<PSimHit> PixelHitAssociator::associateHit(const TrackingRecHit & thit)  {  
  //vector with the matched SimHit
  std::vector<PSimHit> result; 
  //initialize vectors!
  simtrackid.clear();
  simhitCFPos.clear();
  
  //get the Detector type of the rechit
  DetId detid=  thit.geographicalId();
  uint32_t detID = detid.rawId();

  //check we are in the pixel tracker
  if( (unsigned int)(detid.subdetId()) == PixelSubdetector::PixelBarrel || 
      (unsigned int)(detid.subdetId()) == PixelSubdetector::PixelEndcap)   {
    if(const SiPixelRecHit * rechit = dynamic_cast<const SiPixelRecHit *>(&thit))
      {	  
	//std::cout << "associate to pixelHit" << std::endl;
	associatePixelRecHit(rechit,simtrackid );
      }
  }

    
  //cout << "Associator for pixels " << detID << endl;

  // This part is for pixel rechits
  //now get the SimHit from the trackid
  vector<PSimHit> simHit; 
  std::map<unsigned int, std::vector<PSimHit> >::const_iterator it = SimHitMap.find(detID);
  simHit.clear();
  if (it!= SimHitMap.end()){
    //simHit = it->second;
    //vector<PSimHit>::const_iterator simHitIter = simHit.begin();
    //vector<PSimHit>::const_iterator simHitIterEnd = simHit.end();
    vector<PSimHit>::const_iterator simHitIter = (it->second).begin();
    vector<PSimHit>::const_iterator simHitIterEnd = (it->second).end();

    for (;simHitIter != simHitIterEnd; ++simHitIter) {
      const PSimHit ihit = *simHitIter;
      unsigned int simHitid = ihit.trackId();
      EncodedEventId simHiteid = ihit.eventId();
      
      for(size_t i=0; i<simtrackid.size();i++){
	if(simHitid == simtrackid[i].first && simHiteid == simtrackid[i].second){ 
	  //	  cout << "Associator ---> ID" << ihit.trackId() << " Simhit x= " << ihit.localPosition().x() 
	  //	       << " y= " <<  ihit.localPosition().y() << " z= " <<  ihit.localPosition().x() << endl; 
	  result.push_back(ihit);
	}
      }
    }
    
  } else { cout<<" SimHitMap empty "<<endl;}

  return result;  
}



void  PixelHitAssociator::associatePixelRecHit(const SiPixelRecHit * pixelrechit, std::vector<SimHitIdpr> & simtrackid)
{
  const bool myPrint = false;

  //StripHits = false;
  //cout<<" associatePixelRecHit "<<endl;

  //
  // Pixel associator
  //
  DetId detid=  pixelrechit->geographicalId();
  uint32_t detID = detid.rawId();

  edm::DetSetVector<PixelDigiSimLink>::const_iterator isearch = pixeldigisimlink->find(detID); 
  if(isearch != pixeldigisimlink->end()) {  //if it is not empty
    //    edm::DetSet<PixelDigiSimLink> link_detset = (*pixeldigisimlink)[detID];
    edm::DetSet<PixelDigiSimLink> link_detset = (*isearch);
    //    edm::Ref< edm::DetSetVector<SiPixelCluster>, SiPixelCluster> const& cluster = pixelrechit->cluster();
    SiPixelRecHit::ClusterRef const& cluster = pixelrechit->cluster();
    
    //check the reference is valid
    
    if(!(cluster.isNull())){//if the cluster is valid
      
      int minPixelRow = (*cluster).minPixelRow();
      int maxPixelRow = (*cluster).maxPixelRow();
      int minPixelCol = (*cluster).minPixelCol();
      int maxPixelCol = (*cluster).maxPixelCol();    
      if(myPrint) 
	std::cout << "    Cluster minRow " << minPixelRow << " maxRow " 
		  << maxPixelRow << "    Cluster minCol " 
		  << minPixelCol << " maxCol " << maxPixelCol << std::endl;

      edm::DetSet<PixelDigiSimLink>::const_iterator linkiter = link_detset.data.begin();
      int dsl = 0;
      //    std::vector<unsigned int> idcachev;
      std::vector<SimHitIdpr> idcachev;
      for( ; linkiter != link_detset.data.end(); linkiter++) {
	dsl++;
	std::pair<int,int> pixel_coord = PixelDigi::channelToPixel(linkiter->channel());
	if(myPrint) std::cout << "    " << dsl << ") Digi link: row " << pixel_coord.first << " col " << pixel_coord.second << std::endl;      
	if(  pixel_coord.first  <= maxPixelRow && 
	     pixel_coord.first  >= minPixelRow &&
	     pixel_coord.second <= maxPixelCol &&
	     pixel_coord.second >= minPixelCol ) {
	  if(myPrint) std::cout << "      !-> trackid   " << linkiter->SimTrackId() << " fraction  " << linkiter->fraction()   << endl;
	  SimHitIdpr currentId(linkiter->SimTrackId(), linkiter->eventId());
	  //	if(find(idcachev.begin(),idcachev.end(),linkiter->SimTrackId()) == idcachev.end()){
	  if(find(idcachev.begin(),idcachev.end(),currentId) == idcachev.end()){
	    //	  simtrackid.push_back(linkiter->SimTrackId());
	    //idcachev.push_back(linkiter->SimTrackId());
	    simtrackid.push_back(currentId);
	    idcachev.push_back(currentId);
	  }
	} 
      }
    }
    else{      
      edm::LogError("PixelHitAssociator")<<"no Pixel cluster reference attached";
      
    }
  }
  

}



// Do I need this

// //std::vector<unsigned int> PixelHitAssociator::associateHitId(const TrackingRecHit & thit) 
// std::vector< SimHitIdpr > PixelHitAssociator::associateHitId(const TrackingRecHit & thit) 
// {
//   cout<<" is this ever called"<<endl;
//   std::vector< SimHitIdpr > simhitid;
//   associateHitId(thit, simhitid);
//   return simhitid;
// }

// void PixelHitAssociator::associateHitId(const TrackingRecHit & thit, std::vector< SimHitIdpr > & simtkid) 
// {
  
//   cout<<" is this ever called 2"<<endl;

//   //check in case of TTRH
//   if(const TransientTrackingRecHit * ttrh = dynamic_cast<const TransientTrackingRecHit *>(&thit)) {
//     associateHitId(*ttrh->hit(), simtkid);

//   }
//   else{
//     simtkid.clear();
//     simhitCFPos.clear();
//     //vector with the associated Simtkid 
//     //vector with the CF position of the associated simhits
    
//   //get the Detector type of the rechit
//     //DetId detid=  thit.geographicalId();
//     //apparently not used
//     //  uint32_t detID = detid.rawId();

//     // what is this for?
//     //if (const SiTrackerMultiRecHit * rechit = dynamic_cast<const SiTrackerMultiRecHit *>(&thit)){
//     // simtkid=associateMultiRecHitId(rechit);
//     //}
    

//     //else if( (unsigned int)(detid.subdetId()) == PixelSubdetector::PixelBarrel || 
//     //	     (unsigned int)(detid.subdetId()) == PixelSubdetector::PixelEndcap) 
//     //{
//     if(const SiPixelRecHit * rechit = dynamic_cast<const SiPixelRecHit *>(&thit))
//       {	  
// 	associatePixelRecHit(rechit,simtkid );
//       }
//       //}
//   }

//   //cout << "Associator ---> get Detid " << detID << endl;
//   //check we are in the strip tracker
//     // if(detid.subdetId() == StripSubdetector::TIB ||
//     //    detid.subdetId() == StripSubdetector::TOB || 
//     //    detid.subdetId() == StripSubdetector::TID ||
//     //    detid.subdetId() == StripSubdetector::TEC) 
//     //   {
//     // 	//check if it is a simple SiStripRecHit2D
//     // 	if(const SiStripRecHit2D * rechit = 
//     // 	   dynamic_cast<const SiStripRecHit2D *>(&thit))
//     // 	  {	  
//     // 	    associateSimpleRecHit(rechit, simtkid);
//     // 	  }
//     // 	//check if it is a matched SiStripMatchedRecHit2D
//     // 	else  if(const SiStripRecHit1D * rechit = 
//     // 		 dynamic_cast<const SiStripRecHit1D *>(&thit))
//     // 	  {	  
//     // 	    associateSiStripRecHit1D(rechit,simtkid);
//     // 	  }
//     // 	//check if it is a matched SiStripMatchedRecHit2D
//     // 	else  if(const SiStripMatchedRecHit2D * rechit = 
//     // 		 dynamic_cast<const SiStripMatchedRecHit2D *>(&thit))
//     // 	  {	  
//     // 	    simtkid = associateMatchedRecHit(rechit);
//     // 	  }
//     // 	//check if it is a  ProjectedSiStripRecHit2D
//     // 	else if(const ProjectedSiStripRecHit2D * rechit = 
//     // 		dynamic_cast<const ProjectedSiStripRecHit2D *>(&thit))
//     // 	  {	  
//     // 	    simtkid = associateProjectedRecHit(rechit);
//     // 	  }
//     // 	else{
//     // 	  //std::cout << "associate to invalid" << std::endl;
//     // 	  //throw cms::Exception("Unknown RecHit Type") << "PixelHitAssociator failed second casting of " << typeid(thit).name() << " type ";
//     // 	}
//     //   }
//     //check we are in the pixel tracker


//     //check if these are GSRecHits (from FastSim)
//     // if(const SiTrackerGSRecHit2D * rechit = dynamic_cast<const SiTrackerGSRecHit2D *>(&thit))
//     //   {
//     // 	simtkid = associateGSRecHit(rechit);
//     //   }  
//     // if(const SiTrackerGSMatchedRecHit2D * rechit = dynamic_cast<const SiTrackerGSMatchedRecHit2D *>(&thit))
//     //   {
//     // 	simtkid = associateGSMatchedRecHit(rechit);
//     //   }

//     //  }

// }


// void PixelHitAssociator::associateSimpleRecHit(const SiStripRecHit2D * simplerechit, std::vector<SimHitIdpr>& simtrackid)
// {
//   const SiStripCluster* clust = 0; 
//   if(simplerechit->cluster().isNonnull())
//     {
//       clust=&(*simplerechit->cluster());
//     }
//   else if(simplerechit->cluster_regional().isNonnull())
//     {
//       clust=&(*simplerechit->cluster_regional());
//     } 
  
//   associateSimpleRecHitCluster(clust,simplerechit->geographicalId(),simtrackid);
// }


// //This function could be templated to avoid to repeat the same code twice??
// void PixelHitAssociator::associateSiStripRecHit1D(const SiStripRecHit1D * simplerechit, std::vector<SimHitIdpr>& simtrackid)
// {
//   const SiStripCluster* clust = 0; 
//   if(simplerechit->cluster().isNonnull())
//     {
//       clust=&(*simplerechit->cluster());
//     }
//   else if(simplerechit->cluster_regional().isNonnull())
//     {
//       clust=&(*simplerechit->cluster_regional());
//     } 
//   associateSimpleRecHitCluster(clust,simplerechit->geographicalId(),simtrackid);
// }

// void PixelHitAssociator::associateSimpleRecHitCluster(const SiStripCluster* clust,
// 							const uint32_t& detID,
// 							std::vector<SimHitIdpr>& theSimtrackid, std::vector<PSimHit>& simhit)
// {
// // Caller needs to clear simhit before calling this function

//   //initialize vector
//   theSimtrackid.clear();
//   //initialize class vector
//   simhitCFPos.clear();

//   associateSimpleRecHitCluster(clust, detID, theSimtrackid);

//   for(size_t i=0; i<simhitCFPos.size(); i++){
//     simhit.push_back(TrackerHits.getObject(simhitCFPos[i]));
//   }
// }

// void PixelHitAssociator::associateSimpleRecHitCluster(const SiStripCluster* clust,
// 							const uint32_t& detID,
// 							std::vector<PSimHit>& simhit)
// {
// // Caller needs to clear simhit before calling this function

//   //initialize class vectors
//   simtrackid.clear();
//   simhitCFPos.clear();

//   associateSimpleRecHitCluster(clust, detID, simtrackid);

//   for(size_t i=0; i<simhitCFPos.size(); i++){
//     simhit.push_back(TrackerHits.getObject(simhitCFPos[i]));
//   }
// }

// void PixelHitAssociator::associateSimpleRecHitCluster(const SiStripCluster* clust,
// 							const uint32_t& detID,
// 							std::vector<SimHitIdpr>& simtrackid){
//   //  std::cout <<"ASSOCIATE SIMPLE RECHIT" << std::endl;	    
//   StripHits =true;	  
  
//   //DetId detid=  clust->geographicalId();
//   //  uint32_t detID = detid.rawId();
  
//   //to store temporary charge information
//   std::vector<SimHitIdpr> cache_simtrackid; 
//   cache_simtrackid.clear();
  
//   std::map<SimHitIdpr, vector<float> > temp_simtrackid;
//   temp_simtrackid.clear();
  
//   edm::DetSetVector<StripDigiSimLink>::const_iterator isearch = stripdigisimlink->find(detID); 
//   if(isearch != stripdigisimlink->end()) {  //if it is not empty
//     //link_detset is a structure, link_detset.data is a std::vector<StripDigiSimLink>
//     //edm::DetSet<StripDigiSimLink> link_detset = (*stripdigisimlink)[detID];
//     edm::DetSet<StripDigiSimLink> link_detset = (*isearch);
    
//     if(clust!=0){//the cluster is valid

//       //    const edm::Ref<edm::DetSetVector<SiStripCluster>, SiStripCluster, edm::refhelper::FindForDetSetVector<SiStripCluster> > clust=simplerechit->cluster();
      
//       //float chg;
//       int clusiz = clust->amplitudes().size();
//       int first  = clust->firstStrip();     
//       int last   = first + clusiz;
      
// //       std::cout << "CLUSTERSIZE " << clusiz << " first strip = " << first << " last strip = " << last-1 << std::endl;
// //       std::cout << " detID = " << detID << " DETSET size = " << link_detset.data.size() << std::endl;
//       //use a vector
//       std::vector<SimHitIdpr> idcachev;
//       std::vector<int> CFposcachev;
//       for(edm::DetSet<StripDigiSimLink>::const_iterator linkiter = link_detset.data.begin(); linkiter != link_detset.data.end(); linkiter++){
// 	//StripDigiSimLink link = *linkiter;
	
// 	if( (int)(linkiter->channel()) >= first  && (int)(linkiter->channel()) < last ){
	  
// 	  //check this digisimlink
// // 	  printf("%s%4d%s%8d%s%3d%s%8.4f\n", "CHANNEL = ", linkiter->channel(), " TrackID = ", linkiter->SimTrackId(),
// // 		 " Process = ", TrackerHits.getObject(linkiter->CFposition()-1).processType(), " fraction = ", linkiter->fraction());
// 	  /*
// 	    std::cout << "CHECKING CHANNEL  = " << linkiter->channel()   << std::endl;
// 	    std::cout << "TrackID  = " << linkiter->SimTrackId()  << std::endl;
// 	    std::cout << "Position = " << linkiter->CFposition()  << std::endl;
// 	    std::cout << " POS -1 = " << TrackerHits.getObject(linkiter->CFposition()-1).localPosition() << std::endl;
// 	    std::cout << " Process = " << TrackerHits.getObject(linkiter->CFposition()-1).processType() << std::endl;
// 	    std::cout << " fraction = " << linkiter->fraction() << std::endl;
// 	  */
	  
// 	  SimHitIdpr currentId(linkiter->SimTrackId(), linkiter->eventId());
	  
// 	  //create a vector with the list of SimTrack ID's of the tracks that contributed to the RecHit
// 	  //write the id only once in the vector
	  
// 	  if(find(idcachev.begin(),idcachev.end(),currentId ) == idcachev.end()){
// 	    /*
// 	      std::cout << " Adding track id  = " << currentId.first  
// 	      << " Event id = " << currentId.second.event() 
// 	      << " Bunch Xing = " << currentId.second.bunchCrossing() 
// 	      << std::endl;
// 	    */
// 	    idcachev.push_back(currentId);
// 	    simtrackid.push_back(currentId);
// 	  }
	  
// 	  //create a vector that contains all the position (in the MixCollection) of the SimHits that contributed to the RecHit
// 	  //write position only once
// 	  int currentCFPos = linkiter->CFposition()-1;
// 	  if(find(CFposcachev.begin(),CFposcachev.end(),currentCFPos ) == CFposcachev.end()){
// 	    /*
// 	      std::cout << "CHECKING CHANNEL  = " << linkiter->channel()   << std::endl;
// 	      std::cout << "\tTrackID  = " << linkiter->SimTrackId()  << "\tCFPos = " << currentCFPos  << std::endl;
// 	      std::cout << "\tLocal Pos = " << TrackerHits.getObject(currentCFPos).localPosition() 
// 	      << "\tProcess = " << TrackerHits.getObject(currentCFPos).processType() << std::endl;
// 	    */
// 	    CFposcachev.push_back(currentCFPos);
// 	    simhitCFPos.push_back(currentCFPos);
// 	    //	  simhitassoc.push_back( TrackerHits.getObject(currentCFPos));
// 	  }
// 	}
//       }    
//     }
//     else {
//       edm::LogError("PixelHitAssociator")<<"no cluster reference attached";
//     }
//   }
// }

// std::vector<SimHitIdpr>  PixelHitAssociator::associateMatchedRecHit(const SiStripMatchedRecHit2D * matchedrechit)
// {

//   StripHits = true;


//   vector<SimHitIdpr> matched_mono;
//   vector<SimHitIdpr> matched_st;
//   matched_mono.clear();
//   matched_st.clear();

//   const SiStripRecHit2D mono = matchedrechit->monoHit();
//   const SiStripRecHit2D st = matchedrechit->stereoHit();
//   //associate the two simple hits separately
//   associateSimpleRecHit(&mono,matched_mono );
//   associateSimpleRecHit(&st, matched_st );
  
//   //save in a vector all the simtrack-id's that are common to mono and stereo hits
//   if(!matched_mono.empty() && !matched_st.empty()){
//     simtrackid.clear(); //final result vector
//     //    std::vector<unsigned int> idcachev;
//     std::vector<SimHitIdpr> idcachev;
//     //for(vector<unsigned int>::iterator mhit=matched_mono.begin(); mhit != matched_mono.end(); mhit++){
//     for(vector<SimHitIdpr>::iterator mhit=matched_mono.begin(); mhit != matched_mono.end(); mhit++){
//       //save only once the ID
//       if(find(idcachev.begin(), idcachev.end(),(*mhit)) == idcachev.end()) {
// 	idcachev.push_back(*mhit);
// 	//save if the stereoID matched the monoID
// 	if(find(matched_st.begin(), matched_st.end(),(*mhit))!=matched_st.end()){
// 	  simtrackid.push_back(*mhit);
// 	  //std::cout << "matched case: saved ID " << (*mhit) << std::endl; 
// 	}
//       }
//     }
//   }
//   return simtrackid;
// }


// std::vector<SimHitIdpr>  PixelHitAssociator::associateProjectedRecHit(const ProjectedSiStripRecHit2D * projectedrechit)
// {
//   StripHits = true;


//   //projectedRecHit is a "matched" rechit with only one component

//   vector<SimHitIdpr> matched_mono;
//   matched_mono.clear();
 
//   const SiStripRecHit2D mono = projectedrechit->originalHit();
//   associateSimpleRecHit(&mono, matched_mono);
//   return matched_mono;
// }

// std::vector<SimHitIdpr>  PixelHitAssociator::associateGSRecHit(const SiTrackerGSRecHit2D * gsrechit)
// {
//   StripHits = false;
//   //GSRecHit is the FastSimulation RecHit that contains the TrackId already

//   vector<SimHitIdpr> simtrackid;
//   simtrackid.clear();
//   SimHitIdpr currentId(gsrechit->simtrackId(), EncodedEventId(gsrechit->eeId()));
//   simtrackid.push_back(currentId);
//   return simtrackid;
// }

// std::vector<PSimHit> PixelHitAssociator::associateMultiRecHit(const SiTrackerMultiRecHit * multirechit){
//   std::vector<const TrackingRecHit*> componenthits = multirechit->recHits();
//   //        std::vector<PSimHit> assimhits;
//   int size=multirechit->weights().size(), idmostprobable=0;
  
//   for (int i=0; i<size; i++){
//     if(multirechit->weight(i)>multirechit->weight(idmostprobable)) idmostprobable=i;
//   }
  
//   //	std::vector<PSimHit> assimhits = associateHit(**mostpropable);
//   //	assimhits.insert(assimhits.end(), asstocurrent.begin(), asstocurrent.end());
//   //std::cout << "Returning " << assimhits.size() << " simhits" << std::endl;
//   return associateHit(*componenthits[idmostprobable]);
// }

// std::vector<SimHitIdpr> PixelHitAssociator::associateMultiRecHitId(const SiTrackerMultiRecHit * multirechit){
//   std::vector<const TrackingRecHit*> componenthits = multirechit->recHits();
//   int size=multirechit->weights().size(), idmostprobable=0;
  
//   for (int i=0; i<size; i++){
//     if(multirechit->weight(i)>multirechit->weight(idmostprobable)) idmostprobable=i;
//   }
  
//   return associateHitId(*componenthits[idmostprobable]);
// }

// std::vector<SimHitIdpr>  PixelHitAssociator::associateGSMatchedRecHit(const SiTrackerGSMatchedRecHit2D * gsmrechit)
// {
//   StripHits = false;
//   //GSRecHit is the FastSimulation RecHit that contains the TrackId already
  
//   vector<SimHitIdpr> simtrackid;
//   simtrackid.clear();
//   SimHitIdpr currentId(gsmrechit->simtrackId(), EncodedEventId(gsmrechit->eeId()));
//   simtrackid.push_back(currentId);
//   return simtrackid;
// }

