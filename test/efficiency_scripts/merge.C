#include <sys/stat.h>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TList.h"
#include "TLegend.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include "TCanvas.h"
#include "TGaxis.h"
#include "TGraphAsymmErrors.h"

  using namespace std;
  bool DEBUG=false;
  int Nfiles=628;
  
void mergeHisto(const char*,TH1F*,bool=false);
void mergeHisto(TH1F*,bool=false);			//NEW, TAKES NAME FROM MERGED
void mergeHisto(const char*,TH2F*,bool=false);
void mergeHisto(TH2F*,bool=false);			//NEW, TAKES NAME FROM MERGED
void mergeHistoWithLabel(char*,TH1F*);
void makeEfficiency(TH1F*,TH1F*,TH1F*,bool=true);
void makeEfficiency(TH1F*,bool=true);			//NEW, TAKES NAME FROM MERGED --- NEEDS TO BE valid*, missing*, efficiency*
void makeEfficiency(TH2F*,TH2F*,TH2F*);
void makeEfficiencyGraph(TH1F*,TH1F*,TH1F*);
void makeOperation(TH1F*,TH1F*,TH1F*,char*);
void makeMean(TH1F*, TH1F*, TH1F*);//useless, replaced by makeOperation()
void setSameLabel(TH1F*, TH1F*, bool=true);
void Canv(TH1F*,TH1F*,char*,char*,TLegend*,bool=false);


// FROM DANEK
static int module_online(int);
static int ladder_online(int,int);




