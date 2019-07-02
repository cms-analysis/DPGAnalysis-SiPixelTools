// -*- C++ -*-
//
// Package:    PixDigisTestPhase2
// Class:      PixDigisTestPhase2
// 
/**\class PixDigisTestPhase2 PixDigisTestPhase2.cc 

 Description: Test pixel digis. 
 Barrel & Forward digis. Uses root histos.
 Adopted for the new simLinks. 
 Added the online detector index. d.k. 11/09
 Works with CMSSW_7
 New detector ID.
 Modified to use "byToken"
 Modify for 4 layers.
 Include fpix histos.
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

//#define L1
//#define HLT
//#define SINGLE_MODULES
//#define DCOLS
//#define USE_GAINS 

#ifdef USE_GAINS
// Database payloads
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationServiceBase.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationService.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationOfflineService.h"
//#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationForHLTService.h"
#endif

using namespace std;

// Enable this to look at simlinks (link simhit->digis)
// Can be used only with simulated data.
//#define USE_SIM_LINKS


//
// class decleration
//

class PixDigisTestPhase2 : public edm::EDAnalyzer {

public:

  explicit PixDigisTestPhase2(const edm::ParameterSet&);
  ~PixDigisTestPhase2();
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(); 
  int moduleIndex(int ladder,int module); 
  bool validIndex(int index, bool print); 
  int rocId(int pixy,int pixx);  // 0-15, column, row
#ifdef USE_GAINS
  float calibrate(uint32_t detid, int adc, int col, int row);  
#endif

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

  //TH1F *hdetunit;
  TH1F *hdetr, *hdetz, *hdetrF1, *hdetzF1, *hdetrF2, *hdetzF2;
  TH1F *hcolsB,  *hrowsB,  *hcolsF,  *hrowsF;

  TH1F *hpixid,*hpixsubid,*hlayerid,*hshellid,*hsectorid,
    *hladder1id,*hladder2id,*hladder3id,*hladder4id,
    *hz1id,*hz2id,*hz3id,*hz4id;

  TH1F *heloss1,*heloss2, *heloss3, *heloss4; //  *heloss1n;
  //TH1F *hneloss1,*hneloss2, *hneloss3, *hneloss4;
  TH1F *hcols1,*hcols2,*hcols3,*hrows1,*hrows2,*hrows3,*hcols4,*hrows4;

  TH1F *hdigisPerDet1,*hdigisPerDet2,*hdigisPerDet3,*hdigisPerDet4;
  TH1F *hdigisPerLay1,*hdigisPerLay2,*hdigisPerLay3,*hdigisPerLay4;
  //TH1F *hdetsPerLay1,*hdetsPerLay2,*hdetsPerLay3,*hdetsPerLay4;

  TH2F *hpdetMap4,*hpdetMap3,*hpdetMap2,*hpdetMap1; 
  //TH2F *hdetMap4,*hdetMap3,*hdetMap2,*hdetMap1; 
  TH2F *hpixMap1, *hpixMap2, *hpixMap3,*hpixMap4;
   
  TH2F *hpdetMapF1,*hpdetMapF2; 
  TH2F *hpdetMapF21,*hpdetMapF22,*hpdetMapF23,*hpdetMapF24,*hpdetMapF25; 

  // FPix & EPix
  TH1F *helossF11,*helossF12,*helossF13,*helossF14;
  TH1F *helossF21,*helossF22,*helossF23,*helossF24,*helossF25;

  TH1F *hcolsF11,*hcolsF12,*hcolsF13,*hcolsF14;
  TH1F *hcolsF21,*hcolsF22,*hcolsF23,*hcolsF24,*hcolsF25;

  TH1F *hrowsF11,*hrowsF12,*hrowsF13,*hrowsF14;
  TH1F *hrowsF21,*hrowsF22,*hrowsF23,*hrowsF24,*hrowsF25,*hrowsF26;

  TH1F *hdigisPerDetF1,*hdigisPerDetF2;
  TH1F *hdigisPerDetF11,*hdigisPerDetF12,*hdigisPerDetF13,*hdigisPerDetF14;
  TH1F *hdigisPerDetF21,*hdigisPerDetF22,*hdigisPerDetF23,*hdigisPerDetF24,*hdigisPerDetF25;
  //TH1F *hdigisPerLayF11,*hdigisPerLayF12,*hdigisPerLayF13,*hdigisPerLayF14;
  TH1F *hdigisInDiskF21,*hdigisInDiskF22,*hdigisInDiskF23,*hdigisInDiskF24;
 
  TH1F *hdigisPerLayF1,*hdigisPerLayF2;

  TH1F *hblade11, *hblade12, *hblade13, *hblade14;
  TH1F *hblade21, *hblade22, *hblade23, *hblade24, *hblade25;

  //TH1F *hcols1Many,*hhitsPerDcol, *hhitsPerHitDcol, *hhitDcolsPerModule;
  //TProfile *hhitsPerHitDcolModule,*hhitsPerHitDcolLadder,*hDcolHitProbabilityModule,
  //*hDcolHitProbabilityLadder;


  TH1F *hfract, *hsimlinks;


  //* hpixMapNoise; 
  TH2F *hxy, *hphiz1, *hphiz2, *hphiz3, *hphiz4; // bpix 
  TH2F *hzr, *hxy11, *hxy12, *hxy21, *hxy22, *hxy31, *hxy32;  // fpix 

  TH1F *hlumi, *hbx0, *hlumi0, *hlumi1,*hbx1;
  TH1F *hdets, *hdigis, *hdigis0, *hdigis1, *hdigis2,*hdigis3; // *hdigis4,*hdigis5; 
  //TProfile *hadc1ls,*hadc2ls,*hadc3ls,*hadc4ls,*hadc0ls; 
  //TProfile *hadc1bx,*hadc2bx,*hadc3bx,*hadc4bx,*hadc0bx; 

  // FPix
  TH2F *hpixMapFPix11, *hpixMapFPix12, *hpixMapFPix13,*hpixMapFPix14;
  TH2F *hpixMapFPix21, *hpixMapFPix22, *hpixMapFPix23,*hpixMapFPix24, *hpixMapFPix25;
  //TH2F *hpixMapFPix31, *hpixMapFPix32;

#endif

  edm::InputTag src_;  
  bool phase1_;
  //int countFullDcols;
  //int oneModule[416][160];
  //int dCols[120][16][26];

};

PixDigisTestPhase2::PixDigisTestPhase2(const edm::ParameterSet& iConfig) {

  PRINT = iConfig.getUntrackedParameter<bool>("Verbosity",false);
  src_ =  iConfig.getParameter<edm::InputTag>( "src" );
  tPixelDigi = consumes <edm::DetSetVector<PixelDigi>> (src_);
#ifdef USE_SIM_LINKS
  tPixelDigiSimLink = consumes < edm::DetSetVector<PixelDigiSimLink> > ( src_);
#endif 
  phase1_ = iConfig.getUntrackedParameter<bool>( "phase1",false);
  select1 = iConfig.getUntrackedParameter<int>("Select1",0);
  select2 = iConfig.getUntrackedParameter<int>("Select2",0);

  cout<<" Construct PixDigisTestPhase2 "<<endl;

#ifdef USE_GAINS
  theSiPixelGainCalibration_ = new SiPixelGainCalibrationOfflineService(iConfig);
#endif

}


PixDigisTestPhase2::~PixDigisTestPhase2() {
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  cout<<" Destroy PixDigisTestPhase2 "<<endl;

}

//
// member functions
// decode a simplified ROC address
int PixDigisTestPhase2::rocId(int col, int row) {
  int rocRow = row/80;
  int rocCol = col/52;
  int rocId = rocCol + rocRow*8;
  return rocId;
 }
//      int roc = rocId(int(pixy),int(pixx));  // 0-15, column, row


// this is just to turn the L1 ladder&module into a 1D index 
int PixDigisTestPhase2::moduleIndex(int ladder, int module) {
  int index=-1;
  if(module>0) index = module-1;
  else         index = abs(module)-1+4;
  if(ladder>0) index = index + (ladder-1)*10;
  else         index = index + (abs(ladder)-1)*10 + 60;
  return index;
}
//
bool PixDigisTestPhase2::validIndex(int index, bool print = false) {
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
void PixDigisTestPhase2::beginJob() {

   using namespace edm;
   cout << "Initialize PixDigisTestPhase2 " <<endl;
   count0=count1=count2=count3=0;
   //countFullDcols=0;

   // for(int col=0;col<416;++col)
   //   for(int row=0;row<160;++row)
   //     oneModule[col][row]=0;

   // for(int ind=0;ind<120;++ind)
   //   for(int roc=0;roc<16;++roc)
   //     for(int dcol=0;dcol<26;++dcol)
   // 	 dCols[ind][roc][dcol]=0;


#ifdef HISTOS
  edm::Service<TFileService> fs;

  // Histos go to a subdirectory "PixRecHits")     
  //hdetunit = fs->make<TH1F>( "hdetunit", "Det unit", 1000,
  //                          302000000.,302300000.);
    hpixid = fs->make<TH1F>( "hpixid", "Pix det id", 10, 0., 10.);
    hpixsubid = fs->make<TH1F>( "hpixsubid", "Pix Barrel id", 10, 0., 10.);
    hlayerid = fs->make<TH1F>( "hlayerid", "Pix layer id", 10, 0., 10.);
    //hsectorid = fs->make<TH1F>( "hsectorid", "Pix sector ", 10, 0., 10.);
    //hshellid = fs->make<TH1F>( "hshellid", "Shell", 5, 0., 5.);
    hladder1id = fs->make<TH1F>( "hladder1id", "Ladder L1 id", 12, 0.5, 12.5);
    hladder2id = fs->make<TH1F>( "hladder2id", "Ladder L2 id", 28, 0.5, 28.5);
    hladder3id = fs->make<TH1F>( "hladder3id", "Ladder L3 id", 24, 0.5, 24.5);
    hladder4id = fs->make<TH1F>( "hladder4id", "Ladder L4 id", 32, 0.5, 32.5);
    hz1id = fs->make<TH1F>( "hz1id", "Z-index id L1", 9, 0.5, 9.5);
    hz2id = fs->make<TH1F>( "hz2id", "Z-index id L2", 9, 0.5, 9.5);
    hz3id = fs->make<TH1F>( "hz3id", "Z-index id L3", 9, 0.5, 9.5);
    hz4id = fs->make<TH1F>( "hz4id", "Z-index id L4", 9, 0.5, 9.5);
 
    hdigisPerDet1 = fs->make<TH1F>( "hdigisPerDet1", "Digis per det l1", 
			      1000, -0.5, 1999.5);
    hdigisPerDet2 = fs->make<TH1F>( "hdigisPerDet2", "Digis per det l2", 
			      300, -0.5, 599.5);
    hdigisPerDet3 = fs->make<TH1F>( "hdigisPerDet3", "Digis per det l3", 
			      300, -0.5, 599.5);
    hdigisPerDet4 = fs->make<TH1F>( "hdigisPerDet4", "Digis per det l4", 
			      300, -0.5, 599.5);
    //
    //const float maxSize=199.5;
    const float maxSize=99999.5;
    hdigisPerLay1 = fs->make<TH1F>( "hdigisPerLay1", "Digis per layer l1", 
			      1000, -0.5, maxSize);
    hdigisPerLay2 = fs->make<TH1F>( "hdigisPerLay2", "Digis per layer l2", 
			      1000, -0.5, maxSize);
    hdigisPerLay3 = fs->make<TH1F>( "hdigisPerLay3", "Digis per layer l3", 
			      1000, -0.5, maxSize);
    hdigisPerLay4 = fs->make<TH1F>( "hdigisPerLay4", "Digis per layer l4", 
			      1000, -0.5, maxSize);

    // hdetsPerLay1 = fs->make<TH1F>( "hdetsPerLay1", "Full dets per layer l1", 
    // 			      161, -0.5, 160.5);
    // hdetsPerLay2 = fs->make<TH1F>( "hdetsPerLay2", "Full dets per layer l2", 
    // 			      257, -0.5, 256.5);
    // hdetsPerLay3 = fs->make<TH1F>( "hdetsPerLay3", "Full dets per layer l3", 
    // 			      353, -0.5, 352.5);
    // hdetsPerLay4 = fs->make<TH1F>( "hdetsPerLay4", "Full dets per layer l4", 
    // 			      513, -0.5, 512.5);

    hdigisPerDetF1 = fs->make<TH1F>( "hdigisPerDetF1", "Digis per det d1-8", 
			      100, -0.5, 499.5);
    hdigisPerDetF2 = fs->make<TH1F>( "hdigisPerDetF2", "Digis per det d9-12", 
			      100, -0.5, 399.5);

    hdigisPerDetF11 = fs->make<TH1F>( "hdigisPerDetF11", "Digis per det d1, r1", 
			      100, -0.5, 499.5);
    hdigisPerDetF12 = fs->make<TH1F>( "hdigisPerDetF12", "Digis per det d1, r2", 
			      100, -0.5, 399.5);
    hdigisPerDetF13 = fs->make<TH1F>( "hdigisPerDetF13", "Digis per det d1, r3", 
			      100, -0.5, 399.5);
    hdigisPerDetF14 = fs->make<TH1F>( "hdigisPerDetF14", "Digis per det d1, r4", 
			      100, -0.5, 299.5);

    hdigisPerDetF21 = fs->make<TH1F>( "hdigisPerDetF21", "Digis per det d2, r1", 
			      200, -0.5, 199.5);
    hdigisPerDetF22 = fs->make<TH1F>( "hdigisPerDetF22", "Digis per det d2, r2", 
			      200, -0.5, 199.5);
    hdigisPerDetF23 = fs->make<TH1F>( "hdigisPerDetF23", "Digis per det d2, r3", 
			      200, -0.5, 199.5);
    hdigisPerDetF24 = fs->make<TH1F>( "hdigisPerDetF24", "Digis per det d2, r4", 
			      200, -0.5, 199.5);
    hdigisPerDetF25 = fs->make<TH1F>( "hdigisPerDetF25", "Digis per det d2, r5", 
			      200, -0.5, 199.5);


    // hdigisPerLayF11 = fs->make<TH1F>( "hdigisPerLayF11", "Digis per layer d1", 
    // 			      2000, -0.5, 99999.5);
    // hdigisPerLayF12 = fs->make<TH1F>( "hdigisPerLayF12", "Digis per layer d1", 
    // 			      2000, -0.5, 99999.5);
    // hdigisPerLayF13 = fs->make<TH1F>( "hdigisPerLayF13", "Digis per layer d1", 
    // 			      2000, -0.5, 99999.5);
    // hdigisPerLayF14 = fs->make<TH1F>( "hdigisPerLayF14", "Digis per layer d1", 
    // 			      2000, -0.5, 99999.5);

    hdigisInDiskF21 = fs->make<TH1F>( "hdigisInDiskF21", "Digis per mod, d9", 
			      200, -0.5, 199.5);
    hdigisInDiskF22 = fs->make<TH1F>( "hdigisInDiskF22", "Digis per mod, d10", 
			      200, -0.5, 199.5);
    hdigisInDiskF23 = fs->make<TH1F>( "hdigisInDiskF23", "Digis per mod, d11", 
			      200, -0.5, 199.5);
    hdigisInDiskF24 = fs->make<TH1F>( "hdigisInDiskF24", "Digis per mod, d12", 
			      200, -0.5, 199.5);

    hdigisPerLayF1 = fs->make<TH1F>( "hdigisPerLayF1", "Digis per layer d1", 
			      2000, -0.5, 999999.5);
    hdigisPerLayF2 = fs->make<TH1F>( "hdigisPerLayF2", "Digis per layer d2", 
			      2000, -0.5, 999999.5);

    int sizeH=50;
    float lowH = 0.0;
    float highH = 50.;
    heloss1 = fs->make<TH1F>( "heloss1", "Pix charge l1", sizeH, 0., highH);
    //heloss1n= fs->make<TH1F>( "heloss1n","Pix charge l1", sizeH, 0., highH);
    heloss2 = fs->make<TH1F>( "heloss2", "Pix charge l2", sizeH, 0., highH);
    heloss3 = fs->make<TH1F>( "heloss3", "Pix charge l3", sizeH, 0., highH);
    heloss4 = fs->make<TH1F>( "heloss4", "Pix charge l4", sizeH, 0., highH);
    //hneloss1 = fs->make<TH1F>( "hneloss1", "Pix adc elec<0 l1", sizeH, 0., highH);
    //hneloss2 = fs->make<TH1F>( "hneloss2", "Pix adc elec<0 l2", sizeH, 0., highH);
    //hneloss3 = fs->make<TH1F>( "hneloss3", "Pix adc elec<0 l3", sizeH, 0., highH);
    //hneloss4 = fs->make<TH1F>( "hneloss4", "Pix adc elec<0 l4", sizeH, 0., highH);

    hcols1 = fs->make<TH1F>( "hcols1", "Layer 1 cols", 443,-0.5,442.5);
    hcols2 = fs->make<TH1F>( "hcols2", "Layer 2 cols", 443,-0.5,442.5);
    hcols3 = fs->make<TH1F>( "hcols3", "Layer 3 cols", 443,-0.5,442.5);
    hcols4 = fs->make<TH1F>( "hcols4", "Layer 4 cols", 443,-0.5,442.5);

    hrows1 = fs->make<TH1F>( "hrows1", "Layer 1 rows", 1321,-0.5,1320.5);
    hrows2 = fs->make<TH1F>( "hrows2", "Layer 2 rows", 1321,-0.5,1320.5);
    hrows3 = fs->make<TH1F>( "hrows3", "layer 3 rows", 1321,-0.5,1320.5);
    hrows4 = fs->make<TH1F>( "hrows4", "layer 4 rows", 1321,-0.5,1320.5);
 
    helossF11 = fs->make<TH1F>( "helossF11", "Pix charge d", sizeH, 0., highH);
    helossF12 = fs->make<TH1F>( "helossF12", "Pix charge d", sizeH, 0., highH);
    helossF13 = fs->make<TH1F>( "helossF13", "Pix charge d", sizeH, 0., highH);
    helossF14 = fs->make<TH1F>( "helossF14", "Pix charge d", sizeH, 0., highH);

    helossF21 = fs->make<TH1F>( "helossF21", "Pix charge d2", sizeH, 0., highH);
    helossF22 = fs->make<TH1F>( "helossF22", "Pix charge d2", sizeH, 0., highH);
    helossF23 = fs->make<TH1F>( "helossF23", "Pix charge d2", sizeH, 0., highH);
    helossF24 = fs->make<TH1F>( "helossF24", "Pix charge d2", sizeH, 0., highH);
    helossF25 = fs->make<TH1F>( "helossF25", "Pix charge d2", sizeH, 0., highH);

    hcolsF11 = fs->make<TH1F>( "hcolsF11", "Disk 1 cols", 443,-0.5,442.5);
    hcolsF12 = fs->make<TH1F>( "hcolsF12", "Disk 1 cols", 443,-0.5,442.5);
    hcolsF13 = fs->make<TH1F>( "hcolsF13", "Disk 1 cols", 443,-0.5,442.5);
    hcolsF14 = fs->make<TH1F>( "hcolsF14", "Disk 1 cols", 443,-0.5,442.5);

    hcolsF21 = fs->make<TH1F>( "hcolsF21", "Disk 2 cols", 443,-0.5,442.5);
    hcolsF22 = fs->make<TH1F>( "hcolsF22", "Disk 2 cols", 443,-0.5,442.5);
    hcolsF23 = fs->make<TH1F>( "hcolsF23", "Disk 2 cols", 443,-0.5,442.5);
    hcolsF24 = fs->make<TH1F>( "hcolsF24", "Disk 2 cols", 443,-0.5,442.5);
    hcolsF25 = fs->make<TH1F>( "hcolsF25", "Disk 2 cols", 443,-0.5,442.5);

    hrowsF11 = fs->make<TH1F>( "hrowsF11", "Disk 1 rows", 1321,-0.5,1320.5);
    hrowsF12 = fs->make<TH1F>( "hrowsF12", "Disk 1 rows", 1321,-0.5,1320.5);
    hrowsF13 = fs->make<TH1F>( "hrowsF13", "Disk 1 rows", 1321,-0.5,1320.5);
    hrowsF14 = fs->make<TH1F>( "hrowsF14", "Disk 1 rows", 1321,-0.5,1320.5);

    hrowsF21 = fs->make<TH1F>( "hrowsF21", "Disk 2 rows", 1321,-0.5,1320.5);
    hrowsF22 = fs->make<TH1F>( "hrowsF22", "Disk 2 rows", 1321,-0.5,1320.5);
    hrowsF23 = fs->make<TH1F>( "hrowsF23", "Disk 2 rows", 1321,-0.5,1320.5);
    hrowsF24 = fs->make<TH1F>( "hrowsF24", "Disk 2 rows", 1321,-0.5,1320.5);
    hrowsF25 = fs->make<TH1F>( "hrowsF25", "Disk 2 rows", 1321,-0.5,1320.5);

    hblade11 = fs->make<TH1F>( "hblade11", "blade num, disk1-8", 60, 0., 60.);
    hblade12 = fs->make<TH1F>( "hblade12", "blade num, disk1-8", 60, 0., 60.);
    hblade13 = fs->make<TH1F>( "hblade13", "blade num, disk1-8", 60, 0., 60.);
    hblade14 = fs->make<TH1F>( "hblade14", "blade num, disk1-8", 60, 0., 60.);

    hblade21 = fs->make<TH1F>( "hblade21", "blade num, disk9-12", 60, 0., 60.);
    hblade22 = fs->make<TH1F>( "hblade22", "blade num, disk9-12", 60, 0., 60.);
    hblade23 = fs->make<TH1F>( "hblade23", "blade num, disk9-12", 60, 0., 60.);
    hblade24 = fs->make<TH1F>( "hblade24", "blade num, disk9-12", 60, 0., 60.);
    hblade25 = fs->make<TH1F>( "hblade25", "blade num, disk9-12", 60, 0., 60.);

    hdetr = fs->make<TH1F>("hdetr","det r",180,0.,18.);
    hdetz = fs->make<TH1F>("hdetz","det z",520,-26.,26.);
    hdetrF1 = fs->make<TH1F>("hdetrF1","det r",200,0.,20.);
    hdetzF1 = fs->make<TH1F>("hdetzF1","det z",600,-300.,300.);
    hdetrF2 = fs->make<TH1F>("hdetrF2","det r",300,0.,30.);
    hdetzF2 = fs->make<TH1F>("hdetzF2","det z",600,-300.,300.);

    hcolsB = fs->make<TH1F>("hcolsB","cols per bar det",450,0.,450.);
    hrowsB = fs->make<TH1F>("hrowsB","rows per bar det",1400,0.,1400.);
    hcolsF = fs->make<TH1F>("hcolsF","cols per for det",450,0.,450.);
    hrowsF = fs->make<TH1F>("hrowsF","rows per for det",1400,0.,1400.);

    hsimlinks = fs->make<TH1F>("hsimlinks"," track ids",200,0.,200.);
    hfract = fs->make<TH1F>("hfract"," track rractions",100,0.,1.);
    //                                             mod      ladder

    hpdetMap1 = fs->make<TH2F>("hpdetMap1"," ",9,1,9,12,1,12);
    hpdetMap1->SetOption("colz");
    hpdetMap2 = fs->make<TH2F>("hpdetMap2"," ",9,1,9,28,1,28);
    hpdetMap2->SetOption("colz");
    hpdetMap3 = fs->make<TH2F>("hpdetMap3"," ",9,1,9,24,1,24);
    hpdetMap3->SetOption("colz");
    hpdetMap4 = fs->make<TH2F>("hpdetMap4"," ",9,1,9,32,1,32);
    hpdetMap4->SetOption("colz");

    hpdetMapF1 = fs->make<TH2F>("hpdetMapF1","hits in FPix",32,1,32,4,1,5);
    hpdetMapF1->SetOption("colz");
    hpdetMapF2 = fs->make<TH2F>("hpdetMapF2","hits in EPix",56,1,56,5,1,6);
    hpdetMapF2->SetOption("colz");

    hpixMap1 = fs->make<TH2F>("hpixMap1"," ",442,0.,442.,1320,0.,1320.);
    hpixMap1->SetOption("colz");
    hpixMap2 = fs->make<TH2F>("hpixMap2"," ",442,0.,442.,1320,0.,1320.);
    hpixMap2->SetOption("colz");
    hpixMap3 = fs->make<TH2F>("hpixMap3"," ",442,0.,442.,1320,0.,1320.);
    hpixMap3->SetOption("colz");
    hpixMap4 = fs->make<TH2F>("hpixMap4"," ",442,0.,442.,1320,0.,1320.);
    hpixMap4->SetOption("colz");

  hlumi1  = fs->make<TH1F>("hlumi1", "lumi", 2000,0,2000.);
  hlumi0  = fs->make<TH1F>("hlumi0", "lumi", 2000,0,2000.);
  hbx1    = fs->make<TH1F>("hbx1",   "bx",   4000,0,4000.);  
  hbx0    = fs->make<TH1F>("hbx0",   "bx",   4000,0,4000.);  

  hdets  = fs->make<TH1F>( "hdets",  "Dets with hits", 1000, -0.5, 9999.5);

  sizeH=1000;
  lowH = -0.5;
  highH = 999999.5;
  hdigis  = fs->make<TH1F>( "hdigis", "All Digis", sizeH, lowH, highH);
  hdigis0 = fs->make<TH1F>( "hdigis0", "All Digis zoomed", 1000, lowH, 9999.5);
  hdigis1 = fs->make<TH1F>( "hdigis1", "All Digis for full events", sizeH, lowH, highH);
  hdigis2 = fs->make<TH1F>( "hdigis2", "BPix Digis", sizeH, lowH, highH);
  hdigis3 = fs->make<TH1F>( "hdigis3", "Fpix Digis", sizeH, lowH, highH);
  //hdigis4 = fs->make<TH1F>( "hdigis4", "All Digis - on bunch", sizeH, lowH, highH);
  //hdigis5 = fs->make<TH1F>( "hdigis5", "All Digis - off bunch ", sizeH, lowH, highH);

  // 2D
  //F+EPix
  hzr = fs->make<TH2F>("hzr"," ",600,-300.,300.,60,0.,30.);  // z-r plane
  hxy11 = fs->make<TH2F>("hxy11"," ",320,-16.,16.,320,-16.,16.); // x-y pla 
  hxy12 = fs->make<TH2F>("hxy12"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  hxy21 = fs->make<TH2F>("hxy21"," ",300,-30.,30.,300,-30.,30.); // x-y pl
  hxy22 = fs->make<TH2F>("hxy22"," ",300,-30.,30.,300,-30.,30.); // x-y pla
  //hxy31 = fs->make<TH2F>("hxy31"," ",320,-16.,16.,320,-16.,16.); // x-y pla
  //hxy32 = fs->make<TH2F>("hxy32"," ",320,-16.,16.,320,-16.,16.); // x-y plae
  // BPix
  hxy = fs->make<TH2F>("hxy"," ",340,-17.,17.,340,-17.,17.);  // x-y plane
  hphiz1 = fs->make<TH2F>("hphiz1"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz2 = fs->make<TH2F>("hphiz2"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz3 = fs->make<TH2F>("hphiz3"," ",108,-27.,27.,140,-3.5,3.5);
  hphiz4 = fs->make<TH2F>("hphiz4"," ",108,-27.,27.,140,-3.5,3.5);

  // FPix
  hpixMapFPix11 = fs->make<TH2F>("hpixMapFPix11"," disk 1, ring 1",442,0.,442.,1320,0.,1320.);
  hpixMapFPix11->SetOption("colz");
  hpixMapFPix12 = fs->make<TH2F>("hpixMapFPix12"," disk 1, ring 2",442,0.,442.,1320,0.,1320.);
  hpixMapFPix12->SetOption("colz");
  hpixMapFPix13 = fs->make<TH2F>("hpixMapFPix13"," disk 1, ring 3",442,0.,442.,1320,0.,1320.);
  hpixMapFPix13->SetOption("colz");
  hpixMapFPix14 = fs->make<TH2F>("hpixMapFPix14"," disk 1, ring 4",442,0.,442.,1320,0.,1320.);
  hpixMapFPix14->SetOption("colz");


  hpixMapFPix21 = fs->make<TH2F>("hpixMapFPix21"," disk 2, ring 1",442,0.,442.,1320,0.,1320.);
  hpixMapFPix21->SetOption("colz");
  hpixMapFPix22 = fs->make<TH2F>("hpixMapFPix22"," disk 2, ring 2",442,0.,442.,1320,0.,1320.);
  hpixMapFPix22->SetOption("colz");
  hpixMapFPix23 = fs->make<TH2F>("hpixMapFPix23"," disk 2, ring 3",442,0.,442.,1320,0.,1320.);
  hpixMapFPix23->SetOption("colz");
  hpixMapFPix24 = fs->make<TH2F>("hpixMapFPix24"," disk 2, ring 4",442,0.,442.,1320,0.,1320.);
  hpixMapFPix24->SetOption("colz");
  hpixMapFPix25 = fs->make<TH2F>("hpixMapFPix25"," disk 2, ring 5",442,0.,442.,1320,0.,1320.);
  hpixMapFPix25->SetOption("colz");


#endif

}

// ------------ method called to produce the data  ------------
void PixDigisTestPhase2::analyze(const edm::Event& iEvent, 
			   const edm::EventSetup& iSetup) {

  const bool MY_DEBUG = false;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  iSetup.get<TrackerTopologyRcd>().get(tTopo);
  const TrackerTopology* tt = tTopo.product();

  using namespace edm;
  if(PRINT) cout<<" Analyze PixDigisTestPhase2 for phase "<<phase1_<<endl;

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
  int numOfDigisPerDetF1 = 0;
  int totalNumOfDigisF11 = 0;
  int totalNumOfDigisF12 = 0;
  int totalNumOfDigisF13 = 0;
  int totalNumOfDigisF14 = 0;

  int numberOfDetUnitsF2 = 0;
  int totalNumOfDigisF2 = 0;
  int numOfDigisPerDetF2 = 0;

  int totalNumOfDigisF21  = 0;
  int numOfDigisPerDetF21 = 0;
  int totalNumOfDigisF22  = 0;
  int numOfDigisPerDetF22 = 0;
  int totalNumOfDigisF23  = 0;
  int numOfDigisPerDetF23 = 0;
  int totalNumOfDigisF24  = 0;
  int numOfDigisPerDetF24 = 0;
  int totalNumOfDigisF25  = 0;
  int numOfDigisPerDetF25 = 0;

  int numberOfDetUnitsF3 = 0;
  int totalNumOfDigisF3 = 0;
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
      cout<<"Det: "<<detid<<" "<<detId.rawId()<<" "<<detId.null()<<" "<<detType<<" "<<subid<<endl;
    
#ifdef HISTOS
    //hdetunit->Fill(float(detid));
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
    bool badL2Modules = false;
    bool newL1Modules = false;

    unsigned int disk=0; //1,2,3
    unsigned int blade=0; //1-24
    unsigned int bladeC=0; //1-24
    unsigned int side=0; //size=1 for -z, 2 for +z
    unsigned int panel=0; //panel=1
    unsigned int ring=0;

   // Subdet it, pix barrel=1, forward=2
    if(subid==2) {   // forward

      disk=tTopo->pxfDisk(detid); //1-12
      bladeC=tTopo->pxfBlade(detid); // returns ring 1-5
      zindex=tTopo->pxfModule(detid); // returns panel or blade 1-20,32,24,32 40,56,36,40,48
      side=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
      //panel=tTopo->pxfPanel(detid); //panel=1
      
      ring = bladeC;
      blade = zindex;

      if(PRINT) {
	cout<<"FPix (cmssw) disk "<<disk<<", ring "<<bladeC
	    <<", blade "<<zindex<<", side "<<side<<endl;
	if(MY_DEBUG) cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
			 <<pitchX<<" "<<pitchY<<endl;
      }

      // // correct way 
      // PixelEndcapName pfn(detid,tt,phase1_);
      // ring = pfn.ringName();
      // blade = pfn.bladeName();
      // if(PRINT) 
      // 	cout<<pfn.diskName()<<" "<<blade<<" "<<pfn.pannelName()<<" "
      // 	    <<ring<<" "<<pfn.plaquetteName()<<" "<<pfn.name()<<endl;

    } else if(subid == 1) { // Barrel 
      
      // Barell layer = 1,2,3
      layerC=tTopo->pxbLayer(detid);
      // Barrel ladder id 1-20,32,44.
      ladderC=tTopo->pxbLadder(detid);
      // Barrel Z-index=1,8
      zindex=tTopo->pxbModule(detid);

      layer=layerC;
      ladder = ladderC;
      module = zindex;

      // // Convert to online 
      // PixelBarrelName pbn(detid,tt,phase1_);
      // // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
      // PixelBarrelName::Shell sh = pbn.shell(); //enum
      // sector = pbn.sectorName();
      // ladder = pbn.ladderName();
      // layer  = pbn.layerName();
      // module = pbn.moduleName();
      // half  = pbn.isHalfModule();
      // shell = int(sh);
      // // change the module sign for z<0
      // if(shell==1 || shell==2) module = -module;
      // // change ladeer sign for Outer )x<0)
      // if(shell==1 || shell==3) ladder = -ladder;
      

      if(PRINT) { 
	cout<<" BPix layer/ladder/module (cmssw) "
	    <<layerC<<" "<<ladderC<<" "<<zindex<<endl;

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

    unsigned int numberOfDigis = 0;
    //unsigned int numDigisInCol = 0;
    //int oldCol=-1;

    // // Clear the module matrix
    // if(layer==1) {
    //   for(int col=0;col<416;++col)
    // 	for(int row=0;row<160;++row)
    // 	  oneModule[col][row]=0;
    // }

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
	//int channel = PixelChannelIdentifier::pixelToChannel(row,col);
	if(PRINT) cout <<numberOfDigis<< " Col: " << col << " Row: " << row 
		       << " ADC: " << adc <<endl;
	
#ifdef HISTOS
       if(layer==1) {
	 heloss1->Fill(float(adc));
	 hcols1->Fill(float(col));
	 hrows1->Fill(float(row));
	 hpixMap1->Fill(float(col),float(row));
	 hpdetMap1->Fill(float(module),float(ladder));
	 totalNumOfDigis1++;
	 numOfDigisPerDet1++;
	 hladder1id->Fill(float(ladder));
	 hz1id->Fill(float(module));
	   
       } else if(layer==2 ) {
	 heloss2->Fill(float(adc));
	 hcols2->Fill(float(col));
	 hrows2->Fill(float(row));
	 hpixMap2->Fill(float(col),float(row));
	 hpdetMap2->Fill(float(module),float(ladder));
	 hladder2id->Fill(float(ladder));
	 hz2id->Fill(float(module));

	 totalNumOfDigis2++;
	 numOfDigisPerDet2++;

       } else if(layer==3) {
	 heloss3->Fill(float(adc));
	 hcols3->Fill(float(col));
	 hrows3->Fill(float(row));
	 hpdetMap3->Fill(float(module),float(ladder));
	 hpixMap3->Fill(float(col),float(row));
	    hladder3id->Fill(float(ladder));
	    hz3id->Fill(float(module));
	 totalNumOfDigis3++;
	 numOfDigisPerDet3++;

       } else if(layer==4) {
	 heloss4->Fill(float(adc));
	 hcols4->Fill(float(col));
	 hrows4->Fill(float(row));
	 hpdetMap4->Fill(float(module),float(ladder));
	 hpixMap4->Fill(float(col),float(row));
	    hladder4id->Fill(float(ladder));
	    hz4id->Fill(float(module));
	 totalNumOfDigis4++;
	 numOfDigisPerDet4++;

       } else if( (disk>=1) && (disk<=8)) {

	 hpdetMapF1->Fill(float(blade),float(ring));
	 if(ring==1)      {
	     hpixMapFPix11->Fill(float(col),float(row));
	     helossF11->Fill(float(adc));
	     hcolsF11->Fill(float(col));
	     hrowsF11->Fill(float(row));
	     hblade11->Fill(float(blade));
	   } else if(ring==2) {
	     hpixMapFPix12->Fill(float(col),float(row));
	     helossF12->Fill(float(adc));
	     hcolsF12->Fill(float(col));
	     hrowsF12->Fill(float(row));
	     hblade12->Fill(float(blade));
	   } else if(ring==3) {
	     hpixMapFPix13->Fill(float(col),float(row));
	     helossF13->Fill(float(adc));
	     hcolsF13->Fill(float(col));
	     hrowsF13->Fill(float(row));
	     hblade13->Fill(float(blade));
	   } else if(ring==4) {
	     hpixMapFPix14->Fill(float(col),float(row));
	     helossF14->Fill(float(adc));
	     hcolsF14->Fill(float(col));
	     hrowsF14->Fill(float(row));
	     hblade14->Fill(float(blade));
	   } else cout<<" wrong ring "<<ring<<endl;

	   totalNumOfDigisF1++;
	   numOfDigisPerDetF1++;

       } else if( (disk>=8) && (disk<=12)) {

	 hpdetMapF2->Fill(float(blade),float(ring));
	 if(ring==1)      {
	   hpixMapFPix21->Fill(float(col),float(row));
	   helossF21->Fill(float(adc));
	   hcolsF21->Fill(float(col));
	   hrowsF21->Fill(float(row));
	   hblade21->Fill(float(blade));
	   totalNumOfDigisF21++;
	   numOfDigisPerDetF21++;
	 } else if(ring==2) {
	   hpixMapFPix22->Fill(float(col),float(row));
	   helossF22->Fill(float(adc));
	   hcolsF22->Fill(float(col));
	   hrowsF22->Fill(float(row));
	   hblade22->Fill(float(blade));
	   totalNumOfDigisF22++;
	   numOfDigisPerDetF22++;
	 } else if(ring==3) {
	   hpixMapFPix23->Fill(float(col),float(row));
	   helossF23->Fill(float(adc));
	   hcolsF23->Fill(float(col));
	   hrowsF23->Fill(float(row));
	   hblade23->Fill(float(blade));
	   totalNumOfDigisF23++;
	   numOfDigisPerDetF23++;
	 } else if(ring==4) {
	   hpixMapFPix24->Fill(float(col),float(row));
	   helossF24->Fill(float(adc));
	   hcolsF24->Fill(float(col));
	   hrowsF24->Fill(float(row));
	   hblade24->Fill(float(blade));
	   totalNumOfDigisF24++;
	   numOfDigisPerDetF24++;
	 } else if(ring==5) {
	   hpixMapFPix25->Fill(float(col),float(row));
	   helossF25->Fill(float(adc));
	   hcolsF25->Fill(float(col));
	   hrowsF25->Fill(float(row));
	   hblade25->Fill(float(blade));
	   totalNumOfDigisF25++;
	   numOfDigisPerDetF25++;
	 } else cout<<" wrong ring "<<ring<<endl;
	 
	 totalNumOfDigisF2++;
	 numOfDigisPerDetF2++;
	 
       } // end if layer

#endif
       
      } // end for digis in detunit

      //if(PRINT) cout<<" for det "<<detid<<" digis = "<<numberOfDigis<<endl;

#ifdef HISTOS
      // Some histos
      //if(valid) {  // to igore noise pixels
	if(subid==2) {   // forward
	  

	  hcolsF->Fill(float(cols));
	  hrowsF->Fill(float(rows));
	  hzr->Fill(detZ,detR);
	  
	  if( (disk>=1) && (disk<=8) ) {

	    hdetrF1->Fill(detR);
	    hdetzF1->Fill(detZ);
	    hdigisPerDetF1->Fill(float(numOfDigisPerDetF1));

	    if(ring==1)      hdigisPerDetF11->Fill(float(numOfDigisPerDetF1));
	    else if(ring==2) hdigisPerDetF12->Fill(float(numOfDigisPerDetF1));
	    else if(ring==3) hdigisPerDetF13->Fill(float(numOfDigisPerDetF1));
	    else if(ring==4) hdigisPerDetF14->Fill(float(numOfDigisPerDetF1));
	    numOfDigisPerDetF1=0;	
	    if(detZ<0.) hxy11->Fill(detX,detY);
	    else        hxy12->Fill(detX,detY);

	  } else if( (disk>=9) && (disk<=12) ) {
	    hdetrF2->Fill(detR);
	    hdetzF2->Fill(detZ);
	    hdigisPerDetF2->Fill(float(numOfDigisPerDetF2));

	    if(ring==1)      hdigisPerDetF21->Fill(float(numOfDigisPerDetF2));
	    else if(ring==2) hdigisPerDetF22->Fill(float(numOfDigisPerDetF2));
	    else if(ring==3) hdigisPerDetF23->Fill(float(numOfDigisPerDetF2));
	    else if(ring==4) hdigisPerDetF24->Fill(float(numOfDigisPerDetF2));
	    else if(ring==5) hdigisPerDetF25->Fill(float(numOfDigisPerDetF2));

	    if(disk==9)       hdigisInDiskF21->Fill(float(numOfDigisPerDetF2));
	    else if(disk==10) hdigisInDiskF22->Fill(float(numOfDigisPerDetF2));
	    else if(disk==11) hdigisInDiskF23->Fill(float(numOfDigisPerDetF2));
	    else if(disk==12) hdigisInDiskF24->Fill(float(numOfDigisPerDetF2));

	    numOfDigisPerDetF2=0;
	    if(detZ<0.) hxy21->Fill(detX,detY);
	    else        hxy22->Fill(detX,detY);

	  } // if disk

	} else if (subid==1) { // barrel
	  
	  hdetr->Fill(detR);
	  hdetz->Fill(detZ);
	  hcolsB->Fill(float(cols));
	  hrowsB->Fill(float(rows));
	  hxy->Fill(detX,detY);

	  hlayerid->Fill(float(layer));

	  if(layer==1) {
	    
	    hdigisPerDet1->Fill(float(numOfDigisPerDet1));
	    numOfDigisPerDet1=0;
	    hphiz1->Fill(detZ,detPhi);
	    
	  } else if(layer==2 ) {

	    hdigisPerDet2->Fill(float(numOfDigisPerDet2));
	    numOfDigisPerDet2=0;
	    hphiz2->Fill(detZ,detPhi);
	    
	  } else if(layer==3) {

	    hdigisPerDet3->Fill(float(numOfDigisPerDet3));
	    numOfDigisPerDet3=0;
	    hphiz3->Fill(detZ,detPhi);

	  } else if(layer==4) {

	    hdigisPerDet4->Fill(float(numOfDigisPerDet4));
	    numOfDigisPerDet4=0;
	    hphiz4->Fill(detZ,detPhi);
	    
	  } // layer

	} // if bpix	
	
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
    //hevent->Fill(float(event));
    hlumi1->Fill(float(lumiBlock));
    hbx1->Fill(float(bx));
    //horbit->Fill(float(orbit));

    hdigis1->Fill(float(totalNumOfDigis));
    float tmp = float(totalNumOfDigis1) + float(totalNumOfDigis2) 
      + float(totalNumOfDigis3)+ float(totalNumOfDigis3); 
    hdigis2->Fill(tmp);
    tmp = float(totalNumOfDigisF1) + float(totalNumOfDigisF2);  
    hdigis3->Fill(tmp);

    //if(bunch) hdigis4->Fill(float(totalNumOfDigis));
    //else hdigis5->Fill(float(totalNumOfDigis));

    //if(numberOfDetUnits>20) hbx3->Fill(float(bx));
    //if(totalNumOfDigis>100) hbx4->Fill(float(bx));
    //else if(totalNumOfDigis>4) hbx5->Fill(float(bx));
    //else hbx6->Fill(float(bx));

  }

  
#ifdef HISTOS
  hdigisPerLay1 ->Fill(float(totalNumOfDigis1));
  hdigisPerLay2 ->Fill(float(totalNumOfDigis2));
  hdigisPerLay3 ->Fill(float(totalNumOfDigis3));
  hdigisPerLay4 ->Fill(float(totalNumOfDigis4));
  //if(totalNumOfDigis1>0) hdetsPerLay1->Fill(float(numberOfDetUnits1));
  //if(totalNumOfDigis2>0) hdetsPerLay2->Fill(float(numberOfDetUnits2));
  //if(totalNumOfDigis3>0) hdetsPerLay3->Fill(float(numberOfDetUnits3));
  //if(totalNumOfDigis4>0) hdetsPerLay4->Fill(float(numberOfDetUnits4));

  hdigisPerLayF1 ->Fill(float(totalNumOfDigisF1));
  hdigisPerLayF2 ->Fill(float(totalNumOfDigisF2));

  // hdigisPerLayF11 ->Fill(float(totalNumOfDigisF11));
  // hdigisPerLayF12 ->Fill(float(totalNumOfDigisF12));
  // hdigisPerLayF13 ->Fill(float(totalNumOfDigisF13));
  // hdigisPerLayF14 ->Fill(float(totalNumOfDigisF14));

  // hdigisPerLayF21 ->Fill(float(totalNumOfDigisF21));
  // hdigisPerLayF22 ->Fill(float(totalNumOfDigisF22));
  // hdigisPerLayF23 ->Fill(float(totalNumOfDigisF23));
  // hdigisPerLayF24 ->Fill(float(totalNumOfDigisF24));
  // hdigisPerLayF25 ->Fill(float(totalNumOfDigisF21));

  //hdigisPerLayF3 ->Fill(float(totalNumOfDigisF3));
  //hdetsPerLayF1 ->Fill(float(numberOfDetUnitsF1));
  //hdetsPerLayF2 ->Fill(float(numberOfDetUnitsF2));
  //hdetsPerLayF3 ->Fill(float(numberOfDetUnitsF3));
#endif

}
// ------------ method called to at the end of the job  ------------
void PixDigisTestPhase2::endJob(){
  cout << " End PixDigisTestPhase2 " << endl;
  if(count0>0) 
    cout<<" events "<<count0<<" full events "<<count3<<" dets per event "<<count1/count0
	<<" digis per event "<<count2/count0<<endl;
  else 
    cout<<count0<<" "<<count1<<" "<<count2<<" "<<count3<<endl;

  //cout<<" Cols with many hits "<<countFullDcols<<endl;

//   if(0) {
//     int emptyROC=0, totalEmptyCols=0, rocsWithEmptyCols=0;
//     for(int ind=0;ind<120;++ind) {
//       if( !validIndex(ind) ) continue;
//       for(int roc=0;roc<16;++roc) {
// 	int rocCount=0;
// 	int emptyCols=0;
// 	for(int dcol=0;dcol<26;++dcol) {
// 	  int count = dCols[ind][roc][dcol];
// 	  rocCount += count;
// #ifdef DCOLS
// 	  // average number of hits per dcol per event 
// 	  hDcolsCount->Fill(float(count)/float(count3));
// #endif
// 	  if(count==0) {
// 	    emptyCols++;
// 	    //cout<<" empty dcol "<<dcol<<" "<<roc<<" "<<ind<<" "
// 	    //	<<validIndex(ind,true)<<endl;
// 	  } else if( (float(count)/float(count3)) >20) {
// 	    cout<<" hot dcol "<<count<<" "<<dcol<<" "
// 		<<roc<<" "<<ind<<endl;
// 	    //} else {
// 	    //cout<<" count "<<count<<" "<<dcol<<" "
// 	    //	<<roc<<" "<<ind<<" "<<validIndex(ind,true)<<endl;
// 	  }
// 	} // dcol
// 	if(rocCount==0) {emptyROC++;} //cout<<" empty roc "<<ind<<" "<<roc<<endl;
// 	else {
// 	  if(emptyCols>0) { 
// 	    totalEmptyCols += emptyCols;
// 	    rocsWithEmptyCols++;
// 	    cout<<" empty cols in module (index)="<<ind<<" roc "<<roc<<" num "<<emptyCols<<endl;
// 	  }
// 	}
//       } // roc
//     } // module 
//     cout<<" empty ROCs "<<emptyROC<<" empty cols "<<totalEmptyCols
// 	<<" rocs with empty cols"<<rocsWithEmptyCols<<endl;
//   }

  float norm = 1.;
  if(count3>0) {
    norm = 1./float(count3);
  }

  // Rescale all 2D plots
  //hdetMap1->Scale(norm);
  //hdetMap2->Scale(norm);
  //hdetMap3->Scale(norm);
  //hdetMap4->Scale(norm);
  hpdetMap1->Scale(norm);
  hpdetMap2->Scale(norm);
  hpdetMap3->Scale(norm);
  hpdetMap4->Scale(norm);
  //hpdetMaps1->Scale(norm);
  //hpdetMaps2->Scale(norm);

  hpixMapFPix11->Scale(norm);
  hpixMapFPix12->Scale(norm);
  hpixMapFPix21->Scale(norm);
  hpixMapFPix22->Scale(norm);
  //hpixMapFPix31->Scale(norm);
  //hpixMapFPix32->Scale(norm);


  hladder1id->Scale(norm);
  hladder2id->Scale(norm);
  hladder3id->Scale(norm);
  hladder4id->Scale(norm);
  hz1id->Scale(norm);
  hz2id->Scale(norm);
  hz3id->Scale(norm);
  hz4id->Scale(norm);
  hblade11->Scale(norm);
  hblade12->Scale(norm);
  hblade13->Scale(norm);
  hblade14->Scale(norm);
  hblade21->Scale(norm);
  hblade22->Scale(norm);
  hblade23->Scale(norm);
  hblade24->Scale(norm);
  hblade25->Scale(norm);

#ifdef DCOLS
  hAllDcols->Scale(norm);
#endif

}

//define this as a plug-in
DEFINE_FWK_MODULE(PixDigisTestPhase2);
