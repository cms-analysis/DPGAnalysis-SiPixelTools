// File: PixClustersTest.cc
// Description: To test the pixel clusters. 
// Author: Danek Kotlinski 
// Creation Date:  Initial version. 3/06
// Modify to work with CMSSW620, 8/13, CMSSW700, 10/13 d.k.
// Add ByToken data access.
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
//#include "FWCore/Utilities/interface/EDGetToken.h"  // not needed
//#include "DataFormats/Common/interface/EDProduct.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
//#include "DataFormats/Phase2ITPixelCluster/interface/Phase2ITPixelCluster.h"


#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
//#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"

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

// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// For ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TProfile.h>
#include <TProfile2D.h>

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#define HISTOS

using namespace std;
namespace {
  const bool myDEBUG = false;
}

//=============================================================================

class PixClusterTestPhase2 : public edm::EDAnalyzer {
 public:
  
  explicit PixClusterTestPhase2(const edm::ParameterSet& conf);  
  virtual ~PixClusterTestPhase2();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginRun(const edm::EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
  
 private:
  edm::ParameterSet conf_;
  edm::InputTag src_;
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>tPixelCluster;

  bool printLocal;
  bool phase1_;
  int countEvents, countAllEvents;
  double sumClusters;

  //TFile* hFile;
  TH1F *hdetunit;
  TH1F *hpixid,*hpixsubid,
    *hlayerid,
    *hladder1id,*hladder2id,*hladder3id,*hladder4id,
    *hz1id,*hz2id,*hz3id,*hz4id;
  // histos 1,2,3,4 are for bpix, 5,6,7 for fpix 
  TH1F *hcharge1,*hcharge2, *hcharge3, *hcharge4, *hcharge5, *hcharge6, *hcharge7 ;
  TH1F *hpixcharge1,*hpixcharge2, *hpixcharge3, *hpixcharge4, *hpixcharge5, *hpixcharge6, *hpixcharge7;
  TH1F *hsize1,*hsize2,*hsize3,*hsize4,*hsize5,*hsize6,*hsize7,
    *hsizex1,*hsizex2,*hsizex3,*hsizex4,*hsizex5,*hsizex6,*hsizex7,
    *hsizey1,*hsizey2,*hsizey3,*hsizey4,*hsizey5,*hsizey6,*hsizey7;

  TH1F *hy1,*hy2,*hy3,*hy4,*hy5,*hy6,*hy7,*hx1,*hx2,*hx3,*hx4,*hx5,*hx6,*hx7;
  //TH1F *hpy1,*hpy2,*hpy3,*hpy4,*hpx1,*hpx2,*hpx3,*hpx4;

  TH1F *hclusPerDet1,*hclusPerDet2,*hclusPerDet3,*hclusPerDet4;
  TH1F *hpixPerDet1, *hpixPerDet2, *hpixPerDet3, *hpixPerDet4;
  TH1F *hclusPerLay1,*hclusPerLay2,*hclusPerLay3,*hclusPerLay4;
  TH1F *hpixPerLay1, *hpixPerLay2, *hpixPerLay3, *hpixPerLay4;

  TH1F *hclus, *hclusBPix, *hclusFPix, *hdigis, *hdigisB, *hdigisF;

  // 2D
  TH2F *hcluDetMap4,*hcluDetMap3,*hcluDetMap2,*hcluDetMap1; 
  TH2F *hpixDetMap4,*hpixDetMap3,*hpixDetMap2,*hpixDetMap1; 
  TH2F *hpixMap1, *hpixMap2, *hpixMap3,*hpixMap4;  // pixels in a module 
   
  // FPix
  TH2F *hpixDetMapF1,*hpixDetMapF2; 
  TH2F *hcluDetMapF1,*hcluDetMapF2; 
  TH2F *hpixMapF11, *hpixMapF12, *hpixMapF13,*hpixMapF14;
  TH2F *hpixMapF21, *hpixMapF22, *hpixMapF23,*hpixMapF24, *hpixMapF25;

  TH1F *hclusPerDetF1,*hclusPerDetF2;
  TH1F *hpixPerDetF1, *hpixPerDetF2;
  TH1F *hclusPerDetF11,*hclusPerDetF12,*hclusPerDetF13,*hclusPerDetF14;
  TH1F *hpixPerDetF11, *hpixPerDetF12, *hpixPerDetF13, *hpixPerDetF14;
  TH1F *hclusPerDetF21,*hclusPerDetF22,*hclusPerDetF23,*hclusPerDetF24,*hclusPerDetF25;
  TH1F *hpixPerDetF21, *hpixPerDetF22, *hpixPerDetF23, *hpixPerDetF24, *hpixPerDetF25;


  TH1F *hclusPerDisk1,*hclusPerDisk2,*hclusPerDisk3,*hclusPerDisk4;

  TProfile2D *hcluDetFPix,*hcluDetEPix, *hpixDetFPix,*hpixDetEPix, *hsizeDetFPix,*hsizeDetEPix; 

  TH2F *hxy, *hphiz1, *hphiz2, *hphiz3, *hphiz4; // bpix 
  TH2F *hzr, *hxy11, *hxy12, *hxy21, *hxy22, *hxy31, *hxy32;  // fpix 

  TH1F *hlumi, *hlumi0, *hlumi1; 
  TH1F *hbx, *hbx0, *hbx1;
  TH1F *hdets, *hmbits1,*hmbits2,*hmbits3, *hmaxPixPerDet;


};

/////////////////////////////////////////////////////////////////
// Contructor, empty.
PixClusterTestPhase2::PixClusterTestPhase2(edm::ParameterSet const& conf) 
  : conf_(conf), src_(conf.getParameter<edm::InputTag>( "src" )) { 

  printLocal = conf.getUntrackedParameter<bool>("Verbosity",false);
  phase1_ = conf.getUntrackedParameter<bool>("phase1",false);
  //src_ =  conf.getParameter<edm::InputTag>( "src" );

  cout<<" Construct for phase "<<phase1_<<" print "<<printLocal<<endl;
  tPixelCluster = consumes<edmNew::DetSetVector<SiPixelCluster>>( src_ );
  //tPixelCluster = consumes<edmNew::DetSetVector<Phase2ITPixelCluster>>( src_ );

}
// Virtual destructor needed.
PixClusterTestPhase2::~PixClusterTestPhase2() { }  

// ------------ method called at the begining   ------------
void PixClusterTestPhase2::beginRun(const edm::EventSetup& iSetup) {
  cout << "beginRun -  PixelClusterTest " <<printLocal<<endl;
}

// ------------ method called at the begining   ------------
void PixClusterTestPhase2::beginJob() {
  cout << "Initialize PixelClusterTest " <<printLocal<<endl;

#ifdef HISTOS

  edm::Service<TFileService> fs;

  hladder1id = fs->make<TH1F>( "hladder1id", "Ladder L1 id", 12, 0.5, 12.5);
  hladder2id = fs->make<TH1F>( "hladder2id", "Ladder L2 id", 28, 0.5, 28.5);
  hladder3id = fs->make<TH1F>( "hladder3id", "Ladder L3 id", 24, 0.5, 24.5);
  hladder4id = fs->make<TH1F>( "hladder4id", "Ladder L4 id", 32, 0.5, 32.5);
  hz1id = fs->make<TH1F>( "hz1id", "Z-index id L1", 9, 0.5, 9.5);
  hz2id = fs->make<TH1F>( "hz2id", "Z-index id L2", 9, 0.5, 9.5);
  hz3id = fs->make<TH1F>( "hz3id", "Z-index id L3", 9, 0.5, 9.5);
  hz4id = fs->make<TH1F>( "hz4id", "Z-index id L4", 9, 0.5, 9.5);
  
  int sizeH=200;
  float lowH = -0.5;
  float highH = 199.5;

  hclusPerDet1 = fs->make<TH1F>( "hclusPerDet1", "Clus per det l1",
			    sizeH, lowH, highH);
  hclusPerDet2 = fs->make<TH1F>( "hclusPerDet2", "Clus per det l2",
			    sizeH, lowH, highH);
  hclusPerDet3 = fs->make<TH1F>( "hclusPerDet3", "Clus per det l3",
			    sizeH, lowH, highH);
  hclusPerDet4 = fs->make<TH1F>( "hclusPerDet4", "Clus per det l4",
			    sizeH, lowH, highH);

  hclusPerDetF1 = fs->make<TH1F>( "hclusPerDetF1", "Clus per det disk1-8",
			    sizeH, lowH, highH);
  hclusPerDetF2 = fs->make<TH1F>( "hclusPerDetF2", "Clus per det disk9-12",
			    sizeH, lowH, highH);

  hclusPerDetF11 = fs->make<TH1F>("hclusPerDetF11","Clus det R1 Disk1-8",sizeH,lowH,highH);
  hclusPerDetF12 = fs->make<TH1F>("hclusPerDetF12","Clus det R2 Disk1-8",sizeH,lowH,highH);
  hclusPerDetF13 = fs->make<TH1F>("hclusPerDetF13","Clus det R3 Disk1-8",sizeH,lowH,highH);
  hclusPerDetF14 = fs->make<TH1F>("hclusPerDetF14","Clus det R4 Disk1-8",sizeH,lowH,highH);

  hclusPerDetF21 = fs->make<TH1F>("hclusPerDetF21","Clus det R1 Disk9-12",sizeH,lowH,highH);
  hclusPerDetF22 = fs->make<TH1F>("hclusPerDetF22","Clus det R2 Disk9-12",sizeH,lowH,highH);
  hclusPerDetF23 = fs->make<TH1F>("hclusPerDetF23","Clus det R3 Disk9-12",sizeH,lowH,highH);
  hclusPerDetF24 = fs->make<TH1F>("hclusPerDetF24","Clus det R4 Disk9-12",sizeH,lowH,highH);
  hclusPerDetF25 = fs->make<TH1F>("hclusPerDetF25","Clus det R5 Disk9-12",sizeH,lowH,highH);

  highH = 399.5;
  hpixPerDetF11 = fs->make<TH1F>("hpixPerDetF11","Pix det R1 Disk1-8",sizeH,lowH,highH);
  hpixPerDetF12 = fs->make<TH1F>("hpixPerDetF12","Pix det R2 Disk1-8",sizeH,lowH,highH);
  hpixPerDetF13 = fs->make<TH1F>("hpixPerDetF13","Pix det R3 Disk1-8",sizeH,lowH,highH);
  hpixPerDetF14 = fs->make<TH1F>("hpixPerDetF14","Pix det R4 Disk1-8",sizeH,lowH,highH);

  hpixPerDetF21 = fs->make<TH1F>("hpixPerDetF21","Pix det R1 Disk9-12",sizeH,lowH,highH);
  hpixPerDetF22 = fs->make<TH1F>("hpixPerDetF22","Pix det R2 Disk9-12",sizeH,lowH,highH);
  hpixPerDetF23 = fs->make<TH1F>("hpixPerDetF23","Pix det R3 Disk9-12",sizeH,lowH,highH);
  hpixPerDetF24 = fs->make<TH1F>("hpixPerDetF24","Pix det R4 Disk9-12",sizeH,lowH,highH);
  hpixPerDetF25 = fs->make<TH1F>("hpixPerDetF25","Pix det R5 Disk9-12",sizeH,lowH,highH);

  hcluDetFPix = fs->make<TProfile2D>("hcluDetFPix","clu per det",8,1,9,4,1,5,0.,1000.);
  hpixDetFPix = fs->make<TProfile2D>("hpixDetFPix","pix per det",8,1,9,4,1,5,0.,1000.);
  hsizeDetFPix = fs->make<TProfile2D>("hsizeDetFPix","size per det",8,1,9,4,1,5,0.,1000.);
  hcluDetEPix = fs->make<TProfile2D>("hcluDetEPix","clu per det",4,9,13,5,1,6,0.,1000.);
  hpixDetEPix = fs->make<TProfile2D>("hpixDetEPix","pix per det",4,9,13,5,1,6,0.,1000.);
  hsizeDetEPix = fs->make<TProfile2D>("hsizeDetEPix","size per det",4,9,13,5,1,6,0.,1000.);

  sizeH=1000;
  highH = 1999.5;
  hpixPerDet1 = fs->make<TH1F>( "hpixPerDet1", "Pix per det l1",
			    sizeH, lowH, highH);
  hpixPerDet2 = fs->make<TH1F>( "hpixPerDet2", "Pix per det l2",
			    sizeH, lowH, highH);
  hpixPerDet3 = fs->make<TH1F>( "hpixPerDet3", "Pix per det l3",
			    sizeH, lowH, highH);
  hpixPerDet4 = fs->make<TH1F>( "hpixPerDet4", "Pix per det l4",
			    sizeH, lowH, highH);
  hpixPerDetF1 = fs->make<TH1F>( "hpixPerDetF1", "Pix per det disk1-8",
			    sizeH, lowH, highH);
  hpixPerDetF2 = fs->make<TH1F>( "hpixPerDetF2", "Pix per det disk9-12",
			    sizeH, lowH, highH);

  // sizeH=1000;
  // highH = 999.5;
  // hpixPerLink1 = fs->make<TH1F>( "hpixPerLink1", "Pix per link l1",
  // 			    sizeH, lowH, highH);
  // hpixPerLink2 = fs->make<TH1F>( "hpixPerLink2", "Pix per link l2",
  // 			    sizeH, lowH, highH);
  // hpixPerLink3 = fs->make<TH1F>( "hpixPerLink3", "Pix per link l3",
  // 			    sizeH, lowH, highH);
  // hpixPerLink4 = fs->make<TH1F>( "hpixPerLink4", "Pix per link l4",
  // 			    sizeH, lowH, highH);

  sizeH=1000;
  highH = 59999.5;
  hclusPerLay1 = fs->make<TH1F>( "hclusPerLay1", "Clus per layer l1",
				 sizeH, lowH, highH);
  hclusPerLay2 = fs->make<TH1F>( "hclusPerLay2", "Clus per layer l2",
			    sizeH, lowH, highH);
  hclusPerLay3 = fs->make<TH1F>( "hclusPerLay3", "Clus per layer l3",
			    sizeH, lowH, highH);
  hclusPerLay4 = fs->make<TH1F>( "hclusPerLay4", "Clus per layer l4",
			    sizeH, lowH, highH);

  hclusPerDisk1 = fs->make<TH1F>( "hclusPerDisk1", "Clus per -disk1-8",
			    sizeH, lowH, highH);
  hclusPerDisk2 = fs->make<TH1F>( "hclusPerDisk2", "Clus per +disk1-8",
			    sizeH, lowH, highH);
  hclusPerDisk3 = fs->make<TH1F>( "hclusPerDisk3", "Clus per -disk9-12",
			    sizeH, lowH, highH);
  hclusPerDisk4 = fs->make<TH1F>( "hclusPerDisk4", "Clus per +disk9-12",
			    sizeH, lowH, highH);

  sizeH=1000;
  highH = 99999.5;
  hpixPerLay1 = fs->make<TH1F>( "hpixPerLay1", "Pix per layer l1",
				 sizeH, lowH, highH);
  hpixPerLay2 = fs->make<TH1F>( "hpixPerLay2", "Pix per layer l2",
				 sizeH, lowH, highH);
  hpixPerLay3 = fs->make<TH1F>( "hpixPerLay3", "Pix per layer l3",
				 sizeH, lowH, highH);
  hpixPerLay4 = fs->make<TH1F>( "hpixPerLay4", "Pix per layer l4",
				 sizeH, lowH, highH);

  hclus = fs->make<TH1F>( "hclus", "Clus per event",
			    1000, lowH, 999999.5);
  hdigis = fs->make<TH1F>( "hdigis", "All Digis in clus per event",
			    1000, lowH, 999999.5);
  hdigisB = fs->make<TH1F>( "hdigisB", "BPix Digis in clus per event",
			    1000, lowH, 999999.5);
  hdigisF = fs->make<TH1F>( "hdigisF", "FPix Digis in clus per event",
			    1000, lowH, 999999.5);
  hclusBPix = fs->make<TH1F>( "hclusBPix", "Bpix Clus per event",
			     1000, 0.,  999999.5);
  hclusFPix = fs->make<TH1F>( "hclusFPix", "Fpix Clus per event",
			     1000, 0.,  999999.5);
  hdets = fs->make<TH1F>( "hdets","Dets per event",2000, -0.5, 9999.5);

  // sizeH=1000;
  // highH = 1999.5;

  // hdetsPerLay1 = fs->make<TH1F>( "hdetsPerLay1", "Full dets per layer l1",
  // 				 161, -0.5, 160.5);
  // hdetsPerLay3 = fs->make<TH1F>( "hdetsPerLay3", "Full dets per layer l3",
  // 				 353, -0.5, 352.5);
  // hdetsPerLay2 = fs->make<TH1F>( "hdetsPerLay2", "Full dets per layer l2",
  // 				 257, -0.5, 256.5);
  // hdetsPerLay4 = fs->make<TH1F>( "hdetsPerLay4", "Full dets per layer l4",
  // 				 512, -0.5, 511.5);
 
  sizeH=400;
  lowH = 0.;
  highH = 200.0; // charge limit in kelec
  hcharge1 = fs->make<TH1F>( "hcharge1", "Clu charge l1", sizeH, 0.,highH); //in ke
  hcharge2 = fs->make<TH1F>( "hcharge2", "Clu charge l2", sizeH, 0.,highH);
  hcharge3 = fs->make<TH1F>( "hcharge3", "Clu charge l3", sizeH, 0.,highH);
  hcharge4 = fs->make<TH1F>( "hcharge4", "Clu charge l4", sizeH, 0.,highH);
  hcharge5 = fs->make<TH1F>( "hcharge5", "Clu charge d1", sizeH, 0.,highH);
  hcharge6 = fs->make<TH1F>( "hcharge6", "Clu charge d2", sizeH, 0.,highH);
  //hcharge7 = fs->make<TH1F>( "hcharge7", "Clu charge d3", sizeH, 0.,highH);
 
  sizeH=200;
  highH = 20.0; // charge limit in kelec
  hpixcharge1 = fs->make<TH1F>( "hpixcharge1", "Pix charge l1",sizeH, 0.,highH);//in ke
  hpixcharge2 = fs->make<TH1F>( "hpixcharge2", "Pix charge l2",sizeH, 0.,highH);
  hpixcharge3 = fs->make<TH1F>( "hpixcharge3", "Pix charge l3",sizeH, 0.,highH);
  hpixcharge4 = fs->make<TH1F>( "hpixcharge4", "Pix charge l4",sizeH, 0.,highH);
  hpixcharge5 = fs->make<TH1F>( "hpixcharge5", "Pix charge d1",sizeH, 0.,highH);
  hpixcharge6 = fs->make<TH1F>( "hpixcharge6", "Pix charge d2",sizeH, 0.,highH);
  //hpixcharge7 = fs->make<TH1F>( "hpixcharge7", "Pix charge d3",sizeH, 0.,highH);
  
  hy1 = fs->make<TH1F>( "hy1", "Layer 1 y pos", 500,-0.5,499.5);
  hy2 = fs->make<TH1F>( "hy2", "Layer 2 y pos", 500,-0.5,499.5);
  hy3 = fs->make<TH1F>( "hy3", "Layer 3 y pos", 500,-0.5,499.5);
  hy4 = fs->make<TH1F>( "hy4", "Layer 4 y pos", 500,-0.5,499.5);
  hy5 = fs->make<TH1F>( "hy5", "Disk 1 y pos", 500,-0.5,499.5);
  hy6 = fs->make<TH1F>( "hy6", "Disk 2 y pos", 500,-0.5,499.5);
  //hy7 = fs->make<TH1F>( "hy7", "Disk 3 y pos", 500,-0.5,499.5);
  
  hx1 = fs->make<TH1F>( "hx1", "Layer 1 x pos", 1321,-0.5,1320.5);
  hx2 = fs->make<TH1F>( "hx2", "Layer 2 x pos", 1321,-0.5,1320.5);
  hx3 = fs->make<TH1F>( "hx3", "Layer 3 x pos", 1321,-0.5,1320.5);
  hx4 = fs->make<TH1F>( "hx4", "Layer 4 x pos", 1321,-0.5,1320.5);
  hx5 = fs->make<TH1F>( "hx5", "Disk 1 x pos", 1321,-0.5,1320.5);
  hx6 = fs->make<TH1F>( "hx6", "Disk 2 x pos", 1321,-0.5,1320.5);
  //hx7 = fs->make<TH1F>( "hx7", "Disk 3 x pos", 1321,-0.5,1320.5);

  //hpy1 = fs->make<TH1F>( "hpy1", "Layer 1 pix cols", 500,-0.5,499.5);
  //hpy2 = fs->make<TH1F>( "hpy2", "Layer 2 pix cols", 500,-0.5,499.5);
  //hpy3 = fs->make<TH1F>( "hpy3", "Layer 3 pix cols", 500,-0.5,499.5);
  //hpy4 = fs->make<TH1F>( "hpy4", "Layer 4 pix cols", 500,-0.5,499.5);
  
  //hpx1 = fs->make<TH1F>( "hpx1", "Layer 1 pix rows", 200,-0.5,199.5);
  //hpx2 = fs->make<TH1F>( "hpx2", "Layer 2 pix rows", 200,-0.5,199.5);
  //hpx3 = fs->make<TH1F>( "hpx3", "layer 3 pix rows", 200,-0.5,199.5);
  //hpx4 = fs->make<TH1F>( "hpx4", "layer 4 pix rows", 200,-0.5,199.5);

  sizeH=300;
  highH = 299.5; // charge limit in kelec
  hsize1 = fs->make<TH1F>( "hsize1", "layer 1 clu size",sizeH,-0.5,highH);
  hsize2 = fs->make<TH1F>( "hsize2", "layer 2 clu size",sizeH,-0.5,highH);
  hsize3 = fs->make<TH1F>( "hsize3", "layer 3 clu size",sizeH,-0.5,highH);
  hsize4 = fs->make<TH1F>( "hsize4", "layer 4 clu size",sizeH,-0.5,highH);
  hsize5 = fs->make<TH1F>( "hsize5", "disk 1 clu size",sizeH,-0.5,highH);
  hsize6 = fs->make<TH1F>( "hsize6", "disk 2 clu size",sizeH,-0.5,highH);
  //hsize7 = fs->make<TH1F>( "hsize7", "disk 3 clu size",sizeH,-0.5,highH);

  hsizex1 = fs->make<TH1F>( "hsizex1", "lay1 clu size in x",
		      20,-0.5,19.5);
  hsizex2 = fs->make<TH1F>( "hsizex2", "lay2 clu size in x",
		      20,-0.5,19.5);
  hsizex3 = fs->make<TH1F>( "hsizex3", "lay3 clu size in x",
		      20,-0.5,19.5);
  hsizex4 = fs->make<TH1F>( "hsizex4", "lay4 clu size in x",
		      20,-0.5,19.5);
  hsizex5 = fs->make<TH1F>( "hsizex5", "d1 clu size in x",
		      20,-0.5,19.5);
  hsizex6 = fs->make<TH1F>( "hsizex6", "d2 clu size in x",
		      20,-0.5,19.5);

  hsizey1 = fs->make<TH1F>( "hsizey1", "lay1 clu size in y",
		      30,-0.5,29.5);
  hsizey2 = fs->make<TH1F>( "hsizey2", "lay2 clu size in y",
		      30,-0.5,29.5);
  hsizey3 = fs->make<TH1F>( "hsizey3", "lay3 clu size in y",
		      30,-0.5,29.5);
  hsizey4 = fs->make<TH1F>( "hsizey4", "lay4 clu size in y",
		      30,-0.5,29.5);
  hsizey5 = fs->make<TH1F>( "hsizey5", "d1 clu size in y",
		      30,-0.5,29.5);
  hsizey6 = fs->make<TH1F>( "hsizey6", "d2 clu size in y",
		      30,-0.5,29.5);

  //hevent = fs->make<TH1F>("hevent","event",1000,0,10000000.);
  //horbit = fs->make<TH1F>("horbit","orbit",100, 0,100000000.);

  hlumi  = fs->make<TH1F>("hlumi", "lumi",   2000,0,2000.);
  hlumi1  = fs->make<TH1F>("hlumi1", "lumi", 2000,0,2000.);
  hlumi0  = fs->make<TH1F>("hlumi0", "lumi", 2000,0,2000.);
  hbx1    = fs->make<TH1F>("hbx1",   "bx",   4000,0,4000.);  
  hbx0    = fs->make<TH1F>("hbx0",   "bx",   4000,0,4000.);  
  hbx    = fs->make<TH1F>("hbx",   "bx",     4000,0,4000.);  

  hzr = fs->make<TH2F>("hzr"," ",600,-300.,300.,60,0.,30.);  // x-y plane
  hxy11 = fs->make<TH2F>("hxy11"," ",320,-16.,16.,320,-16.,16.); // x-y pla 
  hxy12 = fs->make<TH2F>("hxy12"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy21 = fs->make<TH2F>("hxy21"," ",520,-26.,26.,520,-26.,26.); // x-y pl
  hxy22 = fs->make<TH2F>("hxy22"," ",520,-26.,26.,520,-26.,26.); // x-y pla
  //hxy31 = fs->make<TH2F>("hxy31"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  //hxy32 = fs->make<TH2F>("hxy32"," ",320,-16.,16.,320,-16.,16.); // x-y plae

  hxy = fs->make<TH2F>("hxy"," ",340,-17.,17.,340,-17.,17.);  // x-y plane
  hphiz1 = fs->make<TH2F>("hphiz1"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz2 = fs->make<TH2F>("hphiz2"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz3 = fs->make<TH2F>("hphiz3"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz4 = fs->make<TH2F>("hphiz4"," ",108,-27.,27.,140,-3.5,3.5);

  //htest = fs->make<TH1F>( "htest", "FPix R", 400, -20., 20.);


  // 2D
  hpixDetMap1 = fs->make<TH2F>("hpixDetMap1"," ",10,0,10,13,0,13);
  hpixDetMap1->SetOption("colz");
  hpixDetMap2 = fs->make<TH2F>("hpixDetMap2"," ",10,0,10,29,0,29);
  hpixDetMap2->SetOption("colz");
  hpixDetMap3 = fs->make<TH2F>("hpixDetMap3"," ",10,0,10,25,0,25);
  hpixDetMap3->SetOption("colz");
  hpixDetMap4 = fs->make<TH2F>("hpixDetMap4"," ",10,0,10,33,0,33);
  hpixDetMap4->SetOption("colz");
  
  hcluDetMap1 = fs->make<TH2F>("hcluDetMap1"," ",10,0,10,13,0,13);
  hcluDetMap1->SetOption("colz");
  hcluDetMap2 = fs->make<TH2F>("hcluDetMap2"," ",10,0,10,29,0,29);
  hcluDetMap2->SetOption("colz");
  hcluDetMap3 = fs->make<TH2F>("hcluDetMap3"," ",10,0,10,25,0,25);
  hcluDetMap3->SetOption("colz");
  hcluDetMap4 = fs->make<TH2F>("hcluDetMap4"," ",10,0,10,33,0,33);
  hcluDetMap4->SetOption("colz");
  
  hpixMap1 = fs->make<TH2F>("hpixMap1"," ",442,0.,442.,1320,0.,1320.);
  hpixMap1->SetOption("colz");
  hpixMap2 = fs->make<TH2F>("hpixMap2"," ",442,0.,442.,1320,0.,1320.);
  hpixMap2->SetOption("colz");
  hpixMap3 = fs->make<TH2F>("hpixMap3"," ",442,0.,442.,1320,0.,1320.);
  hpixMap3->SetOption("colz");
  hpixMap4 = fs->make<TH2F>("hpixMap4"," ",442,0.,442.,1320,0.,1320.);
  hpixMap4->SetOption("colz");
  
  // FPix
  hpixDetMapF1 = fs->make<TH2F>("hpixDetMapF1","hits in FPix",64,-32,32,4,1,5);
  hpixDetMapF1->SetOption("colz");
  hpixDetMapF2 = fs->make<TH2F>("hpixDetMapF2","hits in EPix",112,-56,56,5,1,6);
  hpixDetMapF2->SetOption("colz");

  hcluDetMapF1 = fs->make<TH2F>("hcluDetMapF1","clus in FPix",64,-32.,32.,4,1.,5.);
  hcluDetMapF1->SetOption("colz");
  hcluDetMapF2 = fs->make<TH2F>("hcluDetMapF2","clus in EPix",112,-56,56,5,1,6);
  hcluDetMapF2->SetOption("colz");

  hpixMapF11 = fs->make<TH2F>("hpixMapF11","FPix, ring 1",442,0.,442.,1320,0.,1320.);
  hpixMapF11->SetOption("colz");
  hpixMapF12 = fs->make<TH2F>("hpixMapF12","FPix, ring 2",442,0.,442.,1320,0.,1320.);
  hpixMapF12->SetOption("colz");
  hpixMapF13 = fs->make<TH2F>("hpixMapF13","FPix, ring 3",442,0.,442.,1320,0.,1320.);
  hpixMapF13->SetOption("colz");
  hpixMapF14 = fs->make<TH2F>("hpixMapF14","FPix, ring 4",442,0.,442.,1320,0.,1320.);
  hpixMapF14->SetOption("colz");

  hpixMapF21 = fs->make<TH2F>("hpixMapF21","EPix, ring 1",442,0.,442.,1320,0.,1320.);
  hpixMapF21->SetOption("colz");
  hpixMapF22 = fs->make<TH2F>("hpixMapF22","EPix, ring 2",442,0.,442.,1320,0.,1320.);
  hpixMapF22->SetOption("colz");
  hpixMapF23 = fs->make<TH2F>("hpixMapF23","EPix, ring 3",442,0.,442.,1320,0.,1320.);
  hpixMapF23->SetOption("colz");
  hpixMapF24 = fs->make<TH2F>("hpixMapF24","EPix, ring 4",442,0.,442.,1320,0.,1320.);
  hpixMapF24->SetOption("colz");
  hpixMapF25 = fs->make<TH2F>("hpixMapF25","EPix, ring 5",442,0.,442.,1320,0.,1320.);
  hpixMapF25->SetOption("colz");

#endif

  countEvents=0;
  countAllEvents=0;
  sumClusters=0.;

}
// ------------ method called to at the end of the job  ------------
void PixClusterTestPhase2::endJob(){
  cout<<" endJob "<<endl;
  if(countEvents>0) sumClusters = sumClusters/float(countEvents);
  cout << " End PixClusterTestPhase2, events all/with hits=  " << countAllEvents<<"/"<<countEvents<<" "
       <<sumClusters<<endl;

}
//////////////////////////////////////////////////////////////////
// Functions that gets called by framework every event
void PixClusterTestPhase2::analyze(const edm::Event& e, 
			      const edm::EventSetup& es) {
  using namespace edm;
  const bool debugLocal = false;

  // Get event setup 
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoH;
  es.get<TrackerTopologyRcd>().get(tTopoH);
  const TrackerTopology *tTopo=tTopoH.product();

  countAllEvents++;
  int run       = e.id().run();
  int event     = e.id().event();

  int lumiBlock = e.luminosityBlock();
  int bx        = e.bunchCrossing();
  int orbit     = e.orbitNumber();

  // Get Cluster Collection from InputTag
  edm::Handle< edmNew::DetSetVector<SiPixelCluster> > clusters;
  //edm::Handle< edmNew::DetSetVector<Phase2ITPixelCluster> > clusters;
  e.getByToken( tPixelCluster , clusters);
  //const edmNew::DetSetVector<SiPixelCluster>& input = *clusters;     

  int numOf = clusters->size();
  
  hbx0->Fill(float(bx));
  hlumi0->Fill(float(lumiBlock));

  if(printLocal) cout<<"run "<<run<<" event "<<event<<" bx "<<bx<<" lumi "<<lumiBlock<<" orbit "<<orbit<<" "<<numOf<<endl;  

  hdets->Fill(float(numOf)); // number of modules with pix

  // Select events with pixels
  //if(numOf<1) return; // skip events with  pixel dets
  //if(numOf<4) return; // skip events with few pixel dets

  //hevent->Fill(float(event));
  hlumi->Fill(float(lumiBlock));
  hbx->Fill(float(bx));
  //horbit->Fill(float(orbit));



  countEvents++;
  int numberOfDetUnits = 0;
  int numberOfClusters = 0;
  int numberOfPixels = 0;
  int numberOfDetUnits1 = 0;
  int numOfClustersPerDet1=0;        
  int numOfClustersPerLay1=0;        
  int numberOfDetUnits2 = 0;
  int numOfClustersPerDet2=0;        
  int numOfClustersPerLay2=0;        
  int numberOfDetUnits3 = 0;
  int numOfClustersPerDet3=0;        
  int numOfClustersPerLay3=0;        
  int numberOfDetUnits4 = 0;
  int numOfClustersPerDet4=0;        
  int numOfClustersPerLay4=0;        
  int numOfClustersPerDetF1=0;        
  int numOfClustersPerDetF2=0;        

  int numOfPixPerLay1=0;     
  int numOfPixPerLay2=0;     
  int numOfPixPerLay3=0;     
  int numOfPixPerLay4=0;     

  int numOfPixPerDet1=0;  
  int numOfPixPerDet2=0;  
  int numOfPixPerDet3=0;  
  int numOfPixPerDet4=0;  
  int numOfPixPerDetF1=0;  
  int numOfPixPerDetF2=0;  
      
  // int numOfPixPerLink11=0;  
  // int numOfPixPerLink12=0;  
  // int numOfPixPerLink21=0;  
  // int numOfPixPerLink22=0;  
  //SK:unused  int numOfPixPerLink3=0;  
  //int maxClusPerDet=0;
  //int maxPixPerDet=0;

  int numOfClustersPerDisk1=0;  
  int numOfClustersPerDisk2=0;  
  int numOfClustersPerDisk3=0;  
  int numOfClustersPerDisk4=0;  
  //int numOfClustersPerDisk5=0;  
  //int numOfClustersPerDisk6=0;  
  int numOfPixPerDisk1=0;  
  int numOfPixPerDisk2=0;  
  int numOfPixPerDisk3=0;  
  int numOfPixPerDisk4=0;  
  //int numOfPixPerDisk5=0;  
  //int numOfPixPerDisk6=0;  
        
  // float aveCharge1 = 0., aveCharge2 = 0., aveCharge3 = 0., 
  //   aveCharge4 = 0., aveCharge5 = 0.;

  //static int module1[416][160] = {{0}};
  //static int module2[416][160] = {{0}};
  //static int module3[416][160] = {{0}};
  //static int module4[416][160] = {{0}};

  
  // get vector of detunit ids
  //--- Loop over detunits.
  for (edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=clusters->begin();
       DSViter != clusters->end() ; ++DSViter) {

    unsigned int detid = DSViter->detId();
    // Det id
    DetId detId = DetId(detid);       // Get the Detid object
    unsigned int detType=detId.det(); // det type, pixel=1
    unsigned int subid=detId.subdetId(); //subdetector type, barrel=1
 
    if(debugLocal)
      cout<<"Det: "<<detId.rawId()<<" "<<detId.null()<<" "<<detType<<" "<<subid<<endl;    

    if(detType!=1) continue; // look only at pixels
    ++numberOfDetUnits;
  
    //const GeomDetUnit * genericDet = geom->idToDet(detId);
    //const PixelGeomDetUnit * pixDet = 
    //dynamic_cast<const PixelGeomDetUnit*>(genericDet);

    // Get the geom-detector
    const PixelGeomDetUnit * theGeomDet =
      dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );

    double detX = theGeomDet->surface().position().x();
    double detY = theGeomDet->surface().position().y();
    double detZ = theGeomDet->surface().position().z();
    double detR = theGeomDet->surface().position().perp();
    double detPhi = theGeomDet->surface().position().phi();

    //const BoundPlane& plane = theGeomDet->surface(); //for transf.
    
    //double detThick = theGeomDet->specificSurface().bounds().thickness();
    //int cols = theGeomDet->specificTopology().ncolumns();
    //int rows = theGeomDet->specificTopology().nrows();
    
    const PixelTopology * topol = &(theGeomDet->specificTopology());

    // barrel ids
    //unsigned int layerC=0;
    //unsigned int ladderC=0;
    //unsigned int zindex=0;
    //int shell  = 0; // shell id // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
    //int sector = 0; // 1-8
    int ladder = 0; // 1-22
    int layer  = 0; // 1-3
    int module = 0; // 1-4
    //bool half  = false; // 

    // Endcap ids
    unsigned int disk=0; //1,2,3
    unsigned int blade=0; //1-24
    unsigned int ring=0; // plaquette 1,2,3,4
    unsigned int side=0; //size=1 for -z, 2 for +z
    //unsigned int panel=0; //panel=1

    // Subdet id, pix barrel=1, forward=2
    if(subid==2) {  // forward

      disk=tTopo->pxfDisk(detid); //1-12
      ring=tTopo->pxfBlade(detid); // returns ring 
      blade=tTopo->pxfModule(detid); // return blade 
      side=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
      //panel=tTopo->pxfPanel(detid); //panel=1 always 
      //PixelEndcapName pen(detid,tTopo,phase1_);

      if(printLocal) cout<<" fpix (cmssw): disk "<<disk<<", blade "
 		    <<blade<<", ring "<<ring<<", side "<<side<<endl;
 
    } else if (subid==1) {  // barrel

      layer=tTopo->pxbLayer(detid);
      ladder=tTopo->pxbLadder(detid);
      module=tTopo->pxbModule(detid);
      //PixelBarrelName pbn(detid,tTopo,phase1_);

      // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
      //PixelBarrelName::Shell sh = pbn.shell(); //enum
      //sector = 0; // pbn.sectorName();
      //ladder = ladderC; //pbn.ladderName();
      //layer  = layerC; //pbn.layerName();
      //module = zindex; //pbn.moduleName();
      //half  = 0; // pbn.isHalfModule();
      //shell = 0; //int(sh);

      // change the module sign for z<0
      //if(shell==1 || shell==2) module = -module;
      // change ladeer sign for Outer )x<0)
      //if(shell==1 || shell==3) ladder = -ladder;
      
      if(printLocal) { 
	cout<<" BPix (cmssw) ladder, module "<<layer<<" "<<ladder<<" "<<module<< endl;
	//cout<<" Barrel det, thick "<<detThick<<" "
	//  <<" layer, ladder, module "
	//  <<layer<<" "<<ladder<<" "<<zindex<<endl;
	//cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
	//  <<pitchX<<" "<<pitchY<<endl;
      }      
      
    } // if subid

    if(printLocal) {
      cout<<"List clusters : "<<endl;
      cout<<"Num Charge Size SizeX SizeY X Y Xmin Xmax Ymin Ymax Edge"
	  <<endl;
    }


    // Loop over clusters
    edmNew::DetSet<SiPixelCluster>::const_iterator clustIt;
    for (clustIt = DSViter->begin(); clustIt != DSViter->end(); clustIt++) {

      sumClusters++;
      numberOfClusters++;
      float ch = float(clustIt->charge())/1000.; // convert ke to electrons
      int size = clustIt->size();
      int sizeX = clustIt->sizeX(); //x=row=rfi, starting from CMSSW6 this is limited to 64
      int sizeY = clustIt->sizeY(); //y=col=z_global, starting from CMSSW6 this is limited to 64
      float x = clustIt->x(); // cluster position as float (int+0.5)
      float y = clustIt->y(); // analog average

      // Returns int index of the cluster min/max  
      // This parameters are affected by the 64*64 limit on the cluster grid, so max-min<=64.
      int minPixelRow = clustIt->minPixelRow(); //x
      int maxPixelRow = clustIt->maxPixelRow();
      int minPixelCol = clustIt->minPixelCol(); //y
      int maxPixelCol = clustIt->maxPixelCol();

      //unsigned int geoId = clustIt->geographicalId(); // always 0?!

      // edge method moved to topologu class
      bool edgeHitX = (topol->isItEdgePixelInX(minPixelRow)) || 
 	(topol->isItEdgePixelInX(maxPixelRow)); 
      bool edgeHitY = (topol->isItEdgePixelInY(minPixelCol)) || 
 	(topol->isItEdgePixelInY(maxPixelCol)); 

            
      if(printLocal) cout<<" Clu "<<numberOfClusters<<" "<<ch<<" "<<size<<" "<<sizeX<<" "<<sizeY<<" "
		    <<x<<" "<<y<<" "<<minPixelRow<<" "<<maxPixelRow<<" "<<minPixelCol<<" "
		    <<maxPixelCol<<" "<<edgeHitX<<" "<<edgeHitY<<endl;


      //bool bigInX=false, bigInY=false;
      //bool edgeHitX2 = false; // edge method moved 
      //bool edgeHitY2 = false; // to topologu class
      //bool edgeInX = false; // edge method moved 
      //bool edgeInY = false; // to topology class

      // Get the pixels in the Cluster
      //const vector<SiPixelCluster::Pixel>& pixelsVec = clustIt->pixels();
      if(printLocal) cout<<" Pixels in this cluster, size = "<< size <<endl;

      // Look at pixels in this cluster. ADC is calibrated, in electrons 
      for (int i = 0;  i < size; ++i) { // loop over pixels
	numberOfPixels++;
	auto pix = clustIt->pixel(i);
	float pixx = pix.x; // index as float=iteger, row index, 0-159
	float pixy = pix.y; // same, col index, 0-415
	float adc = (float(pix.adc)/1000.);

	//int chan = PixelChannelIdentifier::pixelToChannel(int(pixx),int(pixy));
	//bool binInX = (RectangularPixelTopology::isItBigPixelInX(int(pixx)));
	//bool bigInY = (RectangularPixelTopology::isItBigPixelInY(int(pixy)));
	//int roc = rocId(int(pixy),int(pixx));  // column, row

#ifdef HISTOS
	// Pixel histos
	if(myDEBUG) cout<<" pix histos "<<subid<<" "<<layer<<" "<<disk<<endl;
	if (subid==1) {  // barrel
	  if(layer==1) {
	    numOfPixPerDet1++;
	    numOfPixPerLay1++;     
	    hpixcharge1->Fill(adc);
	    hpixMap1->Fill(pixy,pixx);
	    hpixDetMap1->Fill(float(module),float(ladder));
	    
	  } else if(layer==2) {

	    numOfPixPerDet2++;
	    numOfPixPerLay2++;   
	    hpixcharge2->Fill(adc);
	    hpixMap2->Fill(pixy,pixx);
	    hpixDetMap2->Fill(float(module),float(ladder));

	  } else if(layer==3) {

	    numOfPixPerDet3++;
	    numOfPixPerLay3++; 
	    hpixcharge3->Fill(adc);
	    hpixMap3->Fill(pixy,pixx);
	    hpixDetMap3->Fill(float(module),float(ladder));

	  } else if(layer==4) {

	    numOfPixPerDet4++;
	    numOfPixPerLay4++; 
	    hpixcharge4->Fill(adc);
	    hpixMap4->Fill(pixy,pixx);
	    hpixDetMap4->Fill(float(module),float(ladder));

	  }  // if layer

	} else if (subid==2) {  // endcap
	  // pixels

	  numOfPixPerDetF1++;
	  if( (disk>=1) && (disk<=8) ) { // disk1 -+z
	    if(side==1) {
	      numOfPixPerDisk1++;      // d1,-z
	      float tmp = -float(blade);
	      hpixDetMapF1->Fill(tmp,float(ring));
	    } else if(side==2) {
	      numOfPixPerDisk2++; // d1, +z
	      hpixDetMapF1->Fill(float(blade),float(ring));
	    } else cout<<" unknown side "<<side<<endl;

	    hpixcharge5->Fill(adc);
	    if(ring==1)      hpixMapF11->Fill(pixy,pixx);
	    else if(ring==2) hpixMapF12->Fill(pixy,pixx);
	    else if(ring==3) hpixMapF13->Fill(pixy,pixx);
	    else if(ring==4) hpixMapF14->Fill(pixy,pixx);

	  } else if( (disk>=9) && (disk<=12) ) { // disk2 -+z
	    
	    numOfPixPerDetF2++;
	    if(side==1) {
	      numOfPixPerDisk3++;      // d2, -z
	      float tmp=-float(blade);
	      hpixDetMapF2->Fill(tmp,float(ring));
	    } else if(side==2) {
	      numOfPixPerDisk4++; // d2, +z
	      hpixDetMapF2->Fill(float(blade),float(ring));
	    } else cout<<" unknown side "<<side<<endl;

	    hpixcharge6->Fill(adc);

	    if(ring==1)      hpixMapF21->Fill(pixy,pixx);
	    else if(ring==2) hpixMapF22->Fill(pixy,pixx);
	    else if(ring==3) hpixMapF23->Fill(pixy,pixx);
	    else if(ring==4) hpixMapF24->Fill(pixy,pixx);
	    else if(ring==5) hpixMapF25->Fill(pixy,pixx);

	  } else cout<<" (pix) unknown disk "<<disk<<endl;

	} // end if subdet (pixel loop)

#endif // HISTOS
	
	// This looks if there is an edge pixel in the cluster
	//bool edgeInX = topol->isItEdgePixelInX(int(pixx));
	//bool edgeInY = topol->isItEdgePixelInY(int(pixy));
	//if(edgeInX) edgeHitX2=true;
	//if(edgeInY) edgeHitY2=true; 
	
	if(printLocal) cout<<" pix "<<i<<" "<<pixx<<" "<<pixy<<" "<<adc<<endl;
	//" "<<bigInX<<" "<<bigInY<<" "<<edgeInX<<" "<<edgeInY<<endl;
	
      } // pixel loop

#ifdef HISTOS
      // Cluster histos
      if(myDEBUG) cout<<" cluster histos "<<subid<<" "<<layer<<endl;
      if (subid==1) {  // barrel
	if(layer==1) {  // layer
	  
	  hcluDetMap1->Fill(float(module),float(ladder));
	  hcharge1->Fill(ch);
	  hy1->Fill(y);
	  hx1->Fill(x);
	  hsize1->Fill(float(size));
	  hsizex1->Fill(float(sizeX));
	  hsizey1->Fill(float(sizeY));
	  hladder1id->Fill(float(ladder));
	  hz1id->Fill(float(module));

	  numOfClustersPerDet1++;
	  numOfClustersPerLay1++;

	} else if(layer==2) {

	  hcluDetMap2->Fill(float(module),float(ladder));
	  hcharge2->Fill(ch);
	  hy2->Fill(y);
	  hx2->Fill(x);
	  hsize2->Fill(float(size));
	  hsizex2->Fill(float(sizeX));
	  hsizey2->Fill(float(sizeY));
	  hladder2id->Fill(float(ladder));
	  hz2id->Fill(float(module));

	  numOfClustersPerDet2++;
	  numOfClustersPerLay2++;


	} else if(layer==3) {

	  hcluDetMap3->Fill(float(module),float(ladder));
	  hcharge3->Fill(ch);
	  hy3->Fill(y);
	  hx3->Fill(x);
	  hsize3->Fill(float(size));
	  hsizex3->Fill(float(sizeX));
	  hsizey3->Fill(float(sizeY));
	  hladder3id->Fill(float(ladder));
	  hz3id->Fill(float(module));

	  numOfClustersPerDet3++;
	  numOfClustersPerLay3++;


	} else if(layer==4) {

	  hcluDetMap4->Fill(float(module),float(ladder));
	  hcharge4->Fill(ch);
	  hy4->Fill(y);
	  hx4->Fill(x);
	  hsize4->Fill(float(size));
	  hsizex4->Fill(float(sizeX));
	  hsizey4->Fill(float(sizeY));
	  hladder4id->Fill(float(ladder));
	  hz4id->Fill(float(module));

	  numOfClustersPerDet4++;
	  numOfClustersPerLay4++;

	} // end if bpix layer

      } else if(subid==2 ) {  // endcap

	//cout<<disk<<" "<<side<<endl;

	if( (disk>=1) && (disk<=8) ) { // disk 1-8 -+z
	  numOfClustersPerDetF1++;
	  if(side==1)  {
	    numOfClustersPerDisk1++;      // d1,-z
	    float tmp = -float(blade);
	    hcluDetMapF1->Fill(tmp,float(ring));
	  } else if(side==2) {
	    numOfClustersPerDisk2++; // d1, +z
	    hcluDetMapF1->Fill(float(blade),float(ring));
	  } else cout<<" unknown side "<<side<<endl;

	  hcharge5->Fill(ch);
	  hsize5->Fill(float(size));
	  hsizex5->Fill(float(sizeX));
	  hsizey5->Fill(float(sizeY));
	  hy5->Fill(y);
	  hx5->Fill(x);
	  hsizeDetFPix->Fill(float(disk),float(ring),float(size));

	} else if( (disk>=9) && (disk<=12) ) { // disk2 -+z

	  numOfClustersPerDetF2++;
	  if(side==1) {
	    numOfClustersPerDisk3++;      // d2, -z
	    float tmp = -float(blade); // using -blade does not work, blade is uint!
	    hcluDetMapF2->Fill(tmp,float(ring));
	  } else if(side==2) {
	    numOfClustersPerDisk4++; // d2, +z
	    hcluDetMapF2->Fill(float(blade),float(ring));
	  } else cout<<" unknown side "<<side<<endl;

	  hcharge6->Fill(ch);
	  hsize6->Fill(float(size));
	  hsizex6->Fill(float(sizeX));
	  hsizey6->Fill(float(sizeY));
	  hy6->Fill(y);
	  hx6->Fill(x);
	  hsizeDetEPix->Fill(float(disk),float(ring),float(size));

	} else cout<<" (clusters) unknown disk "<<disk<<endl; // end fpix disk 

      } // end if barrel/forward 
      
#endif // HISTOS


    } // clusters 

    if(printLocal) {
      if(layer==1) 
	cout<<"Lay1: number of clusters per det = "<<numOfClustersPerDet1<<endl;
      else if(layer==2) 
	cout<<"Lay2: number of clusters per det = "<<numOfClustersPerDet2<<endl;
      else if(layer==3) 
	cout<<"Lay3: number of clusters per det = "<<numOfClustersPerDet3<<endl;
      else if(layer==4) 
	cout<<"Lay4: number of clusters per det = "<<numOfClustersPerDet4<<endl;
    } // end if printLocal

#ifdef HISTOS
    // Det histos
    if(myDEBUG)cout<<" det histos "<<subid<<" "<<layer<<" "<<disk<<endl;
    if (subid==1 ) {  // barrel
      hxy->Fill(detX,detY);

      if(layer==1) {
	
	++numberOfDetUnits1;
	hclusPerDet1->Fill(float(numOfClustersPerDet1));
	hpixPerDet1->Fill(float(numOfPixPerDet1));
	numOfClustersPerDet1=0;        
	numOfPixPerDet1=0;
	hphiz1->Fill(detZ,detPhi);

      } else if(layer==2) {

	++numberOfDetUnits2;
	hclusPerDet2->Fill(float(numOfClustersPerDet2));
	hpixPerDet2->Fill(float(numOfPixPerDet2));
	numOfClustersPerDet2=0;
	numOfPixPerDet2=0;        
	hphiz2->Fill(detZ,detPhi);

      } else if(layer==3) {

	++numberOfDetUnits3;
	hclusPerDet3->Fill(float(numOfClustersPerDet3));
	hpixPerDet3->Fill(float(numOfPixPerDet3));
	numOfClustersPerDet3=0;
	numOfPixPerDet3=0;
	hphiz3->Fill(detZ,detPhi);
        
	//SK:unused	numOfPixPerLink3=0;        

      } else if(layer==4) {

	++numberOfDetUnits4;
	hclusPerDet4->Fill(float(numOfClustersPerDet4));
	hpixPerDet4->Fill(float(numOfPixPerDet4));
	numOfClustersPerDet4=0;
	numOfPixPerDet4=0;        
	hphiz4->Fill(detZ,detPhi);

      } // layer

    } else { // fpix

      hzr->Fill(detZ,detR);

      if( (disk>=1)&&(disk<=8) ) { // disk1 -+z	
	hclusPerDetF1->Fill(float(numOfClustersPerDetF1));
	hpixPerDetF1->Fill(float(numOfPixPerDetF1));

	if(ring==1) {
	  hclusPerDetF11->Fill(float(numOfClustersPerDetF1));
	  hpixPerDetF11->Fill(float(numOfPixPerDetF1));
	} else if (ring==2) {
	  hclusPerDetF12->Fill(float(numOfClustersPerDetF1));
	  hpixPerDetF12->Fill(float(numOfPixPerDetF1));
	} else if (ring==3) {
	  hclusPerDetF13->Fill(float(numOfClustersPerDetF1));
	  hpixPerDetF13->Fill(float(numOfPixPerDetF1));
	} else if (ring==4) {
	  hclusPerDetF14->Fill(float(numOfClustersPerDetF1));
	  hpixPerDetF14->Fill(float(numOfPixPerDetF1));
	}
	hpixDetFPix->Fill(float(disk),float(ring),float(numOfPixPerDetF1));
	hcluDetFPix->Fill(float(disk),float(ring),float(numOfClustersPerDetF1));

	numOfClustersPerDetF1=0;
	numOfPixPerDetF1=0;        

	if(side==1)      hxy11->Fill(detX,detY);      // d1,-z
	else if(side==2) hxy12->Fill(detX,detY); // d1, +z

      } else if( (disk>=9)&&(disk<=12)) { // disk2 -+z	

	hclusPerDetF2->Fill(float(numOfClustersPerDetF2));
	hpixPerDetF2->Fill(float(numOfPixPerDetF2));
	if(ring==1) {
	  hclusPerDetF21->Fill(float(numOfClustersPerDetF2));
	  hpixPerDetF21->Fill(float(numOfPixPerDetF2));
	} else if (ring==2) {
	  hclusPerDetF22->Fill(float(numOfClustersPerDetF2));
	  hpixPerDetF22->Fill(float(numOfPixPerDetF2));
	} else if (ring==3) {
	  hclusPerDetF23->Fill(float(numOfClustersPerDetF2));
	  hpixPerDetF23->Fill(float(numOfPixPerDetF2));
	} else if (ring==4) {
	  hclusPerDetF24->Fill(float(numOfClustersPerDetF2));
	  hpixPerDetF24->Fill(float(numOfPixPerDetF2));
	} else if (ring==5) {
	  hclusPerDetF25->Fill(float(numOfClustersPerDetF2));
	  hpixPerDetF25->Fill(float(numOfPixPerDetF2));
	}

	hpixDetEPix->Fill(float(disk),float(ring),float(numOfPixPerDetF2));
	hcluDetEPix->Fill(float(disk),float(ring),float(numOfClustersPerDetF2));

	numOfClustersPerDetF2=0;
	numOfPixPerDetF2=0;        
	if(side==1)      hxy21->Fill(detX,detY);      // d1,-z
	else if(side==2) hxy22->Fill(detX,detY); // d1, +z
      }      

    } // end barrel/forward

#endif // HISTOS
    
  } // detunits loop

  if(myDEBUG) {
    cout<<"run "<<run<<" event "<<event<<" bx "<<bx<<" lumi "<<lumiBlock<<" orbit "<<orbit<<" num "<<countEvents<<endl;   
    cout<<"Num of pix "<<numberOfPixels<<" num of clus "<<numberOfClusters<<" num of dets "<<numOf
	<<" count "<<countEvents<<endl;
    cout<<"Number of clusters per      Lay1,2,3: "<<numOfClustersPerLay1<<" "
	<<numOfClustersPerLay2<<" "<<numOfClustersPerLay3<<endl;
    cout<<"Number of pixels per        Lay1,2,3: "<<numOfPixPerLay1<<" "
	<<numOfPixPerLay2<<" "<<numOfPixPerLay3<<endl;
    cout<<"Number of dets with clus in Lay1,2,3: "<<numberOfDetUnits1<<" "
	<<numberOfDetUnits2<<" "<<numberOfDetUnits3<<endl;
    cout<<"Number of clus in disks 1-8,9-12     : "<<numOfClustersPerDisk1<<" "
	<<numOfClustersPerDisk2<<" "<<numOfClustersPerDisk3<<" "<<numOfClustersPerDisk4<<endl;
    // aveCharge1 /= float(numOfClustersPerLay1);
    // aveCharge2 /= float(numOfClustersPerLay2);
    // aveCharge3 /= float(numOfClustersPerLay3);
    // aveCharge4 /= float(numOfClustersPerDisk2 + numOfClustersPerDisk3);
    // aveCharge5 /= float(numOfClustersPerDisk1 + numOfClustersPerDisk4);
    // cout<<" Average charge "<<aveCharge1<<" "<<aveCharge2<<" "<<aveCharge3<<" "
    // 	<<aveCharge4<<" "<<aveCharge5<<endl;
  }
    
#ifdef HISTOS
  
  hdigis->Fill(float(numberOfPixels));  // all pix 
  int tmp1 = numOfPixPerLay1+numOfPixPerLay2+numOfPixPerLay3;
  hdigisB->Fill(float(tmp1));  // pix in bpix
  tmp1 = numOfPixPerDisk1 + numOfPixPerDisk2 + numOfPixPerDisk3 + numOfPixPerDisk4;
  hdigisF->Fill(float(tmp1));   // pix in fpix

  hclus->Fill(float(numberOfClusters)); // clusters fpix+bpix
  tmp1 = numOfClustersPerLay1+numOfClustersPerLay2+numOfClustersPerLay3;
  hclusBPix->Fill(float(tmp1));  // clusters in bpix
  int tmp2 = numOfClustersPerDisk1+numOfClustersPerDisk2
    +numOfClustersPerDisk3+numOfClustersPerDisk4;
  //+numOfClustersPerDisk5+numOfClustersPerDisk6;
  hclusFPix->Fill(float(tmp2));  // clusters in fpix
  
  hclusPerLay1->Fill(float(numOfClustersPerLay1));
  hclusPerLay2->Fill(float(numOfClustersPerLay2));
  hclusPerLay3->Fill(float(numOfClustersPerLay3));
  hclusPerLay4->Fill(float(numOfClustersPerLay4));

  hpixPerLay1->Fill(float(numOfPixPerLay1));
  hpixPerLay2->Fill(float(numOfPixPerLay2));
  hpixPerLay3->Fill(float(numOfPixPerLay3));
  hpixPerLay4->Fill(float(numOfPixPerLay4));

  hclusPerDisk1->Fill(float(numOfClustersPerDisk1));
  hclusPerDisk2->Fill(float(numOfClustersPerDisk2));
  hclusPerDisk3->Fill(float(numOfClustersPerDisk3));
  hclusPerDisk4->Fill(float(numOfClustersPerDisk4));

  // Select only events with more tha 3 clusters	  
  if(myDEBUG) cout<<" number of clusters "<<numberOfClusters<<endl;
  if(numberOfClusters>3) {
    hbx1->Fill(float(bx));
    hlumi1->Fill(float(lumiBlock));
  } // if num of clusters

#endif // HISTOS

  if(myDEBUG)cout<<" end "<<endl;
} // end 

//define this as a plug-in
DEFINE_FWK_MODULE(PixClusterTestPhase2);
