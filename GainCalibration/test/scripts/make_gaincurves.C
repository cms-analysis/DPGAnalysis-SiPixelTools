#include <iostream>
#include <vector>
#include "TKey.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TLegend.h"
#include "TList.h"
#include "TStyle.h"
#include "TObject.h"
#include "TSystem.h"

#include "header.h"

int n_histo_max = 100;


void getInfo(TString, int& , int& , int&);
float getgainresult(TString, TFile*, TString, float*);

Double_t mycurvefunc(Double_t *x, Double_t *par){
  return par[0]+ x[0]*par[1];  
}

void make_gaincurves(){
  
  vector<int> n_histo_per_status(9,0) ;
  int n_hist_tot = 0;
  
  //TFile *file = TFile::Open("rfio:///castor/cern.ch/user/r/rougny//GainRun_184644_2/GainRun_184644/0.root");
  TFile *file = TFile::Open("rfio:///castor/cern.ch/user/r/rougny//GainRun_184644_3/GainRun_184644/GainCalibration.root");
  //TFile *file = TFile::Open("/afs/cern.ch/user/r/rougny/gain/CMSSW_3_11_2/src/DQM/SiPixelCommon/test/Run_184644/test/DQM_V0001_Pixel_R000184644.root");
  assert(file);
  file->cd();
  // make a loop over all plots
  TList *list = file->GetListOfKeys();
  gStyle->SetOptStat(0);
  //Int_t nkeys = file->GetNkeys();
  TDirectory *dir = file->GetDirectory("DQMData");
  assert(dir);


  TLatex CMSprelim(0.3,0.3,"CMS Preliminary");
  CMSprelim.SetTextColor(2);
  CMSprelim.SetTextSize(CMSprelim.GetTextSize()*1.2);
  
  TString comparestring = "Module";
  TString curvestring = "GainCurve";
  std::vector<TString> keylist;
  std::vector<TString> hist1list;
  std::vector<TString> hist2list;
  std::vector<TString> dirlist;
  std::vector<TString> notdonelist;
  std::vector<int> nsubdirs;
  
  //TDirectory *dirsav = dir;
  list = dir->GetListOfKeys();
  
  int ikey=0;

  
  dirlist = makedirlist(file , comparestring);
  
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  
  //int ncurves=0;
  for(unsigned i=0; i<dirlist.size() ; ++i){
    
    dir = file->GetDirectory(dirlist[i]); 
    list= dir->GetListOfKeys();
    
    PIX<TH2F*> h;
    h.getFromDir(file , dirlist[i]);
    
    for(ikey=0;ikey<list->GetEntries();  ikey++){
    
      //if(n_hist_tot > n_histo_per_status.size() * n_histo_max * 10) break;
      if(n_histo_per_status[1] > 50 && n_histo_per_status[2] > 50 &&
         n_histo_per_status[3] > 50 && n_histo_per_status[5] > 50 &&
         n_histo_per_status[7] > 50 && n_histo_per_status[8] > 50 ) break;
      
      if ( (n_hist_tot % 100) == 0 ){
        cout << " ++ n_histo_tot: " << n_hist_tot << "   nmod: " << i << endl << " ++ ";
        for(unsigned k = 0 ; k < n_histo_per_status.size() ; ++k)
	  cout << "st_" << k << ": " << n_histo_per_status[k] << "   ";
	cout << endl;
      }
    
      //      std::cout << ikey << std::endl;
      TKey *thekey = (TKey*)list->At(ikey);
      if(thekey==0) continue;
      TString keyname=thekey->GetName();
      keyname.ReplaceAll(" ","");
      TString keytype=thekey->GetClassName();
      keytype.ReplaceAll(" ","");
      //std::cout << ikey << " " <<  keyname << std::endl;

      if(keytype=="TH1F"){
	//std::cout << keyname << std::endl;
	if(keyname.Contains(curvestring)){
	  
	  ++n_hist_tot;
	  
	  int mod=0;int row=0; int col=0;
	  getInfo(keyname , mod , row , col);
	  
	  PIX<double> pix;
	  pix.fill(h , col+1 , row+1);
	  	  
	  ++n_histo_per_status[fabs(pix.fit_result)];
	  if(n_histo_per_status[fabs(pix.fit_result)] > n_histo_max) continue;
	
	  cout << "mod: " << mod << "  row,col: " << row << "," << col << "  gain,ped: " << pix.gain << "," << pix.pedestal
	       << "  status: " << pix.fit_result << endl;
	
	  TH1F* temp = (TH1F*) dir->Get(keyname);
	  dir->cd();
	  TCanvas *curvecanvas1 = new TCanvas();
	  curvecanvas1->cd();
	  temp->SetMarkerSize(2.0); 
	  temp->GetXaxis()->SetTitle("VCAL");
	  temp->GetYaxis()->SetTitle("Average ADC response");
	  temp->Draw(); 
	  
	  
	  
	  if(pix.gain > 0.){
	    TString funcname = keyname;
	    funcname+="func";
	    TF1 *func = new TF1(funcname,mycurvefunc,pix.low_point,pix.high_point,2);
	    func->SetLineWidth(3);
	    func->SetParameter(0,pix.pedestal);
	    func->SetParameter(1,1./pix.gain);
	    func->Draw("lsame");
	    
	    TF1* f2 = (TF1*) func->Clone("f_big_range");
	    f2->SetRange(100,800);
	    f2->SetLineStyle(2);
	    f2->SetLineWidth(2);
	    //f2->Draw("lsame");
	    
	    //CMSprelim.DrawText(10,10,"CMS Pixel");
	    TLegend *legend = new TLegend(0.11,0.2,0.7,0.3);
	    legend->SetFillStyle(0);
	    legend->SetBorderSize(0);
	    TString fitstring = " y = #frac{x}{";
	    fitstring+= pix.gain;
	    fitstring+="} + " ;
	    fitstring+= pix.pedestal;
	    
	    legend->AddEntry(func,fitstring,"l");
	    legend->Draw("same");
	  }
	  //curvecanvas->Update();
	  //curvecanvas->Print(keyname+".jpg");
	  ostringstream fig_str("");
	  fig_str << keyname << "_status_" << pix.fit_result << ".png";
	  curvecanvas1->Print(fig_str.str().c_str());
	  curvecanvas1->Close();
	  delete curvecanvas1;
	  delete temp;
	}
      } // end of if(TH1F)
      

    }//end of keylist loop
    
    h._delete();
  } //end of dirlist loop
  //  for(int i=0; i<notdonelist.size();++i){
  //    std::cout << notdonelist[i] << std::endl;
  //  }
  for(unsigned i=0 ; i < n_histo_per_status.size() ; ++i)
    cout << "status " << i << " :  " << n_histo_per_status[i] << endl << endl;


  gSystem->Exec("tar czf figs.bz.tar *.png");
  gSystem->Exec("echo \"scp $USER@lxplus.cern.ch:`pwd`/figs.bz.tar .\"");
  gSystem->Exec("echo \"tar xzf figs.bz.tar\"");
  
}

