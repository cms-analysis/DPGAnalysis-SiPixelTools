// -*- C++ -*-
//
// Package:    PixDigisTest
// Class:      PixDigisTest
// 
/**\class PixDigisTest PixDigisTest.cc 

 Description: Test pixel digis. 
 Barrel & Forward digis. Uses root histos.
 Adopted for the new simLinks. 
 Added the online detector index. d.k. 11/09
 Works with CMSSW_7
 New detector ID.
 Modified to use "byToken"
 Modify for 4 layers.
*/
//
// Original Author:  d.k.
//         Created:  Jan CET 2006
//
//
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"

// my includes
//#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" 

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelNameUpgrade.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapNameUpgrade.h"

// data formats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
//#include "DataFormats/SiPixelDigi/interface/PixelDigiCollection.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"

// For the big pixel recongnition
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"

// for simulated Tracker hits
//#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

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

#define HISTOS
#define L1
#define HLT
#define SINGLE_MODULES
#define DCOLS

using namespace std;

// Enable this to look at simlinks (link simhit->digis)
// Can be used only with simulated data.
//#define USE_SIM_LINKS


//
// class decleration
//

class PixDigisTest : public edm::EDAnalyzer {

public:

  explicit PixDigisTest(const edm::ParameterSet&);
  ~PixDigisTest();
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(); 
  int moduleIndex(int ladder,int module); 
  bool validIndex(int index, bool print); 
  int rocId(int pixy,int pixx);  // 0-15, column, row

private:
  // ----------member data ---------------------------
  bool PRINT;
  edm::EDGetTokenT<edm::DetSetVector<PixelDigi>> tPixelDigi;
#ifdef USE_SIM_LINKS
  edm::EDGetTokenT< edm::DetSetVector<PixelDigiSimLink> > tPixelDigiSimLink;
#endif
  float count0, count1, count2, count3;
  int select1, select2;

#ifdef HISTOS

  //TFile* hFile;
  TH1F *hdetunit;
  TH1F *heloss1,*heloss2, *heloss3, *heloss4;
  TH1F *hneloss1,*hneloss2, *hneloss3, *hneloss4;
  TH1F *helossF1,*helossF2,*helossF3;
  TH1F *hpixid,*hpixsubid,*hlayerid,*hshellid,*hsectorid,
    *hladder1id,*hladder2id,*hladder3id,*hladder4id,
    *hz1id,*hz2id,*hz3id,*hz4id;
  TH1F *hcols1,*hcols2,*hcols3,*hrows1,*hrows2,*hrows3,*hcols4,*hrows4;
  TH1F *hcolsF1,*hcolsF2,*hcolsF3,*hrowsF1,*hrowsF2,*hrowsF3;
  TH1F *hdigisPerDet1,*hdigisPerDet2,*hdigisPerDet3,*hdigisPerDet4;
  TH1F *hdigisPerLay1,*hdigisPerLay2,*hdigisPerLay3,*hdigisPerLay4;
  TH1F *hdetsPerLay1,*hdetsPerLay2,*hdetsPerLay3,*hdetsPerLay4;
  TH1F *hdigisPerDetF1,*hdigisPerDetF2,*hdigisPerDetF3;
  TH1F *hdigisPerLayF1,*hdigisPerLayF2,*hdigisPerLayF3;
  TH1F *hdetsPerLayF1,*hdetsPerLayF2,*hdetsPerLayF3;
  TH1F *hdetr, *hdetz, *hdetrF, *hdetzF;
  TH1F *hcolsB,  *hrowsB,  *hcolsF,  *hrowsF;
  TH1F *hcols1Many,*hhitsPerDcol, *hhitsPerHitDcol, *hhitDcolsPerModule; // , *hrows1big, *heloss1bigx, *heloss1bigy,*hcols11;
  TH1F *hsimlinks, *hfract;
  TH1F *hblade1, *hblade2, *hblade3;
  

  TH2F *hpdetMap4,*hpdetMap3,*hpdetMap2,*hpdetMap1; 
  TH2F *hdetMap4,*hdetMap3,*hdetMap2,*hdetMap1; 
  TH2F *hpixMap1, *hpixMap2, *hpixMap3,*hpixMap4;

  TH2F *hpdetMaps4,*hpdetMaps3,*hpdetMaps2,*hpdetMaps1; 

  //* hpixMapNoise; 
  TH2F *hxy, *hphiz1, *hphiz2, *hphiz3, *hphiz4; // bpix 
  TH2F *hzr, *hxy11, *hxy12, *hxy21, *hxy22, *hxy31, *hxy32;  // fpix 

#ifdef SINGLE_MODULES
  // single mosules 
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

  TH1F *hevent, *hlumi, *horbit, *hbx0, *hlumi0, *hlumi1,*hbx1,*hbx2,*hbx3,*hbx4,*hbx5,*hbx6;
  TH1F *hdets, *hdigis, *hdigis0, *hdigis1, *hdigis2,*hdigis3,*hdigis4,*hdigis5; 

#ifdef DCOLS
  TH1F *hDcolsCount, *hAllDcols;
#endif 

  TProfile *hadc1ls,*hadc2ls,*hadc3ls,*hadc4ls,*hadc0ls; 
  TProfile *hadc1bx,*hadc2bx,*hadc3bx,*hadc4bx,*hadc0bx; 


#endif

