// -*- C++ -*-dz%
//
// Package:  Pixel
// Class:    Pixel
//
// my/Pixel/src/Pixel.cc
//
// Pixel (and strip) triplet residuals
//
// Author: Valere Lambert, UZH 2015
//
// Based off of original Triplet Author:  Daniel Pitzl, DESY
//         Created:  Sat Feb 12 12:12:42 CET 2011
// 
//
// system include files:
#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>
// ROOT:
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TFile.h"
#include "TTree.h"
// CMS and user include files:
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/EventSetup.h>
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <DataFormats/BeamSpot/interface/BeamSpot.h>

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <DataFormats/TrackReco/interface/HitPattern.h>

#include <MagneticField/Engine/interface/MagneticField.h>
// To convert detId to subdet/layer number:
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
//#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
//#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
//#include "DataFormats/SiStripDetId/interface/TECDetId.h"
//#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h" //GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/Topology.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"

#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"


#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajectoryBuilder.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryFitter.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include <TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "DataFormats/TrackerRecHit2D/interface/TkCloner.h"

#include "TrackingTools/DetLayers/interface/DetLayer.h"
//#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleRcd.h"

// Helix script from Morris Schwartz
#include "SimpleHelix.h"

// Flag for new tracking rechis, has to be ON for pre7 and later   
#define NEW_TRACKINGRECHITS  // For V71X_pre7 and later 

struct Histos{
  TH1D *hclusprob_fpix;
  
  TH1D *h420f1, *h421f1, *h420f2, *h421f2, *h420f3, *h421f3;
  TH1D *h077f1, *h078f1, *h079f1, *h069f1;
  TH1D *h077f2, *h078f2, *h079f2, *h069f2;
  TH1D *h077f3, *h078f3, *h079f3, *h069f3;

  TH1D *h420b1, *h421b1, *h420b2, *h421b2, *h420b3, *h421b3;
  TH1D *h077b1, *h078b1, *h079b1, *h069b1;
  TH1D *h077b2, *h078b2, *h079b2, *h069b2;
  TH1D *h077b3, *h078b3, *h079b3, *h069b3;
  void InitBPix(TFileDirectory* fs);
  void InitFPix(TFileDirectory* fs);
};

class Pixel : public edm::EDAnalyzer, public Histos {
public:
  explicit Pixel(const edm::ParameterSet&);
  ~Pixel();

private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void getResiduals(const edm::Event&, const edm::EventSetup&, std::string detTag);
  std::vector<double> getIntersection(std::vector<double> p1, std::vector<double> p2, double rho, const GeomDet *detHit, std::vector<double> intersection);

  edm::InputTag _triggerSrc;
  std::string _ttrhBuilder;
  HLTConfigProvider HLTConfig;
  bool doBPix;
  bool doFPix;

  edm::EDGetTokenT<reco::BeamSpot>  t_offlineBeamSpot_;
  edm::EDGetTokenT<reco::VertexCollection> t_offlinePrimaryVertices_ ;
  edm::EDGetTokenT <reco::TrackCollection>  t_generalTracks_;
  edm::EDGetTokenT< edm::View<reco::PFMET>> t_pfMet_;

  // ----------member data ---------------------------
  std::map<int, Histos> runmap;

  std::string processName_;
  int run_num = -999;
  int lumi_block = -999;

  //Cluster Probability
  int clusSize_X = -999;
  int clusSize_Y = -999;
  float clusProb_FPix = -999;
  
  // Rechit coordinates in local - 3 hits (xpx1_l, xpy1_l), (xpx2_l, xpy2_l), and (xpx3_l, xpy3_l)
  double xpx1_l = -999;
  double xpy1_l = -999;

  double xpx2_l = -999;
  double xpy2_l = -999;
  
  double xpx3_l = -999;
  double xpy3_l = -999;

  // Estimated coordinates in local 
  double xl_ideal_1 = -999;
  double yl_ideal_1 = -999;

  double xl_ideal_2 = -999;
  double yl_ideal_2 = -999;

  double xl_ideal_3 = -999;
  double yl_ideal_3 = -999;

  // Residuals
  double residual_x_1 = -999;
  double residual_x_2 = -999;
  double residual_x_3 = -999;

  double residual_y_1 = -999;
  double residual_y_2 = -999;
  double residual_y_3 = -999;

  // Errors on Rechit coordinates in local
  double x_local_error_1 = -999;
  double y_local_error_1 = -999;

  double x_local_error_2 = -999;
  double y_local_error_2 = -999;

  double x_local_error_3 = -999;
  double y_local_error_3 = -999;

  // Helix parameters
  int Pass = -99;
  int nloops = -99;
  int insideD = -99;
  double radius = -99;
  double xcenter = -99;
  double ycenter = -99;
  double dzdphi = -99;
  double z0 = -99;
  double rho = -99;

};

class myCountersPixel{
   public:
      static int neve;
      static unsigned int prevrun;
};

int myCountersPixel::neve = 0;
unsigned int myCountersPixel::prevrun = 0;


Pixel::Pixel(const edm::ParameterSet& iConfig)
{
  std::cout << "PxlFPix constructed\n";
  _triggerSrc = iConfig.getParameter<edm::InputTag>("triggerSource");
  _ttrhBuilder = iConfig.getParameter<std::string>("ttrhBuilder");
  doBPix=iConfig.getParameter<bool>("doBPix");
  doFPix=iConfig.getParameter<bool>("doFPix");
  //std::cout<<_triggerSrc<<" "<<_triggerSrc.label()<<" "<<_triggerSrc.process()<<" "
  //	   <<_triggerSrc.instance()<<" "<<std::endl;

  t_offlineBeamSpot_ =    consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
  t_offlinePrimaryVertices_ =   consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
  t_generalTracks_= consumes<reco::TrackCollection> (edm::InputTag("generalTracks"));
  t_pfMet_= consumes< edm::View<reco::PFMET>>(edm::InputTag("pfMet"));
  
}
Pixel::~Pixel()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


