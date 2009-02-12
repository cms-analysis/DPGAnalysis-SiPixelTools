#include <sys/stat.h>
#include "TH1.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include <vector>
#include <iostream>
void merge(){

  using namespace std;
  bool DEBUG=false;

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
  TH1F* invalidVsAlphaMerged = new TH1F("invalidVsAlphaMerged","invalidVsAlphaMerged",200,-3.5,3.5);
  TH1F* validVsBetaMerged = new TH1F("validVsBetaMerged","validVsBetaMerged",200,-3.5,3.5);
  TH1F* invalidVsBetaMerged = new TH1F("invalidVsBetaMerged","invalidVsBetaMerged",200,-3.5,3.5);

  TH1F*  windowSearchMerged = new  TH1F("windowSearchMerged","windowSearchMerged",100,0,0.10);

  //invalidity version
  TH1F* histInvalidRecHitCollectionMerged = new TH1F("histInvalidRecHitCollectionMerged","histInvalidRecHitCollectionMerged", 4, 0,4);
  TH1F* histInvalidRecHitWithBadmoduleListMerged = new TH1F("histInvalidRecHitWithBadmoduleListMerged","histInvalidRecHitWithBadmoduleListMerged",5,0,5);
  
  //test
  TH1F* checkoutTrajMerged = new TH1F("checkoutTrajMerged","checkoutTrajMerged",10,0,10); 
  TH1F* checkoutValidityFlagMerged = new TH1F("checkoutValidityFlagMerged","checkoutValidityFlagMerged",4,0,4); 

  //TH1F* totalPerTrackMerged= new TH1F ("totalPerTrackMerged","totalPerTrackMerged",20,0,20);
  TH1F* inactivePerTrackMerged= new TH1F ("inactivePerTrackMerged","inactivePerTrackMerged",101,0,101);
  TH1F* missingPerTrackMerged= new TH1F ("missingPerTrackMerged","missingPerTrackMerged",101,0,101);


  vector< vector<int> > tempTree;
  int id, isModuleBad, inactive, missing, valid, isBarrelModule;
  TTree *treeMerged = new TTree("moduleAnalysisMerged","moduleAnalysisMerged");
  treeMerged->Branch("id",&id,"id/I");
  treeMerged->Branch("isModuleBad",&isModuleBad,"isModuleBad/I");
  treeMerged->Branch("inactive",&inactive,"inactive/I");
  treeMerged->Branch("missing",&missing,"missing/I");
  treeMerged->Branch("valid",&valid,"valid/I");
  treeMerged->Branch("isBarrelModule",&isBarrelModule,"isBarrelModule/I");

  int Nfiles=170;

  char name[120];
  for(int i=1;i<=Nfiles;i++)
    {
    sprintf(name,"pixelEfficiency%d.root",i);
        
    cout<<name<<endl;
  
    TFile* myFile = TFile::Open(name);
    if(myFile==0) continue;
    myFile->cd();
    
    if(DEBUG) cout<<"File opened"<<endl;
    
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

    for (int bin=1; bin<=10; bin++)
      {
      TH1F *htest = (TH1F*)gDirectory->Get( "checkoutTraj" );
      checkoutTrajMerged->SetBinContent( bin, checkoutTrajMerged->GetBinContent(bin) + htest->GetBinContent(bin));
      
      delete htest;
      }
//update to alpha and beta
    for (int bin=1; bin<=200; bin++)
      {
      TH1F *hx = (TH1F*)gDirectory->Get( "validVsAlpha" );
      validVsAlphaMerged->SetBinContent( bin, validVsAlphaMerged->GetBinContent(bin) + hx->GetBinContent(bin));
      TH1F *hy = (TH1F*)gDirectory->Get( "invalidVsAlpha" );
      invalidVsAlphaMerged->SetBinContent( bin, invalidVsAlphaMerged->GetBinContent(bin) + hy->GetBinContent(bin));
      TH1F *hw = (TH1F*)gDirectory->Get( "validVsBeta" );
      validVsBetaMerged->SetBinContent( bin, validVsBetaMerged->GetBinContent(bin) + hw->GetBinContent(bin));
      TH1F *hz = (TH1F*)gDirectory->Get( "invalidVsBeta" );
      invalidVsBetaMerged->SetBinContent( bin, invalidVsBetaMerged->GetBinContent(bin) + hz->GetBinContent(bin));
      
      delete hx;
      delete hy;
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
    for (int bin=0; bin<=101; bin++)
      {
      TH1F *hw = (TH1F*)gDirectory->Get( "windowSearch" );
      windowSearchMerged->SetBinContent( bin,
      windowSearchMerged->GetBinContent(bin) + hw->GetBinContent(bin));

      delete hw;
      }

//
    
    if(DEBUG) cout<<"Histo merged"<<endl;
    
    TTree *tree = (TTree*)gDirectory->Get("moduleAnalysis");
    tree->SetBranchAddress("id",&id);
    tree->SetBranchAddress("isModuleBad",&isModuleBad);
    tree->SetBranchAddress("inactive",&inactive);
    tree->SetBranchAddress("missing",&missing);
    tree->SetBranchAddress("valid",&valid);
    tree->SetBranchAddress("isBarrelModule",&isBarrelModule);
    
    if(DEBUG) cout<<"TTree opened"<<endl;
    if(tree==0) cout<<"Problem opening TTree"<<endl;
    
    int entries = tree->GetEntries();
        if(DEBUG) cout<<entries<<endl;

    for(int n=0;n<entries;n++){
      tree->GetEntry(n);  //get the tree entries in the proper (already addressed) variables 
          
      int PlaceInVector=-1;
      for(int j=0;j<tempTree.size();j++) if(id==tempTree[j][0]) {PlaceInVector = j;break;}
      
      if(PlaceInVector==-1){
        vector<int> temp;
	temp.push_back(id);
	temp.push_back(isModuleBad);
	temp.push_back(inactive);
	temp.push_back(missing);
	temp.push_back(valid);
	temp.push_back(isBarrelModule);
	tempTree.push_back(temp);	
      }
      else {
        tempTree[PlaceInVector][2]+=inactive;
        tempTree[PlaceInVector][3]+=missing;
        tempTree[PlaceInVector][4]+=valid;
      } 
    }
    
    if(DEBUG) cout<<"TTree merged"<<endl;
    
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
     else 
       {
       efficiencyPerRun->SetBinContent(i,0);
       efficiencyPerRun->GetXaxis()->SetBinLabel( i,"deleted" );
       }
     inactivePercentPerRun->SetBinContent(i,inactivePerRunMerged->GetBinContent(i)/(invalidPerRunMerged->GetBinContent(i)+validPerRunMerged->GetBinContent(i)) );
     inactivePercentPerRun->GetXaxis()->SetBinLabel( i,validPerRunMerged->GetXaxis()->GetBinLabel(i) );
       
     } 
    
  //Filling TTree merged
  for(int i=0;i<tempTree.size();i++){
    id=tempTree[i][0];
    isModuleBad=tempTree[i][1];
    inactive=tempTree[i][2];
    missing=tempTree[i][3];
    valid=tempTree[i][4];
    isBarrelModule=tempTree[i][5];
    treeMerged->Fill();
  }


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

  invalidPerRunMerged->Write(); 
  validPerRunMerged->Write();
  inactivePerRunMerged->Write();

  efficiencyPerRun->LabelsDeflate("X");
  efficiencyPerRun->Write();
  inactivePercentPerRun->LabelsDeflate("X");
  inactivePercentPerRun->Write();

  validVsAlphaMerged->Write();
  invalidVsAlphaMerged->Write(); 
  validVsBetaMerged->Write();
  invalidVsBetaMerged->Write(); 

  windowSearchMerged->Write();
  
  checkoutTrajMerged->Write();
  checkoutValidityFlagMerged->Write();   
  
  missingPerTrackMerged->Write();
  inactivePerTrackMerged->Write();
  
  treeMerged->Write();

  fOutputFile->Close() ;
    
}