  edm::InputTag src_;  
  bool phase1_;
  int countFullDcols;
  int oneModule[416][160];
  int dCols[120][16][26];

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PixDigisTest::PixDigisTest(const edm::ParameterSet& iConfig) {

  PRINT = iConfig.getUntrackedParameter<bool>("Verbosity",false);
  src_ =  iConfig.getParameter<edm::InputTag>( "src" );
  tPixelDigi = consumes <edm::DetSetVector<PixelDigi>> (src_);
#ifdef USE_SIM_LINKS
  tPixelDigiSimLink = consumes < edm::DetSetVector<PixelDigiSimLink> > ( src_);
#endif 
  phase1_ = iConfig.getUntrackedParameter<bool>( "phase1",false);
  select1 = iConfig.getUntrackedParameter<int>("Select1",0);
  select2 = iConfig.getUntrackedParameter<int>("Select2",0);

  cout<<" Construct PixDigisTest "<<endl;
}


PixDigisTest::~PixDigisTest() {
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  cout<<" Destroy PixDigisTest "<<endl;

}

//
// member functions
// decode a simplified ROC address
int PixDigisTest::rocId(int col, int row) {
  int rocRow = row/80;
  int rocCol = col/52;
  int rocId = rocCol + rocRow*8;
  return rocId;
 }
//      int roc = rocId(int(pixy),int(pixx));  // 0-15, column, row


// this is just to turn the L1 ladder&module into a 1D index 
int PixDigisTest::moduleIndex(int ladder, int module) {
  int index=-1;
  if(module>0) index = module-1;
  else         index = abs(module)-1+4;
  if(ladder>0) index = index + (ladder-1)*10;
  else         index = index + (abs(ladder)-1)*10 + 60;
  return index;
}
//
bool PixDigisTest::validIndex(int index, bool print = false) {
  bool valid=true;
  int module = index%10;
  int ladder = index/10;
  if(module<0 || module>7)  valid=false;
  if(ladder<0 || ladder>11) valid=false;
  if(print) {
    module += 1;
    ladder += 1;
    if(module>4) module = -(module - 4);
    if(ladder>6) ladder = -(ladder - 6);
    cout<<index<<" module "<<(module)<<" ladder "<<(ladder)<<endl;
  }
  return valid;
}
//
// ------------ method called at the begining   ------------
void PixDigisTest::beginJob() {

   using namespace edm;
   cout << "Initialize PixDigisTest " <<endl;
   count0=count1=count2=count3=0;
   countFullDcols=0;

   for(int col=0;col<416;++col)
     for(int row=0;row<160;++row)
       oneModule[col][row]=0;

   for(int ind=0;ind<120;++ind)
     for(int roc=0;roc<16;++roc)
       for(int dcol=0;dcol<26;++dcol)
	 dCols[ind][roc][dcol]=0;


#ifdef HISTOS
  edm::Service<TFileService> fs;

  // Histos go to a subdirectory "PixRecHits")
  //TFileDirectory subDir = fs->mkdir( "mySubDirectory" );
  //TFileDirectory subSubDir = subDir.mkdir( "mySubSubDirectory" );

   // put here whatever you want to do at the beginning of the job
  //hFile = new TFile ( "histo.root", "RECREATE" );
     
    hdetunit = fs->make<TH1F>( "hdetunit", "Det unit", 1000,
                              302000000.,302300000.);
    hpixid = fs->make<TH1F>( "hpixid", "Pix det id", 10, 0., 10.);
    hpixsubid = fs->make<TH1F>( "hpixsubid", "Pix Barrel id", 10, 0., 10.);
    hlayerid = fs->make<TH1F>( "hlayerid", "Pix layer id", 10, 0., 10.);
    hsectorid = fs->make<TH1F>( "hsectorid", "Pix sector ", 10, 0., 10.);
    hshellid = fs->make<TH1F>( "hshellid", "Shell", 5, 0., 5.);
    hladder1id = fs->make<TH1F>( "hladder1id", "Ladder L1 id", 23, -11.5, 11.5);
    hladder2id = fs->make<TH1F>( "hladder2id", "Ladder L2 id", 35, -17.5, 17.5);
    hladder3id = fs->make<TH1F>( "hladder3id", "Ladder L3 id", 47, -23.5, 23.5);
    hladder4id = fs->make<TH1F>( "hladder4id", "Ladder L4 id", 67, -33.5, 33.5);
    hz1id = fs->make<TH1F>( "hz1id", "Z-index id L1", 11, -5.5, 5.5);
    hz2id = fs->make<TH1F>( "hz2id", "Z-index id L2", 11, -5.5, 5.5);
    hz3id = fs->make<TH1F>( "hz3id", "Z-index id L3", 11, -5.5, 5.5);
    hz4id = fs->make<TH1F>( "hz4id", "Z-index id L4", 11, -5.5, 5.5);
 
    hdigisPerDet1 = fs->make<TH1F>( "hdigisPerDet1", "Digis per det l1", 
			      1000, -0.5, 999.5);
    hdigisPerDet2 = fs->make<TH1F>( "hdigisPerDet2", "Digis per det l2", 
			      600, -0.5, 599.5);
    hdigisPerDet3 = fs->make<TH1F>( "hdigisPerDet3", "Digis per det l3", 
			      400, -0.5, 399.5);
    hdigisPerDet4 = fs->make<TH1F>( "hdigisPerDet4", "Digis per det l4", 
			      400, -0.5, 399.5);
    //
    //const float maxSize=199.5;
    const float maxSize=39999.5;
    hdigisPerLay1 = fs->make<TH1F>( "hdigisPerLay1", "Digis per layer l1", 
			      1000, -0.5, maxSize);
    hdigisPerLay2 = fs->make<TH1F>( "hdigisPerLay2", "Digis per layer l2", 
			      1000, -0.5, maxSize);
    hdigisPerLay3 = fs->make<TH1F>( "hdigisPerLay3", "Digis per layer l3", 
			      1000, -0.5, maxSize);
    hdigisPerLay4 = fs->make<TH1F>( "hdigisPerLay4", "Digis per layer l4", 
			      1000, -0.5, maxSize);
    hdetsPerLay1 = fs->make<TH1F>( "hdetsPerLay1", "Full dets per layer l1", 
			      161, -0.5, 160.5);
    hdetsPerLay2 = fs->make<TH1F>( "hdetsPerLay2", "Full dets per layer l2", 
			      257, -0.5, 256.5);
    hdetsPerLay3 = fs->make<TH1F>( "hdetsPerLay3", "Full dets per layer l3", 
			      353, -0.5, 352.5);
    hdetsPerLay4 = fs->make<TH1F>( "hdetsPerLay4", "Full dets per layer l4", 
			      513, -0.5, 512.5);

    hdigisPerDetF1 = fs->make<TH1F>( "hdigisPerDetF1", "Digis per det d1", 
			      200, -0.5, 199.5);
    hdigisPerDetF2 = fs->make<TH1F>( "hdigisPerDetF2", "Digis per det d2", 
			      200, -0.5, 199.5);
    hdigisPerDetF3 = fs->make<TH1F>( "hdigisPerDetF3", "Digis per det d3", 
			      200, -0.5, 199.5);
    hdigisPerLayF1 = fs->make<TH1F>( "hdigisPerLayF1", "Digis per layer d1", 
			      2000, -0.5, 1999.5);
    hdigisPerLayF2 = fs->make<TH1F>( "hdigisPerLayF2", "Digis per layer d2", 
			      2000, -0.5, 1999.5);
    hdigisPerLayF3 = fs->make<TH1F>( "hdigisPerLayF3", "Digis per layer d3", 
			      2000, -0.5, 1999.5);
    hdetsPerLayF1 = fs->make<TH1F>( "hdetsPerLayF1", "Full dets per layer d1", 
			      161, -0.5, 160.5);
    hdetsPerLayF2 = fs->make<TH1F>( "hdetsPerLayF2", "Full dets per layer d2", 
			      257, -0.5, 256.5);
    hdetsPerLayF3 = fs->make<TH1F>( "hdetsPerLayF3", "Full dets per layer d3", 
			      257, -0.5, 256.5);

    heloss1 = fs->make<TH1F>( "heloss1", "Pix charge l1", 256, 0., 256.);
    heloss2 = fs->make<TH1F>( "heloss2", "Pix charge l2", 256, 0., 256.);
    heloss3 = fs->make<TH1F>( "heloss3", "Pix charge l3", 256, 0., 256.);
    heloss4 = fs->make<TH1F>( "heloss4", "Pix charge l4", 256, 0., 256.);
    hneloss1 = fs->make<TH1F>( "hneloss1", "Pix noise charge l1", 256, 0., 256.);
    hneloss2 = fs->make<TH1F>( "hneloss2", "Pix noise charge l2", 256, 0., 256.);
    hneloss3 = fs->make<TH1F>( "hneloss3", "Pix noise charge l3", 256, 0., 256.);
    hneloss4 = fs->make<TH1F>( "hneloss4", "Pix noise charge l4", 256, 0., 256.);
    //heloss1bigx = fs->make<TH1F>( "heloss1bigx", "L1 big-x pix", 256, 0., 256.);
    //heloss1bigy = fs->make<TH1F>( "heloss1bigy", "L1 big-y pix", 256, 0., 256.);

    hcols1 = fs->make<TH1F>( "hcols1", "Layer 1 cols", 500,-1.5,498.5);
    hcols2 = fs->make<TH1F>( "hcols2", "Layer 2 cols", 500,-1.5,498.5);
    hcols3 = fs->make<TH1F>( "hcols3", "Layer 3 cols", 500,-1.5,498.5);
    hcols4 = fs->make<TH1F>( "hcols4", "Layer 4 cols", 500,-1.5,498.5);
    hcols1Many = fs->make<TH1F>( "hcols1Many", "Layer 1 cols with many hits", 500,-1.5,498.5);

    hhitsPerDcol= fs->make<TH1F>( "hhitsPerDcol","Layer 1: hits per dcol",200,-1.5,198.5);
    hhitsPerHitDcol= fs->make<TH1F>( "hhitsPerHitDcol","Layer 1: hits per hit dcol",200,-1.5,198.5);
    hhitDcolsPerModule= fs->make<TH1F>( "hhitDcolsPerModule","Layer 1: hit dcol per Hit Module",200,-1.5,198.5);
 
    hrows1 = fs->make<TH1F>( "hrows1", "Layer 1 rows", 200,-1.5,198.5);
    hrows2 = fs->make<TH1F>( "hrows2", "Layer 2 rows", 200,-1.5,198.5);
    hrows3 = fs->make<TH1F>( "hrows3", "layer 3 rows", 200,-1.5,198.5);
    hrows4 = fs->make<TH1F>( "hrows4", "layer 4 rows", 200,-1.5,198.5);
    //hrows1big = fs->make<TH1F>( "hrows1big", "Layer 1 big rows", 200,-1.5,198.5);
 
    hblade1 = fs->make<TH1F>( "hblade1", "blade num, disk1", 60, 0., 60.);
    hblade2 = fs->make<TH1F>( "hblade2", "blade num, disk2", 60, 0., 60.);
    hblade3 = fs->make<TH1F>( "hblade3", "blade num, disk3", 60, 0., 60.);

    helossF1 = fs->make<TH1F>( "helossF1", "Pix charge d1", 100, 0., 300.);
    helossF2 = fs->make<TH1F>( "helossF2", "Pix charge d2", 100, 0., 300.);
    helossF3 = fs->make<TH1F>( "helossF3", "Pix charge d3", 100, 0., 300.);
    hcolsF1 = fs->make<TH1F>( "hcolsF1", "Disk 1 cols", 500,-1.5,498.5);
    hcolsF2 = fs->make<TH1F>( "hcolsF2", "Disk 2 cols", 500,-1.5,498.5);
    hcolsF3 = fs->make<TH1F>( "hcolsF3", "Disk 3 cols", 500,-1.5,498.5);
    hrowsF1 = fs->make<TH1F>( "hrowsF1", "Disk 1 rows", 200,-1.5,198.5);
    hrowsF2 = fs->make<TH1F>( "hrowsF2", "Disk 2 rows", 200,-1.5,198.5);
    hrowsF3 = fs->make<TH1F>( "hrowsF3", "Disk 3 rows", 200,-1.5,198.5);

    hdetr = fs->make<TH1F>("hdetr","det r",180,0.,18.);
    hdetz = fs->make<TH1F>("hdetz","det z",520,-26.,26.);
    hdetrF = fs->make<TH1F>("hdetrF","det r",150,0.,15.);
    hdetzF = fs->make<TH1F>("hdetzF","det z",700,-70.,70.);

    hcolsB = fs->make<TH1F>("hcolsB","cols per bar det",450,0.,450.);
    hrowsB = fs->make<TH1F>("hrowsB","rows per bar det",200,0.,200.);
    hcolsF = fs->make<TH1F>("hcolsF","cols per for det",450,0.,450.);
    hrowsF = fs->make<TH1F>("hrowsF","rows per for det",200,0.,200.);

    hsimlinks = fs->make<TH1F>("hsimlinks"," track ids",200,0.,200.);
    hfract = fs->make<TH1F>("hfract"," track rractions",100,0.,1.);
    //                                             mod      ladder
    hdetMap1 = fs->make<TH2F>("hdetMap1"," ",9,-4.5,4.5,13,-6.5,6.5);
    hdetMap1->SetOption("colz");
    hdetMap2 = fs->make<TH2F>("hdetMap2"," ",9,-4.5,4.5,29,-14.5,14.5);
    hdetMap2->SetOption("colz");
    hdetMap3 = fs->make<TH2F>("hdetMap3"," ",9,-4.5,4.5,45,-22.5,22.5);
    hdetMap3->SetOption("colz");
    hdetMap4 = fs->make<TH2F>("hdetMap4"," ",9,-4.5,4.5,65,-32.5,32.5);
    hdetMap4->SetOption("colz");

    hpdetMap1 = fs->make<TH2F>("hpdetMap1"," ",9,-4.5,4.5,13,-6.5,6.5);
    hpdetMap1->SetOption("colz");
    hpdetMap2 = fs->make<TH2F>("hpdetMap2"," ",9,-4.5,4.5,29,-14.5,14.5);
    hpdetMap2->SetOption("colz");
    hpdetMap3 = fs->make<TH2F>("hpdetMap3"," ",9,-4.5,4.5,45,-22.5,22.5);
    hpdetMap3->SetOption("colz");
    hpdetMap4 = fs->make<TH2F>("hpdetMap4"," ",9,-4.5,4.5,65,-32.5,32.5);
    hpdetMap4->SetOption("colz");

    hpixMap1 = fs->make<TH2F>("hpixMap1"," ",416,0.,416.,160,0.,160.);
    hpixMap1->SetOption("colz");
    hpixMap2 = fs->make<TH2F>("hpixMap2"," ",416,0.,416.,160,0.,160.);
    hpixMap2->SetOption("colz");
    hpixMap3 = fs->make<TH2F>("hpixMap3"," ",416,0.,416.,160,0.,160.);
    hpixMap3->SetOption("colz");
    hpixMap4 = fs->make<TH2F>("hpixMap4"," ",416,0.,416.,160,0.,160.);
    hpixMap4->SetOption("colz");

    hpdetMaps1 = fs->make<TH2F>("hpdetMaps1","hits in l1 with adc<0",9,-4.5,4.5,13,-6.5,6.5);
    hpdetMaps1->SetOption("colz");
    hpdetMaps2 = fs->make<TH2F>("hpdetMaps2","l1 with many hits per col",9,-4.5,4.5,13,-6.5,6.5);
    hpdetMaps2->SetOption("colz");


    //hpixMapNoise = fs->make<TH2F>("hpixMapNoise"," ",416,0.,416.,160,0.,160.);
    //hpixMapNoise->SetOption("colz");

    //htest = fs->make<TH2F>("htest"," ",10,0.,10.,20,0.,20.);
    //htest2 = fs->make<TH2F>("htest2"," ",10,0.,10.,300,0.,300.);
    //htest->SetOption("colz");
    //htest2->SetOption("colz");

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
#endif

  hevent = fs->make<TH1F>("hevent","event",1000,0,10000000.);
  horbit = fs->make<TH1F>("horbit","orbit",100, 0,100000000.);

  hlumi1  = fs->make<TH1F>("hlumi1", "lumi", 2000,0,2000.);
  hlumi0  = fs->make<TH1F>("hlumi0", "lumi", 2000,0,2000.);

  hbx6    = fs->make<TH1F>("hbx6",   "bx",   4000,0,4000.);  
  hbx5    = fs->make<TH1F>("hbx5",   "bx",   4000,0,4000.);  
  hbx4    = fs->make<TH1F>("hbx4",   "bx",   4000,0,4000.);  
  hbx3    = fs->make<TH1F>("hbx3",   "bx",   4000,0,4000.);  
  hbx2    = fs->make<TH1F>("hbx2",   "bx",   4000,0,4000.);  
  hbx1    = fs->make<TH1F>("hbx1",   "bx",   4000,0,4000.);  
  hbx0    = fs->make<TH1F>("hbx0",   "bx",   4000,0,4000.);  

  hdets  = fs->make<TH1F>( "hdets",  "Dets with hits", 2000, -0.5, 1999.5);
  const int sizeH=20000;
  const float lowH = -0.5;
  const float highH = 99999.5;
  hdigis  = fs->make<TH1F>( "hdigis", "All Digis", sizeH, lowH, highH);
  hdigis0 = fs->make<TH1F>( "hdigis0", "All Digis zoomed", 2000, lowH, 1999.5);
  hdigis1 = fs->make<TH1F>( "hdigis1", "All Digis for full events", sizeH, lowH, highH);
  hdigis2 = fs->make<TH1F>( "hdigis2", "BPix Digis", sizeH, lowH, highH);
  hdigis3 = fs->make<TH1F>( "hdigis3", "Fpix Digis", sizeH, lowH, highH);
  hdigis4 = fs->make<TH1F>( "hdigis4", "All Digis - on bunch", sizeH, lowH, highH);
  hdigis5 = fs->make<TH1F>( "hdigis5", "All Digis - off bunch ", sizeH, lowH, highH);

  // 2D
  hzr = fs->make<TH2F>("hzr"," ",240,-60.,60.,68,0.,17.);  // x-y plane
  hxy11 = fs->make<TH2F>("hxy11"," ",320,-16.,16.,320,-16.,16.); // x-y pla 
  hxy12 = fs->make<TH2F>("hxy12"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy21 = fs->make<TH2F>("hxy21"," ",320,-16.,16.,320,-16.,16.); // x-y pl
  hxy22 = fs->make<TH2F>("hxy22"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy31 = fs->make<TH2F>("hxy31"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy32 = fs->make<TH2F>("hxy32"," ",320,-16.,16.,320,-16.,16.); // x-y plae

  hxy = fs->make<TH2F>("hxy"," ",340,-17.,17.,340,-17.,17.);  // x-y plane
  hphiz1 = fs->make<TH2F>("hphiz1"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz2 = fs->make<TH2F>("hphiz2"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz3 = fs->make<TH2F>("hphiz3"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz4 = fs->make<TH2F>("hphiz4"," ",108,-27.,27.,140,-3.5,3.5);

  hadc1ls = fs->make<TProfile>("hadc1ls","adc1 vs ls",1000,0,1000,     0.,255.);
  hadc1bx = fs->make<TProfile>("hadc1bx","adc1 vs bx",4000,-0.5,3999.5,0.,255.);
  hadc2ls = fs->make<TProfile>("hadc2ls","adc2 vs ls",1000,0,1000,     0.,255.);
  hadc2bx = fs->make<TProfile>("hadc2bx","adc2 vs bx",4000,-0.5,3999.5,0.,255.);
  hadc3ls = fs->make<TProfile>("hadc3ls","adc3 vs ls",1000,0,1000,     0.,255.);
  hadc3bx = fs->make<TProfile>("hadc3bx","adc3 vs bx",4000,-0.5,3999.5,0.,255.);
  hadc4ls = fs->make<TProfile>("hadc4ls","adc4 vs ls",1000,0,1000,     0.,255.);
  hadc4bx = fs->make<TProfile>("hadc4bx","adc4 vs bx",4000,-0.5,3999.5,0.,255.);
  hadc0ls = fs->make<TProfile>("hadc0ls","adc0 vs ls",1000,0,1000,     0.,255.);
  hadc0bx = fs->make<TProfile>("hadc0bx","adc0 vs bx",4000,-0.5,3999.5,0.,255.);

#ifdef DCOLS 
  hDcolsCount = fs->make<TH1F>( "hDcolsCount", "Counts per dcol per event",500,0.,5.);
  hAllDcols   = fs->make<TH1F>( "hAllDcols", "hits per dcol",50000,0.,50000.);
#endif

#endif

}

// ------------ method called to produce the data  ------------
void PixDigisTest::analyze(const edm::Event& iEvent, 
			   const edm::EventSetup& iSetup) {

  const bool MY_DEBUG = false;
  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  iSetup.get<TrackerTopologyRcd>().get(tTopo);
  const TrackerTopology* tt = tTopo.product();

  using namespace edm;
  if(PRINT) cout<<" Analyze PixDigisTest for phase "<<phase1_<<endl;

  int run       = iEvent.id().run();
  int event     = iEvent.id().event();
  int lumiBlock = iEvent.luminosityBlock();
  int bx        = iEvent.bunchCrossing();
  int orbit     = iEvent.orbitNumber();

  hbx0->Fill(float(bx));
  hlumi0->Fill(float(lumiBlock));

  if(select1>0) {
    // skip events with few pixel dets
    if(select1==1) { if(-1<select2) return; } // dummy 
    // select events only for a defined bx
    else if(select1==2) { if(bx!=select2) return; } 
    else if(select1==3) { if(  !( (bx==39)||(bx==201)||(bx==443)||(bx==499)||(bx==1083)||(bx==1337)||(bx==1492)||(bx==1977)||(bx==2231)||(bx==2287)||(bx==2871)||(bx==3224)||(bx==3280) )   ) return; } 
    else if(select1==4) { if( ( (bx==1)||(bx==39)||(bx==201)||(bx==443)||(bx==499)||(bx==1083)||(bx==1337)||(bx==1492)||(bx==1977)||(bx==2231)||(bx==2287)||(bx==2871)||(bx==3224)||(bx==3280) )   ) return; } 
    // select specific event
    else if(select1==10) { if(event!=select2) return; } 
    //....
  }


  // // eliminate bunches with beam
  bool bunch = false;
  // if( bx==410 || bx==460 || bx==510) bunch = true;
  // else if( bx==560 || bx==610 || bx==660 ) bunch = true;
  // else if( bx==1292 || bx==1342 || bx==1392 ) bunch = true;
  // else if( bx==1454 || bx==1504 || bx==1554 ) bunch = true;
  // else if( bx==2501 || bx==2601 ) bunch = true;
  // else if( bx==3080 || bx==3030 || bx == 3180 ) bunch = true;

  // if(bx>=1 && bx<=351) { if( (bx%50) == 1 ) bunch = true; }
  // else if(bx>=892 && bx<=1245) {
  //   if( ((bx-892)%50) == 0 ) bunch = true;
  //   else if( ((bx-895)%50) == 0 ) bunch = true;
  // } else if(bx>=1786 && bx<=2286) { if( ((bx-1786)%50) == 0 ) bunch = true; }
  // else if(bx>=2671 && bx<=3021) { if( ((bx-2671)%50) == 0 ) bunch = true; }

  // if(bunch) {
  //   //cout<<" reject "<<bx<<endl;
  //   hbx2->Fill(float(bx));
  // } else {
  //   if(bx==892) cout<<" something wrong"<<endl;
  //   if(bx==1245) cout<<" something wrong"<<endl;
  //   if(bx==3021) cout<<" something wrong"<<endl;
  //   if(bx==2286) cout<<" something wrong"<<endl;
  // } 

    // Get digis
  edm::Handle< edm::DetSetVector<PixelDigi> > pixelDigis;
  iEvent.getByToken( tPixelDigi , pixelDigis);

#ifdef USE_SIM_LINKS
  // Get simlink data
  edm::Handle< edm::DetSetVector<PixelDigiSimLink> > pixelSimLinks;
  iEvent.getByToken( tPixelDigiSimLink,   pixelSimLinks);
#endif

  // Get event setup (to get global transformation)
  edm::ESHandle<TrackerGeometry> geom;
  iSetup.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);

  int numberOfDetUnits = 0;
  int totalNumOfDigis = 0;

  int numberOfDetUnits1 = 0;
  int totalNumOfDigis1 = 0;
  int numberOfDetUnits2 = 0;
  int totalNumOfDigis2 = 0;
  int numberOfDetUnits3 = 0;
  int totalNumOfDigis3 = 0;
  int numberOfDetUnits4 = 0;
  int totalNumOfDigis4 = 0;
  int numOfDigisPerDet1 = 0;
  int numOfDigisPerDet2 = 0;
  int numOfDigisPerDet3 = 0;
  int numOfDigisPerDet4 = 0;

  int numberOfDetUnitsF1 = 0;
  int totalNumOfDigisF1 = 0;
  int numberOfDetUnitsF2 = 0;
  int totalNumOfDigisF2 = 0;
  int numberOfDetUnitsF3 = 0;
  int totalNumOfDigisF3 = 0;
  int numOfDigisPerDetF1 = 0;
  int numOfDigisPerDetF2 = 0;
  int numOfDigisPerDetF3 = 0;

  count0++; // count events 

  // Iterate on detector units
  edm::DetSetVector<PixelDigi>::const_iterator DSViter;
  for(DSViter = pixelDigis->begin(); DSViter != pixelDigis->end(); DSViter++) {
    bool valid = true;
    unsigned int detid = DSViter->id; // = rawid
    DetId detId(detid);
    //const GeomDetUnit      * geoUnit = geom->idToDetUnit( detId );
    //const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
    unsigned int detType=detId.det(); // det type, tracker=1
    unsigned int subid=detId.subdetId(); //subdetector type, barrel=1
    
    if(MY_DEBUG) 
      cout<<"Det: "<<detId.rawId()<<" "<<detId.null()<<" "<<detType<<" "<<subid<<endl;
    
#ifdef HISTOS
    hdetunit->Fill(float(detid));
    hpixid->Fill(float(detType));
    hpixsubid->Fill(float(subid));
#endif
    
    if(detType!=1) continue; // look only at tracker
    ++numberOfDetUnits;
    count1++; // count det untis

    // Get the geom-detector 
    const PixelGeomDetUnit * theGeomDet = 
      dynamic_cast<const PixelGeomDetUnit*> ( theTracker.idToDet(detId) );
    double detX = theGeomDet->surface().position().x();
    double detY = theGeomDet->surface().position().y();
    double detZ = theGeomDet->surface().position().z();
    double detR = theGeomDet->surface().position().perp();
    double detPhi = theGeomDet->surface().position().phi();
    //const BoundPlane plane = theGeomDet->surface(); // does not work
    
//     int cols = theGeomDet->specificTopology().ncolumns();
//     int rows = theGeomDet->specificTopology().nrows();
//     float pitchX = theGeomDet->specificTopology().pitch().first;
//     float pitchY = theGeomDet->specificTopology().pitch().second;
    
    const PixelTopology &topology = theGeomDet->specificTopology(); 
    int cols = topology.ncolumns();
    int rows = topology.nrows();
    float pitchX = topology.pitch().first;
    float pitchY = topology.pitch().second;
     
    unsigned int layerC=0;
    unsigned int ladderC=0;
    unsigned int zindex=0;

    // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
    int shell  = 0; // shell id 
    int sector = 0; // 1-8
    int ladder = 0; // 1-22
    int layer  = 0; // 1-3
    int module = 0; // 1-4
    bool half  = false; // 

    unsigned int disk=0; //1,2,3
    unsigned int blade=0; //1-24
    unsigned int side=0; //size=1 for -z, 2 for +z
    unsigned int panel=0; //panel=1
 
   // Subdet it, pix barrel=1, forward=2
    if(subid==2) {   // forward

#ifdef HISTOS
      hdetrF->Fill(detR);
      hdetzF->Fill(detZ);
      hcolsF->Fill(float(cols));
      hrowsF->Fill(float(rows));
#endif

      
      disk=tTopo->pxfDisk(detid); //1,2,3
      blade=tTopo->pxfBlade(detid); //1-24
      zindex=tTopo->pxfModule(detid); //
      side=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
      panel=tTopo->pxfPanel(detid); //panel=1
      
      if(PRINT) {
	cout<<"FPix (cmssw) disk "<<disk<<", blade "
		    <<blade<<", module "<<zindex<<", side "<<side<<", panel "
		    <<panel<<endl;
	if(MY_DEBUG) cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
			 <<pitchX<<" "<<pitchY<<endl;
      }

      // correct way 
      PixelEndcapName pfn(detid,tt,phase1_);
      if(PRINT) 
	cout<<pfn.diskName()<<" "<<pfn.bladeName()<<" "<<pfn.pannelName()<<" "
	    <<pfn.ringName()<<" "<<pfn.plaquetteName()<<" "<<pfn.name()<<endl;

      // All these produce wrong names for phase1
      // PixelEndcapName pfn1(detid,false);
      // cout<<pfn1.diskName()<<" "<<pfn1.bladeName()<<" "<<pfn1.pannelName()<<" "<<pfn1.ringName()<<" "<<pfn1.plaquetteName()
      // 	  <<" "<<pfn1.name()<<endl;
      // PixelEndcapName pfn2(detid,true);
      // cout<<pfn2.diskName()<<" "<<pfn2.bladeName()<<" "<<pfn2.pannelName()<<" "<<pfn2.ringName()<<" "<<pfn2.plaquetteName()
      // 	  <<" "<<pfn2.name()<<endl;
      // PixelEndcapNameUpgrade pfn3(detid);
      // cout<<pfn3.diskName()<<" "<<pfn3.bladeName()<<" "<<pfn3.pannelName()<<" "<<pfn3.plaquetteName()
      // 	  <<" "<<pfn3.name()<<endl;



    } else if(subid == 1) { // Barrel 
      
      // Barell layer = 1,2,3
      layerC=tTopo->pxbLayer(detid);
      // Barrel ladder id 1-20,32,44.
      ladderC=tTopo->pxbLadder(detid);
      // Barrel Z-index=1,8
      zindex=tTopo->pxbModule(detid);
      // Convert to online 
      PixelBarrelName pbn(detid,tt,phase1_);
      // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
      PixelBarrelName::Shell sh = pbn.shell(); //enum
      sector = pbn.sectorName();
      ladder = pbn.ladderName();
      layer  = pbn.layerName();
      module = pbn.moduleName();
      half  = pbn.isHalfModule();
      shell = int(sh);
      // change the module sign for z<0
      if(shell==1 || shell==2) module = -module;
      // change ladeer sign for Outer )x<0)
      if(shell==1 || shell==3) ladder = -ladder;
      

      if(PRINT) { 
	cout<<" BPix layer/ladder/module (cmssw) "
	    <<layerC<<" "<<ladderC<<" "<<zindex<<" (online) "
	    <<pbn.name()<<" "<<sh<<"("<<shell<<") "<<sector<<" "
	    <<layer<<" "<<ladder<<" "<<module<<" "<<half<< endl;
	//cout<<" Barrel det, thick "<<detThick<<" "
	//  <<" layer, ladder, module "
	//  <<layer<<" "<<ladder<<" "<<zindex<<endl;
	//cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
	//  <<pitchX<<" "<<pitchY<<endl;

	// wrong for phase1
	//PixelBarrelNameUpgrade pbn1(detid);
	//cout<<pbn1.sectorName()<<" "<<pbn1.ladderName()<<" "<<pbn1.layerName()<<" "<<pbn1.moduleName()<<pbn1.name();



      }      

    } // end fb-bar

#ifdef USE_SIM_LINKS
    // Look at simlink information (simulated data only)

    int numberOfSimLinks = 0;
    edm::DetSetVector<PixelDigiSimLink>::const_iterator 
      isearch = pixelSimLinks->find(detid);

    if(isearch != pixelSimLinks->end()) {      //if it is not empty
      edm::DetSet<PixelDigiSimLink> link_detset = (*pixelSimLinks)[detid];
      edm::DetSet<PixelDigiSimLink>::const_iterator it;
      // Loop over DigisSimLink in this det unit
      for(it = link_detset.data.begin(); 
	  it != link_detset.data.end(); it++) {
	
	numberOfSimLinks++;
	// these methods should be declared const, fixed by M.P.
	// wait for next releasse and then uncomment
	unsigned int chan = it->channel();
	unsigned int simTrack = it->SimTrackId();
	float frac = it->fraction();
#ifdef HISTOS
	hsimlinks->Fill(float(simTrack));
	hfract->Fill(frac);
#endif

	if(PRINT) cout<<" Sim link "<<numberOfSimLinks<<" "<<chan<<" "
		      <<simTrack<<" "<<frac<<endl;
      } // end simlink det loop

    } // end simlink if

#endif  // USE_SIM_LINKS

    bool select = false; // (abs(module)==4) && (layer==1);
    if(select) cout<<" run "<<run<<" event "<<event<<" bx "<<bx<<" ladder/module "<<ladder<<"/"<<module<<endl;

    unsigned int numberOfDigis = 0;
    //unsigned int numDigisInCol = 0;
    //int oldCol=-1;

    // Clear the module matrix
    if(layer==1) {
      for(int col=0;col<416;++col)
	for(int row=0;row<160;++row)
	  oneModule[col][row]=0;
    }

    // Look at digis in this module 
    edm::DetSet<PixelDigi>::const_iterator  di;
      for(di = DSViter->data.begin(); di != DSViter->data.end(); di++) {
	//for(di = begin; di != end; di++) {
	
	numberOfDigis++;
	totalNumOfDigis++;
	count2++;
	int adc = di->adc();    // charge, modifued to unsiged short 
	int col = di->column(); // column 
	int row = di->row();    // row
	//int tof = di->time();    // tof always 0, method deleted
	
	// channel index needed to look for the simlink to simtracks
	int channel = PixelChannelIdentifier::pixelToChannel(row,col);
	if(PRINT || select) cout <<numberOfDigis<< " Col: " << col << " Row: " << row 
		       << " ADC: " << adc <<" channel = "<<channel<<endl;
	
	
	// Accumuate dcols, do only for L1
	if(layer==1) {
	  // fill the module 
	  oneModule[col][row]++;
	  int ind = moduleIndex(ladder,module);
	  int roc = rocId(col,row);
	  int dcol = (col%52)/2;
	  dCols[ind][roc][dcol]++;
#ifdef DCOLS
	  hAllDcols->Fill(float(ind*16*26+roc*26+dcol));
#endif
	  //cout<<layer<<" "<<ladder<<" "<<module<<" "<<col<<" "<<row<<" "
	  //  <<ind<<" "<<roc<<" "<<dcol<<" "<<dCols[ind][roc][dcol]<<endl;
	}

	// // Analyse dcols with many hits 
	// if(col==oldCol) { // same column 
	//   numDigisInCol++;
	// } else {
	//   if( (layer==1) && (numDigisInCol>20)) {
	//     if(numDigisInCol>79) { 
	//       cout<<" col with many hits "<<module<<" "<<ladder<<" "
	// 	  <<oldCol<<" "<<numDigisInCol<<endl;
	//       //countFullDcols++;
	//     }
	//   }
	//   numDigisInCol=1;
	//   oldCol=col;	  
	// }

       if(col>415) cout<<" Error: column index too large "<<col<<" Barrel layer, ladder, module "
		       <<layer<<" "<<ladder<<" "<<zindex<<endl;
       if(row>159) cout<<" Error: row index too large "<<row<<endl;

#ifdef HISTOS
       bool noise = false;

#ifdef SINGLE_MODULES
       float weight=1; // adc 
       float pixy = col; float pixx=row;
#endif
       if(layer==1) {
	 //noise = (ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   heloss1->Fill(float(adc));
	   hcols1->Fill(float(col));
	   hrows1->Fill(float(row));
	   hpixMap1->Fill(float(col),float(row));
	   hpdetMap1->Fill(float(module),float(ladder));
	   if(adc<1) hpdetMaps1->Fill(float(module),float(ladder));
	   hadc1ls->Fill(float(lumiBlock),float(adc));
	   hadc1bx->Fill(float(bx),float(adc));

#ifdef SINGLE_MODULES
	    if     ( ladder==1 && module== 1) hpixDetMap10->Fill(pixy,pixx,weight); // 
	    else if( ladder==2 && module== 3) hpixDetMap11->Fill(pixy,pixx,weight); // " 
	    else if( ladder==3 && module==-1) hpixDetMap12->Fill(pixy,pixx,weight); // "
	    else if( ladder==3 && module== 3) hpixDetMap13->Fill(pixy,pixx,weight); // 
	    else if( ladder==5 && module==-4) hpixDetMap14->Fill(pixy,pixx,weight); // 
	    else if( ladder==5 && module== 2) hpixDetMap15->Fill(pixy,pixx,weight); // 
	    else if( ladder==1 && module==-4) hpixDetMap16->Fill(pixy,pixx,weight); //
	    else if( ladder==2 && module==-4) hpixDetMap17->Fill(pixy,pixx,weight); // 
	    else if( ladder==3 && module==-4) hpixDetMap18->Fill(pixy,pixx,weight); // 
	    else if( ladder==4 && module==-4) hpixDetMap19->Fill(pixy,pixx,weight); // 
#endif

	   totalNumOfDigis1++;
	   //htest2->Fill(float(module),float(adc));
	   numOfDigisPerDet1++;

//old 	   if(RectangularPixelTopology::isItBigPixelInX(row)) {
//new	   if(topology.isItBigPixelInX(row)) { 
// 	     //cout<<" big in X "<<row<<endl;
// 	     heloss1bigx->Fill(float(adc));
// 	     hrows1big->Fill(float(row));
// 	   }
//old	   if(RectangularPixelTopology::isItBigPixelInY(col)) {
//new	   if(topology.isItBigPixelInY(col)) {
// 	     //cout<<" big in Y "<<col<<endl;
// 	     heloss1bigy->Fill(float(adc));
// 	     hcols1big->Fill(float(col));
// 	   }

	 } // noise
       } else if(layer==2) {
	 // look for the noisy pixel
	 //noise = false; // (ladder==6) && (module==-2) && (col==364) && (row==1);
	 if(noise) {
	   //cout<<" noise pixel "<<layer<<" "<<sector<<" "<<shell<<endl;
	   //hpixMapNoise->Fill(float(col),float(row));
	   hneloss2->Fill(float(adc));
	 } else {		     
	   heloss2->Fill(float(adc));
	   hcols2->Fill(float(col));
	   hrows2->Fill(float(row));
	   hpixMap2->Fill(float(col),float(row));
	   hpdetMap2->Fill(float(module),float(ladder));
	   hadc2ls->Fill(float(lumiBlock),float(adc));
	   hadc2bx->Fill(float(bx),float(adc));
	   totalNumOfDigis2++;
	   numOfDigisPerDet2++;
	 } // noise 

#ifdef SINGLE_MODULES
	 if     ( ladder== 3 && module== 1) hpixDetMap20->Fill(pixy,pixx,weight); // 
	 else if( ladder== 3 && module== 2) hpixDetMap21->Fill(pixy,pixx,weight); // 
	 else if( ladder== 3 && module== 3) hpixDetMap22->Fill(pixy,pixx,weight); // 
	 else if( ladder==13 && module==-1) hpixDetMap23->Fill(pixy,pixx,weight); //
	 else if( ladder==-3 && module==4) hpixDetMap24->Fill(pixy,pixx,weight); //
	 else if( ladder==-4 && module==4) hpixDetMap25->Fill(pixy,pixx,weight); // 
	 else if( ladder==-5 && module==4) hpixDetMap26->Fill(pixy,pixx,weight); // 
	 else if( ladder==-6 && module==4) hpixDetMap27->Fill(pixy,pixx,weight); // 
	 else if( ladder==-1 && module==-4) hpixDetMap28->Fill(pixy,pixx,weight); //
	 else if( ladder==-2 && module==-4) hpixDetMap29->Fill(pixy,pixx,weight); // 
#endif

       } else if(layer==3) {
	 //noise = false; //(ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   heloss3->Fill(float(adc));
	   hcols3->Fill(float(col));
	   hrows3->Fill(float(row));
	   //if(ladder==-13 && module==-4) 
	   hpdetMap3->Fill(float(module),float(ladder));
	   hpixMap3->Fill(float(col),float(row));
	   hadc3ls->Fill(float(lumiBlock),float(adc));
	   hadc3bx->Fill(float(bx),float(adc));
	   totalNumOfDigis3++;
	   numOfDigisPerDet3++;
	 } // noise

#ifdef SINGLE_MODULES
	 if     ( ladder==13 && module==-4) hpixDetMap30->Fill(pixy,pixx,weight); // 
	 else if( ladder==-1 && module==-1) hpixDetMap31->Fill(pixy,pixx,weight); // 
	 else if( ladder==15 && module== 1) hpixDetMap32->Fill(pixy,pixx,weight); // 
	 else if( ladder==12 && module== 4) hpixDetMap33->Fill(pixy,pixx,weight); // 
	 else if( ladder==19 && module==-4) hpixDetMap34->Fill(pixy,pixx,weight); // 
	 else if( ladder==14 && module==-3) hpixDetMap35->Fill(pixy,pixx,weight); // 
#endif
       } else if(layer==4) {
	 //noise = false; //(ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   heloss4->Fill(float(adc));
	   hcols4->Fill(float(col));
	   hrows4->Fill(float(row));
	   hpdetMap4->Fill(float(module),float(ladder));
	   hpixMap4->Fill(float(col),float(row));
	   hadc4ls->Fill(float(lumiBlock),float(adc));
	   hadc4bx->Fill(float(bx),float(adc));
	   totalNumOfDigis4++;
	   numOfDigisPerDet4++;
	 } // noise
#ifdef SINGLE_MODULES
	 if     ( ladder== 6 && module== 1) hpixDetMap40->Fill(pixy,pixx,weight); // 
	 else if( ladder== 6 && module== 2) hpixDetMap41->Fill(pixy,pixx,weight); // 
	 else if( ladder== 6 && module== 3) hpixDetMap42->Fill(pixy,pixx,weight); // 
	 else if( ladder== 6 && module== 4) hpixDetMap43->Fill(pixy,pixx,weight); // 
	 else if( ladder==-27 && module==1) hpixDetMap44->Fill(pixy,pixx,weight); // 
	 else if( ladder== 3 && module==-4) hpixDetMap45->Fill(pixy,pixx,weight); // 
#endif
       } else if(disk==1) {
	 //noise = false; //(ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   helossF1->Fill(float(adc));
	   hcolsF1->Fill(float(col));
	   hrowsF1->Fill(float(row));
	   hadc0ls->Fill(float(lumiBlock),float(adc));
	   hadc0bx->Fill(float(bx),float(adc));
	   totalNumOfDigisF1++;
	   numOfDigisPerDetF1++;
	 } // noise 

       } else if(disk==2) {
	 // noise = (ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   helossF2->Fill(float(adc));
	   hcolsF2->Fill(float(col));
	   hrowsF2->Fill(float(row));
	   hadc0ls->Fill(float(lumiBlock),float(adc));
	   hadc0bx->Fill(float(bx),float(adc));
	   totalNumOfDigisF2++;
	   numOfDigisPerDetF2++;
	 } // noise 
       } else if(disk==3) {
	 // noise = (ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   helossF3->Fill(float(adc));
	   hcolsF3->Fill(float(col));
	   hrowsF3->Fill(float(row));
	   hadc0ls->Fill(float(lumiBlock),float(adc));
	   hadc0bx->Fill(float(bx),float(adc));
	   totalNumOfDigisF3++;
	   numOfDigisPerDetF3++;
	 } // noise 
       } // end if layer

       if(noise) valid = false;

#endif
       
      } // end for digis in detunit

      //if(PRINT) 
      //cout<<" for det "<<detid<<" digis = "<<numberOfDigis<<endl;

#ifdef HISTOS
      // Some histos
      if(valid) {  // to igore noise pixels
	if(subid==2) {   // forward
	  
	  hdetrF->Fill(detR);
	  hdetzF->Fill(detZ);
	  hcolsF->Fill(float(cols));
	  hrowsF->Fill(float(rows));
	  hzr->Fill(detZ,detR);
	  
	  if(disk==1) {
	    hblade1->Fill(float(blade));
	    ++numberOfDetUnitsF1;
	    hdigisPerDetF1->Fill(float(numOfDigisPerDetF1));
	    numOfDigisPerDetF1=0;	
	    if(detZ<0.) hxy11->Fill(detX,detY);
	    else        hxy12->Fill(detX,detY);
	  } else if(disk==2) {
	    hblade2->Fill(float(blade));
	    ++numberOfDetUnitsF2;
	    hdigisPerDetF2->Fill(float(numOfDigisPerDetF2));
	    numOfDigisPerDetF2=0;
	    if(detZ<0.) hxy21->Fill(detX,detY);
	    else        hxy22->Fill(detX,detY);

	  } else if(disk==3) {
	    hblade3->Fill(float(blade));
	    ++numberOfDetUnitsF3;
	    hdigisPerDetF3->Fill(float(numOfDigisPerDetF3));
	    numOfDigisPerDetF3=0;
	    if(detZ<0.) hxy31->Fill(detX,detY);
	    else        hxy32->Fill(detX,detY);
	  } // if disk

	} else if (subid==1) { // barrel
	  
	  hdetr->Fill(detR);
	  hdetz->Fill(detZ);
	  hcolsB->Fill(float(cols));
	  hrowsB->Fill(float(rows));
	  hxy->Fill(detX,detY);

	  hlayerid->Fill(float(layer));
	  hshellid->Fill(float(shell));
	  hsectorid->Fill(float(sector));

	  if(layer==1) {
	    
	    hladder1id->Fill(float(ladder));
	    hz1id->Fill(float(module));
	    hdetMap1->Fill(float(module),float(ladder));
	    //if(numOfDigisPerDet1>200 ) hpdetMaps2->Fill(float(module),float(ladder));
	    ++numberOfDetUnits1;
	    hdigisPerDet1->Fill(float(numOfDigisPerDet1));
	    numOfDigisPerDet1=0;
	    hphiz1->Fill(detZ,detPhi);
	    
	  } else if(layer==2) {
	    hladder2id->Fill(float(ladder));
	    hz2id->Fill(float(module));
	    hdetMap2->Fill(float(module),float(ladder));
	    ++numberOfDetUnits2;
	    hdigisPerDet2->Fill(float(numOfDigisPerDet2));
	    numOfDigisPerDet2=0;
	    hphiz2->Fill(detZ,detPhi);
	    
	  } else if(layer==3) {
	    hladder3id->Fill(float(ladder));
	    hz3id->Fill(float(module));
	    hdetMap3->Fill(float(module),float(ladder));
	    ++numberOfDetUnits3;
	    hdigisPerDet3->Fill(float(numOfDigisPerDet3));
	    numOfDigisPerDet3=0;
	    hphiz3->Fill(detZ,detPhi);

	  } else if(layer==4) {
	    hladder4id->Fill(float(ladder));
	    hz4id->Fill(float(module));
	    hdetMap4->Fill(float(module),float(ladder));
	    ++numberOfDetUnits4;
	    hdigisPerDet4->Fill(float(numOfDigisPerDet4));
	    numOfDigisPerDet4=0;
	    hphiz4->Fill(detZ,detPhi);
	    
	  } // layer


	  if(layer==1) {
	    const int dcolThr = 100;
	    int countDcols=0;
	    // Analyse this module 
	    for(int col=0;col<416;col+=2) {
	      //cout<<col<<endl;
	      int dcolCount=0;
	      for(int row=0;row<80;++row) { 
		if(oneModule[col][row]>0)   
		  {dcolCount++;}
		//{dcolCount++; cout<<col<<" "<<row<<" "<<oneModule[col][row]<<endl;}
	      }
	      for(int row=0;row<80;++row) {
		if(oneModule[col+1][row]>0) 
		  {dcolCount++;}
		//{dcolCount++; cout<<(col+1)<<" "<<row<<" "<<oneModule[col+1][row]<<endl;}
	      }

	      hhitsPerDcol->Fill(float(dcolCount));   // hits per dcol 
	      if(dcolCount>0) {countDcols++;hhitsPerHitDcol->Fill(float(dcolCount));}

	      if(dcolCount>dcolThr) {
		cout<<" full dcol (lower roc) "<<dcolCount<<" "
		    <<col<<" "<<module<<" "<<ladder<<endl;
		countFullDcols++;
		hpdetMaps2->Fill(float(module),float(ladder));
		hcols1Many->Fill(float(col));
		
		// for(int row=0;row<80;++row) 
		//   if(oneModule[col][row]>0)   
		//     {cout<<col<<" "<<row<<" "<<oneModule[col][row]<<endl;}
		// for(int row=0;row<80;++row) 
		//   if(oneModule[col+1][row]>0) 
		//     {cout<<(col+1)<<" "<<row<<" "<<oneModule[col+1][row]<<endl;}
		
	      }

	      dcolCount=0;
	      for(int row=80;row<160;++row) 
		if(oneModule[col][row]>0) {dcolCount++;}

	      //{dcolCount++; cout<<col<<" "<<row<<" "<<oneModule[col][row]<<endl;}
	      for(int row=80;row<160;++row) 
		if(oneModule[col+1][row]>0) {dcolCount++;}
	      //{dcolCount++; cout<<(col+1)<<" "<<row<<" "<<oneModule[col+1][row]<<endl;}

	      hhitsPerDcol->Fill(float(dcolCount));   // hits per dcol 
	      if(dcolCount>0) {countDcols++;hhitsPerHitDcol->Fill(float(dcolCount));}

	      if(dcolCount>dcolThr) {
		cout<<" full dcol (upper roc)"<<dcolCount<<" "
		    <<col<<" "<<module<<" "<<ladder<<endl;
		countFullDcols++;
		hpdetMaps2->Fill(float(module),float(ladder));
		hcols1Many->Fill(float(col));
		
		// for(int row=80;row<160;++row) 
		//   if(oneModule[col][row]>0)   
		//     {cout<<col<<" "<<row<<" "<<oneModule[col][row]<<endl;}
		// for(int row=80;row<160;++row) 
		//   if(oneModule[col+1][row]>0) 
		//     {cout<<(col+1)<<" "<<row<<" "<<oneModule[col+1][row]<<endl;}
		
	      }
	    } // loop over cols
	    hhitDcolsPerModule->Fill(float(countDcols));
	  } // if layer 1


	} // if bpix	
	

      } // if valid
#endif

  } // end for det-units

