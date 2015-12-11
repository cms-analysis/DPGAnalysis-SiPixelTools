#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TH1F.h"
#include "TString.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TSystem.h"

using namespace std;

#include "files_funcs.C"



bool PrintImages = true;
bool PrintAllFormats = false;

void makeEvolHisto(vector<TFile*>& , vector<TString>& , TString&);
void write(TH1F* , Bool_t = 0 , Bool_t = 0 , TString = "rmou"); // logY , largePad , optstatoption
void writeBarrel(TH1F* , TH1F* , TH1F* , TString name);
void writeEndCap(TH1F* , TH1F* , TH1F* , TH1F* , TString name);
void addRun(TString , vector<TString>& , vector<TString>& );


vector<TString> subdet;

void make_EvolutionPlots(){

  subdet.push_back("l1");subdet.push_back("l2");subdet.push_back("l3");
  subdet.push_back("d-2");subdet.push_back("d-1");subdet.push_back("d+1");subdet.push_back("d+2");

  vector<TString> sfiles;
  vector<TString> sruns;

  sfiles.push_back("../Run_126418/Summary_Run126418/Comp_Run126418.root");
  sruns.push_back("126418");
  sfiles.push_back("../Run_157650/Summary_Run157650/Comp_Run157650.root");
  sruns.push_back("157650");
  sfiles.push_back("../Run_164648/Summary_Run164648/Comp_Run164648.root");
  sruns.push_back("164648");
  sfiles.push_back("../Run_172233/Summary_Run172233/Comp_Run172233.root");
  sruns.push_back("172233");
  sfiles.push_back("../Run_173484/Summary_Run173484/Comp_Run173484.root");
  sruns.push_back("173484");
  sfiles.push_back("../Run_180770/Summary_Run180770/Comp_Run180770.root");
  sruns.push_back("180770");
  sfiles.push_back("../Run_182716/Summary_Run182716/Comp_Run182716.root");
  sruns.push_back("182716");
  
  addRun( "184644" , sfiles , sruns );
  addRun( "185458" , sfiles , sruns );
  
  vector<TString> histos;
  histos.push_back("SUB_Gain_whole");
  histos.push_back("SUB_Pedestal_whole");
  histos.push_back("SUB_GainHighPoint_whole");
  histos.push_back("SUB_GainLowPoint_whole");
  histos.push_back("SUB_GainNPoints_whole");
  histos.push_back("SUB_frac_gain_high_whole");
  
  vector<TFile*> files;
  for(unsigned f = 0 ; f < sfiles.size() ; ++f)
    files.push_back(getFile(sfiles[f]));
    
    
  TFile* output = new TFile("evolutionPlots.root","RECREATE");
  output->cd();
  
  for(vector<TString>::iterator h = histos.begin() ; h != histos.end() ; ++h)
    makeEvolHisto(files , sruns , *h);
    
  output->Close();
  
  
  gSystem->Exec("mkdir -p figs/");
  gSystem->Exec("mv *.png figs/.");
  
  cout << endl << "  To retrieve the figurse, please do:" << endl;
  cout << "scp -r rougny@lxplus.cern.ch:/afs/cern.ch/user/r/rougny/gain/CMSSW_3_11_2/src/DQM/SiPixelCommon/test/scripts/figs ." << endl << endl;
  
}


void makeEvolHisto(vector<TFile*>& files , vector<TString>& sruns , TString& shisto){
  TDirectory* cDir = gDirectory;
  
  TString name = shisto;
  name.Remove(0 , name.First("_")+1);
  name.Remove(name.First("_") , name.Length() - name.First("_"));

  vector<TH1F> evol(subdet.size() , TH1F());
  for(unsigned s = 0 ; s < subdet.size() ; ++s)  
    evol[s] = TH1F("evol_"+name+"_"+subdet[s] , "evol_"+name+"_"+subdet[s] , files.size() , 0 , files.size());

  for(unsigned i = 0 ; i < files.size() ; ++i){
    TH1F* h = getHist<TH1F>( *(files[i]) , shisto , 0);
    
    for(unsigned s = 0 ; s < subdet.size() ; ++s){
      //cout << evol[s].GetName() << "  " << evol[s].GetNbinsX() << endl;
      evol[s].SetBinContent(i+1 , h->GetBinContent(s+1));
      evol[s].SetBinError(i+1 , h->GetBinError(s+1));
      evol[s].GetXaxis()->SetBinLabel(i+1 , sruns[i]);    
    }
    //delete h;
  }
  
  cDir->cd();
  
  for(unsigned s = 0 ; s < subdet.size() ; ++s){
    write( &(evol[s]) );
  }
  
  writeBarrel( &(evol[0]) , &(evol[1]) , &(evol[2]) , name );
  writeEndCap( &(evol[3]) , &(evol[4]) , &(evol[5]) , &(evol[6]) , name );
  
  //delete cDir;
}




