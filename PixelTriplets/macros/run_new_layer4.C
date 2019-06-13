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

TString Run="300558";//"1";

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
int fittp0(const char* hs , float & sigma_res,float & sigma_res_err , float & mean_res,float & mean_res_err ) {
 gROOT->Reset();
  gStyle->SetOptStat(0);
  TCanvas* c = new TCanvas("c","c",0,0,800,800); 
  TPad *pad = new TPad("pad","pad",0.2,0.01,0.99,0.99);
  pad->SetBottomMargin(0.18);
  // pad->SetLeftMargin(0.25);
  cout <<" Created pad for hist"<< hs  <<endl;

  TH1 *h = (TH1*)gDirectory->Get(hs);

  if( h == NULL ){

    cout << hs << " does not exist\n";

  }

  else{
   
    h->SetMarkerStyle(21);
    h->SetMarkerSize(0.8);
    /*commented out to make the plot look nice 
    /// h->SetStats(1);
    /// gStyle->SetOptFit(101);
    */
    /*to make the plot look nice */
    //  gROOT->ForceStyle();
    gStyle->SetOptStat(0000);
   
    h->SetTitle("");
    /*to make the plot look nice */

    gROOT->ForceStyle();
    
    h->Rebin(2);
    TGaxis::SetMaxDigits(3);

    double dx = h->GetBinWidth(1);
    double nmax = h->GetBinContent(h->GetMaximumBin());
    double xmax = h->GetBinCenter(h->GetMaximumBin());
    double nn = 7*nmax;

    int nb = h->GetNbinsX();
    double n1 = h->GetBinContent(1);
    double n9 = h->GetBinContent(nb);
    double bg = 0.5*(n1+n9);

    double x1 = h->GetBinCenter(1);
    double x9 = h->GetBinCenter(nb);
    cout << hs << ": " << x1 << " - " << x9 << endl;

    // create a TF1 with the range from x1 to x9 and 5 parameters
   
     TF1 *tp0Fcn ;
    //Template
    // if (hs=="h420_out_xplus_zplus") {
    //   tp0Fcn = new TF1( "tp0Fcn", tp0Fit, -100, 100, 5 );
    // }else{
    //   tp0Fcn = new TF1( "tp0Fcn", tp0Fit, x1, x9, 5 );
    // }
     //Generic
     // if (hs.Contains("h420_in_xminus_zplus") ){
     //   tp0Fcn = new TF1( "tp0Fcn", tp0Fit, -50, 50, 5 );
     //}else{
     tp0Fcn = new TF1( "tp0Fcn", tp0Fit, x1, x9, 5 );
     //}



    tp0Fcn->SetParName( 0, "mean" );
    tp0Fcn->SetParName( 1, "sigma" );
    tp0Fcn->SetParName( 2, "nu" );
    tp0Fcn->SetParName( 3, "area" );
    tp0Fcn->SetParName( 4, "BG" );

    tp0Fcn->SetNpx(500);
    tp0Fcn->SetLineWidth(4);
    tp0Fcn->SetLineColor(kMagenta);
    tp0Fcn->SetLineColor(kGreen);
   
    // set start values for some parameters:

    cout << hs << " " << dx << ", " << nn << ", " << xmax << endl;

    tp0Fcn->SetParameter( 0, 0.);//xmax ); // peak position
    tp0Fcn->SetParameter( 1, 4*dx ); // width
    tp0Fcn->SetParameter( 2, 2.2 ); // nu
    tp0Fcn->SetParameter( 3, nn ); // N
    tp0Fcn->SetParameter( 4, bg );
    h->Fit( "tp0Fcn", "R", "ep" );
     // h->Fit("tp0Fcn","V+","ep");
    sigma_res= tp0Fcn->GetParameter(1);
    sigma_res_err= tp0Fcn->GetParError(1);
    mean_res= tp0Fcn->GetParameter(0);
    mean_res_err= tp0Fcn->GetParError(0);
    
    std::cout <<  hs << ": mean " <<mean_res << " +-"  << mean_res_err   << " sigma "<< sigma_res << " +-"  << sigma_res_err  <<std::endl;
    std::cout << "*************"<<std::endl;

    ////just plotting it nice 
 
    // TLegend *pl1 = new TLegend(0.10,0.70,0.30,0.89);
    // pl1->SetTextSize(0.040); 
    // pl1->SetFillColor(0);
    // pl1->SetBorderSize(0);
    // TLegendEntry *ple1 = pl1->AddEntry((TObject*)0, "Track p_{T} > 12 GeV",  "");

    TLegend *pl2 = new TLegend(0.55,0.54,0.65,0.89);
    pl2->SetTextSize(0.04); 
    pl2->SetFillColor(0);
    pl2->SetBorderSize(0);

    TString name =  (TString) hs ;
    Tsring header_text;
    // if ( name.Contains("h52") )  pl2->SetHeader("Barrel Pixel Layer 1");
    // if ( name.Contains("f52") || name.Contains("g52") )  pl2->SetHeader("Barrel Pixel Layer 4");
    // if ( name.Contains("h42") || name.Contains("hf42") )  pl2->SetHeader("Barrel Pixel Layer 2");
    // if ( name.Contains("hg42") || name.Contains("i52") )  pl2->SetHeader("Barrel Pixel Layer 3");

    if ( name.Contains("h52") )  header_text="Barrel Pixel Layer 1";
    if ( name.Contains("f52") || name.Contains("g52") )  header_text="Barrel Pixel Layer 4";
    if ( name.Contains("h42") || name.Contains("hf42") )  header_text="Barrel Pixel Layer 2";
    if ( name.Contains("hg42") || name.Contains("i52") )   header_text="Barrel Pixel Layer 1";

    if ( name.Contains("in")) header_text+=" in";
    else header_text+=" out";

    pl2->SetHeader(header_text);

    // pl2->SetHeaderSize(0.045);
    TLegendEntry *ple2 = pl2->AddEntry(h, "Triplet Residuals",  "P");
    ple2 =pl2->AddEntry(tp0Fcn, "Student-t function fit",  "L");
    TLegendEntry *ple4 = pl2->AddEntry((TObject*)0, "#mu_{r}="+TString::Format("%.2f",mean_res )+"#pm"+ TString::Format("%.2f",mean_res_err )  +" #mum",  "");
    ple4->SetTextColor(kRed);
    TLegendEntry *ple3 = pl2->AddEntry((TObject*)0, "#sigma_{r}="+TString::Format("%.2f",sigma_res )+"#pm"+ TString::Format("%.2f",sigma_res_err )  +" #mum",  "");
    TLegendEntry *ple5=  pl2->AddEntry((TObject*)0, "RMS="+TString::Format("%.2f",h->GetRMS())+" #mum",  "");
    ple3->SetTextColor(kRed);
    ple5->SetTextColor(kRed);
    
    TLegendEntry *header = (TLegendEntry*)pl2->GetListOfPrimitives()->First();
    header->SetTextAlign(22);
    header->SetTextSize(0.045); 
   
    h->GetYaxis()->SetTitle("Number of hits / "+ TString::Format("%.0f",dx) +" #mum");
    h->GetXaxis()->SetTitle("#Deltax [#mum]");
    if ( name.Contains("21"))  h->GetXaxis()->SetTitle("#Deltaz [#mum]");

    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleOffset(0.85); 
    h->GetXaxis()->SetTitleOffset(0.85);
    h->GetXaxis()->SetLabelSize(0.05);
    h->GetYaxis()->SetLabelSize(0.05);

    TString cmsText     = "CMS";
    float cmsTextFont   = 61;  // default is helvetic-bold
    bool writeExtraText = true;
    TString extraText   = "Work in progress Phase1";
    float extraTextFont = 52;  // default is helvetica-italics
    // text sizes and text offsets with respect to the top frame in unit of the top margin size
    float lumiTextSize     = 0.5;
    float lumiTextOffset   = 0.15;
    float cmsTextSize      = 0.65;
    float cmsTextOffset    = 0.1;  // only used in outOfFrame version
    float relPosX    = 0.045;
    float relPosY    = 0.035;
    float relExtraDY = 1.2;
    // ratio of "CMS" and extra text size
    float extraOverCmsTextSize  = 0.65;
    TString lumi_13TeV = "20.1 fb^{-1}";
    TString lumi_8TeV  = "19.7 fb^{-1}";
    TString lumi_7TeV  = "5.1 fb^{-1}";
    TString lumiText;
    // lumiText += lumi_8TeV;
    // lumiText += " (13 TeV)";
    lumiText = "#sqrt{s} = 13 TeV ";
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
    latex.DrawLatex(l+0.03,1-t+lumiTextOffset*t-0.09,cmsText);   
    latex.SetTextFont(extraTextFont);
    latex.SetTextSize(extraTextSize*t);
    latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t-0.09-0.06, extraText);      
    TString TrackSelctionText = "Track p_{T} > 12 GeV";
    latex.SetTextFont(42);
    latex.SetTextSize(extraTextSize*t);
    latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t-0.09-0.06-0.12, TrackSelctionText);      

    //  pl1->Draw("same");
    tp0Fcn->Draw("same");
    pl2->Draw("same");
    h->Draw("hist ep same"); // data again on top
    // gPad->Print(hs+".png"); 
    gPad->RedrawAxis("same");
    c->Update();
    h->SetMaximum(h->GetMaximum() *1.4);
    h->Draw("hist ep same");

    c->Update();

   
    TString name_root =  (TString) hs ;
    name +=(TString) "_"+Run+(TString)".pdf" ;
    name_root +=(TString) "_"+Run+(TString)".root" ;
  
   


    c->SaveAs("file_niceplots/Template/"+name_root);
    c->SaveAs("file_niceplots/Template/"+name);
    gPad->Print(name); 
    delete tp0Fcn;

  }
 return 0;
 
}
int run_new_layer4(const char* file ){
  
  TFile *_file0 = new TFile(file);

  _file0->cd((TString)"Histos/Run"+Run);
 //Get resolution along x
  TH1F*  h420= (TH1F*) _file0->Get((TString)"Histos/Run"+Run+(TString)"/h420");
  h420->SetDirectory(0); // "detach" the histogram from the file
  float sigma_x,sigma_x_err,mean_x,mean_x_err ;
  fittp0("h420",sigma_x,sigma_x_err,mean_x,mean_x_err);
 
  //Get resolution along y
  TH1F*  h421= (TH1F*) _file0 ->Get((TString)"Histos/Run"+Run+(TString)"/h421");
  h421->SetDirectory(0); // "detach" the histogram from the file
  float sigma_y,sigma_y_err, mean_y, mean_y_err ;
  fittp0("h421", sigma_y, sigma_y_err, mean_y, mean_y_err);

 
  //Get resolution along x
  TH1F*  hf420= (TH1F*) _file0->Get((TString)"Histos/Run"+Run+(TString)"/hf420");
  hf420->SetDirectory(0); // "detach" the histogram from the file
  float sigma_x_l142,sigma_x_err_l142,mean_x_l142,mean_x_err_l142 ;
  fittp0("hf420",sigma_x_l142,sigma_x_err_l142,mean_x_l142,mean_x_err_l142);
 
  //Get resolution along y
  TH1F*  hf421= (TH1F*) _file0 ->Get((TString)"Histos/Run"+Run+(TString)"/hf421");
  hf421->SetDirectory(0); // "detach" the histogram from the file
  float sigma_y_l142,sigma_y_err_l142, mean_y_l142, mean_y_err_l142 ;
  fittp0("hf421", sigma_y_l142, sigma_y_err_l142, mean_y_l142, mean_y_err_l142);

 
 //Get resolution along x
  TH1F*  f520= (TH1F*) _file0->Get((TString)"Histos/Run"+Run+(TString)"/f520");
  f520->SetDirectory(0); // "detach" the histogram from the file
  float sigma_x_l124,sigma_x_err_l124,mean_x_l124,mean_x_err_l124 ;
  fittp0("f520",sigma_x_l124,sigma_x_err_l124,mean_x_l124,mean_x_err_l124);
 
  //Get resolution along y
  TH1F*  f521= (TH1F*) _file0 ->Get((TString)"Histos/Run"+Run+(TString)"/f521");
  f521->SetDirectory(0); // "detach" the histogram from the file
  float sigma_y_l124,sigma_y_err_l124, mean_y_l124, mean_y_err_l124 ;
  fittp0("f521", sigma_y_l124, sigma_y_err_l124, mean_y_l124, mean_y_err_l124);

 
 //Get resolution along x
  TH1F*  i520= (TH1F*) _file0->Get((TString)"Histos/Run"+Run+(TString)"/i520");
  i520->SetDirectory(0); // "detach" the histogram from the file
  float sigma_x_l123,sigma_x_err_l123,mean_x_l123,mean_x_err_l123 ;
  fittp0("i520",sigma_x_l123,sigma_x_err_l123,mean_x_l123,mean_x_err_l123);
 
  //Get resolution along y
  TH1F*  i521= (TH1F*) _file0 ->Get((TString)"Histos/Run"+Run+(TString)"/i521");
  i521->SetDirectory(0); // "detach" the histogram from the file
  float sigma_y_l123,sigma_y_err_l123, mean_y_l123, mean_y_err_l123 ;
  fittp0("i521", sigma_y_l123, sigma_y_err_l123, mean_y_l123, mean_y_err_l123);

  //Get resolution along x
  TH1F*  h520= (TH1F*) _file0->Get((TString)"Histos/Run"+Run+(TString)"/h520");
  h520->SetDirectory(0); // "detach" the histogram from the file
  float sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231 ;
  fittp0("h520",sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231);
 
  //Get resolution along y
  TH1F*  h521= (TH1F*) _file0 ->Get((TString)"Histos/Run"+Run+(TString)"/h521");
  h521->SetDirectory(0); // "detach" the histogram from the file
  float sigma_y_l231,sigma_y_err_l231, mean_y_l231, mean_y_err_l231 ;
  fittp0("h521", sigma_y_l231, sigma_y_err_l231, mean_y_l231, mean_y_err_l231);

   //Get resolution along x
  TH1F*  g520= (TH1F*) _file0->Get((TString)"Histos/Run"+Run+(TString)"/g520");
  g520->SetDirectory(0); // "detach" the histogram from the file
  float sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234 ;
  fittp0("g520",sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234);
 
  //Get resolution along y
  TH1F*  g521= (TH1F*) _file0 ->Get((TString)"Histos/Run"+Run+(TString)"/g521");
  g521->SetDirectory(0); // "detach" the histogram from the file
  float sigma_y_l234,sigma_y_err_l234, mean_y_l234, mean_y_err_l234 ;
  fittp0("g521", sigma_y_l234, sigma_y_err_l234, mean_y_l234, mean_y_err_l234);

  //Get resolution along x
  TH1F*  hg420= (TH1F*) _file0->Get((TString)"Histos/Run"+Run+(TString)"/hg420");
  hg420->SetDirectory(0); // "detach" the histogram from the file
  float sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243 ;
  fittp0("hg420",sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243);
 
  //Get resolution along y
  TH1F*  hg421= (TH1F*) _file0 ->Get((TString)"Histos/Run"+Run+(TString)"/hg421");
  hg421->SetDirectory(0); // "detach" the histogram from the file
  float sigma_y_l243,sigma_y_err_l243, mean_y_l243, mean_y_err_l243 ;
  fittp0("hg421", sigma_y_l243, sigma_y_err_l243, mean_y_l243, mean_y_err_l243);

  _file0->Close();
  return 0;


}
