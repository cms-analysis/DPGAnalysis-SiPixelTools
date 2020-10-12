// -*- C++ -*-
//
// Package:    SiPixelOfflineCalibAnalysisBase
// Class:      SiPixelOfflineCalibAnalysisBase
// 
/**\class SiPixelOfflineCalibAnalysisBase SiPixelOfflineCalibAnalysisBase.cc CalibTracker/SiPixelTools/src/SiPixelOfflineCalibAnalysisBase.cc

 Description: Base class for Pixel calibrations

*/
//
// Original Author:  Evan Klose Friis
//    additions by:  Freya Blekman
//         Created:  Tue Nov  6 17:27:19 CET 2007
//

#include "TList.h"
#include "TGraphErrors.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CalibTracker/SiStripCommon/interface/Book.h"
#include "SiPixelOfflineCalibAnalysisBase.h"
#include "Geometry/CommonTopologies/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelGeomDetType.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" 
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFrameConverter.h"
#include "CondFormats/SiPixelObjects/interface/ElectronicIndex.h"
#include "CondFormats/SiPixelObjects/interface/DetectorIndex.h"
#include "CondFormats/SiPixelObjects/interface/LocalPixel.h"

 
TF1* SiPixelOfflineCalibAnalysisBase::fitFunction_ = NULL;
std::vector<int>  SiPixelOfflineCalibAnalysisBase::vCalValues_Int_(0);
std::vector<int>  SiPixelOfflineCalibAnalysisBase::calibrows_Int_(0);
std::vector<int>  SiPixelOfflineCalibAnalysisBase::calibcols_Int_(0);
// std::vector<short>  SiPixelOfflineCalibAnalysisBase::vCalValues_(0);
// std::vector<short>  SiPixelOfflineCalibAnalysisBase::calibrows(0);
// std::vector<short>  SiPixelOfflineCalibAnalysisBase::calibcols(0);

//
// constructors and destructor
//

// <use name="PhysicsTools/UtilAlgos"/>
SiPixelOfflineCalibAnalysisBase::SiPixelOfflineCalibAnalysisBase(const edm::ParameterSet& iConfig):runnumbers_(0),eventCounter_(0){
   siPixelCalibDigiProducer_ = iConfig.getParameter<edm::InputTag>("DetSetVectorSiPixelCalibDigiTag");
   createOutputFile_ = iConfig.getUntrackedParameter<bool>("saveFile",false);
   outputFileName_ = iConfig.getParameter<std::string>("outputFileName");
   //daqBE_ = &*edm::Service<DQMStore>();
   tPixelCalibDigi = consumes <edm::DetSetVector<SiPixelCalibDigi> > (siPixelCalibDigiProducer_);
   calibrationMode_ = iConfig.getParameter<std::string>("CalibMode");
   nTriggers_ =  iConfig.getParameter<int>("Repeat");
   vCalValues_Int_ = iConfig.getParameter<std::vector<int> >("vCalValues_Int");
   calibcols_Int_ = iConfig.getParameter<std::vector<int> >("calibcols_Int");
   calibrows_Int_ = iConfig.getParameter<std::vector<int> >("calibrows_Int");
   folderMaker_ = new SiPixelFolderOrganizerGC();
   phase1_ = iConfig.getUntrackedParameter<bool>("phase1",false);
}

SiPixelOfflineCalibAnalysisBase::SiPixelOfflineCalibAnalysisBase(){
   throw cms::Exception("") << "ERROR: Classes derived from SiPixelOfflineCalibAnalysisBase must call SiPixelOfflineCalibAnalysisBase::SiPixelOfflineCalibAnalysisBase(const edm::ParameterSet& iConfig) from their constructor." << std::endl;
}

SiPixelOfflineCalibAnalysisBase::~SiPixelOfflineCalibAnalysisBase() { }


//
// member functions
//


//// ------------ method called to for each event  ------------
//void beginRun(const edm::Run& iEvent, const edm::EventSetup&){
//  
//}