void merge(){


  if(DEBUG) cout<<"********* CREATING MERGED HISTO/TTREE ************"<<endl;


  TH1F* histoMerged = new TH1F("histoMerged", "histoMerged", 3, 0, 3);

  TH1F* histLayer1Merged = new TH1F("histLayer1Merged", "histLayer1Merged", 3, 0, 3);
  TH1F* histLayer2Merged = new TH1F("histLayer2Merged", "histLayer2Merged", 3, 0, 3);
  TH1F* histLayer3Merged = new TH1F("histLayer3Merged", "histLayer3Merged", 3, 0, 3);
  TH1F* histEndcapPlusMerged  = new TH1F("histEndcapPlusMerged", "histEndcapPlusMerged", 3, 0, 3);
  TH1F* histEndcapMinusMerged = new TH1F("histEndcapMinusMerged", "histEndcapMinusMerged", 3, 0, 3);
  TH1F* histBarrelMerged = new TH1F("histBarrelMerged", "histBarrelMerged", 3, 0, 3);
  TH1F* histEndcapMerged = new TH1F("histEndcapMerged", "histEndcapMerged", 3, 0, 3);
  TH1F* validPerSubdetectorMerged   = new TH1F("validPerSubdetectorMerged", "validPerSubdetectorMerged", 5, 0, 5);
  TH1F* missingPerSubdetectorMerged = new TH1F("missingPerSubdetectorMerged", "missingPerSubdetectorMerged", 5, 0, 5);
  TH1F* validSummaryMerged   = new TH1F("validSummaryMerged", "validSummaryMerged", 3, 0, 3);
  TH1F* missingSummaryMerged = new TH1F("missingSummaryMerged", "missingSummaryMerged", 3, 0, 3);
  
  TH1F* consistencyCheckMerged     = new TH1F("consistencyCheckMerged","consistencyCheckMerged", 3, 0,3);
  TH1F* consistencyCheckTrajMerged = new TH1F("consistencyCheckTrajMerged","consistencyCheckTrajMerged", 5, 0,5);
    consistencyCheckTrajMerged->GetXaxis()->SetBinLabel(1,"Total Tracks");
    consistencyCheckTrajMerged->GetXaxis()->SetBinLabel(2,"in Tracker");
    consistencyCheckTrajMerged->GetXaxis()->SetBinLabel(3,"in Pixel");
    consistencyCheckTrajMerged->GetXaxis()->SetBinLabel(4,"in BPix");
    consistencyCheckTrajMerged->GetXaxis()->SetBinLabel(5,"in FPix");
  TH1F* trackingEfficiencyMerged = new TH1F("trackingEfficiencyMerged","trackingEfficiencyMerged", 2, 0, 2);
    trackingEfficiencyMerged->GetXaxis()->SetBinLabel(1,"tracks in pix volume");
    trackingEfficiencyMerged->GetXaxis()->SetBinLabel(2,"+ with 1 valid pix hit");
  
  TH1F* validPerRunMerged = new  TH1F("validPerRunMerged","validPerRunMerged",200,0,200);
  TH1F* missingPerRunMerged = new  TH1F("missingPerRunMerged","missingPerRunMerged",200,0,200);
  TH1F* invalidPerRunMerged = new  TH1F("invalidPerRunMerged","invalidPerRunMerged",200,0,200);
  TH1F* inactivePerRunMerged = new  TH1F("inactivePerRunMerged","inactivePerRunMerged",200,0,200);
  TH1F* efficiencyPerRun = new  TH1F("efficiencyPerRun","efficiencyPerRun",200,0,200);
  TH1F* inactivePercentPerRun = new  TH1F("inactivePercentPerRun","inactivePercentPerRun",200,0,200);

  TH1F* validVsAlphaMerged = new TH1F("validVsAlphaMerged","validVsAlphaMerged",200,-3.5,3.5);
  TH1F* validVsCotanAlphaMerged = new TH1F("validVsCotanAlphaMerged","validVsCotanAlphaMerged",200,-3.5,3.5);
  TH1F* validVsAlphaBPixMerged = new TH1F("validVsAlphaBPixMerged","validVsAlphaBPixMerged",200,-3.5,3.5);
  TH1F* validVsAlphaFPixMerged = new TH1F("validVsAlphaFPixMerged","validVsAlphaFPixMerged",200,-3.5,3.5);
  TH1F* missingVsAlphaMerged = new TH1F("missingVsAlphaMerged","missingVsAlphaMerged",200,-3.5,3.5);
  TH1F* missingVsCotanAlphaMerged = new TH1F("missingVsCotanAlphaMerged","missingVsCotanAlphaMerged",200,-3.5,3.5);
  TH1F* missingVsAlphaBPixMerged = new TH1F("missingVsAlphaBPixMerged","missingVsAlphaBPixMerged",200,-3.5,3.5);
  TH1F* missingVsAlphaFPixMerged = new TH1F("missingVsAlphaFPixMerged","missingVsAlphaFPixMerged",200,-3.5,3.5);
  TH1F* validVsBetaMerged = new TH1F("validVsBetaMerged","validVsBetaMerged",200,-3.5,3.5);
  TH1F* validVsBetaBPixMerged = new TH1F("validVsBetaBPixMerged","validVsBetaBPixMerged",200,-3.5,3.5);
  TH1F* validVsBetaFPixMerged = new TH1F("validVsBetaFPixMerged","validVsBetaFPixMerged",200,-3.5,3.5);
  TH1F* missingVsBetaMerged = new TH1F("missingVsBetaMerged","missingVsBetaMerged",200,-3.5,3.5);
  TH1F* missingVsBetaBPixMerged = new TH1F("missingVsBetaBPixMerged","missingVsBetaBPixMerged",200,-3.5,3.5);
  TH1F* missingVsBetaFPixMerged = new TH1F("missingVsBetaFPixMerged","missingVsBetaFPixMerged",200,-3.5,3.5);

  TH2F* validAlphaBetaMerged   = new TH2F("validAlphaBetaMerged"  ,"validAlphaBetaMerged"  ,50,-3.5,3.5,50,-3.5,3.5);
  TH2F* missingAlphaBetaMerged = new TH2F("missingAlphaBetaMerged","missingAlphaBetaMerged",50,-3.5,3.5,50,-3.5,3.5);
  TH2F* alphaBetaEfficiency    = new TH2F("alphaBetaEfficiency"   ,"alphaBetaEfficiency"   ,50,-3.5,3.5,50,-3.5,3.5);
  
  TH1F* validVsLocalXMerged = new TH1F("validVsLocalXMerged","validVsLocalXMerged;X [cm];nValid",100,-1.5,1.5);
  TH1F* validVsLocalYMerged = new TH1F("validVsLocalYMerged","validVsLocalYMerged;X [cm];nValid",100,-4.,4.);
  TH1F* missingVsLocalXMerged = new TH1F("missingVsLocalXMerged","missingVsLocalXMerged;X [cm];nMissing",100,-1.5,1.5);
  TH1F* missingVsLocalYMerged = new TH1F("missingVsLocalYMerged","missingVsLocalYMerged;X [cm];nMissing",100,-4.,4.);

  TH1F* validVsMuontimePre68094Merged    = new TH1F("validVsMuontimePre68094Merged","validVsMuontimePre68094Merged",50,-40.,80.);
  TH1F* missingVsMuontimePre68094Merged  = new TH1F("missingVsMuontimePre68094Merged","missingVsMuontimePre68094Merged",50,-40.,80.);
  TH1F* validVsMuontimePost68094Merged   = new TH1F("validVsMuontimePost68094Merged","validVsMuontimePost68094Merged",50,-40.,80.);
  TH1F* missingVsMuontimePost68094Merged = new TH1F("missingVsMuontimePost68094Merged","missingVsMuontimePost68094Merged",50,-40.,80.);

  TH1F* validVsPTMerged   = new TH1F("validVsPTMerged","validVsPTMerged",100,0.,50.);
  TH1F* missingVsPTMerged = new TH1F("missingVsPTMerged","missingVsPTMerged",100,0.,50.);

  TH1F* validVsLocalXBigMerged     = new TH1F("validVsLocalXBigMerged","validVsLocalXBigMerged;X [cm];nValid",100,-1.5,1.5);
  TH1F* missingVsLocalXBigMerged   = new TH1F("missingVsLocalXBigMerged","missingVsLocalXBigMerged;X [cm];nMissing",100,-1.5,1.5);
  TH1F* validVsLocalXSmallMerged   = new TH1F("validVsLocalXSmallMerged","validVsLocalXSmallMerged;X [cm];nValid",100,-1.5,1.5);
  TH1F* missingVsLocalXSmallMerged = new TH1F("missingVsLocalXSmallMerged","missingVsLocalXSmallMerged;X [cm];nMissing",100,-1.5,1.5);

  TH1F* localXBigEfficiency   = new TH1F("localXBigEfficiency","localXBigEfficiency",100,-1.5,1.5);
  TH1F* localXSmallEfficiency = new TH1F("localXSmallEfficiency","localXSmallEfficiency",100,-1.5,1.5);

  TH2F* validAlphaLocalXBigMerged     = new TH2F("validAlphaLocalXBigMerged","validAlphaLocalXBigMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* missingAlphaLocalXBigMerged   = new TH2F("missingAlphaLocalXBigMerged","missingAlphaLocalXBigMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* validAlphaLocalXSmallMerged   = new TH2F("validAlphaLocalXSmallMerged","validAlphaLocalXSmallMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* missingAlphaLocalXSmallMerged = new TH2F("missingAlphaLocalXSmallMerged","missingAlphaLocalXSmallMerged",200,-3.5,3.5,100,-1.5,1.5);

  TH2F* missingAlphaLocalXBigBPixMerged = new TH2F("missingAlphaLocalXBigBPixMerged","missingAlphaLocalXBigBPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* missingBetaLocalXBigBPixMerged  = new TH2F("missingBetaLocalXBigBPixMerged","missingBetaLocalXBigBPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* missingAlphaLocalYBPixMerged    = new TH2F("missingAlphaLocalYBPixMerged","missingAlphaLocalYBPixMerged",200,-3.5,3.5,100,-4,4);
  TH2F* missingBetaLocalYBPixMerged     = new TH2F("missingBetaLocalYBPixMerged","missingBetaLocalYBPixMerged",200,-3.5,3.5,100,-4,4);
  TH2F* validAlphaLocalXBigBPixMerged   = new TH2F("validAlphaLocalXBigBPixMerged","validAlphaLocalXBigBPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* validBetaLocalXBigBPixMerged    = new TH2F("validBetaLocalXBigBPixMerged","validBetaLocalXBigBPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* validAlphaLocalYBPixMerged      = new TH2F("validAlphaLocalYBPixMerged","validAlphaLocalYBPixMerged",200,-3.5,3.5,100,-4,4);
  TH2F* validBetaLocalYBPixMerged       = new TH2F("validBetaLocalYBPixMerged","validBetaLocalYBPixMerged",200,-3.5,3.5,100,-4,4);
  
  TH2F* missingAlphaLocalXBigFPixMerged = new TH2F("missingAlphaLocalXBigFPixMerged","missingAlphaLocalXBigFPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* missingBetaLocalXBigFPixMerged  = new TH2F("missingBetaLocalXBigFPixMerged","missingBetaLocalXBigFPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* missingAlphaLocalYFPixMerged    = new TH2F("missingAlphaLocalYFPixMerged","missingAlphaLocalYFPixMerged",200,-3.5,3.5,100,-4,4);
  TH2F* missingBetaLocalYFPixMerged     = new TH2F("missingBetaLocalYFPixMerged","missingBetaLocalYFPixMerged",200,-3.5,3.5,100,-4,4);
  TH2F* validAlphaLocalXBigFPixMerged   = new TH2F("validAlphaLocalXBigFPixMerged","validAlphaLocalXBigFPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* validBetaLocalXBigFPixMerged    = new TH2F("validBetaLocalXBigFPixMerged","validBetaLocalXBigFPixMerged",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* validAlphaLocalYFPixMerged      = new TH2F("validAlphaLocalYFPixMerged","validAlphaLocalYFPixMerged",200,-3.5,3.5,100,-4,4);
  TH2F* validBetaLocalYFPixMerged       = new TH2F("validBetaLocalYFPixMerged","validBetaLocalYFPixMerged",200,-3.5,3.5,100,-4,4);
 
  TH2F* efficiencyAlphaLocalXBigBPix = new TH2F("efficiencyAlphaLocalXBigBPix","efficiencyAlphaLocalXBigBPix",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* efficiencyBetaLocalXBigBPix  = new TH2F("efficiencyBetaLocalXBigBPix","efficiencyBetaLocalXBigBPix",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* efficiencyAlphaLocalYBPix    = new TH2F("efficiencyAlphaLocalYBPix","efficiencyAlphaLocalYBPix",200,-3.5,3.5,100,-4,4);
  TH2F* efficiencyBetaLocalYBPix     = new TH2F("efficiencyBetaLocalYBPix","efficiencyBetaLocalYBPix",200,-3.5,3.5,100,-4,4);
  TH2F* efficiencyAlphaLocalXBigFPix = new TH2F("efficiencyAlphaLocalXBigFPix","efficiencyAlphaLocalXBigFPix",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* efficiencyBetaLocalXBigFPix  = new TH2F("efficiencyBetaLocalXBigFPix","efficiencyBetaLocalXBigFPix",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* efficiencyAlphaLocalYFPix    = new TH2F("efficiencyAlphaLocalYFPix","efficiencyAlphaLocalYFPix",200,-3.5,3.5,100,-4,4);
  TH2F* efficiencyBetaLocalYFPix     = new TH2F("efficiencyBetaLocalYFPix","efficiencyBetaLocalYFPix",200,-3.5,3.5,100,-4,4);
  
  TH2F* alphaLocalXBigEfficiency = new TH2F("alphaLocalXBigEfficiency","alphaLocalXBigEfficiency",200,-3.5,3.5,100,-1.5,1.5);
  TH2F* alphaLocalXSmallEfficiency = new TH2F("alphaLocalXSmallEfficiency","alphaLocalXSmallEfficiency",200,-3.5,3.5,100,-1.5,1.5);

  TH1F* validChiSquareMerged   = new TH1F("validChiSquareMerged","validChiSquareMerged",200, 0., 100.);
  TH1F* missingChiSquareMerged = new TH1F("missingChiSquareMerged","missingChiSquareMerged",200, 0., 100.);
  TH1F* chiSquareEfficiency    = new TH1F("chiSquareEfficiency","chiSquareEfficiency",200, 0., 100.);

  TH1F* validChiSquareNdfMerged   = new TH1F("validChiSquareNdfMerged","validChiSquareNdfMerged",200, 0., 100.);
  TH1F* missingChiSquareNdfMerged = new TH1F("missingChiSquareNdfMerged","missingChiSquareNdfMerged",200, 0., 100.);
  TH1F* chiSquareNdfEfficiency    = new TH1F("chiSquareNdfEfficiency","chiSquareNdfEfficiency",200, 0., 100.);

  TH2F* missPerTrackVsChiSquareNdfMerged = new TH2F("missPerTrackVsChiSquareNdfMerged","missPerTrackVsChiSquareNdfMerged", 200,0.,100., 5,0,4);
  TH2F* missPerTrackPercentVsChiSquareNdfMerged = new TH2F("missPerTrackPercentVsChiSquareNdfMerged","missPerTrackPercentVsChiSquareNdfMerged", 200,0.,100.,100,0,1.);

 
  double maxwindowsearch = 0.5;
  TH1F* windowSearchMerged = new  TH1F("windowSearchMerged","windowSearchMerged",(int)(maxwindowsearch*1000),0,maxwindowsearch);
  TH1F* windowSearchSameModuleMerged = new TH1F("windowSearchSameModuleMerged","windowSearchSameModuleMerged",1000,0,10);
  TH1F* windowSearchBPixMerged = new  TH1F("windowSearchBPixMerged","windowSearchBPixMerged",(int)(maxwindowsearch*1000),0,maxwindowsearch);
  TH1F* windowSearchGoodModulesBPixMerged = new  TH1F("windowSearchGoodModulesBPixMerged","windowSearchGoodModulesBPixMerged",(int)(maxwindowsearch*1000),0,maxwindowsearch);
  TH1F* windowSearchFPixMerged = new  TH1F("windowSearchFPixMerged","windowSearchFPixMerged",(int)(maxwindowsearch*1000),0,maxwindowsearch);
  TH1F* missingButClusterOnSameModuleMerged = new TH1F("missingButClusterOnSameModuleMerged","missingButClusterOnSameModuleMerged",2,0,2);
  TH1F* missingButClusterMerged = new TH1F("missingButClusterMerged","missingButClusterMerged",2,0,2);

  vector<TH1F*> windowSearch_pTMerged;
  int ptranges=11;
  for (int p=0;p<=ptranges;p++)
    windowSearch_pTMerged.push_back(0);
  for (int p=0;p<=ptranges;p++){
    char basename[20];
    sprintf (basename,"windowSearch_pTMerged_range%i",p) ;
    std::string m_baseName = basename ;
    windowSearch_pTMerged[p]= new TH1F( (m_baseName).c_str(),(m_baseName).c_str(),(int)(maxwindowsearch*1000),0,maxwindowsearch);;
    }
   
  TH1F* histoMethod2Merged = new TH1F("histoMethod2Merged", "histoMethod2Merged", 2, 0, 2);
  TH1F* histoMethod2AfterMerged = new TH1F("histoMethod2AfterMerged", "histoMethod2AfterMerged", 2, 0, 2);
  TH1F* histoMethod2FPixMerged = new TH1F("histoMethod2FPixMerged", "histoMethod2FPixMerged", 2, 0, 2);
  TH1F* histoMethod2AfterFPixMerged = new TH1F("histoMethod2AfterFPixMerged", "histoMethod2AfterFPixMerged", 2, 0, 2);

  //invalidity version
  TH1F* histInvalidRecHitCollectionMerged = new TH1F("histInvalidRecHitCollectionMerged","histInvalidRecHitCollectionMerged", 4, 0,4);
  TH1F* histInvalidRecHitWithBadmoduleListMerged = new TH1F("histInvalidRecHitWithBadmoduleListMerged","histInvalidRecHitWithBadmoduleListMerged",5,0,5);
  
  //test
  TH1F* checkoutTrajMerged = new TH1F("checkoutTrajMerged","checkoutTrajMerged",10,0,10); 
  TH1F* checkoutValidityFlagMerged = new TH1F("checkoutValidityFlagMerged","checkoutValidityFlagMerged",4,0,4); 

  //TH1F* totalPerTrackMerged= new TH1F ("totalPerTrackMerged","totalPerTrackMerged",20,0,20);
  TH1F* inactivePerTrackMerged= new TH1F ("inactivePerTrackMerged","inactivePerTrackMerged",101,0,101);
  TH1F* missingPerTrackMerged= new TH1F ("missingPerTrackMerged","missingPerTrackMerged",101,0,101);

  TH2F* xPosFracValMerged = new TH2F("xPosFracVal","xPosFracVal", 101,0,1.01, 6,0,6);
  TH2F* xPosFracMisMerged = new TH2F("xPosFracMis","xPosFracMis", 101,0,1.01 ,6,0,6);
  TH2F* yPosFracValMerged = new TH2F("yPosFracVal","yPosFracVal", 101,0,1.01 ,6,0,6);
  TH2F* yPosFracMisMerged = new TH2F("yPosFracMis","yPosFracMis", 101,0,1.01 ,6,0,6);
 
  TH1F* hitsPassingCutsValMerged = new TH1F("hitsPassingCutsValMerged","hitsPassingCutsValMerged",10,0,10);
  TH1F* hitsPassingCutsMisMerged = new TH1F("hitsPassingCutsMisMerged","hitsPassingCutsMisMerged",10,0,10);
  TH1F* hitsPassingCutsValBPixMerged = new TH1F("hitsPassingCutsValBPixMerged","hitsPassingCutsValBPixMerged",10,0,10);
  TH1F* hitsPassingCutsMisBPixMerged = new TH1F("hitsPassingCutsMisBPixMerged","hitsPassingCutsMisBPixMerged",10,0,10);
  TH1F* hitsPassingCutsValFPixMerged = new TH1F("hitsPassingCutsValFPixMerged","hitsPassingCutsValFPixMerged",10,0,10);
  TH1F* hitsPassingCutsMisFPixMerged = new TH1F("hitsPassingCutsMisFPixMerged","hitsPassingCutsMisFPixMerged",10,0,10);

  TH1F* numTracksVsMuonTimeMerged = new TH1F("numTracksVsMuonTimeMerged","numTracksVsMuonTimeMerged",80,-40,40);
  TH1F* denTracksVsMuonTimeMerged = new TH1F("denTracksVsMuonTimeMerged","denTracksVsMuonTimeMerged",80,-40,40);
  TH1F* effTracksVsMuonTimeMerged = new TH1F("effTracksVsMuonTimeMerged","effTracksVsMuonTimeMerged",80,-40,40);

  TH2F* xy_standardDevMerged = new TH2F("xy_standardDevMerged","xy_standardDevMerged",100,0,1.5,300,0,4);
  TH2F* xy_standardDev_insideModuleMerged = new TH2F("xy_standardDev_insideModuleMerged","xy_standardDev_insideModuleMerged",100,0,1.5,300,0,4);

  vector< vector<double> > tempTree;
  int id, isModuleBad, inactive, missing, valid, isBarrelModule, ladder, blade, moduleInLadder;
  double globalX, globalY, globalZ;
  
  TTree *treeMerged = new TTree("moduleAnalysisMerged","moduleAnalysisMerged");
  treeMerged->Branch("id",&id,"id/I");
  treeMerged->Branch("isModuleBad",&isModuleBad,"isModuleBad/I");
  treeMerged->Branch("inactive",&inactive,"inactive/I");
  treeMerged->Branch("missing",&missing,"missing/I");
  treeMerged->Branch("valid",&valid,"valid/I");
  treeMerged->Branch("isBarrelModule",&isBarrelModule,"isBarrelModule/I");
  treeMerged->Branch("ladder",&ladder,"ladder/I");
  treeMerged->Branch("blade",&blade,"blade/I");
  treeMerged->Branch("moduleInLadder",&moduleInLadder,"moduleInLadder/I");
  treeMerged->Branch("globalX",&globalX,"globalX/D");
  treeMerged->Branch("globalY",&globalY,"globalY/D");
  treeMerged->Branch("globalZ",&globalZ,"globalZ/D");
  
  TH1F* histSubdetectors = new TH1F("histSubdetectors", "histSubdetectors", 5, 0, 5);
  TH1F* histSummary = new TH1F("histSummary", "histSummary", 3, 0, 3);
  TH1F* efficiencyPerSubdetector = new TH1F("efficiencyPerSubdetector", "efficiencyPerSubdetector", 5, 0, 5);
  TH1F* efficiencySummary = new TH1F("efficiencySummary", "efficiencySummary", 3, 0, 3);
  

  TH1F* effDistriLayer1 = new TH1F("effDistriLayer1","effDistriLayer1",101,0,1.01);
  TH1F* effDistriLayer2 = new TH1F("effDistriLayer2","effDistriLayer2",101,0,1.01);
  TH1F* effDistriLayer3 = new TH1F("effDistriLayer3","effDistriLayer3",101,0,1.01);
  TH1F* effDistriFpixPlus = new TH1F("effDistriFpixPlus","effDistriFpixPlus",101,0,1.01);
  TH1F* effDistriFpixMinus = new TH1F("effDistriFpixMinus","effDistriFpixMinus",101,0,1.01);
  
//cluster analysis histos
  TH1F* chargeDistriPreCutsMerged = new TH1F("chargeDistriPreCutsMerged","chargeDistriPreCutsMerged",200,0,100000);
  TH1F* numbPixInClusterPreCutsMerged = new TH1F("numbPixInClusterPreCutsMerged","numbPixInClusterPreCutsMerged",50,0,50);
  TH1F* chargeDistriBPixPreCutsMerged = new TH1F("chargeDistriBPixPreCutsMerged","chargeDistriBPixPreCutsMerged",200,0,100000);
  TH1F* numbPixInClusterBPixPreCutsMerged = new TH1F("numbPixInClusterBPixPreCutsMerged","numbPixInClusterBPixPreCutsMerged",50,0,50);
  TH1F* chargeDistriFPixPlusPreCutsMerged = new TH1F("chargeDistriFPixPlusPreCutsMerged","chargeDistriFPixPlusPreCutsMerged",200,0,100000);
  TH1F* numbPixInClusterFPixPlusPreCutsMerged = new TH1F("numbPixInClusterFPixPlusPreCutsMerged","numbPixInClusterFPixPlusPreCutsMerged",50,0,50);  
  TH1F* chargeDistriFPixMinusPreCutsMerged = new TH1F("chargeDistriFPixMinusPreCutsMerged","chargeDistriFPixMinusPreCutsMerged",200,0,100000);
  TH1F* numbPixInClusterFPixMinusPreCutsMerged = new TH1F("numbPixInClusterFPixMinusPreCutsMerged","numbPixInClusterFPixMinusPreCutsMerged",50,0,50);  

  TH2F* chargeVsDimensionBPixMerged = new TH2F ("chargeVsDimensionBPixMerged","chargeVsDimensionBPixMerged",200,0,100000,50,-0.5,49.5); ///// CHANGED BINNING
    chargeVsDimensionBPixMerged->GetYaxis()->SetRangeUser(0.,20.);
    
  TH1F* chargeDistriMerged = new TH1F("chargeDistriMerged","chargeDistriMerged",200,0,100000);
  TH1F* numbPixInClusterMerged = new TH1F("numbPixInClusterMerged","numbPixInClusterMerged",25,0,25);
  TH1F* chargeDistriBPixMerged = new TH1F("chargeDistriBPixMerged","chargeDistriBPixMerged",200,0,100000);
  TH1F* numbPixInClusterBPixMerged = new TH1F("numbPixInClusterBPixMerged","numbPixInClusterBPixMerged",25,-0.5,24.5); ///// CHANGED BINNING
  TH1F* chargeDistriFPixPlusMerged = new TH1F("chargeDistriFPixPlusMerged","chargeDistriFPixPlusMerged",200,0,100000);
  TH1F* numbPixInClusterFPixPlusMerged = new TH1F("numbPixInClusterFPixPlusMerged","numbPixInClusterFPixPlusMerged",25,0,25);  
  TH1F* chargeDistriFPixMinusMerged = new TH1F("chargeDistriFPixMinusMerged","chargeDistriFPixMinusMerged",200,0,100000);
  TH1F* numbPixInClusterFPixMinusMerged = new TH1F("numbPixInClusterFPixMinusMerged","numbPixInClusterFPixMinusMerged",25,0,25);  
    chargeDistriMerged->GetXaxis()->SetTitle("cluster_{charge}");
    numbPixInClusterMerged->GetXaxis()->SetTitle("cluster_{dimension}");
    chargeDistriBPixMerged->GetXaxis()->SetTitle("cluster_{charge}");
    numbPixInClusterBPixMerged->GetXaxis()->SetTitle("cluster_{dimension}");
    chargeDistriFPixPlusMerged->GetXaxis()->SetTitle("cluster_{charge}");
    numbPixInClusterFPixPlusMerged->GetXaxis()->SetTitle("cluster_{dimension}");  
    chargeDistriFPixMinusMerged->GetXaxis()->SetTitle("cluster_{charge}");
    numbPixInClusterFPixMinusMerged->GetXaxis()->SetTitle("cluster_{dimension}");  
    chargeDistriMerged->GetYaxis()->SetTitle("a.u.");
    numbPixInClusterMerged->GetYaxis()->SetTitle("a.u.");
    chargeDistriBPixMerged->GetYaxis()->SetTitle("a.u.");
    numbPixInClusterBPixMerged->GetYaxis()->SetTitle("a.u.");
    chargeDistriFPixPlusMerged->GetYaxis()->SetTitle("a.u.");
    numbPixInClusterFPixPlusMerged->GetYaxis()->SetTitle("a.u.");  
    chargeDistriFPixMinusMerged->GetYaxis()->SetTitle("a.u.");
    numbPixInClusterFPixMinusMerged->GetYaxis()->SetTitle("a.u.");  
  
  TH1F* numbPixInClusterXMerged = new TH1F("numbPixInClusterXMerged","numbPixInClusterXMerged",30,0,30);
  TH1F* numbPixInClusterYMerged = new TH1F("numbPixInClusterYMerged","numbPixInClusterYMerged",30,0,30);
  TH1F* xposClusterMerged = new TH1F("xposClusterMerged","xposClusterMerged",200,-2,2);
  TH1F* yposClusterMerged = new TH1F("yposClusterMerged","yposClusterMerged",500,-10,10);
  TH1F* xposClusterValidMerged = new TH1F("xposClusterValidMerged","xposClusterValidMerged",200,-2,2);
  TH1F* yposClusterValidMerged = new TH1F("yposClusterValidMerged","yposClusterValidMerged",500,-10,10);
  TH1F* xposClusterMisRecoveredMerged = new TH1F("xposClusterMisRecoveredMerged","xposClusterMisRecoveredMerged",200,-2,2);
  TH1F* yposClusterMisRecoveredMerged = new TH1F("yposClusterMisRecoveredMerged","yposClusterMisRecoveredMerged",500,-10,10);

  TH2F* tunningValMerged     = new TH2F("tunningValMerged" ,"tunningValMerged" ,50,0,5,40,0,40);
  TH2F* tunningMisMerged     = new TH2F("tunningMisMerged" ,"tunningMisMerged" ,50,0,5,40,0,40);
  TH1F* tunningEdgeValMerged = new TH1F("tunningEdgeValMerged","tunningEdgeValMerged",50,0,5);
  TH1F* tunningEdgeMisMerged = new TH1F("tunningEdgeMisMerged","tunningEdgeMisMerged",50,0,5);
  TH1F* tunningMuonValMerged = new TH1F("tunningMuonValMerged","tunningMuonValMerged",40,0,40);
  TH1F* tunningMuonMisMerged = new TH1F("tunningMuonMisMerged","tunningMuonMisMerged",40,0,40);
    
  TH1F* histAlphaAnalysis      = new TH1F("histAlphaAnalysis", "histAlphaAnalysis", 200, -3.5,3.5);
  TH1F* histAlphaAnalysisBPix  = new TH1F("histAlphaAnalysisBPix", "histAlphaAnalysisBPix", 200, -3.5,3.5);
  TH1F* histAlphaAnalysisFPix  = new TH1F("histAlphaAnalysisFPix", "histAlphaAnalysisFPix", 200, -3.5,3.5);
  TH1F* histCotanAlphaAnalysis = new TH1F("histCotanAlphaAnalysis", "histCotanAlphaAnalysis", 200, -3.5,3.5); 
  TH1F* histBetaAnalysis       = new TH1F("histBetaAnalysis", "histBetaAnalysis", 200, -3.5,3.5); 
  TH1F* histBetaAnalysisBPix   = new TH1F("histBetaAnalysisBPix", "histBetaAnalysisBPix", 200, -3.5,3.5); 
  TH1F* histBetaAnalysisFPix   = new TH1F("histBetaAnalysisFPix", "histBetaAnalysisFPix", 200, -3.5,3.5); 
  
  TGraphAsymmErrors*  histAlphaAnalysisBPixGraph;
  TGraphAsymmErrors*  histBetaAnalysisBPixGraph;
  
  TH2F* tunningEfficiency                = new TH2F("tunningEfficiency" ,"tunningEfficiency" ,50,0,5,40,0,40);
  TH1F* tunningEdgeEfficiency            = new TH1F("tunningEdgeEfficiency","tunningEdgeEfficiency",50,0,5);
  TH1F* tunningMuonEfficiency            = new TH1F("tunningMuonEfficiency","tunningMuonEfficiency",40,0,40);
  TH1F* tunningSlice                     = new TH1F("tunningSlice","tunningSlice",40,0,40);
  TH1F* nTotVsTunningMuon                = new TH1F("nTotVsTunningMuon","nTotVsTunningMuon",40,0,40); 

  TH1F* muontimePre68094Efficiency       = new TH1F("muontimePre68094Efficiency","muontimePre68094Efficiency",50,-40.,80.);
  TH1F* muontimePost68094Efficiency      = new TH1F("muontimePost68094Efficiency","muontimePost68094Efficiency",50,-40.,80.);

 TH2F* validMuonTimeVSchargeBPixMerged   = new TH2F("validMuonTimeVSchargeBPixMerged","validMuonTimeVSchargeBPixMerged",60,-40.,80.,200,0,200000);
 TH2F* missingMuonTimeVSchargeBPixMerged = new TH2F("missingMuonTimeVSchargeBPixMerged","missingMuonTimeVSchargeBPixMerged",60,-40.,80.,200,0,200000);
 TH2F* validMuonTimeVSchargeFPixMerged   = new TH2F("validMuonTimeVSchargeFPixMerged","validMuonTimeVSchargeFPixMerged",60,-40.,80.,200,0,200000);
 TH2F* missingMuonTimeVSchargeFPixMerged = new TH2F("missingMuonTimeVSchargeFPixMerged","missingMuonTimeVSchargeFPixMerged",60,-40.,80.,200,0,200000);

 TH2F* efficiencyMuonTimeVSchargeBPix    = new TH2F("efficiencyMuonTimeVSchargeBPix","efficiencyMuonTimeVSchargeBPix",60,-40.,80.,200,0,200000);
 TH2F* efficiencyMuonTimeVSchargeFPix    = new TH2F("efficiencyMuonTimeVSchargeFPix","efficiencyMuonTimeVSchargeFPix",60,-40.,80.,200,0,200000);

 TH1F* PTEfficiency                      = new TH1F ("PTEfficiency","PTEfficiency",100,0,50);

 TH1F* muonTimeVSRunNumberMerged         = new TH1F("muonTimeVSRunNumberMerged","muonTimeVSRunNumberMerged",200,0,200);
 TH1F* nMuonTimeVSRunNumberMerged        = new TH1F("nMuonTimeVSRunNumberMerged","nMuonTimeVSRunNumberMerged",200,0,200);
 TH1F* muonTimeErrorVSRunNumberMerged    = new TH1F("muonTimeErrorVSRunNumberMerged","muonTimeErrorVSRunNumberMerged",200,0,200);
 TH1F* nMuonTimeErrorVSRunNumberMerged   = new TH1F("nMuonTimeErrorVSRunNumberMerged","nMuonTimeErrorVSRunNumberMerged",200,0,200);
 TH1F* muonTimeErrorDistriMerged         = new TH1F("muonTimeErrorDistriMerged","muonTimeErrorDistriMerged",80,0,40);
 TH1F* validVSMuonTimeErrorMerged        = new TH1F("validVSMuonTimeErrorMerged","validVSMuonTimeErrorMerged",80,0,40);
 TH1F* missingVSMuonTimeErrorMerged      = new TH1F("missingVSMuonTimeErrorMerged","missingVSMuonTimeErrorMerged",80,0,40);
 TH1F* muonTimeNdofDistriMerged          = new TH1F("muonTimeNdofDistriMerged","muonTimeNdofDistriMerged",50,0,50);
 TH1F* validVSMuonTimeNdofMerged         = new TH1F("validVSMuonTimeNdofMerged","validVSMuonTimeNdofMerged",50,0,50);
 TH1F* missingVSMuonTimeNdofMerged       = new TH1F("missingVSMuonTimeNdofMerged","missingVSMuonTimeNdofMerged",50,0,50);

 TH1F* efficiencyVSMuonTimeError         = new TH1F("efficiencyVSMuonTimeError","efficiencyVSMuonTimeError",80,0,40);
 TH1F* efficiencyVSMuonTimeNdof         = new TH1F("efficiencyVSMuonTimeNdof","efficiencyVSMuonTimeNdof",50,0,50);
 TH1F* meanMuonTimeVSRunNumber           = new TH1F("meanMuonTimeVSRunNumber","meanMuonTimeVSRunNumber",200,0,200);
 TH1F* meanMuonTimeErrorVSRunNumber       = new TH1F("meanMuonTimeErrorVSRunNumber","meanMuonTimeErrorVSRunNumber",200,0,200);

  char name[120];
  for(int i=1;i<=Nfiles;i++){
  
    sprintf(name,"pixelEfficiency%d.root",i);
        
    cout<<name<<endl;
  
    TFile* myFile = TFile::Open(name);
    if(myFile==0) continue;
    
    //TList list = 
    if((myFile->GetListOfKeys())->First()==0) {cout<< "IS VOID" << endl; continue;}
    
    myFile->cd();
    
    if(DEBUG) cout<<"********* FILE OPENED, STARTING TO MERGE ************"<<endl;
          
    mergeHisto("histo",histoMerged);
    mergeHisto("histLayer1",histLayer1Merged);
    mergeHisto("histLayer2",histLayer2Merged);
    mergeHisto("histLayer3",histLayer3Merged);
    mergeHisto("histEndcapPlus",histEndcapPlusMerged);
    mergeHisto("histEndcapMinus",histEndcapMinusMerged);
    mergeHisto("histBarrel",histBarrelMerged);
    mergeHisto("histEndcap",histEndcapMerged);
    mergeHisto("consistencyCheck",consistencyCheckMerged);
    mergeHisto("consistencyCheckTraj",consistencyCheckTrajMerged);
    mergeHisto("trackingEfficiency",trackingEfficiencyMerged);
    
    mergeHisto("validPerSubdetector",validPerSubdetectorMerged);
    mergeHisto("missingPerSubdetector",missingPerSubdetectorMerged);
    validSummaryMerged->Fill(0.5,validPerSubdetectorMerged->GetBinContent(1)
    				+validPerSubdetectorMerged->GetBinContent(2)
    				+validPerSubdetectorMerged->GetBinContent(3));
    validSummaryMerged->Fill(1.5,validPerSubdetectorMerged->GetBinContent(4)
    				+validPerSubdetectorMerged->GetBinContent(5));
    validSummaryMerged->Fill(2.5,validSummaryMerged->GetBinContent(1)
    				+validSummaryMerged->GetBinContent(2));
    missingSummaryMerged->Fill(0.5,missingPerSubdetectorMerged->GetBinContent(1)
    				  +missingPerSubdetectorMerged->GetBinContent(2)
    				  +missingPerSubdetectorMerged->GetBinContent(3));
    missingSummaryMerged->Fill(1.5,missingPerSubdetectorMerged->GetBinContent(4)
    				  +missingPerSubdetectorMerged->GetBinContent(5));
    missingSummaryMerged->Fill(2.5,missingSummaryMerged->GetBinContent(1)
    				  +missingSummaryMerged->GetBinContent(2));
      
    mergeHisto("histInvalidRecHitWithBadmoduleList",histInvalidRecHitWithBadmoduleListMerged);
    mergeHisto("histInvalidRecHitCollection",histInvalidRecHitCollectionMerged);
    
    mergeHisto("histoMethod2",histoMethod2Merged);
    mergeHisto("histoMethod2After",histoMethod2AfterMerged);
    mergeHisto("histoMethod2FPix",histoMethod2FPixMerged);
    mergeHisto("histoMethod2AfterFPix",histoMethod2AfterFPixMerged);
    
    if(DEBUG) cout<<"********* NOW MERGING ANALYSIS PER RUN ************"<<endl;


    mergeHistoWithLabel("invalidPerRun",invalidPerRunMerged);
    mergeHistoWithLabel("validPerRun",validPerRunMerged);
    mergeHistoWithLabel("inactivePerRun",inactivePerRunMerged);
    
    mergeHistoWithLabel("muonTimeVSRunNumber",muonTimeVSRunNumberMerged);
    mergeHistoWithLabel("nMuonTimeVSRunNumber",nMuonTimeVSRunNumberMerged);
    mergeHistoWithLabel("muonTimeErrorVSRunNumber",muonTimeErrorVSRunNumberMerged);
    mergeHistoWithLabel("nMuonTimeErrorVSRunNumber",nMuonTimeErrorVSRunNumberMerged);
    
    mergeHisto("muonTimeErrorDistri",muonTimeErrorDistriMerged);
    mergeHisto("validVSMuonTimeError",validVSMuonTimeErrorMerged);
    mergeHisto("missingVSMuonTimeError",missingVSMuonTimeErrorMerged);
    mergeHisto("muonTimeNdofDistri",muonTimeNdofDistriMerged);
    mergeHisto("validVSMuonTimeNdof",validVSMuonTimeNdofMerged);
    mergeHisto("missingVSMuonTimeNdof",missingVSMuonTimeNdofMerged);
    

    if(DEBUG) cout<<"********* NOW MERGING ANALYSIS FOR TRAJECTORY ************"<<endl;


    TH1F *htest = (TH1F*)gDirectory->Get( "checkoutTraj" );
    for (int bin=1; bin<=10; bin++)
      checkoutTrajMerged->SetBinContent( bin, checkoutTrajMerged->GetBinContent(bin) + htest->GetBinContent(bin));  
    delete htest;
    
//update to alpha and beta
    mergeHisto("validVsAlpha",validVsAlphaMerged);
    mergeHisto("validVsCotanAlpha",validVsCotanAlphaMerged);
    mergeHisto("validVsAlphaBPix",validVsAlphaBPixMerged);
    mergeHisto("validVsAlphaFPix",validVsAlphaFPixMerged);
    mergeHisto("validVsBeta",validVsBetaMerged);
    mergeHisto("validVsBetaBPix",validVsBetaBPixMerged);
    mergeHisto("validVsBetaFPix",validVsBetaFPixMerged);
    mergeHisto("missingVsAlpha",missingVsAlphaMerged);
    mergeHisto("missingVsAlphaBPix",missingVsAlphaBPixMerged);
    mergeHisto("missingVsAlphaFPix",missingVsAlphaFPixMerged);
    mergeHisto("missingVsCotanAlpha",missingVsCotanAlphaMerged);
    mergeHisto("missingVsBeta",missingVsBetaMerged);
    mergeHisto("missingVsBetaBPix",missingVsBetaBPixMerged);
    mergeHisto("missingVsBetaFPix",missingVsBetaFPixMerged);
    
    mergeHisto("missingVsLocalX",missingVsLocalXMerged);
    mergeHisto("missingVsLocalY",missingVsLocalYMerged);
    mergeHisto("validVsLocalX",validVsLocalXMerged);
    mergeHisto("validVsLocalY",validVsLocalYMerged);
    
    mergeHisto("inactivePerTrack",inactivePerTrackMerged);
    mergeHisto("missingPerTrack",missingPerTrackMerged);
    
    mergeHisto("checkoutValidityFlag",checkoutValidityFlagMerged);


    if(DEBUG) cout<<"********* NOW MERGING analysis for numerator and denominator ************"<<endl;
   
    mergeHisto("xPosFracVal",xPosFracValMerged,true);
    mergeHisto("xPosFracMis",xPosFracMisMerged,true);
    mergeHisto("yPosFracVal",yPosFracValMerged,true);
    mergeHisto("yPosFracMis",yPosFracMisMerged,true);


    mergeHisto("hitsPassingCutsValBPix",hitsPassingCutsValBPixMerged);
    mergeHisto("hitsPassingCutsMisBPix",hitsPassingCutsMisBPixMerged);
    mergeHisto("hitsPassingCutsValFPix",hitsPassingCutsValFPixMerged);
    mergeHisto("hitsPassingCutsMisFPix",hitsPassingCutsMisFPixMerged);
    
 
    if(DEBUG) cout<<"********* NOW MERGING *EKLUND* plots ************"<<endl;
    mergeHisto("validAlphaBeta",validAlphaBetaMerged);
    mergeHisto("missingAlphaBeta",missingAlphaBetaMerged);
    mergeHisto("validVsLocalXBig",validVsLocalXBigMerged);
    mergeHisto("missingVsLocalXBig",missingVsLocalXBigMerged);
    mergeHisto("validVsLocalXSmall",validVsLocalXSmallMerged);
    mergeHisto("missingVsLocalXSmall",missingVsLocalXSmallMerged);
    mergeHisto("validAlphaLocalXBig",validAlphaLocalXBigMerged);
    mergeHisto("missingAlphaLocalXBig",missingAlphaLocalXBigMerged);
    mergeHisto("validAlphaLocalXSmall",validAlphaLocalXSmallMerged);
    mergeHisto("missingAlphaLocalXSmall",missingAlphaLocalXSmallMerged);
    
    
    if(DEBUG) cout<<"********* NOW MERGING *Correlation* analysis ************"<<endl;
    mergeHisto("missingAlphaLocalXBigBPix",missingAlphaLocalXBigBPixMerged);
    mergeHisto("missingBetaLocalXBigBPix",missingBetaLocalXBigBPixMerged);
    mergeHisto("missingAlphaLocalYBPix",missingAlphaLocalYBPixMerged);
    mergeHisto("missingBetaLocalYBPix",missingBetaLocalYBPixMerged);
    mergeHisto("validAlphaLocalXBigBPix",validAlphaLocalXBigBPixMerged);
    mergeHisto("validBetaLocalXBigBPix",validBetaLocalXBigBPixMerged);
    mergeHisto("validAlphaLocalYBPix",validAlphaLocalYBPixMerged);
    mergeHisto("validBetaLocalYBPix",validBetaLocalYBPixMerged);
  
    mergeHisto("missingAlphaLocalXBigFPix",missingAlphaLocalXBigFPixMerged);
    mergeHisto("missingBetaLocalXBigFPix",missingBetaLocalXBigFPixMerged);
    mergeHisto("missingAlphaLocalYFPix",missingAlphaLocalYFPixMerged);
    mergeHisto("missingBetaLocalYFPix",missingBetaLocalYFPixMerged);
    mergeHisto("validAlphaLocalXBigFPix",validAlphaLocalXBigFPixMerged);
    mergeHisto("validBetaLocalXBigFPix",validBetaLocalXBigFPixMerged);
    mergeHisto("validAlphaLocalYFPix",validAlphaLocalYFPixMerged);
    mergeHisto("validBetaLocalYFPix",validBetaLocalYFPixMerged);
    
    if(DEBUG) cout<<"********* NOW MERGING *ChiSquare* analysis ************"<<endl;
    mergeHisto("validChiSquare",validChiSquareMerged);
    mergeHisto("missingChiSquare",missingChiSquareMerged);
    mergeHisto("validChiSquareNdf",validChiSquareNdfMerged);
    mergeHisto("missingChiSquareNdf",missingChiSquareNdfMerged);

    mergeHisto("missPerTrackVsChiSquareNdf",missPerTrackVsChiSquareNdfMerged);
    mergeHisto("missPerTrackPercentVsChiSquareNdf",missPerTrackPercentVsChiSquareNdfMerged);

    mergeHisto("validVsMuontimePre68094",validVsMuontimePre68094Merged);
    mergeHisto("missingVsMuontimePre68094",missingVsMuontimePre68094Merged);
    //mergeHisto("validVsMuontimePost68094",validVsMuontimePost68094Merged);
    mergeHisto(validVsMuontimePost68094Merged);
    mergeHisto("missingVsMuontimePost68094",missingVsMuontimePost68094Merged);
    
    mergeHisto("validMuonTimeVSchargeBPix",validMuonTimeVSchargeBPixMerged);
    mergeHisto("missingMuonTimeVSchargeBPix",missingMuonTimeVSchargeBPixMerged);
    mergeHisto("validMuonTimeVSchargeFPix",validMuonTimeVSchargeFPixMerged);
    mergeHisto("missingMuonTimeVSchargeFPix",missingMuonTimeVSchargeFPixMerged);
    
    mergeHisto("validVsPT",validVsPTMerged);
    mergeHisto("missingVsPT",missingVsPTMerged);

    if(DEBUG) cout<<"********* NOW MERGING *tuning* analysis ************"<<endl;
    mergeHisto("tunningEdgeVal",tunningEdgeValMerged);
    mergeHisto("tunningEdgeMis",tunningEdgeMisMerged);
    mergeHisto("tunningMuonVal",tunningMuonValMerged);
    mergeHisto("tunningMuonMis",tunningMuonMisMerged);
    mergeHisto("tunningVal",tunningValMerged);
    mergeHisto("tunningMis",tunningMisMerged);


    if(DEBUG) cout<<"********* NOW MERGING cluster analysis ************"<<endl;
    
    mergeHisto("chargeDistriPreCuts",chargeDistriPreCutsMerged);
    mergeHisto("numbPixInClusterPreCuts",numbPixInClusterPreCutsMerged);
    mergeHisto("chargeDistriBPixPreCuts",chargeDistriBPixPreCutsMerged);
    mergeHisto("numbPixInClusterBPixPreCuts",numbPixInClusterBPixPreCutsMerged);
    mergeHisto("chargeDistriFPixPlusPreCuts",chargeDistriFPixPlusPreCutsMerged);
    mergeHisto("numbPixInClusterFPixPlusPreCuts",numbPixInClusterFPixPlusPreCutsMerged);
    mergeHisto("chargeDistriFPixMinusPreCuts",chargeDistriFPixMinusPreCutsMerged);
    mergeHisto("numbPixInClusterFPixMinusPreCuts",numbPixInClusterFPixMinusPreCutsMerged);
   
    mergeHisto("chargeVsDimensionBPix",chargeVsDimensionBPixMerged);
    
    mergeHisto("chargeDistri",chargeDistriMerged);
    mergeHisto("numbPixInCluster",numbPixInClusterMerged);
    mergeHisto("chargeDistriBPix",chargeDistriBPixMerged);
    mergeHisto("numbPixInClusterBPix",numbPixInClusterBPixMerged);
    mergeHisto("chargeDistriFPixPlus",chargeDistriFPixPlusMerged);
    mergeHisto("numbPixInClusterFPixPlus",numbPixInClusterFPixPlusMerged);
    mergeHisto("chargeDistriFPixMinus",chargeDistriFPixMinusMerged);
    mergeHisto("numbPixInClusterFPixMinus",numbPixInClusterFPixMinusMerged);
    
    mergeHisto("numbPixInClusterX",numbPixInClusterXMerged);
    mergeHisto("numbPixInClusterY",numbPixInClusterYMerged);
    
    mergeHisto(xposClusterMerged);
    mergeHisto(yposClusterMerged);
    
    mergeHisto("xposClusterValid",xposClusterValidMerged);
    mergeHisto("yposClusterValid",yposClusterValidMerged);
    mergeHisto("xposClusterMisRecovered",xposClusterMisRecoveredMerged);
    mergeHisto("yposClusterMisRecovered",yposClusterMisRecoveredMerged);

    mergeHisto("numTracksVsMuonTime",numTracksVsMuonTimeMerged);
    mergeHisto("denTracksVsMuonTime",denTracksVsMuonTimeMerged);
  
    mergeHisto("xy_standardDev",xy_standardDevMerged);
    mergeHisto("xy_standardDev_insideModule",xy_standardDev_insideModuleMerged);
 
    if(DEBUG) cout<<"********* NOW MERGING WINDOW SEARCH ************"<<endl;

    mergeHisto(windowSearchMerged,true);
    mergeHisto(windowSearchSameModuleMerged,true);
    mergeHisto(windowSearchBPixMerged,true);
    mergeHisto(windowSearchGoodModulesBPixMerged,true);
    mergeHisto(windowSearchFPixMerged,true);
    for (int p=0;p<ptranges;p++){
      char auxname[20];
      sprintf(auxname,"windowSearch_pT_range%d",p);
      std::string auxstring=auxname;
      mergeHisto( (auxstring).c_str(),windowSearch_pTMerged[p],true);
      }
    
    mergeHisto(missingButClusterOnSameModuleMerged);
    mergeHisto(missingButClusterMerged);
        
    if(DEBUG) cout<<"********* ALL HISTO WERE SUCCESSFULLY MERGED ************"<<endl;
    if(DEBUG) cout<<"*********    NOW STARTING TO MERGE TTREE     ************"<<endl;

    
    TTree *tree = (TTree*)gDirectory->Get("moduleAnalysis");
    tree->SetBranchAddress("id",&id);
    tree->SetBranchAddress("isModuleBad",&isModuleBad);
    tree->SetBranchAddress("inactive",&inactive);
    tree->SetBranchAddress("missing",&missing);
    tree->SetBranchAddress("valid",&valid);
    tree->SetBranchAddress("isBarrelModule",&isBarrelModule);
    tree->SetBranchAddress("ladder",&ladder);
    tree->SetBranchAddress("blade",&blade);
    tree->SetBranchAddress("moduleInLadder",&moduleInLadder);
    tree->SetBranchAddress("globalX",&globalX);
    tree->SetBranchAddress("globalY",&globalY);
    tree->SetBranchAddress("globalZ",&globalZ);
    
    if(tree==0) cout<<" !!!!!!!!  Problem opening TTree, it is probably void !!!!!!!!!!"<<endl;
    if(DEBUG) cout<<"********* TTREE WAS OPENED SUCCESSFULLY ************"<<endl;
    
    int entries = tree->GetEntries();
    if(DEBUG) cout<<"== We have "<<entries<<" in the TTree =="<<endl;

    for(int n=0;n<entries;n++){
      tree->GetEntry(n);  //get the tree entries in the proper (already addressed) variables 
      int PlaceInVector=-1;
      for(unsigned int j=0;j<tempTree.size();j++) if(id==tempTree[j][0]) {PlaceInVector = j;break;}
      
      if(PlaceInVector==-1){
        vector<double> temp;
	temp.push_back((double)id);
	temp.push_back((double)isModuleBad);
	temp.push_back((double)inactive);
	temp.push_back((double)missing);
	temp.push_back((double)valid);
	temp.push_back((double)isBarrelModule);
	temp.push_back((double)ladder);
	temp.push_back((double)blade);
	temp.push_back((double)moduleInLadder);
	temp.push_back(globalX);
	temp.push_back(globalY);
	temp.push_back(globalZ);
	tempTree.push_back(temp);	
      }
      else {
        tempTree[PlaceInVector][2]+=inactive;
        tempTree[PlaceInVector][3]+=missing;
        tempTree[PlaceInVector][4]+=valid;
      } 
    }
    
    if(DEBUG) cout<<"********* TTREE MERGED ************"<<endl;    
    
    delete tree;
    
    myFile->Close();
    delete myFile;
    
    }//end of looping over the .root
    

    
  //Filling TTree merged
  for(unsigned int i=0;i<tempTree.size();i++){
    id=(int)tempTree[i][0];
    isModuleBad=(int)tempTree[i][1];
    inactive=(int)tempTree[i][2];
    missing=(int)tempTree[i][3];
    valid=(int)tempTree[i][4];
    isBarrelModule=(int)tempTree[i][5];
    ladder=(int)tempTree[i][6];
    blade=(int)tempTree[i][7];
    moduleInLadder=(int)tempTree[i][8];
    globalX=tempTree[i][9];
    globalY=tempTree[i][10];
    globalZ=tempTree[i][11];
    treeMerged->Fill();
  }

  //****************** END OF MERGING ****************
  
  if(DEBUG) cout<<"********* THE MERGING IS FINISHED ************"<<endl;
  
   
  //EFFIENCY PER RUN
   
   invalidPerRunMerged->LabelsDeflate("X"); 
   validPerRunMerged->LabelsDeflate("X"); 
   inactivePerRunMerged->LabelsDeflate("X");
    
   for (int i=1; i<=validPerRunMerged->GetNbinsX(); i++){       
     inactivePercentPerRun->SetBinContent(i,inactivePerRunMerged->GetBinContent(i)/(invalidPerRunMerged->GetBinContent(i)+validPerRunMerged->GetBinContent(i)) );
     inactivePercentPerRun->GetXaxis()->SetBinLabel( i,validPerRunMerged->GetXaxis()->GetBinLabel(i) );
   } 
  
  setSameLabel(validPerRunMerged,missingPerRunMerged);
  setSameLabel(validPerRunMerged,efficiencyPerRun);
  makeOperation(invalidPerRunMerged,inactivePerRunMerged,missingPerRunMerged,"-");
  //makeEfficiency(validPerRunMerged,missingPerRunMerged,efficiencyPerRun);
  makeEfficiency(efficiencyPerRun);
  
  setSameLabel(muonTimeVSRunNumberMerged,meanMuonTimeVSRunNumber);
  setSameLabel(muonTimeErrorVSRunNumberMerged,meanMuonTimeErrorVSRunNumber);
  
  makeEfficiency(validVSMuonTimeErrorMerged,missingVSMuonTimeErrorMerged,efficiencyVSMuonTimeError);
  makeEfficiency(validVSMuonTimeNdofMerged,missingVSMuonTimeNdofMerged,efficiencyVSMuonTimeNdof);
  makeOperation(muonTimeVSRunNumberMerged,nMuonTimeVSRunNumberMerged,meanMuonTimeVSRunNumber,"/");
  makeOperation(muonTimeErrorVSRunNumberMerged,nMuonTimeErrorVSRunNumberMerged,meanMuonTimeErrorVSRunNumber,"/");
  
  makeOperation(hitsPassingCutsMisBPixMerged,hitsPassingCutsMisFPixMerged,hitsPassingCutsMisMerged,"+");
  makeOperation(hitsPassingCutsValBPixMerged,hitsPassingCutsValFPixMerged,hitsPassingCutsValMerged,"+");
  
  TH1F* histBarrelEfficiencyComparison = new TH1F("histBarrelEfficiencyComparison", "histBarrelEfficiencyComparison", 4, 0, 4);
  TH1F* histEndCapEfficiencyComparison = new TH1F("histEndCapEfficiencyComparison", "histEndCapEfficiencyComparison", 4, 0, 4);
  
  

  double a, b, error;
  /*
  sigma(eff)=sqrt(eff*(1-eff)/N))
  where N is the total amount of tracks that ou used to calculate your efficiency
  (in your case total recHit for the run you  analyze).      
  */
  //BPIX ********
  a=histBarrelMerged->GetBinContent(3); b=histBarrelMerged->GetBinContent(2);
  histBarrelEfficiencyComparison->SetBinContent(1, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histBarrelEfficiencyComparison->SetBinError(1,error);
  histBarrelEfficiencyComparison->GetXaxis()->SetBinLabel(1,"Method 1");
  
  a=histoMethod2Merged->GetBinContent(2); b=histoMethod2Merged->GetBinContent(1);
  histBarrelEfficiencyComparison->SetBinContent(2, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histBarrelEfficiencyComparison->SetBinError(2,error);
  histBarrelEfficiencyComparison->GetXaxis()->SetBinLabel(2,"Method 1 Check");
  
  a=histoMethod2AfterMerged->GetBinContent(2); b=histoMethod2AfterMerged->GetBinContent(1);
  histBarrelEfficiencyComparison->SetBinContent(3, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histBarrelEfficiencyComparison->SetBinError(3,error);
  histBarrelEfficiencyComparison->GetXaxis()->SetBinLabel(3,"Method 2 infinity");
  
  //FPIX *******
  a=histEndcapMerged->GetBinContent(3); b=histEndcapMerged->GetBinContent(2);
  histEndCapEfficiencyComparison->SetBinContent(1, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histEndCapEfficiencyComparison->SetBinError(1,error);
  histEndCapEfficiencyComparison->GetXaxis()->SetBinLabel(1,"Method 1");
  
  a=histoMethod2FPixMerged->GetBinContent(2); b=histoMethod2FPixMerged->GetBinContent(1);
  histEndCapEfficiencyComparison->SetBinContent(2, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histEndCapEfficiencyComparison->SetBinError(2,error);
  histEndCapEfficiencyComparison->GetXaxis()->SetBinLabel(2,"Method 1 Check");
  
  a=histoMethod2AfterFPixMerged->GetBinContent(2); b=histoMethod2AfterFPixMerged->GetBinContent(1);
  histEndCapEfficiencyComparison->SetBinContent(3, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histEndCapEfficiencyComparison->SetBinError(3,error);
  histEndCapEfficiencyComparison->GetXaxis()->SetBinLabel(3,"Method 2 infinity");
  

  a=histLayer1Merged->GetBinContent(3); b=histLayer1Merged->GetBinContent(2);
  if((a+b)!=0) histSubdetectors->SetBinContent( 1, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(1,error);
  histSubdetectors->GetXaxis()->SetBinLabel(1," BPix Layer 1");

  a=histLayer2Merged->GetBinContent(3); b=histLayer2Merged->GetBinContent(2);
  if((a+b)!=0) histSubdetectors->SetBinContent( 2, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(2,error);
  histSubdetectors->GetXaxis()->SetBinLabel(2," BPix Layer 2");
  
  a=histLayer3Merged->GetBinContent(3); b=histLayer3Merged->GetBinContent(2);
  if((a+b)!=0) histSubdetectors->SetBinContent( 3, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(3,error);
  histSubdetectors->GetXaxis()->SetBinLabel(3," BPix Layer 3");
  
  a=histEndcapPlusMerged->GetBinContent(3); b=histEndcapPlusMerged->GetBinContent(2);
  if((a+b)!=0) histSubdetectors->SetBinContent( 4, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(4,error);
  histSubdetectors->GetXaxis()->SetBinLabel(4," FPix Plus");
  
  a=histEndcapMinusMerged->GetBinContent(3); b=histEndcapMinusMerged->GetBinContent(2);
  if((a+b)!=0) histSubdetectors->SetBinContent( 5, a/(a+b) );
  error=0; if((a+b)!=0) error=sqrt( ((a)/(a+b))*(1-((a)/(a+b)))/(a+b) );
  histSubdetectors->SetBinError(5,error);
  histSubdetectors->GetXaxis()->SetBinLabel(5," FPix Minus");
     
  a=histBarrelMerged->GetBinContent(3); b=histBarrelMerged->GetBinContent(2);
  if((a+b)!=0) histSummary->SetBinContent( 1, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSummary->SetBinError(1,error);
  histSummary->GetXaxis()->SetBinLabel(1,"BPix");

  a=histEndcapMerged->GetBinContent(3); b=histEndcapMerged->GetBinContent(2);
  if((a+b)!=0) histSummary->SetBinContent( 2, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSummary->SetBinError(2,error);
  histSummary->GetXaxis()->SetBinLabel(2,"FPix");
  
  a=histoMerged->GetBinContent(3); b=histoMerged->GetBinContent(2);
  if((a+b)!=0) histSummary->SetBinContent( 3, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSummary->SetBinError(3,error);
  histSummary->GetXaxis()->SetBinLabel(3,"total");

  makeEfficiency(validPerSubdetectorMerged,missingPerSubdetectorMerged,efficiencyPerSubdetector);
  efficiencyPerSubdetector->GetXaxis()->SetBinLabel(1," BPix Layer 1");
  efficiencyPerSubdetector->GetXaxis()->SetBinLabel(2," BPix Layer 2");
  efficiencyPerSubdetector->GetXaxis()->SetBinLabel(3," BPix Layer 3");
  efficiencyPerSubdetector->GetXaxis()->SetBinLabel(4," FPix Plus");
  efficiencyPerSubdetector->GetXaxis()->SetBinLabel(5," FPix Minus");

  makeEfficiency(validSummaryMerged,missingSummaryMerged,efficiencySummary);
  efficiencySummary->GetXaxis()->SetBinLabel(1," BPix");
  efficiencySummary->GetXaxis()->SetBinLabel(2," FPix");
  efficiencySummary->GetXaxis()->SetBinLabel(3," Total");
  
  TH1F* auxDen = new TH1F("auxDen","auxDen",validVsBetaBPixMerged->GetNbinsX(),validVsBetaBPixMerged->GetMinimumBin(),validVsBetaBPixMerged->GetMaximumBin());
  makeOperation(validVsBetaBPixMerged,missingVsBetaBPixMerged,auxDen,"+");
  histBetaAnalysisBPixGraph = new TGraphAsymmErrors (validVsBetaBPixMerged,auxDen);  

  TH1F* auxDen2 = new TH1F("auxDen2","auxDen2",validVsAlphaBPixMerged->GetNbinsX(),validVsAlphaBPixMerged->GetMinimumBin(),validVsAlphaBPixMerged->GetMaximumBin());
  makeOperation(validVsAlphaBPixMerged,missingVsAlphaBPixMerged,auxDen2,"+");
  histAlphaAnalysisBPixGraph = new TGraphAsymmErrors (validVsAlphaBPixMerged,auxDen2);
  
  makeEfficiency(validVsBetaBPixMerged,missingVsBetaBPixMerged,histBetaAnalysisBPix,false);
  makeEfficiency(validVsAlphaBPixMerged,missingVsAlphaBPixMerged,histAlphaAnalysisBPix,false);
  makeEfficiency(validVsAlphaMerged,missingVsAlphaMerged,histAlphaAnalysis);  
  makeEfficiency(validVsAlphaFPixMerged,missingVsAlphaFPixMerged,histAlphaAnalysisFPix);
  makeEfficiency(validVsCotanAlphaMerged,missingVsCotanAlphaMerged,histCotanAlphaAnalysis);
  makeEfficiency(validVsBetaMerged,missingVsBetaMerged,histBetaAnalysis);
  
  
  makeEfficiency(validVsBetaFPixMerged,missingVsBetaFPixMerged,histBetaAnalysisFPix);
  makeEfficiency(validAlphaBetaMerged,missingAlphaBetaMerged,alphaBetaEfficiency); 
  makeEfficiency(validVsLocalXBigMerged,missingVsLocalXBigMerged,localXBigEfficiency);
  makeEfficiency(validVsLocalXSmallMerged,missingVsLocalXSmallMerged,localXSmallEfficiency);
  makeEfficiency(validAlphaLocalXBigMerged,missingAlphaLocalXBigMerged,alphaLocalXBigEfficiency );
  makeEfficiency(validAlphaLocalXSmallMerged,missingAlphaLocalXSmallMerged,alphaLocalXSmallEfficiency);
  makeEfficiency(validChiSquareMerged,missingChiSquareMerged,chiSquareEfficiency);
  makeEfficiency(validChiSquareNdfMerged,missingChiSquareNdfMerged,chiSquareNdfEfficiency);
      
  makeEfficiency(validAlphaLocalXBigBPixMerged,missingAlphaLocalXBigBPixMerged,efficiencyAlphaLocalXBigBPix);
  makeEfficiency(validBetaLocalXBigBPixMerged,missingBetaLocalXBigBPixMerged,efficiencyBetaLocalXBigBPix);
  makeEfficiency(validAlphaLocalYBPixMerged,missingAlphaLocalYBPixMerged,efficiencyAlphaLocalYBPix);
  makeEfficiency(validBetaLocalYBPixMerged,missingBetaLocalYBPixMerged,efficiencyBetaLocalYBPix);
  makeEfficiency(validAlphaLocalXBigFPixMerged,missingAlphaLocalXBigFPixMerged,efficiencyAlphaLocalXBigFPix);
  makeEfficiency(validBetaLocalXBigFPixMerged,missingBetaLocalXBigFPixMerged,efficiencyBetaLocalXBigFPix);
  makeEfficiency(validAlphaLocalYFPixMerged,missingAlphaLocalYFPixMerged,efficiencyAlphaLocalYFPix);
  makeEfficiency(validBetaLocalYFPixMerged,missingBetaLocalYFPixMerged,efficiencyBetaLocalYFPix);
  
  makeEfficiency(validVsMuontimePre68094Merged,missingVsMuontimePre68094Merged,muontimePre68094Efficiency);
  makeEfficiency(validVsMuontimePost68094Merged,missingVsMuontimePost68094Merged,muontimePost68094Efficiency);
  
  makeEfficiency(validMuonTimeVSchargeBPixMerged,missingMuonTimeVSchargeBPixMerged,efficiencyMuonTimeVSchargeBPix);
  makeEfficiency(validMuonTimeVSchargeFPixMerged,missingMuonTimeVSchargeFPixMerged,efficiencyMuonTimeVSchargeFPix);
  
  makeEfficiency(validVsPTMerged,missingVsPTMerged,PTEfficiency);

  TH1F* localXAnalysis = new TH1F("localXAnalysis", "hist", 100,-1.5,1.5); 
  for (int bin=1;bin<=100;bin++){
    double val=validVsLocalXMerged->GetBinContent(bin);
    double mis=missingVsLocalXMerged->GetBinContent(bin);
    if ((val+mis)!=0){
      localXAnalysis->SetBinContent(bin,val/(val+mis));
      error=0;
      error=sqrt(((val)/(val+mis))*(1-((val)/(val+mis)))/(val+mis));
      localXAnalysis->SetBinError(bin,error);
    }
  }
  TH1F* localYAnalysis = new TH1F("localYAnalysis", "hist", 100,-4.,4.); 
  for (int bin=1;bin<=100;bin++){
    double val=validVsLocalYMerged->GetBinContent(bin);
    double mis=missingVsLocalYMerged->GetBinContent(bin);
    if ((val+mis)!=0){
      localYAnalysis->SetBinContent(bin,val/(val+mis));
      error=0;
      error=sqrt(((val)/(val+mis))*(1-((val)/(val+mis)))/(val+mis));
      localYAnalysis->SetBinError(bin,error);
    }
  }


  TH1F* moduleBreakoutEff = new TH1F("moduleBreakoutEff", "moduleBreakoutEff", 1500, 0,1500); 
  TH1F* moduleGoodBPix = new TH1F("moduleGoodBPix", "moduleGoodBPix", 800, 0,800); 
  TH1F* moduleBadBPix = new TH1F("moduleBadBPix", "moduleBadBPix", 800, 0,800); 
  TH1F* moduleGoodFPix = new TH1F("moduleGoodFPix", "moduleGoodFPix", 620, 0,620); 
  TH1F* moduleBadFPix = new TH1F("moduleBadFPix", "moduleBadFPix", 620, 0,620); 

  int entries = treeMerged->GetEntries();
  int binGoodBPix,binBadBPix,binGoodFPix,binBadFPix;
  int zeroStatModule=0;
  binGoodBPix = 1;
  binBadBPix  = 1;
  binGoodFPix = 1;
  binBadFPix  = 1;


  for(int n=0;n<entries;n++){
    treeMerged->GetEntry(n);
    if (missing==0 && valid==0)
    //std::cout<<"dei, tiraghe via sto mona..."<< id <<std::endl;
    if (( valid+missing)!=0){
      double setbin = (double)valid/(double)(valid+missing);
      error=0;
      error=sqrt( ((double)valid/(double)(valid+missing))*(1-((double)valid/(double)(valid+missing)))/(double)(valid+missing) );
      
      moduleBreakoutEff->SetBinContent(n+1,setbin);
      moduleBreakoutEff->SetBinError(n+1,error);

      if ( (isModuleBad==0) && (isBarrelModule==1) ){
        moduleGoodBPix->SetBinContent(binGoodBPix , setbin);
        moduleGoodBPix->SetBinError(binGoodBPix , error);
        binGoodBPix++;
      }
      if ( (isModuleBad==1) && (isBarrelModule==1) ){
        moduleBadBPix->SetBinContent(binBadBPix , setbin);
        moduleBadBPix->SetBinError(binBadBPix , error);
        binBadBPix++;
      }
      if ((isModuleBad==0) && (isBarrelModule==2) ){
        moduleGoodFPix->SetBinContent(binGoodFPix , setbin);
        moduleGoodFPix->SetBinError(binGoodFPix , error);
        binGoodFPix++;
      }
      if ( (isModuleBad==1) && (isBarrelModule==2) ){
        moduleBadFPix->SetBinContent(binBadFPix , setbin);
        moduleBadFPix->SetBinError(binBadFPix , error);
        binBadFPix++;
      }
	
    }//end-if fill non "zero statistics" module hist
    else zeroStatModule++;

  }//end-for loop on tree

  //Fit on goodBPixModule with cut at 75%
  TH1F* qualityBPixModule = new TH1F("qualityBPixModule", "qualityBPixModule", moduleGoodBPix->GetNbinsX(), 0 ,moduleGoodBPix->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX())); 
  int fillme=1;
  for (int bin=1; bin<=moduleGoodBPix->GetNbinsX(); bin++){
    if (moduleGoodBPix->GetBinContent(bin)>0.75){
      qualityBPixModule->SetBinContent( fillme,moduleGoodBPix->GetBinContent(bin) );
      qualityBPixModule->SetBinError( fillme,moduleGoodBPix->GetBinError(bin));
      fillme++;
    }
  }
  
  TF1 *myfit = new TF1("myfit","[0]", 0, fillme);
  myfit->SetParName(0,"eff");
  myfit->SetParameter(0, 1);
  
  qualityBPixModule->Fit("myfit");

  
    //Total SCURVE ***********
    double totalHit=0., validHit=0.;

    TH1F* scurve = new TH1F("scurve", "scurve", windowSearchMerged->GetNbinsX(), 0, windowSearchMerged->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX()));
    for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //+1 includes overflow := definitevely missing
      totalHit+=windowSearchMerged->GetBinContent(bin);
   
    for (int bin=1; bin<=scurve->GetNbinsX(); bin++){
      validHit+=windowSearchMerged->GetBinContent(bin);
      scurve->SetBinContent(bin, validHit/totalHit);
    }
    
    scurve->GetXaxis()->SetTitle("window search [cm]");
    scurve->GetYaxis()->SetTitle("eff");
    
    double up,down,delta;
    up= windowSearchMerged->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX());
    delta= windowSearchMerged->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX())/(double)windowSearchMerged->GetNbinsX();
    down=delta*(windowSearchMerged->GetNbinsX()-100.);
    TF1 *fitSC = new TF1("fitSC","[0]", down, up);
    fitSC->SetParName(0,"asympt_eff");
    fitSC->SetParameter(0, 0.7);
    scurve->Fit("fitSC","R");

    //sameModule SCURVE ***********
    totalHit=0.; validHit=0.;
    TH1F* scurveSameModule = new TH1F("scurveSameModule", "scurveSameModule", windowSearchSameModuleMerged->GetNbinsX(), 0, windowSearchSameModuleMerged->GetXaxis()->GetBinUpEdge(windowSearchSameModuleMerged->GetNbinsX()));
    for (int bin=1;bin<=windowSearchSameModuleMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
      totalHit+=windowSearchSameModuleMerged->GetBinContent(bin);
   
    for (int bin=1; bin<=scurveSameModule->GetNbinsX(); bin++){
      validHit+=windowSearchSameModuleMerged->GetBinContent(bin);
      scurveSameModule->SetBinContent(bin, validHit/totalHit);
    }
    
    scurveSameModule->GetXaxis()->SetTitle("window search [cm]");
    scurveSameModule->GetYaxis()->SetTitle("eff [same module]");
    TF1 *fitSCSameModule = new TF1("fitSCSameModule","[0]", down, up);
    fitSCSameModule->SetParName(0,"asympt_eff");
    fitSCSameModule->SetParameter(0, 0.7);
    scurveSameModule->Fit("fitSCSameModule","R");


    //BPix SCURVE ***********
    totalHit=0.; validHit=0.;
    TH1F* scurveBPix = new TH1F("scurveBPix", "scurveBPix", windowSearchMerged->GetNbinsX(), 0, windowSearchMerged->GetXaxis()->GetXmax());
    for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
      totalHit+=windowSearchBPixMerged->GetBinContent(bin);
   
    for (int bin=1; bin<=scurveBPix->GetNbinsX(); bin++){
      validHit+=windowSearchBPixMerged->GetBinContent(bin);
      scurveBPix->SetBinContent(bin, validHit/totalHit);
    }
    
    scurveBPix->GetXaxis()->SetTitle("#Delta R [cm]");
    scurveBPix->GetYaxis()->SetTitle("#epsilon_hit(#Delta R)");
    scurveBPix->GetXaxis()->SetLabelSize(0.03);
    scurveBPix->GetYaxis()->SetLabelSize(0.03);
    TF1 *fitSCBPix = new TF1("fitSCBPix","[0]", down, up);
    fitSCBPix->SetParName(0,"asympt_eff_BPix");
    fitSCBPix->SetParameter(0, 0.7);
    scurveBPix->Fit("fitSCBPix","R");

    a=validHit ; b=totalHit-validHit;
    histBarrelEfficiencyComparison->SetBinContent(4, a/(a+b) );
    error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
    histBarrelEfficiencyComparison->SetBinError(4,error);
    histBarrelEfficiencyComparison->GetXaxis()->SetBinLabel(4,"Method 2 s-curve");
  
    //GoodModulesBPix SCURVE ***********

    totalHit=0.; validHit=0.;
    TH1F* scurveGoodModulesBPix = new TH1F("scurveGoodModulesBPix", "scurveGoodModulesBPix", windowSearchMerged->GetNbinsX(), 0, windowSearchMerged->GetXaxis()->GetXmax());
    for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
      totalHit+=windowSearchGoodModulesBPixMerged->GetBinContent(bin);
   
    for (int bin=1; bin<=scurveBPix->GetNbinsX(); bin++){
      validHit+=windowSearchGoodModulesBPixMerged->GetBinContent(bin);
      scurveGoodModulesBPix->SetBinContent(bin, validHit/totalHit);
    }
    
    scurveGoodModulesBPix->GetXaxis()->SetTitle("#Delta R [cm]");
    scurveGoodModulesBPix->GetYaxis()->SetTitle("#epsilon_{hit}(#Delta R)");
    scurveGoodModulesBPix->GetXaxis()->SetLabelSize(0.03);
    scurveGoodModulesBPix->GetYaxis()->SetLabelSize(0.03);
    TF1 *fitSCGoodModulesBPix = new TF1("fitSCGoodModulesBPix","[0]", down, up);
    fitSCGoodModulesBPix->SetParName(0,"asympt_eff_GoodModulesBPix");
    fitSCGoodModulesBPix->SetParameter(0, 0.7);
    scurveGoodModulesBPix->Fit("fitSCGoodModulesBPix","R");

    // SCURVE for FPix ********
    totalHit=0.; validHit=0.;
    TH1F* scurveFPix = new TH1F("scurveFPix", "scurveFPix", windowSearchMerged->GetNbinsX(), 0, windowSearchMerged->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX()));
    for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
      totalHit+=windowSearchFPixMerged->GetBinContent(bin);
   
    for (int bin=1; bin<=scurveFPix->GetNbinsX(); bin++){
      validHit+=windowSearchFPixMerged->GetBinContent(bin);
      scurveFPix->SetBinContent(bin, validHit/totalHit);
    }
    
    scurveFPix->GetXaxis()->SetTitle("window search [cm]");
    scurveFPix->GetYaxis()->SetTitle("eff [FPIX]");
    TF1 *fitSCFPix = new TF1("fitSCFPix","[0]", down, up);
    fitSCFPix->SetParName(0,"asympt_eff");
    fitSCFPix->SetParameter(0, 0.7);
    scurveFPix->Fit("fitSCFPix","R");

    a=validHit ; b=totalHit-validHit;
    histEndCapEfficiencyComparison->SetBinContent(4, a/(a+b) );
    error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
    histEndCapEfficiencyComparison->SetBinError(4,error);
    histEndCapEfficiencyComparison->GetXaxis()->SetBinLabel(4,"Method 2 s-curve");


    //pTdependent SCURVE ***********
    vector<TH1F*> scurve_pT;
    vector<TF1*>  fitSC_pT;

    for (int p=0;p<=ptranges;p++){
      scurve_pT.push_back(0);
      fitSC_pT.push_back(0);
    }
    for (int p=0;p<=ptranges;p++){
      char basename[10];
      sprintf (basename,"scurve_pT_range%i",p) ;
      std::string m_baseName = basename ;
      scurve_pT[p] = new TH1F( (m_baseName).c_str(),(m_baseName).c_str(),windowSearchMerged->GetNbinsX(), 0, windowSearchMerged->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX()));;
      sprintf (basename,"fitSC_pT_range%i",p) ;
      std::string fit_baseName = basename ;
      fitSC_pT[p] = new TF1( (fit_baseName).c_str(),"[0]", down, up);
    }

    for (int p=0;p<=ptranges;p++){
      totalHit=0.; validHit=0.;
      for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
        totalHit+=windowSearch_pTMerged[p]->GetBinContent(bin);
      std::cout<<"diiicaaaa: "<<totalHit<<std::endl;
      for (int bin=1; bin<=scurve_pT[p]->GetNbinsX(); bin++){
        validHit+=windowSearch_pTMerged[p]->GetBinContent(bin);
        scurve_pT[p]->SetBinContent(bin, validHit/totalHit);
        }
      scurve_pT[p]->GetXaxis()->SetTitle("#Delta R [cm]");
      scurve_pT[p]->GetYaxis()->SetTitle("#epsilon_hit(#Delta R)");
      scurve_pT[p]->GetXaxis()->SetLabelSize(0.03);
      scurve_pT[p]->GetYaxis()->SetLabelSize(0.03);

      fitSC_pT[p]->SetParName(0,"asympt_eff_ptdependent");
      fitSC_pT[p]->SetParameter(0, 0.7);
      //scurve_pT[p]->Fit( fitSC_pT[p]->GetTitle(),"R");
      }
      

  //********* efficiency FreyaPlot
  for (int i=1;i<denTracksVsMuonTimeMerged->GetNbinsX();i++){
    double iNum=numTracksVsMuonTimeMerged->GetBinContent(i);
    double iDen=denTracksVsMuonTimeMerged->GetBinContent(i);
    if (iDen!=0.){
      double iEff=iNum/iDen;
      effTracksVsMuonTimeMerged->SetBinContent(i,iEff);
      double iErr = sqrt(iEff*(1-iEff)/(iDen));
      effTracksVsMuonTimeMerged->SetBinError(i,iErr);
      }
    }
  effTracksVsMuonTimeMerged->GetXaxis()->SetTitle("muon arrival time from DT [ns]");
  effTracksVsMuonTimeMerged->GetYaxis()->SetTitle("#epsilon finding pixel tracks");

    //****************** BEGIN OF 2D MAPS FOR MODULE ANALYSIS ****************

   //****************** Barrel Maps ****************
  
  TH2F* layer1         =  new TH2F("layer1","layer1;module;ladder;efficiency",8,1,9 ,20,1,21);
  TH2F* layer1valid    =  new TH2F("layer1valid","layer1valid;module;ladder;nValid",8,1,9 ,20,1,21);
  TH2F* layer1missing  =  new TH2F("layer1missing","layer1missing;module;ladder;nMissing",8,1,9 ,20,1,21);
  TH2F* layer1inactive =  new TH2F("layer1inactive","layer1inactive;module;ladder;nInactive",8,1,9 ,20,1,21);
  TH2F* layer2         =  new TH2F("layer2","layer2;module;ladder;efficiency",8,1,9 ,32,1,33);
  TH2F* layer2valid    =  new TH2F("layer2valid","layer2valid;module;ladder;nValid",8,1,9 ,32,1,33);
  TH2F* layer2missing  =  new TH2F("layer2missing","layer2missing;module;ladder;nMissing",8,1,9 ,32,1,33);
  TH2F* layer2inactive =  new TH2F("layer2inactive","layer2inactive;module;ladder;nInactive",8,1,9 ,32,1,33);
  TH2F* layer3         =  new TH2F("layer3","layer3;module;ladder;efficiency",8,1,9 ,44,1,45);
  TH2F* layer3valid    =  new TH2F("layer3valid","layer3valid;module;ladder;nValid",8,1,9 ,44,1,45);
  TH2F* layer3missing  =  new TH2F("layer3missing","layer3missing;module;ladder;nMissing",8,1,9 ,44,1,45);
  TH2F* layer3inactive =  new TH2F("layer3inactive","layer3inactive;module;ladder;nInactive",8,1,9 ,44,1,45);
  
  TH2F* layer1_online         =  new TH2F("layer1_online","layer1_online;module;ladder;efficiency",10,-5,5,22,-11,11);
  TH2F* layer1valid_online    =  new TH2F("layer1valid_online","layer1valid_online;module;ladder;nValid",10,-5,5,22,-11,11);
  TH2F* layer1missing_online  =  new TH2F("layer1missing_online","layer1missing_online;module;ladder;nMissing",10,-5,5,22,-11,11);
  TH2F* layer2_online         =  new TH2F("layer2_online","layer2_online;module;ladder;efficiency",10,-5,5,34,-17,17);
  TH2F* layer2valid_online    =  new TH2F("layer2valid_online","layer2valid_online;module;ladder;nValid",10,-5,5,34,-17,17);
  TH2F* layer2missing_online  =  new TH2F("layer2missing_online","layer2missing_online;module;ladder;nMissing",10,-5,5,34,-17,17);
  TH2F* layer3_online         =  new TH2F("layer3_online","layer3_online;module;ladder;efficiency",10,-5,5,46,-23,23);
  TH2F* layer3valid_online    =  new TH2F("layer3valid_online","layer3valid_online;module;ladder;nValid",10,-5,5,46,-23,23);
  TH2F* layer3missing_online  =  new TH2F("layer3missing_online","layer3missing_online;module;ladder;nMissing",10,-5,5,46,-23,23);

  cout<<"Listing Modules with no hits :"<<endl;  
  entries = treeMerged->GetEntries();
  for(int n=0;n<entries;n++){
    treeMerged->GetEntry(n);
    if (isModuleBad!=0 || isBarrelModule!=1) continue;
    
    if(id<302080000){
      layer1valid->Fill(moduleInLadder,ladder,valid);
      layer1missing->Fill(moduleInLadder,ladder,missing);
      layer1inactive->Fill(moduleInLadder,ladder,inactive);
      
      layer1valid_online->Fill(module_online(moduleInLadder),ladder_online(1,ladder),valid);
      layer1missing_online->Fill(module_online(moduleInLadder),ladder_online(1,ladder),missing);
      
      if ( valid+missing == 0 ) std::cout<<"layer 1, module "<<moduleInLadder<<", ladder "<<ladder<<std::endl;
    }
    if(id>302080000 && id<302160000){
      layer2valid->Fill(moduleInLadder,ladder,valid);
      layer2missing->Fill(moduleInLadder,ladder,missing);
      layer2inactive->Fill(moduleInLadder,ladder,inactive);
      
      layer2valid_online->Fill(module_online(moduleInLadder),ladder_online(2,ladder),valid);
      layer2missing_online->Fill(module_online(moduleInLadder),ladder_online(2,ladder),missing);
      
      if ( valid+missing == 0 ) std::cout<<"layer 2, module "<<moduleInLadder<<", ladder "<<ladder<<std::endl;
    }
    if(id>302160000 && id<310000000){
      layer3valid->Fill(moduleInLadder,ladder,valid);
      layer3missing->Fill(moduleInLadder,ladder,missing);
      layer3inactive->Fill(moduleInLadder,ladder,inactive);
      
      layer3valid_online->Fill(module_online(moduleInLadder),ladder_online(3,ladder),valid);
      layer3missing_online->Fill(module_online(moduleInLadder),ladder_online(3,ladder),missing);
      
      if ( valid+missing == 0 ) std::cout<<"layer 3, module "<<moduleInLadder<<", ladder "<<ladder<<std::endl;
    }
  }

  makeEfficiency(layer1valid,layer1missing,layer1);
  makeEfficiency(layer2valid,layer2missing,layer2);
  makeEfficiency(layer3valid,layer3missing,layer3);
  
  makeEfficiency(layer1valid_online,layer1missing_online,layer1_online);
  makeEfficiency(layer2valid_online,layer2missing_online,layer2_online);
  makeEfficiency(layer3valid_online,layer3missing_online,layer3_online);

 
  TH2F* danekBadModuleLayer1 = new TH2F ("danekBadModuleLayer1","danekBadModuleLayer1",8,1,9,20,1,21);
  for (int x=1;x<=danekBadModuleLayer1->GetNbinsX();x++){
    for (int y=1;y<=danekBadModuleLayer1->GetNbinsY();y++){
     danekBadModuleLayer1->SetBinContent(x,y,0.);
     }
    } 
  TH2F* danekBadModuleLayer2 = new TH2F ("danekBadModuleLayer2","danekBadModuleLayer2",8,1,9,32,1,33);
  for (int x=1;x<=danekBadModuleLayer2->GetNbinsX();x++){
    for (int y=1;y<=danekBadModuleLayer2->GetNbinsY();y++){
     danekBadModuleLayer2->SetBinContent(x,y,0.);
     }
    }
  TH2F* danekBadModuleLayer3 = new TH2F ("danekBadModuleLayer3","danekBadModuleLayer3",8,1,9,44,1,45);
  for (int x=1;x<=danekBadModuleLayer3->GetNbinsX();x++){
    for (int y=1;y<=danekBadModuleLayer3->GetNbinsY();y++){
     danekBadModuleLayer3->SetBinContent(x,y,0.);
     }
    }
  /*danekBadModuleLayer1->SetBinContent(6,4,1.);
  danekBadModuleLayer1->SetBinContent(5,12,1.);
  danekBadModuleLayer1->SetBinContent(5,14,1.);
  danekBadModuleLayer1->SetBinContent(5,16,1.);
  danekBadModuleLayer1->SetBinContent(7,19,1.);
  
  danekBadModuleLayer2->SetBinContent(8,3,1.); 
  danekBadModuleLayer2->SetBinContent(2,4,1.);
  danekBadModuleLayer2->SetBinContent(8,9,1.);
  danekBadModuleLayer2->SetBinContent(5,12,1.);
  danekBadModuleLayer2->SetBinContent(7,12,1.);
  danekBadModuleLayer2->SetBinContent(8,12,1.);
  danekBadModuleLayer2->SetBinContent(5,16,1.);
  danekBadModuleLayer2->SetBinContent(8,20,1.);
  danekBadModuleLayer2->SetBinContent(7,21,1.);
  danekBadModuleLayer2->SetBinContent(1,22,1.);
  danekBadModuleLayer2->SetBinContent(8,24,1.);
  danekBadModuleLayer2->SetBinContent(1,26,1.);
  danekBadModuleLayer2->SetBinContent(2,28,1.);
  
  danekBadModuleLayer3->SetBinContent(1,3,1.);
  danekBadModuleLayer3->SetBinContent(2,3,1.);
  danekBadModuleLayer3->SetBinContent(3,3,1.);
  danekBadModuleLayer3->SetBinContent(2,5,1.);
  danekBadModuleLayer3->SetBinContent(2,8,1.);
  danekBadModuleLayer3->SetBinContent(5,19,1.);
  danekBadModuleLayer3->SetBinContent(5,21,1.);
  danekBadModuleLayer3->SetBinContent(6,21,1.);
  danekBadModuleLayer3->SetBinContent(7,21,1.);
  danekBadModuleLayer3->SetBinContent(8,21,1.);
  danekBadModuleLayer3->SetBinContent(1,32,1.);
  danekBadModuleLayer3->SetBinContent(8,34,1.);
  danekBadModuleLayer3->SetBinContent(1,35,1.);
  danekBadModuleLayer3->SetBinContent(2,35,1.);
  danekBadModuleLayer3->SetBinContent(3,35,1.);
  danekBadModuleLayer3->SetBinContent(4,35,1.);
  danekBadModuleLayer3->SetBinContent(7,35,1.);
  danekBadModuleLayer3->SetBinContent(2,37,1.);
  danekBadModuleLayer3->SetBinContent(3,37,1.);
  danekBadModuleLayer3->SetBinContent(4,37,1.);
  danekBadModuleLayer3->SetBinContent(5,37,1.);
  danekBadModuleLayer3->SetBinContent(6,37,1.);
  danekBadModuleLayer3->SetBinContent(1,39,1.);
  danekBadModuleLayer3->SetBinContent(8,39,1.);
  danekBadModuleLayer3->SetBinContent(7,43,1.);
  danekBadModuleLayer3->SetBinContent(6,44,1.);*/
  
  TH1F* goodStatLayer1Eff = new TH1F("goodStatLayer1Eff","goodStatLayer1Eff",160,0,160);
  TH1F* goodStatLayer2Eff = new TH1F("goodStatLayer2Eff","goodStatLayer2Eff",256,0,256);
  TH1F* goodStatLayer3Eff = new TH1F("goodStatLayer3Eff","goodStatLayer3Eff",352,0,352);
  TH1F* Layer1EffDistri = new TH1F("Layer1EffDistri","Layer1EffDistri",50,0.,1.);
  TH1F* Layer2EffDistri = new TH1F("Layer2EffDistri","Layer2EffDistri",50,0.,1.);
  TH1F* Layer3EffDistri = new TH1F("Layer3EffDistri","Layer3EffDistri",50,0.,1.);
  TH1F* goodStatLayer1EffDistri = new TH1F("goodStatLayer1EffDistri","goodStatLayer1EffDistri",30,0.7,1.);
  TH1F* goodStatLayer2EffDistri = new TH1F("goodStatLayer2EffDistri","goodStatLayer2EffDistri",30,0.7,1.);
  TH1F* goodStatLayer3EffDistri = new TH1F("goodStatLayer3EffDistri","goodStatLayer3EffDistri",30,0.7,1.);
  TH1F* goodStatValid = new TH1F("goodstatvalid","goodstatvalid",3,0,3);
  TH1F* goodStatMissing = new TH1F("goodstatmissing","goodstatmissing",3,0,3);
  int setbin=0;
  int ngoodstat = 0;
  int ngoodstattot = 0;

  std::cout<<"+++++++++++++++low statistic modules++++++++++++++++++++"<<std::endl;

  for (int binX=1; binX<=layer1valid->GetNbinsX(); binX++){
    for (int binY=1; binY<=layer1valid->GetNbinsY(); binY++){
      if (layer1->GetBinContent(binX,binY)!=0)
        Layer1EffDistri->Fill(layer1->GetBinContent(binX,binY));
      if (layer1valid->GetBinContent(binX,binY)+layer1missing->GetBinContent(binX,binY) >10 &&
          danekBadModuleLayer1->GetBinContent(binX,binY)==0. ){
        setbin++;
	goodStatLayer1Eff->SetBinContent(setbin,layer1->GetBinContent(binX,binY));
	goodStatValid->Fill(0.5,layer1valid->GetBinContent(binX,binY));
	goodStatMissing->Fill(0.5,layer1missing->GetBinContent(binX,binY));
	goodStatLayer1EffDistri->Fill(layer1->GetBinContent(binX,binY));
	if(layer1->GetBinContent(binX,binY)>0.99) ngoodstat++;
      }
      //if (layer1valid->GetBinContent(binX,binY)+layer1missing->GetBinContent(binX,binY) <=10)
         //std::cout<<"layer 1: module "<<binX<<"; ladder "<<binY<<std::endl;
    }
  }
  
  cout<<endl<<"Fitting Layer 1, modules with nTot>10 && without known problems"<<endl;
  TF1 *goodStatFit1 = new TF1("goodStatFit1","[0]", 0, setbin);
  goodStatFit1->SetParName(0,"eff");
  goodStatFit1->SetParameter(0, 1);
  goodStatLayer1Eff->Fit("goodStatFit1");    
  ngoodstattot+=setbin;
  
  setbin=0;
  for (int binX=1; binX<=layer2valid->GetNbinsX(); binX++){
    for (int binY=1; binY<=layer2valid->GetNbinsY(); binY++){
      if (layer2->GetBinContent(binX,binY)!=0)
        Layer2EffDistri->Fill(layer2->GetBinContent(binX,binY));
      if (layer2valid->GetBinContent(binX,binY)+layer2missing->GetBinContent(binX,binY) >10 &&
          danekBadModuleLayer2->GetBinContent(binX,binY)==0. ){
        setbin++;
	goodStatLayer2Eff->SetBinContent(setbin,layer2->GetBinContent(binX,binY));
	goodStatValid->Fill(1.5,layer2valid->GetBinContent(binX,binY));
	goodStatMissing->Fill(1.5,layer2missing->GetBinContent(binX,binY));
	goodStatLayer2EffDistri->Fill(layer2->GetBinContent(binX,binY));
	if(layer2->GetBinContent(binX,binY)>0.99) ngoodstat++;
      }
      //if (layer2valid->GetBinContent(binX,binY)+layer2missing->GetBinContent(binX,binY) <=10)
        //std::cout<<"layer 2: module "<<binX<<"; ladder "<<binY<<std::endl;      
    }
  }
  cout<<endl<<"Fitting Layer 2, modules with nTot>10 && without known problems"<<endl;
  TF1 *goodStatFit2 = new TF1("goodStatFit2","[0]", 0, setbin);
  goodStatFit2->SetParName(0,"eff");
  goodStatFit2->SetParameter(0, 1);
  goodStatLayer2Eff->Fit("goodStatFit1");    
  ngoodstattot+=setbin;    

  setbin=0;
  for (int binX=1; binX<=layer3valid->GetNbinsX(); binX++){
    for (int binY=1; binY<=layer3valid->GetNbinsY(); binY++){
      if (layer3->GetBinContent(binX,binY)!=0)
        Layer3EffDistri->Fill(layer3->GetBinContent(binX,binY));
      if (layer3valid->GetBinContent(binX,binY)+layer3missing->GetBinContent(binX,binY) >10 &&
          danekBadModuleLayer3->GetBinContent(binX,binY)==0.){
        setbin++;
	goodStatLayer3Eff->SetBinContent(setbin,layer3->GetBinContent(binX,binY));
	goodStatValid->Fill(2.5,layer3valid->GetBinContent(binX,binY));
	goodStatMissing->Fill(2.5,layer3missing->GetBinContent(binX,binY));
	goodStatLayer3EffDistri->Fill(layer3->GetBinContent(binX,binY));
	if(layer3->GetBinContent(binX,binY)>0.99) ngoodstat++;
      }
    }
  }

  cout<<endl<<"Fitting Layer 3, modules with nTot>10 && without known problems"<<endl;
  TF1 *goodStatFit3 = new TF1("goodStatFit3","[0]", 0, setbin);
  goodStatFit3->SetParName(0,"eff");
  goodStatFit3->SetParameter(0, 1);
  goodStatLayer3Eff->Fit("goodStatFit3");     
  ngoodstattot+=setbin;   
  
  TH1F* goodStatEfficiency = new TH1F("goodStatEfficiency","goodStatEfficiency",3,0,3);
  makeEfficiency(goodStatValid,goodStatMissing,goodStatEfficiency);
  
  cout<<"Number of modules with good stat : "<<ngoodstat<<" over "<<ngoodstattot<<endl;
  
  TH2F* layer1RelativeErr =  new TH2F("layer1RelativeErr","layer1RelativeErr",8,1,9 ,20,1,21);
  TH2F* layer2RelativeErr =  new TH2F("layer2RelativeErr","layer2RelativeErr",8,1,9 ,32,1,33);
  TH2F* layer3RelativeErr =  new TH2F("layer3RelativeErr","layer3RelativeErr",8,1,9 ,44,1,45);

  TGraphAsymmErrors* auxiliumEff;
  TH1F* auxiliumNum = new TH1F("auxiliumNum","auxiliumNum",8,1,9);
  TH1F* auxiliumDen = new TH1F("auxiliumDen","auxiliumDen",8,1,9);

  for (int j=1;j<=20;j++){
    for (int i=1;i<=8;i++){
      auxiliumNum->SetBinContent(i,layer1valid->GetBinContent(i,j));
      auxiliumDen->SetBinContent(i,layer1valid->GetBinContent(i,j)+layer1missing->GetBinContent(i,j));
      }
    auxiliumEff = new TGraphAsymmErrors(auxiliumNum,auxiliumDen);
    for (int bin=1;bin<=8;bin++)
      layer1RelativeErr->SetBinContent(bin,j,(auxiliumEff->GetErrorY(bin-1))/layer1->GetBinContent(bin,j));
    }
    
  for (int j=1;j<=32;j++){
    for (int i=1;i<=8;i++){
      auxiliumNum->SetBinContent(i,layer2valid->GetBinContent(i,j));
      auxiliumDen->SetBinContent(i,layer2valid->GetBinContent(i,j)+layer2missing->GetBinContent(i,j));
      }
    auxiliumEff = new TGraphAsymmErrors(auxiliumNum,auxiliumDen);
    int counter=0;
    for (int bin=1;bin<=8;bin++){
      if ((bin==2&&j==4)||(bin==8&&j==9)||(bin==5&&j==16)||(bin==7&&j==21)||(bin==1&&j==22)||(bin==8&&j==24))
        layer2RelativeErr->SetBinContent(bin,j,0.);
      else{
        layer2RelativeErr->SetBinContent(bin,j,(auxiliumEff->GetErrorY(counter))/layer2->GetBinContent(bin,j));
	counter++;
	}
      }
    }
    
  for (int j=1;j<=44;j++){
    for (int i=1;i<=8;i++){
      auxiliumNum->SetBinContent(i,layer3valid->GetBinContent(i,j));
      auxiliumDen->SetBinContent(i,layer3valid->GetBinContent(i,j)+layer3missing->GetBinContent(i,j));
      }
    auxiliumEff = new TGraphAsymmErrors(auxiliumNum,auxiliumDen);
    int counter=0;
    for (int bin=1;bin<=8;bin++){
      if ((bin==2&&j==1)||(bin==4&&j==1)||(bin==7&&j==1)||(bin==5&&j==22)||(bin==7&&j==22)||(bin==2&&j==8)||
          (bin==2&&j==23)||(bin==3&&j==23)||(bin==5&&j==23)||(bin==6&&j==23)||(bin==7&&j==23)||(bin==8&&j==23)||
          (bin==8&&j==34)||(bin==1&&j==44)||(bin==2&&j==44)||(bin==3&&j==44)||(bin==6&&j==44)||(bin==8&&j==44)
         )
        layer3RelativeErr->SetBinContent(bin,j,0.);
      else{
        layer3RelativeErr->SetBinContent(bin,j,(auxiliumEff->GetErrorY(counter))/layer3->GetBinContent(bin,j));
	counter++;
	}
      }
    }

  std::cout<<"******** NUMBER TO QUOTE: average on the whole layer, no cleaning ********"<<std::endl;
  int totNum=0;
  int totDen=0;
  int wholeNum=0;
  int wholeDen=0;
  double minLayer1=1;
  double minLayer2=1;
  double minLayer3=1;
  
  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=22;lad++){
      totNum+=(int)layer1valid->GetBinContent(mod,lad);
      totDen+=(int)layer1valid->GetBinContent(mod,lad)+(int)layer1missing->GetBinContent(mod,lad);
      if(layer1->GetBinContent(mod,lad)<minLayer1 && layer1->GetBinContent(mod,lad)>0)
        minLayer1=layer1->GetBinContent(mod,lad);
    }
  }
  wholeNum+=totNum;  wholeDen+=totDen;
  double averageEff = (double)totNum/(double)totDen;
  double binomialErr = sqrt(averageEff*(1-averageEff)/(double)(totNum+totDen));
  std::cout<<"layer 1 average efficiency: "<<averageEff<<" +- "<<binomialErr<<" binomial error"<<std::endl;    
       
  totNum=0;
  totDen=0;
  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=32;lad++){
      totNum+=(int)layer2valid->GetBinContent(mod,lad);
      totDen+=(int)layer2valid->GetBinContent(mod,lad)+(int)layer2missing->GetBinContent(mod,lad);
      if(layer2->GetBinContent(mod,lad)<minLayer2 && layer2->GetBinContent(mod,lad)>0)
        minLayer2=layer2->GetBinContent(mod,lad);
    }
  }
  wholeNum+=totNum;  wholeDen+=totDen;
  averageEff = (double)totNum/(double)totDen;
  binomialErr = sqrt(averageEff*(1-averageEff)/(double)(totNum+totDen));
  std::cout<<"layer 2 average efficiency: "<<averageEff<<" +- "<<binomialErr<<" binomial error"<<std::endl;    

  totNum=0;
  totDen=0;
  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=44;lad++){
      totNum+=(int)layer3valid->GetBinContent(mod,lad);
      totDen+=(int)layer3valid->GetBinContent(mod,lad)+(int)layer3missing->GetBinContent(mod,lad);
      if(layer3->GetBinContent(mod,lad)<minLayer3 && layer3->GetBinContent(mod,lad)>0)
        minLayer3=layer3->GetBinContent(mod,lad);
    }
  }
  wholeNum+=totNum;  wholeDen+=totDen;
  averageEff = (double)totNum/(double)totDen;
  binomialErr = sqrt(averageEff*(1-averageEff)/(double)(totNum+totDen));
  std::cout<<"layer 3 average efficiency: "<<averageEff<<" +- "<<binomialErr<<" binomial error"<<std::endl;

  averageEff = (double)wholeNum/(double)wholeDen;
  binomialErr = sqrt(averageEff*(1-averageEff)/(double)(wholeNum+wholeDen));
  std::cout<<std::endl<<"whole barrel average "<<averageEff<<"+-"<<binomialErr<<std::endl;
  
  std::cout<<"******** NUMBER TO QUOTE: average on the whole layer without danekList-modules ********"<<std::endl;
  totNum=0;  totDen=0;  wholeNum=0;  wholeDen=0;
  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=22;lad++){
      if (danekBadModuleLayer1->GetBinContent(mod,lad)==0){
        totNum+=(int)layer1valid->GetBinContent(mod,lad);
        totDen+=(int)layer1valid->GetBinContent(mod,lad)+(int)layer1missing->GetBinContent(mod,lad);
      }
    }
  }
  wholeNum+=totNum;  wholeDen+=totDen;
  averageEff = (double)totNum/(double)totDen;
  binomialErr = sqrt(averageEff*(1-averageEff)/(double)(totNum+totDen));
  std::cout<<"layer 1 average efficiency: "<<averageEff<<" +- "<<binomialErr<<" binomial error"<<std::endl;    
       
  totNum=0;
  totDen=0;
  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=32;lad++){
      if (danekBadModuleLayer2->GetBinContent(mod,lad)==0){
        totNum+=(int)layer2valid->GetBinContent(mod,lad);
        totDen+=(int)layer2valid->GetBinContent(mod,lad)+(int)layer2missing->GetBinContent(mod,lad);
      }
    }
  }
  wholeNum+=totNum;  wholeDen+=totDen;
  averageEff = (double)totNum/(double)totDen;
  binomialErr = sqrt(averageEff*(1-averageEff)/(double)(totNum+totDen));
  std::cout<<"layer 2 average efficiency: "<<averageEff<<" +- "<<binomialErr<<" binomial error"<<std::endl;    

  totNum=0;
  totDen=0;
  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=44;lad++){
      if (danekBadModuleLayer3->GetBinContent(mod,lad)==0){
        totNum+=(int)layer3valid->GetBinContent(mod,lad);
        totDen+=(int)layer3valid->GetBinContent(mod,lad)+(int)layer3missing->GetBinContent(mod,lad);
      }
    }
  }
  wholeNum+=totNum;  wholeDen+=totDen;
  averageEff = (double)totNum/(double)totDen;
  binomialErr = sqrt(averageEff*(1-averageEff)/(double)(totNum+totDen));
  std::cout<<"layer 3 average efficiency: "<<averageEff<<" +- "<<binomialErr<<" binomial error"<<std::endl;    

  averageEff = (double)wholeNum/(double)wholeDen;
  binomialErr = sqrt(averageEff*(1-averageEff)/(double)(wholeNum+wholeDen));
  std::cout<<std::endl<<"whole barrel average "<<averageEff<<"+-"<<binomialErr<<std::endl;

    //****************** cleaning 2D maps for high-relativeErrors and danekList modules ***************    

  TH2F* layer1publication =  new TH2F("layer1publication","layer1publication",8,1,9 ,20,1,21);
  TH2F* layer2publication =  new TH2F("layer2publication","layer2publication",8,1,9 ,32,1,33);
  TH2F* layer3publication =  new TH2F("layer3publication","layer3publication",8,1,9 ,44,1,45);
  layer1publication->GetZaxis()->SetRangeUser(minLayer1*0.98,1.);
  layer2publication->GetZaxis()->SetRangeUser(minLayer2*0.98,1.);
  layer3publication->GetZaxis()->SetRangeUser(minLayer3*0.98,1.);

  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=22;lad++){
      if (layer1RelativeErr->GetBinContent(mod,lad)<0.1)
        layer1publication->SetBinContent(mod,lad,layer1->GetBinContent(mod,lad));
      else layer1publication->SetBinContent(mod,lad,0.);
    }
  }

  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=32;lad++){
      if ( (layer2RelativeErr->GetBinContent(mod,lad)>0.1))
           //|| (mod==2&&lad==4)||(mod==8&&lad==9)||(mod==5&&lad==16)||(mod==7&&lad==21)||(mod==1&&lad==22)||(mod==8&&lad==24) )
        layer2publication->SetBinContent(mod,lad,0.);
      else layer2publication->SetBinContent(mod,lad,layer2->GetBinContent(mod,lad));
    }
  }

  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=44;lad++){
      if (layer3RelativeErr->GetBinContent(mod,lad)>0.1)
          //|| (mod==2&&lad==8)||(mod==5&&lad==21)||(mod==8&&lad==21)||(mod==8&&lad==34)||(mod==6&&lad==44) )
        layer3publication->SetBinContent(mod,lad,0);
      else layer3publication->SetBinContent(mod,lad,layer3->GetBinContent(mod,lad));
    }
  }

    //****************** END   OF 2D MAPS FOR MODULE ANALYSIS ****************    
  
    //****************** 1D Projections: eff per number of modules ****************    
  
  
  for(int n=0;n<entries;n++){
    treeMerged->GetEntry(n);
    //double eff = 0;
    if(id<302080000){
      if((valid+missing)!=0)
        effDistriLayer1->Fill(double(valid)/double(valid+missing));
    }
    if(id>302080000 && id<302160000){
      if( (valid+missing)!=0 )
        effDistriLayer2->Fill(double(valid)/double(valid+missing));
    }
    if(id>302160000 && id<310000000){
      if( (valid+missing)!=0 )
        effDistriLayer3->Fill(double(valid)/double(valid+missing));
    }
    if(id>340000000 && globalZ>0){
      if( (valid+missing)!=0 )
        effDistriFpixPlus->Fill(double(valid)/double(valid+missing));
    }
    if(id>340000000 && globalZ<0){
      if( (valid+missing)!=0 )
        effDistriFpixMinus->Fill(double(valid)/double(valid+missing));
    }
  }
    
  cout<<endl<<"Doing Fit on efficiency per run"<<endl;
  TF1 *runfit = new TF1("runfit","[0]", 0, efficiencyPerRun->GetNbinsX());
  runfit->SetParName(0,"mean_eff");
  runfit->SetParameter(0, 1);
  efficiencyPerRun->Fit("runfit");

  TH1F* cutsEfficiencyValid = new TH1F("cutsEfficiencyValid","cutsEfficiencyValid",5,0,5);
  TH1F* cutsEfficiencyMissing = new TH1F("cutsEfficiencyMissing","cutsEfficiencyMissing",5,0,5);
  TH1F* cutsEfficiencyValidBPix = new TH1F("cutsEfficiencyValidBPix","cutsEfficiencyValidBPix",5,0,5);
  TH1F* cutsEfficiencyMissingBPix = new TH1F("cutsEfficiencyMissingBPix","cutsEfficiencyMissingBPix",5,0,5);
  TH1F* cutsEfficiencyValidFPix = new TH1F("cutsEfficiencyValidFPix","cutsEfficiencyValidFPix",5,0,5);
  TH1F* cutsEfficiencyMissingFPix = new TH1F("cutsEfficiencyMissingFPix","cutsEfficiencyMissingFPix",5,0,5);
  for(int i=1;i<6;i++){
    cutsEfficiencyValid->SetBinContent(i,double(hitsPassingCutsValMerged->GetBinContent(i+1))/double(hitsPassingCutsValMerged->GetBinContent(1)));
    cutsEfficiencyMissing->SetBinContent(i,double(hitsPassingCutsMisMerged->GetBinContent(i+1))/double(hitsPassingCutsMisMerged->GetBinContent(1)));
    cutsEfficiencyValidBPix->SetBinContent(i,double(hitsPassingCutsValBPixMerged->GetBinContent(i+1))/double(hitsPassingCutsValBPixMerged->GetBinContent(1)));
    cutsEfficiencyMissingBPix->SetBinContent(i,double(hitsPassingCutsMisBPixMerged->GetBinContent(i+1))/double(hitsPassingCutsMisBPixMerged->GetBinContent(1)));
    cutsEfficiencyValidFPix->SetBinContent(i,double(hitsPassingCutsValFPixMerged->GetBinContent(i+1))/double(hitsPassingCutsValFPixMerged->GetBinContent(1)));
    cutsEfficiencyMissingFPix->SetBinContent(i,double(hitsPassingCutsMisFPixMerged->GetBinContent(i+1))/double(hitsPassingCutsMisFPixMerged->GetBinContent(1)));
  }


  TH1F* cutsEfficiency = new TH1F("cutsEfficiency","cutsEfficiency;;#epsilon_{hit}",10,0,10);
  TH1F* cutsEfficiencyBPix = new TH1F("cutsEfficiencyBPix","cutsEfficiencyBPix;;Efficiency [BPix]",10,0,10);
  TH1F* cutsEfficiencyFPix = new TH1F("cutsEfficiencyFPix","cutsEfficiencyFPix;;Efficiency [FPix]",10,0,10);
  TH1F* cutsTotal = new TH1F("cutsTotal","cutsTotal;;nHits",10,0,10);
  TH1F* cutsTotalBPix = new TH1F("cutsTotalBPix","cutsTotalBPix;;nHits [BPix]",10,0,10);
  TH1F* cutsTotalFPix = new TH1F("cutsTotalFPix","cutsTotalFPix;;nHits [FPix]",10,0,10);
  for(int i=1;i<11;i++){
    cutsEfficiency->SetBinContent(i,double(hitsPassingCutsValMerged->GetBinContent(i))/double(hitsPassingCutsValMerged->GetBinContent(i)+hitsPassingCutsMisMerged->GetBinContent(i)));
    cutsEfficiencyBPix->SetBinContent(i,double(hitsPassingCutsValBPixMerged->GetBinContent(i))/double(hitsPassingCutsValBPixMerged->GetBinContent(i)+hitsPassingCutsMisBPixMerged->GetBinContent(i)));
    cutsEfficiencyFPix->SetBinContent(i,double(hitsPassingCutsValFPixMerged->GetBinContent(i))/double(hitsPassingCutsValFPixMerged->GetBinContent(i)+hitsPassingCutsMisFPixMerged->GetBinContent(i)));
    cutsTotal->SetBinContent(i,hitsPassingCutsValMerged->GetBinContent(i)+hitsPassingCutsMisMerged->GetBinContent(i));
    cutsTotalBPix->SetBinContent(i,hitsPassingCutsValBPixMerged->GetBinContent(i)+hitsPassingCutsMisBPixMerged->GetBinContent(i));
    cutsTotalFPix->SetBinContent(i,hitsPassingCutsValFPixMerged->GetBinContent(i)+hitsPassingCutsMisFPixMerged->GetBinContent(i));
  }
  
  cutsEfficiency->GetXaxis()->SetBinLabel(1,"No Cut");
  cutsEfficiency->GetXaxis()->SetBinLabel(2,"Loose cut");
  cutsEfficiency->GetXaxis()->SetBinLabel(3,"Telescope");
  cutsEfficiency->GetXaxis()->SetBinLabel(5,"Edge cut");
  cutsEfficiency->GetXaxis()->SetBinLabel(4,"Muon Timing");
  cutsEfficiency->GetXaxis()->SetBinLabel(6,"pT cut");
  cutsEfficiency->GetXaxis()->SetBinLabel(7,"Telescope + Muon ");
  cutsEfficiency->GetXaxis()->SetBinLabel(8,"Telescope + Edge");
  cutsEfficiency->GetXaxis()->SetBinLabel(9,"Edge + Muon");
  cutsEfficiency->GetXaxis()->SetBinLabel(10,"Analysis cut");
  
  setSameLabel(cutsEfficiency,cutsEfficiencyBPix,false);
  setSameLabel(cutsEfficiency,cutsEfficiencyFPix,false);
  setSameLabel(cutsEfficiency,cutsTotal,false);
  setSameLabel(cutsEfficiency,cutsTotalBPix,false);
  setSameLabel(cutsEfficiency,cutsTotalFPix,false);
  
  makeEfficiency(tunningValMerged, tunningMisMerged, tunningEfficiency);
  makeEfficiency(tunningEdgeValMerged, tunningEdgeMisMerged, tunningEdgeEfficiency);
  makeEfficiency(tunningMuonValMerged, tunningMuonMisMerged, tunningMuonEfficiency);
  
  for (int bin=1; bin<=40; bin++){ 
    tunningSlice->SetBinContent(bin,tunningEfficiency->GetBinContent(10,bin));
    int nTot=(int)tunningValMerged->GetBinContent(10,bin)+(int)tunningMisMerged->GetBinContent(10,bin);
    nTotVsTunningMuon->SetBinContent(bin,nTot);
  } 
  
  if (DEBUG) std::cout<<"please tell me that you are at least here"<<std::endl;

  TFile* fOutputFile = new TFile("merged.root", "RECREATE");
  fOutputFile->cd();

  histoMerged->Write();  
  histLayer1Merged->Write();  
  histLayer2Merged->Write();  
  histLayer3Merged->Write();  
  histEndcapPlusMerged->Write();
  histEndcapMinusMerged->Write();
  histBarrelMerged->Write();  
  histEndcapMerged->Write();    
  validPerSubdetectorMerged->Write();  
  missingPerSubdetectorMerged->Write(); 
  
  consistencyCheckMerged->Write();
  consistencyCheckTrajMerged->Write();
  trackingEfficiencyMerged->Write();
  
  histInvalidRecHitCollectionMerged->Write();
  histInvalidRecHitWithBadmoduleListMerged->Write();
  histoMethod2Merged->Write();
  histoMethod2AfterMerged->Write();

  invalidPerRunMerged->Write(); 
  validPerRunMerged->Write(); 
  missingPerRunMerged->Write();
  inactivePerRunMerged->Write();

  TCanvas* cRun = new TCanvas("cRun","cRun",1200,600); 
  cRun->cd();
  gStyle->SetStatX( 0.8);
  gStyle->SetStatY( 0.45);


  //efficiencyPerRun->LabelsDeflate("X");
  efficiencyPerRun->LabelsOption("a","X");
  efficiencyPerRun->Write();
  efficiencyPerRun->Draw();
  cRun->Print("efficiencyPerRun.png","png");
  cRun->Close();


  inactivePercentPerRun->LabelsDeflate("X");
  inactivePercentPerRun->Write();


  validVsAlphaMerged->Write();
  validVsCotanAlphaMerged->Write();
  validVsAlphaBPixMerged->Write();
  validVsAlphaFPixMerged->Write();
  missingVsAlphaMerged->Write();
  missingVsCotanAlphaMerged->Write();
  missingVsAlphaBPixMerged->Write();
  missingVsAlphaFPixMerged->Write(); 
  validVsBetaMerged->Write(); 
  validVsBetaBPixMerged->Write(); 
  validVsBetaFPixMerged->Write();
  missingVsBetaMerged->Write();
  missingVsBetaBPixMerged->Write();
  missingVsBetaFPixMerged->Write();
  validAlphaBetaMerged->Write();
  missingAlphaBetaMerged->Write();
  
  missingVsLocalXMerged->Write();
  missingVsLocalYMerged->Write();
  validVsLocalXMerged->Write();
  validVsLocalYMerged->Write();  
  
  validVsLocalXBigMerged->Write();  
  missingVsLocalXBigMerged->Write();  
  validVsLocalXSmallMerged->Write();  
  missingVsLocalXSmallMerged->Write();  
  
  windowSearchMerged->Write();
  windowSearchSameModuleMerged->Write();
  windowSearchBPixMerged->Write();
  windowSearchGoodModulesBPixMerged->Write();
  windowSearchFPixMerged->Write();
  missingButClusterMerged->Write();
  missingButClusterOnSameModuleMerged->Write();
  for (int p=0;p<=ptranges;p++)
    windowSearch_pTMerged[p]->Write();
  
  checkoutTrajMerged->Write();
  checkoutValidityFlagMerged->Write(); 
    
  validVsPTMerged->Write();   
  missingVsPTMerged->Write();   
  
  missingPerTrackMerged->Write();
  inactivePerTrackMerged->Write();
  
  treeMerged->Write();

  xPosFracValMerged->Write();
  xPosFracMisMerged->Write();
  yPosFracValMerged->Write();
  yPosFracMisMerged->Write();

  hitsPassingCutsValMerged->Write();
  hitsPassingCutsMisMerged->Write();
  hitsPassingCutsValBPixMerged->Write();
  hitsPassingCutsMisBPixMerged->Write();
  hitsPassingCutsValFPixMerged->Write();
  hitsPassingCutsMisFPixMerged->Write();
  
  chargeDistriMerged->Write();
  numbPixInClusterMerged->Write();
  chargeDistriBPixMerged->Write();
  numbPixInClusterBPixMerged->Write();
  chargeDistriFPixPlusMerged->Write();
  numbPixInClusterFPixPlusMerged->Write();
  chargeDistriFPixMinusMerged->Write();
  numbPixInClusterFPixMinusMerged->Write();

  chargeVsDimensionBPixMerged->GetXaxis()->SetTitle("cluster_{charge} [e^{-}]");
  chargeVsDimensionBPixMerged->GetYaxis()->SetTitle("cluster_{dimension}");
  chargeVsDimensionBPixMerged->Write();

  chargeDistriPreCutsMerged->Write();
  numbPixInClusterPreCutsMerged->Write();
  chargeDistriBPixPreCutsMerged->Write();
  numbPixInClusterBPixPreCutsMerged->Write();
  chargeDistriFPixPlusPreCutsMerged->Write();
  numbPixInClusterFPixPlusPreCutsMerged->Write();
  chargeDistriFPixMinusPreCutsMerged->Write();
  numbPixInClusterFPixMinusPreCutsMerged->Write();
  
  numbPixInClusterXMerged->Write();
  numbPixInClusterYMerged->Write();
  xposClusterMerged->Write();
  yposClusterMerged->Write();
  xposClusterValidMerged->Write();
  yposClusterValidMerged->Write();
  xposClusterMisRecoveredMerged->Write();
  yposClusterMisRecoveredMerged->Write();
  
  validVsMuontimePre68094Merged->Write();
  validVsMuontimePost68094Merged->Write();
  missingVsMuontimePre68094Merged->Write();
  missingVsMuontimePost68094Merged->Write();
  
  validMuonTimeVSchargeBPixMerged->Write();
  missingMuonTimeVSchargeBPixMerged->Write();
  validMuonTimeVSchargeFPixMerged->Write();
  missingMuonTimeVSchargeFPixMerged->Write();
  
  //muonTimeVSRunNumberMerged->Write();
  //nMuonTimeVSRunNumberMerged->Write();
  //muonTimeErrorVSRunNumberMerged->Write();
  //nMuonTimeErrorVSRunNumberMerged->Write();
  
  efficiencyVSMuonTimeError->Write();
  meanMuonTimeVSRunNumber->Write();
  meanMuonTimeErrorVSRunNumber->Write();
  muonTimeErrorDistriMerged->Write();
  validVSMuonTimeErrorMerged->Write();
  missingVSMuonTimeErrorMerged->Write();


  efficiencyVSMuonTimeNdof->Write();
  muonTimeNdofDistriMerged->Write();
  validVSMuonTimeNdofMerged->Write();
  missingVSMuonTimeNdofMerged->Write();
  
  //******* EFFICIENCY PLOTS WRITING ************
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  if(DEBUG) cout<<"********* STARTING TO WRITE/DRAW NEW PLOTS ************"<<endl;

  
  layer1->Write();
  layer1valid->Write();
  layer1missing->Write();
  layer1inactive->Write();
  layer2->Write();
  layer2valid->Write();
  layer2missing->Write();
  layer2inactive->Write();
  layer3->Write();
  layer3valid->Write();
  layer3missing->Write();
  layer3inactive->Write();
  
  layer1_online->Write();
  layer1valid_online->Write();
  layer1missing_online->Write();
  layer2_online->Write();
  layer2valid_online->Write();
  layer2missing_online->Write();
  layer3_online->Write();
  layer3valid_online->Write();
  layer3missing_online->Write();

  histBarrelEfficiencyComparison->Write();
  histSubdetectors->Write();
  histSummary->Write();
  efficiencyPerSubdetector->Write();
  efficiencySummary->Write();
  
  goodStatLayer1Eff->GetYaxis()->SetTitle("#epsilon_{hit}"); 
  goodStatLayer1Eff->GetXaxis()->SetTitle("modules Layer 1"); 
  goodStatLayer2Eff->GetYaxis()->SetTitle("#epsilon_{hit}"); 
  goodStatLayer2Eff->GetXaxis()->SetTitle("modules Layer 2"); 
  goodStatLayer3Eff->GetYaxis()->SetTitle("#epsilon_{hit}"); 
  goodStatLayer3Eff->GetXaxis()->SetTitle("modules Layer 3");
  goodStatLayer1Eff->Write(); 
  goodStatLayer1Eff->Write(); 
  goodStatLayer2Eff->Write();  
  goodStatLayer1Eff->Draw(); 
  c1->Print("goodStatLayer1Eff.png","png");   
  goodStatLayer2Eff->Draw(); 
  c1->Print("goodStatLayer2Eff.png","png");  
  goodStatLayer3Eff->Draw(); 
  c1->Print("goodStatLayer3Eff.png","png"); 
  
  gStyle->SetStatX( 0.4);
  gStyle->SetStatY( 0.6);
  goodStatLayer1EffDistri->SetTitle(";Efficiency;# modules");
  goodStatLayer2EffDistri->SetTitle(";Efficiency;# modules");
  goodStatLayer3EffDistri->SetTitle(";Efficiency;# modules");
  goodStatLayer1EffDistri->Write();
  goodStatLayer2EffDistri->Write();
  goodStatLayer3EffDistri->Write();
  goodStatLayer1EffDistri->Draw(); 
  c1->Print("goodStatLayer1EffDistri.pdf","pdf");  
  goodStatLayer2EffDistri->Draw(); 
  c1->Print("goodStatLayer2EffDistri.pdf","pdf"); 
  goodStatLayer3EffDistri->Draw(); 
  c1->Print("goodStatLayer3EffDistri.pdf","pdf");
  
  
  Layer1EffDistri->SetTitle(";Efficiency;# modules");
  Layer2EffDistri->SetTitle(";Efficiency;# modules");
  Layer3EffDistri->SetTitle(";Efficiency;# modules");
  Layer1EffDistri->Write();
  Layer2EffDistri->Write();
  Layer3EffDistri->Write();
  Layer1EffDistri->Draw(); 
  c1->Print("Layer1EffDistri.pdf","pdf");  
  Layer2EffDistri->Draw(); 
  c1->Print("Layer2EffDistri.pdf","pdf"); 
  Layer3EffDistri->Draw(); 
  c1->Print("Layer3EffDistri.pdf","pdf"); 
  
  
  gStyle->SetStatX( 0.8);
  gStyle->SetStatY( 0.45);
  
  //moduleBreakoutEff->LabelsDeflate("X");
  moduleBreakoutEff->Write();
  
  moduleGoodBPix->GetYaxis()->SetTitle("eff_{goodBPix}"); 
  moduleGoodBPix->GetXaxis()->SetTitle("detid_{list}"); 
  moduleGoodBPix->Write(); 
  qualityBPixModule->Write();
  qualityBPixModule->Draw();
  c1->Print("qualityBPixModule.png","png");

  moduleGoodFPix->GetYaxis()->SetTitle("eff_{goodFPix}"); 
  moduleGoodFPix->GetXaxis()->SetTitle("detid_{list}"); 
  moduleGoodFPix->Write(); 
  moduleGoodFPix->Draw();
  c1->Print("moduleFPix.png","png");

  moduleBadBPix->Write(); 
  moduleBadFPix->Write(); 
     
  effDistriLayer1->Write();
  effDistriLayer2->Write();
  effDistriLayer3->Write();
  effDistriFpixPlus->Write();
  effDistriFpixMinus->Write();

  cutsEfficiencyValidBPix->Write();
  cutsEfficiencyMissingBPix->Write();
  cutsEfficiencyValidFPix->Write();
  cutsEfficiencyMissingFPix->Write();
  cutsEfficiencyValid->Write();
  cutsEfficiencyMissing->Write();
  
  cutsEfficiency->Write();
  cutsEfficiencyBPix->Write();
  cutsEfficiencyFPix->Write();
  cutsTotal->Write();
  cutsTotalBPix->Write();
  cutsTotalFPix->Write();
  
  tunningValMerged->Write();
  tunningMisMerged->Write();
  tunningEdgeValMerged->Write();
  tunningEdgeMisMerged->Write();
  tunningMuonValMerged->Write();
  tunningMuonMisMerged->Write();
 
  tunningEfficiency->GetYaxis()->SetRange(2,40);
  tunningEfficiency->GetYaxis()->SetTitle("muon timing window [ns]");
  tunningEfficiency->GetXaxis()->SetTitle("N_{sigma}");
  tunningEfficiency->Write();
  tunningSlice->GetXaxis()->SetTitle("muon timing window");
  tunningSlice->GetYaxis()->SetTitle("#epsilon_{hit}");
  tunningSlice->GetYaxis()->SetRangeUser(0.994,0.998);
  tunningSlice->Write();
  nTotVsTunningMuon->Write();
  tunningEdgeEfficiency->GetXaxis()->SetTitle("\"Edge cut\" : number of sigma");
  tunningEdgeEfficiency->Write();
  tunningMuonEfficiency->GetXaxis()->SetTitle("muon timing window [ns]");
  tunningMuonEfficiency->GetXaxis()->SetRange(2,40);
 // tunningMuonEfficiency->GetYaxis()->SetRange(0.5,1.05);
  tunningMuonEfficiency->Write();
  
  alphaBetaEfficiency->GetXaxis()->SetTitle("alpha");
  alphaBetaEfficiency->GetYaxis()->SetTitle("beta");
  alphaBetaEfficiency->Write();
  localXBigEfficiency->GetXaxis()->SetTitle("X [cm]");
  localXBigEfficiency->Write();
  localXSmallEfficiency->GetXaxis()->SetTitle("X [cm]");
  localXSmallEfficiency->Write();
  alphaLocalXBigEfficiency->GetXaxis()->SetTitle("#alpha_{local}");
  alphaLocalXBigEfficiency->GetYaxis()->SetTitle("x_{local} [cm]");
  alphaLocalXBigEfficiency->Write();
  alphaLocalXSmallEfficiency->GetXaxis()->SetTitle("#alpha_{local}");
  alphaLocalXSmallEfficiency->GetYaxis()->SetTitle("x_{local} [cm]");
  alphaLocalXSmallEfficiency->Write();
  chiSquareEfficiency->GetXaxis()->SetTitle("Chi2");
  chiSquareEfficiency->Write();
  chiSquareNdfEfficiency->GetXaxis()->SetTitle("#Chi^{2}_{ndf}");
  chiSquareNdfEfficiency->Write();
  
  validAlphaLocalXBigBPixMerged->SetTitle(";#alpha_{Local};X_{Local}");
  validAlphaLocalXBigBPixMerged->Write();
  missingAlphaLocalXBigBPixMerged->SetTitle(";#alpha_{Local};X_{Local}");
  missingAlphaLocalXBigBPixMerged->Write();
  efficiencyAlphaLocalXBigBPix->SetTitle(";#alpha_{Local};X_{Local}");
  efficiencyAlphaLocalXBigBPix->Write();
  validBetaLocalXBigBPixMerged->SetTitle(";#beta_{Local};X_{Local}");
  validBetaLocalXBigBPixMerged->Write();
  missingBetaLocalXBigBPixMerged->SetTitle(";#beta_{Local};X_{Local}");
  missingBetaLocalXBigBPixMerged->Write();
  efficiencyBetaLocalXBigBPix->SetTitle(";#beta_{Local};X_{Local}");
  efficiencyBetaLocalXBigBPix->Write();
  validAlphaLocalYBPixMerged->SetTitle(";#alpha_{Local};Y_{Local}");
  validAlphaLocalYBPixMerged->Write();
  missingAlphaLocalYBPixMerged->SetTitle(";#alpha_{Local};Y_{Local}");
  missingAlphaLocalYBPixMerged->Write();
  efficiencyAlphaLocalYBPix->SetTitle(";#alpha_{Local};Y_{Local}");
  efficiencyAlphaLocalYBPix->Write();
  validBetaLocalYBPixMerged->SetTitle(";#beta_{Local};Y_{Local}");
  validBetaLocalYBPixMerged->Write();
  missingBetaLocalYBPixMerged->SetTitle(";#beta_{Local};Y_{Local}");
  missingBetaLocalYBPixMerged->Write();
  efficiencyBetaLocalYBPix->SetTitle(";#beta_{Local};Y_{Local}");
  efficiencyBetaLocalYBPix->Write();
  
  validAlphaLocalXBigFPixMerged->SetTitle(";#alpha_{Local};X_{Local}");
  validAlphaLocalXBigFPixMerged->Write();
  missingAlphaLocalXBigFPixMerged->SetTitle(";#alpha_{Local};X_{Local}");
  missingAlphaLocalXBigFPixMerged->Write();
  efficiencyAlphaLocalXBigFPix->SetTitle(";#alpha_{Local};X_{Local}");
  efficiencyAlphaLocalXBigFPix->Write();
  validBetaLocalXBigFPixMerged->SetTitle(";#beta_{Local};X_{Local}");
  validBetaLocalXBigFPixMerged->Write();
  missingBetaLocalXBigFPixMerged->SetTitle(";#beta_{Local};X_{Local}");
  missingBetaLocalXBigFPixMerged->Write();
  efficiencyBetaLocalXBigFPix->SetTitle(";#beta_{Local};X_{Local}");
  efficiencyBetaLocalXBigFPix->Write();
  validAlphaLocalYFPixMerged->SetTitle(";#alpha_{Local};Y_{Local}");
  validAlphaLocalYFPixMerged->Write();
  missingAlphaLocalYFPixMerged->SetTitle(";#alpha_{Local};Y_{Local}");
  missingAlphaLocalYFPixMerged->Write();
  efficiencyAlphaLocalYFPix->SetTitle(";#alpha_{Local};Y_{Local}");
  efficiencyAlphaLocalYFPix->Write();
  validBetaLocalYFPixMerged->SetTitle(";#beta_{Local};Y_{Local}");
  validBetaLocalYFPixMerged->Write();
  missingBetaLocalYFPixMerged->SetTitle(";#beta_{Local};Y_{Local}");
  missingBetaLocalYFPixMerged->Write();
  efficiencyBetaLocalYFPix->SetTitle(";#beta_{Local};Y_{Local}");
  efficiencyBetaLocalYFPix->Write();
  
  missPerTrackVsChiSquareNdfMerged->GetXaxis()->SetTitle("#chi^{2}_{/ndf}");
  missPerTrackVsChiSquareNdfMerged->GetYaxis()->SetTitle("num^{miss}_{track}");
  missPerTrackVsChiSquareNdfMerged->Write();
  missPerTrackPercentVsChiSquareNdfMerged->GetXaxis()->SetTitle("#chi^{2}_{/ndf}");
  missPerTrackPercentVsChiSquareNdfMerged->GetYaxis()->SetTitle("%^{miss}_{track}");
  missPerTrackPercentVsChiSquareNdfMerged->Write();


  validAlphaLocalXBigMerged->Write();
  missingAlphaLocalXBigMerged->Write();
  validAlphaLocalXSmallMerged->Write();
  missingAlphaLocalXSmallMerged->Write();
  
  
  muontimePre68094Efficiency->GetXaxis()->SetTitle("Muon arrival time [ns]");
  muontimePre68094Efficiency->Write();
  muontimePost68094Efficiency->GetXaxis()->SetTitle("Muon arrival time [ns]");
  muontimePost68094Efficiency->Write();
  
  
  PTEfficiency->GetXaxis()->SetTitle("pT Track [GeV]");
  PTEfficiency->Write();
  
  
  //**************** NO PAD ************
  gStyle->SetOptStat(0);
  
  gStyle->SetOptFit(0);
  
  scurve->Write();
  scurve->Draw();
  c1->Print("scurve.png","png");
  scurveSameModule->Write();
  scurveSameModule->Draw();
  c1->Print("scurveSameModule.png","png");
  scurveBPix->Write();
  scurveBPix->Draw();
  c1->Print("scurveBPix.png","png");
  scurveFPix->Write();
  scurveFPix->Draw();
  c1->Print("scurveFPix.png","png");
  scurveGoodModulesBPix->Write();
  scurveGoodModulesBPix->Draw();
  c1->Print("scurveGoodModulesBPix.png","png");
 
  scurveGoodModulesBPix->SetLineColor(kBlue);
  scurveGoodModulesBPix->Draw();
  scurveBPix->Draw("same");
  TLegend* legComp = new TLegend(0.3,0.3,0.7,0.7);
  legComp->AddEntry(scurveGoodModulesBPix,"all non-dead modules","l");
  legComp->AddEntry(scurveBPix,"all non-dead non-misconfigured","l");
  legComp->AddEntry(fitSCGoodModulesBPix,"Fitted region","l");
  legComp->SetFillColor(10);
  legComp->SetTextSize(0.03);
  legComp->SetLineWidth(2);
  legComp->Draw("same");
  c1->Print("scurveBPixCompareWOcleaning.png","png");
    
  for (int p=0;p<=ptranges;p++){
    scurve_pT[p]->Write();    
    scurve_pT[p]->Draw();
    c1->Print((scurve_pT[p]->GetTitle()+std::string(".png")).c_str(),"png");
    }
  
  goodStatEfficiency->GetXaxis()->SetBinLabel(1,"Layer 1");
  goodStatEfficiency->GetXaxis()->SetBinLabel(2,"Layer 2");
  goodStatEfficiency->GetXaxis()->SetBinLabel(3,"Layer 3");
  goodStatEfficiency->Write();
  goodStatEfficiency->Draw(); 
  c1->Print("goodStatEfficiency.png","png"); 
  
  PTEfficiency->Draw("colz");
  c1->Print("PTEfficiency.png","png");
  
  alphaBetaEfficiency->Draw("colz");
  c1->Print("alphaBetaEfficiency.png","png");
  localXBigEfficiency->Draw();
  c1->Print("localXBigEfficiency.png","png");
  localXSmallEfficiency->Draw();
  c1->Print("localXSmallEfficiency.png","png");
  alphaLocalXBigEfficiency->Draw("colz");
  c1->Print("alphaLocalXBigEfficiency.png","png");
  alphaLocalXSmallEfficiency->Draw("colz");
  c1->Print("alphaLocalXSmallEfficiency.png","png");
  chiSquareEfficiency->Draw();
  c1->Print("chiSquareEfficiency.png","png"); 

  tunningEfficiency->Draw("colz");
  c1->Print("tunningEfficiency.png","png");
  tunningEdgeEfficiency->Draw();
  c1->Print("tunningEdgeEfficiency.png","png");
  tunningMuonEfficiency->Draw();
  c1->Print("tunningMuonEfficiency.png","png");
  
  tunningSlice->Draw("P");
  c1->Print("tunningSlice.png","png");
  
  cutsEfficiency->Draw();
  c1->Print("cutsEfficiency.png","png");
  cutsEfficiencyBPix->Draw();
  c1->Print("cutsEfficiencyBPix.png","png");
  cutsEfficiencyFPix->Draw();
  c1->Print("cutsEfficiencyFPix.png","png");
  cutsTotal->Draw();
  c1->Print("cutsTotal.png","png");
  cutsTotalBPix->Draw();
  c1->Print("cutsTotalBPix.png","png");
  cutsTotalFPix->Draw();
  c1->Print("cutsTotalFPix.png","png");
   
  histAlphaAnalysis->GetXaxis()->SetTitle("#alpha_{local}");  
  histAlphaAnalysis->Write();
  histAlphaAnalysis->Draw();
  c1->Print("histAlphaAnalysis.png","png");  
  histAlphaAnalysisFPix->GetXaxis()->SetTitle("#alpha_{local}");  
  histAlphaAnalysisFPix->Write();  
  
  histAlphaAnalysisBPix->GetXaxis()->SetTitle("#alpha_{local}");  
  histAlphaAnalysisBPix->Draw();
  histAlphaAnalysisBPix->Write();
  histAlphaAnalysisBPixGraph->Draw("same");
  c1->Print("histAlphaAnalysisBPixMakeEfficiencyNoErrorGraph.png","png");
  histAlphaAnalysisBPixGraph->Write(); 
 
  histCotanAlphaAnalysis->Write();

  xy_standardDevMerged->Write();
  xy_standardDev_insideModuleMerged->Write();

  numTracksVsMuonTimeMerged->Write();
  denTracksVsMuonTimeMerged->Write();
  effTracksVsMuonTimeMerged->Draw();
  effTracksVsMuonTimeMerged->Write();
  TLegend* smallLeg = new TLegend(0.6,0.8,0.8,0.99);
  smallLeg->AddEntry(effTracksVsMuonTimeMerged,"CRAFT09 data","l");
  smallLeg->SetFillColor(kWhite);
  smallLeg->Draw("same");
  c1->Print("trackEffVSmuonTime.png","png");

  c1->Clear();
  c1->cd();
  histBetaAnalysisBPix->GetXaxis()->SetTitle("#beta_{local}");  
  histBetaAnalysisBPix->Write();
  histBetaAnalysisBPix->Draw();
  histBetaAnalysisBPixGraph->Draw("same");
  c1->Print("histBetaAnalysisMakeEfficiencyNoErrorBPixGraph.png","png");
  histBetaAnalysisBPixGraph->Write(); 
  
  histBetaAnalysisFPix->GetXaxis()->SetTitle("#beta_{local}");  
  histBetaAnalysisFPix->Write();
  
  histBetaAnalysis->GetXaxis()->SetTitle("#beta_{local}");  
  histBetaAnalysis->Write();
  histBetaAnalysis->Draw();
  c1->Print("histBetaAnalysis.png","png");
  
  localXAnalysis->GetXaxis()->SetTitle("x_{local} [cm]");
  localXAnalysis->GetYaxis()->SetTitle("#epsilon_{hit}");
  localXAnalysis->Write();
  localXAnalysis->Draw();
  c1->Print("localXAnalysis.png","png");
  localYAnalysis->GetXaxis()->SetTitle("y_{local} [cm]");
  localYAnalysis->GetYaxis()->SetTitle("#epsilon_{hit}");
  localYAnalysis->Write();
  localYAnalysis->Draw();
  c1->Print("localYAnalysis.png","png");
  
  
  validVsLocalXBigMerged->Draw();
  c1->Print("validVsLocalXBigMerged.png","png");
  missingVsLocalXBigMerged->Draw();
  c1->Print("missingVsLocalXBigMerged.png","png");
  validVsLocalXSmallMerged->Draw();
  c1->Print("validVsLocalXSmallMerged.png","png");
  missingVsLocalXSmallMerged->Draw();
  c1->Print("missingVsLocalXSmallMerged.png","png");
  
  
  histSummary->Draw();
  c1->Print("histSummary.png","png");
  histSubdetectors->Draw();
  c1->Print("histSubdetectors.png","png");
  histSubdetectors->GetXaxis()->SetRangeUser(0., 2.);
  histSubdetectors->Draw();
  c1->Print("histSubdetectorsNOFPIX.png","png");
  histSubdetectors->GetXaxis()->SetRangeUser(0., 4.);
  histSubdetectors->Draw();
  c1->Print("histSubdetectorsFullRange.png","png");
  
  layer1->Draw("colz");
  c1->Print("layer1.png","png");
  layer1missing->Draw("colz");
  c1->Print("layer1missing.png","png");
  layer1valid->Draw("colz");
  c1->Print("layer1valid.png","png");
  layer1inactive->Draw("colz");
  c1->Print("layer1inactive.png","png");
  layer2->Draw("colz");
  c1->Print("layer2.png","png");
  layer2missing->Draw("colz");
  c1->Print("layer2missing.png","png");
  layer2valid->Draw("colz");
  c1->Print("layer2valid.png","png");
  layer2inactive->Draw("colz");
  c1->Print("layer2inactive.png","png");
  layer3->Draw("colz");
  c1->Print("layer3.png","png");
  layer3missing->Draw("colz");
  c1->Print("layer3missing.png","png");
  layer3valid->Draw("colz");
  c1->Print("layer3valid.png","png");
  layer1inactive->Draw("colz");
  c1->Print("layer1inactive.png","png");

  layer1RelativeErr->GetXaxis()->SetTitle("module");
  layer1RelativeErr->GetYaxis()->SetTitle("ladder");
  layer2RelativeErr->GetXaxis()->SetTitle("module");
  layer2RelativeErr->GetYaxis()->SetTitle("ladder");
  layer3RelativeErr->GetXaxis()->SetTitle("module");
  layer3RelativeErr->GetYaxis()->SetTitle("ladder");
  layer1RelativeErr->Write();
  layer1RelativeErr->Draw("colz");
  c1->Print("layer1RelativeErr.png","png");
  layer2RelativeErr->Write();
  layer2RelativeErr->Draw("colz");
  c1->Print("layer2RelativeErr.png","png");
  layer3RelativeErr->Write();
  layer3RelativeErr->Draw("colz");
  c1->Print("layer3RelativeErr.png","png");

  layer1publication->GetXaxis()->SetTitle("module");
  layer1publication->GetYaxis()->SetTitle("ladder");
  layer2publication->GetXaxis()->SetTitle("module");
  layer2publication->GetYaxis()->SetTitle("ladder");
  layer3publication->GetXaxis()->SetTitle("module");
  layer3publication->GetYaxis()->SetTitle("ladder");
  layer1publication->Write();
  layer1publication->Draw("colz");
  c1->Print("layer1publication.png","png");
  layer2publication->Write();
  layer2publication->Draw("colz");
  c1->Print("layer2publication.png","png");
  layer3publication->Write();
  layer3publication->Draw("colz");
  c1->Print("layer3publication.png","png");
  
  TLegend* leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->AddEntry(chargeDistriMerged,"ALL CUTS","l");
  leg->AddEntry(chargeDistriPreCutsMerged,"NO CUTS","l");
  leg->SetFillColor(10);
  
  Canv(chargeDistriMerged,chargeDistriPreCutsMerged,"chargeDistri.png","",leg,true);
  Canv(numbPixInClusterMerged,numbPixInClusterPreCutsMerged,"numbPixInCluster.png","",leg,true);
  Canv(chargeDistriBPixMerged,chargeDistriBPixPreCutsMerged,"chargeDistriBPix.png","",leg,true);
  Canv(numbPixInClusterBPixMerged,numbPixInClusterBPixPreCutsMerged,"numbPixInClusterBPix.png","",leg,true);
  Canv(chargeDistriFPixPlusMerged,chargeDistriFPixPlusPreCutsMerged,"chargeDistriFPixPlus.png","",leg,true);
  Canv(numbPixInClusterFPixPlusMerged,numbPixInClusterFPixPlusPreCutsMerged,"numbPixInClusterFPixPlus.png","",leg,true);
  Canv(chargeDistriFPixMinusMerged,chargeDistriFPixMinusPreCutsMerged,"chargeDistriFPixMinus.png","",leg,true);
  Canv(numbPixInClusterFPixMinusMerged,numbPixInClusterFPixMinusPreCutsMerged,"numbPixInClusterFPixMinus.png","",leg,true);
  
  chargeVsDimensionBPixMerged->Draw("colz");
  c1->Print("chargeVsDimensionBPixMerged.png","png");

  delete leg;
  leg = new TLegend(0.2,0.2,0.4,0.4);
  leg->AddEntry(muontimePre68094Efficiency,"before run 68094","P");
  leg->AddEntry(muontimePost68094Efficiency," after run 68094","P");
  leg->SetFillColor(10);
  
  Canv(muontimePre68094Efficiency,muontimePost68094Efficiency,"EfficiencyVsMuonTime.png","ERR",leg);
  
  
  efficiencyMuonTimeVSchargeBPix->GetXaxis()->SetTitle("Muon arrival time [ns]");
  efficiencyMuonTimeVSchargeBPix->GetYaxis()->SetTitle("cluster_{charge} [e^{-}]");
  efficiencyMuonTimeVSchargeFPix->GetXaxis()->SetTitle("Muon arrival time [ns]");
  efficiencyMuonTimeVSchargeFPix->GetYaxis()->SetTitle("cluster_{charge} [e^{-}]");
  efficiencyMuonTimeVSchargeBPix->Write();
  efficiencyMuonTimeVSchargeBPix->Draw("colz");
  c1->Print("efficiencyMuonTimeVSchargeBPix.png","png");
  efficiencyMuonTimeVSchargeFPix->Write();
  efficiencyMuonTimeVSchargeFPix->Draw("colz");
  c1->Print("efficiencyMuonTimeVSchargeFPix.png","png");

//nice overimposion
  histAlphaAnalysisFPix->GetXaxis()->SetTitle("#alpha_{local}");
  histAlphaAnalysisFPix->GetYaxis()->SetTitle("#epsilon_{hit}");
  histAlphaAnalysisFPix->Draw();
  c1->Update();
  Float_t rightmax = 1.1*validVsAlphaFPixMerged->GetMaximum();
  Float_t scale = gPad->GetUymax()/rightmax;
  validVsAlphaFPixMerged->Scale(scale);
  validVsAlphaFPixMerged->SetLineColor(kRed);
  validVsAlphaFPixMerged->Draw("same");
  TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),0,rightmax,510,"+L");
  axis->SetLineColor(kRed);
  axis->SetLabelColor(kRed);
  axis->SetTitleColor(kRed);
  axis->SetTitle("number_{valid hits}");
  axis->Draw();
  c1->Print("histAlphaAnlaysisANDvalidstatistic.png","png");
  
  
  
  //************************************************
  //****************    SUMMARY   ******************
  //************************************************
  if(DEBUG) cout<<"Writing summary to file"<<endl;
  ofstream summary;
  summary.open("Summary.tex");
    
  if(summary.is_open()){
    summary<<"1. Total number of events :          "<<consistencyCheckMerged->GetBinContent(1)<<endl;
    summary<<"2. Total number of tracks :          "<<consistencyCheckMerged->GetBinContent(2)<<endl;
    summary<<"3. With at least 1 hit in pixel :    "<<consistencyCheckTrajMerged->GetBinContent(4)<<endl;
    summary<<"4. With at least 1 hit in BPIX  :    "<<consistencyCheckTrajMerged->GetBinContent(5)<<endl;
    summary<<"5. With at least 1 hit in FPIX  :    "<<consistencyCheckTrajMerged->GetBinContent(6)<<endl;
    summary<<"6. In pix volume with $\\chi^2<15$  : "<<trackingEfficiencyMerged->GetBinContent(1)<<endl;
    summary<<"7. In pix volume with $\\chi^2<15$"<<endl;
    summary<<"    and at least 1 pixel hit :       "<<trackingEfficiencyMerged->GetBinContent(2)<<endl;
    summary<<"Efficiencies :"<<endl;
    summary<<"$\\epsilon = \\frac{(3)}{(2)}$ = ";
      summary<<trackingEfficiencyMerged->GetBinContent(2)/trackingEfficiencyMerged->GetBinContent(1)<<endl;
    summary<<"$\\epsilon = \\frac{(7)}{(6)}$ = ";
      summary<<consistencyCheckTrajMerged->GetBinContent(4)/consistencyCheckMerged->GetBinContent(2)<<endl;
      
    
    summary<<"Number of hits :           "<<cutsTotal->GetBinContent(1)<<endl;
    summary<<"in BPIX :                  "<<cutsTotalBPix->GetBinContent(1)<<endl;
    summary<<"in FPIX :                  "<<cutsTotalFPix->GetBinContent(1)<<endl;
    summary<<"After edge cut :           "<<cutsTotal->GetBinContent(5)<<endl;
    summary<<"in BPIX :                  "<<cutsTotalBPix->GetBinContent(5)<<endl;
    summary<<"in FPIX :                  "<<cutsTotalFPix->GetBinContent(5)<<endl;
    summary<<"After edge+telescope cut : "<<cutsTotal->GetBinContent(8)<<endl;
    summary<<"in BPIX :                  "<<cutsTotalBPix->GetBinContent(8)<<endl;
    summary<<"in FPIX :                  "<<cutsTotalFPix->GetBinContent(8)<<endl;
    
    summary.close();
  }

  fOutputFile->Close() ;
    
}


