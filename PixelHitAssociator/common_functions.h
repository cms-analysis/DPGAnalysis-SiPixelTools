#include "TCanvas.h"
#include "TH2.h"
#include "TStyle.h"
#include <sys/stat.h>
#include <libgen.h>
#define NOVAL_F -9999.0

TFile *f; TCanvas *c; TArrow *arr; TGaxis* axis; TGraphAsymmErrors* g; TH1D* h; TH2D* h2;
std::vector<std::string> formats = { ".png", ".eps", ".root", ".C", ".pdf" };
std::string save_dir;
bool save;
int remove_prelim_label, ileg;

TCanvas* custom_can_(TH1* h, std::string canname, int gx = 0, int gy = 0,
		     int histosize_x = 500, int histosize_y = 500,
		     int mar_left = 80, int mar_right = 20, int mar_top = 20, int mar_bottom = 60, 
		     int title_align = 33, float title_x = 1.0, float title_y = 1.0,
		     bool log=false) {
  if (std::string(h->GetTitle()).size()>0&&title_y==1.0) mar_top += 20;
  int titlefontsize = 32;
  int labelfontsize = 20;
  int yoffset_x = mar_left - titlefontsize - 4;
  int xoffset_y = mar_bottom - titlefontsize - 4;
  int zoffset_x = mar_right - titlefontsize - 4;
  int padsize_x = histosize_x + mar_left + mar_right;
  int padsize_y = histosize_y + mar_top + mar_bottom;
  int padsize = ((padsize_x<=padsize_y) ? padsize_x : padsize_y);
  float padratio_yx = (float)padsize_y/padsize_x > 1 ? 1 : (float)padsize_y/padsize_x;
  float padratio_xy = (float)padsize_x/padsize_y > 1 ? 1 : (float)padsize_x/padsize_y;
  Float_t xoffset = ((Float_t)xoffset_y/titlefontsize+0.5) * padratio_xy /1.6;
  Float_t yoffset = ((Float_t)yoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t zoffset = ((Float_t)zoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t titlesize = (Float_t)titlefontsize/padsize;
  Float_t labelsize = (Float_t)labelfontsize/padsize;
  //gStyle->SetOptStat(0);
  if (std::string(h->GetTitle()).size()) {
    gStyle->SetOptTitle(1);
    gStyle->SetTitleH(titlefontsize/padsize);
    gStyle->SetTitleFontSize(titlesize*0.8);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetTitleAlign(title_align);
    gStyle->SetTitleX(title_x);
    gStyle->SetTitleY(title_y);
  }
  h->SetTitleFont(42,"xyz");
  h->SetLabelFont(42,"xyz");
  h->SetTitleSize(titlesize,"xyz");
  h->SetLabelSize(labelsize,"xyz");
  h->GetXaxis()->SetTitleOffset(xoffset);
  h->GetYaxis()->SetTitleOffset(yoffset);
  h->GetZaxis()->SetTitleOffset(zoffset);
  h->GetYaxis()->SetDecimals(1);
  h->GetZaxis()->SetDecimals(1);
  TCanvas* canvas = new TCanvas(canname.c_str(), h->GetTitle(), padsize_x + 4, padsize_y + 26);
  TVirtualPad* pad = canvas->cd(1);
  pad->SetLeftMargin((Float_t)mar_left/padsize_x);
  pad->SetRightMargin((Float_t)mar_right/padsize_x);
  pad->SetTopMargin((Float_t)mar_top/padsize_y);
  pad->SetBottomMargin((Float_t)mar_bottom/padsize_y);
  canvas->SetGrid(gx,gy);
  if (log) canvas->SetLogy(1);
  gPad->Update();
  return canvas;
}

TCanvas* custom_can_(TH2* h, std::string canname, int gx = 0, int gy = 0,
		     int histosize_x = 500, int histosize_y = 500, 
		     int mar_left = 80, int mar_right = 120, int mar_top = 20, int mar_bottom = 60, 
		     int title_align = 33, float title_x = 1.0, float title_y = 1.0,
		     std::string draw="COLZ", bool norm=false, bool log=false) {
  if (std::string(h->GetTitle()).size()>0&&title_y==1.0) mar_top += 20;
  int titlefontsize = 32;
  int labelfontsize = 20;
  int pal_offset_x = 5;
  int pal_width_x = 25;
  int xoffset_y = mar_bottom - titlefontsize - 4;
  int yoffset_x = mar_left - titlefontsize - 4;
  int zoffset_x = mar_right - pal_offset_x - pal_width_x - titlefontsize;
  int padsize_x = histosize_x + mar_left + mar_right;
  int padsize_y = histosize_y + mar_top + mar_bottom;
  int padsize = ((padsize_x<=padsize_y) ? padsize_x : padsize_y);
  float padratio_yx = (Float_t)padsize_y/padsize_x > 1 ? 1 : (Float_t)padsize_y/padsize_x;
  float padratio_xy = (Float_t)padsize_x/padsize_y > 1 ? 1 : (Float_t)padsize_x/padsize_y;
  Float_t xoffset = ((Float_t)xoffset_y/titlefontsize+0.5) * padratio_xy /1.6;
  Float_t yoffset = ((Float_t)yoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t zoffset = ((Float_t)zoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  zoffset *= 500./histosize_y;
  Float_t titlesize = (Float_t)titlefontsize/padsize;
  Float_t labelsize = (Float_t)labelfontsize/padsize;
  h->SetTitleFont(42,"xyz");
  h->SetLabelFont(42,"xyz");
  h->SetTitleSize(titlesize,"xyz");
  h->SetLabelSize(labelsize,"xyz");
  h->GetXaxis()->SetTitleOffset(xoffset);
  h->GetYaxis()->SetTitleOffset(yoffset);
  h->GetZaxis()->SetTitleOffset(zoffset);
  h->GetZaxis()->RotateTitle(1);
  h->GetYaxis()->SetDecimals(1);
  h->GetZaxis()->SetDecimals(1);
  if (histosize_y<250) h->GetZaxis()->SetNdivisions(505);
  if (std::string(h->GetTitle()).size()) {
    gStyle->SetOptTitle(1);
    gStyle->SetTitleH(titlefontsize/padsize);
    gStyle->SetTitleFontSize(titlesize*0.8);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetTitleAlign(title_align);
    gStyle->SetTitleX(title_x);
    gStyle->SetTitleY(title_y);
  }
  TCanvas* canvas = new TCanvas(canname.c_str(), h->GetTitle(), padsize_x + 4, padsize_y + 26);
  TVirtualPad* pad = canvas->cd(1);
  pad->SetLeftMargin((Float_t)mar_left/padsize_x);
  pad->SetRightMargin((Float_t)mar_right/padsize_x);
  pad->SetTopMargin((Float_t)mar_top/padsize_y);
  pad->SetBottomMargin((Float_t)mar_bottom/padsize_y);
  canvas->SetGrid(gx,gy);
  if (log) canvas->SetLogz(1);
  if (norm&&h->Integral()>0) h = (TH2D*)h->DrawNormalized(draw.c_str());
  else h->Draw(draw.c_str());
  if (h->Integral()>0&&draw=="COLZ") {
    gPad->Update();
    TPaletteAxis* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
    if (palette) {
      palette->SetX1NDC(1 - (Float_t)(mar_right - pal_offset_x)/padsize_x);
      palette->SetX2NDC(1 - (Float_t)(mar_right - pal_offset_x - pal_width_x)/padsize_x);
      palette->SetY1NDC((Float_t)mar_bottom/padsize_y);
      palette->SetY2NDC(1 - (Float_t)mar_top/padsize_y);
    }
  }
  return canvas;
}

TGraphAsymmErrors* asym_(TH1D* h) {
  int n = 0;
  for (int i=0; i<h->GetNbinsX(); ++i) if (h->GetBinContent(i+1)>0) n++;
  TGraphAsymmErrors* tgae = new TGraphAsymmErrors(n);
  n = 0;
  for (int i=0; i<h->GetNbinsX(); ++i) {
    double y = h->GetBinContent(i+1);
    double x = h->GetBinCenter(i+1);
    if (y>0) {
      n++;
      tgae->SetPoint(n-1,x,y);
    }
  }
  // Calculate the asymmetric wilson score interval
  double z = 1; // 1 Sigma confidence
  for (int bin=1; bin<=h->GetNbinsX(); ++bin) {
    double p = h->GetBinContent(bin);
    double err = h->GetBinError(bin);
    tgae->SetPointEYhigh(bin-1,err);
    tgae->SetPointEYlow (bin-1,err);
  }
  // Set same ranges/settings etc
  double xlow = h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->GetFirst());
  double xup  = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetLast());
  double ylow = h->GetMinimum();
  double yup  = h->GetMaximum();
  tgae->GetXaxis()->SetRangeUser(xlow,xup);
  tgae->GetYaxis()->SetRangeUser(ylow,yup);
  tgae->SetTitle(h->GetTitle());
  tgae->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
  tgae->GetYaxis()->SetTitle(h->GetYaxis()->GetTitle());
  tgae->GetXaxis()->SetTitleSize(h->GetXaxis()->GetTitleSize());
  tgae->GetYaxis()->SetTitleSize(h->GetYaxis()->GetTitleSize());
  tgae->GetXaxis()->SetTitleOffset(h->GetXaxis()->GetTitleOffset());
  tgae->GetYaxis()->SetTitleOffset(h->GetYaxis()->GetTitleOffset());
  tgae->GetXaxis()->SetTitleFont(h->GetXaxis()->GetTitleFont());
  tgae->GetYaxis()->SetTitleFont(h->GetYaxis()->GetTitleFont());
  tgae->GetXaxis()->SetNdivisions(h->GetNdivisions("X"));
  tgae->GetYaxis()->SetNdivisions(h->GetNdivisions("Y"));
  tgae->GetYaxis()->SetDecimals(1);
  tgae->SetMarkerStyle(h->GetMarkerStyle());
  tgae->SetMarkerColor(h->GetMarkerColor());
  tgae->SetMarkerSize(h->GetMarkerSize());
  return tgae;
}

void asym_labels_(TH1D *orig, TGraphAsymmErrors* tgae, int hor_vert_up_down = 1) {
  int angle = hor_vert_up_down==0 ? 0 : hor_vert_up_down==1 ? 90 : hor_vert_up_down==2 ? 20 : -20 ;
  int align = hor_vert_up_down==0 ? 23 : hor_vert_up_down==1 ? 32 : hor_vert_up_down==2 ? 33 : 13;
  if (orig->GetNbinsX()>0) tgae->GetXaxis()->SetLabelColor(0);
  double labelsize = orig->GetXaxis()->GetLabelSize();
  double offset = (gPad->GetY2()-gPad->GetY1()) * orig->GetXaxis()->GetLabelOffset()*0.16;
  for (size_t i=0; i<orig->GetNbinsX(); ++i) {
    TLatex *lat = new TLatex(orig->GetBinCenter(i+1), orig->GetMinimum()-offset, orig->GetXaxis()->GetBinLabel(i+1));
    //std::cout<<i+1<<" "<<orig->GetXaxis()->GetBinLabel(i+1)<<" x "<<orig->GetBinCenter(i+1)<<" y "<<orig->GetMinimum()-offset<<std::endl;
    lat->SetTextAlign(align);
    lat->SetTextAngle(angle);
    lat->SetTextFont(orig->GetXaxis()->GetLabelFont());
    lat->SetTextSize(labelsize*0.65);
    lat->Draw();
  }
}

void draw_with_right_yaxis_(TH1D* hleft, TH1D* hright, std::string opt, std::string ytitle_right,
      		      Double_t xmin = NOVAL_F, Double_t xmax = NOVAL_F,
      		      Double_t left_ymin = NOVAL_F, Double_t left_ymax = NOVAL_F,
      		      Double_t right_ymin = NOVAL_F, Double_t right_ymax = NOVAL_F,
      		      Double_t left_localymax_x = NOVAL_F, Double_t shift_maximum = 1.0) {
  if (hleft->GetEntries()>0&&hleft->GetMaximum()!=0&&hright->GetEntries()>0&&hright->GetMaximum()!=0) {
    // Set X axis range
    Int_t binx_min = hleft->GetXaxis()->FindBin(xmin);
    Int_t binx_max = hleft->GetXaxis()->FindBin(xmax);
    xmin = (xmin == NOVAL_F) ? hleft->GetXaxis()->GetXmin()
      : hleft->GetXaxis()->GetBinLowEdge(binx_min);
    xmax = xmax == NOVAL_F ? hleft->GetXaxis()->GetXmax()
      : hleft->GetXaxis()->GetBinUpEdge(binx_max);
    binx_min = hleft->GetXaxis()->FindBin(xmin);
    binx_max = hleft->GetXaxis()->FindBin(xmax);
    hleft->GetXaxis()->SetRange(binx_min,binx_max-1);
    hright->GetXaxis()->SetRange(binx_min,binx_max-1);
    // Set Styles and Draw First histogram with left y axis
    if (opt.find("P")!=std::string::npos) {
      hleft->SetMarkerStyle(21);
      hright->SetMarkerStyle(20);
      hright->SetMarkerColor(2);
    } else hright->SetLineColor(2);
    hleft->Draw(opt.c_str());
    // Rescale Right Histogram according to specified Right Y axis ranges
    // If ranges unspecified, will set histogram to same minimum/maximum
    // (On both X and right Y axis)
    Double_t left_maximum = left_localymax_x==NOVAL_F ? hleft->GetMaximum() :
      hleft->GetBinContent(hleft->GetXaxis()->FindBin(left_localymax_x));
    Double_t right_maximum = hright->GetMaximum();
    left_ymin = (left_ymin==NOVAL_F) ? hleft->GetMinimum() : left_ymin;
    left_ymax = (left_ymax==NOVAL_F) ? left_maximum *1.05 : left_ymax;
    hleft->GetYaxis()->SetRangeUser(left_ymin,left_ymax);
    // Find Minimum Plotted value
    Double_t left_minimum = left_maximum;
    Double_t right_minimum = right_maximum;
    for (int binx = binx_min; binx<binx_max; binx++) {
      Double_t value_left = hleft->GetBinContent(binx);
      if (value_left<left_minimum&&value_left>xmin) left_minimum = value_left;
      Double_t value_right = hright->GetBinContent(binx);
      if (value_right<right_maximum&&value_right>0) right_minimum = value_right;
    }
    // Rescale Right Histogram
    if (right_ymin==NOVAL_F&&right_ymax==NOVAL_F) {
      // No ranges specified 
      // Minimums, Maximums forced to same height (or shifted by factor)
      Double_t rescale_factor = (left_maximum - left_minimum) / (right_maximum - right_minimum) * shift_maximum;
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_minimum + (value - right_minimum) * rescale_factor);
      }
      right_ymin = right_minimum - (left_minimum - left_ymin) / rescale_factor;
      right_ymax = right_minimum + (left_ymax - left_minimum) / rescale_factor;
    } else if (right_ymax==NOVAL_F) {
      // Right Y-axis low range Specified
      // Maximums forced to same height (or shifted by factor)
      Double_t rescale_factor = (left_maximum - left_ymin) / (right_maximum - right_ymin) * shift_maximum;
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_ymin + (value - right_ymin) * rescale_factor);
      }
      right_ymax = right_ymin + (left_ymax-left_ymin) / rescale_factor;
    } else if (right_ymin==NOVAL_F) {
      // Right Y-axis upper range Specified 
      // Maximums forced to same height (or shifted by factor)
      Double_t rescale_factor = (left_ymax - left_maximum) / (right_ymax - right_maximum) / shift_maximum;
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_ymax - (right_ymax - value) * rescale_factor);
      }
      right_ymin = right_ymax - (left_ymax-left_ymin) / rescale_factor;
    } else {
      // Right Y-axis ranges Specified (Manual mode)
      // Rescale accordingl, no shift possible
      Double_t rescale_factor = (left_ymax - left_ymin) / (right_ymax - right_ymin);
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_ymin + (value - right_ymin) * rescale_factor);
      }
    }
    // Draw Rescaled histo and Paint Right axis
    hright->Draw("SAMEPE1X0");
    Double_t smallshift = hleft->GetBinWidth(binx_max)/1000;
    TGaxis* axis = new TGaxis(xmax+smallshift,left_ymin,xmax+smallshift,left_ymax,right_ymin,right_ymax,510,"+L");
    axis->SetDecimals(hleft->GetZaxis()->GetDecimals());
    axis->SetLabelFont(hleft->GetZaxis()->GetLabelFont());
    axis->SetLabelSize(hleft->GetZaxis()->GetLabelSize());
    axis->SetLabelColor(2);
    axis->SetLineColor(2);
    axis->SetTitleColor(2);
    axis->SetTitle(ytitle_right.c_str());
    axis->SetTitleFont(hleft->GetZaxis()->GetTitleFont());
    axis->SetTitleSize(hleft->GetZaxis()->GetTitleSize());
    axis->SetTitleOffset(hleft->GetZaxis()->GetTitleOffset());
    axis->Draw();
  }
}

