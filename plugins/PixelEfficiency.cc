// system include files
#include <memory> 

// user include files
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h" 
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h" 
#include "FWCore/Framework/interface/EventSetup.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/UniformEngine/src/UniformMagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
//#include "MagneticField/UniformEngine/src/UniformMagneticFieldESProducer.h"
//class to take the pixel submodules
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
//class for the trajectories
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"
#include "Alignment/CommonAlignment/interface/Utilities.h"
//class to handle the pixelcluster
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

//classes to handle muons
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

//Triggers
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

//TRoot classes
#include "TH1F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <vector>
#include <map>

using namespace std;
using namespace edm;
using namespace reco;
using namespace TMath;

 const unsigned int kBPIX = PixelSubdetector::PixelBarrel;
 const unsigned int kFPIX = PixelSubdetector::PixelEndcap;

 const int numOfBadRuns=16;

//
// class decleration
//

class PixelEfficiency : public edm::EDAnalyzer {
public:
  explicit PixelEfficiency(const edm::ParameterSet&);
  ~PixelEfficiency();
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  typedef std::vector< edm::ParameterSet > Parameters;
  Parameters  BadModuleList_;

      // ----------member data ---------------------------
  std::string fOutputFileName;  
  edm::InputTag TkTag_ ; 
  std::string fOutputFileName0T;
  edm::InputTag TkTag0T_ ;

  edm::InputTag trajectoryInput_;
  edm::InputTag pixelClusterInput_;
  
  edm::ParameterSet listOfCuts_;
  
  bool DEBUG;
  
  std::string dataType_;
  
  bool keepAllRuns_;
  bool skip0TRuns_;
  bool keep0TRuns_;
  bool keep38TRuns_;
  bool keepOnlyOneTrackEvts_;
  bool skipBadModules_;
  
  double peak_MuonCut_;
  double window_MuonCut_;
  double timeErr_MuonCut_;
  double timeNdof_MuonCut_;
  double nSigma_EdgeCut_;
  double minHits_TrackCut_;
  string quality_TrackCut_;

  TH1F*  histo;
  TH1F*  histLayer1;
  TH1F*  histLayer2;
  TH1F*  histLayer3;
  TH1F*  histEndcapPlus;
  TH1F*  histEndcapMinus;
  TH1F*  histBarrel;
  TH1F*  histEndcap;
  TH1F*  validPerSubdetector;
  TH1F*  missingPerSubdetector;
  TH1F*  inactivePerSubdetector;

  TH1F*  consistencyCheck;
  TH1F*  consistencyCheckTraj;
  TH1F*  trackingEfficiency;
  
  TH1F*  histInvalidRecHitCollection;
  TH1F*  histInvalidRecHitWithBadmoduleList;
  TH1F*  windowSearch;
  TH1F*  windowSearchSameModule;
  TH1F*  windowSearchBPix;
  TH1F*  windowSearchGoodModulesBPix;
  TH1F*  windowSearchFPix;
  TH1F*  missingButClusterOnSameModule;
  TH1F*  missingButCluster;

  vector<TH1F*> windowSearch_pT; 
         
  TH1F*  chargeDistriPreCuts;
  TH1F*  numbPixInClusterPreCuts;
  TH1F*  chargeDistriBPixPreCuts;
  TH1F*  numbPixInClusterBPixPreCuts;
  TH1F*  chargeDistriFPixPlusPreCuts;
  TH1F*  numbPixInClusterFPixPlusPreCuts;  
  TH1F*  chargeDistriFPixMinusPreCuts;
  TH1F*  numbPixInClusterFPixMinusPreCuts;  

  TH1F*  chargeDistri;
  TH1F*  numbPixInCluster;
  TH1F*  chargeDistriBPix;
  TH1F*  numbPixInClusterBPix;
  TH1F*  chargeDistriFPixPlus;
  TH1F*  numbPixInClusterFPixPlus;  
  TH1F*  chargeDistriFPixMinus;
  TH1F*  numbPixInClusterFPixMinus;  
  TH2F*  chargeVsDimensionBPix;
  
  TH1F*  numbPixInClusterX;
  TH1F*  numbPixInClusterY;
  TH1F*  xposCluster;
  TH1F*  yposCluster;
  TH1F*  chargeDistriValid;
  TH1F*  chargeDistriMisRecovered;
  TH1F*  numbPixInClusterValid;
  TH1F*  numbPixInClusterMisRecovered;
  TH1F*  xposClusterValid;
  TH1F*  yposClusterValid;
  TH1F*  xposClusterMisRecovered;
  TH1F*  yposClusterMisRecovered;
  
  TH1F*  validPerRun;
  TH1F*  invalidPerRun;
  TH1F*  inactivePerRun;
  
  TH1F*  validVsAlpha;
  TH1F*  missingVsAlpha;
  TH1F*  validVsCotanAlpha;
  TH1F*  missingVsCotanAlpha;
  TH1F*  validVsBeta;
  TH1F*  missingVsBeta;
  TH2F*  validAlphaBeta;
  TH2F*  missingAlphaBeta;

  TH1F*  validVsAlphaBPix;
  TH1F*  missingVsAlphaBPix;
  TH1F*  validVsBetaBPix;
  TH1F*  missingVsBetaBPix;
  
  TH1F*  validVsAlphaFPix;
  TH1F*  missingVsAlphaFPix;
  TH1F*  validVsBetaFPix;
  TH1F*  missingVsBetaFPix;
  
  TH1F*  validVsLocalX;
  TH1F*  missingVsLocalX;
  TH1F*  validVsLocalY;
  TH1F*  missingVsLocalY;

  TH2F*  validAlphaLocalXBig;
  TH2F*  missingAlphaLocalXBig;
  TH2F*  validAlphaLocalXSmall;
  TH2F*  missingAlphaLocalXSmall;
  
  TH2F*  missingAlphaLocalXBigBPix;
  TH2F*  missingBetaLocalXBigBPix;
  TH2F*  missingAlphaLocalYBPix;
  TH2F*  missingBetaLocalYBPix;
  TH2F*  validAlphaLocalXBigBPix;
  TH2F*  validBetaLocalXBigBPix;
  TH2F*  validAlphaLocalYBPix;
  TH2F*  validBetaLocalYBPix;
  
  TH2F*  missingAlphaLocalXBigFPix;
  TH2F*  missingBetaLocalXBigFPix;
  TH2F*  missingAlphaLocalYFPix;
  TH2F*  missingBetaLocalYFPix;
  TH2F*  validAlphaLocalXBigFPix;
  TH2F*  validBetaLocalXBigFPix;
  TH2F*  validAlphaLocalYFPix;
  TH2F*  validBetaLocalYFPix;
  
  TH1F*  validVsLocalXBig;
  TH1F*  missingVsLocalXBig;
  TH1F*  validVsLocalXSmall;
  TH1F*  missingVsLocalXSmall;

  TH1F*  validVsMuontimePre68094;
  TH1F*  missingVsMuontimePre68094;
  TH1F*  validVsMuontimePost68094;
  TH1F*  missingVsMuontimePost68094;
  
  TH2F* validMuonTimeVSchargeBPix;
  TH2F* missingMuonTimeVSchargeBPix;
  TH2F* validMuonTimeVSchargeFPix;
  TH2F* missingMuonTimeVSchargeFPix;

  TH1F*  validVsPT;
  TH1F*  missingVsPT;
  TH1F*  validVsPT_lowpt;
  TH1F*  missingVsPT_lowpt;
  
  TH1F*  validVsEta;
  TH1F*  validVsPhi;
  TH1F*  missingVsEta;
  TH1F*  missingVsPhi;
  TH1F*  inactiveVsEta;
  TH1F*  inactiveVsPhi;
  
  TH1F*  validPerMinHitsOnTrackBPix;
  TH1F*  validPerMinHitsOnTrackFPix;
  TH1F*  missingPerMinHitsOnTrackBPix;
  TH1F*  missingPerMinHitsOnTrackFPix;
  //
  TH1F*  checkoutValidityFlag;
  TH1F*  checkoutTraj;
  //
  TH1F*  totalPerTrack;
  TH1F*  inactivePerTrack;
  TH1F*  missingPerTrack;
  TFile* fOutputFile;
  
  
  //// HIT QUALITY ANALYSIS
  //P->Passing Cut
  //NP->Not Passing Cut
  TH1F* hitsPassingCutsValBPix;
  TH1F* hitsPassingCutsMisBPix;
  TH1F* hitsPassingCutsValFPix;
  TH1F* hitsPassingCutsMisFPix;
  TH2F* xPosFracVal;
  TH2F* xPosFracMis;
  TH2F* yPosFracVal;
  TH2F* yPosFracMis;
  
  //TUNING
  TH2F* tunningVal;
  TH2F* tunningMis;
  TH1F* tunningEdgeVal;
  TH1F* tunningEdgeMis;
  TH1F* tunningMuonVal;
  TH1F* tunningMuonMis;

  //ChiSqaure
  TH1F* validChiSquare;
  TH1F* missingChiSquare;
  TH1F* validChiSquareNdf;
  TH1F* missingChiSquareNdf;

  TH2F* missPerTrackVsChiSquareNdf;
  TH2F* missPerTrackPercentVsChiSquareNdf;

  //test
  TH1F* histoMethod2After;
  TH1F* histoMethod2;
  TH1F* histoMethod2AfterFPix;
  TH1F* histoMethod2FPix;
  
  //track analysis, muon time analysis
  TH1F* numTracksVsMuonTime;
  TH1F* denTracksVsMuonTime;
  
  TH1F* numTracksVsRunNumber;
  TH1F* denTracksVsRunNumber;
  TH1F* muonTimeVSRunNumber;
  TH1F* nMuonTimeVSRunNumber;
  TH1F* muonTimeErrorVSRunNumber;
  TH1F* nMuonTimeErrorVSRunNumber;
  TH1F* muonTimeErrorDistri;
  TH1F* validVSMuonTimeError;
  TH1F* missingVSMuonTimeError;
  TH1F* muonTimeNdofDistri;
  TH1F* validVSMuonTimeNdof;
  TH1F* missingVSMuonTimeNdof;

  TH2F* muonTimeVsRunNumber2D;

  TH2F* xy_standardDev;
  TH2F* xy_standardDev_insideModule;
  
  //Technical Trigger
  TH2F* validPerTriggerBPix;
  TH2F* missingPerTriggerBPix;
  TH2F* inactivePerTriggerBPix;
  TH2F* validPerTriggerFPix;
  TH2F* missingPerTriggerFPix;
  TH2F* inactivePerTriggerFPix;
  
  TH2F* validMapLayer1;
  TH2F* validMapLayer2;
  TH2F* validMapLayer3;
  TH2F* missingMapLayer1;
  TH2F* missingMapLayer2;
  TH2F* missingMapLayer3;
  TH2F* inactiveMapLayer1;
  TH2F* inactiveMapLayer2;
  TH2F* inactiveMapLayer3;
  
  TH1F* validPerPixHitOnTrack;
  TH1F* missingPerPixHitOnTrack;
  TH1F* inactivePerPixHitOnTrack;
 
  //"maps" for module analysis: <rawModuleID, counterOn[inactive,missing,valid]>
  vector< vector<int> > badModuleMap;  
  vector< vector<double> > goodModuleMap; //but interesting only in-active!!
  //TTree for module analysis
  TTree* tree;
  int idTree, isModuleBadTree, inactiveTree, missingTree, validTree, barrelTree, ladderTree, bladeTree, moduleInLadderTree;
  double globalXTree, globalYTree, globalZTree;

  bool badRun;  
  int ptranges;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//The current directory (root) is not associated with a file. The object (Pt Ctf) has not been written.


//
// constructors and destructor
//
PixelEfficiency::PixelEfficiency(const edm::ParameterSet& iConfig) :
  fOutputFileName( iConfig.getUntrackedParameter<std::string>("HistOutFile",std::string("pixelEfficiency.root")) ), 
  TkTag_( iConfig.getParameter<edm::InputTag>("TkTag") ),
  fOutputFileName0T( iConfig.getUntrackedParameter<std::string>("HistOutFile0T",std::string("pixelEfficiency0T.root")) ), 
  TkTag0T_( iConfig.getParameter<edm::InputTag>("TkTag0T") ),
  trajectoryInput_( iConfig.getParameter<edm::InputTag>("trajectoryInput") ),
  pixelClusterInput_( iConfig.getParameter<edm::InputTag>("pixelClusterInput") ),
  DEBUG( iConfig.getUntrackedParameter<bool>("debug",false)),
  dataType_( iConfig.getUntrackedParameter<string>("dataType",std::string("collision")) ),
  keepAllRuns_( iConfig.getUntrackedParameter<bool>("keepAllRuns",true)),
  skip0TRuns_( iConfig.getUntrackedParameter<bool>("skip0TRuns")),
  keep0TRuns_( iConfig.getUntrackedParameter<bool>("keep0TRuns")),
  keep38TRuns_( iConfig.getUntrackedParameter<bool>("keep38TRuns")),
  keepOnlyOneTrackEvts_( iConfig.getUntrackedParameter<bool>("keepOnlyOneTrackEvts")),
  skipBadModules_( iConfig.getUntrackedParameter<bool>("skipBadModules")),
  peak_MuonCut_( iConfig.getUntrackedParameter<double>("peak_MuonCut")),
  window_MuonCut_( iConfig.getUntrackedParameter<double>("window_MuonCut")),
  timeErr_MuonCut_( iConfig.getUntrackedParameter<double>("timeErr_MuonCut")),
  timeNdof_MuonCut_( iConfig.getUntrackedParameter<double>("timeNdof_MuonCut")),
  nSigma_EdgeCut_( iConfig.getUntrackedParameter<double>("nSigma_EdgeCut")),
  minHits_TrackCut_( iConfig.getUntrackedParameter<double>("minHits_TrackCut")),
  quality_TrackCut_( iConfig.getUntrackedParameter<string>("quality_TrackCut"))
{   
 //now do what ever initialization is needed
 if(DEBUG) std::cout<<"Starting constructor"<<std::endl;
 listOfCuts_ = iConfig.getUntrackedParameter<edm::ParameterSet>("ListOfCuts");
 BadModuleList_ = iConfig.getUntrackedParameter<Parameters>("BadModuleList");

}


PixelEfficiency::~PixelEfficiency()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
PixelEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if(DEBUG) std::cout<<"Starting Analyze"<<std::endl;

