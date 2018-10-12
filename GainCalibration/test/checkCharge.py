import ROOT as rt
from time import sleep 
f1 = rt.TFile.Open("/afs/cern.ch/user/d/dkotlins/public/CMSSW/CMSSW_10_2_0/src/DPGAnalysis-SiPixelTools/HitAnalyzer/scripts/digis_clus_323391_def.root","READ") #-- post PH optimisation DAC change
f2 = rt.TFile.Open("/afs/cern.ch/user/d/dkotlins/public/CMSSW/CMSSW_10_2_0/src/DPGAnalysis-SiPixelTools/HitAnalyzer/scripts/digis_clus_323391_v8.root","READ") #-- post PH optimisation DAC change
f3 = rt.TFile.Open("/afs/cern.ch/user/d/dkotlins/public/CMSSW/CMSSW_10_2_0/src/DPGAnalysis-SiPixelTools/HitAnalyzer/scripts/digis_clus_323416_def.root","READ") # -- old PH settings
f4 = rt.TFile.Open("/afs/cern.ch/user/d/dkotlins/public/CMSSW/CMSSW_10_2_0/src/DPGAnalysis-SiPixelTools/HitAnalyzer/scripts/digis_clus_323416_v7.root","READ") # -- old PH settings

hists = ['hcharge1',"hcharge2","hpixcharge1","hpixcharge2",'hcharge5','hcharge6','hcharge7']

H_ref = 600
W_ref = 800
W = W_ref
H  = H_ref
T = 0.08*H_ref
B = 0.12*H_ref 
L = 0.12*W_ref
R = 0.04*W_ref

##############################
def getCanvas(name="c1"):
    
    canvas = rt.TCanvas(name,name,50,50,W,H)
    canvas.SetFillColor(0)
    canvas.SetBorderMode(0)
    canvas.SetFrameFillStyle(0)
    canvas.SetFrameBorderMode(0)
    canvas.SetLeftMargin( L/W )
    canvas.SetRightMargin( R/W )
    canvas.SetTopMargin( 0.9*T/H )
    canvas.SetBottomMargin( 1.1*B/H )
    canvas.SetTickx(0)
    canvas.SetTicky(0)
    
    rt.gStyle.SetOptStat(0)
    rt.gStyle.SetOptTitle(0)
    canvas.cd()
    legend = rt.TLegend(0.62,0.72,0.92,0.92,"","brNDC")
    legend.SetBorderSize(0)
    legend.SetLineColor(1)
    legend.SetLineStyle(1)
    legend.SetLineWidth(1)
    legend.SetFillColor(0)
    legend.SetFillStyle(0)
    legend.SetTextFont(42)
    
    pt = rt.TPaveText(0.6893233,0.1829268,0.8095489,0.452439,"brNDC")
    pt.SetTextFont(42)
    pt.SetTextSize(0.040)
    pt.SetTextAlign(12)
    pt.SetFillColor(0)
    pt.SetLineColor(0)
    pt.SetFillStyle(0)
    pt.SetBorderSize(0)
    
    
    return canvas,legend, pt
    
for h in hists:
  h1 = f1.Get("d/"+h)
  h2 = f2.Get("d/"+h)
  h3 = f3.Get("d/"+h)
  h4 = f4.Get("d/"+h)
  
  h1.SetLineColor(435)
  h2.SetLineColor(435)
  h3.SetLineColor(626)
  h4.SetLineColor(626)
  
  h1.SetLineWidth(2)
  h2.SetLineWidth(2)
  h3.SetLineWidth(2)
  h4.SetLineWidth(2)
  
  h1.SetLineStyle(2)
  h2.SetLineStyle(1)
  h3.SetLineStyle(2)
  h4.SetLineStyle(1)
  
  
  h1.GetXaxis().SetTitle(h)
  h1.GetYaxis().SetTitle("N pixels")
  h1.GetYaxis().SetRangeUser(0,h2.GetMaximum()*1.2)
  c,l,p = getCanvas()
  l.AddEntry(h1,"Old gain (Post PH opt run)","L")
  l.AddEntry(h2,"New v8 (Post PH opt run)","L")
  l.AddEntry(h3,"Old gain (Old PH settings run)","L")
  l.AddEntry(h4,"New v7 (Old PH settings run)","L")
  
  h1.DrawNormalized("HIST")
  h2.DrawNormalized("sameHIST")
  h3.DrawNormalized("sameHIST")
  h4.DrawNormalized("sameHIST")
  l.Draw("same")
  c.SaveAs(h+"_norm.pdf")
  