void Histos::InitFPix(TFileDirectory* fs)
{
  /*         Initialize histograms for FPIX residuals         */
  hclusprob_fpix = fs->make<TH1D>( "hclusprob_fpix", "FPix Cluster Probability;", 120, 0, 1.2 );

  h420f1 = fs->make<TH1D>( "h420f1", "PXB1 residuals #Deltax, p_{t} > 4;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h420f2 = fs->make<TH1D>( "h420f2", "PXF1 residuals #Deltax, p_{t} > 4;PXF1 #Deltax [#mum];hits", 100, -150, 150 );
  h420f3 = fs->make<TH1D>( "h420f3", "PXF2 residuals #Deltax, p_{t} > 4;PXF2 #Deltax [#mum];hits", 100, -150, 150 );

  h421f1 = fs->make<TH1D>( "h421f1", "PXB1 residuals #Deltay, p_{t} > 4;PXB1 #Deltay [#mum];hits", 100, -300, 300 );
  h421f2 = fs->make<TH1D>( "h421f2", "PXF1 residuals #Deltay, p_{t} > 4;PXF1 #Deltay [#mum];hits", 100, -300, 300 );
  h421f3 = fs->make<TH1D>( "h421f3", "PXF2 residuals #Deltay, p_{t} > 4;PXF2 #Deltay [#mum];hits", 100, -300, 300 );

  h077f1 = fs->make<TH1D>( "h077f1", "PXB1 x error ", 100, 0., 100. );
  h077f2 = fs->make<TH1D>( "h077f2", "PXF1 x error ", 100, 0., 100. );
  h077f3 = fs->make<TH1D>( "h077f3", "PXF2 x error ", 100, 0., 100. );

  h078f1 = fs->make<TH1D>( "h078f1", "PXB1 y error ", 100, 0., 100. );
  h078f2 = fs->make<TH1D>( "h078f2", "PXF1 y error ", 100, 0., 100. );
  h078f3 = fs->make<TH1D>( "h078f3", "PXF2 y error ", 100, 0., 100. );

  h079f1 = fs->make<TH1D>( "h079f1", "PXB1 x pull ", 100, -10., 10. );
  h079f2 = fs->make<TH1D>( "h079f2", "PXF1 x pull ", 100, -10., 10. );
  h079f3 = fs->make<TH1D>( "h079f3", "PXF2 x pull ", 100, -10., 10. );

  h069f1 = fs->make<TH1D>( "h069f1", "PXB1 y pull ", 100, -10., 10. );
  h069f2 = fs->make<TH1D>( "h069f2", "PXF1 y pull ", 100, -10., 10. );
  h069f3 = fs->make<TH1D>( "h069f3", "PXF2 y pull ", 100, -10., 10. );

}

void Histos::InitBPix(TFileDirectory* fs)
{
  /*         Initialize histograms for BPIX residuals         */
  
  h420b1 = fs->make<TH1D>( "h420b1", "PXB1 residuals #Deltax, p_{t} > 12;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h420b2 = fs->make<TH1D>( "h420b2", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h420b3 = fs->make<TH1D>( "h420b3", "PXB3 residuals #Deltax, p_{t} > 12;PXB3 #Deltax [#mum];hits", 100, -150, 150 );

  h421b1 = fs->make<TH1D>( "h421b1", "PXB1 residuals #Deltay, p_{t} > 12;PXB1 #Deltay [#mum];hits", 100, -300, 300 );
  h421b2 = fs->make<TH1D>( "h421b2", "PXB2 residuals #Deltay, p_{t} > 12;PXB2 #Deltay [#mum];hits", 100, -300, 300 );
  h421b3 = fs->make<TH1D>( "h421b3", "PXB3 residuals #Deltay, p_{t} > 12;PXB3 #Deltay [#mum];hits", 100, -300, 300 );

  h077b1 = fs->make<TH1D>( "h077b1", "PXB1 x error ", 100, 0., 100. );
  h077b2 = fs->make<TH1D>( "h077b2", "PXB2 x error ", 100, 0., 100. );
  h077b3 = fs->make<TH1D>( "h077b3", "PXB3 x error ", 100, 0., 100. );

  h078b1 = fs->make<TH1D>( "h078b1", "PXB1 y error ", 100, 0., 100. );
  h078b2 = fs->make<TH1D>( "h078b2", "PXB2 y error ", 100, 0., 100. );
  h078b3 = fs->make<TH1D>( "h078b3", "PXB3 y error ", 100, 0., 100. );

  h079b1 = fs->make<TH1D>( "h079b1", "PXB1 x pull ", 100, -10., 10. );
  h079b2 = fs->make<TH1D>( "h079b2", "PXB2 x pull ", 100, -10., 10. );
  h079b3 = fs->make<TH1D>( "h079b3", "PXB3 x pull ", 100, -10., 10. );

  h069b1 = fs->make<TH1D>( "h069b1", "PXB1 y pull ", 100, -10., 10. );
  h069b2 = fs->make<TH1D>( "h069b2", "PXB2 y pull ", 100, -10., 10. );
  h069b3 = fs->make<TH1D>( "h069b3", "PXB3 y pull ", 100, -10., 10. );
}

// member functions:
// method called once each job just before starting event loop

void Pixel::beginJob()
{

}

//----------------------------------------------------------------------
// method called for each event:

void Pixel::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup)
{

  const int run = iRun.run();

  std::map<int, Histos>::iterator iter = runmap.find(run);
  if(iter != runmap.end())
    {

      static_cast<Histos&>(*this) = iter->second;
    }
  else
    {
      edm::Service<TFileService> fs;

      std::stringstream runstr;
      runstr << "Run" << run;
      TFileDirectory subdir = fs->mkdir(runstr.str().c_str());
      if(doBPix){
	runmap[run].InitBPix(&subdir);
      }
      if(doFPix){
	runmap[run].InitFPix(&subdir);
      }
      static_cast<Histos&>(*this) = runmap[run];
    }
  
  bool hltSetupChanged = false;
  if(_triggerSrc.label()!="") {
    if(!HLTConfig.init(iRun, iSetup, _triggerSrc.process(), hltSetupChanged))
      throw cms::Exception("Failed to init HLT config");
  }
}

std::vector<double> Pixel::getIntersection(std::vector<double> point1, std::vector<double> point2, double rho,  const GeomDet *detHit, std::vector<double> intersection){
  /* 
         Takes two points and the curvature to create a helix, then finds the intersection with a detector plane
	 returns the (x,y,z) local coordinates for the intersection point
  */


  // Create helix from p1, p2 and rho
  SimpleHelix Hel = SimpleHelix(point1, point2, fabs(rho), &Pass);
  
  // Information about the detector plane
  double x_0 = detHit->position().x();
  double y_0 = detHit->position().y();
  double z_0 = detHit->position().z();

  double nX = detHit->surface().normalVector().x();
  double nY = detHit->surface().normalVector().y();
  double nZ = detHit->surface().normalVector().z();

  std::vector<double> plane = {x_0, y_0, z_0, nX, nY, nZ};

  // Find the intersection of the detector plane and helix
  nloops = Hel.SimpleHelix::pposdir(plane, intersection);

  // Convert global intersection point to local
  Surface::GlobalPoint EstimatedPoint(intersection[0],intersection[1],intersection[2]);
  Surface::LocalPoint LocalEstimate = detHit->toLocal(EstimatedPoint);

  // boolean of if the intersection is within the detector plane bounds - currently not used
  insideD = detHit->surface().bounds().inside(LocalEstimate);

  // Get Helix Parameters - currently not used
  Hel.SimpleHelix::parameters(radius, xcenter, ycenter, dzdphi, z0);
  
  std::vector<double> EstLocalCoordinates = {LocalEstimate.x(), LocalEstimate.y(), LocalEstimate.z()};

  return EstLocalCoordinates;
}



void Pixel::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  /*  Run for either/both FPix or BPix residuals  */

  if(doFPix){
    std::string detTag = "fpix";
    Pixel::getResiduals(iEvent, iSetup, detTag);
  }
  if(doBPix){
    std::string detTag = "bpix";
    Pixel::getResiduals(iEvent, iSetup, detTag);
  }

}