//************** Quick function to merge easilly histos **************
void mergeHisto(const char* name,TH1F* histo, bool mergeOverflows){
    TH1F*  temp=(TH1F*)gDirectory->Get(name);
    if(!temp) return;
    
    if(!mergeOverflows)
      for (int bin=1; bin<=histo->GetNbinsX(); bin++)
        histo->SetBinContent( bin, histo->GetBinContent(bin) + temp->GetBinContent(bin));

    if(mergeOverflows)
      for (int bin=0; bin<=histo->GetNbinsX()+1; bin++)
        histo->SetBinContent( bin, histo->GetBinContent(bin) + temp->GetBinContent(bin));
}

void mergeHisto(TH1F* histo, bool mergeOverflows){ //NEW, TAKES NAME OF HISTO FROM MERGED ONE
  TString name = histo->GetName();
  name=name.Remove(name.Length()-6);
  mergeHisto(name,histo,mergeOverflows);
}

void mergeHisto(const char* name,TH2F* histo, bool mergeOverflows){
    TH2F*  temp=(TH2F*)gDirectory->Get(name);
    if(!temp) return;
    
    if(!mergeOverflows){
      for (int binx=1; binx<=histo->GetNbinsX(); binx++){
        for (int biny=1; biny<=histo->GetNbinsY(); biny++)
	  histo->SetBinContent( binx, biny, histo->GetBinContent(binx,biny) + temp->GetBinContent(binx,biny));
      }
    }
    
    if(mergeOverflows){
      for (int binx=0; binx<=histo->GetNbinsX()+1; binx++){
        for (int biny=0; biny<=histo->GetNbinsY()+1; biny++)
	  histo->SetBinContent( binx, biny, histo->GetBinContent(binx,biny) + temp->GetBinContent(binx,biny));
      }
    }

}