  using namespace edm;
  using namespace std;
  
  unsigned int runNumber=iEvent.id().run();
    
  //skip 0T runs for reprocessing errors
  //CRAFT08
  if ( !keepAllRuns_ && skip0TRuns_ && (
  runNumber==69557 || runNumber==69559|| runNumber==69564|| runNumber==69572|| runNumber==69573|| runNumber==69587|| runNumber==69594||
  runNumber==69728|| runNumber==69743|| runNumber==70147|| runNumber==70170|| runNumber==70195|| runNumber==70344 || runNumber==70347 ||
  runNumber==70410|| runNumber==70411||
  runNumber==70412|| runNumber==70413|| runNumber==70414|| runNumber==70415|| runNumber==70416|| runNumber==70417|| runNumber==70421||
  runNumber==70454|| runNumber==70664|| runNumber==70674|| runNumber==70675 ||
       (runNumber>= 66951 && runNumber<=67085) ||
       (runNumber>=67264 && runNumber<=67432) ||
       (runNumber>=67676 && runNumber<=67777) ||
       (runNumber>=69536 && runNumber<=69671) ||
       (runNumber>=70196 && runNumber<=99999)) ) return;
  
  //CRAFT 09 @ 0T     
  if ( !keepAllRuns_ && keep0TRuns_ && (
  runNumber!=108483 && runNumber!=108498 && runNumber!=108521
  && runNumber!=108526 && runNumber!=108562 && runNumber!=108597
  && runNumber!=108624 && runNumber!=108645 && runNumber!=108662
  && runNumber!=108666 && runNumber!=108669 && runNumber!=108671
  && runNumber!=108681 && runNumber!=108686 && runNumber!=108706
  && runNumber!=108852 && runNumber!=108856 && runNumber!=108859
  && runNumber!=108866 && runNumber!=109654 && runNumber!=109673
  && runNumber!=109703 && runNumber!=109705 && runNumber!=109706
  && runNumber!=109712 && runNumber!=109718 && runNumber!=109721
  && runNumber!=109747 && runNumber!=109784 && runNumber!=109807
  && runNumber!=109815 && runNumber!=109817 && runNumber!=109822
  && runNumber!=109826 && runNumber!=109829 && runNumber!=109839
  && runNumber!=109844 && runNumber!=109861 && runNumber!=109887
  && runNumber!=109897 && runNumber!=109902 && runNumber!=109910
  && runNumber!=109911 && runNumber!=110217 && runNumber!=110219
  && runNumber!=110232 && runNumber!=110286 && runNumber!=110290
  && runNumber!=110308 && runNumber!=110315 && runNumber!=110323
  && runNumber!=110341 ) ) return;
   
  //CRAFT 09 @ 3.8T     
  if ( !keepAllRuns_ && keep38TRuns_ && (
  runNumber!=109032 && runNumber!=109035 && runNumber!=109037 && runNumber!=109039
  && runNumber!=109043 && runNumber!=109046 && runNumber!=109049 && runNumber!=109134
  && runNumber!=109142 && runNumber!=109143 && runNumber!=109144 && runNumber!=109146
  && runNumber!=109457 && runNumber!=109459 && runNumber!=109468 && runNumber!=109470
  && runNumber!=109472 && runNumber!=109474 && runNumber!=109490 && runNumber!=109504
  && runNumber!=109508 && runNumber!=109519 && runNumber!=109524 && runNumber!=109562
  && runNumber!=109573 && runNumber!=109575 && runNumber!=109578 && runNumber!=109584
  && runNumber!=109586 && runNumber!=109603 && runNumber!=109606 && runNumber!=109624
  && runNumber!=110388 && runNumber!=110394 && runNumber!=110395 && runNumber!=110397
  && runNumber!=110407 && runNumber!=110409 && runNumber!=110419 && runNumber!=110428
  && runNumber!=110431 && runNumber!=110437 && runNumber!=110440 && runNumber!=110452
  && runNumber!=110476 && runNumber!=110485 && runNumber!=110496 && runNumber!=110508
  && runNumber!=110520 && runNumber!=110535 && runNumber!=110546 && runNumber!=110683
  && runNumber!=110686 && runNumber!=110689 && runNumber!=110690 && runNumber!=110692
  && runNumber!=110697 && runNumber!=110699 && runNumber!=110703 && runNumber!=110708
  && runNumber!=110722 && runNumber!=110751 && runNumber!=110756 && runNumber!=110784
  && runNumber!=110832 && runNumber!=110835 && runNumber!=110842 && runNumber!=110846
  && runNumber!=110878 && runNumber!=110894 && runNumber!=110900 && runNumber!=110916
  && runNumber!=110921 && runNumber!=110924 && runNumber!=110958 && runNumber!=110972
  && runNumber!=110987 && runNumber!=110998 && runNumber!=111009 && runNumber!=111017
  && runNumber!=111023 && runNumber!=111045 && runNumber!=111047 && runNumber!=111130
  && runNumber!=111138
  
  ) ) return;  
       
       
       
       
  badRun=false;

  if(DEBUG) std::cout<<"pre iSetup"<<std::endl;
 
  // Get event setup (to get global transformation)
  edm::ESHandle<TrackerGeometry> geom;
  iSetup.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker( *geom );

   //handle of the tracks 
   consistencyCheck->Fill(0); //fill for each event
   
   // Loop over trajectories
   edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
   
  //**************************************************************
  //residual (loop on traj)
  
  // get the ccmuons
  edm::Handle<MuonCollection> MuonHandle;

  if(DEBUG) std::cout<<"Trying to get all the collections"<<std::endl;


  try{iEvent.getByLabel(trajectoryInput_,trajTrackCollectionHandle);}
  catch(...){std::cout<<"trajTrackCollectionHandle was not retrieved successfully ..."<<endl;return;}
 

  edm::Handle< edmNew::DetSetVector<SiPixelCluster> > theClusters;
  try{iEvent.getByLabel(pixelClusterInput_, theClusters);}
  catch(...){std::cout<<"Clusters were not retrieved successfully ..."<<endl;return;}
  
  edm::Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  try{iEvent.getByLabel("gtDigis",L1GTRR);}
  catch(...){cout<<"The L1 Trigger branch was not correctly taken"<<endl;}
  
  /*edm::ESHandle<L1GtTriggerMenu> hL1GtMenu;
  try{iSetup.get<L1GtTriggerMenuRcd>().get(hL1GtMenu);}
  catch(...){cout<<"The L1 Trigger menu was not correctly taken"<<endl;}
  const L1GtTriggerMenu* l1GtMenu = hL1GtMenu.product();*/
  
  const edmNew::DetSetVector<SiPixelCluster>& input = *theClusters;

  int NbrTracks =  trajTrackCollectionHandle->size();
  if (NbrTracks!=1 && keepOnlyOneTrackEvts_) return;  

  checkoutTraj->Fill(NbrTracks);
  consistencyCheck->Fill(2,NbrTracks); //2 = each track //*************
  
  //iEvent.getByLabel("muons", MuonHandle);
  //iEvent.getByLabel("muons1Leg", MuonHandle);//NEW
  if(dataType_=="cosmics"){
    try{iEvent.getByLabel("muons1Leg", MuonHandle);}
    catch(...){std::cout<<"Clusters were not retrieved successfully ..."<<endl;return;}
  }
  
  
  //******** MUON CUT ************
  std::map<double,bool> muonTimingMap;
  bool hasGoodTiming=false;
  int nmuon = 0;
  double time = -999;
  double timerr = 999;
  double timendof = -999;
  if(dataType_=="cosmics"){
    for(MuonCollection::const_iterator itmuon = MuonHandle->begin(), itmuonEnd = MuonHandle->end(); itmuon!=itmuonEnd;++itmuon){   
      if(nmuon>1) break;
      if(itmuon->isTimeValid() == false) continue;
   
      if(itmuon->time().timeAtIpInOutErr<timerr && itmuon->time().nDof!=0){//NEW
        time = itmuon->time().timeAtIpInOut;
        timerr = itmuon->time().timeAtIpInOutErr;
        timendof = itmuon->time().nDof;
      }
      nmuon++;
    }
  }
    
  double peak=-8;    //For CRAFT08
  //double peak=10;      //For MC
  peak = peak_MuonCut_; //from config file
  for(double t=0;t<20;t+=0.5){
    hasGoodTiming=false;
    if(runNumber<68094 && time<peak+t && time>peak-t && timerr<timeErr_MuonCut_ && timendof>timeNdof_MuonCut_)
      hasGoodTiming=true;
    if(runNumber>=68094 && time<peak+9+t && time>peak+9-t && timerr<timeErr_MuonCut_ && timendof>timeNdof_MuonCut_)
      hasGoodTiming=true;
    muonTimingMap[2.*t]=hasGoodTiming;
  }


  hasGoodTiming = false;
  double muonwindow = 5; //For CRAFT08
  muonwindow = window_MuonCut_; //from config file
  if(runNumber<68094 && time<peak+muonwindow && time>peak-muonwindow && timerr<timeErr_MuonCut_ && timendof>timeNdof_MuonCut_)
     hasGoodTiming=true;
  if(runNumber>=68094 && time<peak+9+muonwindow && time>peak+9-muonwindow && timerr<timeErr_MuonCut_ && timendof>timeNdof_MuonCut_)
    hasGoodTiming=true;


  //********  STARTING TO LOOP OVER TRACKS
  TrajectoryStateCombiner tsoscomb;
  for(TrajTrackAssociationCollection::const_iterator it = trajTrackCollectionHandle->begin(),
      itEnd = trajTrackCollectionHandle->end(); it!=itEnd;++it){
     
    consistencyCheck->Fill(1); //1 = each track //*************

    const Trajectory& traj  = *it->key;
    const Track& track  = *it->val;
    float chiSquare = traj.chiSquared();
    float chiSquareNdf;
    chiSquareNdf = traj.chiSquared()/traj.ndof();
    float eta = track.eta();
    float phi = track.phi();

    std::vector<TrajectoryMeasurement> tmColl = traj.measurements();
    
    float missingInTrack=0.;
    float validInTrack=0.;
       
    

//*************

    //runNumber analysis
       char testLabel[10] ;
       sprintf (testLabel,"%d",runNumber) ;
       bool found=false;
       int setBin=190;
       for (int binSearch=1; binSearch<=200; binSearch++){
	 if ( !strcmp(validPerRun->GetXaxis()->GetBinLabel(binSearch),"") ) {setBin=binSearch; break;}
	 else {
	   if ( !strcmp(testLabel,validPerRun->GetXaxis()->GetBinLabel(binSearch)) ) {found=true; break;}
	 } 
       }
	 
       if (!found){
	 validPerRun->GetXaxis()->SetBinLabel(setBin,testLabel);
	 invalidPerRun->GetXaxis()->SetBinLabel(setBin,testLabel);
	 inactivePerRun->GetXaxis()->SetBinLabel(setBin,testLabel);
	 numTracksVsRunNumber->GetXaxis()->SetBinLabel(setBin,testLabel);
	 denTracksVsRunNumber->GetXaxis()->SetBinLabel(setBin,testLabel);
	 muonTimeVSRunNumber->GetXaxis()->SetBinLabel(setBin,testLabel);
	 nMuonTimeVSRunNumber->GetXaxis()->SetBinLabel(setBin,testLabel);
	 muonTimeErrorVSRunNumber->GetXaxis()->SetBinLabel(setBin,testLabel);
	 nMuonTimeErrorVSRunNumber->GetXaxis()->SetBinLabel(setBin,testLabel);

       }
       
    
    bool isInTracker = false;
    bool isInPixel   = false;
    bool isInBarrel  = false;
    bool isInEndCap  = false;
    
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
      itTraj != itTrajEnd; ++itTraj) {
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
      uint testSubDetID = (testhit->geographicalId().subdetId());
      if(testhit->geographicalId().det() == DetId::Tracker && (*testhit).isValid()) isInTracker=true;
      if((testSubDetID == kBPIX || testSubDetID == kFPIX) && (*testhit).isValid())  isInPixel  =true;
      if(testSubDetID == kBPIX && (*testhit).isValid())                             isInBarrel =true;
      if(testSubDetID == kFPIX && (*testhit).isValid())                             isInEndCap =true;
    }
    
    consistencyCheckTraj->Fill(0);//number of total trajectories
    if(isInTracker) consistencyCheckTraj->Fill(1);
    if(isInPixel)   consistencyCheckTraj->Fill(2);
    if(isInBarrel)  consistencyCheckTraj->Fill(3);
    if(isInEndCap)  consistencyCheckTraj->Fill(4);  
        
    //******** TRACK CUT **********
    bool isTrackGood = true;
    if(track.recHitsSize()<minHits_TrackCut_)
      isTrackGood = false;
    //!!!!!!!!!!!!!   NOT IMPLEMENTED YET
    //if(quality_TrackCut!="none")
    //  isTrackGood = false;
    
    if( (!isTrackGood) && listOfCuts_.getParameter<bool>("track_cut"))
      continue;
    
    //********  IN PIXEL VOLUME *********  
    if(track.d0()<9 && fabs(track.dz())<30){
    
      //FREYA PLOT
      if(track.pt()>10 && timerr<5 && TMath::Prob(track.chi2(),(int)track.ndof())<0.5){
        denTracksVsMuonTime->Fill(time);
	denTracksVsRunNumber->Fill(testLabel,1);
        if(isInPixel){
	  numTracksVsMuonTime->Fill(time);
	  numTracksVsRunNumber->Fill(testLabel,1);
	}
      }
      
      if(track.chi2()<15){
        trackingEfficiency->Fill(0);
        if(isInPixel) trackingEfficiency->Fill(1);
      }
      
    }
    
    if(isInPixel){
      muonTimeVSRunNumber->Fill(testLabel,time);
      nMuonTimeVSRunNumber->Fill(testLabel,1);
      muonTimeErrorVSRunNumber->Fill(testLabel,timerr);
      nMuonTimeErrorVSRunNumber->Fill(testLabel,1);
      muonTimeErrorDistri->Fill(timerr);
    }
    
    
    float numPixHit=0;
    float numInactiveHit=0;
    float numMissingHit=0;
    float numValidHit=0;
    int numofhit=0;
    
