#include <sys/stat.h>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TList.h"
#include <vector>
#include <iostream>
#include "TCanvas.h"

void merge(){

  using namespace std;
  bool DEBUG=false;
  int Nfiles=300;

  if(DEBUG) cout<<"********* CREATING MERGED HISTO/TTREE ************"<<endl;


  TH1F* histoMerged = new TH1F("histoMerged", "histoMerged", 3, 0, 3);

  TH1F* histLayer1Merged = new TH1F("histLayer1Merged", "histLayer1Merged", 3, 0, 3);
  TH1F* histLayer2Merged = new TH1F("histLayer2Merged", "histLayer2Merged", 3, 0, 3);
  TH1F* histLayer3Merged = new TH1F("histLayer3Merged", "histLayer3Merged", 3, 0, 3);
  TH1F* histEndcapPlusMerged = new TH1F("histEndcapPlusMerged", "histEndcapPlusMerged", 3, 0, 3);
  TH1F* histEndcapMinusMerged = new TH1F("histEndcapMinusMerged", "histEndcapMinusMerged", 3, 0, 3);
  TH1F* histBarrelMerged = new TH1F("histBarrelMerged", "histBarrelMerged", 3, 0, 3);
  TH1F* histEndcapMerged = new TH1F("histEndcapMerged", "histEndcapMerged", 3, 0, 3);
  TH1F* consistencyCheckMerged = new TH1F("consistencyCheckMerged","consistencyCheckMerged", 3, 0,3);
  TH1F* consistencyCheckTrajMerged = new TH1F("consistencyCheckTrajMerged","consistencyCheckTrajMerged", 3, 0,3);

  TH1F* validPerRunMerged = new  TH1F("validPerRunMerged","validPerRunMerged",200,0,200);
  TH1F* invalidPerRunMerged = new  TH1F("invalidPerRunMerged","invalidPerRunMerged",200,0,200);
  TH1F* inactivePerRunMerged = new  TH1F("inactivePerRunMerged","inactivePerRunMerged",200,0,200);
  TH1F* efficiencyPerRun = new  TH1F("efficiencyPerRun","efficiencyPerRun",200,0,200);
  TH1F* inactivePercentPerRun = new  TH1F("inactivePercentPerRun","inactivePercentPerRun",200,0,200);

  TH1F* validVsAlphaMerged = new TH1F("validVsAlphaMerged","validVsAlphaMerged",200,-3.5,3.5);
  TH1F* validVsAlphaBPixMerged = new TH1F("validVsAlphaBPixMerged","validVsAlphaMerged",200,-3.5,3.5);
  TH1F* validVsAlphaFPixMerged = new TH1F("validVsAlphaFPixMerged","validVsAlphaFPixMerged",200,-3.5,3.5);
  TH1F* missingVsAlphaMerged = new TH1F("missingVsAlphaMerged","missingVsAlphaMerged",200,-3.5,3.5);
  TH1F* missingVsAlphaBPixMerged = new TH1F("missingVsAlphaBPixMerged","missingVsAlphaBPixMerged",200,-3.5,3.5);
  TH1F* missingVsAlphaFPixMerged = new TH1F("missingVsAlphaFPixMerged","missingVsAlphaFPixMerged",200,-3.5,3.5);
  TH1F* validVsBetaMerged = new TH1F("validVsBetaMerged","validVsBetaMerged",200,-3.5,3.5);
  TH1F* validVsBetaBPixMerged = new TH1F("validVsBetaBPixMerged","validVsBetaBPixMerged",200,-3.5,3.5);
  TH1F* validVsBetaFPixMerged = new TH1F("validVsBetaFPixMerged","validVsBetaFPixMerged",200,-3.5,3.5);
  TH1F* missingVsBetaMerged = new TH1F("missingVsBetaMerged","missingVsBetaMerged",200,-3.5,3.5);
  TH1F* missingVsBetaBPixMerged = new TH1F("missingVsBetaBPixMerged","missingVsBetaBPixMerged",200,-3.5,3.5);
  TH1F* missingVsBetaFPixMerged = new TH1F("missingVsBetaFPixMerged","missingVsBetaFPixMerged",200,-3.5,3.5);

  TH1F* validVsLocalXMerged = new TH1F("validVsLocalXMerged","validVsLocalXMerged",100,-1.5,1.5);
  TH1F* validVsLocalYMerged = new TH1F("validVsLocalYMerged","validVsLocalYMerged",100,-4.,4.);
  TH1F* missingVsLocalXMerged = new TH1F("missingVsLocalXMerged","missingVsLocalXMerged",100,-1.5,1.5);
  TH1F* missingVsLocalYMerged = new TH1F("missingVsLocalYMerged","missingVsLocalYMerged",100,-4.,4.);

  double maxwindowsearch = 0.5;
  TH1F*  windowSearchMerged = new  TH1F("windowSearchMerged","windowSearchMerged",maxwindowsearch*1000,0,maxwindowsearch);
  TH1F*  windowSearchSameModuleMerged = new  TH1F("windowSearchSameModuleMerged","windowSearchSameModuleMerged",maxwindowsearch*1000,0,maxwindowsearch);
  TH1F*  windowSearchBPixMerged = new  TH1F("windowSearchBPixMerged","windowSearchBPixMerged",maxwindowsearch*1000,0,maxwindowsearch);
  TH1F*  windowSearchFPixMerged = new  TH1F("windowSearchFPixMerged","windowSearchFPixMerged",maxwindowsearch*1000,0,maxwindowsearch);
    
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


  char name[120];
  for(int i=1;i<=Nfiles;i++)
    {
    sprintf(name,"pixelEfficiency%d.root",i);
        
    cout<<name<<endl;
  
    TFile* myFile = TFile::Open(name);
    if(myFile==0) continue;
    
    //TList list = 
    if((myFile->GetListOfKeys())->First()==0) {cout<< "IS VOID" << endl; continue;}
    
    myFile->cd();
    
    if(DEBUG) cout<<"********* FILE OPENED, STARTING TO MERGE ************"<<endl;
    
    for (int bin=1; bin<=3; bin++)
      {
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
      }
    for (int bin=1; bin<=4; bin++)
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
      }      
    
    
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
//////////////////////////////////////////////delete delete delete


    if(DEBUG) cout<<"********* NOW MERGING ANALYSIS FOR TRAJECTORY ************"<<endl;


    TH1F *htest = (TH1F*)gDirectory->Get( "checkoutTraj" );
    for (int bin=1; bin<=10; bin++)
      checkoutTrajMerged->SetBinContent( bin, checkoutTrajMerged->GetBinContent(bin) + htest->GetBinContent(bin));  
    delete htest;
    
//update to alpha and beta
    TH1F *validVsAlpha = (TH1F*)gDirectory->Get( "validVsAlpha" );
    TH1F *validVsAlphaBPix = (TH1F*)gDirectory->Get( "validVsAlphaBPix" );
    TH1F *validVsAlphaFPix = (TH1F*)gDirectory->Get( "validVsAlphaFPix" );
    TH1F *missingVsAlpha = (TH1F*)gDirectory->Get( "missingVsAlpha" );
    TH1F *missingVsAlphaBPix = (TH1F*)gDirectory->Get( "missingVsAlphaBPix" );
    TH1F *missingVsAlphaFPix = (TH1F*)gDirectory->Get( "missingVsAlphaFPix" );
    TH1F *validVsBeta = (TH1F*)gDirectory->Get( "validVsBeta" );
    TH1F *validVsBetaBPix = (TH1F*)gDirectory->Get( "validVsBetaBPix" );
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


/*
 For all histogram types: nbins, xlow, xup
 bin = 0;       underflow bin
 bin = 1;       first bin with low-edge xlow INCLUDED
 bin = nbins;   last bin with upper-edge xup EXCLUDED
 bin = nbins+1; overflow bin
*/


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
    
    TH1F *windowSearchFPix = (TH1F*)gDirectory->Get( "windowSearchFPix" );
    for (int bin=1; bin<=windowSearchFPixMerged->GetNbinsX()+1; bin++)
      windowSearchFPixMerged->SetBinContent( bin, windowSearchFPixMerged->GetBinContent(bin) + windowSearchFPix->GetBinContent(bin));
    
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
      for(int j=0;j<tempTree.size();j++) if(id==tempTree[j][0]) {PlaceInVector = j;break;}
      
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
  for(int i=0;i<tempTree.size();i++){
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
  
  
  
  
  
  
  TH1F* histSubdetectors = new TH1F("histSubdetectors", "histSubdetectors", 5, 0, 5);
  TH1F* histSummary = new TH1F("histSummary", "histSummary", 3, 0, 3);
  

  a=histLayer1Merged->GetBinContent(3); b=histLayer1Merged->GetBinContent(2);
  histSubdetectors->SetBinContent( 1, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(1,error);
  histSubdetectors->GetXaxis()->SetBinLabel(1," BPix Layer 1");

  a=histLayer2Merged->GetBinContent(3); b=histLayer2Merged->GetBinContent(2);
  histSubdetectors->SetBinContent( 2, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(2,error);
  histSubdetectors->GetXaxis()->SetBinLabel(2," BPix Layer 2");
  
  a=histLayer3Merged->GetBinContent(3); b=histLayer3Merged->GetBinContent(2);
  histSubdetectors->SetBinContent( 3, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(3,error);
  histSubdetectors->GetXaxis()->SetBinLabel(3," BPix Layer 3");
  
  a=histEndcapPlusMerged->GetBinContent(3); b=histEndcapPlusMerged->GetBinContent(2);
  histSubdetectors->SetBinContent( 4, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSubdetectors->SetBinError(4,error);
  histSubdetectors->GetXaxis()->SetBinLabel(4," FPix Plus");
  
  a=histEndcapMinusMerged->GetBinContent(3); b=histEndcapMinusMerged->GetBinContent(2);
  histSubdetectors->SetBinContent( 5, a/(a+b) );
  error=0; if((a+b)!=0) error=sqrt( ((a)/(a+b))*(1-((a)/(a+b)))/(a+b) );
  histSubdetectors->SetBinError(5,error);
  histSubdetectors->GetXaxis()->SetBinLabel(5," FPix Minus");
     
  a=histBarrelMerged->GetBinContent(3); b=histBarrelMerged->GetBinContent(2);
  histSummary->SetBinContent( 1, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSummary->SetBinError(1,error);
  histSummary->GetXaxis()->SetBinLabel(1,"BPix");

  a=histEndcapMerged->GetBinContent(3); b=histEndcapMerged->GetBinContent(2);
  histSummary->SetBinContent( 2, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSummary->SetBinError(2,error);
  histSummary->GetXaxis()->SetBinLabel(2,"FPix");
  
  a=histoMerged->GetBinContent(3); b=histoMerged->GetBinContent(2);
  histSummary->SetBinContent( 3, a/(a+b) );
  error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
  histSummary->SetBinError(3,error);
  histSummary->GetXaxis()->SetBinLabel(3,"total");

  
  TH1F* histAlphaAnalysis = new TH1F("histAlphaAnalysis", "hist", 200, -3.5,3.5); 
  for (int bin=1;bin<=200;bin++)
    {
    double val=validVsAlphaMerged->GetBinContent(bin);
    double mis=missingVsAlphaMerged->GetBinContent(bin);
    if ((val+mis)!=0)
      {
      histAlphaAnalysis->SetBinContent(bin,val/(val+mis));
      error=0;
      error=sqrt(((val)/(val+mis))*(1-((val)/(val+mis)))/(val+mis));
      histAlphaAnalysis->SetBinError(bin,error);
      }
    }
    
  TH1F* histBetaAnalysis = new TH1F("histBetaAnalysis", "hist", 200, -3.5,3.5); 
  for (int bin=1;bin<=200;bin++)
    {
    double val=validVsBetaMerged->GetBinContent(bin);
    double mis=missingVsBetaMerged->GetBinContent(bin);
    if ((val+mis)!=0)
      {
      histBetaAnalysis->SetBinContent(bin,val/(val+mis));
      error=0;
      error=sqrt(((val)/(val+mis))*(1-((val)/(val+mis)))/(val+mis));
      histBetaAnalysis->SetBinError(bin,error);
      }
    }

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
    for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
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
    TH1F* scurveSameModule = new TH1F("scurveSameModule", "scurveSameModule", windowSearchMerged->GetNbinsX(), 0, windowSearchMerged->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX()));
    for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
      totalHit+=windowSearchMerged->GetBinContent(bin);
   
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
    TH1F* scurveBPix = new TH1F("scurveBPix", "scurveBPix", windowSearchMerged->GetNbinsX(), 0, windowSearchMerged->GetXaxis()->GetBinUpEdge(windowSearchMerged->GetNbinsX()));
    for (int bin=1;bin<=windowSearchMerged->GetNbinsX()+1; bin++) //201 includes overflow=definitevely missing
      totalHit+=windowSearchBPixMerged->GetBinContent(bin);
   
    for (int bin=1; bin<=scurveBPix->GetNbinsX(); bin++){
      validHit+=windowSearchBPixMerged->GetBinContent(bin);
      scurveBPix->SetBinContent(bin, validHit/totalHit);
    }
    
    scurveBPix->GetXaxis()->SetTitle("window search [cm]");
    scurveBPix->GetYaxis()->SetTitle("eff [BPIX]");
    TF1 *fitSCBPix = new TF1("fitSCBPix","[0]", down, up);
    fitSCBPix->SetParName(0,"asympt_eff");
    fitSCBPix->SetParameter(0, 0.7);
    scurveBPix->Fit("fitSCBPix","R");

    a=validHit ; b=totalHit-validHit;
    histBarrelEfficiencyComparison->SetBinContent(4, a/(a+b) );
    error=0;if((a+b)!=0) error=sqrt(((a)/(a+b))*(1-((a)/(a+b)))/(a+b));
    histBarrelEfficiencyComparison->SetBinError(4,error);
    histBarrelEfficiencyComparison->GetXaxis()->SetBinLabel(4,"Method 2 s-curve");
  
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
  TH2F* layer2 =  new TH2F("layer2","layer2",8,1,9 ,32,1,33);
  TH2F* layer3 =  new TH2F("layer3","layer3",8,1,9 ,44,1,45);

  layer1->GetXaxis()->SetTitle("module");
  layer1->GetYaxis()->SetTitle("ladder");
  layer2->GetXaxis()->SetTitle("module");
  layer2->GetYaxis()->SetTitle("ladder");
  layer3->GetXaxis()->SetTitle("module");
  layer3->GetYaxis()->SetTitle("ladder");
    
  vector< vector<double> > tempTreeLayer1;
  entries = treeMerged->GetEntries();
  for(int n=0;n<entries;n++){
    treeMerged->GetEntry(n);
    if (isModuleBad==0    &&
        isBarrelModule==1 &&
	id<302080000       ) 
      {
      vector<double> aux;
      aux.push_back(ladder);aux.push_back(moduleInLadder);
      if ((((double)valid)+((double)missing))!=0.) aux.push_back( ((double)valid)/(((double)valid)+((double)missing)) );
      else {aux.push_back(-1.);std::cout<<"e' successo"<<std::endl;}
      tempTreeLayer1.push_back( aux );
      }
    }

  vector< vector<double> > tempTreeLayer2;
  for(int n=0;n<entries;n++){
    treeMerged->GetEntry(n);
    if (isModuleBad==0    &&
        isBarrelModule==1 &&
	id>302080000 && id<302160000 ) 
      {
      vector<double> aux;
      aux.push_back(ladder);aux.push_back(moduleInLadder);
      if ((((double)valid)+((double)missing))!=0.) aux.push_back( ((double)valid)/(((double)valid)+((double)missing)) );
      else {aux.push_back(-1.);std::cout<<"e' successo"<<std::endl;}
      tempTreeLayer2.push_back( aux );
      }
    }
  vector< vector<double> > tempTreeLayer3;
  for(int n=0;n<entries;n++){
    treeMerged->GetEntry(n);
    if (isModuleBad==0    &&
        isBarrelModule==1 &&
	id>302160000 && id<310000000 ) //test sul layer1
      {
      vector<double> aux;
      aux.push_back(ladder);aux.push_back(moduleInLadder);
      if ((((double)valid)+((double)missing))!=0.) aux.push_back( ((double)valid)/(((double)valid)+((double)missing)) );
      else {aux.push_back(-1.);std::cout<<"e' successo"<<std::endl;}
      tempTreeLayer3.push_back( aux );
      }
    }

  for (int iter=0; iter<tempTreeLayer1.size(); iter++)
    {
    layer1->Fill(tempTreeLayer1[iter][1],tempTreeLayer1[iter][0],tempTreeLayer1[iter][2]);
    }
  for (int iter=0; iter<tempTreeLayer2.size(); iter++)
    {
    layer2->Fill(tempTreeLayer2[iter][1],tempTreeLayer2[iter][0],tempTreeLayer2[iter][2]);
    }
  for (int iter=0; iter<tempTreeLayer3.size(); iter++)
    {
    layer3->Fill(tempTreeLayer3[iter][1],tempTreeLayer3[iter][0],tempTreeLayer3[iter][2]);
    }
/*
//force holes from dead modules (badModuleList) for "nice show"
    layer2->Fill(1,22,-0.5);
    layer2->Fill(8,24,-0.5);
    layer2->Fill(8,9,-0.5);
    layer3->Fill(6,44,-0.5);
    layer3->Fill(8,34,-0.5);
    layer3->Fill(2,8,-0.5);
*/
  
if (DEBUG)
  {
  std::cout<<"Layer1"<<std::endl;         
  for (int ladderIter=1; ladderIter<=32; ladderIter++)
    {
    int moduleIter=1;
    for (int dim=0;dim<tempTreeLayer1.size();dim++)
      {
      if (tempTreeLayer1[dim][0]==ladderIter)
        {
	moduleIter++;
        }
      }
    std::cout<<"we have "<<moduleIter-1<<" module for ladder"<<ladderIter<<std::endl;
    }

  std::cout<<"Layer2"<<std::endl;         
  for (int ladderIter=1; ladderIter<=32; ladderIter++)
    {
    int moduleIter=1;
    for (int dim=0;dim<tempTreeLayer2.size();dim++)
      {
      if (tempTreeLayer2[dim][0]==ladderIter)
        {
	moduleIter++;
        }
      }
    std::cout<<"we have "<<moduleIter-1<<" module for ladder"<<ladderIter<<std::endl;
    }

  std::cout<<"Layer3"<<std::endl;         
  for (int ladderIter=1; ladderIter<=44; ladderIter++)
    {
    int moduleIter=1;
    for (int dim=0;dim<tempTreeLayer3.size();dim++)
      {
      if (tempTreeLayer3[dim][0]==ladderIter)
        {
	moduleIter++;
        }
      }
    std::cout<<"we have "<<moduleIter-1<<" module for ladder"<<ladderIter<<std::endl;
    }
    
  }//end if debug module entries
   //****************** Disk Maps ****************

    
    //****************** END   OF 2D MAPS FOR MODULE ANALYSIS ****************    

  TF1 *runfit = new TF1("runfit","[0]", 0, efficiencyPerRun->GetNbinsX());
  runfit->SetParName(0,"mean_eff");
  runfit->SetParameter(0, 1);
  efficiencyPerRun->Fit("runfit");



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
  consistencyCheckMerged->Write();
  consistencyCheckTrajMerged->Write();
  histInvalidRecHitCollectionMerged->Write();
  histInvalidRecHitWithBadmoduleListMerged->Write();
  histoMethod2Merged->Write();
  histoMethod2AfterMerged->Write();

  invalidPerRunMerged->Write(); 
  validPerRunMerged->Write();
  inactivePerRunMerged->Write();

  TCanvas* cRun = new TCanvas("cRun","cRun",1200,600); 
  cRun->cd();
  efficiencyPerRun->LabelsDeflate("X");
  efficiencyPerRun->LabelsOption("a","X");
  efficiencyPerRun->Write();
  efficiencyPerRun->Draw();
  cRun->Print("efficiencyPerRun.gif","gif");
  cRun->Close();


  inactivePercentPerRun->LabelsDeflate("X");
  inactivePercentPerRun->Write();


  validVsAlphaMerged->Write();
  validVsAlphaBPixMerged->Write();
  validVsAlphaFPixMerged->Write();
  missingVsAlphaMerged->Write();
  missingVsAlphaBPixMerged->Write();
  missingVsAlphaFPixMerged->Write(); 
  validVsBetaMerged->Write(); 
  validVsBetaBPixMerged->Write(); 
  validVsBetaFPixMerged->Write();
  missingVsBetaMerged->Write();
  missingVsBetaBPixMerged->Write();
  missingVsBetaFPixMerged->Write();
  
  missingVsLocalXMerged->Write();
  missingVsLocalYMerged->Write();
  validVsLocalXMerged->Write();
  validVsLocalYMerged->Write();  

  windowSearchMerged->Write();
  windowSearchBPixMerged->Write();
  windowSearchFPixMerged->Write();
  
  checkoutTrajMerged->Write();
  checkoutValidityFlagMerged->Write();   
  
  missingPerTrackMerged->Write();
  inactivePerTrackMerged->Write();
  
  treeMerged->Write();


  //******* EFFICIENCY PLOTS WRITING ************
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
 
  localXAnalysis->Write();
  localXAnalysis->Draw();
  c1->Print("localXAnalysis.gif","gif");
  localYAnalysis->Write();
  localYAnalysis->Draw();
  c1->Print("localYAnalysis.gif","gif");
  
  histBarrelEfficiencyComparison->Write();
  histSubdetectors->Write();
  histSubdetectors->Draw();
  c1->Print("histSubdetectors.gif","gif");
  histSubdetectors->GetYaxis()->SetRangeUser(0., 1.);
  histSubdetectors->Draw();
  c1->Print("histSubdetectorsFullRange.gif","gif");
    
  histSummary->Write();
  histAlphaAnalysis->GetXaxis()->SetTitle("#alpha_{local}");  
  histAlphaAnalysis->Write();
  histAlphaAnalysis->Draw();
  c1->Print("histAlphaAnalysis.gif","gif");
  
  histBetaAnalysis->GetXaxis()->SetTitle("#beta_{local}");  
  histBetaAnalysis->Write();
  histBetaAnalysis->Draw();
  c1->Print("histBetaAnalysis.gif","gif");

  //moduleBreakoutEff->LabelsDeflate("X");
  moduleBreakoutEff->Write();
  
  moduleGoodBPix->GetYaxis()->SetTitle("eff_{goodBPix}"); 
  moduleGoodBPix->GetXaxis()->SetTitle("detid_{list}"); 
  moduleGoodBPix->Write(); 
  qualityBPixModule->Write();
  qualityBPixModule->Draw();
  c1->Print("qualityBPixModule.gif","gif");

  moduleGoodFPix->GetYaxis()->SetTitle("eff_{goodFPix}"); 
  moduleGoodFPix->GetXaxis()->SetTitle("detid_{list}"); 
  moduleGoodFPix->Write(); 
  moduleGoodFPix->Draw();
  c1->Print("moduleFPix.gif","gif");

  moduleBadBPix->Write(); 
  moduleBadFPix->Write(); 
  
  scurve->Write();
  scurve->Draw();
  c1->Print("scurve.gif","gif");
  scurveSameModule->Write();
  scurveSameModule->Draw();
  c1->Print("scurveSameModule.gif","gif");
  scurveBPix->Write();
  scurveBPix->Draw();
  c1->Print("scurveBPix.gif","gif");
  scurveFPix->Write();
  scurveFPix->Draw();
  c1->Print("scurveFPix.gif","gif");  

  layer1->Write();
  layer2->Write();
  layer3->Write();
  layer1->Draw("colz");
  c1->Print("layer1.gif","gif");
  layer2->Draw("colz");
  c1->Print("layer2.gif","gif");
  layer3->Draw("colz");
  c1->Print("layer3.gif","gif");

  fOutputFile->Close() ;
    
}
