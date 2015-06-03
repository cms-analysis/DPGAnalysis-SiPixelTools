{
  gStyle->Reset();
  TCanvas *c100 =new TCanvas("c100","different scales hists",600,400);
  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(1111);
  const bool bpix = false;

  if(bpix) {
  _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitBPIX");

  c100->Clear();
  c100->Divide(2,2);
  c100_1->cd();
  //RecHit_XRes_Layer1->Draw();
  RecHit_XRes_Layer2->Draw();
  c100_2->cd();
  //RecHit_YRes_Layer1->Draw();
  RecHit_YRes_Layer2->Draw();

  c100_3->cd();
  RecHit_XRes_Layer3->Draw();
  c100_4->cd();
  RecHit_YRes_Layer3->Draw();

  } else {

    _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitFPIX");
    
    c100->Clear();
    c100->Divide(2,2);
    c100_1->cd();
    RecHit_XRes_Disk11->Draw();
    c100_2->cd();
    RecHit_YRes_Disk11->Draw();
    c100_3->cd();
    RecHit_XRes_Disk21->Draw();
    c100_4->cd();
    RecHit_YRes_Disk21->Draw();

  }

}