void mergeHisto(TH2F* histo, bool mergeOverflows){
  TString name = histo->GetName();
  name=name.Remove(name.Length()-5);
  mergeHisto(name,histo,mergeOverflows);
}

void mergeHistoWithLabel(char* name,TH1F* histo){
  int lastBin=-999;
  for (int bin=1; bin<=histo->GetNbinsX();bin++){
    if ( !strcmp(histo->GetXaxis()->GetBinLabel(bin),"") ) {lastBin=bin; break;}
  }
 
  TH1F*  temp=(TH1F*)gDirectory->Get(name);
  if(!temp) return;
  
  for (int bin=1; bin<=temp->GetNbinsX(); bin++){
    bool foundLabel=false;
    for (int binMerged=1; binMerged<=histo->GetNbinsX(); binMerged++){
      if ( !strcmp(temp->GetXaxis()->GetBinLabel(bin),histo->GetXaxis()->GetBinLabel(binMerged)) ){
        foundLabel=true;
        histo->SetBinContent( binMerged,histo->GetBinContent(binMerged)+temp->GetBinContent(bin));
        break;
      }
    }
   
   
    if (!foundLabel){
      histo->GetXaxis()->SetBinLabel(lastBin,temp->GetXaxis()->GetBinLabel(bin));
      lastBin++;
    }
  }
}