void SiPixelOfflineCalibAnalysisBase::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup){
  // Load the calibration information from the database
  //iSetup.get<SiPixelCalibConfigurationRcd>().get(calib_);
  iSetup.get<TrackerDigiGeometryRecord>().get( geom_ );
  iSetup.get<SiPixelFedCablingMapRcd>().get(theCablingMap_);
  edm::ESHandle<SiPixelVCal> siPixelVCal;
  iSetup.get<SiPixelVCalRcd>().get(siPixelVCal);
  
  for(size_t i=0; i<vCalValues_Int_.size(); i++){
    vCalValues_.push_back( (short) vCalValues_Int_[i]);
  }
  for(size_t i=0; i<calibcols_Int_.size(); i++){
    calibcols.push_back( (short) calibcols_Int_[i]);
  }
  
  for(size_t i=0; i<calibrows_Int_.size(); i++){
    calibrows.push_back( (short) calibrows_Int_[i]);
  }
  
  std::cout << "Begin run. Loading calibration file. Mode: " << calibrationMode_ << " nTriggers: " << nTriggers_ << " Vcal steps: " << vCalValues_.size() << std::endl;
  edm::LogInfo("SiPixelOfflineCalibAnalysisBase") << "Calibration file loaded. Mode: " << calibrationMode_ << " nTriggers: " << nTriggers_ << " Vcal steps: " << vCalValues_.size() << std::endl;
  //call calibrationSetup virtual function
  this->calibrationSetup(iSetup);
  theHistogramIdWorker_ = new SiPixelHistogramId(siPixelCalibDigiProducer_.label());
  
  // Get SiPixelVCalDB object, VCal = slope * #electrons + offset
  VcalToEleMap_ = siPixelVCal->getSlopeAndOffset();
  //VCalMapType::iterator it = VcalToEleMap_.begin();
  //std::cout << ">>> Run " << iRun.id().run() << ": VCal to #electron conversion factors per layer/ring (BPix=1000, FPix=2000):\n";
  //while(it!=VcalToEleMap_.end()){
  //   std::cout<<"detid="<<it->first<<": slope="<<it->second.slope<<", offset="<<it->second.offset<<std::endl;
  //   it++;
  //}
}


// ------------ method called to for each event  ------------
void SiPixelOfflineCalibAnalysisBase::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;
  iSetup.get<TrackerDigiGeometryRecord>().get( geom_ );
  iSetup.get<SiPixelFedCablingMapRcd>().get(theCablingMap_);
  //iSetup.get<SiPixelCalibConfigurationRcd>().get(calib_);
  edm::ESHandle<TrackerTopology> tTopo;
  iSetup.get<TrackerTopologyRcd>().get(tTopo);
  tt_ = tTopo.product();
  
  if(eventCounter_==0)
    this->calibrationSetup(iSetup);
  eventCounter_++;
  
  // Check first if you're analyzing the right type of calibration
  if(!checkCorrectCalibrationType())
    return;
   
  uint32_t runnumber = iEvent.id().run();
  if(runnumbers_.size()==0)
    runnumbers_.push_back(runnumber);
  else{
    bool foundnumber = false;
    for(size_t iter=0; iter<runnumbers_.size() && !foundnumber; ++ iter){
      if(runnumbers_[iter]==runnumber){
        foundnumber = true;
        continue;
      }
    }
    if(!foundnumber)
      runnumbers_.push_back(runnumber);
  }
  
  Handle<DetSetVector<SiPixelCalibDigi> > thePlaquettes;
  iEvent.getByToken( tPixelCalibDigi, thePlaquettes);
  
  // Loop over the plaquettes pulsed in this pattern
  DetSetVector<SiPixelCalibDigi>::const_iterator digiIter;
  for(digiIter=thePlaquettes->begin(); digiIter!=thePlaquettes->end(); ++digiIter){
    uint32_t detId = digiIter->id;
    // Check to see if this detID has not been encountered. If not, run the newDetID (pure virtual) function
    if (detIdsEntered_.find(detId) == detIdsEntered_.end()){
      detIdsEntered_.insert(std::make_pair(detId,0));
      detIdNames_.insert(std::make_pair(detId,translateDetIdToString(detId)));
      newDetID(detId);
    }
    DetSet<SiPixelCalibDigi>::const_iterator ipix;
    // Loop over pixels pulsed in the current plaquette
    // and fit (SiPixelGainCalibrationAnalysis::doFits)
    for(ipix=digiIter->data.begin(); ipix!=digiIter->end(); ++ipix){
      this->doFits(detId,ipix);
    }
  }
  
}