void draw_line(double x1, double x2, double y1, double y2, int width=2, int style=1, int color=1) {
  TLine* l = new TLine(x1, y1, x2, y2);
  l->SetLineWidth(width);
  l->SetLineStyle(style);
  l->SetLineColor(color);
  l->Draw();
}

void draw_line_old(double x1, double y1, double x2, double y2, Color_t col = 1, Style_t s = 1, Width_t w = 1) { 
  TLine* line = new TLine(x1,y1,x2,y2); if (col!=1) line->SetLineColor(col); if (s!=1) line->SetLineStyle(s); line->SetLineWidth(w); line->Draw(); }

void draw_lat_(double x, double y, const char* text, Int_t col, double size=0.04, double angle=0.0, short al=11) { 
  TLatex* l = new TLatex(x,y,text); l->SetTextColor(col); l->SetTextSize(size); l->SetTextAngle(angle); l->SetTextAlign(al); l->Draw(); 
}

void prelim_lat_(double xmin, double xmax, double ymin, double ymax, bool in) {
  std::string prelim = "CMS #scale[0.8]{#font[52]{Preliminary 2016}}";// #font[22]{Times bold} and #font[12]{Times Italic}
  if (in) { TLatex* cms_lat = new TLatex(xmin+(xmax-xmin)/20.0, ymax-(ymax-ymin)/10.0, prelim.c_str()); cms_lat->SetLineWidth(2); cms_lat->Draw(); }
  else { TLatex* cms_lat = new TLatex(xmin, ymax+(ymax-ymin)/40.0, prelim.c_str()); cms_lat->SetLineWidth(2); cms_lat->Draw(); }
}