void makeEfficiency(TH1F* valid, TH1F* missing, TH1F* efficiency, bool withError){
  for(int i=1;i<=efficiency->GetNbinsX();i++){
    double val = valid->GetBinContent(i);
    double mis = missing->GetBinContent(i);
    if( (val+mis)!=0 ){
      double eff = val / (val+mis);
      double err = sqrt(eff*(1-eff)/(val+mis));
      efficiency->SetBinContent(i,eff);
      if(withError) efficiency->SetBinError(i,err);
    }
  }
  efficiency->GetYaxis()->SetTitle("#epsilon_{hit}");
}

void makeEfficiency(TH1F* efficiency, bool withError){
  TString name = efficiency->GetName();
  name=name.Replace(0,10,0,0);
  TH1F* valid = (TH1F*) gDirectory->Get("valid"+name+"Merged");
  TH1F* missing = (TH1F*) gDirectory->Get("missing"+name+"Merged");
  makeEfficiency(valid,missing,efficiency,withError);
}
/*
void makeEfficiencyNoError(TH1F* valid, TH1F* missing, TH1F* efficiency){
  for(int i=1;i<=efficiency->GetNbinsX();i++){
    double val = valid->GetBinContent(i);
    double mis = missing->GetBinContent(i);
    if( (val+mis)!=0 ){
      double eff = val / (val+mis);
      //double err = sqrt(eff*(1-eff)/(val+mis));
      efficiency->SetBinContent(i,eff);
    }
  }
  efficiency->GetYaxis()->SetTitle("#epsilon_{hit}");
}*/

