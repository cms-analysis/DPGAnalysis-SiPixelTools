{
  const bool lookAtAlignmentErrors = false;

  _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitBPIX");
  //gROOT->ProcessLine(".L macros/fittp0.C");
  //fittp0("h420");

  RecHit_XRes_Layer1->Fit("gaus");
  float rms = RecHit_XRes_Layer1->GetRMS();
  cout<<" layer 1 x RMS = "<<rms<<endl;

  RecHit_XRes_Layer2->Fit("gaus");
  rms = RecHit_XRes_Layer2->GetRMS();
  cout<<" layer 2 x RMS = "<<rms<<endl;

  RecHit_XRes_Layer3->Fit("gaus");
  rms = RecHit_XRes_Layer3->GetRMS();
  cout<<" layer 3 x RMS = "<<rms<<endl;

  RecHit_XRes_Layer4->Fit("gaus");
  rms = RecHit_XRes_Layer4->GetRMS();
  cout<<" layer 4 x RMS = "<<rms<<endl;

  RecHit_YRes_Layer1->Fit("gaus");
  rms = RecHit_YRes_Layer1->GetRMS();
  cout<<" layer 1 y RMS = "<<rms<<endl;

  RecHit_YRes_Layer2->Fit("gaus");
  rms = RecHit_YRes_Layer2->GetRMS();
  cout<<" layer 2 y RMS = "<<rms<<endl;

  RecHit_YRes_Layer3->Fit("gaus");
  rms = RecHit_YRes_Layer3->GetRMS();
  cout<<" layer 3 y RMS = "<<rms<<endl;

  RecHit_YRes_Layer4->Fit("gaus");
  rms = RecHit_YRes_Layer4->GetRMS();
  cout<<" layer 4 y RMS = "<<rms<<endl;

  if(lookAtAlignmentErrors) {

    rms = RecHitXAlignError1->GetMean();
    cout<<" layer 1 x alignment error = "<<rms<<endl;
    rms = RecHitXAlignError2->GetMean();
    cout<<" layer 2 x alignment error = "<<rms<<endl;
    rms = RecHitXAlignError3->GetMean();
    cout<<" layer 3 x alignment error = "<<rms<<endl;
    rms = RecHitYAlignError1->GetMean();
    cout<<" layer 1 y alignment error = "<<rms<<endl;
    rms = RecHitYAlignError2->GetMean();
    cout<<" layer 2 y alignment error = "<<rms<<endl;
    rms = RecHitYAlignError3->GetMean();
    cout<<" layer 3 y alignment error = "<<rms<<endl;

    _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitPullsBPIX");
    rms = recHitXError1B->GetMean(); cout<<" bpix1 x error x = "<<rms;
    rms = recHitYError1B->GetMean(); cout<<", y = "<<rms<<endl;
    rms = recHitXError2B->GetMean(); cout<<" bpix2 x error x = "<<rms;
    rms = recHitYError2B->GetMean(); cout<<", y = "<<rms<<endl;
    rms = recHitXError3B->GetMean(); cout<<" bpix3 x error x = "<<rms;
    rms = recHitYError3B->GetMean(); cout<<", y = "<<rms<<endl;

  }

  _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitFPIX");

  RecHit_xres_f_All->Fit("gaus");
  rms = RecHit_xres_f_All->GetRMS();
  cout<<" disk x RMS = "<<rms<<endl;

  RecHit_yres_f_All->Fit("gaus");
  rms = RecHit_yres_f_All->GetRMS();
  cout<<" disk y RMS = "<<rms<<endl;

  RecHit_XRes_Disk11->Fit("gaus");
  rms = RecHit_XRes_Disk11->GetRMS();
  cout<<" disk 1 x RMS = "<<rms<<endl;

  RecHit_YRes_Disk11->Fit("gaus");
  rms = RecHit_YRes_Disk11->GetRMS();
  cout<<" disk 1 y RMS = "<<rms<<endl;

  RecHit_XRes_Disk21->Fit("gaus");
  rms = RecHit_XRes_Disk21->GetRMS();
  cout<<" disk 2 x RMS = "<<rms<<endl;

  RecHit_YRes_Disk21->Fit("gaus");
  rms = RecHit_YRes_Disk21->GetRMS();
  cout<<" disk 2 y RMS = "<<rms<<endl;

  RecHit_XRes_Disk31->Fit("gaus");
  rms = RecHit_XRes_Disk31->GetRMS();
  cout<<" disk 3 x RMS = "<<rms<<endl;

  RecHit_YRes_Disk31->Fit("gaus");
  rms = RecHit_YRes_Disk31->GetRMS();
  cout<<" disk 3 y RMS = "<<rms<<endl;

  // -Z
  RecHit_XRes_Disk12->Fit("gaus");
  rms = RecHit_XRes_Disk12->GetRMS();
  cout<<" disk 1 x -z RMS = "<<rms<<endl;

  RecHit_YRes_Disk12->Fit("gaus");
  rms = RecHit_YRes_Disk12->GetRMS();
  cout<<" disk 1 y -z RMS = "<<rms<<endl;

  RecHit_XRes_Disk22->Fit("gaus");
  rms = RecHit_XRes_Disk22->GetRMS();
  cout<<" disk 2 x -z RMS = "<<rms<<endl;

  RecHit_YRes_Disk22->Fit("gaus");
  rms = RecHit_YRes_Disk22->GetRMS();
  cout<<" disk 2 y -z RMS = "<<rms<<endl;

  RecHit_XRes_Disk32->Fit("gaus");
  rms = RecHit_XRes_Disk32->GetRMS();
  cout<<" disk 3 x -z RMS = "<<rms<<endl;

  RecHit_YRes_Disk32->Fit("gaus");
  rms = RecHit_YRes_Disk32->GetRMS();
  cout<<" disk 3 y -z RMS = "<<rms<<endl;

  // +Z
  RecHit_XRes_Disk13->Fit("gaus");
  rms = RecHit_XRes_Disk13->GetRMS();
  cout<<" disk 1 x +z RMS = "<<rms<<endl;

  RecHit_YRes_Disk13->Fit("gaus");
  rms = RecHit_YRes_Disk13->GetRMS();
  cout<<" disk 1 y +z RMS = "<<rms<<endl;

  RecHit_XRes_Disk23->Fit("gaus");
  rms = RecHit_XRes_Disk23->GetRMS();
  cout<<" disk 2 x +z RMS = "<<rms<<endl;

  RecHit_YRes_Disk23->Fit("gaus");
  rms = RecHit_YRes_Disk23->GetRMS();
  cout<<" disk 2 y +z RMS = "<<rms<<endl;

  RecHit_XRes_Disk33->Fit("gaus");
  rms = RecHit_XRes_Disk33->GetRMS();
  cout<<" disk 3 x +z RMS = "<<rms<<endl;

  RecHit_YRes_Disk33->Fit("gaus");
  rms = RecHit_YRes_Disk33->GetRMS();
  cout<<" disk 3 y +z RMS = "<<rms<<endl;





  if(lookAtAlignmentErrors) {
    float rms1 = RecHitXAlignError4->GetMean();
    float rms2 = RecHitYAlignError4->GetMean();
    cout<<" disk -1 x/y alignment error = "<<rms1<<"/"<<rms2<<endl;

    rms1 = RecHitXAlignError5->GetMean();
    rms2 = RecHitYAlignError5->GetMean();
    cout<<" disk +1 x/y alignment error = "<<rms1<<"/"<<rms2<<endl;

    rms1 = RecHitXAlignError6->GetMean();
    rms2 = RecHitYAlignError6->GetMean();
    cout<<" disk -2 x/y alignment error = "<<rms1<<"/"<<rms2<<endl;

    rms1 = RecHitXAlignError7->GetMean();
    rms2 = RecHitYAlignError7->GetMean();
    cout<<" disk +2 x/y alignment error = "<<rms1<<"/"<<rms2<<endl;


    _file0->cd("DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/recHitPullsFPIX");
    rms = RecHit_XError_f_All->GetMean();
    cout<<" fpix x error = "<<rms<<endl;
    rms = RecHit_YError_f_All->GetMean();
    cout<<" fpix y error = "<<rms<<endl;

  }

}