void SiPixelOfflineCalibAnalysisBase::beginJob() { }

void SiPixelOfflineCalibAnalysisBase::endJob(){
  this->calibrationEnd();
   edm::LogInfo("SiPixelOfflineCalibAnalysisBase") << "Running end job... output file name is: " << outputFileName_;
   //if (!outputFileName_.empty() && createOutputFile_) 
   //{
   //   edm::LogInfo("SiPixelOfflineCalibAnalysisBase") << "Writing ROOT file to: " << outputFileName_ << std::endl;
   //   daqBE_->save(outputFileName_);
   //}
}

// ------------ helper functions ---------------------------------------------------------

// const std::vector<short>* 
// SiPixelOfflineCalibAnalysisBase::getVcalValues(){
//    return &vCalValues_;
// }

std::string SiPixelOfflineCalibAnalysisBase::translateDetIdToString(uint32_t detid){
   std::map<uint32_t, std::string>::iterator detNameIter = detIdNames_.find(detid);
   if(detNameIter != detIdNames_.end()){
      return detNameIter->second;
   }
   std::string output = "DetID translation error";
   DetId detId(detid);
   uint32_t detSubId = detId.subdetId();
   if(detSubId > 2 || detSubId < 1){
      edm::LogError("SiPixelOfflineCalibAnalysisBase") << "ERROR: Expected a pixel detector ID (1 - barrel, 2 - forward) but got " << detSubId << std::endl;
      return output;
   }
   if(detSubId == 2){ // FPIX
     PixelEndcapName nameworker(detid,tt_,phase1_); //or detID(detid)
      output = nameworker.name();
   }else{             // BPIX
     PixelBarrelName nameworker(detid,tt_,phase1_);
      output = nameworker.name();
   }
   //detIdNames_.insert(std::make_pair(detid,output));
   return output;
}

// MonitorElement* SiPixelOfflineCalibAnalysisBase::bookDQMHistogram1D(uint32_t detid, std::string name, std::string title, int nchX, double lowX, double highX){
//   std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
//   return daqBE_->book1D(hid, title, nchX, lowX, highX);
// }

// MonitorElement* SiPixelOfflineCalibAnalysisBase::bookDQMHistogram1D(uint32_t detid, std::string name, std::string title, int nchX, float *xbinsize){
//   std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
//   return daqBE_->book1D(hid, title, nchX, xbinsize);
// }

// MonitorElement* SiPixelOfflineCalibAnalysisBase::bookDQMHistogram2D(uint32_t detid, std::string name, std::string title, int nchX, double lowX, double highX, int nchY, double lowY, double highY){
//   std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
//   return daqBE_->book2D(hid, title, nchX, lowX, highX, nchY, lowY, highY);
// }

// MonitorElement* SiPixelOfflineCalibAnalysisBase::bookDQMHistoPlaquetteSummary2D(uint32_t detid, std::string name,std::string title){
//   DetId detId(detid);
//   const TrackerGeometry &theTracker(*geom_);
//   const PixelGeomDetUnit *theGeomDet = dynamic_cast<const PixelGeomDetUnit*> ( theTracker.idToDet(detId) ); 
//   int maxcol = theGeomDet->specificTopology().ncolumns();
//   int maxrow = theGeomDet->specificTopology().nrows();
//   std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
//   return daqBE_->book2D(hid,title,maxcol,0,maxcol,maxrow,0,maxrow);
// }

TGraphErrors* SiPixelOfflineCalibAnalysisBase::bookTGraphs(uint32_t detid, std::string name, int points, double *x, double *y, double *xE, double *yE, std::string dir){
  TGraphErrors* temp;
  std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
  //return fs->make<TH1F>( hid.c_str()  , title.c_str(), nchX,  lowX, highX );

  if (myTFileDirMap_.find(dir)==myTFileDirMap_.end()){
    temp= fs->make<TGraphErrors>( points, x, y, xE, yE); 
  }  
  else  temp= myTFileDirMap_[dir].make<TGraphErrors>( points, x, y, xE, yE);
  return temp;// TH1F * TEMP= new TH1F( hid.c_str(), title.c_str(), nchX,  lowX, highX );
  // return  new TH1F( hid.c_str(), title.c_str(), nchX,  lowX, highX );
}

