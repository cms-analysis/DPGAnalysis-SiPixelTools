#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TString.h"
#include <fstream>
#include <sys/stat.h>
//TString Run="301912";
//TString Reco="Generic";
// Student's t function:

Double_t tp0Fit( Double_t *x, Double_t *par5 ) {

  static int nn = 0;
  nn++;
  static double dx = 0.1;
  static double b1 = 0;
  if( nn == 1 ) b1 = x[0];
  if( nn == 2 ) dx = x[0] - b1;
  //
  //--  Mean and width:
  //
  double xm = par5[0];
  double t = ( x[0] - xm ) / par5[1];
  double tt = t*t;
  //
  //--  exponent:
  //
  double rn = par5[2];
  double xn = 0.5 * ( rn + 1.0 );
  //
  //--  Normalization needs Gamma function:
  //
  double pk = 0.0;

  if( rn > 0.0 ) {

    double pi = 3.14159265358979323846;
    double aa = dx / par5[1] / sqrt(rn*pi) * TMath::Gamma(xn) / TMath::Gamma(0.5*rn);

    pk = par5[3] * aa * exp( -xn * log( 1.0 + tt/rn ) );
  }

  return pk + par5[4];
}
//
//----------------------------------------------------------------------
//
int fittp0(const char* hs, Double_t& bias, Double_t& delta_bias, int timeCall=1) {
  TString dir = "z";
  TH1 *h;
  if (dir == (TString)"phi"){
    TH1 *hs_1 = (TH1*)gDirectory->Get(hs);
    h  = new TH1F(hs_1->GetName(), hs_1->GetTitle(), hs_1->GetNbinsX(), -TMath::Pi(), TMath::Pi());
    // hs_1->Sumw2();
    double rad_per_degree = TMath::Pi()/180.;
    for(int i = 0; i < hs_1->GetNbinsX(); i++){
      h->Fill(hs_1->GetXaxis()->GetBinCenter(i)*rad_per_degree, hs_1->GetBinContent(i));
      h->SetBinError(i,hs_1->GetBinError(i));
    }
    h->GetXaxis()->SetTitle("#phi [rad]");
    h->GetYaxis()->SetTitle(hs_1->GetYaxis()->GetTitle());
  }
  else{
    h = (TH1*)gDirectory->Get(hs);
  }

  if( h == NULL ){

    cout << hs << " does not exist\n";

  }

  else{
    h->SetMarkerStyle(21);
    h->SetMarkerSize(0.8);
    gStyle->SetOptStat(0000);

    h->SetTitle("");
    TGaxis::SetMaxDigits(3);


    TLegend *pl2;
    TLegend *pl3;

    pl2 = new TLegend(0.5,0.65,0.85,0.9);
    pl3 = new TLegend(0.5,0.6,0.85,0.65);


    pl2->SetTextSize(0.04);
    pl2->SetFillStyle(0);
    pl2->SetBorderSize(0);

    pl3->SetTextSize(0.04);
    pl3->SetFillStyle(0);
    pl3->SetBorderSize(0);

    TString name =  (TString) hs ;


    if ( name.Contains("h5") )  pl2->SetHeader("Barrel Pixel Layer 1");
    if ( name.Contains("f5") || name.Contains("g5") )  pl2->SetHeader("Barrel Pixel Layer 4");
    if ( name.Contains("h4") || name.Contains("hf4")  || name.Contains("hf52"))  pl2->SetHeader("Barrel Pixel Layer 2");
    if ( name.Contains("hg4") || name.Contains("i5") )  pl2->SetHeader("Barrel Pixel Layer 3");

    // pl2->SetHeaderSize(0.045);
    TLegendEntry *ple2 = pl2->AddEntry((TObject*)0, "Triplet Residuals",  "P");
    TF1 *myfit1, *myfit2;

    if(timeCall == 1){
      if(name.Contains("h514") || name.Contains("h524") || name.Contains("h525")) {
        myfit1 = new TF1("myf1","[0]", -10, 10);
      }
      if(name.Contains("h515")) {
        myfit1 = new TF1("myf1","[0]*x+[1]", -10, 10);
      }
      // h->Fit("myf1", "RM", "ep same");
      // bias = myfit1->GetParameter(0);
      // bias *= 0.0001;
      // delta_bias = myfit1->GetParError(0);
      // delta_bias *= 0.0001;
    }

    if(timeCall == 2){
      // if(name.Contains("h514") || name.Contains("h524") || name.Contains("h525")) {
      //   myfit2 = new TF1("myf2","[0]", -10, 10);
      // }
      // if(name.Contains("h515")) {
      //   myfit2 = new TF1("myf2","[0]*x+[1]", -10, 10);
      // }
      // // h->Fit("myf2", "RM", "ep same");
      // bias = myfit2->GetParameter(0);
      // delta_bias = myfit2->GetParError(0);
    }

    if ( name.Contains("in")){
      TLegendEntry  *ple3 =pl3->AddEntry(h, "in- facing modules",  "L");
      h->SetMarkerColor(kViolet);

      h->SetLineColor(kViolet);
      ple3->SetTextColor(h->GetLineColor());
    }else	if ( name.Contains("out")){
      TLegendEntry  *ple4 =pl2->AddEntry(h, "out- facing modules",  "L");

      h->SetMarkerColor(kViolet+8);

      h->SetLineColor(kViolet+8);
      ple4->SetTextColor(h->GetLineColor());
    }
    else{
      h->SetMarkerColor(kViolet+8);
      h->SetLineColor(kViolet+8);
      h->GetYaxis()->SetRangeUser(-20.,20.);
    }

    h->Draw(" ep same");
    if ( name.Contains("h52")){
      if ( name.Contains("h524")) h->GetYaxis()->SetRangeUser(0,60);
      if ( name.Contains("h525")){
        h->GetYaxis()->SetRangeUser(20,130);
        h->GetXaxis()->SetTitle("PXB1 z [cm]");
        h->GetYaxis()->SetTitle("PXB1 rms(#Delta z) [#mum]");
      }
    }

    if ( name.Contains("h51")){
      h->GetYaxis()->SetRangeUser(-50,50);
      if ( name.Contains("h515")){
        h->GetXaxis()->SetTitle("PXB1 z [cm]");
      }
    }

    h->Draw(" ep same");




    if (timeCall==1) pl2->Draw("same");
    if (timeCall==2) pl3->Draw("same");



  }
 return 0;

}


