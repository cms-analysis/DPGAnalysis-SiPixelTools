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
#include <utility>
#include <map>
#include "TVirtualFitter.h"
//TString Run="306458";//"1";
//TString Reco="Template";//"1";

// Student's t function:
const bool isMiniBiasScan = false;
Double_t rej = 6.;
Bool_t reject = kFALSE;

std::map<int,double> runToIntLumiInit = {
std::make_pair(317434,19.22),
std::make_pair(317435,19.58),
std::make_pair(317509,20.21),
std::make_pair(317527,20.90),
std::make_pair(317626,21.55),
std::make_pair(317640,21.81),
std::make_pair(317648,22.12),
std::make_pair(317649,22.32),
std::make_pair(317650,22.57),
std::make_pair(317683,23.26),
std::make_pair(317696,23.49),
std::make_pair(317661,22.97),
std::make_pair(317663,23.12),
std::make_pair(315784,4.84),
std::make_pair(315785,4.95),
std::make_pair(315786,4.97),
std::make_pair(315787,5.10),
std::make_pair(315840,5.96),
std::make_pair(316058,6.41),
std::make_pair(316059,6.58),
std::make_pair(316060,6.78),
std::make_pair(316153,7.74),
std::make_pair(316187,8.30),
std::make_pair(316199,8.71),
std::make_pair(316216,9.09),
std::make_pair(316217,9.18),
std::make_pair(316218,9.43),
std::make_pair(316219,9.48),
std::make_pair(316239,9.70),
std::make_pair(316505,11.53),
std::make_pair(316590,12.22),
std::make_pair(316715,13.15),
std::make_pair(316716,13.22),
std::make_pair(316717,13.29),
std::make_pair(316758,14.10),
std::make_pair(316876,14.90),
std::make_pair(316944,15.61),
std::make_pair(317087,16.36),
std::make_pair(317182,17.01),
std::make_pair(317291,17.54),
std::make_pair(317340,18.57)
};

Double_t tp0Fit( Double_t *x, Double_t *par5 ) {
  if (reject && x[0] > -rej && x[0] < rej) {
      TF1::RejectPoint();
      return 0;
   }
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
    // double aa = dx / sqrt(rn*pi) * TMath::Gamma(xn) / TMath::Gamma(0.5*rn);

    pk = par5[3] * aa * exp( -xn * log( 1.0 + tt/rn ) );
  }
  return pk + par5[4];
}

Double_t gaus4(Double_t *x, Double_t *par){
  if (reject && x[0] > -rej && x[0] < rej) {
      TF1::RejectPoint();
      return 0;
   }
   return TMath::Gaus(x[0],par[0],par[4],1)*par[8]+TMath::Gaus(x[0],par[1],par[5],1)*par[9]+
          TMath::Gaus(x[0],par[2],par[6],1)*par[10]+TMath::Gaus(x[0],par[3],par[7],1)*par[11]+par[12];
}

Double_t Gauss(Double_t *x, Double_t *par){
  if (reject && x[0] > -rej && x[0] < rej) {
      TF1::RejectPoint();
      return 0;
   }
   return TMath::Gaus(x[0],par[0],par[1],1)*par[2];
}

