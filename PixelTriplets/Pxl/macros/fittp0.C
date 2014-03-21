//
// Daniel Pitzl, Dec 2010
// fit Student's t + p0
// .x fittp0.C("h012")
//
   
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

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
int fittp0( char* hs ) {

  TH1 *h = (TH1*)gDirectory->Get(hs);

  if( h == NULL ){

    cout << hs << " does not exist\n";

  }

  else{
   
    h->SetMarkerStyle(21);
    h->SetMarkerSize(0.8);
    h->SetStats(1);
    gStyle->SetOptFit(101);

    gROOT->ForceStyle();

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

    TF1 *tp0Fcn = new TF1( "tp0Fcn", tp0Fit, x1, x9, 5 );

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

    tp0Fcn->SetParameter( 0, xmax ); // peak position
    tp0Fcn->SetParameter( 1, 4*dx ); // width
    tp0Fcn->SetParameter( 2, 2.2 ); // nu
    tp0Fcn->SetParameter( 3, nn ); // N
    tp0Fcn->SetParameter( 4, bg );
    
    h->Fit( "tp0Fcn", "R", "ep" );
    // h->Fit("tp0Fcn","V+","ep");

    h->Draw("histepsame");  // data again on top
  }
}
