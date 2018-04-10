import ROOT as rt
from time import sleep 
f1 = rt.TFile.Open("/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_184/GainRun_184/Summary_payload_Run184_2018_v1.root","READ")
f2 = rt.TFile.Open("/eos/cms/store/group/dpg_tracker_pixel/comm_pixel/GainCalibrations/Phase1/Run_3005551591/GainRun_3005551591/Summary_payload_Run3005551591_2017_v1337.root","READ")

hists = ['goodgains',"VCAL_endpoint","goodgains","goodpeds","totgains","totpeds"]


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
  hold = f2.Get("gainDBOffline/"+h)
  hnew = f1.Get("gainDBOffline/"+h)
  
  hold.SetLineColor(435)
  hnew.SetLineColor(626)
  hold.SetLineWidth(2)
  hnew.SetLineWidth(2)
  hold.GetXaxis().SetTitle(h)
  hold.GetYaxis().SetTitle("N pixels")
  hold.GetYaxis().SetRangeUser(0,hold.GetMaximum()*1.2)
  c,l,p = getCanvas()
  l.AddEntry(hold,"Old","L")
  l.AddEntry(hnew,"New","L")
  
  hold.Draw("HIST")
  hnew.Draw("sameHIST")
  l.Draw("same")
  c.SaveAs(h+".pdf")
  