TH1F* SiPixelOfflineCalibAnalysisBase::bookHistogram1D(uint32_t detid, std::string name, std::string title, int nchX, double lowX, double highX, std::string dir){
  TH1F* temp;
  std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
  //return fs->make<TH1F>( hid.c_str(), title.c_str(), nchX,  lowX, highX );

  if (myTFileDirMap_.find(dir)==myTFileDirMap_.end()){
    temp= fs->make<TH1F>( hid.c_str(), title.c_str(), nchX,  lowX, highX ); 
  }
  else  temp= myTFileDirMap_[dir].make<TH1F>( hid.c_str(), title.c_str(), nchX,  lowX, highX );

  return temp;// TH1F * TEMP= new TH1F( hid.c_str(), title.c_str(), nchX,  lowX, highX );
  // return  new TH1F( hid.c_str(), title.c_str(), nchX,  lowX, highX );
}

TH1F* SiPixelOfflineCalibAnalysisBase::bookHistogram1D(uint32_t detid, std::string name, std::string title, int nchX, float *xbinsize, std::string dir){
  std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
  return myTFileDirMap_[dir].make<TH1F>( hid.c_str(), title.c_str(), nchX, xbinsize); 
  // return fs->make<TH1F>(hid.c_str(), title.c_str(), nchX, xbinsize);
  //return  new TH1F( hid.c_str(), title.c_str(), nchX,  xbinsize );
}

TH2F* SiPixelOfflineCalibAnalysisBase::bookHistogram2D(uint32_t detid, std::string name, std::string title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, std::string dir){
  std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
  return myTFileDirMap_[dir].make<TH2F>( hid.c_str(), title.c_str(), nchX,  lowX, highX , nchY,  lowY, highY);
  // return fs->make<TH2F>( hid.c_str(), title.c_str(), nchX,  lowX, highX , nchY,  lowY, highY);
  //  return new TH2F( hid.c_str(), title.c_str(), nchX,  lowX, highX , nchY,  lowY, highY);
}

TH2F* SiPixelOfflineCalibAnalysisBase::bookHistoPlaquetteSummary2D(uint32_t detid, std::string name,std::string title, std::string dir){
  DetId detId(detid);
  const TrackerGeometry &theTracker(*geom_);
  const PixelGeomDetUnit *theGeomDet = dynamic_cast<const PixelGeomDetUnit*> ( theTracker.idToDet(detId) ); 
  int maxcol = theGeomDet->specificTopology().ncolumns();
  int maxrow = theGeomDet->specificTopology().nrows();

  std::string hid = theHistogramIdWorker_->setHistoId(name,detid);
  return myTFileDirMap_[dir].make<TH2F>( hid.c_str(),title.c_str(),maxcol,0,maxcol,maxrow,0,maxrow);
  //return fs->make<TH2F>( hid.c_str(),title.c_str(),maxcol,0,maxcol,maxrow,0,maxrow);
  //return  new TH2F( hid.c_str(),title.c_str(),maxcol,0,maxcol,maxrow,0,maxrow);
}

//bool
//SiPixelOfflineCalibAnalysisBase::setDQMDirectory(std::string dirName)
//{
//   daqBE_->setCurrentFolder(dirName);
//   return daqBE_->dirExists(dirName);
//}
//
//bool
//SiPixelOfflineCalibAnalysisBase::setDQMDirectory(uint32_t detID)
//{
//  return folderMaker_->setModuleFolder(&myTFileDirMap_,detID,tt_,0,phase1_); //Camilla da cambiare o da riscrivere quella vecchia
//;
//}
// bool
// SiPixelOfflineCalibAnalysisBase::GetPixelDirectory(std::string dirName)
// {
//    daqBE_->setCurrentFolder(dirName);
//    return daqBE_->dirExists(dirName);
// }


