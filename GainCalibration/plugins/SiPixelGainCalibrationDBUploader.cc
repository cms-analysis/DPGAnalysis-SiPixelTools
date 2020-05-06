
// -*- C++ -*-
//
// Package:    SiPixelGainCalibrationDBUploader
// Class:      SiPixelGainCalibrationDBUploader
// 
/**\class SiPixelGainCalibrationDBUploader SiPixelGainCalibrationDBUploader.cc CalibTracker/SiPixelGainCalibrationDBUploader/src/SiPixelGainCalibrationDBUploader.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Freya BLEKMAN
//         Created:  Tue Aug  5 16:22:46 CEST 2008
// $Id: SiPixelGainCalibrationDBUploader.cc,v 1.7 2010/01/12 11:29:54 rougny Exp $
//
//


// system include files
#include <memory>
#include <fstream>
#include <sys/stat.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelCalibConfiguration.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibration.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibrationOffline.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibrationForHLT.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationService.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonTopologies/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"

#include "TH2F.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TString.h"
#include "TList.h"
#include "TTree.h"

#include "SiPixelGainCalibrationDBUploader.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//#define Tree    # fill the tree
//#define TreeOffline # fill the offline tree

//
// class decleration
//

void SiPixelGainCalibrationDBUploader::fillDatabase(const edm::EventSetup& iSetup){
  // only create when necessary.
  // process the minimum and maximum gain & ped values...
  std::cout << "Start filling db" << std::endl;
  
  std::map<uint32_t,std::pair<TString,int> > badresults;

  edm::Service<TFileService> fs;

  TH1F *VCAL_endpoint = fs->make<TH1F>("VCAL_endpoint","value where response = 255 ( x = (255 - ped)/gain )",256,0,256);
  TH1F *goodgains = fs->make<TH1F>("goodgains","gain values",500,0,gainmax_);
  TH1F *goodpeds = fs->make<TH1F>("goodpeds","pedestal values",356,-100,256); //512,-256,256 original value: 356,-100,256
  TH1F *totgains = fs->make<TH1F>("totgains","gain values",500,0,gainmax_);
  TH1F *totpeds = fs->make<TH1F>("totpeds","pedestal values",356,-100,256);
  int useddefaultfortree;
#ifdef Tree
  TTree *tree = new TTree("tree","tree");
  int detidfortree,rowfortree,colfortree;
  float pedfortree, gainfortree,chi2fortree;
  tree->Branch("detid",&detidfortree,"detid/I");
  tree->Branch("row",&rowfortree,"row/I");
  tree->Branch("col",&colfortree,"col/I");
  tree->Branch("defaultval",&useddefaultfortree,"defaultval/I");
  tree->Branch("ped",&pedfortree,"ped/F");
  tree->Branch("gain",&gainfortree,"gain/F");
  tree->Branch("chi2",&chi2fortree,"chi2/F");
#endif // Tree
#ifdef TreeOffline 
  TTree *treeOffline = new TTree("treeOffline","treeOffline");
  int TOrow(0),TOcol(0),TOdetid(0);
  double TOped(0),TOgain(0);
  treeOffline->Branch("detid",&TOdetid,"detid/I");
  treeOffline->Branch("row",&TOrow,"row/I");
  treeOffline->Branch("col",&TOcol,"col/I");
  treeOffline->Branch("ped",&TOped,"ped/D");
  treeOffline->Branch("gain",&TOgain,"gain/D");
#endif // TreeOffline

  int countModulesMissing=0, countModulesWithHistos=0, lookAtModules=0;

  size_t ntimes=0;
  std::cout << "Filling record " << record_ << std::endl;
  if(record_!="SiPixelGainCalibrationForHLTRcd" && record_!="SiPixelGainCalibrationOfflineRcd" && record_!="SiPixelGainCalibrationRcd"){
    //if(record_!="SiPixelGainCalibrationForHLTRcd" && record_!="SiPixelGainCalibrationOfflineRcd" && record_!="SiPixelGainCalibrationRcd" && record_!="SiPixelGainCalibrationPhase1OfflineRcd"){
    std::cout << "you passed record " << record_ << ", which I have no idea what to do with!" << std::endl;
    return;
  }

  // Verify again that the high gain is not too high (code from Thea)
  if(gainhi_>gainmax_) gainhi_=gainmax_;
  if(pedhi_>pedmax_) pedhi_=pedmax_;

  int npixEmpty=0, npixUsed=0, npixGood=0, npixGood2=0, npixBad=0; 
  float badpedval=pedlow_-200;
  float badgainval=gainlow_-200;
  float meangain= meanGainHist_->GetMean();
  float meanped = meanPedHist_->GetMean();

  std::cout << "pedestals low: " << pedlow_ << " high: " << pedhi_ << " gains low: " << gainlow_ << " high: " << gainhi_
	    << ", and mean gain " << meangain<< ", ped " << meanped << std::endl;

  // and fill the dummy histos:  
  for(size_t icol=0; icol<nmaxcols;++icol){
    for(size_t irow=0; irow<nmaxrows; ++irow){
      defaultGain_->SetBinContent(icol+1,irow+1,meangain);
      defaultPed_->SetBinContent(icol+1,irow+1,meanped);
      defaultChi2_->SetBinContent(icol+1,irow+1,1.0);
      defaultFitResult_->SetBinContent(icol+1,irow+1,0);
    }
  }
  
  // theGainCalibrationDbInput_= new SiPixelGainCalibration(pedlow_*0.999,pedhi_*1.001,gainlow_*0.999,gainhi_*1.001);
  // theGainCalibrationDbInputHLT_ = new SiPixelGainCalibrationForHLT(pedlow_*0.999,pedhi_*1.001,gainlow_*0.999,gainhi_*1.001);
  // theGainCalibrationDbInputOffline_ = new SiPixelGainCalibrationOffline(pedlow_*0.999,pedhi_*1.001,gainlow_*0.999,gainhi_*1.001);
  theGainCalibrationDbInput_= new SiPixelGainCalibration(pedlow_,pedhi_,gainlow_*0.999,gainhi_*1.001);
  theGainCalibrationDbInputHLT_ = new SiPixelGainCalibrationForHLT(pedlow_,pedhi_,gainlow_*0.999,gainhi_*1.001);
  theGainCalibrationDbInputOffline_ = new SiPixelGainCalibrationOffline(pedlow_,pedhi_,gainlow_*0.999,gainhi_*1.001);
  
  //  theGainCalibrationDbInputPhase1Offline_ = new SiPixelGainCalibrationPhase1Offline(pedlow_*0.999,pedhi_*1.001,gainlow_*0.999,gainhi_*1.001);  
  
  uint32_t nchannels=0;
  uint32_t nmodules=0;
  uint32_t detid=0;
  therootfile_->cd();
  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );     
  std::cout<<"SiPixelCondObjOfflineBuilder" <<" There are "<<pDD->dets().size() <<" detectors"<<std::endl;
  std::cout << "Start looping on detids, there are " << bookkeeper_.size() << " histograms to consider..." << std::endl;
  
  int NDetid = 0;
  size_t nemptypixels=0;
  for(TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); 
      it != pDD->dets().end(); it++) {
    detid=0;
    if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0) detid=((*it)->geographicalId()).rawId();
    if(detid==0) continue;
    ++nmodules;
    NDetid++;
    int badDetId=0;
    //ntimes=0;
    useddefaultfortree=0;

    // Get the module sizes.
    TH2F *tempchi2;
    TH2F *tempfitresult;
    TH2F *tempgain;
    TH2F *tempped;
    TString tempgainstring;

    if(!badDetId) {
    
      TString tempchi2string = bookkeeper_[detid]["chi2prob_2d"];
      tempchi2 = (TH2F*)therootfile_->Get(tempchi2string);
      if(tempchi2==0 || badDetId){
	tempchi2=defaultChi2_;
	useddefaultfortree=1; // histo does not exits use default
      }
      TString tempfitresultstring = bookkeeper_[detid]["fitresult_2d"];
      tempfitresult = (TH2F*)therootfile_->Get(tempfitresultstring);
      if(tempfitresult==0){
	tempfitresult=defaultFitResult_;  
	useddefaultfortree=1;
      }
      TString tempgainstring = bookkeeper_[detid]["gain_2d"];
      tempgain = (TH2F*)therootfile_->Get(tempgainstring);
      if(tempgain==0){
	std::cout <<"WARNING, gain histo " << bookkeeper_[detid]["gain_2d"] << " does not exist, using default instead" << std::endl;
	tempgain=defaultGain_;  
	useddefaultfortree=1;
      }
      TString temppedstring = bookkeeper_[detid]["ped_2d"];
      tempped = (TH2F*) therootfile_->Get(temppedstring);
      if(tempped==0){
	std::cout <<"WARNING, ped histo " << bookkeeper_[detid]["ped_2d"] << " for detid " << detid << " does not exist, using default instead" << std::endl;
	std::pair<TString,int> tempval(tempgainstring,0);
	badresults[detid]=tempval;
	tempped=defaultPed_;
	useddefaultfortree=1;
      }
      if(useddefaultfortree==1) countModulesMissing++;
      else countModulesWithHistos++;

    } else { // does this ever happen
      std::cout<<" does this ever happen "<<std::endl;
      tempchi2=defaultChi2_;
      tempgain=defaultGain_; 
      tempfitresult=defaultFitResult_;  
      std::pair<TString,int> tempval(tempgainstring,0);
      badresults[detid]=tempval;
      tempped=defaultPed_;
      useddefaultfortree=1;
    }
    
    
    const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>((*it));
    const PixelTopology & topol = pixDet->specificTopology();       
    // Get the module sizes.
    size_t nrows = topol.nrows();      // rows in x
    size_t ncols = topol.ncolumns();   // cols in y
    size_t nrowsrocsplit = theGainCalibrationDbInputHLT_->getNumberOfRowsToAverageOver();

    if(theGainCalibrationDbInputOffline_->getNumberOfRowsToAverageOver()!=nrowsrocsplit)
      throw  cms::Exception("GainCalibration Payload configuration error")
	<< "[SiPixelGainCalibrationAnalysis::fillDatabase] ERROR the SiPixelGainCalibrationOffline and SiPixelGainCalibrationForHLT database payloads have different settings for the number of rows per roc: " << theGainCalibrationDbInputHLT_->getNumberOfRowsToAverageOver() << "(HLT), " << theGainCalibrationDbInputOffline_->getNumberOfRowsToAverageOver() << "(offline)";
    std::vector<char> theSiPixelGainCalibrationPerPixel;
    std::vector<char> theSiPixelGainCalibrationPerColumn;
    std::vector<char> theSiPixelGainCalibrationGainPerColPedPerPixel;
    std::vector<char> theSiPixelGainCalibrationGainPerClusterColPedPerPixel;
    
    //Get mean of gain/pedestal of this Detid
    meangain= meanGainHist_->GetMean();
    meanped = meanPedHist_->GetMean();
    int npix=0;
    double meanGainForThisModule=0;
    double meanPedForThisModule=0;
    for(size_t icol=1; icol<=ncols; icol++){
      for(size_t jrow=1; jrow<=nrows; jrow++){
	if(tempfitresult->GetBinContent(icol,jrow)>0){
          npix++; if(useddefaultfortree==0) npixUsed++;
          meanGainForThisModule+=tempgain->GetBinContent(icol,jrow);
          meanPedForThisModule+=tempped->GetBinContent(icol,jrow);
	} else { if(useddefaultfortree==0) npixEmpty++;}
	
      }
    }
    if(npix!=0) meanPedForThisModule/=npix;
    if(npix!=0) meanGainForThisModule/=npix;
    if(usemeanwhenempty_){
      if(meanGainForThisModule>gainlow_ && meanGainForThisModule<gainhi_ && npix>100) meangain = meanGainForThisModule;
      if(meanPedForThisModule>pedlow_   && meanPedForThisModule<pedhi_   && npix>100) meanped  = meanPedForThisModule;
    }
    
    lookAtModules++;
    // Loop over columns and rows of this DetID
    float peds[160];
    float gains[160];
    float pedforthiscol[2];
    float gainforthiscol[2];
    float gainClusterCol[2][2];
    int nusedrows[2];
    for(size_t icol=1; icol<=ncols; icol++) {
      nusedrows[0]=nusedrows[1]=0;
      pedforthiscol[0]=pedforthiscol[1]=0;
      gainforthiscol[0]=gainforthiscol[1]=0;
      gainClusterCol[0][0]=gainClusterCol[0][1]=0;
      gainClusterCol[1][0]=gainClusterCol[1][1]=0;
      for(size_t jrow=1; jrow<=nrows; jrow++) {
	size_t iglobalrow=0;
	if(jrow>nrowsrocsplit) iglobalrow=1;
	peds[jrow]=badpedval;
	gains[jrow]=badgainval;
	float ped = tempped->GetBinContent(icol,jrow);
	float gain = tempgain->GetBinContent(icol,jrow);
	float chi2 = tempchi2->GetBinContent(icol,jrow);
	float fitresult = tempfitresult->GetBinContent(icol,jrow);
	if(gain>500.) std::cout<<gain<<" "<<fitresult<<std::endl;
	if(ped>pedlow_ && gain>gainlow_ && ped<pedhi_ && gain<gainhi_ && (fitresult>0)){
	  ntimes++; if(useddefaultfortree==0) npixGood++;
	  VCAL_endpoint->Fill((255 - ped)/gain);
	  peds[jrow]=ped;
	  gains[jrow]=gain;
	  pedforthiscol[iglobalrow]+=ped;
	  gainforthiscol[iglobalrow]+=gain;
	  gainClusterCol[iglobalrow][(jrow+1)%2] += gain;
	  //if (detid==303050776) {std::cout << "adding gain for col " << icol << " and row " << jrow << ": " << gain << " to iglobalrow[row]: "<< iglobalrow << "[" << (jrow+1)%2 << "]" << std::endl;}

	  nusedrows[iglobalrow]++;
	  goodpeds->Fill(ped);
	  goodgains->Fill(gain);

#ifdef Tree
	  //filling the tree
	  detidfortree=detid;
	  rowfortree=jrow-1;
	  colfortree=icol-1;
	  gainfortree=gain;
	  pedfortree=ped;
	  chi2fortree=chi2;
	  tree->Fill();
#endif
	  
	} else {  

	  nemptypixels++; if(useddefaultfortree==0) npixBad++;
	  if(usemeanwhenempty_){
	    peds[jrow]=meanped;
	    gains[jrow]=meangain;
	    std::pair<TString,int> tempval(tempgainstring,1);
	    if(useddefaultfortree==0) badresults[detid]=tempval;
	  } else{
	    std::pair<TString,int> tempval(tempgainstring,2);
	    if(useddefaultfortree==0) badresults[detid]=tempval;
	    // if everything else fails: set the gain & ped now to dead
	    peds[jrow]=badpedval;
	    gains[jrow]=badgainval;
	  }
	}
	totgains->Fill(gains[jrow]);
	totpeds->Fill(peds[jrow]);
      }// now collected all info, actually do the filling
      
      for(size_t jrow=1; jrow<=nrows; jrow++) {
	nchannels++;
	size_t iglobalrow=0;
	if(jrow>nrowsrocsplit)
	  iglobalrow=1;
	float ped=peds[jrow];
	float gain=gains[jrow];
	
	// This is probbaly a good place to insert the vcal calibration, 
	// someting like this:
	// include the vcal claibration already here 
	//double newGain = 1, newPed  = 0.;
	//if(layer==1) { 
	//newGain = gain * electronsPerVcal_L1_;
	//newPed  = ped  - (electronsPerVcal_L1_Offset_/newGain);
	//} else {
	//newGain = gain * electronsPerVcal_;
	//newPed  = ped  - (electronsPerVcal_Offset_/newGain);
	//}
	//ped = newPed;
	//gain = newGain;


	if( ped>pedlow_ && gain>gainlow_ && ped<pedhi_ && gain<gainhi_ ){
	  if(useddefaultfortree==0) npixGood2++;
	  theGainCalibrationDbInput_->setData(ped, gain, theSiPixelGainCalibrationPerPixel);
	  theGainCalibrationDbInputOffline_->setDataPedestal(ped, theSiPixelGainCalibrationGainPerColPedPerPixel);
	  //	  theGainCalibrationDbInputPhase1Offline_->setDataPedestal(ped, theSiPixelGainCalibrationGainPerClusterColPedPerPixel);
	} else {
	  theGainCalibrationDbInput_->setDeadPixel(theSiPixelGainCalibrationPerPixel);
	  theGainCalibrationDbInputOffline_->setDeadPixel(theSiPixelGainCalibrationGainPerColPedPerPixel);
	  //	  theGainCalibrationDbInputPhase1Offline_->setDeadPixel(theSiPixelGainCalibrationGainPerClusterColPedPerPixel);
	}

	//nrowsrocsplit == 80 --> the gain is written at the end of each column
	if(jrow%nrowsrocsplit==0){
	  if(nusedrows[iglobalrow]>0){
	    //std::cout << "nusedrows[iglobalrow]: " << nusedrows[iglobalrow] << std::endl;
	    pedforthiscol[iglobalrow]/=(float)nusedrows[iglobalrow];
	    gainforthiscol[iglobalrow]/=(float)nusedrows[iglobalrow];
	    gainClusterCol[iglobalrow][0]/=((float)nusedrows[iglobalrow]/2);
	    gainClusterCol[iglobalrow][1]/=((float)nusedrows[iglobalrow]/2);

#ifdef TreeOffline
	    // This tree has cols & rows in module units 
	    int startingRow(1);
	    if ( jrow==160 ) {startingRow=81;}
	    //filling the offline tree
	    TOdetid = detid;
	    for(size_t row_tmp=startingRow; row_tmp<=jrow; row_tmp++) {
	      float ped_tmp = peds[row_tmp];
	      TOrow = row_tmp-1;
	      TOcol = icol-1;
	      TOped = ped_tmp;
	      int globalrow_tmp(0);
	      if(row_tmp>nrowsrocsplit) {globalrow_tmp=1;}
	      TOgain = gainforthiscol[globalrow_tmp];
	      treeOffline->Fill();
	    }
#endif // TreeOffline

	    //if (detid==303050776) {std::cout << "detid: " << detid << " gain to be filled: [0]: " << gainClusterCol[iglobalrow][0] << " [1] : " << gainClusterCol[iglobalrow][1] << std::endl << "  gainforthiscol: " << gainforthiscol[iglobalrow] << std::endl;}
	  } 
	  if(gainforthiscol[iglobalrow]>gainlow_ && gainforthiscol[iglobalrow]<gainhi_ && pedforthiscol[iglobalrow]>pedlow_ && pedforthiscol[iglobalrow]<pedhi_ ){// good 
	    //	    std::cout << "setting ped & col aves: " << pedforthiscol[iglobalrow] << " " <<  gainforthiscol[iglobalrow]<< std::endl;
	  } else {	
	    if(usemeanwhenempty_) {
	      pedforthiscol[iglobalrow]=meanped;
	      gainforthiscol[iglobalrow]=meangain;
	      std::pair<TString,int> tempval(tempgainstring,3);
	      if(useddefaultfortree==0) badresults[detid]=tempval;
	    } else { //make dead
	      pedforthiscol[iglobalrow]=badpedval;
	      gainforthiscol[iglobalrow]=badgainval;
	      std::pair<TString,int> tempval(tempgainstring,4);
	      if(useddefaultfortree==0) badresults[detid]=tempval;
	    }
	  }

	  if(gainClusterCol[iglobalrow][0]>gainlow_ && gainClusterCol[iglobalrow][0]<gainhi_  && gainClusterCol[iglobalrow][1]>gainlow_ && gainClusterCol[iglobalrow][1]<gainhi_){// good 
	    //	    std::cout << "setting ped & col aves: " << pedforthiscol[iglobalrow] << " " <<  gainforthiscol[iglobalrow]<< std::endl;
	  } else{	
	    if(usemeanwhenempty_){
	      gainClusterCol[iglobalrow][0]=meangain;
	      gainClusterCol[iglobalrow][1]=meangain;
	      std::pair<TString,int> tempval(tempgainstring,3);
	      if(useddefaultfortree==0) badresults[detid]=tempval;
	    } else{ //make dead
	      gainClusterCol[iglobalrow][0]=badgainval;
	      gainClusterCol[iglobalrow][1]=badgainval;
	      std::pair<TString,int> tempval(tempgainstring,4);
	      if(useddefaultfortree==0) badresults[detid]=tempval;
	    }
	  }

	  if(gainforthiscol[iglobalrow]>gainlow_ && gainforthiscol[iglobalrow]<gainhi_ && pedforthiscol[iglobalrow]>pedlow_ && pedforthiscol[iglobalrow]<pedhi_ ){
	    //if (detid==303050776) {std::cout << "filling offline gain: " << gainforthiscol[iglobalrow] << std::endl;}
	    theGainCalibrationDbInputOffline_->setDataGain(gainforthiscol[iglobalrow],nrowsrocsplit,theSiPixelGainCalibrationGainPerColPedPerPixel);
	    theGainCalibrationDbInputHLT_->setData(pedforthiscol[iglobalrow],gainforthiscol[iglobalrow],theSiPixelGainCalibrationPerColumn);
	  } else{
	    theGainCalibrationDbInputOffline_->setDeadColumn(nrowsrocsplit,theSiPixelGainCalibrationGainPerColPedPerPixel);
	    theGainCalibrationDbInputHLT_->setDeadColumn(nrowsrocsplit,theSiPixelGainCalibrationPerColumn);
	  }
	  //phase1Offline
	  //if(gainClusterCol[iglobalrow][0]>gainlow_ && gainClusterCol[iglobalrow][0]<gainhi_ && pedforthiscol[iglobalrow]>pedlow_ && pedforthiscol[iglobalrow]<pedhi_ && gainClusterCol[iglobalrow][1]>gainlow_ && gainClusterCol[iglobalrow][1]<gainhi_ ){
	  //if (detid==303050776) {std::cout << "filling gain [0]: " << gainClusterCol[iglobalrow][0] << " [1]: " << gainClusterCol[iglobalrow][1] << std::endl;}
	  //theGainCalibrationDbInputPhase1Offline_->setDataGain(gainClusterCol[iglobalrow][0],gainClusterCol[iglobalrow][1], nrowsrocsplit,theSiPixelGainCalibrationGainPerClusterColPedPerPixel);
	  //} else{
	  //theGainCalibrationDbInputPhase1Offline_->setDeadColumn(nrowsrocsplit,theSiPixelGainCalibrationGainPerClusterColPedPerPixel);
	  //}
	}
      }
    }

  
    SiPixelGainCalibration::Range range(theSiPixelGainCalibrationPerPixel.begin(),theSiPixelGainCalibrationPerPixel.end());
    SiPixelGainCalibrationForHLT::Range hltrange(theSiPixelGainCalibrationPerColumn.begin(),theSiPixelGainCalibrationPerColumn.end());
    SiPixelGainCalibrationOffline::Range offlinerange(theSiPixelGainCalibrationGainPerColPedPerPixel.begin(),theSiPixelGainCalibrationGainPerColPedPerPixel.end());
    //SiPixelGainCalibrationPhase1Offline::Range phase1offlinerange(theSiPixelGainCalibrationGainPerClusterColPedPerPixel.begin(),theSiPixelGainCalibrationGainPerClusterColPedPerPixel.end());
    
    // now start creating the various database objects
    if( !theGainCalibrationDbInput_->put(detid,range,ncols) )
      edm::LogError("SiPixelGainCalibrationAnalysis")<<"warning: detid already exists for Offline (gain per col, ped per pixel) calibration database"<<std::endl;
    if( !theGainCalibrationDbInputOffline_->put(detid,offlinerange,ncols) )
      edm::LogError("SiPixelGainCalibrationAnalysis")<<"warning: detid already exists for Offline (gain per col, ped per pixel) calibration database"<<std::endl;
    //if( !theGainCalibrationDbInputPhase1Offline_->put(detid,phase1offlinerange,ncols) )
    //edm::LogError("SiPixelGainCalibrationAnalysis")<<"warning: detid already exists for phase1 Offline (gain per col, ped per pixel) calibration database"<<std::endl;
    if(!theGainCalibrationDbInputHLT_->put(detid,hltrange, ncols) )
      edm::LogError("SiPixelGainCalibrationAnalysis")<<"warning: detid already exists for HLT (pedestal and gain per column) calibration database"<<std::endl;
  }
  // now printing out summary:
  size_t nempty=0;
  size_t ndefault=0;
  size_t ndead=0;
  size_t ncoldefault=0;
  size_t ncoldead=0;
  for(std::map<uint32_t,std::pair<TString,int> >::const_iterator ibad= badresults.begin(); ibad!=badresults.end(); ++ibad){
    uint32_t detid = ibad->first;
    //std::cout<<detid<<" "<<badresults[detid].first<<" "<<badresults[detid].second<<std::endl;
    if(badresults[detid].second==0) {
      //std::cout << detid << " empty module, use default "<< std::endl;
      nempty++;
    } else if(badresults[detid].second==1) {
      ndefault++;
      std::cout << detid << " has one or more dead pixels, use default "<< std::endl;
    } else if(badresults[detid].second==2) {
      std::cout << detid << " has one or more dead pixels"<< std::endl;
      ndead++;
    } else if(badresults[detid].second==3) {
      std::cout << detid << " has one or more dead columns, use default"<<std::endl;
      ncoldefault++;
    } else if(badresults[detid].second==4) {
      std::cout << detid << " has one or more dead columns"<<std::endl;
      ncoldead++;
    }
  }
  std::cout << nempty << " modules were empty and now have pixels filled with default values." << std::endl;
  std::cout << ndefault << " modules have pixels filled with default values." << std::endl;
  std::cout << ndead << " modules have pixels flagged as dead." << std::endl;
  std::cout << ncoldefault << " modules have columns filled with default values." << std::endl;
  std::cout << ncoldead << " modules have columns filled with dead values." << std::endl;
  std::cout << " ---> PIXEL Modules  " << nmodules  << "\n"
	    << " ---> PIXEL Channels " << nchannels << std::endl;
  //std::cout << " "<<ntimes<<" "<<nemptypixels<<" "<<NDetid<<std::endl;
  std::cout << " modules missing "<<countModulesMissing<<" modules with data "<<countModulesWithHistos
	    <<" all modules"<<lookAtModules<<std::endl;
  std::cout <<" empty pixels (no entry in histos) "<<npixEmpty<<" pixels with data entries "<<npixGood<<std::endl;
  std::cout<<npixUsed<<" "<<npixGood2<<" "<<npixBad<<std::endl;

  std::cout << " --- writing to DB!" << std::endl;
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if(!mydbservice.isAvailable() ){
    edm::LogError("db service unavailable");
    return;
  } else {
    if(record_=="SiPixelGainCalibrationForHLTRcd") {
      std::cout << "now doing SiPixelGainCalibrationForHLTRcd payload..." << std::endl;
      if( mydbservice->isNewTagRequest(record_) ) {
	mydbservice->createNewIOV<SiPixelGainCalibrationForHLT>(
				    theGainCalibrationDbInputHLT_,
      				    mydbservice->beginOfTime(),
				    mydbservice->endOfTime(),
				    "SiPixelGainCalibrationForHLTRcd");
      } else {
	mydbservice->appendSinceTime<SiPixelGainCalibrationForHLT>(
				   theGainCalibrationDbInputHLT_, 
				   mydbservice->currentTime(),
				   "SiPixelGainCalibrationForHLTRcd");
      
      }
    } else if (record_=="SiPixelGainCalibrationOfflineRcd") {
      std::cout << "now doing SiPixelGainCalibrationOfflineRcd payload..." << std::endl; 
      if( mydbservice->isNewTagRequest(record_) ) {
	mydbservice->createNewIOV<SiPixelGainCalibrationOffline>(
				  theGainCalibrationDbInputOffline_,
				  mydbservice->beginOfTime(),
				  mydbservice->endOfTime(),
				  "SiPixelGainCalibrationOfflineRcd");
      } else {
	mydbservice->appendSinceTime<SiPixelGainCalibrationOffline>(
				 theGainCalibrationDbInputOffline_, 
				 mydbservice->currentTime(),
				 "SiPixelGainCalibrationOfflineRcd");
      }
    } else if (record_=="SiPixelGainCalibrationRcd") {
      std::cout << "now doing SiPixelGainCalibrationRcd payload..." << std::endl; 
      if( mydbservice->isNewTagRequest(record_) ) {
	mydbservice->createNewIOV<SiPixelGainCalibration>(
							     theGainCalibrationDbInput_,
						             mydbservice->beginOfTime(),
							     mydbservice->endOfTime(),
							     "SiPixelGainCalibrationRcd");
      } else {
	mydbservice->appendSinceTime<SiPixelGainCalibration>(
							     theGainCalibrationDbInput_, 
							     mydbservice->currentTime(),
							     "SiPixelGainCalibrationRcd");
      }
    }

    // else if (record_=="SiPixelGainCalibrationPhase1OfflineRcd"){
    //   std::cout << "now doing SiPixelGainCalibrationPhase1OfflineRcd payload..." << std::endl; 
    //   if( mydbservice->isNewTagRequest(record_) ){
    // 	std::cout << "new tag request" << std::endl;
    // 	mydbservice->createNewIOV<SiPixelGainCalibrationPhase1Offline>(
    // 								       theGainCalibrationDbInputPhase1Offline_,
    // 								       mydbservice->beginOfTime(),
    // 								       mydbservice->endOfTime(),
    // 								       "SiPixelGainCalibrationPhase1OfflineRcd");
    //   }
    //   else{
    // 	std::cout << "NO new tag request" << std::endl;//confirmed
    //   	mydbservice->appendSinceTime<SiPixelGainCalibrationPhase1Offline>(
    //   								    theGainCalibrationDbInputPhase1Offline_, 
    //   								    mydbservice->currentTime(),
    //   								    "SiPixelGainCalibrationPhase1OfflineRcd");	
    //   }
    //}

    edm::LogInfo(" --- all OK");
  } 
}
//--------------------------------------------------------------------------------------
SiPixelGainCalibrationDBUploader::SiPixelGainCalibrationDBUploader(const edm::ParameterSet& iConfig):
  conf_(iConfig),
  appendMode_(conf_.getUntrackedParameter<bool>("appendMode",true)),
  theGainCalibrationDbInput_(0),
  theGainCalibrationDbInputOffline_(0),
  //theGainCalibrationDbInputPhase1Offline_(0),
  theGainCalibrationDbInputHLT_(0),
  theGainCalibrationDbInputService_(iConfig),
  record_(conf_.getUntrackedParameter<std::string>("record","SiPixelGainCalibrationOfflineRcd")),
  gainlow_(0.),gainhi_(1000.),pedlow_(conf_.getUntrackedParameter<double>("pedlow")),pedhi_(conf_.getUntrackedParameter<double>("pedhigh")),
  usemeanwhenempty_(conf_.getUntrackedParameter<bool>("useMeanWhenEmpty",false)),
  rootfilestring_(conf_.getUntrackedParameter<std::string>("inputrootfile","inputfile.root")),
					    //gainmax_(6),
  gainmax_(conf_.getUntrackedParameter<double>("gainmax",6.)),
  pedmax_(conf_.getUntrackedParameter<double>("pedmax")),
  badchi2_(conf_.getUntrackedParameter<double>("badChi2Prob",0.01)), // not used
  nmaxcols(10*52),nmaxrows(160)
{
  //now do what ever initialization is needed
  ::putenv((char*)"CORAL_AUTH_USER=me");
  ::putenv((char*)"CORAL_AUTH_PASSWORD=test");   
  meanGainHist_= new TH1F("meanGainHist","mean Gain Hist",500,0,gainmax_);
  meanPedHist_ = new TH1F("meanPedHist", "mean Ped Hist", 512,-200,pedmax_);
  defaultGain_=new TH2F("defaultGain","default gain, contains mean",nmaxcols,0,nmaxcols,nmaxrows,0,nmaxrows);// using dummy (largest) module size
  defaultPed_=new TH2F("defaultPed","default pedestal, contains mean",nmaxcols,0,nmaxcols,nmaxrows,0,nmaxrows);// using dummy (largest) module size
  defaultFitResult_=new TH2F("defaultFitResult","default fitresult, contains '0'",nmaxcols,0,nmaxcols,nmaxrows,0,nmaxrows);// using dummy (largest) module size
  defaultChi2_=new TH2F("defaultChi2","default chi2 probability, contains '1'",nmaxcols,0,nmaxcols,nmaxrows,0,nmaxrows);// using dummy (largest) module size

  if(gainlow_>gainhi_){  
    float temp=gainhi_;
    gainhi_=gainlow_;
    gainlow_=temp;
  }
  if(pedlow_>pedhi_){
    float temp=pedhi_;
    pedhi_=pedlow_;
    pedlow_=temp;
  }
  if(gainhi_>gainmax_) gainhi_=gainmax_;
  if(pedhi_>pedmax_) pedhi_=pedmax_;

  std::cout<<" max gain "<<gainmax_<<" hi/low gain "<<gainhi_<<"/"<<gainlow_
	   <<" max ped "<<pedmax_<<" hi/low ped "<<pedhi_<<"/"<<pedlow_
	   <<" chi2 cut "<<badchi2_<<" fill empty with mean "<<usemeanwhenempty_<<std::endl;
}
//--------------------------------------------------------------------------

SiPixelGainCalibrationDBUploader::~SiPixelGainCalibrationDBUploader() {}

//
// member functions
//

// ------------ method called to for each event  ------------
void
SiPixelGainCalibrationDBUploader::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{ 
  if (!getHistograms()) {return;}
  fillDatabase(iSetup);
  // empty but should not be called anyway
}
//-------------------------------------------------------------------------------
void SiPixelGainCalibrationDBUploader::beginRun(const edm::EventSetup& iSetup){
}
// ------------ method called once each job just before starting event loop  ------------
void SiPixelGainCalibrationDBUploader::beginJob() {
  countModulesFound=0;
  emptyPed_=emptyGain_=badPed_=badGain_=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void SiPixelGainCalibrationDBUploader::endJob() {
  std::cout<<"Found modules in the root file "<<countModulesFound<<std::endl;
  std::cout<<" pixels with: empty pedestal "<<emptyPed_<<" pedestal off range "<<badPed_
	   <<" empty gain "<<emptyGain_<<" gain off range "<<badGain_<<std::endl;
}
//----------------------------------------------------------------------------
bool SiPixelGainCalibrationDBUploader::getHistograms() {
  std::cout <<"Parsing file " << rootfilestring_ << std::endl;
  therootfile_ = new TFile(rootfilestring_.c_str());
  if ( !therootfile_ || therootfile_->IsZombie() )
    {std::cout << "ERROR: Could not open the root file." << std::endl;return false;}
  therootfile_->cd();
  
  TDirectory *dir = therootfile_->GetDirectory("siPixelGainCalibrationAnalysis");
  TList *list = dir->GetListOfKeys();

  TString comparestring = "Module";

  std::vector<TString> keylist;
  std::vector<TString> hist2list;
  std::vector<TString> dirlist;
  std::vector<TString> notdonelist;
  std::vector<int> nsubdirs;
  int ikey=0;
  int num_dir_list=list->GetEntries() ;

  for(ikey=0;ikey<num_dir_list;  ikey++){
    TKey *thekey = (TKey*)list->At(ikey);
    if(thekey==0)
      continue;
    TString keyname=thekey->GetName();
    TString keytype=thekey->GetClassName();
  
    if(keytype=="TDirectoryFile"){
      TString dirname=dir->GetPath();
      dirname+="/";
      dirname+=keyname;
      dir=therootfile_->GetDirectory(dirname);
  
      list=dir->GetListOfKeys();
      if(dirname.Contains(comparestring)){
	dirlist.push_back(dirname);
      } else {
	notdonelist.push_back(dirname);
	nsubdirs.push_back(-1);
      }
    }
  }
  size_t nempty=0;
  while(nempty!=notdonelist.size()){
    for(size_t idir=0; idir<notdonelist.size(); ++idir){
      if(nsubdirs[idir]==0)
	continue;
      dir = therootfile_->GetDirectory(notdonelist[idir]); 
      list= dir->GetListOfKeys(); 
      int ndirectories=0;
      for(ikey=0;ikey<list->GetEntries();  ikey++){
	TKey *thekey = (TKey*)list->At(ikey);
	if(thekey==0)
	  continue;
	TString keyname=thekey->GetName();
	TString keytype=thekey->GetClassName();
	if(keytype=="TDirectoryFile"){
	  TString dirname=dir->GetPath();
	  dirname+="/";
	  dirname+=keyname;
	  ndirectories++;
	  if(dirname.Contains(comparestring)){
	    dirlist.push_back(dirname);
	  } else{ 
	    notdonelist.push_back(dirname);
	    nsubdirs.push_back(-1);
	  }
	}
      }
      nsubdirs[idir]=ndirectories;
    }
    nempty=0;
    for(size_t i=0; i<nsubdirs.size(); i++){
      if(nsubdirs[i]!=-1)
	nempty++;
    }
  }
  
  for(size_t idir=0; idir<dirlist.size() ; ++idir){
    uint32_t detid = 1;
    
    dir = therootfile_->GetDirectory(dirlist[idir]);
    list = dir->GetListOfKeys();
    for(ikey=0;ikey<list->GetEntries();  ikey++){
      TKey *thekey = (TKey*)list->At(ikey);
      if(thekey==0)
	continue;
      TString keyname=thekey->GetName();
      TString keytype=thekey->GetClassName();
      if(keytype=="TH2F" && (keyname.BeginsWith("Gain2d")||keyname.BeginsWith("Pedestal2d")||keyname.BeginsWith("GainChi2Prob2d")||keyname.BeginsWith("GainFitResult2d"))){
	TString detidstring = keyname;
	detidstring.Remove(0,detidstring.Sizeof()-10);
	
	detid = atoi(detidstring.Data());
	  
	if(keyname.BeginsWith("GainChi2Prob2d")){

	  TString tempstr =dirlist[idir];
	  tempstr+="/";
	  tempstr+=keyname;
	  TString replacestring = rootfilestring_;
	  replacestring+=":";
	  tempstr.ReplaceAll(replacestring,"");
	  bookkeeper_[detid]["chi2prob_2d"]=tempstr;
	} else if(keyname.BeginsWith("GainFitResult2d")){
	  TString tempstr =dirlist[idir];
	  tempstr+="/";
	  tempstr+=keyname;
	  TString replacestring = rootfilestring_;
	  replacestring+=":";
	  tempstr.ReplaceAll(replacestring,"");
	  bookkeeper_[detid]["fitresult_2d"]=tempstr;
	} else if(keyname.BeginsWith("Gain2d")){
	  std::map<std::string,TString> tempmap;
	  TString tempstr =dirlist[idir];
	  tempstr+="/";
	  tempstr+=keyname;
	  TString replacestring = rootfilestring_;
	  replacestring+=":";
	  tempstr.ReplaceAll(replacestring,"");
	  bookkeeper_[detid]["gain_2d"]=tempstr;
	}
	else if(keyname.BeginsWith("Pedestal2d")){
	  std::map<std::string,TString> tempmap;
	  TString tempstr =dirlist[idir];
	  tempstr+="/";
	  tempstr+=keyname;
	  TString replacestring = rootfilestring_;
	  replacestring+=":";
	  tempstr.ReplaceAll(replacestring,"");
	  bookkeeper_[detid]["ped_2d"]=tempstr;
	}
	else
	  {std::cout << "Could not find histo: " << keyname << std::endl;}
      }
    }
    
    TH2F *temphistoped  = (TH2F*)therootfile_->Get(bookkeeper_[detid]["ped_2d"]);
    TH2F *temphistogain = (TH2F*)therootfile_->Get(bookkeeper_[detid]["gain_2d"]);
    TH2F *temphistofitresult = (TH2F*)therootfile_->Get(bookkeeper_[detid]["gain_2d"]);
	
    //std::cout<<std::endl<<" found det "<<detid<<std::endl;
    countModulesFound++;

    for(int xbin=1; xbin<=temphistoped->GetNbinsX(); ++xbin){
      for(int ybin=1; ybin<=temphistoped->GetNbinsY(); ++ybin){
	if(temphistofitresult->GetBinContent(xbin,ybin)<=0) {emptyPed_++;continue;}
	float val = temphistoped->GetBinContent(xbin,ybin);
	if(val>pedmax_) {badPed_++; continue;}  // skip large pedestals
	//std::cout<<val<<" ";
	// if(pedlow_>val)
	//     pedlow_=val;
	//   if(pedhi_<val)
	//     pedhi_=val;
	meanPedHist_->Fill(val);
      }
    }
	  
    for(int xbin=1; xbin<=temphistogain->GetNbinsX(); ++xbin){
      for(int ybin=1; ybin<=temphistogain->GetNbinsY(); ++ybin){
	if(temphistofitresult->GetBinContent(xbin,ybin)<=0) {emptyGain_++;continue;}
	float val = temphistogain->GetBinContent(xbin,ybin);
	if(val<=0.0001 || val>gainmax_) {badGain_++;continue;} // skip low gains
	//std::cout<<val<<",";
	if(gainlow_>val)
	  gainlow_=val;
	if(gainhi_<val)
	  gainhi_=val;
	meanGainHist_->Fill(val);
      }
    }
  }// end of loop over dirlist
  return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelGainCalibrationDBUploader);
