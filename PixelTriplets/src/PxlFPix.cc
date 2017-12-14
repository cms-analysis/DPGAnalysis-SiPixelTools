// -*- C++ -*-dz
//
// Package:  PxlFPix
// Class:    PxlFPix
//
// my/PxlFPix/src/PxlFPix.cc
//
// Pixel (and strip) triplet residuals
//
// Original Author:  Daniel Pitzl, DESY
//         Created:  Sat Feb 12 12:12:42 CET 2011
// $Id$
//
// Modifed:  Valere Lambert, UZH, 2015
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
//#include "PxlFPix.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/EventSetup.h>
#include "FWCore/Framework/interface/MakerMacros.h"

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
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
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

#include "SimpleHelix.h"

// Flag for new tracking rechis, has to be ON for pre7 and later   
#define NEW_TRACKINGRECHITS  // For V71X_pre7 and later 

struct Histos{

  TH1D *h420, *h421;
  TH1D *h077, *h078, *h079, *h069;
  void Init(TFileDirectory* fs);
};

class PxlFPix : public edm::EDAnalyzer, public Histos {
public:
  explicit PxlFPix(const edm::ParameterSet&);
  ~PxlFPix();

private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  edm::InputTag _triggerSrc;
  std::string _ttrhBuilder;
  HLTConfigProvider HLTConfig;

  edm::EDGetTokenT<reco::BeamSpot>  t_offlineBeamSpot_;
  edm::EDGetTokenT<reco::VertexCollection> t_offlinePrimaryVertices_ ;
  edm::EDGetTokenT <reco::TrackCollection>  t_generalTracks_;
  edm::EDGetTokenT< edm::View<reco::PFMET>> t_pfMet_;

  // ----------member data ---------------------------
  std::map<int, Histos> runmap;

  std::string processName_;
  TTree * outtree;

  double xpx1_g,xpy1_g,xpz1_g;
  double xpx1_l,xpy1_l,xpz1_l; 

  double xpx2_g,xpy2_g,xpz2_g;
  double xpx2_l,xpy2_l,xpz2_l; 
  double xl_ideal, yl_ideal, zl_ideal;
  double xblade;
  double xpanel;
  double xmodule;

  double residual;
  double residual_x = -999;
  double residual_y = -999;
  double residual_z = -999;
  double x_local_error = -999;
  double y_local_error = -999;;

  double resid_disk1 = -999;
  double resid_disk2 = -999;
  double residual_refit = -999;

  double final_pt = -999;
  double final_p = -999;
  double final_phi = -999;
  double final_eta = -999;
  int run_num = -999;
  int lumi_block = -999;

  int Pass = -99;
  int nloops = -99;
  int insideN = -99;
  double radius = -99;
  double xcenter = -99;
  double ycenter = -99;
  double dzdphi = -99;
  double z0 = -99;
  double rho = -99;

};

class myCountersF{
   public:
      static int neve;
      static unsigned int prevrun;
};

int myCountersF::neve = 0;
unsigned int myCountersF::prevrun = 0;


PxlFPix::PxlFPix(const edm::ParameterSet& iConfig)
{
  std::cout << "PxlFPix constructed\n";
  _triggerSrc = iConfig.getParameter<edm::InputTag>("triggerSource");
  _ttrhBuilder = iConfig.getParameter<std::string>("ttrhBuilder");
  //std::cout<<_triggerSrc<<" "<<_triggerSrc.label()<<" "<<_triggerSrc.process()<<" "
  //	   <<_triggerSrc.instance()<<" "<<std::endl;

  t_offlineBeamSpot_ =    consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
  t_offlinePrimaryVertices_ =   consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
  t_generalTracks_= consumes<reco::TrackCollection> (edm::InputTag("generalTracks"));
  t_pfMet_= consumes< edm::View<reco::PFMET>>(edm::InputTag("pfMet"));

}
PxlFPix::~PxlFPix()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}
void Histos::Init(TFileDirectory* fs)
{
  h420 = fs->make<TH1D>( "h420", "PXF1 residuals #Deltax, p_{t} > 2;PXF1 #Deltax [#mum];hits", 100, -150, 150 );
  h421 = fs->make<TH1D>( "h421", "PXF1 residuals #Deltay, p_{t} > 2;PXF1 #Deltay [#mum];hits", 100, -300, 300 );

  h077 = fs->make<TH1D>( "h077", "PXF x error ", 100, 0., 100. );
  h078 = fs->make<TH1D>( "h078", "PXF y error ", 100, 0., 100. );
  h079 = fs->make<TH1D>( "h079", "PXF x pull ", 100, -10., 10. );
  h069 = fs->make<TH1D>( "h069", "PXF y pull ", 100, -10., 10. );

}
// member functions:
// method called once each job just before starting event loop

void PxlFPix::beginJob()
{
  edm::Service<TFileService> fs;
  outtree =fs->make<TTree>("outtree","test");
  outtree->Branch("residual",&residual,"residual/D");  
  outtree->Branch("residual_x",&residual_x,"residual_x/D");
  outtree->Branch("residual_y",&residual_y,"residual_y/D");
  outtree->Branch("residual_z",&residual_z,"residual_z/D");
  outtree->Branch("x_local_error",&x_local_error,"x_local_error/D");
  outtree->Branch("y_local_error",&y_local_error,"y_local_error/D");
  outtree->Branch("resid_disk1",&resid_disk1,"resid_disk1/D");
  outtree->Branch("resid_disk2",&resid_disk2,"resid_disk2/D");
  outtree->Branch("residual_refit",&residual_refit,"residual_refit/D");
  outtree->Branch("final_pt",&final_pt,"final_pt/D");
  outtree->Branch("final_p",&final_p,"final_p/D");
  outtree->Branch("final_phi",&final_phi,"final_phi/D");
  outtree->Branch("final_eta",&final_eta,"final_eta/D");
  outtree->Branch("run_num",&run_num,"run_num/I");
  outtree->Branch("lumi_block",&lumi_block,"lumi_block/I");
  outtree->Branch("xpx1_g",&xpx1_g,"xpx1_g/D");
  outtree->Branch("xpy1_g",&xpy1_g,"xpy1_g/D");
  outtree->Branch("xpz1_g",&xpz1_g,"xpz1_g/D");
  outtree->Branch("xpx1_l",&xpx1_l,"xpx1_l/D");
  outtree->Branch("xpy1_l",&xpy1_l,"xpy1_l/D");
  outtree->Branch("xpz1_l",&xpz1_l,"xpz1_l/D");

  outtree->Branch("xl_ideal",&xl_ideal,"xl_ideal/D");
  outtree->Branch("yl_ideal",&yl_ideal,"yl_ideal/D");
  outtree->Branch("zl_ideal",&zl_ideal,"zl_ideal/D");
  outtree->Branch("xpx2_g",&xpx2_g,"xpx2_g/D");
  outtree->Branch("xpy2_g",&xpy2_g,"xpy2_g/D");
  outtree->Branch("xpz2_g",&xpz2_g,"xpz2_g/D");
  outtree->Branch("xpx2_l",&xpx2_l,"xpx2_l/D");
  outtree->Branch("xpy2_l",&xpy2_l,"xpy2_l/D");
  outtree->Branch("xpz2_l",&xpz2_l,"xpz2_l/D");
  outtree->Branch("xblade",&xblade,"xblade/D");
  outtree->Branch("xpanel",&xpanel,"xpanel/D");
  outtree->Branch("xmodule",&xmodule,"xmodule/D");

  outtree->Branch("Pass",&Pass,"Pass/I");
  outtree->Branch("nloops",&nloops,"nloops/I");
  outtree->Branch("insideN",&insideN,"insideN/I");

  outtree->Branch("radius",&radius,"radius/D");
  outtree->Branch("xcenter",&xcenter,"xcenter/D");
  outtree->Branch("ycenter",&ycenter,"ycenter/D");
  outtree->Branch("dzdphi",&dzdphi,"dzdphi/D");
  outtree->Branch("z0",&z0,"z0/D");
  outtree->Branch("rho",&rho,"rho/D");
}

//----------------------------------------------------------------------
// method called for each event:

void PxlFPix::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup)
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
      
      runmap[run].Init(&subdir);
      static_cast<Histos&>(*this) = runmap[run];
    }
  
  bool hltSetupChanged = false;
  if(_triggerSrc.label()!="") {
    if(!HLTConfig.init(iRun, iSetup, _triggerSrc.process(), hltSetupChanged))
      throw cms::Exception("Failed to init HLT config");
  }
}

void PxlFPix::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace math;
  
  const double pi = 4*atan(1);
  const double wt = 180/pi;
  const double twopi = 2*pi;
  const double pihalf = 2*atan(1);
  //const double sqrtpihalf = sqrt(pihalf);
  
  myCountersF::neve++;
  
  if( myCountersF::prevrun != iEvent.run() ){
    time_t unixZeit = iEvent.time().unixTime();
    cout << "new run " << iEvent.run();
    cout << ", LumiBlock " << iEvent.luminosityBlock();
    cout << " taken " << ctime(&unixZeit); // ctime has endline
    myCountersF::prevrun = iEvent.run();
  }// new run
  
  int idbg = 0;
  if( myCountersF::neve < 2 ) idbg = 1;
  
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
  //iEvent.getByLabel( "offlineBeamSpot", rbs );
  iEvent.getByToken( t_offlineBeamSpot_, rbs );

  XYZPoint bsP = XYZPoint(0,0,0);        // beam spot point
  //int ibs = 0;                           // boolean for valid beam spot point
  
  if( rbs.failedToGet() ) return;
  if( ! rbs.isValid() ) return;
  //ibs = 1;
  bsP = XYZPoint( rbs->x0(), rbs->y0(), rbs->z0() );
  //double bx = rbs->BeamWidthX();
  //double by = rbs->BeamWidthY();
  if( idbg ){
    cout << "beam spot x " << rbs->x0();
    cout << ", y " << rbs->y0();
    cout << ", z " << rbs->z0();
    cout << endl;
  }
  //--------------------------------------------------------------------
  // primary vertices:
  Handle<VertexCollection> vertices;
  //iEvent.getByLabel( "offlinePrimaryVertices", vertices );
  iEvent.getByToken( t_offlinePrimaryVertices_,vertices );

  if( vertices.failedToGet() ) return;
  if( !vertices.isValid() ) return;
  //int nvertex = vertices->size();

  // need vertex global point for tracks
  // from #include "DataFormats/GeometryVector/interface/GlobalPoint.h"
  // Global points are three-dimensional by default
  // typedef Global3DPoint  GlobalPoint;
  // typedef Point3DBase< float, GlobalTag> Global3DPoint;
  
  //XYZPoint vtxN = XYZPoint(0,0,0);    // Not sure of the purpose for vtxN
  XYZPoint vtxP = XYZPoint(0,0,0);    //
  
  //double bestNdof = 0;
  //double maxSumPt = 0;
  Vertex bestPvx;
  
  //double xBS = 0;                     
  //double yBS = 0;
  // If valid beam spot set beam spot to point
  // if( ibs ) {                        
  //   xBS = bsP.x();
  //   yBS = bsP.y();
  // }
  // // otherwise set the beam spot to (0,0)
  // else {
  //   xBS = vtxP.x();
  //   yBS = vtxP.y();
  // }
  
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
  std::unique_ptr<TrajectoryFitter> theFitter = aFitter->clone();               // pointer which destroys object when pointer out of scope
  
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
  //iEvent.getByLabel( "generalTracks", tracks );
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
  
  // loop over tracker detectors:
#if 0
  for( TrackerGeometry::DetContainer::const_iterator idet = pTG->dets().begin(); idet != pTG->dets().end(); ++idet ) {
        
    // Just to test
    DetId mydetId = (*idet)->geographicalId();
    uint32_t mysubDet = mydetId.subdetId();
    //if( myCountersF::neve == 1 ){
    if( myCountersF::neve == 0 ){
      //cout << "Det " << mydetId.det();
      //cout << ", subDet " << mydetId.subdetId();
      if( mysubDet == PixelSubdetector::PixelBarrel ) {
	/*cout << ": PXB layer " << PXBDetId(mydetId).layer();
	cout << ", ladder "    << PXBDetId(mydetId).ladder();
	cout << ", module "    << PXBDetId(mydetId).module();
	cout << ", at R "      << (*idet)->position().perp();
	cout << ", F "         << (*idet)->position().barePhi()*wt;
	cout << ", z "         << (*idet)->position().z();
	cout << endl;
	cout << "rot x";       // rotation about x axis
	cout << "\t"           << (*idet)->rotation().xx();
	cout << "\t"           << (*idet)->rotation().xy();
	cout << "\t"           << (*idet)->rotation().xz();
	cout << endl;
	cout << "rot y";       // rotation about y axis
	cout << "\t"           << (*idet)->rotation().yx();
	cout << "\t"           << (*idet)->rotation().yy();
	cout << "\t"           << (*idet)->rotation().yz();
	cout << endl;
	cout << "rot z";       // rotation about z axis
	cout << "\t"           << (*idet)->rotation().zx();
	cout << "\t"           << (*idet)->rotation().zy();
	cout << "\t"           << (*idet)->rotation().zz();
	cout << endl;
	*/
	// normal vector: includes alignment (varies from module to module along z on one ladder)
	// neighbouring ladders alternate with inward/outward orientation
	/*
	cout << "normal";
	cout << ": x " << (*idet)->surface().normalVector().x();
	cout << ", y " << (*idet)->surface().normalVector().y();
	cout << ", z " << (*idet)->surface().normalVector().z();
	cout << ", f " << (*idet)->surface().normalVector().barePhi()*wt;
	cout << endl;
	*/
      }//PXB
      
      if( mysubDet == PixelSubdetector::PixelEndcap ) {
	/*
	cout << ": PXD side "   << PXFDetId(mydetId).side();
	cout << ", disk "       << PXFDetId(mydetId).disk();
	cout << ", blade "      << PXFDetId(mydetId).blade();
	cout << ", panel "      << PXFDetId(mydetId).panel();
	cout << ", module "     << PXFDetId(mydetId).module();
	cout << ", at R "       << (*idet)->position().perp();
	cout << ", F "          << (*idet)->position().barePhi()*wt;
	cout << ", z "          << (*idet)->position().z();
	cout << endl;
	cout << "rot x";
	cout << "\t"            << (*idet)->rotation().xx();
	cout << "\t"            << (*idet)->rotation().xy();
	cout << "\t"            << (*idet)->rotation().xz();
	cout << endl;
	cout << "rot y";
	cout << "\t"            << (*idet)->rotation().yx();
	cout << "\t"            << (*idet)->rotation().yy();
	cout << "\t"            << (*idet)->rotation().yz();
	cout << endl;
	cout << "rot z";
	cout << "\t"            << (*idet)->rotation().zx();
	cout << "\t"            << (*idet)->rotation().zy();
	cout << "\t"            << (*idet)->rotation().zz();
	cout << endl;
	cout << "normal";
	cout << ": x "          << (*idet)->surface().normalVector().x();
	cout << ", y "          << (*idet)->surface().normalVector().y();
	cout << ", z "          << (*idet)->surface().normalVector().z();
	cout << ", f "          << (*idet)->surface().normalVector().barePhi()*wt;
	cout << endl;
	*/
      }//PXD
    }//idbg
  }//idet
#endif
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
  // Tracks:
  
  double sumpt = 0;     // total pt of tracks from vtx
  double sumq = 0;      // total charge from vtx
  Surface::GlobalPoint origin = Surface::GlobalPoint(0,0,0);
  for( TrackCollection::const_iterator iTrack = tracks->begin();
       iTrack != tracks->end(); ++iTrack ) {
    // cpt = cqRB = 0.3*R[m]*B[T] = 1.14*R[m] for B=3.8T
    // D = 2R = 2*pt/1.14
    // calo: D = 1.3 m => pt = 0.74 GeV/c
    double pt = iTrack->pt();
    double pp = iTrack->p();
    //if( pt < 0.75 ) continue;// curls up
    //if( pt < 1.75 ) continue;// want sharper image
    if( abs( iTrack->dxy(vtxP) ) > 5*iTrack->dxyError() ) continue; // if trans. IP > 5x its error, skip
    sumpt += pt;
    sumq += iTrack->charge();
    
    //double logpt = log(pt) / log(10);
    
    const reco::HitPattern& hp = iTrack->hitPattern();
    
    if( idbg ) {
      cout << endl;
      cout << "Track "  << distance( tracks->begin(), iTrack );
      cout << ": pt "   << iTrack->pt();
      cout << ", eta "  << iTrack->eta();
      cout << ", phi "  << iTrack->phi()*wt;
      cout << setprecision(1);
      cout << ", dxyv " << iTrack->dxy(vtxP)*1E4 << " um";
      cout << ", dzv "  << iTrack->dz(vtxP)*1E1 << " mm";
      cout << setprecision(4);
      cout << ", hits " << hp.numberOfAllHits(HitPattern::TRACK_HITS); 
      cout << ", valid "<< hp.numberOfValidTrackerHits();
      cout << endl;
    }
    
    double phi = iTrack->phi();
    double eta = iTrack->eta();
    double dca = iTrack->d0(); // w.r.t. origin                               
    //double dca = -iTrack->dxy(); // dxy = -d0                               
    double dip = iTrack->lambda();
    double z0  = iTrack->dz();
    //double tet = pihalf - dip;

    // beam line at z of track, taking beam tilt into account
    //double zBeam = iTrack->dz(bsP);                       // z0p of track along beam line w.r.t. beam z center
    //double xBeam = rbs->x0() + rbs->dxdz() * zBeam;       // beam at z of track
    //double yBeam = rbs->y0() + rbs->dydz() * zBeam;
    //double z0p =  zBeam + bsP.z();                        // z0p of track along beam line w.r.t. CMS z = 0
    //XYZPoint blP = XYZPoint( xBeam, yBeam, z0p );         // point on beam line at z of track
    
    // beam spot position from track
    //xBS = xBeam;  // improve
    //yBS = yBeam;  //beam tilt taken into account
    
    //double bcap = iTrack->dxy(blP);          //impact parameter to beam
    //double edca = iTrack->dxyError();
    //double ebca = sqrt( edca*edca + bx*by ); //round beam
    //double sbca = bcap / ebca;               //impact parameter significance
    
    // transient track:    
    TransientTrack tTrack = theB->build(*iTrack);
    TrajectoryStateOnSurface initialTSOS = tTrack.innermostMeasurementState();
    double kap = tTrack.initialFreeState().transverseCurvature();                          // curvature of track
    //TrajectoryStateClosestToPoint tscp = tTrack.trajectoryStateClosestToPoint( origin );
    
    double cf = cos(phi);
    double sf = sin(phi);
    //double xdca =  dca * sf;
    //double ydca = -dca * cf;
    
    //double tt = tan(tet);
    
    double rinv = -kap; // Karimaki                                           
    //double rho = 1/kap;
    rho = 1/kap;  
    double erd = 1.0 - kap*dca;
    double drd = dca * ( 0.5*kap*dca - 1.0 ); // 0.5 * kap * dca**2 - dca;
    double hkk = 0.5*kap*kap;
    
    // track w.r.t. beam (cirmov):
    
    //double dp = -xBS*sf + yBS*cf + dca;
    //double dl = -xBS*cf - yBS*sf;
    //double sa = 2*dp + rinv * ( dp*dp + dl*dl );
    //double dcap = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to beam
    //double ud = 1 + rinv*dca;
    //double phip = atan2( -rinv*xBS + ud*sf, rinv*yBS + ud*cf );//direction
    
    // track at R(PXB1), from FUNPHI, FUNLEN:
    double R1 = 4.4; // PXB1
    
    double s1 = 0;
    double fpos1 = phi - pihalf;
    
    if( R1 >= abs(dca) ) {
      // sin(delta phi):
      double sindp = ( 0.5*kap * (R1*R1 + dca*dca) - dca ) / (R1*erd);
      fpos1 = phi + asin(sindp); // phi position at R1

      // sin(alpha):
      double sina = R1*kap * sqrt( 1.0 - sindp*sindp );

      // s = alpha / kappa:
      if( sina >= 1.0 )
	s1 = pi / kap;
      else{
	if( sina <= -1.0 )
	  s1 = -pi / kap;
	else
	  s1 = asin(sina) / kap;//always positive
      }
      // Check direction: limit to half-turn
      if( hkk * ( R1*R1 - dca*dca ) > erd ) s1 = pi/abs(kap) - s1; // always positive
    }// R1 > dca
    
    if( fpos1 > pi ) fpos1 -= twopi;
    else if( fpos1 < -pi ) fpos1 += twopi;
    
    double zR1 = z0 + s1*tan(dip); // z at R1
    //--------------------------------------------------------------------------
    // loop over tracker detectors:
    double xcrss[99];
    double ycrss[99];
    //double zcrss[99];
    int ncrss = 0;
    for( TrackerGeometry::DetContainer::const_iterator idet = pTG->dets().begin(); idet != pTG->dets().end(); ++idet ) {
      DetId mydetId = (*idet)->geographicalId();
      uint32_t mysubDet = mydetId.subdetId();
      

      // Want first layer in BPix
      if( mysubDet != PixelSubdetector::PixelBarrel ) continue; 
      
      //cout << ": PXB layer " << PXBDetId(mydetId).layer();
      // cout << ", ladder "    << PXBDetId(mydetId).ladder();
      //cout << ", module "    << PXBDetId(mydetId).module();
      //cout << ", at R1 "     << (*idet)->position().perp();
      //cout << ", F "         << (*idet)->position().barePhi()*wt;
      //cout << ", z "         << (*idet)->position().z();
      //cout << endl;
      
      
      if( PXBDetId(mydetId).layer() == 1 ) {
	double dz = zR1 - (*idet)->position().z();
	if( abs(dz) > 4.0 ) continue;
	double df = fpos1 - (*idet)->position().barePhi();//track phi at R1 vs ladder phi
	if( df > pi ) df -= twopi;
	else if( df < -pi ) df += twopi;
	if( abs(df)*wt > 36.0 ) continue;//coarse matching track to ladder
	// normal vector: includes alignment (varies from module to module along z on one ladder)
	// neighbouring ladders alternate with inward/outward orientation
	/*
	  cout << "normal";
	  cout << ": x " << (*idet)->surface().normalVector().x();
	  cout << ", y " << (*idet)->surface().normalVector().y();
	  cout << ", z " << (*idet)->surface().normalVector().z();
	  cout << ", f " << (*idet)->surface().normalVector().barePhi()*wt;
	  cout << endl;
	*/
	
	double phiN = (*idet)->surface().normalVector().barePhi(); //normal vector
	double phidet = phiN - pihalf;                             // orientation of sensor plane in x-y
	double ux = cos(phidet);                                   // vector in sensor plane
	double uy = sin(phidet);
	double x = (*idet)->position().x();
	double y = (*idet)->position().y();
	
	// intersect helix with line: FUNRXY (in FUNPHI) from V. Blobel
	// factor f for intersect point (x + f*ux, y + f*uy)
	
	double a =                                 kap * ( ux*ux + uy*uy ) * 0.5;
	double b =       erd * ( ux*sf - uy*cf ) + kap * ( ux*x + uy*y );
	double c = drd + erd * (  x*sf -  y*cf ) + kap * (  x*x +  y*y ) * 0.5;
	double dis = b*b - 4.0*a*c;
	double f = 0;
	
	if( dis > 0 ) {
	  
	  dis = sqrt(dis);
	  double f1 = 0;
	  double f2 = 0;
	  
	  if( b < 0 ) {
	    f1 = 0.5 * ( dis - b ) / a;
	    f2 = 2.0 * c / ( dis - b );
	  }
	  else{
	    f1 = -0.5 * ( dis + b ) / a;
	    f2 = -2.0 * c / ( dis + b );
	  }
	  
	  f = f1;
	  if( abs(f2) < abs(f1) ) f = f2;
	  
	}//dis
	
	xcrss[ncrss] = x + f*ux;
	ycrss[ncrss] = y + f*uy;
	double r = sqrt( xcrss[ncrss]*xcrss[ncrss] + ycrss[ncrss]*ycrss[ncrss] );
	
	double s = 0;
	
	if( r >= abs(dca) ) {
	  double sindp = ( 0.5*kap * ( r*r + dca*dca ) - dca ) / (r*erd);
	  double sina = r*kap * sqrt( 1.0 - sindp*sindp );
	  if( sina >= 1.0 )
	    s = pi / kap;
	  else{
	    if( sina <= -1.0 )
	      s = -pi / kap;
	    else
	      s = asin(sina) / kap;
	  }
	  if( hkk * ( r*r - dca*dca ) > erd ) s = pi/abs(kap) - s;
	}
	
	//zcrss[ncrss] = z0 + s*tan(dip); // z at r
	
	ncrss++;
	
      }//PXB1
      
    }//idet
  
    //--------------------------------------------------------------------------
    // rec hits from track extra:
    
    if( iTrack->extra().isNull() ) continue;//next track
    if( ! iTrack->extra().isAvailable() ) continue;//next track

    double rmin = 99.9;
    uint32_t innerDetId = 0;
    double xPXB1 = 0;      // global x hit 1
    double yPXB1 = 0;      // global y hit 1
    double zPXB1 = 0;      // global z hit 1
    //double uPXB1 = 0;      // local position x rechit
    //double vPXB1 = 0;      // local position y rechit
    //double ePXB1 = 0;      // sqrt(covariance of x)
    //double fPXB1 = 0;      // sqrt(covariance of y)
    
    double xPXB2 = 0;
    double yPXB2 = 0;
    double zPXB2 = 0;
    double uPXB2 = 0;
    //double vPXB2 = 0;
    double ePXB2 = 0;
    double fPXB2 = 0;
    
    double xPXB3 = 0;
    double yPXB3 = 0;
    double zPXB3 = 0;
    //double uPXB3 = 0;
    //double vPXB3 = 0;
    //double ePXB3 = 0;
    //double fPXB3 = 0;

    //double P3x = 0;
    //double P3y = 0;
    //double P3z = 0;
    

    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    //double phiN1 = 0;
    double phiN2 = 0;
    //double phiN3 = 0;
    //double clch1 = 0;
    //double clch2 = 0;
    //double clch3 = 0;
    //int ncol1 = 0;
    //int ncol2 = 0;
    //int ncol3 = 0;
    //int nrow1 = 0;
    //int nrow2 = 0;
    //int nrow3 = 0;
    //double etaX1 = 0;
    //double etaX2 = 0;
    //double etaX3 = 0;
    //double cogp1 = 0;
    //double cogp2 = 0;
    //double cogp3 = 0;
    double xmid2 = 0;
    double ymid2 = 0;
    const GeomDet * det2 = NULL;
    //int ilad2 = 0;
    //int xmin2 = 0;
    //int xmax2 = 0;
    //int zmin2 = 0;
    //int zmax2 = 0;
    
    
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
      
      int xmin = 0;
      int xmax = 0;
      int ymin = 0;
      int ymax = 0;
      
      //double cogp = 0;
      
      //int icol = 0;
      //int irow = 0;
      int ncol = 0;
      int nrow = 0;
      //double clch = 0;
      
      //bool halfmod = 0;
      
      double Q_f_X = 0.0;//first
      double Q_l_X = 0.0;//last
      double Q_m_X = 0.0;//middle
      //double etaX = 0;
      
      double Q_f_Y = 0.0;//first
      double Q_l_Y = 0.0;//last
      double Q_m_Y = 0.0;//middle
      
      if( (*irecHit)->isValid() ) {
	
	// enum SubDetector{ PixelBarrel=1, PixelEndcap=2 };
	// enum SubDetector{ TIB=3, TID=4, TOB=5, TEC=6 };
	
	//if( idbg ) cout << "  hit in " << subDet << endl;
	
	// cast to SiPixelRecHit:
	// TrackingRecHit -> RecHit2DLocalPos -> BaseSiTrackerRecHit2DLocalPos -> SiPixelRecHit
	
	if( subDet == 1 ){ // PXB  Barrel
	  
	  //int ilay = PXBDetId(detId).layer();
	  //int ilad = PXBDetId(detId).ladder();
	  
	  // if( ilay == 1 ){
	    
	  //   if(      ilad ==  5 ) halfmod = 1;
	  //   else if( ilad ==  6 ) halfmod = 1;
	  //   else if( ilad == 15 ) halfmod = 1;
	  //   else if( ilad == 16 ) halfmod = 1;
	    
	  // }
	  
	  // if( ilay == 2 ){
	    
	  //   if(      ilad ==  8 ) halfmod = 1;
	  //   else if( ilad ==  9 ) halfmod = 1;
	  //   else if( ilad == 24 ) halfmod = 1;
	  //   else if( ilad == 25 ) halfmod = 1;
	    
	  // }
	  
	  // if( ilay == 3 ){
	    
	  //   if( ilad == 11 ) halfmod = 1;
	  //   if( ilad == 12 ) halfmod = 1;
	  //   if( ilad == 33 ) halfmod = 1;
	  //   if( ilad == 34 ) halfmod = 1;
	    
	  // }
	  
	  if( idbg ) {
	    cout << "  layer  " << PXBDetId(detId).layer();
	    cout << ", ladder " << PXBDetId(detId).ladder();
	    cout << ", module " << PXBDetId(detId).module();
		  cout << endl;
	  }
	  
	  const SiPixelRecHit *pixhit = dynamic_cast<const SiPixelRecHit*>( &*(*irecHit) );
	  
	  if( pixhit->hasFilledProb() ){
	    float clusProb = pixhit->clusterProbability(0);
	    
	    if( idbg ) cout << "  cluster prob " << clusProb << endl;
	  }
	  
	  // pixel cluster:
	  // TrackingRecHit -> RecHit2DLocalPos -> BaseSiTrackerRecHit2DLocalPos -> SiPixelRecHit -> SiPixelCluster
	  
	  edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const & clust = pixhit->cluster();
	  
	  if( clust.isNonnull() ) {
	    
	    if( idbg ) {
	      cout << setprecision(0);
	      cout << "  charge "   << clust->charge();
	      cout << setprecision(4);
	      cout << ", cols "     << clust->minPixelCol() << " - " << clust->maxPixelCol(); //0..416 = 8*52
	      cout << " = " << clust->sizeY();
	      cout << ", rows "     << clust->minPixelRow() << " - " << clust->maxPixelRow();//0..159 left and right
	      cout << " = "         << clust->sizeX();
	      cout << endl;
	    }
	    
	    // Fetch the pixels vector from the cluster:
	    const vector<SiPixelCluster::Pixel> & pixelsVec = clust->pixels();
	    
	    // Obtain boundaries in index units:
	    xmin = clust->minPixelRow();
	    xmax = clust->maxPixelRow();
	    ymin = clust->minPixelCol();
	    ymax = clust->maxPixelCol();
	    
	    // cluster matrix:
	    int QQ[9][99];
	    for( int ix = 0; ix < 9; ++ix ){
	      for( int jz = 0; jz < 99; ++jz ){
		QQ[ix][jz] = 0;
	      }
	    }
	    double xsum = 0;
	    double qsum = 0;
	    
	    // loop over the pixels:
	    int isize = pixelsVec.size();
	    for( int i = 0;  i < isize; ++i ) {
	      int ix = pixelsVec[i].x - xmin;
	      int jz = pixelsVec[i].y - ymin;
	      if( ix > 8 ) ix = 8;
	      if( jz > 98 ) jz = 98;
	      QQ[ix][jz] = pixelsVec[i].adc;
	      
	      double pix_adc = pixelsVec[i].adc;
	      qsum += pix_adc;
	      xsum += pix_adc * pixelsVec[i].x;

	      // X projection:
	      if( pixelsVec[i].x == xmin )
		Q_f_X += pix_adc;
	      else{
		if( pixelsVec[i].x == xmax ) 
		  Q_l_X += pix_adc;
		else
		  Q_m_X += pix_adc;
	      }
	      
	      // Y projection:
	      if( pixelsVec[i].y == ymin ) 
		Q_f_Y += pix_adc;
	      else{
		if( pixelsVec[i].y == ymax ) 
		  Q_l_Y += pix_adc;
		else 
		  Q_m_Y += pix_adc;
	      }
	      
	    }//loop over pixels
	    
	    //etaX = ( Q_f_X - Q_l_X ) / ( Q_f_X + Q_l_X + Q_m_X );
	    //cogp = xsum / qsum;
	    //clch = clust->charge();//electrons
	    //icol = clust->minPixelCol();
	    //irow = clust->minPixelRow();
	    ncol = clust->sizeY();
	    nrow = clust->sizeX();
	    
	    if( ncol > 5 && idbg ){
	      cout << setprecision(1);
	      cout.setf(ios::showpoint);
	      cout.setf(ios::uppercase);
	      //cout.setf(ios::scientific);
	      cout.setf(ios::fixed);
	      cout << "  dip "       << setw(5) << dip*wt;
	      cout << setprecision(4);
	      cout << ", layer  "    << PXBDetId(detId).layer();
	      cout << ", ladder "    << PXBDetId(detId).ladder();
	      cout << ", module "    << PXBDetId(detId).module();
	      cout << ", x " << xmin << " - " << xmax;
	      cout << ", z " << ymin << " - " << ymax;
	      cout << endl;
	      for( int ix = 0; ix < min( 9, nrow ); ++ix ){
		cout << "    row " << setw(3) << ix + xmin;
		for( int jz = 0; jz < min( 99, ncol ); ++jz ){
		  cout << setw(6) << QQ[ix][jz] / 100;
		}
		cout << endl;
	      }
	    }//long ncol
	    //
	    //dip -7.E+01, layer  1, ladder 18, module 2, x 148 - 149, z 141 - 148
	    //row 148     0     0     0     0     0     0    75    78
	    //row 149   191   164   111   166    98    96   150     0
	    //
	    //dip 8.E+01, layer  1, ladder 9, module 8, x 21 - 22, z 368 - 375
	    //row  21     0     0     0     0     0     0     0    84
	    //row  22   259   171   113   115   144   106   161     0
	    //
	    //dip -8.E+01, layer  1, ladder 8, module 1, x 8 - 10, z 279 - 290
	    //row   8     0     0     0     0     0     0     0     0     0     0    70    99
	    //row   9     0     0    91   129   131   112    97    97   146   134   107     0
	    //row  10    55   107    56     0     0     0     0     0     0     0     0     0
	    //
	    //dip 8.E+01, layer  1, ladder 2, module 7, x 139 - 140, z 135 - 144
	    //row 139    94   107   173    52     0     0     0     0     0     0
	    //row 140     0     0     0    70   121   128   116   132    99    91
	    //
	    //dip -7.E+01, layer  2, ladder 10, module 2, x 139 - 140, z 408 - 415
	    //row 139    50   397   107   100    71     0     0     0
	    //row 140     0     0     0     0    48   130   246   239
	    
	    
	  }//clust nonNull
	  
	}//PXB
	
      }//valid
      
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
      //double yloc = transRecHit->localPosition().y();       // 2nd meas coord or zero
      double vxloc = transRecHit->localPositionError().xx();//covariance
      //double vyloc = transRecHit->localPositionError().yy();//covariance
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
	  //if( idbg ) cout << "  have propTSOS\n";
	  TransientTrackingRecHit::RecHitPointer preciseHit = transRecHit->clone(propTSOS);   // adding in trajetory with detector effects
#endif
	  //if( idbg ) cout << "  have preciseHit\n";
	  xloc = preciseHit->localPosition().x();// 1st meas coord
	  //yloc = preciseHit->localPosition().y();// 2nd meas coord or zero

	  vxloc = preciseHit->localPositionError().xx();//covariance
	  //vyloc = preciseHit->localPositionError().yy();//covariance
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
      
      double gF = atan2( gY, gX );
      double gR = sqrt( gX*gX + gY*gY );
      
    
      if( gR < rmin ) {
	rmin = gR;
	innerDetId = detId.rawId();
      }
      
      double phiN = transRecHit->det()->surface().normalVector().barePhi();//normal vector
      double xmid = transRecHit->det()->position().x();
      double ymid = transRecHit->det()->position().y();
      
      // PXB:      
      if( subDet == PixelSubdetector::PixelBarrel ) {
	
	//double xpix = fmod( xloc+0.82, 0.01 );// xpix = 0..0.01

	double df = phiN - gF;//normal vector vs position vector: inwards or outwards
	// take care of cut at +180/-180:
	if( df > pi ) df -= twopi;
	else if( df < -pi ) df += twopi;
	
	int ilay = PXBDetId(detId).layer();
	//int ilad = PXBDetId(detId).ladder();
	//int imod = PXBDetId(detId).module();
	
	if( idbg ) {
	  //cout << "  xloc " << xloc;
	  //cout << ", cogp " << cogp;
	  //double cogx = (cogp + 0.5 - 80) * 0.01 - 0.0054;
	  //if( cogp < 79 ) cogx -= 0.01; // big pix
	  //if( cogp > 80 ) cogx += 0.01; // big pix
	  //cout << ", cogx " << cogx;
	  //cout << ", dx = " << cogx - xloc;
	  //cout << endl;
	}
        if( ilay == 1 ) {

          n1++;
          xPXB1 = gX;
          yPXB1 = gY;
          zPXB1 = gZ;
          //uPXB1 = xloc;
          //vPXB1 = yloc;
          //ePXB1 = sqrt( vxloc );
          //fPXB1 = sqrt( vyloc );
          //phiN1 = phiN;
          //clch1 = clch;
          //ncol1 = ncol;
          //nrow1 = nrow;
          //etaX1 = etaX;
          //cogp1 = cogp;

          //if(      ilad ==  5 ) halfmod = 1;
          //else if( ilad ==  6 ) halfmod = 1;
          //else if( ilad == 15 ) halfmod = 1;
          //else if( ilad == 16 ) halfmod = 1;
        }// BPIX1

	// track impact parameter to beam:
	// my crossings:	
	for( int icrss = 0; icrss < ncrss; ++icrss ){
	  double fcrss = atan2( ycrss[icrss], xcrss[icrss] );
	  double df = gF - fcrss;
	  if( df > pi ) df -= twopi;
	  else if( df < -pi ) df += twopi;
	  //double du = gR*df;
	  //double dz = gZ - zcrss[icrss];
	  //std::cout<<"dz  "<<dz<<std::endl;
	  
	  
	}//crss
	
      }//PXB1
    
    
      if( subDet == PixelSubdetector::PixelEndcap) {
	
	int idisk = PXFDetId(detId).disk();
	int iblade = PXFDetId(detId).blade();
	int ipanel =  PXFDetId(detId).panel();
	int imodule =   PXFDetId(detId).module();
	
	if( idisk == 1 ){
	  n2++;
	  xPXB2 = gX; // precise hit in CMS global coordinates
	  yPXB2 = gY;
	  zPXB2 = gZ;
	  uPXB2 = xloc; // precise hit in local coordinates (w.r.t. sensor center)
	  //vPXB2 = yloc;
	  phiN2 = phiN;
	  ePXB2 = sqrt( vxloc );
	  //fPXB2 = sqrt( vyloc );

	  //clch2 = clch; // cluster charge [e]
	  //ncol2 = ncol;
	  //nrow2 = nrow;
	  //etaX2 = etaX;
	  //cogp2 = cogp;
	  xmid2 = xmid; // middle of sensor in global CMS coordinates
	  ymid2 = ymid;
	  //xmin2 = xmin;
	  //xmax2 = xmax;
	  //zmin2 = ymin;
	  //zmax2 = ymax;
	  
	  xpx1_g=xPXB2;
	  xpy1_g=yPXB2;
	  xpz1_g=zPXB2;
	  
	  xblade = iblade;
	  xpanel = ipanel;
	  xmodule = imodule;
	  
	  det2 = transRecHit->det();
	  
	}//PXF1

	if( idisk == 2 ){
	  
	  n3++;
	  xPXB3 = gX;
	  yPXB3 = gY;
	  zPXB3 = gZ;
	  //uPXB3 = xloc;
	  //vPXB3 = yloc;
	  //ePXB3 = sqrt( vxloc );
	  //fPXB3 = sqrt( vyloc );
	  //phiN3 = phiN;
	  //clch3 = clch;
	  //ncol3 = ncol;
	  //nrow3 = nrow;
	  //etaX3 = etaX;
	  //cogp3 = cogp;
	  
	  xpx2_g=xPXB3;
	  xpy2_g=yPXB3;
	  xpz2_g=zPXB3;

	  
	}//PXF2
	
      }//PXF

    }//loop rechits
  
    //if( pt < 0.75 ) continue;// curls up
    //------------------------------------------------------------------------
    // refit the track:
    
    //if( idbg ) cout << "  prepare refit\n";
    
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
	
	//int ilay = 0;
	//if( detId.subdetId() == 1 ){
	//ilay = PXBDetId( detId ).layer();
	//}
	/*
	  if( subDet == 1 && idbg ){//1=PXB
	  cout << "  PXB layer " << ilay << endl;
	  }
	*/
	
	double dx = xHit - iTM->predictedState().localPosition().x();
	double dy = yHit - iTM->predictedState().localPosition().y();
	//double vxh = iTM->recHit()->localPositionError().xx();//covariance
	//double vxt = iTM->predictedState().localError().positionError().xx();//
	
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
	
	//double R = combinedPredictedState.globalPosition().perp();
	//double F = combinedPredictedState.globalPosition().barePhi();
	//double Z = combinedPredictedState.globalPosition().z();
	
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
	//vxh = iTM->recHit()->localPositionError().xx();//covariance
	//vxt = combinedPredictedState.localError().positionError().xx();//
	
	// angles of incidence:
	// local z = upwards = normal vector
	// local x = primary measurement direction
	// local y = secondary measurement direction
	
	double alf_inc = atan2( combinedPredictedState.localDirection().x(), combinedPredictedState.localDirection().z() );  // ~ dX/dZ
	double bet_inc = atan2( combinedPredictedState.localDirection().y(), combinedPredictedState.localDirection().z() );  // ~ dY/dZ
	
	double phiinc = alf_inc;                    
	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;
	if( bet_inc > pihalf ) bet_inc -= pi;
	else if( bet_inc < -pihalf ) bet_inc += pi;

	//if( subDet == 1 && idbg ){//1=PXB
	
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
	
	//if( jdbg ) cout << "  have combinedPredictedMeasurement\n";
	
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
	//2012 Surface::LocalPoint lp = stripTopol.localPosition( mp ); // makes no difference in TEC
	
	//const GeomDet * myGeomDet = pTG->idToDet( detId );
	const GeomDet * myGeomDet = iTM->recHit()->det(); // makes no difference in TEC
	Surface::GlobalPoint gp = myGeomDet->toGlobal( lp );
	
	double gX = gp.x();
	double gY = gp.y();
	double gZ = gp.z();
	
	double lX = lp.x();
	double lY = lp.y();
	double lZ = lp.z();

	//double phiN = iTM->recHit()->det()->surface().normalVector().barePhi();//normal vector
	
	//2012: overwrite PXB global coordinates once more, using topology:

	if( subDet == PixelSubdetector::PixelBarrel ) {
	  
	  int ilay = PXBDetId(detId).layer();
	       
	  if( ilay == 1 ) {
	    xPXB1 = gX;
	    yPXB1 = gY;
	    zPXB1 = gZ;
	  }
	}

	else if( subDet == PixelSubdetector::PixelEndcap) {
	  
	  int idisk = PXFDetId(detId).disk();
	  
	  if( idisk == 1 ) {
	    xPXB2 = gX;
	    yPXB2 = gY;
	    zPXB2 = gZ;
	    
	    xpx1_l=lX;
	    xpy1_l=lY;
	    xpz1_l=lZ;
	    det2 = iTM->recHit()->det();
	    //cout<<"gx="<<gX<<"  gy="<<gY<<"   gz="<<gZ<<endl;
	  }// disk 1

	  else if( idisk == 2 ) {
	    xPXB3 = gX;
	    yPXB3 = gY;
	    zPXB3 = gZ;
	    
	    xpx2_l=lX;
	    xpy2_l=lY;
	    xpz2_l=lZ;
	  }// disk 2
	  

	}// endcaps
	
	
      }//loop iTM
      
    }//refitted trajectory
    
    //if( pt < 0.75 ) continue;// curls up
      
    //------------------------------------------------------------------------
    // refit once more, leaving out hit in 1st Disk
    
    double kap2 = 0;
    bool refit2valid = 0;
    
    if( n2 > 0 ){

      Trajectory::RecHitContainer nyTTRHvec; // for fit
      
      for( vector<TransientTrackingRecHit::RecHitPointer>::iterator iTTRH = myTTRHvec.begin();
	   iTTRH != myTTRHvec.end(); ++iTTRH ) {

	if( idbg == 9 ) {
	  //cout << "  hit " << distance( (vector<TransientTrackingRecHit::RecHitPointer>::iterator)myTTRHvec.begin(), iTTRH );
	  if( (*iTTRH)->hit()->isValid() ){
	    //cout << ": subdet " << (*iTTRH)->hit()->geographicalId().subdetId();
	    //cout << ", weight " << (*iTTRH)->weight(); // default weight is 1
	    //cout << endl;
	  }
	  else cout << " not valid\n";
	}
	
	int iuse = 1;
	if( (*iTTRH)->hit()->isValid() ){
	  if( (*iTTRH)->hit()->geographicalId().subdetId() == PixelSubdetector::PixelEndcap ){ //PXF   
	    if( PXFDetId( (*iTTRH)->hit()->geographicalId() ).disk() == 1 ) iuse = 0; // skip PXF1: unbiased track 
	  }
	}
	if( iuse ) nyTTRHvec.push_back( *iTTRH );
      }//copy
      
      if( idbg ) {
	//cout << "  all hits " << myTTRHvec.size();
	//cout << ", without PXF1 " << nyTTRHvec.size();
	//cout << endl;
      }
      
      // re-fit without PXF1:
      
      std::vector<Trajectory> refitTrajectoryVec2 = theFitter->fit( seed, nyTTRHvec, initialTSOS );
      if( refitTrajectoryVec2.size() > 0 ) { // should be either 0 or 1            
	const Trajectory& refitTrajectory2 = refitTrajectoryVec2.front();
	// Trajectory.measurements:
	Trajectory::DataContainer refitTMvec2 = refitTrajectory2.measurements();
	
	if( idbg ) {
	  //cout << "  refitTrajectory2 has " << refitTMvec2.size() <<" hits in subdet";
	}
	
	for( Trajectory::DataContainer::iterator iTM = refitTMvec2.begin();
	     iTM != refitTMvec2.end(); iTM++ ) {
	  
	  TransientTrackingRecHit::ConstRecHitPointer iTTRH = iTM->recHit();
	  if( iTTRH->hit()->isValid() ){
	    //if( idbg ) cout << "  " << iTTRH->geographicalId().subdetId();
	  }
	}
	if( idbg ) cout << endl;
	
	if( idbg ) {
	  //cout << "  ndof " << refitTrajectory2.ndof();
	  //cout << ", found " << refitTrajectory2.foundHits();
	  //cout << ", missing " << refitTrajectory2.lostHits();
	  //cout << ", chi2 " << refitTrajectory2.chiSquared();
	  //cout << ", /ndof " << refitTrajectory2.chiSquared() / refitTrajectory2.ndof();
	  //cout << endl;
	}
	
	if( idbg ) {
	  //cout << "         pt " << refitTrajectory2.geometricalInnermostState().globalMomentum().perp();
	  //cout << ", eta " << refitTrajectory2.geometricalInnermostState().globalMomentum().eta();
	  //cout << ", phi " << refitTrajectory2.geometricalInnermostState().globalMomentum().barePhi()*wt;
	  //cout << ", at R " << refitTrajectory2.geometricalInnermostState().globalPosition().perp();
	  //cout << ", z " << refitTrajectory2.geometricalInnermostState().globalPosition().z();
	  //cout << ", phi " << refitTrajectory2.geometricalInnermostState().globalPosition().barePhi()*wt;
	  //cout << endl;
	}
	refit2valid = 1;
	kap2 = refitTrajectory2.geometricalInnermostState().transverseCurvature();

      }//refit PXF1
      else 
	if( idbg ) cout << "  no refit\n";
      
    }// n2 > 0
  
    //------------------------------------------------------------------------
    // 1-2-3 pixel triplet:
    
    if( n1*n2*n3 > 0 ) {
      
      {// let's open a scope, so we can redefine the variables further down
	
	if( jdbg ); //cout << "  triplet 1+3 -> 2\n";
	  
	//double f2 = atan2( yPXB2, xPXB2 );//position angle in disk 1
	
	//double ax = xPXB3 - xPXB1;
	//double ay = yPXB3 - yPXB1;
	//double aa = sqrt( ax*ax + ay*ay ); // distance from 1 to 3
	
	//double xmid = 0.5 * ( xPXB1 + xPXB3 );
	//double ymid = 0.5 * ( yPXB1 + yPXB3 );
	//double bx = xPXB2 - xmid;
	//double by = yPXB2 - ymid;
	//double bb = sqrt( bx*bx + by*by ); // from mid point to point 2
	
	// Make track from 2 space points and kappa (cmz98/ftn/csmktr.f)
	// Definition of the Helix :
	
	// x( t ) = X0 + KAPPA^-1 * SIN( PHI0 + t )
	// y( t ) = Y0 - KAPPA^-1 * COS( PHI0 + t )          t > 0
	// z( t ) = Z0 + KAPPA^-1 * TAN( DIP ) * t
	
	// Center of the helix in the xy-projection:
	
	// X0 = + ( DCA - KAPPA^-1 ) * SIN( PHI0 )
	// Y0 = - ( DCA - KAPPA^-1 ) * COS( PHI0 )
	
	// Point 1 must be in the pixel, 3 in the outer disk
	double r1 = sqrt( xPXB1*xPXB1 + yPXB1*yPXB1 );
	//double r2=  sqrt( xPXB2*xPXB2+  yPXB2*yPXB2 );
	double r3 = sqrt( xPXB3*xPXB3 + yPXB3*yPXB3 );
	
	if( r3-r1 < 2.0 ) cout << "warn r1 = " << r1 << ", r3 = " << r3 << endl;
	
	// Calculate the centre of the helix in xy-projection that
	// transverses the two spacepoints. The points with the same
	// distance from the two points are lying on a line.
	// LAMBDA is the distance between the point in the middle of
	// the two spacepoints and the centre of the helix.
	
	// we already have kap and rho = 1/kap
	
	double lam = sqrt( -0.25 + 
			   rho*rho / ( ( xPXB1 - xPXB3 )*( xPXB1 - xPXB3 ) + ( yPXB1 - yPXB3 )*( yPXB1 - yPXB3 ) ) );
	
	// There are two solutions, the sign of kap gives the information
	// which of them is correct:
	
	if( kap > 0 ) lam = -lam;
	
	// ( X0, Y0 ) is the centre of the circle
	// that describes the helix in xy-projection:
	
	double x0 =  0.5*( xPXB1 + xPXB3 ) + lam * ( -yPXB1 + yPXB3 );
	double y0 =  0.5*( yPXB1 + yPXB3 ) + lam * (  xPXB1 - xPXB3 );
	
	// Calculate theta:
	
	double num = ( yPXB3 - y0 ) * ( xPXB1 - x0 ) - ( xPXB3 - x0 ) * ( yPXB1 - y0 );
	double den = ( xPXB1 - x0 ) * ( xPXB3 - x0 ) + ( yPXB1 - y0 ) * ( yPXB3 - y0 );
	double tandip = kap * ( zPXB3 - zPXB1 ) / atan( num / den );
	//double udip = atan(tandip);
	//double utet = pihalf - udip;
	
	// To get phi0 in the right interval one must distinguish
	// two cases with positve and negative kap:
	
	double uphi;
	if( kap > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );
	
	// The distance of the closest approach DCA depends on the sign
	// of kappa:
	
	double udca;
	if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
	else          udca = rho + sqrt( x0*x0 + y0*y0 );
	
	// angle from first hit to dca point:
	
	double dphi = atan( ( ( xPXB1 - x0 ) * y0 - ( yPXB1 - y0 ) * x0 )
			    / ( ( xPXB1 - x0 ) * x0 + ( yPXB1 - y0 ) * y0 ) );
	
	double uz0 = zPXB1 + tandip * dphi * rho;
		
	// interpolate to middle hit:
	// cirmov
	// we already have rinv = -kap
	
	double cosphi = cos(uphi);
	double sinphi = sin(uphi);
	double dp = -xPXB2*sinphi + yPXB2*cosphi + udca;
	double dl = -xPXB2*cosphi - yPXB2*sinphi;
	double sa = 2*dp + rinv * ( dp*dp + dl*dl );
	double dca2 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
	double ud = 1 + rinv*udca;
	double phi2 = atan2( -rinv*xPXB2 + ud*sinphi, rinv*yPXB2 + ud*cosphi );//direction
	
	double phiinc = phi2 - phiN2;//angle of incidence in rphi w.r.t. normal vector
	
	// phiN alternates inward/outward
	// reduce phiinc:
	
	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;
	
	// arc length:
	
	double xx = xPXB2 + dca2 * sin(phi2); // point on track
	double yy = yPXB2 - dca2 * cos(phi2);
	
	double vx = xx - xmid2;//from module center
	double vy = yy - ymid2;
	double vv = sqrt( vx*vx + vy*vy );
	
	double f0 = uphi;//
	double kx = kap*xx;
	double ky = kap*yy;
	double kd = kap*udca;
	
	// Solve track equation for s:
	
	double dx = kx - (kd-1)*sin(f0);
	double dy = ky + (kd-1)*cos(f0);
	double ks = atan2( dx, -dy ) - f0;// turn angle
	
	// Limit to half-turn:
	
	if(      ks >  pi ) ks = ks - twopi;
	else if( ks < -pi ) ks = ks + twopi;
	
	double s = ks*rho; // signed
	double uz2 = uz0 + s*tandip; // track z at R2
	//double dz2 = zPXB2 - uz2;
	
	Surface::GlobalPoint gp2( xx, yy, uz2 );
	Surface::LocalPoint lp2 = det2->toLocal( gp2 );
	
	// local x = phi
	// local y = z in barrel
	// local z = radial in barrel (thickness)
	
	//double xpix = fmod( uPXB2 + 0.82, 0.01 ); // xpix = 0..0.01 reconstructed
	//double xpx2 = fmod( uPXB2 + 0.82, 0.02 ); // xpix = 0..0.02 reconstructed
	//double xpx1 = fmod( uPXB1 + 0.82, 0.01 ); // xpix = 0..0.01 reconstructed 
	//double xpx3 = fmod( uPXB3 + 0.82, 0.01 ); // xpix = 0..0.01 reconstructed

	
	//double dpix = fmod( uPXB2 + dca2 + 0.82, 0.01 ); // dpix = 0..0.01 predicted
	
	double vpix = fmod( vv, 0.01 ); // vpix = 0..0.01 predicted
	if( uPXB2 < 0 ) vpix = -vpix; // vv is unsigned distance from module center
	
	//double lpix = fmod( lp2.x() + 0.82, 0.01 ); // lpix = 0..0.01 predicted
	//double tpix = fmod( lp2.x() + 0.82, 0.02 ); // tpix = 0..0.02 predicted
	
	//double zpix = fmod( lp2.y() + 3.24, 0.015 ); // zpix = 0..0.015 predicted
	//double spix = fmod( lp2.y() + 3.24, 0.03  ); // spix = 0..0.03  predicted
	
	//int smin = zmin2%52; // 0..51 column along z
	//int smax = zmax2%52; // 0..51 column along z
	
	//double cogx = (cogp2 + 0.5 - 80) * 0.01 - 0.0054; // Lorentz shift
	//if( cogp2 < 79 ) cogx -= 0.01; // big pix
	//if( cogp2 > 80 ) cogx += 0.01; // big pix
	
	//double mpix = fmod( cogx + 0.82, 0.01 ); // mpix = 0..0.01 from cluster COG
	//double cogdx = cogx - lp2.x(); // residual
	
	// hybrid method:
	
	//double hybx = uPXB2; // template
	//if( mpix*1E4 < 20 ) hybx = cogx; // COG
	//if( mpix*1E4 > 75 ) hybx = cogx;
	//double hpix = fmod( hybx + 0.82, 0.01 ); // hpix = 0..0.01 from cluster hybrid method
	//double hybdx = hybx - lp2.x(); // residual
	
	//bool halfmod = 0;
	//if(      ilad2 ==  8 ) halfmod = 1;
	//else if( ilad2 ==  9 ) halfmod = 1;
	//else if( ilad2 == 24 ) halfmod = 1;
	//else if( ilad2 == 25 ) halfmod = 1;
	
	//	       double theta_ideal0= (-rho*rho+x0*x0+y0*y0+r2*r2)/(2*r2*sqrt(x0*x0+y0*y0));
	//	       double phi_circle = atan2(y0,x0);
	//	       double theta_ideal= acos(theta_ideal0)+phi_circle;
	//             double L=sqrt(1+((y0-yPXB2)*(y0-yPXB2))/((x0-xPXB2)*(x0-xPXB2)))*rho;
	//             double theta_1=acos(rho/L);
	//             double theta_2= asin((yPXB2*x0-y0*xPXB2)/((xPXB2-x0)*L));
	//             double theta_ideal= theta_1+theta_2;
	
	double kk=(y0-yPXB2)/(x0-xPXB2);
	double kb = (yPXB2*x0-y0*xPXB2)/(x0-xPXB2);
	
	double phi_ideal = atan(kk);
	double phi2_ideal = asin((x0*kk+kb-y0)/(rho*sqrt(1+kk*kk)));
	double theta_ideal = phi_ideal+phi2_ideal; 

	//int c_unit=1;
	//cout<<"y0="<<y0<<"  "<<"x0="<<x0<<endl;
	//cout<<"yPXB2="<<yPXB2<<"  "<<"xPXB2="<<xPXB2<<endl;
	//              double theta_ideal = acos((rho*rho+x0*x0+y0*y0-r1*r1)/(2*sqrt(2)*rho));
	//               double theta_ideal = acos((rho*rho+x0*x0+y0*y0-r1*r1)/(2*sqrt(2)*rho))+pi/4;
	
	
	double x_ideal;
	double y_ideal;

	x_ideal=rho*cos(theta_ideal)+x0;
	y_ideal=rho*sin(theta_ideal)+y0;

	// ==================================================
	//      Helix Method
	// =================================================

	// create helix
	std::vector<double> p1 = {xPXB1, yPXB1, zPXB1};
	std::vector<double> p3 = {xPXB3, yPXB3, zPXB3};

	// Fit the helix first with two points and the track curvature
	SimpleHelix Hel = SimpleHelix(p1, p3, fabs(rho), &Pass);

	// Information for detector plane
	double x_0 = det2->position().x();
	double y_0 = det2->position().y();
	double z_0 = det2->position().z();

	double nX = det2->surface().normalVector().x();
	double nY = det2->surface().normalVector().y();
	double nZ = det2->surface().normalVector().z();

	//std::cout<<" normal vector : "<< nX << "  ,  " << nY << " , " << nZ <<std::endl;
	//std::cout<<" position      : "<< x_0<< "  ,  " <<y_0 << " , " << z_0<<std::endl;

	std::vector<double> plane = {x_0, y_0, z_0, nX, nY, nZ};
	std::vector<double> intersection = {};

	// Find the intersection of the detector plane and helix
	nloops = Hel.SimpleHelix::pposdir(plane, intersection);

	std::vector<double> p2 = {intersection[0],intersection[1],intersection[2]};
	Surface::GlobalPoint EstimatedPoint(intersection[0],intersection[1],intersection[2]);
	Surface::LocalPoint LocalEstimate = det2->toLocal(EstimatedPoint);

	insideN = det2->surface().bounds().inside(LocalEstimate);
	

	// Use the intersection to create a new helix without the curvature
	Hel.SimpleHelix::parameters(radius, xcenter, ycenter, dzdphi, z0);

	xl_ideal = LocalEstimate.x();
	yl_ideal = LocalEstimate.y();
	zl_ideal = LocalEstimate.z();

	Surface::GlobalPoint gp_ideal( x_ideal, y_ideal, uz2 );
	Surface::LocalPoint lp_ideal = det2->toLocal( gp_ideal );
	
	//xl_ideal= lp_ideal.x();
	//yl_ideal= lp_ideal.y();
	//zl_ideal= lp_ideal.z();

	x_local_error=ePXB2*1E4;
	y_local_error=fPXB2*1E4;
	//double unit=1;
	// if( kap2>=0)  unit=1;
	// if(kap2<=0)  continue;
	

	residual_x= (xpx1_l - xl_ideal)*1E4;
	residual_y= (xpy1_l - yl_ideal)*1E4;
	residual_z= (xpz1_l - zl_ideal)*1E4;
	residual=dca2*1E4;
	if(pt>2){
	  h420->Fill( residual_x ); 
	  h421->Fill( residual_y );
	  // Add errors and pull
	  h077->Fill( ePXB2*1E4 );
	  h078->Fill( fPXB2*1E4 );
	  double pulx=0., puly=0.;
	  if(x_local_error!=0.0) pulx = residual_x/x_local_error; 
	  if(y_local_error!=0.0) puly = residual_y/y_local_error;
	  h079->Fill( pulx );
	  h069->Fill( puly );
	}
	final_pt=pt;
	final_phi=phi;
	final_eta=eta;
	final_p=pp;

	//outtree->Fill();
	
      }//triplet 1+3 -> 2
        
      if( pt > 2 ) {
	
      // loop over hits on this track:
	
	for( vector<TransientTrackingRecHit::RecHitPointer>::iterator jTTRH = myTTRHvec.begin(); jTTRH != myTTRHvec.end(); ++jTTRH ) {
	  if( ! (*jTTRH)->hit()->isValid() ) continue;
	  Surface::GlobalPoint gp = (*jTTRH)->globalPosition();
	  Trajectory::RecHitContainer nyTTRHvec; // for fit
	  
	  for( vector<TransientTrackingRecHit::RecHitPointer>::iterator iTTRH = myTTRHvec.begin(); iTTRH != myTTRHvec.end(); ++iTTRH ) {
	    
	    if( iTTRH == jTTRH ) continue;//skip hit i
	    
	    nyTTRHvec.push_back( *iTTRH );//copy all others
	  }//copy
	
	  // re-fit without hit i:
	  
	  std::vector<Trajectory> refitTrajectoryVec2 = theFitter->fit( seed, nyTTRHvec, initialTSOS );
	  if( refitTrajectoryVec2.size() > 0 ) { // should be either 0 or 1
	    const Trajectory& refitTrajectory2 = refitTrajectoryVec2.front();
	    // Trajectory.measurements:
	    const TrajectoryMeasurement closestTM = refitTrajectory2.closestMeasurement( gp );
	    TrajectoryStateOnSurface closestTSOS = closestTM.updatedState();
	    TrajectoryStateOnSurface propTSOS = thePropagator->propagate( closestTSOS, (*jTTRH)->det()->surface() );
	    
	    if( propTSOS.isValid() ){
	      
	      const Topology* theTopology = &( (*jTTRH)->detUnit()->topology() );
	      // MeasurementPoint [pitch] (like channel number)
	      MeasurementPoint hitMeasurement = theTopology->measurementPosition( (*jTTRH)->localPosition() );
	      
	      // TID and TEC have trapezoidal detectors:
	      // translation from channel number into local x depends on local y
	      // track prediction has local x,y => can convert into proper channel number MeasurementPoint:
	      MeasurementPoint predictedPosition = theTopology->measurementPosition( propTSOS.localPosition() );
	      double resid = hitMeasurement.x() - predictedPosition.x();//[pitch]
	      DetId detId = (*jTTRH)->hit()->geographicalId();
	      uint32_t subDet = detId.subdetId();
	      
	      double xptch;
	      //double yptch;
	      if( subDet <  3 ){//1,2=pixel
		PixelTopology & pixelTopol = (PixelTopology&) (*jTTRH)->detUnit()->topology();
		xptch = pixelTopol.pitch().first;
		//yptch = pixelTopol.pitch().second;
	      }
	      else {//strip
		StripTopology & stripTopol = (StripTopology&) (*jTTRH)->detUnit()->topology();
		xptch = stripTopol.localPitch( propTSOS.localPosition() );
		//yptch = stripTopol.localStripLength( propTSOS.localPosition() );
	      }
	      
	      resid = resid * xptch;//[cm]
	      if( subDet == PixelSubdetector::PixelEndcap ) {
		
		if( PXFDetId(detId).disk() == 1 ) {
		  resid_disk1=resid*1E4;
		}//layer
		
		if( PXFDetId(detId).disk() == 2 ) {
		  resid_disk2=resid*1E4;
		}//layer
		
		
	      }// PXE
	      
	      
	    }//Valid propTSOS
	  }// 
	  
	  //------------------------------------------------------------------------
	  // Triplet 1+3 -> 2 with refitted track:
	  
	  if( refit2valid ){
	    
	    //if( jdbg ) cout << "  triplet 1+3 -> 2 refitted\n";
	    //double f2 = atan2( yPXB2, xPXB2 );//position angle in layer 2
	    double rho2 = 1/kap2;
	    double lam = sqrt( -0.25 + 
			       rho2*rho2 / ( ( xPXB1 - xPXB3 )*( xPXB1 - xPXB3 ) + ( yPXB1 - yPXB3 )*( yPXB1 - yPXB3 ) ) );
	    
	    // There are two solutions, the sign of kap gives the information
	    // which of them is correct:
	    
	    if( kap2 > 0 ) lam = -lam;
	    
	    // ( X0, Y0 ) is the centre of the circle
	    // that describes the helix in xy-projection:
	    double x0 =  0.5*( xPXB1 + xPXB3 ) + lam * ( -yPXB1 + yPXB3 );
	    double y0 =  0.5*( yPXB1 + yPXB3 ) + lam * (  xPXB1 - xPXB3 );
	    
	    // Calculate theta:
	    //double num = ( yPXB3 - y0 ) * ( xPXB1 - x0 ) - ( xPXB3 - x0 ) * ( yPXB1 - y0 );
	    //double den = ( xPXB1 - x0 ) * ( xPXB3 - x0 ) + ( yPXB1 - y0 ) * ( yPXB3 - y0 );
	    //double tandip = kap2 * ( zPXB3 - zPXB1 ) / atan( num / den );
	    //double udip = atan(tandip);
	    //double utet = pihalf - udip;
	    
	    // To get phi0 in the right interval one must distinguish
	    // two cases with positve and negative kap:
	    double uphi;
	    if( kap2 > 0 ) uphi = atan2( -x0,  y0 );
	    else          uphi = atan2(  x0, -y0 );
	    
	    // The distance of the closest approach DCA depends on the sign
	    // of kappa:
	    double udca;
	    if( kap2 > 0 ) udca = rho2 - sqrt( x0*x0 + y0*y0 );
	    else          udca = rho2 + sqrt( x0*x0 + y0*y0 );
	    
	    // angle from first hit to dca point:
	    //double dphi = atan( ( ( xPXB1 - x0 ) * y0 - ( yPXB1 - y0 ) * x0 )
	    //			/ ( ( xPXB1 - x0 ) * x0 + ( yPXB1 - y0 ) * y0 ) );
	    
	    //double uz0 = zPXB1 + tandip * dphi * rho2;
	    
	    
	    // interpolate to middle hit:
	    // cirmov
	    double rinv2 = -kap2;
	    double cosphi = cos(uphi);
	    double sinphi = sin(uphi);
	    double dp = -xPXB2*sinphi + yPXB2*cosphi + udca;
	    double dl = -xPXB2*cosphi - yPXB2*sinphi;
	    double sa = 2*dp + rinv2 * ( dp*dp + dl*dl );
	    double dca2 = sa / ( 1 + sqrt(1 + rinv2*sa) );// distance to hit 2
	    double ud = 1 + rinv2*udca;
	    double phi2 = atan2( -rinv2*xPXB2 + ud*sinphi, rinv2*yPXB2 + ud*cosphi );//direction
	    double phiinc = phi2 - phiN2;//angle of incidence in rphi w.r.t. normal vector
	    
	    // phiN alternates inward/outward
	    // reduce phiinc:
	    
	    if( phiinc > pihalf ) phiinc -= pi;
	    else if( phiinc < -pihalf ) phiinc += pi;
	    
	    // arc length:
	    
	    double xx = xPXB2 + dca2 * sin(phi2); // point on track
	    double yy = yPXB2 - dca2 * cos(phi2);
	    
	    double f0 = uphi;//
	    double kx = kap2*xx;
	    double ky = kap2*yy;
	    double kd = kap2*udca;
	    
	    // Solve track equation for s:
	    double dx = kx - (kd-1)*sin(f0);
	    double dy = ky + (kd-1)*cos(f0);
	    double ks = atan2( dx, -dy ) - f0;// turn angle
	    
	    //---  Limit to half-turn:
	    if(      ks >  pi ) ks = ks - twopi;
	    else if( ks < -pi ) ks = ks + twopi;
	    
	    //double s = ks*rho2;// signed
	    //double uz2 = uz0 + s*tandip; //track z at R2
	    //double dz2 = zPXB2 - uz2;

	    residual_refit = dca2 * 1E4;
	    

	    //triplet 1+3 -> 2 refitted
	    
	    //
	  }// refit2 ok
	}// loop hit i left out
      }// pt>2
      outtree->Fill();
    }// three hits

  }// loop over tracks
    

}//event
//----------------------------------------------------------------------
// method called just after ending the event loop:
//
void PxlFPix::endJob() {
  
  std::cout << "end of job after " << myCountersF::neve << " events.\n";
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(PxlFPix);