void makeEfficiency(TH2F* valid, TH2F* missing, TH2F* efficiency){
  for(int i=1;i<=efficiency->GetNbinsX();i++){
    for(int j=1;j<=efficiency->GetNbinsY();j++){
      double val = valid->GetBinContent(i,j);
      double mis = missing->GetBinContent(i,j);
      if( (val+mis)!=0 ){
        double eff = val / (val+mis);
        efficiency->SetBinContent(i,j,eff);
      }
    }
  }
}

void makeEfficiencyGraph(TH1F* valid, TH1F* missing, TH1F* efficiency){
  TGraphAsymmErrors* auxiliumEff;
  TH1F* auxiliumDen = new TH1F("auxiliumDen","auxiliumDen",efficiency->GetNbinsX(),efficiency->GetXaxis()->GetXmin(),efficiency->GetXaxis()->GetXmax());
  efficiency->GetYaxis()->SetTitle("#epsilon_{hit}");

  for(int i=1;i<=efficiency->GetNbinsX();i++){
    double val = valid->GetBinContent(i);
    double mis = missing->GetBinContent(i);
    auxiliumDen->SetBinContent(i,val+mis);
    if( (val+mis)!=0 ){
      double eff = val / (val+mis);
      efficiency->SetBinContent(i,eff);
      }
    }
  auxiliumEff = new TGraphAsymmErrors(valid,auxiliumDen);

  int graphCounter = 0;
  for(int i=1;i<=efficiency->GetNbinsX();i++){    
    double val = valid->GetBinContent(i);
    double mis = missing->GetBinContent(i);
    if( (val+mis)!=0 )  {efficiency->SetBinError(i,auxiliumEff->GetErrorY(graphCounter)); graphCounter++;}
    }
  delete auxiliumEff;
  delete auxiliumDen;
  }