void getInfo(TString keyname , int& detid , int& row, int& col){
  const char* strversiontemp = keyname;
  std::string strversion = strversiontemp;
  std::string cutstring[20];
  unsigned foundunderscore=0;
  for(int i=0; i<20 && foundunderscore<strversion.size(); i++){
    int found=strversion.find("_",foundunderscore);
    std::string thesubstr = strversion.substr(foundunderscore,found-foundunderscore);
    //std::cout << thesubstr << " " << found << std::endl;
    cutstring[i]=thesubstr;
    foundunderscore=found+1;
  }

  row = atoi(cutstring[2].c_str());
  col = atoi(cutstring[4].c_str());
  detid = atoi(cutstring[6].c_str());
}


float getgainresult(TString keyname, TFile *file, TString dirname, float *gainandped){

  //  std::cout << "examining histo " << keyname << std::endl;
  const char* strversiontemp = keyname;
  std::string strversion = strversiontemp;
  std::string cutstring[20];
  unsigned foundunderscore=0;
  for(int i=0; i<20 && foundunderscore<strversion.size(); i++){
    int found=strversion.find("_",foundunderscore);
    std::string thesubstr = strversion.substr(foundunderscore,found-foundunderscore);
    //std::cout << thesubstr << " " << found << std::endl;
    cutstring[i]=thesubstr;
    foundunderscore=found+1;
  }

  int row = atoi(cutstring[2].c_str());
  int col = atoi(cutstring[4].c_str());
  int detid = atoi(cutstring[6].c_str());
  //  std::cout << row << " " << col << " " << detid << std::endl;
  TString gainhisto = dirname;
  gainhisto+="/Gain2d_siPixelCalibDigis_";
  gainhisto+=detid;
  TString pedhisto =  dirname;
  pedhisto+="/Pedestal2d_siPixelCalibDigis_";
  pedhisto+=detid;
  //  std::cout << gainhisto << " " << pedhisto << std::endl;
  file->cd();
  TH2F *gain2d =(TH2F*)file->Get(gainhisto);
  //float gainval=0;
  //float pedval=0;
  if(gain2d)
    gainandped[0] = gain2d->GetBinContent(col+1,row+1);
  else return 0;
  TH2F *ped2d = (TH2F*)file->Get(pedhisto);
  if(ped2d)
    gainandped[1]=ped2d->GetBinContent(col+1,row+1);
  else return 0;
  //std::cout << ped2d->GetNbinsX() << " " << ped2d->GetNbinsY() << std::endl;

  //std::cout << pedhisto << " " << gainhisto << " " << col+1 << " " << row+1 << " " <<  ped2d->GetBinContent(col+1,row+1) << " " << gain2d->GetBinContent(col+1,row+1) <<  std::endl;
  //gainval = gain2d->GetBinContent(col+1,row+1);
  //pedval = ped2d->GetBinContent(col+1,row+1);
  //  std::cout << gainhisto << " " << row << " " << col << " " << gainval << " " << pedval << std::endl;
  //gainandped[0]=gainval;
  //gainandped[1]=pedval;
  return 1;
}


