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
#include <TProfile2D.h>

#define HISTOS
//#define L1
#define HLT
//#define SINGLE_MODULES
#define DCOLS
#define DO_THR // use the threshold histo

#define USE_GAINS 
#ifdef USE_GAINS
// Database payloads
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationServiceBase.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationService.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationOfflineService.h"
//#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationForHLTService.h"
#endif

using namespace std;

namespace {
  const bool useNewL1 = false;
  const bool useBadL2 = false;
}
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

  //TFile* hFile;
  TH1F *hdetunit;
  TH1F *hadc1,*hadc2, *hadc3, *hadc4, *hadc1n;
  TH1F *hnadc1,*hnadc2, *hnadc3, *hnadc4;  
  TH1F *hadcF11,*hadcF21,*hadcF31,*hadcF12,*hadcF22,*hadcF32;
  TH1F *hpixid,*hpixsubid,*hlayerid,*hshellid,*hsectorid,
    *hladder1id,*hladder2id,*hladder3id,*hladder4id,
    *hz1id,*hz2id,*hz3id,*hz4id;
  TH1F *hcols1,*hcols2,*hcols3,*hrows1,*hrows2,*hrows3,*hcols4,*hrows4;
  TH1F *hcolsF11,*hcolsF21,*hcolsF31,*hrowsF11,*hrowsF21,*hrowsF31,
    *hcolsF12,*hcolsF22,*hcolsF32,*hrowsF12,*hrowsF22,*hrowsF32;
  TH1F *hdigisPerDet1,*hdigisPerDet2,*hdigisPerDet3,*hdigisPerDet4;
  TH1F *hdigisPerLay1,*hdigisPerLay2,*hdigisPerLay3,*hdigisPerLay4;
  TH1F *hdetsPerLay1,*hdetsPerLay2,*hdetsPerLay3,*hdetsPerLay4;
  TH1F *hdigisPerDetF11,*hdigisPerDetF21,*hdigisPerDetF31,
    *hdigisPerDetF12,*hdigisPerDetF22,*hdigisPerDetF32;
  TH1F *hdigisPerLayF1,*hdigisPerLayF2,*hdigisPerLayF3;
  TH1F *hdetsPerLayF1,*hdetsPerLayF2,*hdetsPerLayF3;
  TH1F *hdetr, *hdetz, *hdetrF, *hdetzF;
  TH1F *hcolsB,  *hrowsB,  *hcolsF,  *hrowsF;
  TH1F *hcols1Many,*hhitsPerDcol, *hhitsPerHitDcol, *hhitDcolsPerModule, 
    *hfullDcolsPerModule,*hfullDcolsPerROC,*hfullDcolsPerEvent;
  TProfile *hhitsPerHitDcolModule,*hhitsPerHitDcolLadder,*hDcolHitProbabilityModule,
    *hDcolHitProbabilityLadder;

  TH1F *hblade11, *hblade21, *hblade31, *hblade12, *hblade22, *hblade32;
  TH1F *hfract, *hsimlinks;
  
  TH2F *hpdetMap4,*hpdetMap3,*hpdetMap2,*hpdetMap1; 
  TH2F *hdetMap4,*hdetMap3,*hdetMap2,*hdetMap1; 
  TH2F *hpixMap1, *hpixMap2, *hpixMap3,*hpixMap4;
  TH2F *hpdetMaps1_4,*hpdetMaps1_3,*hpdetMaps1_2,*hpdetMaps1_1; 
  TH2F *hrocMap1_4,*hrocMap1_3,*hrocMap1_2,*hrocMap1_1; 
  TH2F *hrocMapS1,*hrocMapS2,*hrocMapS3,*hrocMapS4; 
  TH1D *hrocEne1,*hrocEne2,*hrocEne3,*hrocEne4; 
   
  //* hpixMapNoise; 
  TH2F *hxy, *hphiz1, *hphiz2, *hphiz3, *hphiz4; // bpix 
  TH2F *hzr, *hxy11, *hxy12, *hxy21, *hxy22, *hxy31, *hxy32;  // fpix 

#ifdef SINGLE_MODULES
  // single modules 
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
#ifdef DCOLS
  TH1F *hDcolsCount, *hAllDcols;
#endif 

  TH1F *hevent, *hlumi, *horbit, *hbx0, *hlumi0, *hlumi1,*hbx1,*hbx2,*hbx3,*hbx4,*hbx5,*hbx6;
  TH1F *hdets, *hdigis, *hdigis0, *hdigis1, *hdigis2,*hdigis3,*hdigis4,*hdigis5; 
  TProfile *hadc1ls,*hadc2ls,*hadc3ls,*hadc4ls,*hadc0ls; 
  TProfile *hadc1bx,*hadc2bx,*hadc3bx,*hadc4bx,*hadc0bx; 
  TH1F *hdcolFull1ls,*hdcolFull1bx,*hadc01bx,*helec01bx,*hadc02bx,*helec02bx; 

#ifdef USE_GAINS
  TH1F *helectrons1, *helectrons2, *helectrons3, *helectrons4, *helectronsf, *helectrons1n;
  TH1F *hvcal1,*hvcal2, *hvcal3, *hvcal4, *hvcalf, *hvcal1n;
#endif

  // FPix
  TH2F *hpixMapFPix11, *hpixMapFPix21, *hpixMapFPix31, 
    *hpixMapFPix12, *hpixMapFPix22, *hpixMapFPix32;
#ifdef DO_THR
  TH2F *hthr;
#endif

#endif

  edm::InputTag src_;  
  bool phase1_;
  int countFullDcols;
  int oneModule[416][160];
  int dCols[120][16][26];

#ifdef USE_GAINS
  SiPixelGainCalibrationServiceBase * theSiPixelGainCalibration_;
#endif

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

#ifdef USE_GAINS
  theSiPixelGainCalibration_ = new SiPixelGainCalibrationOfflineService(iConfig);
#endif

}


PixDigisTest::~PixDigisTest() {
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  cout<<" Destroy PixDigisTest "<<endl;

}


#ifdef USE_GAINS
float PixDigisTest::calibrate(uint32_t detid, int adc, int col, int row) {
          
  float DBgain     = theSiPixelGainCalibration_->getGain(detid, col, row);
  float pedestal   = theSiPixelGainCalibration_->getPedestal(detid, col, row);
  float DBpedestal = pedestal * DBgain;
  //float DBpedestal = theSiPixelGainCalibration_->getPedestal(detid, col, row) * DBgain;
          
          
  // Roc-6 average
  //const float gain = 1./0.313; // 1 ADC = 3.19 VCALs 
  //const float pedestal = -6.2 * gain; // -19.8
  // 
  float vcal = adc * DBgain - DBpedestal;
  //int electrons=0;
  //if (layer_==1) {
  //electrons = int( vcal * theConversionFactor_L1 + theOffset_L1); 
  //} else {
  //electrons = int( vcal * theConversionFactor + theOffset); 
  //}

  return vcal;
}
#endif



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

    hdigisPerDetF11 = fs->make<TH1F>( "hdigisPerDetF11", "Digis per det d1, r1", 
			      200, -0.5, 199.5);
    hdigisPerDetF21 = fs->make<TH1F>( "hdigisPerDetF21", "Digis per det d2, r1", 
			      200, -0.5, 199.5);
    hdigisPerDetF31 = fs->make<TH1F>( "hdigisPerDetF31", "Digis per det d3, r1", 
			      200, -0.5, 199.5);
    hdigisPerDetF12 = fs->make<TH1F>( "hdigisPerDetF12", "Digis per det d1, r2", 
			      200, -0.5, 199.5);
    hdigisPerDetF22 = fs->make<TH1F>( "hdigisPerDetF22", "Digis per det d2, r2", 
			      200, -0.5, 199.5);
    hdigisPerDetF32 = fs->make<TH1F>( "hdigisPerDetF32", "Digis per det d3, r2", 
			      200, -0.5, 199.5);
    hdigisPerLayF1 = fs->make<TH1F>( "hdigisPerLayF1", "Digis per layer d1", 
			      2000, -0.5, 19999.5);
    hdigisPerLayF2 = fs->make<TH1F>( "hdigisPerLayF2", "Digis per layer d2", 
			      2000, -0.5, 19999.5);
    hdigisPerLayF3 = fs->make<TH1F>( "hdigisPerLayF3", "Digis per layer d3", 
			      2000, -0.5, 19999.5);
    hdetsPerLayF1 = fs->make<TH1F>( "hdetsPerLayF1", "Full dets per layer d1", 
			      257, -0.5, 256.5);
    hdetsPerLayF2 = fs->make<TH1F>( "hdetsPerLayF2", "Full dets per layer d2", 
			      257, -0.5, 256.5);
    hdetsPerLayF3 = fs->make<TH1F>( "hdetsPerLayF3", "Full dets per layer d3", 
			      257, -0.5, 256.5);

    hadc1 = fs->make<TH1F>( "hadc1", "Pix charge l1", 256, 0., 256.);
    hadc1n= fs->make<TH1F>( "hadc1n", "Pix charge l1", 256, 0., 256.);
    hadc2 = fs->make<TH1F>( "hadc2", "Pix charge l2", 256, 0., 256.);
    hadc3 = fs->make<TH1F>( "hadc3", "Pix charge l3", 256, 0., 256.);
    hadc4 = fs->make<TH1F>( "hadc4", "Pix charge l4", 256, 0., 256.);
    hnadc1 = fs->make<TH1F>( "hnadc1", "Pix adc elec<0 l1", 256, 0., 256.);
    hnadc2 = fs->make<TH1F>( "hnadc2", "Pix adc elec<0 l2", 256, 0., 256.);
    hnadc3 = fs->make<TH1F>( "hnadc3", "Pix adc elec<0 l3", 256, 0., 256.);
    hnadc4 = fs->make<TH1F>( "hnadc4", "Pix adc elec<0 l4", 256, 0., 256.);

    hcols1 = fs->make<TH1F>( "hcols1", "Layer 1 cols", 500,-1.5,498.5);
    hcols2 = fs->make<TH1F>( "hcols2", "Layer 2 cols", 500,-1.5,498.5);
    hcols3 = fs->make<TH1F>( "hcols3", "Layer 3 cols", 500,-1.5,498.5);
    hcols4 = fs->make<TH1F>( "hcols4", "Layer 4 cols", 500,-1.5,498.5);
    hcols1Many = fs->make<TH1F>( "hcols1Many", "Layer 1 cols with many hits", 500,-1.5,498.5);
    hfullDcolsPerROC = fs->make<TH1F>( "hfullDcolsPerROC", "dcols with many hits per ROC",30,-1.5,28.5);
    hfullDcolsPerModule = fs->make<TH1F>( "hfullDcolsPerModule", "dcols with many hits per module",100,-1.5,98.5);
    hfullDcolsPerEvent = fs->make<TH1F>( "hfullDcolsPerEvent", "dcols with many hits per event", 
					  500,-1.5,498.5);
    hhitsPerDcol= fs->make<TH1F>( "hhitsPerDcol","Layer 1: hits per dcol",200,-1.5,198.5);
    hhitsPerHitDcol= fs->make<TH1F>( "hhitsPerHitDcol","Layer 1: hits per hit dcol",200,-1.5,198.5);
    hhitDcolsPerModule= fs->make<TH1F>( "hhitDcolsPerModule","Layer 1: hit dcol per Hit Module",200,-0.5,399.5);
    hhitsPerHitDcolModule=fs->make<TProfile>("hhitsPerHitDcolModule"," hits per hit dcol",
					     9,-4.5,4.5,0.,1000.);
    hhitsPerHitDcolLadder=fs->make<TProfile>("hhitsPerHitDcolLadder"," hits per hit dcol",
					     13,-6.5,6.5,0.,1000.);
    hDcolHitProbabilityModule=fs->make<TProfile>("hDcolHitProbabilityModule","dcol hit probability",
					     9,-4.5,4.5,0.,1.);
    hDcolHitProbabilityLadder=fs->make<TProfile>("hDcolHitProbabilityLadder","dcol hit probability",
					     13,-6.5,6.5,0.,1.);

    hrows1 = fs->make<TH1F>( "hrows1", "Layer 1 rows", 200,-1.5,198.5);
    hrows2 = fs->make<TH1F>( "hrows2", "Layer 2 rows", 200,-1.5,198.5);
    hrows3 = fs->make<TH1F>( "hrows3", "layer 3 rows", 200,-1.5,198.5);
    hrows4 = fs->make<TH1F>( "hrows4", "layer 4 rows", 200,-1.5,198.5);
    //hrows1big = fs->make<TH1F>( "hrows1big", "Layer 1 big rows", 200,-1.5,198.5);
 
    hblade11 = fs->make<TH1F>( "hblade11", "blade num, disk1", 60, 0., 60.);
    hblade21 = fs->make<TH1F>( "hblade21", "blade num, disk2", 60, 0., 60.);
    hblade31 = fs->make<TH1F>( "hblade31", "blade num, disk3", 60, 0., 60.);
    hblade12 = fs->make<TH1F>( "hblade12", "blade num, disk1", 60, 0., 60.);
    hblade22 = fs->make<TH1F>( "hblade22", "blade num, disk2", 60, 0., 60.);
    hblade32 = fs->make<TH1F>( "hblade32", "blade num, disk3", 60, 0., 60.);

    hadcF11 = fs->make<TH1F>( "hadcF11", "Pix charge d1", 100, 0., 300.);
    hadcF21 = fs->make<TH1F>( "hadcF21", "Pix charge d2", 100, 0., 300.);
    hadcF31 = fs->make<TH1F>( "hadcF31", "Pix charge d3", 100, 0., 300.);
    hadcF12 = fs->make<TH1F>( "hadcF12", "Pix charge d1", 100, 0., 300.);
    hadcF22 = fs->make<TH1F>( "hadcF22", "Pix charge d2", 100, 0., 300.);
    hadcF32 = fs->make<TH1F>( "hadcF32", "Pix charge d3", 100, 0., 300.);
    hcolsF11 = fs->make<TH1F>( "hcolsF11", "Disk 1 cols", 500,-1.5,498.5);
    hcolsF21 = fs->make<TH1F>( "hcolsF21", "Disk 2 cols", 500,-1.5,498.5);
    hcolsF31 = fs->make<TH1F>( "hcolsF31", "Disk 3 cols", 500,-1.5,498.5);
    hcolsF12 = fs->make<TH1F>( "hcolsF12", "Disk 1 cols", 500,-1.5,498.5);
    hcolsF22 = fs->make<TH1F>( "hcolsF22", "Disk 2 cols", 500,-1.5,498.5);
    hcolsF32 = fs->make<TH1F>( "hcolsF32", "Disk 3 cols", 500,-1.5,498.5);
    hrowsF11 = fs->make<TH1F>( "hrowsF11", "Disk 1 rows", 200,-1.5,198.5);
    hrowsF21 = fs->make<TH1F>( "hrowsF21", "Disk 2 rows", 200,-1.5,198.5);
    hrowsF31 = fs->make<TH1F>( "hrowsF31", "Disk 3 rows", 200,-1.5,198.5);
    hrowsF12 = fs->make<TH1F>( "hrowsF12", "Disk 1 rows", 200,-1.5,198.5);
    hrowsF22 = fs->make<TH1F>( "hrowsF22", "Disk 2 rows", 200,-1.5,198.5);
    hrowsF32 = fs->make<TH1F>( "hrowsF32", "Disk 3 rows", 200,-1.5,198.5);

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

    hpdetMaps1_1 = fs->make<TH2F>("hpdetMaps1_1","hits in l1 with adc=0",9,-4.5,4.5,13,-6.5,6.5);
    hpdetMaps1_1->SetOption("colz");
    hpdetMaps1_2 = fs->make<TH2F>("hpdetMaps1_2","l1 with many hits per col",9,-4.5,4.5,13,-6.5,6.5);
    hpdetMaps1_2->SetOption("colz");
    hpdetMaps1_3 = fs->make<TH2F>("hpdetMaps1_3","l1 with elec<0",9,-4.5,4.5,13,-6.5,6.5);
    hpdetMaps1_3->SetOption("colz");
    //hpdetMaps4 = fs->make<TH2F>("hpdetMaps1_4","l1 with elec<0",9,-4.5,4.5,13,-6.5,6.5);
    //hpdetMaps4->SetOption("colz");

    hrocMap1_1 = fs->make<TH2F>("hrocMap1_1","adc=0",     8*9,-4.5,4.5,2*13,-6.5,6.5);
    hrocMap1_1->SetOption("colz");
    hrocMap1_2 = fs->make<TH2F>("hrocMap1_2","busy dcols",8*9,-4.5,4.5,2*13,-6.5,6.5);
    hrocMap1_2->SetOption("colz");
    hrocMap1_3 = fs->make<TH2F>("hrocMap1_3","elec<0",    8*9,-4.5,4.5,2*13,-6.5,6.5);
    hrocMap1_3->SetOption("colz");
    hrocMap1_4 = fs->make<TH2F>("hrocMap1_4","occupancy", 8*9,-4.5,4.5,2*13,-6.5,6.5);
    hrocMap1_4->SetOption("colz");

    hrocMapS1 = fs->make<TH2F>("hrocMapS1","energy deposited L1", 8*9,-4.5,4.5,2*13,-6.5,6.5);
    hrocMapS2 = fs->make<TH2F>("hrocMapS2","energy deposited L2", 8*9,-4.5,4.5,2*29,-14.5,14.5);
    hrocMapS3 = fs->make<TH2F>("hrocMapS3","energy deposited L3", 8*9,-4.5,4.5,2*45,-22.5,22.5);
    hrocMapS4 = fs->make<TH2F>("hrocMapS4","energy deposited L4", 8*9,-4.5,4.5,2*65,-32.5,32.5);
    hrocMapS1->SetOption("colz");
    hrocMapS2->SetOption("colz");
    hrocMapS3->SetOption("colz");
    hrocMapS4->SetOption("colz");

    hrocEne1 = fs->make<TH1D>("hrocEne1","energy deposited L1", 8*9,-4.5,4.5);
    hrocEne2 = fs->make<TH1D>("hrocEne2","energy deposited L2", 8*9,-4.5,4.5);
    hrocEne3 = fs->make<TH1D>("hrocEne3","energy deposited L3", 8*9,-4.5,4.5);
    hrocEne4 = fs->make<TH1D>("hrocEne4","energy deposited L4", 8*9,-4.5,4.5);

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

  hdcolFull1ls = fs->make<TH1F>("hdcolFull1ls","dcol-full vs ls",1000,-0.5,999.5);
  hdcolFull1bx = fs->make<TH1F>("hdcolFull1bx","dcol-full vs bx",3600,-0.5,3599.5);
  hadc01bx     = fs->make<TH1F>("hadc01bx","adc0 vs bx",         3600,-0.5,3599.5);
  helec01bx    = fs->make<TH1F>("helec01bx","elec0 vs bx",       3600,-0.5,3599.5);
  //hdcolFull2bx = fs->make<TH1F>("hdcolFull2bx","dcol-full vs bx",3600,-0.5,3599.5);
  hadc02bx     = fs->make<TH1F>("hadc02bx","adc0 vs bx",         3600,-0.5,3599.5);
  helec02bx    = fs->make<TH1F>("helec02bx","elec0 vs bx",       3600,-0.5,3599.5);