void makeOperation(TH1F* input1, TH1F* input2, TH1F* output, char* operationtype){
  for (int bin=1;bin<=input1->GetNbinsX();bin++){
    if(operationtype=="+")
      output->SetBinContent(bin,input1->GetBinContent(bin)+input2->GetBinContent(bin));
    if(operationtype=="-")
      output->SetBinContent(bin,input1->GetBinContent(bin)-input2->GetBinContent(bin));
    if(operationtype=="*")
      output->SetBinContent(bin,input1->GetBinContent(bin)*input2->GetBinContent(bin));
    if(operationtype=="/")
      if(input2->GetBinContent(bin)!=0)
        output->SetBinContent(bin,input1->GetBinContent(bin)/input2->GetBinContent(bin));
  }
}

void makeMean(TH1F* value, TH1F* nentries, TH1F* mean){
  for (int bin=1;bin<=value->GetNbinsX();bin++)
    if(nentries->GetBinContent(bin)!=0)
      mean->SetBinContent(bin,double(value->GetBinContent(bin))/double(nentries->GetBinContent(bin)));
}

void setSameLabel(TH1F* copyfrom, TH1F* output, bool deflate){
  if(deflate) copyfrom->LabelsDeflate("X");
  for (int i=1; i<=copyfrom->GetNbinsX(); i++)
    output->GetXaxis()->SetBinLabel( i,copyfrom->GetXaxis()->GetBinLabel(i) );
  if(deflate) output->LabelsDeflate("X");
}

