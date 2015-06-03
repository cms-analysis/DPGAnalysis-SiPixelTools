{
  const bool lookAtAlignmentErrors = false;

  // _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitBPIX");
  // //gROOT->ProcessLine(".L macros/fittp0.C");
  // //fittp0("h420");

  // RecHit_XRes_Layer1->Fit("gaus");
  // float rms = RecHit_XRes_Layer1->GetRMS();
  // cout<<" layer 1 x RMS = "<<rms<<endl;

  // RecHit_XRes_Layer2->Fit("gaus");
  // rms = RecHit_XRes_Layer2->GetRMS();
  // cout<<" layer 2 x RMS = "<<rms<<endl;

  // RecHit_XRes_Layer3->Fit("gaus");
  // rms = RecHit_XRes_Layer3->GetRMS();
  // cout<<" layer 3 x RMS = "<<rms<<endl;

  // RecHit_XRes_Layer4->Fit("gaus");
  // rms = RecHit_XRes_Layer4->GetRMS();
  // cout<<" layer 4 x RMS = "<<rms<<endl;

  // RecHit_YRes_Layer1->Fit("gaus");
  // rms = RecHit_YRes_Layer1->GetRMS();
  // cout<<" layer 1 y RMS = "<<rms<<endl;

  // RecHit_YRes_Layer2->Fit("gaus");
  // rms = RecHit_YRes_Layer2->GetRMS();
  // cout<<" layer 2 y RMS = "<<rms<<endl;

  // RecHit_YRes_Layer3->Fit("gaus");
  // rms = RecHit_YRes_Layer3->GetRMS();
  // cout<<" layer 3 y RMS = "<<rms<<endl;

  // RecHit_YRes_Layer4->Fit("gaus");
  // rms = RecHit_YRes_Layer4->GetRMS();
  // cout<<" layer 4 y RMS = "<<rms<<endl;

  // if(lookAtAlignmentErrors) {

  //   rms = RecHitXAlignError1->GetMean();
  //   cout<<" layer 1 x alignment error = "<<rms<<endl;
  //   rms = RecHitXAlignError2->GetMean();
  //   cout<<" layer 2 x alignment error = "<<rms<<endl;
  //   rms = RecHitXAlignError3->GetMean();
  //   cout<<" layer 3 x alignment error = "<<rms<<endl;
  //   rms = RecHitYAlignError1->GetMean();
  //   cout<<" layer 1 y alignment error = "<<rms<<endl;
  //   rms = RecHitYAlignError2->GetMean();
  //   cout<<" layer 2 y alignment error = "<<rms<<endl;
  //   rms = RecHitYAlignError3->GetMean();
  //   cout<<" layer 3 y alignment error = "<<rms<<endl;

  //   _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitPullsBPIX");
  //   rms = recHitXError1B->GetMean(); cout<<" bpix1 x error x = "<<rms;
  //   rms = recHitYError1B->GetMean(); cout<<", y = "<<rms<<endl;
  //   rms = recHitXError2B->GetMean(); cout<<" bpix2 x error x = "<<rms;
  //   rms = recHitYError2B->GetMean(); cout<<", y = "<<rms<<endl;
  //   rms = recHitXError3B->GetMean(); cout<<" bpix3 x error x = "<<rms;
  //   rms = recHitYError3B->GetMean(); cout<<", y = "<<rms<<endl;

  // }

  _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitFPIX");

  // RecHit_xres_f_All->Fit("gaus");
  // rms = RecHit_xres_f_All->GetRMS();
  // cout<<" disk x RMS = "<<rms<<endl;

  // RecHit_yres_f_All->Fit("gaus");
  // rms = RecHit_yres_f_All->GetRMS();
  // cout<<" disk y RMS = "<<rms<<endl;

  // RecHit_XRes_Disk11->Fit("gaus");
  // rms = RecHit_XRes_Disk11->GetRMS();
  // cout<<" disk 1 x RMS = "<<rms<<endl;

  // RecHit_YRes_Disk11->Fit("gaus");
  // rms = RecHit_YRes_Disk11->GetRMS();
  // cout<<" disk 1 y RMS = "<<rms<<endl;

  // RecHit_XRes_Disk21->Fit("gaus");
  // rms = RecHit_XRes_Disk21->GetRMS();
  // cout<<" disk 2 x RMS = "<<rms<<endl;

  // RecHit_YRes_Disk21->Fit("gaus");
  // rms = RecHit_YRes_Disk21->GetRMS();
  // cout<<" disk 2 y RMS = "<<rms<<endl;

  // RecHit_XRes_Disk31->Fit("gaus");
  // rms = RecHit_XRes_Disk31->GetRMS();
  // cout<<" disk 3 x RMS = "<<rms<<endl;

  // RecHit_YRes_Disk31->Fit("gaus");
  // rms = RecHit_YRes_Disk31->GetRMS();
  // cout<<" disk 3 y RMS = "<<rms<<endl;

  // panel 1
  RecHit_XRes_Disk14->Fit("gaus");
  float rms = RecHit_XRes_Disk14->GetRMS();
  cout<<" disk 1 x panel 1  RMS = "<<rms<<endl;

  RecHit_YRes_Disk14->Fit("gaus");
  rms = RecHit_YRes_Disk14->GetRMS();
  cout<<" disk 1 y panel 1 RMS = "<<rms<<endl;

  RecHit_XRes_Disk24->Fit("gaus");
  rms = RecHit_XRes_Disk24->GetRMS();
  cout<<" disk 2 x panel 1  RMS = "<<rms<<endl;

  RecHit_YRes_Disk24->Fit("gaus");
  rms = RecHit_YRes_Disk24->GetRMS();
  cout<<" disk 2 y panel 1 RMS = "<<rms<<endl;

  RecHit_XRes_Disk34->Fit("gaus");
  rms = RecHit_XRes_Disk34->GetRMS();
  cout<<" disk 3 x panel 1  RMS = "<<rms<<endl;

  RecHit_YRes_Disk34->Fit("gaus");
  rms = RecHit_YRes_Disk34->GetRMS();
  cout<<" disk 3 y panel 1 RMS = "<<rms<<endl;


  // panel 2
  RecHit_XRes_Disk15->Fit("gaus");
  rms = RecHit_XRes_Disk15->GetRMS();
  cout<<" disk 1 x panel 2  RMS = "<<rms<<endl;

  RecHit_YRes_Disk15->Fit("gaus");
  rms = RecHit_YRes_Disk15->GetRMS();
  cout<<" disk 1 y panel 2 RMS = "<<rms<<endl;

  RecHit_XRes_Disk25->Fit("gaus");
  rms = RecHit_XRes_Disk25->GetRMS();
  cout<<" disk 2 x panel 2  RMS = "<<rms<<endl;

  RecHit_YRes_Disk25->Fit("gaus");
  rms = RecHit_YRes_Disk25->GetRMS();
  cout<<" disk 2 y panel 2 RMS = "<<rms<<endl;

  RecHit_XRes_Disk35->Fit("gaus");
  rms = RecHit_XRes_Disk35->GetRMS();
  cout<<" disk 3 x panel 2  RMS = "<<rms<<endl;

  RecHit_YRes_Disk35->Fit("gaus");
  rms = RecHit_YRes_Disk35->GetRMS();
  cout<<" disk 3 y panel 2 RMS = "<<rms<<endl;

}
