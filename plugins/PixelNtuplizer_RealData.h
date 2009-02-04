
#ifndef PixelNtuplizer_RD_h
#define PixelNtuplizer_RD_h

/** \class PixelNtuplizer_RealData
 *
 *
 ************************************************************/

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DQM/TrackerMonitorTrack/interface/MonitorTrackResiduals.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "Alignment/OfflineValidation/interface/TrackerValidationVariables.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"

using namespace reco;
class TTree;
class TFile;
class RectangularPixelTopology;

class PixelNtuplizer_RealData : public edm::EDAnalyzer
{
 public:
  
  explicit PixelNtuplizer_RealData(const edm::ParameterSet& ps);
  virtual ~PixelNtuplizer_RealData();
  virtual void beginJob(const edm::EventSetup& iSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

 protected:
  void fillTrackOnly(const edm::Event&, const edm::EventSetup &, int, int, int, const Track&);
  void fillEvt(const edm::Event&,int NbrTracks);
  void fillDet(const DetId &, uint, const PixelGeomDetUnit*);
  void fillVertex(const PixelGeomDetUnit*);
  void fillClust(const SiPixelCluster&, const RectangularPixelTopology*, const PixelGeomDetUnit*, TrajectoryStateOnSurface&);
  void fillPix(const SiPixelCluster&, const RectangularPixelTopology*, const PixelGeomDetUnit*);
  void fillTrack(TrajectoryStateOnSurface&,const Trajectory&, int);
  
  bool isValidMuonAssoc(const edm::Event& iEvent,const Track& track, int TrackNumber);		   
		   

 private:
  edm::ParameterSet conf_;
  edm::ESHandle<TrackerGeometry> tkGeom_;
  edm::ESHandle<MagneticField> magneticField_;
  // muon association.
    

  TFile* tfile_;
  TTree* t_;  // tree filled on every pixel rec hit
  //  TTree* ts_; // tree filled on every strip rec hit
  TTree* tt_; // tree filled every track

  float bias[5][15][5][15];
  float rms[5][15][5][15];
  float points[5][15][5][15];
  
  void init();
  
  //--- Structures for ntupling:

  struct EvtStruct {

    int run;
    int evtnum;
    int nbrTracks;

    void init();
  } evt_;
  
  struct DetStruct {

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

  struct VertexStruct {

    float r;
    float z;

    void init();
  } vertex_;

  struct ClusterStruct {

    float row;
    float col;
    float x;
    float y;
    float charge;
    float normalized_charge;
    int size;
    int size_x;
    int size_y;
    int maxPixelCol;
    int maxPixelRow;
    int minPixelCol;
    int minPixelRow;
    uint32_t geoId;
    int edgeHitX;
    int edgeHitY;    
    float clust_alpha;
    float clust_beta;

    void init();
  } clust_;


  
  struct MuonInfoStruct {
    int nmuon;
    bool HasOverFlow;
    bool IsGlobalMuon[2];
    bool IsStandAloneMuon[2];
    bool IsTrackerMuon[2];
     bool IsTimeValid[2];
     bool HasGlobalTrack[2];
      bool HasPixelHit[2];
    float timeAtIpInOut[2];
    float errorTime[2];
    //  float momentumDiff[2];
     float trackpt[2];
     float tracketa[2];
     float trackphi[2];
    void init();
  }muoninfo_;

  struct PixInfoStruct {

    int npix;
    float row[200];
    float col[200];
    float adc[200];
    // Just added
    float x[200];
    float y[200];
    float gx[200];
    float gy[200];
    float gz[200];
    int hasOverFlow;
    void init();
  } pixinfo_;
  size_t maxsize_PixInfoStruct_;

  struct RecHitStruct{

    float localX;
    float localY;
    float globalX;
    float globalY;
    float globalZ;
    float residualX;
    float residualY;
    float resErrX;
    float resErrY;
    float hit_errX;
    float hit_errY;
    float resXprime;
    float resXprimeErr;

    void init();
    } rechit_;

  struct TrackStruct{

    float pt;
    float p;
    float px;
    float py;
    float pz;
    float globalTheta;
    float globalEta;
    float globalPhi;
    float localTheta;
    float localPhi;
    float chi2;
    float ndof;
    int foundHits;
    int tracknum;     // number of track processed (correlates with others)

    void init();
    } track_;

  /*  struct TrackerHitStruct{

    float globalX;
    float globalY;
    float globalZ;
    int run;
    int evtnum;
    int tracknum;     // number of track processed (correlates with others)

    void init();
    } trackerhits_; */

  struct TrackOnlyStruct{

    int run;
    int evtnum;
    int tracknum;     // number of track processed (correlates with others)
    int pixelTrack;   // 0 = no, 1 = yes
    int NumPixelHits;
    int NumStripHits;
    int charge;
    float chi2;
    float ndof;
    float theta;
    float d0;
    float dz;
    float p;
    float pt;
    float px;
    float py;
    float pz;
    float phi;
    float eta;
    float vx;
    float vy;
    float vz;
    float muonT0;

    void init();
    } trackonly_;

};

#endif
