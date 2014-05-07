{
  _file0.cd("Histos/Run1");
  gROOT->ProcessLine(".L macros/fittp0.C");
  fittp0("h420");
  float rms = h420.GetRMS();
  cout<<" RMS = "<<rms<<endl;

  fittp0("h421");
  rms = h421.GetRMS();
  cout<<" RMS = "<<rms<<endl;

  float mean = h077.GetMean();
  cout<<" Mean h077 = "<<mean<<endl;

  mean = h078.GetMean();
  cout<<" Mean h078 = "<<mean<<endl;

  rms = h079.GetRMS();
  cout<<" RMS h079 = "<<rms<<endl;

  rms = h069.GetRMS();
  cout<<" RMS h069 = "<<rms<<endl;
}