void era_lat_(double xmin, double xmax, double ymin, double ymax) {
  std::string prelim = "#sqrt{s}=13 TeV (25ns)";// #font[22]{Times bold} and #font[12]{Times Italic}
  TLatex* era_lat = new TLatex(xmax, ymax+(ymax-ymin)/25.0, prelim.c_str()); era_lat->SetTextAlign(32); era_lat->SetTextSize(0.04); era_lat->SetTextFont(42); era_lat->SetLineWidth(2); era_lat->Draw();
}

void save_plots(TCanvas* c, std::string save_dir, std::vector<std::string> formats, int remove_prelim_label=1) {  
  if (remove_prelim_label==1) for (int i=0; i<c->GetListOfPrimitives()->GetEntries(); ++i)
    if (TString(c->GetListOfPrimitives()->At(i)->GetTitle()).Contains("Preliminary"))
      c->GetListOfPrimitives()->RemoveAt(i);
  if (remove_prelim_label==2) {
    for (int i=0; i<c->GetListOfPrimitives()->GetEntries(); ++i)
      if (TString(c->GetListOfPrimitives()->At(i)->GetTitle()).Contains("Preliminary")) {
	((TLatex*)c->GetListOfPrimitives()->At(i))->SetTitle("CMS #scale[0.8]{#font[52]{Work in progress}}");
      }
  }
  gPad->Update(); 
  std::string plotname = c->GetName();
  if (plotname.find("_ForApproval")!=std::string::npos) plotname.erase(plotname.find("_ForApproval"), 12);
  std::string dir  = dirname(strdup((save_dir+plotname).c_str()));
  std::string base = basename(strdup((save_dir+plotname).c_str()));
  mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  bool first_other = 1;
  for (auto format : formats) {
    if (format != ".png") {
      if (first_other) {
	mkdir((dir+"/other_formats").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	first_other = 0;
      }
      c->SaveAs((dir+"/other_formats/"+base+format).c_str());
    } else {
      c->SaveAs((dir+"/"+base+format).c_str());
    }
  }
}

void remove_points(TCanvas* c, int iplot, int first, int last) {
  TGraphAsymmErrors* g = (TGraphAsymmErrors*)c->GetListOfPrimitives()->At(iplot);
  for (int i=last; i>=first; --i) g->RemovePoint(i);
}

void remove_plot(TCanvas* c, int iplot, int ileg) {
  ((TLegend*)c->GetListOfPrimitives()->At(ileg))->GetListOfPrimitives()->RemoveAt(iplot+1);
  c->GetListOfPrimitives()->RemoveAt(iplot);
}

void modify_legend(TCanvas* c, int ileg, std::string title, double x1, double x2, double y1, double y2) {
  TLegend *leg = (TLegend*)c->GetListOfPrimitives()->At(ileg);
  leg->SetHeader(title.c_str()); leg->SetX1(x1); leg->SetX2(x2); leg->SetY1(y1); leg->SetY2(y2);
}

void rename_legend(TCanvas* c, int ileg, std::string title) {
  TLegend *leg = (TLegend*)c->GetListOfPrimitives()->At(ileg);
  leg->SetHeader(title.c_str());
}

void move_legend(TCanvas* c, int ileg, double x1, double y2) {
  TLegend *leg = (TLegend*)c->GetListOfPrimitives()->At(ileg);
  double x2 = x1 + leg->GetX2() - leg->GetX1();
  double y1 = y2 + leg->GetY1() - leg->GetY2();
  leg->SetX1(x1); leg->SetX2(x2); leg->SetY1(y1); leg->SetY2(y2);
}

void era_and_prelim_lat(double xmin, double xmax, double ymin, double ymax, bool era, bool approval, bool in=0) {
  if (era) {
    std::string era = "#sqrt{s}=13 TeV (25ns)";
    TLatex* era_lat = new TLatex(xmax, ymax+(ymax-ymin)/25.0, era.c_str());
    era_lat->SetTextAlign(32);
    era_lat->SetTextSize(0.04);
    era_lat->SetTextFont(42);
    era_lat->SetLineWidth(2);
    era_lat->Draw();
  }
  if (approval) {
    // Latex example: #font[22]{Times bold} and #font[12]{Times Italic}
    std::string prelim = "CMS #scale[0.8]{#font[52]{Preliminary 2016}}";
    TLatex* cms_lat = new TLatex(in ? xmin+(xmax-xmin)/20.0 : xmin, in ? ymax-(ymax-ymin)/10.0 : ymax+(ymax-ymin)/40.0, prelim.c_str()); 
    cms_lat->SetLineWidth(2); 
    cms_lat->Draw();
  }
}
void add_labels(TCanvas* c, bool era=1, bool approval=1) {
  double xmin = ((TFrame*)c->GetListOfPrimitives()->At(0))->GetX1();
  double xmax = ((TFrame*)c->GetListOfPrimitives()->At(0))->GetX2();
  double ymin = ((TFrame*)c->GetListOfPrimitives()->At(0))->GetY1();
  double ymax = ((TFrame*)c->GetListOfPrimitives()->At(0))->GetY2();
  era_and_prelim_lat(xmin, xmax, ymin, ymax, era, approval);
}
void add_labels(TGraphAsymmErrors* g, bool era=1, bool approval=1) {
  double xmin = g->GetXaxis()->GetBinLowEdge(g->GetXaxis()->GetFirst());
  double xmax = g->GetXaxis()->GetBinUpEdge(g->GetXaxis()->GetLast());
  double ymin = g->GetMinimum();
  double ymax = g->GetMaximum();
  era_and_prelim_lat(xmin, xmax, ymin, ymax, era, approval);
}
void add_labels(TH1* h, bool era=1, bool approval=1) {
  double xmin = h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->GetFirst());
  double xmax = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetLast());
  double ymin = h->GetMinimum();
  double ymax = h->GetMaximum();
  era_and_prelim_lat(xmin, xmax, ymin, ymax, era, approval);
}
void add_labels(TH2* h, bool era=1, bool approval=1) {
  double xmin = h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->GetFirst());
  double xmax = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetLast());
  double ymin = h->GetYaxis()->GetBinLowEdge(h->GetYaxis()->GetFirst());
  double ymax = h->GetYaxis()->GetBinUpEdge(h->GetYaxis()->GetLast());
  era_and_prelim_lat(xmin, xmax, ymin, ymax, era, approval);
}