Double_t bkgBias(Double_t *x, Double_t *par){
  if (reject && x[0] > -rej && x[0] < rej) {
      TF1::RejectPoint();
      return 0;
   }
   return par[0];
}
//
//----------------------------------------------------------------------
//
int fittp0(const char* hs , float & sigma_res,float & sigma_res_err , float & mean_res,float & mean_res_err, TString Run, TString Reco, TString HV_val, TString RunDir, TString inputDir, bool doubleTFit = 0, const char* fit = "double Student-t fit" ) {
  TCanvas* c = new TCanvas("c","c",0,0,800,800);
  TPad *padRatio;
  TPad *pad;
  gROOT->Reset();
  gStyle->SetOptStat(0000);
  c->SetLeftMargin(0.15);

  bool plotRatio = false;
  Double_t yPadSeparation;
  if(plotRatio){
    yPadSeparation = 0.25;
    pad = new TPad("pad","",0.,yPadSeparation,1.,1.);
    pad->SetBottomMargin(.04);
    padRatio = new TPad("padRatio","",0.,0.,1.,yPadSeparation);
    padRatio->SetTopMargin(0);
    padRatio->SetBottomMargin(.09/yPadSeparation);
    padRatio->Draw();
    pad->Draw();
    padRatio->SetGridx();
    padRatio->SetGridy();
    pad->SetGridx();
    pad->SetGridy();
    // pad->SetLogy(1);
    pad->cd();
  }
  else{
    pad = new TPad("pad","pad",0.15,0.01,0.99,0.99);
    pad->SetBottomMargin(0.18);
    pad->SetLeftMargin(0.15);
    // pad->Draw();
  }
   //pad->cd();
   //c->SetLeftMargin(0.25);
  cout <<" Created pad for hist"<< hs  <<endl;


  TH1 *h = (TH1*)gDirectory->Get(hs);
  if( h == NULL ){

    cout << hs << " does not exist\n";

  }

  else{
    // bool justPlot = true;
    const bool isSystematicUnc    = true;
    const bool isDrawConstituents = true;
    const bool isLogScale         = false;

    Int_t numberOfFits = 5;
    Int_t rebin   = 0.2*h->GetNbinsX()/300.;
    Int_t rebin2  = 6*h->GetNbinsX()/600.;

    TF1 *tp0Fcn ;
    TF1* gauss31;
    TF1* gauss32;
    TF1* gauss33;
    TF1* gauss34;
    TF1* bkg;
    float chiSquare = 0;

    double dx;
    double nmax;
    double xmax;
    double nn;

    int nb;
    double n1;
    double n9;
    double bg;

    double x1;
    double x9;

    TString name =  (TString) hs ;
    TString TrackSelctionText;
    h = (TH1*)gDirectory->Get(hs);
    if(isLogScale){
      c->SetLogy();
      h->SetMaximum(h->GetMaximum()*10);
    }
    dx = h->GetBinWidth(1);
    // cout << "dx = " << dx << endl;
    nmax = h->GetBinContent(h->GetMaximumBin());
    xmax = h->GetBinCenter(h->GetMaximumBin());
    nn = 7*nmax;
    nb = h->GetNbinsX();
    n1 = h->GetBinContent(1);
    n9 = h->GetBinContent(nb);
    bg = 0.5*(n1+n9);
    x1 = h->GetBinCenter(1);
    x9 = h->GetBinCenter(nb);

    cout << hs << ": " << x1 << " - " << x9 << endl;
    bool isPt12GeV;
    if(name.Contains("hg") && name[3] == '1')       isPt12GeV = false ;
    else if(name.Contains("hg") && name[3] == '2')  isPt12GeV = true  ;
    else if(!name.Contains("hg") && name[2] == '1') isPt12GeV = false ;
    else if(!name.Contains("hg") && name[2] == '2') isPt12GeV = true  ;

    bool useSingleStudent = true;
    bool useStudentGaus   = false;
    bool isNotTripleGaus;
    if(isPt12GeV) isNotTripleGaus = name[3] == '1' || name[4] == '1' || Reco == (TString)"Generic";
    else          isNotTripleGaus = (name[3] == '1' && name[4] != '1') || name[4] == '1' || Reco == (TString)"Generic";
    bool isStudentGaus    = !isNotTripleGaus  && useStudentGaus;
    bool isTripleGaus     = !isNotTripleGaus  && !useStudentGaus;
    bool isDG             = !useSingleStudent && isNotTripleGaus;
    bool isSingleStudent  = isNotTripleGaus   && useSingleStudent;
    bool putFitBehind     = false;
    bool isDrawLegend     = true;
    bool is4Gaus          = true;
    bool isApproval       = true;
    TString dir0="file_niceplots";

    isTripleGaus = isTripleGaus && !is4Gaus;
    // if (h->GetNbinsX() == 3000) {
    //   rebin = 3;
    //   rebin2 = 30;
    // }
    // else if (h->GetNbinsX() == 5000){
    //   rebin = 5;
    //   rebin2 = 50;
    // }

    if (!isSingleStudent) h->Rebin(rebin );
    else                  h->Rebin(rebin2);

    dx = h->GetBinWidth(1);
    cout << dx << endl;
    std::vector<std::pair<TString, Double_t>> initValuesOfParameters;
    std::vector<std::pair<TString, std::pair<Double_t, Double_t>>> limitValuesOfParameters;
    std::vector<std::pair<TString, bool>> setLimits;
    if(isSingleStudent){
      tp0Fcn = new TF1("resFitFunc", tp0Fit, x1, x9, 5 );
      initValuesOfParameters = {
        std::make_pair("mean" ,  0.          ),
        std::make_pair("sigma", h->GetRMS()  ),
        std::make_pair("nu"   , 2.2          ),
        std::make_pair("area" , h->Integral()),
        std::make_pair("bkg"  , 0.           )
      };

      //Set the limit values of the parameters
      limitValuesOfParameters = {
        std::make_pair("mean"  , std::make_pair(-10., 10.             )),
        std::make_pair("sigma" , std::make_pair(0.  , 120.            )),
        std::make_pair("nu"    , std::make_pair(0.  , 2000.           )),
        std::make_pair("area"  , std::make_pair(0.  , 10*h->Integral())),
        std::make_pair("bkg"   , std::make_pair(0.  , 1000000.        )),
      };
      setLimits = {
        std::make_pair("mean"  , 0),
        std::make_pair("sigma" , 0),
        std::make_pair("nu"    , 0),
        std::make_pair("area"  , 0),
        std::make_pair("bkg"   , 0)
      };
    }
    else if (isDG || isTripleGaus || is4Gaus){
      tp0Fcn = new TF1("resFitFunc",gaus4, x1, x9, 13);
      // tp0Fcn = new TF1("resFitFunc",gaus3, x1, x9, 8);
      //Initialize the values of the parameters
      initValuesOfParameters = {
        std::make_pair("mean1"  , 0.    ),
        std::make_pair("mean2"  , 0.    ),
        std::make_pair("mean3"  , h->GetMean()    ),
        std::make_pair("mean4"  , h->GetMean()    ),
        std::make_pair("sigma1" , 0.2             ),
        std::make_pair("sigma2" , 0.7             ),
        std::make_pair("sigma3" , 10.              ),
        std::make_pair("sigma4" , 25              ),
        std::make_pair("area1"  , h->Integral()/3.),
        std::make_pair("area2"  , h->Integral()/3.),
        std::make_pair("area3"  , h->Integral()/3.),
        std::make_pair("area4"  , h->Integral()/3.),
        std::make_pair("bkg"    , 10              )
      };
      //Set the limit values of the parameters
      limitValuesOfParameters = {
        std::make_pair("mean1" , std::make_pair(-1., 1.                         )),
        std::make_pair("mean2" , std::make_pair(-1., 1.                         )),
        std::make_pair("mean3" , std::make_pair(h->GetMean()-5.,h->GetMean()+5. )),
        std::make_pair("mean4" , std::make_pair(h->GetMean()-5.,h->GetMean()+5. )),
        std::make_pair("sigma1", std::make_pair(5e-2, 0.5                       )),
        std::make_pair("sigma2", std::make_pair(1e-1, 12.                       )),
        std::make_pair("sigma3", std::make_pair(1., 50.                         )),
        std::make_pair("sigma4", std::make_pair(7., 50.                         )),
        std::make_pair("area1" , std::make_pair(0., h->Integral()               )),
        std::make_pair("area2" , std::make_pair(0., h->Integral()               )),
        std::make_pair("area3" , std::make_pair(0., h->Integral()               )),
        std::make_pair("area4" , std::make_pair(0., h->Integral()               )),
        std::make_pair("bkg"   , std::make_pair(0., 1e6                         ))
      };
      setLimits = {
        std::make_pair("mean1" , 1),
        std::make_pair("mean2" , 1),
        std::make_pair("mean3" , 1),
        std::make_pair("mean4" , 1),
        std::make_pair("sigma1", 1),
        std::make_pair("sigma2", 1),
        std::make_pair("sigma3", 1),
        std::make_pair("sigma4", 1),
        std::make_pair("area1" , 1),
        std::make_pair("area2" , 1),
        std::make_pair("area3" , 1),
        std::make_pair("area4" , 1),
        std::make_pair("bkg"   , 0)
      };
    }

    //Initilaize and set limits of the parameters in TF1
    TString nameOfPar;
    Double_t initValue;
    Double_t lowerLimit;
    Double_t upperLimit;
    Int_t numberOfParams = initValuesOfParameters.size();
    TString fixedParamsString = "";
    for(unsigned int numOfPar = 0; numOfPar < numberOfParams; numOfPar++){
      nameOfPar   = initValuesOfParameters[numOfPar].first;
      initValue   = initValuesOfParameters[numOfPar].second;
      lowerLimit  = limitValuesOfParameters[numOfPar].second.first;
      upperLimit  = limitValuesOfParameters[numOfPar].second.second;
      tp0Fcn->SetParName  (numOfPar, nameOfPar);
      tp0Fcn->SetParameter(numOfPar, initValue);

      if(isTripleGaus){
        tp0Fcn->FixParameter(tp0Fcn->GetParNumber("mean4") , 0.);
        tp0Fcn->FixParameter(tp0Fcn->GetParNumber("sigma4"), 0.);
        tp0Fcn->FixParameter(tp0Fcn->GetParNumber("area4") , 0.);
        fixedParamsString += "mean4 sigma4 area4";
      }
      if(isDG){
        if(nameOfPar.Contains("area3"))  tp0Fcn->FixParameter(numOfPar, 0);
        if(nameOfPar.Contains("sigma3")) tp0Fcn->FixParameter(numOfPar, 1);
        if(nameOfPar.Contains("area4"))  tp0Fcn->FixParameter(numOfPar, 0);
        if(nameOfPar.Contains("sigma4")) tp0Fcn->FixParameter(numOfPar, 1);
        fixedParamsString += "area3 sigma3 sigma4 area4";
      }
      if(setLimits[numOfPar].second){
        tp0Fcn->SetParLimits(numOfPar, limitValuesOfParameters[numOfPar].second.first, limitValuesOfParameters[numOfPar].second.second);
      }
    }

    if(isTripleGaus){
      cout << "================================" << endl;
      cout << "      Triple Gaussian Fit       " << endl;
      cout << "================================" << endl;
    }
    else if(isDG){
      cout << "================================" << endl;
      cout << "      Double Gaussian Fit       " << endl;
      cout << "================================" << endl;
    }
    else if(isSingleStudent){
      cout << "================================" << endl;
      cout << "         Student-t Fit          " << endl;
      cout << "================================" << endl;
    }
    else if(is4Gaus){
      cout << "================================" << endl;
      cout << "       Four gaussians Fit       " << endl;
      cout << "================================" << endl;
    }

    TVirtualFitter::SetMaxIterations(100000);
    for(int numOfFits = 0; numOfFits< numberOfFits-1; numOfFits++) {
      Double_t parmin, parmax;
      h->Fit( "resFitFunc", "RNQ", "ep");
      // for(unsigned int numOfPar = 0; numOfPar < numberOfParams; numOfPar++){
      //   tp0Fcn->GetParLimits(numOfPar, parmin, parmax);
      //   cout << tp0Fcn->GetParName(numOfPar) << " = " << tp0Fcn->GetParameter(numOfPar) << endl;
      //   if(fixedParamsString.Contains(tp0Fcn->GetParName(numOfPar))){
      //     continue;
      //   }
      //   if(tp0Fcn->GetParameter(numOfPar) <= (1.+1e-9)*parmin && setLimits[numOfPar].second){
      //     tp0Fcn->SetParLimits(numOfPar, 0.8*parmin, parmax);
      //   }
      //   else if(tp0Fcn->GetParameter(numOfPar) >= (1.-1e-9)*parmax && setLimits[numOfPar].second){
      //     tp0Fcn->SetParLimits(numOfPar, parmin, 1.2*parmax);
      //   }
      // }
    }
    h->Fit( "resFitFunc", "RN", "ep");
    h->Draw("ep same");

    gStyle->SetOptStat(0000);
    tp0Fcn->SetNpx(10000);
    tp0Fcn->SetLineColor(kGreen);
    tp0Fcn->SetLineWidth(3);
    tp0Fcn->Draw("ep same");
    Double_t integral_sum = 0;
    if(isDrawConstituents && !isSingleStudent){
      if(isStudentGaus) gauss31 = new TF1("f1", tp0Fit, x1, x9, 4);
      else gauss31 = new TF1("f1", Gauss, x1, x9, 3);
      gauss32 = new TF1("f2", Gauss, x1, x9, 3);
      gauss33 = new TF1("f3" , Gauss, x1, x9, 3);
      gauss34 = new TF1("f4" , Gauss, x1, x9, 3);
      bkg     = new TF1("bkg", bkgBias, x1, x9, 1);

      if(isTripleGaus || isDG || is4Gaus){
        gauss31->SetParameter(0,tp0Fcn->GetParameter("mean1"));
        gauss31->SetParameter(1,tp0Fcn->GetParameter("sigma1"));
        gauss31->SetParameter(2,tp0Fcn->GetParameter("area1"));
        cout << "Integral1 = " << gauss31->Integral(-10*gauss31->GetParameter(1),10*gauss31->GetParameter(1)) << " - should be equal to: area1 = " << tp0Fcn->GetParameter("area1") << endl;
        integral_sum += gauss31->Integral(-10*gauss31->GetParameter(1),10*gauss31->GetParameter(1));

        gauss32->SetParameter(0,tp0Fcn->GetParameter("mean2"));
        gauss32->SetParameter(1,tp0Fcn->GetParameter("sigma2"));
        gauss32->SetParameter(2,tp0Fcn->GetParameter("area2"));
        cout << "Integral2 = " << gauss32->Integral(-10*gauss32->GetParameter(1),10*gauss32->GetParameter(1)) << " - should be equal to: area2 = " << tp0Fcn->GetParameter("area2") << endl;
        integral_sum += gauss32->Integral(-10*gauss32->GetParameter(1),10*gauss32->GetParameter(1));

      }
      if(isTripleGaus || is4Gaus){
        gauss33->SetParameter(0,tp0Fcn->GetParameter("mean3"));
        gauss33->SetParameter(1,tp0Fcn->GetParameter("sigma3"));
        gauss33->SetParameter(2,tp0Fcn->GetParameter("area3"));
        cout << "Integral3 = " << gauss33->Integral(-10*gauss33->GetParameter(1),10*gauss33->GetParameter(1)) << " - should be equal to: area3 = " << tp0Fcn->GetParameter("area3") << endl;
        integral_sum += gauss33->Integral(-10*gauss33->GetParameter(1),10*gauss33->GetParameter(1));

      }
      if(is4Gaus){
        gauss34->SetParameter(0,tp0Fcn->GetParameter("mean4"));
        gauss34->SetParameter(1,tp0Fcn->GetParameter("sigma4"));
        gauss34->SetParameter(2,tp0Fcn->GetParameter("area4"));
        cout << "Integral4 = " << gauss34->Integral(-10*gauss34->GetParameter(1),10*gauss34->GetParameter(1)) << " - should be equal to: area4 = " << tp0Fcn->GetParameter("area4") << endl;
        integral_sum += gauss34->Integral(-10*gauss34->GetParameter(1),10*gauss34->GetParameter(1));

      }

      // cout << "Total integral = " << integral_sum + tp0Fcn->GetParameter("bkg")*(h->GetXaxis()->GetXmax()-h->GetXaxis()->GetXmin())<< " should be equal to: " << h->GetEntries()*dx << endl;
      // cout << "DELTA0 = " << tp0Fcn->Integral(h->GetBinLowEdge(h->GetMaximumBin()), h->GetBinLowEdge(h->GetMaximumBin()+1)) << "-" << h->GetMaximum()*dx << "=" << tp0Fcn->Integral(h->GetBinLowEdge(h->GetMaximumBin()), h->GetBinLowEdge(h->GetMaximumBin()+1)) - h->GetMaximum()*dx << endl;
      // cout << "range = " << h->GetXaxis()->GetXmax()-h->GetXaxis()->GetXmin() << endl;
      // cout << "dx = " << dx << endl;
      // cout << "h->GetMaximum() = " << h->GetMaximum() << endl;
      // cout << "f->GetMaximum() = " << tp0Fcn->GetMaximum() << endl;
      bkg->SetParameter(0,tp0Fcn->GetParameter("bkg"));

      gauss31->SetLineWidth(2);
      gauss32->SetLineWidth(2);
      if(isTripleGaus || is4Gaus) gauss33->SetLineWidth(2);
      if(is4Gaus)  gauss34->SetLineWidth(2);

      bkg->SetLineWidth(2);

      gauss31->SetLineColor(kViolet);
      gauss32->SetLineColor(kOrange);
      if(isTripleGaus || is4Gaus ) gauss33->SetLineColor(kBlue);
      if(is4Gaus ) gauss34->SetLineColor(kCyan);

      bkg->SetLineColor(kYellow);

      gauss31->Draw("ep same");
      gauss32->Draw("ep same");
      if(isTripleGaus || is4Gaus) gauss33->Draw("ep same");
      if(is4Gaus)gauss34->Draw("ep same");
      // bkg->Draw("ep same");

      gauss31->SetNpx(10000);
      gauss32->SetNpx(10000);
      if (isTripleGaus || is4Gaus) gauss33->SetNpx(10000);
      if (is4Gaus) gauss34->SetNpx(10000);

      bkg->SetNpx(100);
    }
    Double_t fitArea = 0;
    vector<Double_t> fitAreaConstituents;
    vector<Double_t> sigmaConstituents;
    vector<Double_t> sigmaErrorConstituents;
    vector<Double_t> meanConstituents;
    vector<Double_t> meanErrorConstituents;
    sigma_res     = 0;
    sigma_res_err = 0;
    mean_res      = 0;
    mean_res_err  = 0;

    for(unsigned int numOfPar = 0; numOfPar < numberOfParams; numOfPar++){
      nameOfPar   = initValuesOfParameters[numOfPar].first;
      if(nameOfPar.Contains("mean")){
        meanConstituents.push_back(tp0Fcn->GetParameter(nameOfPar));
        meanErrorConstituents.push_back(tp0Fcn->GetParError(numOfPar));
      }
      if(nameOfPar.Contains("area")){
        fitAreaConstituents.push_back(tp0Fcn->GetParameter(nameOfPar));
        fitArea   += tp0Fcn->GetParameter(nameOfPar);
        // cout << numOfPar << endl;
      }
      if(nameOfPar.Contains("sigma")){
        sigmaConstituents.push_back(tp0Fcn->GetParameter(nameOfPar));
        sigmaErrorConstituents.push_back(tp0Fcn->GetParError(numOfPar));
      }
    }

    for(unsigned int it = 0; it < fitAreaConstituents.size(); it++){
      sigma_res     += sigmaConstituents[it]*fitAreaConstituents[it];
      sigma_res_err += sigmaErrorConstituents[it]*fitAreaConstituents[it]*sigmaErrorConstituents[it]*fitAreaConstituents[it];
      mean_res      += meanConstituents[it]*fitAreaConstituents[it];
      mean_res_err  += meanErrorConstituents[it]*fitAreaConstituents[it]*meanErrorConstituents[it]*fitAreaConstituents[it];
    }
    sigma_res /= fitArea;
    mean_res /= fitArea;
    // cout << "fitArea = " << fitArea << " ~ " <<tp0Fcn->Integral(-150,150) - tp0Fcn->GetParameter("bkg")*abs(x1-x9)<< endl;
    sigma_res_err /= fitArea*fitArea;
    mean_res_err /= fitArea*fitArea;
    if(isSystematicUnc) sigma_res_err += 1.;
    mean_res_err = TMath::Sqrt(mean_res_err);
    // if(isTripleGaus) sigma_res_err += 0.5*0.5;
    sigma_res_err = TMath::Sqrt(sigma_res_err);

    chiSquare = tp0Fcn->GetChisquare()/(h->GetNbinsX()-numberOfParams);

    h->SetMarkerStyle(21);
    h->SetMarkerSize(0.8);
    h->SetTitle("");
    TGaxis::SetMaxDigits(3);

    TLegend *pl2 = new TLegend(0.55,0.54,0.65,0.89);
    pl2->SetTextSize(0.035);
    pl2->SetFillColor(0);
    pl2->SetBorderSize(0);

    if ( name.Contains("h51")   || name.Contains("h52") )  pl2->SetHeader("Barrel Pixel Layer 1");
    if ( name.Contains("f51")   || name.Contains("g51")  || name.Contains("f52")   || name.Contains("g52"))  pl2->SetHeader("Barrel Pixel Layer 4");
    if ( name.Contains("h41")   || name.Contains("hf41") || name.Contains("hf51")  ||
         name.Contains("h42")   || name.Contains("hf42") || name.Contains("hf52"))  pl2->SetHeader("Barrel Pixel Layer 2");
    if ( name.Contains("hg41")  || name.Contains("i51")  || name.Contains("hg42")  || name.Contains("i52"))  pl2->SetHeader("Barrel Pixel Layer 3");

    TLegendEntry *ple2 = pl2->AddEntry(h, "Triplet Residuals",  "P");
    TLegendEntry *plfit;
    std::pair<TString,TString> fitMethodMap[] = {
        std::make_pair("student_t" , "Student-t fit"),
        std::make_pair("doublegaus", "Double gaussian fit"),
        std::make_pair("triplegaus", "Triple gaussian fit"),
        std::make_pair("fourgauss" , "Functional fit"),
    };
    TString fitMethodFileName;
    TString fitMethodLegend;

    if (isSingleStudent)  {
      fitMethodFileName = fitMethodMap[0].first;
      fitMethodLegend   = fitMethodMap[0].second ;
    }
    else if(isDG)         {
      fitMethodFileName = fitMethodMap[1].first;
      fitMethodLegend   = fitMethodMap[1].second ;
    }
    else if(isTripleGaus) {
      fitMethodFileName = fitMethodMap[2].first;
      fitMethodLegend   = fitMethodMap[2].second ;
    }
    else if(is4Gaus)      {
      fitMethodFileName = fitMethodMap[3].first;
      fitMethodLegend   = fitMethodMap[3].second ;
    }

    pl2->AddEntry(tp0Fcn, fitMethodLegend, "L");
    // pl2->SetTextAlign(22);
    TLegendEntry *ple3 = pl2->AddEntry((TObject*)0, "#mu_{r}="+TString::Format("%.2f",mean_res )+"#pm"+ TString::Format("%.2f",mean_res_err )  +" #mum",  "");
    TLegendEntry *ple4 = pl2->AddEntry((TObject*)0, "#sigma_{r}="+TString::Format("%.2f",sigma_res )+"#pm"+ TString::Format("%.2f",sigma_res_err )  +" #mum",  "");
    TLegendEntry *ple6 = pl2->AddEntry((TObject*)0, "mean="+TString::Format("%.2f",h->GetMean())+"#pm"+ TString::Format("%.2f",h->GetMeanError() )  +" #mum",  "");
    TLegendEntry *ple5 = pl2->AddEntry((TObject*)0, "RMS="+TString::Format("%.2f",h->GetRMS())+"#pm"+ TString::Format("%.2f",h->GetRMSError() )  +" #mum",  "");
    ple3->SetTextColor(kRed);
    ple4->SetTextColor(kRed);
    ple5->SetTextColor(kRed);
    ple6->SetTextColor(kRed);
    if(isDrawConstituents && !isSingleStudent){
      if(isDG || isTripleGaus || is4Gaus){
        TLegendEntry *ple7 = pl2->AddEntry((TObject*)0, "#sigma_{r}="+TString::Format("%.2f", tp0Fcn->GetParameter("sigma1") )+"#pm"+ TString::Format("%.2f",tp0Fcn->GetParError(tp0Fcn->GetParNumber("sigma1")) )  +" #mum",  "");
        TLegendEntry *ple8 = pl2->AddEntry((TObject*)0, "#sigma_{r}="+TString::Format("%.2f", tp0Fcn->GetParameter("sigma2") )+"#pm"+ TString::Format("%.2f",tp0Fcn->GetParError(tp0Fcn->GetParNumber("sigma2")) )  +" #mum",  "");
        ple7->SetTextColor(gauss31->GetLineColor());
        ple8->SetTextColor(gauss32->GetLineColor());
      }
      if(isTripleGaus || is4Gaus){
        TLegendEntry *ple9 = pl2->AddEntry((TObject*)0, "#sigma_{r}="+TString::Format("%.2f", tp0Fcn->GetParameter("sigma3") )+"#pm"+ TString::Format("%.2f",tp0Fcn->GetParError(tp0Fcn->GetParNumber("sigma3")) )  +" #mum",  "");
        ple9->SetTextColor(gauss33->GetLineColor());
      }
      if(is4Gaus){
        TLegendEntry *ple10 = pl2->AddEntry((TObject*)0, "#sigma_{r}="+TString::Format("%.2f", tp0Fcn->GetParameter("sigma4") )+"#pm"+ TString::Format("%.2f",tp0Fcn->GetParError(tp0Fcn->GetParNumber("sigma4")) )  +" #mum",  "");
        ple10->SetTextColor(gauss34->GetLineColor());
      }
    }



    // TLegendEntry *ple15 = pl2->AddEntry((TObject*)0, "#chi^{2}/ndof="+TString::Format("%.2f",chiSquare));
    bool drawRun = false;
    if(drawRun) TLegendEntry *ple16 = pl2->AddEntry((TObject*)0, "Run "+Run,  "");
    if(HV_val.IsDec()) TLegendEntry *ple8 = pl2->AddEntry((TObject*)0, HV_val+"V",  "");
    if(isDrawLegend) pl2->Draw("same");
    if (isSingleStudent) h->GetYaxis()->SetTitle("Number of hits / "+ TString::Format("%.2f",dx) +" #mum");
    else                 h->GetYaxis()->SetTitle("Number of hits / "+ TString::Format("%.2f",dx) +" #mum");
    h->GetXaxis()->SetTitle("Residuals r-#phi direction (#mum)");
    if ( name.Contains("11") || name.Contains("21"))  h->GetXaxis()->SetTitle("Residuals z direction (#mum)");

    if(isApproval) {
      h->GetXaxis()->SetTitleSize(0.06);
      h->GetYaxis()->SetTitleSize(0.06);
      h->GetYaxis()->SetTitleOffset(1.15);
      h->GetXaxis()->SetTitleOffset(0.78);
      h->GetXaxis()->SetLabelSize(0.045);
      h->GetYaxis()->SetLabelSize(0.045);
    }
    else {
      h->GetXaxis()->SetTitleSize(0.05);
      h->GetYaxis()->SetTitleSize(0.05);
      h->GetYaxis()->SetTitleOffset(1.4);
      h->GetXaxis()->SetTitleOffset(0.9);
      h->GetXaxis()->SetLabelSize(0.045);
      h->GetYaxis()->SetLabelSize(0.045);
    }

    if(plotRatio){
      padRatio->cd();
      Double_t padSizeRatio = (pad->GetWh()*pad->GetAbsHNDC())/(padRatio->GetWh()*padRatio->GetAbsHNDC());
      // cout << "NOERROR!!!" << padSizeRatio << endl;
      TH1F *ratio = (TH1F*)h->Clone("ratio");
      ratio->Reset();

      ratio->GetYaxis()->SetLabelSize(padSizeRatio*h->GetYaxis()->GetLabelSize());
      ratio->GetXaxis()->SetLabelSize(padSizeRatio*h->GetXaxis()->GetLabelSize());
      ratio->GetYaxis()->SetTitleSize(padSizeRatio*h->GetYaxis()->GetTitleSize());
      ratio->GetXaxis()->SetTitleSize(padSizeRatio*h->GetXaxis()->GetTitleSize());
      ratio->GetYaxis()->SetTitleOffset(1./padSizeRatio*h->GetYaxis()->GetTitleOffset());
      ratio->GetXaxis()->SetTitleOffset(1./h->GetXaxis()->GetTitleOffset());
      ratio->GetYaxis()->SetTitle("Pull");
      ratio->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
      ratio->GetYaxis()->SetNdivisions(805);
      Double_t fx;
      Double_t pull;
      for(int i = 0; i < ratio->GetNbinsX(); i++){
        fx = tp0Fcn->Eval(ratio->GetBinCenter(i));
        pull = (h->GetBinContent(i) - fx)/h->GetBinError(i);
        // cout << "herr(" << i << ") = " << h->GetBinError(i) << endl;
        if(h->GetBinError(i) != 0.) {
          ratio->SetBinContent(i, pull);
          ratio->SetBinError(i, 1);
        }
      }

      ratio->SetMaximum(5);
      ratio->SetMinimum(-5);
      ratio->Draw("e1");
      TF1 *zerolevel = new TF1("zerolevel", bkgBias, x1, x9, 1);
      zerolevel->SetParameter(0,0.);
      zerolevel->SetLineColor(kBlack);
      zerolevel->SetLineWidth(3);
      zerolevel->Draw("ep same");
      // cout << "padRatio = " << padRatio->GetAbsHNDC() << endl;
      pad->cd();
    }

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
    if (h->GetMaximum() <= 1e3) shiftOfPow = -0.1;
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


    latex.SetTextSize(extraTextSize*t*0.93);
    if (isApproval) latex.DrawLatex(l+0.02, 1-t+lumiTextOffset*t-0.09, TrackRecoText);
    else            latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t-0.09-0.06-0.04, TrackRecoText);

    TString TrackSelectionText;
    if(name.Contains("hg") && name[3] == '1')       TrackSelectionText = "Track p_{T} > 4 GeV" ;
    else if(name.Contains("hg") && name[3] == '2')  TrackSelectionText = "Track p_{T} > 12 GeV";
    else if(!name.Contains("hg") && name[2] == '1') TrackSelectionText = "Track p_{T} > 4 GeV" ;
    else if(!name.Contains("hg") && name[2] == '2') TrackSelectionText = "Track p_{T} > 12 GeV";
    latex.SetTextFont(42);
    latex.SetTextSize(extraTextSize*t*0.9);
    if (isApproval) latex.DrawLatex(l+0.02, 1-t+lumiTextOffset*t-0.14, TrackSelectionText);
    else            latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t-0.09-0.06-0.1, TrackSelectionText);

    //  pl1->Draw("same");
    //tp0Fcn->Draw("same");

    // h->Draw("hist ep same"); // data again on top
    // gPad->Print(hs+".png");
    gPad->RedrawAxis("same");
    c->Update();
    h->SetMaximum(h->GetMaximum() *1.5);
    tp0Fcn->Draw("ep same");
    if(putFitBehind) h->Draw("hist ep same");

    TString cmssw = "CMSSW_10_2_0";
    TString name_root =  (TString) hs ;
    TString name_png = (TString) hs ;
    TString name_eps = (TString) hs ;

    if (HV_val!=""){
      name +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+"_"+HV_val+(TString)".pdf" ;
      name_root +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+"_"+HV_val+(TString)".root" ;
      name_png +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+"_"+HV_val+(TString)".png" ;
      name_eps +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+"_"+HV_val+(TString)".eps" ;
    }
    else{
      name +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+(TString)".pdf" ;
      name_root +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+(TString)".root" ;
      name_png +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+(TString)".png" ;
      name_eps +=(TString) "_"+Run+"_"+Reco+"_"+cmssw+(TString)".eps" ;
    }
    if (inputDir!="") dir0=dir0+"_"+inputDir;

    const int dir_err0 = mkdir(dir0.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    TString dir=dir0+"/"+RunDir;
    const int dir_err1 = mkdir(dir.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    dir=dir0+"/"+RunDir+"/"+Reco;
    const int dir_err2 = mkdir(dir.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    c->SaveAs(dir0+"/"+RunDir+"/"+Reco+"/"+name_root);
    c->SaveAs(dir0+"/"+RunDir+"/"+Reco+"/"+name);
    c->SaveAs(dir0+"/"+RunDir+"/"+Reco+"/"+name_png);
    c->SaveAs(dir0+"/"+RunDir+"/"+Reco+"/"+name_eps);
  }
 return 0;

}
int res_test(const char* file ){
  std::map<int,double> runToIntLumi = runToIntLumiInit ;
  TString pt = "12 GeV";

  std::ofstream outTable;
  TString nameOfFileOutTable = (TString)"OutTable.txt";
  outTable.open(nameOfFileOutTable.Data(), std::ios_base::app);

  std::ofstream outTable1;
  TString nameOfFileOutTable1 = (TString)"OutTableL12.txt";
  outTable1.open(nameOfFileOutTable1.Data(), std::ios_base::app);

  std::ofstream outTable2;
  TString nameOfFileOutTable2 = (TString)"OutTableL34.txt";
  outTable2.open(nameOfFileOutTable2.Data(), std::ios_base::app);

  TFile *_file0 = new TFile(file);
  //TString directory = "Histos/Run";
  TString Run = "";
  TString RunDir = "";
  TString inputDir = "";
  TString HV_val;
  TString Reco;
  TString layerHV;
  int bias_index=11;
  Ssiz_t ind = 0;
  while(((TString)file).Index("/Resolution", ind+1) > 0) ind = ((TString)file).Index("/Resolution",ind+1);
  if(ind != 0) ind++;
  bias_index += ind;
  for(int i = 0; i < 6; i++) Run+=(TString)file[bias_index+i];
  for(int i = 0; i < ((TString)file).Length()-5-bias_index; i++) RunDir+=(TString)file[bias_index+i];
  for(int i = 0; i < ((TString)file).Index("/", 1); i++) inputDir+=(TString)file[i];
  cout << Run << endl;
  cout << file << endl;
  cout << RunDir << endl;
  cout << inputDir << endl;
  // Run = (TString)file[bias_index+0]+(TString)file[bias_index+1]+(TString)file[bias_index+2]+(TString)file[bias_index+3]+(TString)file[bias_index+4]+(TString)file[bias_index+5];
  if(!Run.IsDec()) Run="1";
  if ((TString)file[bias_index+7] == "L"){
    layerHV = (TString)file[bias_index+7]+(TString)file[bias_index+8];
    // cout << layerHV << endl;
    HV_val = (TString)file[bias_index+10]+(TString)file[bias_index+11]+(TString)file[bias_index+12];
    if((TString)file[bias_index+12] == (TString)"V") HV_val = (TString)file[bias_index+10]+(TString)file[bias_index+11];
    if((TString)file[bias_index+12] == (TString)"_") HV_val = (TString)file[bias_index+10];


    for(int is_template_reco = 1; is_template_reco < 2; is_template_reco++){
      if(is_template_reco == 0)      Reco = "Generic";
      else if(is_template_reco == 1) Reco = "Template";
      TString directory = "BPixResolution_"+Reco+"/Run";

      _file0->cd(directory+Run);


      //HV_val = (TString)"150";
      //Get resolution along x
      if (pt == "12 GeV"){
        if (layerHV == (TString)"L1"){
          TH1F*  h520= (TH1F*) _file0->Get(directory+Run+(TString)"/h520");
          h520->SetDirectory(0); // "detach" the histogram from the file
          float sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231 ;
          fittp0("h520",sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",h520->GetRMS())  << "#pm" << TString::Format("%.2f",h520->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_x_l231) << "#pm" << TString::Format("%.2f",sigma_x_err_l231) << " & ";

          //Get resolution along y
          TH1F*  h521= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h521");
          h521->SetDirectory(0); // "detach" the histogram from the file
          float sigma_y_l231,sigma_y_err_l231, mean_y_l231, mean_y_err_l231 ;
          fittp0("h521", sigma_y_l231, sigma_y_err_l231, mean_y_l231, mean_y_err_l231, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",h521->GetRMS())  << "#pm" << TString::Format("%.2f",h521->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_y_l231) << "#pm" << TString::Format("%.2f",sigma_y_err_l231) << " & ";

          //Get resolution along x
          TH1F*  h520_in= (TH1F*) _file0->Get(directory+Run+(TString)"/h520_in");
          h520_in->SetDirectory(0); // "detach" the histogram from the file
          float sigma_x_l231_in,sigma_x_err_l231_in,mean_x_l231_in,mean_x_err_l231_in ;
          fittp0("h520_in",sigma_x_l231_in,sigma_x_err_l231_in,mean_x_l231_in,mean_x_err_l231_in, Run, Reco, HV_val, RunDir, inputDir);

          //Get resolution along y
          TH1F*  h521_in= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h521_in");
          h521_in->SetDirectory(0); // "detach" the histogram from the file
          float sigma_y_l231_in,sigma_y_err_l231_in, mean_y_l231_in, mean_y_err_l231_in ;
          fittp0("h521_in", sigma_y_l231_in, sigma_y_err_l231_in, mean_y_l231_in, mean_y_err_l231_in, Run, Reco, HV_val, RunDir, inputDir);

          //Get resolution along x
          TH1F*  h520_out= (TH1F*) _file0->Get(directory+Run+(TString)"/h520_out");
          h520_out->SetDirectory(0); // "detach" the histogram from the file
          float sigma_x_l231_out,sigma_x_err_l231_out,mean_x_l231_out,mean_x_err_l231_out ;
          fittp0("h520_out",sigma_x_l231_out,sigma_x_err_l231_out,mean_x_l231_out,mean_x_err_l231_out, Run, Reco, HV_val, RunDir, inputDir);

          //Get resolution along y
          TH1F*  h521_out= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h521_out");
          h521_out->SetDirectory(0); // "detach" the histogram from the file
          float sigma_y_l231_out,sigma_y_err_l231_out, mean_y_l231_out, mean_y_err_l231_out ;
          fittp0("h521_out", sigma_y_l231_out, sigma_y_err_l231_out, mean_y_l231_out, mean_y_err_l231_out, Run, Reco, HV_val, RunDir, inputDir);

        }

        if (layerHV == (TString)"L2"){
          //Get resolution along x
          TH1F*  h420= (TH1F*) _file0->Get(directory+Run+(TString)"/h420");
          h420->SetDirectory(0); // "detach" the histogram from the file
          float sigma_x,sigma_x_err,mean_x,mean_x_err ;
          fittp0("h420",sigma_x,sigma_x_err,mean_x,mean_x_err, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",h420->GetRMS())  << "#pm" << TString::Format("%.2f",h420->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_x) << "#pm" << TString::Format("%.2f",sigma_x_err) << " & ";

          //Get resolution along y
          TH1F*  h421= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h421");
          h421->SetDirectory(0); // "detach" the histogram from the file
          float sigma_y,sigma_y_err, mean_y, mean_y_err ;
          fittp0("h421", sigma_y, sigma_y_err, mean_y, mean_y_err, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",h421->GetRMS())  << "#pm" << TString::Format("%.2f",h421->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_y) << "#pm" << TString::Format("%.2f",sigma_y_err) << " & ";
        }

        if (layerHV == (TString)"L3"){
          //Get resolution along x
          TH1F*  hg420= (TH1F*) _file0->Get(directory+Run+(TString)"/hg420");
          hg420->SetDirectory(0); // "detach" the histogram from the file
          float sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243 ;
          fittp0("hg420",sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",hg420->GetRMS())  << "#pm" << TString::Format("%.2f",hg420->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_x_l243) << "#pm" << TString::Format("%.2f",sigma_x_err_l243) << " & ";

          //Get resolution along y
          TH1F*  hg421= (TH1F*) _file0 ->Get(directory+Run+(TString)"/hg421");
          hg421->SetDirectory(0); // "detach" the histogram from the file
          float sigma_y_l243,sigma_y_err_l243, mean_y_l243, mean_y_err_l243 ;
          fittp0("hg421", sigma_y_l243, sigma_y_err_l243, mean_y_l243, mean_y_err_l243, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",hg421->GetRMS())  << "#pm" << TString::Format("%.2f",hg421->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_y_l243) << "#pm" << TString::Format("%.2f",sigma_y_err_l243) << " & ";
        }

        if (layerHV == (TString)"L4"){
          //Get resolution along x
          TH1F*  g520= (TH1F*) _file0->Get(directory+Run+(TString)"/g520");
          g520->SetDirectory(0); // "detach" the histogram from the file
          float sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234 ;
          fittp0("g520",sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",g520->GetRMS())  << "#pm" << TString::Format("%.2f",g520->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_x_l234) << "#pm" << TString::Format("%.2f",sigma_x_err_l234) << " & ";

          //Get resolution along y
          TH1F*  g521= (TH1F*) _file0 ->Get(directory+Run+(TString)"/g521");
          g521->SetDirectory(0); // "detach" the histogram from the file
          float sigma_y_l234,sigma_y_err_l234, mean_y_l234, mean_y_err_l234 ;
          fittp0("g521", sigma_y_l234, sigma_y_err_l234, mean_y_l234, mean_y_err_l234, Run, Reco, HV_val, RunDir, inputDir);
          outTable  << TString::Format("%.2f",g521->GetRMS())  << "#pm" << TString::Format("%.2f",g521->GetRMSError()) << " & "
                    << TString::Format("%.2f",sigma_y_l234) << "#pm" << TString::Format("%.2f",sigma_y_err_l234) << " || " << endl;
        }
        for(int i = 0; i < (2+1+2+1+1+1+2+1+2+1+2+1)*4-1; i++) outTable << "-";
        outTable << endl;
      }
      if (pt == "4 GeV"){
        if (layerHV == (TString)"L1"){
          cout << "isMiniBiasScan = " << isMiniBiasScan<< endl;

          if (!isMiniBiasScan){
            TH1F*  h510= (TH1F*) _file0->Get(directory+Run+(TString)"/h510");
            h510->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231 ;
            fittp0("h510",sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h511= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h511");
            h511->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231,sigma_y_err_l231, mean_y_l231, mean_y_err_l231 ;
            fittp0("h511", sigma_y_l231, sigma_y_err_l231, mean_y_l231, mean_y_err_l231, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along x
            TH1F*  h510_in= (TH1F*) _file0->Get(directory+Run+(TString)"/h520_in");
            h510_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_in,sigma_x_err_l231_in,mean_x_l231_in,mean_x_err_l231_in ;
            fittp0("h510_in",sigma_x_l231_in,sigma_x_err_l231_in,mean_x_l231_in,mean_x_err_l231_in, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h511_in= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h511_in");
            h511_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_in,sigma_y_err_l231_in, mean_y_l231_in, mean_y_err_l231_in ;
            fittp0("h511_in", sigma_y_l231_in, sigma_y_err_l231_in, mean_y_l231_in, mean_y_err_l231_in, Run, Reco, HV_val, RunDir, inputDir);

            TH1F*  h510_out= (TH1F*) _file0->Get(directory+Run+(TString)"/h510_out");
            h510_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_out,sigma_x_err_l231_out,mean_x_l231_out,mean_x_err_l231_out ;
            fittp0("h510_out",sigma_x_l231_out,sigma_x_err_l231_out,mean_x_l231_out,mean_x_err_l231_out, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h511_out= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h511_out");
            h511_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_out,sigma_y_err_l231_out, mean_y_l231_out, mean_y_err_l231_out ;
            fittp0("h511_out", sigma_y_l231_out, sigma_y_err_l231_out, mean_y_l231_out, mean_y_err_l231_out, Run, Reco, HV_val, RunDir, inputDir);

          }
          else{
            TH1F*  h510_bmo2= (TH1F*) _file0->Get(directory+Run+(TString)"/h510_bmo2");
            h510_bmo2->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_bmo2,sigma_x_err_l231_bmo2,mean_x_l231_bmo2,mean_x_err_l231_bmo2 ;
            fittp0("h510_bmo2",sigma_x_l231_bmo2,sigma_x_err_l231_bmo2,mean_x_l231_bmo2,mean_x_err_l231_bmo2, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h511_bmo2= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h511_bmo2");
            h511_bmo2->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_bmo2,sigma_y_err_l231_bmo2, mean_y_l231_bmo2, mean_y_err_l231_bmo2 ;
            fittp0("h511_bmo2", sigma_y_l231_bmo2, sigma_y_err_l231_bmo2, mean_y_l231_bmo2, mean_y_err_l231_bmo2, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along x
            TH1F*  h510_bmo2_in= (TH1F*) _file0->Get(directory+Run+(TString)"/h510_bmo2_in");
            h510_bmo2_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_in_bmo2,sigma_x_err_l231_in_bmo2,mean_x_l231_in_bmo2,mean_x_err_l231_in_bmo2 ;
            fittp0("h510_bmo2_in",sigma_x_l231_in_bmo2,sigma_x_err_l231_in_bmo2,mean_x_l231_in_bmo2,mean_x_err_l231_in_bmo2, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h511_bmo2_in= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h511_bmo2_in");
            h511_bmo2_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_in_bmo2,sigma_y_err_l231_in_bmo2, mean_y_l231_in_bmo2, mean_y_err_l231_in_bmo2 ;
            fittp0("h511_bmo2_in", sigma_y_l231_in_bmo2, sigma_y_err_l231_in_bmo2, mean_y_l231_in_bmo2, mean_y_err_l231_in_bmo2, Run, Reco, HV_val, RunDir, inputDir);

            TH1F*  h510_bmo2_out= (TH1F*) _file0->Get(directory+Run+(TString)"/h510_bmo2_out");
            h510_bmo2_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_out_bmo2,sigma_x_err_l231_out_bmo2,mean_x_l231_out_bmo2,mean_x_err_l231_out_bmo2 ;
            fittp0("h510_bmo2_out",sigma_x_l231_out_bmo2,sigma_x_err_l231_out_bmo2,mean_x_l231_out_bmo2,mean_x_err_l231_out_bmo2, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h511_bmo2_out= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h511_bmo2_out");
            h511_bmo2_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_out_bmo2,sigma_y_err_l231_out_bmo2, mean_y_l231_out_bmo2, mean_y_err_l231_out_bmo2 ;
            fittp0("h511_bmo2_out", sigma_y_l231_out_bmo2, sigma_y_err_l231_out_bmo2, mean_y_l231_out_bmo2, mean_y_err_l231_out_bmo2, Run, Reco, HV_val, RunDir, inputDir);

          }

        }

        if (layerHV == (TString)"L2"){
          //Get resolution along x
          if(!isMiniBiasScan){
            TH1F*  h410= (TH1F*) _file0->Get(directory+Run+(TString)"/h410");
            h410->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x,sigma_x_err,mean_x,mean_x_err ;
            fittp0("h410",sigma_x,sigma_x_err,mean_x,mean_x_err, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h411= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h411");
            h411->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y,sigma_y_err, mean_y, mean_y_err ;
            fittp0("h411", sigma_y, sigma_y_err, mean_y, mean_y_err, Run, Reco, HV_val, RunDir, inputDir);
          }
          else{
            TH1F*  h410_bpi6= (TH1F*) _file0->Get(directory+Run+(TString)"/h410_bpi6");
            // cout << h410_bpi6->GetNbinsX() << endl;
            // cout << "Line 927(926: 'cout << h410_bpi6->GetNbinsX() << endl;' - additional line): check of the histogram h411_bpi6: Done!" << endl;
            h410_bpi6->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_bpi6,sigma_x_err_l231_bpi6,mean_x_l231_bpi6,mean_x_err_l231_bpi6 ;
            fittp0("h410_bpi6",sigma_x_l231_bpi6,sigma_x_err_l231_bpi6,mean_x_l231_bpi6,mean_x_err_l231_bpi6, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h411_bpi6= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h411_bpi6");
            // cout << h411_bpi6->GetNbinsX() << endl;
            // cout << "Line 935(934: 'cout << h411_bpi6->GetNbinsX() << endl;' - additional line): check of the histogram h411_bpi6: Done!" << endl;
            h411_bpi6->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_bpi6,sigma_y_err_l231_bpi6, mean_y_l231_bpi6, mean_y_err_l231_bpi6 ;
            fittp0("h411_bpi6", sigma_y_l231_bpi6, sigma_y_err_l231_bpi6, mean_y_l231_bpi6, mean_y_err_l231_bpi6, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along x
            TH1F*  h410_bpi6_in= (TH1F*) _file0->Get(directory+Run+(TString)"/h410_bpi6_in");
            h410_bpi6_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_in_bpi6,sigma_x_err_l231_in_bpi6,mean_x_l231_in_bpi6,mean_x_err_l231_in_bpi6 ;
            fittp0("h410_bpi6_in",sigma_x_l231_in_bpi6,sigma_x_err_l231_in_bpi6,mean_x_l231_in_bpi6,mean_x_err_l231_in_bpi6, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h411_bpi6_in= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h411_bpi6_in");
            h411_bpi6_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_in_bpi6,sigma_y_err_l231_in_bpi6, mean_y_l231_in_bpi6, mean_y_err_l231_in_bpi6 ;
            fittp0("h411_bpi6_in", sigma_y_l231_in_bpi6, sigma_y_err_l231_in_bpi6, mean_y_l231_in_bpi6, mean_y_err_l231_in_bpi6, Run, Reco, HV_val, RunDir, inputDir);

            TH1F*  h410_bpi6_out= (TH1F*) _file0->Get(directory+Run+(TString)"/h410_bpi6_out");
            h410_bpi6_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_out_bpi6,sigma_x_err_l231_out_bpi6,mean_x_l231_out_bpi6,mean_x_err_l231_out_bpi6 ;
            fittp0("h410_bpi6_out",sigma_x_l231_out_bpi6,sigma_x_err_l231_out_bpi6,mean_x_l231_out_bpi6,mean_x_err_l231_out_bpi6, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  h411_bpi6_out= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h411_bpi6_out");
            h411_bpi6_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_out_bpi6,sigma_y_err_l231_out_bpi6, mean_y_l231_out_bpi6, mean_y_err_l231_out_bpi6 ;
            fittp0("h411_bpi6_out", sigma_y_l231_out_bpi6, sigma_y_err_l231_out_bpi6, mean_y_l231_out_bpi6, mean_y_err_l231_out_bpi6, Run, Reco, HV_val, RunDir, inputDir);

          }
        }

        if (layerHV == (TString)"L3"){
          //Get resolution along x
          if(!isMiniBiasScan){
            TH1F*  hg410= (TH1F*) _file0->Get(directory+Run+(TString)"/hg410");
            hg410->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243 ;
            fittp0("hg410",sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  hg411= (TH1F*) _file0 ->Get(directory+Run+(TString)"/hg411");
            hg411->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l243,sigma_y_err_l243, mean_y_l243, mean_y_err_l243 ;
            fittp0("hg411", sigma_y_l243, sigma_y_err_l243, mean_y_l243, mean_y_err_l243, Run, Reco, HV_val, RunDir, inputDir);
          }
          else{
            TH1F*  hg410_bpo3= (TH1F*) _file0->Get(directory+Run+(TString)"/hg410_bpo3");
            hg410_bpo3->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_bpo3,sigma_x_err_l231_bpo3,mean_x_l231_bpo3,mean_x_err_l231_bpo3 ;
            fittp0("hg410_bpo3",sigma_x_l231_bpo3,sigma_x_err_l231_bpo3,mean_x_l231_bpo3,mean_x_err_l231_bpo3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  hg411_bpo3= (TH1F*) _file0 ->Get(directory+Run+(TString)"/hg411_bpo3");
            hg411_bpo3->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_bpo3,sigma_y_err_l231_bpo3, mean_y_l231_bpo3, mean_y_err_l231_bpo3 ;
            fittp0("hg411_bpo3", sigma_y_l231_bpo3, sigma_y_err_l231_bpo3, mean_y_l231_bpo3, mean_y_err_l231_bpo3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along x
            TH1F*  hg410_bpo3_in= (TH1F*) _file0->Get(directory+Run+(TString)"/hg410_bpo3_in");
            hg410_bpo3_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_in_bpo3,sigma_x_err_l231_in_bpo3,mean_x_l231_in_bpo3,mean_x_err_l231_in_bpo3 ;
            fittp0("hg410_bpo3_in",sigma_x_l231_in_bpo3,sigma_x_err_l231_in_bpo3,mean_x_l231_in_bpo3,mean_x_err_l231_in_bpo3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  hg411_bpo3_in= (TH1F*) _file0 ->Get(directory+Run+(TString)"/hg411_bpo3_in");
            hg411_bpo3_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_in_bpo3,sigma_y_err_l231_in_bpo3, mean_y_l231_in_bpo3, mean_y_err_l231_in_bpo3 ;
            fittp0("hg411_bpo3_in", sigma_y_l231_in_bpo3, sigma_y_err_l231_in_bpo3, mean_y_l231_in_bpo3, mean_y_err_l231_in_bpo3, Run, Reco, HV_val, RunDir, inputDir);

            TH1F*  hg410_bpo3_out= (TH1F*) _file0->Get(directory+Run+(TString)"/hg410_bpo3_out");
            hg410_bpo3_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_out_bpo3,sigma_x_err_l231_out_bpo3,mean_x_l231_out_bpo3,mean_x_err_l231_out_bpo3 ;
            fittp0("hg410_bpo3_out",sigma_x_l231_out_bpo3,sigma_x_err_l231_out_bpo3,mean_x_l231_out_bpo3,mean_x_err_l231_out_bpo3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  hg411_bpo3_out= (TH1F*) _file0 ->Get(directory+Run+(TString)"/hg411_bpo3_out");
            hg411_bpo3_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_out_bpo3,sigma_y_err_l231_out_bpo3, mean_y_l231_out_bpo3, mean_y_err_l231_out_bpo3 ;
            fittp0("hg411_bpo3_out", sigma_y_l231_out_bpo3, sigma_y_err_l231_out_bpo3, mean_y_l231_out_bpo3, mean_y_err_l231_out_bpo3, Run, Reco, HV_val, RunDir, inputDir);

          }
        }

        if (layerHV == (TString)"L4"){
          //Get resolution along x
          if(!isMiniBiasScan){
            TH1F*  g510= (TH1F*) _file0->Get(directory+Run+(TString)"/g510");
            g510->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234 ;
            fittp0("g510",sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  g511= (TH1F*) _file0 ->Get(directory+Run+(TString)"/g511");
            g511->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l234,sigma_y_err_l234, mean_y_l234, mean_y_err_l234 ;
            fittp0("g511", sigma_y_l234, sigma_y_err_l234, mean_y_l234, mean_y_err_l234, Run, Reco, HV_val, RunDir, inputDir);
          }
          else{
            TH1F*  g510_bmi3= (TH1F*) _file0->Get(directory+Run+(TString)"/g510_bmi3");
            g510_bmi3->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_bmi3,sigma_x_err_l231_bmi3,mean_x_l231_bmi3,mean_x_err_l231_bmi3 ;
            fittp0("g510_bmi3",sigma_x_l231_bmi3,sigma_x_err_l231_bmi3,mean_x_l231_bmi3,mean_x_err_l231_bmi3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  g511_bmi3= (TH1F*) _file0 ->Get(directory+Run+(TString)"/g511_bmi3");
            g511_bmi3->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_bmi3,sigma_y_err_l231_bmi3, mean_y_l231_bmi3, mean_y_err_l231_bmi3 ;
            fittp0("g511_bmi3", sigma_y_l231_bmi3, sigma_y_err_l231_bmi3, mean_y_l231_bmi3, mean_y_err_l231_bmi3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along x
            TH1F*  g510_bmi3_in= (TH1F*) _file0->Get(directory+Run+(TString)"/g510_bmi3_in");
            g510_bmi3_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_in_bmi3,sigma_x_err_l231_in_bmi3,mean_x_l231_in_bmi3,mean_x_err_l231_in_bmi3 ;
            fittp0("g510_bmi3_in",sigma_x_l231_in_bmi3,sigma_x_err_l231_in_bmi3,mean_x_l231_in_bmi3,mean_x_err_l231_in_bmi3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  g511_bmi3_in= (TH1F*) _file0 ->Get(directory+Run+(TString)"/g511_bmi3_in");
            g511_bmi3_in->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_in_bmi3,sigma_y_err_l231_in_bmi3, mean_y_l231_in_bmi3, mean_y_err_l231_in_bmi3 ;
            fittp0("g511_bmi3_in", sigma_y_l231_in_bmi3, sigma_y_err_l231_in_bmi3, mean_y_l231_in_bmi3, mean_y_err_l231_in_bmi3, Run, Reco, HV_val, RunDir, inputDir);

            TH1F*  g510_bmi3_out= (TH1F*) _file0->Get(directory+Run+(TString)"/g510_bmi3_out");
            g510_bmi3_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_x_l231_out_bmi3,sigma_x_err_l231_out_bmi3,mean_x_l231_out_bmi3,mean_x_err_l231_out_bmi3 ;
            fittp0("g510_bmi3_out",sigma_x_l231_out_bmi3,sigma_x_err_l231_out_bmi3,mean_x_l231_out_bmi3,mean_x_err_l231_out_bmi3, Run, Reco, HV_val, RunDir, inputDir);

            //Get resolution along y
            TH1F*  g511_bmi3_out= (TH1F*) _file0 ->Get(directory+Run+(TString)"/g511_bmi3_out");
            g511_bmi3_out->SetDirectory(0); // "detach" the histogram from the file
            float sigma_y_l231_out_bmi3,sigma_y_err_l231_out_bmi3, mean_y_l231_out_bmi3, mean_y_err_l231_out_bmi3 ;
            fittp0("g511_bmi3_out", sigma_y_l231_out_bmi3, sigma_y_err_l231_out_bmi3, mean_y_l231_out_bmi3, mean_y_err_l231_out_bmi3, Run, Reco, HV_val, RunDir, inputDir);

          }
        }
      }
    }
  }
  else{
    //Run="1";

    std::ofstream RMS_generic_x_l1_file;
    std::ofstream RMS_generic_y_l1_file;
    std::ofstream RMS_template_x_l1_file;
    std::ofstream RMS_template_y_l1_file;
    std::ofstream sigma_generic_x_l1_file;
    std::ofstream sigma_generic_y_l1_file;
    std::ofstream sigma_template_x_l1_file;
    std::ofstream sigma_template_y_l1_file;
    std::ofstream RMS_generic_x_l2_file;
    std::ofstream RMS_generic_y_l2_file;
    std::ofstream RMS_template_x_l2_file;
    std::ofstream RMS_template_y_l2_file;
    std::ofstream sigma_generic_x_l2_file;
    std::ofstream sigma_generic_y_l2_file;
    std::ofstream sigma_template_x_l2_file;
    std::ofstream sigma_template_y_l2_file;
    std::ofstream RMS_generic_x_l3_file;
    std::ofstream RMS_generic_y_l3_file;
    std::ofstream RMS_template_x_l3_file;
    std::ofstream RMS_template_y_l3_file;
    std::ofstream sigma_generic_x_l3_file;
    std::ofstream sigma_generic_y_l3_file;
    std::ofstream sigma_template_x_l3_file;
    std::ofstream sigma_template_y_l3_file;
    std::ofstream RMS_generic_x_l4_file;
    std::ofstream RMS_generic_y_l4_file;
    std::ofstream RMS_template_x_l4_file;
    std::ofstream RMS_template_y_l4_file;
    std::ofstream sigma_generic_x_l4_file;
    std::ofstream sigma_generic_y_l4_file;
    std::ofstream sigma_template_x_l4_file;
    std::ofstream sigma_template_y_l4_file;

    for(int is_template_reco = 1; is_template_reco < 2; is_template_reco++){
      if(is_template_reco == 0)      Reco = "Generic";
      else if(is_template_reco == 1) Reco = "Template";
      TString directory = "BPixResolution_"+Reco+"/Run";
      layerHV = "";
      HV_val = "";
      // HV_val = "ClusterRew_reweight_v1";
      // HV_val = "ClusterRew_v1";
      // HV_val = "DynIneff_v1";
      // HV_val = "DynIneff_v2";
      // HV_val = "MC_ref";
      // HV_val = "ClusterRew_reweight_end2017";
      _file0->cd(directory+Run);

      RMS_generic_x_l1_file.open("RMS_generic_x_l1.dat", std::ios_base::app);
      RMS_generic_y_l1_file.open("RMS_generic_y_l1.dat", std::ios_base::app);
      RMS_template_x_l1_file.open("RMS_template_x_l1.dat", std::ios_base::app);
      RMS_template_y_l1_file.open("RMS_template_y_l1.dat", std::ios_base::app);
      sigma_generic_x_l1_file.open("sigma_generic_x_l1.dat", std::ios_base::app);
      sigma_generic_y_l1_file.open("sigma_generic_y_l1.dat", std::ios_base::app);
      sigma_template_x_l1_file.open("sigma_template_x_l1.dat", std::ios_base::app);
      sigma_template_y_l1_file.open("sigma_template_y_l1.dat", std::ios_base::app);

      RMS_generic_x_l2_file.open("RMS_generic_x_l2.dat", std::ios_base::app);
      RMS_generic_y_l2_file.open("RMS_generic_y_l2.dat", std::ios_base::app);
      RMS_template_x_l2_file.open("RMS_template_x_l2.dat", std::ios_base::app);
      RMS_template_y_l2_file.open("RMS_template_y_l2.dat", std::ios_base::app);
      sigma_generic_x_l2_file.open("sigma_generic_x_l2.dat", std::ios_base::app);
      sigma_generic_y_l2_file.open("sigma_generic_y_l2.dat", std::ios_base::app);
      sigma_template_x_l2_file.open("sigma_template_x_l2.dat", std::ios_base::app);
      sigma_template_y_l2_file.open("sigma_template_y_l2.dat", std::ios_base::app);

      RMS_generic_x_l3_file.open("RMS_generic_x_l3.dat", std::ios_base::app);
      RMS_generic_y_l3_file.open("RMS_generic_y_l3.dat", std::ios_base::app);
      RMS_template_x_l3_file.open("RMS_template_x_l3.dat", std::ios_base::app);
      RMS_template_y_l3_file.open("RMS_template_y_l3.dat", std::ios_base::app);
      sigma_generic_x_l3_file.open("sigma_generic_x_l3.dat", std::ios_base::app);
      sigma_generic_y_l3_file.open("sigma_generic_y_l3.dat", std::ios_base::app);
      sigma_template_x_l3_file.open("sigma_template_x_l3.dat", std::ios_base::app);
      sigma_template_y_l3_file.open("sigma_template_y_l3.dat", std::ios_base::app);

      RMS_generic_x_l4_file.open("RMS_generic_x_l4.dat", std::ios_base::app);
      RMS_generic_y_l4_file.open("RMS_generic_y_l4.dat", std::ios_base::app);
      RMS_template_x_l4_file.open("RMS_template_x_l4.dat", std::ios_base::app);
      RMS_template_y_l4_file.open("RMS_template_y_l4.dat", std::ios_base::app);
      sigma_generic_x_l4_file.open("sigma_generic_x_l4.dat", std::ios_base::app);
      sigma_generic_y_l4_file.open("sigma_generic_y_l4.dat", std::ios_base::app);
      sigma_template_x_l4_file.open("sigma_template_x_l4.dat", std::ios_base::app);
      sigma_template_y_l4_file.open("sigma_template_y_l4.dat", std::ios_base::app);

      if (pt == "12 GeV"){
        outTable << "\\verb|" <<RunDir << "| & ";
        outTable1 << "\\verb|" <<RunDir << "| & ";
        outTable2 << "\\verb|" <<RunDir << "| & ";
        TH1F*  h520= (TH1F*) _file0->Get(directory+Run+(TString)"/h520");
        h520->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231 ;
        fittp0("h520",sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231, Run, Reco, HV_val, RunDir, inputDir);
        outTable1  << TString::Format("%.2f",h520->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h520->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x_l231) << "$\\pm$" << TString::Format("%.2f",sigma_x_err_l231) << " & ";
        outTable  << TString::Format("%.2f",h520->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h520->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x_l231) << "$\\pm$" << TString::Format("%.2f",sigma_x_err_l231) << " & ";
        //
        // //
        //Get resolution along y
        TH1F*  h521= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h521");
        h521->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y_l231,sigma_y_err_l231, mean_y_l231, mean_y_err_l231 ;
        fittp0("h521", sigma_y_l231, sigma_y_err_l231, mean_y_l231, mean_y_err_l231, Run, Reco, HV_val, RunDir, inputDir);
        outTable1  << TString::Format("%.2f",h521->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h521->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y_l231) << "$\\pm$" << TString::Format("%.2f",sigma_y_err_l231) << " & ";
        outTable  << TString::Format("%.2f",h521->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h521->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y_l231) << "$\\pm$" << TString::Format("%.2f",sigma_y_err_l231) << " & ";

        // if (is_template_reco == 0){
        //   RMS_generic_x_l1_file     << runToIntLumi[Run.Atoi()] << "\t" << h520->GetRMS() << "\t" << 0.0              << std::endl;
        //   RMS_generic_y_l1_file     << runToIntLumi[Run.Atoi()] << "\t" << h521->GetRMS() << "\t" << 0.0              << std::endl;
        //   sigma_generic_x_l1_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_x_l231   << "\t" << sigma_x_err_l231 << std::endl;
        //   sigma_generic_y_l1_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_y_l231   << "\t" << sigma_y_err_l231 << std::endl;
        // }
        // else{
        //   RMS_template_x_l1_file    << runToIntLumi[Run.Atoi()] << "\t" << h520->GetRMS() << "\t" << 0.0              << std::endl;
        //   RMS_template_y_l1_file    << runToIntLumi[Run.Atoi()] << "\t" << h521->GetRMS() << "\t" << 0.0              << std::endl;
        //   sigma_template_x_l1_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_x_l231   << "\t" << sigma_x_err_l231 << std::endl;
        //   sigma_template_y_l1_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_y_l231   << "\t" << sigma_y_err_l231 << std::endl;
        // }
        //
        // // Get resolution along x
        TH1F*  h420= (TH1F*) _file0->Get(directory+Run+(TString)"/h420");
        h420->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x,sigma_x_err,mean_x,mean_x_err ;
        fittp0("h420",sigma_x,sigma_x_err,mean_x,mean_x_err, Run, Reco, HV_val, RunDir, inputDir);
        outTable1  << TString::Format("%.2f",h420->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h420->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x) << "$\\pm$" << TString::Format("%.2f",sigma_x_err) << " & ";
        outTable  << TString::Format("%.2f",h420->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h420->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x) << "$\\pm$" << TString::Format("%.2f",sigma_x_err) << " & ";
        //
        //Get resolution along y
        TH1F*  h421= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h421");
        h421->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y,sigma_y_err, mean_y, mean_y_err ;
        fittp0("h421", sigma_y, sigma_y_err, mean_y, mean_y_err, Run, Reco, HV_val, RunDir, inputDir);
        outTable1  << TString::Format("%.2f",h421->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h421->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y) << "$\\pm$" << TString::Format("%.2f",sigma_y_err) << " \\\\ " << endl;
        outTable  << TString::Format("%.2f",h421->GetRMS())  << "$\\pm$" << TString::Format("%.2f",h421->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y) << "$\\pm$" << TString::Format("%.2f",sigma_y_err) << " & ";
        //
        // if (is_template_reco == 0){
        //   RMS_generic_x_l2_file     << runToIntLumi[Run.Atoi()] << "\t" << h420->GetRMS() << "\t" << 0.0         << std::endl;
        //   RMS_generic_y_l2_file     << runToIntLumi[Run.Atoi()] << "\t" << h421->GetRMS() << "\t" << 0.0         << std::endl;
        //   sigma_generic_x_l2_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_x        << "\t" << sigma_x_err << std::endl;
        //   sigma_generic_y_l2_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_y        << "\t" << sigma_y_err << std::endl;
        // }
        // else{
        //   RMS_template_x_l2_file    << runToIntLumi[Run.Atoi()] << "\t" << h420->GetRMS() << "\t" << 0.0         << std::endl;
        //   RMS_template_y_l2_file    << runToIntLumi[Run.Atoi()] << "\t" << h421->GetRMS() << "\t" << 0.0         << std::endl;
        //   sigma_template_x_l2_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_x        << "\t" << sigma_x_err << std::endl;
        //   sigma_template_y_l2_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_y        << "\t" << sigma_y_err << std::endl;
        // }
        //
        // //Get resolution along x
        TH1F*  hg420= (TH1F*) _file0->Get(directory+Run+(TString)"/hg420");
        hg420->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243 ;
        fittp0("hg420",sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243, Run, Reco, HV_val, RunDir, inputDir);
        outTable2  << TString::Format("%.2f",hg420->GetRMS())  << "$\\pm$" << TString::Format("%.2f",hg420->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x_l243) << "$\\pm$" << TString::Format("%.2f",sigma_x_err_l243) << " & ";
        outTable  << TString::Format("%.2f",hg420->GetRMS())  << "$\\pm$" << TString::Format("%.2f",hg420->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x_l243) << "$\\pm$" << TString::Format("%.2f",sigma_x_err_l243) << " & ";
        //
        //Get resolution along y
        TH1F*  hg421= (TH1F*) _file0 ->Get(directory+Run+(TString)"/hg421");
        hg421->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y_l243,sigma_y_err_l243, mean_y_l243, mean_y_err_l243 ;
        fittp0("hg421", sigma_y_l243, sigma_y_err_l243, mean_y_l243, mean_y_err_l243, Run, Reco, HV_val, RunDir, inputDir);
        outTable2  << TString::Format("%.2f",hg421->GetRMS())  << "$\\pm$" << TString::Format("%.2f",hg421->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y_l243) << "$\\pm$" << TString::Format("%.2f",sigma_y_err_l243) << " & ";
        outTable  << TString::Format("%.2f",hg421->GetRMS())  << "$\\pm$" << TString::Format("%.2f",hg421->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y_l243) << "$\\pm$" << TString::Format("%.2f",sigma_y_err_l243) << " & ";
        //
        //
        // if (is_template_reco == 0){
        //   RMS_generic_x_l3_file     << runToIntLumi[Run.Atoi()] << "\t" << h520->GetRMS() << "\t" << 0.0              << std::endl;
        //   RMS_generic_y_l3_file     << runToIntLumi[Run.Atoi()] << "\t" << h521->GetRMS() << "\t" << 0.0              << std::endl;
        //   sigma_generic_x_l3_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_x_l243   << "\t" << sigma_x_err_l243 << std::endl;
        //   sigma_generic_y_l3_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_y_l243   << "\t" << sigma_y_err_l243 << std::endl;
        // }
        // else{
        //   RMS_template_x_l3_file    << runToIntLumi[Run.Atoi()] << "\t" << h520->GetRMS() << "\t" << 0.0              << std::endl;
        //   RMS_template_y_l3_file    << runToIntLumi[Run.Atoi()] << "\t" << h521->GetRMS() << "\t" << 0.0              << std::endl;
        //   sigma_template_x_l3_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_x_l243   << "\t" << sigma_x_err_l243 << std::endl;
        //   sigma_template_y_l3_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_y_l243   << "\t" << sigma_y_err_l243 << std::endl;
        // }

        //Get resolution along x
        TH1F*  g520= (TH1F*) _file0->Get(directory+Run+(TString)"/g520");
        g520->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234 ;
        fittp0("g520",sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234, Run, Reco, HV_val, RunDir, inputDir);
        outTable2  << TString::Format("%.2f",g520->GetRMS())  << "$\\pm$" << TString::Format("%.2f",g520->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x_l234) << "$\\pm$" << TString::Format("%.2f",sigma_x_err_l234) << " & ";
        outTable  << TString::Format("%.2f",g520->GetRMS())  << "$\\pm$" << TString::Format("%.2f",g520->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_x_l234) << "$\\pm$" << TString::Format("%.2f",sigma_x_err_l234) << " & ";

        //Get resolution along y
        TH1F*  g521= (TH1F*) _file0 ->Get(directory+Run+(TString)"/g521");
        g521->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y_l234,sigma_y_err_l234, mean_y_l234, mean_y_err_l234 ;
        fittp0("g521", sigma_y_l234, sigma_y_err_l234, mean_y_l234, mean_y_err_l234, Run, Reco, HV_val, RunDir, inputDir);
        outTable2  << TString::Format("%.2f",g521->GetRMS())  << "$\\pm$" << TString::Format("%.2f",g521->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y_l234) << "$\\pm$" << TString::Format("%.2f",sigma_y_err_l234) << " \\\\ " << endl;
        outTable  << TString::Format("%.2f",g521->GetRMS())  << "$\\pm$" << TString::Format("%.2f",g521->GetRMSError()) << " & "
                  << TString::Format("%.2f",sigma_y_l234) << "$\\pm$" << TString::Format("%.2f",sigma_y_err_l234) << " \\\\ " << endl;
        //
        //
        // if (is_template_reco == 0){
        //   RMS_generic_x_l4_file     << runToIntLumi[Run.Atoi()] << "\t" << g520->GetRMS() << "\t" << 0.0              << std::endl;
        //   RMS_generic_y_l4_file     << runToIntLumi[Run.Atoi()] << "\t" << g521->GetRMS() << "\t" << 0.0              << std::endl;
        //   sigma_generic_x_l4_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_x_l234   << "\t" << sigma_x_err_l234 << std::endl;
        //   sigma_generic_y_l4_file   << runToIntLumi[Run.Atoi()] << "\t" << sigma_y_l234   << "\t" << sigma_y_err_l234 << std::endl;
        // }
        // else{
        //   RMS_template_x_l4_file    << runToIntLumi[Run.Atoi()] << "\t" << g520->GetRMS() << "\t" << 0.0              << std::endl;
        //   RMS_template_y_l4_file    << runToIntLumi[Run.Atoi()] << "\t" << g521->GetRMS() << "\t" << 0.0              << std::endl;
        //   sigma_template_x_l4_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_x_l234   << "\t" << sigma_x_err_l234 << std::endl;
        //   sigma_template_y_l4_file  << runToIntLumi[Run.Atoi()] << "\t" << sigma_y_l234   << "\t" << sigma_y_err_l234 << std::endl;
        // }

      }
      outTable  << "\\hline" << endl;
      outTable1 << "\\hline" << endl;
      outTable2 << "\\hline" << endl;

      if (pt == "4 GeV"){
        TH1F*  h510= (TH1F*) _file0->Get(directory+Run+(TString)"/h510");
        h510->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231 ;
        fittp0("h510",sigma_x_l231,sigma_x_err_l231,mean_x_l231,mean_x_err_l231, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along y
        TH1F*  h511= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h511");
        h511->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y_l231,sigma_y_err_l231, mean_y_l231, mean_y_err_l231 ;
        fittp0("h511", sigma_y_l231, sigma_y_err_l231, mean_y_l231, mean_y_err_l231, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along x
        TH1F*  h410= (TH1F*) _file0->Get(directory+Run+(TString)"/h410");
        h410->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x,sigma_x_err,mean_x,mean_x_err ;
        fittp0("h410",sigma_x,sigma_x_err,mean_x,mean_x_err, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along y
        TH1F*  h411= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h411");
        h411->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y,sigma_y_err, mean_y, mean_y_err ;
        fittp0("h411", sigma_y, sigma_y_err, mean_y, mean_y_err, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along x
        TH1F*  hg410= (TH1F*) _file0->Get(directory+Run+(TString)"/hg410");
        hg410->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243 ;
        fittp0("hg410",sigma_x_l243,sigma_x_err_l243,mean_x_l243,mean_x_err_l243, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along y
        TH1F*  hg411= (TH1F*) _file0 ->Get(directory+Run+(TString)"/hg411");
        hg411->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y_l243,sigma_y_err_l243, mean_y_l243, mean_y_err_l243 ;
        fittp0("hg411", sigma_y_l243, sigma_y_err_l243, mean_y_l243, mean_y_err_l243, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along x
        TH1F*  g510= (TH1F*) _file0->Get(directory+Run+(TString)"/g510");
        g510->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234 ;
        fittp0("g510",sigma_x_l234,sigma_x_err_l234,mean_x_l234,mean_x_err_l234, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along y
        TH1F*  g511= (TH1F*) _file0 ->Get(directory+Run+(TString)"/g511");
        g511->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y_l234,sigma_y_err_l234, mean_y_l234, mean_y_err_l234 ;
        fittp0("g511", sigma_y_l234, sigma_y_err_l234, mean_y_l234, mean_y_err_l234, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along x
        TH1F*  h520_in= (TH1F*) _file0->Get(directory+Run+(TString)"/h520_in");
        h520_in->SetDirectory(0); // "detach" the histogram from the file
        float sigma_x_l231_in,sigma_x_err_l231_in,mean_x_l231_in,mean_x_err_l231_in ;
        fittp0("h520_in",sigma_x_l231_in,sigma_x_err_l231_in,mean_x_l231_in,mean_x_err_l231_in, Run, Reco, HV_val, RunDir, inputDir);

        //Get resolution along y
        TH1F*  h521_in= (TH1F*) _file0 ->Get(directory+Run+(TString)"/h521_in");
        h521_in->SetDirectory(0); // "detach" the histogram from the file
        float sigma_y_l231_in,sigma_y_err_l231_in, mean_y_l231_in, mean_y_err_l231_in ;
        fittp0("h521_in", sigma_y_l231_in, sigma_y_err_l231_in, mean_y_l231_in, mean_y_err_l231_in, Run, Reco, HV_val, RunDir, inputDir);


      }
      RMS_generic_x_l1_file.close();
      RMS_generic_y_l1_file.close();
      RMS_template_x_l1_file.close();
      RMS_template_y_l1_file.close();
      sigma_generic_x_l1_file.close();
      sigma_generic_y_l1_file.close();
      sigma_template_x_l1_file.close();
      sigma_template_y_l1_file.close();

      RMS_generic_x_l2_file.close();
      RMS_generic_y_l2_file.close();
      RMS_template_x_l2_file.close();
      RMS_template_y_l2_file.close();
      sigma_generic_x_l2_file.close();
      sigma_generic_y_l2_file.close();
      sigma_template_x_l2_file.close();
      sigma_template_y_l2_file.close();

      RMS_generic_x_l3_file.close();
      RMS_generic_y_l3_file.close();
      RMS_template_x_l3_file.close();
      RMS_template_y_l3_file.close();
      sigma_generic_x_l3_file.close();
      sigma_generic_y_l3_file.close();
      sigma_template_x_l3_file.close();
      sigma_template_y_l3_file.close();

      RMS_generic_x_l4_file.close();
      RMS_generic_y_l4_file.close();
      RMS_template_x_l4_file.close();
      RMS_template_y_l4_file.close();
      sigma_generic_x_l4_file.close();
      sigma_generic_y_l4_file.close();
      sigma_template_x_l4_file.close();
      sigma_template_y_l4_file.close();
    }
  }

  _file0->Close();
  return 0;


}
