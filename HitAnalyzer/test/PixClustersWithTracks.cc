// Description: TO test the pixel clusters with tracks (full)
// Author: Danek Kotlinski 
// Creation Date:  Initial version. 3/06
// Fails in V71 because of TrackingRecHit. <--- fix 
// Move to getByToken, 80X, 4/16
// Extend to phase1, 81X, 9/16 dk
//--------------------------------------------
#include <memory>
#include <string>
#include <iostream>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
//#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"

//#include "DataFormats/Common/interface/EDProduct.h"

//#include "DataFormats/SiPixelCluster/interface/SiPixelClusterCollection.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/DetId/interface/DetId.h"

//#include "DataFormats/SiPixelDetId/interface/PXBDetId.h" 
//#include "DataFormats/SiPixelDetId/interface/PXFDetId.h" 
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"


#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
//#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
//#include "Geometry/CommonTopologies/interface/PixelTopology.h"

// For L1
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

// For HLT
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"


// For tracks 
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"

//#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
//#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"

#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"

//#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
//#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

//#include "Alignment/OfflineValidation/interface/TrackerValidationVariables.h"


//#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
//#include "TrackingTools/PatternTools/interface/Trajectory.h"
//#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
//#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
//#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>



// hit pattern 
#include "DataFormats/TrackReco/interface/HitPattern.h"

// For luminisoty
#include "DataFormats/Luminosity/interface/LumiSummary.h"
#include "DataFormats/Common/interface/ConditionsInEdm.h"

// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// For ROOT
//#include <TROOT.h>
//#include <TChain.h>
//#include <TFile.h>
//#include <TF1.h>
#include <TH2.h>
#include <TH1.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TVector3.h>

//#define L1
//#define HLT
//#define LUMI

//#define LS_STUDIES
//#define ALIGN_STUDIES
//#define OVERLAP_STUDIES
//#define STUDY_LAY1
//#define SINGLE_MODULES
//#define CLU_SHAPE    // L1, use one or the other but not both
//#define CLU_SHAPE_L2  // same for L2 
#define PHI_PROFILES
//#define STUDY_ONEMOD

//#define TRAJECTORY // needs a track refit

#ifdef TRAJECTORY
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
//#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
//#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"
//#include "TrackingTools/PatternTools/interface/Trajectory.h"
//#include "TrackingTools/PatternTools/interface/TrajectoryBuilder.h"
//#include "TrackingTools/TrackFitters/interface/TrajectoryFitter.h"
//#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
//#include <TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
//#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
//#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
//#include "DataFormats/TrackerRecHit2D/interface/TkCloner.h"
#endif


const bool isData = true; // set false for MC

using namespace std;

class PixClustersWithTracks : public edm::EDAnalyzer {
 public:
  