TF1* fit_landau_plus_gaus_(TH1* h, double &mpv, double &error) {
  double xmax = h->GetBinCenter(h->GetMaximumBin());
  double ymax = h->GetMaximum();
  // First fit with landau to get some parameter estimates
  TF1 *f = new TF1((std::string(h->GetName())+"_fit").c_str(),"landau", (xmax<25 ? 0 : xmax-25), xmax+25);
  f->SetParameter(0, 5.5*ymax);
  f->SetParLimits(0, 4*ymax,7*ymax);
  f->SetParameter(1, xmax);
  f->SetParLimits(1, xmax<5 ? 0: xmax-5, xmax + 5);
  f->SetParameter(2, 2.3);
  f->SetParLimits(2, 0.1, 5);
  h->Fit((std::string(h->GetName())+"_fit").c_str(),"RBQ0");
  //double Chi2NDoF = f->GetChisquare()/f->GetNDF();
  double Const = f->GetParameter(0);
  //double MPV = f->GetParameter(1);
  double Sigma = f->GetParameter(2);
  //double YMax = f->GetMaximum();
  double XMax = f->GetMaximumX();
  //std::cout<<printf( "Chi2/NDoF: %10.1f  Const: %2.1f  MPV: %3.1f  Sigma: %2.2f", Chi2NDoF, Const/ymax, MPV, Sigma)<<std::endl;
  delete f;
  // Fit again with added gaus
  f = new TF1((std::string(h->GetName())+"_fit").c_str(),"landau(0)+gaus(3)", (XMax<25 ? 0 : XMax-25), XMax+25);
  f->SetParameter(0, 1.4*Const);
  f->SetParLimits(0, 2*ymax,10*ymax);
  f->SetParameter(1, XMax);
  f->SetParLimits(1, XMax<6.5 ? 1.5: XMax-5, XMax + 5);
  f->SetParameter(2, Sigma);
  f->SetParLimits(2, 0.1,10);
  f->SetParameter(3, -0.75*ymax);
  f->SetParLimits(3, -1.5*ymax,ymax);
  f->SetParameter(4, XMax);
  f->SetParLimits(4, 1, 45);
  f->SetParameter(5, Sigma);
  f->SetParLimits(5, 0, 8);
  h->Fit((std::string(h->GetName())+"_fit").c_str(),"RBQ0+");
  //Chi2NDoF = f->GetChisquare()/f->GetNDF();
  //double old_Const = Const;
  //Const = f->GetParameter(0);
  //MPV = f->GetParameter(1);
  Sigma = f->GetParameter(2);
  //double Const2 = f->GetParameter(3);
  //double Mean = f->GetParameter(4);
  //double Sigma2 = f->GetParameter(5);
  //YMax = f->GetMaximum();
  XMax = f->GetMaximumX();
  //std::cout<<printf( "Chi2/NDoF: %4.1f  C1: %2.1f  MPV: %3.1f  S1: %2.2f  C2: %5.2f  M: %3.1f  S2: %2.2f", Chi2NDoF, Const/ymax, MPV, Sigma, Const2/ymax, Mean, Sigma2)<<std::endl;
  mpv = XMax;
  error = f->GetParError(1);
  return f;
}

void add_mpv_fit(TCanvas* c, int nhist) {
  TH1D* h = (TH1D*)c->GetListOfPrimitives()->At(0);
  double xmin = h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->GetFirst());
  double xmax = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetLast());
  double ymin = h->GetMinimum();
  double ymax = h->GetMaximum();
  TPave *p = new TPave(xmax-(xmax-xmin)*0.25, ymax-((nhist+0.25)*(ymax-ymin)/20.0), xmax, ymax, 4, "bl");
  p->SetFillColor(0); p->SetShadowColor(0); p->Draw();
  for (int i=0; i<nhist; ++i) {
    h = (TH1D*)c->GetListOfPrimitives()->At(i);
    double mpv, error;
    TF1* f = fit_landau_plus_gaus_(h, mpv, error);
    f->SetLineColor(h->GetLineColor());
    f->SetLineWidth(1);
    f->SetLineStyle(7);
    f->Draw("SAME");
    std::stringstream ss; ss<<"MPV "<<std::setprecision(3)<<mpv;
    TLatex* mpv_lat = new TLatex(xmax-(xmax-xmin)*0.23, ymax-((i+0.25)*(ymax-ymin)/20.0), ss.str().c_str());
    mpv_lat->SetTextAlign(13); mpv_lat->SetTextSize(0.04); mpv_lat->SetTextFont(42); mpv_lat->SetLineWidth(2);
    mpv_lat->SetTextColor(h->GetLineColor()); mpv_lat->Draw("SAME");
  }
}

void add_mean(TCanvas* c, int nhist) {
  TH1D* h = (TH1D*)c->GetListOfPrimitives()->At(0);
  double xmin = h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->GetFirst());
  double xmax = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetLast());
  double ymin = h->GetMinimum();
  double ymax = h->GetMaximum();
  TPave *p = new TPave(xmax-(xmax-xmin)*0.23, ymax-((nhist+0.25)*(ymax-ymin)/20.0), xmax, ymax, 4, "bl");
  p->SetFillColor(0); p->SetShadowColor(0); p->Draw();
  for (int i=0; i<nhist; ++i) {
    h = (TH1D*)c->GetListOfPrimitives()->At(i);
    double mean = h->GetMean();
    std::stringstream ss; ss<<"Mean "<<std::setprecision(2)<<mean;
    TLatex* mean_lat = new TLatex(xmax-(xmax-xmin)*0.21, ymax-((i+0.25)*(ymax-ymin)/20.0), ss.str().c_str());
    mean_lat->SetTextAlign(13); mean_lat->SetTextSize(0.04); mean_lat->SetTextFont(42); mean_lat->SetLineWidth(2);
    mean_lat->SetTextColor(h->GetLineColor()); mean_lat->Draw("SAME");
  }
}

// HV Scans
TH1D* extend(TH1D* h) {
  TH1D* c = (TH1D*)h->Clone();
  h->SetBins(100100,-0.1,100);
  for (int i=1; i<h->GetNbinsX(); ++i) {
    h->SetBinContent(h->FindBin(c->GetBinCenter(i)), c->GetBinContent(i));
    h->SetBinError  (h->FindBin(c->GetBinCenter(i)), c->GetBinError(i));
  }
  return h;
}

TF1* fit_turnon_(double p1, double p2, TH1D *hv) {
  if (hv->GetEntries()>0) {
    TF1* f = new TF1("turnon", "1/(1+exp(([0]-x)/[1]))", 15, 205);
    f->SetParameters(p1,p2);
    hv->Fit("turnon","RMQ0");
    return hv->GetFunction("turnon");
  } else return NULL;
}

TF1* fit_turnon_improved_(TH1D *h, TH1D* dist, bool draw=0) {
  std::cout<<"Fitting "<<h->GetName()<<std::endl;
  if (h->GetEntries()>0) {
    // Estimate Parameters
    float p0 = 0;
    float first_point = 0;
    for (int i=h->GetNbinsX(); i>0; i--) if (dist->GetBinContent(i)>0) {
      p0 = h->GetBinContent(i);
      first_point = h->GetBinCenter(i);
    }
    float p1 = h->GetBinContent(h->GetMaximumBin()) - p0;
    int bin1 = 0;
    int bin2=9999;
    float mid = (p0 + p1) / 2.0;
    for (int i=h->GetNbinsX(); i>0; i--) {
      double cont = h->GetBinContent(i);
      if (cont<mid&&cont>0&&bin1==0) bin1 =i;
      if (cont>mid) bin2 = i;
    }
    double x1 = h->GetBinCenter(bin1);
    double x2 = h->GetBinCenter(bin2);
    double y1 = h->GetBinContent(bin1);
    double y2 = h->GetBinContent(bin2);
    float p2 = x1+ (x2-x1)*(mid-y1)/(y2-y1);
    // Fit
    if (std::string(h->GetName()).find("hv_l1_HV16_eff")==0) first_point = 7.5;
    TF1* f = new TF1((std::string(h->GetName())+"_turnon").c_str(), "[0]+[1]/(1+exp(([2]-x)/[3]))", first_point, 300);    
    f->SetParameter(0,p0);
    f->SetParameter(1,p1);
    f->SetParameter(2,p2);
    f->SetParameter(3,3.1);
    f->SetParLimits(0,0,0.6);
    f->SetParLimits(1,0,1);
    //f->SetParLimits(2,0,1);
    f->SetParLimits(3,0.25,15);
    f->SetLineColor(h->GetMarkerColor());
    if (draw) new TCanvas(h->GetName());
    //std::cout<<h->GetName()<<" "<<p0<<" "<<p1<<" "<<p2<<std::endl;
    h->Fit((std::string(h->GetName())+"_turnon").c_str(),draw?"RMQ":"RMQ0");
    //double Chi2NDoF = f->GetChisquare()/f->GetNDF();
    //double Const = f->GetParameter(0);
    //double Rise = f->GetParameter(1);
    //double Mid = f->GetParameter(2);
    //double Width = f->GetParameter(3);
    //std::cout<<printf( "Chi2/NDoF: %10.1f  Const: %1.3f  Rise: %1.3f  Mid: %3.1f  Width: %2.2lf", Chi2NDoF, Const, Rise, Mid, Width)<<std::endl;
    // gPad->Update();
    // if (fabs(Mid-p2)>4) sleep(5);
    return h->GetFunction((std::string(h->GetName())+"_turnon").c_str());
  } else return NULL;
}