void plot_per_layer(const char* hs1, const char* hs2, const char* file , TString Reco, TString Run, TString layerHV, TString HV_val){
  TCanvas* c = new TCanvas("c","c",0,0,800,800);
  c->Draw();
  Double_t bias1, bias2, delta_bias1, delta_bias2, bias, delta_bias;
  TPad *pad = new TPad("pad","pad",0.2,0.01,0.99,0.99);
  pad->SetBottomMargin(0.18);

  TFile *_file0 = new TFile(file);
  //TString directory = "Histos/Run";
  TString directory = "BPixResolution_"+Reco+"/Run";
  TString dir0="file_niceplots_check_vs_z";
  bool isApproval = false;
  _file0->cd(directory+Run);
  //Get resolution along x
  TH2F*  hs_1= (TH2F*) _file0->Get(directory+Run+(TString)"/"+ (TString)hs1);
  hs_1->SetDirectory(0);
  fittp0(hs1,bias1,delta_bias1, 1);
  TString name_of_file = (TString) hs1;
  //hs_1->Draw();
  if ((TString)hs2!=(TString)""){
    TH2F*  hs_2= (TH2F*) _file0->Get(directory+Run+(TString)"/"+ (TString)hs2);
    hs_2->SetDirectory(0);
    fittp0(hs2,bias2,delta_bias2, 2);

  }
  // if(name_of_file.Contains("h514") || name_of_file.Contains("h524") || name_of_file.Contains("h525")) {
  //   bias = abs(bias1 - bias2);
  //   delta_bias = TMath::Sqrt(delta_bias1*delta_bias1 + delta_bias2*delta_bias2);
  // }
  // if(name_of_file.Contains("h515")) {
  //   bias = abs(TMath::ATan(bias1) - TMath::ATan(bias2));
  //   delta_bias = TMath::Sqrt(delta_bias1/(1 + bias1*bias1)*delta_bias1/(1 + bias1*bias1) + delta_bias2/(1 + bias2*bias2)*delta_bias2/(1 + bias2*bias2));
  // }

  // std::ofstream outfile;
  // outfile.open("Bias_"+Run+"_"+Reco+".txt", std::ios_base::app);
  // if(name_of_file.Contains("h514"))   outfile << "L1, x, Delta, delta_Delta, "       << HV_val << ": ";
  // if(name_of_file.Contains("h515"))   outfile << "L1, z, Delta, delta_Delta, "       << HV_val << ": ";
  // if(name_of_file.Contains("h524"))   outfile << "L1, x, RMS_Delta, delta_RMS_Delta, " << HV_val << ": ";
  // if(name_of_file.Contains("h525"))   outfile << "L1, z, RMS_Delta, delta_RMS_Delta, " << HV_val << ": ";
  // outfile << bias << ", " << delta_bias << std::endl;

  TF1 *constant = new TF1("constant", "0*x", -100, 100);
  constant->SetLineWidth(2);
  constant->SetLineColor(kBlack);
  constant->Draw("ep same");

  fittp0(hs1,bias1,delta_bias1, 1);
  fittp0(hs2,bias2,delta_bias2, 2);

  TString cmsText     = "CMS";
  float cmsTextFont   = 61;  // default is helvetic-bold
  bool writeExtraText = true;
  TString extraText;
  if(isApproval){
    if(Run=="1") extraText   = "Simulation 2018";// Phase1";
    else extraText          = "Preliminary 2018";
  }
  else {
    if(Run=="1") extraText = "Simulation 2018";// Phase1";
    else extraText         = "Work in progress 2018";
  }
  float extraTextFont    = 52;  // default is helvetica-italics
  // text sizes and text offsets with respect to the top frame in unit of the top margin size
  float lumiTextSize     = 0.5;
  float lumiTextOffset   = 0.15;
  float cmsTextSize      = 0.65;
  float cmsTextOffset    = 0.1;  // only used in outOfFrame version
  float relPosX          = 0.045;
  float relPosY          = 0.035;
  float relExtraDY       = 1.2;
  // ratio of "CMS" and extra text size
  float extraOverCmsTextSize  = 0.65;
  TString lumi_13TeV = "20.1 fb^{-1}";
  TString lumi_8TeV  = "19.7 fb^{-1}";
  TString lumi_7TeV  = "5.1 fb^{-1}";
  TString lumiText;
  // lumiText += lumi_8TeV;
  // lumiText += " (13 TeV)";
  if (isApproval) lumiText = "(13 TeV)";
  else            lumiText = "#sqrt{s} = 13 TeV ";
  float t = pad->GetTopMargin();
  float b = pad->GetBottomMargin();
  float r = pad->GetRightMargin();
  float l = pad->GetLeftMargin();
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  float extraTextSize = extraOverCmsTextSize*cmsTextSize;
  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize*t);
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);
  latex.SetTextFont(cmsTextFont);
  latex.SetTextAlign(11);
  latex.SetTextSize(cmsTextSize*t);
  float shiftOfPow = 0;
  shiftOfPow = -0.1;
  if (isApproval) latex.DrawLatex(l+0.10+shiftOfPow,1-t+lumiTextOffset*t-0.00,cmsText);
  else            latex.DrawLatex(l+0.03,1-t+lumiTextOffset*t-0.09,cmsText);
  latex.SetTextFont(extraTextFont);
  latex.SetTextSize(extraTextSize*t);
  if (isApproval) latex.DrawLatex(l+0.25+shiftOfPow, 1-t+lumiTextOffset*t-0.00, extraText);
  else            latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t-0.09-0.06, extraText);

  TString TrackRecoText;
  if (isApproval) TrackRecoText = "Template reconstruction";
  else            TrackRecoText = "Template reco";
  if (Reco.Contains("Generic")){
    if (isApproval) TrackRecoText = "Generic reconstruction";
    else            TrackRecoText = "Generic reco";
  }
  latex.SetTextFont(42);
  latex.SetTextSize(extraTextSize*t*0.9);
  if (isApproval) latex.DrawLatex(l+0.02, 1-t+lumiTextOffset*t-0.09-0.02, TrackRecoText);
  else            latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t-0.09-0.06-0.06, TrackRecoText);

  TString TrackSelectionText;
  TrackSelectionText = "Track p_{T} > 4 GeV" ;
  latex.SetTextFont(42);
  latex.SetTextSize(extraTextSize*t*0.9);
  if (isApproval) latex.DrawLatex(l+0.02, 1-t+lumiTextOffset*t-0.09-0.06, TrackSelectionText);
  else            latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t-0.09-0.06-0.12, TrackSelectionText);



  gPad->RedrawAxis("same");
  c->Update();
  TString name;
  TString name_root;
  if ((TString)hs2!=(TString)""){
    name =  (TString) hs1 +(TString)"_"+ (TString) hs2;
    name_root =  (TString) hs1 +(TString)"_"+ (TString) hs2;
  }
  else{
    name =  (TString) hs1;
    name_root =  (TString) hs1;
  }

  name +=(TString) "_"+Run+(TString)"_defThre.pdf" ;
  name_root +=(TString) "_"+Run+(TString)"_defThre.root" ;

  // if (!HV_val.IsDec()){
  //   TString dir="file_niceplots_HV_bias_scan/"+Run;
  //   const int dir_err1 = mkdir(dir.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  //   dir="file_niceplots_HV_bias_scan/"+Run+"/"+Reco;
  //   const int dir_err2 = mkdir(dir.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  //   c->SaveAs("file_niceplots_HV_bias_scan/"+Run+"/"+Reco+"/"+name_root);
  //   c->SaveAs("file_niceplots_HV_bias_scan/"+Run+"/"+Reco+"/"+name);
  // }
  // else{
  const int dir_err0 = mkdir(dir0.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  TString dir=dir0+"/"+Run;
  const int dir_err1 = mkdir(dir.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  dir=dir0+"/"+Run+"/"+Reco;
  const int dir_err2 = mkdir(dir.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  c->SaveAs(dir0+"/"+Run+"/"+Reco+"/"+name_root);
  c->SaveAs(dir0+"/"+Run+"/"+Reco+"/"+name);
  // c->SaveAs(dir0+"/"+Run+"/"+Reco+"/"+name_png);
  // c->SaveAs(dir0+"/"+Run+"/"+Reco+"/"+name_eps);
  // }
  // gPad->Print(name);

  _file0->Close();


}



int run_new_RMSvsZ_JustPlot(const char* file ){
  TString Run;
  TString HV_val;
  TString Reco;
  TString layerHV;
  int bias_index=11;
  Ssiz_t ind = 0;
  while(((TString)file).Index("/Resolution", ind+1) > 0) ind = ((TString)file).Index("/Resolution",ind+1);
  if(ind != 0) ind++;
  bias_index += ind;
  Run = (TString)file[bias_index+0]+(TString)file[bias_index+1]+(TString)file[bias_index+2]+(TString)file[bias_index+3]+(TString)file[bias_index+4]+(TString)file[bias_index+5];
  if(!Run.IsDec()) Run="1";
  if ((TString)file[bias_index+7] == "L"){
    layerHV = (TString)file[bias_index+7]+(TString)file[bias_index+8];
    // cout << layerHV << endl;
    HV_val = (TString)file[bias_index+10]+(TString)file[bias_index+11]+(TString)file[bias_index+12];
    if((TString)file[bias_index+12] == (TString)"V") HV_val = (TString)file[bias_index+10]+(TString)file[bias_index+11];
    if((TString)file[bias_index+12] == (TString)"_") HV_val = (TString)file[bias_index+10];
  }
  HV_val = "allHV";
  if (layerHV == (TString)"L1"){
    // if(!Run.IsDec()) {
      // Run = (TString)"1";
      // HV_val = "ClusterRew_reweight_v1";
      // HV_val = "ClusterRew_v1";
      // HV_val = "DynIneff_v1";
      // HV_val = "DynIneff_v2";
      // HV_val = "MC_ref";
      // HV_val = "ClusterRew_reweight_end2017";
    }
    for(int is_template_reco = 0; is_template_reco < 2; is_template_reco++){
      if(is_template_reco == 0)      Reco = "Generic";
      else if(is_template_reco == 1) Reco = "Template";
      // if (layerHV == (TString)"L1"){
      //   plot_per_layer("h514_out","h514_in", file, Reco, Run, layerHV, HV_val);
      //   plot_per_layer("h515_out","h515_in", file, Reco, Run, layerHV, HV_val);
      //   plot_per_layer("h524_out","h524_in", file, Reco, Run, layerHV, HV_val);
      //   plot_per_layer("h525_out","h525_in", file, Reco, Run, layerHV, HV_val);
      // }
      plot_per_layer("h514_out","h514_in", file, Reco, Run, layerHV, HV_val);
      plot_per_layer("h515_out","h515_in", file, Reco, Run, layerHV, HV_val);
      plot_per_layer("h524_out","h524_in", file, Reco, Run, layerHV, HV_val);
      plot_per_layer("h525_out","h525_in", file, Reco, Run, layerHV, HV_val);
      //
      // plot_per_layer("h514_in_bmo2", "h514_out_bmo2", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h515_in_bmo2", "h515_out_bmo2", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h524_in_bmo2", "h524_out_bmo2", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h525_in_bmo2", "h525_out_bmo2", file, Reco, Run, layerHV, HV_val);




      // plot_per_layer("h512", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h513", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h412", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h413", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("hg412","", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("hg413","", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("g512", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("g513", "", file, Reco, Run, layerHV, HV_val);

      // plot_per_layer("h522", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h523", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h422", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("h423", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("hg422","", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("hg423","", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("g522", "", file, Reco, Run, layerHV, HV_val);
      // plot_per_layer("g523", "", file, Reco, Run, layerHV, HV_val);

  }
  return 0;


}
