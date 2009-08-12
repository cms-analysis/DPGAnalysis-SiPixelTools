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
#include "TCanvas.h"
#include "TGaxis.h"
#include "TGraphAsymmErrors.h"

  using namespace std;
  bool DEBUG=false;
  int Nfiles=450;
  
  
//************** Quick function to merge easilly histos **************
void mergeHisto(char* name,TH1F* histo, bool mergeOverflows = false){
    TH1F*  temp=(TH1F*)gDirectory->Get(name);
    
    if(!mergeOverflows)
      for (int bin=1; bin<=histo->GetNbinsX(); bin++)
        histo->SetBinContent( bin, histo->GetBinContent(bin) + temp->GetBinContent(bin));

    if(mergeOverflows)
      for (int bin=0; bin<=histo->GetNbinsX()+1; bin++)
        histo->SetBinContent( bin, histo->GetBinContent(bin) + temp->GetBinContent(bin));
}

void mergeHisto(char* name,TH2F* histo){
    TH2F*  temp=(TH2F*)gDirectory->Get(name);
      for (int binx=1; binx<=histo->GetNbinsX(); binx++){
        for (int biny=1; biny<=histo->GetNbinsY(); biny++)
	  histo->SetBinContent( binx, biny, histo->GetBinContent(binx,biny) + temp->GetBinContent(binx,biny));
	}

}

void makeEfficiency(TH1F* valid, TH1F* missing, TH1F* efficiency){
  for(int i=1;i<=efficiency->GetNbinsX();i++){
    double val = valid->GetBinContent(i);
    double mis = missing->GetBinContent(i);
    if( (val+mis)!=0 ){
      double eff = val / (val+mis);
      double err = sqrt(eff*(1-eff)/(val+mis));
      efficiency->SetBinContent(i,eff);
      efficiency->SetBinError(i,err);
    }
  }
  efficiency->GetYaxis()->SetTitle("#epsilon_{hit}");
}

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
}

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

void summHist(TH1F* h1, TH1F* h2, TH1F* h3){
  for (int bin=1;bin<=h1->GetNbinsX();bin++)
    h3->SetBinContent(bin,h1->GetBinContent(bin)+h2->GetBinContent(bin));
  }