TF1* fit_turnon_improved_(TH2D *h, bool draw=0) {
  return fit_turnon_improved_(h->ProfileX(), h->ProjectionX(), draw);
}

double vturnon_(TH1D* h) { 
  TF1 *f = h->GetFunction("turnon");
  if (f!=0) return f->GetX(f->GetMaximum()-0.01);
  else return -9999;
}

double vturnon_(TF1* f) { 
  if (f!=0) return f->GetX(f->GetMaximum()-0.01);
  else return -9999;
}

void add_turnon_fits_(TCanvas* c, std::vector<TH2D*> parents, double xmin = -2.5, double xmax = 202.5, double ymin=0, double ymax=1 ) {
  double pave_size = 0.28;
  TPave *p = new TPave(xmax-(xmax-xmin)*pave_size, ymax-((parents.size()+2.25)*(ymax-ymin)/20.0), xmax, ymax-(ymax-ymin)/20.0, 4, "bl");
  p->SetFillColor(0); p->SetShadowColor(0); p->Draw();
  std::stringstream ss; ss<<"V (1% Ineff)";
  TLatex* lat = new TLatex(xmax-(xmax-xmin)*(pave_size-0.02), ymax-(0+1.25*(ymax-ymin)/20.0), ss.str().c_str());
  lat->SetTextAlign(13); lat->SetTextSize(0.04); lat->SetTextFont(42); lat->SetLineWidth(2); lat->Draw("SAME");
  for (size_t i=0; i<parents.size(); ++i) {
    TF1 *fit = fit_turnon_improved_(parents[i]); 
    int color = ((TGraphAsymmErrors*)c->GetListOfPrimitives()->At(i))->GetLineColor();
    fit->SetLineColor(color);
    fit->SetLineWidth(1);
    //fit->SetLineStyle(7);
    fit->Draw("SAME");
    gPad->Update();
    std::stringstream ss; ss<<std::setprecision(3)<<vturnon_(fit);
    TLatex* lat = new TLatex(xmax-(xmax-xmin)*(pave_size-0.1), ymax-((i+2.25)*(ymax-ymin)/20.0), ss.str().c_str());
    lat->SetTextAlign(13); lat->SetTextSize(0.04); lat->SetTextFont(42); lat->SetLineWidth(2);
    lat->SetTextColor(color); lat->Draw("SAME");
  }
}

void add_mpvturnon_fits_(TCanvas* c, int nhist = 5, double xmin = -2.5, double xmax = 202.5, double ymin=0, double ymax=1 ) {
  for (int i=0; i<nhist; ++i) {
    TH1D* mpv = (TH1D*)c->GetListOfPrimitives()->At(i);
    double maximum = mpv->GetMaximum();
    mpv->Scale(1.0/maximum);
    TF1* f = new TF1("mpvturnon", "([0]+[1]*x)/(1+exp(([2]-x)/[3]))", 10, 300);
    f->SetParLimits(0,0.9,1);
    f->SetParLimits(1,0.0001,0.01);
    f->SetParLimits(2,0,100);
    f->SetParLimits(3,0,80);
    mpv->Fit("mpvturnon","RMQ0");
    mpv->Scale(maximum);
    // Redraw the function after rescaling
    f = new TF1("mpvturnon2", "([0]+[1]*x)/(1+exp(([2]-x)/[3]))", 10, 300);
    f->SetParLimits(0,0.8*maximum,1.1*maximum);
    f->SetParLimits(1,0.0001*maximum,0.01*maximum);
    f->SetParLimits(2,0,100);
    f->SetParLimits(3,0,80);
    mpv->Fit("mpvturnon2","RMQ0");
    f->Draw("SAME");
  }
}

void usual(std::string name, double xmin=-1, double xmax=-1, double ymin=-1, double ymax=-1, double leg_x1=-1, double leg_y2=-1) {
  if (!f) std::cout<<"No such file exist\n"<<std::endl;
  else {
    c = (TCanvas*)f->Get(name.c_str());
    if (!c) std::cout<<"Canvas doesn't exist: "<<name<<std::endl;
    else {
      c = (TCanvas*)c->Clone();
      c->Draw();
      bool ratio = std::string(c->GetName()).find("Ratio")!=std::string::npos;
      bool eff = std::string(c->GetName()).find("Eff")!=std::string::npos;
      if (ratio) {
	TVirtualPad* pad = (TVirtualPad*)c->GetListOfPrimitives()->At(0);
	if (leg_x1>=0&&leg_y2>=0) move_legend(c, pad->GetListOfPrimitives()->GetEntries()-2, leg_x1, leg_y2);
	if (ymin!=ymax||xmin!=xmax) {
	  TH1D* h = (TH1D*)pad->GetListOfPrimitives()->At(1);
	  if (xmin!=xmax) h->GetXaxis()->SetRangeUser(xmin, xmax);
	  if (ymin!=ymax) h->GetYaxis()->SetRangeUser(ymin, ymax);
	  if (ymin==0 ? 0 : ymax/ymin>1e5) c->SetLogy(1);
	  h = (TH1D*)((TVirtualPad*)c->GetListOfPrimitives()->At(1))->GetListOfPrimitives()->At(0);
	  if (xmin!=xmax) h->GetXaxis()->SetRangeUser(xmin, xmax);
	  TLine* l = (TLine*)((TVirtualPad*)c->GetListOfPrimitives()->At(1))->GetListOfPrimitives()->At(2);
	  l->SetX1(xmin); l->SetX2(xmax);
	}
      } else {
	TVirtualPad* pad = (TVirtualPad*)c->GetListOfPrimitives()->At(0);
	if (leg_x1>=0&&leg_y2>=0) move_legend(c, pad->GetListOfPrimitives()->GetEntries()-1, leg_x1, leg_y2);
	if (ymin!=ymax||xmin!=xmax) {
	  if (!eff) {
	    TH1D* h = (TH1D*)c->GetListOfPrimitives()->At(0);
	    if (xmin!=xmax) h->GetXaxis()->SetRangeUser(xmin, xmax);
	    if (ymin!=ymax) h->GetYaxis()->SetRangeUser(ymin, ymax);
	    if (ymin==0 ? 0 : ymax/ymin>1e5) c->SetLogy(1);
	  }
	}
      }
      if (save) save_plots(c, save_dir, formats, remove_prelim_label);
    }
  }
}

