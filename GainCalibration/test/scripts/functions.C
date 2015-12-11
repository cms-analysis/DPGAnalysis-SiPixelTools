
//Get ROC if from a pixel position
int rocId(int col, int row){
 int rocRow = row/80;
 int rocCol = col/52;
 int rocId = rocCol + rocRow*8;
 return rocId;
}

//write TH1F
void write(TH1F* histo , Bool_t logY , Bool_t largePad , TString statOptions){
  TString t=histo->GetName();
  t=t+RunNumber;
  int canv_y = 500 ;
  int canv_x = canv_y;
  if(largePad) canv_x = 2 * canv_y;
  TCanvas* c1 = new TCanvas("c1" , "c1" , canv_x , canv_y);
  c1->cd();
  
  if(logY) gPad->SetLogy(1);
  gStyle->SetOptStat(statOptions);
  
  histo->Draw();
  histo->Write();
  if(PrintImages){
    c1->Print(t+".png","png");
    if(PrintAllFormats){
      c1->Print(t+".eps","eps");
      c1->Print(t+".root","root");
    }
  }
  
  delete c1;
}

//Write TH2F
void write(TH2F* histo , Bool_t logZ){
  TString t=histo->GetName();
  t=t+RunNumber;
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  
  gStyle->SetOptStat(0);
  gPad->SetRightMargin(0.15);
  if(logZ) gPad->SetLogz(1);
  
  histo->Draw("colz");
  histo->Write();
  if(PrintImages){
    c1->Print(t+".png","png");
    if(PrintAllFormats){
      c1->Print(t+".eps","eps");
      c1->Print(t+".root","root");
    }
  }
  delete c1;
}

//Write pixel, column and ROC level on same canvas
void writeAllLevels(TH1F* pixel,TH1F* column,TH1F* roc,TString name){
  if(pixel->Integral()!=0)  pixel-> Scale(1./pixel->Integral());
  if(column->Integral()!=0) column->Scale(1./column->Integral());
  if(roc->Integral()!=0)    roc->   Scale(1./roc->Integral());
  
  column->SetLineColor(2);
  roc->SetLineColor(4);
  
  double Max=1;
  (pixel->GetMaximum() <= column->GetMaximum() )? Max=column->GetMaximum():Max=pixel->GetMaximum();
  if(Max<roc->GetMaximum()) Max=roc->GetMaximum();
  pixel->SetMaximum(1.1*Max);
  
  pixel->GetYaxis()->SetTitle("a.u.");
  
  TLegend* leg = new TLegend(0.65,0.7,0.85,0.9);
  leg->AddEntry(pixel,"Pixel Level","l");
  leg->AddEntry(column,"Column Level","l");
  leg->AddEntry(roc,"Roc Level","l");
  leg->SetFillColor(10);
  
  gStyle->SetOptStat(0);
  TCanvas* canv = new TCanvas(name,name);
  canv->cd();
  
  gStyle->SetOptStat(0);
  pixel->Draw();
  column->Draw("same");
  roc->Draw("same");
  leg->Draw("same");
  
  canv->Write();
  if(PrintImages){
    canv->Print(name+RunNumber+".png","png");
    if(PrintAllFormats){
      canv->Print(name+RunNumber+".eps","eps");
      canv->Print(name+RunNumber+".root","root");
    }
  }
}

//Get all modules from file
vector<TString> makedirlist(TFile* file,TString comparestring)
{
// make a loop over all plots
  file->cd();
  TList *list = file->GetListOfKeys();
  //Int_t nkeys = file->GetNkeys();
  
  std::vector<TString> dirlist;
  // TDirectory *dir = file->GetDirectory("DQMData");
 TDirectory *dir = file->GetDirectory("siPixelGainCalibrationAnalysis");
 
  if(dir==0) return dirlist;

  cout<<"Starting to find Directories"<<endl;
  std::vector<TString> keylist;
  std::vector<TString> notdonelist;
  std::vector<int> nsubdirs;
  //TDirectory *dirsav = dir;
  list = dir->GetListOfKeys();
  int ikey=0;
  //int localkey=0;
  //int ntimes=0;

  //*******************************
  //Get First Directory
  

  int num_dir_list=list->GetEntries() ;
  for(ikey=0;ikey<num_dir_list;  ikey++)
  {
    TKey *thekey = (TKey*)list->At(ikey);
    if(thekey==0) continue;
      
    TString keyname=thekey->GetName();
    TString keytype=thekey->GetClassName();
    // keytype.ReplaceAll(" ","");

    
    // if(keyname=="EventInfo") continue;
    if(keytype=="TDirectoryFile")
    {
      TString dirname=dir->GetPath();
      dirname+="/";
      dirname+=keyname;
      dirname.Remove(0,dirname.Last(':')+1);   
      dir=file->GetDirectory(dirname);
      list=dir->GetListOfKeys();
      if(dirname.Contains(comparestring)) dirlist.push_back(dirname);
      else
      {
	notdonelist.push_back(dirname);
	nsubdirs.push_back(-1);
      }
    }
  }
 
  //*******************************
  //Get List of Directory
 
  unsigned int nempty=0;
  while(nempty!=notdonelist.size())
  {
    for(unsigned int idir=0; idir<notdonelist.size(); ++idir)
    {
      if(nsubdirs[idir]==0) continue;
      dir = file->GetDirectory(notdonelist[idir]); 
      list= dir->GetListOfKeys();
      int ndirectories=0;
      for(ikey=0;ikey<list->GetEntries();  ikey++)
      {
	      TKey *thekey = (TKey*)list->At(ikey);
	      if(thekey==0) continue;
	      TString keyname=thekey->GetName();
	      TString keytype=thekey->GetClassName();
	      keytype.ReplaceAll(" ","");
	      if(keytype=="TDirectoryFile")
	      {
	        TString dirname=dir->GetPath();
	        dirname+="/";
	        dirname+=keyname;
                dirname.Remove(0,dirname.Last(':')+1);
	        ndirectories++;
	        if(dirname.Contains(comparestring)) dirlist.push_back(dirname);
	        else
	        {
	          notdonelist.push_back(dirname);
	          nsubdirs.push_back(-1);
	        }
	      }
      }
      nsubdirs[idir]=ndirectories;
    }
    // count number of done dirs;
    nempty=0;
    for(unsigned int i=0; i<nsubdirs.size(); i++) if(nsubdirs[i]!=-1)	nempty++;
  }

  return dirlist;

}


int getDetId(TString hname){
  return hname.Remove(0,hname.Last('_')+1).Atoi();
}


void getBarrelPos(TString& dirname , int& layer , int& ladder , int& module){
  if(! dirname.Contains("Barrel")){
    cout << "module not in barrel" << endl;
    return ;
  }
  
  TString tmp = dirname , tmp2 = dirname ;
  //getting layer
  module=tmp.Remove(0,tmp.Last('_')+1).Atoi();
  
  tmp2.Remove(tmp2.Last('_') , tmp2.Length());
  tmp = tmp2;
  
  ladder=tmp.Remove(0,tmp.Last('_')+1).Remove(2,tmp.Length()).Atoi();
  
  tmp2.Remove(tmp2.Last('_') , tmp2.Length());
  tmp = tmp2;
  
  layer=tmp.Remove(0,tmp.Last('_')+1).Remove(1,tmp.Length()).Atoi();
  
  if(dirname.Contains("Shell_m"))
    module = -1 * module;
  if(dirname.Contains("Shell_mI") || dirname.Contains("Shell_pI"))
    ladder = -1 * ladder;
}
