void print_residuals(std::string filename = "job.root", std::string run="282735") {
  TFile *f = TFile::Open(filename.c_str());
  TH1D *h_bpix_x = (TH1D*)f->Get((std::string("Histos/Run")+run+"/h420").c_str());
  TH1D *h_bpix_y = (TH1D*)f->Get((std::string("Histos/Run")+run+"/h421").c_str());
  std::string cond1, cond2;
  if (TString(filename).Contains("_201")) cond1 = filename.substr(filename.find("_201")+1,7);
  if (TString(filename).Contains("_Tem")) cond2 = "Templates";
  if (TString(filename).Contains("_Gen")) cond2 = "Generic";
  if (TString(filename).Contains("_GenErrOnly")) cond2 = "GenErrors";
  if (TString(filename).Contains("_LAOnly")) cond2 = "LA";
  printf("Condition         BPix-X Mean  RMS  BPix-Y Mean  RMS\n");
  printf("%7s %-9s        %2.1f   %+2.1f        %+2.1f  %2.1f\n", cond1.c_str(), cond2.c_str(),
	 h_bpix_x->GetMean(), h_bpix_x->GetRMS(), h_bpix_y->GetMean(), h_bpix_y->GetRMS());
  gApplication->Terminate();
}