  explicit PixClustersWithTracks(const edm::ParameterSet& conf);  
  virtual ~PixClustersWithTracks();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginRun(const edm::EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
#if defined(CLU_SHAPE) || defined(CLU_SHAPE_L2) 
  void histogramClus(float cha, int size, int sizex, int sizey, bool same, int corner);
  void histogramPix(float pixchar, int size, int sizex, int sizey, bool same, int corner);
#endif
  
 private:
  edm::ParameterSet conf_;
  edm::InputTag src_;
  //const static bool PRINT = false;
  bool phase1_;
  bool PRINT;
  bool DEBUG;
  bool Normalise;
  float countTracks, countGoodTracks, countTracksInPix, countPVs, countEvents, countLumi;  
  float count1, count2, count3, count4;
  float countTracks1, countTracks2, countTracks3, countTracks4, countTracks5;
  int select1, select2;
  int selectLayer, selectLadder, selectModule;

  // Needed for the ByToken method
  //edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > myClus;
  //edm::EDGetTokenT<SiPixelRecHitCollection> PixelRecHitToken;
  edm::EDGetTokenT<LumiSummary> LumiToken;
  edm::EDGetTokenT<edm::ConditionsInLumiBlock> ConditionsInLumiBlockToken;
  edm::EDGetTokenT<L1GlobalTriggerReadoutRecord> L1TrigReadoutToken;
  edm::EDGetTokenT<edm::TriggerResults> TrigResultsToken;
  edm::EDGetTokenT<reco::VertexCollection> VertexCollectionToken;
  edm::EDGetTokenT<std::vector<reco::Track>> TrackToken;

  TH1D *hcharge1,*hcharge2, *hcharge3, *hcharge4, 
    *hcharge5, *hcharge6, *hcharge7; 
  TH1D *hsize1,*hsize2,*hsize3,*hsize4, 
    *hsizex1,*hsizex2,*hsizex3,*hsizex4,
    *hsizey1,*hsizey2,*hsizey3,*hsizey4;
  TH1D *hsize5,*hsize6,*hsize7,  // ADD FPIX
       *hsizex5,*hsizex6,*hsizex7,
       *hsizey5,*hsizey6,*hsizey7;
  TH1D *hcharge1n, *hsize1n, *hsizex1n, *hsizey1n; 
  TH1D *hcharge2b, *hsize2b, *hsizex2b, *hsizey2b; 
  TH1D *hcharge2g, *hsize2g, *hsizex2g, *hsizey2g; 

  TH1D *hclusPerTrk1,*hclusPerTrk2,*hclusPerTrk3,*hclusPerTrk4,
    *hclusPerTrk5,*hclusPerTrk6,*hclusPerTrk7;
  TH1D *hclusPerLay1,*hclusPerLay2,*hclusPerLay3,*hclusPerLay4;
  TH1D *hclusPerDisk1,*hclusPerDisk2,*hclusPerDisk3;
  TH1D *hclusPerTrk,*hclusPerTrkB,*hclusPerTrkF;

  TH2F *hDetMap1, *hDetMap2, *hDetMap3, *hDetMap4;  // clusters 
  TH2F *hcluDetMap1, *hcluDetMap2,*hcluDetMap3,*hcluDetMap4; // MODULE PROJECTION 

  TProfile2D *hsizeDets1, *hsizeDets2, *hsizeDets3, *hsizeDets4;
  TProfile2D *hchargeDets1, *hchargeDets2, *hchargeDets3, *hchargeDets4;
  TProfile2D *hpixchargeDets1, *hpixchargeDets2, *hpixchargeDets3, *hpixchargeDets4;

  TH2F *hpvxy, *hetaphiMap1, *hetaphiMap2,*hetaphiMap3,*hetaphiMap4; // eta vs PHI

  TH2F *hsizeyz1,*hsizeyz2,*hsizeyz3,*hsizeyz4 ;

  TH1D *hladder1id,*hladder2id,*hladder3id,*hladder4id,
    *hz1id,*hz2id,*hz3id,*hz4id;
  TH1D *hpladder1id,*hpladder2id,*hpladder3id,*hpladder4id,
    *hpz1id,*hpz2id,*hpz3id,*hpz4id;
  TH1D *hcols1,*hcols2,*hcols3,*hrows1,*hrows2,*hrows3,*hcols4,*hrows4;
  TH1D *hpcols1,*hpcols2,*hpcols3,*hprows1,*hprows2,*hprows3,*hpcols4,*hprows4;

  TH1D *hcols11, *hrows11; // test 

  TH1D *hpvz, *hpvr, *hNumPv, *hNumPvClean;
  TH1D *hPt, *hEta, *hDz, *hD0,*hzdiff, *hPhi;
  TH1D *hPtP, *hEtaP, *hDzP, *hD0P,*hPhiP;
  TH1D *hPhi0;
  TH1D *hPhi1,*hPhi2,*hPhi3,*hPhi4; // ,*hPhi5,*hPhi6,*hPhi7;
  TH1D *hEta1,*hEta2,*hEta3,*hEta4,*hEta5,*hEta6,*hEta7;
  TH1D *hEta0, *hEta10,*hEta20,*hEta30,*hEta40;

  TH1D *hEta11,*hEta12, *hPhi11, *hPhi12;

  TH2F *hbpixXY, *hfpixXY1,*hfpixXY2,*hfpixXY3, 
    *hzphi1,*hzphi2,*hzphi3,*hzphi4;
  TH2F *htest,*htest1,*htest2,*htest3, *htest4;

  TH1D *hProbabilityXYBpix, *hProbabilityQBpix,*hProbabilityXYFpix, *hProbabilityQFpix;

  // pixel histos 
  TH1D *hpixcharge1,*hpixcharge2, *hpixcharge3, *hpixcharge4, *hpixcharge5;
  TH1D *hpixcharge1n,*hpixcharge2b,*hpixcharge2g;
  TH2F *hpixDetMap1, *hpixDetMap2, *hpixDetMap3, *hpixDetMap4;  // in a  modules

#if defined(CLU_SHAPE) || defined(CLU_SHAPE_L2) 
  TH1D *hcharge111, *hpixcharge111;
  TH1D *hcharge111_1, *hcharge111_2, *hcharge111_3, *hcharge111_4 ;
  
  TH1D *hcharge112, *hpixcharge112;
  TH1D *hcharge112_1, *hpixcharge112_1;
  TH1D *hcharge112_2, *hpixcharge112_2;
  TH1D *hcharge121, *hpixcharge121;
  TH1D *hcharge121_1, *hpixcharge121_1;
  TH1D *hcharge121_2, *hpixcharge121_2;
  TH1D *hcharge122, *hpixcharge122;
  TH1D *hcharge122_1, *hpixcharge122_1;
  TH1D *hcharge122_2, *hpixcharge122_2;

  TH1D *hcharge113, *hpixcharge113;
  TH1D *hcharge131, *hpixcharge131;
  TH1D *hcharge123, *hpixcharge123;
  TH1D *hcharge132, *hpixcharge132;

  TH1D *hcharge130, *hpixcharge130;
  TH1D *hcharge140, *hpixcharge140;
  TH1D *hcharge160, *hpixcharge160;
  TH1D *hcharge180, *hpixcharge180;
  TH1D *hcharge181, *hpixcharge181;
  TH1D *hcharge182, *hpixcharge182;
  TH1D *hcharge183, *hpixcharge183;
  TH1D *hcharge184, *hpixcharge184;

  TH2F *hpixDetMap35,*hpixDetMap36, *hpixDetMap37, *hpixDetMap38, *hpixDetMap39;

#endif // CLU_SHAPE

#ifdef SINGLE_MODULES
  // single module 
  TH2F *hpixDetMap10, *hpixDetMap20, *hpixDetMap30, *hpixDetMap40;
  TH2F *hpixDetMap11, *hpixDetMap12, *hpixDetMap13, *hpixDetMap14, *hpixDetMap15;
  TH2F *hpixDetMap16, *hpixDetMap17, *hpixDetMap18, *hpixDetMap19;
  TH2F *hpixDetMap21, *hpixDetMap22, *hpixDetMap23, *hpixDetMap24, *hpixDetMap25;
  TH2F *hpixDetMap26, *hpixDetMap27, *hpixDetMap28, *hpixDetMap29;
  TH2F *hpixDetMap31, *hpixDetMap32, *hpixDetMap33, *hpixDetMap34, *hpixDetMap35;
  TH2F *hpixDetMap36, *hpixDetMap37, *hpixDetMap38, *hpixDetMap39;
  TH2F *hpixDetMap41, *hpixDetMap42, *hpixDetMap43, *hpixDetMap44, *hpixDetMap45;
  TH2F *hpixDetMap46, *hpixDetMap47, *hpixDetMap48, *hpixDetMap49;
#endif


#ifdef L1
  TH1D *hl1a, *hl1t, *hlt1;
#endif

  TH1D *hclusBpix, *hpixBpix;
  TH1D *htracks, *htracksGood, *htracksGoodInPix;
  TH1D *hls, *hls0, *hbx, *hbx0;
  TH1D *hstatus;

#ifdef USE_PROFILES
  TProfile *hclumult1,  *hclumult2,  *hclumult3;
  TProfile *hclumultx1, *hclumultx2, *hclumultx3;
  TProfile *hclumulty1, *hclumulty2, *hclumulty3;
  TProfile *hcluchar1,  *hcluchar2,  *hcluchar3;

  TProfile *hclumultld1,  *hclumultld2,  *hclumultld3;
  TProfile *hclumultxld1, *hclumultxld2, *hclumultxld3;
  TProfile *hclumultyld1, *hclumultyld2, *hclumultyld3;
  TProfile *hclucharld1,  *hclucharld2,  *hclucharld3;

  TProfile *htracksls,  *hpvsls, *htrackslsn,  *hpvslsn;
  TProfile *hmult1,  *hmult2,  *hmult3;
  TProfile *hclusPerTrkVsEta,*hclusPerTrkVsPt,*hclusPerTrkVsls,*hclusPerTrkVsEtaB,*hclusPerTrkVsEtaF; 
#endif 

#ifdef PHI_PROFILES
  TProfile *hclumultxPhi1, *hclumultxPhi2; // , *hclumultx3, *hclumultx4;
  TProfile *hclumultyPhi1, *hclumultyPhi2; // *hclumulty3, *hclumulty4;
  TProfile *hclucharPhi1, *hclucharPhi2; //  *hcluchar3,  *hcluchar4;
  TProfile *hpixcharPhi1, *hpixcharPhi2; //  *hpixchar3,  *hpixchar4;
#endif

#ifdef LUMI
  TProfile *hintgl, *hinstl, *hbeam1, *hbeam2;
#endif

#ifdef STUDY_LAY1
  TH1D *hPt11, *hEta11, *hsizey11, *hsizex11, *hcharge11, *hpixcharge11;
  TH1D *hPt14, *hEta14, *hsizey14, *hsizex14, *hcharge14, *hpixcharge14;
  TH1D *hPt15, *hEta15, *hsizey15, *hsizex15, *hcharge15, *hpixcharge15;
#endif // STUDY_LAY1

#ifdef ALIGN_STUDIES
  TH1D *recHitXError1,*recHitXError2,*recHitXError3; 
  TH1D *recHitYError1,*recHitYError2,*recHitYError3; 
  TH1D *recHitXAlignError1,*recHitXAlignError2,*recHitXAlignError3; 
  TH1D *recHitYAlignError1,*recHitYAlignError2,*recHitYAlignError3; 
  TH1D *recHitXError4,*recHitXError5,*recHitXError6,*recHitXError7; 
  TH1D *recHitYError4,*recHitYError5,*recHitYError6,*recHitYError7; 
  TH1D *recHitXAlignError4,*recHitXAlignError5,*recHitXAlignError6,*recHitXAlignError7; 
  TH1D *recHitYAlignError4,*recHitYAlignError5,*recHitYAlignError6,*recHitYAlignError7; 
  TProfile *hErrorXB,*hErrorYB,*hErrorXF,*hErrorYF; 
  TProfile *hAErrorXB,*hAErrorYB,*hAErrorXF,*hAErrorYF; 
#endif

#ifdef LS_STUDIES
  TProfile *hcharCluls, *hcharPixls, *hsizeCluls, *hsizeXCluls;
  TProfile *hcharCluls1, *hcharPixls1, *hsizeCluls1, *hsizeXCluls1;
  TProfile *hcharCluls2, *hcharPixls2, *hsizeCluls2, *hsizeXCluls2;
  TProfile *hcharCluls3, *hcharPixls3, *hsizeCluls3, *hsizeXCluls3;
  TProfile *hclusls; //   *hpixls;
  TProfile *hclusls1, *hclusls2,*hclusls3;
  TProfile *hclubx, *hpvbx, *htrackbx; // *hcharClubx, *hcharPixbx,*hsizeClubx, *hsizeYClubx;
#endif

#ifdef OVERLAP_STUDIES
  TH1D *hcluphi1,*hcluphi2,*hcluphi3,*hcluphi4 ;
#endif

  TH1D *hbpixHits, *hbpixLayers, *htrackLayers;
  TProfile *hbpixLayersPt, *htrackLayersPt, *hbpixLayersEta, *htrackLayersEta;

#ifdef STUDY_ONEMOD
  TProfile2D *hsizeMap1, *hsizeXMap1, *hsizeYMap1,*hclucharMap1,*hpixcharMap1;
#endif


};
/////////////////////////////////////////////////////////////////
// Contructor,
PixClustersWithTracks::PixClustersWithTracks(edm::ParameterSet const& conf) 
//  : conf_(conf), src_(conf.getParameter<edm::InputTag>( "src" )) { }
  : conf_(conf), Normalise(true) { 

  phase1_ = conf.getUntrackedParameter<bool>("phase1",false);
  PRINT = conf.getUntrackedParameter<bool>("Verbosity",false);
  Normalise = conf.getUntrackedParameter<bool>("Normalise",true);
  src_ =  conf.getParameter<edm::InputTag>( "src" );
  select1 = conf.getUntrackedParameter<int>("Select1",0);
  select2 = conf.getUntrackedParameter<int>("Select2",0);

  if(PRINT) cout<<" Construct: Normalise = "<<Normalise<<endl;

  // +-LDDM L layer DD ladder M module e.g. 1065 for BmI8/1/6/1  
  if(select1==201) { //select specifix module 
    int sign  = select2/abs(select2); // negative sign for outer O ladders
    selectLayer = abs(select2)/1000; // layer
    selectLadder = (abs(select2)%1000)/10; // ladder
    selectModule = (abs(select2)%10); // module
    selectLadder *= sign;
    if(selectModule>4) selectModule = -(selectModule-4); // 8 7 6 5 - 1 2 3 4
  }
  // Consumes 
  // For the ByToken method
  //myClus = consumes<edmNew::DetSetVector<SiPixelCluster> >(conf.getParameter<edm::InputTag>( "src" ));
  //PixelRecHitToken = consumes  <SiPixelRecHitCollection>("...") ;
  LumiToken                  = consumes <LumiSummary>(edm::InputTag("lumiProducer"));
  ConditionsInLumiBlockToken = consumes <edm::ConditionsInLumiBlock> (edm::InputTag("conditionsInEdm"));
  L1TrigReadoutToken         = consumes <L1GlobalTriggerReadoutRecord>(edm::InputTag("gtDigis"));
  TrigResultsToken           = consumes <edm::TriggerResults>(edm::InputTag("TriggerResults","","HLT"));
  VertexCollectionToken      = consumes <reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
  TrackToken                 = consumes <std::vector<reco::Track>>(src_) ;

  DEBUG = false; // debug print flag

}

// Virtual destructor needed.
PixClustersWithTracks::~PixClustersWithTracks() { }  

// ------------ method called at the begining   ------------
void PixClustersWithTracks::beginRun(const edm::EventSetup& iSetup) {
  cout << "BeginRun, Verbosity =  " <<PRINT<<" Phase1 "<<phase1_<<endl;
}

// ------------ method called at the begining   ------------
void PixClustersWithTracks::beginJob() {
  cout << "BeginJob, Verbosity " <<PRINT<<endl;

  countTracks=0.; countGoodTracks=0.; countTracksInPix=0.; countPVs=0.; countEvents=0.; countLumi=0.;  
  count1=0.; count2=0.; count3=0.; count4=0.; 
  countTracks1=0; countTracks2=0; countTracks3=0; countTracks4=0, countTracks5=0; 
  edm::Service<TFileService> fs;

  int sizeH=20;
  float lowH = -0.5;
  float highH = 19.5;
  hclusPerTrk1 = fs->make<TH1D>( "hclusPerTrk1", "Clus per track l1",
			    sizeH, lowH, highH);
  hclusPerTrk2 = fs->make<TH1D>( "hclusPerTrk2", "Clus per track l2",
			    sizeH, lowH, highH);
  hclusPerTrk3 = fs->make<TH1D>( "hclusPerTrk3", "Clus per track l3",
			    sizeH, lowH, highH);
  hclusPerTrk4 = fs->make<TH1D>( "hclusPerTrk4", "Clus per track l4",
			    sizeH, lowH, highH);
  hclusPerTrk5 = fs->make<TH1D>( "hclusPerTrk5", "Clus per track d1",
			    sizeH, lowH, highH);
  hclusPerTrk6 = fs->make<TH1D>( "hclusPerTrk6", "Clus per track d2",
			    sizeH, lowH, highH);
  hclusPerTrk7 = fs->make<TH1D>( "hclusPerTrk7", "Clus per track d3",
			    sizeH, lowH, highH);

  hclusPerTrk = fs->make<TH1D>( "hclusPerTrk", "Clus per track",
			    sizeH, lowH, highH);
  hclusPerTrkB = fs->make<TH1D>( "hclusPerTrkB", "B Clus per track",
			    sizeH, lowH, highH);
  hclusPerTrkF = fs->make<TH1D>( "hclusPerTrkF", "F Clus per track",
			    sizeH, lowH, highH);

  sizeH=2000;
  highH = 3999.5;
  hclusPerLay1 = fs->make<TH1D>( "hclusPerLay1", "Clus per layer l1",
			    sizeH, lowH, highH);
  hclusPerLay2 = fs->make<TH1D>( "hclusPerLay2", "Clus per layer l2",
			    sizeH, lowH, highH);
  hclusPerLay3 = fs->make<TH1D>( "hclusPerLay3", "Clus per layer l3",
			    sizeH, lowH, highH);
  hclusPerLay4 = fs->make<TH1D>( "hclusPerLay4", "Clus per layer l4",
			    sizeH, lowH, highH);
  hclusPerDisk1 = fs->make<TH1D>( "hclusPerDisk1", "Clus per disk 1",
			    sizeH, lowH, highH);
  hclusPerDisk2 = fs->make<TH1D>( "hclusPerDisk2", "Clus per disk 2",
			    sizeH, lowH, highH);
  hclusPerDisk3 = fs->make<TH1D>( "hclusPerDisk3", "Clus per disk 3",
			    sizeH, lowH, highH);
 
  hcharge1 = fs->make<TH1D>( "hcharge1", "Clu charge l1", 800, 0.,800.); //in ke
  hcharge2 = fs->make<TH1D>( "hcharge2", "Clu charge l2", 400, 0.,400.);
  hcharge3 = fs->make<TH1D>( "hcharge3", "Clu charge l3", 400, 0.,400.);
  hcharge4 = fs->make<TH1D>( "hcharge4", "Clu charge l4", 400, 0.,400.);
  hcharge5 = fs->make<TH1D>( "hcharge5", "Clu charge d1", 400, 0.,400.);
  hcharge6 = fs->make<TH1D>( "hcharge6", "Clu charge d2", 400, 0.,400.);
  hcharge7 = fs->make<TH1D>( "hcharge7", "Clu charge d3", 400, 0.,400.);
  hcharge1n= fs->make<TH1D>( "hcharge1n","Clu charge l1", 800, 0.,800.); //in ke
  hcharge2b= fs->make<TH1D>( "hcharge2b", "Clu charge l2", 400, 0.,400.);
  hcharge2g= fs->make<TH1D>( "hcharge2g", "Clu charge l2", 400, 0.,400.);

  hsize1 = fs->make<TH1D>( "hsize1", "layer 1 clu size",300,-0.5,299.5);
  hsize2 = fs->make<TH1D>( "hsize2", "layer 2 clu size",300,-0.5,299.5);
  hsize3 = fs->make<TH1D>( "hsize3", "layer 3 clu size",300,-0.5,299.5);
  hsize4 = fs->make<TH1D>( "hsize4", "layer 4 clu size",300,-0.5,299.5);
  hsize5 = fs->make<TH1D>( "hsize5", "disk 1 clu size",100,-0.5,99.5);
  hsize6 = fs->make<TH1D>( "hsize6", "disk 2 clu size",100,-0.5,99.5);
  hsize7 = fs->make<TH1D>( "hsize7", "disk 3 clu size",100,-0.5,99.5);
  hsize1n= fs->make<TH1D>( "hsize1n","layer 1 clu size",300,-0.5,299.5);
  hsize2b= fs->make<TH1D>( "hsize2b","layer 2 clu size",300,-0.5,299.5);
  hsize2g= fs->make<TH1D>( "hsize2g","layer 2 clu size",300,-0.5,299.5);

  hsizex1 = fs->make<TH1D>( "hsizex1", "lay1 clu size in x",
		      20,-0.5,19.5);
  hsizex2 = fs->make<TH1D>( "hsizex2", "lay2 clu size in x",
		      20,-0.5,19.5);
  hsizex3 = fs->make<TH1D>( "hsizex3", "lay3 clu size in x",
		      20,-0.5,19.5);
  hsizex4 = fs->make<TH1D>( "hsizex4", "lay4 clu size in x",
		      20,-0.5,19.5);
  hsizex5 = fs->make<TH1D>( "hsizex5", "d1 clu size in x",
		      20,-0.5,19.5);
  hsizex6 = fs->make<TH1D>( "hsizex6", "d2 clu size in x",
		      20,-0.5,19.5);
  hsizex7 = fs->make<TH1D>( "hsizex7", "d2 clu size in x",
		      20,-0.5,19.5);
  hsizex1n= fs->make<TH1D>( "hsizex1n","lay1 clu size in x",
		      20,-0.5,19.5);
  hsizex2b= fs->make<TH1D>( "hsizex2b","layer 2 clu size x",20,-0.5,19.5);
  hsizex2g= fs->make<TH1D>( "hsizex2g","layer 2 clu size x",20,-0.5,19.5);

  hsizey1 = fs->make<TH1D>( "hsizey1", "lay1 clu size in y",
		      30,-0.5,29.5);
  hsizey2 = fs->make<TH1D>( "hsizey2", "lay2 clu size in y",
		      30,-0.5,29.5);
  hsizey3 = fs->make<TH1D>( "hsizey3", "lay3 clu size in y",
		      30,-0.5,29.5);
  hsizey4 = fs->make<TH1D>( "hsizey4", "lay4 clu size in y",
		      30,-0.5,29.5);
  hsizey5 = fs->make<TH1D>( "hsizey5", "d1 clu size in y",
		      30,-0.5,29.5);
  hsizey6 = fs->make<TH1D>( "hsizey6", "d2 clu size in y",
		      30,-0.5,29.5);
  hsizey7 = fs->make<TH1D>( "hsizey7", "d3 clu size in y",
		      30,-0.5,29.5);
  hsizey1n= fs->make<TH1D>( "hsizey1n", "lay1 clu size in y",
		      30,-0.5,29.5);
  hsizey2b = fs->make<TH1D>( "hsizey2b", "lay2 clu size in y",
		      30,-0.5,29.5);
  hsizey2g = fs->make<TH1D>( "hsizey2g", "lay2 clu size in y",
		      30,-0.5,29.5);
  
  // // dets hit per event
  // hDetsMap1 = fs->make<TH2F>("hDetsMap1"," hit modules per event",
  // 			     9,-4.5,4.5,13,-6.5,6.5);
  // hDetsMap1->SetOption("colz");
  // hDetsMap2 = fs->make<TH2F>("hDetsMap2"," hit modules per event",
  // 			     9,-4.5,4.5,29,-14.5,14.5);
  // hDetsMap2->SetOption("colz");
  // hDetsMap3 = fs->make<TH2F>("hDetsMap3"," hit modules per event",
  // 			     9,-4.5,4.5,45,-22.5,22.5);
  // hDetsMap3->SetOption("colz");
  // hDetsMap4 = fs->make<TH2F>("hDetsMap4"," hit modules per event",
  // 			     9,-4.5,4.5,65,-32.5,32.5);
  // hDetsMap4->SetOption("colz");

  // clus per det
  hDetMap1 = fs->make<TH2F>("hcluDetMap1"," clusters L1 per event",
			    9,-4.5,4.5,13,-6.5,6.5);
  hDetMap1->SetOption("colz");
  hDetMap2 = fs->make<TH2F>("hcluDetMap2"," clusters L2 per event",
			    9,-4.5,4.5,29,-14.5,14.5);
  hDetMap2->SetOption("colz");
  hDetMap3 = fs->make<TH2F>("hcluDetMap3"," clusters L3 per event",
			    9,-4.5,4.5,45,-22.5,22.5);
  hDetMap3->SetOption("colz");
  hDetMap4 = fs->make<TH2F>("hcluDetMap4"," clusters L4 per event",
			    9,-4.5,4.5,65,-32.5,32.5);
  hDetMap4->SetOption("colz");


  hcluDetMap1 = fs->make<TH2F>( "hclusMap1", "clu det layer 1",
				416,0.,416.,160,0.,160.);
  hcluDetMap2 = fs->make<TH2F>( "hclusMap2", "clu det layer 2",
				416,0.,416.,160,0.,160.);
  hcluDetMap3 = fs->make<TH2F>( "hclusMap3", "clu det layer 3",
				416,0.,416.,160,0.,160.);
  hcluDetMap4 = fs->make<TH2F>( "hclusMap4", "clu det layer 4",
				416,0.,416.,160,0.,160.);

  // clu size per det
  hsizeDets1 = fs->make<TProfile2D>("hsizeDets1"," cluster size L1",
				9,-4.5,4.5,13,-6.5,6.5,0.,1000.);
  hsizeDets1->SetOption("colz");
  hsizeDets2 = fs->make<TProfile2D>("hsizeDets2"," cluster size L2",
				9,-4.5,4.5,29,-14.5,14.5,0.,1000.);
  hsizeDets2->SetOption("colz");
  hsizeDets3 = fs->make<TProfile2D>("hsizeDets3"," cluster size L3",
				9,-4.5,4.5,45,-22.5,22.5,0.,1000.);
  hsizeDets3->SetOption("colz");
  hsizeDets4 = fs->make<TProfile2D>("hsizeDets4"," cluster size L4",
				9,-4.5,4.5,65,-32.5,32.5,0.,1000.);
  hsizeDets4->SetOption("colz");

  hchargeDets1 = fs->make<TProfile2D>("hchargeDets1"," cluster charge L1",
				      9,-4.5,4.5,13,-6.5,6.5,0.,1000.);
  hchargeDets1->SetOption("colz");
  hchargeDets2 = fs->make<TProfile2D>("hchargeDets2"," cluster charge L2",
				9,-4.5,4.5,29,-14.5,14.5,0.,1000.);
  hchargeDets2->SetOption("colz");
  hchargeDets3 = fs->make<TProfile2D>("hchargeDets3"," cluster charge L3",
				9,-4.5,4.5,45,-22.5,22.5,0.,1000.);
  hchargeDets3->SetOption("colz");
  hchargeDets4 = fs->make<TProfile2D>("hchargeDets4"," cluster charge L4",
				9,-4.5,4.5,65,-32.5,32.5,0.,1000.);
  hchargeDets4->SetOption("colz");

  hpixchargeDets1 = fs->make<TProfile2D>("hpixchargeDets1"," cluster charge L1",
				      9,-4.5,4.5,13,-6.5,6.5,0.,1000.);
  hpixchargeDets1->SetOption("colz");
  hpixchargeDets2 = fs->make<TProfile2D>("hpixchargeDets2"," cluster charge L2",
				9,-4.5,4.5,29,-14.5,14.5,0.,1000.);
  hpixchargeDets2->SetOption("colz");
  hpixchargeDets3 = fs->make<TProfile2D>("hpixchargeDets3"," cluster charge L3",
				9,-4.5,4.5,45,-22.5,22.5,0.,1000.);
  hpixchargeDets3->SetOption("colz");
  hpixchargeDets4 = fs->make<TProfile2D>("hpixchargeDets4"," cluster charge L4",
				9,-4.5,4.5,65,-32.5,32.5,0.,1000.);
  hpixchargeDets4->SetOption("colz");

  hladder1id = fs->make<TH1D>( "hladder1id", "Clus in ladder L1", 13, -6.5, 6.5);
  hladder2id = fs->make<TH1D>( "hladder2id", "Clus in ladder L2", 29, -14.5, 14.5);
  hladder3id = fs->make<TH1D>( "hladder3id", "Clus in adder L3", 45, -22.5, 22.5);
  hladder4id = fs->make<TH1D>( "hladder4id", "Clus in ladder L4", 65, -32.5, 32.5);
  hpladder1id = fs->make<TH1D>( "hpladder1id", "Pix in ladder L1", 13, -6.5, 6.5);
  hpladder2id = fs->make<TH1D>( "hpladder2id", "Pix in ladder L2", 29, -14.5, 14.5);
  hpladder3id = fs->make<TH1D>( "hpladder3id", "Pix in adder L3", 45, -22.5, 22.5);
  hpladder4id = fs->make<TH1D>( "hpladder4id", "Pix in ladder L4", 65, -32.5, 32.5);

  hz1id = fs->make<TH1D>( "hz1id", "Clus in ring L1", 11, -5.5, 5.5);
  hz2id = fs->make<TH1D>( "hz2id", "Clus in ring L2", 11, -5.5, 5.5);
  hz3id = fs->make<TH1D>( "hz3id", "Clus in ring L3", 11, -5.5, 5.5);
  hz4id = fs->make<TH1D>( "hz4id", "Clus in ring L4", 11, -5.5, 5.5);
  hpz1id = fs->make<TH1D>( "hpz1id", "Pix in ring L1", 11, -5.5, 5.5);
  hpz2id = fs->make<TH1D>( "hpz2id", "Pix in ring L2", 11, -5.5, 5.5);
  hpz3id = fs->make<TH1D>( "hpz3id", "Pix in ring L3", 11, -5.5, 5.5);
  hpz4id = fs->make<TH1D>( "hpz4id", "Pix in ring L4", 11, -5.5, 5.5);

  hcols11 = fs->make<TH1D>( "hcols11", "Layer 1 clus per cols", 8000,-4.,4.);
  hrows11 = fs->make<TH1D>( "hrows11", "Layer 1 clus per rows", 2000,-1.,1.);

  hcols1 = fs->make<TH1D>( "hcols1", "Layer 1 clus per cols", 500,0.,500.);
  hcols2 = fs->make<TH1D>( "hcols2", "Layer 2 clus per cols", 500,0.,500.);
  hcols3 = fs->make<TH1D>( "hcols3", "Layer 3 clus per cols", 500,0.,500.);
  hcols4 = fs->make<TH1D>( "hcols4", "Layer 4 clus per cols", 500,0.,500.);
  hpcols1 = fs->make<TH1D>( "hpcols1", "Layer 1 pix per cols", 500,-0.5,499.5);
  hpcols2 = fs->make<TH1D>( "hpcols2", "Layer 2 pix per cols", 500,-0.5,499.5);
  hpcols3 = fs->make<TH1D>( "hpcols3", "Layer 3 pix per cols", 500,-0.5,499.5);
  hpcols4 = fs->make<TH1D>( "hpcols4", "Layer 4 pix percols", 500,-0.5,499.5);
  
  hrows1 = fs->make<TH1D>( "hrows1", "Layer 1 clus per rows", 200,-0.5,199.5);
  hrows2 = fs->make<TH1D>( "hrows2", "Layer 2 clus per rows", 200,-0.5,199.5);
  hrows3 = fs->make<TH1D>( "hrows3", "layer 3 clus per rows", 200,-0.5,199.5);
  hrows4 = fs->make<TH1D>( "hrows4", "layer 4 clus per rows", 200,-0.5,199.5);
  hprows1 = fs->make<TH1D>( "hprows1", "Layer 1 pix per rows", 200,-0.5,199.5);
  hprows2 = fs->make<TH1D>( "hprows2", "Layer 2 pix per rows", 200,-0.5,199.5);
  hprows3 = fs->make<TH1D>( "hprows3", "layer 3 pix per rows", 200,-0.5,199.5);
  hprows4 = fs->make<TH1D>( "hprows4", "layer 4 pix per rows", 200,-0.5,199.5);

  htracksGoodInPix = fs->make<TH1D>( "htracksGoodInPix", "count good tracks in pix",4000,-0.5,3999.5);
  htracksGood = fs->make<TH1D>( "htracksGood", "count good tracks",4000,-0.5,3999.5);
  htracks = fs->make<TH1D>( "htracks", "count tracks",4000,-0.5,3999.5);
  hclusBpix = fs->make<TH1D>( "hclusBpix", "count clus in bpix",200,-0.5,3999.5);
  hpixBpix = fs->make<TH1D>( "hpixBpix", "count pixels",200,-0.5,19999.5);

  hpvxy = fs->make<TH2F>( "hpvxy", "pv xy",100,-1.,1.,100,-1.,1.);
  hpvz = fs->make<TH1D>( "hpvz", "pv z",1000,-50.,50.);
  hpvr = fs->make<TH1D>( "hpvr", "pv r",100,0.,1.);
  hNumPv = fs->make<TH1D>( "hNumPv", "num of pv",100,0.,100.);
  hNumPvClean = fs->make<TH1D>( "hNumPvClean", "num of pv clean",100,0.,100.);

  hPt = fs->make<TH1D>( "hPt", "pt",120,0.,120.);
  hD0 = fs->make<TH1D>( "hD0", "d0",500,0.,20.);
  hDz = fs->make<TH1D>( "hDz", "pt",200,-100.,100.);
  hPtP = fs->make<TH1D>( "hPtP", "pt",120,0.,120.);
  hD0P = fs->make<TH1D>( "hD0P", "d0",500,0.,20.);
  hDzP = fs->make<TH1D>( "hDzP", "pt",200,-100.,100.);
  hzdiff = fs->make<TH1D>( "hzdiff", "PVz-Trackz",200,-10.,10.);

  hEta = fs->make<TH1D>( "hEta", "eta",120,-3.0,3.0);
  hPhi = fs->make<TH1D>( "hPhi", "track phi",350,-3.5,3.5);
  hEtaP = fs->make<TH1D>( "hEtaP", "eta",120,-3.0,3.0);
  hPhiP = fs->make<TH1D>( "hPhiP", "track phi",350,-3.5,3.5);

  hPhi1 = fs->make<TH1D>( "hPhi1", "track phi 1",350,-3.5,3.5);
  hPhi2 = fs->make<TH1D>( "hPhi2", "track phi 2",350,-3.5,3.5);
  hPhi3 = fs->make<TH1D>( "hPhi3", "track phi 3",350,-3.5,3.5);
  hPhi4 = fs->make<TH1D>( "hPhi4", "track phi 4",350,-3.5,3.5);
  hPhi0 = fs->make<TH1D>( "hPhi0", "track phi 0",350,-3.5,3.5);

  hEta1 = fs->make<TH1D>( "hEta1", "eta1",120,-3.0,3.0);
  hEta2 = fs->make<TH1D>( "hEta2", "eta2",120,-3.0,3.0);
  hEta3 = fs->make<TH1D>( "hEta3", "eta3",120,-3.0,3.0);
  hEta4 = fs->make<TH1D>( "hEta4", "eta4",120,-3.0,3.0);
  hEta5 = fs->make<TH1D>( "hEta5", "eta5",120,-3.0,3.0);
  hEta6 = fs->make<TH1D>( "hEta6", "eta6",120,-3.0,3.0);
  hEta7 = fs->make<TH1D>( "hEta7", "eta7",120,-3.0,3.0);
  hEta0 = fs->make<TH1D>( "hEta0", "eta0",120,-3.0,3.0);

  hEta10 = fs->make<TH1D>( "hEta10", "eta1 no hit",120,-3.0,3.0);
  hEta20 = fs->make<TH1D>( "hEta20", "eta2 no hit",120,-3.0,3.0);
  hEta30 = fs->make<TH1D>( "hEta30", "eta3 no hit",120,-3.0,3.0);
  hEta40 = fs->make<TH1D>( "hEta40", "eta4 no hit",120,-3.0,3.0);

  // special l1 tests
  hEta11 = fs->make<TH1D>( "hEta11", "eta1",120,-3.0,3.0);
  hEta12 = fs->make<TH1D>( "hEta12", "eta1",120,-3.0,3.0);
  hPhi11 = fs->make<TH1D>( "hPhi11", "phi1",350,-3.5,3.5);
  hPhi12 = fs->make<TH1D>( "hPhi12", "phi1",350,-3.5,3.5);

  hProbabilityXYBpix = fs->make<TH1D>( "hProbabilityXYBpix", "hit probability XY- bpix",
				       110,0.,1.1);
  hProbabilityQBpix = fs->make<TH1D>( "hProbabilityQBpix", "hit probabaility Q - bpix",
				      110,0.,1.1);
  hProbabilityXYFpix = fs->make<TH1D>( "hProbabilityXYFpix", "hit probability XY- fpix",
				       110,0.,1.1);
  hProbabilityQFpix = fs->make<TH1D>( "hProbabilityQFpix", "hit probabaility Q - fpix",
				      110,0.,1.1);

#if defined(CLU_SHAPE) || defined(CLU_SHAPE_L2) 
  hcharge111 = fs->make<TH1D>( "hcharge111", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge111_1 = fs->make<TH1D>( "hcharge111_1", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge111_2 = fs->make<TH1D>( "hcharge111_2", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge111_3 = fs->make<TH1D>( "hcharge111_3", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge111_4 = fs->make<TH1D>( "hcharge111_4", "Clu charge l1", 100, 0.,400.); //in ke

  hcharge112 = fs->make<TH1D>( "hcharge112", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge112_1 = fs->make<TH1D>( "hcharge112_1", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge112_2 = fs->make<TH1D>( "hcharge112_2", "Clu charge l1", 100, 0.,400.); //in ke

  hcharge121 = fs->make<TH1D>( "hcharge121", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge121_1 = fs->make<TH1D>( "hcharge121_1", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge121_2 = fs->make<TH1D>( "hcharge121_2", "Clu charge l1", 100, 0.,400.); //in ke

  hcharge122 = fs->make<TH1D>( "hcharge122", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge122_1 = fs->make<TH1D>( "hcharge122_1", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge122_2 = fs->make<TH1D>( "hcharge122_2", "Clu charge l1", 100, 0.,400.); //in ke

  hcharge130 = fs->make<TH1D>( "hcharge130", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge113 = fs->make<TH1D>( "hcharge113", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge131 = fs->make<TH1D>( "hcharge131", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge123 = fs->make<TH1D>( "hcharge123", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge132 = fs->make<TH1D>( "hcharge132", "Clu charge l1", 100, 0.,400.); //in ke

  hcharge140 = fs->make<TH1D>( "hcharge140", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge160 = fs->make<TH1D>( "hcharge160", "Clu charge l1", 100, 0.,400.); //in ke
  hcharge180 = fs->make<TH1D>( "hcharge180", "Clu charge l1", 100, 0.,800.); //in ke
  hcharge181 = fs->make<TH1D>( "hcharge181", "Clu charge l1", 100, 0.,800.); //in ke
  hcharge182 = fs->make<TH1D>( "hcharge182", "Clu charge l1", 100, 0.,800.); //in ke
  hcharge183 = fs->make<TH1D>( "hcharge183", "Clu charge l1", 100, 0.,800.); //in ke
  hcharge184 = fs->make<TH1D>( "hcharge184", "Clu charge l1", 100, 0.,800.); //in ke

  hpixcharge111 = fs->make<TH1D>( "hpixcharge111", "charge l1", 100, 0.,100.); //in ke

  hpixcharge112 = fs->make<TH1D>( "hpixcharge112", "charge l1", 100, 0.,100.); //in ke
  hpixcharge112_1 = fs->make<TH1D>( "hpixcharge112_1", "charge l1", 100, 0.,100.); //in ke
  hpixcharge112_2 = fs->make<TH1D>( "hpixcharge112_2", "charge l1", 100, 0.,100.); //in ke

  hpixcharge121 = fs->make<TH1D>( "hpixcharge121", "charge l1", 100, 0.,100.); //in ke
  hpixcharge121_1 = fs->make<TH1D>( "hpixcharge121_1", "charge l1", 100, 0.,100.); //in ke
  hpixcharge121_2 = fs->make<TH1D>( "hpixcharge121_2", "charge l1", 100, 0.,100.); //in ke

  hpixcharge122 = fs->make<TH1D>( "hpixcharge122", "charge l1", 100, 0.,100.); //in ke
  hpixcharge122_1 = fs->make<TH1D>( "hpixcharge122_1", "charge l1", 100, 0.,100.); //in ke
  hpixcharge122_2 = fs->make<TH1D>( "hpixcharge122_2", "charge l1", 100, 0.,100.); //in ke

  hpixcharge113 = fs->make<TH1D>( "hpixcharge113", "charge l1", 100, 0.,100.); //in ke
  hpixcharge131 = fs->make<TH1D>( "hpixcharge131", "charge l1", 100, 0.,100.); //in ke
  hpixcharge123 = fs->make<TH1D>( "hpixcharge123", "charge l1", 100, 0.,100.); //in ke
  hpixcharge132 = fs->make<TH1D>( "hpixcharge132", "charge l1", 100, 0.,100.); //in ke

  hpixcharge130 = fs->make<TH1D>( "hpixcharge130", "charge l1", 100, 0.,100.); //in ke
  hpixcharge140 = fs->make<TH1D>( "hpixcharge140", "charge l1", 100, 0.,100.); //in ke
  hpixcharge160 = fs->make<TH1D>( "hpixcharge160", "charge l1", 100, 0.,100.); //in ke
  hpixcharge180 = fs->make<TH1D>( "hpixcharge180", "charge l1", 100, 0.,100.); //in ke
  hpixcharge181 = fs->make<TH1D>( "hpixcharge181", "charge l1", 100, 0.,100.); //in ke
  hpixcharge182 = fs->make<TH1D>( "hpixcharge182", "charge l1", 100, 0.,100.); //in ke
  hpixcharge183 = fs->make<TH1D>( "hpixcharge183", "charge l1", 100, 0.,100.); //in ke
  hpixcharge184 = fs->make<TH1D>( "hpixcharge184", "charge l1", 100, 0.,100.); //in ke

#endif // CLU_SHAPE


#ifdef STUDY_LAY1
  //TH1D *hPt14, *hEta14, *hsizey14, *hsizex14, *hcharge14, *hpixcharge14;
  hPt14 = fs->make<TH1D>( "hPt14", "pt14",120,0.,120.);
  hEta14 = fs->make<TH1D>( "hEta14", "eta14",60,-3.0,3.0);
  hcharge14 = fs->make<TH1D>( "hcharge14", "Clu charge l14", 400, 0.,800.); 
  hsizex14 = fs->make<TH1D>( "hsizex14", "lay14 clu size in x",20,-0.5,19.5);
  hsizey14 = fs->make<TH1D>( "hsizey14", "lay14 clu size in y",30,-0.5,29.5);

  hPt11 = fs->make<TH1D>( "hPt11", "pt11",120,0.,120.);
  hEta11 = fs->make<TH1D>( "hEta11", "eta11",60,-3.0,3.0);
  hcharge11 = fs->make<TH1D>( "hcharge11", "Clu charge l11", 400, 0.,800.); 
  hsizex11 = fs->make<TH1D>( "hsizex11", "lay11 clu size in x",20,-0.5,19.5);
  hsizey11 = fs->make<TH1D>( "hsizey11", "lay11 clu size in y",30,-0.5,29.5);

  hPt15 = fs->make<TH1D>( "hPt15", "pt1",120,0.,120.);
  hEta15 = fs->make<TH1D>( "hEta15", "eta1",60,-3.0,3.0);
  hcharge15 = fs->make<TH1D>( "hcharge15", "Clu charge l1", 400, 0.,800.); 
  hsizex15 = fs->make<TH1D>( "hsizex15", "lay1 clu size in x",20,-0.5,19.5);
  hsizey15 = fs->make<TH1D>( "hsizey15", "lay1 clu size in y",30,-0.5,29.5);

  hpixDetMap35 = fs->make<TH2F>( "hpixDetMap35", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap36 = fs->make<TH2F>( "hpixDetMap36", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap37 = fs->make<TH2F>( "hpixDetMap37", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap38 = fs->make<TH2F>( "hpixDetMap38", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap39 = fs->make<TH2F>( "hpixDetMap39", "pix det layer 3",
				  416,0.,416.,160,0.,160.);

#endif // STUDY_LAY1

#ifdef L1
  hl1a    = fs->make<TH1D>("hl1a",   "l1a",   128,-0.5,127.5);
  hl1t    = fs->make<TH1D>("hl1t",   "l1t",   128,-0.5,127.5);
  hlt1    = fs->make<TH1D>("hlt1","hlt1",256,-0.5,255.5);
#endif

#ifdef USE_PROFILES
   hclumult1 = fs->make<TProfile>("hclumult1","cluster size layer 1",600,-3.,3.,0.0,100.);
   hclumult2 = fs->make<TProfile>("hclumult2","cluster size layer 2",600,-3.,3.,0.0,100.);
   hclumult3 = fs->make<TProfile>("hclumult3","cluster size layer 3",600,-3.,3.,0.0,100.);
   hclumult4 = fs->make<TProfile>("hclumult4","cluster size layer 4",600,-3.,3.,0.0,100.);

   hclumultx1 = fs->make<TProfile>("hclumultx1","cluster x-size layer 1",600,-3.,3.,0.0,100.);
   hclumultx2 = fs->make<TProfile>("hclumultx2","cluster x-size layer 2",600,-3.,3.,0.0,100.);
   hclumultx3 = fs->make<TProfile>("hclumultx3","cluster x-size layer 3",600,-3.,3.,0.0,100.);
   hclumultx4 = fs->make<TProfile>("hclumultx4","cluster x-size layer 3",600,-3.,3.,0.0,100.);

   hclumulty1 = fs->make<TProfile>("hclumulty1","cluster y-size layer 1",600,-3.,3.,0.0,100.);
   hclumulty2 = fs->make<TProfile>("hclumulty2","cluster y-size layer 2",600,-3.,3.,0.0,100.);
   hclumulty3 = fs->make<TProfile>("hclumulty3","cluster y-size layer 3",600,-3.,3.,0.0,100.);
   hclumulty4 = fs->make<TProfile>("hclumulty4","cluster y-size layer 3",600,-3.,3.,0.0,100.);

   hcluchar1 = fs->make<TProfile>("hcluchar1","cluster char layer 1",60,-3.,3.,0.0,1000.);
   hcluchar2 = fs->make<TProfile>("hcluchar2","cluster char layer 2",60,-3.,3.,0.0,1000.);
   hcluchar3 = fs->make<TProfile>("hcluchar3","cluster char layer 3",60,-3.,3.,0.0,1000.);
   hcluchar4 = fs->make<TProfile>("hcluchar4","cluster char layer 3",60,-3.,3.,0.0,1000.);

  // profiles versus ladder
   hclumultld1 = fs->make<TProfile>("hclumultld1","cluster size layer 1",23,-11.5,11.5,0.0,100.);
   hclumultld2 = fs->make<TProfile>("hclumultld2","cluster size layer 2",35,-17.5,17.5,0.0,100.);
   hclumultld3 = fs->make<TProfile>("hclumultld3","cluster size layer 3",47,-23.5,23.5,0.0,100.);

   hclumultxld1 = fs->make<TProfile>("hclumultxld1","cluster x-size layer 1",23,-11.5,11.5,0.0,100.);
   hclumultxld2 = fs->make<TProfile>("hclumultxld2","cluster x-size layer 2",35,-17.5,17.5,0.0,100.);
   hclumultxld3 = fs->make<TProfile>("hclumultxld3","cluster x-size layer 3",47,-23.5,23.5,0.0,100.);

   hclumultyld1 = fs->make<TProfile>("hclumultyld1","cluster y-size layer 1",23,-11.5,11.5,0.0,100.);
   hclumultyld2 = fs->make<TProfile>("hclumultyld2","cluster y-size layer 2",35,-17.5,17.5,0.0,100.);
   hclumultyld3 = fs->make<TProfile>("hclumultyld3","cluster y-size layer 3",47,-23.5,23.5,0.0,100.);

   hclucharld1 = fs->make<TProfile>("hclucharld1","cluster char layer 1",23,-11.5,11.5,0.0,1000.);
   hclucharld2 = fs->make<TProfile>("hclucharld2","cluster char layer 2",35,-17.5,17.5,0.0,1000.);
   hclucharld3 = fs->make<TProfile>("hclucharld3","cluster char layer 3",47,-23.5,23.5,0.0,1000.);


   htracksls = fs->make<TProfile>("htracksls","tracks with pix hits  vs ls",1000,0.,3000.,0.0,10000.);
   hpvsls = fs->make<TProfile>("hpvsls","pvs  vs ls",1000,0.,3000.,0.0,1000.);
   htrackslsn = fs->make<TProfile>("htrackslsn","tracks with pix hits/lumi  vs ls",1000,0.,3000.,0.0,10000.);
   hpvslsn = fs->make<TProfile>("hpvslsn","pvs/lumi  vs ls",1000,0.,3000.,0.0,1000.);

   hmult1 = fs->make<TProfile>("hmult1","clu mult layer 1",10,0.,10.,0.0,1000.);
   hmult2 = fs->make<TProfile>("hmult2","clu mult layer 2",10,0.,10.,0.0,1000.);
   hmult3 = fs->make<TProfile>("hmult3","clu mult layer 3",10,0.,10.,0.0,1000.);

   hclusPerTrkVsEta = fs->make<TProfile>("hclusPerTrkVsEta","clus per trk vs.eta",60,-3.,3.,0.0,100.);
   hclusPerTrkVsPt = fs->make<TProfile>("hclusPerTrkVsPt","clus per trk vs.pt",120,0.,120.,0.0,100.);
   hclusPerTrkVsls = fs->make<TProfile>("hclusPerTrkVsls","clus per trk vs.ls",300,0.,3000.,0.0,100.);
   hclusPerTrkVsEtaF = fs->make<TProfile>("hclusPerTrkVsEtaF","F clus per trk vs.eta",60,-3.,3.,0.0,100.);
   hclusPerTrkVsEtaB = fs->make<TProfile>("hclusPerTrkVsEtaB","B clus per trk vs.eta",60,-3.,3.,0.0,100.);
#endif // USE_PROFILES

#ifdef PHI_PROFILES
   // phi profiles 
   hclumultxPhi1 = fs->make<TProfile>("hclumultxPhi1","cluster x-size layer 1",700,-3.5,3.5,0.0,100.);
   hclumultxPhi2 = fs->make<TProfile>("hclumultxPhi2","cluster x-size layer 2",700,-3.5,3.5,0.0,100.);
   //hclumultxPhi3 = fs->make<TProfile>("hclumultxPhi3","cluster x-size layer 3",700,-28.,28.,0.0,100.);
   //hclumultxPhi4 = fs->make<TProfile>("hclumultxPhi4","cluster x-size layer 4",700,-28.,28.,0.0,100.);

   hclumultyPhi1 = fs->make<TProfile>("hclumultyPhi1","cluster y-size layer 1",700,-3.5,3.5,0.0,100.);
   hclumultyPhi2 = fs->make<TProfile>("hclumultyPhi2","cluster y-size layer 2",700,-3.5,3.5,0.0,100.);
   //hclumultyPhi3 = fs->make<TProfile>("hclumultyPhi3","cluster y-size layer 3",560,-28.,28.,0.0,100.);
   //hclumultyPhi4 = fs->make<TProfile>("hclumultyPhi4","cluster y-size layer 4",560,-28.,28.,0.0,100.);

   hclucharPhi1 = fs->make<TProfile>("hclucharPhi1","cluster char layer 1",700,-3.5,3.5,0.0,1000.);
   hclucharPhi2 = fs->make<TProfile>("hclucharPhi2","cluster char layer 2",700,-3.5,3.5,0.0,1000.);
   //hclucharPhi3 = fs->make<TProfile>("hclucharPhi3","cluster char layer 3",560,-28.,28.,0.0,1000.);
   //hclucharPhi4 = fs->make<TProfile>("hclucharPhi4","cluster char layer 4",560,-28.,28.,0.0,1000.);

   hpixcharPhi1 = fs->make<TProfile>("hpixcharPhi1","pix char layer 1",700,-3.5,3.5,0.0,1000.);
   hpixcharPhi2 = fs->make<TProfile>("hpixcharPhi2","pix char layer 2",700,-3.5,3.5,0.0,1000.);
   //hpixcharPhi3 = fs->make<TProfile>("hpixcharPhi3","pix char layer 3",560,-28.,28.,0.0,1000.);
   //hpixcharPhi4 = fs->make<TProfile>("hpixcharPhi4","pix char layer 4",560,-28.,28.,0.0,1000.);
#endif // PHI_PROFILES

#ifdef LUMI
   hintgl  = fs->make<TProfile>("hintgl", "inst lumi vs ls ",1000,0.,3000.,0.0,10000.);
   hinstl  = fs->make<TProfile>("hinstl", "intg lumi vs ls ",1000,0.,3000.,0.0,100.);
   hbeam1  = fs->make<TProfile>("hbeam1", "beam1 vs ls ",1000,0.,3000.,0.0,1000.);
   hbeam2  = fs->make<TProfile>("hbeam2", "beam2 vs ls ",1000,0.,3000.,0.0,1000.);
#endif

   hls0  = fs->make<TH1D>("hls0","LS", 2000,0,2000.);
   hls   = fs->make<TH1D>("hls", "LS", 2000,0,2000.);
   hbx0    = fs->make<TH1D>("hbx0",   "bx",   4000,0,4000.);  
   hbx    = fs->make<TH1D>("hbx",   "bx",     4000,0,4000.);  

   hetaphiMap1 = fs->make<TH2F>("hetaphiMap1","phi-eta clus - lay1",250,-2.5,2.5,350,-3.5,3.5);
   hetaphiMap2 = fs->make<TH2F>("hetaphiMap2","phi-eta clus - lay2",250,-2.5,2.5,350,-3.5,3.5);
   hetaphiMap3 = fs->make<TH2F>("hetaphiMap3","phi-eta clus - lay3",250,-2.5,2.5,350,-3.5,3.5);
   hetaphiMap4 = fs->make<TH2F>("hetaphiMap4","phi-eta clus - lay4",250,-2.5,2.5,350,-3.5,3.5);
   
   hstatus = fs->make<TH1D>("hstatus","status", 100, -0.5, 99.5);

   hbpixXY = fs->make<TH2F>("hbpixXY","bpix XY",180,-18.,18.,180,-18.,18.);
   hfpixXY1 = fs->make<TH2F>("hfpixXY1","fpix XY -z d2",160,-16.,16.,160,-16.,16.);
   hfpixXY2 = fs->make<TH2F>("hfpixXY2","fpix XY -z d1",160,-16.,16.,160,-16.,16.);
   hfpixXY3 = fs->make<TH2F>("hfpixXY3","fpix XY +z d1",160,-16.,16.,160,-16.,16.);
   hzphi1 = fs->make<TH2F>("hzphi1","bpix z phi 1",208,-26.,26.,140,-3.5,3.5);
   hzphi2 = fs->make<TH2F>("hzphi2","bpix z phi 2",208,-26.,26.,140,-3.5,3.5);
   hzphi3 = fs->make<TH2F>("hzphi3","bpix z phi 3",208,-26.,26.,140,-3.5,3.5);
   hzphi4 = fs->make<TH2F>("hzphi4","bpix z phi 4",208,-26.,26.,140,-3.5,3.5);

   htest  = fs->make<TH2F>("htest", "bpix eta-hit size-y",60,-3.,3.,20,0.5,20.5);
   htest1 = fs->make<TH2F>("htest1","sizey vs clu charge",50,0.,800.,15,0.5,15.5);
   htest2 = fs->make<TH2F>("htest2","sizey vs pix charge",50,0.,100.,15,0.5,15.5);
   htest3 = fs->make<TH2F>("htest3","sizey vs clu charge",50,0.,800.,10,0.5,10.5);
   htest4 = fs->make<TH2F>("htest4","sizey vs pix charge",50,0.,100.,10,0.5,10.5);


#ifdef SINGLE_MODULES
  // Special test hitos for inefficiency effects
  hpixDetMap10 = fs->make<TH2F>( "hpixDetMap10", "pix det layer 1",
				 416,0.,416.,160,0.,160.);
  hpixDetMap11 = fs->make<TH2F>( "hpixDetMap11", "pix det layer 1",
				 416,0.,416.,160,0.,160.);
  hpixDetMap12 = fs->make<TH2F>( "hpixDetMap12", "pix det layer 1",
				 416,0.,416.,160,0.,160.);
  hpixDetMap13 = fs->make<TH2F>( "hpixDetMap13", "pix det layer 1",
				 416,0.,416.,160,0.,160.);
  hpixDetMap14 = fs->make<TH2F>( "hpixDetMap14", "pix det layer 1",
				  416,0.,416.,160,0.,160.);
  hpixDetMap15 = fs->make<TH2F>( "hpixDetMap15", "pix det layer 1",
				  416,0.,416.,160,0.,160.);
  hpixDetMap16 = fs->make<TH2F>( "hpixDetMap16", "pix det layer 1",
				  416,0.,416.,160,0.,160.);
  hpixDetMap17 = fs->make<TH2F>( "hpixDetMap17", "pix det layer 1",
				  416,0.,416.,160,0.,160.);
  hpixDetMap18 = fs->make<TH2F>( "hpixDetMap18", "pix det layer 1",
				  416,0.,416.,160,0.,160.);
  hpixDetMap19 = fs->make<TH2F>( "hpixDetMap19", "pix det layer 1",
				  416,0.,416.,160,0.,160.);

  hpixDetMap20 = fs->make<TH2F>( "hpixDetMap20", "pix det layer 2",
				 416,0.,416.,160,0.,160.);
  hpixDetMap21 = fs->make<TH2F>( "hpixDetMap21", "pix det layer 2",
				 416,0.,416.,160,0.,160.);
  hpixDetMap22 = fs->make<TH2F>( "hpixDetMap22", "pix det layer 2",
				 416,0.,416.,160,0.,160.);
  hpixDetMap23 = fs->make<TH2F>( "hpixDetMap23", "pix det layer 2",
				 416,0.,416.,160,0.,160.);
  hpixDetMap24 = fs->make<TH2F>( "hpixDetMap24", "pix det layer 2",
				  416,0.,416.,160,0.,160.);
  hpixDetMap25 = fs->make<TH2F>( "hpixDetMap25", "pix det layer 2",
				  416,0.,416.,160,0.,160.);
  hpixDetMap26 = fs->make<TH2F>( "hpixDetMap26", "pix det layer 2",
				  416,0.,416.,160,0.,160.);
  hpixDetMap27 = fs->make<TH2F>( "hpixDetMap27", "pix det layer 2",
				  416,0.,416.,160,0.,160.);
  hpixDetMap28 = fs->make<TH2F>( "hpixDetMap28", "pix det layer 2",
				  416,0.,416.,160,0.,160.);
  hpixDetMap29 = fs->make<TH2F>( "hpixDetMap29", "pix det layer 2",
				  416,0.,416.,160,0.,160.);

  hpixDetMap30 = fs->make<TH2F>( "hpixDetMap30", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap31 = fs->make<TH2F>( "hpixDetMap31", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap32 = fs->make<TH2F>( "hpixDetMap32", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap33 = fs->make<TH2F>( "hpixDetMap33", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap34 = fs->make<TH2F>( "hpixDetMap34", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap35 = fs->make<TH2F>( "hpixDetMap35", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap36 = fs->make<TH2F>( "hpixDetMap36", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap37 = fs->make<TH2F>( "hpixDetMap37", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap38 = fs->make<TH2F>( "hpixDetMap38", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap39 = fs->make<TH2F>( "hpixDetMap39", "pix det layer 3",
				  416,0.,416.,160,0.,160.);

  hpixDetMap40 = fs->make<TH2F>( "hpixDetMap40", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap41 = fs->make<TH2F>( "hpixDetMap41", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap42 = fs->make<TH2F>( "hpixDetMap42", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap43 = fs->make<TH2F>( "hpixDetMap43", "pix det layer 3",
				 416,0.,416.,160,0.,160.);
  hpixDetMap44 = fs->make<TH2F>( "hpixDetMap44", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap45 = fs->make<TH2F>( "hpixDetMap45", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap46 = fs->make<TH2F>( "hpixDetMap46", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap47 = fs->make<TH2F>( "hpixDetMap47", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap48 = fs->make<TH2F>( "hpixDetMap48", "pix det layer 3",
				  416,0.,416.,160,0.,160.);
  hpixDetMap49 = fs->make<TH2F>( "hpixDetMap49", "pix det layer 3",
				  416,0.,416.,160,0.,160.);

#endif // SINGLE_MODULES

   // RecHit errors
   // alignment errors
#ifdef ALIGN_STUDIES
   recHitXAlignError1 = fs->make<TH1D>("recHitXAlignError1","RecHit X Alignment errors bpix 1", 100, 0., 100.);
   recHitYAlignError1 = fs->make<TH1D>("recHitYAlignError1","RecHit Y Alignment errors bpix 1", 100, 0., 100.);
   recHitXAlignError2 = fs->make<TH1D>("recHitXAlignError2","RecHit X Alignment errors bpix 2", 100, 0., 100.);
   recHitYAlignError2 = fs->make<TH1D>("recHitYAlignError2","RecHit Y Alignment errors bpix 2", 100, 0., 100.);
   recHitXAlignError3 = fs->make<TH1D>("recHitXAlignError3","RecHit X Alignment errors bpix 3", 100, 0., 100.);
   recHitYAlignError3 = fs->make<TH1D>("recHitYAlignError3","RecHit Y Alignment errors bpix 3", 100, 0., 100.);
   recHitXAlignError4 = fs->make<TH1D>("recHitXAlignError4","RecHit X Alignment errors fpix -d2", 100, 0., 100.);
   recHitYAlignError4 = fs->make<TH1D>("recHitYAlignError4","RecHit Y Alignment errors fpix -d2", 100, 0., 100.);
   recHitXAlignError5 = fs->make<TH1D>("recHitXAlignError5","RecHit X Alignment errors fpix -d1", 100, 0., 100.);
   recHitYAlignError5 = fs->make<TH1D>("recHitYAlignError5","RecHit Y Alignment errors fpix -d1", 100, 0., 100.);
   recHitXAlignError6 = fs->make<TH1D>("recHitXAlignError6","RecHit X Alignment errors fpix +d1", 100, 0., 100.);
   recHitYAlignError6 = fs->make<TH1D>("recHitYAlignError6","RecHit Y Alignment errors fpix +d1", 100, 0., 100.);
   recHitXAlignError7 = fs->make<TH1D>("recHitXAlignError7","RecHit X Alignment errors fpix +d2", 100, 0., 100.);
   recHitYAlignError7 = fs->make<TH1D>("recHitYAlignError7","RecHit Y Alignment errors fpix +d2", 100, 0., 100.);

   recHitXError1 = fs->make<TH1D>("recHitXError1","RecHit X errors bpix 1", 100, 0., 100.);
   recHitYError1 = fs->make<TH1D>("recHitYError1","RecHit Y errors bpix 1", 100, 0., 100.);
   recHitXError2 = fs->make<TH1D>("recHitXError2","RecHit X errors bpix 2", 100, 0., 100.);
   recHitYError2 = fs->make<TH1D>("recHitYError2","RecHit Y errors bpix 2", 100, 0., 100.);
   recHitXError3 = fs->make<TH1D>("recHitXError3","RecHit X errors bpix 3", 100, 0., 100.);
   recHitYError3 = fs->make<TH1D>("recHitYError3","RecHit Y errors bpix 3", 100, 0., 100.);
   recHitXError4 = fs->make<TH1D>("recHitXError4","RecHit X errors fpix -d2", 100, 0., 100.);
   recHitYError4 = fs->make<TH1D>("recHitYError4","RecHit Y errors fpix -d2", 100, 0., 100.);
   recHitXError5 = fs->make<TH1D>("recHitXError5","RecHit X errors fpix -d1", 100, 0., 100.);
   recHitYError5 = fs->make<TH1D>("recHitYError5","RecHit Y errors fpix -d1", 100, 0., 100.);
   recHitXError6 = fs->make<TH1D>("recHitXError6","RecHit X errors fpix +d1", 100, 0., 100.);
   recHitYError6 = fs->make<TH1D>("recHitYError6","RecHit Y errors fpix +d1", 100, 0., 100.);
   recHitXError7 = fs->make<TH1D>("recHitXError7","RecHit X errors fpix +d2", 100, 0., 100.);
   recHitYError7 = fs->make<TH1D>("recHitYError7","RecHit Y errors fpix +d2", 100, 0., 100.);

  hErrorXB = fs->make<TProfile>("hErrorXB","bpix x errors per ladder",220,0.,220.,0.0,1000.);
  hErrorXF = fs->make<TProfile>("hErrorXF","fpix x errors per ladder",100,0.,100.,0.0,1000.);
  hErrorYB = fs->make<TProfile>("hErrorYB","bpix y errors per ladder",220,0.,220.,0.0,1000.);
  hErrorYF = fs->make<TProfile>("hErrorYF","fpix y errors per ladder",100,0.,100.,0.0,1000.);

  hAErrorXB = fs->make<TProfile>("hAErrorXB","bpix x errors per ladder",220,0.,220.,0.0,1000.);
  hAErrorXF = fs->make<TProfile>("hAErrorXF","fpix x errors per ladder",100,0.,100.,0.0,1000.);
  hAErrorYB = fs->make<TProfile>("hAErrorYB","bpix y errors per ladder",220,0.,220.,0.0,1000.);
  hAErrorYF = fs->make<TProfile>("hAErrorYF","fpix y errors per ladder",100,0.,100.,0.0,1000.);

#endif

#ifdef LS_STUDIES
   highH = 3000.; 
   sizeH = 1000;

   hclusls = fs->make<TProfile>("hclusls","clus vs ls",sizeH,0.,highH,0.0,30000.);
   //hpixls  = fs->make<TProfile>("hpixls", "pix vs ls ",sizeH,0.,highH,0.0,100000.);

   hcharCluls = fs->make<TProfile>("hcharCluls","clu char vs ls",sizeH,0.,highH,0.0,1000.);
   //hcharPixls = fs->make<TProfile>("hcharPixls","pix char vs ls",sizeH,0.,highH,0.0,100.);
   hsizeCluls = fs->make<TProfile>("hsizeCluls","clu size vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeXCluls= fs->make<TProfile>("hsizeXCluls","clu size-x vs ls",sizeH,0.,highH,0.0,1000.);

   hcharCluls1 = fs->make<TProfile>("hcharCluls1","clu char vs ls",sizeH,0.,highH,0.0,1000.);
   //hcharPixls1 = fs->make<TProfile>("hcharPixls1","pix char vs ls",sizeH,0.,highH,0.0,100.);
   hsizeCluls1 = fs->make<TProfile>("hsizeCluls1","clu size vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeXCluls1= fs->make<TProfile>("hsizeXCluls1","clu size-x vs ls",sizeH,0.,highH,0.0,1000.);
   hcharCluls2 = fs->make<TProfile>("hcharCluls2","clu char vs ls",sizeH,0.,highH,0.0,1000.);
   //hcharPixls2 = fs->make<TProfile>("hcharPixls2","pix char vs ls",sizeH,0.,highH,0.0,100.);
   hsizeCluls2 = fs->make<TProfile>("hsizeCluls2","clu size vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeXCluls2= fs->make<TProfile>("hsizeXCluls2","clu size-x vs ls",sizeH,0.,highH,0.0,100.);
   hcharCluls3 = fs->make<TProfile>("hcharCluls3","clu char vs ls",sizeH,0.,highH,0.0,1000.);
   //hcharPixls3 = fs->make<TProfile>("hcharPixls3","pix char vs ls",sizeH,0.,highH,0.0,100.);
   hsizeCluls3 = fs->make<TProfile>("hsizeCluls3","clu size vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeXCluls3= fs->make<TProfile>("hsizeXCluls3","clu size-x vs ls",sizeH,0.,highH,0.0,1000.);
   hclusls1 = fs->make<TProfile>("hclusls1","clus vs ls",sizeH,0.,highH,0.0,30000.);
   //hpixls1  = fs->make<TProfile>("hpixls1", "pix vs ls ",sizeH,0.,highH,0.0,100000.);
   hclusls2 = fs->make<TProfile>("hclusls2","clus vs ls",sizeH,0.,highH,0.0,30000.);
   //hpixls2  = fs->make<TProfile>("hpixls2", "pix vs ls ",sizeH,0.,highH,0.0,100000.);
   hclusls3 = fs->make<TProfile>("hclusls3","clus vs ls",sizeH,0.,highH,0.0,30000.);
   //hpixls3  = fs->make<TProfile>("hpixls3", "pix vs ls ",sizeH,0.,highH,0.0,100000.);

   // Profiles versus bx
   //hpixbx  = fs->make<TProfile>("hpixbx", "pixs vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hclubx  = fs->make<TProfile>("hclubx", "clus vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hpvbx   = fs->make<TProfile>("hpvbx",  "pvs vs bx ", 4000,-0.5,3999.5,0.0,1000000.);
   htrackbx   = fs->make<TProfile>("htrackbx",  "tracks vs bx ", 4000,-0.5,3999.5,0.0,1000000.);
#endif

#ifdef OVERLAP_STUDIES
   hcluphi1 = fs->make<TH1D>("hcluphi1",  "clus1 per track vs phi", 
				 1400,-3.5,3.5);
   hcluphi2 = fs->make<TH1D>("hcluphi2",  "clus2 per track vs phi", 
				 1400,-3.5,3.5);
   hcluphi3 = fs->make<TH1D>("hcluphi3",  "clus3 per track vs phi", 
				 1400,-3.5,3.5);
   hcluphi4 = fs->make<TH1D>("hcluphi4",  "clus4 per track vs phi", 
				 1400,-3.5,3.5);

#endif
  hsizeyz1 = fs->make<TH2F>( "hsizeyz1", "sizy vs z for layer 1",
			     60,-3.0,3.0,20,0.5,20.5);
  hsizeyz2 = fs->make<TH2F>( "hsizeyz2", "sizy vs z for layer 2",
			     50,-2.5,2.5,14,0.5,14.5);
  hsizeyz3 = fs->make<TH2F>( "hsizeyz3", "sizy vs z for layer 3",
			     50,-2.5,2.5,14,0.5,14.5);
  hsizeyz4 = fs->make<TH2F>( "hsizeyz4", "sizy vs z for layer 4",
			     50,-2.5,2.5,14,0.5,14.5);

  // pixel histos
  sizeH = 400;   // 600
  highH = 100.0; // charge limit in kelec
  hpixcharge1 = fs->make<TH1D>( "hpixcharge1", "Pix charge l1",sizeH, 0.,highH);
  hpixcharge2 = fs->make<TH1D>( "hpixcharge2", "Pix charge l2",sizeH, 0.,highH);
  hpixcharge3 = fs->make<TH1D>( "hpixcharge3", "Pix charge l3",sizeH, 0.,highH);
  hpixcharge4 = fs->make<TH1D>( "hpixcharge4", "Pix charge l4",sizeH, 0.,highH);
  hpixcharge5 = fs->make<TH1D>( "hpixcharge5", "Pix charge d",sizeH, 0.,highH);
  hpixcharge1n= fs->make<TH1D>( "hpixcharge1n","Pix charge l1",sizeH, 0.,highH);
  hpixcharge2b= fs->make<TH1D>( "hpixcharge2b","Pix charge l2",sizeH, 0.,highH);
  hpixcharge2g= fs->make<TH1D>( "hpixcharge2g","Pix charge l2",sizeH, 0.,highH);

#ifdef STUDY_LAY1
  hpixcharge11 = fs->make<TH1D>( "hpixcharge11", "Pix charge l11",sizeH, 0.,highH);
  hpixcharge14 = fs->make<TH1D>( "hpixcharge14", "Pix charge l14",sizeH, 0.,highH);
  hpixcharge15 = fs->make<TH1D>( "hpixcharge15", "Pix charge l1",sizeH, 0.,highH);
#endif

  // pix in all dets (superimposed)
  hpixDetMap1 = fs->make<TH2F>( "hpixMap1", "pix in det layer 1",
		      416,0.,416.,160,0.,160.);
  hpixDetMap1->SetOption("colz");
  hpixDetMap2 = fs->make<TH2F>( "hpixMap2", "pix in det layer 2",
		      416,0.,416.,160,0.,160.);
  hpixDetMap2->SetOption("colz");
  hpixDetMap3 = fs->make<TH2F>( "hpixMap3", "pix in det layer 3",
		      416,0.,416.,160,0.,160.);
  hpixDetMap3->SetOption("colz");
  hpixDetMap4 = fs->make<TH2F>( "hpixMap4", "pix in det layer 4",
		      416,0.,416.,160,0.,160.);
  hpixDetMap4->SetOption("colz");


  hbpixHits   = fs->make<TH1D>("hpixHits","bpix hits per track",10,-0.5,9.5); 
  hbpixLayers = fs->make<TH1D>("hpixLayers",  "bpix hit layers per track",10,-0.5,9.5); 
  htrackLayers= fs->make<TH1D>("htrackLayers","tracker layers per track",30,-0.5,29.5);

  hbpixLayersPt = fs->make<TProfile>("hbpixLayersPt", "BPix Layers per track vs Pt",100,0.,100.,0.,100.);
  htrackLayersPt= fs->make<TProfile>("htrackLayersPt","Tracking Layers per track vs Pt",100,0.,100.,0.,100.);
  hbpixLayersEta = fs->make<TProfile>("hbpixLayersEta", "BPix Layers per track vs Eta",70,-3.5,3.5,0.,100.);
  htrackLayersEta= fs->make<TProfile>("htrackLayersEta","Tracking Layers per track vs Eta",70,-3.5,3.5,0.,100.);

#ifdef STUDY_ONEMOD
  hsizeMap1 = fs->make<TProfile2D>("hsizeMap1", "clus size, module in L1",
                                    416,0.,416.,160,0.,160.,0.,10000.);
  hsizeMap1->SetOption("colz");
  hsizeXMap1 = fs->make<TProfile2D>("hsizeXMap1", "clus sizeX, module in L1",
                                    416,0.,416.,160,0.,160.,0.,10000.);
  hsizeXMap1->SetOption("colz");
  hsizeYMap1 = fs->make<TProfile2D>("hsizeYMap1", "clus sizeY, module in L1",
                                    416,0.,416.,160,0.,160.,0.,10000.);
  hsizeYMap1->SetOption("colz");
  hclucharMap1 = fs->make<TProfile2D>("hclucharMap1", "clus char, module in L1",
                                    416,0.,416.,160,0.,160.,0.,10000.);
  hclucharMap1->SetOption("colz");
  hpixcharMap1 = fs->make<TProfile2D>("hpixcharMap1", "pix char, module in L1",
                                    416,0.,416.,160,0.,160.,0.,10000.);
  hpixcharMap1->SetOption("colz");
#endif


}
//-------------------------------------------------------------------------
#if defined(CLU_SHAPE) || defined(CLU_SHAPE_L2) 
void PixClustersWithTracks::histogramClus(float cha, int size, int sizex, int sizey, 
					  bool same, int corner) {

  if(size==1) {
    hcharge111->Fill(cha); 
    if(corner==1) hcharge111_1->Fill(cha); 
    else if(corner==2) hcharge111_2->Fill(cha); 
    else if(corner==3) hcharge111_3->Fill(cha); 
    else if(corner==4) hcharge111_4->Fill(cha); 


  } else  {

    if(sizex==1 && sizey==2) {
      hcharge112->Fill(cha); 
      if(same) hcharge112_1->Fill(cha); 
      else     hcharge112_2->Fill(cha);
      
    } else if (sizex==2 && sizey==1) {
      hcharge121->Fill(cha); 
      if(same) hcharge121_1->Fill(cha);
      else     hcharge121_2->Fill(cha);
      
    } else if (sizex==2 && sizey==2) {
      hcharge122->Fill(cha); 
      if(same) hcharge122_1->Fill(cha); 
      else     hcharge122_2->Fill(cha);

    } else if (sizex==3 && sizey==2) {
      hcharge132->Fill(cha); 
      
    } else if (sizex==3 && sizey==1) {
      hcharge131->Fill(cha); 

    } else if(sizey==3) {

      hcharge130->Fill(cha); 
      if(sizex==1)       hcharge113->Fill(cha); 
      else if (sizex==2) hcharge123->Fill(cha); 

    } else if (sizey==4) {
      hcharge140->Fill(cha); 
    } else if (sizey==5 || sizey==6) {
      hcharge160->Fill(cha); 
    } else if (sizey==7 || sizey==8) {
      hcharge180->Fill(cha); 
    } else if (sizey==9 || sizey==10) {
      hcharge181->Fill(cha); 
    } else if (sizey==11 || sizey==12) {
      hcharge182->Fill(cha); 
    } else if (sizey==13 || sizey==14) {
      hcharge183->Fill(cha); 
    } else if (sizey>=15) {
      hcharge184->Fill(cha); 
    }
    
  }
}
void PixClustersWithTracks::histogramPix(float cha, int size, int sizex, int sizey, 
					 bool same, int corner) {

  if(size==1) {
    hpixcharge111->Fill(cha); 

  } else {

    if(sizex==1 && sizey==2) {
      hpixcharge112->Fill(cha); 
      if(same) hpixcharge112_1->Fill(cha); 
      else     hpixcharge112_2->Fill(cha);
      
    } else if (sizex==2 && sizey==1) {
      hpixcharge121->Fill(cha); 
      if(same) hpixcharge121_1->Fill(cha);
      else     hpixcharge121_2->Fill(cha);
      
    } else if (sizex==2 && sizey==2) {
      hpixcharge122->Fill(cha); 
      if(same) hpixcharge122_1->Fill(cha); 
      else     hpixcharge122_2->Fill(cha);
               
    } else if (sizex==3 && sizey==1) {
      hpixcharge131->Fill(cha); 
      
    } else if (sizex==3 && sizey==2) {
      hpixcharge132->Fill(cha); 
      
    } else if(sizey==3) {

      hpixcharge130->Fill(cha); 
      if(sizex==1)       hpixcharge113->Fill(cha); 
      else if (sizex==2) hpixcharge123->Fill(cha); 

    } else if (sizey==4) {
      hpixcharge140->Fill(cha); 
    } else if (sizey==5 || sizey==6) {
      hpixcharge160->Fill(cha); 
    } else if (sizey==7 || sizey==8) {
      hpixcharge180->Fill(cha); 
    } else if (sizey==9 || sizey==10) {
      hpixcharge181->Fill(cha); 
    } else if (sizey==11 || sizey==12) {
      hpixcharge182->Fill(cha); 
    } else if (sizey==13 || sizey==14) {
      hpixcharge183->Fill(cha); 
    } else if (sizey>=15) {
      hpixcharge184->Fill(cha); 
    }
  }
}
#endif

// ------------ method called to at the end of the job  ------------
void PixClustersWithTracks::endJob(){
  cout << " End PixelTracksTest, events =  " << countEvents 
       << " tracks "<< countTracks 
       << " good tracks "<< countGoodTracks 
       << " in pix "<< countTracksInPix 
       <<endl;

  double norm = 1;

  if(countEvents>0.) {
    countTracks /= countEvents;
    countGoodTracks /= countEvents;
    countTracksInPix /= countEvents;
    countPVs /= countEvents;
    countLumi /= 1000.;
    cout<<" Average tracks/event "<< countTracks<<" good "<< countGoodTracks<<" in pix "<< countTracksInPix
	<<" PVs "<< countPVs<<" events " << countEvents<<" lumi pb-1 "<< countLumi<<"/10, bug!"<<endl;  
    cout<<"Missing clus "<<count1<<" "<<count2<<endl;
    cout<<" Tracks in layers "<<countTracks1<<" "<<countTracks2<<" "
	<<countTracks3<<" "<<countTracks4<<" "<<countTracks5<<endl;
    //Divide the size histos
    //htest->Divide(htest,htest3,1.,1.);

    if(!Normalise) return;  // skip if not required 

    norm = 1./float(countEvents);
    cout<<" plots are rescaled by the number of full events "<<countEvents<<endl;

    // rescale if histos 
    hladder1id->Scale(norm);
    hladder2id->Scale(norm);
    hladder3id->Scale(norm);
    hladder4id->Scale(norm);
    
    hz1id->Scale(norm);
    hz2id->Scale(norm);
    hz3id->Scale(norm);
    hz4id->Scale(norm);
    
    hpladder1id->Scale(norm);
    hpladder2id->Scale(norm);
    hpladder3id->Scale(norm);
    hpladder4id->Scale(norm);
    
    hpz1id->Scale(norm);
    hpz2id->Scale(norm);
    hpz3id->Scale(norm);
    hpz4id->Scale(norm);
    
    hpcols1->Scale(norm);
    hpcols2->Scale(norm);
    hpcols3->Scale(norm);
    hpcols4->Scale(norm);
    hprows1->Scale(norm);
    hprows2->Scale(norm);
    hprows3->Scale(norm);
    hprows4->Scale(norm);
    
    hcols1->Scale(norm);
    hcols2->Scale(norm);
    hcols3->Scale(norm);
    hcols4->Scale(norm);
    hrows1->Scale(norm);
    hrows2->Scale(norm);
    hrows3->Scale(norm);
    hrows4->Scale(norm);
    
    // Rescale all 2D plots
    hDetMap1->Scale(norm);
    hDetMap2->Scale(norm);
    hDetMap3->Scale(norm);
    hDetMap4->Scale(norm);

    //hpDetMap1->Scale(norm);
    //hpDetMap2->Scale(norm);
    //hpDetMap3->Scale(norm);
    //hpDetMap4->Scale(norm);
    hpixDetMap1->Scale(norm);
    hpixDetMap2->Scale(norm);
    hpixDetMap3->Scale(norm);
    hpixDetMap4->Scale(norm);
    hcluDetMap1->Scale(norm);
    hcluDetMap2->Scale(norm);
    hcluDetMap3->Scale(norm);
    hcluDetMap4->Scale(norm);

  }

}
//////////////////////////////////////////////////////////////////
// Functions that gets called by framework every event
void PixClustersWithTracks::analyze(const edm::Event& e, 
			    const edm::EventSetup& es) {

  using namespace edm;
  using namespace reco;
  static int lumiBlockOld = -9999;
  const bool TrackCuts = false;
  const float CLU_SIZE_PT_CUT = 0.;

  
  int trackNumber = 0;
  int countNiceTracks=0;
  int countPixTracks=0;

  int numberOfClusters = 0;
  int numberOfPixels   = 0;

  // bpix
  int numOfClusPerTrk1=0;        
  int numOfClustersPerLay1=0;        
  int numOfPixelsPerLay1=0;        

  int numOfClusPerTrk2=0;        
  int numOfClustersPerLay2=0;        
  int numOfPixelsPerLay2=0;        

  int numOfClusPerTrk3=0;        
  int numOfClustersPerLay3=0;        
  int numOfPixelsPerLay3=0;     

  int numOfClusPerTrk4=0;        
  int numOfClustersPerLay4=0;        
  int numOfPixelsPerLay4=0;     

  // fpix
  int numOfClusPerTrk5=0;        
  int numOfClustersPerLay5=0;        
  int numOfPixelsPerLay5=0;     
  int numOfClusPerTrk6=0;        
  int numOfClustersPerLay6=0;        
  int numOfPixelsPerLay6=0;     
  int numOfClusPerTrk7=0;        
  int numOfClustersPerLay7=0;        
  int numOfPixelsPerLay7=0;     
  
  int numOfClustersPerDisk1=0;  
  int numOfClustersPerDisk2=0;  
  int numOfClustersPerDisk3=0; 
  int numOfClustersPerDisk4=0; 
  int numOfClustersPerDisk5=0; 
  int numOfClustersPerDisk6=0; 
  
  int run       = e.id().run();
  int event     = e.id().event();
  int lumiBlock = e.luminosityBlock();
  int bx        = e.bunchCrossing();
  //int orbit     = e.orbitNumber(); // unused 

  if(PRINT) cout<<"Run "<<run<<" Event "<<event<<" LS "<<lumiBlock<<" bx "<<bx<<endl;

  bool select = false;

  //if(lumiBlock==21) {
  ////if(event==20659585) cout<<" Event "<<event<<" LS "<<lumiBlock<<endl;
  //for(int i=0;i<19;++i) {
  //  if(event==badBx[i]) {select=true; break;}
  //}      
  //} else {return;}
  //if(!select) return;
  //cout<<"Run "<<run<<" Event "<<event<<" LS "<<lumiBlock<<" bx "<<bx<<endl;

  hbx0->Fill(float(bx));
  hls0->Fill(float(lumiBlock));



#ifdef LUMI
  edm::LuminosityBlock const& iLumi = e.getLuminosityBlock();

  edm::Handle<LumiSummary> lumi;
  //iLumi.getByLabel("lumiProducer", lumi);
  iLumi.getByToken(LumiToken, lumi);

  edm::Handle<edm::ConditionsInLumiBlock> cond;
  //iLumi.getByLabel("conditionsInEdm", cond);
  iLumi.getByToken(ConditionsInLumiBlockToken, cond);

  float intlumi = 0, instlumi=0;
  int beamint1=0, beamint2=0;
  // This will only work when running on RECO until (if) they fix it in the FW
  // When running on RAW and reconstructing, the LumiSummary will not appear
  // in the event before reaching endLuminosityBlock(). Therefore, it is not
  // possible to get this info in the event
  if (lumi.isValid()) {
    intlumi =(lumi->intgRecLumi())/1000.;  // 10^30 -> 10^33/cm2/sec  ->  1/nb/sec
    instlumi=(lumi->avgInsDelLumi())/1000.;
    beamint1=(cond->totalIntensityBeam1)/1000;
    beamint2=(cond->totalIntensityBeam2)/1000;
  } else {
    //std::cout << "** ERROR: Event does not get lumi info\n";
  }

  hinstl->Fill(float(lumiBlock),float(instlumi));
  hintgl->Fill(float(lumiBlock),float(intlumi));
  hbeam1->Fill(float(lumiBlock),float(beamint1));
  hbeam2->Fill(float(lumiBlock),float(beamint2));
#endif

#ifdef L1
  // Get L1
  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  //e.getByLabel("gtDigis",L1GTRR);
  e.getByToken(L1TrigReadoutToken,L1GTRR);

  if (L1GTRR.isValid()) {
    //bool l1a = L1GTRR->decision();  // global decission?
    //cout<<" L1 status :"<<l1a<<" "<<hex;
    for (unsigned int i = 0; i < L1GTRR->decisionWord().size(); ++i) {
      int l1flag = L1GTRR->decisionWord()[i]; 
      int t1flag = L1GTRR->technicalTriggerWord()[i]; 
    
      if( l1flag>0 ) hl1a->Fill(float(i));
      if( t1flag>0 && i<64) hl1t->Fill(float(i));
    } // for loop
  } // if l1a
#endif

#ifdef HLT

  bool hlt[256];
  for(int i=0;i<256;++i) hlt[i]=false;


  // HLT results 
  edm::TriggerNames TrigNames;
  edm::Handle<edm::TriggerResults> HLTResults;
  //e.getByLabel(edm::InputTag("TriggerResults","","HLT"),HLTResults);
  e.getByToken(TrigResultsToken, HLTResults);

  if ((HLTResults.isValid() == true) && (HLTResults->size() > 0)) {

    //TrigNames.init(*HLTResults);
    const edm::TriggerNames & TrigNames = e.triggerNames(*HLTResults);

    //cout<<TrigNames.triggerNames().size()<<endl;

    for (unsigned int i = 0; i < TrigNames.triggerNames().size(); i++) {  // loop over trigger
      //if(countAllEvents==1) cout<<i<<" "<<TrigNames.triggerName(i)<<endl;

      if ( 
           (HLTResults->wasrun(TrigNames.triggerIndex(TrigNames.triggerName(i))) == true) &&
           (HLTResults->accept(TrigNames.triggerIndex(TrigNames.triggerName(i))) == true) &&
           (HLTResults->error( TrigNames.triggerIndex(TrigNames.triggerName(i))) == false) ) {

        hlt[i]=true;
        hlt1->Fill(float(i));

      } // if hlt

    } // loop 
  } // if valid
#endif


  // -- Does this belong into beginJob()?
  //ESHandle<TrackerGeometry> TG;
  //iSetup.get<TrackerDigiGeometryRecord>().get(TG);
  //const TrackerGeometry* theTrackerGeometry = TG.product();
  //const TrackerGeometry& theTracker(*theTrackerGeometry);
  // Get event setup 
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoH;
  es.get<TrackerTopologyRcd>().get(tTopoH);
  const TrackerTopology *tTopo=tTopoH.product();

#ifdef TRAJECTORY
  //string _ttrhBuilder = "WithAngleAndTemplate"; // in construct
  string _ttrhBuilder = "WithTrackAngle"; // in construct
  //----------------------------------------------------------------------------
  // Transient Rechit Builders
  edm::ESHandle<TransientTrackBuilder> theB;
  es.get<TransientTrackRecord>().get( "TransientTrackBuilder", theB ); // needs a refitter?
  
  // Transient rec hits:
  ESHandle<TransientTrackingRecHitBuilder> hitBuilder;
  es.get<TransientRecHitRecord>().get( _ttrhBuilder, hitBuilder );
  
  // Cloner:
  const TkTransientTrackingRecHitBuilder * builder =
    static_cast<TkTransientTrackingRecHitBuilder const *>(hitBuilder.product());
  auto hitCloner = builder->cloner();

  //theFitter->setHitCloner(&hitCloner);

  // TrackPropagator:
  //edm::ESHandle<Propagator> prop;
  //es.get<TrackingComponentsRecord>().get( "PropagatorWithMaterial", prop );
  //const Propagator* thePropagator = prop.product();

#endif


  // -- Primary vertices
  // ----------------------------------------------------------------------
  edm::Handle<reco::VertexCollection> vertices;
  //e.getByLabel("offlinePrimaryVertices", vertices);
  e.getByToken(VertexCollectionToken, vertices);
  
  if(PRINT) cout<<" PV list "<<vertices->size()<<endl;
  int pvNotFake = 0, pvsTrue = 0;
  vector<float> pvzVector;
  for (reco::VertexCollection::const_iterator iv = vertices->begin(); iv != vertices->end(); ++iv) {

    if( (iv->isFake()) == 1 ) continue; 
      pvNotFake++;
      float pvx = iv->x();
      float pvy = iv->y();
      float pvz = iv->z();
      //int numTracksPerPV = iv->tracksSize();
      //int numTracksPerPV = iv->nTracks();
      
      //float xe = iv->xError();
      //float ye = iv->yError();
      //float ze = iv->zError();      
      //int chi2 = iv->chi2(); 
      //int dof = iv->ndof(); 

      if(PRINT) cout<<" PV "<<pvNotFake<<" pos = "<<pvx<<"/"<<pvy<<"/"<<pvz<<endl;
      //<<", Num of tracks "<<numTracksPerPV<<endl;

      hpvz->Fill(pvz);
      if(pvz>-22. && pvz<22.) {
	float pvr = sqrt(pvx*pvx + pvy*pvy); 
	hpvxy->Fill(pvx,pvy);
	hpvr->Fill(pvr);
	if(pvr<0.3) {
	  pvsTrue++;
	  pvzVector.push_back(pvz);
	  //if(PRINT) cout<<"PV "<<pvsTrue<<" "<<pvz<<endl;
	} //pvr
      } // pvz
     
      //if(pvsTrue<1) continue; // skip events with no PV

  } // loop pvs
  hNumPv->Fill(float(pvNotFake));
  hNumPvClean->Fill(float(pvsTrue));

  if(PRINT) cout<<" Not fake PVs = "<<pvNotFake<<" good position "<<pvsTrue<<endl;
  
  // Clusters and rechits, not needed
  //edm::Handle<SiPixelRecHitCollection> hRecHitColl;
  //iEvent.getByToken(PixelRecHitToken, hRecHitColl); 

  // Get Cluster Collection from InputTag
  //edm::Handle< edmNew::DetSetVector<SiPixelCluster> > clusters;
  //e.getByToken( myClus , clusters);
  //const edmNew::DetSetVector<SiPixelCluster>& input = *clusters;     


  // -- Tracks
  // ----------------------------------------------------------------------
  Handle<reco::TrackCollection> recTracks;
  //edm::Handle<std::vector<reco::Track> > hTrackCollection;
  // e.getByLabel("generalTracks", recTracks);
  // e.getByLabel("ctfWithMaterialTracksP5", recTracks);
  // e.getByLabel("splittedTracksP5", recTracks);
  //e.getByLabel("cosmictrackfinderP5", recTracks);
  ///e.getByLabel(src_ , recTracks);

  e.getByToken(TrackToken, recTracks);
  bool missingHit=false, missingHitB=false;
  if(PRINT) cout<<" Tracks "<<recTracks->size()<<endl;


  float etaCut=-1, ptCut=-1;
  if(select1>0) {
    // skip events with few pixel dets
    if(select1==1) { if( int(recTracks->size())<select2) return; } 
    // select events only for a defined bx
    else if(select1==2) { if(bx!=select2) return; } 
    else if(select1==3) { if(bx==select2) return; } 

    //else if(select1==4) { if(  !( (bx==39)||(bx==201)||(bx==443)||(bx==499)||(bx==1083)||(bx==1337)||(bx==1492)||(bx==1977)||(bx==2231)||(bx==2287)||(bx==2871)||(bx==3224)||(bx==3280) )   ) return; } 
    //else if(select1==5) { if( ( (bx==1)||(bx==39)||(bx==201)||(bx==443)||(bx==499)||(bx==1083)||(bx==1337)||(bx==1492)||(bx==1977)||(bx==2231)||(bx==2287)||(bx==2871)||(bx==3224)||(bx==3280) )   ) return; } 
    // select specific event
    else if(select1==10) { if(event!=select2) return; } 

    else if(select1==11) { // eta cuts 
      if(select2==1)      etaCut=0.2; 
      else if(select2==2) etaCut=0.5; 
      else if(select2==3) etaCut=1.0; 
      else if(select2==4) etaCut=2.5; 
      else if(select2==5) etaCut=2.0; 
      else if(select2==6) etaCut=1.5; }
    else if(select1==12) {ptCut=float(select2);}  // pt cut
    else if(select1==13) {ptCut=float(select2);}  // very nice track cut
    else if(select1==14) {ptCut=float(select2);}  // very nice track cut
    else if(select1==15) {ptCut=1.;if(bx!=select2) return;}  //combined
    else if(select1==16) {ptCut=1.;if(bx!=select2) return;}  //combined
    //....
  }
  
  
  for(reco::TrackCollection::const_iterator t=recTracks->begin();
      t!=recTracks->end(); ++t){
    
    trackNumber++;
    numOfClusPerTrk1=0;  // this is confusing, it is used as clus per track        
    numOfClusPerTrk2=0;        
    numOfClusPerTrk3=0;        
    numOfClusPerTrk4=0;        
    numOfClusPerTrk5=0;        
    int pixelHits=0;
    
    int size = t->recHitsSize();
    float pt = t->pt();
    float eta = t->eta(); // track eta
    float phi = t->phi();  
    //float trackCharge = t->charge(); // unused 
    float d0 = t->d0();
    float dz = t->dz();
    //float tkvx = t->vx();  // unused 
    //float tkvy = t->vy();
    //float tkvz = t->vz();

    
    if(PRINT) cout<<"Track "<<trackNumber<<" Pt "<<pt<<" Eta "<<eta<<" d0/dz "<<d0<<" "<<dz
		  <<" Hits "<<size<<" cuts "<<etaCut<<" "<<ptCut<<endl;
    
    // applay special select cuts
    if( (select1==13) || (select1==15) )  {
      if(pt<ptCut) continue;   // cut on min pt
      if( (abs(eta)<1.75) || (abs(eta)>1.95) ) continue;
      if(d0>0.1) continue;   // cut on max d0
      if( abs(dz)>1.0 ) continue;
      //select=true;
    } else if( (select1==14) || (select1==16) )  {
      if(pt<ptCut) continue;   // cut on min pt
      if( abs(eta)>0.25 ) continue;
      if(d0>0.1) continue;   // cut on max d0
      if( abs(dz)>1.0 ) continue;
      //select=true;
    } else {
      
      if(etaCut>-1) {
	if(select2<4 && abs(eta)>etaCut) continue;   // cut on max eta
	if(select2>=4 && abs(eta)<etaCut) continue; // cut on min eta
      }
      if(ptCut>-1) {
	if(pt<ptCut) continue;   // cut on min pt
      }
    }


    hEta->Fill(eta);
    hPhi->Fill(phi);
    hDz->Fill(dz);

    // apply default selection cuts 
    if(TrackCuts && (abs(eta)>3.0 || abs(dz)>25.) ) continue;  //  skip  
    
    hD0->Fill(d0);
    if(TrackCuts && d0>1.0) continue; // skip 
    
    // hit pattern 
    HitPattern hp = t->hitPattern();
    int pa = hp.numberOfValidPixelHits();
    int pb = hp.numberOfValidPixelBarrelHits();
    int pf = hp.numberOfValidPixelEndcapHits();
    int numTrackLayers  = hp.trackerLayersWithMeasurement();
    int numBPixLayers   = hp.pixelBarrelLayersWithMeasurement();
    int pm1 = hp.numberOfLostPixelHits(HitPattern::HitCategory::TRACK_HITS);
    int pm2 = hp.numberOfLostPixelHits(HitPattern::HitCategory::MISSING_INNER_HITS);
    int pbm1 = hp.numberOfLostPixelBarrelHits(HitPattern::HitCategory::TRACK_HITS);
    int pbm2 = hp.numberOfLostPixelBarrelHits(HitPattern::HitCategory::MISSING_INNER_HITS);
    int pp=pa+pb+pf+pm1+pm2+pbm1+pbm2;
    if(PRINT && pp>0) cout<<hp.numberOfValidPixelHits()<< " "
		<<hp.numberOfValidPixelBarrelHits()<< " "
		<<hp.numberOfValidPixelEndcapHits()<<" "
		<<hp.numberOfLostPixelHits(HitPattern::HitCategory::TRACK_HITS)<<" "
		<<hp.numberOfLostPixelHits(HitPattern::HitCategory::MISSING_INNER_HITS)<<" "
		<<hp.numberOfLostPixelBarrelHits(HitPattern::HitCategory::TRACK_HITS)<<" "
		<<hp.numberOfLostPixelBarrelHits(HitPattern::HitCategory::MISSING_INNER_HITS)
		<<endl;

    if(pa>0) hstatus->Fill(0.);
    if(pb>0) hstatus->Fill(1.);
    if(pf>0) hstatus->Fill(2.);
    if(pm1>0) {hstatus->Fill(3.); missingHit=true;}
    if(pm2>0) {hstatus->Fill(4.); missingHit=true;}
    if(pbm1>0) {hstatus->Fill(5.); missingHitB=true;}
    if(pbm2>0) {hstatus->Fill(6.); missingHitB=true;}
    if(missingHitB) {hPhi0->Fill(phi);hEta0->Fill(eta);}

    hbpixHits->Fill(float(pb));
    hbpixLayers->Fill(float(numBPixLayers));
    htrackLayers->Fill(float(numTrackLayers));
    hbpixLayersPt->Fill( pt,float(numBPixLayers));
    htrackLayersPt->Fill(pt,float(numTrackLayers));
    hbpixLayersEta->Fill( eta,float(numBPixLayers));
    htrackLayersEta->Fill(eta,float(numTrackLayers));

    bool goodTrack = false; 
    float zpv=-999., tmp0=999.;
    for(vector<float>::iterator m=pvzVector.begin(); m!=pvzVector.end();++m) {
      float z = *m;
      float tmp = abs(z-dz);
      hzdiff->Fill(tmp); 
      if(tmp<tmp0) {zpv=z; tmp0=tmp;}
      if( tmp < 1.) goodTrack=true; 
    }

    // select only tracks associated to a vertex close to z=0
    if(select1==13 || select1==14|| select1==15|| select1==16) {
      if( !goodTrack ||  (abs(zpv)>1.) ) continue; 
    }

    if(isData && TrackCuts && !goodTrack) continue;
    countNiceTracks++;      
    hPt->Fill(pt);
        
    //cout<<" rechits : " <<endl;


#ifdef TRAJECTORY
    // transient track:
    TransientTrack tTrack = theB->build(*t);

    TrajectoryStateOnSurface initialTSOS = tTrack.innermostMeasurementState();

    double kap = tTrack.initialFreeState().transverseCurvature();
    const Surface::GlobalPoint origin = Surface::GlobalPoint(0,0,0);
    TrajectoryStateClosestToPoint tscp = tTrack.trajectoryStateClosestToPoint( origin );

    if( tscp.isValid() ) {
      cout<<" trajectory ok"<<endl;
    }
#endif
    // Loop over rechits
    for ( trackingRecHit_iterator recHit = t->recHitsBegin();
	  recHit != t->recHitsEnd(); ++recHit ) {
      
      if ( !((*recHit)->isValid()) ) continue;

      if( (*recHit)->geographicalId().det() != DetId::Tracker ) continue; 

      const DetId & hit_detId = (*recHit)->geographicalId();
      uint IntSubDetID = (hit_detId.subdetId());

      //cout<<IntSubDetID<<endl;
      // Select valid rechits	
      if(IntSubDetID == 0 ) continue;  // Select ??

#ifdef TRAJECTORY
      auto tmprh =
	(*recHit)->cloneForFit(*builder->geometry()->idToDet((**recHit).geographicalId()));
      auto transRecHit = hitCloner.makeShared(tmprh, initialTSOS);

      //myTTRHvec.push_back( transRecHit );
      //coTTRHvec.push_back( transRecHit );

      double xloc = transRecHit->localPosition().x();// 1st meas coord
      double yloc = transRecHit->localPosition().y();// 2nd meas coord or zero
#endif



      int layer=0, ladderIndex=0, zindex=0, ladderOn=0, module=0, shell=0;
      bool badL2Modules = false;
      bool goodL2Modules = false;
      bool newL1Modules = false;

      unsigned int disk=0; //1,2,3
      unsigned int blade=0; //1-24
      unsigned int zindexF=0; //
      unsigned int side=0; //size=1 for -z, 2 for +z
      unsigned int panel=0; //panel=1
      hstatus->Fill(7.);

      bool inner = false; // inner and outer ladders
      if(IntSubDetID == PixelSubdetector::PixelBarrel) {  // bpix

	if(PRINT) cout<<" a pixel b rechit "<<endl;
	hstatus->Fill(11.);

	layer=tTopo->pxbLayer(hit_detId);
	ladderIndex=tTopo->pxbLadder(hit_detId);
	zindex=tTopo->pxbModule(hit_detId);
	if(zindex<5) side=1; else side=2;
	PixelBarrelName pbn(hit_detId,tTopo,phase1_);

	// Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
	PixelBarrelName::Shell sh = pbn.shell(); //enum
	//sector = pbn.sectorName();
	ladderOn = pbn.ladderName();
	//layerOn  = pbn.layerName();
	module = pbn.moduleName(); // 1 to 4
	//half  = pbn.isHalfModule();
	shell = int(sh);
	// change the module sign for z<0
	if(shell==1 || shell==2) module = -module; // make -1 to -4 for -z
	// change ladeer sign for Outer )x<0)
	if(shell==1 || shell==3) ladderOn = -ladderOn;
	
	if( layer==2 && select1!=9998) {
	  if(      (ladderOn ==-1) &&( (module == 1)  || (module == 2)  || (module == 3)) ) badL2Modules=true;
	  else if( (ladderOn ==-5) &&( (module == -1) || (module == -2)) ) badL2Modules=true;
	  else if( (ladderOn == 14) && (module == -1) ) badL2Modules=true;
	  else if( (ladderOn == 13) && (module == -4) ) badL2Modules=true;
	  else if( (ladderOn == 12) && (module == -1) ) badL2Modules=true;
	  else if( (ladderOn == 11) && (module == -4) ) badL2Modules=true;

	  else if( (ladderOn == 1) &&( (module == 1) || (module == 2) || (module == 3))) goodL2Modules=true;
	  else if( (ladderOn ==-1) &&( (module ==-1) || (module ==-2) || (module ==-3))) goodL2Modules=true;
	  else if( (ladderOn ==-8) &&( (module ==-1) || (module ==-2) || (module ==-3))) goodL2Modules=true;
	  else if( (ladderOn == 7) && (module == 1) ) goodL2Modules=true;
	  else if( (ladderOn == 6) && (module == 4) ) goodL2Modules=true;
	  else if( (ladderOn ==-5) && (module ==-3) ) goodL2Modules=true;

	}
	
	// find inner and outer modules for layer 1 onl
	if( (layer==1)  ) {
	  if( (ladderOn==2) || (ladderOn==4) || (ladderOn==6) ||
	      (ladderOn==-1) || (ladderOn==-3) || (ladderOn==-5) ) inner=true;
	  else inner=false;
	  
	  if( select1!=9998 ) {
	    if     ( (ladderOn ==-1) && (module == 3) ) newL1Modules=true;
	    else if( (ladderOn ==-3) && (module == 3) ) newL1Modules=true;
	    else if( (ladderOn ==-1) && (module ==-3) ) newL1Modules=true;
	    else if( (ladderOn ==-1) && (module ==-1) ) newL1Modules=true;
	    else if( (ladderOn ==-3) && (module ==-1) ) newL1Modules=true;
	    else if( (ladderOn ==-5) && (module ==-1) ) newL1Modules=true;
	  }
	}
      
	if(PRINT) cout<<"barrel layer/ladder/module: "<<layer<<"/"<<ladderIndex<<"/"<<zindex<<endl;
	
      } else if(IntSubDetID == PixelSubdetector::PixelEndcap) {  // fpix

	if(PRINT) cout<<" a pixel f rechit "<<endl;
	hstatus->Fill(21.);

	disk=tTopo->pxfDisk(hit_detId); //1,2,3
	blade=tTopo->pxfBlade(hit_detId); //1-24
	zindexF=tTopo->pxfModule(hit_detId); //
	side=tTopo->pxfSide(hit_detId); //size=1 for -z, 2 for +z
	panel=tTopo->pxfPanel(hit_detId); //panel=1
	PixelEndcapName pen(hit_detId,tTopo,phase1_);

	// PXFDetId pdetId = PXFDetId(hit_detId);       
	// disk=pdetId.disk(); //1,2,3
	// blade=pdetId.blade(); //1-24
	// zindexF=pdetId.module(); //
	// side=pdetId.side(); //size=1 for -z, 2 for +z
	// panel=pdetId.panel(); //panel=1
	
	if(PRINT) cout<<" forward det, disk "<<disk<<", blade "
		      <<blade<<", module "<<zindexF<<", side "<<side<<", panel "
		      <<panel<<endl;

      } else { // nothings
	continue; // skip non pixel hits 
      }


      if(select1==201) { // select a specific module 
	if( (layer!=selectLayer) || (ladderOn!=selectLadder) || (module!=selectModule) ) continue; // skip module 
      }

      hstatus->Fill(8.);

      // Get the geom-detector
      const PixelGeomDetUnit * theGeomDet =
	dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(hit_detId) );
      //double detZ = theGeomDet->surface().position().z();  // unused 
      //double detR = theGeomDet->surface().position().perp(); // unused 
      const PixelTopology * topol = &(theGeomDet->specificTopology());  // pixel topology
      
      //std::vector<SiStripRecHit2D*> output = getRecHitComponents((*recHit).get()); 
      //std::vector<SiPixelRecHit*> TrkComparison::getRecHitComponents(const TrackingRecHit* rechit){

      const SiPixelRecHit* hit = dynamic_cast<const SiPixelRecHit*>((*recHit)->hit());

      if(hit) {

	//if(hit->clusterProbability(0)>1.) {
	  // cout<<hit->localPosition().x()<<" ";
	  //cout<<hit->localPosition().y()<<" ";
	// cout<<hit->localPositionError().xx()<<" ";
	// cout<<hit->localPositionError().yy()<<" ";
	//cout<<hit->clusterProbability(0)<<" ";  // XY
	//cout<<hit->clusterProbability(1)<<" ";  // XY*Q 
	//cout<<hit->clusterProbability(2)<<" ";  // Q
	// cout<<hit->probabilityX()<<" ";
	// cout<<hit->probabilityY()<<" ";
	// cout<<hit->probabilityXY()<<" ";
	// cout<<hit->probabilityQ()<<" ";
	//cout<<endl;
	//}


#ifdef ALIGN_STUDIES
	if(pt>1.) { // eliminate low pt tracks
	  // RecHit (recthits are transient, so not available without track refit)
	  double xloc = hit->localPosition().x();// 1st meas coord
	  double yloc = hit->localPosition().y();// 2nd meas coord or zero
	  //double zloc = hit->localPosition().z();// up, always zero
	  LocalError lerr = hit->localPositionError();
	  float lerr_x = sqrt(lerr.xx())*1E4;
	  float lerr_y = sqrt(lerr.yy())*1E4;
	  
	  if( layer == 1) 
	    {recHitXError1->Fill(lerr_x); recHitYError1->Fill(lerr_y);
	      hErrorXB->Fill(float(ladderIndex+(110*(side-1))),lerr_x);
	      hErrorYB->Fill(float(ladderIndex+(110*(side-1))),lerr_y);
	    }
	  else if( layer == 2) 
	    {recHitXError2->Fill(lerr_x); recHitYError2->Fill(lerr_y);
	      hErrorXB->Fill(float(ladderIndex+25+(110*(side-1))),lerr_x);
	      hErrorYB->Fill(float(ladderIndex+25+(110*(side-1))),lerr_y);

	    }
	  else if( layer == 3) 
	    {recHitXError3->Fill(lerr_x); recHitYError3->Fill(lerr_y);
	      hErrorXB->Fill(float(ladderIndex+60+(110*(side-1))),lerr_x);
	      hErrorYB->Fill(float(ladderIndex+60+(110*(side-1))),lerr_y);
	    }
	  else if( (disk == 2) && (side==1) ) 
	    {recHitXError4->Fill(lerr_x); recHitYError4->Fill(lerr_y);
	      hErrorXF->Fill(float(blade),lerr_x);
	      hErrorYF->Fill(float(blade),lerr_y);

	    }
	  else if( (disk == 1) && (side==1) ) 
	    {recHitXError5->Fill(lerr_x); recHitYError5->Fill(lerr_y);
	      hErrorXF->Fill(float(blade+25),lerr_x);
	      hErrorYF->Fill(float(blade+25),lerr_y);

	    }
	  else if( (disk == 1) && (side==2) ) 
	    {recHitXError6->Fill(lerr_x); recHitYError6->Fill(lerr_y);
	      hErrorXF->Fill(float(blade+50),lerr_x);
	      hErrorYF->Fill(float(blade+50),lerr_y);
	    }
	  else if( (disk == 2) && (side==2) ) 
	    {recHitXError7->Fill(lerr_x); recHitYError7->Fill(lerr_y);
	      hErrorXF->Fill(float(blade+75),lerr_x);
	      hErrorYF->Fill(float(blade+75),lerr_y);
	    }
	  
	  LocalError lape = theGeomDet->localAlignmentError();
	  if (lape.valid()) {
	    float tmp11= 0.;
	    if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
	    //float tmp12= sqrt(lape.xy())*1E4;
	    float tmp13= 0.;
	    if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
	    //bool tmp14=tmp2<tmp1;
	    if( layer == 1) 
	      {recHitXAlignError1->Fill(tmp11); recHitYAlignError1->Fill(tmp13);
		hAErrorXB->Fill(float(ladderIndex+(110*(side-1))),tmp11);
		hAErrorYB->Fill(float(ladderIndex+(110*(side-1))),tmp13);
	      }
	    else if( layer == 2) 
	      {recHitXAlignError2->Fill(tmp11); recHitYAlignError2->Fill(tmp13);
		hAErrorXB->Fill(float(ladderIndex+25+(110*(side-1))),tmp11);
		hAErrorYB->Fill(float(ladderIndex+25+(110*(side-1))),tmp13);
	      }
	    else if( layer == 3) 
	      {recHitXAlignError3->Fill(tmp11); recHitYAlignError3->Fill(tmp13);
		hAErrorXB->Fill(float(ladderIndex+60+(110*(side-1))),tmp11);
		hAErrorYB->Fill(float(ladderIndex+60+(110*(side-1))),tmp13);

	      }
	    else if( (disk == 2) && (side==1) ) 
	      {recHitXAlignError4->Fill(tmp11); recHitYAlignError4->Fill(tmp13);
		hAErrorXF->Fill(float(blade),tmp11);
		hAErrorYF->Fill(float(blade),tmp13);

	      }
	    else if( (disk == 1) && (side==1) ) 
	      {recHitXAlignError5->Fill(tmp11); recHitYAlignError5->Fill(tmp13);
		hAErrorXF->Fill(float(blade+25),tmp11);
		hAErrorYF->Fill(float(blade+25),tmp13);
	      }
	    else if( (disk == 1) && (side==2) ) 
	      {recHitXAlignError6->Fill(tmp11); recHitYAlignError6->Fill(tmp13);
		hAErrorXF->Fill(float(blade+50),tmp11);
		hAErrorYF->Fill(float(blade+50),tmp13);
	      }
	    else if( (disk == 2) && (side==2) ) 
	      {recHitXAlignError7->Fill(tmp11); recHitYAlignError7->Fill(tmp13);
		hAErrorXF->Fill(float(blade+75),tmp11);
		hAErrorYF->Fill(float(blade+75),tmp13);
	      }
	    
	    
	    //cout<<tTopo->pxbLayer(detId)<<" "<<tTopo->pxbModule(detId)<<" "<<rows<<" "<<tmp14<<" "
	    if(PRINT) cout<<" align error "<<layer<<tmp11<<" "<<tmp13<<endl;
	  } else {
	    cout<<" lape = 0"<<endl;
	  }  // if lape
	  
	  if(PRINT) cout<<" rechit loc "<<xloc<<" "<<yloc<<" "<<lerr_x<<" "
			<<lerr_y<<endl;
	} // limit pt
#endif  // ALIGN_STUDIES      
	
	hstatus->Fill(8.);
	edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = hit->cluster();

	//  check if the ref is not null
	if (!clust.isNonnull()) continue;
	hstatus->Fill(9.);
      
	numberOfClusters++;
	pixelHits++;
	float charge = (clust->charge())/1000.0; // convert electrons to kilo-electrons
	int size = clust->size();
	int sizeX = clust->sizeX();
	int sizeY = clust->sizeY();
	float row = clust->x(); //x
	float col = clust->y(); // y
	numberOfPixels += size;
	
	//cout<<" clus loc "<<row<<" "<<col<<endl;
	
	//select = (abs(eta)>2.)&&(abs(module)==4); 
	if(PRINT) cout<<" cluster "<<numberOfClusters<<" charge = "<<charge<<" size = "<<size<<endl;
	
	LocalPoint lp = topol->localPosition( MeasurementPoint( clust->x(), clust->y() ) );
	float lx = lp.x();
	float ly = lp.y();
	//cout<<" clu loc "<<x<<" "<<y<<endl;
	
	GlobalPoint clustgp = theGeomDet->surface().toGlobal( lp );
	double gX = clustgp.x();
	double gY = clustgp.y();
	double gZ = clustgp.z();
	
	
	TVector3 v(gX,gY,gZ);
	float gPhi = v.Phi(); // phi of the hit
	float gR = v.Perp(); // r of the hit
	
	if(PRINT) cout<<" CLU GLOBAL "<<gX<<" "<<gY<<" "<<gZ<<" "<<gR<<" "<<gPhi<<endl;

#if defined(CLU_SHAPE) || defined(CLU_SHAPE_L2) 
	int maxPixelCol = clust->maxPixelCol();
	int maxPixelRow = clust->maxPixelRow();
	int minPixelCol = clust->minPixelCol();
	int minPixelRow = clust->minPixelRow();
	bool same = false;
	int corner=0;
#endif
	//int geoId = PixGeom->geographicalId().rawId();
	// Replace with the topology methods
	// edge method moved to topologi class
	//int edgeHitX = (int) ( topol->isItEdgePixelInX( minPixelRow ) || topol->isItEdgePixelInX( maxPixelRow ) );
	//int edgeHitY = (int) ( topol->isItEdgePixelInY( minPixelCol ) || topol->isItEdgePixelInY( maxPixelCol ) );
	
	// calculate alpha and beta from cluster position
	//LocalTrajectoryParameters ltp = tsos.localParameters();
	//LocalVector localDir = ltp.momentum()/ltp.momentum().mag();
	//float locx = localDir.x();
	//float locy = localDir.y();
	//float locz = localDir.z();
	//float loctheta = localDir.theta(); // currently unused
	//float alpha = atan2( locz, locx );
	//float beta = atan2( locz, locy );
	
	if(layer==1) {
	  if(select) cout<<"LAYER1: Run "<<run<<" Event "<<event<<" bx "<<bx
			 <<" track "<<trackNumber<<" eta "<<eta<<" pt "<<pt
			 <<" ladder/module "<<ladderOn<<"/"<<module
			 <<" cluster "<<numberOfClusters<<" x/y "<<row<<"/"<<col
			 <<" charge = "<<charge
			 <<" size = "<<size<<"/"<<sizeX<<"/"<<sizeY
			 <<endl;

	    hDetMap1->Fill(float(module),float(ladderOn)); 
	    hsizeDets1->Fill(float(module),float(ladderOn),float(size));
	    hchargeDets1->Fill(float(module),float(ladderOn),charge);
	    hcluDetMap1->Fill(col,row);
	    hcols1->Fill(col);
	    hrows1->Fill(row);
	    hcols11->Fill(ly);
	    hrows11->Fill(lx);
	    hladder1id->Fill(float(ladderOn));
	    hz1id->Fill(float(module));

	    if(newL1Modules) hcharge1n->Fill(charge);
	    else             hcharge1->Fill(charge);

	    hbpixXY->Fill(gX,gY);
	    hzphi1->Fill(gZ,gPhi);  // hit phi

	    htest->Fill(eta,float(sizeY));
	    htest1->Fill(charge,float(sizeY));

	    hsizeyz1->Fill(eta,float(sizeY));

	    hetaphiMap1->Fill(eta,phi);
	    hstatus->Fill(12.);

	    if(inner) {hPhi11->Fill(phi); hEta11->Fill(eta);}  
	    else      {hPhi12->Fill(phi); hEta12->Fill(eta);}  

	    if(pt>CLU_SIZE_PT_CUT) {
	      if(newL1Modules) {
		hsize1n->Fill(float(size));
		hsizex1n->Fill(float(sizeX));
		hsizey1n->Fill(float(sizeY));
	      } else {
		hsize1->Fill(float(size));
		hsizex1->Fill(float(sizeX));
		hsizey1->Fill(float(sizeY));
	      }

#ifdef STUDY_ONEMOD
	    hsizeMap1->Fill(col,row,size);
	    hsizeXMap1->Fill(col,row,sizeX);
	    hsizeYMap1->Fill(col,row,sizeY);
	    hclucharMap1->Fill(col,row,charge);
#endif

#ifdef USE_PROFILES
	      hclumult1->Fill(eta,float(size));
	      hclumultx1->Fill(eta,float(sizeX));
	      hclumulty1->Fill(eta,float(sizeY));
	      hcluchar1->Fill(eta,float(charge));

	      //cout<<ladder<<" "<<ladderOn<<endl;

	      hclumultld1->Fill(float(ladderOn),size);
	      hclumultxld1->Fill(float(ladderOn),sizeX);
	      hclumultyld1->Fill(float(ladderOn),sizeY);
	      hclucharld1->Fill(float(ladderOn),charge);
#endif
#ifdef PHI_PROFILES
	      hclumultxPhi1->Fill(phi,float(sizeX));
	      hclumultyPhi1->Fill(phi,float(sizeY));
	      hclucharPhi1->Fill(phi,float(charge));
#endif
	    }

#ifdef USE_PROFILES
	    hmult1->Fill(zindex,float(size));
#endif

	      

#ifdef CLU_SHAPE 
	    if( (minPixelCol%2)==0 ) { // event col
	      if( (minPixelRow%2)==0 ) corner=1; // event row
	      else corner=3;
	    } else { // odd col
	      if( (minPixelRow%2)==0 ) corner=2; // event row
	      else corner=4;
	    }

	    if(maxPixelCol == minPixelCol) {
	      if( (minPixelRow%2)==0 ) same=true;// event row 
	    } else {
	      if( (minPixelCol%2)==0 ) same=true;// event col
	    }
	    
	    histogramClus(charge, size, sizeX, sizeY, same, corner);
#endif


#ifdef STUDY_LAY1
	    if(abs(module)==4) {
	      hEta14->Fill(eta);
	      hPt14->Fill(pt);
	      hsizex14->Fill(float(sizeX));
	      hsizey14->Fill(float(sizeY));
	      hcharge14->Fill(charge);
	    } else if(abs(module)==1) {
	      hEta11->Fill(eta);
	      hPt11->Fill(pt);
	      hsizex11->Fill(float(sizeX));
	      hsizey11->Fill(float(sizeY));
	      hcharge11->Fill(charge);
	    }
	    if(pt>1.) {
	      hEta15->Fill(eta);
	      hPt15->Fill(pt);
	      hsizex15->Fill(float(sizeX));
	      hsizey15->Fill(float(sizeY));
	      hcharge15->Fill(charge);
	    }
#endif 

#ifdef OVERLAP_STUDIES
	    hcluphi1->Fill(gPhi);
#endif

#ifdef LS_STUDIES
	    hcharCluls->Fill(lumiBlock,charge);
	    hsizeCluls->Fill(lumiBlock,size);
	    hsizeXCluls->Fill(lumiBlock,sizeX);
	    hcharCluls1->Fill(lumiBlock,charge);
	    hsizeCluls1->Fill(lumiBlock,size);
	    hsizeXCluls1->Fill(lumiBlock,sizeX);
#endif

	    hProbabilityXYBpix->Fill(hit->clusterProbability(0));
	    hProbabilityQBpix->Fill(hit->clusterProbability(2));

	    numOfClusPerTrk1++;
	    numOfClustersPerLay1++;
	    numOfPixelsPerLay1 += size;     
   	
	} else if(layer==2) {
	  
	  if(select) cout<<"LAYER2: Run "<<run<<" Event "<<event<<" bx "<<bx
			 <<" track "<<trackNumber<<" eta "<<eta<<" pt "<<pt
			 <<" ladder/module "<<ladderOn<<"/"<<module
			 <<" cluster "<<numberOfClusters<<" x/y "<<row<<"/"<<col
			     <<" charge = "<<charge
			 <<" size = "<<size<<"/"<<sizeX<<"/"<<sizeY
			 <<endl;
	  
	  hDetMap2->Fill(float(module),float(ladderOn));
	  hsizeDets2->Fill(float(module),float(ladderOn),float(size));
	  hchargeDets2->Fill(float(module),float(ladderOn),charge);

	  hladder2id->Fill(float(ladderOn));
	  hz2id->Fill(float(module));
	  
	  //hPhi2->Fill(phi);
	  hbpixXY->Fill(gX,gY);
	  hzphi2->Fill(gZ,gPhi);
	  //htest->Fill(gPhi,float(size));
	  //htest2->Fill(gPhi,float(sizeX));
	  //htest3->Fill(gPhi,float(sizeY));
	  
	  hetaphiMap2->Fill(eta,phi);
	  hstatus->Fill(13.);

	  if(badL2Modules) {  // behind dcdc, bad 
	    hcharge2b->Fill(charge);
	    if(pt>CLU_SIZE_PT_CUT) hsize2b->Fill(float(size));

	  } else if(goodL2Modules) { // behind dcdc ok
	    hcharge2g->Fill(charge);
	    if(pt>CLU_SIZE_PT_CUT) hsize2g->Fill(float(size));

	  } else { // really good

	    hcharge2->Fill(charge);
	    if(pt>CLU_SIZE_PT_CUT) {
	      hsize2->Fill(float(size));
	      hsizex2->Fill(float(sizeX));
	      hsizey2->Fill(float(sizeY));
	    }

	    hcluDetMap2->Fill(col,row);
	    hcols2->Fill(col);
	    hrows2->Fill(row);
	    
	    hProbabilityXYBpix->Fill(hit->clusterProbability(0));
	    hProbabilityQBpix->Fill(hit->clusterProbability(2));
	    
	    hsizeyz2->Fill(eta,float(sizeY));
	    htest3->Fill(charge,float(sizeY));

#ifdef PHI_PROFILES
	    hclumultxPhi2->Fill(phi,float(sizeX));
	    hclumultyPhi2->Fill(phi,float(sizeY));
	    hclucharPhi2->Fill(phi,float(charge));
#endif
#ifdef USE_PROFILES
	    hmult2->Fill(zindex,float(size));
	    if(pt>CLU_SIZE_PT_CUT) {
	      hclumult2->Fill(eta,float(size));
	      hclumultx2->Fill(eta,float(sizeX));
	      hclumulty2->Fill(eta,float(sizeY));
	      hcluchar2->Fill(eta,float(charge));
	      hclumultld2->Fill(float(ladderOn),size);
	      hclumultxld2->Fill(float(ladderOn),sizeX);
	      hclumultyld2->Fill(float(ladderOn),sizeY);
	      hclucharld2->Fill(float(ladderOn),charge);
	    }
#endif
	    
#ifdef LS_STUDIES
	    hcharCluls->Fill(lumiBlock,charge);
	    hsizeCluls->Fill(lumiBlock,size);
	    hsizeXCluls->Fill(lumiBlock,sizeX);
	    hcharCluls2->Fill(lumiBlock,charge);
	    hsizeCluls2->Fill(lumiBlock,size);
	    hsizeXCluls2->Fill(lumiBlock,sizeX);
#endif

	  } // if badL2

	      
#ifdef CLU_SHAPE_L2
	  if( (minPixelCol%2)==0 ) { // event col
	    if( (minPixelRow%2)==0 ) corner=1; // event row
	    else corner=3;
	  } else { // odd col
	    if( (minPixelRow%2)==0 ) corner=2; // event row
	    else corner=4;
	  }
	  
	  if(maxPixelCol == minPixelCol) {
	    if( (minPixelRow%2)==0 ) same=true;// event row 
	  } else {
	    if( (minPixelCol%2)==0 ) same=true;// event col
	  }
	  
	  histogramClus(charge, size, sizeX, sizeY, same, corner);
#endif
	  
#ifdef OVERLAP_STUDIES
	  hcluphi2->Fill(gPhi);
#endif
	  	  
	  numOfClusPerTrk2++;
	  numOfClustersPerLay2++;
	  numOfPixelsPerLay2 += size;     

	} else if(layer==3) {
	
	  hDetMap3->Fill(float(module),float(ladderOn));
	  hcluDetMap3->Fill(col,row);
	  hsizeDets3->Fill(float(module),float(ladderOn),float(size));
	  hchargeDets3->Fill(float(module),float(ladderOn),charge);
	  hcharge3->Fill(charge);
	  hcols3->Fill(col);
	  hrows3->Fill(row);
	  hladder3id->Fill(float(ladderOn));
	  hz3id->Fill(float(module));
	  //hPhi3->Fill(phi);
	  hbpixXY->Fill(gX,gY);
	  hzphi3->Fill(gZ,gPhi);
	  //htest->Fill(gPhi,float(size));
	  //htest2->Fill(gPhi,float(sizeX));
	  //htest3->Fill(gPhi,float(sizeY));
	  
	  hetaphiMap3->Fill(eta,phi);
	  hstatus->Fill(14.);
	  
#ifdef OVERLAP_STUDIES
	  hcluphi3->Fill(gPhi);
#endif
#ifdef USE_PROFILES
	  hmult3->Fill(zindex,float(size));
#endif 
	  
	  if(pt>CLU_SIZE_PT_CUT) {
	    hsize3->Fill(float(size));
	    hsizex3->Fill(float(sizeX));
	    hsizey3->Fill(float(sizeY));
#ifdef USE_PROFILES
	    hclumult3->Fill(eta,float(size));
	    hclumultx3->Fill(eta,float(sizeX));
	    hclumulty3->Fill(eta,float(sizeY));
	    hcluchar3->Fill(eta,float(charge));
	    hclumultld3->Fill(float(ladderOn),size);
	    hclumultxld3->Fill(float(ladderOn),sizeX);
	    hclumultyld3->Fill(float(ladderOn),sizeY);
	    hclucharld3->Fill(float(ladderOn),charge);
#endif
	  }

#ifdef LS_STUDIES
	  hcharCluls->Fill(lumiBlock,charge);
	  hsizeCluls->Fill(lumiBlock,size);
	  hsizeXCluls->Fill(lumiBlock,sizeX);
	  hcharCluls3->Fill(lumiBlock,charge);
	  hsizeCluls3->Fill(lumiBlock,size);
	  hsizeXCluls3->Fill(lumiBlock,sizeX);
#endif
	  
	  hsizeyz3->Fill(eta,float(sizeY));
	  htest3->Fill(charge,float(sizeY));
	  
	  hProbabilityXYBpix->Fill(hit->clusterProbability(0));
	  hProbabilityQBpix->Fill(hit->clusterProbability(2));
	  
	  numOfClusPerTrk3++;
	  numOfClustersPerLay3++;
	  numOfPixelsPerLay3 += size;     
	  
	} else if(layer==4) {
	  
	  hDetMap4->Fill(float(module),float(ladderOn));
	  hcluDetMap4->Fill(col,row);
	  hsizeDets4->Fill(float(module),float(ladderOn),float(size));
	  hchargeDets4->Fill(float(module),float(ladderOn),charge);
	  hcharge4->Fill(charge);
	  hladder4id->Fill(float(ladderOn));
	  hz4id->Fill(float(module));
	  hcols4->Fill(col);
	  hrows4->Fill(row);
	  //hPhi4->Fill(phi);
	  hbpixXY->Fill(gX,gY);
	  hzphi4->Fill(gZ,gPhi);
	  hetaphiMap4->Fill(eta,phi);
	  hstatus->Fill(15.);
	  
	  if(pt>CLU_SIZE_PT_CUT) {
	    hsize4->Fill(float(size));
	    hsizex4->Fill(float(sizeX));
	    hsizey4->Fill(float(sizeY));
	  }
	  
	  hsizeyz4->Fill(eta,float(sizeY));
	  htest3->Fill(charge,float(sizeY));
	  
	  hProbabilityXYBpix->Fill(hit->clusterProbability(0));
	  hProbabilityQBpix->Fill(hit->clusterProbability(2));
	  
	  numOfClusPerTrk4++;
	  numOfClustersPerLay4++;
	  numOfPixelsPerLay4 += size;     
	  
	} else if (disk==1) {
	  
	  numOfClusPerTrk5++;
	  numOfClustersPerLay5++;
	  numOfPixelsPerLay5 += size;     
	  hstatus->Fill(22.);
	  //hPhi5->Fill(phi);
	  
	  
	  hcharge5->Fill(charge);
	  if(pt>CLU_SIZE_PT_CUT) {
	    hsize5->Fill(float(size));
	    hsizex5->Fill(float(sizeX));
	    hsizey5->Fill(float(sizeY));
	  }
	  
	  hfpixXY1->Fill(gX,gY);
	  
	  if(side==1) {numOfClustersPerDisk3++; hstatus->Fill(23.);}  // -z
	  else if(side==2) {numOfClustersPerDisk4++; hstatus->Fill(24.);} //+z
	  
	  hProbabilityXYFpix->Fill(hit->clusterProbability(0));
	  hProbabilityQFpix->Fill(hit->clusterProbability(2));
	  
	} else if (disk==2) {
	  
	  numOfClusPerTrk6++;
	  numOfClustersPerLay6++;
	  numOfPixelsPerLay6 += size;   
	  hstatus->Fill(25.);
	  //hPhi6->Fill(phi);
	  
	  hcharge6->Fill(charge);
	  if(pt>CLU_SIZE_PT_CUT) {
	    hsize6->Fill(float(size));
	    hsizex6->Fill(float(sizeX));
	    hsizey6->Fill(float(sizeY));
	  }
	  
	  hfpixXY2->Fill(gX,gY);
	  
	  if(side==1)      {numOfClustersPerDisk2++; hstatus->Fill(26.);} //-z
	  else if(side==2) {numOfClustersPerDisk5++; hstatus->Fill(27.);} //+z
	  
	  hProbabilityXYFpix->Fill(hit->clusterProbability(0));
	  hProbabilityQFpix->Fill(hit->clusterProbability(2));
	  
	} else if (disk==3) {
	  
	  numOfClusPerTrk7++;
	  numOfClustersPerLay7++;
	  numOfPixelsPerLay7 += size;   
	  hstatus->Fill(28.);
	  //hPhi7->Fill(phi);
	  
	  hcharge7->Fill(charge);
	  if(pt>CLU_SIZE_PT_CUT) {
	    hsize7->Fill(float(size));
	    hsizex7->Fill(float(sizeX));
	    hsizey7->Fill(float(sizeY));
	  }
	  
	  hfpixXY3->Fill(gX,gY);
	  
	  if(side==1)      {numOfClustersPerDisk1++; hstatus->Fill(29.);} //-z
	  else if(side==2) {numOfClustersPerDisk6++; hstatus->Fill(30.);} //+z
	  
	  hProbabilityXYFpix->Fill(hit->clusterProbability(0));
	  hProbabilityQFpix->Fill(hit->clusterProbability(2));
	  
	} else {
	  cout<<" which layer is this? "<<layer<<" "<< disk<<endl;
	} // if layer


	// Get the pixels in the Cluster
	const vector<SiPixelCluster::Pixel>& pixelsVec = clust->pixels();
	if(PRINT) cout<<" Pixels in this cluster (i/x/y/char)"<<endl;
	for (unsigned int i = 0;  i < pixelsVec.size(); ++i) { // loop over pixels
	  //sumPixels++;
	  //numberOfPixels++;
	  float pixx = pixelsVec[i].x; // index as float=iteger, row index
	  float pixy = pixelsVec[i].y; // same, col index
	  float adc = (float(pixelsVec[i].adc)/1000.);
	  int ladder = ladderOn;  //to mimic the code from CluAna

	  if(layer==1) {
	    if(select) cout<<i<<" "<<pixx<<" "<<pixy<<" "<<adc<<endl;

	    if(newL1Modules) hpixcharge1n->Fill(adc);
	    else             hpixcharge1->Fill(adc);
	    hpixDetMap1->Fill(pixy,pixx);
	    hpixchargeDets1->Fill(float(module),float(ladderOn),adc);
 	    htest2->Fill(adc,float(sizeY));
	    hpladder1id->Fill(float(ladderOn));
	    hpz1id->Fill(float(module));
	    hpcols1->Fill(pixy);
	    hprows1->Fill(pixx);

#ifdef PHI_PROFILES
	    hpixcharPhi1->Fill(phi,adc);
#endif

#ifdef CLU_SHAPE 
	    histogramPix(adc, size, sizeX, sizeY, same, corner);
#endif

#ifdef STUDY_LAY1
	    if(abs(module)==4) {
	      hpixcharge14->Fill(adc);
	    } else if(abs(module)==1) {
	      hpixcharge11->Fill(adc);
	    }
	    if(pt>1.) {
	      hpixcharge15->Fill(adc);
	    }

	    //if(sizeY==1)      hpixDetMap35->Fill(pixy,pixx);
	    //else if(sizeY==2) hpixDetMap36->Fill(pixy,pixx);
	    //else if(sizeY==3) hpixDetMap37->Fill(pixy,pixx);
	    //else if(sizeY==4) hpixDetMap38->Fill(pixy,pixx);
	    //else              hpixDetMap39->Fill(pixy,pixx);

#endif 

#ifdef SINGLE_MODULES
	    float weight=1.;
	    if     (ladder==-6 && module==-1) hpixDetMap10->Fill(pixy,pixx,weight); //  BmO1,2
	    else if(ladder==-6 && module==-2) hpixDetMap11->Fill(pixy,pixx,weight); // 
	    else if(ladder==-6 && module==-3) hpixDetMap12->Fill(pixy,pixx,weight); // 
	    else if(ladder==-6 && module==-4) hpixDetMap13->Fill(pixy,pixx,weight); //  
	    else if(ladder==-2 && module== 3) hpixDetMap14->Fill(pixy,pixx,weight); // "
	    else if(ladder==-3 && module== 3) hpixDetMap15->Fill(pixy,pixx,weight); // "
	    else if(ladder==-4 && module== 3) hpixDetMap16->Fill(pixy,pixx,weight); // "
	    else if(ladder==-5 && module== 3) hpixDetMap17->Fill(pixy,pixx,weight); // 
	    else if(ladder== 2 && module== 3) hpixDetMap18->Fill(pixy,pixx,weight); //  
	    else if(ladder== 3 && module== 3) hpixDetMap19->Fill(pixy,pixx,weight); // 
#endif

	  } else if(layer==2) {
	    
	    if(select) cout<<i<<" "<<pixx<<" "<<pixy<<" "<<adc<<endl;
	    hpixchargeDets2->Fill(float(module),float(ladderOn),adc);
	    htest4->Fill(adc,float(sizeY));
	    hpladder2id->Fill(float(ladderOn));
	    hpz2id->Fill(float(module));

	    if( badL2Modules) {
	      hpixcharge2b->Fill(adc);

	    } else if(goodL2Modules) {
	      hpixcharge2g->Fill(adc);

	    } else { // really good modules 

	      hpixcharge2->Fill(adc);

	      hpixDetMap2->Fill(pixy,pixx);
	      hpcols2->Fill(pixy);
	      hprows2->Fill(pixx);
	      
#ifdef PHI_PROFILES
	      hpixcharPhi2->Fill(phi,adc);
#endif
	    }

	      
#ifdef CLU_SHAPE_L2
	    histogramPix(adc, size, sizeX, sizeY, same, corner);
#endif
	      
#ifdef SINGLE_MODULES
	    if     (ladder== -1 && module== -1) hpixDetMap20->Fill(pixy,pixx); // dcdc 
	    else if(ladder== -1 && module== -2) hpixDetMap21->Fill(pixy,pixx); //  "
	    else if(ladder== -1 && module== -3) hpixDetMap22->Fill(pixy,pixx); //  "
	    else if(ladder== -5 && module==-3) hpixDetMap23->Fill(pixy,pixx); //  "
	    else if(ladder== -8 && module==-1) hpixDetMap24->Fill(pixy,pixx); //  "
	    else if(ladder== -8 && module==-2) hpixDetMap25->Fill(pixy,pixx); //  '
	    else if(ladder== -8 && module==-3) hpixDetMap26->Fill(pixy,pixx); // dcdc
	    else if(ladder==  6 && module== 4) hpixDetMap27->Fill(pixy,pixx); // dcdc
	    else if(ladder==  7 && module== 1) hpixDetMap28->Fill(pixy,pixx); // dcdc
	    else if(ladder==  1 && module== 1) hpixDetMap29->Fill(pixy,pixx); //  '
	    // this is bad I use layer 3,4 for layer 2 modules 
	    else if(ladder==  1 && module== 2) hpixDetMap30->Fill(pixy,pixx); // fed errors
	    else if(ladder==  1 && module== 3) hpixDetMap31->Fill(pixy,pixx); // bad caldel
	    
	    else if(ladder== -5 && module==-1) hpixDetMap32->Fill(pixy,pixx); // pix 0,0
	    else if(ladder== -5 && module==-2) hpixDetMap33->Fill(pixy,pixx); // unmaskable pixel
	    else if(ladder== -1 && module== 1) hpixDetMap34->Fill(pixy,pixx); // noisy
	    else if(ladder== -1 && module== 2) hpixDetMap35->Fill(pixy,pixx); // " 
	    else if(ladder== -1 && module== 3) hpixDetMap36->Fill(pixy,pixx); // "   
	    else if(ladder== 14 && module==-1) hpixDetMap37->Fill(pixy,pixx); // fed errors 
	    else if(ladder== 13 && module==-4) hpixDetMap38->Fill(pixy,pixx); // noisy  
	    else if(ladder== 12 && module==-1) hpixDetMap39->Fill(pixy,pixx); // fed errors, masked 1/2
	    else if(ladder== 11 && module==-4) hpixDetMap40->Fill(pixy,pixx); // fed errors, masked 1/2
	    
#endif	    

	  } else if(layer==3) {
	    
	    hpixcharge3->Fill(adc);
	    hpixDetMap3->Fill(pixy,pixx);
	    hpixchargeDets3->Fill(float(module),float(ladderOn),adc);
 	    htest4->Fill(adc,float(sizeY));
	    hpladder3id->Fill(float(ladderOn));
	    hpz3id->Fill(float(module));
	    hpcols3->Fill(pixy);
	    hprows3->Fill(pixx);

#ifdef SINGLE_MODULES
	    // if     (ladder==  9 && module== 2) hpixDetMap30->Fill(pixy,pixx); // fed errors
	    // else if(ladder==  8 && module== 3) hpixDetMap31->Fill(pixy,pixx); // bad caldel
	    // else if(ladder== -6 && module== 2) hpixDetMap32->Fill(pixy,pixx); // pix 0,0
	    // else if(ladder== 21 && module== 4) hpixDetMap33->Fill(pixy,pixx); // unmaskable pixel
	    // else if(ladder==-5  && module==-2) hpixDetMap34->Fill(pixy,pixx); // noisy
	    // else if(ladder==-10 && module== 3) hpixDetMap35->Fill(pixy,pixx); // " 
	    // else if(ladder==3   && module==-3) hpixDetMap36->Fill(pixy,pixx); // "   
	    // else if(ladder==-19 && module==-4) hpixDetMap37->Fill(pixy,pixx); // fed errors 
	    // else if(ladder== 9  && module==-2) hpixDetMap38->Fill(pixy,pixx); // noisy  
	    // else if(ladder==-22 && module==-2) hpixDetMap39->Fill(pixy,pixx); // fed errors, masked 1/2
#endif

	    
	  } else if(layer==4) {
	    
	    hpixcharge4->Fill(adc);
	    hpixDetMap4->Fill(pixy,pixx);
	    hpixchargeDets4->Fill(float(module),float(ladderOn),adc);
 	    htest4->Fill(adc,float(sizeY));
	    hpladder4id->Fill(float(ladderOn));
	    hpz4id->Fill(float(module));
	    hpcols4->Fill(pixy);
	    hprows4->Fill(pixx);

#ifdef SINGLE_MODULES
	    // if     (ladder==  2 && module==-2) hpixDetMap40->Fill(pixy,pixx); // fed errors 
	    //else if(ladder== 21 && module== 3) hpixDetMap41->Fill(pixy,pixx); // noise
	    //else if(ladder==  2 && module== 4) hpixDetMap42->Fill(pixy,pixx); //  "
	    //else if(ladder==-26 && module==-2) hpixDetMap43->Fill(pixy,pixx); //  "
	    //else if(ladder==-20 && module==-1) hpixDetMap44->Fill(pixy,pixx); //  " 
	    //else if(ladder==  3 && module==-4) hpixDetMap45->Fill(pixy,pixx); // " 
	    // else if(ladder== 31 && module==-1) hpixDetMap46->Fill(pixy,pixx); // "
	    //else if(ladder== 15 && module== 1) hpixDetMap47->Fill(pixy,pixx); // "
	    //else if(ladder==-22 && module== 2) hpixDetMap48->Fill(pixy,pixx); //  
	    //else if(ladder== 32 && module== 2) hpixDetMap49->Fill(pixy,pixx); //  
#endif
	    
	  } // if layer 
	  	  
	} // for loop 
	  
      } // if valid
      
    } // clusters
    
    if(pixelHits>0) countPixTracks++;

    if(PRINT) cout<<" Clusters for track "<<trackNumber<<" num of clusters "<<numberOfClusters
		  <<" num of pixels "<<pixelHits<<endl;

    // per track histos
    if(numberOfClusters>0) { 

      // With pixel hits
      hPtP->Fill(pt);
      hEtaP->Fill(eta);
      hPhiP->Fill(phi);
      hDzP->Fill(dz);
      hD0P->Fill(d0);

      hclusPerTrk1->Fill(float(numOfClusPerTrk1));
      if(PRINT) cout<<"Lay1: number of clusters per track = "<<numOfClusPerTrk1<<endl;
      hclusPerTrk2->Fill(float(numOfClusPerTrk2));
      if(PRINT) cout<<"Lay2: number of clusters per track = "<<numOfClusPerTrk2<<endl;
      hclusPerTrk3->Fill(float(numOfClusPerTrk3));
      if(PRINT) cout<<"Lay3: number of clusters per track = "<<numOfClusPerTrk3<<endl;
      hclusPerTrk4->Fill(float(numOfClusPerTrk4));
      if(PRINT) cout<<"Lay4: number of clusters per track = "<<numOfClusPerTrk4<<endl;
      hclusPerTrk5->Fill(float(numOfClusPerTrk5));  // fpix  disk1
      if(PRINT) cout<<"Disk1: number of clusters per track = "
		    <<numOfClusPerTrk5<<endl;
      hclusPerTrk6->Fill(float(numOfClusPerTrk6)); // fpix disk2
      if(PRINT) cout<<"Disk2: number of clusters per track = "
		    <<numOfClusPerTrk6<<endl;
      hclusPerTrk7->Fill(float(numOfClusPerTrk7)); // fpix disk3
      if(PRINT) cout<<"Disk3: number of clusters per track = "
		    <<numOfClusPerTrk7<<endl;

      if(numOfClusPerTrk1>0) {hEta1->Fill(eta);hPhi1->Fill(phi);countTracks1++;}
      else hEta10->Fill(eta);
      if(numOfClusPerTrk2>0) {hEta2->Fill(eta);hPhi2->Fill(phi);countTracks2++;}
      else hEta20->Fill(eta);
      if(numOfClusPerTrk3>0) {hEta3->Fill(eta);hPhi3->Fill(phi);countTracks3++;}
      else hEta30->Fill(eta);
      if(numOfClusPerTrk4>0) {hEta4->Fill(eta);hPhi4->Fill(phi);countTracks4++;}
      else hEta40->Fill(eta);
      if(numOfClusPerTrk5>0) {hEta5->Fill(eta); countTracks5++;}
      if(numOfClusPerTrk6>0) {hEta6->Fill(eta); countTracks5++;}
      if(numOfClusPerTrk7>0) {hEta7->Fill(eta); countTracks5++;}
      
      float clusPerTrkB = numOfClusPerTrk1 + numOfClusPerTrk2 + 
	numOfClusPerTrk3 + numOfClusPerTrk4;
      float clusPerTrkF = numOfClusPerTrk5 + numOfClusPerTrk6+ numOfClusPerTrk7;
      float clusPerTrk =  clusPerTrkB + clusPerTrkF;
      
      hclusPerTrkB->Fill(clusPerTrkB);
      hclusPerTrkF->Fill(clusPerTrkF);
      hclusPerTrk->Fill(clusPerTrk);

#ifdef USE_PROFILES
      hclusPerTrkVsEta->Fill(eta,clusPerTrk);
      hclusPerTrkVsEtaB->Fill(eta,clusPerTrkB);
      hclusPerTrkVsEtaF->Fill(eta,clusPerTrkF);
      hclusPerTrkVsPt->Fill(pt,clusPerTrk);
      hclusPerTrkVsls->Fill(lumiBlock,clusPerTrk);
#endif 
    } // numOf clustres 

  } // tracks

  // total layer histos
  if(numberOfClusters>0) {
    hclusPerLay1->Fill(float(numOfClustersPerLay1));
    hclusPerLay2->Fill(float(numOfClustersPerLay2));
    hclusPerLay3->Fill(float(numOfClustersPerLay3));
    hclusPerLay4->Fill(float(numOfClustersPerLay4));
    
    hclusPerDisk1->Fill(float(numOfClustersPerDisk3+numOfClustersPerDisk4));
    hclusPerDisk2->Fill(float(numOfClustersPerDisk2+numOfClustersPerDisk5));
    hclusPerDisk3->Fill(float(numOfClustersPerDisk1+numOfClustersPerDisk6));

    hclusBpix->Fill(float(numberOfClusters));
    hpixBpix->Fill(float(numberOfPixels));

  }
  htracksGood->Fill(float(countNiceTracks));
  htracksGoodInPix->Fill(float(countPixTracks));
  htracks->Fill(float(trackNumber));

  hbx->Fill(float(bx));
  hls->Fill(float(lumiBlock));

#ifdef USE_PROFILES
  htracksls->Fill(float(lumiBlock),float(countPixTracks));
  hpvsls->Fill(float(lumiBlock),float(pvsTrue));
#endif

#ifdef LUMI
  if(instlumi>0.) {
    float tmp = float(countPixTracks)/instlumi;
    htrackslsn->Fill(float(lumiBlock),tmp);
    tmp = float(pvsTrue)/instlumi;
    hpvslsn->Fill(float(lumiBlock),tmp);
  }
#endif

#ifdef LS_STUDIES

  hclusls->Fill(float(lumiBlock),float(numberOfClusters)); // clusters fpix+bpix
  //hpixls->Fill(float(lumiBlock),float(numberOfPixels)); // pixels fpix+bpix
  
  hclubx->Fill(float(bx),float(numberOfClusters)); // clusters fpix+bpix
  //hpixbx->Fill(float(bx),float(numberOfPixels)); // pixels fpix+bpix
  hpvbx->Fill(float(bx),float(pvsTrue)); // pvs
  htrackbx->Fill(float(bx),float(countPixTracks)); // tracks

  hclusls1->Fill(float(lumiBlock),float(numOfClustersPerLay1)); // clusters bpix1
  //hpixls1->Fill( float(lumiBlock),float(numOfPixPerLay1)); // pixels bpix1
  hclusls2->Fill(float(lumiBlock),float(numOfClustersPerLay2)); // clusters bpix2
  //hpixls2->Fill( float(lumiBlock),float(numOfPixPerLay2)); // pixels bpix2
  hclusls3->Fill(float(lumiBlock),float(numOfClustersPerLay3)); // clusters bpix3
  //hpixls3->Fill( float(lumiBlock),float(numOfPixPerLay3)); // pixels bpix3
#endif

  //
  countTracks += float(trackNumber);
  countGoodTracks += float(countNiceTracks);
  countTracksInPix += float(countPixTracks);
  countPVs += float(pvsTrue);
  countEvents++;

  if(missingHit) count1++;
  if(missingHitB) count2++;

  if(lumiBlock != lumiBlockOld) {
#ifdef Lumi
    countLumi += intlumi; 
#endif
    lumiBlockOld = lumiBlock;
  }


  if(PRINT) cout<<" event with tracks = "<<trackNumber<<" "<<countNiceTracks<<endl;
  //cout<<" event with tracks = "<<trackNumber<<" "<<countGoodTracks<<endl;

  return;
  
} // end 
    
//define this as a plug-in
DEFINE_FWK_MODULE(PixClustersWithTracks);
  


  
// #ifdef USE_TRAJ
//   // Not used 
  
//   //----------------------------------------------------------------------------
//   // Use Trajectories

//   edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
//   e.getByLabel(conf_.getParameter<std::string>("trajectoryInput"),trajTrackCollectionHandle);

//   TrajectoryStateCombiner tsoscomb;
 
//   int NbrTracks =  trajTrackCollectionHandle->size();
//   std::cout << " track measurements " << trajTrackCollectionHandle->size()  << std::endl;

//   int trackNumber = 0;
//   int numberOfClusters = 0;

//   for(TrajTrackAssociationCollection::const_iterator it = trajTrackCollectionHandle->begin(), itEnd = trajTrackCollectionHandle->end(); it!=itEnd;++it){

//     int pixelHits = 0;
//     int stripHits = 0;
//     const Track&      track = *it->val;
//     const Trajectory& traj  = *it->key;
   
//     std::vector<TrajectoryMeasurement> checkColl = traj.measurements();
//     for(std::vector<TrajectoryMeasurement>::const_iterator checkTraj = checkColl.begin();
// 	checkTraj != checkColl.end(); ++checkTraj) {

//       if(! checkTraj->updatedState().isValid()) continue;
//       TransientTrackingRecHit::ConstRecHitPointer testhit = checkTraj->recHit();
//       if(! testhit->isValid() || testhit->geographicalId().det() != DetId::Tracker ) continue;
//       uint testSubDetID = (testhit->geographicalId().subdetId());
//       if(testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) pixelHits++;
//       else if (testSubDetID == StripSubdetector::TIB || testSubDetID == StripSubdetector::TOB ||
// 	       testSubDetID == StripSubdetector::TID || testSubDetID == StripSubdetector::TEC) stripHits++;

//     }


//     if (pixelHits == 0) continue;

//     trackNumber++;
//     std::cout << " track " << trackNumber <<" has pixelhits "<<pixelHits << std::endl;
//     pixelHits = 0;

//     //std::vector<TrajectoryMeasurement> tmColl = traj.measurements();
//     for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = checkColl.begin();
// 	itTraj != checkColl.end(); ++itTraj) {
//       if(! itTraj->updatedState().isValid()) continue;

//       TrajectoryStateOnSurface tsos = tsoscomb( itTraj->forwardPredictedState(), itTraj->backwardPredictedState() );
//       TransientTrackingRecHit::ConstRecHitPointer hit = itTraj->recHit();
//       if(! hit->isValid() || hit->geographicalId().det() != DetId::Tracker ) continue; 

//       const DetId & hit_detId = hit->geographicalId();
//       uint IntSubDetID = (hit_detId.subdetId());
	
//       if(IntSubDetID == 0 ) continue;  // Select ??
//       if(IntSubDetID != PixelSubdetector::PixelBarrel) continue; // look only at bpix || IntSubDetID == PixelSubdetector::PixelEndcap) {


// //       const GeomDetUnit* detUnit = hit->detUnit();
// //       if(detUnit) {
// // 	const Surface& surface = hit->detUnit()->surface();
// // 	const TrackerGeometry& theTracker(*tkGeom_);
// // 	const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(hit_detId) );
// // 	const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
// //       }


//       // get the enclosed persistent hit
//       const TrackingRecHit *persistentHit = hit->hit();
//       // check if it's not null, and if it's a valid pixel hit
//       if ((persistentHit != 0) && (typeid(*persistentHit) == typeid(SiPixelRecHit))) {
// 	// tell the C++ compiler that the hit is a pixel hit
// 	const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>( hit->hit() );
// 	// get the edm::Ref to the cluster
// 	edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = (*pixhit).cluster();
// 	//  check if the ref is not null
// 	if (clust.isNonnull()) {

// 	  numberOfClusters++;
// 	  pixelHits++;
// 	  float charge = (clust->charge())/1000.0; // convert electrons to kilo-electrons
// 	  int size = clust->size();
// 	  int size_x = clust->sizeX();
// 	  int size_y = clust->sizeY();
// 	  float row = clust->x();
// 	  float col = clust->y();
  
// 	  //LocalPoint lp = topol->localPosition(MeasurementPoint(clust_.row,clust_.col));
// 	  //float x = lp.x();
// 	  //float y = lp.y();
  
// 	  int maxPixelCol = clust->maxPixelCol();
// 	  int maxPixelRow = clust->maxPixelRow();
// 	  int minPixelCol = clust->minPixelCol();
// 	  int minPixelRow = clust->minPixelRow();
  
// 	  //int geoId = PixGeom->geographicalId().rawId();

// 	  // Replace with the topology methods
// 	  // edge method moved to topologi class
// 	  //int edgeHitX = (int) ( topol->isItEdgePixelInX( minPixelRow ) || topol->isItEdgePixelInX( maxPixelRow ) );
// 	  //int edgeHitY = (int) ( topol->isItEdgePixelInY( minPixelCol ) || topol->isItEdgePixelInY( maxPixelCol ) );

// 	  // calculate alpha and beta from cluster position
// 	  //LocalTrajectoryParameters ltp = tsos.localParameters();
// 	  //LocalVector localDir = ltp.momentum()/ltp.momentum().mag();

// 	  //float locx = localDir.x();
// 	  //float locy = localDir.y();
// 	  //float locz = localDir.z();
// 	  //float loctheta = localDir.theta(); // currently unused

// 	  //float alpha = atan2( locz, locx );
// 	  //float beta = atan2( locz, locy );

// 	  //clust_.normalized_charge = clust_.charge*sqrt(1.0/(1.0/pow(tan(clust_.clust_alpha),2)+1.0/pow(tan(clust_.clust_beta),2)+1.0));
// 	}  // valid cluster
//       } // valid peristant hit

//     } // loop over trajectory meas.

//     if(PRINT) cout<<" Cluster for track "<<trackNumber<<" cluaters "<<numberOfClusters<<" "<<pixelHits<<endl;
    
//   } // loop over tracks
  
// #endif // USE_TRAJ