  if(PRINT) 
    cout << " Number of full det-units = " <<numberOfDetUnits
	 <<" total digis = "<<totalNumOfDigis<<endl;
  hdets->Fill(float(numberOfDetUnits));
  hdigis->Fill(float(totalNumOfDigis));
  hdigis0->Fill(float(totalNumOfDigis));

  if(numberOfDetUnits>0) {
    count3++; // count full events 
    hevent->Fill(float(event));
    hlumi1->Fill(float(lumiBlock));
    hbx1->Fill(float(bx));
    horbit->Fill(float(orbit));

    hdigis1->Fill(float(totalNumOfDigis));
    float tmp = float(totalNumOfDigis1) + float(totalNumOfDigis2) 
      + float(totalNumOfDigis3); 
    hdigis2->Fill(tmp);
    tmp = float(totalNumOfDigisF1) + float(totalNumOfDigisF2);  
    hdigis3->Fill(tmp);

    if(bunch) hdigis4->Fill(float(totalNumOfDigis));
    else hdigis5->Fill(float(totalNumOfDigis));

    if(numberOfDetUnits>20) hbx3->Fill(float(bx));

    if(totalNumOfDigis>100) hbx4->Fill(float(bx));
    else if(totalNumOfDigis>4) hbx5->Fill(float(bx));
    else hbx6->Fill(float(bx));


  }

  
#ifdef HISTOS
  hdigisPerLay1 ->Fill(float(totalNumOfDigis1));
  hdigisPerLay2 ->Fill(float(totalNumOfDigis2));
  hdigisPerLay3 ->Fill(float(totalNumOfDigis3));
  hdigisPerLay4 ->Fill(float(totalNumOfDigis4));
  if(totalNumOfDigis1>0) hdetsPerLay1->Fill(float(numberOfDetUnits1));
  if(totalNumOfDigis2>0) hdetsPerLay2->Fill(float(numberOfDetUnits2));
  if(totalNumOfDigis3>0) hdetsPerLay3->Fill(float(numberOfDetUnits3));
  if(totalNumOfDigis4>0) hdetsPerLay4->Fill(float(numberOfDetUnits4));