void save_1d(bool save, std::string ext, std::string in, std::string out, std::string canname,
	     float xlow=-1, float xhigh=-1, float ylow=-1, float yhigh=-1,
	     float x1=-1, float x2=-1, float y1=-1, float y2=-1) {
  TFile *f = TFile::Open(in.c_str());
  if (!f) std::cout<<"No such file exist:\n"<<in<<std::endl;
  else {
    TCanvas* c = (TCanvas*)f->Get(canname.c_str());
    bool ratio = canname.find("Ratio")!=std::string::npos;
    bool eff = canname.find("Eff")!=std::string::npos;
    if (!c) std::cout<<"No such canvas exist:\n"<<canname<<std::endl;
    else {
      c = (TCanvas*)c->Clone();
      if (ratio) {
	TVirtualPad* pad = (TVirtualPad*)c->GetListOfPrimitives()->At(0);
	TLegend *leg = (TLegend*)pad->GetListOfPrimitives()->At(pad->GetListOfPrimitives()->GetEntries()-2);
	if (x1!=-1) leg->SetX1(x1);
	if (x2!=-1) leg->SetX2(x2);
	if (y1!=-1) leg->SetY1(y1);
	if (y2!=-1) leg->SetY2(y2);
	c->Draw();
	if (ylow!=yhigh||xlow!=xhigh) {
	  TH1D* h = (TH1D*)pad->GetListOfPrimitives()->At(1);
	  if (xlow!=xhigh) h->GetXaxis()->SetRangeUser(xlow, xhigh);
	  if (ylow!=yhigh) h->GetYaxis()->SetRangeUser(ylow, yhigh);
	  if (ylow==0 ? 0 : yhigh/ylow>1e5) c->SetLogy(1);
	  h = (TH1D*)((TVirtualPad*)c->GetListOfPrimitives()->At(1))->GetListOfPrimitives()->At(0);
	  if (xlow!=xhigh) h->GetXaxis()->SetRangeUser(xlow, xhigh);
	  TLine* l = (TLine*)((TVirtualPad*)c->GetListOfPrimitives()->At(1))->GetListOfPrimitives()->At(2);
	  l->SetX1(xlow); l->SetX2(xhigh);
	}
      } else {
	TLegend *leg = (TLegend*)c->GetListOfPrimitives()->At(c->GetListOfPrimitives()->GetEntries()-1);
	if (x1!=-1) leg->SetX1(x1);
	if (x2!=-1) leg->SetX2(x2);
	if (y1!=-1) leg->SetY1(y1);
	if (y2!=-1) leg->SetY2(y2);
	c->Draw();
	if (ylow!=yhigh||xlow!=xhigh) {
	  if (!eff) {
	    TH1D* h = (TH1D*)c->GetListOfPrimitives()->At(0);
	    if (xlow!=xhigh) h->GetXaxis()->SetRangeUser(xlow, xhigh);
	    if (ylow!=yhigh) h->GetYaxis()->SetRangeUser(ylow, yhigh);
	    if (ylow==0 ? 0 : yhigh/ylow>1e5) c->SetLogy(1);
	  }
	}
      }
      if (save)	c->SaveAs((out+"."+ext).c_str());
      else std::cout<<"Showing plot: "<<canname<<std::endl;
      f->Close();
    }
  }
}

TH1D* getplot(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH1D *h= (TH1D*)f.Get(hname.Data());
  TH1D *c=(TH1D*)h->Clone();
  c->SetDirectory(0);
  return c;
}

TH2D* getplot2d(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH2D *h= (TH2D*)f.Get(hname.Data());
  TH2D *c=(TH2D*)h->Clone();
  c->SetDirectory(0);
  return c;
}

TH3D* getplot3d(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH3D *h= (TH3D*)f.Get(hname.Data());
  TH3D *c=(TH3D*)h->Clone();
  c->SetDirectory(0);
  return c;
}

void norm_to_unit_bins(TH2D* h, bool along_x=1, bool along_y=1) {
  // Get Profile Histo
  TH1D *normx, *normy;
  // Calculate normalization factor along each axis
  if (along_x) {
    normx = h->ProjectionX();
    int countx=0;
    for (int binx=1; binx<=h->GetNbinsX(); ++binx) if (normx->GetBinContent(binx)>0) countx++;
    double avgx = normx->Integral()/countx;
    for (int binx=1; binx<=h->GetNbinsX(); ++binx) if (normx->GetBinContent(binx)>0) 
      normx->SetBinContent(binx, avgx/normx->GetBinContent(binx));
  }
  if (along_y) {
    normy = h->ProjectionY();
    int county=0;
    for (int biny=1; biny<=h->GetNbinsY(); ++biny) if (normy->GetBinContent(biny)>0) county++;
    double avgy = normy->Integral()/county;
    for (int biny=1; biny<=h->GetNbinsY(); ++biny) if (normy->GetBinContent(biny)>0) 
      normy->SetBinContent(biny, avgy/normy->GetBinContent(biny));
  }
  // Normalize to uniform occupancy in the detector
  for (int binx=1; binx<=h->GetNbinsX(); ++binx) for (int biny=1; biny<=h->GetNbinsY(); ++biny) {
    double cont = h->GetBinContent(binx,biny);
    // Multiply each bin with the 2 norm factors
    if (along_x) cont *= normx->GetBinContent(binx);
    if (along_y) cont *= normy->GetBinContent(biny);
    h->SetBinContent(binx,biny, cont);
  }
}