void Pixel::getResiduals(const edm::Event & iEvent, const edm::EventSetup& iSetup, std::string detTag){
  
  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace math;
  
  //const double wt = 180/pi;  // Unused
  //const double pi = 4*atan(1); // Ditto 
  
  myCountersPixel::neve++;
  
  if( myCountersPixel::prevrun != iEvent.run() ){
    time_t unixZeit = iEvent.time().unixTime();
    cout << "new run " << iEvent.run();
    cout << ", LumiBlock " << iEvent.luminosityBlock();
    cout << " taken " << ctime(&unixZeit); // ctime has endline
    myCountersPixel::prevrun = iEvent.run();
  }// new run
  
  int idbg = 0;
  if( myCountersPixel::neve < 2 ) idbg = 1;
  
  int jdbg = 0;
  if( idbg ) {
    cout << endl;
    cout << "run " << iEvent.run();
    cout << ", LumiBlock " << iEvent.luminosityBlock();
    cout << ", event " << iEvent.eventAuxiliary().event();
    time_t unixZeit = iEvent.time().unixTime();
    cout << ", taken " << ctime(&unixZeit); // ctime has endline
  }
  
  
  run_num=iEvent.run();
  lumi_block=iEvent.luminosityBlock();   
  
  //--------------------------------------------------------------------
  // beam spot:
  
  edm::Handle<reco::BeamSpot> rbs;
  //  iEvent.getByLabel( "offlineBeamSpot", rbs );
  iEvent.getByToken( t_offlineBeamSpot_, rbs );

  XYZPoint bsP = XYZPoint(0,0,0);        // beam spot point
  
  if( rbs.failedToGet() ) return;
  if( ! rbs.isValid() ) return;
  bsP = XYZPoint( rbs->x0(), rbs->y0(), rbs->z0() );
  if( idbg ){
    cout << "beam spot x " << rbs->x0();
    cout << ", y " << rbs->y0();
    cout << ", z " << rbs->z0();
    cout << endl;
  }
 
  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoH;
  iSetup.get<TrackerTopologyRcd>().get(tTopoH);
  const TrackerTopology *tTopo=tTopoH.product();

  //--------------------------------------------------------------------
  // primary vertices:
  Handle<VertexCollection> vertices;
  //  iEvent.getByLabel( "offlinePrimaryVertices", vertices );
  iEvent.getByToken( t_offlinePrimaryVertices_,vertices );
  if( vertices.failedToGet() ) return;
  if( !vertices.isValid() ) return;

  // need vertex global point for tracks
  // from #include "DataFormats/GeometryVector/interface/GlobalPoint.h"
  // Global points are three-dimensional by default
  // typedef Global3DPoint  GlobalPoint;
  // typedef Point3DBase< float, GlobalTag> Global3DPoint;
  
  //XYZPoint vtxN = XYZPoint(0,0,0);    // Not sure of the purpose for vtxN
  XYZPoint vtxP = XYZPoint(0,0,0);    //
  Vertex bestPvx;
  
  
  //--------------------------------------------------------------------
  // MET:
  
  edm::Handle< edm::View<reco::PFMET> > pfMEThandle;
  //  iEvent.getByLabel( "pfMet", pfMEThandle );
  iEvent.getByToken(t_pfMet_, pfMEThandle );
  
  //--------------------------------------------------------------------
  // get a fitter to refit TrackCandidates, the same fitter as used in standard reconstruction:
  // Fitter = cms.string('KFFittingSmootherWithOutliersRejectionAndRK'),
  // KalmanFilter
  // RungeKutta
  
  
#ifdef NEW_TRACKINGRECHITS

  // Fitter                                                                                                                                                                         
  edm::ESHandle<TrajectoryFitter> aFitter;
  iSetup.get<TrajectoryFitter::Record>().get("KFFittingSmootherWithOutliersRejectionAndRK",aFitter);
  std::unique_ptr<TrajectoryFitter> theFitter = aFitter->clone();         // pointer which destroys object when pointer out of scope
  
  //----------------------------------------------------------------------------                                                                                                                        
  // Transient Rechit Builders                                                                                                                                                                          
  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", theB );
  
  // Transient rec hits:                                                                                                                                                                                
  ESHandle<TransientTrackingRecHitBuilder> hitBuilder;
  iSetup.get<TransientRecHitRecord>().get( _ttrhBuilder, hitBuilder );
  
  // Cloner, New from 71Xpre7                                                                                                                                                                           
  const TkTransientTrackingRecHitBuilder * builder = static_cast<TkTransientTrackingRecHitBuilder const *>(hitBuilder.product());
  auto hitCloner = builder->cloner();
  theFitter->setHitCloner(&hitCloner);
  
#else
  //old
  ESHandle<TrajectoryFitter> TF;
  iSetup.get<TrajectoryFitter::Record>().get( "KFFittingSmootherWithOutliersRejectionAndRK", TF );
  std::unique_ptr<TrajectoryFitter> theFitter = TF->clone();
  
  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", theB );
  
  // transient rec hits:                                                                                                                                                                                
  ESHandle<TransientTrackingRecHitBuilder> hitBuilder;
  iSetup.get<TransientRecHitRecord>().get( _ttrhBuilder, hitBuilder );
  
#endif
  //-------------------------------------------------------------------- 
  // Trackpropagator:
  edm::ESHandle<Propagator> prop;
  iSetup.get<TrackingComponentsRecord>().get( "PropagatorWithMaterial", prop );
  const Propagator* thePropagator = prop.product();
  
  //--------------------------------------------------------------------
  // tracks:
  Handle<TrackCollection> tracks;
  //  iEvent.getByLabel( "generalTracks", tracks );
  iEvent.getByToken( t_generalTracks_, tracks );
  if( tracks.failedToGet() ) return;
  if( !tracks.isValid() ) return;
  if( idbg ){
    cout << "  tracks " << tracks->size();
    cout << endl;
  }

  //----------------------------------------------------------------------------
  // get tracker geometry:
  edm::ESHandle<TrackerGeometry> pTG;
  iSetup.get<TrackerDigiGeometryRecord>().get( pTG );
  if( ! pTG.isValid() ) {
    cout << "Unable to find TrackerDigiGeometry. Return\n";
    return;
  }
  
  //----------------------------------------------------------------------------
  // transient track builder, needs B-field from data base (global tag in .py)
  
  
#if 0
  edm::ESHandle<SiPixelLorentzAngle> SiPixelLorentzAngle_; 
  iSetup.get<SiPixelLorentzAngleRcd>().get(SiPixelLorentzAngle_);
  std::map<unsigned int,float> detid_la= SiPixelLorentzAngle_->getLorentzAngles();  // get Lorentz angles for subdetectors
  std::map<unsigned int,float>::const_iterator it;
  for (it=detid_la.begin();it!=detid_la.end();it++)
    {
      unsigned int subdet   = DetId(it->first).subdetId();
      if(subdet == static_cast<int>(PixelSubdetector::PixelBarrel))   
	//std::cout << it->first << ": " << it->second << std::endl;
    }
#endif
  
  //----------------------------------------------------------------------------
  //                           Loop over tracks
  //----------------------------------------------------------------------------   

  double sumpt = 0;     // total pt of tracks from vtx
  double sumq = 0;      // total charge from vtx
  Surface::GlobalPoint origin = Surface::GlobalPoint(0,0,0);
  for( TrackCollection::const_iterator iTrack = tracks->begin();
       iTrack != tracks->end(); ++iTrack ) {
    // cpt = cqRB = 0.3*R[m]*B[T] = 1.14*R[m] for B=3.8T
    // D = 2R = 2*pt/1.14
    // calo: D = 1.3 m => pt = 0.74 GeV/c
    double pt = iTrack->pt();
    if( abs( iTrack->dxy(vtxP) ) > 5*iTrack->dxyError() ) continue; // if trans. IP > 5x its error, skip
    sumpt += pt;
    sumq += iTrack->charge();
    
    //const reco::HitPattern& hp = iTrack->hitPattern();
    //double phi = iTrack->phi(); // Not used
    //double eta = iTrack->eta(); // ditto 
    //if( idbg ) {
    //  cout << endl;
    //  cout << "Track "  << distance( tracks->begin(), iTrack );
    //  cout << ": pt "   << iTrack->pt();
    //  cout << ", eta "  << eta;
    //  cout << ", phi "  << phi*wt;
    //  cout << setprecision(1);
    //  cout << ", dxyv " << iTrack->dxy(vtxP)*1E4 << " um";
    //  cout << ", dzv "  << iTrack->dz(vtxP)*1E1 << " mm";
    //  cout << setprecision(4);
    //  cout << ", hits " << hp.numberOfHits(HitPattern::TRACK_HITS); 
    //  cout << ", valid "<< hp.numberOfValidTrackerHits();
    //  cout << endl;
    //}
    
    // transient track:    
    TransientTrack tTrack = theB->build(*iTrack);
    TrajectoryStateOnSurface initialTSOS = tTrack.innermostMeasurementState();
    double kap = tTrack.initialFreeState().transverseCurvature();                          // curvature of track
    rho = 1/kap;  
    
  
    //--------------------------------------------------------------------------
    //   Get the Pixel Hits from the track for the triplet
    //--------------------------------------------------------------------------   

    // rec hits from track extra:
    if( iTrack->extra().isNull() ) continue;//next track
    if( ! iTrack->extra().isAvailable() ) continue;//next track

    uint32_t innerDetId = 0;
    double xPX1 = 0;      // global x hit 1
    double yPX1 = 0;      // global y hit 1
    double zPX1 = 0;      // global z hit 1
    double ePX1 = 0;      // sqrt(covariance of x)
    double fPX1 = 0;      // sqrt(covariance of y)
    
    double xPX2 = 0;
    double yPX2 = 0;
    double zPX2 = 0;
    double ePX2 = 0;
    double fPX2 = 0;
    
    double xPX3 = 0;
    double yPX3 = 0;
    double zPX3 = 0;
    double ePX3 = 0;
    double fPX3 = 0;

    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    const GeomDet * det1 = NULL;  // Detector for first hit
    const GeomDet * det2 = NULL;
    const GeomDet * det3 = NULL;
    
    edm::OwnVector<TrackingRecHit> recHitVector;                     // for seed
    std::vector<TransientTrackingRecHit::RecHitPointer> myTTRHvec;
    Trajectory::RecHitContainer coTTRHvec;                           // for fit, constant
    
    // loop over recHits on this track:

    for( trackingRecHit_iterator irecHit = iTrack->recHitsBegin();
	 irecHit != iTrack->recHitsEnd(); ++irecHit ) {
      DetId detId = (*irecHit)->geographicalId();                          // get detector 
      uint32_t subDet = detId.subdetId();                                  // get subdetector
      // enum Detector { Tracker=1, Muon=2, Ecal=3, Hcal=4, Calo=5 };
      if( detId.det() != 1 ){
	cout << "rec hit ID = " << detId.det() << " not in tracker!?!?\n";
	continue;
      }
      
      recHitVector.push_back( (*irecHit)->clone() );
            
      // build transient hit: 
      
#ifdef NEW_TRACKINGRECHITS
      auto tmprh = (*irecHit)->cloneForFit(*builder->geometry()->idToDet((**irecHit).geographicalId()));
      auto transRecHit = hitCloner.makeShared(tmprh, initialTSOS);
      
#else
      TransientTrackingRecHit::RecHitPointer transRecHit = hitBuilder->build( &*(*irecHit), initialTSOS);
#endif	 
      
      myTTRHvec.push_back( transRecHit );
      coTTRHvec.push_back( transRecHit );
      
      if( ! (*irecHit)->isValid() ) continue;
      
      double xloc = transRecHit->localPosition().x();       // 1st meas coord
      double yloc = transRecHit->localPosition().y();       // 2nd meas coord or zero
      double vxloc = transRecHit->localPositionError().xx();//covariance
      double vyloc = transRecHit->localPositionError().yy();//covariance
      double gX = transRecHit->globalPosition().x();
      double gY = transRecHit->globalPosition().y();
      double gZ = transRecHit->globalPosition().z();
      if( transRecHit->canImproveWithTrack() ) {//use z from track to apply alignment
	//if( idbg ) cout << "  try to improve\n";
	TrajectoryStateOnSurface propTSOS = thePropagator->propagate( initialTSOS, transRecHit->det()->surface() );
	if( propTSOS.isValid() ){

#ifdef NEW_TRACKINGRECHITS
	  auto preciseHit = hitCloner.makeShared(tmprh,propTSOS); //pre7  
#else
	  TransientTrackingRecHit::RecHitPointer preciseHit = transRecHit->clone(propTSOS);   // adding in trajetory with detector effects
#endif
	  //if( idbg ) cout << "  have preciseHit\n";
	  xloc = preciseHit->localPosition().x();// 1st meas coord
	  yloc = preciseHit->localPosition().y();// 2nd meas coord or zero

	  vxloc = preciseHit->localPositionError().xx();//covariance
	  vyloc = preciseHit->localPositionError().yy();//covariance
	  if( idbg ) {
	    //cout << "  improved hit in "   << subDet;
	    //cout << setprecision(4);
	    //cout << ", xloc from "         << transRecHit->localPosition().x();
	    //cout << " to "                 << preciseHit->localPosition().x();
	    //cout << ", yloc from "         << transRecHit->localPosition().y();
	    //cout << " to "                 << preciseHit->localPosition().y();
	    //cout << endl;
	  }
	  
	  gX = preciseHit->globalPosition().x();
	  gY = preciseHit->globalPosition().y();
	  gZ = preciseHit->globalPosition().z();
	  
	}//valid propTSOS
	else{
	  if( idbg ) cout << "  propTSOS not valid\n";
	}
      }//canImprove
      
      // ======================================================================
      //  Fill BPIX points
      // ======================================================================
      if(detTag == "bpix"){
      
	// PXB:      
	if( subDet == PixelSubdetector::PixelBarrel ) {
	  
	  int ilay=tTopo->pxbLayer(detId);
	  
	  if( ilay == 1 ) {
	    
	    n1++;
	    xPX1 = gX;
	    yPX1 = gY;
	    zPX1 = gZ;
	    xpx1_l = xloc;
	    xpy1_l = yloc;
	    ePX1 = sqrt( vxloc );
	    fPX1 = sqrt( vyloc );

	    det1 = transRecHit->det();	    
	  }//PXB1
	  
	  if( ilay == 2){
	    n2++;
	    xPX2 = gX;    // precise hit in CMS global coordinates
	    yPX2 = gY;
	    zPX2 = gZ;
	    xpx2_l = xloc;  // precise hit in local coordinates (w.r.t. sensor center)
	    xpy2_l = yloc;
	    ePX2 = sqrt( vxloc );
	    fPX2 = sqrt( vyloc );
	    
	    det2 = transRecHit->det();
	  
	    
	  }//PXB2
	  
	  if( ilay == 3 ){
	    
	    n3++;
	    xPX3 = gX;
	    yPX3 = gY;
	    zPX3 = gZ;
	    xpx3_l = xloc;
	    xpy3_l = yloc;
	    ePX3 = sqrt( vxloc );
	    fPX3 = sqrt( vyloc );

	    det3 = transRecHit->det();
	    
	  }//PXB3	  
	}// Pixel

      }// doBPIX

      // ==============================================================
      //  Fill FPIX points
      // ============================================================== 
      else if(detTag == "fpix"){
	
	// PXB:      
	if( subDet == PixelSubdetector::PixelBarrel ) {

	  int ilay=tTopo->pxbLayer(detId);
	  
	  if( ilay == 1 ) {
	        
	    n1++;
	    xPX1 = gX;
	    yPX1 = gY;
	    zPX1 = gZ;
	    xpx1_l = xloc;
	    xpy1_l = yloc;	        
  	    ePX1 = sqrt( vxloc );
	    fPX1 = sqrt( vyloc );

	    det1 = transRecHit->det();
	    
	  }// BPIX1
	  
	}//PXB1
    
    
	if( subDet == PixelSubdetector::PixelEndcap) {

	  int idisk=tTopo->pxfDisk(detId); 

	  if( idisk == 1 ){
	    n2++;
	    xPX2 = gX; // precise hit in CMS global coordinates
	    yPX2 = gY;
	    zPX2 = gZ;
	    xpx2_l = xloc; // precise hit in local coordinates (w.r.t. sensor center)
	    xpy2_l = yloc;
	    ePX2 = sqrt( vxloc );
	    fPX2 = sqrt( vyloc );
	        	        
	    det2 = transRecHit->det();
	        
	    const SiPixelRecHit *pixhit = dynamic_cast<const SiPixelRecHit*>( &*(*irecHit) );
	    
	    if( pixhit->hasFilledProb() ){
	      clusProb_FPix = pixhit->clusterProbability(0);                                      
	    }
	   
	    edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const & clust = pixhit->cluster();

	    if( clust.isNonnull() ) {
	      clusSize_Y = clust->sizeY();
	      clusSize_X = clust->sizeX();
	    }
	    

	  }//PXF1
	    
	  if( idisk == 2 ){
	        
	    n3++;
	    xPX3 = gX;
	    yPX3 = gY;
	    zPX3 = gZ;
	    xpx3_l = xloc;
	    xpy3_l = yloc;
	    ePX3 = sqrt( vxloc );
	    fPX3 = sqrt( vyloc );

	    det3 = transRecHit->det();

	  }//PXF2	  
	  
	}//PXF
	
      }//doFPix
      else{
	cout << "detector tag not specified"<< endl;
      }
    
    }//loop rechits

    //------------------------------------------------------------------------
    // refit the track:
    
    PTrajectoryStateOnDet PTraj = trajectoryStateTransform::persistentState( initialTSOS, innerDetId );
    const TrajectorySeed seed( PTraj, recHitVector, alongMomentum );
    
    //if( idbg ) cout << "  have seed\n";

    std::vector<Trajectory> refitTrajectoryCollection = theFitter->fit( seed, coTTRHvec, initialTSOS );
    
    if( refitTrajectoryCollection.size() > 0 ) { // should be either 0 or 1            
      const Trajectory& refitTrajectory = refitTrajectoryCollection.front();
      // Trajectory.measurements:
      Trajectory::DataContainer refitTMs = refitTrajectory.measurements();
      if( idbg ) {
	//cout << "  refitTrajectory has " << refitTMs.size() <<" hits in subdet";
      }

      // hits in subDet:
      
      if( idbg ) {
	for( Trajectory::DataContainer::iterator iTM = refitTMs.begin();
	     iTM != refitTMs.end(); iTM++ ) {
	  
	  TransientTrackingRecHit::ConstRecHitPointer iTTRH = iTM->recHit();
	  if( iTTRH->hit()->isValid() ){
	    //cout << "  " << iTTRH->geographicalId().subdetId();
	  }
	}
	//cout << endl;
	
	//cout << "         pt " << refitTrajectory.geometricalInnermostState().globalMomentum().perp();
	//cout << ", eta " << refitTrajectory.geometricalInnermostState().globalMomentum().eta();
	//cout << ", phi " << refitTrajectory.geometricalInnermostState().globalMomentum().barePhi()*wt;
	//cout << ", at R " << refitTrajectory.geometricalInnermostState().globalPosition().perp();
	//cout << ", z " << refitTrajectory.geometricalInnermostState().globalPosition().z();
	//cout << ", phi " << refitTrajectory.geometricalInnermostState().globalPosition().barePhi()*wt;
	//cout << endl;
	
      }//dbg
      
      // trajectory residuals:
      
      for( Trajectory::DataContainer::iterator iTM = refitTMs.begin(); iTM != refitTMs.end(); iTM++ ) {
	if( ! iTM->recHit()->isValid() ) continue;
	DetId detId = iTM->recHit()->geographicalId();
	uint32_t subDet = detId.subdetId();
	
	// enum SubDetector{ PixelBarrel=1, PixelEndcap=2 };
	// enum SubDetector{ TIB=3, TID=4, TOB=5, TEC=6 };
	
	double xHit = iTM->recHit()->localPosition().x(); // primary measurement direction
	double yHit = iTM->recHit()->localPosition().y(); // always 0 in strips
	
	double dx = xHit - iTM->predictedState().localPosition().x();
	double dy = yHit - iTM->predictedState().localPosition().y();
	//double vxh = iTM->recHit()->localPositionError().xx();//covariance Not used
	//double vxt = iTM->predictedState().localError().positionError().xx();// ditto
	
	//if( subDet == 1 && idbg ){//1=PXB
	//if( subDet == 4 && idbg ){4=TID
	  //cout << "  predictdStateResid = " << dx*1E4 << " um";
	  //cout << ", eh = " << sqrt(vxh)*1E4 << " um";
	  //cout << ", et = " << sqrt(vxt)*1E4 << " um";
	  //cout << endl;
	//}

	TrajectoryStateOnSurface combinedPredictedState =
	  TrajectoryStateCombiner().combine( iTM->forwardPredictedState(), iTM->backwardPredictedState() );
	
	if( ! combinedPredictedState.isValid() ) continue;//skip hit
	
	if( jdbg ) cout << "  have combinedPredictedState\n";
	
	double xptch;
	double yptch;	
	
	if( subDet <  3 ){//1,2=pixel
	  PixelTopology & pixelTopol = (PixelTopology&) iTM->recHit()->detUnit()->topology();
	  xptch = pixelTopol.pitch().first;
	  yptch = pixelTopol.pitch().second;
	}
	else {//strip
	  StripTopology & stripTopol = (StripTopology&) iTM->recHit()->detUnit()->topology();
	  xptch = stripTopol.localPitch( combinedPredictedState.localPosition() );
	  yptch = stripTopol.localStripLength( combinedPredictedState.localPosition() );
	}
      
	dx = xHit - combinedPredictedState.localPosition().x(); //x = primary measurement
	dy = yHit - combinedPredictedState.localPosition().y(); //
	//vxh = iTM->recHit()->localPositionError().xx();//covariance Unused
	//vxt = combinedPredictedState.localError().positionError().xx();// ditto
	
	// angles of incidence:
	// local z = upwards = normal vector
	// local x = primary measurement direction
	// local y = secondary measurement direction
	
	// use Topology. no effect in PXB, essential in TID, TEC
	
	const Topology* theTopology = &(iTM->recHit()->detUnit()->topology() );
	    
	// MeasurementPoint [pitch] (like channel number)
	
	// TODO: Use the measurementPosition(point, trackdir) version of this function in order to take bows into account!
	MeasurementPoint hitMeasurement = theTopology->measurementPosition( iTM->recHit()->localPosition() );
	
	// TID and TEC have trapezoidal detectors:
	// translation from channel number into local x depends on local y
	// track prediction has local x,y => can convert into proper channel number MeasurementPoint:
	
	// TODO: Use the measurementPosition(point, trackdir) version of this function in order to take bows into account!
	MeasurementPoint combinedPredictedMeasurement = theTopology->measurementPosition( combinedPredictedState.localPosition() );

	dx = hitMeasurement.x() - combinedPredictedMeasurement.x(); //in units of pitch  (pitch = size of pixel or strip)
	dy = hitMeasurement.y() - combinedPredictedMeasurement.y(); //in units of pitch
	dx = dx * xptch;//convert back into [cm] using local pitch
	dy = dy * yptch;//[cm]
	
	// use topology: needed for TEC	
	double xx = hitMeasurement.x();
	double yy;
	if( subDet < 3 ) // pixel is 2D
	  yy = hitMeasurement.y();
	else // strips are 1D
	  yy = combinedPredictedMeasurement.y();
	
	MeasurementPoint mp( xx, yy );
	
	//2012 StripTopology & stripTopol = (StripTopology&) iTM->recHit()->detUnit()->topology();
	
	Surface::LocalPoint lp = theTopology->localPosition( mp );
	const GeomDet * myGeomDet = iTM->recHit()->det(); // makes no difference in TEC
	Surface::GlobalPoint gp = myGeomDet->toGlobal( lp );
	
	double gX = gp.x();
	double gY = gp.y();
	double gZ = gp.z();
	
	double lX = lp.x();
	double lY = lp.y();


	//overwrite PXB global coordinates once more, using topology:

	// =================================================
	// Modify BPIX Points with topology refit
	// =================================================
	if(detTag == "bpix"){
	  if( subDet == PixelSubdetector::PixelBarrel ) {
	        
	    int ilay=tTopo->pxbLayer(detId);
 
	    if( ilay == 1 ) {
	      xPX1 = gX;
	      yPX1 = gY;
	      zPX1 = gZ;

	      xpx1_l=lX;
	      xpy1_l=lY;
	      det1 = iTM->recHit()->det();
	    }// layer 1
	    else if( ilay == 2 ) {
	      xPX2 = gX;
	      yPX2 = gY;
	      zPX2 = gZ;
	            
	      xpx2_l=lX;
	      xpy2_l=lY;
	      det2 = iTM->recHit()->det();
	    }// layer 2
	    else if( ilay == 3 ) {
	      xPX3 = gX;
	      yPX3 = gY;
	      zPX3 = gZ;
	            
	      xpx3_l=lX;
	      xpy3_l=lY;
	      det3 = iTM->recHit()->det();
	    }// layer 3
	      
	        
	  }// barrel
	  }// doBPix

	// ================================================
	// Modify FPIX Points with topology refit
	// ================================================
	else if(detTag =="fpix"){
	  if( subDet == PixelSubdetector::PixelBarrel ) {

	    int ilay=tTopo->pxbLayer(detId);

	    if( ilay == 1 ) {
	      xPX1 = gX;
	      yPX1 = gY;
	      zPX1 = gZ;

	      xpx1_l=lX;
	      xpy1_l=lY;
	      det1 = iTM->recHit()->det();
	    }// layer 1
	  }// barrel	  
	  else if( subDet == PixelSubdetector::PixelEndcap) {
	    
	    int idisk=tTopo->pxfDisk(detId);

	    if( idisk == 1 ) {
	      xPX2 = gX;
	      yPX2 = gY;
	      zPX2 = gZ;
	      
	      xpx2_l=lX;
	      xpy2_l=lY;
	      det2 = iTM->recHit()->det();
	    }// disk 1
	    else if( idisk == 2 ) {
	      xPX3 = gX;
	      yPX3 = gY;
	      zPX3 = gZ;
	      
	      xpx3_l=lX;
	      xpy3_l=lY;
	      det3 = iTM->recHit()->det();
	    }// disk 2
	    
	  }// endcaps
	}// doFPix
	else {
	  cout << "detTag not specified"<<endl;
	}
	
      }//loop iTM
      
    }//refitted trajectory

    //------------------------------------------------------------------------
    // 1-2-3 pixel triplet:
    
    if( n1*n2*n3 > 0 ) {

      {// let's open a scope, so we can redefine the variables further down
	
	// create points to feed helix
	std::vector<double> p1 = {xPX1, yPX1, zPX1};
	std::vector<double> p2 = {xPX2, yPX2, zPX2};
	std::vector<double> p3 = {xPX3, yPX3, zPX3};
	
	std::vector<double> intersection1 = {};
	std::vector<double> intersection2 = {};
	std::vector<double> intersection3 = {};
	
	// Create helix from two points and curvature, return the intersection point in local coordinates
	std::vector<double> IntersectionPointLocal_1 = Pixel::getIntersection(p2, p3, rho, det1, intersection1); 
	std::vector<double> IntersectionPointLocal_2 = Pixel::getIntersection(p1, p3, rho, det2, intersection2);
	std::vector<double> IntersectionPointLocal_3 = Pixel::getIntersection(p1, p2, rho, det3, intersection3);
	
	// Intersection point in local coordinates
	xl_ideal_1 = IntersectionPointLocal_1[0];
	yl_ideal_1 = IntersectionPointLocal_1[1];
	
	xl_ideal_2 = IntersectionPointLocal_2[0];
	yl_ideal_2 = IntersectionPointLocal_2[1];
	
	xl_ideal_3 = IntersectionPointLocal_3[0];
	yl_ideal_3 = IntersectionPointLocal_3[1];
	
	// Residuals with rechit and intersection point
	residual_x_1= (xpx1_l - xl_ideal_1)*1E4;
	residual_y_1= (xpy1_l - yl_ideal_1)*1E4;
	
	residual_x_2= (xpx2_l - xl_ideal_2)*1E4;
	residual_y_2= (xpy2_l - yl_ideal_2)*1E4;
	
	residual_x_3= (xpx3_l - xl_ideal_3)*1E4;
	residual_y_3= (xpy3_l - yl_ideal_3)*1E4;
	
	// Local errors for rechit
	x_local_error_1 = ePX1*1E4;
	y_local_error_1 = fPX1*1E4;
	
	x_local_error_2 = ePX2*1E4;
	y_local_error_2 = fPX2*1E4;
	
	x_local_error_3 = ePX3*1E4;
	y_local_error_3 = fPX3*1E4;
      
	// Add errors and pulls
	double pulx1=0.;
	double pulx2=0.;
	double pulx3=0.;
	double puly1=0.;
	double puly2=0.;
	double puly3=0.;
	if(x_local_error_1!=0.0) pulx1 = residual_x_1/x_local_error_1;
	if(x_local_error_2!=0.0) pulx2 = residual_x_2/x_local_error_2;
	if(x_local_error_3!=0.0) pulx3 = residual_x_3/x_local_error_3;

	if(y_local_error_1!=0.0) puly1 = residual_y_1/y_local_error_1;
	if(y_local_error_2!=0.0) puly2 = residual_y_2/y_local_error_2;
	if(y_local_error_3!=0.0) puly3 = residual_y_3/y_local_error_3;

	// Fill Histograms for FPIX
	if(detTag == "fpix"){
	  
	  if(pt>4){
	    
	    hclusprob_fpix ->Fill(clusProb_FPix);

	    h420f1->Fill( residual_x_1 ); 
	    h421f1->Fill( residual_y_1 );
	    
	    h420f2->Fill( residual_x_2 );
	    h421f2->Fill( residual_y_2 );
	    
	    h420f3->Fill( residual_x_3 );
	    h421f3->Fill( residual_y_3 );
	    
	    // Fill errors
	    h077f1->Fill( x_local_error_1 );
	    h078f1->Fill( y_local_error_1 );
	    
	    h077f2->Fill( x_local_error_2 );
	    h078f2->Fill( y_local_error_2 );
	  
	    h077f3->Fill( x_local_error_3 );
	    h078f3->Fill( y_local_error_3 );

	    // Fill pulls
	    h079f1->Fill( pulx1 );
	    h069f1->Fill( puly1 );

	    h079f2->Fill( pulx2 );
	    h069f2->Fill( puly2 );

	    h079f3->Fill( pulx3 );
	    h069f3->Fill( puly3 );

	  }
	}
      
	// Fill Histograms for BPIX
	else if(detTag == "bpix"){
	  
	  if(pt>12){	  
	    
	    h420b1->Fill( residual_x_1 );
	    h421b1->Fill( residual_y_1 );

	    h420b2->Fill( residual_x_2 );
	    h421b2->Fill( residual_y_2 );

	    h420b3->Fill( residual_x_3 );
	    h421b3->Fill( residual_y_3 );

	    // Fill errors
	    h077b1->Fill( x_local_error_1 );
	    h078b1->Fill( y_local_error_1 );

	    h077b2->Fill( x_local_error_2 );
	    h078b2->Fill( y_local_error_2 );

	    h077b3->Fill( x_local_error_3 );
	    h078b3->Fill( y_local_error_3 );

	    // Fill pulls
	    h079b1->Fill( pulx1 );
	    h069b1->Fill( puly1 );

	    h079b2->Fill( pulx2 );
	    h069b2->Fill( puly2 );

	    h079b3->Fill( pulx3 );
	    h069b3->Fill( puly3 );
	  }
	}
	else{}

      }//triplet 
    }// three hits

  }// loop over tracks
 

}//event
//----------------------------------------------------------------------
// method called just after ending the event loop:
//
void Pixel::endJob() {
  
  std::cout << "end of job after " << myCountersPixel::neve << " events.\n";
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(Pixel);