    //***********  LOOP OVER HITS    
    for(std::vector<TrajectoryMeasurement>::const_iterator itTraj = tmColl.begin(), itTrajEnd = tmColl.end();
	itTraj != itTrajEnd; ++itTraj) {
      numofhit++;
	
      //are we in the pixels with valid stuffs?
      TransientTrackingRecHit::ConstRecHitPointer testhit = itTraj->recHit();
      
      //check if in PixelDetector
      uint testSubDetID = (testhit->geographicalId().subdetId());
      int type = testSubDetID;
      if(! (testSubDetID == PixelSubdetector::PixelBarrel || testSubDetID == PixelSubdetector::PixelEndcap) ) continue;
      
      numPixHit++;
      
      
      //check if belonging to badmodule list
      bool badModule=false;
      for(Parameters::iterator it = BadModuleList_.begin(); it != BadModuleList_.end(); ++it) {         
        if ( (testhit->geographicalId().rawId())==(it->getParameter<uint32_t>("detid")) ) {badModule=true; break;}
      }
      if (badModule && skipBadModules_) continue;
            	
		
	 //******take the geometrical parameters of the hit**********

  if(DEBUG) std::cout<<"take the geometrical parameters of the hit"<<std::endl;
	 	
	 int moduleRawId = (testhit)->geographicalId().rawId();
	 DetId detId = ((testhit)->geographicalId());
        
         unsigned int layer=0;
         unsigned int ladder=0;
         unsigned int disk=0;
         unsigned int blade=0;
	 unsigned int moduleInLadder=0;
	 if (type==int(kBPIX))
	   {PXBDetId pdetId = PXBDetId(detId); layer=pdetId.layer(); ladder=pdetId.ladder(); moduleInLadder=pdetId.module();}
	 if (type==int(kFPIX))
	   {PXFDetId pfdetId = PXFDetId(detId); blade=pfdetId.blade(); disk=pfdetId.disk();}
	 double globalX = 0;
	 double globalY = 0;
	 double globalZ = 0;
         const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
         globalX = theGeomDet->surface().position().x();
         globalY = theGeomDet->surface().position().y();
         globalZ = theGeomDet->surface().position().z();
	 
	 TrajectoryStateOnSurface tsos = tsoscomb( itTraj->forwardPredictedState(), itTraj->backwardPredictedState() );
	 double gX = tsos.globalPosition().x();
	 double gY = tsos.globalPosition().y();
	 double gZ = tsos.globalPosition().z();
	 
         //2D maps of the hits
	 if(layer==1){
	   if((*testhit).getType()==TrackingRecHit::valid) validMapLayer1->Fill(gZ,atan2(gY,gX));
	   if((*testhit).getType()==TrackingRecHit::missing) missingMapLayer1->Fill(gZ,atan2(gY,gX));
	   if((*testhit).getType()==TrackingRecHit::inactive) inactiveMapLayer1->Fill(gZ,atan2(gY,gX));
	 }
	 if(layer==2){
	   if((*testhit).getType()==TrackingRecHit::valid) validMapLayer2->Fill(gZ,atan2(gY,gX));
	   if((*testhit).getType()==TrackingRecHit::missing) missingMapLayer2->Fill(gZ,atan2(gY,gX));
	   if((*testhit).getType()==TrackingRecHit::inactive) inactiveMapLayer2->Fill(gZ,atan2(gY,gX));
	 }
	 if(layer==3){
	   if((*testhit).getType()==TrackingRecHit::valid) validMapLayer3->Fill(gZ,atan2(gY,gX));
	   if((*testhit).getType()==TrackingRecHit::missing) missingMapLayer3->Fill(gZ,atan2(gY,gX));
	   if((*testhit).getType()==TrackingRecHit::inactive) inactiveMapLayer3->Fill(gZ,atan2(gY,gX));
	 }
	 
	 //****** compute the  pT cut **********

    	 bool hasHighPT = false;
	 double PT = tsos.globalMomentum().perp();
    	 if ( PT>5.) hasHighPT = true;

	 //****** compute the edge-cut **********

         bool isBigModule=false;
	 int nrows = theGeomDet->specificTopology().nrows();
	 if (nrows>80) isBigModule=true;
	 else          isBigModule=false;
	 int ncols = theGeomDet->specificTopology().ncolumns();
	 //
	 //std::pair<float,float> pitchTest = theGeomDet->specificTopology().pitch();
	 //RectangularPixelTopology rectTopolTest = RectangularPixelTopology(nrows, ncols, pitch.first, pitch.second);
	 //std::pair<float,float> pixelTest = rectTopol.pixel(tsos.localPosition());
	 //
	 std::pair<float,float> pixel = theGeomDet->specificTopology().pixel(tsos.localPosition());

	 //****Edge cut
	 bool isNotInMiddle = false; 
	 double nsigma = nSigma_EdgeCut_;
	 LocalPoint  exagerated;
	 LocalPoint  actual;  actual = LocalPoint( tsos.localPosition().x(),tsos.localPosition().y() );
	 std::pair<float,float> pixelActual = theGeomDet->specificTopology().pixel(actual);

	 LocalError tsosErr = tsos.localError().positionError();	 
	 if ( tsos.localPosition().x()>0 && tsos.localPosition().y()>0 )
	   exagerated = LocalPoint(tsos.localPosition().x()+nsigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()+nsigma*std::sqrt(tsosErr.yy()) );
	 if ( tsos.localPosition().x()<0 && tsos.localPosition().y()>0)
	   exagerated = LocalPoint(tsos.localPosition().x()-nsigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()+nsigma*std::sqrt(tsosErr.yy())  );
	 if ( tsos.localPosition().x()<0 && tsos.localPosition().y()<0)
	   exagerated = LocalPoint(tsos.localPosition().x()-nsigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()-nsigma*std::sqrt(tsosErr.yy()) );
	 if ( tsos.localPosition().x()>0 && tsos.localPosition().y()<0)
	   exagerated = LocalPoint(tsos.localPosition().x()+nsigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()-nsigma*std::sqrt(tsosErr.yy()) );
	 	 
	 std::pair<float,float> pixelExagerated = theGeomDet->specificTopology().pixel(exagerated);
	 
	     //** general geometric information: **
	     xy_standardDev->Fill( std::sqrt(tsosErr.xx()),std::sqrt(tsosErr.xx()) );
	     if (pixelActual.first<nrows  && pixelActual.first>0 &&
	         pixelActual.second<ncols && pixelActual.second>0)
		   xy_standardDev->Fill( std::sqrt(tsosErr.xx()),std::sqrt(tsosErr.xx()) );
	     //**  **
	 
	 if( pixelExagerated.first>nrows  || pixelExagerated.first<0)
	   isNotInMiddle = true;
	 if( pixelExagerated.second>ncols || pixelExagerated.second<0)
	   isNotInMiddle = true;
	 
	 
	 //map for the TH2
	 std::map <double,bool> edgeCutMap;

	 for (int divideMe=0;divideMe<50;divideMe++){
	   double sigma=divideMe/10.;
	   
          LocalPoint  exageratedMap;
	   if ( tsos.localPosition().x()>0 && tsos.localPosition().y()>0 )
	     exageratedMap =LocalPoint(tsos.localPosition().x()+sigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()+sigma*std::sqrt(tsosErr.yy()) );
	   if ( tsos.localPosition().x()<0 && tsos.localPosition().y()>0)
	     exageratedMap =LocalPoint(tsos.localPosition().x()-sigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()+sigma*std::sqrt(tsosErr.yy())  );
	   if ( tsos.localPosition().x()<0 && tsos.localPosition().y()<0)
	     exageratedMap =LocalPoint(tsos.localPosition().x()-sigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()-sigma*std::sqrt(tsosErr.yy()) );
	   if ( tsos.localPosition().x()>0 && tsos.localPosition().y()<0)
	     exageratedMap = LocalPoint(tsos.localPosition().x()+sigma*std::sqrt(tsosErr.xx()),tsos.localPosition().y()-sigma*std::sqrt(tsosErr.yy()) );
	 
	   std::pair<float,float> pixelExageratedMap = theGeomDet->specificTopology().pixel(exageratedMap);
	 
	   if( pixelExageratedMap.first>nrows  || pixelExageratedMap.first<0)
	     edgeCutMap[sigma] = true;
	   else if( pixelExageratedMap.second>ncols || pixelExageratedMap.second<0)
	      edgeCutMap[sigma] = true;
	   else edgeCutMap[sigma] = false;	   
	  
	 }
	 
	 //PERCENTAGE CUT
	 //bool isNotInMiddle = false;
	 //double edgeRejectionPercentage = 0.95;
	 double xposfrac_ = TMath::Abs(pixel.first-double(nrows)/2.)  /(double(nrows)/2.);
	 double yposfrac_ = TMath::Abs(pixel.second-double(ncols)/2.) /(double(ncols)/2.);
	 //if(xposfrac_ > edgeRejectionPercentage || pixel.first>nrows  || pixel.first<0)
	 //  isNotInMiddle = true;
	 //if(yposfrac_ > edgeRejectionPercentage || pixel.second>ncols || pixel.second<0)
	 //  isNotInMiddle = true;
	
 
	//********************* HERE IS THE TELESCOPE CUT ON HITS
  if(DEBUG) std::cout<<"HERE IS THE TELESCOPE CUT ON HIT"<<std::endl;

	int numofhitCut=0;
        int numOfOtherValid=0;  
	bool hasValidInUpperPix = false;
	bool hasValidInLowerPix = false;
	for(std::vector<TrajectoryMeasurement>::const_iterator itCut1 = tmColl.begin(), itCut1End = tmColl.end();
	itCut1 != itCut1End; ++itCut1) {
	  numofhitCut++;
	
	  //FPixTest;
	  
	  TransientTrackingRecHit::ConstRecHitPointer testhitCut = itCut1->recHit();
          uint testSubDetIDCut = (testhitCut->geographicalId().subdetId());
          int typeCut = testSubDetIDCut;
          if(! (testSubDetIDCut == PixelSubdetector::PixelBarrel || testSubDetIDCut == PixelSubdetector::PixelEndcap) ) continue;
         
	  //check if belonging to badmodule list
          bool badModuleCut=false;
          for(Parameters::iterator itCut = BadModuleList_.begin(); itCut != BadModuleList_.end(); ++itCut) {         
            if ( (testhitCut->geographicalId().rawId())==(itCut->getParameter<uint32_t>("detid")) ) {badModuleCut=true; break;}
	  }
          if (badModuleCut && skipBadModules_) continue;
           	
	   //int moduleRawIdCut = (testhitCut)->geographicalId().rawId();

	   DetId detIdCut = ((testhitCut)->geographicalId());
        
           unsigned int layerCut=99999;
           unsigned int diskCut=99999;
	   if (typeCut==int(kBPIX))
	     {PXBDetId pdetIdCut = PXBDetId(detIdCut); layerCut=pdetIdCut.layer();}
	   if (typeCut==int(kFPIX)){PXFDetId pfdetIdCut = PXFDetId(detIdCut); diskCut=pfdetIdCut.disk();}
	   double globalYCut = 0;
           const PixelGeomDetUnit* theGeomDetCut = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detIdCut) );
           globalYCut = theGeomDetCut->surface().position().y();
	   