std::string SiPixelOfflineCalibAnalysisBase::GetPixelDirectory(uint32_t detID){
  //Get the directory name and create it if it doesn't exists///
  // return  folderMaker_->setModuleFolder(detID,0,myTFileDirMap_ );
  std::string path= folderMaker_->setModuleFolderPath(detID,tt_,0,phase1_);
  if (myTFileDirMap_.find(path)==myTFileDirMap_.end()){
    // std::cout << " Creating the directory " << std::endl;
    folderMaker_->setModuleFolder(&myTFileDirMap_,detID,tt_,0,phase1_);
    if (myTFileDirMap_.find(path)==myTFileDirMap_.end()) std::cout << " I think I created the directory but I dont find it" << std::endl;
  }  
  // std::cout << "SiPixelOfflineCalibAnalysisBase.cc  get the directory with path " <<path << std::endl;
  return path;
}


// ------------ virtual functions ------------------------------------------------
bool SiPixelOfflineCalibAnalysisBase::checkCorrectCalibrationType(){
  return true;
}

bool SiPixelOfflineCalibAnalysisBase::doFits(uint32_t detid, std::vector<SiPixelCalibDigi>::const_iterator ipix){ //, std::string layerString
  short row=ipix->row();
  short col=ipix->col();
  std::vector<uint8_t> nentries = ipix->getnentries();
  std::vector<uint32_t> sum = ipix->getsum();
  std::vector<uint32_t> sumquares = ipix->getsumsquares();
  //do nothing
  //return false;
  //
  //DEBUG
  std::cout << "Row: " << row << "   Col: " << col << std::endl;
  for(unsigned int i = 0; i < sum.size(); i++){
     std::cout << sum[i] << "  ";
  }
  std::cout << std::endl;
  return false;
}

void SiPixelOfflineCalibAnalysisBase::calibrationSetup(const edm::EventSetup&) { }

void SiPixelOfflineCalibAnalysisBase::calibrationEnd() { }

void SiPixelOfflineCalibAnalysisBase::newDetID(uint32_t detid){
   edm::LogInfo("SiPixelOfflineCalibAnalysisBase") << "SiPixelOfflineCalibAnalysisBase - Found new DetID: " << detid << "  Name: " << detIdNames_[detid];
}

bool SiPixelOfflineCalibAnalysisBase::checkPixel(uint32_t detid,short row, short col) {
  // finds the fed ID:
  int thefedid = -1;
  for(int fedid=0; fedid<=40 && thefedid==-1; ++fedid){
    SiPixelFrameConverter converter(theCablingMap_.product(),fedid);
    if(converter.hasDetUnit(detid))
      thefedid=fedid;
  }
  if(thefedid==-1)
    return false; // fed ID not associated with det ID. No pattern check possible
  
  SiPixelFrameConverter formatter(theCablingMap_.product(),thefedid);
  sipixelobjects::DetectorIndex detector ={detid, row, col};
  sipixelobjects::ElectronicIndex cabling;
  
  formatter.toCabling(cabling,detector);
  // cabling should now contain cabling.roc and cabling.dcol  and cabling.pxid

  // however, the coordinates now need to be converted from dcl, pxid to the row,col coordinates used in the calibration info
  sipixelobjects::LocalPixel::DcolPxid loc;
  loc.dcol = cabling.dcol;
  loc.pxid = cabling.pxid;
  sipixelobjects::LocalPixel locpixel(loc);
  short localrow = locpixel.rocRow();
  short localcol = locpixel.rocCol();
 
  for(size_t irow=0; irow<calibrows.size(); ++irow){
    if(calibrows[irow]==localrow){
      // check the columns
      for(size_t icol=0; icol<calibcols.size(); ++icol){
        if(calibcols[icol]==localcol)
          return true;
      }
    }
  }

  return false;
}

//// function to add tf1's to ME's:
//void SiPixelOfflineCalibAnalysisBase::addTF1ToDQMMonitoringElement(MonitorElement *ele, TF1 *func){
//  
//  if(func){
//    ele->getTH1()->GetListOfFunctions()->Add(func);
//  }
//  return;
//}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelOfflineCalibAnalysisBase);
