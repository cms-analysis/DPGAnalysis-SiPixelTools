#include "header.h"


template <class T>
class PIX_FINAL : public PIX<T> {
  public:
  T n_points;
  T frac_bad_fit;
  T frac_gain_high;
};

//#########################

void get_module_summary(int nRun , TString module = "0"){
  ostringstream f_str("");
  f_str << "Run_" << nRun << "/Summary_Run" << nRun << "/Comp_Run" << nRun << ".root" ;
  TFile* f = new TFile(f_str.str().c_str() , "READ");

  PIX_FINAL<TH1F*> h;
  h.gain   		= (TH1F*) f->Get("SUM_Gain_whole");
  h.pedestal   		= (TH1F*) f->Get("SUM_Pedestal_whole");
  h.err_gain   		= (TH1F*) f->Get("SUM_ErrorGain_whole");
  h.err_pedestal   	= (TH1F*) f->Get("SUM_ErrorPedestal_whole");
  h.plateau   		= (TH1F*) f->Get("SUM_GainSaturate_whole");
  h.dynamic_range   	= (TH1F*) f->Get("SUM_GainDynamicRange_whole");
  h.low_point   	= (TH1F*) f->Get("SUM_GainLowPoint_whole");
  h.high_point   	= (TH1F*) f->Get("SUM_GainHighPoint_whole");
  h.chi2NDF   		= (TH1F*) f->Get("SUM_GainChi2NDF_whole");
  h.chi2_prob   	= (TH1F*) f->Get("SUM_GainChi2Prob_whole");
  h.fit_result   	= (TH1F*) f->Get("SUM_GainFitResult_whole");
  h.n_points   		= (TH1F*) f->Get("SUM_GainNPoints_whole");
  h.frac_bad_fit   	= (TH1F*) f->Get("SUM_frac_bad_fit_whole");
  h.frac_gain_high   	= (TH1F*) f->Get("SUM_frac_gain_high_whole");
  
  h.gain->ResetBit(TH1::kCanRebin);
  int bin = h.gain->GetXaxis()->FindBin(module);
  if (bin==0){
    cout << "Module " << module << " is not present ..." << endl;
    return;
  }
  
  cout << "Module : " << module << endl;
  cout << "gain           : " << h.gain->GetBinContent(bin) << endl;	 
  cout << "pedestal       : " << h.pedestal->GetBinContent(bin) << endl;	 
  cout << "err_gain       : " << h.err_gain->GetBinContent(bin) << endl;	 
  cout << "err_pedestal   : " << h.err_pedestal  ->GetBinContent(bin) << endl; 
  cout << "plateau        : " << h.plateau->GetBinContent(bin) << endl;	 
  cout << "dynamic_range  : " << h.dynamic_range ->GetBinContent(bin) << endl; 
  cout << "low_point      : " << h.low_point->GetBinContent(bin) << endl;	 
  cout << "high_point     : " << h.high_point  ->GetBinContent(bin) << endl; 
  cout << "chi2NDF        : " << h.chi2NDF->GetBinContent(bin) << endl;	 
  cout << "chi2_prob      : " << h.chi2_prob->GetBinContent(bin) << endl;	 
  cout << "fit_result     : " << h.fit_result ->GetBinContent(bin) << endl;  
  cout << "n_points       : " << h.n_points->GetBinContent(bin) << endl;	 
  cout << "frac_bad_fit   : " << h.frac_bad_fit  ->GetBinContent(bin) << endl; 
  cout << "frac_gain_high : " << h.frac_gain_high ->GetBinContent(bin) << endl; 
  
  f->Close();
}


//#########################

void get_module_info(int module , int& iFED , TString& dir){
  ifstream t("module_info.txt");
  while(! t.eof()){
    int m ; int f ; TString d;
    t >> m >> f >> d;
    if(m==module){
      iFED = f;
      dir = d;
      return;
    }  
  }
  cout << "[get_module_info] ERROR !! module " << module << " can't be found ..." << endl;
  exit(0);
}

void print_module_info(int module){
  int fed; TString dir;
  get_module_info(module , fed , dir);
  cout << "Module " << module << " is in FED " << fed << " and dir " << dir << endl;
}