	   if(numofhitCut!=numofhit && globalYCut>0 && layerCut!=layer && disk!=diskCut && (*testhitCut).getType()==TrackingRecHit::valid)
	     hasValidInUpperPix = true;
	   if(numofhitCut!=numofhit && globalYCut<0 && layerCut!=layer && disk!=diskCut && (*testhitCut).getType()==TrackingRecHit::valid)
	     hasValidInLowerPix = true;
	     
		
	   if(numofhitCut!=numofhit && layerCut!=layer && disk!=diskCut && (*testhitCut).getType()==TrackingRecHit::valid)
	     numOfOtherValid++;  

	}//end of loop for trajmeas for cut
	
	bool isTelescopeGood = hasValidInUpperPix && hasValidInLowerPix;
	
      //**********  TAKING CLUSTER FROM recHIT
      edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> testclust;
      if((*testhit).isValid()){
        const TrackingRecHit *persistenthit = testhit->hit();
	if((persistenthit!=0) && (typeid(*persistenthit)==typeid(SiPixelRecHit))){
	  const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(testhit->hit());
	  testclust = (*pixhit).cluster();
	}
      }
      //**********  fill CLUSTER properties for valid recHit without CUTS requirements    
      if(testclust.isNonnull()){
        chargeDistriPreCuts->Fill(testclust->charge());
	numbPixInClusterPreCuts->Fill(testclust->size());
	
        if(type==int(kBPIX)) {chargeDistriBPixPreCuts->Fill(testclust->charge());numbPixInClusterBPixPreCuts->Fill(testclust->size());}
	if(type==int(kFPIX) && globalZ>0.) {chargeDistriFPixPlusPreCuts->Fill(testclust->charge());numbPixInClusterFPixPlusPreCuts->Fill(testclust->size());}
	if(type==int(kFPIX) && globalZ<0.) {chargeDistriFPixMinusPreCuts->Fill(testclust->charge());numbPixInClusterFPixMinusPreCuts->Fill(testclust->size());}	
      }



      //*******  INSERTING TUNNIG INTO HISTOS *******
      if(DEBUG) std::cout<<"INSERTING TUNNIG INTO HISTOS"<<std::endl;
      
      if(muonTimingMap.size()!=unsigned(tunningMuonMis->GetNbinsX()))
        cout<<"There is a problem with the muon tunning histo's binning !!"<<endl;
      if(edgeCutMap.size()!=unsigned(tunningEdgeMis->GetNbinsX()))
        cout<<"There is a problem with the edge tunning histo's binning !! But it's not really a problem ..."<<endl;
      
      if((*testhit).getType()==TrackingRecHit::missing){
        int i=0;
        for(map<double,bool>::const_iterator mapit=edgeCutMap.begin();mapit!=edgeCutMap.end();mapit++){
	  i++;
	  if( !(mapit->second) )  tunningEdgeMis->SetBinContent(i,tunningEdgeMis->GetBinContent(i)+1);
	}
	
	i=0;   
	for(map<double,bool>::const_iterator mapit=muonTimingMap.begin();mapit!=muonTimingMap.end();mapit++){
	  i++;
	  if(mapit->second) tunningMuonMis->SetBinContent(i,tunningMuonMis->GetBinContent(i)+1);
	  //std::cout<<"map["<<mapit->first<<"]="<<mapit->second<<"    ";
	  
	  int j=0;
	  for(map<double,bool>::const_iterator mapit2=edgeCutMap.begin();mapit2!=edgeCutMap.end();mapit2++){
	    j++;
	    if(!(mapit2->second) && mapit->second && isTelescopeGood && hasHighPT) tunningMis->SetBinContent(j,i, tunningMis->GetBinContent(j,i)+1);
	  }
	}
      }
      else if((*testhit).isValid()){
        int i=0;
        for(map<double,bool>::const_iterator mapit=edgeCutMap.begin();mapit!=edgeCutMap.end();mapit++){
	  i++;
	  if(!(mapit->second))  tunningEdgeVal->SetBinContent(i,tunningEdgeVal->GetBinContent(i)+1);
	}
	
	i=0;   
	for(map<double,bool>::const_iterator mapit=muonTimingMap.begin();mapit!=muonTimingMap.end();mapit++){
	  i++;
	  if(mapit->second) tunningMuonVal->SetBinContent(i,tunningMuonVal->GetBinContent(i)+1);
	  //std::cout<<"map["<<mapit->first<<"]="<<mapit->second<<"    ";
	  
	  int j=0;
	  for(map<double,bool>::const_iterator mapit2=edgeCutMap.begin();mapit2!=edgeCutMap.end();mapit2++){
	    j++;
	    if(!(mapit2->second) && mapit->second && isTelescopeGood && hasHighPT) tunningVal->SetBinContent(j,i, tunningVal->GetBinContent(j,i)+1);
	  }
	}
      }
      
      

  	//************************ Fill the HISTOS for the analysis on the Selection Cuts Quality  *****************************
        /*legend:
	0=without any selection;
	1=loose cut (i.e. at least 1 another valid hit in the same track);
	2=telscopic cut;
	3=timing cut;
	4=edge cuts;
	5= final combined cuts;
	*/
	
       if((*testhit).getType()==TrackingRecHit::missing){
         xPosFracMis->Fill(xposfrac_,0);
         yPosFracMis->Fill(yposfrac_,0);
	 if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(0.5);
	 if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(0.5);
	 
	 if(numOfOtherValid>=1){
           xPosFracMis->Fill(xposfrac_,1);
           yPosFracMis->Fill(yposfrac_,1);
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(1.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(1.5);
	 }
	 
	 if(isTelescopeGood){
           xPosFracMis->Fill(xposfrac_,2);
           yPosFracMis->Fill(yposfrac_,2);
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(2.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(2.5);
	 }
	 if(hasGoodTiming){
           xPosFracMis->Fill(xposfrac_,3);
           yPosFracMis->Fill(yposfrac_,3);
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(3.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(3.5);
	 }
	 if(!isNotInMiddle){
           xPosFracMis->Fill(xposfrac_,4);
           yPosFracMis->Fill(yposfrac_,4);
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(4.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(4.5);
	 }
	 if(hasHighPT){
           xPosFracMis->Fill(xposfrac_,4);
           yPosFracMis->Fill(yposfrac_,4);
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(5.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(5.5);
	 }
//combination of cuts: 3 entries	 
	 if(isTelescopeGood && hasGoodTiming){
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(6.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(6.5);
	 }
	 if(isTelescopeGood && !isNotInMiddle){
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(7.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(7.5);
	 }
	 if(!isNotInMiddle && hasGoodTiming){
	   if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(8.5);
	   if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(8.5);
	 }
       }//end-if missing recHit
       
       else if((*testhit).isValid()){
         xPosFracVal->Fill(xposfrac_,0);
         yPosFracVal->Fill(yposfrac_,0);
	 if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(0.5);
	 if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(0.5);
	 
	 if(numOfOtherValid>=1){
           xPosFracVal->Fill(xposfrac_,1);
           yPosFracVal->Fill(yposfrac_,1);
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(1.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(1.5);
	 }
	 
	 if(isTelescopeGood){
           xPosFracVal->Fill(xposfrac_,2);
           yPosFracVal->Fill(yposfrac_,2);
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(2.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(2.5);
	 }
	 
	 if(hasGoodTiming){
           xPosFracVal->Fill(xposfrac_,3);
           yPosFracVal->Fill(yposfrac_,3);
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(3.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(3.5);
	 }
	 
	 if(!isNotInMiddle){
           xPosFracVal->Fill(xposfrac_,4);
           yPosFracVal->Fill(yposfrac_,4);
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(4.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(4.5);
	 }
	 
	 if(hasHighPT){
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(5.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(5.5);
	 }
	 
	 if(isTelescopeGood && hasGoodTiming){
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(6.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(6.5);
	 }
	 if(isTelescopeGood && !isNotInMiddle){
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(7.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(7.5);
	 }
	 if(!isNotInMiddle && hasGoodTiming){
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(8.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(8.5);
	 }
	 
       }
  
  
  
        //do the efficiencyVSmuontime with the "official cuts" applied
      //  if( (listOfCuts_.getParameter<bool>("telescope_cut") && isTelescopeGood)
	// && (listOfCuts_.getParameter<bool>("edge_cut") && !isNotInMiddle) ) {
          if ((*testhit).getType()== TrackingRecHit::valid){
            if ( runNumber < 68094 ) validVsMuontimePre68094->Fill(time);
            else    validVsMuontimePost68094->Fill(time);
	  }
          if ((*testhit).getType()== TrackingRecHit::missing){
            if ( runNumber < 68094 ) missingVsMuontimePre68094->Fill(time);
            else    missingVsMuontimePost68094->Fill(time);
	  }
	//}
	
	
	//do the eff VS muon time VS charge
	if( isTelescopeGood && !isNotInMiddle && testclust.isNonnull()) {
	  if(type==int(kBPIX)){
	    if ((*testhit).getType()==TrackingRecHit::valid) ;
	      validMuonTimeVSchargeBPix->Fill(time,testclust->charge());
	    if ((*testhit).getType()==TrackingRecHit::missing)
	      missingMuonTimeVSchargeBPix->Fill(time,testclust->charge());
	  }
	  if(type==int(kFPIX)){
	    if ((*testhit).getType()==TrackingRecHit::valid)
	      validMuonTimeVSchargeFPix->Fill(time,testclust->charge());
	    if ((*testhit).getType()==TrackingRecHit::missing)
	      missingMuonTimeVSchargeFPix->Fill(time,testclust->charge());
	  }
  	}
	
	if((*testhit).getType()==TrackingRecHit::valid){
	  validVSMuonTimeError->Fill(timerr);
	  validVSMuonTimeNdof->Fill(timendof);
	}
	if((*testhit).getType()==TrackingRecHit::missing){
	  missingVSMuonTimeError->Fill(timerr);
	  missingVSMuonTimeNdof->Fill(timendof);
	}
	
	//FILLING TRIGGER PLOTS
	if (L1GTRR.isValid()) {
          for(Int_t iTrig1 = 0;iTrig1<64;++iTrig1){
	    for(Int_t iTrig2 = 0;iTrig2<64;++iTrig2){
	      if(L1GTRR->technicalTriggerWord()[iTrig1] && L1GTRR->technicalTriggerWord()[iTrig2]){
	        if(type==int(kBPIX)){
		  if((*testhit).getType()==TrackingRecHit::missing)  missingPerTriggerBPix->Fill(iTrig1,iTrig2);
		  if((*testhit).getType()==TrackingRecHit::valid)    validPerTriggerBPix->Fill(iTrig1,iTrig2);
		  if((*testhit).getType()==TrackingRecHit::inactive) inactivePerTriggerBPix->Fill(iTrig1,iTrig2);
		}
	        if(type==int(kFPIX)){
		  if((*testhit).getType()==TrackingRecHit::missing)  missingPerTriggerFPix->Fill(iTrig1,iTrig2);
		  if((*testhit).getType()==TrackingRecHit::valid)    validPerTriggerFPix->Fill(iTrig1,iTrig2);
		  if((*testhit).getType()==TrackingRecHit::inactive) inactivePerTriggerFPix->Fill(iTrig1,iTrig2);
		}
	      }
	    }
	  }
	}
	
	
	
  
  	//************************************ HERE IS THE CUT *****************************************************
        //different tightness in the cuts: 

        //if (numOfOtherValid<1) continue;
        //if( (!hasValidInLowerPix) || (!hasValidInUpperPix)  ) continue; 
        //if(  isNotInMiddle  ) continue; 
        //if(  (!hasGoodTiming) ) continue;
	//if( !hasHighPT || isNotInMiddle || (!hasGoodTiming)) continue;
        //if( (!isTelescopeGood) || isNotInMiddle || (!hasGoodTiming) ) continue; 
        //if( (!isTelescopeGood) || isNotInMiddle || (!hasGoodTiming) || !hasHighPT ) continue;
        //if( (numOfOtherValid<1) || isNotInMiddle || (!hasGoodTiming) || !hasHighPT ) continue;
        //if( isNotInMiddle || (!hasGoodTiming) || !hasHighPT ) continue;
	//if( !isTelescopeGood || isNotInMiddle || !hasHighPT) continue;
        //can also use : isNotInMiddle && hasGoodTiming && isTelescopeGood
	
	if(listOfCuts_.getParameter<bool>("pT_cut") && !hasHighPT ) continue;
	if(listOfCuts_.getParameter<bool>("edge_cut") && isNotInMiddle ) continue;
	if(listOfCuts_.getParameter<bool>("telescope_cut") && !isTelescopeGood ) continue;
	if(listOfCuts_.getParameter<bool>("muon_cut") && !hasGoodTiming ) continue;
	if(listOfCuts_.getParameter<bool>("loose_cut") && numOfOtherValid<1 ) continue;

        if((*testhit).getType()==TrackingRecHit::missing){
	  missingChiSquare->Fill(chiSquare);
          missingChiSquareNdf->Fill(chiSquareNdf);
          missingInTrack++;
	    
	  xPosFracMis->Fill(xposfrac_,5);
          yPosFracMis->Fill(yposfrac_,5);
	  if(type==int(kBPIX)) hitsPassingCutsMisBPix->Fill(9.5);
	  if(type==int(kFPIX)) hitsPassingCutsMisFPix->Fill(9.5);
	}
	else if((*testhit).isValid()){
	
	  validChiSquare->Fill(chiSquare);
	  validChiSquareNdf->Fill(chiSquareNdf);
	  validInTrack++;
	  
          xPosFracVal->Fill(xposfrac_,5);
          yPosFracVal->Fill(yposfrac_,5);
	   if(type==int(kBPIX)) hitsPassingCutsValBPix->Fill(9.5);
	   if(type==int(kFPIX)) hitsPassingCutsValFPix->Fill(9.5);
	}
	 //********** Fill efficiency VS pT, eta, phi ************
      
        if( (*testhit).isValid() ){
	  validVsPT->Fill(PT);
	  validVsPT_lowpt->Fill(PT);
	  validVsEta->Fill(eta);
	  validVsPhi->Fill(phi);
	}
        if( (*testhit).getType()==TrackingRecHit::missing ){
	  missingVsPT->Fill(PT);
	  missingVsPT_lowpt->Fill(PT);
	  missingVsEta->Fill(eta);
	  missingVsPhi->Fill(phi);
	}
	if( (*testhit).getType()==TrackingRecHit::inactive ){
	  inactiveVsEta->Fill(eta);
	  inactiveVsPhi->Fill(phi);
	}
	
	for(int nHits = 1;nHits<=12;++nHits){
	  if(int(track.recHitsSize())>=nHits){
            if((*testhit).isValid()){
	      if(type==int(kBPIX))
	        validPerMinHitsOnTrackBPix->Fill(nHits);
	      if(type==int(kFPIX))
	        validPerMinHitsOnTrackFPix->Fill(nHits);
	    }
            if( (*testhit).getType()==TrackingRecHit::missing ){
	      if(type==int(kBPIX))
	        missingPerMinHitsOnTrackBPix->Fill(nHits);
	      if(type==int(kFPIX))
	        missingPerMinHitsOnTrackFPix->Fill(nHits);
	    }
	  }
	}
	
	//cout<<"tunning2 VAL"<<tunningVal->GetBinContent(11,11)<<endl;
	//cout<<"hits VAL"<<hitsPassingCutsValFPix->GetBinContent(9)+hitsPassingCutsValBPix->GetBinContent(9)<<endl;
	
	 
      //**********  fill CLUSTER properties for valid recHit passing ALL the selection CUTS     
      if(testclust.isNonnull()){
        chargeDistri->Fill(testclust->charge());
	numbPixInCluster->Fill(testclust->size());
	numbPixInClusterX->Fill(testclust->sizeX());
	numbPixInClusterY->Fill(testclust->sizeY());
	xposCluster->Fill(testclust->x());
	yposCluster->Fill(testclust->y());
	
        if(type==int(kBPIX)){
	  chargeDistriBPix->Fill(testclust->charge());
	  numbPixInClusterBPix->Fill(testclust->size());
	  chargeVsDimensionBPix->Fill(testclust->charge(),testclust->size());
	}
	if(type==int(kFPIX) && globalZ>0.) {chargeDistriFPixPlus->Fill(testclust->charge());numbPixInClusterFPixPlus->Fill(testclust->size());}
	if(type==int(kFPIX) && globalZ<0.) {chargeDistriFPixMinus->Fill(testclust->charge());numbPixInClusterFPixMinus->Fill(testclust->size());}	
      }
      
      
      
      //THIS IS ONLY TO CHECK THE DEFINITION OF INACTIVE-FLAG!!!
	 if ( (*testhit).getType()==TrackingRecHit::inactive )  numInactiveHit++;
	 if ( (*testhit).getType()==TrackingRecHit::missing )   numMissingHit++;
	 if ( (*testhit).getType()==TrackingRecHit::valid )   numValidHit++;
	  
	 //***********************************
	 //type of invalid recHiT
	
	 //let's observe better invalid not missing recHits
         double specificInvalid = 0.5;  //something else than inactive or missing or bad
	 if ( ! ((*testhit).isValid()) ){
	   if ( (*testhit).getType()==TrackingRecHit::inactive )     specificInvalid = 1.5;
	   if ( (*testhit).getType()==TrackingRecHit::bad )          specificInvalid = 2.5;
	   if ( (*testhit).getType()==TrackingRecHit::missing )      specificInvalid = 3.5;
	 }
	 if ( ( (*testhit).isValid() && badModule ) )
	   specificInvalid = 4.5;
	 
	 histInvalidRecHitCollection->Fill(specificInvalid);
	 
	 //histWithBadmoduleList
	 //fill 4th bin when we have a valid recHit belonging to badModule list: we expect 0 !!!
	 if (specificInvalid == 4)                    histInvalidRecHitWithBadmoduleList->Fill(specificInvalid);
	 //the other 3 bins are filled for the invalid statistic and excluding bad module list
	 if ( (!((*testhit).isValid()))&&(!(badModule)) )  histInvalidRecHitWithBadmoduleList->Fill(specificInvalid);
	 
	 
       //**********  create the TREE used for the analysis ****************
     
             bool founded=0;
             for (unsigned int it=0; it<goodModuleMap.size(); it++)
	       {
	       if (goodModuleMap[it][0]==moduleRawId)
	         {
	         founded=1;
	         if ( (*testhit).getType()==TrackingRecHit::inactive ) (goodModuleMap[it])[1]++;
	         if ( (*testhit).getType()==TrackingRecHit::missing )  (goodModuleMap[it])[2]++;
		 if ( (*testhit).isValid() )                           (goodModuleMap[it])[3]++;
	         break;
	         }
	       }
             if (!founded)
               {
	       vector<double> aux;
	       aux.push_back(moduleRawId);
	       for (int j=1;j<=10; j++) aux.push_back(0);
	       goodModuleMap.push_back(aux);
               if ( (*testhit).getType()==TrackingRecHit::inactive ) (goodModuleMap[goodModuleMap.size()-1])[1]++;
	       if ( (*testhit).getType()==TrackingRecHit::missing )  (goodModuleMap[goodModuleMap.size()-1])[2]++;
	       if ( (*testhit).isValid() )                           (goodModuleMap[goodModuleMap.size()-1])[3]++;
	       if ( type==int(kBPIX) )
	         {
		 (goodModuleMap[goodModuleMap.size()-1])[4]= 1;
		 (goodModuleMap[goodModuleMap.size()-1])[5]= ladder;
		 (goodModuleMap[goodModuleMap.size()-1])[6]= 0;
		 (goodModuleMap[goodModuleMap.size()-1])[7]= moduleInLadder;
		 }
	       if ( type==int(kFPIX) ) 
	         {
		 (goodModuleMap[goodModuleMap.size()-1])[4]= 2;
		 (goodModuleMap[goodModuleMap.size()-1])[5]= 0;
		 (goodModuleMap[goodModuleMap.size()-1])[6]= blade;
		 (goodModuleMap[goodModuleMap.size()-1])[7]= moduleInLadder;  //by now it's ZERO for BPix
		 }
	       (goodModuleMap[goodModuleMap.size()-1])[8]=globalX;       
	       (goodModuleMap[goodModuleMap.size()-1])[9]=globalY;       	       
	       (goodModuleMap[goodModuleMap.size()-1])[10]=globalZ;       	       
  	       }
	       
	       
	 if ( ! ((*testhit).isValid()) )  invalidPerRun->Fill(testLabel,1);
	 if (   ((*testhit).isValid()) )  validPerRun->Fill(testLabel,1);
	 if ( (*testhit).getType()==TrackingRecHit::inactive ) inactivePerRun->Fill(testLabel,1);
	 

      //**********  METHOD 1 plots for subdetectors and run *************************
 
         int filling = 0;
         if((*testhit).isValid() ) filling=2;
         if (!((*testhit).isValid()) ) filling=0;
	 if (!((*testhit).isValid()) && (*testhit).getType()==TrackingRecHit::missing) filling=1;	 
	 
	 histo->Fill(filling);

         if (type==int(kBPIX)){
	   histBarrel->Fill(filling);
	   
	   
	   if (layer==1) histLayer1->Fill(filling);
	   if (layer==2) histLayer2->Fill(filling);
	   if (layer==3) histLayer3->Fill(filling);
	 }//barrel
	 
	 if (type==int(kFPIX)){
	   histEndcap->Fill(filling);
	   
	   if(theGeomDet->surface().position().z() < 0.0) histEndcapMinus->Fill(filling);
	   else                                           histEndcapPlus->Fill(filling);
	 }
	 
	 if( (*testhit).isValid() ){
	   if(layer==1) validPerSubdetector->Fill(0.5);
	   if(layer==2) validPerSubdetector->Fill(1.5);
	   if(layer==3) validPerSubdetector->Fill(2.5);
	   if(type==int(kFPIX) && globalZ < 0.0) validPerSubdetector->Fill(3.5);
	   if(type==int(kFPIX) && globalZ >= 0.0) validPerSubdetector->Fill(4.5);
	 }
	 else if ( (*testhit).getType()==TrackingRecHit::missing ){
	   if(layer==1) missingPerSubdetector->Fill(0.5);
	   if(layer==2) missingPerSubdetector->Fill(1.5);
	   if(layer==3) missingPerSubdetector->Fill(2.5);
	   if(type==int(kFPIX) && globalZ < 0.0) missingPerSubdetector->Fill(3.5);
	   if(type==int(kFPIX) && globalZ >= 0.0) missingPerSubdetector->Fill(4.5);
	 }
	 else if ( (*testhit).getType()==TrackingRecHit::inactive ){
	   if(layer==1) inactivePerSubdetector->Fill(0.5);
	   if(layer==2) inactivePerSubdetector->Fill(1.5);
	   if(layer==3) inactivePerSubdetector->Fill(2.5);
	   if(type==int(kFPIX) && globalZ < 0.0) inactivePerSubdetector->Fill(3.5);
	   if(type==int(kFPIX) && globalZ >= 0.0) inactivePerSubdetector->Fill(4.5);
	 }
	 

      //****************  angular analysis: METHOD 1    ******************
      
      LocalTrajectoryParameters ltp = tsos.localParameters();
      LocalVector localDir = ltp.momentum()/ltp.momentum().mag();

      float locx = localDir.x();
      float locy = localDir.y();
      float locz = localDir.z();
      float alpha = atan2( locz, locx );
      float  beta = atan2( locz, locy );   
      
      if ((*testhit).getType()== TrackingRecHit::valid){
        validVsAlpha->Fill( alpha );
        validVsCotanAlpha->Fill( 1/tan(alpha) );
	validVsBeta->Fill( beta );
	validAlphaBeta->Fill(alpha,beta);
      }
      if ((*testhit).getType()== TrackingRecHit::missing){
        missingVsAlpha->Fill( alpha );
        missingVsCotanAlpha->Fill( 1/tan(alpha) );
	missingVsBeta->Fill( beta);
	missingAlphaBeta->Fill(alpha,beta);
      }
      
      if(testSubDetID == PixelSubdetector::PixelBarrel){
        if ((*testhit).getType()== TrackingRecHit::valid){
	  validVsAlphaBPix->Fill( alpha );
	  validVsBetaBPix->Fill( beta );
	  
	  if (isBigModule){
	    validAlphaLocalXBigBPix->Fill(alpha,tsos.localPosition().x());
	    validBetaLocalXBigBPix->Fill(beta,tsos.localPosition().x());
	  }
	  validAlphaLocalYBPix->Fill(alpha,tsos.localPosition().y());
	  validBetaLocalYBPix->Fill(beta,tsos.localPosition().y());
	}
        if ((*testhit).getType()== TrackingRecHit::missing){
	   missingVsAlphaBPix->Fill( alpha);
	   missingVsBetaBPix->Fill( beta);
	   
	   if (isBigModule){
	    missingAlphaLocalXBigBPix->Fill(alpha,tsos.localPosition().x());
	    missingBetaLocalXBigBPix->Fill(beta,tsos.localPosition().x());
	  }
	  missingAlphaLocalYBPix->Fill(alpha,tsos.localPosition().y());
	  missingBetaLocalYBPix->Fill(beta,tsos.localPosition().y());	   
	}
      }
      
      if(testSubDetID == PixelSubdetector::PixelEndcap){
        if ((*testhit).getType()== TrackingRecHit::valid){
	  validVsAlphaFPix->Fill( alpha );
	  validVsBetaFPix->Fill( beta );
	  
	  if (isBigModule){
	    validAlphaLocalXBigFPix->Fill(alpha,tsos.localPosition().x());
	    validBetaLocalXBigFPix->Fill(beta,tsos.localPosition().x());
	  }
	  validAlphaLocalYFPix->Fill(alpha,tsos.localPosition().y());
	  validBetaLocalYFPix->Fill(beta,tsos.localPosition().y());
	}
        if ((*testhit).getType()== TrackingRecHit::missing){
	   missingVsAlphaFPix->Fill( alpha);
           missingVsBetaFPix->Fill( beta);
	   
	   if (isBigModule){
	    missingAlphaLocalXBigFPix->Fill(alpha,tsos.localPosition().x());
	    missingBetaLocalXBigFPix->Fill(beta,tsos.localPosition().x());
	  }
	  missingAlphaLocalYFPix->Fill(alpha,tsos.localPosition().y());
	  missingBetaLocalYFPix->Fill(beta,tsos.localPosition().y());	
	}
      }

	if ((*testhit).getType()== TrackingRecHit::missing){
          missingVsLocalX->Fill(tsos.localPosition().x());
          missingVsLocalY->Fill(tsos.localPosition().y());
	  
	  if (isBigModule){
	    missingVsLocalXBig->Fill(tsos.localPosition().x());
	    missingAlphaLocalXBig->Fill(alpha,tsos.localPosition().x());
	  }
	  else {
	    missingVsLocalXSmall->Fill(tsos.localPosition().x());
	    missingAlphaLocalXSmall->Fill(alpha,tsos.localPosition().x());
	  }
	}
	
	if ((*testhit).getType()== TrackingRecHit::valid){
	  validVsLocalX->Fill(tsos.localPosition().x());	  
	  validVsLocalY->Fill(tsos.localPosition().y());
	  
	  if (isBigModule){
	    validVsLocalXBig->Fill(tsos.localPosition().x());
	    validAlphaLocalXBig->Fill(alpha,tsos.localPosition().x());
	  }
	  else{
	    validVsLocalXSmall->Fill(tsos.localPosition().x());
	    validAlphaLocalXSmall->Fill(alpha,tsos.localPosition().x());
	  }
	}
	

      //*****************   METHOD 2: window searching      *****************
	
      bool hasClusterOnSameModule=false;
      bool hasCluster=false;
      int  clusterSize=-1;
      double clusterCharge=-1;
      double clusterXPos=-999;
      double clusterYPos=-999;
      
      //LocalError tsosErr = tsos.localError().positionError();
      //double error
      
      if ( testSubDetID==int(kBPIX) ){

      DetId hitDetId = (testhit->geographicalId());
      PXBDetId hitPdetId = PXBDetId(hitDetId);
      unsigned int hitLayer  =hitPdetId.layer();
      unsigned int hitLadder =hitPdetId.ladder();
      unsigned int hitModule=hitPdetId.module();
      
//+++++++++++++++++++++++cleaning with danek misconfigured module List+++++++++++++++++++++
      bool goodModule=true;
      if ((hitLayer==1&&
            ((hitLadder==4&&hitModule==6)||
	    (hitLadder==12&&hitModule==5)||
	    (hitLadder==14&&hitModule==5)||
	    (hitLadder==16&&hitModule==5)))
          ||(hitLayer==2&&	  
            ((hitLadder==3&&hitModule==8)||
	    (hitLadder==12&&(hitModule==5||hitModule==7||hitModule==8))||
	    (hitLadder==20&&hitModule==8)||
	    (hitLadder==26&&hitModule==1)||
	    (hitLadder==28&&hitModule==2)))
	  ||(hitLayer==3&&
	    ((hitLadder==3&&hitModule==3)||
	    (hitLadder==19&&hitModule==5)||
	    (hitLadder==32&&hitModule==1)||
	    (hitLadder==35&&(hitModule<=4||hitModule==7))||
	    (hitLadder==37&&(hitModule<=6&&hitModule>=2))||
	    (hitLadder==39&&hitModule==8)||
	    (hitLadder==43&&hitModule==7))) 
         )
        goodModule=false;        
	
//test
      if ((*testhit).getType()==TrackingRecHit::missing) histoMethod2->Fill(0);
      if ((*testhit).getType()==TrackingRecHit::valid)   histoMethod2->Fill(1);
//
      if ( (*testhit).getType()==TrackingRecHit::valid || (*testhit).getType()==TrackingRecHit::missing ){
	double minDistance=999999;
	double minDistanceOnSameModule=999999;
	for (edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter = input.begin(); DSViter != input.end(); DSViter++)
	  {
	  unsigned int ClusterId = DSViter->id();
          DetId ClusterDetId(ClusterId);
	  PXBDetId pdetId = PXBDetId(ClusterDetId);
          unsigned int clusterLayer=0;
	  clusterLayer=pdetId.layer();
	  if ( clusterLayer != hitLayer) continue;
	  unsigned int detidaux = DSViter->detId();
          DetId detIdObject( detidaux );  
          //you don't use this last line
	  //const GeomDetUnit * genericDet = geom->idToDetUnit( detIdObject );
		  
          edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
          edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
          	  
	  for(edmNew::DetSet<SiPixelCluster>::const_iterator clustIt=begin; clustIt!=end;++clustIt)
            {
            const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detIdObject) );
	    const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	    LocalPoint lpclust = topol->localPosition(MeasurementPoint((*clustIt).x(),(*clustIt).y()));
	    GlobalPoint GPclust =  theGeomDet->surface().toGlobal(Local3DPoint(lpclust.x(),lpclust.y(),lpclust.z()));
            double deltaX,deltaY,deltaZ;
	    deltaX= GPclust.x()- tsos.globalPosition().x();
	    deltaY= GPclust.y()- tsos.globalPosition().y();
	    deltaZ= GPclust.z()- tsos.globalPosition().z();
	    double distance;
	    distance = sqrt( pow(deltaZ,2)+pow(deltaX,2)+pow(deltaY,2) );
	    
	    if (distance<minDistance)
	      minDistance=distance;

	    if (distance<minDistanceOnSameModule && 
	        testhit->geographicalId().rawId()==theGeomDet->geographicalId().rawId()){
	           minDistanceOnSameModule=distance;
		clusterCharge = clustIt->charge();
		clusterSize = clustIt->size();
		clusterXPos = clustIt->x();
		clusterYPos = clustIt->y();
	    }
		   
	    }//end-for first cluster loop	   
	  }//end-for second cluster loop 

	if (minDistance>0.5)
	  {
          //missingVsLocalX->Fill(tsos.localPosition().x());
          //missingVsLocalY->Fill(tsos.localPosition().y());
	  //test
	  histoMethod2After->Fill(0);
	  }
	else
	  {
	  //validVsLocalX->Fill(tsos.localPosition().x());	  
	  //validVsLocalY->Fill(tsos.localPosition().y());
	  //test
	  histoMethod2After->Fill(1);
	  }
	
	windowSearchSameModule->Fill(minDistanceOnSameModule);
	windowSearch->Fill(minDistance); 
	windowSearchBPix->Fill(minDistance);
	if (goodModule) windowSearchGoodModulesBPix->Fill(minDistance);
	
	for (int p=0;p<ptranges;p++)
	  if (PT>(double)p && PT<=(double)(p+1)) windowSearch_pT[p]->Fill(minDistance);
	if (PT>(double)(ptranges))  windowSearch_pT[ptranges]->Fill(minDistance);
	  

	if(minDistance!=999999)
	  hasCluster=true;
	if(minDistanceOnSameModule!=999999)
	  hasClusterOnSameModule=true;
	   
	}//end-if "work-on-valid-&-missing"    
      }//end-if method 2 for barrel



      //**********************************
      //window searching analysis FPix
      
      if ( testSubDetID==int(kFPIX) ){

      DetId hitDetId = (testhit->geographicalId());
      PXFDetId hitPdetId = PXFDetId(hitDetId);
      unsigned int hitDisk=0;
      hitDisk=hitPdetId.disk();        

      if ((*testhit).getType()==TrackingRecHit::missing) histoMethod2FPix->Fill(0);
      if ((*testhit).getType()==TrackingRecHit::valid)   histoMethod2FPix->Fill(1);

      if ( (*testhit).getType()==TrackingRecHit::valid || (*testhit).getType()==TrackingRecHit::missing ){
      
	double minDistance=999999;
	double minDistanceOnSameModule=999999;
	for (edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter = input.begin(); DSViter != input.end(); DSViter++) {
	  unsigned int ClusterId = DSViter->id();
          DetId ClusterDetId(ClusterId);
	  PXFDetId pdetId = PXFDetId(ClusterDetId);
          unsigned int clusterDisk=0;
	  clusterDisk=pdetId.disk();
	  if ( clusterDisk != hitDisk) continue;	  
	  unsigned int detidaux = DSViter->detId();
          DetId detIdObject( detidaux );  
          //const GeomDetUnit * genericDet = geom->idToDetUnit( detIdObject );
		  
          edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
          edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
          	  
	  for(edmNew::DetSet<SiPixelCluster>::const_iterator clustIt=begin; clustIt!=end;++clustIt) {
            const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detIdObject) );
	    const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	    LocalPoint lpclust = topol->localPosition(MeasurementPoint((*clustIt).x(),(*clustIt).y()));
	    GlobalPoint GPclust =  theGeomDet->surface().toGlobal(Local3DPoint(lpclust.x(),lpclust.y(),lpclust.z()));
            double deltaX,deltaY,deltaZ;
	    deltaX= GPclust.x()- tsos.globalPosition().x();
	    deltaY= GPclust.y()- tsos.globalPosition().y();
	    deltaZ= GPclust.z()- tsos.globalPosition().z();
	    double distance;
	    distance = sqrt( pow(deltaZ,2)+pow(deltaX,2)+pow(deltaY,2) );
	    
	    if (distance<minDistanceOnSameModule &&
	        testhit->geographicalId().rawId()==theGeomDet->geographicalId().rawId() ){
		minDistanceOnSameModule=distance;
		clusterCharge = clustIt->charge();
		clusterSize = clustIt->size();
		clusterXPos = clustIt->x();
		clusterYPos = clustIt->y();
	    }
	      //??LocalError errClust = (*clustIt).localError().positionError();
              //??LocalError err=(*clustIt).localPositionError();
              //??LocalError leclust = topol->localError().positionError(MeasurementPoint((*clustIt).x(),(*clustIt).y()));

	    if (distance<minDistance)
	      minDistance=distance;
	      
	      
	    }//end-for first cluster loop	   
	  }//end-for second cluster loop 

//test
	if (minDistance>0.5)  histoMethod2After->Fill(0);
	else                  histoMethod2After->Fill(1);
	  
	windowSearchSameModule->Fill(minDistanceOnSameModule);
	windowSearchFPix->Fill(minDistance); 
	windowSearch->Fill(minDistance);
	
	if (PT<5.)            windowSearch_pT[0]->Fill(minDistance);
	if (PT>=5. && PT<10.) windowSearch_pT[1]->Fill(minDistance);
	if (PT>=10.)          windowSearch_pT[2]->Fill(minDistance);

	if(minDistance!=999999)
	  hasCluster=true;
	if(minDistanceOnSameModule!=999999)
	  hasClusterOnSameModule=true;
	  
	}//end-if "work-on-valid-&-missing"    
      }//end-if method 2 for forward
      
      
      //FOR BARREL && ENDCAP
      if ((*testhit).getType()==TrackingRecHit::missing){
        missingButClusterOnSameModule->Fill(hasClusterOnSameModule);
        missingButCluster->Fill(hasCluster);
	
	if(hasClusterOnSameModule){
	  chargeDistriMisRecovered->Fill(clusterCharge);
	  numbPixInClusterMisRecovered->Fill(clusterSize);
	  xposClusterMisRecovered->Fill(clusterXPos);
	  yposClusterMisRecovered->Fill(clusterYPos);
	}
      }
      else if((*testhit).isValid() && hasClusterOnSameModule){
	  chargeDistriValid->Fill(clusterCharge);
	  numbPixInClusterValid->Fill(clusterSize);
	  xposClusterValid->Fill(clusterXPos);
	  yposClusterValid->Fill(clusterYPos);
      }
      
      
      
      }//end-for trajMeasurements 
      
      //**********
       totalPerTrack->Fill( numPixHit );
       inactivePerTrack->Fill( (numInactiveHit/numPixHit)*100.);
       missingPerTrack->Fill( (numMissingHit/numPixHit)*100.);
       //****************
       
       validPerPixHitOnTrack->Fill(numPixHit,numValidHit);
       missingPerPixHitOnTrack->Fill(numPixHit,numMissingHit);
       inactivePerPixHitOnTrack->Fill(numPixHit,numInactiveHit);
       
       missPerTrackVsChiSquareNdf->Fill(chiSquareNdf,missingInTrack);
       missPerTrackPercentVsChiSquareNdf->Fill(chiSquareNdf,missingInTrack/(missingInTrack+validInTrack));
	     
    }//end-for of Trajectories

  //not needed anymore ...
  //}catch ( ... ) {std::cout<<"I've had a problem, skipped the try !!"<<std::endl;}

  if(DEBUG) cout<<"End of Analyze"<<endl;

}

// ------------ method called once each job just before starting event loop  ------------

void 
PixelEfficiency::beginJob(const edm::EventSetup& iSetup)
{
 if(DEBUG) std::cout<<"Begin job"<<std::endl;

 ptranges=11;  //the last one is the right tail of the distribution 
 
 fOutputFile = new TFile(fOutputFileName.c_str(), "RECREATE"); 

//test
 histoMethod2After = new TH1F("histoMethod2After","histoMethod2After",2,0,2);
 histoMethod2 = new TH1F("histoMethod2","histoMethod2",2,0,2);
 histoMethod2AfterFPix = new TH1F("histoMethod2AfterFPix","histoMethod2AfterFPix",2,0,2);
 histoMethod2FPix = new TH1F("histoMethod2FPix","histoMethod2FPix",2,0,2);
 
 
 histo = new TH1F("histo", "histo", 3, 0, 3);
 histLayer1 = new TH1F("histLayer1", "histLayer1", 3, 0, 3);
 histLayer2 = new TH1F("histLayer2", "histLayer2", 3, 0, 3);
 histLayer3 = new TH1F("histLayer3", "histLayer3", 3, 0, 3);
 histEndcapPlus = new TH1F("histEndcapPlus", "histEndcapPlus", 3, 0, 3);
 histEndcapMinus = new TH1F("histEndcapMinus", "histEndcapMinus", 3, 0, 3);
 histBarrel = new TH1F("histBarrel", "histBarrel", 3, 0, 3);
 histEndcap = new TH1F("histEndcap", "histEndcap", 3, 0, 3);
 validPerSubdetector = new TH1F("validPerSubdetector", "validPerSubdetector", 5, 0, 5);
 missingPerSubdetector = new TH1F("missingPerSubdetector", "missingPerSubdetector", 5, 0, 5);
 inactivePerSubdetector = new TH1F("inactivePerSubdetector", "inactivePerSubdetector", 5, 0, 5);
  
  
 histInvalidRecHitCollection = new TH1F("histInvalidRecHitCollection","histInvalidRecHitCollection",5,0,5);
 histInvalidRecHitWithBadmoduleList = new TH1F("histInvalidRecHitWithBadmoduleList","histInvalidRecHitWithBadmoduleList",5,0,5);
 
 consistencyCheck = new TH1F("consistencyCheck","consistencyCheck", 3, 0, 3);
 consistencyCheckTraj = new TH1F("consistencyCheckTraj","consistencyCheckTraj", 5, 0, 5);
 trackingEfficiency = new TH1F("trackingEfficiency","trackingEfficiency", 2, 0, 2);

 windowSearch = new TH1F("windowSearch","windowSearch",500,0,0.50);
 windowSearchSameModule = new TH1F("windowSearchSameModule","windowSearchSameModule",1000,0,10);
 windowSearchBPix = new TH1F("windowSearchBPix","windowSearchBPix",500,0,0.50);
 windowSearchGoodModulesBPix = new TH1F("windowSearchGoodModulesBPix","windowSearchGoodModulesBPix",500,0,0.50);
 windowSearchFPix = new TH1F("windowSearchFPix","windowSearchFPix",500,0,0.50);
 missingButClusterOnSameModule = new TH1F("missingButClusterOnSameModule","missingButClusterOnSameModule",2,0,2);
 missingButCluster = new TH1F("missingButCluster","missingButCluster",2,0,2);
 
 for (int p=0;p<=ptranges;p++)
   windowSearch_pT.push_back(0);
 for (int p=0;p<=ptranges;p++){
   char basename[20];
   sprintf (basename,"windowSearch_pT_range%i",p) ;
   std::string m_baseName = basename ;
   windowSearch_pT[p]= new TH1F( (m_baseName).c_str(),(m_baseName).c_str(), 500,0,0.50);;
   }
     
 chargeDistri = new TH1F("chargeDistri","chargeDistri",200,0,100000);
 numbPixInCluster = new TH1F("numbPixInCluster","numbPixInCluster",50,0,50);
 chargeDistriBPix = new TH1F("chargeDistriBPix","chargeDistriBPix",200,0,100000);
 numbPixInClusterBPix = new TH1F("numbPixInClusterBPix","numbPixInClusterBPix",50,0,50);
 chargeDistriFPixPlus = new TH1F("chargeDistriFPixPlus","chargeDistriFPixPlus",200,0,100000);
 numbPixInClusterFPixPlus = new TH1F("numbPixInClusterFPixPlus","numbPixInClusterFPixPlus",50,0,50);
 chargeDistriFPixMinus = new TH1F("chargeDistriFPixMinus","chargeDistriFPixMinus",200,0,100000);
 numbPixInClusterFPixMinus = new TH1F("numbPixInClusterFPixMinus","numbPixInClusterFPixMinus",50,0,50);

 chargeVsDimensionBPix = new TH2F("chargeVsDimensionBPix","chargeVsDimensionBPix",200,0,100000,50,0,50);

 chargeDistriPreCuts = new TH1F("chargeDistriPreCuts","chargeDistriPreCuts",200,0,100000);
 numbPixInClusterPreCuts = new TH1F("numbPixInClusterPreCuts","numbPixInClusterPreCuts",50,0,50);
 chargeDistriBPixPreCuts = new TH1F("chargeDistriBPixPreCuts","chargeDistriBPixPreCuts",200,0,100000);
 numbPixInClusterBPixPreCuts = new TH1F("numbPixInClusterBPixPreCuts","numbPixInClusterBPixPreCuts",50,0,50);
 chargeDistriFPixPlusPreCuts = new TH1F("chargeDistriFPixPlusPreCuts","chargeDistriFPixPlusPreCuts",200,0,100000);
 numbPixInClusterFPixPlusPreCuts = new TH1F("numbPixInClusterFPixPlusPreCuts","numbPixInClusterFPixPlusPreCuts",50,0,50);
 chargeDistriFPixMinusPreCuts = new TH1F("chargeDistriFPixMinusPreCuts","chargeDistriFPixMinusPreCuts",200,0,100000);
 numbPixInClusterFPixMinusPreCuts = new TH1F("numbPixInClusterFPixMinusPreCuts","numbPixInClusterFPixMinusPreCuts",50,0,50);

 numbPixInClusterX = new TH1F("numbPixInClusterX","numbPixInClusterX",30,0,30);
 numbPixInClusterY = new TH1F("numbPixInClusterY","numbPixInClusterY",30,0,30);
 xposCluster = new TH1F("xposCluster","xposCluster",200,-2,2);
 yposCluster = new TH1F("yposCluster","yposCluster",500,-10,10);
 chargeDistriValid = new TH1F("chargeDistriValid","chargeDistriValid",1000,0,100000);
 numbPixInClusterValid = new TH1F("numbPixInClusterValid","numbPixInClusterValid",50,0,50);
 chargeDistriMisRecovered = new TH1F("chargeDistriMisRecovered","chargeDistriMisRecovered",1000,0,100000);
 numbPixInClusterMisRecovered = new TH1F("numbPixInClusterMisRecovered","numbPixInClusterMisRecovered",50,0,50);
 xposClusterValid = new TH1F("xposClusterValid","xposClusterValid",200,-2,2);
 yposClusterValid = new TH1F("yposClusterValid","yposClusterValid",500,-10,10);
 xposClusterMisRecovered = new TH1F("xposClusterMisRecovered","xposClusterMisRecovered",200,-2,2);
 yposClusterMisRecovered = new TH1F("yposClusterMisRecovered","yposClusterMisRecovered",500,-10,10);
 
 
 validPerRun    = new TH1F("validPerRun","validPerRun",200,0,200);
 invalidPerRun  = new TH1F("invalidPerRun","invalidPerRun",200,0,200);
 inactivePerRun = new TH1F("inactivePerRun","inactivePerRun",200,0,200);
 
 numTracksVsRunNumber      = new TH1F("numTracksVsRunNumber","numTracksVsRunNumber",200,0,200);
 denTracksVsRunNumber      = new TH1F("denTracksVsRunNumber","denTracksVsRunNumber",200,0,200);
 muonTimeVSRunNumber       = new TH1F("muonTimeVSRunNumber","muonTimeVSRunNumber",200,0,200);
 nMuonTimeVSRunNumber      = new TH1F("nMuonTimeVSRunNumber","nMuonTimeVSRunNumber",200,0,200);
 muonTimeErrorVSRunNumber  = new TH1F("muonTimeErrorVSRunNumber","muonTimeErrorVSRunNumber",200,0,200);
 nMuonTimeErrorVSRunNumber = new TH1F("nMuonTimeErrorVSRunNumber","nMuonTimeErrorVSRunNumber",200,0,200);
 muonTimeErrorDistri       = new TH1F("muonTimeErrorDistri","muonTimeErrorDistri",80,0,40);
 validVSMuonTimeError      = new TH1F("validVSMuonTimeError","validVSMuonTimeError",80,0,40);
 missingVSMuonTimeError    = new TH1F("missingVSMuonTimeError","missingVSMuonTimeError",80,0,40);
 muonTimeNdofDistri        = new TH1F("muonTimeNdofDistri","muonTimeNdofDistri",50,0,50);
 validVSMuonTimeNdof       = new TH1F("validVSMuonTimeNdof","validVSMuonTimeNdof",50,0,50);
 missingVSMuonTimeNdof     = new TH1F("missingVSMuonTimeNdof","missingVSMuonTimeNdof",50,0,50);
 
//
 validVsAlpha = new TH1F("validVsAlpha","validVsAlpha",200,-3.5,3.5);
 missingVsAlpha = new TH1F("missingVsAlpha","missingVsAlpha",200,-3.5,3.5);
 validVsCotanAlpha = new TH1F("validVsCotanAlpha","validVsCotanAlpha",200,-3.5,3.5);
 missingVsCotanAlpha = new TH1F("missingVsCotanAlpha","missingVsCotanAlpha",200,-3.5,3.5);
 validVsBeta = new TH1F("validVsBeta","validVsBeta",200,-3.5,3.5);
 missingVsBeta = new TH1F("missingVsBeta","missingVsBeta",200,-3.5,3.5);

 validAlphaBeta   = new TH2F("validAlphaBeta"  ,"validAlphaBeta"  ,50,-3.5,3.5,50,-3.5,3.5);
 missingAlphaBeta = new TH2F("missingAlphaBeta","missingAlphaBeta",50,-3.5,3.5,50,-3.5,3.5);

 validVsAlphaBPix = new TH1F("validVsAlphaBPix","validVsAlphaBPix",200,-3.5,3.5);
 missingVsAlphaBPix = new TH1F("missingVsAlphaBPix","missingVsAlphaBPix",200,-3.5,3.5);
 validVsBetaBPix = new TH1F("validVsBetaBPix","validVsBetaBPix",200,-3.5,3.5);
 missingVsBetaBPix = new TH1F("missingVsBetaBPix","missingVsBetaBPix",200,-3.5,3.5);

 validVsAlphaFPix = new TH1F("validVsAlphaFPix","validVsAlphaFPix",200,-3.5,3.5);
 missingVsAlphaFPix = new TH1F("missingVsAlphaFPix","missingVsAlphaFPix",200,-3.5,3.5);
 validVsBetaFPix = new TH1F("validVsBetaFPix","validVsBetaFPix",200,-3.5,3.5);
 missingVsBetaFPix = new TH1F("missingVsBetaFPix","missingVsBetaFPix",200,-3.5,3.5);

 validVsLocalX = new TH1F("validVsLocalX","validVsLocalX",100,-1.5,1.5);
 missingVsLocalX = new TH1F("missingVsLocalX","missingVsLocalX",100,-1.5,1.5);
 validVsLocalY = new TH1F("validVsLocalY","validVsLocalY",100,-4.,4.);
 missingVsLocalY = new TH1F("missingVsLocalY","missingVsLocalY",100,-4.,4.);

 validVsLocalXBig = new TH1F("validVsLocalXBig","validVsLocalXBig",100,-1.5,1.5);
 missingVsLocalXBig = new TH1F("missingVsLocalXBig","missingVsLocalXBig",100,-1.5,1.5);
 validVsLocalXSmall = new TH1F("validVsLocalXSmall","validVsLocalXSmall",100,-1.5,1.5);
 missingVsLocalXSmall = new TH1F("missingVsLocalXSmall","missingVsLocalXSmall",100,-1.5,1.5);

 validAlphaLocalXBig = new TH2F("validAlphaLocalXBig","validAlphaLocalXBig",200,-3.5,3.5,100,-1.5,1.5);
 missingAlphaLocalXBig = new TH2F("missingAlphaLocalXBig","missingAlphaLocalXBig",200,-3.5,3.5,100,-1.5,1.5);
 validAlphaLocalXSmall = new TH2F("validAlphaLocalXSmall","validAlphaLocalXSmall",200,-3.5,3.5,100,-1.5,1.5);
 missingAlphaLocalXSmall = new TH2F("missingAlphaLocalXSmall","missingAlphaLocalXSmall",200,-3.5,3.5,100,-1.5,1.5);

 missingAlphaLocalXBigBPix = new TH2F("missingAlphaLocalXBigBPix","missingAlphaLocalXBigBPix",200,-3.5,3.5,100,-1.5,1.5);
 missingBetaLocalXBigBPix = new TH2F("missingBetaLocalXBigBPix","missingBetaLocalXBigBPix",200,-3.5,3.5,100,-1.5,1.5);
 missingAlphaLocalYBPix = new TH2F("missingAlphaLocalYBPix","missingAlphaLocalYBPix",200,-3.5,3.5,100,-4,4);
 missingBetaLocalYBPix = new TH2F("missingBetaLocalYBPix","missingBetaLocalYBPix",200,-3.5,3.5,100,-4,4);
 validAlphaLocalXBigBPix = new TH2F("validAlphaLocalXBigBPix","validAlphaLocalXBigBPix",200,-3.5,3.5,100,-1.5,1.5);
 validBetaLocalXBigBPix = new TH2F("validBetaLocalXBigBPix","validBetaLocalXBigBPix",200,-3.5,3.5,100,-1.5,1.5);
 validAlphaLocalYBPix = new TH2F("validAlphaLocalYBPix","validAlphaLocalYBPix",200,-3.5,3.5,100,-4,4);
 validBetaLocalYBPix = new TH2F("validBetaLocalYBPix","validBetaLocalYBPix",200,-3.5,3.5,100,-4,4);
  
 missingAlphaLocalXBigFPix = new TH2F("missingAlphaLocalXBigFPix","missingAlphaLocalXBigFPix",200,-3.5,3.5,100,-1.5,1.5);
 missingBetaLocalXBigFPix = new TH2F("missingBetaLocalXBigFPix","missingBetaLocalXBigFPix",200,-3.5,3.5,100,-1.5,1.5);
 missingAlphaLocalYFPix = new TH2F("missingAlphaLocalYFPix","missingAlphaLocalYFPix",200,-3.5,3.5,100,-4,4);
 missingBetaLocalYFPix = new TH2F("missingBetaLocalYFPix","missingBetaLocalYFPix",200,-3.5,3.5,100,-4,4);
 validAlphaLocalXBigFPix = new TH2F("validAlphaLocalXBigFPix","validAlphaLocalXBigFPix",200,-3.5,3.5,100,-1.5,1.5);
 validBetaLocalXBigFPix = new TH2F("validBetaLocalXBigFPix","validBetaLocalXBigFPix",200,-3.5,3.5,100,-1.5,1.5);
 validAlphaLocalYFPix = new TH2F("validAlphaLocalYFPix","validAlphaLocalYFPix",200,-3.5,3.5,100,-4,4);
 validBetaLocalYFPix = new TH2F("validBetaLocalYFPix","validBetaLocalYFPix",200,-3.5,3.5,100,-4,4);
 
 validVsMuontimePost68094 = new TH1F("validVsMuontimePost68094","validVPost68094sMuontime",50,-40.,80.);
 missingVsMuontimePost68094 = new TH1F("missingVsMuontimePost68094","missingVsMuontimePost68094",50,-40.,80.);
 validVsMuontimePre68094 = new TH1F("validVsMuontimePre68094","validVsMuontimePre68094",50,-40.,80.);
 missingVsMuontimePre68094 = new TH1F("missingVsMuontimePre68094","missingVsMuontimePre68094",50,-40.,80.);
 
 
 validMuonTimeVSchargeBPix   = new TH2F("validMuonTimeVSchargeBPix","validMuonTimeVSchargeBPix",60,-40.,80.,200,0,200000);
 missingMuonTimeVSchargeBPix = new TH2F("missingMuonTimeVSchargeBPix","missingMuonTimeVSchargeBPix",60,-40.,80.,200,0,200000);
 validMuonTimeVSchargeFPix   = new TH2F("validMuonTimeVSchargeFPix","validMuonTimeVSchargeFPix",60,-40.,80.,200,0,200000);
 missingMuonTimeVSchargeFPix = new TH2F("missingMuonTimeVSchargeFPix","missingMuonTimeVSchargeFPix",60,-40.,80.,200,0,200000);
     
 validVsPT = new TH1F("validVsPT","validVsPT",100,0.,50.);
 missingVsPT = new TH1F("missingVsPT","missingVsPT",100,0.,50.);
 validVsPT_lowpt = new TH1F("validVsPT_lowpt","validVsPT_lowpt",200,0.,5.);
 missingVsPT_lowpt = new TH1F("missingVsPT_lowpt","missingVsPT_lowpt",200,0.,5.);
      
 validVsEta = new TH1F("validVsEta","validVsEta",50,-5.,5.);
 missingVsEta = new TH1F("missingVsEta","missingVsEta",50,-5.,5.);
 inactiveVsEta = new TH1F("inactiveVsEta","inactiveVsEta",50,-5.,5.);
 validVsPhi = new TH1F("validVsPhi","validVsPhi",64,-3.2,3.2);
 missingVsPhi = new TH1F("missingVsPhi","missingVsPhi",64,-3.2,3.2);
 inactiveVsPhi = new TH1F("inactiveVsPhi","inactiveVsPhi",64,-3.2,3.2);

 validPerMinHitsOnTrackBPix   = new TH1F("validPerMinHitsOnTrackBPix","validPerMinHitsOnTrackBPix",12,1,13);
 validPerMinHitsOnTrackFPix   = new TH1F("validPerMinHitsOnTrackFPix","validPerMinHitsOnTrackFPix",12,1,13);
 missingPerMinHitsOnTrackBPix = new TH1F("missingPerMinHitsOnTrackBPix","missingPerMinHitsOnTrackBPix",12,1,13);
 missingPerMinHitsOnTrackFPix = new TH1F("missingPerMinHitsOnTrackFPix","missingPerMinHitsOnTrackFPix",12,1,13);
 
 checkoutValidityFlag = new TH1F("checkoutValidityFlag","checkoutValidityFlag", 4, 0,4);
 checkoutTraj = new TH1F("checkoutTraj","checkoutTraj",10,0,10);
 //
 totalPerTrack= new TH1F ("totalPerTrack","totalPerTrack",20,0,20);
 inactivePerTrack= new TH1F ("inactivePerTrack","inactivePerTrack",101,0,101);
 missingPerTrack= new TH1F ("missingPerTrack","missingPerTrack",101,0,101);
 
 hitsPassingCutsValBPix = new TH1F("hitsPassingCutsValBPix","hitsPassingCutsValBPix",10,0,10);
 hitsPassingCutsMisBPix = new TH1F("hitsPassingCutsMisBPix","hitsPassingCutsMisBPix",10,0,10);
 hitsPassingCutsValFPix = new TH1F("hitsPassingCutsValFPix","hitsPassingCutsValFPix",10,0,10);
 hitsPassingCutsMisFPix = new TH1F("hitsPassingCutsMisFPix","hitsPassingCutsMisFPix",10,0,10);
 xPosFracVal = new TH2F("xPosFracVal" ,"xPosFracVal" ,101,0,1.01,6,0,6);
 xPosFracMis = new TH2F("xPosFracMis" ,"xPosFracMis" ,101,0,1.01,6,0,6);
 yPosFracVal = new TH2F("yPosFracVal" ,"yPosFracVal" ,101,0,1.01,6,0,6);
 yPosFracMis = new TH2F("yPosFracMis" ,"yPosFracMis" ,101,0,1.01,6,0,6);

 validChiSquare   = new TH1F("validChiSquare","validChiSquare",200, 0., 100.);
 missingChiSquare = new TH1F("missingChiSquare","missingChiSquare",200, 0., 100.);;
 validChiSquareNdf   = new TH1F("validChiSquareNdf","validChiSquareNdf",100, 0., 30.);
 missingChiSquareNdf = new TH1F("missingChiSquareNdf","missingChiSquareNdf",100, 0., 30.);;

 missPerTrackVsChiSquareNdf = new TH2F("missPerTrackVsChiSquareNdf","missPerTrackVsChiSquareNdf", 200,0.,100., 5,0,4);
 missPerTrackPercentVsChiSquareNdf = new TH2F("missPerTrackPercentVsChiSquareNdf","missPerTrackPercentVsChiSquareNdf", 200,0.,100.,100,0,1.);

 tunningVal = new TH2F("tunningVal" ,"tunningVal" ,50,0,5,40,0,40);
 tunningMis = new TH2F("tunningMis" ,"tunningMis" ,50,0,5,40,0,40);
 tunningEdgeVal = new TH1F("tunningEdgeVal","tunningEdgeVal",50,0,5);
 tunningEdgeMis = new TH1F("tunningEdgeMis","tunningEdgeMis",50,0,5);
 tunningMuonVal = new TH1F("tunningMuonVal","tunningMuonVal",40,0,40);
 tunningMuonMis = new TH1F("tunningMuonMis","tunningMuonMis",40,0,40);

 numTracksVsMuonTime = new TH1F("numTracksVsMuonTime","numTracksVsMuonTime",80,-40,40);
 denTracksVsMuonTime = new TH1F("denTracksVsMuonTime","denTracksVsMuonTime",80,-40,40);
 
 xy_standardDev = new TH2F("xy_standardDev","xy_standardDev",100,0,1.5,300,0,4);
 xy_standardDev_insideModule = new TH2F("xy_standardDev_insideModule","xy_standardDev_insideModule",100,0,1.5,300,0,4);

 validPerTriggerBPix    = new TH2F("validPerTriggerBPix","validPerTriggerBPix",64,-0.5,63.5,64,-0.5,63.5);
 missingPerTriggerBPix  = new TH2F("missingPerTriggerBPix","missingPerTriggerBPix",64,-0.5,63.5,64,-0.5,63.5);
 inactivePerTriggerBPix = new TH2F("inactivePerTriggerBPix","inactivePerTriggerBPix",64,-0.5,63.5,64,-0.5,63.5);
 validPerTriggerFPix    = new TH2F("validPerTriggerFPix","validPerTriggerFPix",64,-0.5,63.5,64,-0.5,63.5);
 missingPerTriggerFPix  = new TH2F("missingPerTriggerFPix","missingPerTriggerFPix",64,-0.5,63.5,64,-0.5,63.5);
 inactivePerTriggerFPix = new TH2F("inactivePerTriggerFPix","inactivePerTriggerFPix",64,-0.5,63.5,64,-0.5,63.5);
 
 validMapLayer1    = new TH2F("validMapLayer1","validMapLayer1",1000,-30.,30.,1000,-3.15,3.15);
 validMapLayer2    = new TH2F("validMapLayer2","validMapLayer2",1000,-30.,30.,1000,-3.15,3.15);
 validMapLayer3    = new TH2F("validMapLayer3","validMapLayer3",1000,-30.,30.,1000,-3.15,3.15);
 missingMapLayer1  = new TH2F("missingMapLayer1","missingMapLayer1",1000,-30.,30.,1000,-3.15,3.15);
 missingMapLayer2  = new TH2F("missingMapLayer2","missingMapLayer2",1000,-30.,30.,1000,-3.15,3.15);
 missingMapLayer3  = new TH2F("missingMapLayer3","missingMapLayer3",1000,-30.,30.,1000,-3.15,3.15);
 inactiveMapLayer1 = new TH2F("inactiveMapLayer1","inactiveMapLayer1",1000,-30.,30.,1000,-3.15,3.15);
 inactiveMapLayer2 = new TH2F("inactiveMapLayer2","inactiveMapLayer2",1000,-30.,30.,1000,-3.15,3.15);
 inactiveMapLayer3 = new TH2F("inactiveMapLayer3","inactiveMapLayer3",1000,-30.,30.,1000,-3.15,3.15);
 
 validPerPixHitOnTrack    = new TH1F("validPerPixHitOnTrack","validPerPixHitOnTrack",8,1,9);
 missingPerPixHitOnTrack  = new TH1F("missingPerPixHitOnTrack","missingPerPixHitOnTrack",8,1,9);
 inactivePerPixHitOnTrack = new TH1F("inactivePerPixHitOnTrack","inactivePerPixHitOnTrack",8,1,9);
 
 tree = new TTree("moduleAnalysis","moduleAnalysis");
 tree->Branch("id",&idTree,"id/I");
 tree->Branch("isModuleBad",&isModuleBadTree,"isModuleBad/I");
 tree->Branch("inactive",&inactiveTree,"inactive/I");
 tree->Branch("missing",&missingTree,"missing/I");
 tree->Branch("valid",&validTree,"valid/I");
 tree->Branch("isBarrelModule",&barrelTree,"isBarrelModule/I");
 tree->Branch("ladder",&ladderTree,"ladder/I");
 tree->Branch("blade",&bladeTree,"blade/I");
 tree->Branch("moduleInLadder",&moduleInLadderTree,"moduleInLadder/I");
 tree->Branch("globalX",&globalXTree,"globalX/D");
 tree->Branch("globalY",&globalYTree,"globalY/D");
 tree->Branch("globalZ",&globalZTree,"globalZ/D");

 
 if(DEBUG) std::cout<<"End of begin job"<<std::endl;
 
 //f = new TFile("checktkcollection.root","RECREATE");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PixelEfficiency::endJob() {
 
  if(DEBUG) std::cout<<"End job"<<std::endl;

  using namespace std;

  fOutputFile->cd();

  histo->Write();  
  histLayer1->Write();  
  histLayer2->Write();  
  histLayer3->Write();  
  histEndcapPlus->Write();
  histEndcapMinus->Write();
  histBarrel->Write();  
  histEndcap->Write();  
  validPerSubdetector->Write();  
  missingPerSubdetector->Write();  
  inactivePerSubdetector->Write();  
  
  histInvalidRecHitCollection->Write();
  histInvalidRecHitWithBadmoduleList->Write();

  consistencyCheck->Write();
  consistencyCheckTraj->Write();
  trackingEfficiency->Write();

  windowSearch->Write();
  windowSearchSameModule->Write();
  windowSearchBPix->Write();
  windowSearchGoodModulesBPix->Write();
  windowSearchFPix->Write();
  missingButClusterOnSameModule->Write();
  missingButCluster->Write();
  
  for (int p=0;p<=ptranges;p++)
    windowSearch_pT[p]->Write();
  
  chargeDistri->Write();
  numbPixInCluster->Write();
  chargeDistriBPix->Write();
  numbPixInClusterBPix->Write();
  chargeDistriFPixPlus->Write();
  numbPixInClusterFPixPlus->Write();
  chargeDistriFPixMinus->Write();
  numbPixInClusterFPixMinus->Write();
  chargeVsDimensionBPix->Write();

  chargeDistriPreCuts->Write();
  numbPixInClusterPreCuts->Write();
  chargeDistriBPixPreCuts->Write();
  numbPixInClusterBPixPreCuts->Write();
  chargeDistriFPixPlusPreCuts->Write();
  numbPixInClusterFPixPlusPreCuts->Write();
  chargeDistriFPixMinusPreCuts->Write();
  numbPixInClusterFPixMinusPreCuts->Write();

  numbPixInClusterX->Write();
  numbPixInClusterY->Write();
  xposCluster->Write();
  yposCluster->Write();
  xposClusterValid->Write();
  yposClusterValid->Write();
  xposClusterMisRecovered->Write();
  yposClusterMisRecovered->Write();

  chargeDistriValid->Write();
  numbPixInClusterValid->Write();
  chargeDistriMisRecovered->Write();
  numbPixInClusterMisRecovered->Write();
  
  validVsAlpha->Write();
  missingVsAlpha->Write();
  validVsCotanAlpha->Write();
  missingVsCotanAlpha->Write();
  validVsBeta->Write();
  missingVsBeta->Write();
  validAlphaBeta->Write();
  missingAlphaBeta->Write();
  validVsAlphaBPix->Write();
  missingVsAlphaBPix->Write();
  validVsBetaBPix->Write();
  missingVsBetaBPix->Write();
  validVsAlphaFPix->Write();
  missingVsAlphaFPix->Write();
  validVsBetaFPix->Write();
  missingVsBetaFPix->Write();

  validVsLocalX->Write();
  missingVsLocalX->Write();
  validVsLocalY->Write();
  missingVsLocalY->Write();
  validVsLocalXBig->Write();
  missingVsLocalXBig->Write();
  validVsLocalXSmall->Write();
  missingVsLocalXSmall->Write();
  
  missingAlphaLocalXBigBPix->Write();
  missingBetaLocalXBigBPix->Write();
  missingAlphaLocalYBPix->Write();
  missingBetaLocalYBPix->Write();
  validAlphaLocalXBigBPix->Write();
  validBetaLocalXBigBPix->Write();
  validAlphaLocalYBPix->Write();
  validBetaLocalYBPix->Write();
  
  missingAlphaLocalXBigFPix->Write();
  missingBetaLocalXBigFPix->Write();
  missingAlphaLocalYFPix->Write();
  missingBetaLocalYFPix->Write();
  validAlphaLocalXBigFPix->Write();
  validBetaLocalXBigFPix->Write();
  validAlphaLocalYFPix->Write();
  validBetaLocalYFPix->Write();
  
  missingVsMuontimePre68094->Write();
  validVsMuontimePre68094->Write();
  missingVsMuontimePost68094->Write();
  validVsMuontimePost68094->Write();

  validMuonTimeVSchargeBPix->Write();
  missingMuonTimeVSchargeBPix->Write();
  validMuonTimeVSchargeFPix->Write();
  missingMuonTimeVSchargeFPix->Write();
     
  validVsPT->Write();
  missingVsPT->Write();
  validVsPT_lowpt->Write();
  missingVsPT_lowpt->Write();
  
  validVsEta->Write();
  validVsPhi->Write();
  missingVsEta->Write();
  missingVsPhi->Write();
  inactiveVsEta->Write();
  inactiveVsPhi->Write();

  validAlphaLocalXBig->Write();
  missingAlphaLocalXBig->Write();
  validAlphaLocalXSmall->Write();
  missingAlphaLocalXSmall->Write();

  validPerRun->LabelsDeflate("X");
  validPerRun->Write();
  invalidPerRun->LabelsDeflate("X");
  invalidPerRun->Write();
  inactivePerRun->LabelsDeflate("X");
  inactivePerRun->Write();
  
  
  numTracksVsRunNumber->LabelsDeflate("X");
  numTracksVsRunNumber->Write();
  denTracksVsRunNumber->LabelsDeflate("X");
  denTracksVsRunNumber->Write();
  muonTimeVSRunNumber->LabelsDeflate("X");
  muonTimeVSRunNumber->Write();
  nMuonTimeVSRunNumber->LabelsDeflate("X");
  nMuonTimeVSRunNumber->Write();
  muonTimeErrorVSRunNumber->LabelsDeflate("X");
  muonTimeErrorVSRunNumber->Write();
  nMuonTimeErrorVSRunNumber->LabelsDeflate("X");
  nMuonTimeErrorVSRunNumber->Write();
  muonTimeErrorDistri->Write();
  validVSMuonTimeError->Write();
  missingVSMuonTimeError->Write();
  muonTimeNdofDistri->Write();
  validVSMuonTimeNdof->Write();
  missingVSMuonTimeNdof->Write();
  
  validPerMinHitsOnTrackBPix->Write();
  validPerMinHitsOnTrackFPix->Write();
  missingPerMinHitsOnTrackBPix->Write();
  missingPerMinHitsOnTrackFPix->Write();
  
  checkoutValidityFlag->Write();
  checkoutTraj->Write();
  //
  totalPerTrack->Write();
  inactivePerTrack->Write();
  missingPerTrack->Write();
  
  hitsPassingCutsValBPix->Write();
  hitsPassingCutsMisBPix->Write();
  hitsPassingCutsValFPix->Write();
  hitsPassingCutsMisFPix->Write();
  xPosFracVal->Write();
  xPosFracMis->Write();
  yPosFracVal->Write();
  yPosFracMis->Write();

  validChiSquare->Write();
  missingChiSquare->Write();
  validChiSquareNdf->Write();
  missingChiSquareNdf->Write();

  missPerTrackVsChiSquareNdf->Write();
  missPerTrackPercentVsChiSquareNdf->Write();
  
  tunningVal->Write();
  tunningMis->Write();
  tunningEdgeVal->Write();
  tunningEdgeMis->Write();
  tunningMuonVal->Write();
  tunningMuonMis->Write();

  //test
  histoMethod2After->Write();
  histoMethod2->Write();  
  histoMethod2AfterFPix->Write();
  histoMethod2FPix->Write();  
  
  numTracksVsMuonTime->Write();
  denTracksVsMuonTime->Write();

  xy_standardDev->Write();
  xy_standardDev_insideModule->Write();
  
  validPerTriggerBPix->Write();
  missingPerTriggerBPix->Write();
  inactivePerTriggerBPix->Write();
  validPerTriggerFPix->Write();
  missingPerTriggerFPix->Write();
  inactivePerTriggerFPix->Write();
  
  validMapLayer1->Write();
  validMapLayer2->Write();
  validMapLayer3->Write();
  missingMapLayer1->Write();
  missingMapLayer2->Write();
  missingMapLayer3->Write();
  inactiveMapLayer1->Write();
  inactiveMapLayer2->Write();
  inactiveMapLayer3->Write();
  
  
  validPerPixHitOnTrack->Write();
  missingPerPixHitOnTrack->Write();
  inactivePerPixHitOnTrack->Write();
  
  
    isModuleBadTree=1;
    for (unsigned int l=0; l<badModuleMap.size(); l++)
      {
      idTree=(badModuleMap[l])[0];inactiveTree=(badModuleMap[l])[1]; missingTree=(badModuleMap[l])[2]; validTree=(badModuleMap[l])[3];
      barrelTree=(badModuleMap[l])[4] ;
      tree->Fill();
      }
    isModuleBadTree=0;
    //validTree=-10;
    for (unsigned int l=0; l<goodModuleMap.size(); l++)
      {
      idTree=int((goodModuleMap[l])[0]);
      inactiveTree=int((goodModuleMap[l])[1]);
      missingTree=int((goodModuleMap[l])[2]);
      validTree=int((goodModuleMap[l])[3]);
      
      barrelTree=int((goodModuleMap[l])[4]);
      ladderTree=int((goodModuleMap[l])[5]);
      bladeTree=int((goodModuleMap[l])[6]);
      moduleInLadderTree=int((goodModuleMap[l])[7]);
      
      globalXTree=(goodModuleMap[l])[8];
      globalYTree=(goodModuleMap[l])[9];
      globalZTree=(goodModuleMap[l])[10];
      tree->Fill();
      }
    tree->Write();
   
  fOutputFile->Write() ;
  fOutputFile->Close() ;

}

//define this as a plug-in
//DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(PixelEfficiency);
