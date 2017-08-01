//--------------------------------------------
// File: PixRecHitTest.cc
//--------------------------------------------
#include <memory>
#include <string>
#include <iostream>

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
//#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
//#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
//#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


using namespace std;

//#define DO_HISTO Defined in *.h
#define DO_HISTO

#ifdef DO_HISTO
// For ROOT
#include <TH2.h>
#include <TH1.h>
#include <TProfile.h>
#endif

class PixRecHitTest : public edm::EDAnalyzer {
 public:
  
  explicit PixRecHitTest(const edm::ParameterSet& conf);
  
  virtual ~PixRecHitTest();
  
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginJob();
  virtual void endJob();

 private:
  edm::ParameterSet conf_;
  edm::InputTag src_;
  bool print, phase1_;
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelRecHit>>tPixelRecHit;

  TH1F *hpixid,*hpixsubid,
    *hlayerid,
    *hladder1id,*hladder2id,*hladder3id,*hladder4id,
    *hz1id,*hz2id,*hz3id,*hz4id;                                                                                
  TH1F *hcharge1,*hcharge2, *hcharge3, *hcharge4;
  TH1F *hpixCharge1,*hpixCharge2, *hpixCharge3, *hpixCharge4, *hpixCharge1big;
  TH1F *hxpos1,*hxpos2,*hxpos3,*hxpos4,*hypos1,*hypos2,*hypos3,*hypos4;
  TH1F *hsize1,*hsize2,*hsize3,*hsize4,
    *hsizex1,*hsizex2,*hsizex3,*hsizex4,
    *hsizey1,*hsizey2,*hsizey3,*hsizey4;
 
  TH1F *hrecHitsPerDet1,*hrecHitsPerDet2,*hrecHitsPerDet3,*hrecHitsPerDet4;
  TH1F *hrecHitsPerLay1,*hrecHitsPerLay2,*hrecHitsPerLay3,*hrecHitsPerLay4;
  TH1F *hdetsPerLay1,*hdetsPerLay2,*hdetsPerLay3,*hdetsPerLay4;
 
  TH1F *hdetr, *hdetz;  
  //Forward endcaps
  TH1F *hdetrF, *hdetzF;
  TH1F *hdisk, *hblade, *hmodule, *hpanel, *hside;
  TH1F *hcharge1F,*hcharge2F,*hcharge3F;
  TH1F *hpixCharge1F,*hpixCharge2F,*hpixCharge3F;
  TH1F *hxpos1F,*hxpos2F,*hxpos3F,*hypos1F,*hypos2F,*hypos3F;
  TH1F *hsize1F,*hsize2F,*hsize3F,
    *hsizex1F,*hsizex2F,*hsizex3F,
    *hsizey1F,*hsizey2F,*hsizey3F;
  TH1F *hrecHitsPerDet1F,*hrecHitsPerDet2F,*hrecHitsPerDet3F;
  TH1F *hrecHitsPerLay1F,*hrecHitsPerLay2F,*hrecHitsPerLay3F;
  TH1F *hdetsPerLay1F,*hdetsPerLay2F,*hdetsPerLay3F;

  TH2F *hxy,*hxy1,*hxy2,*hxy3,*hxy4, *hphiz1, *hphiz2, *hphiz3, *hphiz4; // bpix 
  TH2F *hzr, *hxy11, *hxy12, *hxy21, *hxy22, *hxy31, *hxy32;  // fpix 
  TH2F *hrhMap1, *hrhMap2, *hrhMap3, *hrhMap4;

  // Alignment 
  TH1F *hAlignErrorX1,*hAlignErrorX2,*hAlignErrorX3, *hAlignErrorX4,*hAlignErrorX5;
  TH1F *hAlignErrorX6,*hAlignErrorX7,*hAlignErrorX8, *hAlignErrorX9,*hAlignErrorX10;
  TH1F *hAlignErrorY1,*hAlignErrorY2,*hAlignErrorY3, *hAlignErrorY4,*hAlignErrorY5;
  TH1F *hAlignErrorY6,*hAlignErrorY7,*hAlignErrorY8, *hAlignErrorY9,*hAlignErrorY10;
  TH1F *hErrorX1,*hErrorX2,*hErrorX3,*hErrorX4,*hErrorX5;
  TH1F *hErrorX6,*hErrorX7,*hErrorX8,*hErrorX9,*hErrorX10;
  TH1F *hErrorY1,*hErrorY2,*hErrorY3,*hErrorY4,*hErrorY5;
  TH1F *hErrorY6,*hErrorY7,*hErrorY8,*hErrorY9,*hErrorY10;

  TProfile *hErrorXB, *hErrorXF, *hErrorYB, *hErrorYF;
  TProfile *hAErrorXB, *hAErrorXF, *hAErrorYB, *hAErrorYF;

};