#ifdef DCOLS 
  hDcolsCount = fs->make<TH1F>( "hDcolsCount", "L1: Hits per dcol per event",500,0.,5.);
  hAllDcols   = fs->make<TH1F>( "hAllDcols", "L1: hits per dcol per event",50000,0.,50000.);
#endif

#ifdef USE_GAINS
  helectrons1 = fs->make<TH1F>("helectrons1","adc in electrons l1",  2400,-40.,80.);
  helectrons1n = fs->make<TH1F>("helectrons1n","adc in electrons l1",  2400,-40.,80.);
  helectrons2 = fs->make<TH1F>("helectrons2","adc in electrons l2",  2400,-40.,80.);
  helectrons3 = fs->make<TH1F>("helectrons3","adc in electrons l3",  2400,-40.,80.);
  helectrons4 = fs->make<TH1F>("helectrons4","adc in electrons l4",  2400,-40.,80.);
  helectronsf = fs->make<TH1F>("helectronsf","adc in electrons fpix",2400,-40.,80.);
  hvcal1  = fs->make<TH1F>("hvcal1","adc in vcals l1",  600,-100.,500.);
  hvcal1n = fs->make<TH1F>("hvcal1n","adc in vcals l1", 600,-100.,500.);
  hvcal2  = fs->make<TH1F>("hvcal2","adc in vcals l2",  600,-100.,500.);
  hvcal3  = fs->make<TH1F>("hvcal3","adc in vcals l3",  600,-100.,500.);
  hvcal4  = fs->make<TH1F>("hvcal4","adc in vcals l4",  600,-100.,500.);
  hvcalf  = fs->make<TH1F>("hvcalf","adc in vcals fpix",600,-100.,500.);
#endif

  // FPix
  hpixMapFPix11 = fs->make<TH2F>("hpixMapFPix11"," disk 1, ring 1",416,0.,416.,160,0.,160.);
  hpixMapFPix11->SetOption("colz");
  hpixMapFPix21 = fs->make<TH2F>("hpixMapFPix21"," disk 2, ring 1",416,0.,416.,160,0.,160.);
  hpixMapFPix21->SetOption("colz");
  hpixMapFPix31 = fs->make<TH2F>("hpixMapFPix31"," disk 3, ring 1",416,0.,416.,160,0.,160.);
  hpixMapFPix31->SetOption("colz");
  hpixMapFPix12 = fs->make<TH2F>("hpixMapFPix12"," disk 1, ring 2",416,0.,416.,160,0.,160.);
  hpixMapFPix12->SetOption("colz");
  hpixMapFPix22 = fs->make<TH2F>("hpixMapFPix22"," disk 2, ring 2",416,0.,416.,160,0.,160.);
  hpixMapFPix22->SetOption("colz");
  hpixMapFPix32 = fs->make<TH2F>("hpixMapFPix32"," disk 3, ring 2",416,0.,416.,160,0.,160.);
  hpixMapFPix32->SetOption("colz");
#ifdef DO_THR
  hthr = fs->make<TH2F>( "hthr", "threshold in vcal",
				  13000,0.,13000,50,0.,100.);
#endif
  
#endif

}