void print_pix_info(int nRun , int module , int row , int col , TString f_str = ""){
  ostringstream cfg_str("");
  cfg_str << "Run_" << nRun << "/config";
  CFG cfg(cfg_str.str());
  //cfg.print();
  
  int fed; TString dir;
  get_module_info(module , fed , dir);
  cout << "Module " << module << " is in FED " << fed << " and dir " << dir << endl;
  
  TFile* f ;
  if(f_str == "") f = TFile::Open("rfio://"+cfg.storedir+"/GainCalibration.root","READ");
  else            f = TFile::Open(f_str,"READ");
  assert(f);
  
  ostringstream full_dir("");
  full_dir << "/DQMData/Run " << nRun << "/Pixel/Run summary/" << dir;
  
  PIX<TH2F*> h;
  h.getFromDir(f , full_dir.str().c_str());
    
  PIX<double> pix;
  pix.fill(h , col+1 , row+1);
  
  pix.print();
  f->Close();
}

//#########################

void module_info(int nRun , int module){
  ostringstream cfg_str("");
  cfg_str << "Run_" << nRun << "/config";
  CFG cfg(cfg_str.str());
  cfg.print();
  
  int FED ; TString dir;
  get_module_info(module , FED , dir);
  cout << "Looking at module " << module << " in FED " << FED << " and dir " << dir << endl;
  
  TFile* f = TFile::Open("rfio://"+cfg.storedir+"/GainCalibration.root","READ");
  assert(f);
  
  ostringstream full_dir("");
  full_dir << "/DQMData/Run " << nRun << "/Pixel/Run summary/" << dir;
  //TDirectory* ddir =  f->GetDirectory(full_dir.str().c_str());
  //assert(ddir);
  
  ostringstream h_str("");
  
  h_str << full_dir.str() << "/Gain2d_siPixelCalibDigis_" << module;
  cout << h_str.str() << endl;
  TH2F* h_gain = (TH2F*) f->Get(h_str.str().c_str());
  assert(h_gain);
  
  h_str.str("") ; h_str << full_dir.str() << "/Pedestal2d_siPixelCalibDigis_" << module;
  TH2F* h_pedestal = (TH2F*) f->Get(h_str.str().c_str());
  assert(h_pedestal);
  
  h_str.str("") ; h_str << full_dir.str() << "/GainFitResult2d_siPixelCalibDigis_" << module;
  TH2F* h_fitresult = (TH2F*) f->Get(h_str.str().c_str());
  assert(h_fitresult);
  
  
  h_gain->Draw("colz");
  gPad->WaitPrimitive();
  h_pedestal->Draw("colz");
  gPad->WaitPrimitive();
  h_fitresult->Draw("colz");
  gPad->WaitPrimitive();

}

//#########################

void create_module_info(TString dir = "/castor/cern.ch/user/r/rougny/vcalH2L_7/GainRun_173484"){
  ofstream t;
  t.open("module_info.txt");
  if(! t.is_open()){
    cout << "[create_module_info] Could not open output txt file. Exiting ..." << endl;
    exit(1);
  }

  for(int i=0 ; i<40 ; ++i){
    ostringstream f_str("");
    f_str << "rfio://" << dir << "/" << i << ".root";
    cout << "Opening file : " << f_str.str() << endl;
    TFile* f = TFile::Open(f_str.str().c_str() , "READ");
    vector<TString> dir_list = makedirlist(f , "Module");
    
    for(int m = 0 ; m < dir_list.size() ; ++m){
      
      TDirectory* dir = f->GetDirectory(dir_list[m]);
      int detid = getDetId(((TH1*) dir->Get(((TKey*) dir->GetListOfKeys()->At(0))->GetName()))->GetName());
    
      t << detid << "  " << i << "  " << dir_list[m].Remove(0,dir_list[m].First('y')+2) << endl;
    
    }


    f->Close();
  
  }

  t.close();

}




void debug_tools(){

  pix_trans.init();
  cout << "  get_module_summary(int nRun , TString module)" << endl;
  cout << "  print_module_summary(int module)" << endl;
  cout << "  create_module_info(TString dir = \"/castor/cern.ch/user/r/rougny/GainRun_184644/\")" << endl;
  cout << "  module_info(184644,302128916)" << endl;
  cout << "  print_pix_info(int nRun , int module , int row , int col , TString file)" << endl;
}