void Canv(TH1F *Histo1,TH1F *Histo2,char CName[100], char Option[60], TLegend* Leg, bool integrate = false)
{
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
  TH1F* invalidPerRunMerged = new  TH1F("invalidPerRunMerged","invalidPerRunMerged",200,0,200);
  TH1F* inactivePerRunMerged = new  TH1F("inactivePerRunMerged","inactivePerRunMerged",200,0,200);
  TH1F* efficiencyPerRun = new  TH1F("efficiencyPerRun","efficiencyPerRun",200,0,200);
  TH1F* inactivePercentPerRun = new  TH1F("inactivePercentPerRun","inactivePercentPerRun",200,0,200);

  TH1F* validVsAlphaMerged = new TH1F("validVsAlphaMerged","validVsAlphaMerged",200,-3.5,3.5);
  TH1F* validVsCotanAlphaMerged = new TH1F("validVsCotanAlphaMerged","validVsCotanAlphaMerged",200,-3.5,3.5);
  TH1F* validVsAlphaBPixMerged = new TH1F("validVsAlphaBPixMerged","validVsAlphaMerged",200,-3.5,3.5);
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
  
  TH1F* validVsLocalXMerged = new TH1F("validVsLocalXMerged","validVsLocalXMerged",100,-1.5,1.5);
  TH1F* validVsLocalYMerged = new TH1F("validVsLocalYMerged","validVsLocalYMerged",100,-4.,4.);
  TH1F* missingVsLocalXMerged = new TH1F("missingVsLocalXMerged","missingVsLocalXMerged",100,-1.5,1.5);
  TH1F* missingVsLocalYMerged = new TH1F("missingVsLocalYMerged","missingVsLocalYMerged",100,-4.,4.);

  TH1F* validVsMuontimePre68094Merged    = new TH1F("validVsMuontimePre68094Merged","validVsMuontimePre68094Merged",50,-40.,80.);
  TH1F* missingVsMuontimePre68094Merged  = new TH1F("missingVsMuontimePre68094Merged","missingVsMuontimePre68094Merged",50,-40.,80.);
  TH1F* validVsMuontimePost68094Merged   = new TH1F("validVsMuontimePost68094Merged","validVsMuontimePost68094Merged",50,-40.,80.);
  TH1F* missingVsMuontimePost68094Merged = new TH1F("missingVsMuontimePost68094Merged","missingVsMuontimePost68094Merged",50,-40.,80.);

  TH1F* validVsPTMerged   = new TH1F("validVsPTMerged","validVsPTMerged",100,0.,50.);
  TH1F* missingVsPTMerged = new TH1F("missingVsPTMerged","missingVsPTMerged",100,0.,50.);

  TH1F* validVsLocalXBigMerged     = new TH1F("validVsLocalXBigMerged","validVsLocalXBigMerged",100,-1.5,1.5);
  TH1F* missingVsLocalXBigMerged   = new TH1F("missingVsLocalXBigMerged","missingVsLocalXBigMerged",100,-1.5,1.5);
  TH1F* validVsLocalXSmallMerged   = new TH1F("validVsLocalXSmallMerged","validVsLocalXSmallMerged",100,-1.5,1.5);
  TH1F* missingVsLocalXSmallMerged = new TH1F("missingVsLocalXSmallMerged","missingVsLocalXSmallMerged",100,-1.5,1.5);

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

  TH2F* tunningValMerged = new TH2F("tunningValMerged" ,"tunningValMerged" ,50,0,5,40,0,40);
  TH2F* tunningMisMerged = new TH2F("tunningMisMerged" ,"tunningMisMerged" ,50,0,5,40,0,40);
  TH1F* tunningEdgeValMerged = new TH1F("tunningEdgeValMerged","tunningEdgeValMerged",50,0,5);
  TH1F* tunningEdgeMisMerged = new TH1F("tunningEdgeMisMerged","tunningEdgeMisMerged",50,0,5);
  TH1F* tunningMuonValMerged = new TH1F("tunningMuonValMerged","tunningMuonValMerged",40,0,40);
  TH1F* tunningMuonMisMerged = new TH1F("tunningMuonMisMerged","tunningMuonMisMerged",40,0,40);
    
  TH1F* histAlphaAnalysis = new TH1F("histAlphaAnalysis", "hist", 200, -3.5,3.5);
  TH1F* histAlphaAnalysisBPix = new TH1F("histAlphaAnalysisBPix", "hist", 200, -3.5,3.5);
  TH1F* histAlphaAnalysisFPix = new TH1F("histAlphaAnalysisFPix", "hist", 200, -3.5,3.5);
  TH1F* histCotanAlphaAnalysis = new TH1F("histCotanAlphaAnalysis", "hist:cotan(#alpha)", 200, -3.5,3.5); 
  TH1F* histBetaAnalysis = new TH1F("histBetaAnalysis", "hist", 200, -3.5,3.5); 
  TH1F* histBetaAnalysisBPix = new TH1F("histBetaAnalysisBPix", "hist", 200, -3.5,3.5); 
  TH1F* histBetaAnalysisFPix = new TH1F("histBetaAnalysisFPix", "hist", 200, -3.5,3.5); 
  
  TGraphAsymmErrors*  histAlphaAnalysisBPixGraph;
  TGraphAsymmErrors*  histBetaAnalysisBPixGraph;
  
  TH2F* tunningEfficiency = new TH2F("tunningEfficiency" ,"tunningEfficiency" ,50,0,5,40,0,40);
  TH1F* tunningEdgeEfficiency = new TH1F("tunningEdgeEfficiency","tunningEdgeEfficiency",50,0,5);
  TH1F* tunningMuonEfficiency = new TH1F("tunningMuonEfficiency","tunningMuonEfficiency",40,0,40);
  TH1F* tunningSlice          = new TH1F("tunningSlice","tunningSlice",40,0,40);
  TH1F* nTotVsTunningMuon     = new TH1F("nTotVsTunningMuon","nTotVsTunningMuon",40,0,40); 

  TH1F* muontimePre68094Efficiency = new TH1F("muontimePre68094Efficiency","muontimePre68094Efficiency",50,-40.,80.);
  TH1F* muontimePost68094Efficiency = new TH1F("muontimePost68094Efficiency","muontimePost68094Efficiency",50,-40.,80.);


 
 TH2F* validMuonTimeVSchargeBPixMerged   = new TH2F("validMuonTimeVSchargeBPixMerged","validMuonTimeVSchargeBPixMerged",60,-40.,80.,200,0,200000);
 TH2F* missingMuonTimeVSchargeBPixMerged = new TH2F("missingMuonTimeVSchargeBPixMerged","missingMuonTimeVSchargeBPixMerged",60,-40.,80.,200,0,200000);
 TH2F* validMuonTimeVSchargeFPixMerged   = new TH2F("validMuonTimeVSchargeFPixMerged","validMuonTimeVSchargeFPixMerged",60,-40.,80.,200,0,200000);
 TH2F* missingMuonTimeVSchargeFPixMerged = new TH2F("missingMuonTimeVSchargeFPixMerged","missingMuonTimeVSchargeFPixMerged",60,-40.,80.,200,0,200000);

 TH2F* efficiencyMuonTimeVSchargeBPix = new TH2F("efficiencyMuonTimeVSchargeBPix","efficiencyMuonTimeVSchargeBPix",60,-40.,80.,200,0,200000);
 TH2F* efficiencyMuonTimeVSchargeFPix = new TH2F("efficiencyMuonTimeVSchargeFPix","efficiencyMuonTimeVSchargeFPix",60,-40.,80.,200,0,200000);

  TH1F* PTEfficiency = new TH1F ("PTEfficiency","PTEfficiency",100,0,50);

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
    
    /*for (int bin=1; bin<=3; bin++){
      TH1F *h1 = (TH1F*)gDirectory->Get( "histo" );
      histoMerged->SetBinContent( bin, histoMerged->GetBinContent(bin) + h1->GetBinContent(bin));
    
      TH1F *h2 = (TH1F*)gDirectory->Get( "histLayer1" ); 
      histLayer1Merged->SetBinContent( bin, histLayer1Merged->GetBinContent(bin) + h2->GetBinContent(bin));
    
      TH1F *h3 = (TH1F*)gDirectory->Get( "histLayer2" );
      histLayer2Merged->SetBinContent( bin, histLayer2Merged->GetBinContent(bin) + h3->GetBinContent(bin));
    
      TH1F *h4 = (TH1F*)gDirectory->Get( "histLayer3" );
      histLayer3Merged->SetBinContent( bin, histLayer3Merged->GetBinContent(bin) + h4->GetBinContent(bin));
    
      TH1F *h5 = (TH1F*)gDirectory->Get( "histEndcapPlus" );
      histEndcapPlusMerged->SetBinContent( bin, histEndcapPlusMerged->GetBinContent(bin) + h5->GetBinContent(bin));
    
      TH1F *h6 = (TH1F*)gDirectory->Get( "histEndcapMinus" );
      histEndcapMinusMerged->SetBinContent( bin, histEndcapMinusMerged->GetBinContent(bin) + h6->GetBinContent(bin));
    
      TH1F *h7 = (TH1F*)gDirectory->Get( "histBarrel" );
      histBarrelMerged->SetBinContent( bin, histBarrelMerged->GetBinContent(bin) + h7->GetBinContent(bin));
    
      TH1F *h8 = (TH1F*)gDirectory->Get( "histEndcap" );
      histEndcapMerged->SetBinContent( bin, histEndcapMerged->GetBinContent(bin) + h8->GetBinContent(bin));
    
      TH1F *h9 = (TH1F*)gDirectory->Get( "consistencyCheck" );
      consistencyCheckMerged->SetBinContent( bin, consistencyCheckMerged->GetBinContent(bin) + h9->GetBinContent(bin));
      delete h1;
      delete h2;
      delete h3;
      delete h4;
      delete h5;
      delete h6;
      delete h7;
      delete h8;
      delete h9;
    }*/
      
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
  
    
    /*for (int bin=1; bin<=4; bin++)
      {
      TH1F *h10 = (TH1F*)gDirectory->Get( "histInvalidRecHitCollection" );
      histInvalidRecHitCollectionMerged->SetBinContent( bin, histInvalidRecHitCollectionMerged->GetBinContent(bin) + h10->GetBinContent(bin));
      delete h10;
      }
    for (int bin=1; bin<=5; bin++)
      {
      TH1F *h12 = (TH1F*)gDirectory->Get( "histInvalidRecHitWithBadmoduleList" );
      histInvalidRecHitWithBadmoduleListMerged->SetBinContent( bin, histInvalidRecHitWithBadmoduleListMerged->GetBinContent(bin) + h12->GetBinContent(bin));
      delete h12;
      }      

    for (int bin=1; bin<=3; bin++)
      {
      TH1F *h16 = (TH1F*)gDirectory->Get( "consistencyCheckTraj" );
      consistencyCheckTrajMerged->SetBinContent( bin, consistencyCheckTrajMerged->GetBinContent(bin) + h16->GetBinContent(bin));
      delete h16;
      } */     
    
    mergeHisto("histInvalidRecHitWithBadmoduleList",histInvalidRecHitWithBadmoduleListMerged);
    mergeHisto("histInvalidRecHitCollection",histInvalidRecHitCollectionMerged);
    
    TH1F *histoMethod2 = (TH1F*)gDirectory->Get( "histoMethod2" );
    TH1F *histoMethod2After = (TH1F*)gDirectory->Get( "histoMethod2After" );
    TH1F *histoMethod2FPix = (TH1F*)gDirectory->Get( "histoMethod2FPix" );
    TH1F *histoMethod2AfterFPix = (TH1F*)gDirectory->Get( "histoMethod2AfterFPix" );
      
    for(int bin=1; bin<=3; bin++){
      histoMethod2Merged->SetBinContent( bin, histoMethod2Merged->GetBinContent(bin) + histoMethod2->GetBinContent(bin));
      histoMethod2AfterMerged->SetBinContent( bin, histoMethod2AfterMerged->GetBinContent(bin) + histoMethod2After->GetBinContent(bin));
      histoMethod2FPixMerged->SetBinContent( bin, histoMethod2FPixMerged->GetBinContent(bin) + histoMethod2FPix->GetBinContent(bin));
      histoMethod2AfterFPixMerged->SetBinContent( bin, histoMethod2AfterFPixMerged->GetBinContent(bin) + histoMethod2AfterFPix->GetBinContent(bin));
    }
 
    if(DEBUG) cout<<"********* NOW MERGING ANALYSIS PER RUN ************"<<endl;

      
//merging analysisPerRun
    int lastBin=-999;
    for (int bin=1; bin<=validPerRunMerged->GetNbinsX();bin++)
      {
      if ( !strcmp(validPerRunMerged->GetXaxis()->GetBinLabel(bin),"") ) {lastBin=bin; break;}
      }

    TH1F *h13 = (TH1F*)gDirectory->Get( "validPerRun" );
    TH1F *h14 = (TH1F*)gDirectory->Get( "invalidPerRun" );
    TH1F *h15 = (TH1F*)gDirectory->Get( "inactivePerRun" );
    for (int bin=1; bin<=h13->GetNbinsX(); bin++)
      {
      bool founded=false;
      for (int binMerged=1; binMerged<=validPerRunMerged->GetNbinsX(); binMerged++)
        {
	if ( !strcmp(h13->GetXaxis()->GetBinLabel(bin),validPerRunMerged->GetXaxis()->GetBinLabel(binMerged)) )
	  {
	  founded=true;
	  validPerRunMerged->SetBinContent( binMerged,validPerRunMerged->GetBinContent(binMerged)+h13->GetBinContent(bin));
	  invalidPerRunMerged->SetBinContent(binMerged,invalidPerRunMerged->GetBinContent(binMerged)+h14->GetBinContent(bin));
	  inactivePerRunMerged->SetBinContent(binMerged,inactivePerRunMerged->GetBinContent(binMerged)+h15->GetBinContent(bin));
	  break;
	  }
	}
      if (!founded)
	{
	validPerRunMerged->GetXaxis()->SetBinLabel(lastBin, h13->GetXaxis()->GetBinLabel(bin) );
	validPerRunMerged->SetBinContent( lastBin,h13->GetBinContent(bin) );
	invalidPerRunMerged->GetXaxis()->SetBinLabel(lastBin, h14->GetXaxis()->GetBinLabel(bin) );
	invalidPerRunMerged->SetBinContent( lastBin,h14->GetBinContent(bin) );
	inactivePerRunMerged->GetXaxis()->SetBinLabel(lastBin, h15->GetXaxis()->GetBinLabel(bin) );
	inactivePerRunMerged->SetBinContent( lastBin,h15->GetBinContent(bin) );
	lastBin++;
	}
      }


    if(DEBUG) cout<<"********* NOW MERGING ANALYSIS FOR TRAJECTORY ************"<<endl;


    TH1F *htest = (TH1F*)gDirectory->Get( "checkoutTraj" );
    for (int bin=1; bin<=10; bin++)
      checkoutTrajMerged->SetBinContent( bin, checkoutTrajMerged->GetBinContent(bin) + htest->GetBinContent(bin));  
    delete htest;
    
//update to alpha and beta
    TH1F *validVsAlpha = (TH1F*)gDirectory->Get( "validVsAlpha" );
    mergeHisto("validVsCotanAlpha",validVsCotanAlphaMerged);
    TH1F *validVsAlphaBPix = (TH1F*)gDirectory->Get( "validVsAlphaBPix" );
    TH1F *validVsAlphaFPix = (TH1F*)gDirectory->Get( "validVsAlphaFPix" );
    TH1F *missingVsAlpha = (TH1F*)gDirectory->Get( "missingVsAlpha" );
    TH1F *missingVsAlphaBPix = (TH1F*)gDirectory->Get( "missingVsAlphaBPix" );
    TH1F *missingVsAlphaFPix = (TH1F*)gDirectory->Get( "missingVsAlphaFPix" );
    TH1F *validVsBeta = (TH1F*)gDirectory->Get( "validVsBeta" );
    mergeHisto("missingVsCotanAlpha",missingVsCotanAlphaMerged);
    //TH1F *validVsBetaBPix = (TH1F*)gDirectory->Get( "validVsBetaBPix" );
    TH1F *validVsBetaFPix = (TH1F*)gDirectory->Get( "validVsBetaFPix" );
    TH1F *missingVsBeta = (TH1F*)gDirectory->Get( "missingVsBeta" );
    TH1F *missingVsBetaBPix = (TH1F*)gDirectory->Get( "missingVsBetaBPix" );
    TH1F *missingVsBetaFPix = (TH1F*)gDirectory->Get( "missingVsBetaFPix" );

    for (int bin=1; bin<=200; bin++){
      validVsAlphaMerged->SetBinContent( bin, validVsAlphaMerged->GetBinContent(bin) + validVsAlpha->GetBinContent(bin));
      validVsAlphaBPixMerged->SetBinContent( bin, validVsAlphaBPixMerged->GetBinContent(bin) + validVsAlphaBPix->GetBinContent(bin));
      validVsAlphaFPixMerged->SetBinContent( bin, validVsAlphaFPixMerged->GetBinContent(bin) + validVsAlphaFPix->GetBinContent(bin));
      missingVsAlphaMerged->SetBinContent( bin, missingVsAlphaMerged->GetBinContent(bin) + missingVsAlpha->GetBinContent(bin));
      missingVsAlphaBPixMerged->SetBinContent( bin, missingVsAlphaBPixMerged->GetBinContent(bin) + missingVsAlphaBPix->GetBinContent(bin));
      missingVsAlphaFPixMerged->SetBinContent( bin, missingVsAlphaFPixMerged->GetBinContent(bin) + missingVsAlphaFPix->GetBinContent(bin));
      validVsBetaMerged->SetBinContent( bin, validVsBetaMerged->GetBinContent(bin) + validVsBeta->GetBinContent(bin));
      validVsBetaBPixMerged->SetBinContent( bin, validVsBetaBPixMerged->GetBinContent(bin) + validVsBeta->GetBinContent(bin));
      validVsBetaFPixMerged->SetBinContent( bin, validVsBetaFPixMerged->GetBinContent(bin) + validVsBetaFPix->GetBinContent(bin));
      missingVsBetaMerged->SetBinContent( bin, missingVsBetaMerged->GetBinContent(bin) + missingVsBeta->GetBinContent(bin));
      missingVsBetaBPixMerged->SetBinContent( bin, missingVsBetaBPixMerged->GetBinContent(bin) + missingVsBetaBPix->GetBinContent(bin));
      missingVsBetaFPixMerged->SetBinContent( bin, missingVsBetaFPixMerged->GetBinContent(bin) + missingVsBetaFPix->GetBinContent(bin));
    }

  
    TH1F *missingVsLocalX = (TH1F*)gDirectory->Get( "missingVsLocalX" );
    TH1F *missingVsLocalY = (TH1F*)gDirectory->Get( "missingVsLocalY" );
    TH1F *validVsLocalX = (TH1F*)gDirectory->Get( "validVsLocalX" );
    TH1F *validVsLocalY = (TH1F*)gDirectory->Get( "validVsLocalY" );

    for (int bin=1; bin<=missingVsLocalX->GetNbinsX(); bin++){
      missingVsLocalXMerged->SetBinContent( bin, missingVsLocalXMerged->GetBinContent(bin) + missingVsLocalX->GetBinContent(bin));
      missingVsLocalYMerged->SetBinContent( bin, missingVsLocalYMerged->GetBinContent(bin) + missingVsLocalY->GetBinContent(bin));
      validVsLocalXMerged->SetBinContent( bin, validVsLocalXMerged->GetBinContent(bin) + validVsLocalX->GetBinContent(bin));
      validVsLocalYMerged->SetBinContent( bin, validVsLocalYMerged->GetBinContent(bin) + validVsLocalY->GetBinContent(bin));
    }
    
    
    
    
    for (int bin=1; bin<=101; bin++)
      {
      TH1F *h17 = (TH1F*)gDirectory->Get( "inactivePerTrack" );
      inactivePerTrackMerged->SetBinContent( bin, inactivePerTrackMerged->GetBinContent(bin) + h17->GetBinContent(bin));
      TH1F *h18 = (TH1F*)gDirectory->Get( "missingPerTrack" );
      missingPerTrackMerged->SetBinContent( bin, missingPerTrackMerged->GetBinContent(bin) + h18->GetBinContent(bin));
      
      delete h17;
      delete h18;
      }

    for (int bin=1; bin<=4; bin++)
      {
      TH1F *h19 = (TH1F*)gDirectory->Get( "checkoutValidityFlag" );
      checkoutValidityFlagMerged->SetBinContent( bin, checkoutValidityFlagMerged->GetBinContent(bin) + h19->GetBinContent(bin));
 
      delete h19;
      }

    if(DEBUG) cout<<"********* NOW MERGING analysis for numerator and denominator ************"<<endl;
   
    TH2F * xPosFracVal= (TH2F*)gDirectory->Get("xPosFracVal");
    for (int cutBin=1; cutBin<=6; cutBin++){
      for (int bin=0; bin<=xPosFracValMerged->GetNbinsX()+1; bin++)
        xPosFracValMerged->SetBinContent( bin,cutBin, xPosFracValMerged->GetBinContent(bin,cutBin) + xPosFracVal->GetBinContent(bin,cutBin));
      }

    TH2F * xPosFracMis= (TH2F*)gDirectory->Get("xPosFracMis");
    for (int cutBin=1; cutBin<=6; cutBin++){
      for (int bin=0; bin<=xPosFracMisMerged->GetNbinsX()+1; bin++)
        xPosFracMisMerged->SetBinContent( bin,cutBin, xPosFracMisMerged->GetBinContent(bin,cutBin) + xPosFracMis->GetBinContent(bin,cutBin));
      }

    TH2F * yPosFracVal= (TH2F*)gDirectory->Get("yPosFracVal");
    for (int cutBin=1; cutBin<=6; cutBin++){    
      for (int bin=0; bin<=yPosFracValMerged->GetNbinsX()+1; bin++)
        yPosFracValMerged->SetBinContent( bin,cutBin, yPosFracValMerged->GetBinContent(bin,cutBin) + yPosFracVal->GetBinContent(bin,cutBin));
      }

    TH2F * yPosFracMis= (TH2F*)gDirectory->Get("yPosFracMis");
    for (int cutBin=1; cutBin<=6; cutBin++){
      for (int bin=0; bin<=yPosFracMisMerged->GetNbinsX()+1; bin++)
        yPosFracMisMerged->SetBinContent( bin,cutBin, yPosFracMisMerged->GetBinContent(bin,cutBin) + yPosFracMis->GetBinContent(bin,cutBin));
      }


    TH1F*  hitsPassingCutsValBPix=(TH1F*)gDirectory->Get("hitsPassingCutsValBPix");
    for (int bin=1; bin<=hitsPassingCutsValBPixMerged->GetNbinsX(); bin++)
      hitsPassingCutsValBPixMerged->SetBinContent( bin, hitsPassingCutsValBPix->GetBinContent(bin) + hitsPassingCutsValBPixMerged->GetBinContent(bin));

    TH1F*  hitsPassingCutsMisBPix=(TH1F*)gDirectory->Get("hitsPassingCutsMisBPix");
    for (int bin=1; bin<=hitsPassingCutsMisBPixMerged->GetNbinsX(); bin++)
      hitsPassingCutsMisBPixMerged->SetBinContent( bin, hitsPassingCutsMisBPix->GetBinContent(bin) + hitsPassingCutsMisBPixMerged->GetBinContent(bin));
    
    TH1F*  hitsPassingCutsValFPix=(TH1F*)gDirectory->Get("hitsPassingCutsValFPix");
    for (int bin=1; bin<=hitsPassingCutsValFPixMerged->GetNbinsX(); bin++)
      hitsPassingCutsValFPixMerged->SetBinContent( bin, hitsPassingCutsValFPix->GetBinContent(bin) + hitsPassingCutsValFPixMerged->GetBinContent(bin));

    TH1F*  hitsPassingCutsMisFPix=(TH1F*)gDirectory->Get("hitsPassingCutsMisFPix");
    for (int bin=1; bin<=hitsPassingCutsMisFPixMerged->GetNbinsX(); bin++)
      hitsPassingCutsMisFPixMerged->SetBinContent( bin, hitsPassingCutsMisFPix->GetBinContent(bin) + hitsPassingCutsMisFPixMerged->GetBinContent(bin));
    
    for (int bin=1; bin<=hitsPassingCutsValMerged->GetNbinsX(); bin++)
      hitsPassingCutsValMerged->SetBinContent( bin, hitsPassingCutsValBPix->GetBinContent(bin) + hitsPassingCutsValFPix->GetBinContent(bin) + hitsPassingCutsValMerged->GetBinContent(bin));

    for (int bin=1; bin<=hitsPassingCutsMisMerged->GetNbinsX(); bin++)
      hitsPassingCutsMisMerged->SetBinContent( bin, hitsPassingCutsMisBPix->GetBinContent(bin) + hitsPassingCutsMisFPix->GetBinContent(bin) + hitsPassingCutsMisMerged->GetBinContent(bin));
    
    
    //***TO COMMENT IF U WANT TO RUN
    /*TH1F*  hitsPassingCutsVal=(TH1F*)gDirectory->Get("hitsPassingCutsVal");
    for (int bin=1; bin<=hitsPassingCutsValMerged->GetNbinsX(); bin++)
      hitsPassingCutsValMerged->SetBinContent( bin, hitsPassingCutsValBPix->GetBinContent(bin) + hitsPassingCutsValFPix->GetBinContent(bin) + hitsPassingCutsValMerged->GetBinContent(bin));

    TH1F*  hitsPassingCutsMis=(TH1F*)gDirectory->Get("hitsPassingCutsMis");
    for (int bin=1; bin<=hitsPassingCutsMisMerged->GetNbinsX(); bin++)
      hitsPassingCutsMisMerged->SetBinContent( bin, hitsPassingCutsMisBPix->GetBinContent(bin) + hitsPassingCutsMisFPix->GetBinContent(bin) + hitsPassingCutsMisMerged->GetBinContent(bin));
*/
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
    mergeHisto("validVsMuontimePost68094",validVsMuontimePost68094Merged);
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
    
    TH1F* numbPixInClusterX =(TH1F*)gDirectory->Get("numbPixInClusterX");
    for (int bin=1; bin<=numbPixInClusterXMerged->GetNbinsX(); bin++)
      numbPixInClusterXMerged->SetBinContent( bin, numbPixInClusterX->GetBinContent(bin) + numbPixInClusterXMerged->GetBinContent(bin));
    
    TH1F* numbPixInClusterY =(TH1F*)gDirectory->Get("numbPixInClusterY");
    for (int bin=1; bin<=numbPixInClusterYMerged->GetNbinsX(); bin++)
      numbPixInClusterYMerged->SetBinContent( bin, numbPixInClusterY->GetBinContent(bin) + numbPixInClusterYMerged->GetBinContent(bin));
    
    /*TH1F* xposCluster =(TH1F*)gDirectory->Get("xposCluster");
    for (int bin=1; bin<=xposClusterMerged->GetNbinsX(); bin++)
      xposClusterMerged->SetBinContent( bin, xposCluster->GetBinContent(bin) + xposClusterMerged->GetBinContent(bin));
    
    TH1F*  yposCluster=(TH1F*)gDirectory->Get("yposCluster");
    for (int bin=1; bin<=yposClusterMerged->GetNbinsX(); bin++)
      yposClusterMerged->SetBinContent( bin, yposCluster->GetBinContent(bin) + yposClusterMerged->GetBinContent(bin));
    */
    TH1F* xposClusterValid =(TH1F*)gDirectory->Get("xposClusterValid");
    for (int bin=1; bin<=xposClusterValidMerged->GetNbinsX(); bin++)
      xposClusterValidMerged->SetBinContent( bin, xposClusterValid->GetBinContent(bin) + xposClusterValidMerged->GetBinContent(bin));
    
    TH1F* yposClusterValid =(TH1F*)gDirectory->Get("yposClusterValid");
    for (int bin=1; bin<=yposClusterValidMerged->GetNbinsX(); bin++)
      yposClusterValidMerged->SetBinContent( bin, yposClusterValid->GetBinContent(bin) + yposClusterValidMerged->GetBinContent(bin));
    
    TH1F* xposClusterMisRecovered =(TH1F*)gDirectory->Get("xposClusterMisRecovered");
    for (int bin=1; bin<=xposClusterMisRecoveredMerged->GetNbinsX(); bin++)
      xposClusterMisRecoveredMerged->SetBinContent( bin, xposClusterMisRecovered->GetBinContent(bin) + xposClusterMisRecoveredMerged->GetBinContent(bin));
    
    TH1F*  yposClusterMisRecovered=(TH1F*)gDirectory->Get("yposClusterMisRecovered");
    for (int bin=1; bin<=yposClusterMisRecoveredMerged->GetNbinsX(); bin++)
      yposClusterMisRecoveredMerged->SetBinContent( bin, yposClusterMisRecovered->GetBinContent(bin) + yposClusterMisRecoveredMerged->GetBinContent(bin));

 

    if(DEBUG) cout<<"********* NOW MERGING WINDOW SEARCH ************"<<endl;


    TH1F *windowSearch = (TH1F*)gDirectory->Get( "windowSearch" );
    for (int bin=1; bin<=windowSearchMerged->GetNbinsX()+1; bin++)
      windowSearchMerged->SetBinContent( bin, windowSearchMerged->GetBinContent(bin) + windowSearch->GetBinContent(bin));
 
     TH1F *windowSearchSameModule = (TH1F*)gDirectory->Get( "windowSearchSameModule" );
    for (int bin=1; bin<=windowSearchSameModuleMerged->GetNbinsX()+1; bin++)
      windowSearchSameModuleMerged->SetBinContent( bin, windowSearchSameModuleMerged->GetBinContent(bin) + windowSearchSameModule->GetBinContent(bin));
    
    TH1F *windowSearchBPix = (TH1F*)gDirectory->Get( "windowSearchBPix" );
    for (int bin=1; bin<=windowSearchBPixMerged->GetNbinsX()+1; bin++)
      windowSearchBPixMerged->SetBinContent( bin, windowSearchBPixMerged->GetBinContent(bin) + windowSearchBPix->GetBinContent(bin));
    
    TH1F *windowSearchGoodModulesBPix = (TH1F*)gDirectory->Get( "windowSearchGoodModulesBPix" );
    for (int bin=1; bin<=windowSearchGoodModulesBPixMerged->GetNbinsX()+1; bin++)
      windowSearchGoodModulesBPixMerged->SetBinContent( bin,windowSearchGoodModulesBPixMerged->GetBinContent(bin) + windowSearchGoodModulesBPix->GetBinContent(bin));

    TH1F *windowSearchFPix = (TH1F*)gDirectory->Get( "windowSearchFPix" );
    for (int bin=1; bin<=windowSearchFPixMerged->GetNbinsX()+1; bin++)
      windowSearchFPixMerged->SetBinContent( bin, windowSearchFPixMerged->GetBinContent(bin) + windowSearchFPix->GetBinContent(bin));
    
    TH1F *missingButClusterOnSameModule = (TH1F*)gDirectory->Get( "missingButClusterOnSameModule" );
    missingButClusterOnSameModuleMerged->Fill(0.,missingButClusterOnSameModule->GetBinContent(1));
    missingButClusterOnSameModuleMerged->Fill(1.,missingButClusterOnSameModule->GetBinContent(2));
    
    TH1F *missingButCluster = (TH1F*)gDirectory->Get( "missingButCluster" );
    missingButClusterMerged->Fill(0.,missingButCluster->GetBinContent(1));
    missingButClusterMerged->Fill(1.,missingButCluster->GetBinContent(2));
//
    
    if(DEBUG) cout<<"********* ALL HISTO WERE SUCCESSFULLY MERGED ************"<<endl;
    if(DEBUG) cout<<"********* NOW STARTING TO MERGE TTREE ************"<<endl;

    
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
    
   invalidPerRunMerged->LabelsDeflate("X"); 
   validPerRunMerged->LabelsDeflate("X"); 
   inactivePerRunMerged->LabelsDeflate("X"); 
    
   for (int i=1; i<=validPerRunMerged->GetNbinsX(); i++)
     {
     double a,b;
     a=validPerRunMerged->GetBinContent(i);
     //by definition: take into account only VALID and MISSING!
     b=(invalidPerRunMerged->GetBinContent(i)-inactivePerRunMerged->GetBinContent(i));
     if ((a+b)!=0)
       {
       efficiencyPerRun->SetBinContent(i,(a)/(a+b));
       efficiencyPerRun->SetBinError( i,sqrt( (a/(a+b))*(1-(a/(a+b)))/(a+b)) );
       efficiencyPerRun->GetXaxis()->SetBinLabel( i,validPerRunMerged->GetXaxis()->GetBinLabel(i) );
       }
/*     else 
       {
       efficiencyPerRun->SetBinContent(i,0);
       efficiencyPerRun->GetXaxis()->SetBinLabel( i,"deleted" );
       }
*/       
     inactivePercentPerRun->SetBinContent(i,inactivePerRunMerged->GetBinContent(i)/(invalidPerRunMerged->GetBinContent(i)+validPerRunMerged->GetBinContent(i)) );
     inactivePercentPerRun->GetXaxis()->SetBinLabel( i,validPerRunMerged->GetXaxis()->GetBinLabel(i) );
       
     } 
    
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
  summHist(validVsBetaBPixMerged,missingVsBetaBPixMerged,auxDen);
  histBetaAnalysisBPixGraph = new TGraphAsymmErrors (validVsBetaBPixMerged,auxDen);  

  TH1F* auxDen2 = new TH1F("auxDen2","auxDen2",validVsAlphaBPixMerged->GetNbinsX(),validVsAlphaBPixMerged->GetMinimumBin(),validVsAlphaBPixMerged->GetMaximumBin());
  summHist(validVsAlphaBPixMerged,missingVsAlphaBPixMerged,auxDen2);
  histAlphaAnalysisBPixGraph = new TGraphAsymmErrors (validVsAlphaBPixMerged,auxDen2);
  
  makeEfficiencyNoError(validVsBetaBPixMerged,missingVsBetaBPixMerged,histBetaAnalysisBPix);
  makeEfficiencyNoError(validVsAlphaBPixMerged,missingVsAlphaBPixMerged,histAlphaAnalysisBPix);
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
  for (int bin=1;bin<=100;bin++)
    {
    double val=validVsLocalXMerged->GetBinContent(bin);
    double mis=missingVsLocalXMerged->GetBinContent(bin);
    if ((val+mis)!=0)
      {
      localXAnalysis->SetBinContent(bin,val/(val+mis));
      error=0;
      error=sqrt(((val)/(val+mis))*(1-((val)/(val+mis)))/(val+mis));
      localXAnalysis->SetBinError(bin,error);
      }
    }
  TH1F* localYAnalysis = new TH1F("localYAnalysis", "hist", 100,-4.,4.); 
  for (int bin=1;bin<=100;bin++)
    {
    double val=validVsLocalYMerged->GetBinContent(bin);
    double mis=missingVsLocalYMerged->GetBinContent(bin);
    if ((val+mis)!=0)
      {
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
    std::cout<<"dei, tiraghe via sto mona..."<< id <<std::endl;
    if (( valid+missing)!=0)
      {
      double setbin = (double)valid/(double)(valid+missing);
      error=0;
      error=sqrt( ((double)valid/(double)(valid+missing))*(1-((double)valid/(double)(valid+missing)))/(double)(valid+missing) );
      
      moduleBreakoutEff->SetBinContent(n+1,setbin);
      moduleBreakoutEff->SetBinError(n+1,error);

      if ( (isModuleBad==0) && (isBarrelModule==1) )
        {
        moduleGoodBPix->SetBinContent(binGoodBPix , setbin);
        moduleGoodBPix->SetBinError(binGoodBPix , error);
        binGoodBPix++;
	}
      if ( (isModuleBad==1) && (isBarrelModule==1) )
        {
        moduleBadBPix->SetBinContent(binBadBPix , setbin);
        moduleBadBPix->SetBinError(binBadBPix , error);
        binBadBPix++;
	}
      if ((isModuleBad==0) && (isBarrelModule==2) )
        {
        moduleGoodFPix->SetBinContent(binGoodFPix , setbin);
        moduleGoodFPix->SetBinError(binGoodFPix , error);
        binGoodFPix++;
	}
      if ( (isModuleBad==1) && (isBarrelModule==2) )
        {
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
  for (int bin=1; bin<=moduleGoodBPix->GetNbinsX(); bin++)
    {
    if (moduleGoodBPix->GetBinContent(bin)>0.75)//
      {
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
  
  
    //****************** BEGIN OF 2D MAPS FOR MODULE ANALYSIS ****************

   //****************** Barrel Maps ****************
  
  TH2F* layer1 =  new TH2F("layer1","layer1",8,1,9 ,20,1,21);
  //TH2F* test =  new TH2F("test","test",8,1,9 ,20,1,21);
  TH2F* layer1valid =  new TH2F("layer1valid","layer1valid",8,1,9 ,20,1,21);
  TH2F* layer1missing =  new TH2F("layer1missing","layer1missing",8,1,9 ,20,1,21);
  TH2F* layer2 =  new TH2F("layer2","layer2",8,1,9 ,32,1,33);
  TH2F* layer2valid =  new TH2F("layer2valid","layer2valid",8,1,9 ,32,1,33);
  TH2F* layer2missing =  new TH2F("layer2missing","layer2missing",8,1,9 ,32,1,33);
  TH2F* layer3 =  new TH2F("layer3","layer3",8,1,9 ,44,1,45);
  TH2F* layer3valid =  new TH2F("layer3valid","layer3valid",8,1,9 ,44,1,45);
  TH2F* layer3missing =  new TH2F("layer3missing","layer3missing",8,1,9 ,44,1,45);

  layer1->GetXaxis()->SetTitle("module");
  layer1->GetYaxis()->SetTitle("ladder");
  layer2->GetXaxis()->SetTitle("module");
  layer2->GetYaxis()->SetTitle("ladder");
  layer3->GetXaxis()->SetTitle("module");
  layer3->GetYaxis()->SetTitle("ladder");
  layer1valid->GetXaxis()->SetTitle("module");
  layer1valid->GetYaxis()->SetTitle("ladder");
  layer2valid->GetXaxis()->SetTitle("module");
  layer2valid->GetYaxis()->SetTitle("ladder");
  layer3valid->GetXaxis()->SetTitle("module");
  layer3valid->GetYaxis()->SetTitle("ladder");
  layer1missing->GetXaxis()->SetTitle("module");
  layer1missing->GetYaxis()->SetTitle("ladder");
  layer2missing->GetXaxis()->SetTitle("module");
  layer2missing->GetYaxis()->SetTitle("ladder");
  layer3missing->GetXaxis()->SetTitle("module");
  layer3missing->GetYaxis()->SetTitle("ladder");
  
  
  int nhigheffmodule = 0;
  int nhigheffmoduletot = 0;
  entries = treeMerged->GetEntries();
  for(int n=0;n<entries;n++){
    treeMerged->GetEntry(n);
    if (isModuleBad!=0 || isBarrelModule!=1) continue;
    
    if(id<302080000){
      layer1valid->Fill(moduleInLadder,ladder,valid);
      layer1missing->Fill(moduleInLadder,ladder,missing);
      nhigheffmoduletot++;
      
      if( (valid+missing)!=0 ){
        layer1->Fill(moduleInLadder,ladder,double(valid)/double(valid+missing));
	if (double(valid)/double(valid+missing)<0.2)
          std::cout<<"cms.PSet(errortype = cms.string('whole'),detid = cms.uint32("<<id<<")),"<<std::endl;
	}
	
	if(layer1->GetBinContent(moduleInLadder,ladder)>0.99)
	  nhigheffmodule++;
    }
    if(id>302080000 && id<302160000){
      layer2valid->Fill(moduleInLadder,ladder,valid);
      layer2missing->Fill(moduleInLadder,ladder,missing);
      nhigheffmoduletot++;
      
      if( (valid+missing)!=0 ){
        layer2->Fill(moduleInLadder,ladder,double(valid)/double(valid+missing));
	if (double(valid)/double(valid+missing)<0.2)
          std::cout<<"cms.PSet(errortype = cms.string('whole'),detid = cms.uint32("<<id<<")),"<<std::endl;
	}
	
	if(layer2->GetBinContent(moduleInLadder,ladder)>0.99)
	  nhigheffmodule++;
    }
    if(id>302160000 && id<310000000){
      layer3valid->Fill(moduleInLadder,ladder,valid);
      layer3missing->Fill(moduleInLadder,ladder,missing);
      nhigheffmoduletot++;
      
      if( (valid+missing)!=0 )
        {
        layer3->Fill(moduleInLadder,ladder,double(valid)/double(valid+missing));
	if ( valid+missing == 0 ) std::cout<<"layer 3 void: module "<<moduleInLadder<<" ladder"<<ladder<<std::endl;
	if (double(valid)/double(valid+missing)<0.2)
          std::cout<<"cms.PSet(errortype = cms.string('whole'),detid = cms.uint32("<<id<<")),"<<std::endl;
	}
	
	if(layer3->GetBinContent(moduleInLadder,ladder)>0.99)
	  nhigheffmodule++;
    }
  }

  cout<<"Number of modules with good stat : "<<nhigheffmodule<<" over "<<nhigheffmoduletot<<endl;

 
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
  danekBadModuleLayer1->SetBinContent(6,4,1.);
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
  danekBadModuleLayer3->SetBinContent(6,44,1.);
  
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
      if (layer1valid->GetBinContent(binX,binY)+layer1missing->GetBinContent(binX,binY) <=10)
         std::cout<<"layer 1: module "<<binX<<"; ladder "<<binY<<std::endl;
    }
  }
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
      if (layer2valid->GetBinContent(binX,binY)+layer2missing->GetBinContent(binX,binY) <=10)
        std::cout<<"layer 2: module "<<binX<<"; ladder "<<binY<<std::endl;      
    }
  }
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
      if (layer3valid->GetBinContent(binX,binY)+layer3missing->GetBinContent(binX,binY) <=10)
        std::cout<<"layer 3: module "<<binX<<"; ladder "<<binY<<std::endl;
    }
  }

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
  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=22;lad++){
      totNum+=(int)layer1valid->GetBinContent(mod,lad);
      totDen+=(int)layer1valid->GetBinContent(mod,lad)+(int)layer1missing->GetBinContent(mod,lad);
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
  layer2publication->GetZaxis()->SetRangeUser(0.97,1.);
  layer3publication->GetZaxis()->SetRangeUser(0.93,1.);

  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=22;lad++){
      if (layer1RelativeErr->GetBinContent(mod,lad)<0.1)
        layer1publication->SetBinContent(mod,lad,layer1->GetBinContent(mod,lad));
      else layer1publication->SetBinContent(mod,lad,0.);
      }
    }

  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=32;lad++){
      if ( (layer2RelativeErr->GetBinContent(mod,lad)>0.1)||
           (mod==2&&lad==4)||(mod==8&&lad==9)||(mod==5&&lad==16)||(mod==7&&lad==21)||(mod==1&&lad==22)||(mod==8&&lad==24) )
        layer2publication->SetBinContent(mod,lad,0.);
      else layer2publication->SetBinContent(mod,lad,layer2->GetBinContent(mod,lad));
      }
    }

  for (int mod=1;mod<=8;mod++){
    for (int lad=1;lad<=44;lad++){
      if (layer3RelativeErr->GetBinContent(mod,lad)>0.1 || 
          (mod==2&&lad==8)||(mod==5&&lad==21)||(mod==8&&lad==21)||(mod==8&&lad==34)||(mod==6&&lad==44) )
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


  TH1F* cutsEfficiency = new TH1F("cutsEfficiency","cutsEfficiency",10,0,10);
  TH1F* cutsEfficiencyBPix = new TH1F("cutsEfficiencyBPix","cutsEfficiencyBPix",10,0,10);
  TH1F* cutsEfficiencyFPix = new TH1F("cutsEfficiencyFPix","cutsEfficiencyFPix",10,0,10);
  TH1F* cutsTotal = new TH1F("cutsTotal","cutsTotal",10,0,10);
  TH1F* cutsTotalBPix = new TH1F("cutsTotalBPix","cutsTotalBPix",10,0,10);
  TH1F* cutsTotalFPix = new TH1F("cutsTotalFPix","cutsTotalFPix",10,0,10);
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
  cutsEfficiency->GetYaxis()->SetTitle("#epsilon_{hit}");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(1,"No Cut");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(2,"Loose cut");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(3,"Telescope");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(5,"Edge cut");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(4,"Muon Timing");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(6,"pT cut");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(7,"Telescope + Muon ");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(8,"Telescope + Edge");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(9,"Edge + Muon");
  cutsEfficiencyBPix->GetXaxis()->SetBinLabel(10,"Analysis cut");
  cutsEfficiencyBPix->GetYaxis()->SetTitle("Efficiency [BPix]");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(1,"No Cut");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(2,"Loose cut");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(3,"Telescope");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(5,"Edge cut");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(4,"Muon Timing");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(6,"pT cut");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(7,"Telescope + Muon ");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(8,"Telescope + Edge");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(9,"Edge + Muon");
  cutsEfficiencyFPix->GetXaxis()->SetBinLabel(10,"Analysis cut");
  cutsEfficiencyFPix->GetYaxis()->SetTitle("Efficiency [FPix]");
  cutsTotal->GetXaxis()->SetBinLabel(1,"No Cut");
  cutsTotal->GetXaxis()->SetBinLabel(2,"Loose cut");
  cutsTotal->GetXaxis()->SetBinLabel(3,"Telescope");
  cutsTotal->GetXaxis()->SetBinLabel(5,"Edge cut");
  cutsTotal->GetXaxis()->SetBinLabel(4,"Muon Timing");
  cutsTotal->GetXaxis()->SetBinLabel(6,"pT cut");
  cutsTotal->GetXaxis()->SetBinLabel(7,"Telescope + Muon ");
  cutsTotal->GetXaxis()->SetBinLabel(8,"Telescope + Edge");
  cutsTotal->GetXaxis()->SetBinLabel(9,"Edge + Muon");
  cutsTotal->GetXaxis()->SetBinLabel(10,"Analysis cut");
  cutsTotal->GetYaxis()->SetTitle("number_{hit}");
  cutsTotalBPix->GetXaxis()->SetBinLabel(1,"No Cut");
  cutsTotalBPix->GetXaxis()->SetBinLabel(2,"Loose cut");
  cutsTotalBPix->GetXaxis()->SetBinLabel(3,"Telescope");
  cutsTotalBPix->GetXaxis()->SetBinLabel(5,"Edge cut");
  cutsTotalBPix->GetXaxis()->SetBinLabel(4,"Muon Timing");
  cutsTotalBPix->GetXaxis()->SetBinLabel(6,"pT cut");
  cutsTotalBPix->GetXaxis()->SetBinLabel(7,"Telescope + Muon ");
  cutsTotalBPix->GetXaxis()->SetBinLabel(8,"Telescope + Edge");
  cutsTotalBPix->GetXaxis()->SetBinLabel(9,"Edge + Muon");
  cutsTotalBPix->GetXaxis()->SetBinLabel(10,"Analysis cut");
  cutsTotalBPix->GetYaxis()->SetTitle("number_{hit} [BPix]");
  cutsTotalFPix->GetXaxis()->SetBinLabel(1,"No Cut");
  cutsTotalFPix->GetXaxis()->SetBinLabel(2,"Loose cut");
  cutsTotalFPix->GetXaxis()->SetBinLabel(3,"Telescope");
  cutsTotalFPix->GetXaxis()->SetBinLabel(5,"Edge cut");
  cutsTotalFPix->GetXaxis()->SetBinLabel(4,"Muon Timing");
  cutsTotalFPix->GetXaxis()->SetBinLabel(6,"pT cut");
  cutsTotalFPix->GetXaxis()->SetBinLabel(7,"Telescope + Muon ");
  cutsTotalFPix->GetXaxis()->SetBinLabel(8,"Telescope + Edge");
  cutsTotalFPix->GetXaxis()->SetBinLabel(9,"Edge + Muon");
  cutsTotalFPix->GetXaxis()->SetBinLabel(10,"Analysis cut");
  cutsTotalFPix->GetYaxis()->SetTitle("number_{hit} [FPix]");



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
  inactivePerRunMerged->Write();

  TCanvas* cRun = new TCanvas("cRun","cRun",1200,600); 
  cRun->cd();
  gStyle->SetStatX( 0.8);
  gStyle->SetStatY( 0.45);

  efficiencyPerRun->LabelsDeflate("X");
  efficiencyPerRun->LabelsOption("a","X");
  efficiencyPerRun->Write();
  efficiencyPerRun->Draw();
  cRun->Print("efficiencyPerRun.png","png");
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

  windowSearchMerged->Write();
  windowSearchSameModuleMerged->Write();
  windowSearchBPixMerged->Write();
  windowSearchGoodModulesBPixMerged->Write();
  windowSearchFPixMerged->Write();
  missingButClusterMerged->Write();
  missingButClusterOnSameModuleMerged->Write();
  
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
  
  
  validMuonTimeVSchargeBPixMerged->Write();
  missingMuonTimeVSchargeBPixMerged->Write();
  validMuonTimeVSchargeFPixMerged->Write();
  missingMuonTimeVSchargeFPixMerged->Write();
  

  //******* EFFICIENCY PLOTS WRITING ************
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  if(DEBUG) cout<<"********* STARTING TO WRITE/DRAW NEW PLOTS ************"<<endl;

  
  layer1->Write();
  layer1valid->Write();
  layer1missing->Write();
  layer2->Write();
  layer2valid->Write();
  layer2missing->Write();
  layer3->Write();
  layer3valid->Write();
  layer3missing->Write();

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
  layer2->Draw("colz");
  c1->Print("layer2.png","png");
  layer2missing->Draw("colz");
  c1->Print("layer2missing.png","png");
  layer2valid->Draw("colz");
  c1->Print("layer2valid.png","png");
  layer3->Draw("colz");
  c1->Print("layer3.png","png");
  layer3missing->Draw("colz");
  c1->Print("layer3missing.png","png");
  layer3valid->Draw("colz");
  c1->Print("layer3valid.png","png");

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

