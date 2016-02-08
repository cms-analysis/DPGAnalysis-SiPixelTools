#include "header.h"

// ******** MAIN FUNCTION ********
void make_ComparisonPlots(TString fname="" , TString run="" , TString fname2="" , TString run2="" ){

  Bool_t diff = 0;
  
  RunNumber+=run;

  pix_trans.init();

  gStyle->SetPalette(1);
  
  TFile* file2 = TFile::Open(fname2,"READ");
  if(fname2 != "" && run2 != "") diff = 1;
  if(diff) RunNumber+="-"+run2;
  
  //logging of bad ROCs
  ofstream log_highGainROCs;
  log_highGainROCs.open("highGainROCs"+RunNumber+".log");
  if(! log_highGainROCs.is_open()){
    cout << "[log:highGainROCs] Could not open log file. Exiting ..." << endl;
    exit(1);
  }
    
  //logging of bad Fits ROCs
  ofstream log_badFitsROCs;
  log_badFitsROCs.open("badFitsROCs"+RunNumber+".log");
  if(! log_badFitsROCs.is_open()){
    cout << "[log:badFitsROCs] Could not open log file. Exiting ..." << endl;
    exit(1);
  }
  
  //logging of diff bad Fits ROCs
  ofstream log_diff_badFits;
  log_diff_badFits.open("diff_badFits"+RunNumber+".log");
  if(! log_diff_badFits.is_open()){
    cout << "[log:diff_badFits] Could not open log file. Exiting ..." << endl;
    exit(1);
  }
  double frac_flag_high_gain = 7; //flag pixels when gain is higher than this
  if(diff) frac_flag_high_gain = 0.5;
 
  TFile* file = TFile::Open(fname,"READ");
  file->cd();
 

  // Getting list of modules to loop on
  std::vector<TString> dirlist;
  dirlist = makedirlist(file,"Module");

  
  TDirectory* dir;
  TList* list;
  
  Double_t bmin = 999999999999 , bmax = 0 , fmin = 999999999999 , fmax = 0;
  
  
  //*******************************
  //Declaration of histograms
  
  //gStyle->SetOptStat(0);
  
  #include "hist_declarations.C"
  
  
  
  PIX<TH2F*> hist;
  PIX<TH2F*> hist_2;
   
  //TH2F *temp;
  //std::vector< TH2F* > *histo2save;
  
  //TH1F* test = new TH1F("tt","tt",100,0,100);
  
  int NModules = 0;
  int Npix = 0;
  int NBpix = 0;
  int NFpix = 0;
  int NpixGoodFit   = 0;
  int NBpixGoodFit  = 0;
  int NFpixGoodFit  = 0;
  
  int nPixHigherMax   = 0;
  int nPixFlaggedHigh = 0;
  int nColFlaggedHigh = 0;
  int nROCFlaggedHigh = 0;
  
  int nPix_better    = 0;
  int nPix_worse     = 0;
  int nPix_still_bad = 0;
  
  vector<int> npix_status(9,0);
  
  //********* Loop on Modules *******
  //cout<<"starting loop on good dirs"<<endl;

  for(unsigned int i=0; i<dirlist.size() ; ++i)
  { 
    NModules++;
   //if(NModules==647  || NModules==648) continue;
    
   //cout<<NModules<<"  "<<dirlist[i]<<endl;
   
     //if(i>10) break;
     
     
    
    //****************************************************************************************
    dir = file->GetDirectory(dirlist[i]); 
    list= dir->GetListOfKeys();
  

    for(int ikey=0;ikey<list->GetEntries();ikey++){
      TKey *thekey = (TKey*)list->At(ikey);
      if(thekey==0) continue;
      TString keyname=thekey->GetName();
      // if (ikey<100) cout <<" keyname "<< keyname<<endl;
      keyname.ReplaceAll(" ","");
      TString keytype=thekey->GetClassName();
      keytype.ReplaceAll(" ","");
      
      //Getting histo from file
      /*if(keytype=="TH2F"){
	if(keyname.Contains("344081672")){
	  temp = new TH2F();
	  temp = (TH2F*) dir->Get(keyname);
	  histo2save->push_back(temp);
	}
      }*/
      
      hist.getFromDir(dir , keytype , keyname);
      // cout <<"keytype "<< keytype << "  ; keyname " <<  keyname  << endl;

      if(keytype=="TH1F"){
	if(keyname.Contains("GainNPoints1d")) {GainNPoints1d           = (TH1F*) dir->Get(keyname);
	  // cout <<" Got Histogram " << keyname <<  endl;
	}
      }
    }
    

    //****************************************************************************************
    TString dirname = dirlist[i];
    
    if(diff){
      dirname.ReplaceAll("Run "+run,"Run "+run2);
      dir = file2->GetDirectory(dirname);
      if(dir==0){
        cout << dirname << " was not found ..." << endl;
        continue;
      }
      list= dir->GetListOfKeys();
      for(int ikey=0;ikey<list->GetEntries();ikey++){
        TKey *thekey = (TKey*)list->At(ikey);
        if(thekey==0) continue;
        TString keyname=thekey->GetName();
        keyname.ReplaceAll(" ","");
        TString keytype=thekey->GetClassName();
        keytype.ReplaceAll(" ","");
	
        //Getting histo from file
        /*if(keytype=="TH2F"){
	  if(keyname.Contains("344081672")){
	  temp = new TH2F();
	  temp = (TH2F*) dir->Get(keyname);
	  histo2save->push_back(temp);
	  }
	  }*/
	
        hist_2.getFromDir(dir , keytype , keyname);
        
        if(keytype=="TH1F"){
	  if(keyname.Contains("GainNPoints1d")) 		GainNPoints1d_2      = (TH1F*) dir->Get(keyname);
        }
      }
    }
    
    
    int NX = hist.gain->GetNbinsX();
    int NY = hist.gain->GetNbinsY();
    
    int detid = getDetId(hist.gain->GetName());
    cout << "Module: " << detid << endl;
    
    if(dirname.Contains("Barrel")){
      if(detid>bmax) bmax=detid;
      if(detid<bmin) bmin=detid;
    }
    if(dirname.Contains("Endcap")){
      if(detid>fmax) fmax=detid;
      if(detid<fmin) fmin=detid;
    }
    
    /*int layer = 0, ladder = 0, module = 0;
    pix.detID2Bpix(detid , layer , ladder , module);
    cout << "    " << layer << "  " << ladder << "  " << module << endl;
    getBarrelPos(dirlist[i] , layer , ladder , module);
    cout << "    " << layer << "  " << ladder << "  " << module << endl;
    */
    
    PIX<double> pix;
    PIX<double> pix_1;
    PIX<double> pix_2;
    
    
   /* if(NModules==1 || NModules==100 || NModules==1000){
      output->cd();
      write(Gain2d);
      write(ErrorGain2d);
      write(Pedestal2d);
      write(ErrorPedestal2d);
      write(GainSaturate2d);
      write(GainDynamicRange2d);
      write(GainFitResult2d);
      write(GainChi2NDF2d);
      write(GainChi2Prob2d);
      file->cd();
    }
    */
    TMean* MGain2d             = new TMean();
    TMean* MErrorGain2d        = new TMean();
    TMean* MPedestal2d         = new TMean();
    TMean* MErrorPedestal2d    = new TMean();
    TMean* MGainSaturate2d     = new TMean();
    TMean* MGainDynamicRange2d = new TMean();
    TMean* MGainFitResult2d    = new TMean();
    TMean* MGainChi2NDF2d      = new TMean();
    TMean* MGainChi2Prob2d     = new TMean();
    TMean* MGainHighPoint2d    = new TMean();
    TMean* MGainLowPoint2d     = new TMean();
    
    PIX<TMean> M_hist; 
    
    TMean* M_frac_gain_high    = new TMean();   
    TMean* M_frac_bad_fit      = new TMean();   
    
    double gainsperROC[16]={0};
    double errorgainsperROC[16]={0};
    double pedestalsperROC[16]={0};
    double errorpedestalsperROC[16]={0};
    
    int nHighGainPerROC[16]={0};
    int nBadFitPerROC[16]={0};
    int nBetterFitPerROC[16]={0};
    int nWorseFitPerROC[16]={0};
    int nperROC[16]={0};
    
    vector<PIX<TMean> > ROC(16 , PIX<TMean>()); 
    vector<PIX<TMean> > ROC_1(16 , PIX<TMean>()); 
    vector<PIX<TMean> > ROC_2(16 , PIX<TMean>()); 

   

    //******** Looping over pixels ********
    for(int xpix=1;xpix<=NX;xpix++)
    {
      double gainspercol[2]={0};
      double errorgainspercol[2]={0};
      double pedestalspercol[2]={0};
      double errorpedestalspercol[2]={0};
      double npercol[2]={0};
      int nBetterFitPerCol[2]={0};
      int nWorseFitPerCol[2]={0};
      int changecol=0;
      
      for(int ypix=1;ypix<=NY;ypix++){
      
	      /*if(i==0 && Gain2d->GetBinContent(xpix,ypix)==0){
	        test->Fill(Gain2d->GetBinContent(xpix,ypix));
	        cout<<xpix<<"  "<<ypix<<"  "<<Gain2d->GetBinContent(xpix,ypix)<<endl;
	      }*/
	      
	 if(ypix%80==0) changecol=1;
	 int rocID = rocId(xpix-1,ypix-1);
	   
	 Npix++;
	 if(dirlist[i].Contains("Barrel")) NBpix++;
	 if (dirlist[i].Contains("Endcap")) NFpix++;
	 		 
	 pix.fill(hist , xpix , ypix);
	 pix_1.fill(hist , xpix , ypix);
	 	 
	 if(diff){
	   
	   pix_2.fill(hist_2 , xpix , ypix);
	   	   
	   pix.gain	     -= pix_2.gain;
	   pix.err_gain	     -= pix_2.err_gain;
	   pix.pedestal	     -= pix_2.pedestal;
	   pix.err_pedestal  -= pix_2.err_pedestal;
	   pix.plateau       -= pix_2.plateau;
	   pix.dynamic_range -= pix_2.dynamic_range;
	   pix.fit_result    -= pix_2.fit_result;
	   pix.chi2NDF       -= pix_2.chi2NDF;
	   pix.chi2_prob     -= pix_2.chi2_prob;
	   pix.high_point    -= pix_2.high_point;
	   pix.low_point     -= pix_2.low_point;
	   
         }
	 
	 
	 //DIFF: checking if better or worse
	 if(diff){
	        if(pix_1.fit_result <= 0 && pix_2.fit_result > 0)  {++nPix_better ; ++nBetterFitPerCol[changecol] ; ++nBetterFitPerROC[rocID] ;}
	   else if(pix_1.fit_result > 0  && pix_2.fit_result <= 0) {++nPix_worse  ; ++nWorseFitPerCol[changecol]  ; ++nWorseFitPerROC[rocID]  ;}
	   else if(pix_1.fit_result <= 0 && pix_2.fit_result <= 0) ++nPix_still_bad;
	 
	 }
	 
	 ++npix_status[fabs(pix_1.fit_result)];
	 //	 cout << "pix_1.fit_result " << pix_1.fit_result << " with gain "<<  pix_1.gain << " and err " << pix_1.err_gain <<endl;
	 
	 if( pix_1.fit_result <= 0 ){
	   nBadFitPerROC[rocID]++;
	   M_frac_bad_fit->Add(1);
	   
	 }
	 else {
	   
	   if(diff && pix_2.fit_result<=0) continue;
	   
	   //bad fits
	   M_frac_bad_fit->Add(0);
	   
	   //gains higher than given value
           if( fabs(pix.gain) > frac_flag_high_gain ){
	     M_frac_gain_high->Add(1);
	     ++nPixFlaggedHigh;
	   }
	   else M_frac_gain_high->Add(0);
	   
	   if(fabs(pix.gain) > gain_max || fabs(pix.pedestal) > ped_max){
	     ++nPixHigherMax;
	     cout << "      col,row " << xpix << "," << ypix
	          <<" gain: "<< pix.gain << " ped: "<< pix.pedestal << endl;
	     continue;
	   }
	   
	   NpixGoodFit++;
	   if(dirlist[i].Contains("Barrel")) NBpixGoodFit++;
	   if (dirlist[i].Contains("Endcap")) NFpixGoodFit++;
	   	   	   
	   ++npercol[changecol];
	   gainspercol[changecol]		+=pix.gain;
	   errorgainspercol[changecol]		+=pix.err_gain;
	   pedestalspercol[changecol]		+=pix.pedestal;
	   errorpedestalspercol[changecol]	+=pix.err_pedestal;
	   
	   ++nperROC[rocID];
	   gainsperROC[rocID]		+=pix.gain;
	   errorgainsperROC[rocID]	+=pix.err_gain;
	   pedestalsperROC[rocID]	+=pix.pedestal;
	   errorpedestalsperROC[rocID]	+=pix.err_pedestal;
	   if(fabs(pix.gain) > frac_flag_high_gain) ++nHighGainPerROC[rocID];
	   ROC[rocID].fill(pix);
	   if(diff){
	     ROC_1[rocID].fill(pix); ROC_2[rocID].fill(pix);
	   }
	   
	   MGain2d		->Add(pix.gain);
	   MErrorGain2d		->Add(pix.err_gain);
	   MPedestal2d		->Add(pix.pedestal);
	   MErrorPedestal2d	->Add(pix.err_pedestal);
	   MGainSaturate2d	->Add(pix.plateau);
	   MGainDynamicRange2d	->Add(pix.dynamic_range);
	   MGainFitResult2d	->Add(pix.fit_result);
	   MGainChi2NDF2d	->Add(pix.chi2NDF);
	   MGainChi2Prob2d	->Add(pix.chi2_prob);
	   MGainHighPoint2d	->Add(pix.high_point);
	   MGainLowPoint2d	->Add(pix.low_point);
	   
	   M_hist.fill(pix);
	   
	   TOT_Gain->Fill(pix.gain);
	   TOT_ErrorGain->Fill(pix.err_gain);
	   TOT_Pedestal->Fill(pix.pedestal);
	   TOT_ErrorPedestal->Fill(pix.err_pedestal);
	   
	   CorrelationError->Fill(pix.err_gain , pix.err_pedestal);
	   ErrorVsGain->Fill(pix.gain , pix.err_gain);
	   ErrorVsPedestal->Fill(pix.pedestal , pix.err_pedestal);
	   CorrelationGainPed->Fill(pix.gain , pix.pedestal);
	   
	   if(dirlist[i].Contains("Barrel")){
	     TOT_GainBPix->Fill(pix.gain);
	     TOT_ErrorGainBPix->Fill(pix.err_gain);
	     TOT_PedestalBPix->Fill(pix.pedestal);
	     TOT_ErrorPedestalBPix->Fill(pix.err_pedestal);
	     
	     
	     CorrelationErrorBPix->Fill(pix.err_gain , pix.err_pedestal);
	     ErrorVsGainBPix->Fill(pix.gain , pix.err_gain);
	     ErrorVsPedestalBPix->Fill(pix.pedestal , pix.err_pedestal);
	     CorrelationGainPedBPix->Fill(pix.gain , pix.pedestal);
	     
	   }
	   else if (dirlist[i].Contains("Endcap")) {
	     TOT_GainFPix->Fill(pix.gain);
	     TOT_ErrorGainFPix->Fill(pix.err_gain);
	     TOT_PedestalFPix->Fill(pix.pedestal);
	     TOT_ErrorPedestalFPix->Fill(pix.err_pedestal);
	     
	     CorrelationErrorFPix->Fill(pix.err_gain , pix.err_pedestal);
	     ErrorVsGainFPix->Fill(pix.gain , pix.err_gain);
	     ErrorVsPedestalFPix->Fill(pix.pedestal , pix.err_pedestal);
	     CorrelationGainPedFPix->Fill(pix.gain , pix.pedestal);
	     
	   }
         }
       }//end of loop over Y pixels
       
              
       //Looping over column
       for(int nCOL=0;nCOL<2;++nCOL){
       
	 if(diff){
	   if(nBetterFitPerCol[nCOL] > 5){
	     cout << "    **** COL " << xpix << "," << nCOL << " :  " << nBetterFitPerCol[nCOL]  << " pixels were recovered" << endl;
             log_diff_badFits << detid << " COL " << setw(3) << xpix << "," << nCOL << " : " << setw(5) << nBetterFitPerCol[nCOL] << " pixels were lost" << endl;
	   }
	 
	   if(nWorseFitPerCol[nCOL] > 5){
	     cout << "    **** COL " << xpix << "," << nCOL << " :  " << nWorseFitPerCol[nCOL]  << " pixels were lost" << endl;
             log_diff_badFits << detid << " COL " << setw(3) << xpix << "," << nCOL << " : " <<  setw(5) << nWorseFitPerCol[nCOL] << " pixels were lost" << endl;
	   }
	 }
       
         if(npercol[nCOL]!=0){
	   
	   gainspercol[nCOL]/=double(npercol[nCOL]);
	   pedestalspercol[nCOL]/=double(npercol[nCOL]);
	   errorgainspercol[nCOL]/=double(npercol[nCOL]);
	   errorpedestalspercol[nCOL]/=double(npercol[nCOL]);
	   
	   if(gainspercol[nCOL] > frac_flag_high_gain) ++nColFlaggedHigh;
	   
	   
           TOT_GainPerCol->Fill(gainspercol[nCOL]);
           TOT_PedestalPerCol->Fill(pedestalspercol[nCOL]);
	   
	   CorrelationGainPedPerCol->Fill(gainspercol[nCOL],pedestalspercol[nCOL]);
	   CorrelationErrorPerCol->Fill(errorgainspercol[nCOL],errorpedestalspercol[nCOL]);
	   ErrorVsGainPerCol->Fill(gainspercol[nCOL],errorgainspercol[nCOL]);
	   ErrorVsPedestalPerCol->Fill(pedestalspercol[nCOL],errorpedestalspercol[nCOL]);
	   
	   
	   if(dirlist[i].Contains("Barrel")){
             TOT_GainPerColBPix->Fill(gainspercol[nCOL]);
             TOT_PedestalPerColBPix->Fill(pedestalspercol[nCOL]);
	     TOT_ErrorGainPerColBPix->Fill(errorgainspercol[nCOL]);
             TOT_ErrorPedestalPerColBPix->Fill(errorpedestalspercol[nCOL]);
	     
	     CorrelationGainPedPerColBPix->Fill(gainspercol[nCOL],pedestalspercol[nCOL]);
	     CorrelationErrorPerColBPix->Fill(errorgainspercol[nCOL],errorpedestalspercol[nCOL]);
	     ErrorVsGainPerColBPix->Fill(gainspercol[nCOL],errorgainspercol[nCOL]);
	     ErrorVsPedestalPerColBPix->Fill(pedestalspercol[nCOL],errorpedestalspercol[nCOL]);
	   }
	   if(dirlist[i].Contains("Endcap")){
             TOT_GainPerColFPix->Fill(gainspercol[nCOL]);
             TOT_PedestalPerColFPix->Fill(pedestalspercol[nCOL]);
	     TOT_ErrorGainPerColFPix->Fill(errorgainspercol[nCOL]);
             TOT_ErrorPedestalPerColFPix->Fill(errorpedestalspercol[nCOL]);
	     
	     CorrelationGainPedPerColFPix->Fill(gainspercol[nCOL],pedestalspercol[nCOL]);
	     CorrelationErrorPerColFPix->Fill(errorgainspercol[nCOL],errorpedestalspercol[nCOL]);
	     ErrorVsGainPerColFPix->Fill(gainspercol[nCOL],errorgainspercol[nCOL]);
	     ErrorVsPedestalPerColFPix->Fill(pedestalspercol[nCOL],errorpedestalspercol[nCOL]);
	   }
         }
       }//end of loop over col
       
     }//end of loop over X for pixels
     
     
     
     
     //Looping over ROC
     /*for(int nROC=0;nROC<16;nROC++){
     
       if( nHighGainPerROC[nROC] > 5 ){
         printf("    ===> ROC %i : %i pixels with gains > %f \n" , nROC , nHighGainPerROC[nROC] , frac_flag_high_gain);
         log_highGainROCs << detid <<" ROC " << setw(2) << nROC << " : " <<  setw(4) <<  nHighGainPerROC[nROC] << " pixels with gains > " << frac_flag_high_gain << endl;
       }
	 
       if( nBadFitPerROC[nROC] > 100 && nBadFitPerROC[nROC] != 4160 ){
         printf("    ===> ROC %i : %i pixels with bad fits\n" , nROC , nBadFitPerROC[nROC]);
         log_badFitsROCs << detid <<" ROC " << setw(2) << nROC << " : " <<  setw(4) << nBadFitPerROC[nROC] << " pixels with bad fits" << endl;
       }	 
     
       if(nperROC[nROC]!=0){
	   
	 gainsperROC[nROC]/=double(nperROC[nROC]);
	 pedestalsperROC[nROC]/=double(nperROC[nROC]);
	 errorgainsperROC[nROC]/=double(nperROC[nROC]);
	 errorpedestalsperROC[nROC]/=double(nperROC[nROC]);
	   
	 if(gainsperROC[nROC] > frac_flag_high_gain) ++nROCFlaggedHigh;
	   
         TOT_GainPerROC->Fill(gainsperROC[nROC]);
         TOT_PedestalPerROC->Fill(pedestalsperROC[nROC]);
	   
	 CorrelationGainPedPerROC->Fill(gainsperROC[nROC],pedestalsperROC[nROC]);
	 CorrelationErrorPerROC->Fill(errorgainsperROC[nROC],errorpedestalsperROC[nROC]);
	 ErrorVsGainPerROC->Fill(gainsperROC[nROC],errorgainsperROC[nROC]);
	 ErrorVsPedestalPerROC->Fill(pedestalsperROC[nROC],errorpedestalsperROC[nROC]);
	 	   
	 if(dirlist[i].Contains("Barrel")){
           TOT_GainPerROCBPix->Fill(gainsperROC[nROC]);
           TOT_PedestalPerROCBPix->Fill(pedestalsperROC[nROC]);
	   TOT_ErrorGainPerROCBPix->Fill(errorgainsperROC[nROC]);
           TOT_ErrorPedestalPerROCBPix->Fill(errorpedestalsperROC[nROC]);
	     
	   CorrelationGainPedPerROCBPix->Fill(gainsperROC[nROC],pedestalsperROC[nROC]);
	   CorrelationErrorPerROCBPix->Fill(errorgainsperROC[nROC],errorpedestalsperROC[nROC]);
	   ErrorVsGainPerROCBPix->Fill(gainsperROC[nROC],errorgainsperROC[nROC]);
	   ErrorVsPedestalPerROCBPix->Fill(pedestalsperROC[nROC],errorpedestalsperROC[nROC]);
	 }
	 if(dirlist[i].Contains("Endcap")){
           TOT_GainPerROCFPix->Fill(gainsperROC[nROC]);
           TOT_PedestalPerROCFPix->Fill(pedestalsperROC[nROC]);
	   TOT_ErrorGainPerROCFPix->Fill(errorgainsperROC[nROC]);
           TOT_ErrorPedestalPerROCFPix->Fill(errorpedestalsperROC[nROC]);
	     
	   CorrelationGainPedPerROCFPix->Fill(gainsperROC[nROC],pedestalsperROC[nROC]);
	   CorrelationErrorPerROCFPix->Fill(errorgainsperROC[nROC],errorpedestalsperROC[nROC]);
	   ErrorVsGainPerROCFPix->Fill(gainsperROC[nROC],errorgainsperROC[nROC]);
	   ErrorVsPedestalPerROCFPix->Fill(pedestalsperROC[nROC],errorpedestalsperROC[nROC]);
	 }
       }
     }*/
     
     
     for(int nROC=0;nROC<16;nROC++){
     
       if( nHighGainPerROC[nROC] > 5 ){
         printf("    ===> ROC %i : %i pixels with gains > %f \n" , nROC , nHighGainPerROC[nROC] , frac_flag_high_gain);
         log_highGainROCs << detid <<" ROC " << setw(2) << nROC << " : " <<  setw(5) <<  nHighGainPerROC[nROC] << " pixels with gains > " << frac_flag_high_gain << endl;
       }
	 
       if( nBadFitPerROC[nROC] > 100 && nBadFitPerROC[nROC] != 4160 ){
         printf("    ===> ROC %i : %i pixels with bad fits\n" , nROC , nBadFitPerROC[nROC]);
         log_badFitsROCs << detid <<" ROC " << setw(2) << nROC << " : " <<  setw(5) << nBadFitPerROC[nROC] << " pixels with bad fits" << endl;
       }
       	 
       if(diff){
	 if(nBetterFitPerROC[nROC] > 20){
	   cout << "	===> ROC " << nROC << " : " << nBetterFitPerROC[nROC]  << " pixels were recovered" << endl;
           log_diff_badFits << detid <<" ROC " << setw(2) << nROC << " : " <<  setw(5) << nBetterFitPerROC[nROC] << " pixels were recovered" << endl;
	 }
	 
	 if(nWorseFitPerROC[nROC] > 20){
	   cout << "	===> ROC " << nROC << " : " << nWorseFitPerROC[nROC]  << " pixels were lost" << endl;
           log_diff_badFits << detid <<" ROC " << setw(2) << nROC << " : " <<  setw(5) << nWorseFitPerROC[nROC] << " pixels were lost" << endl;
	 }
       }
	 

       if(ROC[nROC].gain.GetMean() != 0){
	   	   
	 if(ROC[nROC].gain.GetMean() > frac_flag_high_gain) ++nROCFlaggedHigh;
	   
         TOT_GainPerROC->Fill(ROC[nROC].gain.GetMean());
         TOT_PedestalPerROC->Fill(ROC[nROC].pedestal.GetMean());
	   
	 CorrelationGainPedPerROC->Fill(ROC[nROC].gain.GetMean(),ROC[nROC].pedestal.GetMean());
	 CorrelationErrorPerROC->Fill(ROC[nROC].err_gain.GetMean(),ROC[nROC].err_pedestal.GetMean());
	 ErrorVsGainPerROC->Fill(ROC[nROC].gain.GetMean(),ROC[nROC].err_gain.GetMean());
	 ErrorVsPedestalPerROC->Fill(ROC[nROC].pedestal.GetMean(),ROC[nROC].err_pedestal.GetMean());
	 	   
	 if(dirlist[i].Contains("Barrel")){
           TOT_GainPerROCBPix->Fill(ROC[nROC].gain.GetMean());
           TOT_PedestalPerROCBPix->Fill(ROC[nROC].pedestal.GetMean());
	   TOT_ErrorGainPerROCBPix->Fill(ROC[nROC].err_gain.GetMean());
           TOT_ErrorPedestalPerROCBPix->Fill(ROC[nROC].err_pedestal.GetMean());
	     
	   CorrelationGainPedPerROCBPix->Fill(ROC[nROC].gain.GetMean(),ROC[nROC].pedestal.GetMean());
	   CorrelationErrorPerROCBPix->Fill(ROC[nROC].err_gain.GetMean(),ROC[nROC].err_pedestal.GetMean());
	   ErrorVsGainPerROCBPix->Fill(ROC[nROC].gain.GetMean(),ROC[nROC].err_gain.GetMean());
	   ErrorVsPedestalPerROCBPix->Fill(ROC[nROC].pedestal.GetMean(),ROC[nROC].err_pedestal.GetMean());
	 }
	 if(dirlist[i].Contains("Endcap")){
           TOT_GainPerROCFPix->Fill(ROC[nROC].gain.GetMean());
           TOT_PedestalPerROCFPix->Fill(ROC[nROC].pedestal.GetMean());
	   TOT_ErrorGainPerROCFPix->Fill(ROC[nROC].err_gain.GetMean());
           TOT_ErrorPedestalPerROCFPix->Fill(ROC[nROC].err_pedestal.GetMean());
	     
	   CorrelationGainPedPerROCFPix->Fill(ROC[nROC].gain.GetMean(),ROC[nROC].pedestal.GetMean());
	   CorrelationErrorPerROCFPix->Fill(ROC[nROC].err_gain.GetMean(),ROC[nROC].err_pedestal.GetMean());
	   ErrorVsGainPerROCFPix->Fill(ROC[nROC].gain.GetMean(),ROC[nROC].err_gain.GetMean());
	   ErrorVsPedestalPerROCFPix->Fill(ROC[nROC].pedestal.GetMean(),ROC[nROC].err_pedestal.GetMean());
	 }
       }
     }
     
     
     cout << "    -- gain: " << MGain2d->GetMean() << "    pedestal: " << MPedestal2d->GetMean() << endl;
     
     SUM_Gain2d->SetBinContent(NModules,MGain2d->GetMean());
     SUM_ErrorGain2d->SetBinContent(NModules,MErrorGain2d->GetMean());
     SUM_Pedestal2d->SetBinContent(NModules,MPedestal2d->GetMean());
     SUM_ErrorPedestal2d->SetBinContent(NModules,MErrorPedestal2d->GetMean());
     SUM_GainSaturate2d->SetBinContent(NModules,MGainSaturate2d->GetMean());
     SUM_GainDynamicRange2d->SetBinContent(NModules,MGainDynamicRange2d->GetMean());
     SUM_GainFitResult2d->SetBinContent(NModules,MGainFitResult2d->GetMean());
     SUM_GainChi2NDF2d->SetBinContent(NModules,MGainChi2NDF2d->GetMean());
     SUM_GainChi2Prob2d->SetBinContent(NModules,MGainChi2Prob2d->GetMean());
     SUM_GainHighPoint2d->SetBinContent(NModules,MGainHighPoint2d->GetMean());
     SUM_GainLowPoint2d->SetBinContent(NModules,MGainLowPoint2d->GetMean());
     if(diff == 0) SUM_GainNPoints1d->SetBinContent(NModules,GainNPoints1d->GetMean());
     else          SUM_GainNPoints1d->SetBinContent(NModules,GainNPoints1d->GetMean() - GainNPoints1d_2->GetMean());
          
     MAP_Gain			.fill(detid , dirlist[i] , MGain2d->GetMean());    
     MAP_ErrorGain		.fill(detid , dirlist[i] , MErrorGain2d->GetMean());		    
     MAP_Pedestal		.fill(detid , dirlist[i] , MPedestal2d->GetMean());		     
     MAP_ErrorPedestal		.fill(detid , dirlist[i] , MErrorPedestal2d->GetMean());   
     MAP_GainSaturate		.fill(detid , dirlist[i] , MGainSaturate2d->GetMean());      
     MAP_GainDynamicRange	.fill(detid , dirlist[i] , MGainDynamicRange2d->GetMean());
     MAP_GainFitResult		.fill(detid , dirlist[i] , MGainFitResult2d->GetMean());    
     MAP_GainChi2NDF		.fill(detid , dirlist[i] , MGainChi2NDF2d->GetMean());     
     MAP_GainChi2Prob		.fill(detid , dirlist[i] , MGainChi2Prob2d->GetMean());      
     MAP_GainHighPoint		.fill(detid , dirlist[i] , MGainHighPoint2d->GetMean());    
     MAP_GainLowPoint		.fill(detid , dirlist[i] , MGainLowPoint2d->GetMean());
     if(diff==0) MAP_GainNPoints.fill(detid , dirlist[i] , GainNPoints1d->GetMean());
     else        MAP_GainNPoints.fill(detid , dirlist[i] , GainNPoints1d->GetMean() - GainNPoints1d_2->GetMean());	  
   
     MAP_frac_gain_high         .fill(detid , dirlist[i] , M_frac_gain_high->GetMean());
     MAP_frac_bad_fit           .fill(detid , dirlist[i] , M_frac_bad_fit->GetMean());
   
     if(TMath::Abs(MGain2d->GetMean())>20) cout<<"************  "<<MGain2d->GetMean()<<" mod "<<NModules<<"  "<<dirlist[i]<<endl;
     if(MPedestal2d->GetMean()==0) cout<<"=============="<<" mod "<<NModules<<dirlist[i]<<endl;
     
     
     delete MGain2d; 	    
     delete MErrorGain2d;	 
     delete MPedestal2d;	 
     delete MErrorPedestal2d;   
     delete MGainSaturate2d;     
     delete MGainDynamicRange2d;
     delete MGainFitResult2d;    
     delete MGainChi2NDF2d;     
     delete MGainChi2Prob2d;     
     delete MGainHighPoint2d;    
     delete MGainLowPoint2d;
     delete M_frac_gain_high;
     delete M_frac_bad_fit;
     
     
     
     
     
     //break;
     
   }//End of Loop over modules
   
  cout << "Number of Modules : "<<   NModules <<endl;
  
  
  //********* WRITING IN FILE ***************
  cout << "Writing to file ..." << endl;
  
  TFile* output = new TFile("Comp"+RunNumber+".root","RECREATE");
  //output->cd();
  
 
  output->cd();
  
  //test->Write();
  
  Bool_t   log = 0;
  if(diff) log = 1;
  
  //Pixel Level
  write(TOT_Gain , log);
  write(TOT_ErrorGain , log);
  write(TOT_Pedestal , log);
  write(TOT_ErrorPedestal , log);
  write(TOT_GainBPix , log);
  write(TOT_ErrorGainBPix , log);
  write(TOT_PedestalBPix , log);
  write(TOT_ErrorPedestalBPix , log);
  write(TOT_GainFPix , log);
  write(TOT_ErrorGainFPix , log);
  write(TOT_PedestalFPix , log);
  write(TOT_ErrorPedestalFPix , log);
  
  log = 1;
  
  //Column Level
  write(TOT_GainPerCol , log);
  write(TOT_PedestalPerCol , log);
  write(TOT_GainPerColBPix , log);
  write(TOT_ErrorGainPerColBPix , log);
  write(TOT_PedestalPerColBPix , log);
  write(TOT_ErrorPedestalPerColBPix , log);
  write(TOT_GainPerColFPix , log);
  write(TOT_ErrorGainPerColFPix , log);
  write(TOT_PedestalPerColFPix , log);
  write(TOT_ErrorPedestalPerColFPix , log);
  
  //ROC Level
  write(TOT_GainPerROC , log);
  write(TOT_PedestalPerROC , log);
  write(TOT_GainPerROCBPix , log);
  write(TOT_ErrorGainPerROCBPix , log);
  write(TOT_PedestalPerROCBPix , log);
  write(TOT_ErrorPedestalPerROCBPix , log);
  write(TOT_GainPerROCFPix , log);
  write(TOT_ErrorGainPerROCFPix , log);
  write(TOT_PedestalPerROCFPix , log);
  write(TOT_ErrorPedestalPerROCFPix , log);
  
  //All levels
  writeAllLevels(TOT_GainBPix,TOT_GainPerColBPix,TOT_GainPerROCBPix,"TOT_GainAllLevelBPix");
  writeAllLevels(TOT_ErrorGainBPix,TOT_ErrorGainPerColBPix,TOT_ErrorGainPerROCBPix,"TOT_ErrorGainAllLevelBPix");
  writeAllLevels(TOT_PedestalBPix,TOT_PedestalPerColBPix,TOT_PedestalPerROCBPix,"TOT_PedestalAllLevelBPix");
  writeAllLevels(TOT_ErrorPedestalBPix,TOT_ErrorPedestalPerColBPix,TOT_ErrorPedestalPerROCBPix,"TOT_ErrorPedestalAllLevelBPix");
  writeAllLevels(TOT_GainFPix,TOT_GainPerColFPix,TOT_GainPerROCFPix,"TOT_GainAllLevelFPix");
  writeAllLevels(TOT_ErrorGainFPix,TOT_ErrorGainPerColFPix,TOT_ErrorGainPerROCFPix,"TOT_ErrorGainAllLevelFPix");
  writeAllLevels(TOT_PedestalFPix,TOT_PedestalPerColFPix,TOT_PedestalPerROCFPix,"TOT_PedestalAllLevelFPix");
  writeAllLevels(TOT_ErrorPedestalFPix,TOT_ErrorPedestalPerColFPix,TOT_ErrorPedestalPerROCFPix,"TOT_ErrorPedestalAllLevelFPix");
    
  //Pixel level
  write(CorrelationGainPed);
  write(CorrelationGainPedBPix);
  write(CorrelationGainPedFPix);
  write(CorrelationError);
  write(ErrorVsGain);
  write(ErrorVsPedestal);
  write(CorrelationErrorBPix);
  write(ErrorVsGainBPix);
  write(ErrorVsPedestalBPix);
  write(CorrelationErrorFPix);
  write(ErrorVsGainFPix);
  write(ErrorVsPedestalFPix);
  	
  //Column level
  write(CorrelationGainPedPerCol);
  write(CorrelationGainPedPerColBPix);
  write(CorrelationGainPedPerColFPix);
  write(CorrelationErrorPerCol);
  write(ErrorVsGainPerCol);
  write(ErrorVsPedestalPerCol);
  write(CorrelationErrorPerColBPix);
  write(ErrorVsGainPerColBPix);
  write(ErrorVsPedestalPerColBPix);
  write(CorrelationErrorPerColFPix);
  write(ErrorVsGainPerColFPix);
  write(ErrorVsPedestalPerColFPix);
  	
  //ROC level
  write(CorrelationGainPedPerROC);
  write(CorrelationGainPedPerROCBPix);
  write(CorrelationGainPedPerROCFPix);
  write(CorrelationErrorPerROC);
  write(ErrorVsGainPerROC);
  write(ErrorVsPedestalPerROC);
  write(CorrelationErrorPerROCBPix);
  write(ErrorVsGainPerROCBPix);
  write(ErrorVsPedestalPerROCBPix);
  write(CorrelationErrorPerROCFPix);
  write(ErrorVsGainPerROCFPix);
  write(ErrorVsPedestalPerROCFPix);
  
  //Summary plots
  gStyle->SetOptStat(0);
  write(SUM_Gain2d);
  write(SUM_ErrorGain2d);
  write(SUM_Pedestal2d);
  write(SUM_ErrorPedestal2d);
  write(SUM_GainSaturate2d);
  write(SUM_GainDynamicRange2d);
  write(SUM_GainFitResult2d);
  write(SUM_GainChi2NDF2d);
  write(SUM_GainChi2Prob2d);
  write(SUM_GainHighPoint2d);
  write(SUM_GainLowPoint2d);
  write(SUM_GainNPoints1d);
  
  MAP_Gain.Write();
  MAP_ErrorGain.Write();
  MAP_Pedestal.Write();
  MAP_ErrorPedestal.Write();
  MAP_GainSaturate.Write();
  MAP_GainDynamicRange.Write();
  MAP_GainFitResult.Write();
  MAP_GainChi2NDF.Write();
  MAP_GainChi2Prob.Write();
  MAP_GainHighPoint.Write();
  MAP_GainLowPoint.Write();
  MAP_GainNPoints.Write();
  
  MAP_frac_gain_high.Write();
  MAP_frac_bad_fit.Write();
  
  //for(int i=0;i<histo2save->size();i++){;}
    //(histo2save[i]).Write();
    
  
  //Closing logs
  log_highGainROCs.close();
  log_badFitsROCs.close();
  log_diff_badFits.close();
  

  //********* SUMMARY WRITING ***************
  
  ofstream summary;
  ofstream texsummary;
  
  if(printSummary)
    summary.open("Summary"+RunNumber+".txt");
  if(printTexSummary)
    texsummary.open("texSummary"+RunNumber+".tex");
    
  if(summary.is_open()){
    cout << "Writing summary ... " << endl;
    summary<<"Looked @ "<<NModules<<" modules"<<endl;
    summary<<"Total number of pixels with good fit : "<<NpixGoodFit<<" over "<<Npix<<" -> "<<(double)NpixGoodFit/(double)Npix*100<<" %"<<endl;
    summary<<"In BPIX : "<<NBpixGoodFit<<" over "<<NBpix<<" -> "<<(double)NBpixGoodFit/(double)NBpix*100<<" %"<<endl;
    summary<<"In FPIX : "<<NFpixGoodFit<<" over "<<NFpix<<" -> "<<(double)NFpixGoodFit/(double)NFpix*100<<" %"<<endl<<endl;
    
    for(unsigned i = 0 ; i < npix_status.size() ; ++i)
      summary<<"Number of pixels with |status| = " << i << " : " << setw(8) << npix_status[i] << endl;
    summary<<endl;
    
    summary<<"Number of  pixels with gain > "<<gain_max<<" || fabs(ped) > "<<ped_max<<" :   " <<nPixHigherMax<<endl;
    summary<<"Number of  pixels with gain > "<<frac_flag_high_gain<<" :   " <<nPixFlaggedHigh<<endl;
    summary<<"Number of columns with gain > "<<frac_flag_high_gain<<" :   " <<nColFlaggedHigh<<endl;
    summary<<"Number of    ROCs with gain > "<<frac_flag_high_gain<<" :   " <<nROCFlaggedHigh<<endl<<endl;
    
    if(diff){
       summary << "Number of lost pixels we recovered: " << nPix_better << endl;
       summary << "Number of good pixels we lost:      " << nPix_worse << endl;
       summary << "Number of pixels still lost:        " << nPix_still_bad << endl << endl;
    }
    
    summary<<"Gain Mean : "<<endl;
    summary<<"BPIX : "<<TOT_GainBPix->GetMean()<<" +- "<<TOT_GainBPix->GetRMS()<<endl;
    summary<<"FPIX : "<<TOT_GainFPix->GetMean()<<" +- "<<TOT_GainFPix->GetRMS()<<endl;
    summary<<"Gain Error Mean : "<<endl;
    summary<<"BPIX : "<<TOT_ErrorGainBPix->GetMean()<<" +- "<<TOT_ErrorGainBPix->GetRMS()<<endl;
    summary<<"FPIX : "<<TOT_ErrorGainFPix->GetMean()<<" +- "<<TOT_ErrorGainFPix->GetRMS()<<endl<<endl;
    summary<<"Pedestal Mean : "<<endl;
    summary<<"BPIX : "<<TOT_PedestalBPix->GetMean()<<" +- "<<TOT_PedestalBPix->GetRMS()<<endl;
    summary<<"FPIX : "<<TOT_PedestalFPix->GetMean()<<" +- "<<TOT_PedestalFPix->GetRMS()<<endl;
    summary<<"Pedestal Error Mean : "<<endl;
    summary<<"BPIX : "<<TOT_ErrorPedestalBPix->GetMean()<<" +- "<<TOT_ErrorPedestalBPix->GetRMS()<<endl;
    summary<<"FPIX : "<<TOT_ErrorPedestalFPix->GetMean()<<" +- "<<TOT_ErrorPedestalFPix->GetRMS()<<endl<<endl;
    summary.close();
  }
  
  if(texsummary.is_open()){
    cout << "Writing tex summary ... " << endl;
    texsummary<<"Looked @ "<<NModules<<" modules"<< " \\\\" << endl;
    texsummary<<"Total number of pixels with good fit : "<<NpixGoodFit<<" over "<<Npix<<" $\\rightarrow$  "<<(double)NpixGoodFit/(double)Npix*100<<" \\%"<< " \\\\" << endl;
    texsummary<<"In BPIX : "<<NBpixGoodFit<<" over "<<NBpix<<" $\\rightarrow$ "<<(double)NBpixGoodFit/(double)NBpix*100<<" \\%"<< " \\\\" << endl;
    texsummary<<"In FPIX : "<<NFpixGoodFit<<" over "<<NFpix<<" $\\rightarrow$ "<<(double)NFpixGoodFit/(double)NFpix*100<<" \\%"<< " \\\\" << endl<< " \\\\" << endl;
    
    for(unsigned i = 0 ; i < npix_status.size() ; ++i)
      texsummary<<"Number of pixels with |status| = " << i << " : " << setw(8) << npix_status[i] << " \\\\" << endl;
    texsummary << " \\\\" << endl << " \\\\" << endl;


    texsummary<<"Number of  pixels with gain $>$ "<<gain_max<<" $||$ fabs(ped) $>$ "<<ped_max<<" :   " <<nPixHigherMax<<" \\\\" <<endl;
    texsummary<<"Number of  pixels with gain $>$ "<<frac_flag_high_gain<<" :   " <<nPixFlaggedHigh<<" \\\\" <<endl;
    texsummary<<"Number of columns with gain $>$ "<<frac_flag_high_gain<<" :   " <<nColFlaggedHigh<<" \\\\" <<endl;
    texsummary<<"Number of    ROCs with gain $>$ "<<frac_flag_high_gain<<" :   " <<nROCFlaggedHigh<<" \\\\" <<endl<<" \\\\" <<endl;
    
    if(diff){
       texsummary << "Number of lost pixels we recovered: " << nPix_better << " \\\\" <<endl;
       texsummary << "Number of good pixels we lost:      " << nPix_worse << " \\\\" <<endl;
       texsummary << "Number of pixels still lost:        " << nPix_still_bad << " \\\\" <<endl<<" \\\\" <<endl;
    }
    
    texsummary<<"Gain Mean : "<< " \\\\" << endl;
    texsummary<<"BPIX : "<<TOT_GainBPix->GetMean()<<" $\\pm$ "<<TOT_GainBPix->GetRMS()<< " \\\\" << endl;
    texsummary<<"FPIX : "<<TOT_GainFPix->GetMean()<<" $\\pm$ "<<TOT_GainFPix->GetRMS()<< " \\\\" << endl;
    texsummary<<"Gain Error Mean : "<< " \\\\" << endl;
    texsummary<<"BPIX : "<<TOT_ErrorGainBPix->GetMean()<<" $\\pm$ "<<TOT_ErrorGainBPix->GetRMS()<< " \\\\" << endl;
    texsummary<<"FPIX : "<<TOT_ErrorGainFPix->GetMean()<<" $\\pm$ "<<TOT_ErrorGainFPix->GetRMS()<< " \\\\" << endl<< " \\\\" << endl;
    texsummary<<"Pedestal Mean : "<< " \\\\" << endl;
    texsummary<<"BPIX : "<<TOT_PedestalBPix->GetMean()<<" $\\pm$ "<<TOT_PedestalBPix->GetRMS()<< " \\\\" << endl;
    texsummary<<"FPIX : "<<TOT_PedestalFPix->GetMean()<<" $\\pm$ "<<TOT_PedestalFPix->GetRMS()<< " \\\\" << endl;
    texsummary<<"Pedestal Error Mean : "<< " \\\\" << endl;
    texsummary<<"BPIX : "<<TOT_ErrorPedestalBPix->GetMean()<<" $\\pm$ "<<TOT_ErrorPedestalBPix->GetRMS()<< " \\\\" << endl;
    texsummary<<"FPIX : "<<TOT_ErrorPedestalFPix->GetMean()<<" $\\pm$ "<<TOT_ErrorPedestalFPix->GetRMS()<< " \\\\" << endl<< " \\\\" << endl;
    texsummary.close();
  }
  
  
  cout << setprecision(9) << "Barrel: " << bmin << " to " << bmax << endl;
  cout << "Endcap: " << fmin << " to " << fmax << endl;
  
  
	
  file->Close();
  output->Close(); 
  
}//End of main function