//----------------------------------------------------------------------
PixRecHitTest::PixRecHitTest(edm::ParameterSet const& conf) : 
  conf_(conf),
  src_( conf.getParameter<edm::InputTag>( "src" ) ) { 
    print = conf.getUntrackedParameter<bool>("Verbosity",false);
    phase1_ = conf.getUntrackedParameter<bool>("phase1",false);
    cout<<" Verbosity "<<print<<endl;
    tPixelRecHit = consumes<edmNew::DetSetVector<SiPixelRecHit>>( src_ );

}
//----------------------------------------------------------------------
// Virtual destructor needed.
PixRecHitTest::~PixRecHitTest() { }  
//---------------------------------------------------------------------
// ------------ method called at the begining   ------------
void PixRecHitTest::beginJob() {
  cout << "Initialize PixelRecHitTest " <<endl;

#ifdef DO_HISTO
  edm::Service<TFileService> fs;

  // Histos go to a subdirectory "PixRecHits") 
  //TFileDirectory subDir = fs->mkdir( "mySubDirectory" );
  //TFileDirectory subSubDir = subDir.mkdir( "mySubSubDirectory" );
  //h_pt    = fs->make<TH1F>( "pt"  , "p_{t}", 100,  0., ptMax_ );

  hpixid = fs->make<TH1F>( "hpixid", "Pix det id", 10, 0., 10.);
  hpixsubid = fs->make<TH1F>( "hpixsubid", "Pix Barrel id", 10, 0., 10.);
  hlayerid = fs->make<TH1F>( "hlayerid", "Pix layer id", 10, 0., 10.);
  hladder1id = fs->make<TH1F>( "hladder1id", "Ladder L1 id", 50, 0., 50.);
  hladder2id = fs->make<TH1F>( "hladder2id", "Ladder L2 id", 50, 0., 50.);
  hladder3id = fs->make<TH1F>( "hladder3id", "Ladder L3 id", 50, 0., 50.);
  hladder4id = fs->make<TH1F>( "hladder4id", "Ladder L4 id", 70, 0., 70.);
  hz1id = fs->make<TH1F>( "hz1id", "Z-index id L1", 10, 0., 10.);
  hz2id = fs->make<TH1F>( "hz2id", "Z-index id L2", 10, 0., 10.);
  hz3id = fs->make<TH1F>( "hz3id", "Z-index id L3", 10, 0., 10.);
  hz4id = fs->make<TH1F>( "hz4id", "Z-index id L4", 10, 0., 10.);
   
  hrecHitsPerDet1 = fs->make<TH1F>( "hrecHitsPerDet1", "RecHits per det l1",
                            200, -0.5, 199.5);
  hrecHitsPerDet2 = fs->make<TH1F>( "hrecHitsPerDet2", "RecHits per det l2",
                            200, -0.5, 199.5);
  hrecHitsPerDet3 = fs->make<TH1F>( "hrecHitsPerDet3", "RecHits per det l3",
                            200, -0.5, 199.5);
  hrecHitsPerDet4 = fs->make<TH1F>( "hrecHitsPerDet4", "RecHits per det l4",
                            200, -0.5, 199.5);
  hrecHitsPerLay1 = fs->make<TH1F>( "hrecHitsPerLay1", "RecHits per layer l1",
                            2000, -0.5, 1999.5);
  hrecHitsPerLay2 = fs->make<TH1F>( "hrecHitsPerLay2", "RecHits per layer l2",
                            2000, -0.5, 1999.5);
  hrecHitsPerLay3 = fs->make<TH1F>( "hrecHitsPerLay3", "RecHits per layer l3",
                            2000, -0.5, 1999.5);
  hrecHitsPerLay4 = fs->make<TH1F>( "hrecHitsPerLay4", "RecHits per layer l4",
                            2000, -0.5, 1999.5);
  hdetsPerLay1 = fs->make<TH1F>( "hdetsPerLay1", "Full dets per layer l1",
                           161, -0.5, 160.5);
  hdetsPerLay2 = fs->make<TH1F>( "hdetsPerLay2", "Full dets per layer l2",
                           257, -0.5, 256.5);
  hdetsPerLay3 = fs->make<TH1F>( "hdetsPerLay3", "Full dets per layer l3",
                           353, -0.5, 352.5);
  hdetsPerLay4 = fs->make<TH1F>( "hdetsPerLay4", "Full dets per layer l4",
                           353, -0.5, 352.5);
  
  hcharge1 = fs->make<TH1F>( "hcharge1", "Clu charge l1", 200, 0.,200.); //in ke
  hcharge2 = fs->make<TH1F>( "hcharge2", "Clu charge l2", 200, 0.,200.);
  hcharge3 = fs->make<TH1F>( "hcharge3", "Clu charge l3", 200, 0.,200.);
  hcharge4 = fs->make<TH1F>( "hcharge4", "Clu charge l4", 200, 0.,200.);
  hpixCharge1 = fs->make<TH1F>( "hpixCharge1", "pix charge l1", 50, 0.,50.); //in ke
  hpixCharge2 = fs->make<TH1F>( "hpixCharge2", "pix charge l2", 50, 0.,50.); //in ke
  hpixCharge3 = fs->make<TH1F>( "hpixCharge3", "pix charge l3", 50, 0.,50.); //in ke
  hpixCharge4 = fs->make<TH1F>( "hpixCharge4", "pix charge l4", 50, 0.,50.); //in ke
  hpixCharge1big = fs->make<TH1F>( "hpixCharge1big", "big pix charge l1", 50, 0.,50.); //in ke
  
  hypos1 = fs->make<TH1F>( "hypos1", "Layer 1 y pos", 700,-3.5,3.5);
  hypos2 = fs->make<TH1F>( "hypos2", "Layer 2 y pos", 700,-3.5,3.5);
  hypos3 = fs->make<TH1F>( "hypos3", "Layer 3 y pos", 700,-3.5,3.5);
  hypos4 = fs->make<TH1F>( "hypos4", "Layer 4 y pos", 700,-3.5,3.5);
   
  hxpos1 = fs->make<TH1F>( "hxpos1", "Layer 1 x pos", 200,-1.,1.);
  hxpos2 = fs->make<TH1F>( "hxpos2", "Layer 2 x pos", 200,-1.,1.);
  hxpos3 = fs->make<TH1F>( "hxpos3", "layer 3 x pos", 200,-1.,1.);
  hxpos4 = fs->make<TH1F>( "hxpos4", "layer 4 x pos", 200,-1.,1.);
 
  hsize1 = fs->make<TH1F>( "hsize1", "layer 1 clu size",100,-0.5,99.5);
  hsize2 = fs->make<TH1F>( "hsize2", "layer 2 clu size",100,-0.5,99.5);
  hsize3 = fs->make<TH1F>( "hsize3", "layer 3 clu size",100,-0.5,99.5);
  hsize4 = fs->make<TH1F>( "hsize4", "layer 4 clu size",100,-0.5,99.5);
  hsizex1 = fs->make<TH1F>( "hsizex1", "lay1 clu size in x",
                      10,-0.5,9.5);
  hsizex2 = fs->make<TH1F>( "hsizex2", "lay2 clu size in x",
                      10,-0.5,9.5);
  hsizex3 = fs->make<TH1F>( "hsizex3", "lay3 clu size in x",
                      10,-0.5,9.5);
  hsizex4 = fs->make<TH1F>( "hsizex4", "lay4 clu size in x",
                      10,-0.5,9.5);
  hsizey1 = fs->make<TH1F>( "hsizey1", "lay1 clu size in y",
                      20,-0.5,19.5);
  hsizey2 = fs->make<TH1F>( "hsizey2", "lay2 clu size in y",
                      20,-0.5,19.5);
  hsizey3 = fs->make<TH1F>( "hsizey3", "lay3 clu size in y",
                      20,-0.5,19.5);
  hsizey4 = fs->make<TH1F>( "hsizey4", "lay4 clu size in y",
                      20,-0.5,19.5);
  // 2D
  // fpix 
  hzr = fs->make<TH2F>("hzr"," ",240,-60.,60.,68,0.,17.);  // x-y plane
  hxy11 = fs->make<TH2F>("hxy11"," ",320,-16.,16.,320,-16.,16.); // x-y pla 
  hxy12 = fs->make<TH2F>("hxy12"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy21 = fs->make<TH2F>("hxy21"," ",320,-16.,16.,320,-16.,16.); // x-y pl
  hxy22 = fs->make<TH2F>("hxy22"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy31 = fs->make<TH2F>("hxy31"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy32 = fs->make<TH2F>("hxy32"," ",320,-16.,16.,320,-16.,16.); // x-y plae
  // bpix
  hxy = fs->make<TH2F>("hxy"," ",340,-17.,17.,340,-17.,17.);  // x-y plane
  hxy1 = fs->make<TH2F>("hxy1"," ",80,-4.,4.,80,-4.,4.);  // x-y plane
  hxy2 = fs->make<TH2F>("hxy2"," ",160,-8.,8.,160,-8.,8.);  // x-y plane
  hxy3 = fs->make<TH2F>("hxy3"," ",240,-12.,12.,240,-12.,12.);  // x-y plane
  hxy4 = fs->make<TH2F>("hxy4"," ",340,-17.,17.,340,-17.,17.);  // x-y plane
  hphiz1 = fs->make<TH2F>("hphiz1"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz2 = fs->make<TH2F>("hphiz2"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz3 = fs->make<TH2F>("hphiz3"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz4 = fs->make<TH2F>("hphiz4"," ",108,-27.,27.,140,-3.5,3.5);

  hrhMap1 = fs->make<TH2F>("hrhMap1"," ",350,-3.5,3.5,100,-1.,1.);
  hrhMap2 = fs->make<TH2F>("hrhMap2"," ",350,-3.5,3.5,100,-1.,1.);
  hrhMap3 = fs->make<TH2F>("hrhMap3"," ",350,-3.5,3.5,100,-1.,1.);
  hrhMap4 = fs->make<TH2F>("hrhMap4"," ",350,-3.5,3.5,100,-1.,1.);

  hAlignErrorX1 = fs->make<TH1F>( "hAlignErrorX1", "Align error Layer 1 X", 100,0.0,100.);
  hAlignErrorY1 = fs->make<TH1F>( "hAlignErrorY1", "Align error Layer 1 Y", 100,0.0,100.);
  hAlignErrorX2 = fs->make<TH1F>( "hAlignErrorX2", "Align error Layer 2 X", 100,0.0,100.);
  hAlignErrorY2 = fs->make<TH1F>( "hAlignErrorY2", "Align error Layer 2 Y", 100,0.0,100.);
  hAlignErrorX3 = fs->make<TH1F>( "hAlignErrorX3", "Align error Layer 3 X", 100,0.0,100.);
  hAlignErrorY3 = fs->make<TH1F>( "hAlignErrorY3", "Align error Layer 3 Y", 100,0.0,100.);
  hAlignErrorX4 = fs->make<TH1F>( "hAlignErrorX4", "Align error Layer 4 X", 100,0.0,100.);
  hAlignErrorY4 = fs->make<TH1F>( "hAlignErrorY4", "Align error Layer 4 Y", 100,0.0,100.);
  hAlignErrorX5 = fs->make<TH1F>( "hAlignErrorX5", "Align error Disk -1 X", 100,0.0,100.);
  hAlignErrorY5 = fs->make<TH1F>( "hAlignErrorY5", "Align error Disk -1 Y", 100,0.0,100.);
  hAlignErrorX6 = fs->make<TH1F>( "hAlignErrorX6", "Align error Disk -2 X", 100,0.0,100.);
  hAlignErrorY6 = fs->make<TH1F>( "hAlignErrorY6", "Align error Disk -2 Y", 100,0.0,100.);
  hAlignErrorX7 = fs->make<TH1F>( "hAlignErrorX7", "Align error Disk -3 X", 100,0.0,100.);
  hAlignErrorY7 = fs->make<TH1F>( "hAlignErrorY7", "Align error Disk -3 Y", 100,0.0,100.);
  hAlignErrorX8 = fs->make<TH1F>( "hAlignErrorX8", "Align error Disk +1 X", 100,0.0,100.);
  hAlignErrorY8 = fs->make<TH1F>( "hAlignErrorY8", "Align error Disk +1 Y", 100,0.0,100.);
  hAlignErrorX9 = fs->make<TH1F>( "hAlignErrorX9", "Align error Disk +2 X", 100,0.0,100.);
  hAlignErrorY9 = fs->make<TH1F>( "hAlignErrorY9", "Align error Disk +2 Y", 100,0.0,100.);
  hAlignErrorX10 = fs->make<TH1F>( "hAlignErrorX10", "Align error Disk +3 X", 100,0.0,100.);
  hAlignErrorY10 = fs->make<TH1F>( "hAlignErrorY10", "Align error Disk +3 Y", 100,0.0,100.);

  hErrorX1 = fs->make<TH1F>( "hErrorX1", "Error Layer 1 X", 100,0.0,100.);
  hErrorY1 = fs->make<TH1F>( "hErrorY1", "Error Layer 1 Y", 100,0.0,100.);
  hErrorX2 = fs->make<TH1F>( "hErrorX2", "Error Layer 2 X", 100,0.0,100.);
  hErrorY2 = fs->make<TH1F>( "hErrorY2", "Error Layer 2 Y", 100,0.0,100.);
  hErrorX3 = fs->make<TH1F>( "hErrorX3", "Error Layer 3 X", 100,0.0,100.);
  hErrorY3 = fs->make<TH1F>( "hErrorY3", "Error Layer 3 Y", 100,0.0,100.);
  hErrorX4 = fs->make<TH1F>( "hErrorX4", "Error Layer 4 X", 100,0.0,100.);
  hErrorY4 = fs->make<TH1F>( "hErrorY4", "Error Layer 4 Y", 100,0.0,100.);
  hErrorX5 = fs->make<TH1F>( "hErrorX5", "Error Disk -1 X", 100,0.0,100.);
  hErrorY5 = fs->make<TH1F>( "hErrorY5", "Error Disk -1 Y", 100,0.0,100.);
  hErrorX6 = fs->make<TH1F>( "hErrorX6", "Error Disk -2 X", 100,0.0,100.);
  hErrorY6 = fs->make<TH1F>( "hErrorY6", "Error Disk -2 Y", 100,0.0,100.);
  hErrorX7 = fs->make<TH1F>( "hErrorX7", "Error Disk -3 X", 100,0.0,100.);
  hErrorY7 = fs->make<TH1F>( "hErrorY7", "Error Disk -3 Y", 100,0.0,100.);
  hErrorX8 = fs->make<TH1F>( "hErrorX8", "Error Disk +1 X", 100,0.0,100.);
  hErrorY8 = fs->make<TH1F>( "hErrorY8", "Error Disk +1 Y", 100,0.0,100.);
  hErrorX9 = fs->make<TH1F>( "hErrorX9", "Error Disk +2 X", 100,0.0,100.);
  hErrorY9 = fs->make<TH1F>( "hErrorY9", "Error Disk +2 Y", 100,0.0,100.);
  hErrorX10 = fs->make<TH1F>( "hErrorX10", "Error Disk +3 X", 100,0.0,100.);
  hErrorY10 = fs->make<TH1F>( "hErrorY10", "Error Disk +3 Y", 100,0.0,100.);
   
  hdetr = fs->make<TH1F>("hdetr","det r",150,0.,15.);
  hdetz = fs->make<TH1F>("hdetz","det z",520,-26.,26.);
  
  // Forward edcaps
  hdetrF = fs->make<TH1F>("hdetrF","Fdet r",150,5.,20.);
  hdetzF = fs->make<TH1F>("hdetzF","Fdet z",600,-60.,60.);
 
  hdisk = fs->make<TH1F>( "hdisk", "FPix disk id", 10, 0., 10.);
  hblade = fs->make<TH1F>( "hblade", "FPix blade id", 30, 0., 30.);
  hmodule = fs->make<TH1F>( "hmodule", "FPix plaq. id", 10, 0., 10.);
  hpanel = fs->make<TH1F>( "hpanel", "FPix panel id", 10, 0., 10.);
  hside = fs->make<TH1F>( "hside", "FPix size id", 10, 0., 10.);
 
  hcharge1F = fs->make<TH1F>( "hcharge1F", "Clu charge d1", 200, 0.,200.); //in ke
  hcharge2F = fs->make<TH1F>( "hcharge2F", "Clu charge d2", 200, 0.,200.);
  hcharge3F = fs->make<TH1F>( "hcharge3F", "Clu charge d3", 200, 0.,200.);
  hypos1F = fs->make<TH1F>( "hypos1F", "Disk 1 y pos", 700,-3.5,3.5);
  hypos2F = fs->make<TH1F>( "hypos2F", "Disk 2 y pos", 700,-3.5,3.5);
  hypos3F = fs->make<TH1F>( "hypos3F", "Disk 3 y pos", 700,-3.5,3.5);
  hxpos1F = fs->make<TH1F>( "hxpos1F", "Disk 1 x pos", 200,-1.,1.);
  hxpos2F = fs->make<TH1F>( "hxpos2F", "Disk 2 x pso", 200,-1.,1.);
  hxpos3F = fs->make<TH1F>( "hxpos3F", "Disk 3 x pos", 200,-1.,1.);
  hsize1F = fs->make<TH1F>( "hsize1F", "Disk 1 clu size",100,-0.5,99.5);
  hsize2F = fs->make<TH1F>( "hsize2F", "Disk 2 clu size",100,-0.5,99.5);
  hsize3F = fs->make<TH1F>( "hsize3F", "Disk 3 clu size",100,-0.5,99.5);
  hsizex1F = fs->make<TH1F>( "hsizex1F", "d1 clu size in x",
                      10,-0.5,9.5);
  hsizex2F = fs->make<TH1F>( "hsizex2F", "d2 clu size in x",
                      10,-0.5,9.5);
  hsizex3F = fs->make<TH1F>( "hsizex3F", "d3 clu size in x",
                      10,-0.5,9.5);
  hsizey1F = fs->make<TH1F>( "hsizey1F", "d1 clu size in y",
                      20,-0.5,19.5);
  hsizey2F = fs->make<TH1F>( "hsizey2F", "d2 clu size in y",
                      20,-0.5,19.5);
  hsizey3F = fs->make<TH1F>( "hsizey3F", "d3 clu size in y",
                      20,-0.5,19.5);
  hpixCharge1F = fs->make<TH1F>( "hpixCharge1F", "pix charge d1", 50, 0.,50.); //in ke
  hpixCharge2F = fs->make<TH1F>( "hpixCharge2F", "pix charge d2", 50, 0.,50.); //in ke
  hpixCharge3F = fs->make<TH1F>( "hpixCharge3F", "pix charge d3", 50, 0.,50.); //in ke

  hrecHitsPerDet1F = fs->make<TH1F>( "hrecHitsPerDet1F", "RecHits per det l1",
                            200, -0.5, 199.5);
  hrecHitsPerDet2F = fs->make<TH1F>( "hrecHitsPerDet2F", "RecHits per det l2",
                            200, -0.5, 199.5);
  hrecHitsPerDet3F = fs->make<TH1F>( "hrecHitsPerDet3F", "RecHits per det l3",
                            200, -0.5, 199.5);
  hrecHitsPerLay1F = fs->make<TH1F>( "hrecHitsPerLay1F", "RecHits per layer l1",
                            2000, -0.5, 1999.5);
  hrecHitsPerLay2F = fs->make<TH1F>( "hrecHitsPerLay2F", "RecHits per layer l2",
                            2000, -0.5, 1999.5);
  hrecHitsPerLay3F = fs->make<TH1F>( "hrecHitsPerLay3F", "RecHits per layer l3",
                            2000, -0.5, 1999.5);
  hdetsPerLay1F = fs->make<TH1F>( "hdetsPerLay1F", "Full dets per layer l1",
                           161, -0.5, 160.5);
  hdetsPerLay2F = fs->make<TH1F>( "hdetsPerLay2F", "Full dets per layer l2",
                           257, -0.5, 256.5);
  hdetsPerLay3F = fs->make<TH1F>( "hdetsPerLay3F", "Full dets per layer l3",
                           257, -0.5, 256.5);

  hErrorXB = fs->make<TProfile>("hErrorXB","bpix x errors per ladder",220,0.,220.,0.0,1000.);
  hErrorXF = fs->make<TProfile>("hErrorXF","fpix x errors per ladder",100,0.,100.,0.0,1000.);
  hErrorYB = fs->make<TProfile>("hErrorYB","bpix y errors per ladder",220,0.,220.,0.0,1000.);
  hErrorYF = fs->make<TProfile>("hErrorYF","fpix y errors per ladder",100,0.,100.,0.0,1000.);

  hAErrorXB = fs->make<TProfile>("hAErrorXB","bpix x errors per ladder",220,0.,220.,0.0,1000.);
  hAErrorXF = fs->make<TProfile>("hAErrorXF","fpix x errors per ladder",100,0.,100.,0.0,1000.);
  hAErrorYB = fs->make<TProfile>("hAErrorYB","bpix y errors per ladder",220,0.,220.,0.0,1000.);
  hAErrorYF = fs->make<TProfile>("hAErrorYF","fpix y errors per ladder",100,0.,100.,0.0,1000.);

  cout<<" book histos "<<endl;
#endif

}

//-----------------------------------------------------------------------
void PixRecHitTest::endJob(){
  cout << " End PixelRecHitTest " << endl;
}

//---------------------------------------------------------------------
// Functions that gets called by framework every event
void PixRecHitTest::analyze(const edm::Event& e, 
			      const edm::EventSetup& es) {
  using namespace edm;
  const bool localPrint = false;
  //const bool localPrint = true;

  // Get event setup (to get global transformation)
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoH;
  es.get<TrackerTopologyRcd>().get(tTopoH);
  const TrackerTopology *tTopo=tTopoH.product();

  edm::Handle< edmNew::DetSetVector<SiPixelRecHit> > recHitColl;
  //e.getByLabel( src_ , recHitColl);
  e.getByToken(tPixelRecHit , recHitColl);

  if(print) cout <<" FOUND "<<(recHitColl.product())->dataSize()<<" Pixel Hits"
		 <<endl;

  SiPixelRecHitCollection::const_iterator recHitIdIterator      = (recHitColl.product())->begin();
  SiPixelRecHitCollection::const_iterator recHitIdIteratorEnd   = (recHitColl.product())->end();


   int numberOfDetUnits = 0;
   int numOfRecHits = 0;

   int numberOfDetUnits1 = 0;
   int numOfRecHitsPerDet1=0;
   int numOfRecHitsPerLay1=0;
   int numberOfDetUnits2 = 0;
   int numOfRecHitsPerDet2=0;
   int numOfRecHitsPerLay2=0;
   int numberOfDetUnits3 = 0;
   int numOfRecHitsPerDet3=0;
   int numOfRecHitsPerLay3=0;
   int numberOfDetUnits4 = 0;
   int numOfRecHitsPerDet4=0;
   int numOfRecHitsPerLay4=0;
   int numberOfDetUnits1F = 0;
   int numOfRecHitsPerDet1F=0;
   int numOfRecHitsPerLay1F=0;
   int numberOfDetUnits2F = 0;
   int numOfRecHitsPerDet2F=0;
   int numOfRecHitsPerLay2F=0;
   int numberOfDetUnits3F = 0;
   int numOfRecHitsPerDet3F=0;
   int numOfRecHitsPerLay3F=0;

  // Loop over Detector IDs
  for ( ; recHitIdIterator != recHitIdIteratorEnd; recHitIdIterator++) {
    SiPixelRecHitCollection::DetSet detset = *recHitIdIterator;

    DetId detId = DetId(detset.detId()); // Get the Detid object
    unsigned int detType=detId.det();    // det type, tracker=1
    unsigned int subid=detId.subdetId(); //subdetector type, barrel=1, fpix=2
  
    if(detType!=1) 
      cout<<" wrong det id "<<detType<<endl;; //look only at tracker
    if( !((subid==1) || (subid==2))) 
      cout<<" wrong sub det id "<<subid<<endl;  // look only at bpix&fpix

    if(print) cout <<"     Det ID " << detId.rawId() <<endl;
    
    //  Get rechits
   if( detset.empty() ) continue;

    // Get the geometrical information for this det
    const PixelGeomDetUnit * theGeomDet =
      dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
    double detZ = theGeomDet->surface().position().z();
    double detR = theGeomDet->surface().position().perp();
     
    //const BoundPlane& plane = theGeomDet->surface(); //for transf.  unused   
    //double detThick = theGeomDet->specificSurface().bounds().thickness(); unused
    
    //const RectangularPixelTopology * topol =
    //dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
 
    const PixelTopology * topol = &(theGeomDet->specificTopology());

    //int cols = theGeomDet->specificTopology().ncolumns(); UNUSED
    //int rows = theGeomDet->specificTopology().nrows();

    float alignErrorX= 0., alignErrorY=0.;
    LocalError lape = theGeomDet->localAlignmentError();
    //cout<<lape.valid()<<endl;
    if (lape.valid()) {
      if(lape.xx()>0.) alignErrorX = sqrt(lape.xx())*1E4;
      //float tmp12= sqrt(lape.xy())*1E4;
      if(lape.yy()>0.) alignErrorY = sqrt(lape.yy())*1E4;
      if(print) cout<<" Alignment errors "<<alignErrorX<<" "<<alignErrorY<<endl;
      //cout<<" Alignment errors "<<alignErrorX<<" "<<alignErrorY<<endl;
    }

    unsigned int layer=0, disk=0, ladder=0, zindex=0, blade=0, panel=0, side=0, module=0;
    if(subid==1) {  // Subdet it, pix barrel=1 
      ++numberOfDetUnits;
      
      layer=tTopo->pxbLayer(detId);
      ladder=tTopo->pxbLadder(detId);
      zindex=tTopo->pxbModule(detId);
      //PixelBarrelName pbn(detId,tTopo,phase1_);

      // PXBDetId pdetId = PXBDetId(detId);
      // //unsigned int detTypeP=pdetId.det();   unused 
      // //unsigned int subidP=pdetId.subdetId(); unused
      // // Barell layer = 1,2,3
      // layer=pdetId.layer();
      // // Barrel ladder id 1-20,32,44.
      // ladder=pdetId.ladder();
      // // Barrel Z-index=1,8
      // zindex=pdetId.module();
      if(zindex<5) side=1; else side=2;

      if(localPrint)
	cout<<" Layer "<<layer<<" ladder "<<ladder<<" z "<<zindex<<endl;
      //<<pdetId.rawId()<<" "<<pdetId.null()<<detTypeP<<" "<<subidP<<" "<<endl;

#ifdef DO_HISTO
      hdetr->Fill(detR);
      hdetz->Fill(detZ);
      //hcolsB->Fill(float(cols));
      //hyposB->Fill(float(rows));
      hlayerid->Fill(float(layer));
#endif

    }  else {  // FPIX -------------------------------------

      // test cols & rows
      //cout<<" det z/r "<<detZ<<" "<<detR<<" "<<detThick<<" "
      //  <<cols<<" "<<rows<<endl;

      disk=tTopo->pxfDisk(detId); //1,2,3
      blade=tTopo->pxfBlade(detId); //1-24
      module=tTopo->pxfModule(detId); //
      side=tTopo->pxfSide(detId); //size=1 for -z, 2 for +z
      panel=tTopo->pxfPanel(detId); //panel=1
      //PixelEndcapName pen(detId,tTopo,phase1_);

      // PXFDetId pdetId = PXFDetId(detId.rawId());
      // disk=pdetId.disk(); //1,2,3
      // blade=pdetId.blade(); //1-24
      // side=pdetId.side(); //size=1 for -z, 2 for +z
      // panel=pdetId.panel(); //panel=1,2
      // unsigned int module=pdetId.module(); // plaquette

      if(print) cout<<" forward hit "<<side<<" "<<disk<<" "<<blade<<" "
		    <<panel<<" "<<module<<endl;

#ifdef DO_HISTO
      hdetrF->Fill(detR);
      hdetzF->Fill(detZ);
      hdisk->Fill(float(disk));
      hblade->Fill(float(blade));
      hmodule->Fill(float(module));
      hpanel->Fill(float(panel));
      hside->Fill(float(side));
#endif

    } // end BPix FPix if

#ifdef DO_HISTO
    // det histos
    if(layer==1) {
      hladder1id->Fill(float(ladder));
      hz1id->Fill(float(zindex));
      hAlignErrorX1->Fill(alignErrorX);
      hAlignErrorY1->Fill(alignErrorY);
      hAErrorXB->Fill(float(ladder+(110*(side-1))),alignErrorX);
      hAErrorYB->Fill(float(ladder+(110*(side-1))),alignErrorY);
      ++numberOfDetUnits1;
      numOfRecHitsPerDet1=0; 

    } else if(layer==2) {
      hladder2id->Fill(float(ladder));
      hz2id->Fill(float(zindex));
      hAlignErrorX2->Fill(alignErrorX);
      hAlignErrorY2->Fill(alignErrorY);
      hAErrorXB->Fill(float(ladder+25+(110*(side-1))),alignErrorX);
      hAErrorYB->Fill(float(ladder+25+(110*(side-1))),alignErrorY);
      ++numberOfDetUnits2;
      numOfRecHitsPerDet2=0;

    } else if(layer==3) {
      hladder3id->Fill(float(ladder));
      hz3id->Fill(float(zindex));
      hAlignErrorX3->Fill(alignErrorX);
      hAlignErrorY3->Fill(alignErrorY);
      hAErrorXB->Fill(float(ladder+60+(110*(side-1))),alignErrorX);
      hAErrorYB->Fill(float(ladder+60+(110*(side-1))),alignErrorY);
      ++numberOfDetUnits3;
      numOfRecHitsPerDet3=0;

    } else if(layer==4) {
      hladder4id->Fill(float(ladder));
      hz4id->Fill(float(zindex));
      hAlignErrorX4->Fill(alignErrorX);
      hAlignErrorY4->Fill(alignErrorY);
      //hAErrorXB->Fill(float(ladder+60+(110*(side-1))),alignErrorX);
      //hAErrorYB->Fill(float(ladder+60+(110*(side-1))),alignErrorY);
      ++numberOfDetUnits4;
      numOfRecHitsPerDet4=0;

    } else if(disk==1) {
      ++numberOfDetUnits1F;
      numOfRecHitsPerDet1F=0;
      if(side==1) {
	hAlignErrorX5->Fill(alignErrorX);
	hAlignErrorY5->Fill(alignErrorY);
	hAErrorXF->Fill(float(blade+25),alignErrorX);
	hAErrorYF->Fill(float(blade+25),alignErrorY);
      } else {
	hAlignErrorX8->Fill(alignErrorX);
	hAlignErrorY8->Fill(alignErrorY);
	hAErrorXF->Fill(float(blade+50),alignErrorX);
	hAErrorYF->Fill(float(blade+50),alignErrorY);
      }

    } else if(disk==2) {
      ++numberOfDetUnits2F;
      numOfRecHitsPerDet2F=0;
      if(side==1) {
	hAlignErrorX6->Fill(alignErrorX);
	hAlignErrorY6->Fill(alignErrorY);
	hAErrorXF->Fill(float(blade),alignErrorX);
	hAErrorYF->Fill(float(blade),alignErrorY);
      } else {
	hAlignErrorX9->Fill(alignErrorX);
	hAlignErrorY9->Fill(alignErrorY);
	hAErrorXF->Fill(float(blade+75),alignErrorX);
	hAErrorYF->Fill(float(blade+75),alignErrorY);
      }
    } else if(disk==3) {
      ++numberOfDetUnits3F;
      numOfRecHitsPerDet3F=0;
      if(side==1) {
	hAlignErrorX7->Fill(alignErrorX);
	hAlignErrorY7->Fill(alignErrorY);
	//hAErrorXF->Fill(float(blade),alignErrorX);
	//hAErrorYF->Fill(float(blade),alignErrorY);
      } else {
	hAlignErrorX10->Fill(alignErrorX);
	hAlignErrorY10->Fill(alignErrorY);
	//hAErrorXF->Fill(float(blade+75),alignErrorX);
	//hAErrorYF->Fill(float(blade+75),alignErrorY);
      }
    }
#endif  
   
    //----Loop over rechits for this detId
    SiPixelRecHitCollection::DetSet::const_iterator pixeliter=detset.begin();
    SiPixelRecHitCollection::DetSet::const_iterator rechitRangeIteratorEnd   = detset.end();
    for(;pixeliter!=rechitRangeIteratorEnd;++pixeliter) { //loop on the rechit

      numOfRecHits++;

      // RecHit local position is now transient, 
      // one needs to run tracking to get position OR rerun localreco 
      LocalPoint lp = pixeliter->localPosition();
      LocalError le = pixeliter->localPositionError();
      float xRecHit = lp.x();
      float yRecHit = lp.y();
      float xerror = sqrt(le.xx())*1E4;
      float yerror = sqrt(le.yy())*1E4;
      if(print) cout<<" RecHit: "<<numOfRecHits<<" x/y "<<xRecHit<<" "<<yRecHit<<" errors x/y "<<xerror<<" "<<yerror<<endl;
      
      //MeasurementPoint mp = topol->measurementPosition(xRecHit,yRecHit);
      GlobalPoint gp = theGeomDet->surface().toGlobal(Local3DPoint(lp));
      if(print) cout<<" global rechit "<<gp.x()<<" "<<gp.y()<<" "<<gp.z()<<endl;

      // Get cluster 
      edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = 
	pixeliter->cluster();
	
      float ch = (clust->charge())/1000.; // convert ke to electrons
      int size = clust->size();
      int sizeX = clust->sizeX();
      int sizeY = clust->sizeY();
      float xClu = clust->x();
      float yClu = clust->y();
      int maxPixelCol = clust->maxPixelCol();
      int maxPixelRow = clust->maxPixelRow();
      int minPixelCol = clust->minPixelCol();
      int minPixelRow = clust->minPixelRow();
      
      // unsigned int geoId = clust->geographicalId(); // alsways 0

      // edge method moved to topologu class
      bool edgeHitX = (topol->isItEdgePixelInX(minPixelRow)) ||
        (topol->isItEdgePixelInX(maxPixelRow));
      bool edgeHitY = (topol->isItEdgePixelInY(minPixelCol)) ||
        (topol->isItEdgePixelInY(maxPixelCol));

      if(print) 
	cout<<"Clu: charge "<<ch<<" size "<<size<<" size x/y "<<sizeX<<" "
	    <<sizeY<<" meas. "<<xClu<<" "<<yClu<<" edge "<<edgeHitX<<" "<<edgeHitY<<endl;
      
      if(print) cout<<" pixels:"<<endl;

      // Get the pixels in the Cluster
      const vector<SiPixelCluster::Pixel>& pixelsVec = clust->pixels();
      //if(localPrint) cout<<" Pixels in this cluster "<<endl;
      map<unsigned int, float, less<unsigned int> > chanMap;  // Channel map
      // Look at pixels in this cluster. ADC is calibrated, in electrons 
      for (unsigned int i = 0;  i < pixelsVec.size(); ++i) {
	float pixx = pixelsVec[i].x; // index as a float so = i+0.5
	float pixy = pixelsVec[i].y;
	float adc = ((pixelsVec[i].adc)/1000); // in kelec.

	// OLD way
	//int chan = PixelChannelIdentifier::pixelToChannel(int(pixx),int(pixy));
	bool bigInX = topol->isItBigPixelInX(int(pixx));
	bool bigInY = topol->isItBigPixelInY(int(pixy));
	
	bool edgeInX = topol->isItEdgePixelInX(int(pixx));
	bool edgeInY = topol->isItEdgePixelInY(int(pixy));
	  
	if(print)
	  cout<<i<<" index "<<pixx<<" "<<pixy<<" adc "<<adc<<" edge "
	      <<edgeInX<<" "<<edgeInY<<" big "<<bigInX<<" "<<bigInY<<endl;
	

	//if(print && sizeX==1 && bigInX) 
	//cout<<" single big x "<<xClu<<" "<<pixx<<" "<<endl;
	//if(print && sizeY==1 && bigInY) 
	//cout<<" single big y "<<yClu<<" "<<pixy<<" "<<endl;
#ifdef DO_HISTO
	// pixel histos
	if(layer==1) {
	  hpixCharge1->Fill(adc);
	  //if(bigInX || bigInY) hpixCharge1big->Fill(adc);
	} else if(layer==2) {
	  hpixCharge2->Fill(adc);
	} else if(layer==3) {
	  hpixCharge3->Fill(adc);
	} else if(layer==4) {
	  hpixCharge4->Fill(adc);
	} else if(disk==1) {
	  hpixCharge1F->Fill(adc);
	} else if(disk==2) {
	  hpixCharge2F->Fill(adc);
	} else if(disk==3) {
	  hpixCharge3F->Fill(adc);
	}
#endif
      } // End pixel loop
      
#ifdef DO_HISTO
      // cluster rechit histos
      if(layer==1) {
	hcharge1->Fill(ch);
	hypos1->Fill(yRecHit);
	hxpos1->Fill(xRecHit);
	  hrhMap1->Fill(yRecHit,xRecHit);
	hsize1->Fill(float(size));
	hsizex1->Fill(float(sizeX));
	hsizey1->Fill(float(sizeY));
	numOfRecHitsPerDet1++;
	numOfRecHitsPerLay1++;
	hErrorX1->Fill(xerror);
	hErrorY1->Fill(yerror);
	hErrorXB->Fill(float(ladder+(110*(side-1))),xerror);
	hErrorYB->Fill(float(ladder+(110*(side-1))),yerror);
	hxy->Fill(gp.x(),gp.y());
	hxy1->Fill(gp.x(),gp.y());
	hphiz1->Fill(gp.z(),gp.phi());

      } else if(layer==2) {  // layer 2
	
	hcharge2->Fill(ch);
	hypos2->Fill(yRecHit);
	hxpos2->Fill(xRecHit);
	hrhMap2->Fill(yRecHit,xRecHit);
	hsize2->Fill(float(size));
	hsizex2->Fill(float(sizeX));
	hsizey2->Fill(float(sizeY));
	numOfRecHitsPerDet2++;
	numOfRecHitsPerLay2++;
	hErrorX2->Fill(xerror);
	hErrorY2->Fill(yerror);
	hErrorXB->Fill(float(ladder+25+(110*(side-1))),xerror);
	hErrorYB->Fill(float(ladder+25+(110*(side-1))),yerror);
	hxy->Fill(gp.x(),gp.y());
	hxy2->Fill(gp.x(),gp.y());
	hphiz2->Fill(gp.z(),gp.phi());

      } else if(layer==3) {  // Layer 3
	
	hcharge3->Fill(ch);
	hypos3->Fill(yRecHit);
	hxpos3->Fill(xRecHit);
 	//if(ladder==24 && zindex==1)	
	  hrhMap3->Fill(yRecHit,xRecHit);
	hsize3->Fill(float(size));
	hsizex3->Fill(float(sizeX));
	hsizey3->Fill(float(sizeY));
	numOfRecHitsPerDet3++;
	numOfRecHitsPerLay3++;
	hErrorX3->Fill(xerror);
	hErrorY3->Fill(yerror);
	hErrorXB->Fill(float(ladder+60+(110*(side-1))),xerror);
	hErrorYB->Fill(float(ladder+60+(110*(side-1))),yerror);
	hxy->Fill(gp.x(),gp.y());
 	//if(ladder==24 && zindex==1)	
	  hxy3->Fill(gp.x(),gp.y());
	hphiz3->Fill(gp.z(),gp.phi());

      } else if(layer==4) {  // Layer 4
	
	hcharge4->Fill(ch);
	hypos4->Fill(yRecHit);
	hxpos4->Fill(xRecHit);
	hrhMap4->Fill(yRecHit,xRecHit);
	hsize4->Fill(float(size));
	hsizex4->Fill(float(sizeX));
	hsizey4->Fill(float(sizeY));
	numOfRecHitsPerDet4++;
	numOfRecHitsPerLay4++;
	hErrorX4->Fill(xerror);
	hErrorY4->Fill(yerror);
	//hErrorXB->Fill(float(ladder+60+(110*(side-1))),xerror);
	//hErrorYB->Fill(float(ladder+60+(110*(side-1))),yerror);
	hxy->Fill(gp.x(),gp.y());
	hxy4->Fill(gp.x(),gp.y());
	hphiz4->Fill(gp.z(),gp.phi());

      } else if(disk==1) {
	
	hcharge1F->Fill(ch);
	hypos1F->Fill(yRecHit);
	hxpos1F->Fill(xRecHit);
	hsize1F->Fill(float(size));
	hsizex1F->Fill(float(sizeX));
	hsizey1F->Fill(float(sizeY));
	hzr->Fill(gp.z(),gp.perp());
	if(side==1) { // -z
	  hErrorX5->Fill(xerror);
	  hErrorY5->Fill(yerror);
	  hErrorXF->Fill(float(blade+25),xerror);
	  hErrorYF->Fill(float(blade+25),yerror);
	  hxy11->Fill(gp.x(),gp.y());
	} else { // +z
	  hErrorX8->Fill(xerror);
	  hErrorY8->Fill(yerror);
	  hErrorXF->Fill(float(blade+50),xerror);
	  hErrorYF->Fill(float(blade+50),yerror);
	  hxy12->Fill(gp.x(),gp.y());
	}
	numOfRecHitsPerDet1F++;
	numOfRecHitsPerLay1F++;

      } else if(disk==2) {  // disk 2
	
	hcharge2F->Fill(ch);
	hypos2F->Fill(yRecHit);
	hxpos2F->Fill(xRecHit);
	hsize2F->Fill(float(size));
	hsizex2F->Fill(float(sizeX));
	hsizey2F->Fill(float(sizeY));
	numOfRecHitsPerDet2F++;
	numOfRecHitsPerLay2F++;
	hzr->Fill(gp.z(),gp.perp());
	if(side==1) { // -z
	  hErrorX6->Fill(xerror);
	  hErrorY6->Fill(yerror);
	  hErrorXF->Fill(float(blade),xerror);
	  hErrorYF->Fill(float(blade),yerror);
	  hxy21->Fill(gp.x(),gp.y());
	} else { // +z
	  hErrorX9->Fill(xerror);
	  hErrorY9->Fill(yerror);
	  hErrorXF->Fill(float(blade+75),xerror);
	  hErrorYF->Fill(float(blade+75),yerror);
	  hxy22->Fill(gp.x(),gp.y());
	}

      } else if(disk==3) {  // disk 3
	
	hcharge3F->Fill(ch);
	hypos3F->Fill(yRecHit);
	hxpos3F->Fill(xRecHit);
	hsize3F->Fill(float(size));
	hsizex3F->Fill(float(sizeX));
	hsizey3F->Fill(float(sizeY));
	numOfRecHitsPerDet3F++;
	numOfRecHitsPerLay3F++;
	hzr->Fill(gp.z(),gp.perp());
	if(side==1) { // -z
	  hErrorX7->Fill(xerror);
	  hErrorY7->Fill(yerror);
	  //hErrorXF->Fill(float(blade),xerror);
	  //hErrorYF->Fill(float(blade),yerror);
	  hxy31->Fill(gp.x(),gp.y());
	} else { // +z
	  hErrorX10->Fill(xerror);
	  hErrorY10->Fill(yerror);
	  //hErrorXF->Fill(float(blade+75),xerror);
	  //hErrorYF->Fill(float(blade+75),yerror);
	  hxy32->Fill(gp.x(),gp.y());
	}
      }
#endif  
      
    } // End RecHit loop
    
#ifdef DO_HISTO
    if(layer==1) 
      hrecHitsPerDet1->Fill(float(numOfRecHitsPerDet1));
    else if(layer==2) 
      hrecHitsPerDet2->Fill(float(numOfRecHitsPerDet2));
    else if(layer==3) 
      hrecHitsPerDet3->Fill(float(numOfRecHitsPerDet3));
    else if(layer==4) 
      hrecHitsPerDet4->Fill(float(numOfRecHitsPerDet4));
    else if(disk==1) 
      hrecHitsPerDet1F->Fill(float(numOfRecHitsPerDet1F));
    else if(disk==2) 
      hrecHitsPerDet2F->Fill(float(numOfRecHitsPerDet2F));
    else if(disk==3) 
      hrecHitsPerDet3F->Fill(float(numOfRecHitsPerDet3F));
#endif  
    
    
  } // End Det loop

#ifdef DO_HISTO
  if(print) cout<<" Rechits per layer "<<numOfRecHitsPerLay1<<" "
		<<numOfRecHitsPerLay2<<" "<<numOfRecHitsPerLay3<<" "<<numOfRecHitsPerLay4<<" "
		<<numOfRecHitsPerLay1F<<" "<<numOfRecHitsPerLay2F<<" "<<numOfRecHitsPerLay3F<<endl;
  hrecHitsPerLay1->Fill(float(numOfRecHitsPerLay1));
  hrecHitsPerLay2->Fill(float(numOfRecHitsPerLay2));
  hrecHitsPerLay3->Fill(float(numOfRecHitsPerLay3));
  hrecHitsPerLay4->Fill(float(numOfRecHitsPerLay4));
  hdetsPerLay1 ->Fill(float(numberOfDetUnits1));
  hdetsPerLay2 ->Fill(float(numberOfDetUnits2));
  hdetsPerLay3 ->Fill(float(numberOfDetUnits3));
  hdetsPerLay4 ->Fill(float(numberOfDetUnits4));

  hrecHitsPerLay1F->Fill(float(numOfRecHitsPerLay1F));
  hrecHitsPerLay2F->Fill(float(numOfRecHitsPerLay2F));
  hrecHitsPerLay3F->Fill(float(numOfRecHitsPerLay3F));
  hdetsPerLay1F ->Fill(float(numberOfDetUnits1F));
  hdetsPerLay2F ->Fill(float(numberOfDetUnits2F));
  hdetsPerLay3F ->Fill(float(numberOfDetUnits3F));

#endif  

}

//define this as a plug-in
DEFINE_FWK_MODULE(PixRecHitTest);