  hdigisPerLayF1 ->Fill(float(totalNumOfDigisF1));
  hdigisPerLayF2 ->Fill(float(totalNumOfDigisF2));
  hdigisPerLayF3 ->Fill(float(totalNumOfDigisF3));
  hdetsPerLayF1 ->Fill(float(numberOfDetUnitsF1));
  hdetsPerLayF2 ->Fill(float(numberOfDetUnitsF2));
  hdetsPerLayF3 ->Fill(float(numberOfDetUnitsF3));
#endif

}
// ------------ method called to at the end of the job  ------------
void PixDigisTest::endJob(){
  cout << " End PixDigisTest " << endl;
  if(count0>0) 
    cout<<" events "<<count0<<" full events "<<count3<<" dets per event "<<count1/count0
	<<" digis per event "<<count2/count0<<endl;
  else 
    cout<<count0<<" "<<count1<<" "<<count2<<" "<<count3<<endl;

  cout<<" Cols with many hits "<<countFullDcols<<endl;

  if(1) {
    int emptyROC=0, totalEmptyCols=0, rocsWithEmptyCols=0;
    for(int ind=0;ind<120;++ind) {
      if( !validIndex(ind) ) continue;
      for(int roc=0;roc<16;++roc) {
	int rocCount=0;
	int emptyCols=0;
	for(int dcol=0;dcol<26;++dcol) {
	  int count = dCols[ind][roc][dcol];
	  rocCount += count;
#ifdef DCOLS
	  hDcolsCount->Fill(float(count)/float(count3));
#endif
	  if(count==0) {
	    emptyCols++;
	    //cout<<" empty dcol "<<dcol<<" "<<roc<<" "<<ind<<" "
	    //	<<validIndex(ind,true)<<endl;
	  } else if( (float(count)/float(count3)) >20) {
	    cout<<" hot dcol "<<count<<" "<<dcol<<" "
		<<roc<<" "<<ind<<endl;
	    //} else {
	    //cout<<" count "<<count<<" "<<dcol<<" "
	    //	<<roc<<" "<<ind<<" "<<validIndex(ind,true)<<endl;
	  }
	} // dcol
	if(rocCount==0) {emptyROC++;} //cout<<" empty roc "<<ind<<" "<<roc<<endl;
	else {
	  if(emptyCols>0) { 
	    totalEmptyCols += emptyCols;
	    rocsWithEmptyCols++;
	    cout<<" empty cols in module (index)="<<ind<<" roc "<<roc<<" num "<<emptyCols<<endl;
	  }
	}
      } // roc
    } // module 
    cout<<" empty ROCs "<<emptyROC<<" empty cols "<<totalEmptyCols
	<<" rocs with empty cols"<<rocsWithEmptyCols<<endl;
  }

  float norm = 1.;
  if(count3>0) {
    norm = 1./float(count3);
  }

  // Rescale all 2D plots
  hdetMap1->Scale(norm);
  hdetMap2->Scale(norm);
  hdetMap3->Scale(norm);
  hdetMap4->Scale(norm);
  hpdetMap1->Scale(norm);
  hpdetMap2->Scale(norm);
  hpdetMap3->Scale(norm);
  hpdetMap4->Scale(norm);
  hpdetMaps1->Scale(norm);
  hpdetMaps2->Scale(norm);
  //hpdetMaps3->Scale(norm);
  //hpdetMaps4->Scale(norm);

#ifdef DCOLS
  hAllDcols->Scale(norm);
#endif

}

//define this as a plug-in
DEFINE_FWK_MODULE(PixDigisTest);