// ------------ method called to produce the data  ------------
void PixDigisTest::analyze(const edm::Event& iEvent, 
			   const edm::EventSetup& iSetup) {

  const bool MY_DEBUG = false;
  const bool rescaleVcal = false; // to try escaling vcal to account for radiation

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  iSetup.get<TrackerTopologyRcd>().get(tTopo);
  const TrackerTopology* tt = tTopo.product();

#ifdef USE_GAINS
  //Setup gain calibration service
  theSiPixelGainCalibration_->setESObjects( iSetup );
#endif


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
    else if(select1==9999) { if(event!=select2) return; } 
    else { if( !((bx>=select1) && (bx<=select2)) ) return; } // skip bx outside the select1-select2 region 
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
  int countFullDcolsInEvent = 0;

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

#ifdef HISTOS
      hdetrF->Fill(detR);
      hdetzF->Fill(detZ);
      hcolsF->Fill(float(cols));
      hrowsF->Fill(float(rows));
#endif

      
      disk=tTopo->pxfDisk(detid); //1,2,3
      bladeC=tTopo->pxfBlade(detid); //1-24
      zindex=tTopo->pxfModule(detid); //
      side=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
      panel=tTopo->pxfPanel(detid); //panel=1
      
      if(PRINT) {
	cout<<"FPix (cmssw) disk "<<disk<<", blade "<<bladeC
	    <<", module "<<zindex<<", side "<<side<<", panel "
		    <<panel<<endl;
	if(MY_DEBUG) cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
			 <<pitchX<<" "<<pitchY<<endl;
      }

      // correct way 
      PixelEndcapName pfn(detid,tt,phase1_);
      ring = pfn.ringName();
      blade = pfn.bladeName();
      if(PRINT) 
	cout<<pfn.diskName()<<" "<<blade<<" "<<pfn.pannelName()<<" "
	    <<ring<<" "<<pfn.plaquetteName()<<" "<<pfn.name()<<endl;



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
      


      if( (layer==2) && (select1==99) && useBadL2 ) {
	if( (ladder ==-1) && ( (module == 1) || (module == 2) || (module == 3)) ) badL2Modules=true;
	else if( (ladder ==-5) &&( (module == -1) || (module == -2) || (module == -3)) ) badL2Modules=true;
	else if( (ladder == 14) && (module == -1) ) badL2Modules=true;
	else if( (ladder == 13) && (module == -4) ) badL2Modules=true;
	else if( (ladder == 12) && (module == -1) ) badL2Modules=true;
	else if( (ladder == 11) && (module == -4) ) badL2Modules=true;
      }

      // find inner and outer modules for layer 1 onl
      if( (layer==1 && useNewL1 ) ) {
	//if( (ladder==2) || (ladder==4) || (ladder==6) ||
	//  (ladder==-1) || (ladder==-3) || (ladder==-5) ) inner=true;
	//else inner=false;

	if     ( (ladder ==-1) && (module == 3) ) newL1Modules=true;
	else if( (ladder ==-3) && (module == 3) ) newL1Modules=true;
	else if( (ladder ==-1) && (module ==-3) ) newL1Modules=true;
	else if( (ladder ==-1) && (module ==-1) ) newL1Modules=true;
	else if( (ladder ==-3) && (module ==-1) ) newL1Modules=true;
	else if( (ladder ==-5) && (module ==-1) ) newL1Modules=true;

      }
      

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

        int roc = rocId(col,row);  // 0-15, column, row
        int link = int(roc/8); // link 0 & 1
        int rocInCol = roc%8; // 0-7
	float rocZ=0;
        if(module>0) rocZ = float(module) + (0.125/2.) - (float(rocInCol) * 0.125); //z
        else         rocZ = float(module) + 1.0 + (0.125/2.) - (float(rocInCol) * 0.125); //z
        float rocPhi = float(ladder) - 0.5 + (0.5/2.)   + (float(link) * 0.5); 
	

#ifdef USE_GAINS
	// Apply the calibration 
	float vcal = calibrate(detid, adc, col, row);
	float electrons=0.;
	const float conversionFactor_L1=50., conversionFactor=47., offset=-60., offset_L1=-670.; 
	  //if (layer_==1) {
	  //electrons = int( vcal * theConversionFactor_L1 + theOffset_L1); 
          //} else {
	  //electrons = int( vcal * theConversionFactor + theOffset); 
          //}
#endif
	
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
       float weight=1.; // adc; 
       float pixy = col; float pixx=row;
#endif
       if(layer==1) { // convert adc to kelec
	 if(rescaleVcal) electrons = (vcal * 53.9 + offset_L1)/1000.; //L1 at 51.5fb-1 50->53.9
	 else            electrons = (vcal * conversionFactor_L1 + offset_L1)/1000.; //default 
	 if(newL1Modules) {
	   helectrons1n->Fill(electrons);
	   hvcal1n->Fill(vcal);
	   hadc1n->Fill(float(adc));
	 } else {
	   helectrons1->Fill(electrons);
	   hvcal1->Fill(vcal);
	   hadc1->Fill(float(adc));
	 }
#ifdef DO_THR
	 int rindex = (ladder+6)*1000 + (module+4)*100 + roc;
	 hthr->Fill(rindex,vcal);
#endif
	 hcols1->Fill(float(col));
	 hrows1->Fill(float(row));
	 hpixMap1->Fill(float(col),float(row));
	 hrocMap1_4->Fill(rocZ,rocPhi);
	 float energy = electrons * 3.61; // convert electrons to keV
	 hrocMapS1->Fill(rocZ,rocPhi,energy);
	 hrocEne1->Fill(rocZ,energy);
	 hpdetMap1->Fill(float(module),float(ladder));
	 if(adc<1) {
	   hpdetMaps1_1->Fill(float(module),float(ladder));
	   hrocMap1_1->Fill(rocZ,rocPhi);
	   hadc01bx->Fill(float(bx));
	 }
	 if(electrons<0) { 
	   hpdetMaps1_3->Fill(float(module),float(ladder));
	   hnadc1->Fill(float(adc));
	   helec01bx->Fill(float(bx));
	   hrocMap1_3->Fill(rocZ,rocPhi);
	 }
	 hadc1ls->Fill(float(lumiBlock),float(adc));
	 hadc1bx->Fill(float(bx),float(adc));

#ifdef SINGLE_MODULES
	    if     ( ladder==-1 && module== 1) hpixDetMap10->Fill(pixy,pixx,weight); // 
	    else if( ladder==-2 && module== 1) hpixDetMap11->Fill(pixy,pixx,weight); // "
	    else if( ladder==-3 && module== 1) hpixDetMap12->Fill(pixy,pixx,weight); // "
	    else if( ladder== 1 && module== 1) hpixDetMap13->Fill(pixy,pixx,weight); // 
	    else if( ladder== 1 && module==-1) hpixDetMap14->Fill(pixy,pixx,weight); // 
	    float weight1=electrons;
	    if     ( ladder==-1 && module== 1) hpixDetMap15->Fill(pixy,pixx,weight1); // 
	    else if( ladder==-2 && module== 1) hpixDetMap16->Fill(pixy,pixx,weight1); //
	    else if( ladder==-3 && module== 1) hpixDetMap17->Fill(pixy,pixx,weight1); // 
	    else if( ladder== 1 && module== 1) hpixDetMap18->Fill(pixy,pixx,weight1); // 
	    else if( ladder== 1 && module==-1) hpixDetMap19->Fill(pixy,pixx,weight1); // 

	    //if     ( ladder==-1 && module==-4) hpixDetMap11->Fill(pixy,pixx,1.); // "
	    //else if( ladder== 6 && module== 1) hpixDetMap12->Fill(pixy,pixx,1.); // 

#endif

	   totalNumOfDigis1++;
	   //htest2->Fill(float(module),float(adc));
	   numOfDigisPerDet1++;

//old 	   if(RectangularPixelTopology::isItBigPixelInX(row)) {
//new	   if(topology.isItBigPixelInX(row)) { 
// 	     //cout<<" big in X "<<row<<endl;
// 	     hadc1bigx->Fill(float(adc));
// 	     hrows1big->Fill(float(row));
// 	   }
//old	   if(RectangularPixelTopology::isItBigPixelInY(col)) {
//new	   if(topology.isItBigPixelInY(col)) {
// 	     //cout<<" big in Y "<<col<<endl;
// 	     hadc1bigy->Fill(float(adc));
// 	     hcols1big->Fill(float(col));
// 	   }

       } else if(layer==2 && !badL2Modules) {
	 if(rescaleVcal) electrons = (vcal * 47.8 + offset)/1000.; //L2 at 51.5fb-1 47->47.8
	 else            electrons = (vcal * conversionFactor + offset)/1000.; //default 
	 helectrons2->Fill(electrons);
	 hvcal2->Fill(vcal);
	 float energy = electrons * 3.61; // convert electrons to keV
	 hrocMapS2->Fill(rocZ,rocPhi,energy);
	 hrocEne2->Fill(rocZ,energy);

        // look for the noisy pixel
	 //noise = false; // (ladder==6) && (module==-2) && (col==364) && (row==1);
	 if(adc<1) hadc02bx->Fill(float(bx));
	 if(electrons<0) {
	   hnadc2->Fill(float(adc));
	   helec02bx->Fill(float(bx));
	 }
	 hadc2->Fill(float(adc));
	 hcols2->Fill(float(col));
	 hrows2->Fill(float(row));
	 hpixMap2->Fill(float(col),float(row));
	 hpdetMap2->Fill(float(module),float(ladder));
	 hadc2ls->Fill(float(lumiBlock),float(adc));
	 hadc2bx->Fill(float(bx),float(adc));
	 totalNumOfDigis2++;
	 numOfDigisPerDet2++;

#ifdef SINGLE_MODULES
	 //if     ( ladder== 5 && module==-3) hpixDetMap20->Fill(pixy,pixx,weight); // 
	 //if( ladder== 5 && module==-3) hpixDetMap21->Fill(pixy,pixx,1.); // special case
	 // else if( ladder== 3 && module== 3) hpixDetMap22->Fill(pixy,pixx,weight); // 
	 // else if( ladder==13 && module==-1) hpixDetMap23->Fill(pixy,pixx,weight); //
	 // else if( ladder==-3 && module==4) hpixDetMap24->Fill(pixy,pixx,weight); //
	 // else if( ladder==-4 && module==4) hpixDetMap25->Fill(pixy,pixx,weight); // 
	 // else if( ladder==-5 && module==4) hpixDetMap26->Fill(pixy,pixx,weight); // 
	 // else if( ladder==-6 && module==4) hpixDetMap27->Fill(pixy,pixx,weight); // 
	 // else if( ladder==-1 && module==-4) hpixDetMap28->Fill(pixy,pixx,weight); //
	 // else if( ladder==-2 && module==-4) hpixDetMap29->Fill(pixy,pixx,weight); // 
#endif

       } else if(layer==3) {
	 if(rescaleVcal) electrons = (vcal * 47.5 + offset)/1000.; //L3 at 51.5fb-1 47->47.5
	 else            electrons = (vcal * conversionFactor + offset)/1000.; //default 
	 float energy = electrons * 3.61; // convert electrons to keV
	 hrocMapS3->Fill(rocZ,rocPhi,energy);
	 hrocEne3->Fill(rocZ,energy);
	 helectrons3->Fill(electrons); 
	 hvcal3->Fill(vcal);
	 //noise = false; //(ladder==6) || (module==-2) || (col==364) || (row==1);	
	 if(electrons<0) hnadc3->Fill(float(adc));
	 hadc3->Fill(float(adc));
	 hcols3->Fill(float(col));
	 hrows3->Fill(float(row));
	 //if(ladder==-13 && module==-4) 
	 hpdetMap3->Fill(float(module),float(ladder));
	 hpixMap3->Fill(float(col),float(row));
	 hadc3ls->Fill(float(lumiBlock),float(adc));
	 hadc3bx->Fill(float(bx),float(adc));
	 totalNumOfDigis3++;
	 numOfDigisPerDet3++;

#ifdef SINGLE_MODULES
	 if     ( ladder== 20 && module== 2) hpixDetMap30->Fill(pixy,pixx,weight); // 
	 //if( ladder==-22 && module==-2) hpixDetMap31->Fill(pixy,pixx,1.); // special case
	 else if( ladder== 5 && module== -1) hpixDetMap31->Fill(pixy,pixx,weight); // 
	 //else if( ladder==12 && module== 4) hpixDetMap32->Fill(pixy,pixx,weight); // 
	 //else if( ladder==19 && module==-4) hpixDetMap33->Fill(pixy,pixx,weight); // 
	 //else if( ladder==14 && module==-3) hpixDetMap34->Fill(pixy,pixx,weight); // 
#endif
       } else if(layer==4) {
	 if(rescaleVcal) electrons = (vcal * 47.25 + offset)/1000.; //L4 at 51.5fb-1 47->47.25
	 else            electrons = (vcal * conversionFactor + offset)/1000.; //default 
	 helectrons4->Fill(electrons);
	 float energy = electrons * 3.61; // convert electrons to keV
	 hrocMapS4->Fill(rocZ,rocPhi,energy);
	 hrocEne4->Fill(rocZ,energy);
	 hvcal4->Fill(vcal);
	 //noise = false; //(ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(electrons<0) hnadc4->Fill(float(adc));
	 hadc4->Fill(float(adc));
	 hcols4->Fill(float(col));
	 hrows4->Fill(float(row));
	 hpdetMap4->Fill(float(module),float(ladder));
	 hpixMap4->Fill(float(col),float(row));
	 hadc4ls->Fill(float(lumiBlock),float(adc));
	 hadc4bx->Fill(float(bx),float(adc));
	 totalNumOfDigis4++;
	 numOfDigisPerDet4++;

#ifdef SINGLE_MODULES
	 //if     ( ladder== 6 && module== 1) hpixDetMap40->Fill(pixy,pixx,weight); // 
	 //if( ladder== 6 && module== 1) hpixDetMap41->Fill(pixy,pixx,1.); //  special case  
	 //else if( ladder== 6 && module== 3) hpixDetMap42->Fill(pixy,pixx,weight); // 
	 //else if( ladder== 6 && module== 4) hpixDetMap43->Fill(pixy,pixx,weight); // 
	 //else if( ladder==-27 && module==1) hpixDetMap44->Fill(pixy,pixx,weight); // 
	 //else if( ladder== 3 && module==-4) hpixDetMap45->Fill(pixy,pixx,weight); // 
#endif
       } else if(disk==1) {
	 electrons = (vcal * conversionFactor + offset)/1000.; 
	 helectronsf->Fill(electrons);
	 //noise = false; //(ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   hadc0ls->Fill(float(lumiBlock),float(adc));
	   hadc0bx->Fill(float(bx),float(adc));
	   if(ring==1)      {
	     hpixMapFPix11->Fill(float(col),float(row));
	     hadcF11->Fill(float(adc));
	     hcolsF11->Fill(float(col));
	     hrowsF11->Fill(float(row));
	     hblade11->Fill(float(blade));
	   } else if(ring==2) {
	     hpixMapFPix12->Fill(float(col),float(row));
	     hadcF12->Fill(float(adc));
	     hcolsF12->Fill(float(col));
	     hrowsF12->Fill(float(row));
	     hblade12->Fill(float(blade));
	   } else cout<<" wrong ring "<<ring<<endl;
	   totalNumOfDigisF1++;
	   numOfDigisPerDetF1++;
	 } // noise 

       } else if(disk==2) {
	 electrons = (vcal * conversionFactor + offset)/1000.; 
	 helectronsf->Fill(electrons);
	 // noise = (ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   hadc0ls->Fill(float(lumiBlock),float(adc));
	   hadc0bx->Fill(float(bx),float(adc));
	   if(ring==1)      {
	     hpixMapFPix21->Fill(float(col),float(row));
	     hadcF21->Fill(float(adc));
	     hcolsF21->Fill(float(col));
	     hrowsF21->Fill(float(row));
	     hblade21->Fill(float(blade));
	   } else if(ring==2) {
	     hpixMapFPix22->Fill(float(col),float(row));
	     hadcF22->Fill(float(adc));
	     hcolsF22->Fill(float(col));
	     hrowsF22->Fill(float(row));
	     hblade22->Fill(float(blade));
	   } else cout<<" wrong ring "<<ring<<endl;
	   totalNumOfDigisF2++;
	   numOfDigisPerDetF2++;
	 } // noise 
       } else if(disk==3) {
	electrons = (vcal * conversionFactor + offset)/1000.; 
	helectronsf->Fill(electrons);
	 // noise = (ladder==6) || (module==-2) || (col==364) || (row==1);
	 if(!noise) {		     
	   hadc0ls->Fill(float(lumiBlock),float(adc));
	   hadc0bx->Fill(float(bx),float(adc));
	   if(ring==1) {
	     hpixMapFPix31->Fill(float(col),float(row));
	     hadcF31->Fill(float(adc));
	     hcolsF31->Fill(float(col));
	     hrowsF31->Fill(float(row));
	     hblade31->Fill(float(blade));
	   } else if(ring==2) {
	     hpixMapFPix32->Fill(float(col),float(row));
	     hadcF32->Fill(float(adc));
	     hcolsF32->Fill(float(col));
	     hrowsF32->Fill(float(row));
	     hblade32->Fill(float(blade));
	   } else cout<<" wrong ring "<<ring<<endl;
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
	    //hblade1->Fill(float(blade));
	    ++numberOfDetUnitsF1;
	    if(ring==1) hdigisPerDetF11->Fill(float(numOfDigisPerDetF1));
	    else        hdigisPerDetF12->Fill(float(numOfDigisPerDetF1));
	    numOfDigisPerDetF1=0;	
	    if(detZ<0.) hxy11->Fill(detX,detY);
	    else        hxy12->Fill(detX,detY);
	  } else if(disk==2) {
	    ++numberOfDetUnitsF2;
	    //hdigisPerDetF2->Fill(float(numOfDigisPerDetF2));
	    if(ring==1) hdigisPerDetF21->Fill(float(numOfDigisPerDetF2));
	    else        hdigisPerDetF22->Fill(float(numOfDigisPerDetF2));
	    numOfDigisPerDetF2=0;
	    if(detZ<0.) hxy21->Fill(detX,detY);
	    else        hxy22->Fill(detX,detY);

	  } else if(disk==3) {
	    ++numberOfDetUnitsF3;
	    //hdigisPerDetF3->Fill(float(numOfDigisPerDetF3));
	    if(ring==1) hdigisPerDetF31->Fill(float(numOfDigisPerDetF3));
	    else        hdigisPerDetF32->Fill(float(numOfDigisPerDetF3));
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
	    ++numberOfDetUnits1;
	    hdigisPerDet1->Fill(float(numOfDigisPerDet1));
	    numOfDigisPerDet1=0;
	    hphiz1->Fill(detZ,detPhi);
	    
	  } else if(layer==2 && !badL2Modules) {
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

	  // DCOL code 
	  if(layer==1) {
	    const int dcolThr = 100;
	    int countDcols=0;  // hit dcols in a module 
	    int countFullDcolsInModule=0,
	      countFullDcolsInROC1=0,countFullDcolsInROC2=0;
	    // Analyse this module 
	    for(int col=0;col<416;col+=2) {
	      //cout<<col<<endl;
	      int dcolCount=0;  // hits in a dcol



	      // ROC 
	      if( (col>0)  && (col%52 == 0) ) { // 1 roc finished 
		hfullDcolsPerROC->Fill(float(countFullDcolsInROC1));
		hfullDcolsPerROC->Fill(float(countFullDcolsInROC2));
		countFullDcolsInROC1=0;
		countFullDcolsInROC2=0;
	      }

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
	      if(dcolCount>0) {
		countDcols++;
		hhitsPerHitDcol->Fill(float(dcolCount));
		hhitsPerHitDcolModule->Fill(float(module),float(dcolCount));
		hhitsPerHitDcolLadder->Fill(float(ladder),float(dcolCount));
	      }

	      if(dcolCount>dcolThr) {
		//cout<<" full dcol (lower roc). count "<<dcolCount<<" col "
		//  <<col<<" mod "<<module<<" ladder "<<ladder<<endl;

		int roc = rocId(col,0);  // 0-15, column, row
		int link = int(roc/8); // link 0 & 1
		int rocInCol = roc%8; // 0-7
		float rocZ=0;
		if(module>0) rocZ = float(module) + (0.125/2.) - (float(rocInCol) * 0.125); //z
		else         rocZ = float(module) + 1.0 + (0.125/2.) - (float(rocInCol) * 0.125); //z
		float rocPhi = float(ladder) - 0.5 + (0.5/2.)   + (float(link) * 0.5); 
		hrocMap1_2->Fill(rocZ,rocPhi);

		countFullDcols++;
		countFullDcolsInModule++;
		countFullDcolsInROC1++;
		countFullDcolsInEvent++;
		hpdetMaps1_2->Fill(float(module),float(ladder));
		hcols1Many->Fill(float(col));
		hdcolFull1bx->Fill(float(bx));
		hdcolFull1ls->Fill(float(lumiBlock));
		
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
	      if(dcolCount>0) {
		countDcols++;
		hhitsPerHitDcol->Fill(float(dcolCount));
		hhitsPerHitDcolModule->Fill(float(module),float(dcolCount));
		hhitsPerHitDcolLadder->Fill(float(ladder),float(dcolCount));
	      }

	      if(dcolCount>dcolThr) {
		//cout<<" full dcol (upper roc). count "<<dcolCount<<" col "
		//  <<col<<" mod "<<module<<" ladder "<<ladder<<endl;
		int roc = rocId(col,80);  // 0-15, column, row
		int link = int(roc/8); // link 0 & 1
		int rocInCol = roc%8; // 0-7
	        float rocZ=0;
		if(module>0) rocZ = float(module) + (0.125/2.) - (float(rocInCol) * 0.125); //z
		else         rocZ = float(module) + 1.0 + (0.125/2.) - (float(rocInCol) * 0.125); //z
		float rocPhi = float(ladder) - 0.5 + (0.5/2.)   + (float(link) * 0.5); 
		hrocMap1_2->Fill(rocZ,rocPhi);

		countFullDcols++;
		countFullDcolsInModule++;
		countFullDcolsInROC2++;
		countFullDcolsInEvent++;
		hpdetMaps1_2->Fill(float(module),float(ladder));
		//hrocMap1_2->Fill(rocZ,rocPhi);
		hcols1Many->Fill(float(col));
		hdcolFull1bx->Fill(float(bx));
		hdcolFull1ls->Fill(float(lumiBlock));
		
		// for(int row=80;row<160;++row) 
		//   if(oneModule[col][row]>0)   
		//     {cout<<col<<" "<<row<<" "<<oneModule[col][row]<<endl;}
		// for(int row=80;row<160;++row) 
		//   if(oneModule[col+1][row]>0) 
		//     {cout<<(col+1)<<" "<<row<<" "<<oneModule[col+1][row]<<endl;}
		
	      }
	    } // loop over cols

	    hfullDcolsPerROC->Fill(float(countFullDcolsInROC1));
	    hfullDcolsPerROC->Fill(float(countFullDcolsInROC2));
	    hhitDcolsPerModule->Fill(float(countDcols));
	    hDcolHitProbabilityModule->Fill(float(module),float(countDcols)/416.);
	    hDcolHitProbabilityLadder->Fill(float(ladder),float(countDcols)/416.);
	    hfullDcolsPerModule->Fill(float(countFullDcolsInModule));

	  } // if layer 1
	  // End DCOL code 


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

  hfullDcolsPerEvent->Fill(float(countFullDcolsInEvent));

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
	  // average number of hits per dcol per event 
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
	<<" rocs with empty cols "<<rocsWithEmptyCols<<endl;
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
  hpdetMaps1_1->Scale(norm);
  hpdetMaps1_2->Scale(norm);
  hpdetMaps1_3->Scale(norm);
  //hpdetMaps1_4->Scale(norm);

  hpixMapFPix11->Scale(norm);
  hpixMapFPix12->Scale(norm);
  hpixMapFPix21->Scale(norm);
  hpixMapFPix22->Scale(norm);
  hpixMapFPix31->Scale(norm);
  hpixMapFPix32->Scale(norm);

  float normPixel = norm/4096.;
  hrocMapS1->Scale(normPixel);
  hrocMapS2->Scale(normPixel);
  hrocMapS3->Scale(normPixel);
  hrocMapS4->Scale(normPixel);
  hrocEne1->Scale(normPixel/(12.*2.));
  hrocEne2->Scale(normPixel/(28.*2.));
  hrocEne3->Scale(normPixel/(44.*2.));
  hrocEne4->Scale(normPixel/(64.*2.));

#ifdef DCOLS
  hAllDcols->Scale(norm);
#endif

}

//define this as a plug-in
DEFINE_FWK_MODULE(PixDigisTest);