void dress_occup_plot(TH2D* h, int lay, int phase=0, bool half_shift = 1, bool mark_zero=1, bool norm = 0) {
  // Draw Lines around modules
  if (lay>0) {
    std::vector<std::vector<int> > nladder = { { 10, 16, 22 }, { 6, 14, 22, 32 } };
    int nlad = nladder[phase][lay-1];
    for (int xsign=-1; xsign<=1; xsign+=2) for (int ysign=-1; ysign<=1; ysign+=2) {
      float xlow  = xsign * (half_shift*0.5 );
      float xhigh = xsign * (half_shift*0.5 + 4 );
      float ylow  = ysign * (half_shift*0.5 + (phase==0)*0.5 );
      float yhigh = ysign * (half_shift*0.5 - (phase==0)*0.5 + nlad);
      // Outside box
      draw_line(xlow,  xhigh,  ylow,  ylow, 1); // bottom
      draw_line(xlow,  xhigh, yhigh, yhigh, 1); // top
      draw_line(xlow,   xlow,  ylow, yhigh, 1); // left
      draw_line(xhigh, xhigh,  ylow, yhigh, 1); // right
      // Inner Horizontal lines
      for (int lad=1; lad<nlad; ++lad) {
	float y = ysign * (lad + half_shift*0.5);
	draw_line(xlow, xhigh,  y,  y, 1);
      }
      for (int lad=1; lad<=nlad; ++lad) if (!(phase==0&&(lad==1||lad==nlad))) {
	float y = ysign * (lad + half_shift*0.5 - 0.5);
	draw_line(xlow, xhigh,  y,  y, 1, 3);
      }
      // Inner Vertical lines
      for (int mod=1; mod<4; ++mod) {
	float x = xsign * (mod + half_shift*0.5);
	draw_line(x, x,  ylow,  yhigh, 1);
      }
      // Make a BOX around ROC 0
      // Phase 0 - ladder +1 is always non-flipped
      // Phase 1 - ladder +1 is always     flipped
      if (mark_zero) {
	for (int mod=1; mod<=4; ++mod) for (int lad=1; lad<=nlad; ++lad) {
	  bool flipped = ysign==1 ? lad%2==0 : lad%2==1;
	  if (phase==1)  flipped = !flipped;
	  int roc0_orientation = flipped ? -1 : 1;
	  if (xsign==-1) roc0_orientation *= -1;
	  if (ysign==-1) roc0_orientation *= -1;
	  float x1 = xsign * (mod+half_shift*0.5);
	  float x2 = xsign * (mod+half_shift*0.5 - 1./8);
	  float y1 = ysign * (lad+half_shift*0.5-0.5);
	  float y2 = ysign * (lad+half_shift*0.5-0.5 + roc0_orientation*1./2);
	  if (!(phase==0&&(lad==1||lad==nlad)&&xsign==-1)) {
	    draw_line(x1, x2, y1, y1, 1);
	    //draw_line(x1, x2, y2, y2, 1);
	    //draw_line(x1, x1, y1, y2, 1);
	    draw_line(x2, x2, y1, y2, 1);
	  }
	}
      }
    }
    if (norm) norm_to_unit_bins(h);
  } else {
    for (int dsk=1, ndsk=2+(phase==1); dsk<=ndsk; ++dsk) {
      for (int xsign=-1; xsign<=1; xsign+=2) for (int ysign=-1; ysign<=1; ysign+=2) {
        if (phase==0) {
          int first_roc = 3, nbin = 16;
          for (int bld=1, nbld=12; bld<=nbld; ++bld) {
            // Horizontal lines
            for (int plq=1, nplq=7; plq<=nplq; ++plq) {
              float xlow  = xsign * (half_shift*0.5 + dsk - 1   + (first_roc-3+2*plq+(plq==1))/(float)nbin);
              float xhigh = xsign * (half_shift*0.5 + dsk - 1   + (first_roc-3+2*(plq+1)-(plq==7))/(float)nbin);
              float ylow  = ysign * (half_shift*0.5 + (bld-0.5) - (2+plq/2)*0.1);
              float yhigh = ysign * (half_shift*0.5 + (bld-0.5) + (2+plq/2)*0.1);
              draw_line(xlow,  xhigh,   ylow,  ylow, 1); // bottom
              draw_line(xlow,  xhigh,  yhigh, yhigh, 1); // top
            }
            // Vertical lines
            for (int plq=1, nplq=7+1; plq<=nplq; ++plq) {
              float x     = xsign * (half_shift*0.5 + dsk - 1   + (first_roc-3+2*plq+(plq==1)-(plq==8))/(float)nbin);
              float ylow  = ysign * (half_shift*0.5 + (bld-0.5) - (2+(plq-(plq==8))/2)*0.1);
              float yhigh = ysign * (half_shift*0.5 + (bld-0.5) + (2+(plq-(plq==8))/2)*0.1);
              draw_line(x,  x,  ylow,  yhigh, 1);
            }
            // Panel 2 has dashed mid-plane
            for (int plq=2, nplq=6; plq<=nplq; ++plq) if (plq%2==0) {
              float x     = xsign * (half_shift*0.5 + dsk - 1   + (first_roc-3+2*plq+(plq==1)-(plq==8)+1)/(float)nbin);
              float ylow  = ysign * (half_shift*0.5 + (bld-0.5) - (2+(plq-(plq==8))/2)*0.1);
              float yhigh = ysign * (half_shift*0.5 + (bld-0.5) + (2+(plq-(plq==8))/2)*0.1);
              draw_line(x,  x,  ylow,  yhigh, 1, 2);
            }
            // Make a BOX around ROC 0
            for (int plq=1, nplq=7; plq<=nplq; ++plq) {
              float x1 = xsign * (half_shift*0.5 + dsk - 1   + (first_roc-3+2*plq+(plq==1))/(float)nbin);
              float x2 = xsign * (half_shift*0.5 + dsk - 1   + (first_roc-3+2*plq+(plq==1)+1)/(float)nbin);
              int sign = xsign * ysign * ((plq%2) ? 1 : -1);
              float y1 = ysign * (half_shift*0.5 + (bld-0.5) + sign *(2+plq/2)*0.1);
              float y2 = ysign * (half_shift*0.5 + (bld-0.5) + sign *(  plq/2)*0.1);
              //draw_line(x1, x2, y1, y1, 1);
              draw_line(x1, x2, y2, y2, 1);
              //draw_line(x1, x1, y1, y2, 1);
              draw_line(x2, x2, y1, y2, 1);
            }
          }
        } else if (phase==1) {
          for (int ring=1; ring<=2; ++ring) for (int bld=1, nbld=5+ring*6; bld<=nbld; ++bld) {
            float scale = (ring==1) ? 1.5 : 1;
            Color_t p1_color = 1, p2_color = 1;
            // Horizontal lines
            // Panel 2 has dashed mid-plane
            float x1      = xsign * (half_shift*0.5 + dsk - 1 + (ring-1)*0.5);
            float x2      = xsign * (half_shift*0.5 + dsk - 1 +  ring   *0.5);
            int sign = ysign;
            float y1      = ysign * (half_shift*0.5 - 0.5 + scale*bld + sign*0.5);
            float yp1_mid = ysign * (half_shift*0.5 - 0.5 + scale*bld + sign*0.25);
            float y2      = ysign * (half_shift*0.5 - 0.5 + scale*bld);
            float yp2_mid = ysign * (half_shift*0.5 - 0.5 + scale*bld - sign*0.25);
            float y3      = ysign * (half_shift*0.5 - 0.5 + scale*bld - sign*0.5);
            draw_line(x1, x2, y1,      y1,      1, 1, p1_color);
            //draw_line(x1, x2, yp1_mid, yp1_mid, 1, 3);
            draw_line(x1, x2, y2,      y2,      1, 1, p1_color);
            draw_line(x1, x2, yp2_mid, yp2_mid, 1, 2);
            draw_line(x1, x2, y3,      y3,      1, 1, p2_color);
            // Vertical lines
            float x = xsign * (half_shift*0.5 + dsk - 1 + (ring-1)*0.5);
            draw_line(x,  x,  y1,  y2, 1, 1, p1_color);
            draw_line(x,  x,  y2,  y3, 1, 1, p2_color);
            if (ring==2) {
              //draw_line(x,  x,  y2,  y3, 1, 1, p1_color);
              x         = xsign * (half_shift*0.5 + dsk);
              draw_line(x,  x,  y1,  y2, 1, 1, p1_color);
              draw_line(x,  x,  y2,  y3, 1, 1, p2_color);
            }
            // Make a BOX around ROC 0
            x1          = xsign * (half_shift*0.5 + dsk - 1 + ring*0.5 - 1/16.);
            x2          = xsign * (half_shift*0.5 + dsk - 1 + ring*0.5);
            float y1_p1 = ysign * (half_shift*0.5 - 0.5 + scale*bld + sign*0.25);
            float y2_p1 = ysign * (half_shift*0.5 - 0.5 + scale*bld + sign*0.25 + xsign*ysign*0.25);
            draw_line(x1, x2, y1_p1, y1_p1, 1);
            //draw_line(x1, x2, y2_p1, y2_p1, 1);
            draw_line(x1, x1, y1_p1, y2_p1, 1);
            //draw_line(x2, x2, y1_p1, y2_p1, 1);
            float y1_p2 = ysign * (half_shift*0.5 - 0.5 + scale*bld - sign*0.25);
            float y2_p2 = ysign * (half_shift*0.5 - 0.5 + scale*bld - sign*0.25 - xsign*ysign*0.25);
            draw_line(x1, x2, y1_p2, y1_p2, 1);
            //draw_line(x1, x2, y2_p2, y2_p2, 1);
            draw_line(x1, x1, y1_p2, y2_p2, 1);
            //draw_line(x2, x2, y1_p2, y2_p2, 1);
          }
        }
      }
    }
    // Special shifted "rebin" for Phase 0
    // Y axis should always have at least half-roc granularity because
    // there are half-ROC size shifts implemented in the coordinates
    // To remove this and show full ROC granularity
    // We merge bin contents in each pair of bins corresponding to one ROC
    if (phase==0&&h->GetNbinsY()==250&&h->GetNbinsX()==80) {
      nentries = h->GetEntries();
      for (int binx = 1; binx<=80; ++binx) {
	double sum = 0;
	for (int biny = 1; biny<=250; ++biny) {
	  bool odd_nrocy = (binx-1<40) != (((binx-1)/4)%2);
	  if (biny%2==odd_nrocy) sum+= h->GetBinContent(binx, biny);
	  else {
	    sum+= h->GetBinContent(binx, biny);
	    if (sum) {
	      h->SetBinContent(binx, biny, sum);
	      h->SetBinContent(binx, biny-1, sum);
	    }
	    sum = 0;
	  }
	}
      }
      h->SetEntries(nentries);
    }
  }
}

double stat_height = 0.15;
void set_stat_(TH1D* h, Color_t col, int i) {
  if (i<7) {
    gPad->Update();
    TPaveStats* stats = (TPaveStats*)h->FindObject("stats");
    stats->SetLineColor(col);
    stats->SetTextColor(col);
    double width = 0.3;
    stats->SetX1NDC(0.965-width);
    stats->SetX2NDC(0.965);
    stats->SetY1NDC(0.965-stat_height-i*(stat_height+0.01));
    stats->SetY2NDC(0.965            -i*(stat_height+0.01));
    gPad->Update();
  }
}

void fit_residuals_(TH1D* h) {
  TF1 *f = new TF1((std::string(h->GetName())+"_fit").c_str(),"TMath::Student((x-[0])/[1],3)*[2]",-100,100);
  f->SetParameter(0,h->GetMean());
  f->SetParameter(1,h->GetRMS());
  f->SetParameter(2,2*h->Integral());
  f->SetParName(0,"Hit Bias");
  f->SetParName(1,"Resolution");
  f->SetParName(2,"Const");
  h->Fit((std::string(h->GetName())+"_fit").c_str(),"RMQ");
  f->FixParameter(2, f->GetParameter(2));
  h->Fit((std::string(h->GetName())+"_fit").c_str(),"RMQ");
  f->SetLineWidth(1);
  f->SetLineColor(h->GetLineColor());
  f->Draw("SAME");
}

