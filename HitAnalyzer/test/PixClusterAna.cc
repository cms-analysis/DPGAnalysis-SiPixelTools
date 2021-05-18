// File: PixClusterAna.cc
//eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase0/ Description: To test pixel clusters. 
// Author: Danek Kotlinski 
// Creation Date:  Initial version. 3/06
// Modify to work with CMSSW354, 11/03/10 d.k.
// Modify to work with CMSSW620, SLC6, CMSSW700, 10/10/13 d.k.
// Change to ByToken.
// Make for 4 layers. 1/17 
// Add Normalise configurable. 1/18
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

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
//#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
//#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
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

// for resyncs 
#include "DataFormats/Scalers/interface/Level1TriggerScalers.h"

// For luminisoty
#include "DataFormats/Luminosity/interface/LumiSummary.h"
#include "DataFormats/Luminosity/interface/LumiDetails.h"
#include "DataFormats/Common/interface/ConditionsInEdm.h"
#include "RecoLuminosity/LumiProducer/interface/LumiCorrector.h"

// For PVs
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


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
#include <TVector3.h>

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

using namespace std;

#define HISTOS
//#define L1
//#define HLT
#define PV
#define BX_TESTS
//#define STUDY_LAY1
#define SINGLE_MODULES
//#define PHI_PROFILES
//#define TEST_GEOM
//#define TEST_DCOLS

//#define HI
//#define ROC_EFF
//#define Lumi
//#define USE_RESYNCS  // get theinfo about recyncs 

// special hitos with parameters normalized to intlumi (usually disabled)
#define LS_TESTS
//#define VDM_STUDIES
//#define BX_NEW
//#define HF

//#define TESTING_ADC
//#define LAY1_SPLIT
#define ROC_RATE
//#define STUDY_ONEMOD

#ifdef HF
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#endif // HF


//=======================================================================
#ifdef BX_NEW

class getBX {

public:
  getBX(void);
  ~getBX(void);
  static int find(int bx);

private:

};

getBX::getBX(void) {
  cout<<" bx ctor "<<endl;
}

getBX::~getBX(void) {
}

int getBX::find(int bx) {
  // invalid -1, empty 0, beam1 1, beam2 2, collision 3, collison+1 4, beam1+1 5,  beam2+1 6 

  const int limit=3394;

  // For the usuall 1375-1368 50ns fill
  const int coll_num = 1368;
  const int coll[1368] = {
 66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114
 //.........................
  };
  const int beam1_num=6;
  const int beam1[6] = {
  1,3,5,7,9,11
  };
  const int beam2_num=6;
  const int beam2[6] = {
  13,15,17,19,21,23,
  };


  if(bx>limit) return (-1);
  //cout<<bx<<endl;

  // Check collisions
  for(int i=0;i<coll_num;++i) {    
    if(bx == coll[i]) return (3);  // collision
    else if(bx == (coll[i]+1)) return (4); // collision+1
  }
    
  // Check beam1
  for(int i=0;i<beam1_num;++i) {    
    if(bx == beam1[i]) return (1);  // collision
    else if(bx == (beam1[i]+1)) return (5); // collision+1
  }
    
  // Check beam1
  for(int i=0;i<beam2_num;++i) {    
    if(bx == beam2[i]) return (2);  // collision
    else if(bx == (beam2[i]+1)) return (6); // collision+1
  }

  return 0; // return no collision 
}

#endif  // BX_NEW

//=======================================================================

// decode a simplified ROC address
 int rocId(int col, int row) {
   int rocRow = row/80;
   int rocCol = col/52;
   int rocId = rocCol + rocRow*8;
   return rocId;
 }

//=========================================================================
#ifdef ROC_EFF

class rocEfficiency {
public:
  rocEfficiency(void);
  ~rocEfficiency(void);
  void addPixel(int layer, int ladder, int module, int roc);
  float getRoc(int layer, int ladder, int module, int roc);
  float getModule(int layer, int ladder, int module, float & half1, float & half2);
  //int analyzeModule(int layer, int ladder, int module);

  // return array index
  inline int transformLadder1(int ladder) {
    if(ladder>10 || ladder<-10 || ladder==0) {
      cout<<" wrong ladder-1 id "<<ladder<<cout;
      return (-1);
    }
    if(ladder<0) return (ladder+10);
    else return (ladder+10-1);
  };
  inline int transformLadder2(int ladder) {
    if(ladder>16 || ladder<-16 || ladder==0) {
      cout<<" wrong ladder-2 id "<<ladder<<cout;
      return (-1);
    }
    if(ladder<0) return (ladder+16);
    else return (ladder+16-1);
  };
  inline int transformLadder3(int ladder) {
    if(ladder>22 || ladder<-22 || ladder==0) {
      cout<<" wrong ladder-3 id "<<ladder<<cout;
      return (-1);
    }
    if(ladder<0) return (ladder+22);
    else return (ladder+22-1);
  };
  inline int transformModule(int module) {
    if(module>4 || module<-4 || module==0) {
      cout<<" wrong module id "<<module<<cout;
      return (-1);
    }
    if(module<0) return (module+4);
    else return (module+4-1);
  };


private:
  int pixelsLayer1[20][8][16];
  int pixelsLayer2[32][8][16];
  int pixelsLayer3[44][8][16];
};

rocEfficiency::rocEfficiency(void) {

  cout<<" clear"<<endl;
  for(int lad1=0;lad1<20;++lad1)
    for(int mod1=0;mod1<8;++mod1)
      for(int roc1=0;roc1<16;++roc1)
        pixelsLayer1[lad1][mod1][roc1]=0;

  for(int lad2=0;lad2<32;++lad2)
    for(int mod2=0;mod2<8;++mod2)
      for(int roc2=0;roc2<16;++roc2)
        pixelsLayer2[lad2][mod2][roc2]=0;

  for(int lad3=0;lad3<44;++lad3)
    for(int mod3=0;mod3<8;++mod3)
      for(int roc3=0;roc3<16;++roc3)
        pixelsLayer3[lad3][mod3][roc3]=0;

}

rocEfficiency::~rocEfficiency(void) {
//   cout<<" clear"<<endl;
//   for(int lad1=0;lad1<20;++lad1)
//     for(int mod1=0;mod1<8;++mod1)
//       for(int col1=0;col1<416;++col1)
//      for(int row1=0;row1<160;++row1)
//        if(pixelsLayer1[lad1][mod1][col1][row1]>0)
//          cout<<lad1<<" "<<mod1<<" "<<col1<<" "<<row1<<" "
//              <<pixelsLayer1[lad1][mod1][col1][row1] <<endl;
}

void rocEfficiency::addPixel(int layer, int ladder, int module, int roc) {

  if(roc<0 || roc>=16) {
    cout<<" wrong roc number "<<roc<<endl;
    return;
  }

  int module0 = transformModule(module);
  if(module0<0 ||module0>=8) {
    cout<<" wrong module index "<<module0<<" "<<module<<endl;
    return;
  }

  int ladder0=0;
  switch(layer) {
  case 1:
    ladder0 = transformLadder1(ladder);
    //cout<<layer<<" "<<ladder0<<" "<<module0<<" "<<col<<" "<<row<<endl;
    if(ladder0<0 || ladder0>=20)
      cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
    else
      pixelsLayer1[ladder0][module0][roc]++;
    break;
  case 2:
    ladder0 = transformLadder2(ladder);
    if(ladder0<0 || ladder0>=32)
      cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
    else
      pixelsLayer2[ladder0][module0][roc]++;
    break;
  case 3:
    ladder0 = transformLadder3(ladder);
    if(ladder0<0 || ladder0>=44)
      cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
    else
      pixelsLayer3[ladder0][module0][roc]++;
    break;
  default:
    break;
  }
}

float rocEfficiency::getRoc(int layer, int ladder, int module, int roc) {

  if(roc<0 || roc>=16) {
    cout<<" wrong roc number "<<roc<<endl;
    return -1.;
  }
  int module0 = transformModule(module);
  if(module0<0 ||module0>=8) {
    cout<<" wrong module index "<<module0<<" "<<module<<endl;
    return -1.;
  }

  float count=0.;
  int ladder0=0;

  switch(layer) {
  case 1:
    if( (abs(ladder)==1 || abs(ladder)==10) && roc>7 ) { // find half modules
      count = -1.;  // invalid
    } else {
      ladder0 = transformLadder1(ladder);
      if(ladder0<0 || ladder0>=20) {
        cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
        count=-1.;
      } else
        count = float(pixelsLayer1[ladder0][module0][roc]);
      //if(ladder0==10 && module0==5 && col==9 && row==0) cout<<count<<endl;
    }
    break;
  case 2:
    if( (abs(ladder)==1 || abs(ladder)==16) && roc>7 ) { // find half modules
      count = -1.;  // invalid
    } else {
      ladder0 = transformLadder2(ladder);
      if(ladder0<0 || ladder0>=32) {
        cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
        count=-1.;
      } else
        count = float(pixelsLayer2[ladder0][module0][roc]);
    }
    break;
  case 3:
    if( (abs(ladder)==1 || abs(ladder)==22) && roc>7 ) { // find half modules
      count = -1;  // invalid
    } else {
      ladder0 = transformLadder3(ladder);
      if(ladder0<0 || ladder0>=44) {
        cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
        count=-1.;
      } else
        count = float(pixelsLayer3[ladder0][module0][roc]);
    }
    break;
  default:
    break;
  }
  return count;
}

// Return counts averaged per ROC (count per module / number of full ROCs ) 
float rocEfficiency::getModule(int layer, int ladder, int module, float & half1, float & half2 ) {
  half1=0; half2=0;
  int module0 = transformModule(module);
  if(module0<0 ||module0>=8) {
    cout<<" wrong module index "<<module0<<" "<<module<<endl;
    return -1.;
  }

  float count=0;
  int rocs=0;
  int ladder0=0;

  switch(layer) {
  case 1:
    ladder0 = transformLadder1(ladder);
    if(ladder0<0 || ladder0>=20) {
      cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
      return -1.;
    }
    for(int roc=0;roc<16;++roc) {
      //if( !((abs(ladder)==1 || abs(ladder)==10) && roc>7) ) // treat half-modules
      float tmp = float(pixelsLayer1[ladder0][module0][roc]);
      count += tmp;
      if(roc<8) half1 += tmp; 
      else      half2 += tmp;
      if(tmp>0) rocs++;
    }
    break;

  case 2:
    ladder0 = transformLadder2(ladder);
    if(ladder0<0 || ladder0>=32) {
      cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
      return -1.;
    }
    for(int roc=0;roc<16;++roc) {
      //if( !((abs(ladder)==1 || abs(ladder)==22) && roc>7) ) // treat half-modules
      float tmp = float(pixelsLayer2[ladder0][module0][roc]);
      count += tmp;
      if(roc<8) half1 += tmp; 
      else      half2 += tmp;
      if(tmp>0) rocs++;

    }
    break;

  case 3:
    ladder0 = transformLadder3(ladder);
    if(ladder0<0 || ladder0>=44) {
      cout<<" wrong ladder index "<<ladder0<<" "<<ladder<<endl;
      return -1.;
    }
    for(int roc=0;roc<16;++roc) {
      //if( !((abs(ladder)==1 || abs(ladder)==22) && roc>7) ) // treat half-modules
      float tmp = float(pixelsLayer3[ladder0][module0][roc]);
      count += tmp;
      if(roc<8) half1 += tmp; 
      else      half2 += tmp;
      if(tmp>0) rocs++;

    }
    break;

  default:
    break;
  }
  //cout<<count<<" "<<rocs<<" "<<ladder<<" "<<module<<" "<<ladder0<<" "<<module0<<endl;
  if(rocs>0) count = count/float(rocs);  // return counts per ROC (full rocs only)
  if(count<0) cout<<" VERY VERY WRONG "<<count<<endl;
  return count;
}

#endif

//==========================================================================================

class PixClusterAna : public edm::EDAnalyzer {
 public:
  
