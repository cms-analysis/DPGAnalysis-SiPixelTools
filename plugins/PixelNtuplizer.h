#ifndef PixelNtuplizer_h
#define PixelNtuplizer_h

/** \class PixelNtuplizer
 *
 *
 ************************************************************/

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Ref.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

class TTree;
class TFile;
class RectangularPixelTopology;

class PixelNtuplizer : public edm::EDAnalyzer
{
 public:
  
  explicit PixelNtuplizer(const edm::ParameterSet& conf);
  virtual ~PixelNtuplizer();
  virtual void beginJob(const edm::EventSetup& es);
  virtual void endJob();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& es);

 protected:
  void fillClust (const SiPixelCluster& , const RectangularPixelTopology* , const PixelGeomDetUnit*);
  void fillRecHit(SiPixelRecHitCollection::const_iterator, const RectangularPixelTopology *,const PixelGeomDetUnit *);
  void fillDet(DetId &, int );
  void fillSim(std::vector<PSimHit>::const_iterator, unsigned int, const PixelGeomDetUnit *,
  	       const RectangularPixelTopology *, const edm::SimTrackContainer& ); 
  void fillTrack(const edm::SimTrackContainer& trks);
  void fillPix(const SiPixelCluster &, const RectangularPixelTopology *, const PixelGeomDetUnit *);
  void fillEvt(const edm::Event& );

 private:
  edm::ParameterSet conf_;
  edm::InputTag src_;

  TFile* tfile_;
  TTree* t_;
  
  bool checkType_; // do we check that the simHit associated with recHit is of the expected particle type ?
  int genType_; // the type of particle that the simHit associated with recHits should be

  void init();
  
  //--- Structures for ntupling:
  struct evt
  {
    int run;
    int evtnum;
    
    void init();
  } evt_;
  
  struct Det 
  {
    float thickness;
    int cols;
    int rows;
    int layer;
    int ladder;
    int module;
    int disk;
    int blade;
    int panel;
    int plaquette;

    void init();
  } det_;

  struct vertex
  {
    int num;
    float r;
    float z;

    void init();
  } vertex_;

  struct track
  {
    float eta;
    float phi;

    void init();
  } track_;

  struct sim 
  {
    float x;
    float y;
    float px;
    float py;
    float pz;
    float eloss;
    float phi;
    float theta;
    int subdetid;
    int isflipped;
    // alpha and beta are related to phi and theta, but
    // more standard:
    float alpha;   
    float beta;
    int PID;
    unsigned int TID;
    float x1, x2; // entry and exit points
    float y1, y2;
    float z1, z2;
    float row1, row2; //entry and exit row/columns
    float col1, col2;
    float gx1, gx2; //global locales
    float gy1, gy2;
    float gz1, gz2;

    int proc; // ID of the process which created the track which created the PSimHit
    float trk_eta; // wrong, don't use it !!!
    float trk_phi; // wrong, don't use it !!!
    float trk_eta2;
    float trk_phi2; 


    void init();
  } sim_;

  struct clust 
  {
    float row;
    float col;
    float x;
    float y;
    float charge;
    int size;
    int size_x;
    int size_y;
    int maxPixelCol;
    int maxPixelRow;
    int minPixelCol;
    int minPixelRow;
    unsigned int geoId;
    int edgeHitX;
    int edgeHitY;    
    float clust_alpha; // alpha from cluster position w.r.t detector center
    float clust_beta;  // beta from cluster position w.r.t detector center

    void init();
  } clust_;
  
  static const int maxpix = 100;
  struct pixinfo
  {
    int npix;
    float row[maxpix];
    float col[maxpix];
    float adc[maxpix];
    // Just added
    float x[maxpix];
    float y[maxpix];
    float gx[maxpix];
    float gy[maxpix];
    float gz[maxpix];
    
    void init();
  } pixinfo_;


  struct RecHit 
  {
    float x;
    float y;
    float xx;
    float xy;
    float yy;
    float row;
    float col;
    float gx;
    float gy;
    float gz;

    float probx;
    float proby;
    
    int nsimhit; // number of simhits associated with a rechit
    int binq;

    void init();
  } recHit_;

};


#endif