// Student's t function:
Double_t tp0Fit( Double_t *x, Double_t *par6 ) {
  static int nn = 0;
  nn++;
  static double dx = 0.1;
  static double b1 = 0;
  if( nn == 1 ) b1 = x[0];
  if( nn == 2 ) dx = x[0] - b1;
  // Mean and width:
  double xm = par6[0];
  double t = ( x[0] - xm ) / par6[1];
  double tt = t*t;
  // exponent:
  double rn = par6[2];
  double xn = 0.5 * ( rn + 1.0 );
  // Normalization needs Gamma function:
  double pk = 0.0;
  if( rn > 0.0 ) {
    double pi = 3.14159265358979323846;
    double aa = dx / par6[1] / sqrt(rn*pi) * TMath::Gamma(xn) / TMath::Gamma(0.5*rn);
    pk = par6[3] * aa * exp( -xn * log( 1.0 + tt/rn ) );
  }
  par6[5] = par6[1]/std::sqrt(1.5);
  return pk + par6[4];
}


void fit_residuals_data_(TH1D* h) {
  // Camilla's settings
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
  TF1 *f = new TF1((std::string(h->GetName())+"_fit").c_str(),tp0Fit, x1, x9, 6);
  f->SetParName(0,"Mean");
  f->SetParName(1,"#Sigma");
  f->SetParName(2,"nu");
  f->SetParName(3,"area");
  f->SetParName(4,"BG");
  f->SetParName(5,"Resolution");
  // My start parameters (tuned to MC)
  //f->SetParameter(0,h->GetMean());
  //f->SetParameter(1,h->GetRMS());
  //f->SetParameter(2,2*h->Integral());
  //f->SetParameter(3,h->Integral());
  // Camilla's parameters
  f->SetParameter(0, xmax); // peak position
  f->SetParameter(1, 4*dx); // width
  f->SetParameter(2, 2.2); // nu
  f->SetParameter(3, nn); // N
  f->SetParameter(4, bg);
  h->Fit((std::string(h->GetName())+"_fit").c_str(),"RMQ");
  // Fix some parameters and fit again just to remove these from Fit box
  f->FixParameter(1, f->GetParameter(1));
  f->FixParameter(2, f->GetParameter(2));
  f->FixParameter(3, f->GetParameter(3));
  f->FixParameter(4, f->GetParameter(4));
  h->Fit((std::string(h->GetName())+"_fit").c_str(),"RMQ", "ep");
  f->SetLineWidth(1);
  f->SetLineColor(h->GetLineColor());
  f->Draw("SAME");
}

void compare_two(const char* f1, const char* f2, std::string histo,
		 std::string axis, std::string name, std::string leg_title,
		 std::string t1 = "Generic", std::string t2 = "Template", 
		 int col1=601 /* Blue */, int col2=634 /* Dark Red */, bool data = 0) {
  TH1D* h1 = (TH1D*)((TH1D*)TFile::Open(f1)->Get(histo.c_str()))->Clone();
  TH1D* h2 = (TH1D*)((TH1D*)TFile::Open(f2)->Get(histo.c_str()))->Clone();
  h1->SetTitle("");
  c = custom_can_(h1, name);
  h1->GetXaxis()->SetTitle(axis.c_str());
  h1->SetLineColor(col1);
  h2->SetLineColor(col2);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h1->SetStats(1);
  h2->SetStats(1);
  h1->SetName(t1.c_str());
  h2->SetName(t2.c_str());
  h1->GetXaxis()->SetRangeUser(-100,100);
  h1->Draw("HIST");
  h2->Draw("HIST SAMES");
  if (data) {
    fit_residuals_data_(h1);
    fit_residuals_data_(h2);
  } else {
    fit_residuals_(h1);
    fit_residuals_(h2);
  }
  set_stat_(h1, col1,0);
  set_stat_(h2, col2,1);
  TLegend *leg = new TLegend(0.15,0.75,0.35,0.9,leg_title.c_str());
  leg->AddEntry(h1, (std::string("#color[")+std::to_string(col1)+"]{"+t1+"}").c_str(), "L");
  leg->AddEntry(h2, (std::string("#color[")+std::to_string(col2)+"]{"+t2+"}").c_str(), "L");
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->Draw("SAME");
  if (save) save_plots(c, save_dir, formats, remove_prelim_label);
}

void compare_two(const char* file, std::string histo1, std::string histo2,
		 std::string axis, std::string name, std::string leg_title,
		 std::string t1 = "Generic", std::string t2 = "Template", 
		 int col1=601 /* Blue */, int col2=634 /* Dark Red */, bool data = 0) {
  TFile* f = TFile::Open(file);
  TH1D* h1 = (TH1D*)((TH1D*)f->Get(histo1.c_str()))->Clone();
  TH1D* h2 = (TH1D*)((TH1D*)f->Get(histo2.c_str()))->Clone();
  h1->SetTitle("");
  c = custom_can_(h1, name);
  h1->GetXaxis()->SetTitle(axis.c_str());
  h1->SetLineColor(col1);
  h2->SetLineColor(col2);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h1->SetStats(1);
  h2->SetStats(1);
  h1->SetName(t1.c_str());
  h2->SetName(t2.c_str());
  h1->GetXaxis()->SetRangeUser(-100,100);
  h1->Draw("HIST");
  h2->Draw("HIST SAMES");
  if (data) {
    fit_residuals_data_(h1);
    fit_residuals_data_(h2);
  } else {
    fit_residuals_(h1);
    fit_residuals_(h2);
  }
  set_stat_(h1, col1,0);
  set_stat_(h2, col2,1);
  TLegend *leg = new TLegend(0.15,0.75,0.35,0.9,leg_title.c_str());
  leg->AddEntry(h1, (std::string("#color[")+std::to_string(col1)+"]{"+t1+"}").c_str(), "L");
  leg->AddEntry(h2, (std::string("#color[")+std::to_string(col2)+"]{"+t2+"}").c_str(), "L");
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->Draw("SAME");
  if (save) save_plots(c, save_dir, formats, remove_prelim_label);
}

void compare_three(const char* f1, const char* f2, const char* f3, std::string histo,
		   std::string axis, std::string name, std::string leg_title,
		   std::string t1 = "2016 v2 (Prompt-GT)", std::string t2 = "2016 v3", std::string t3 = "2016 v4", 
		   int col1=634 /* Dark Red */, int col2=601 /* Blue */, int col3 =418 /* Dark Green */, bool data = 0) {
  TH1D* h1 = (TH1D*)((TH1D*)TFile::Open(f1)->Get(histo.c_str()))->Clone();
  TH1D* h2 = (TH1D*)((TH1D*)TFile::Open(f2)->Get(histo.c_str()))->Clone();
  TH1D* h3 = (TH1D*)((TH1D*)TFile::Open(f3)->Get(histo.c_str()))->Clone();
  h1->SetTitle("");
  c = custom_can_(h1, name);
  h1->GetYaxis()->SetTitle("Hits");
  h1->GetXaxis()->SetTitle(axis.c_str());
  h1->SetLineColor(col1);
  h2->SetLineColor(col2);
  h3->SetLineColor(col3);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  h1->SetStats(1);
  h2->SetStats(1);
  h3->SetStats(1);
  h1->SetName(t1.c_str());
  h2->SetName(t2.c_str());
  h3->SetName(t3.c_str());
  h1->GetXaxis()->SetRangeUser(-100,100);
  h1->Draw("HIST");
  h2->Draw("HIST SAMES");
  h3->Draw("HIST SAMES");
  if (data) {
    fit_residuals_data_(h1);
    fit_residuals_data_(h2);
    fit_residuals_data_(h3);
  } else {
    fit_residuals_(h1);
    fit_residuals_(h2);
    fit_residuals_(h3);
  }
  set_stat_(h1, col1,0);
  set_stat_(h2, col2,1);
  set_stat_(h3, col3,2);
  TLegend *leg = new TLegend(0.15,0.75,0.35,0.9,leg_title.c_str());
  leg->AddEntry(h1, (std::string("#color[")+std::to_string(col1)+"]{"+t1+"}").c_str(), "L");
  leg->AddEntry(h2, (std::string("#color[")+std::to_string(col2)+"]{"+t2+"}").c_str(), "L");
  leg->AddEntry(h3, (std::string("#color[")+std::to_string(col3)+"]{"+t3+"}").c_str(), "L");
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->Draw("SAME");
  if (save) save_plots(c, save_dir, formats, remove_prelim_label);
}

