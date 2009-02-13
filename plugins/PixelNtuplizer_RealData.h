
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
#include "TObject.h"

using namespace reco;
class TTree;
class TFile;
class RectangularPixelTopology;
class TObject;


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
  bool isOffTrackHits(const edm::Event& iEvent, const edm::EventSetup& iSetup,const RectangularPixelTopology*, uint32_t geoId );		   

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

    int n_neighbour_clust; 
   

    void init();
  } clust_;
  
  struct MuonInfoStruct{// : public TObject {
    //  public:
    //    MuonInfoStruct(){;}
    //    ~MuonInfoStruct(){;}
    Float_t timeAtIpInOut[2];
    Float_t errorTime[2];
    Float_t IsGlobalMuon[2];
    Float_t IsStandAloneMuon[2];
    Float_t IsTrackerMuon[2];
    Float_t IsTimeValid[2];
    Float_t HasGlobalTrack[2];
    Float_t HasPixelHit[2];
    //  Float_t momentumDiff[2];
    Float_t trackpt[2];
    Float_t tracketa[2];
    Float_t trackphi[2];
    Int_t HasOverFlow;
    Int_t nMuon;
    void init(){
      nMuon=0; 
      HasOverFlow=0;
      IsGlobalMuon[0]=IsGlobalMuon[1]=IsStandAloneMuon[0]=IsStandAloneMuon[1]=0.0;
      IsTrackerMuon[0]=IsTrackerMuon[1]=HasGlobalTrack[0]=HasGlobalTrack[1]=HasPixelHit[0]=HasPixelHit[1]=0.0;
      timeAtIpInOut[0]=timeAtIpInOut[1]=errorTime[0]=errorTime[1]=0.0;
      trackpt[0]=trackpt[1]=tracketa[0]=tracketa[1]=trackphi[0]=trackphi[1]=-100;
      
    }
  }muoninfo_;


 struct AllClustStruct
  {
    int n_allclust; // should be the same as n_neighbour_clust 
    float allclust_row[10000];
    float allclust_col[10000];
    float allclust_x[10000];
    float allclust_y[10000];
    float allclust_charge[10000];
    float allclust_nor_charge[10000];
    int allclust_size[10000];
    int allclust_size_x[10000];
    int allclust_size_y[10000];
    int allclust_maxPixelCol[10000];
    int allclust_maxPixelRow[10000];
    int allclust_minPixelCol[10000];
    int allclust_minPixelRow[10000];
    uint32_t allclust_geoId[10000];
    int allclust_edgeHitX[10000];
    int allclust_edgeHitY[10000];    
    float allclust_dist[10000];
    int allclust_hasOverFlow;

    void init(){ 
      n_allclust = 0;
      allclust_hasOverFlow = false;
      allclust_row[0] = allclust_col[0] = allclust_x[0] = allclust_y[0] = allclust_charge[0] = allclust_nor_charge[0] = 0.0;
      allclust_size[0] = allclust_size_x[0] =  allclust_size_y[0] = allclust_maxPixelCol[0] = allclust_maxPixelRow[0] = allclust_minPixelCol[0] = allclust_minPixelRow[0] = 0;
      allclust_geoId[0] = 0;
      allclust_edgeHitX[0] = allclust_edgeHitY[0] =  0;
      allclust_dist[0] = 0.0;
							 

    }
  } allclustinfo_;
  size_t maxsize_AllClustInfoStruct_;

 struct AllPixInfoStruct 
  {
    int allpix_npix;
    float allpix_row[10000];
    float allpix_col[10000];
    float allpix_adc[10000];
    int allpix_hasOverFlow;
    
    void init(){
      allpix_npix = 0;
      allpix_hasOverFlow = 0;
      allpix_row[0] =allpix_col[0] = allpix_adc[0]= 0.;
      

    }
  } allpixinfo_;
  size_t maxsize_AllPixInfoStruct_;

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