  explicit PixClusterAna(const edm::ParameterSet& conf);  
  virtual ~PixClusterAna();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  virtual void beginRun(const edm::EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
#ifdef TEST_DCOLS
  void histogramDcols(int layer, int ladder, int ring);
#endif
  
 private:
  edm::ParameterSet conf_;
  edm::InputTag src_;
  //const static bool PRINT = false;
  bool PRINT;
  bool Normalise;
  int select1, select2;
  int countEvents, countAllEvents;
  double sumClusters, sumPixels, countLumi;
  //float rocHits[3][10]; // layer, ids, store hits in 30 rocs
  //float moduleHits[3][10]; // layer, ids, store hits in 30 modules
  bool phase1_;
  int selectLayer, selectLadder, selectModule;
  bool useTracks;
#ifdef SINGLE_MODULES
  bool eventFlag[40];
#endif
#ifdef TEST_DCOLS
  int moduleArray[160][416];
#endif
  // Needed for the ByToken method
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > myClus;
  edm::EDGetTokenT<LumiSummary> LumiToken;
  edm::EDGetTokenT<edm::ConditionsInLumiBlock> ConditionsInLumiBlockToken;
  edm::EDGetTokenT<L1GlobalTriggerReadoutRecord> L1TrigReadoutToken;
  edm::EDGetTokenT<edm::TriggerResults> TrigResultsToken;
  edm::EDGetTokenT<reco::VertexCollection> VertexCollectionToken;
  edm::EDGetTokenT<std::vector<reco::Track>> TrackToken;

#ifdef HF
edm::EDGetTokenT<HFRecHitCollection> HFHitsToken_;
#endif //HF

  //TFile* hFile;
  //TH1D *hdetunit;
  //TH1D *hpixid,*hpixsubid;
  TH1D *hlayerid;
  TH1D *hladder1id,*hladder2id,*hladder3id,*hladder4id,
    *hz1id,*hz2id,*hz3id,*hz4id;
  TH1D *hpladder1id,*hpladder2id,*hpladder3id,*hpladder4id,
    *hpz1id,*hpz2id,*hpz3id,*hpz4id;

  TH1D *hcols1,*hcols2,*hcols3,*hrows1,*hrows2,*hrows3,*hcols4,*hrows4;
  TH1D *hpcols1,*hpcols2,*hpcols3,*hprows1,*hprows2,*hprows3,*hpcols4,*hprows4;

  TH1D *hcharge1,*hcharge2, *hcharge3, *hcharge4, 
    *hcharge5,*hcharge6,*hcharge7,*hcharge8 ;
  TH1D *hpixcharge1,*hpixcharge2, *hpixcharge3, *hpixcharge4, 
    *hpixcharge5,*hpixcharge6,*hpixcharge7,*hpixcharge8;
  TH1D *hsize1,*hsize2,*hsize3,*hsize4,
    *hsizex1,*hsizex2,*hsizex3,*hsizex4,
    *hsizey1,*hsizey2,*hsizey3,*hsizey4;
  // new L1 modules 
  TH1D *hcharge1n, *hpixcharge1n, *hsize1n, *hsizex1n, *hsizey1n; 
  TH1D *hcharge2g, *hcharge2b, *hpixcharge2g,*hpixcharge2b, *hsize2g, *hsize2b;

  TH1D *hcharge11, *hcharge12, *hpixcharge11, *hpixcharge12;
  TH1D *hcharge1_zoom, *hpixcharge1_zoom;

  TH1D *hcols11,*hcols12,*hrows11; 
  TH1D *hcols21,*hrows21; 

  TH1D *hclusPerDet1,*hclusPerDet2,*hclusPerDet3,*hclusPerDet4;
  TH1D *hpixPerDet1,*hpixPerDet2,*hpixPerDet3,*hpixPerDet4;
  //TH1D *hpixPerLink1,*hpixPerLink2,*hpixPerLink3,*hpixPerLink4;
  TH1D *hclusPerLay1,*hclusPerLay2,*hclusPerLay3,*hclusPerLay4;
  TH1D *hpixPerLay1,*hpixPerLay2,*hpixPerLay3,*hpixPerLay4;
  TH1D *hdetsPerLay1,*hdetsPerLay2,*hdetsPerLay3,*hdetsPerLay4;
  TH1D *hclus, *hclusBPix, *hclusFPix, *hdigis, *hdigis2, *hdigisB, *hdigisF;
  TH1D *hclus2; // ,*hclus5; // *hclus6,*hclus7,*hclus8,*hclus9;
  TH1D *hdets, *hdets2;

  TH1D *hpixDiskR1,*hpixDiskR2,*hpixDiskR3; // *hdetr, *hdetz;

  TH2F *hDets1, *hDets2, *hDets3,*hDets4;    // all modules
  TH2F *hcluDets1,  *hcluDets2,  *hcluDets3, *hcluDets4;     // all modules
  TH2F *hpixDets1, *hpixDets2, *hpixDets3,*hpixDets4;    // all modules
  TH2F *hsizeDets1, *hsizeDets2, *hsizeDets3, *hsizeDets4;

  TProfile2D *hchargeDets1, *hchargeDets2, *hchargeDets3, *hchargeDets4;
  TProfile2D *hpixchargeDets1, *hpixchargeDets2, *hpixchargeDets3, *hpixchargeDets4;

  TH2F *hpixDetMap1, *hpixDetMap2, *hpixDetMap3, *hpixDetMap4;  //in a  modules
  TH2F *hcluDetMap1, *hcluDetMap2, *hcluDetMap3, *hcluDetMap4;  //in a  modules 

#ifdef TESTING_ADC
  TH2F *hpixDetsT1, *hpixDetsT2, *hpixDetsT3,*hpixDetsT4;    // all modules
  TH2F *hpixDetMapT1, *hpixDetMapT2, *hpixDetMapT3, *hpixDetMapT4;  //in a  modules

  TProfile2D *hsizeDetT1, *hsizeDetT2, *hsizeDetT3, *hsizeDetT4;
  TProfile2D *hcluDetT1, *hcluDetT2, *hcluDetT3, *hcluDetT4;     
  TH1D *hchargeT1, *hsizeT1 , *hchargeT12_1 , *hchargeT12_2; 

  TH1D *hclusPerDetT1, *hpixPerDetT1;
#endif

  TH1D *hevent, *hlumi, *hlumi0,*hlumi1; //*hlumi10,*hlumi11, *hlumi12, *hlumi13, *hlumi14;
  TH1D *hbx, *hbx0;
  TH1D *hmaxPixPerDet;

  TH1D *hclusPerDisk1,*hclusPerDisk2,*hclusPerDisk3,*hclusPerDisk4,
    *hclusPerDisk5,*hclusPerDisk6;
  TH1D *hpixPerDisk1,*hpixPerDisk2,*hpixPerDisk3,*hpixPerDisk4,
    *hpixPerDisk5,*hpixPerDisk6;
  //
  TH1D *hpixPerDet11,*hpixPerDet12,*hpixPerDet13,*hpixPerDet14;
  TH1D *hpixPerDet21,*hpixPerDet22,*hpixPerDet23,*hpixPerDet24;
  TH1D *hpixPerDet31,*hpixPerDet32,*hpixPerDet33,*hpixPerDet34;
  TH1D *hpixPerDet41,*hpixPerDet42,*hpixPerDet43,*hpixPerDet44;

  TH1D *hgz1,*hgz2,*hgz3, *hgz4; 

  TH2F *hsizeyz1,*hsizeyz2,*hsizeyz3,*hsizeyz4 ;
  TH2F *htest1,*htest2,*htest3,*htest4;

  // Profiles 
  TProfile *hclumult1,  *hclumult2,  *hclumult3,  *hclumult4;
  TProfile *hclumultx1, *hclumultx2, *hclumultx3, *hclumultx4;
  TProfile *hclumulty1, *hclumulty2, *hclumulty3, *hclumulty4;
  TProfile *hcluchar1,  *hcluchar2,  *hcluchar3,  *hcluchar4;
  TProfile *hpixchar1,  *hpixchar2,  *hpixchar3,  *hpixchar4;

#ifdef PHI_PROFILES
  TProfile *hclumultxPhi1, *hclumultxPhi2; // , *hclumultx3, *hclumultx4;
  TProfile *hclumultyPhi1, *hclumultyPhi2; // *hclumulty3, *hclumulty4;
  TProfile *hclucharPhi1, *hclucharPhi2; //  *hcluchar3,  *hcluchar4;
  TProfile *hpixcharPhi1, *hpixcharPhi2; //  *hpixchar3,  *hpixchar4;
#endif

  TProfile *hclusls,  *hpixls;

#ifdef PV
  TH1D *hpvs, *htracks;
  TProfile *hpvls, *hclupv, *hpixpv,*hpvbx;
#endif

  //TProfile *hcharCluls, *hcharPixls, *hsizeCluls, *hsizeXCluls;
  //TProfile *hpixbx, *hclubx, *hpvbx, *hcharClubx, *hcharPixbx,*hsizeClubx, *hsizeYClubx;
#ifdef MINMAX
  TH1D *hcharMin1,*hcharMin2,*hcharMin3,*hcharMin4,*hcharMax1,*hcharMax2,*hcharMax3,*hcharMax4;
#endif

  TH1D *hpixz1,*hpixz2,*hpixz3,*hpixz4; 

#ifdef TEST_GEOM
  TH2F *hbpixXY;
#endif

#ifdef Lumi
  TH1D *hinst;
  TProfile *hinstls, *hinstlsbx;
  TProfile *hinstbx;
#endif
		  
#ifdef L1
  TH1D *hl1a, *hl1t, *hltt; 
  TH1D *hl1a1, *hl1t1;
#endif
#ifdef HLT
  TH1D *hlt1, *hlt2, *hlt3;
#endif
 
#ifdef TEST_DCOLS 		 
  TProfile *hdcolHitProb1,*hdcolHitProb2,*hdcolHitProb3,*hdcolHitProb4; 
  TProfile *hdcolHitMult1,*hdcolHitMult2,*hdcolHitMult3,*hdcolHitMult4; 
  TProfile *hmoduleHitMult1,*hmoduleHitMult2,*hmoduleHitMult3,*hmoduleHitMult4; 
  TProfile *hrocHitMult1,*hrocHitMult2,*hrocHitMult3,*hrocHitMult4; 
  TProfile *hdcolHitProbLadder1, *hdcolHitMultLadder1;
#endif

#if defined(BX) || defined(BX_NEW)
		  TH1D *hbx1, *hbx2,*hbx3,*hbx4,*hbx5,*hbx6,*hbx7;
		  TH1D *hchargebx1,*hchargebx2,*hchargebx3,*hchargebx4,*hchargebx5,*hchargebx6;
  TH1D *hpixChargebx1,*hpixChargebx2,*hpixChargebx3,*hpixChargebx4,*hpixChargebx5,*hpixChargebx6;
  TH1D *hsizebx1,*hsizebx2,*hsizebx3,*hsizebx4,*hsizebx5,*hsizebx6;
#endif

#ifdef SINGLE_MODULES
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

#ifdef USE_RESYNCS
  TH1D *htest1,*htest2,*htest3,*htest4,*htest5,*htest6 ;
#endif


#ifdef STUDY_LAY1
  TH1D *hcharge111,*hcharge112,*hcharge122,*hcharge121,*hcharge12_1,*hcharge12_2;
  TH1D *hpixcharge111,*hpixcharge112,*hpixcharge122,*hpixcharge121,*hpixcharge12_1,*hpixcharge12_2;
#endif

#ifdef ROC_RATE
  TH2F *hrocMap1,*hrocMap2,*hrocMap3,*hrocMap4;
  TProfile2D *hrocSizeXMap1, *hrocSizeYMap1,*hrocClucharMap1,*hrocPixcharMap1;
  TProfile2D *hrocSizeXMap2, *hrocSizeYMap2,*hrocClucharMap2,*hrocPixcharMap2;
  TProfile2D *hrocSizeXMap3, *hrocSizeYMap3,*hrocClucharMap3,*hrocPixcharMap3;
  TProfile2D *hrocSizeXMap4, *hrocSizeYMap4,*hrocClucharMap4,*hrocPixcharMap4;

  TH1D *hrocLadder1, *hrocZ11, *hrocZ12;
  TH1D *hpixRocRate1,*hpixRocRate2,*hpixRocRate3,*hpixRocRate4, 
    *hcluRocRate1,*hcluRocRate2,*hcluRocRate3,*hcluRocRate4;

#endif

#ifdef LS_TESTS
  TProfile *hcharClu1ls, *hcharPix1ls, *hsizeClu1ls;
  TProfile *hcharClu2ls, *hcharPix2ls, *hsizeClu2ls;
  TProfile *hcharClu3ls, *hcharPix3ls, *hsizeClu3ls;
  TProfile *hcharClu4ls, *hcharPix4ls, *hsizeClu4ls;
  TProfile *hpixb4l,*hpixb1l,*hpixb2l,*hpixb3l,*hpixfl,*hpixfml,*hpixfpl; 
  TProfile *hclusb4l,*hclusb1l,*hclusb2l,*hclusb3l,*hclusfl,*hclusfml,*hclusfpl;
#endif

#ifdef BX_TESTS
  TProfile *hpix1bx, *hclu1bx, *hcharClu1bx, *hcharPix1bx,*hsizeClu1bx;
  TProfile *hpix2bx, *hclu2bx, *hcharClu2bx, *hcharPix2bx,*hsizeClu2bx;
  TProfile *hpix3bx, *hclu3bx, *hcharClu3bx, *hcharPix3bx,*hsizeClu3bx;
  TProfile *hpix4bx, *hclu4bx, *hcharClu4bx, *hcharPix4bx,*hsizeClu4bx;
#endif

#ifdef VDM_STUDIES
  TProfile *hcharCluls1, *hcharPixls1, *hsizeCluls1, *hsizeXCluls1;
  TProfile *hcharCluls2, *hcharPixls2, *hsizeCluls2, *hsizeXCluls2;
  TProfile *hcharCluls3, *hcharPixls3, *hsizeCluls3, *hsizeXCluls3;
  TProfile *hclusls1, *hpixls1,*hclusls2, *hpixls2, *hclusls3, *hpixls3 ;
#endif

#ifdef HF
  TH2F *hclusbhf,*hclusfhf,*hclusb1hf, *hbxhf;   
  TH1D *hhf;
  TProfile *hbxhfp;
#endif //HF

#ifdef ROC_EFF
  rocEfficiency * pixEff;
  TH2F *hbadMap1, *hbadMap2, *hbadMap3;    // modules with bad rocs
  TH2F *hrocHits1ls,*hrocHits2ls,*hrocHits3ls,*hmoduleHits1ls,*hmoduleHits2ls,*hmoduleHits3ls;
  TH1D *hcountInRoc1,*hcountInRoc2,*hcountInRoc3,*hcountInRoc12,*hcountInRoc22,*hcountInRoc32;
#endif

#ifdef LAY1_SPLIT
  TH1D *hcharge1InR12,*hcharge1InR34,*hcharge1OutR12,*hcharge1OutR34; 
  TH1D *hpixcharge1InR12,*hpixcharge1InR34,*hpixcharge1OutR12,*hpixcharge1OutR34; 
  TH1D *hsize1InR12,*hsize1InR34,*hsize1OutR12,*hsize1OutR34; 
  TH1D *hsizex1InR12,*hsizex1InR34,*hsizex1OutR12,*hsizex1OutR34; 
  TH1D *hsizey1InR12,*hsizey1InR34,*hsizey1OutR12,*hsizey1OutR34; 
#endif

#ifdef STUDY_ONEMOD
  TProfile2D *hsizeMap1, *hsizeXMap1, *hsizeYMap1,*hclucharMap1,*hpixcharMap1;
#endif

#ifdef BX
  getBX * getbx;
#endif

#ifdef Lumi
  // To correct lumi  
  LumiCorrector * lumiCorrector;
#endif

};

/////////////////////////////////////////////////////////////////
// Contructor, empty.
PixClusterAna::PixClusterAna(edm::ParameterSet const& conf) 
  : conf_(conf), src_(conf.getParameter<edm::InputTag>( "src" )), Normalise(true) { 

  PRINT = conf.getUntrackedParameter<bool>("Verbosity",false);
  Normalise = conf.getUntrackedParameter<bool>("Normalise",true);
  select1 = conf.getUntrackedParameter<int>("Select1",0);
  select2 = conf.getUntrackedParameter<int>("Select2",0);
  //src_ =  conf.getParameter<edm::InputTag>( "src" );
  cout<<" Construct, Normalise = "<<Normalise<<" Select "<<select1<<"/"<<select2<<endl;
  // +-LDDM L layer DD ladder M module e.g. 1065 for BmI8/1/6/1  
  if(select1==201) { //select specifix module 
    int sign  = select2/abs(select2); // negative sign for outer O ladders
    selectLayer = abs(select2)/1000; // layer
    selectLadder = (abs(select2)%1000)/10; // ladder
    selectModule = (abs(select2)%10); // module
    selectLadder *= sign;
    if(selectModule>4) selectModule = -(selectModule-4); // 8 7 6 5 - 1 2 3 4
  }

  cout<<(conf.getParameter<edm::InputTag>("Tracks")).label()<<endl;
  string tracksTag = (conf.getParameter<edm::InputTag>("Tracks")).label();

  if(tracksTag=="") useTracks = false;
  else useTracks=true;

  // For the ByToken method
  myClus = consumes<edmNew::DetSetVector<SiPixelCluster> >(conf.getParameter<edm::InputTag>( "src" ));
  LumiToken                  = consumes <LumiSummary>(edm::InputTag("lumiProducer"));
  ConditionsInLumiBlockToken = consumes <edm::ConditionsInLumiBlock> (edm::InputTag("conditionsInEdm"));
  L1TrigReadoutToken         = consumes <L1GlobalTriggerReadoutRecord>(edm::InputTag("gtDigis"));
  TrigResultsToken           = consumes <edm::TriggerResults>(edm::InputTag("TriggerResults","","HLT"));
#ifdef PV
  if(useTracks) {
    VertexCollectionToken      = consumes <reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
    TrackToken                 = consumes <std::vector<reco::Track>>(conf.getParameter<edm::InputTag>("Tracks"));
  }
#endif
  phase1_ = conf.getUntrackedParameter<bool>("phase1",false);

#ifdef HF
  //HFHits_       = iConfig.getParameter<edm::InputTag>("HFHitCollection");
  //HFHitsToken_  = consumes<HFRecHitCollection>(edm::InputTag("hltHfreco")); // or "hfreco"
  HFHitsToken_  = consumes<HFRecHitCollection>(edm::InputTag("hfreco")); // for pp "hfreco"
#endif // HF

}
// Virtual destructor needed.
PixClusterAna::~PixClusterAna() { }  
// -------------------------------------
#ifdef TEST_DCOLS
void PixClusterAna::histogramDcols(int layer, int ladder, int module) { 
  int countInModule=0, countInRoc=0, countDcols=0;

  // loop over rocs
  for(int rr=0;rr<2;++rr) {
    for(int rc=0;rc<8;++rc) {
      countInRoc=0;
      for(int dcol=0;dcol<26;++dcol) {
	bool dcolHit = false;
	int dcolMult=0;
	// col 1
	int col=(rc*52)+dcol*2;
	for(int row=(rr*80);row<((rr*80)+80);++row) {
	  if( moduleArray[row][col]>0 ) {countInModule++; countInRoc++;dcolHit=true;dcolMult++;}
	}

	// col 2
	col=(rc*52)+dcol*2+1;
	for(int row=(rr*80);row<((rr*80)+80);++row) {
	  if( moduleArray[row][col]>0 ) {countInModule++; countInRoc++;dcolHit=true;dcolMult++;}
	}

	if(dcolHit) {
	  countDcols++;
	  if(layer==1)      {
	    hdcolHitMult1->Fill(float(module),float(dcolMult));
	    hdcolHitMultLadder1->Fill(float(ladder),float(dcolMult));}
	  else if(layer==2) {hdcolHitMult2->Fill(float(module),float(dcolMult));}
	  else if(layer==3) {hdcolHitMult3->Fill(float(module),float(dcolMult));}
	  else if(layer==4) {hdcolHitMult4->Fill(float(module),float(dcolMult));}
	}
	  
      } // rocs
      if(countInRoc>0) {
	if(layer==1)      {hrocHitMult1->Fill(float(module),float(countInRoc));}
	else if(layer==2) {hrocHitMult2->Fill(float(module),float(countInRoc));}
	else if(layer==3) {hrocHitMult3->Fill(float(module),float(countInRoc));}
	else if(layer==4) {hrocHitMult4->Fill(float(module),float(countInRoc));}
      }
    }
  }

  if(countInModule>0) {
    float prob = float(countDcols)/416.;
    if(layer==1)      {
      hdcolHitProb1->Fill(float(module),prob);
      hdcolHitProbLadder1->Fill(float(ladder),prob);
      hmoduleHitMult1->Fill(float(module),float(countInModule));}
    else if(layer==2) {hdcolHitProb2->Fill(float(module),prob);
      hmoduleHitMult2->Fill(float(module),float(countInModule));}
    else if(layer==3) {hdcolHitProb3->Fill(float(module),prob);
      hmoduleHitMult3->Fill(float(module),float(countInModule));}
    else if(layer==4) {hdcolHitProb4->Fill(float(module),prob);
      hmoduleHitMult4->Fill(float(module),float(countInModule));}
  }
}  
#endif
// ------------ method called at the begining   ------------
void PixClusterAna::beginRun(const edm::EventSetup& iSetup) {
  cout << "beginRun -  PixelClusterTest " <<endl;
}

// ------------ method called at the begining   ------------
void PixClusterAna::beginJob() {
  cout << "Initialize PixClusterAna " <<endl;

  edm::Service<TFileService> fs;

  //=====================================================================

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

  hcols12 = fs->make<TH1D>( "hcols11", "Layer 1 clus per cols", 500,-0.5,499.5); // test 
  hcols11 = fs->make<TH1D>( "hcols12", "Layer 1 clus per cols", 8000,-4.,4.);
  hrows11 = fs->make<TH1D>( "hrows11", "Layer 1 clus per rows", 2000,-1.,1.);
  hcols21 = fs->make<TH1D>( "hcols21", "Layer 2 clus per cols", 8000,-4.,4.);
  hrows21 = fs->make<TH1D>( "hrows21", "Layer 2 clus per rows", 2000,-1.,1.);

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


  int sizeH=200;
  float lowH = -0.5;
  float highH = 199.5;

  hclusPerDet1 = fs->make<TH1D>( "hclusPerDet1", "Clus per det l1",
			    sizeH, lowH, highH);
  hclusPerDet2 = fs->make<TH1D>( "hclusPerDet2", "Clus per det l2",
			    sizeH, lowH, highH);
  hclusPerDet3 = fs->make<TH1D>( "hclusPerDet3", "Clus per det l3",
			    sizeH, lowH, highH);
  hclusPerDet4 = fs->make<TH1D>( "hclusPerDet4", "Clus per det l4",
			    sizeH, lowH, highH);

  sizeH=500;
#ifdef HI
  highH = 1999.5;
#else
  highH = 999.5;
#endif
  hpixPerDet1 = fs->make<TH1D>( "hpixPerDet1", "Pix per det l1",
			    sizeH, lowH, highH);
  hpixPerDet2 = fs->make<TH1D>( "hpixPerDet2", "Pix per det l2",
			    sizeH, lowH, highH);
  hpixPerDet3 = fs->make<TH1D>( "hpixPerDet3", "Pix per det l3",
			    sizeH, lowH, highH);
  hpixPerDet4 = fs->make<TH1D>( "hpixPerDet4", "Pix per det l4",
			    sizeH, lowH, highH);

  hpixPerDet11 = fs->make<TH1D>( "hpixPerDet11", "Pix per det l1 - ring 1",
			    sizeH, lowH, highH);
  hpixPerDet12 = fs->make<TH1D>( "hpixPerDet12", "Pix per det l1 - ring 2",
			    sizeH, lowH, highH);
  hpixPerDet13 = fs->make<TH1D>( "hpixPerDet13", "Pix per det l1 - ring 3",
			    sizeH, lowH, highH);
  hpixPerDet14 = fs->make<TH1D>( "hpixPerDet14", "Pix per det l1 - ring 4",
			    sizeH, lowH, highH);
  hpixPerDet21 = fs->make<TH1D>( "hpixPerDet21", "Pix per det l2 - ring 1",
			    sizeH, lowH, highH);
  hpixPerDet22 = fs->make<TH1D>( "hpixPerDet22", "Pix per det l2 - ring 2",
			    sizeH, lowH, highH);
  hpixPerDet23 = fs->make<TH1D>( "hpixPerDet23", "Pix per det l2 - ring 3",
			    sizeH, lowH, highH);
  hpixPerDet24 = fs->make<TH1D>( "hpixPerDet24", "Pix per det l2 - ring 4",
			    sizeH, lowH, highH);
  hpixPerDet31 = fs->make<TH1D>( "hpixPerDet31", "Pix per det l3 - ring 1",
			    sizeH, lowH, highH);
  hpixPerDet32 = fs->make<TH1D>( "hpixPerDet32", "Pix per det l3 - ring 2",
			    sizeH, lowH, highH);
  hpixPerDet33 = fs->make<TH1D>( "hpixPerDet33", "Pix per det l3 - ring 3",
			    sizeH, lowH, highH);
  hpixPerDet34 = fs->make<TH1D>( "hpixPerDet34", "Pix per det l3 - ring 4",
			    sizeH, lowH, highH);
  hpixPerDet41 = fs->make<TH1D>( "hpixPerDet41", "Pix per det l4 - ring 1",
			    sizeH, lowH, highH);
  hpixPerDet42 = fs->make<TH1D>( "hpixPerDet42", "Pix per det l4 - ring 2",
			    sizeH, lowH, highH);
  hpixPerDet43 = fs->make<TH1D>( "hpixPerDet43", "Pix per det l4 - ring 3",
			    sizeH, lowH, highH);
  hpixPerDet44 = fs->make<TH1D>( "hpixPerDet44", "Pix per det l4 - ring 4",
			    sizeH, lowH, highH);

  //sizeH=1000;
  //highH = 999.5;

  sizeH=10000;
#ifdef HI
  highH = 19999.5;
#else
  highH = 9999.5;
#endif

  hclusPerLay1 = fs->make<TH1D>( "hclusPerLay1", "Clus per layer l1",
				 sizeH, lowH, highH);
  hclusPerLay2 = fs->make<TH1D>( "hclusPerLay2", "Clus per layer l2",
			    sizeH, lowH, highH);
  hclusPerLay3 = fs->make<TH1D>( "hclusPerLay3", "Clus per layer l3",
			    sizeH, lowH, highH);
  hclusPerLay4 = fs->make<TH1D>( "hclusPerLay4", "Clus per layer l4",
			    sizeH, lowH, highH);

  hclus = fs->make<TH1D>( "hclus", "Clus per event, after cut",
			    sizeH, lowH, 5.*highH);
  hclus2 = fs->make<TH1D>( "hclus2", "Clus per event - zoomed",
			   1000, -0.5, 999.5);
  hclusBPix = fs->make<TH1D>( "hclusBPix", "Bpix Clus per event",
			    sizeH, lowH, 4.*highH);
  hclusFPix = fs->make<TH1D>( "hclusFPix", "Fpix Clus per event",
			    sizeH, lowH, 2.*highH);

  sizeH=1000;
#ifdef HI
  highH = 19999.5;
#else
  highH = 4999.5;
#endif

  //hclus5 = fs->make<TH1D>( "hclus5", "Clus per event, none edge",
  //			   sizeH, lowH, highH);

#ifdef HI
  highH = 3999.5;
#else
  highH = 1999.5;
#endif

  hclusPerDisk1 = fs->make<TH1D>( "hclusPerDisk1", "Clus per disk1",
			    sizeH, lowH, highH);
  hclusPerDisk2 = fs->make<TH1D>( "hclusPerDisk2", "Clus per disk2",
			    sizeH, lowH, highH);
  hclusPerDisk3 = fs->make<TH1D>( "hclusPerDisk3", "Clus per disk3",
			    sizeH, lowH, highH);
  hclusPerDisk4 = fs->make<TH1D>( "hclusPerDisk4", "Clus per disk4",
			    sizeH, lowH, highH);
  hclusPerDisk5 = fs->make<TH1D>( "hclusPerDisk5", "Clus per disk5",
			    sizeH, lowH, highH);
  hclusPerDisk6 = fs->make<TH1D>( "hclusPerDisk6", "Clus per disk6",
			    sizeH, lowH, highH);

#ifdef HI
  highH = 9999.5;
#else
  highH = 9999.5;
#endif

  hpixPerDisk1 = fs->make<TH1D>( "hpixPerDisk1", "Pix per disk1",
			    sizeH, lowH, highH);
  hpixPerDisk2 = fs->make<TH1D>( "hpixPerDisk2", "Pix per disk2",
			    sizeH, lowH, highH);
  hpixPerDisk3 = fs->make<TH1D>( "hpixPerDisk3", "Pix per disk3",
			    sizeH, lowH, highH);
  hpixPerDisk4 = fs->make<TH1D>( "hpixPerDisk4", "Pix per disk4",
			    sizeH, lowH, highH);
  hpixPerDisk5 = fs->make<TH1D>( "hpixPerDisk5", "Pix per disk5",
			    sizeH, lowH, highH);
  hpixPerDisk6 = fs->make<TH1D>( "hpixPerDisk6", "Pix per disk6",
			    sizeH, lowH, highH);


  sizeH=4000;
#ifdef HI
  highH = 99999.5;
#else
  highH = 39999.5;
#endif

  hpixPerLay1 = fs->make<TH1D>( "hpixPerLay1", "Pix per layer l1",
				 sizeH, lowH, highH);
  hpixPerLay2 = fs->make<TH1D>( "hpixPerLay2", "Pix per layer l2",
				 sizeH, lowH, highH);
  hpixPerLay3 = fs->make<TH1D>( "hpixPerLay3", "Pix per layer l3",
				 sizeH, lowH, highH);
  hpixPerLay4 = fs->make<TH1D>( "hpixPerLay4", "Pix per layer l4",
				 sizeH, lowH, highH);

  hdigis = fs->make<TH1D>( "hdigis", "All Digis in clus per event, after cut",
				 sizeH, lowH, 5.*highH);
  hdigisB = fs->make<TH1D>( "hdigisB", "BPix Digis in clus per event",
				 sizeH, lowH, 4.*highH);
  hdigisF = fs->make<TH1D>( "hdigisF", "FPix Digis in clus per event",
				 sizeH, lowH, 2.*highH);
  hdigis2 = fs->make<TH1D>( "hdigis2", "Digis per event - zoomed",1000, -0.5, 999.5);

  hdetsPerLay1 = fs->make<TH1D>( "hdetsPerLay1", "Full dets per layer l1",
				 161, -0.5, 160.5);
  hdetsPerLay2 = fs->make<TH1D>( "hdetsPerLay2", "Full dets per layer l2",
				 257, -0.5, 256.5);
  hdetsPerLay3 = fs->make<TH1D>( "hdetsPerLay3", "Full dets per layer l3",
				 353, -0.5, 352.5);
  hdetsPerLay4 = fs->make<TH1D>( "hdetsPerLay4", "Full dets per layer l4",
				 513, -0.5, 512.5);

  hdets2 = fs->make<TH1D>("hdets2","Dets per event after cut",2000, -0.5, 1999.5);
  hdets = fs->make<TH1D>( "hdets","Dets per event before cut",
			  2000, -0.5, 1999.5);
  hmaxPixPerDet = fs->make<TH1D>( "hmaxPixPerDet","Max pixels per det",1000, -0.5, 999.5);

  sizeH= 400;
  lowH = 0.;
  highH = 400.0; // charge limit in kelec
  hcharge1 = fs->make<TH1D>( "hcharge1", "Clu charge l1", sizeH, 0.,highH); //in ke
  hcharge1_zoom = fs->make<TH1D>( "hcharge1_zoom", "Clu charge l1", 500, 0.,10.); //in ke
  hcharge2 = fs->make<TH1D>( "hcharge2", "Clu charge l2", sizeH, 0.,highH);
  hcharge3 = fs->make<TH1D>( "hcharge3", "Clu charge l3", sizeH, 0.,highH);
  hcharge4 = fs->make<TH1D>( "hcharge4", "Clu charge l4", sizeH, 0.,highH);
  hcharge5 = fs->make<TH1D>( "hcharge5", "Clu charge d1", sizeH, 0.,highH);
  hcharge6 = fs->make<TH1D>( "hcharge6", "Clu charge d2", sizeH, 0.,highH);
  hcharge7 = fs->make<TH1D>( "hcharge7", "Clu charge d3", sizeH, 0.,highH);
  hcharge8 = fs->make<TH1D>( "hcharge8", "Clu charge test", sizeH, 0.,highH);
  hcharge1n = fs->make<TH1D>( "hcharge1n", "Clu charge l1", sizeH, 0.,highH); //in ke
  hcharge2b = fs->make<TH1D>( "hcharge2b", "Clu charge l2", sizeH, 0.,highH); //in ke
  hcharge2g = fs->make<TH1D>( "hcharge2g", "Clu charge l2", sizeH, 0.,highH); //in ke

  hcharge11 = fs->make<TH1D>( "hcharge11", "Clu charge l1-inner", sizeH, 0.,highH); //in ke
  hcharge12 = fs->make<TH1D>( "hcharge12", "Clu charge l1-outer", sizeH, 0.,highH); //in ke

  sizeH=  400; // 600
  highH = 100.0; // charge limit in kelec
  hpixcharge1 = fs->make<TH1D>( "hpixcharge1", "Pix charge l1",sizeH, 0.,highH);//in ke
  hpixcharge1_zoom = fs->make<TH1D>( "hpixcharge1_zoom", "Pix charge l1",4000, 0.,4.);//in ke
  hpixcharge2 = fs->make<TH1D>( "hpixcharge2", "Pix charge l2",sizeH, 0.,highH);
  hpixcharge3 = fs->make<TH1D>( "hpixcharge3", "Pix charge l3",sizeH, 0.,highH);
  hpixcharge4 = fs->make<TH1D>( "hpixcharge4", "Pix charge l4",sizeH, 0.,highH);
  hpixcharge5 = fs->make<TH1D>( "hpixcharge5", "Pix charge d1",sizeH, 0.,highH);
  hpixcharge6 = fs->make<TH1D>( "hpixcharge6", "Pix charge d1",sizeH, 0.,highH);
  hpixcharge7 = fs->make<TH1D>( "hpixcharge7", "Pix charge d2",sizeH, 0.,highH);
  hpixcharge8 = fs->make<TH1D>( "hpixcharge8", "Pix charge test",sizeH, 0.,highH);

  hpixcharge11 = fs->make<TH1D>( "hpixcharge11", "Pix charge l1-inner",sizeH, 0.,highH);//in ke
  hpixcharge12 = fs->make<TH1D>( "hpixcharge12", "Pix charge l1-outer",sizeH, 0.,highH);//in ke
  hpixcharge1n = fs->make<TH1D>( "hpixcharge1n", "Pix charge l1",sizeH, 0.,highH);//in ke
  hpixcharge2b = fs->make<TH1D>( "hpixcharge2b", "Pix charge l2",sizeH, 0.,highH);//in ke
  hpixcharge2g = fs->make<TH1D>( "hpixcharge2g", "Pix charge l2",sizeH, 0.,highH);//in ke

  sizeH=300;
  highH = 299.5; // charge limit in kelec
  hsize1 = fs->make<TH1D>( "hsize1", "layer 1 clu size",sizeH,-0.5,highH);
  hsize2 = fs->make<TH1D>( "hsize2", "layer 2 clu size",sizeH,-0.5,highH);
  hsize3 = fs->make<TH1D>( "hsize3", "layer 3 clu size",sizeH,-0.5,highH);
  hsize4 = fs->make<TH1D>( "hsize4", "layer 4 clu size",sizeH,-0.5,highH);
  hsize1n = fs->make<TH1D>( "hsize1n", "layer 1 clu size",sizeH,-0.5,highH);
  hsize2b = fs->make<TH1D>( "hsize2b", "layer 2 clu size",sizeH,-0.5,highH);
  hsize2g = fs->make<TH1D>( "hsize2g", "layer 2 clu size",sizeH,-0.5,highH);
  
  hsizex1 = fs->make<TH1D>( "hsizex1", "lay1 clu size in x",
		      100,-0.5,99.5);
  hsizex1n = fs->make<TH1D>( "hsizex1n", "lay1 clu size in x",
		      100,-0.5,99.5);
  hsizex2 = fs->make<TH1D>( "hsizex2", "lay2 clu size in x",
		      100,-0.5,99.5);
  hsizex3 = fs->make<TH1D>( "hsizex3", "lay3 clu size in x",
		      100,-0.5,99.5);
  hsizex4 = fs->make<TH1D>( "hsizex4", "lay4 clu size in x",
		      100,-0.5,99.5);
  hsizey1 = fs->make<TH1D>( "hsizey1", "lay1 clu size in y",
		      100,-0.5,99.5);
  hsizey1n = fs->make<TH1D>( "hsizey1n", "lay1 clu size in y",
		      100,-0.5,99.5);
  hsizey2 = fs->make<TH1D>( "hsizey2", "lay2 clu size in y",
		      100,-0.5,99.5);
  hsizey3 = fs->make<TH1D>( "hsizey3", "lay3 clu size in y",
		      100,-0.5,99.5);
  hsizey4 = fs->make<TH1D>( "hsizey4", "lay4 clu size in y",
		      100,-0.5,99.5);

  hpixDiskR1 = fs->make<TH1D>("hpixDiskR1","pix vs. r, disk 1",200,0.,20.);
  hpixDiskR2 = fs->make<TH1D>("hpixDiskR2","pix vs. r, disk 2",200,0.,20.);
  hpixDiskR3 = fs->make<TH1D>("hpixDiskR3","pix vs. r, disk 3",200,0.,20.);

  hevent = fs->make<TH1D>("hevent","event",100,0,10000000.);
  //horbit = fs->make<TH1D>("horbit","orbit",100, 0,100000000.);

  hlumi1  = fs->make<TH1D>("hlumi1", "LS", 2000,0,2000.);
  hlumi0  = fs->make<TH1D>("hlumi0", "LS", 2000,0,2000.);
  hlumi   = fs->make<TH1D>("hlumi", "LS",   2000,0,2000.);
  //hlumi10 = fs->make<TH1D>("hlumi10", "LS", 2000,0,2000.);
  //hlumi11 = fs->make<TH1D>("hlumi11", "lumi11",   2000,0,2000.);
  //hlumi12 = fs->make<TH1D>("hlumi12", "lumi12",   2000,0,2000.);
  //hlumi13 = fs->make<TH1D>("hlumi13", "lumi13",   2000,0,2000.);
  //hlumi14 = fs->make<TH1D>("hlumi14", "lumi14",   2000,0,2000.);

  hbx0    = fs->make<TH1D>("hbx0",   "bx",   4000,0,4000.);  
  hbx    = fs->make<TH1D>("hbx",   "bx",     4000,0,4000.);  

  hgz1 = fs->make<TH1D>("hgz1","layer1, clus vs z",600,-30.,30.);
  hgz2 = fs->make<TH1D>("hgz2","layer2, clus vs z",600,-30.,30.);
  hgz3 = fs->make<TH1D>("hgz3","layer3, clus vs z",600,-30.,30.);
  hgz4 = fs->make<TH1D>("hgz4","layer4, clus vs z",600,-30.,30.);

  hpixz1 = fs->make<TH1D>("hpixz1", "pix vs z l1",520,-26.,26.);
  hpixz2 = fs->make<TH1D>("hpixz2", "pix vs z l2",520,-26.,26.);
  hpixz3 = fs->make<TH1D>("hpixz3", "pix vs z l3",520,-26.,26.);
  hpixz4 = fs->make<TH1D>("hpixz4", "pix ve z l4",520,-26.,26.);


  htest1 = fs->make<TH2F>("htest1","size vs clu charge",50,0.,400.,15,0.5,15.5);
  htest2 = fs->make<TH2F>("htest2","size vs pix charge",50,0.,100.,15,0.5,15.5);
  htest3 = fs->make<TH2F>("htest3","size vs clu charge",50,0.,400.,10,0.5,10.5);
  htest4 = fs->make<TH2F>("htest4","size vs pix charge",50,0.,100.,10,0.5,10.5);


  // dets hit per event
  hDets1 = fs->make<TH2F>("hDets1"," hit modules per event",
			     9,-4.5,4.5,13,-6.5,6.5);
  hDets1->SetOption("colz");
  hDets2 = fs->make<TH2F>("hDets2"," hit modules per event",
			     9,-4.5,4.5,29,-14.5,14.5);
  hDets2->SetOption("colz");
  hDets3 = fs->make<TH2F>("hDets3"," hit modules per event",
			     9,-4.5,4.5,45,-22.5,22.5);
  hDets3->SetOption("colz");
  hDets4 = fs->make<TH2F>("hDets4"," hit modules per event",
			     9,-4.5,4.5,65,-32.5,32.5);
  hDets4->SetOption("colz");

  // clus per det
  hcluDets1 = fs->make<TH2F>("hcluDets1"," clusters L1 per event",
			    9,-4.5,4.5,13,-6.5,6.5);
  hcluDets1->SetOption("colz");
  hcluDets2 = fs->make<TH2F>("hcluDets2"," clusters L2 per event",
			    9,-4.5,4.5,29,-14.5,14.5);
  hcluDets2->SetOption("colz");
  hcluDets3 = fs->make<TH2F>("hcluDets3"," clusters L3 per event",
			    9,-4.5,4.5,45,-22.5,22.5);
  hcluDets3->SetOption("colz");
  hcluDets4 = fs->make<TH2F>("hcluDets4"," clusters L4 per event",
			    9,-4.5,4.5,65,-32.5,32.5);
  hcluDets4->SetOption("colz");

  // pix per det
  hpixDets1 = fs->make<TH2F>("hpixDets1"," pixels L1 per event",
			     9,-4.5,4.5,13,-6.5,6.5);
  hpixDets1->SetOption("colz");
  hpixDets2 = fs->make<TH2F>("hpixDets2"," pixels L2 per event",
			     9,-4.5,4.5,29,-14.5,14.5);
  hpixDets2->SetOption("colz");
  hpixDets3 = fs->make<TH2F>("hpixDets3"," pixels L3 per event",
			     9,-4.5,4.5,45,-22.5,22.5);
  hpixDets3->SetOption("colz");
  hpixDets4 = fs->make<TH2F>("hpixDets4"," pixels L4 per event",
			     9,-4.5,4.5,65,-32.5,32.5);
  hpixDets4->SetOption("colz");

  // clu size per det
  hsizeDets1 = fs->make<TH2F>("hsizeDets1"," cluster size L1",
				9,-4.5,4.5,13,-6.5,6.5);
  hsizeDets1->SetOption("colz");
  hsizeDets2 = fs->make<TH2F>("hsizeDets2"," cluster size L2",
				9,-4.5,4.5,29,-14.5,14.5);
  hsizeDets2->SetOption("colz");
  hsizeDets3 = fs->make<TH2F>("hsizeDets3"," cluster size L3",
				9,-4.5,4.5,45,-22.5,22.5);
  hsizeDets3->SetOption("colz");
  hsizeDets4 = fs->make<TH2F>("hsizeDets4"," cluster size L4",
				9,-4.5,4.5,65,-32.5,32.5);
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

  hpixchargeDets1 = fs->make<TProfile2D>("hpixchargeDets1","pix charge L1",
			        9,-4.5,4.5,13,-6.5,6.5,0.,1000.);
  hpixchargeDets1->SetOption("colz");
  hpixchargeDets2 = fs->make<TProfile2D>("hpixchargeDets2","pix charge L2",
				9,-4.5,4.5,29,-14.5,14.5,0.,1000.);
  hpixchargeDets2->SetOption("colz");
  hpixchargeDets3 = fs->make<TProfile2D>("hpixchargeDets3","pix charge L3",
				9,-4.5,4.5,45,-22.5,22.5,0.,1000.);
  hpixchargeDets3->SetOption("colz");
  hpixchargeDets4 = fs->make<TProfile2D>("hpixchargeDets4","pix charge L4",
				9,-4.5,4.5,65,-32.5,32.5,0.,1000.);
  hpixchargeDets4->SetOption("colz");

  // pix in all dets (superimposed)
  hpixDetMap1 = fs->make<TH2F>( "hpixDetMap1", "pix in det layer 1",
		      416,0.,416.,160,0.,160.);
  hpixDetMap1->SetOption("colz");
  hpixDetMap2 = fs->make<TH2F>( "hpixDetMap2", "pix in det layer 2",
		      416,0.,416.,160,0.,160.);
  hpixDetMap2->SetOption("colz");
  hpixDetMap3 = fs->make<TH2F>( "hpixDetMap3", "pix in det layer 3",
		      416,0.,416.,160,0.,160.);
  hpixDetMap3->SetOption("colz");
  hpixDetMap4 = fs->make<TH2F>( "hpixDetMap4", "pix in det layer 4",
		      416,0.,416.,160,0.,160.);
  hpixDetMap4->SetOption("colz");

  // clus in all dets (superimposed)
  hcluDetMap1 = fs->make<TH2F>( "hcluDetMap1", "clu in det layer 1",
				416,0.,416.,160,0.,160.);
  hcluDetMap1->SetOption("colz");
  hcluDetMap2 = fs->make<TH2F>( "hcluDetMap2", "clu in det layer 2",
				416,0.,416.,160,0.,160.);
  hcluDetMap2->SetOption("colz");
  hcluDetMap3 = fs->make<TH2F>( "hcluDetMap3", "clu in det layer 3",
				416,0.,416.,160,0.,160.);
  hcluDetMap3->SetOption("colz");
  hcluDetMap4 = fs->make<TH2F>( "hcluDetMap4", "clu in det layer 3",
				416,0.,416.,160,0.,160.);
  hcluDetMap4->SetOption("colz");
  hsizeyz1 = fs->make<TH2F>( "hsizeyz1", "sizy vs z for layer 1",
			     56,-28.,28.,20,0.5,20.5);
  hsizeyz2 = fs->make<TH2F>( "hsizeyz2", "sizy vs z for layer 2",
			     56,-28.,28.,14,0.5,14.5);
  hsizeyz3 = fs->make<TH2F>( "hsizeyz3", "sizy vs z for layer 3",
			     56,-28.,28.,14,0.5,14.5);
  hsizeyz4 = fs->make<TH2F>( "hsizeyz4", "sizy vs z for layer 4",
			     56,-28.,28.,14,0.5,14.5);

#ifdef TESTING_ADC

  //TH1D *hclusPerDetT1, *hpixPerDetT1;
  hclusPerDetT1 = fs->make<TH1D>( "hclusPerDetT1", "Clus per det l1",
				 200, -0.5, 199.5);
  hpixPerDetT1 = fs->make<TH1D>( "hpixPerDetT1", "Pix per det l1",
				 500, -0.5,999.5);

  // hchargeT1, hsizeT1 , hchargeT12_1 , hchargeT12_2  
  sizeH= 400;
  highH = 400.0; // charge limit in kelec
  hchargeT1 = fs->make<TH1D>( "hchargeT1", "Clu charge l1", sizeH, 0.,highH); //in
  sizeH=300;
  highH = 299.5; // charge limit in kelec
  hsizeT1 = fs->make<TH1D>( "hsizeT1", "layer 1 clu size",sizeH,-0.5,highH);

  hchargeT12_1    = fs->make<TH1D>( "hchargeT12_1", "Clu charge l2_1", 400, 0.,400.);
  hchargeT12_2    = fs->make<TH1D>( "hchargeT12_2", "Clu charge l2_1", 400, 0.,400.);

  // pix per det
  hpixDetsT1 = fs->make<TH2F>("hpixDetsT1"," pixels per event",
			     9,-4.5,4.5,13,-6.5,6.5);
  hpixDetsT1->SetOption("colz");
  // hpixDetsT2 = fs->make<TH2F>("hpixDetsT2"," pixels per event",
  // 			     9,-4.5,4.5,29,-14.5,14.5);
  // hpixDetsT2->SetOption("colz");
  // hpixDetsT3 = fs->make<TH2F>("hpixDetsT3"," pixels per event",
  // 			     9,-4.5,4.5,45,-22.5,22.5);
  // hpixDetsT3->SetOption("colz");
  // hpixDetsT4 = fs->make<TH2F>("hpixDetsT4"," pixels per event",
  // 			     9,-4.5,4.5,65,-32.5,32.5);
  // hpixDetsT4->SetOption("colz");

  // pixel in modules 
  hpixDetMapT1 = fs->make<TH2F>( "hpixDetMapT1", "pix in det layer 1",
		      416,0.,416.,160,0.,160.);
  hpixDetMapT1->SetOption("colz");
  // hpixDetMapT2 = fs->make<TH2F>( "hpixDetMapT2", "pix in det layer 2",
  // 		      416,0.,416.,160,0.,160.);
  // hpixDetMapT2->SetOption("colz");
  // hpixDetMapT3 = fs->make<TH2F>( "hpixDetMapT3", "pix in det layer 3",
  // 		      416,0.,416.,160,0.,160.);
  // hpixDetMapT3->SetOption("colz");
  // hpixDetMapT4 = fs->make<TH2F>( "hpixDetMapT4", "pix in det layer 4",
  // 		      416,0.,416.,160,0.,160.);
  // hpixDetMapT4->SetOption("colz");

  hsizeDetT1 = fs->make<TProfile2D>("hsizeDetT", "clus size",
				    9,-4.5,4.5,13,-6.5,6.5,0.,10000.);
  hcluDetT1 = fs->make<TProfile2D>("hcluDetT", "clus char",
				    9,-4.5,4.5,13,-6.5,6.5,0.,100000.);

#endif


   hclumult1 = fs->make<TProfile>("hclumult1","cluster size layer 1",560,-28.,28.,0.0,100.);
   hclumult2 = fs->make<TProfile>("hclumult2","cluster size layer 2",560,-28.,28.,0.0,100.);
   hclumult3 = fs->make<TProfile>("hclumult3","cluster size layer 3",560,-28.,28.,0.0,100.);
   hclumult4 = fs->make<TProfile>("hclumult4","cluster size layer 4",560,-28.,28.,0.0,100.);

   hclumultx1 = fs->make<TProfile>("hclumultx1","cluster x-size layer 1",560,-28.,28.,0.0,100.);
   hclumultx2 = fs->make<TProfile>("hclumultx2","cluster x-size layer 2",560,-28.,28.,0.0,100.);
   hclumultx3 = fs->make<TProfile>("hclumultx3","cluster x-size layer 3",560,-28.,28.,0.0,100.);
   hclumultx4 = fs->make<TProfile>("hclumultx4","cluster x-size layer 4",560,-28.,28.,0.0,100.);

   hclumulty1 = fs->make<TProfile>("hclumulty1","cluster y-size layer 1",560,-28.,28.,0.0,100.);
   hclumulty2 = fs->make<TProfile>("hclumulty2","cluster y-size layer 2",560,-28.,28.,0.0,100.);
   hclumulty3 = fs->make<TProfile>("hclumulty3","cluster y-size layer 3",560,-28.,28.,0.0,100.);
   hclumulty4 = fs->make<TProfile>("hclumulty4","cluster y-size layer 4",560,-28.,28.,0.0,100.);

   hcluchar1 = fs->make<TProfile>("hcluchar1","cluster char layer 1",560,-28.,28.,0.0,1000.);
   hcluchar2 = fs->make<TProfile>("hcluchar2","cluster char layer 2",560,-28.,28.,0.0,1000.);
   hcluchar3 = fs->make<TProfile>("hcluchar3","cluster char layer 3",560,-28.,28.,0.0,1000.);
   hcluchar4 = fs->make<TProfile>("hcluchar4","cluster char layer 4",560,-28.,28.,0.0,1000.);

   hpixchar1 = fs->make<TProfile>("hpixchar1","pix char layer 1",560,-28.,28.,0.0,1000.);
   hpixchar2 = fs->make<TProfile>("hpixchar2","pix char layer 2",560,-28.,28.,0.0,1000.);
   hpixchar3 = fs->make<TProfile>("hpixchar3","pix char layer 3",560,-28.,28.,0.0,1000.);
   hpixchar4 = fs->make<TProfile>("hpixchar4","pix char layer 4",560,-28.,28.,0.0,1000.);

#ifdef TEST_DCOLS
   hdcolHitProb1 = fs->make<TProfile>("hdcolHitProb1","dcol hit prob layer 1",9,-4.5,4.5,0.0,1.);
   hdcolHitProb2 = fs->make<TProfile>("hdcolHitProb2","dcol hit prob layer 2",9,-4.5,4.5,0.0,1.);
   hdcolHitProb3 = fs->make<TProfile>("hdcolHitProb3","dcol hit prob layer 3",9,-4.5,4.5,0.0,1.);
   hdcolHitProb4 = fs->make<TProfile>("hdcolHitProb4","dcol hit prob layer 4",9,-4.5,4.5,0.0,1.);

   hdcolHitMult1 = fs->make<TProfile>("hdcolHitMult1","dcol hit mult layer 1",9,-4.5,4.5,0.0,200.);
   hdcolHitMult2 = fs->make<TProfile>("hdcolHitMult2","dcol hit mult layer 2",9,-4.5,4.5,0.0,200.);
   hdcolHitMult3 = fs->make<TProfile>("hdcolHitMult3","dcol hit mult layer 3",9,-4.5,4.5,0.0,200.);
   hdcolHitMult4 = fs->make<TProfile>("hdcolHitMult4","dcol hit mult layer 4",9,-4.5,4.5,0.0,200.);

   hrocHitMult1 = fs->make<TProfile>("hrocHitMult1","roc hit mult layer 1",9,-4.5,4.5,0.0,1000.);
   hrocHitMult2 = fs->make<TProfile>("hrocHitMult2","roc hit mult layer 2",9,-4.5,4.5,0.0,1000.);
   hrocHitMult3 = fs->make<TProfile>("hrocHitMult3","roc hit mult layer 3",9,-4.5,4.5,0.0,1000.);
   hrocHitMult4 = fs->make<TProfile>("hrocHitMult4","roc hit mult layer 4",9,-4.5,4.5,0.0,1000.);

   hmoduleHitMult1 = fs->make<TProfile>("hmoduleHitMult1","mod hit mult layer 1",9,-4.5,4.5,0.0,1000.);
   hmoduleHitMult2 = fs->make<TProfile>("hmoduleHitMult2","mod hit mult layer 2",9,-4.5,4.5,0.0,1000.);
   hmoduleHitMult3 = fs->make<TProfile>("hmoduleHitMult3","mod hit mult layer 3",9,-4.5,4.5,0.0,1000.);
   hmoduleHitMult4 = fs->make<TProfile>("hmoduleHitMult4","mod hit mult layer 4",9,-4.5,4.5,0.0,1000.);

   hdcolHitProbLadder1 = fs->make<TProfile>("hdcolHitProbLadder1","dcol hit prob layer 1",13,-6.5,6.5,0.0,1.);
   hdcolHitMultLadder1 = fs->make<TProfile>("hdcolHitMultLadder1","dcol hit mult layer 1",13,-6.5,6.5,0.0,1000.);

#endif

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

#ifdef ROC_RATE
   hrocMap1 = fs->make<TH2F>("hrocMap1"," ",8*9,-4.5,4.5,2*13,-6.5,6.5);
   hrocMap1->SetOption("colz");
   hrocMap2 = fs->make<TH2F>("hrocMap2"," ",8*9,-4.5,4.5,2*29,-14.5,14.5);
   hrocMap2->SetOption("colz");
   hrocMap3 = fs->make<TH2F>("hrocMap3"," ",8*9,-4.5,4.5,2*45,-22.5,22.5);
   hrocMap3->SetOption("colz");
   hrocMap4 = fs->make<TH2F>("hrocMap4"," ",8*9,-4.5,4.5,2*65,-32.5,32.5);
   hrocMap4->SetOption("colz");

   hrocSizeXMap1 = fs->make<TProfile2D>("hrocSizeXMap1"," ",8*9,-4.5,4.5,2*13,-6.5,6.5,0.,1000.);
   hrocSizeXMap1->SetOption("colz");
   hrocSizeXMap2 = fs->make<TProfile2D>("hrocSizeXMap2"," ",8*9,-4.5,4.5,2*29,-14.5,14.5,0.,1000.);
   hrocSizeXMap2->SetOption("colz");
   hrocSizeXMap3 = fs->make<TProfile2D>("hrocSizeXMap3"," ",8*9,-4.5,4.5,2*45,-22.5,22.5,0.,1000.);
   hrocSizeXMap3->SetOption("colz");
   hrocSizeXMap4 = fs->make<TProfile2D>("hrocSizeXMap4"," ",8*9,-4.5,4.5,2*65,-32.5,32.5,0.,1000.);
   hrocSizeXMap4->SetOption("colz");

   hrocSizeYMap1 = fs->make<TProfile2D>("hrocSizeYMap1"," ",8*9,-4.5,4.5,2*13,-6.5,6.5,0.,1000.);
   hrocSizeYMap1->SetOption("colz");
   hrocSizeYMap2 = fs->make<TProfile2D>("hrocSizeYMap2"," ",8*9,-4.5,4.5,2*29,-14.5,14.5,0.,1000.);
   hrocSizeYMap2->SetOption("colz");
   hrocSizeYMap3 = fs->make<TProfile2D>("hrocSizeYMap3"," ",8*9,-4.5,4.5,2*45,-22.5,22.5,0.,1000.);
   hrocSizeYMap3->SetOption("colz");
   hrocSizeYMap4 = fs->make<TProfile2D>("hrocSizeYMap4"," ",8*9,-4.5,4.5,2*65,-32.5,32.5,0.,1000.);
   hrocSizeYMap4->SetOption("colz");

   hrocClucharMap1 = fs->make<TProfile2D>("hrocClucharMap1"," ",8*9,-4.5,4.5,2*13,-6.5,6.5,0.,1000.);
   hrocClucharMap1->SetOption("colz");
   hrocClucharMap2 = fs->make<TProfile2D>("hrocClucharMap2"," ",8*9,-4.5,4.5,2*29,-14.5,14.5,0.,1000.);
   hrocClucharMap2->SetOption("colz");
   hrocClucharMap3 = fs->make<TProfile2D>("hrocClucharMap3"," ",8*9,-4.5,4.5,2*45,-22.5,22.5,0.,1000.);
   hrocClucharMap3->SetOption("colz");
   hrocClucharMap4 = fs->make<TProfile2D>("hrocClucharMap4"," ",8*9,-4.5,4.5,2*65,-32.5,32.5,0.,1000.);
   hrocClucharMap4->SetOption("colz");

   hrocPixcharMap1 = fs->make<TProfile2D>("hrocPixcharMap1"," ",8*9,-4.5,4.5,2*13,-6.5,6.5,0.,1000.);
   hrocPixcharMap1->SetOption("colz");
   hrocPixcharMap2 = fs->make<TProfile2D>("hrocPixcharMap2"," ",8*9,-4.5,4.5,2*29,-14.5,14.5,0.,1000.);
   hrocPixcharMap2->SetOption("colz");
   hrocPixcharMap3 = fs->make<TProfile2D>("hrocPixcharMap3"," ",8*9,-4.5,4.5,2*45,-22.5,22.5,0.,1000.);
   hrocPixcharMap3->SetOption("colz");
   hrocPixcharMap4 = fs->make<TProfile2D>("hrocPixcharMap4"," ",8*9,-4.5,4.5,2*65,-32.5,32.5,0.,1000.);
   hrocPixcharMap4->SetOption("colz");
   
   hrocLadder1 = fs->make<TH1D>("hrocLadder1"," ",2*13,-6.5,6.5);
   hrocZ11 = fs->make<TH1D>("hrocZ11","roc pix occ. l1 inner",8*9,-4.5,4.5);
   hrocZ12 = fs->make<TH1D>("hrocZ12","roc pix occ. l1 outer",8*9,-4.5,4.5);
   
   hpixRocRate1 = fs->make<TH1D>("hpixRocRate1"," ",8*9,-4.5,4.5);
   hpixRocRate2 = fs->make<TH1D>("hpixRocRate2"," ",8*9,-4.5,4.5);
   hpixRocRate3 = fs->make<TH1D>("hpixRocRate3"," ",8*9,-4.5,4.5);
   hpixRocRate4 = fs->make<TH1D>("hpixRocRate4"," ",8*9,-4.5,4.5);
   hcluRocRate1 = fs->make<TH1D>("hcluRocRate1"," ",8*9,-4.5,4.5);
   hcluRocRate2 = fs->make<TH1D>("hcluRocRate2"," ",8*9,-4.5,4.5);
   hcluRocRate3 = fs->make<TH1D>("hcluRocRate3"," ",8*9,-4.5,4.5);
   hcluRocRate4 = fs->make<TH1D>("hcluRocRate4"," ",8*9,-4.5,4.5);
#endif

   sizeH = 1000;
   highH = 3000.; 
   hclusls = fs->make<TProfile>("hclusls","clus vs ls",sizeH,0.,highH,0.0,50000.);
   hpixls  = fs->make<TProfile>("hpixls", "pix vs ls ",sizeH,0.,highH,0.0,200000.);

#ifdef PV
   hpvls = fs->make<TProfile>("hpvls","pvs vs ls",sizeH,0.,highH,0.0,10000.);
   //hpvlsn = fs->make<TProfile>("hpvlsn","pvs/lumi vs ls",sizeH,0.,highH,0.0,1000.);
   hpvs   = fs->make<TH1D>("hpvs", "pvs",100,-0.5,99.5);
   hpixpv  = fs->make<TProfile>("hpixpv","pv vs pix", 100,0.,100,0.0,100000.);
   hclupv  = fs->make<TProfile>("hclupv","pv vs clus",100,0.,100,0.0,50000.);
   //hpixbx  = fs->make<TProfile>("hpixbx", "pix vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   //hclubx  = fs->make<TProfile>("hclubx", "clu vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hpvbx   = fs->make<TProfile>("hpvbx",  "pv vs bx ", 4000,-0.5,3999.5,0.0,1000000.);
   htracks   = fs->make<TH1D>("htracks",  "tracks ", 1000,-0.5,4999.5);
#endif // PV

   //hcharCluls = fs->make<TProfile>("hcharCluls","clu char vs ls",sizeH,0.,highH,0.0,500.);
   //hcharPixls = fs->make<TProfile>("hcharPixls","pix char vs ls",sizeH,0.,highH,0.0,500.);
   //hsizeCluls = fs->make<TProfile>("hsizeCluls","clu size vs ls",sizeH,0.,highH,0.0,300.);
   //hsizeXCluls= fs->make<TProfile>("hsizeXCluls","clu size-x vs ls",sizeH,0.,highH,0.0,100.);
   //hcharClubx  = fs->make<TProfile>("hcharClubx", "clu charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   //hcharPixbx  = fs->make<TProfile>("hcharPixbx", "pix charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   //hsizeClubx  = fs->make<TProfile>("hsizeClubx", "clu size vs bx ",4000,-0.5,3999.5,0.0,1000.);
   //hsizeYClubx = fs->make<TProfile>("hsizeYClubx", "clu size-y vs bx ",4000,-0.5,3999.5,0.0,1000.);
   //sizeH = 1000;
   //highH = 3000.; 

#ifdef MINMAX
   hcharMin1 = fs->make<TH1D>("hcharMin1","min pix char in a cluster L1",600,0.,60.);
   hcharMin2 = fs->make<TH1D>("hcharMin2","min pix char in a cluster L2",600,0.,60.);
   hcharMin3 = fs->make<TH1D>("hcharMin3","min pix char in a cluster L3",600,0.,60.);
   hcharMin4 = fs->make<TH1D>("hcharMin4","min pix char in a cluster L4",600,0.,60.);
   hcharMax1 = fs->make<TH1D>("hcharMax1","max pix char in a cluster L1",600,0.,60.);
   hcharMax2 = fs->make<TH1D>("hcharMax2","max pix char in a cluster L2",600,0.,60.);
   hcharMax3 = fs->make<TH1D>("hcharMax3","max pix char in a cluster L3",600,0.,60.);
   hcharMax4 = fs->make<TH1D>("hcharMax4","max pix char in a cluster L4",600,0.,60.);
#endif

#ifdef NEW_MODULES
  hsize11 = fs->make<TH1D>( "hsize11", "layer 1 clu size new",sizeH,-0.5,highH);
  hsize12 = fs->make<TH1D>( "hsize12", "layer 2 clu size new",sizeH,-0.5,highH);
  hsize13 = fs->make<TH1D>( "hsize13", "layer 3 clu size new",sizeH,-0.5,highH);
  hsizex11 = fs->make<TH1D>( "hsizex11", "lay1 clu size in x new",
		      100,-0.5,99.5);
  hsizex12 = fs->make<TH1D>( "hsizex12", "lay2 clu size in x new",
		      100,-0.5,99.5);
  hsizex13 = fs->make<TH1D>( "hsizex13", "lay3 clu size in x new",
		      100,-0.5,99.5);
  hsizex13 = fs->make<TH1D>( "hsizex14", "lay4 clu size in x new",
		      100,-0.5,99.5);
  hsizey11 = fs->make<TH1D>( "hsizey11", "lay1 clu size in y new",
		      100,-0.5,99.5);
  hsizey12 = fs->make<TH1D>( "hsizey12", "lay2 clu size in y new",
		      100,-0.5,99.5);
  hsizey13 = fs->make<TH1D>( "hsizey13", "lay3 clu size in y new",
		      100,-0.5,99.5);
  hsizey14 = fs->make<TH1D>( "hsizey14", "lay4 clu size in y new",
		      100,-0.5,99.5);
#endif

#ifdef L1
  hl1a    = fs->make<TH1D>("hl1a",   "l1a",   128,-0.5,127.5);
  hl1t    = fs->make<TH1D>("hl1t",   "l1t",   128,-0.5,127.5);
  hltt    = fs->make<TH1D>("hltt",   "ltt",   128,-0.5,127.5);
  hl1t1 = fs->make<TH1D>("hl1t1","l1t1",128,-0.5,127.5);
  hl1a1 = fs->make<TH1D>("hl1a1","l1a1",128,-0.5,127.5);
#endif
#ifdef HLT
  hlt1 = fs->make<TH1D>("hlt1","hlt1",512,-0.5,511.5);
  hlt2 = fs->make<TH1D>("hlt2","hlt2",512,-0.5,511.5);
  hlt3 = fs->make<TH1D>("hlt3","hlt3",512,-0.5,511.5);
#endif
  
#ifdef USE_RESYNCS
  htest1 = fs->make<TH1D>("htest1","test1",1000,0.,1000.);
  htest2 = fs->make<TH1D>("htest2","test2",1000,0.,1000.);
  htest3 = fs->make<TH1D>("htest3","test3",1000,0.,3.0E8);
  htest4 = fs->make<TH1D>("htest4","test4",1000,0.,3.0E8);
#endif

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

  hpixDetMap40 = fs->make<TH2F>( "hpixDetMap40", "pix det layer 4",
				 416,0.,416.,160,0.,160.);
  hpixDetMap41 = fs->make<TH2F>( "hpixDetMap41", "pix det layer 4",
				 416,0.,416.,160,0.,160.);
  hpixDetMap42 = fs->make<TH2F>( "hpixDetMap42", "pix det layer 4",
				 416,0.,416.,160,0.,160.);
  hpixDetMap43 = fs->make<TH2F>( "hpixDetMap43", "pix det layer 4",
				 416,0.,416.,160,0.,160.);
  hpixDetMap44 = fs->make<TH2F>( "hpixDetMap44", "pix det layer 4",
				  416,0.,416.,160,0.,160.);
  hpixDetMap45 = fs->make<TH2F>( "hpixDetMap45", "pix det layer 4",
				  416,0.,416.,160,0.,160.);
  hpixDetMap46 = fs->make<TH2F>( "hpixDetMap46", "pix det layer 4",
				  416,0.,416.,160,0.,160.);
  hpixDetMap47 = fs->make<TH2F>( "hpixDetMap47", "pix det layer 4",
				  416,0.,416.,160,0.,160.);
  hpixDetMap48 = fs->make<TH2F>( "hpixDetMap48", "pix det layer 4",
				  416,0.,416.,160,0.,160.);
  hpixDetMap49 = fs->make<TH2F>( "hpixDetMap49", "pix det layer 4",
				  416,0.,416.,160,0.,160.);
#endif 

#ifdef STUDY_LAY1
  hcharge111     = fs->make<TH1D>( "hcharge111", "Clu charge l11", 400, 0.,400.);
  hpixcharge111  = fs->make<TH1D>( "hpixcharge111", "Pix charge l11",400, 0.,100.);
  hcharge112     = fs->make<TH1D>( "hcharge112", "Clu charge l12", 400, 0.,400.);
  hpixcharge112  = fs->make<TH1D>( "hpixcharge12", "Pix charge l12",400, 0.,100.);
  hcharge121     = fs->make<TH1D>( "hcharge121", "Clu charge l21", 400, 0.,400.);
  hpixcharge121  = fs->make<TH1D>( "hpixcharge121", "Pix charge l21",400, 0.,100.);
  hcharge122     = fs->make<TH1D>( "hcharge122", "Clu charge l22", 400, 0.,400.);
  hpixcharge122  = fs->make<TH1D>( "hpixcharge122", "Pix charge l22",400, 0.,100.);
  hcharge12_1    = fs->make<TH1D>( "hcharge12_1", "Clu charge l2_1", 400, 0.,400.);
  hpixcharge12_1 = fs->make<TH1D>( "hpixcharge12_1", "Pix charge l2_1",400, 0.,100.);
  hcharge12_2    = fs->make<TH1D>( "hcharge12_2", "Clu charge l2_2", 400, 0.,400.);
  hpixcharge12_2 = fs->make<TH1D>( "hpixcharge12_2", "Pix charge l2_2",400, 0.,100.);
#endif

#ifdef LS_TESTS
   sizeH = 250;
   highH =  250.; 
   hcharClu1ls = fs->make<TProfile>("hcharClu1ls","clu char 1 vs ls",sizeH,0.,highH,0.0,400.);
   hcharPix1ls = fs->make<TProfile>("hcharPix1ls","pix char 1 vs ls",sizeH,0.,highH,0.0,400.);
   hsizeClu1ls = fs->make<TProfile>("hsizeClu1ls","clu size 1 vs ls",sizeH,0.,highH,0.0,400.);
   hcharClu2ls = fs->make<TProfile>("hcharClu2ls","clu char 2 vs ls",sizeH,0.,highH,0.0,400.);
   hcharPix2ls = fs->make<TProfile>("hcharPix2ls","pix char 2 vs ls",sizeH,0.,highH,0.0,400.);
   hsizeClu2ls = fs->make<TProfile>("hsizeClu2ls","clu size 2 vs ls",sizeH,0.,highH,0.0,400.);
   hcharClu3ls = fs->make<TProfile>("hcharClu3ls","clu char 3 vs ls",sizeH,0.,highH,0.0,400.);
   hcharPix3ls = fs->make<TProfile>("hcharPix3ls","pix char 3 vs ls",sizeH,0.,highH,0.0,400.);
   hsizeClu3ls = fs->make<TProfile>("hsizeClu3ls","clu size 3 vs ls",sizeH,0.,highH,0.0,400.);
   hcharClu4ls = fs->make<TProfile>("hcharClu4ls","clu char 4 vs ls",sizeH,0.,highH,0.0,400.);
   hcharPix4ls = fs->make<TProfile>("hcharPix4ls","pix char 4 vs ls",sizeH,0.,highH,0.0,400.);
   hsizeClu4ls = fs->make<TProfile>("hsizeClu4ls","clu size 4 vs ls",sizeH,0.,highH,0.0,400.);

   hpixb4l  = fs->make<TProfile>("hpixb4l", "pixb4 vs ls ", sizeH,0.,highH,0.0,100000.);
   hclusb4l = fs->make<TProfile>("hclusb4l","clusb4 vs ls", sizeH,0.,highH,0.0,30000.);
   hpixb1l  = fs->make<TProfile>("hpixb1l", "pixb1 vs ls", sizeH,0.,highH,0.0,100000.);
   hclusb1l = fs->make<TProfile>("hclusb1l","clusb1 vs s",sizeH,0.,highH,0.0,30000.);
   hpixb2l  = fs->make<TProfile>("hpixb2l", "pixb2 vs ls ",sizeH,0.,highH,0.0,100000.);
   hclusb2l = fs->make<TProfile>("hclusb2l","clusb2 vs ls",sizeH,0.,highH,0.0,30000.);
   hpixb3l  = fs->make<TProfile>("hpixb3l", "pixb3 vs ls",sizeH,0.,highH,0.0,100000.);
   hclusb3l = fs->make<TProfile>("hclusb3l","clusb3 vs ls",sizeH,0.,highH,0.0,30000.);
   hpixfl   = fs->make<TProfile>("hpixfl",  "pixf vs ls", sizeH,0.,highH,0.0,100000.);
   hclusfl  = fs->make<TProfile>("hclusfl", "clusf vs ls", sizeH,0.,highH,0.0,30000.);
   hpixfml  = fs->make<TProfile>("hpixfml", "pixfm vs ls",sizeH,0.,highH,0.0,100000.);
   hclusfml = fs->make<TProfile>("hclusfml","clusfm vs ls",sizeH,0.,highH,0.0,30000.);
   hpixfpl  = fs->make<TProfile>("hpixfpl", "pixfp vs ls",sizeH,0.,highH,0.0,100000.);
   hclusfpl = fs->make<TProfile>("hclusfpl","clusfp vs ls",sizeH,0.,highH,0.0,30000.);
#endif


#ifdef Lumi
   hinstbx = fs->make<TProfile>("hinstbx", "inst lumi vs bx ",4000,-0.5,3999.5,0.0,100.);
   hinst  = fs->make<TH1D>("hinst", "inst lumi",100,0.0,10.);
   hinstls  = fs->make<TProfile>("hinstls", "inst bx lumi vs ls ",sizeH,0.,highH,0.0,1000.);
   hinstlsbx= fs->make<TProfile>("hinstlsbx","inst bx lumi vs ls ",sizeH,0.,highH,0.0,1000.);
#endif 

#if defined(BX) || defined(BX_NEW)
   hbx6    = fs->make<TH1D>("hbx6",   "bx",   4000,0,4000.);  
   hbx5    = fs->make<TH1D>("hbx5",   "bx",   4000,0,4000.);  
   hbx4    = fs->make<TH1D>("hbx4",   "bx",   4000,0,4000.);  
   hbx3    = fs->make<TH1D>("hbx3",   "bx",   4000,0,4000.);  
   hbx2    = fs->make<TH1D>("hbx2",   "bx",   4000,0,4000.);  
   hbx1    = fs->make<TH1D>("hbx1",   "bx",   4000,0,4000.);  
   
   hchargebx1 = fs->make<TH1D>( "hchargebx1", "Clu charge", 100, 0.,highH); //in ke
   hchargebx2 = fs->make<TH1D>( "hchargebx2", "Clu charge", 100, 0.,highH); //in ke
   hchargebx3 = fs->make<TH1D>( "hchargebx3", "Clu charge", 100, 0.,highH); //in ke
   hchargebx4 = fs->make<TH1D>( "hchargebx4", "Clu charge", 100, 0.,highH); //in ke
   hchargebx5 = fs->make<TH1D>( "hchargebx5", "Clu charge", 100, 0.,highH); //in ke
   hchargebx6 = fs->make<TH1D>( "hchargebx6", "Clu charge", 100, 0.,highH); //in ke
   sizeH=400; // 600
   highH = 100.0; // charge limit in kelec
   hpixChargebx1 = fs->make<TH1D>( "hpixChargebx1", "Pix charge", sizeH, 0.,highH); //in ke
   hpixChargebx2 = fs->make<TH1D>( "hpixChargebx2", "Pix charge", sizeH, 0.,highH); //in ke
   hpixChargebx3 = fs->make<TH1D>( "hpixChargebx3", "Pix charge", sizeH, 0.,highH); //in ke
   hpixChargebx3 = fs->make<TH1D>( "hpixChargebx4", "Pix charge", sizeH, 0.,highH); //in ke
   hpixChargebx5 = fs->make<TH1D>( "hpixChargebx5", "Pix charge", sizeH, 0.,highH); //in ke
   hpixChargebx6 = fs->make<TH1D>( "hpixChargebx6", "Pix charge", sizeH, 0.,highH); //in ke
   
   sizeH=200;
   highH = 199.5; // charge limit in kelec
   hsizebx1 = fs->make<TH1D>( "hsizebx1", "clu size",sizeH,-0.5,highH);
   hsizebx2 = fs->make<TH1D>( "hsizebx2", "clu size",sizeH,-0.5,highH);
   hsizebx3 = fs->make<TH1D>( "hsizebx3", "clu size",sizeH,-0.5,highH);
   hsizebx5 = fs->make<TH1D>( "hsizebx5", "clu size",sizeH,-0.5,highH);
   hsizebx6 = fs->make<TH1D>( "hsizebx6", "clu size",sizeH,-0.5,highH);
#endif

#ifdef BX_TESTS
   hpix1bx  = fs->make<TProfile>("hpix1bx", "l1 pix vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hclu1bx  = fs->make<TProfile>("hclu1bx", "l1 clu vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hcharClu1bx  = fs->make<TProfile>("hcharClu1bx", "l1 clu charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hcharPix1bx  = fs->make<TProfile>("hcharPix1bx", "l1 pix charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hsizeClu1bx  = fs->make<TProfile>("hsizeClu1bx", "l1 clu size vs bx ",4000,-0.5,3999.5,0.0,1000.);

   hpix2bx  = fs->make<TProfile>("hpix2bx", "l2 pix vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hclu2bx  = fs->make<TProfile>("hclu2bx", "l2 clu vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hcharClu2bx  = fs->make<TProfile>("hcharClu2bx", "l2 clu charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hcharPix2bx  = fs->make<TProfile>("hcharPix2bx", "l2 pix charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hsizeClu2bx  = fs->make<TProfile>("hsizeClu2bx", "l2 clu size vs bx ",4000,-0.5,3999.5,0.0,1000.);

   hpix3bx  = fs->make<TProfile>("hpix3bx", "l3 pix vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hclu3bx  = fs->make<TProfile>("hclu3bx", "l3 clu vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hcharClu3bx  = fs->make<TProfile>("hcharClu3bx", "l3 clu charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hcharPix3bx  = fs->make<TProfile>("hcharPix3bx", "l3 pix charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hsizeClu3bx  = fs->make<TProfile>("hsizeClu3bx", "l3 clu size vs bx ",4000,-0.5,3999.5,0.0,1000.);

   hpix4bx  = fs->make<TProfile>("hpix4bx", "l4 pix vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hclu4bx  = fs->make<TProfile>("hclu4bx", "l4 clu vs bx ",4000,-0.5,3999.5,0.0,1000000.);
   hcharClu4bx  = fs->make<TProfile>("hcharClu4bx", "l4 clu charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hcharPix4bx  = fs->make<TProfile>("hcharPix4bx", "l4 pix charge vs bx ",4000,-0.5,3999.5,0.0,1000.);
   hsizeClu4bx  = fs->make<TProfile>("hsizeClu4bx", "l4 clu size vs bx ",4000,-0.5,3999.5,0.0,1000.);
#endif

#ifdef VDM_STUDIES
   hcharCluls1 = fs->make<TProfile>("hcharCluls1","clu char vs ls",sizeH,0.,highH,0.0,1000.);
   hcharPixls1 = fs->make<TProfile>("hcharPixls1","pix char vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeCluls1 = fs->make<TProfile>("hsizeCluls1","clu size vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeXCluls1= fs->make<TProfile>("hsizeXCluls1","clu size-x vs ls",sizeH,0.,highH,0.0,1000.);
   hcharCluls2 = fs->make<TProfile>("hcharCluls2","clu char vs ls",sizeH,0.,highH,0.0,1000.);
   hcharPixls2 = fs->make<TProfile>("hcharPixls2","pix char vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeCluls2 = fs->make<TProfile>("hsizeCluls2","clu size vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeXCluls2= fs->make<TProfile>("hsizeXCluls2","clu size-x vs ls",sizeH,0.,highH,0.0,1000.);
   hcharCluls3 = fs->make<TProfile>("hcharCluls3","clu char vs ls",sizeH,0.,highH,0.0,1000.);
   hcharPixls3 = fs->make<TProfile>("hcharPixls3","pix char vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeCluls3 = fs->make<TProfile>("hsizeCluls3","clu size vs ls",sizeH,0.,highH,0.0,1000.);
   hsizeXCluls3= fs->make<TProfile>("hsizeXCluls3","clu size-x vs ls",sizeH,0.,highH,0.0,1000.);
   hclusls1 = fs->make<TProfile>("hclusls1","clus vs ls",sizeH,0.,highH,0.0,30000.);
   hpixls1  = fs->make<TProfile>("hpixls1", "pix vs ls ",sizeH,0.,highH,0.0,100000.);
   hclusls2 = fs->make<TProfile>("hclusls2","clus vs ls",sizeH,0.,highH,0.0,30000.);
   hpixls2  = fs->make<TProfile>("hpixls2", "pix vs ls ",sizeH,0.,highH,0.0,100000.);
   hclusls3 = fs->make<TProfile>("hclusls3","clus vs ls",sizeH,0.,highH,0.0,30000.);
   hpixls3  = fs->make<TProfile>("hpixls3", "pix vs ls ",sizeH,0.,highH,0.0,100000.);
#endif // VDM_STUDIES


#ifdef ROC_EFF
  // dets with bad rocs
  hbadMap1 = fs->make<TH2F>("hbadMap1"," ",9,-4.5,4.5,13,-6.5,6.5);
  hbadMap1->SetOption("colz");
  hbadMap2 = fs->make<TH2F>("hbadMap2"," ",9,-4.5,4.5,29,-14.5,14.5);
  hbadMap2->SetOption("colz");
  hbadMap3 = fs->make<TH2F>("hbadMap3"," ",9,-4.5,4.5,45,-22.5,22.5);
  hbadMap3->SetOption("colz");
  hbadMap4 = fs->make<TH2F>("hbadMap4"," ",9,-4.5,4.5,65,-32.5,32.5);
  hbadMap4->SetOption("colz");

  hrocHits1ls = fs->make<TH2F>("hrocHits1ls"," ",1000,0.,3000.,10,0.,10.);
  hrocHits1ls->SetOption("colz");
  hrocHits2ls = fs->make<TH2F>("hrocHits2ls"," ",1000,0.,3000.,10,0.,10.);
  hrocHits2ls->SetOption("colz");
  hrocHits3ls = fs->make<TH2F>("hrocHits3ls"," ",1000,0.,3000.,10,0.,10.);
  hrocHits3ls->SetOption("colz");

  hcountInRoc1 = fs->make<TH1D>("hcountInRoc1","roc 1 count",10000,-0.5,999999.5);
  hcountInRoc2 = fs->make<TH1D>("hcountInRoc2","roc 2 count",10000,-0.5,999999.5);
  hcountInRoc3 = fs->make<TH1D>("hcountInRoc3","roc 3 count",10000,-0.5,999999.5);
  hcountInRoc12 = fs->make<TH1D>("hcountInRoc12","roc 1 count norm",500,-0.5,4.5);
  hcountInRoc22 = fs->make<TH1D>("hcountInRoc22","roc 2 count norm",500,-0.5,4.5);
  hcountInRoc32 = fs->make<TH1D>("hcountInRoc32","roc 3 count norm",500,-0.5,4.5);

//   hmoduleHits1ls = fs->make<TH2F>("hmoduleHits1ls"," ",3000,0.,3000.,3,0.,3.);
//   hmoduleHits1ls->SetOption("colz");
//   hmoduleHits2ls = fs->make<TH2F>("hmoduleHits2ls"," ",3000,0.,3000.,3,0.,3.);
//   hmoduleHits2ls->SetOption("colz");
//   hmoduleHits3ls = fs->make<TH2F>("hmoduleHits3ls"," ",3000,0.,3000.,3,0.,3.);
//   hmoduleHits3ls->SetOption("colz");

#endif // ROC_EFF

#ifdef TEST_GEOM
   hbpixXY = fs->make<TH2F>("hbpixXY","bpix XY",180,-18.,18.,180,-18.,18.);
#endif

#ifdef HF
  hhf = fs->make<TH1D>("hhf"," HF ",2000,0.,20000.);
  hclushf = fs->make<TH2F>("hclushf"," HF versus clusters",100,0.,10000.,100,0.,20000.);
  hclushf->SetOption("colz");
  hclusbhf = fs->make<TH2F>("hclusbhf"," HF versus clusters-b",80,0.,8000.,100,0.,20000.);
  hclusbhf->SetOption("colz");
  hclusfhf = fs->make<TH2F>("hclusfhf"," HF versus clusters-f",30,0.,3000.,100,0.,20000.);
  hclusfhf->SetOption("colz");
  hclusb1hf = fs->make<TH2F>("hclusb1hf"," HF versus clusters-b1",40,0.,4000.,100,0.,20000.);
  hclusb1hf->SetOption("colz");
  //hbxhf = fs->make<TH2F>("hbxhf"," HF versus bx",4000,0.,4000.,50,0.,10000.);
  //hbxhf->SetOption("colz");  // should be a profile 
  hbxhfp = fs->make<TProfile>("hbxhfp"," HF versus bx",4000,0.,4000.,0.,20000.);
#endif


#ifdef LAY1_SPLIT


  sizeH= 400;
  lowH = 0.;
  highH = 400.0; // charge limit in kelec
  //TH1D *hcharge1InR12,*hcharge1InR34,*hcharge1OutR12,*hcharge1OutR34; 
  hcharge1InR12  = fs->make<TH1D>( "hcharge1InR12",  "Clu charge l1 in ring12",  sizeH, 0.,highH); //in ke
  hcharge1InR34  = fs->make<TH1D>( "hcharge1InR34",  "Clu charge l1 in ring34",  sizeH, 0.,highH); //in ke
  hcharge1OutR12 = fs->make<TH1D>( "hcharge1OutR12", "Clu charge l1 out ring12", sizeH, 0.,highH); //in ke
  hcharge1OutR34 = fs->make<TH1D>( "hcharge1OutR34", "Clu charge l1 out ring34", sizeH, 0.,highH); //in ke

  sizeH=  400; // 600
  highH = 100.0; // charge limit in kelec
  //TH1D *hpixcharge1InR12,*hpixcharge1InR34,*hpixcharge1OutR12,*hpixcharge1OutR34; 
  hpixcharge1InR12  = fs->make<TH1D>( "hpixcharge1InR12",  "Pix charge l1 in  ring12",sizeH, 0.,highH);//in ke
  hpixcharge1InR34  = fs->make<TH1D>( "hpixcharge1InR34",  "Pix charge l1 out ring34",sizeH, 0.,highH);//in ke
  hpixcharge1OutR12 = fs->make<TH1D>( "hpixcharge1OutR12", "Pix charge l1 in  ring12",sizeH, 0.,highH);//in ke
  hpixcharge1OutR34 = fs->make<TH1D>( "hpixcharge1OutR34", "Pix charge l1 out ring34",sizeH, 0.,highH);//in ke

  sizeH=300;
  highH = 299.5; // charge limit in kelec
  //TH1D *hsize1InR12,*hsize1InR34,*hsize1OutR12,*hsize1OutR34; 
  //TH1D *hsizex1InR12,*hsizex1InR34,*hsizex1OutR12,*hsizex1OutR34; 
  //TH1D *hsizey1InR12,*hsizey1InR34,*hsizey1OutR12,*hsizey1OutR34; 
  hsize1InR12  = fs->make<TH1D>( "hsize1InR12",  "layer 1 clu size in R12", sizeH,-0.5,highH);
  hsize1InR34  = fs->make<TH1D>( "hsize1InR34",  "layer 1 clu size in R34", sizeH,-0.5,highH);
  hsize1OutR12 = fs->make<TH1D>( "hsize1OutR12", "layer 1 clu size out R12",sizeH,-0.5,highH);
  hsize1OutR34 = fs->make<TH1D>( "hsize1OutR34", "layer 1 clu size out R34",sizeH,-0.5,highH);

  hsizex1InR12   = fs->make<TH1D>( "hsizex1InR12",  "layer 1 clu sizex in R12", 100,-0.5,99.5);
  hsizex1InR34   = fs->make<TH1D>( "hsizex1InR34",  "layer 1 clu sizex in R34", 100,-0.5,99.5);
  hsizex1OutR12  = fs->make<TH1D>( "hsizex1Ou1R12", "layer 1 clu sizex out R12",100,-0.5,99.5);
  hsizex1OutR34  = fs->make<TH1D>( "hsizex1OutR34", "layer 1 clu sizex out R34",100,-0.5,99.5);

  hsizey1InR12  = fs->make<TH1D>( "hsizey1InR12",  "layer 1 clu sizey in R12", 100,-0.5,99.5);
  hsizey1InR34  = fs->make<TH1D>( "hsizey1InR34",  "layer 1 clu sizey in R34", 100,-0.5,99.5);
  hsizey1OutR12 = fs->make<TH1D>( "hsizey1OutR12", "layer 1 clu sizey out R12",100,-0.5,99.5);
  hsizey1OutR34 = fs->make<TH1D>( "hsizey1OutR34", "layer 1 clu sizey out R34",100,-0.5,99.5);
#endif

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

  countEvents=0;
  countAllEvents=0;
  sumClusters=0., sumPixels=0.;
  countLumi = 0.;

#ifdef SINGLE_MODULES
  for(int i=0; i<40;++i) eventFlag[i]=true;
#endif

#ifdef ROC_EFF
  // Analyze single pixel efficiency
  pixEff = new rocEfficiency();
#endif

#ifdef Lumi
  lumiCorrector = new LumiCorrector();
#endif

}
// ------------ method called to at the end of the job  ------------
void PixClusterAna::endJob(){
  double norm = 1, norm1=1, norm2=1, norm3=1,norm4=1;
  cout << " End PixClusterAna, events all/with hits=  " 
       << countAllEvents<<"/"<<countEvents<<endl;

#ifdef ROC_EFF
  double totClusters = sumClusters; // save the total cluster number
#endif

  if(countEvents>0) {
    sumClusters = sumClusters/float(countEvents);
    sumPixels = sumPixels/float(countEvents);
    norm = 1./float(countEvents);
    norm1 = norm/12.; // normalisation for module per ring
    norm2 = norm/28.;
    norm3 = norm/44.;
    norm4 = norm/64.;
  }


  cout <<" clus/pix per full event "<<sumClusters<<"/"<<sumPixels<<endl;

  //countLumi /= 1000.;
  //double c1=0, c2=0;
  //double c3 = hinst->GetMean();
  //if(c3>0.) {c1=sumClusters/c3; c2=sumPixels/c3;}
  //cout<<" Lumi = "<<countLumi<<" still the /10 bug? "<<"clu and pix per lumi unit"<<c1<<" "<<c2<<endl;
  
  if(!Normalise || countEvents==0) return;

  cout<<" 2D plots are rescaled by the number of full events "<<countEvents<<endl;
  //Divide the size histos
  hsizeDets1->Divide(hsizeDets1,hcluDets1,1.,1.);
  hsizeDets2->Divide(hsizeDets2,hcluDets2,1.,1.);
  hsizeDets3->Divide(hsizeDets3,hcluDets3,1.,1.);
  hsizeDets4->Divide(hsizeDets4,hcluDets4,1.,1.);

  // rescale if histos 
  hladder1id->Scale(norm/8.);
  hladder2id->Scale(norm/8.);
  hladder3id->Scale(norm/8.);
  hladder4id->Scale(norm/8.);

  hz1id->Scale(norm1);
  hz2id->Scale(norm2);
  hz3id->Scale(norm3);
  hz4id->Scale(norm4);

  hpladder1id->Scale(norm/8.);
  hpladder2id->Scale(norm/8.);
  hpladder3id->Scale(norm/8.);
  hpladder4id->Scale(norm/8.);

  hpz1id->Scale(norm1);
  hpz2id->Scale(norm2);
  hpz3id->Scale(norm3);
  hpz4id->Scale(norm4);

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
  hDets1->Scale(norm);
  hDets2->Scale(norm);
  hDets3->Scale(norm);
  hDets4->Scale(norm);
  hcluDets1->Scale(norm);
  hcluDets2->Scale(norm);
  hcluDets3->Scale(norm);
  hcluDets4->Scale(norm);
  hpixDets1->Scale(norm);
  hpixDets2->Scale(norm);
  hpixDets3->Scale(norm);
  hpixDets4->Scale(norm);
  hpixDetMap1->Scale(norm);
  hpixDetMap2->Scale(norm);
  hpixDetMap3->Scale(norm);
  hpixDetMap4->Scale(norm);
  hcluDetMap1->Scale(norm);
  hcluDetMap2->Scale(norm);
  hcluDetMap3->Scale(norm);
  hcluDetMap4->Scale(norm);

  hgz1->Scale(norm1); // normalize to modules in a ring
  hgz2->Scale(norm2);
  hgz3->Scale(norm3);
  hgz4->Scale(norm4);

  hpixz1->Scale(norm1);
  hpixz2->Scale(norm2);
  hpixz3->Scale(norm3);
  hpixz4->Scale(norm4);

#ifdef ROC_RATE
  hrocMap1->Scale(norm);
  hrocMap2->Scale(norm);
  hrocMap3->Scale(norm);
  hrocMap4->Scale(norm);

  hpixRocRate1->Scale(norm1/2.); // nptmalize to roc
  hpixRocRate2->Scale(norm2/2.);
  hpixRocRate3->Scale(norm3/2.);
  hpixRocRate4->Scale(norm4/2.);
  hcluRocRate1->Scale(norm1/2.);
  hcluRocRate2->Scale(norm2/2.);
  hcluRocRate3->Scale(norm3/2.);
  hcluRocRate4->Scale(norm4/2.);

  hrocLadder1->Scale(norm/64.); // 8 modules in a ladder, 8 rocs in z per module  
  hrocZ11->Scale(norm1); // only in or outer ladder so 2 times less 
  hrocZ12->Scale(norm1);
#endif

#ifdef TESTING_ADC
  hpixDetsT1->Scale(norm);
  hpixDetMap1->Scale(norm);
#endif

#ifdef ROC_EFF
  // Do this only if there is enough statistics
  double clusPerROC = totClusters/15000.;
  bool doFullAna = true;
  if( clusPerROC < 1000.) {
    cout<<" The average number of clusters per ROC is too low to do the full ROC efficiency analysis "<<clusPerROC<<endl;
    doFullAna=false;
    //return;
  } 

  int deadRocs1 = 0, ineffRocs1=0,deadRocs2 = 0, 
    ineffRocs2=0,deadRocs3 = 0, ineffRocs3=0,
    noisyRocs1=0,noisyRocs2 = 0, noisyRocs3=0;
  
    const int ladders1[20] = {-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8,9,10};  
    const int ladders2[32] = {-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    const int ladders3[44] = {-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,
			      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22};
    const int modules[8] = {-4,-3,-2,-1,1,2,3,4};
    const float effCut = 0.25, noiseCut=5.;
    float half1=1, half2=0;
    
    

  // layer 1
  for(int ilad=0;ilad<20;++ilad) {
    int lad = ladders1[ilad];
    for(int imod=0;imod<8;++imod) {
      int mod = modules[imod];
      half1=0; half2=0;
      float count = pixEff->getModule(1,lad,mod,half1,half2);
      //cout<<" layer 1 "<<lad<<" "<<mod<<" "<<count<<endl;
      if(count<1.) continue;  // skip dead modules 
      for(int roc=0;roc<16;++roc) {
	if     (roc<8 && half1==0) continue;
	else if(roc>7 && half2==0) continue;
        float tmp = pixEff->getRoc(1,lad,mod,roc);
	if(tmp<0.) {  // half-module rocs will show up here
	  if( (abs(lad)==1  || abs(lad)==10) && (roc>7) ) {  // OK half module
	    continue;
	  } else {		
	    cout<<" Layer1, wrong number of hits, roc "<<tmp<<" "<<lad<<" "<<mod<<" "<<roc<<endl;
	  }
	} else if(tmp==0.) {
	  deadRocs1++;
	  if(doFullAna) 
	    cout<<" layer1, dead  roc "<<lad<<" "<<mod<<" "<<roc<<" - "<<count<<" "<<half1<<" "<<half2<<endl;
	} else {
	  hcountInRoc1->Fill(tmp);
          float tmp1 = tmp/count;
          //cout<<" roc "<<roc<<" "<<tmp<<" "<<tmp1<<endl;
          hcountInRoc12->Fill(tmp1);
	  if( (tmp1-1.)>noiseCut ) { // to many hits
	    noisyRocs1++;
	    cout<<"NOISY ROC, Layer 1: "<<tmp1<<"/"<<tmp<<" ladder "<<lad<<" module "<<mod<<" roc(false #) "<<roc<<endl;
	    hbadMap1->Fill(float(mod),float(lad));
 	  } else if( doFullAna && ((1.-tmp1)>effCut )) {
	    ineffRocs1++;
	    cout<<"LOW-EFF ROC, Layer 1: "<<tmp1<<"/"<<tmp<<" ladder "<<lad<<" module "<<mod<<" roc(false #) "<<roc<<endl;
	    hbadMap1->Fill(float(mod),float(lad));
 	  }
        } //if
      } // loop over rocs
    } // mod
  } // lad    

  for(int ilad=0;ilad<32;++ilad) {
    int lad = ladders2[ilad];
    for(int imod=0;imod<8;++imod) {
      int mod = modules[imod];
      half1=0; half2=0;
      float count = pixEff->getModule(2,lad,mod,half1,half2);
      //cout<<" layer 2 "<<lad<<" "<<mod<<" "<<count<<endl;
      if(count<1.) continue; // skip dead whole modules
      for(int roc=0;roc<16;++roc) {
	if     (roc<8 && half1==0) continue;
	else if(roc>7 && half2==0) continue;
        float tmp = pixEff->getRoc(2,lad,mod,roc);
	if(tmp<0.) { 
	  if( (abs(lad)==1  || abs(lad)==16) && (roc>7) ) {  // OK half module
	    continue;
	  } else {
	    cout<<" Layer 2, wrong number of hits, roc "<<tmp<<" "<<lad<<" "<<mod<<" "<<roc<<endl;
	  }
	} else if(tmp==0.) {
	  deadRocs2++;
	  if(doFullAna) 
	    cout<<" layer2, dead  roc "<<lad<<" "<<mod<<" "<<roc<<" - "<<count<<" "<<half1<<" "<<half2<<endl;
	} else {
	  hcountInRoc2->Fill(tmp);
          float tmp1 = tmp/count;
          //cout<<" roc "<<roc<<" "<<tmp<<" "<<tmp1<<endl;
          hcountInRoc22->Fill(tmp1);
	  if( (tmp1-1.)>noiseCut ) {
	    noisyRocs2++;
	    cout<<"NOISY ROC, Layer 2: "<<tmp1<<"/"<<tmp<<" ladder "<<lad<<" module "<<mod<<" roc(false) "<<roc<<endl; 
	    hbadMap2->Fill(float(mod),float(lad));
	  } else if( doFullAna && ((1.-tmp1)>effCut) ) {
	    ineffRocs2++;
	    cout<<"LOW-EFF ROC, Layer 2: "<<tmp1<<"/"<<tmp<<" ladder "<<lad<<" module "<<mod<<" roc(false) "<<roc<<endl; 
	    hbadMap2->Fill(float(mod),float(lad));
	  } 
        } //if
      } // loop over rocs
    } // mod
  } // lad    

  for(int ilad=0;ilad<44;++ilad) {
    int lad = ladders3[ilad];
    for(int imod=0;imod<8;++imod) {
      int mod = modules[imod];
      half1=0; half2=0;
      float count = pixEff->getModule(3,lad,mod,half1,half2);
      //cout<<" layer 3"<<lad<<" "<<mod<<" "<<count<<endl;
      if(count<1.) continue; // skip dead whole modules
      for(int roc=0;roc<16;++roc) {
        float tmp = pixEff->getRoc(3,lad,mod,roc);
	if(tmp<0.) {
	  if( (abs(lad)==1  || abs(lad)==22) && (roc>7) ) {  // OK half module
	    continue;
	  } else {
	    cout<<" Layer 3: wrong number of hits, roc "<<tmp<<" "<<lad<<" "<<mod<<" "<<roc<<endl;
	  }
	} else if(tmp==0.) {
	  deadRocs3++;
	  if(doFullAna) 
	    cout<<" layer3, dead  roc "<<lad<<" "<<mod<<" "<<roc<<" - "<<count<<" "<<half1<<" "<<half2<<endl;
	} else {
	  hcountInRoc3->Fill(tmp);
          float tmp1 = tmp/count;
          //cout<<" roc "<<roc<<" "<<tmp<<" "<<tmp1<<endl;
          hcountInRoc32->Fill(tmp1);
	  if( (tmp1-1.)>noiseCut ) {  // noisy
	    noisyRocs3++;
	    cout<<"NOISY ROC, Layer 3: "<<tmp1<<"/"<<tmp<<" ladder "<<lad<<" module "<<mod<<" roc(false) "<<roc<<endl; 
	    hbadMap3->Fill(float(mod),float(lad));
	  }
	  if( doFullAna && ((1.-tmp1)>effCut )) {
	    ineffRocs3++;
	    cout<<"LOW-EFF ROC, Layer 3: "<<tmp1<<"/"<<tmp<<" ladder "<<lad<<" module "<<mod<<" roc(false) "<<roc<<endl; 
	    hbadMap3->Fill(float(mod),float(lad));
	  }
        } //if
      } // loop over rocs
    } // mod
  } // lad    

  cout<<" Bad Rocs "<<deadRocs1<<" "<<deadRocs2<<" "<<deadRocs3
      <<", Inefficient Rocs "<<ineffRocs1<<" "<<ineffRocs2<<" "<<ineffRocs3<<" "
      <<", Noisy Rocs "<<noisyRocs1<<" "<<noisyRocs2<<" "<<noisyRocs3<<endl;

#endif // ROC_EFF

}
//////////////////////////////////////////////////////////////////
// Functions that gets called by framework every event
void PixClusterAna::analyze(const edm::Event& e, 
			      const edm::EventSetup& es) {
  using namespace edm;
  //const int MAX_CUT = 1000000; unused
  const int selectEvent = -1;
  bool select = false;
  //static int runNumberOld=-1;
  //static int countRuns=0;

  // Get event setup 
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);

  countAllEvents++;
  int run       = e.id().run();
  int event     = e.id().event();

  int lumiBlock = e.luminosityBlock();
  int bx        = e.bunchCrossing();
  int orbit     = e.orbitNumber();

  hbx0->Fill(float(bx));
  hlumi0->Fill(float(lumiBlock));

  //if(lumiBlock<127) return;
  //if(event!=3787937) return;

  //float instlumi=0;
  //int beamint1=0, beamint2=0;

#ifdef Lumi
  float instlumiAv = 0, instlumiBx = 0;

  // Lumi 
  edm::LuminosityBlock const& iLumi = e.getLuminosityBlock();

  edm::Handle<LumiSummary> lumi;
  edm::Handle<LumiDetails> ld;
  //iLumi.getByLabel("lumiProducer", lumi);
  //iLumi.getByLabel("lumiProducer", ld);
  iLumi.getByToken(LumiToken, lumi);
  iLumi.getByToken(LumiToken, ld);

  edm::Handle<edm::ConditionsInLumiBlock> cond;
  //iLumi.getByLabel("conditionsInEdm", cond);
  iLumi.getByToken(ConditionsInLumiBlockToken, cond);

  // This will only work when running on RECO until (if) they fix it in the FW
  // When running on RAW and reconstructing, the LumiSummary will not appear
  // in the event before reaching endLuminosityBlock(). Therefore, it is not
  // possible to get this info in the event
  if (lumi.isValid()) {
    //intlumi =(lumi->intgRecLumi())/1000.; // integrated lumi per LS in -pb
    //instlumi=(lumi->avgInsDelLumi())/1000.; //ave. inst lumi per LS in -pb
    float tmp0 =(lumi->avgInsDelLumi()); //ave. inst lumi per LS in -nb
    //beamint1=(cond->totalIntensityBeam1)/1000;
    //beamint2=(cond->totalIntensityBeam2)/1000;
    const int nbx=1331;  // for 1380 fills 
    float corr = lumiCorrector->TotalNormOcc1( (tmp0/1000.),nbx);
    //float tmp2 = lumiCorrector->TotalNormOcc2(tmp0,nbx);
    //float tmp3 = lumiCorrector->TotalNormET(tmp0,nbx);
    float tmp1 = tmp0 * corr;
    //instlumiAv = tmp1/1000000.;  // in 10^33 
    float tmp2 = tmp1/float(nbx)/1000.;  // per bx
    instlumiAv = tmp2;  // use per bx lumi 

    if( ld.isValid() ) {
      instlumiBx = ld->lumiValue(LumiDetails::kOCC1, bx)*6.37; // cor=6.37 in 2011, 7.13 in 2012?
    }

    //cout<<run<<" "<<lumiBlock<<" "<<tmp0<<" "<<corr<<" "<<tmp1<<" "<<instlumiAv<<" "<<tmp2<<" ";
    //cout<<instlumiBx<<endl;

  } else {
    //std::cout << "** ERROR: Event does not get lumi info\n";
  }

  hinst->Fill(float(instlumiBx));
  //hintg->Fill(float(intlumi));
  hinstls->Fill(float(lumiBlock),float(instlumiAv));
  hinstlsbx->Fill(float(lumiBlock),float(instlumiBx));
  hinstbx->Fill(float(bx),float(instlumiBx));
  //hbeam1->Fill(float(lumiBlock),float(beamint1));
  //hbeam2->Fill(float(lumiBlock),float(beamint2));

  // Use this per bx as int lumi 
  instlumi = instlumiBx;

#endif

#ifdef PV
  // PVs
  int numPVsGood = 0;
  if(useTracks && run>165000) { // skip for earlier runs, crashes
    edm::Handle<reco::VertexCollection> vertices;
    //e.getByLabel( "offlinePrimaryVertices", vertices );
    e.getByToken(VertexCollectionToken, vertices);

    //int numPVs = vertices->size(); // unused 
    if( !vertices.failedToGet() && vertices.isValid() ) {
      for( reco::VertexCollection::const_iterator iVertex = vertices->begin();
	   iVertex != vertices->end(); ++iVertex ) {
	
	if(! iVertex->isValid() ) continue;
	if( iVertex->isFake() ) continue; 
	numPVsGood++;
	
	if( PRINT ){
	  cout << "vertex";
	  cout << ": x " << iVertex->x();
	  cout << ", y " << iVertex->y();
	  cout << ", z " << iVertex->z();
	  cout << ", ndof " << iVertex->ndof();
	  //cout << ", sumpt " << iVertex->p4().pt(); // gonein 71
	  cout << endl;
	} // print 
      } // for loop
    } // if vertex
    
    hpvs->Fill(float(numPVsGood));
    hpvls->Fill(float(lumiBlock),float(numPVsGood));
    //if(instlumi>0.) {
    //float tmp = float(numPVsGood)/instlumi;
      //hpvlsn->Fill(float(lumiBlock),tmp);
    //}
  } // if run

  // Tracks
  int numOfTracks=0;
  if(useTracks) {
    Handle<reco::TrackCollection> recTracks;
    e.getByToken(TrackToken, recTracks);
    numOfTracks = recTracks->size();
    htracks->Fill(float(numOfTracks));
    if(PRINT) cout<<" Tracks "<<numOfTracks<<endl;
  }
#endif // PV 
    

#ifdef BX_NEW
  //cout<<" for bx "<<bx<<endl;
  int bxId = getBX::find(bx);  // get the bunch type 
  //cout<<" id is "<<bxId<<endl;
#endif // BX_NEW

#if defined(BX) || defined(BX_NEW)
  if(bxId==3)      hbx1->Fill(float(bx)); // collision 
  else if(bxId==4) hbx2->Fill(float(bx)); // collision+1
  else if(bxId==1) hbx3->Fill(float(bx)); // beam1
  else if(bxId==2) hbx4->Fill(float(bx)); // beam2
  else if(bxId==5 || bxId==6) hbx5->Fill(float(bx)); // empty gap and in-between
  else if(bxId==0) hbx6->Fill(float(bx)); // empty

#endif

#ifdef HF
  edm::Handle<HFRecHitCollection> HFRecHitsH;
  e.getByToken(HFHitsToken_,HFRecHitsH);
  double sumHFE = 0.;
  for (HFRecHitCollection::const_iterator it=HFRecHitsH->begin(); it!=HFRecHitsH->end(); it++) {
    if (it->energy()>10.0) {   // from eCut_HF
      sumHFE += it->energy();
    }
  }
  hhf->Fill(sumHFE);
  //cout<<" energy sum HF "<<sumHFE<<endl;
#endif // HF


  // Get Cluster Collection from InputTag
  edm::Handle< edmNew::DetSetVector<SiPixelCluster> > clusters;
  //e.getByLabel( src_ , clusters);
  // New By Token method
  e.getByToken( myClus , clusters);


  const edmNew::DetSetVector<SiPixelCluster>& input = *clusters;     
  int numOf = input.size();
  
  //cout<<numOf<<endl;
  //if(numOf<1) return; // skip events with no pixels
  
  //cout<<"run "<<run<<" event "<<event<<" bx "<<bx<<" lumi "<<lumiBlock<<" orbit "<<orbit<<" "
  //  <<numOf<<" lumi "<<instlumi<<endl;  

  if(PRINT) cout<<"run "<<run<<" event "<<event<<" bx "<<bx<<" ls "<<lumiBlock<<" orbit "<<orbit<<" "
		<<numOf<<endl;  

  // For L1 
  //bool bit0=false; // , bit126=false, bit121=false,bit122=false;
  //bool bptx_m=false, bptx_p=false, bptxAnd=false,
  //bptx3=false, bptx4=false, bptx5=false, bptx6=false,bptx7=false;
  //bool  bcsOR=false, bit32_33=false, bit40=false, bit41=false, halo=false, splash1=false, splash2=false;

#ifdef L1
  // Get L1
  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  //e.getByLabel("gtDigis",L1GTRR);
  e.getByToken(L1TrigReadoutToken,L1GTRR);

  if (L1GTRR.isValid()) {
    //bool l1a = L1GTRR->decision();  // global decission?  unused
    const L1GtPsbWord psb = L1GTRR->gtPsbWord(0xbb09, 0); // select PSB#9 and bunch crossing 0
    int techLowNonPres  = 65536*psb.bData(0) + psb.aData(0);
    int techHighNonPres = 65536*psb.bData(1) + psb.aData(1);

    //cout << hex << "high word: " << ( 65536*psb.bData(1) + psb.aData(1) )
    // << " low word: " << ( 65536*psb.bData(0) + psb.aData(0) ) << " " 
    // <<techLowNonPres<<" "<<techHighNonPres<<dec<<endl;

    //cout<<" L1 status = "<<l1a<<" : ";
    for (unsigned int i = 0; i < L1GTRR->decisionWord().size(); ++i) {
      int l1flag = L1GTRR->decisionWord()[i]; 
      int t1flag = L1GTRR->technicalTriggerWord()[i]; 
      int techflag = 0;
      if(i<32) techflag = ( techLowNonPres & (0x1<<i) );
      else if(i<64) techflag = ( techHighNonPres & (0x1<<i) );

      if( l1flag>0 ) {  // look at L1A algoritmic bits
	//cout<<" la "<<i<<" ";
	hl1a->Fill(float(i));
	//if(i==0) bit0=true;
	// 3-pre, 4-bg_bsc, 5-bg_hf, 6-inter_bsc, 7-inter_hf, 8-halo
	//else if(i==121) bit121=true;
	//else if(i==122) bit122=true;
	//else if(i==126) bit126=true; //bscOR and bptx
	
      }
      if( techflag>0 && i<64) { // look at nonprescaled technical bits
	//cout<<" t1 "<<i<<" ";
	hl1t->Fill(float(i));  // non- prescaled 
	//if(i==0) bptxAnd=true;  // bptxAnd
	//else if(i==1) bptx_p=true;
	//else if(i==2) bptx_m=true;
	//else if(i==3) bptx3=true; // OR
	//else if(i==4) bptx4=true; // AND
	//else if(i==5) bptx5=true; // p not m
	//else if(i==6) bptx6=true; // m not p
	//else if(i==7) bptx7=true; // quite
	//else if(i==9) hf=true;
	//else if(i==10) hf=true;
      }
      if( t1flag>0 && i<64) { // look at prescaled L1T technical bits
	//cout<<" lt "<<i<<" ";
	hltt->Fill(float(i)); // prescaled 
      }


    } // for loop
    //cout<<dec<<endl;

  } // if l1a
#endif

  //bool bptx_and = bptx_m && bptx_p;
  //bool bptx_or  = bptx_m || bptx_p;
  //bool bptx_xor = bptx_or && !bptx_and;

  //---------------------------------------
  // Analyse HLT
  //bool passHLT1=false,passHLT2=false,passHLT3=false,passHLT4=false,passHLT5=false;
#ifdef HLT
  const int hltSize = 1024;
  bool hlt[hltSize];
  for(int i=0;i<hltSize;++i) hlt[i]=false;

  edm::TriggerNames TrigNames;
  edm::Handle<edm::TriggerResults> HLTResults;
  // Extract the HLT results
  //e.getByLabel(edm::InputTag("TriggerResults","","HLT"),HLTResults);
  e.getByToken(TrigResultsToken, HLTResults);

  if ((HLTResults.isValid() == true) && (HLTResults->size() > 0)) {
    //TrigNames.init(*HLTResults);
    const edm::TriggerNames & TrigNames = e.triggerNames(*HLTResults);

    if(TrigNames.triggerNames().size() > hltSize) cout<<" extend the hlt array above "<<hltSize<<endl;

    for (unsigned int i = 0; i < TrigNames.triggerNames().size(); i++) {  // loop over trigger
      if(countAllEvents==1) cout<<i<<" "<<TrigNames.triggerName(i)<<endl;

      if ( 
	   (HLTResults->wasrun(TrigNames.triggerIndex(TrigNames.triggerName(i))) == true) &&
	   (HLTResults->accept(TrigNames.triggerIndex(TrigNames.triggerName(i))) == true) &&
	   (HLTResults->error( TrigNames.triggerIndex(TrigNames.triggerName(i))) == false) ) {

	hlt[i]=true;
	hlt1->Fill(float(i));

// 	if (      TrigNames.triggerName(i) == "HLT_L1Tech_BSC_minBias") passHLT1=true;
// 	else if  (TrigNames.triggerName(i) == "HLT_L1_ZeroBias") passHLT5=true;
//	  passHLT5=true;

      } // if hlt

    } // loop 
  } // if valid
#endif // HLT


#ifdef USE_RESYNCS
  Handle<Level1TriggerScalersCollection> l1ts;
  e.getByLabel("scalersRawToDigi",l1ts);

  if(l1ts->size()>0) {
    int r1 = (*l1ts)[0].lastResync();
    // int r2 = (*l1ts)[0].lastOrbitCounter0();
    // int r3 = (*l1ts)[0].lastEventCounter0();
    // int r4 = (*l1ts)[0].lastHardReset();
    // int r5 = (*l1ts)[0].eventID();
    // int r6 = (*l1ts)[0].bunchNumber();
    // int r7 = (*l1ts)[0].lumiSegmentNr();
    // int r8 = (*l1ts)[0].lumiSegmentOrbits();
    // int r9 = (*l1ts)[0].orbitNr();
    // int r10 = (*l1ts)[0].lastStart();
 
    float t1 = r1/2.621E5;  // orbit to LS
    float t2 = (orbit-r1)*88.924E-6; // orbit diff (event-resync) to time in sec 

    //cout<<"run "<<run<<" event "<<event<<" bx "<<bx<<" lumi "<<lumiBlock<<" orbit "<<orbit<<" "
    //<<numOf<<" lumi "<<instlumi<<" - ";  
    //cout <<t1<<" "<<t2<<" "<<r1 <<" "<<r2 <<" "<<r3 <<" "<<r4 <<" "<<r5 <<" "<<r6 <<" "<<r7 <<" "<<r8 <<" "<<r9 <<" "<<r10<<endl;

    htest1->Fill(t1);
    htest2->Fill(t2);
    htest3->Fill(float(r1));
    htest4->Fill(float(orbit));
  }

#endif

  //----------------------------------------------
  hdets->Fill(float(numOf)); // number of modules with pix

  // Select events if select1>0
  if(select1>0) {
    // skip events with few pixel dets
    if(select1==1) { if(numOf<select2) return; } 
    // select events only for a defined bx
    else if(select1==2) { if(bx != select2) return; } 
    else if(select1==3) { if(bx == select2) return; } 
    //else if(select1==4) { if(  !( (bx==39)||(bx==201)||(bx==443)||(bx==499)||(bx==1083)||(bx==1337)||(bx==1492)||(bx==1977)||(bx==2231)||(bx==2287)||(bx==2871)||(bx==3224)||(bx==3280) )   ) return; } 
    //else if(select1==5) { if( ( (bx==1)||(bx==39)||(bx==201)||(bx==443)||(bx==499)||(bx==1083)||(bx==1337)||(bx==1492)||(bx==1977)||(bx==2231)||(bx==2287)||(bx==2871)||(bx==3224)||(bx==3280) )   ) return; } 
    //else if(select1==6) { if(bx < select2) return; } 
    //else if(select1==7) { if(bx > select2) return; } 
    // select specific event
#ifdef PV
    else if(select1==100) { if(numOfTracks<1) return; } // with tracks 
    else if(select1==101) { if(numOfTracks>0) return; } // no tracks
    else if(select1==102) { if(numPVsGood<1) return; } // with pvs 
    else if(select1==103) { if(numPVsGood>0) return; } // no pvs
#endif
    else if(select1== 201) {goto label1;} // select specific module 
    else if(select1==9999) { if(event!=select2) return; } 
    else { 
    // skip bx outside the select1-select2 region 
    //....
      //if( !((bx>=select1) && (bx<=select2)) ) return; 
    } 
  }

 label1:
  hevent->Fill(float(event));
  hlumi->Fill(float(lumiBlock));
  hbx->Fill(float(bx));
  //horbit->Fill(float(orbit));
#ifdef HLT
  for (unsigned int i=0;i<256;i++) if(hlt[i]==true) hlt2->Fill(float(i));
#endif

  // if(run!=runNumberOld) {
  //   runNumberOld=run;
  //   countRuns++;
  // }
  // switch(countRuns) {
  // case 1: {hlumi10->Fill(float(lumiBlock)); break;}
  //   //case 2: {hlumi11->Fill(float(lumiBlock)); break;}
  //   //case 3: {hlumi12->Fill(float(lumiBlock)); break;}
  //   //case 4: {hlumi13->Fill(float(lumiBlock)); break;}
  //   //case 5: {hlumi14->Fill(float(lumiBlock)); break;}
  // //case 6: {hlumi15->Fill(float(lumiBlock)); break;}
  // //case 7: {hlumi16->Fill(float(lumiBlock)); break;}
  // //case 8: {hlumi17->Fill(float(lumiBlock)); break;}
  // //case 9: {hlumi18->Fill(float(lumiBlock)); break;}
  // //case 10: {hlumi19->Fill(float(lumiBlock)); break;}
  // default: cout<<" too many runs "<<countRuns<<endl;
  // }

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoH;
  es.get<TrackerTopologyRcd>().get(tTopoH);
  const TrackerTopology *tTopo=tTopoH.product();

  //---------------------------------------
  if(numOf>0) countEvents++; // count events with pixel hits 
  int numberOfDetUnits = 0;
  int numberOfClusters = 0;
  int numberOfPixels = 0;
  int numberOfNoneEdgePixels = 0;
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

  int numOfPixPerLay1=0;     
  int numOfPixPerLay2=0;     
  int numOfPixPerLay3=0;     
  int numOfPixPerLay4=0;     

  int numOfPixPerDet1=0;  
  int numOfPixPerDet2=0;  
  int numOfPixPerDet3=0;  
  int numOfPixPerDet4=0;  
      
  //int numOfPixPerLink11=0;  
  //int numOfPixPerLink12=0;  
  //int numOfPixPerLink21=0;  
  //int numOfPixPerLink22=0;  
  //int numOfPixPerLink3=0;  

  int maxClusPerDet=0;
  int maxPixPerDet=0;
  unsigned int maxPixPerClu=0;

  int numOfClustersPerDisk1=0;  
  int numOfClustersPerDisk2=0;  
  int numOfClustersPerDisk3=0;  
  int numOfClustersPerDisk4=0;  
  int numOfClustersPerDisk5=0;  
  int numOfClustersPerDisk6=0;  
  int numOfPixPerDisk1=0;  
  int numOfPixPerDisk2=0;  
  int numOfPixPerDisk3=0;  
  int numOfPixPerDisk4=0;  
  int numOfPixPerDisk5=0;  
  int numOfPixPerDisk6=0;  

  float avCharge1=0.,avCharge2=0.,avCharge3=0.,avCharge4=0.,avCharge5=0.;
        
  //static int module1[416][160] = {{0}};
  //static int module2[416][160] = {{0}};
  //static int module3[416][160] = {{0}};

  
  // get vector of detunit ids
  //--- Loop over detunits.
  edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=input.begin();
  for ( ; DSViter != input.end() ; DSViter++) {
    //bool valid = false;
    unsigned int detid = DSViter->detId();
    // Det id
    DetId detId = DetId(detid);       // Get the Detid object
    unsigned int detType=detId.det(); // det type, pixel=1
    unsigned int subid=detId.subdetId(); //subdetector type, barrel=1
 
    if(PRINT)
      cout<<"Det: "<<detId.rawId()<<" "<<detId.null()<<" "<<detType<<" "<<subid<<endl;    

#ifdef NEW_MODULES
    // Find new modules installed in 2014
    bool newModule=false;
    for(vector<unsigned int>::iterator viter=l2New.begin();viter!=l2New.end();++viter) {
      if(*viter == detid) { newModule=true; break;}
    }
    if(!newModule)
      for(vector<unsigned int>::iterator viter=l3New.begin();viter!=l3New.end();++viter) {
	if(*viter == detid) { newModule=true; break;}
      }
    if(!newModule)
      for(vector<unsigned int>::iterator viter=l1New.begin();viter!=l1New.end();++viter) {
	if(*viter == detid) { newModule=true; break;}
      }
    //if(newModule) cout<<" Found new module "<<detid<<endl;
#endif



#ifdef HISTOS
    //hdetunit->Fill(float(detid));
    //hpixid->Fill(float(detType));
    //hpixsubid->Fill(float(subid));
#endif // HISTOS

    if(detType!=1) continue; // look only at pixels
    ++numberOfDetUnits;
  
    //const GeomDetUnit * genericDet = geom->idToDet(detId);
    //const PixelGeomDetUnit * pixDet = 
    //dynamic_cast<const PixelGeomDetUnit*>(genericDet);

    // Get the geom-detector
    const PixelGeomDetUnit * theGeomDet =
      dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
    double detZ = theGeomDet->surface().position().z();
    double detR = theGeomDet->surface().position().perp();

    //const BoundPlane& plane = theGeomDet->surface(); //for transf.
    
    //double detThick = theGeomDet->specificSurface().bounds().thickness();
    //int cols = theGeomDet->specificTopology().ncolumns();
    //int rows = theGeomDet->specificTopology().nrows();
    
    const PixelTopology * topol = &(theGeomDet->specificTopology());
#ifdef TESTING_ADC            
      bool detWithNegativeADC = false;
#endif

    // barrel ids
    unsigned int layerC=0;
    unsigned int ladderC=0;
    unsigned int zindex=0;
    int shell  = 0; // shell id // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
    int sector = 0; // 1-8
    int ladder = 0; // 1-22
    int layer  = 0; // 1-3
    int module = 0; // 1-4
    bool half  = false; // 
    bool goodL2Modules = false;
    bool badL2Modules = false;
    bool newL1Modules = false;


    // Endcap ids
    unsigned int disk=0; //1,2,3
    unsigned int blade=0; //1-24
    unsigned int zindexF=0; //
    unsigned int side=0; //size=1 for -z, 2 for +z
    unsigned int panel=0; //panel=1
#ifdef LAY1_SPLIT
    bool inner = false; // inner and outer ladders
    bool ring12=false; // rings 1&2
#endif
    edmNew::DetSet<SiPixelCluster>::const_iterator clustIt;

    // Subdet id, pix barrel=1, forward=2
    if(subid==2) {  // forward

      disk=tTopo->pxfDisk(detid); //1,2,3
      blade=tTopo->pxfBlade(detid); //1-24
      zindex=tTopo->pxfModule(detid); //
      side=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
      panel=tTopo->pxfPanel(detid); //panel=1
      PixelEndcapName pen(detid,tTopo,phase1_);

      if(PRINT) cout<<" forward det, disk "<<disk<<", blade "
 		    <<blade<<", module "<<zindexF<<", side "<<side<<", panel "
 		    <<panel<<" pos = "<<detZ<<" "<<detR<<endl;
 

    } else if (subid==1) {  // barrel

#ifdef TEST_DCOLS
      for(int i=0;i<160;++i) for(int j=0;j<416;++j) moduleArray[i][j]=0;
#endif

      layerC=tTopo->pxbLayer(detid);
      ladderC=tTopo->pxbLadder(detid);
      zindex=tTopo->pxbModule(detid);
      //PixelBarrelName pbn(detid);
      PixelBarrelName pbn(detid,tTopo,phase1_);

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

      if( layer==2 && select1!=9998 ) {
	if( (ladder ==-1) && ( (module == 1) || (module == 2) || (module == 3)) ) badL2Modules=true;
	else if( (ladder ==-5) &&( (module == -1) || (module == -2) || (module == -3)) ) badL2Modules=true;
	else if( (ladder == 14) && (module == -1) ) badL2Modules=true;
	else if( (ladder == 13) && (module == -4) ) badL2Modules=true;
	else if( (ladder == 12) && (module == -1) ) badL2Modules=true;
	else if( (ladder == 11) && (module == -4) ) badL2Modules=true;

	else if( (ladder == 1) &&( (module == 1) || (module == 2) || (module == 3))) goodL2Modules=true;
	else if( (ladder ==-1) &&( (module ==-1) || (module ==-2) || (module ==-3))) goodL2Modules=true;
	else if( (ladder ==-8) &&( (module ==-1) || (module ==-2) || (module ==-3))) goodL2Modules=true;
	else if( (ladder == 7) && (module == 1) ) goodL2Modules=true;
	else if( (ladder == 6) && (module == 4) ) goodL2Modules=true;
	else if( (ladder ==-5) && (module ==-3) ) goodL2Modules=true;	
      }
      
      // find inner and outer modules for layer 1 onl
      if( (layer==1) ) {
#ifdef LAY1_SPLIT
	if( (ladder==2) || (ladder==4) || (ladder==6) ||
	    (ladder==-1) || (ladder==-3) || (ladder==-5) ) inner=true;
	else inner=false;
#endif
	if( (select1!=9998) ) {
	  if     ( (ladder ==-1) && (module == 3) ) newL1Modules=true;
	  else if( (ladder ==-3) && (module == 3) ) newL1Modules=true;
	  else if( (ladder ==-1) && (module ==-3) ) newL1Modules=true;
	  else if( (ladder ==-1) && (module ==-1) ) newL1Modules=true;
	  else if( (ladder ==-3) && (module ==-1) ) newL1Modules=true;
	  else if( (ladder ==-5) && (module ==-1) ) newL1Modules=true;
	} // seperate new modules 
      }
      
      // find rings 1-2 and 3-4
#ifdef LAY1_SPLIT
      if(abs(module)<3) ring12=true; 
#endif
      if(PRINT) { 
	cout<<" Barrel layer, ladder, module "
	    <<layerC<<" "<<ladderC<<" "<<zindex<<" "
	    <<sh<<"("<<shell<<") "<<sector<<" "<<layer<<" "<<ladder<<" "
	    <<module<<" "<<half<< " "<<newL1Modules<<endl;
	//cout<<" Barrel det, thick "<<detThick<<" "
	//  <<" layer, ladder, module "
	//  <<layer<<" "<<ladder<<" "<<zindex<<endl;
	//cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
	//  <<pitchX<<" "<<pitchY<<endl;
      }      
      
    } // if subid

    // select for special printout 
    //select = (abs(module)==4)  && layer==1;
    if(select) cout<<" run "<<run<<" event "<<event<<" bx "<<bx<< " det "<<ladder<<" "<<module<<endl;

    if(select1==201) { // select a specific module 
      if( (layer!=selectLayer) || (ladder!=selectLadder) || (module!=selectModule) ) continue; // skip module 
    }

    if(PRINT) {
      cout<<"List clusters : "<<endl;
      cout<<"Num Charge Size SizeX SizeY X Y Xmin Xmax Ymin Ymax Edge"
	  <<endl;
    }

    // Loop over clusters
    for (clustIt = DSViter->begin(); clustIt != DSViter->end(); clustIt++) {
      sumClusters++;
      numberOfClusters++;
      float ch = float(clustIt->charge())/1000.; // convert ke to electrons
      int size = clustIt->size();
      int sizeX = clustIt->sizeX(); //x=row=rfi, 
      int sizeY = clustIt->sizeY(); //y=col=z_global
      float x = clustIt->x(); // row, cluster position in pitch units, as float (int+0.5)
      float y = clustIt->y(); // col, analog average
      // Returns int index of the cluster min/max  
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

      bool edgeHitX2 = false; // edge method moved 
      bool edgeHitY2 = false; // to topologu class
#ifdef TESTING_ADC            
      bool cluWithNegativeADC = false;
#endif
      if(PRINT || select) cout<<numberOfClusters<<" "<<ch<<" "<<size<<" "<<sizeX<<" "<<sizeY<<" "
		    <<x<<" "<<y<<" "<<minPixelRow<<" "<<maxPixelRow<<" "<<minPixelCol<<" "
		    <<maxPixelCol<<" "<<edgeHitX<<" "<<edgeHitY<<endl;

      // get global z position of teh cluster
      LocalPoint lp = topol->localPosition(MeasurementPoint(x,y));
      float lx = lp.x(); // local cluster position in cm
      float ly = lp.y();

      GlobalPoint clustgp = theGeomDet->surface().toGlobal( lp );
      double gZ = clustgp.z();  // global z
      double gX = clustgp.x();
      double gY = clustgp.y();      
      TVector3 v(gX,gY,gZ);
#ifdef PHI_PROFILES
      float gPhi = v.Phi(); // phi of the hit
#endif
      float gR = v.Perp(); // r of the hit

      //float zPos = detZ - ly;
      float zPos = gZ;
      //float rPos = detR + lx;
      float rPos = gR;

      //#ifdef PHI_PROFILES
      //#endif
      
      // Get the pixels in the Cluster
      const vector<SiPixelCluster::Pixel>& pixelsVec = clustIt->pixels();
      if(PRINT) cout<<" Pixels in this cluster (i/x/y/char)"<<endl;
      //bool bigInX=false, bigInY=false;
      // Look at pixels in this cluster. ADC is calibrated, in electrons
      bool edgeInX = false; // edge method moved 
      bool edgeInY = false; // to topologu class
      //bool cluBigInX = false; // does this clu include a big pixel
      //bool cluBigInY = false; // does this clu include a big pixel
      //int noisy = 0;

#ifdef ROC_RATE
      int roc = -1, link = -1, rocInCol = -1;
      float rocZ=-1., rocPhi=-1.;
#endif

      if(pixelsVec.size()>maxPixPerClu) maxPixPerClu = pixelsVec.size();
      float adcMin=9999., adcMax=0.;
      for (unsigned int i = 0;  i < pixelsVec.size(); ++i) { // loop over pixels
	//bool isBig=false;
	sumPixels++;
	numberOfPixels++;
	float pixx = pixelsVec[i].x; // index as float=iteger, row index
	float pixy = pixelsVec[i].y; // same, col index
	float intADC = pixelsVec[i].adc;
	float adc = (float(intADC)/1000.);

#ifdef TESTING_ADC
	bool negativeADC = (intADC<=100); // signals that there was negative charge 
	if(negativeADC) {cluWithNegativeADC= true; detWithNegativeADC= true;}
#endif
	if(adc<adcMin) adcMin=adc;
	if(adc>adcMax) adcMax=adc;

#ifdef ROC_RATE
	roc = rocId(int(pixy),int(pixx));  // 0-15, column, row
	link = int(roc/8); // link 0 & 1
	rocInCol = roc%8; // 0-7
	//rocZ   = float(module) - 0.5 + (0.125/2.) + (float(rocInCol) * 0.125); //z
	//rocZ   = float(module) + 0.5 + (0.125/2.) - (float(rocInCol) * 0.125); //z
	if(module>0) rocZ = float(module) + (0.125/2.) - (float(rocInCol) * 0.125); //z
	else         rocZ = float(module) + 1.0 + (0.125/2.) - (float(rocInCol) * 0.125); //z
	rocPhi = float(ladder) - 0.5 + (0.5/2.)   + (float(link) * 0.5); 
#endif

	//int chan = PixelChannelIdentifier::pixelToChannel(int(pixx),int(pixy));
	bool bigInX = topol->isItBigPixelInX(int(pixx));
	bool bigInY = topol->isItBigPixelInY(int(pixy));
	if( !(bigInX || bigInY) ) {numberOfNoneEdgePixels++;}
	//else {isBig=true;}

#ifdef HISTOS
	// Pixel histos
	if (subid==1 && (selectEvent==-1 || countEvents==selectEvent)) {  // barrel
#ifdef TEST_DCOLS
	  moduleArray[int(pixx)][int(pixy)]=1;
#endif	  
	  if(layer==1) {

	    //cout<<" module "<<layer<<" "<<ladder<<" "<<module<<" "
	    //	<<pixx<<" "<<pixy<<endl;

	    numOfPixPerDet1++;
	    numOfPixPerLay1++;     
	    if(newL1Modules) {
	      hpixcharge1n->Fill(adc);
	    } else {
	      hpixcharge1->Fill(adc);
	      hpixcharge1_zoom->Fill(adc);
	    }
 	    htest2->Fill(adc,float(size));

	    hpixDetMap1->Fill(pixy,pixx);
	    hpixDets1->Fill(float(module),float(ladder));
	    hpixchargeDets1->Fill(float(module),float(ladder),adc);

	    hpladder1id->Fill(float(ladder));
	    hpz1id->Fill(float(module));

#ifdef ROC_RATE
	    hrocMap1->Fill(rocZ,rocPhi);
	    hrocPixcharMap1->Fill(rocZ,rocPhi,adc);
	    hrocLadder1->Fill(rocPhi);
	    hpixRocRate1->Fill(rocZ);
#endif
	    hpcols1->Fill(pixy);
	    hprows1->Fill(pixx);
	    hpixz1->Fill(gZ);
	    hpixchar1->Fill(zPos,adc);
	    //hcharPixbx->Fill(bx,adc);
	    //hcharPixls->Fill(lumiBlock,adc);
#ifdef STUDY_ONEMOD
	    hpixcharMap1->Fill(pixy,pixx,adc);
#endif
	
#ifdef LAY1_SPLIT    
	    if(inner) {
	      hpixcharge11->Fill(adc); hrocZ11->Fill(rocZ);
	      if(ring12) hpixcharge1InR12->Fill(adc);
	      else       hpixcharge1InR34->Fill(adc);
	    }  else { // out
	      hpixcharge12->Fill(adc); hrocZ12->Fill(rocZ);
	      if(ring12) hpixcharge1OutR12->Fill(adc);
	      else       hpixcharge1OutR34->Fill(adc);
	    }  
#endif

#ifdef PHI_PROFILES
	    hpixcharPhi1->Fill(gPhi,adc);
#endif 

#ifdef TESTING_ADC
	    if(negativeADC)  {
	      hpixDetsT1->Fill(float(module),float(ladder));
	      hpixDetMapT1->Fill(pixy,pixx);
	    }
#endif

#ifdef STUDY_LAY1
	    if(size==1) {
	      hpixcharge111->Fill(adc);
	    } else if ((size>=2) && (size<=4)) {
	      if((sizeX==2) && (sizeY==1)) {
		hpixcharge121->Fill(adc);
	      } else if((sizeX==2) && (sizeY==2)) {
		hpixcharge122->Fill(adc);
		if( (minPixelCol%2)==0 ) hpixcharge12_1->Fill(adc);  //1st pixel in 1st col of dcol  
		else                     hpixcharge12_2->Fill(adc);  //1st pixel in 2nd col of dcol  
	      } else if((sizeX==1) && (sizeY==2)) {
		hpixcharge112->Fill(adc);
		if( (minPixelCol%2)==0 ) hpixcharge12_1->Fill(adc);  //1st pixel in 1st col of dcol  
		else                     hpixcharge12_2->Fill(adc);  //1st pixel in 2nd col of dcol  
	      }
	    } // size=1/2
#endif

#ifdef SINGLE_MODULES
	    float weight = 1.; // adc
	    // if     ( ladder==-6 && module==-1) hpixDetMap10->Fill(pixy,pixx,weight); // 
	    // else if( ladder==-1 && module==-1) hpixDetMap11->Fill(pixy,pixx,weight); // "
	    // else if( ladder==-1 && module==-2) hpixDetMap12->Fill(pixy,pixx,weight); // "
	    // else if( ladder==-3 && module==-4) hpixDetMap13->Fill(pixy,pixx,weight); // 
	    // else if( ladder== 4 && module==-2) hpixDetMap14->Fill(pixy,pixx,weight); // 
	    // else if( ladder== 6 && module==-1) hpixDetMap15->Fill(pixy,pixx,weight); // 
	    // else if( ladder== 4 && module== 1) hpixDetMap16->Fill(pixy,pixx,weight); //
	    // else if( ladder== 2 && module== 1) hpixDetMap17->Fill(pixy,pixx,weight); // 
	    // else if( ladder==-4 && module== 2) hpixDetMap18->Fill(pixy,pixx,weight); // 
	    // else if( ladder==-1 && module== 2) hpixDetMap19->Fill(pixy,pixx,weight); // 
#endif

#ifdef LS_TESTS
	   hcharPix1ls->Fill(lumiBlock,adc);
#endif

#ifdef BX_TESTS
	   hcharPix1bx->Fill(bx,adc);
#endif

#ifdef VDM_STUDIES
	    hcharPixls1->Fill(lumiBlock,adc);
#endif

#if defined(BX) || defined(BX_NEW)
	   if(bxId>-1) {
	     if(bxId==3)      hpixChargebx1->Fill(adc); // coll
	     else if(bxId==4) hpixChargebx2->Fill(adc); // coll+1
	     else if(bxId==0) hpixChargebx6->Fill(adc); // empty
	     else if(bxId==1) hpixChargebx3->Fill(adc); // beam1
	     else if(bxId==2) hpixChargebx3->Fill(adc); // beam2
	     else if(bxId==5 || bxId==6) hpixChargebx5->Fill(adc); // gap and in-between 
	     else cout<<" wrong bx id "<<bxId<<endl;
	   }
#endif


#ifdef ROC_EFF
	   pixEff->addPixel(1,ladder,module,roc);  // count pixels
	   
	   //index = lumiBlock/5;
// 	   if     (ladder== 3 && module== 3)  hmoduleHits1ls->Fill(float(lumiBlock),0);
// 	   if     (ladder==-9 && module==-3 && roc== 9)  hrocHits1ls->Fill(float(lumiBlock),0);
// 	   else if(ladder==-9 && module==-1 && roc== 0)  hrocHits1ls->Fill(float(lumiBlock),1);
//	   else if(ladder==-8 && module==-1 && roc== 2)  hrocHits1ls->Fill(float(lumiBlock),2);
// 	   else if(ladder==-8 && module== 1 && roc== 2)  hrocHits1ls->Fill(float(lumiBlock),3);
// 	   else if(ladder==-4 && module==-2 && roc== 2)  hrocHits1ls->Fill(float(lumiBlock),4);
// 	   else if(ladder== 2 && module== 2 && roc== 4)  hrocHits1ls->Fill(float(lumiBlock),5);
// 	   else if(ladder== 3 && module==-2 && roc== 4)  hrocHits1ls->Fill(float(lumiBlock),6);
// 	   else if(ladder== 3 && module== 1 && roc== 6)  hrocHits1ls->Fill(float(lumiBlock),7);
// 	   else if(ladder== 4 && module== 3 && roc== 8)  hrocHits1ls->Fill(float(lumiBlock),8);
// 	   else if(ladder== 8 && module==-4 && roc==11)  hrocHits1ls->Fill(float(lumiBlock),9);
	   // run 176286
	   //if     (ladder==-3 && module== 1 && roc== 5)  hrocHits1ls->Fill(float(lumiBlock),0);
	   //else if(ladder== 2 && module==-1 && roc==13)  hrocHits1ls->Fill(float(lumiBlock),1);
	   //else if(ladder== 3 && module== 4 && roc== 9)  hrocHits1ls->Fill(float(lumiBlock),2);
	   //else if(ladder== 5 && module== 2 && roc== 3)  hrocHits1ls->Fill(float(lumiBlock),3);
	   //else if(ladder== 7 && module==-1 && roc== 8)  hrocHits1ls->Fill(float(lumiBlock),4);
	   //else if(ladder== 2 && module== 1 && roc== 0)  hrocHits1ls->Fill(float(lumiBlock),5);
	   // run 180250
	   // if     (ladder==-9 && module== 4 && roc== 12) hrocHits1ls->Fill(float(lumiBlock),0);
	   // else if(ladder==-7 && module==-4 && roc==3)   hrocHits1ls->Fill(float(lumiBlock),1);
	   // else if(ladder==-5 && module==-4 && roc==11)  hrocHits1ls->Fill(float(lumiBlock),2);
	   // else if(ladder== 2 && module==-2 && roc== 1)  hrocHits1ls->Fill(float(lumiBlock),3);
	   // else if(ladder== 8 && module== 2 && roc== 1)  hrocHits1ls->Fill(float(lumiBlock),4);
	   //else if(ladder== 2 && module== 1 && roc== 0)  hrocHits1ls->Fill(float(lumiBlock),5);
#endif
	    
	  } else if(layer==2) {

	    numOfPixPerDet2++;
	    numOfPixPerLay2++;   
	    //if(ladder==1 && module>0) {hpixcharge8->Fill(adc);}
	    hpixDets2->Fill(float(module),float(ladder));
	    hpixchargeDets2->Fill(float(module),float(ladder),adc);
	    hpladder2id->Fill(float(ladder));
	    hpz2id->Fill(float(module));
	    hpixz2->Fill(gZ);
 	    hpixchar2->Fill(zPos,adc);
	    
	    if(badL2Modules ) {
	      hpixcharge2b->Fill(adc);

	    } else if(goodL2Modules) {
	      hpixcharge2g->Fill(adc);

	    } else {
	      hpixcharge2->Fill(adc);
	      htest4->Fill(adc,float(size));
	      hpcols2->Fill(pixy);
	      hprows2->Fill(pixx);
	      hpixDetMap2->Fill(pixy,pixx);
#ifdef ROC_RATE
	      hrocMap2->Fill(rocZ,rocPhi);
	      hrocPixcharMap2->Fill(rocZ,rocPhi,adc);
	      hpixRocRate2->Fill(rocZ);
#endif	      
#ifdef PHI_PROFILES
	      hpixcharPhi2->Fill(gPhi,adc);
#endif 	      
#ifdef LS_TESTS
	      hcharPix2ls->Fill(lumiBlock,adc);
#endif	      
#ifdef BX_TESTS
	      hcharPix2bx->Fill(bx,adc);
#endif
	    }

#ifdef SINGLE_MODULES
	   if     (ladder== -1 && module== 1) hpixDetMap20->Fill(pixy,pixx); // dcdc 
	   else if(ladder== -1 && module== 2) hpixDetMap21->Fill(pixy,pixx); //  "
	   else if(ladder== -1 && module== 3) hpixDetMap22->Fill(pixy,pixx); //  "
	   else if(ladder== -5 && module==-1) hpixDetMap23->Fill(pixy,pixx); //  "
	   else if(ladder== -5 && module==-2) hpixDetMap24->Fill(pixy,pixx); //  "
	   else if(ladder== -5 && module==-3) hpixDetMap25->Fill(pixy,pixx); //
	   else if(ladder== 12 && module==-1) hpixDetMap26->Fill(pixy,pixx); // dcdc
	   else if(ladder== 11 && module==-4) hpixDetMap27->Fill(pixy,pixx); // dcdc
	   else if(ladder== 14 && module==-1) hpixDetMap28->Fill(pixy,pixx); // dcdc
	   else if(ladder== 13 && module==-4) hpixDetMap29->Fill(pixy,pixx); // "

	   else if(ladder== 7 && module== 1) hpixDetMap30->Fill(pixy,pixx); // "
	   else if(ladder== 1 && module== 1) hpixDetMap31->Fill(pixy,pixx); // "
	   else if(ladder== 1 && module== 2) hpixDetMap32->Fill(pixy,pixx); // "
	   else if(ladder== 1 && module== 3) hpixDetMap33->Fill(pixy,pixx); // "
#endif

#if defined(BX) || defined(BX_NEW)
	   if(bxId>-1) {
	     if(bxId==3)      hpixChargebx1->Fill(adc); // coll
	     else if(bxId==4) hpixChargebx2->Fill(adc); // coll+1
	     else if(bxId==0) hpixChargebx6->Fill(adc); // empty
	     else if(bxId==1) hpixChargebx3->Fill(adc); // beam1
	     else if(bxId==2) hpixChargebx3->Fill(adc); // beam2
	     else if(bxId==5 || bxId==6) hpixChargebx5->Fill(adc); // gap and in bwtween 
	     else cout<<" wrong bx id "<<bxId<<endl;
	   }
#endif

#ifdef ROC_EFF
	    pixEff->addPixel(2,ladder,module,roc);

	   //index = lumiBlock/5;
// 	    if     (ladder==10 && module== 2)  hmoduleHits2ls->Fill(float(lumiBlock),0); // many resyncs
// 	   if     (ladder==-2 && module== 1 && roc==10)  hrocHits2ls->Fill(float(lumiBlock),0);
// 	   else if(ladder== 1 && module== 1 && roc== 3)  hrocHits2ls->Fill(float(lumiBlock),1);
// 	   else if(ladder== 1 && module== 2 && roc== 4)  hrocHits2ls->Fill(float(lumiBlock),2);
// 	   else if(ladder== 2 && module==-4 && roc== 3)  hrocHits2ls->Fill(float(lumiBlock),3);
// 	   else if(ladder==10 && module==-3 && roc== 1)  hrocHits2ls->Fill(float(lumiBlock),4);
// 	   else if(ladder==10 && module==-1 && roc==10)  hrocHits2ls->Fill(float(lumiBlock),5);
// 	   else if(ladder==11 && module== 3 && roc==13)  hrocHits2ls->Fill(float(lumiBlock),6);
// 	   else if(ladder==12 && module==-4 && roc==14)  hrocHits2ls->Fill(float(lumiBlock),7);
// 	   else if(ladder==12 && module==-1 && roc== 3)  hrocHits2ls->Fill(float(lumiBlock),8);
// 	   else if(ladder==-5 && module==-2 && roc==10)  hrocHits2ls->Fill(float(lumiBlock),9);
// 	   else if(ladder==10 && module== 2 && roc== 0)  hrocHits3ls->Fill(float(lumiBlock),9); // many resuncs 

#endif



#ifdef VDM_STUDIES
	    hcharPixls2->Fill(lumiBlock,adc);
#endif	    

	  } else if(layer==3) {

	    numOfPixPerDet3++;
	    numOfPixPerLay3++;
	    hpixcharge3->Fill(adc);
	    hpixDetMap3->Fill(pixy,pixx);
	    hpixDets3->Fill(float(module),float(ladder));
	    hpixchargeDets3->Fill(float(module),float(ladder),adc);
	    hpladder3id->Fill(float(ladder));
	    hpz3id->Fill(float(module));

 	    htest4->Fill(adc,float(size));
	    //if(link==0)      hl1DetMap3->Fill(float(module),float(ladder));
	    //else if(link==1) hl2DetMap3->Fill(float(module),float(ladder));
	    //if(isBig) hpixDetsE3->Fill(float(module),float(ladder));
	    //module3[int(pixx)][int(pixy)]++;

	    hpcols3->Fill(pixy);
	    hprows3->Fill(pixx);
	    hpixz3->Fill(gZ);

#ifdef ROC_RATE
	    hrocMap3->Fill(rocZ,rocPhi);
	    hrocPixcharMap3->Fill(rocZ,rocPhi,adc);
	    hpixRocRate3->Fill(rocZ);
#endif

#ifdef SINGLE_MODULES
	    //if     (ladder==  9 && module== 2) hpixDetMap30->Fill(pixy,pixx); // fed errors
	    //else if(ladder==  8 && module== 3) hpixDetMap31->Fill(pixy,pixx); // bad caldel
	    //else if(ladder== -6 && module== 2) hpixDetMap32->Fill(pixy,pixx); // pix 0,0
	    //else if(ladder== 21 && module== 4) hpixDetMap33->Fill(pixy,pixx); // unmaskable pixel
	    //else if(ladder==-5  && module==-2) hpixDetMap34->Fill(pixy,pixx); // noisy
	    //else if(ladder==-10 && module== 3) hpixDetMap35->Fill(pixy,pixx); // " 
	    //else if(ladder==3   && module==-3) hpixDetMap36->Fill(pixy,pixx); // "   
	    //else if(ladder== 19 && module==-3) hpixDetMap37->Fill(pixy,pixx); // noisy 
	    //else if(ladder== 8  && module==-4) hpixDetMap38->Fill(pixy,pixx); // noisy  
	    //else if(ladder==-11 && module== 1) hpixDetMap39->Fill(pixy,pixx); // noisy
#endif
	    hpixchar3->Fill(zPos,adc);
	    //hcharPixbx->Fill(bx,adc);
	    //hcharPixls->Fill(lumiBlock,adc);
#ifdef LS_TESTS
	    hcharPix3ls->Fill(lumiBlock,adc);
#endif

#ifdef BX_TESTS
	   hcharPix3bx->Fill(bx,adc);
#endif

#ifdef VDM_STUDIES
	    hcharPixls3->Fill(lumiBlock,adc);
#endif

#if defined(BX) || defined(BX_NEW)
	   if(bxId>-1) {
	     if(bxId==3)      hpixChargebx1->Fill(adc); // coll
	     else if(bxId==4) hpixChargebx2->Fill(adc); // coll+1
	     else if(bxId==0) hpixChargebx6->Fill(adc); // empty
	     else if(bxId==1) hpixChargebx3->Fill(adc); // beam1
	     else if(bxId==2) hpixChargebx3->Fill(adc); // beam2
	     else if(bxId==5 || bxId==6) hpixChargebx5->Fill(adc); //  gap and in-betwen
	     else cout<<" wrong bx id "<<bxId<<endl;
	   }
#endif

#ifdef ROC_EFF
	    pixEff->addPixel(3,ladder,module,roc);

	    //index = lumiBlock/5;
// 	    if     (ladder==11 && module== 1)  hmoduleHits3ls->Fill(float(lumiBlock),0);
	    
// 	    if     (ladder==-18 && module==-3 && roc== 0)  hrocHits3ls->Fill(float(lumiBlock),0);
// 	    else if(ladder== 5  && module== 1 && roc== 5)  hrocHits3ls->Fill(float(lumiBlock),1);
// 	    else if(ladder== 9  && module==-1 && roc== 2)  hrocHits3ls->Fill(float(lumiBlock),2);
// 	    else if(ladder==10  && module==-3 && roc== 8)  hrocHits3ls->Fill(float(lumiBlock),3);
// 	    else if(ladder==12  && module==-3 && roc==12)  hrocHits3ls->Fill(float(lumiBlock),4);
// 	    else if(ladder==-18 && module==-2 && roc==14)  hrocHits3ls->Fill(float(lumiBlock),5);
	    //else if(ladder==-3  && module==-3 && roc==14)  hrocHits3ls->Fill(float(lumiBlock),6);
	    //else if(ladder==-3  && module==-3 && roc==14)  hrocHits3ls->Fill(float(lumiBlock),7);
	    //else if(ladder==-3  && module==-3 && roc==14)  hrocHits3ls->Fill(float(lumiBlock),8);
	    //else if(ladder==-3  && module==-3 && roc==14)  hrocHits3ls->Fill(float(lumiBlock),9);
	    
	    // run 176286
	    //if     (ladder==-18 && module==-2 && roc==14)  hrocHits3ls->Fill(float(lumiBlock),0);
	    //else if(ladder==-3  && module==-3 && roc==14)  hrocHits3ls->Fill(float(lumiBlock),1);
	    //else if(ladder== 3  && module==-1 && roc==12)  hrocHits3ls->Fill(float(lumiBlock),2);
	    //else if(ladder==16  && module==-3 && roc== 6)  hrocHits3ls->Fill(float(lumiBlock),3);
	    //else if(ladder== 2  && module== 1 && roc== 0)  hrocHits3ls->Fill(float(lumiBlock),4);
#endif

	  } else if(layer==4) {

	    numOfPixPerDet4++;
	    numOfPixPerLay4++;
 
	    hpixcharge4->Fill(adc);
	    hpixDetMap4->Fill(pixy,pixx);
	    hpixDets4->Fill(float(module),float(ladder));
	    hpixchargeDets4->Fill(float(module),float(ladder),adc);
	    hpladder4id->Fill(float(ladder));
	    hpz4id->Fill(float(module));
 	    htest4->Fill(adc,float(size));
	    hpcols4->Fill(pixy);
	    hprows4->Fill(pixx);

	    hpixchar4->Fill(zPos,adc);
	    //hcharPixbx->Fill(bx,adc);
	    //hcharPixls->Fill(lumiBlock,adc);
	    hpixz4->Fill(gZ);

#ifdef ROC_RATE
	    hrocMap4->Fill(rocZ,rocPhi);
	    hrocPixcharMap4->Fill(rocZ,rocPhi,adc);
	    hpixRocRate4->Fill(rocZ);
#endif

#ifdef LS_TESTS
	    hcharPix4ls->Fill(lumiBlock,adc);
#endif

#ifdef BX_TESTS
	   hcharPix4bx->Fill(bx,adc);
#endif

#ifdef SINGLE_MODULES
	   //if     (ladder==-17 && module==-2) hpixDetMap40->Fill(pixy,pixx); // noisy 
	   //else if(ladder== 13 && module== 4) hpixDetMap41->Fill(pixy,pixx); // noise
	   //else if(ladder==-14 && module== 2) hpixDetMap42->Fill(pixy,pixx); //  "
	   //else if(ladder== 13 && module== 3) hpixDetMap43->Fill(pixy,pixx); //  "
	   //else if(ladder== 32 && module== 1) hpixDetMap44->Fill(pixy,pixx); //  " 
	   //else if(ladder== 25 && module== 4) hpixDetMap45->Fill(pixy,pixx); // " 
	   //else if(ladder== -1 && module==-1) hpixDetMap46->Fill(pixy,pixx); // "
	    //else if(ladder==-30 && module==-2) hpixDetMap47->Fill(pixy,pixx); // "
	    //else if(ladder==-22 && module== 2) hpixDetMap48->Fill(pixy,pixx); //  
	    //else if(ladder== 32 && module== 2) hpixDetMap49->Fill(pixy,pixx); //  
#endif
	  }  // if layer

	} else if (subid==2 && (selectEvent==-1 || countEvents==selectEvent)) {  // endcap
	  // pixels

	  if(disk==1) { // disk1 -+z
	    if(side==1) numOfPixPerDisk3++;      // d1,-z
	    else if(side==2) numOfPixPerDisk4++; // d1, +z
	    else cout<<" unknown side "<<side<<endl;

	    hpixcharge5->Fill(adc);
	    hpixDiskR1->Fill(rPos);
	    
	  } else if(disk==2) { // disk2 -+z
	    
	    if(side==1) numOfPixPerDisk2++;      // d2, -z
	    else if(side==2) numOfPixPerDisk5++; // d2, +z
	    else cout<<" unknown side "<<side<<endl;

	    hpixcharge6->Fill(adc);
	    hpixDiskR2->Fill(rPos);
	    
	  } else if(disk==3) { // disk3 -+z

	    if(side==1) numOfPixPerDisk1++;      // d3, -z
	    else if(side==2) numOfPixPerDisk6++; // d3, +z
	    else cout<<" unknown side "<<side<<endl;
	    hpixcharge7->Fill(adc);
	    hpixDiskR3->Fill(rPos);

	  } else cout<<" unknown disk "<<disk<<endl;

	} // end if subdet (pixel loop)

#endif // HISTOS
	
	// Find if this cluster includes an edge pixel
	edgeInX = topol->isItEdgePixelInX(int(pixx));
	edgeInY = topol->isItEdgePixelInY(int(pixy));
	
	if(PRINT || select ) cout<<i<<" "<<pixx<<" "<<pixy<<" "<<adc<<endl;
	//" "<<bigInX<<" "<<bigInY<<" "<<edgeInX<<" "<<edgeInY<<" "<<isBig<<endl;
	
	if(edgeInX) edgeHitX2=true;
	if(edgeInY) edgeHitY2=true; 
	//if(bigInX) cluBigInX=true;
	//if(bigInY) cluBigInY=true;

      } // pixel loop
      
#ifdef HISTOS
      // Cluster histos
      if (subid==1 && (selectEvent==-1 || countEvents==selectEvent) ) {  // barrel
	//if (subid==1) {  // barrel

	if(layer==1) {  // layer 1
	  
	  hcluDets1->Fill(float(module),float(ladder));
	  hsizeDets1->Fill(float(module),float(ladder),float(size));
	  hchargeDets1->Fill(float(module),float(ladder),ch);
	  hladder1id->Fill(float(ladder));
	  hz1id->Fill(float(module));
	  hcols1->Fill(y);
	  hrows1->Fill(x);
	  hcols12->Fill(y);
	  hcols11->Fill(ly);
	  hrows11->Fill(lx);

	  hcluDetMap1->Fill(y,x);


	  if(newL1Modules) {
	    hcharge1n->Fill(ch);
	    hsize1n->Fill(float(size));
	    hsizex1n->Fill(float(sizeX));
	    hsizey1n->Fill(float(sizeY));
	  } else {
	    hcharge1->Fill(ch);
	    hcharge1_zoom->Fill(ch);
	    hsize1->Fill(float(size));
	    hsizex1->Fill(float(sizeX));
	    hsizey1->Fill(float(sizeY));
	  }

	  numOfClustersPerDet1++;
	  numOfClustersPerLay1++;
	  avCharge1 += ch;

	  htest1->Fill(ch,float(size));

#ifdef STUDY_ONEMOD
	    hsizeMap1->Fill(y,x,size);
	    hsizeXMap1->Fill(y,x,sizeX);
	    hsizeYMap1->Fill(y,x,sizeY);
	    hclucharMap1->Fill(y,x,ch);
#endif

#ifdef MINMAX
	  if(size>1) {
	    hcharMin1->Fill(adcMin);
	    hcharMax1->Fill(adcMax);
	  }
#endif

#ifdef ROC_RATE
	    hcluRocRate1->Fill(rocZ);
	    hrocClucharMap1->Fill(rocZ,rocPhi,ch);
	    hrocSizeXMap1->Fill(rocZ,rocPhi,sizeX);
	    hrocSizeYMap1->Fill(rocZ,rocPhi,sizeY);
#endif

#ifdef LAY1_SPLIT
	  if(inner) { // inner
	    hcharge11->Fill(ch);
	    if(ring12) {
	      hcharge1InR12->Fill(ch);
	      hsize1InR12->Fill(size);
	      hsizex1InR12->Fill(sizeX);
	      hsizey1InR12->Fill(sizeY);
	    } else { // ring 34
	      hcharge1InR34->Fill(ch);
	      hsize1InR34->Fill(size);
	      hsizex1InR34->Fill(sizeX);
	      hsizey1InR34->Fill(sizeY);
	    }  
	  } else { // outer
	    hcharge12->Fill(ch);
	    if(ring12) {
	      hcharge1OutR12->Fill(ch);
	      hsize1OutR12->Fill(size);
	      hsizex1OutR12->Fill(sizeX);
	      hsizey1OutR12->Fill(sizeY);
	    } else { // ring 34
	      hcharge1OutR34->Fill(ch);
	      hsize1OutR34->Fill(size);
	      hsizex1OutR34->Fill(sizeX);
	      hsizey1OutR34->Fill(sizeY);
	    }  
	  }  
#endif

#ifdef TESTING_ADC
	  if(cluWithNegativeADC) {
	    hcluDetT1->Fill(float(module),float(ladder),ch);
	    hsizeDetT1->Fill(float(module),float(ladder),float(size));

	    hchargeT1->Fill(ch);
	    hsizeT1->Fill(float(size));

	    if((sizeX==2) && (sizeY==2)) {
	      if( (minPixelCol%2)==0 ) hchargeT12_1->Fill(ch);  //1st pixel in 1st col of dcol  
	      else                     hchargeT12_2->Fill(ch);  //1st pixel in 2nd col of dcol  
	    } else if((sizeX==1) && (sizeY==2)) {
	      if( (minPixelCol%2)==0 ) hchargeT12_1->Fill(ch);  //1st pixel in 1st col of dcol  
	      else                     hchargeT12_2->Fill(ch);  //1st pixel in 2nd col of dcol  
	    } // size=1/2

	  }
#endif

#ifdef STUDY_LAY1
	    if(size==1) {
	      hcharge111->Fill(ch);
	    } else if ((size>=2) && (size<=4)) {
	      if((sizeX==2) && (sizeY==1)) {
		hcharge121->Fill(ch);
	      } else if((sizeX==2) && (sizeY==2)) {
		hcharge122->Fill(ch);
		if( (minPixelCol%2)==0 ) hcharge12_1->Fill(ch);  //1st pixel in 1st col of dcol  
		else                     hcharge12_2->Fill(ch);  //1st pixel in 2nd col of dcol  
	      } else if((sizeX==1) && (sizeY==2)) {
		hcharge112->Fill(ch);
		if( (minPixelCol%2)==0 ) hcharge12_1->Fill(ch);  //1st pixel in 1st col of dcol  
		else                     hcharge12_2->Fill(ch);  //1st pixel in 2nd col of dcol  
	      }
	    } // size=1/2
#endif

#ifdef NEW_MODULES
	  // if(newModule) { // new from 2014
	  //   hcharge11->Fill(ch);
	  //   hsize11->Fill(float(size));
	  //   hsizex11->Fill(float(sizeX));
	  //   hsizey11->Fill(float(sizeY));
	  // } else {  // old 2008 modules 
#endif
	    //	  }
	  //if(numOf<10) hcharge11->Fill(ch);
	  //else if(numOf<25) hcharge12->Fill(ch);
	  //else if(numOf<100) hcharge13->Fill(ch);
	  //else hcharge14->Fill(ch);

	  hgz1->Fill(zPos);

	  hclumult1->Fill(zPos,size);
	  hclumultx1->Fill(zPos,sizeX);
	  hclumulty1->Fill(zPos,sizeY);
	  hsizeyz1->Fill(zPos,sizeY);
	  hcluchar1->Fill(zPos,ch);
#ifdef PHI_PROFILES
	  hclumultxPhi1->Fill(gPhi,sizeX);
	  hclumultyPhi1->Fill(gPhi,sizeY);
	  hclucharPhi1->Fill(gPhi,ch);
#endif 

#if defined(BX) || defined(BX_NEW)
	  if(bxId>-1) {
	    if(bxId==3)      {hchargebx1->Fill(ch); hsizebx1->Fill(size);} // coll
	    else if(bxId==4) {hchargebx2->Fill(ch); hsizebx2->Fill(size);} // coll+1
	    else if(bxId==0) {hchargebx6->Fill(ch); hsizebx6->Fill(size);} // empty
	    else if(bxId==1) {hchargebx3->Fill(ch); hsizebx3->Fill(size);} // beam1
	    else if(bxId==2) {hchargebx4->Fill(ch); hsizebx3->Fill(size);} // beam2
	    else if(bxId==5 || bxId==6) {hchargebx5->Fill(ch); hsizebx5->Fill(size);} // gap and in-between
	    else cout<<" wrong bx id "<<bxId<<endl;
	  }
	  //if(bxId==0) htestbx->Fill(bx,ch);
#endif

	  //hcharClubx->Fill(bx,ch);
	  //hsizeClubx->Fill(bx,size);
	  //hsizeYClubx->Fill(bx,sizeY);

	  //hcharCluls->Fill(lumiBlock,ch);
	  //hsizeCluls->Fill(lumiBlock,size);
	  //hsizeXCluls->Fill(lumiBlock,sizeX);

#ifdef BX_TESTS
	  hcharClu1bx->Fill(float(bx),ch); //
	  hsizeClu1bx->Fill(float(bx),size); //
#endif

#ifdef LS_TESTS
	  hcharClu1ls->Fill(lumiBlock,ch);
	  hsizeClu1ls->Fill(lumiBlock,size);
#endif

	  //hcharCluLumi->Fill(instlumi,ch);
	  //hsizeCluLumi->Fill(instlumi,size);
	  //hsizeXCluLumi->Fill(instlumi,sizeX);
	  //hsizeYCluLumi->Fill(instlumi,sizeY);

	  //hchargen5->Fill(ch,float(size));
	  //hchargen3->Fill(ch);
	  //if(size==1) hchargen4->Fill(ch);

#ifdef TEST_GEOM
	  hbpixXY->Fill(gX,gY);
#endif

#ifdef VDM_STUDIES
	  hcharCluls1->Fill(lumiBlock,ch);
	  hsizeCluls1->Fill(lumiBlock,size);
	  hsizeXCluls1->Fill(lumiBlock,sizeX);
#endif

	} else if( layer==2 ) {

	  numOfClustersPerDet2++;
	  numOfClustersPerLay2++;
	  avCharge2 += ch;

	  hcluDets2->Fill(float(module),float(ladder));
	  hsizeDets2->Fill(float(module),float(ladder),float(size));
	  hchargeDets2->Fill(float(module),float(ladder),ch);
	  hladder2id->Fill(float(ladder));
	  hz2id->Fill(float(module));
	  //hsizeXDetMap2->Fill(float(module),float(ladder),float(sizeX));
	  //hsizeYDetMap2->Fill(float(module),float(ladder),float(sizeY));
	  hcols2->Fill(y);
	  hrows2->Fill(x);
	  hcols21->Fill(ly);
	  hrows21->Fill(lx);

	  //if(ladder==1 && module>0) {hcharge8->Fill(ch);}

	  if(badL2Modules) {
	    hcharge2b->Fill(ch);
	    hsize2b->Fill(float(size));

	  } else if(goodL2Modules) {
	    hcharge2g->Fill(ch);
	    hsize2g->Fill(float(size));

	  } else {
	    hcharge2->Fill(ch);
	    hsize2->Fill(float(size));
	    hsizex2->Fill(float(sizeX));
	    hsizey2->Fill(float(sizeY));
 
	    hgz2->Fill(zPos);
	    hclumult2->Fill(zPos,size);
	    hclumultx2->Fill(zPos,sizeX);
	    hclumulty2->Fill(zPos,sizeY);
	    hcluchar2->Fill(zPos,ch);
	    hsizeyz2->Fill(zPos,sizeY);
	    
	    hcluDetMap2->Fill(y,x);
	    htest3->Fill(ch,float(size));

#ifdef ROC_RATE
	    hcluRocRate2->Fill(rocZ);
	    hrocClucharMap2->Fill(rocZ,rocPhi,ch);
	    hrocSizeXMap2->Fill(rocZ,rocPhi,sizeX);
	    hrocSizeYMap2->Fill(rocZ,rocPhi,sizeY);
#endif
#ifdef PHI_PROFILES
	    hclumultxPhi2->Fill(gPhi,sizeX);
	    hclumultyPhi2->Fill(gPhi,sizeY);
	    hclucharPhi2->Fill(gPhi,ch);
#endif 
#ifdef BX_TESTS
	    hcharClu2bx->Fill(float(bx),ch); //
	    hsizeClu2bx->Fill(float(bx),size); //
#endif
	    
#ifdef LS_TESTS
	    hcharClu2ls->Fill(lumiBlock,ch);
	    hsizeClu2ls->Fill(lumiBlock,size);
#endif
#ifdef MINMAX
	    if(size>1) {
	      hcharMin2->Fill(adcMin);
	      hcharMax2->Fill(adcMax);
	    }
#endif	    
	  } // if bad 

#if defined(BX) || defined(BX_NEW)
	    if(bxId>-1) {
	      if(bxId==3)      {hchargebx1->Fill(ch); hsizebx1->Fill(size);} // coll
	    else if(bxId==4) {hchargebx2->Fill(ch); hsizebx2->Fill(size);} // coll+1
	    else if(bxId==0) {hchargebx6->Fill(ch); hsizebx6->Fill(size);} // empty
	    else if(bxId==1) {hchargebx3->Fill(ch); hsizebx3->Fill(size);} // beam1
	    else if(bxId==2) {hchargebx4->Fill(ch); hsizebx3->Fill(size);} // beam2
	    else if(bxId==5 || bxId==6) {hchargebx5->Fill(ch); hsizebx5->Fill(size);} // gap & in-between
	    else cout<<" wrong bx id "<<bxId<<endl;
	  }
#endif

#ifdef TEST_GEOM
	  hbpixXY->Fill(gX,gY);
#endif
#ifdef VDM_STUDIES
	  hcharCluls2->Fill(lumiBlock,ch);
	  hsizeCluls2->Fill(lumiBlock,size);
	  hsizeXCluls2->Fill(lumiBlock,sizeX);
#endif

	} else if(layer==3) {

	  hcluDets3->Fill(float(module),float(ladder));
	  hsizeDets3->Fill(float(module),float(ladder),float(size));
	  hchargeDets3->Fill(float(module),float(ladder),ch);
	  hladder3id->Fill(float(ladder));
	  hz3id->Fill(float(module));
	  hcols3->Fill(y);
	  hrows3->Fill(x);

	  hcluDetMap3->Fill(y,x);

	  hcharge3->Fill(ch);
	  hsize3->Fill(float(size));
	  hsizex3->Fill(float(sizeX));
	  hsizey3->Fill(float(sizeY));
	  htest3->Fill(ch,float(size));



#ifdef MINMAX
	  if(size>1) {
	    hcharMin3->Fill(adcMin);
	    hcharMax3->Fill(adcMax);
	  }
#endif
	  numOfClustersPerDet3++;
	  numOfClustersPerLay3++;
	  avCharge3 += ch;
	  hgz3->Fill(zPos);
	  hclumult3->Fill(zPos,size);
	  hclumultx3->Fill(zPos,sizeX);
	  hclumulty3->Fill(zPos,sizeY);
	  hcluchar3->Fill(zPos,ch);
	  hsizeyz3->Fill(zPos,sizeY);

#ifdef ROC_RATE
	  hcluRocRate3->Fill(rocZ);
	  hrocClucharMap3->Fill(rocZ,rocPhi,ch);
	  hrocSizeXMap3->Fill(rocZ,rocPhi,sizeX);
	  hrocSizeYMap3->Fill(rocZ,rocPhi,sizeY);
#endif

#ifdef TEST_GEOM
	  hbpixXY->Fill(gX,gY);
#endif

#ifdef BX_TESTS
	  hcharClu3bx->Fill(float(bx),ch); //
	  hsizeClu3bx->Fill(float(bx),size); //
#endif

#if defined(BX) || defined(BX_NEW)
	  if(bxId>-1) {
	    if(bxId==3)      {hchargebx1->Fill(ch); hsizebx1->Fill(size);} // coll
	    else if(bxId==4) {hchargebx2->Fill(ch); hsizebx2->Fill(size);} // coll+1
	    else if(bxId==0) {hchargebx6->Fill(ch); hsizebx6->Fill(size);} // empty
	    else if(bxId==1) {hchargebx3->Fill(ch); hsizebx3->Fill(size);} // beam1
	    else if(bxId==2) {hchargebx4->Fill(ch); hsizebx3->Fill(size);} // beam2
	    else if(bxId==5 || bxId==6) {hchargebx5->Fill(ch); hsizebx5->Fill(size);} // gap & in-between
	    else cout<<" wrong bx id "<<bxId<<endl;
	  }
#endif

	  //hcharClubx->Fill(bx,ch);
	  //hsizeClubx->Fill(bx,size);
	  //hsizeYClubx->Fill(bx,sizeY);
	  //hcharCluls->Fill(lumiBlock,ch);
	  //hsizeCluls->Fill(lumiBlock,size);
	  //hsizeXCluls->Fill(lumiBlock,sizeX);

#ifdef LS_TESTS
	  hcharClu3ls->Fill(lumiBlock,ch);
	  hsizeClu3ls->Fill(lumiBlock,size);
#endif

#ifdef VDM_STUDIES
	  hcharCluls3->Fill(lumiBlock,ch);
	  hsizeCluls3->Fill(lumiBlock,size);
	  hsizeXCluls3->Fill(lumiBlock,sizeX);
#endif

	} else if(layer==4) {


	  numOfClustersPerDet4++;
	  numOfClustersPerLay4++;

	  hcharge4->Fill(ch);
	  hsize4->Fill(float(size));
	  hsizex4->Fill(float(sizeX));
	  hsizey4->Fill(float(sizeY));
	  
	  avCharge4 += ch;
	  hgz4->Fill(zPos);
	  hclumult4->Fill(zPos,size);
	  hclumultx4->Fill(zPos,sizeX);
	  hclumulty4->Fill(zPos,sizeY);
	  hcluchar4->Fill(zPos,ch);

	  hsizeyz4->Fill(zPos,sizeY);

	  htest3->Fill(ch,float(size));
#ifdef MINMAX
	  if(size>1) {
	    hcharMin4->Fill(adcMin);
	    hcharMax4->Fill(adcMax);
	  }
#endif
	  hcluDets4->Fill(float(module),float(ladder));
	  hsizeDets4->Fill(float(module),float(ladder),float(size));
	  hchargeDets4->Fill(float(module),float(ladder),ch);
	  hladder4id->Fill(float(ladder));
	  hz4id->Fill(float(module));
	  hcols4->Fill(y);
	  hrows4->Fill(x);

	  hcluDetMap4->Fill(y,x);


	  //hcharClubx->Fill(bx,ch);
	  //hsizeClubx->Fill(bx,size);
	  //hsizeYClubx->Fill(bx,sizeY);

	  //hcharCluls->Fill(lumiBlock,ch);
	  //hsizeCluls->Fill(lumiBlock,size);
	  //hsizeXCluls->Fill(lumiBlock,sizeX);

#ifdef ROC_RATE
	  hcluRocRate4->Fill(rocZ);
	  hrocClucharMap4->Fill(rocZ,rocPhi,ch);
	  hrocSizeXMap4->Fill(rocZ,rocPhi,sizeX);
	  hrocSizeYMap4->Fill(rocZ,rocPhi,sizeY);
#endif

#ifdef TEST_GEOM
	  hbpixXY->Fill(gX,gY);
#endif

#ifdef BX_TESTS
	  hcharClu4bx->Fill(float(bx),ch); //
	  hsizeClu4bx->Fill(float(bx),size); //
#endif

#ifdef LS_TESTS
	  hcharClu4ls->Fill(lumiBlock,ch);
	  hsizeClu4ls->Fill(lumiBlock,size);
#endif


	} // end if layer

      } else if (subid==2 && (selectEvent==-1 || countEvents==selectEvent) ) {  // endcap

	//cout<<disk<<" "<<side<<endl;
	if(disk==1) { // disk1 -+z
	  if(side==1) {
	    numOfClustersPerDisk3++;      // d1,-z
	    avCharge5 += ch;
	  } else if(side==2) {
	    numOfClustersPerDisk4++; // d1, +z
	    avCharge5 += ch;
	  } else cout<<" unknown side "<<side<<endl;

	  hcharge5->Fill(ch);

	} else if(disk==2) { // disk2 -+z

	  if(side==1) {
	    numOfClustersPerDisk2++;      // d2, -z
	    avCharge5 += ch;
	  } else if(side==2) { 
	    numOfClustersPerDisk5++; // d2, +z
	    avCharge5 += ch;
	  } else cout<<" unknown side "<<side<<endl;

	  hcharge6->Fill(ch);

	} else if(disk==3) { // disk3 -+z

	  if(side==1) {
	    numOfClustersPerDisk1++;      // d3, -z
	    avCharge5 += ch;
	  } else if(side==2) { 
	    numOfClustersPerDisk6++; // d3, +z
	    avCharge5 += ch;
	  } else cout<<" unknown side "<<side<<endl;
	  hcharge7->Fill(ch);

	} else cout<<" unknown disk "<<disk<<endl;

      } // end barrel/forward cluster loop
      
#endif // HISTOS

      if( (edgeHitX != edgeHitX2) && sizeX<64) 
	cout<<" wrong egdeX "<<edgeHitX<<" "<<edgeHitX2<<endl;
      if( (edgeHitY != edgeHitY2) && sizeY<64)  
	cout<<" wrong egdeY "<<edgeHitY<<" "<<edgeHitY2<<endl;

    } // clusters 

    
    if(numOfClustersPerDet1>maxClusPerDet) maxClusPerDet = numOfClustersPerDet1;
    if(numOfClustersPerDet2>maxClusPerDet) maxClusPerDet = numOfClustersPerDet2;
    if(numOfClustersPerDet3>maxClusPerDet) maxClusPerDet = numOfClustersPerDet3;

    if(PRINT) {
      if(layer==1) 
	cout<<"Lay1: number of clusters per det = "<<numOfClustersPerDet1<<endl;
      else if(layer==2) 
	cout<<"Lay2: number of clusters per det = "<<numOfClustersPerDet1<<endl;
      else if(layer==3) 
	cout<<"Lay3: number of clusters per det = "<<numOfClustersPerDet1<<endl;
    } // end if PRINT

#ifdef HISTOS
    if (subid==1 && (selectEvent==-1 || countEvents==selectEvent) ) {  // barrel
      //if (subid==1 && countEvents==selectEvent) {  // barrel

      //hlayerid->Fill(float(layer));

#ifdef TEST_DCOLS
      histogramDcols(layer,ladder,module);
#endif
      // Det histos
      if(layer==1) {
	
	++numberOfDetUnits1;
	hclusPerDet1->Fill(float(numOfClustersPerDet1));
	hpixPerDet1->Fill(float(numOfPixPerDet1));
	if(numOfPixPerDet1>maxPixPerDet) maxPixPerDet = numOfPixPerDet1;  
	hDets1->Fill(float(module),float(ladder));

	if(abs(module)==1)      hpixPerDet11->Fill(float(numOfPixPerDet1));
	else if(abs(module)==2) hpixPerDet12->Fill(float(numOfPixPerDet1));
	else if(abs(module)==3) hpixPerDet13->Fill(float(numOfPixPerDet1));
	else if(abs(module)==4) hpixPerDet14->Fill(float(numOfPixPerDet1));

#ifdef TESTING_ADC
	  if(detWithNegativeADC) {
	    hclusPerDetT1->Fill(float(numOfClustersPerDet1));
	    hpixPerDetT1->Fill(float(numOfPixPerDet1));
	  }
#endif
	numOfClustersPerDet1=0;        
	numOfPixPerDet1=0;

      } else if(layer==2) {

	++numberOfDetUnits2;
	hclusPerDet2->Fill(float(numOfClustersPerDet2));
	hpixPerDet2->Fill(float(numOfPixPerDet2));
	if(numOfPixPerDet2>maxPixPerDet) maxPixPerDet = numOfPixPerDet2;  
	hDets2->Fill(float(module),float(ladder));

	if(abs(module)==1)      hpixPerDet21->Fill(float(numOfPixPerDet2));
	else if(abs(module)==2) hpixPerDet22->Fill(float(numOfPixPerDet2));
	else if(abs(module)==3) hpixPerDet23->Fill(float(numOfPixPerDet2));
	else if(abs(module)==4) hpixPerDet24->Fill(float(numOfPixPerDet2));

	numOfClustersPerDet2=0;
	numOfPixPerDet2=0;        

      } else if(layer==3) {

	++numberOfDetUnits3;
	hclusPerDet3->Fill(float(numOfClustersPerDet3));
	hpixPerDet3->Fill(float(numOfPixPerDet3));
	if(numOfPixPerDet3>maxPixPerDet) maxPixPerDet = numOfPixPerDet3;  
	hDets3->Fill(float(module),float(ladder));

	if(abs(module)==1)      hpixPerDet31->Fill(float(numOfPixPerDet3));
	else if(abs(module)==2) hpixPerDet32->Fill(float(numOfPixPerDet3));
	else if(abs(module)==3) hpixPerDet33->Fill(float(numOfPixPerDet3));
	else if(abs(module)==4) hpixPerDet34->Fill(float(numOfPixPerDet3));

	numOfClustersPerDet3=0;
	numOfPixPerDet3=0;        

      } else if(layer==4) {

	++numberOfDetUnits4;
	hclusPerDet4->Fill(float(numOfClustersPerDet4));
	hpixPerDet4->Fill(float(numOfPixPerDet4));
	if(numOfPixPerDet3>maxPixPerDet) maxPixPerDet = numOfPixPerDet4;  
	hDets4->Fill(float(module),float(ladder));

	numOfClustersPerDet4=0;
	numOfPixPerDet4=0;        

      } // layer
      
    } // end barrel/forward

#endif // HISTOS

// #ifdef SINGLE_MODULES
//     if     (ladder==1 && module==4) eventFlag[0]=false; // 
//     else if(ladder==2 && module==4) eventFlag[1]=false;
//     else if(ladder==3 && module==4) eventFlag[2]=false; // 
//     else if(ladder==4 && module==4) eventFlag[3]=false; // 
//     else if(ladder==5 && module==4) eventFlag[4]=false; // 
//     else if(ladder==6 && module==4) eventFlag[5]=false; // 
//     else if(ladder==1 && module==-4) eventFlag[6]=false; //
//     else if(ladder==2 && module==-4) eventFlag[7]=false; // 
//     else if(ladder==3 && module==-4) eventFlag[8]=false; // 
//     else if(ladder==4 && module==-4) eventFlag[9]=false; // 
//     else if(ladder==5 && module==-4) eventFlag[10]=false; // 
//     else if(ladder==6 && module==-4) eventFlag[11]=false; // 
//     else if(ladder==-1 && module==4) eventFlag[12]=false; // 
//     else if(ladder==-2 && module==4) eventFlag[13]=false;
//     else if(ladder==-3 && module==4) eventFlag[14]=false; // 
//     else if(ladder==-4 && module==4) eventFlag[15]=false; // 
//     else if(ladder==-5 && module==4) eventFlag[16]=false; // 
//     else if(ladder==-6 && module==4) eventFlag[17]=false; // 
//     else if(ladder==-1 && module==-4) eventFlag[18]=false; //
//     else if(ladder==-2 && module==-4) eventFlag[19]=false; // 
//     else if(ladder==-3 && module==-4) eventFlag[20]=false; // 
//     else if(ladder==-4 && module==-4) eventFlag[21]=false; // 
//     else if(ladder==-5 && module==-4) eventFlag[22]=false; // 
//     else if(ladder==-6 && module==-4) eventFlag[23]=false; // 
//     else if(ladder==-1 && module==-2) eventFlag[24]=false; // 
// #endif 
    
  } // detunits loop

  
  if( PRINT || countEvents==selectEvent ) {  // 
    cout<<"run "<<run<<" event "<<event<<" bx "<<bx<<" lumi "<<lumiBlock<<" orbit "<<orbit<<" "<<countEvents<<endl;   
    cout<<"Num of pix "<<numberOfPixels<<" num of clus "<<numberOfClusters<<" max clus per det "
	<<maxClusPerDet<<" max pix per clu "<<maxPixPerClu<<" count "
	<<countEvents<<endl;
    cout<<"Number of clusters per Lay1,2,3: "<<numOfClustersPerLay1<<" "
	<<numOfClustersPerLay2<<" "<<numOfClustersPerLay3<<endl;
    cout<<"Number of pixels per Lay1,2,3: "<<numOfPixPerLay1<<" "
	<<numOfPixPerLay2<<" "<<numOfPixPerLay3<<endl;
    cout<<"Number of dets with clus in Lay1,2,3: "<<numberOfDetUnits1<<" "
	<<numberOfDetUnits2<<" "<<numberOfDetUnits3<<endl;
  } // if PRINT
    
#ifdef HISTOS

  //if(numberOfClusters<=3) continue; // skip events
  if ( (selectEvent==-1 || countEvents==selectEvent) ) { 

    hdets2->Fill(float(numOf));            // number of modules with pix
    hlumi1->Fill(float(lumiBlock));
    
    hdigis->Fill(float(numberOfPixels));  // all pix 
    hdigis2->Fill(float(numberOfPixels));  // same zoomed 

    int pixb = numOfPixPerLay1+numOfPixPerLay2+numOfPixPerLay3+numOfPixPerLay4;
    hdigisB->Fill(float(pixb));  // pix in bpix
    int pixf = numOfPixPerDisk1 + numOfPixPerDisk2 + numOfPixPerDisk3 + numOfPixPerDisk4+ 
      numOfPixPerDisk5+ numOfPixPerDisk6; 
    hdigisF->Fill(float(pixf));   // pix in fpix
    
    hclus->Fill(float(numberOfClusters)); // clusters fpix+bpix
    hclus2->Fill(float(numberOfClusters));  // same, zoomed

#ifdef PV
    //h2clupv->Fill(float(numberOfClusters),float(numPVsGood));
    //h2pixpv->Fill(float(numberOfPixels),float(numPVsGood));
    hclupv->Fill(float(numPVsGood),float(numberOfClusters));
    hpixpv->Fill(float(numPVsGood),float(numberOfPixels));
    //if(numPVsGood>0) {
      //float tmp = float(numberOfClusters)/float(numPVsGood);
      //hclupvlsn->Fill(float(lumiBlock),tmp);
      //tmp = float(numberOfPixels)/float(numPVsGood);
      //hpixpvlsn->Fill(float(lumiBlock),tmp);
    //}
#endif //PV

    int clusb = numOfClustersPerLay1+numOfClustersPerLay2+numOfClustersPerLay3+numOfClustersPerLay4;
    hclusBPix->Fill(float(clusb));  // clusters in bpix

    int clusf = numOfClustersPerDisk1+numOfClustersPerDisk2+
      numOfClustersPerDisk3+numOfClustersPerDisk4+
      numOfClustersPerDisk5+numOfClustersPerDisk6;
    hclusFPix->Fill(float(clusf));  // clusters in fpix

#ifdef HF
    hclushf->Fill(float(numberOfClusters),sumHFE);
    hclusbhf->Fill(float(clusb),sumHFE);
    hclusfhf->Fill(float(clusf),sumHFE);
    hclusb1hf->Fill(float(numOfClustersPerLay1),sumHFE);
    //hbxhf->Fill(float(bx),sumHFE);
    hbxhfp->Fill(float(bx),sumHFE);

#endif // HF

    //hclus5->Fill(float(numberOfNoneEdgePixels));   // count none edge pixels
    hclusls->Fill(float(lumiBlock),float(numberOfClusters)); // clusters fpix+bpix
    hpixls->Fill(float(lumiBlock),float(numberOfPixels)); // pixels fpix+bpix

    //hclubx->Fill(float(bx),float(numberOfClusters)); // clusters fpix+bpix
    //hpixbx->Fill(float(bx),float(numberOfPixels)); // pixels fpix+bpix

#ifdef BX_TESTS
    hclu1bx->Fill(float(bx),float(numOfClustersPerLay1)); //
    hpix1bx->Fill(float(bx),float(numOfPixPerLay1)); // 
    hclu2bx->Fill(float(bx),float(numOfClustersPerLay2)); //
    hpix2bx->Fill(float(bx),float(numOfPixPerLay2)); // 
    hclu3bx->Fill(float(bx),float(numOfClustersPerLay3)); //
    hpix3bx->Fill(float(bx),float(numOfPixPerLay3)); // 
    hclu4bx->Fill(float(bx),float(numOfClustersPerLay4)); //
    hpix4bx->Fill(float(bx),float(numOfPixPerLay4)); // 
#endif

#ifdef PV
    hpvbx->Fill(float(bx),float(numPVsGood)); // pvs
#endif 

#ifdef VDM_STUDIES
    hclusls1->Fill(float(lumiBlock),float(numOfClustersPerLay1)); // clusters bpix1
    hpixls1->Fill( float(lumiBlock),float(numOfPixPerLay1)); // pixels bpix1
    hclusls2->Fill(float(lumiBlock),float(numOfClustersPerLay2)); // clusters bpix2
    hpixls2->Fill( float(lumiBlock),float(numOfPixPerLay2)); // pixels bpix2
    hclusls3->Fill(float(lumiBlock),float(numOfClustersPerLay3)); // clusters bpix3
    hpixls3->Fill( float(lumiBlock),float(numOfPixPerLay3)); // pixels bpix3
#endif


#ifdef LS_TESTS
    // pix & clus per ls
    float tmp = float(numOfPixPerLay1);
    hpixb1l->Fill(float(lumiBlock),tmp); // pixels bpix
    tmp = float(numOfPixPerLay2);
    hpixb2l->Fill(float(lumiBlock),tmp); // pixels bpix
    tmp = float(numOfPixPerLay3);
    hpixb3l->Fill(float(lumiBlock),tmp); // pixels bpix
    tmp = float(numOfPixPerLay4);
    hpixb4l->Fill(float(lumiBlock),tmp); // pixels bpix
    
    tmp = float(pixf);
    hpixfl->Fill(float(lumiBlock),tmp); // pixels fpix
    
    tmp = float(numOfPixPerDisk1 + numOfPixPerDisk2); // -z
    hpixfml->Fill(float(lumiBlock),tmp); // pixels fpix
    tmp = float(numOfPixPerDisk3 + numOfPixPerDisk4); // +z
    hpixfpl->Fill(float(lumiBlock),tmp); // pixels fpix
    
    tmp = float(numOfClustersPerLay1);
    hclusb1l->Fill(float(lumiBlock),tmp); // clus bpix
    tmp = float(numOfClustersPerLay2);
    hclusb2l->Fill(float(lumiBlock),tmp); // clus bpix
    tmp = float(numOfClustersPerLay3);
    hclusb3l->Fill(float(lumiBlock),tmp); // clus bpix
    tmp = float(numOfClustersPerLay4);
    hclusb4l->Fill(float(lumiBlock),tmp); // clus bpix
    
    tmp = float(clusf);
    hclusfl->Fill(float(lumiBlock),tmp); // clus  fpix
    tmp = float(numOfClustersPerDisk1+numOfClustersPerDisk2);
    hclusfml->Fill(float(lumiBlock),tmp); // clus fpix
    tmp = float(numOfClustersPerDisk3+numOfClustersPerDisk4);
    hclusfpl->Fill(float(lumiBlock),tmp); // clus fpix    
#endif // LS_TESTS
      
    hclusPerLay1->Fill(float(numOfClustersPerLay1));
    hclusPerLay2->Fill(float(numOfClustersPerLay2));
    hclusPerLay3->Fill(float(numOfClustersPerLay3));
    hclusPerLay4->Fill(float(numOfClustersPerLay4));
    hpixPerLay1->Fill(float(numOfPixPerLay1));
    hpixPerLay2->Fill(float(numOfPixPerLay2));
    hpixPerLay3->Fill(float(numOfPixPerLay3));
    hpixPerLay4->Fill(float(numOfPixPerLay4));

    if(numOfClustersPerLay1>0) hdetsPerLay1->Fill(float(numberOfDetUnits1));
    if(numOfClustersPerLay2>0) hdetsPerLay2->Fill(float(numberOfDetUnits2));
    if(numOfClustersPerLay3>0) hdetsPerLay3->Fill(float(numberOfDetUnits3));
    if(numOfClustersPerLay4>0) hdetsPerLay4->Fill(float(numberOfDetUnits4));
    
    hclusPerDisk1->Fill(float(numOfClustersPerDisk1));
    hclusPerDisk2->Fill(float(numOfClustersPerDisk2));
    hclusPerDisk3->Fill(float(numOfClustersPerDisk3));
    hclusPerDisk4->Fill(float(numOfClustersPerDisk4));
    hclusPerDisk5->Fill(float(numOfClustersPerDisk5));
    hclusPerDisk6->Fill(float(numOfClustersPerDisk6));

    hpixPerDisk1->Fill(float(numOfPixPerDisk1));
    hpixPerDisk2->Fill(float(numOfPixPerDisk2));
    hpixPerDisk3->Fill(float(numOfPixPerDisk3));
    hpixPerDisk4->Fill(float(numOfPixPerDisk4));
    hpixPerDisk5->Fill(float(numOfPixPerDisk5));
    hpixPerDisk6->Fill(float(numOfPixPerDisk6));
    
    hmaxPixPerDet->Fill(float(maxPixPerDet));


#ifdef L1
    //int numberOfClusters0 = numberOfClusters;  // select all clusters
    
    //if(bptx3) hclus1->Fill(float(numberOfClusters0));
    //if(bptx4) hclus2->Fill(float(numberOfClusters0));
    //if(bit0)  hclus10->Fill(float(numberOfClusters0));   // 
    //if(bptx_and) hclus11->Fill(float(numberOfClusters0));
    //if(bptx_xor) hclus12->Fill(float(numberOfClusters0));
    //if(!bptx_xor && !bptx_and) hclus13->Fill(float(numberOfClusters0));
    //if(bptx_m)   hclus14->Fill(float(numberOfClusters0));
    //if(bptx_p)   hclus15->Fill(float(numberOfClusters0));

    //if(bcs_all)   hclus4->Fill(float(numberOfClusters0));       // or of all BCS bits
    //else      hclus17->Fill(float(numberOfClusters0));      // no BCS
    //if(bit126) hclus6->Fill(float(numberOfClusters0));   // bit 126
    //if(bit124) hclus7->Fill(float(numberOfClusters0));
    //if(bit122) hclus26->Fill(float(numberOfClusters0));
    //if(bcsOR)      hclus8->Fill(float(numberOfClusters0));  // bit 34
    //if(bcs)        hclus9->Fill(float(numberOfClusters0));  // all bcs except 34
    //if(bcs_bptx)   hclus29->Fill(float(numberOfClusters0)); // bits 124,126
    //if(bcs_double) hclus28->Fill(float(numberOfClusters0)); // 36-39, 40-43
    //if(bit32_33)   hclus25->Fill(float(numberOfClusters0)); // bits 32,33? is this usefull
    //if(halo)       hclus3->Fill(float(numberOfClusters0));  // bits 36-39
    //if(bit85) hclus18->Fill(float(numberOfClusters0));      // bit85
    //if(minBias) hclus19->Fill(float(numberOfClusters0));    // bits 40,41

    // Check L1 bits with pixel selection
    if (L1GTRR.isValid()) {
      //bool l1a = L1GTRR->decision();
      //cout<<" L1 status :"<<l1a<<" "<<hex;
      for (unsigned int i = 0; i < L1GTRR->decisionWord().size(); ++i) {
	int l1flag = L1GTRR->decisionWord()[i]; 
	int t1flag = L1GTRR->technicalTriggerWord()[i]; 	
	if( l1flag>0 )        hl1a1->Fill(float(i));
	if( t1flag>0 && i<64) hl1t1->Fill(float(i));
      } // for loop
    } // if l1a

#endif // L1

    // HLT bits
#ifdef HLT
    for (unsigned int i=0;i<256;i++) if(hlt[i]) hlt3->Fill(float(i));
#endif    
  } // if select event

#endif // HISTOS
      
} // end 

//define this as a plug-in
DEFINE_FWK_MODULE(PixClusterAna);