//write TH1F
void write(TH1F* histo , Bool_t logY , Bool_t largePad , TString statOptions){
  TString t=histo->GetName();
  t=t;
  int canv_y = 500 ;
  int canv_x = canv_y;
  if(largePad) canv_x = 2 * canv_y;
  TCanvas* c1 = new TCanvas("c1" , "c1" , canv_x , canv_y);
  c1->cd();

  if(logY) gPad->SetLogy(1);
  gStyle->SetOptStat(statOptions);

  histo->Draw();
  histo->Write();
  if(PrintImages){
    c1->Print(t+".png","png");
    if(PrintAllFormats){
      c1->Print(t+".eps","eps");
      c1->Print(t+".root","root");
    }
  }

  delete c1;
}

//Write pixel, column and ROC level on same canvas
void writeBarrel(TH1F* l1 , TH1F* l2 , TH1F* l3 , TString name){

  l1->SetMaximum(1.15 * l1->GetMaximum());
  l1->SetMinimum(0.9 * l1->GetMinimum());
  l2->SetMarkerColor(kBlue);
  l3->SetMarkerColor(kRed);

  l1->GetYaxis()->SetTitle("mean " + name);

  TLegend* leg = new TLegend(0.78,0.78,0.98,0.98);
  leg->AddEntry(l1,"Layer 1","p");
  leg->AddEntry(l2,"Layer 2","p");
  leg->AddEntry(l3,"Layer 3","p");
  leg->SetFillColor(10);

  gStyle->SetOptStat("");
  TCanvas* canv = new TCanvas(name,name);
  canv->cd();

  gStyle->SetOptStat("");
  l1->Draw("hist p");
  l2->Draw("hist p same");
  l3->Draw("hist p same");
  leg->Draw("same");

  canv->Write();
  if(PrintImages){
    canv->Print("barrel_"+name+".png","png");
    if(PrintAllFormats){
      canv->Print("barrel_"+name+".eps","eps");
      canv->Print("barrel_"+name+".root","root");
    }
  }
}


//Write pixel, column and ROC level on same canvas
void writeEndCap(TH1F* dm2 , TH1F* dm1 , TH1F* dp1 , TH1F* dp2 , TString name){

  dm2->SetMaximum(1.15 * dm2->GetMaximum());
  dm2->SetMinimum(0.9 * dm2->GetMinimum());
  
  dm1->SetMarkerColor(kBlue);
  dp1->SetMarkerColor(kRed);
  dp2->SetMarkerColor(kMagenta);

  dm2->GetYaxis()->SetTitle("mean " + name);

  TLegend* leg = new TLegend(0.78,0.78,0.98,0.98);
  leg->AddEntry(dm2,"Disk -2","p");
  leg->AddEntry(dm1,"Disk -1","p");
  leg->AddEntry(dp1,"Disk +1","p");
  leg->AddEntry(dp2,"Disk +2","p");
  leg->SetFillColor(10);

  gStyle->SetOptStat("");
  TCanvas* canv = new TCanvas(name,name);
  canv->cd();

  gStyle->SetOptStat("");
  dm2->Draw("hist p");
  dm1->Draw("hist p same");
  dp1->Draw("hist p same");
  dp2->Draw("hist p same");
  leg->Draw("same");

  canv->Write();
  if(PrintImages){
    canv->Print("endcap_"+name+".png","png");
    if(PrintAllFormats){
      canv->Print("endcap_"+name+".eps","eps");
      canv->Print("endcap_"+name+".root","root");
    }
  }
}


void addRun(TString nRun, vector<TString>& sfiles, vector<TString>& sruns){
  ostringstream f_str(""); f_str << "../Run_" << nRun << "/Summary_Run" << nRun << "/Comp_Run" << nRun << ".root";
  sfiles.push_back(f_str.str());
  sruns.push_back(nRun);  
}
  
  
  
  
