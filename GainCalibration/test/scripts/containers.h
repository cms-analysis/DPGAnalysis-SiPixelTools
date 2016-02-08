
// *****************************************************************************************************

class map_module_level{
  public:
  vector<TH2F> barrel;
  vector<TH2F> endcap;
  
  TH1F SUM_barrel;
  TH1F SUM_endcap;
  TH1F SUM_whole;
  TH1F TOT_barrel;
  TH1F TOT_endcap;
  
  TH1F SUB_whole;
  vector<TMean> m_whole;
  
  map_module_level(TString name , int nbin = 10 , double xmin = 0 , double xmax = 10){
    barrel.assign(3,TH2F());
    barrel[0] = TH2F("MAP_"+name+"_layer1" , name+"_layer1;module;ladder" , 9 , -4.5 , 4.5 , 21 , -10.5 , 10.5);
    barrel[1] = TH2F("MAP_"+name+"_layer2" , name+"_layer2;module;ladder" , 9 , -4.5 , 4.5 , 33 , -16.5 , 16.5);
    barrel[2] = TH2F("MAP_"+name+"_layer3" , name+"_layer3;module;ladder" , 9 , -4.5 , 4.5 , 45 , -22.5 , 22.5);
    
    endcap.assign(4,TH2F());
    endcap[0] = TH2F("MAP_"+name+"_disk-2" , name+"_disk-2;2*blade+panel-1;plaquette" , 50, -24., 26., 4, 1., 5.);
    endcap[1] = TH2F("MAP_"+name+"_disk-1" , name+"_disk-1;2*blade+panel-1;plaquette" , 50, -24., 26., 4, 1., 5.);
    endcap[2] = TH2F("MAP_"+name+"_disk+1" , name+"_disk+1;2*blade+panel-1;plaquette" , 50, -24., 26., 4, 1., 5.);
    endcap[3] = TH2F("MAP_"+name+"_disk+2" , name+"_disk+2;2*blade+panel-1;plaquette" , 50, -24., 26., 4, 1., 5.);
    
    SUM_barrel = TH1F("SUM_"+name+"_barrel" , "SUM_"+name+"_barrel;;mean per module VS ordered detid" , 900 , 0 , 900);
    SUM_endcap = TH1F("SUM_"+name+"_endcap" , "SUM_"+name+"_endcap;;mean per module VS ordered detid" , 900 , 0 , 900);
    SUM_whole  = TH1F("SUM_"+name+"_whole"  , "SUM_"+name+"_whole ;;mean per module VS ordered detid" , 1450, 0 , 1450);
    
    TOT_barrel = TH1F("TOT_"+name+"_barrel" , "TOT_"+name+"_barrel;module means;# modules" , nbin , xmin , xmax);
    TOT_endcap = TH1F("TOT_"+name+"_endcap" , "TOT_"+name+"_endcap;module means;# modules" , nbin , xmin , xmax);
    
    SUB_whole  = TH1F("SUB_"+name+"_whole" , "SUB_"+name+"_whole;;mean per subdetectors" , 7 , 0 , 7);
    m_whole.assign(7 , TMean());
    
  }
  
  void fill(int detid , TString& dirname , double val){
    dirname.Class();
    int layer = 0, ladder = 0, module = 0;
    int disk = 0, blade = 0, panel = 0 , plaquette = 0 ;
    ostringstream detid_str(""); detid_str << detid;
    
    SUM_whole.Fill(detid_str.str().c_str() , val);
      
    if(pix_trans.detID2Bpix(detid , layer , ladder , module)){
      this->barrel[layer-1].Fill(module , ladder , val);
      SUM_barrel.Fill(detid_str.str().c_str() , val);
      TOT_barrel.Fill(val);
      m_whole[layer-1].Add(val);
      //cout << detid << "  " << layer << "  " << ladder << "  " << module << "  " << endl;
    }
    else if(pix_trans.detID2Fpix(detid, disk, blade, panel, plaquette)){
      if(disk>0) disk=disk-1;
      this->endcap[disk+2].Fill(2*blade + panel -1 , plaquette , val);
      SUM_endcap.Fill(detid_str.str().c_str() , val);
      TOT_endcap.Fill(val);
      m_whole[3+disk+2].Add(val);
    }
  }
  