void Canv(TH1F *Histo1,TH1F *Histo2,char CName[100], char Option[60], TLegend* Leg, bool integrate){
  TCanvas *Can = new TCanvas("cc","cc");
  Can->cd();
  double Max;
  
  if(integrate){
    Histo1->Scale(1./Histo1->Integral());
    Histo2->Scale(1./Histo2->Integral());
  }
  
  (Histo1->GetMaximum() <= Histo2->GetMaximum() )? Max=Histo2->GetMaximum():Max=Histo1->GetMaximum();
  
  Histo1->SetMaximum(1.1*Max);
    
  Histo1->SetLineColor(kBlack);
  Histo2->SetLineColor(kRed);
  Histo1->SetMarkerColor(kBlack);
  Histo2->SetMarkerColor(kRed);
  
  char Option1[60];
  if(!strcmp(Option,"")) strcpy(Option1,"HIST");
  else strcpy(Option1,Option);
  
  Histo1->Draw(Option1);
  
  char Option2[60];
  strcpy(Option2,"same ");
  strcat(Option2,Option1);
  
  Histo2->Draw(Option2);
  
  Leg->Draw("same");
  
  Can->Print(CName,"png");
  Can->Close();
}


//**************************************
//    FROM DANEK
static int module_online(int moduleCMSSW){
  int newModule = moduleCMSSW -4; 
  if (newModule<=0) newModule--;
  return newModule;
}

static int ladder_online(int layer, int ladderCMSSW){

  int newLadder = ladderCMSSW;
    
  if (layer == 1) {
    if (newLadder <= 5) newLadder = 6-newLadder;
    else if (newLadder >= 6 && newLadder <= 15 ) newLadder = 5-newLadder;
    else if (newLadder >= 16) newLadder = 26-newLadder;
  } 
  else if (layer == 2) {
    if (newLadder <= 8) newLadder = 9-newLadder;
    else if (newLadder >= 9 && newLadder <= 24) newLadder = 8-newLadder;
    else if (newLadder >= 25) newLadder = 41-newLadder; 
  } 
  else if (layer == 3) {
    if (newLadder <= 11) newLadder = 12-newLadder;
    else if (newLadder >= 12 && newLadder <= 33) newLadder = 11-newLadder;
    else if (newLadder >= 34) newLadder = 56-newLadder;
  } 

  return newLadder;
}