  void Write(){
    for(unsigned i = 0 ; i < barrel.size() ; ++i)
      write(&(barrel[i]));
    for(unsigned i = 0 ; i < endcap.size() ; ++i)
      write(&(endcap[i]));
    
    
    SUM_barrel.LabelsDeflate();
    SUM_barrel.LabelsOption("a");
    SUM_barrel.GetXaxis()->SetNdivisions(-1,0);
    SUM_endcap.LabelsDeflate();
    SUM_endcap.LabelsOption("a");
    SUM_endcap.GetXaxis()->SetNdivisions(-1,0);
    SUM_whole.LabelsDeflate();
    SUM_whole.LabelsOption("a");
    SUM_whole.GetXaxis()->SetNdivisions(-1,0);
    SUM_barrel.GetXaxis()->SetTickLength(0);
    SUM_endcap.GetXaxis()->SetTickLength(0);
    SUM_whole.GetXaxis()->SetTickLength(0);
    write(&SUM_barrel,0,0,"n");
    write(&SUM_endcap,0,0,"n");
    write(&SUM_whole,0,1,"n");
    
    write(&TOT_barrel , 1);
    write(&TOT_endcap , 1);
    
    SUB_whole.Fill("layer 1" , m_whole[0].GetMean());
    SUB_whole.Fill("layer 2" , m_whole[1].GetMean());
    SUB_whole.Fill("layer 3" , m_whole[2].GetMean());
    SUB_whole.Fill("disk -2" , m_whole[3].GetMean());
    SUB_whole.Fill("disk -1" , m_whole[4].GetMean());
    SUB_whole.Fill("disk +1" , m_whole[5].GetMean());
    SUB_whole.Fill("disk +2" , m_whole[6].GetMean());
    for(int i=0 ; i< 7 ; ++i)
      SUB_whole.SetBinError( i + 1 , m_whole[i].GetRMS());
    write(&SUB_whole,0,0,"n");
    
    
  }
  
  void Divide(map_module_level& m){
    for(unsigned int i = 0 ; i < this->barrel.size() ; ++i)
      this->barrel[i].Divide(&(m.barrel[i]));
    for(unsigned int i = 0 ; i < this->endcap.size() ; ++i)
      this->endcap[i].Divide(&(m.endcap[i]));
  
    //not finished !!!
  
  }
  
  private:

};


/*class pix_val{
  public:
  
    Double_t gain , pedestal , err_gain , err_pedestal , plateau , dynamic_range , low_point , high_point , 
                 chi2NDF , chi2_prob;
    Int_t fit_result , npoints ;
    
};*/


// *****************************************************************************************************


/*template <class T=double> LEVEL{
public:
  T pix;
  T col;
  T ROC;
  T mod;
};*/


// *****************************************************************************************************

template <typename T> class PIX{

public:

 T gain;
 T pedestal;
 T err_gain;
 T err_pedestal;
 T plateau;
 T dynamic_range;
 T low_point;
 T high_point;
 T chi2NDF;
 T chi2_prob;
 T fit_result;
 
 void _new();
 void _delete();
 
 void fill(PIX<TH2F*>& , int , int){cout << "Not available for this type" << endl;};
 void fill(PIX<double>&){cout << "Not available for this type" << endl;};
 void getFromDir(TDirectory* , TString , TString){cout << "Not available for this type" << endl;};
 void getFromDir(TFile* , TString){cout << "Not available for this type" << endl;};
 void print(){cout << "Not available for this type" << endl;};

};

template < >
void PIX<double>::fill(PIX<TH2F*>& p, int xpix , int ypix){

  this->gain	      = p.gain->GetBinContent(xpix,ypix)  	  ;
  this->err_gain      = p.err_gain->GetBinContent(xpix,ypix)	  ; 
  this->pedestal      = p.pedestal->GetBinContent(xpix,ypix)	  ; 
  this->err_pedestal  = p.err_pedestal->GetBinContent(xpix,ypix)  ;
  this->plateau	      = p.plateau->GetBinContent(xpix,ypix)       ;
  this->dynamic_range = p.dynamic_range->GetBinContent(xpix,ypix) ;
  this->fit_result    = p.fit_result->GetBinContent(xpix,ypix)    ;
  this->chi2NDF	      = p.chi2NDF->GetBinContent(xpix,ypix)	  ;
  this->chi2_prob     = p.chi2_prob->GetBinContent(xpix,ypix)     ;
  this->high_point    = p.high_point->GetBinContent(xpix,ypix)    ;
  this->low_point     = p.low_point->GetBinContent(xpix,ypix)     ;

}

template < >
void PIX<TMean>::fill(PIX<double>& p){

  this->gain	      .Add( p.gain              );
  this->err_gain      .Add( p.err_gain          ); 
  this->pedestal      .Add( p.pedestal          ); 
  this->err_pedestal  .Add( p.err_pedestal      );
  this->plateau	      .Add( p.plateau           );
  this->dynamic_range .Add( p.dynamic_range     );
  this->fit_result    .Add( p.fit_result        );
  this->chi2NDF	      .Add( p.chi2NDF           );
  this->chi2_prob     .Add( p.chi2_prob         );
  this->high_point    .Add( p.high_point        );
  this->low_point     .Add( p.low_point         );

}

template < >
void PIX<TH2F*>::getFromDir(TDirectory* dir , TString keytype, TString keyname){

  /* if(keytype=="TH2F"){ */
  /*   if(keyname.Contains("Gain2d")) {		this->gain	    = (TH2F*) dir->Get(keyname); */
  /*     cout <<"Got Gain2d " << keyname <<endl;} */
  /*   if(keyname.Contains("ErrorGain2d")) {	this->err_gain	    = (TH2F*) dir->Get(keyname); */
  /*     cout <<"Got ErrorGain2d " << keyname <<endl;} */
  /*   if(keyname.Contains("Pedestal2d")) 		this->pedestal	    = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("ErrorPedestal2d")) 	this->err_pedestal  = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("GainSaturate2d")) 	this->plateau	    = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("GainDynamicRange2d")) 	this->dynamic_range = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("GainFitResult2d")) 	this->fit_result    = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("GainChi2NDF2d")) 	this->chi2NDF 	    = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("GainChi2Prob2d")) 	this->chi2_prob	    = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("GainHighPoint2d")) 	this->high_point    = (TH2F*) dir->Get(keyname); */
  /*   if(keyname.Contains("GainLowPoint2d")) 	this->low_point	    = (TH2F*) dir->Get(keyname); */
  /* }
 */

  if(keytype=="TH2F"){
    if(keyname.BeginsWith("Gain2d")) {		this->gain	    = (TH2F*) dir->Get(keyname);
      /* cout <<"Got Gain2d " << keyname <<endl; */}
    if(keyname.BeginsWith("ErrorGain2d")) {	this->err_gain	    = (TH2F*) dir->Get(keyname);
      /* cout <<"Got ErrorGain2d " << keyname <<endl; */}
    if(keyname.BeginsWith("Pedestal2d")) 		this->pedestal	    = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("ErrorPedestal2d")) 	this->err_pedestal  = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("GainSaturate2d")) 	this->plateau	    = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("GainDynamicRange2d")) 	this->dynamic_range = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("GainFitResult2d")) 	this->fit_result    = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("GainChi2NDF2d")) 	this->chi2NDF 	    = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("GainChi2Prob2d")) 	this->chi2_prob	    = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("GainHighPoint2d")) 	this->high_point    = (TH2F*) dir->Get(keyname);
    if(keyname.BeginsWith("GainLowPoint2d")) 	this->low_point	    = (TH2F*) dir->Get(keyname);
  }
  
}

template < >
void PIX<TH2F*>::getFromDir(TFile* f , TString dir_str){

  TDirectory* dir = f->GetDirectory(dir_str); 
  TList* list = dir->GetListOfKeys();
  for(int ikey=0;ikey<list->GetEntries();ikey++){
    TKey *thekey = (TKey*)list->At(ikey);
    if(thekey==0) continue;
    TString keyname=thekey->GetName();
    keyname.ReplaceAll(" ","");
    TString keytype=thekey->GetClassName();
    keytype.ReplaceAll(" ","");
    
    this->getFromDir(dir , keytype , keyname);
  }
}    
    
    
template < class T >
void PIX<T>::_new(){

  this->gain	      = new T();
  this->err_gain      = new T();
  this->pedestal      = new T();
  this->err_pedestal  = new T();
  this->plateau	      = new T();
  this->dynamic_range = new T();
  this->fit_result    = new T();
  this->chi2NDF	      = new T();
  this->chi2_prob     = new T();
  this->high_point    = new T();
  this->low_point     = new T();

}



template < class T >
void PIX<T>::_delete(){

  delete this->gain	     ;
  delete this->err_gain      ;
  delete this->pedestal      ;
  delete this->err_pedestal  ;
  delete this->plateau	     ;
  delete this->dynamic_range ;
  delete this->fit_result    ;
  delete this->chi2NDF	     ;
  delete this->chi2_prob     ;
  delete this->high_point    ;
  delete this->low_point     ;

}

template < >
void PIX<double>::print(){

  cout << "gain          : " <<  this->gain	     << endl ;
  cout << "err_gain      : " <<  this->err_gain      << endl ;
  cout << "pedestal      : " <<  this->pedestal      << endl ;
  cout << "err_pedestal  : " <<  this->err_pedestal  << endl ;
  cout << "plateau       : " <<  this->plateau       << endl ;
  cout << "dynamic_range : " <<  this->dynamic_range << endl ;
  cout << "fit_result    : " <<  this->fit_result    << endl ;
  cout << "chi2NDF       : " <<  this->chi2NDF       << endl ;
  cout << "chi2_prob     : " <<  this->chi2_prob     << endl ;
  cout << "high_point    : " <<  this->high_point    << endl ;
  cout << "low_point     : " <<  this->low_point     << endl ;

}



// *****************************************************************************************************

class CFG{
  public:
    TString cfg_file;
    int run;
    TString indir;
    TString storedir;
    TString calib_payload;
    
    CFG(TString file):cfg_file(file){read_config();}
    
    void read_config(){
      ifstream f(cfg_file);
      TString g;
      f >> g >> g >> run;
      f >> g >> g >> indir;
      f >> g >> g >> storedir;
      f >> g >> g >> calib_payload;
    }
    
    void print(){
      cout << "Config file " << cfg_file << " :" << endl;
      cout << "  indir : " << indir << endl;
      cout << "  storedir : " << storedir << endl;
      cout << "  calib_payload : " << calib_payload << endl;
    }
};



