#include <memory>

#include "SiPixelGainsDBReader.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"

#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationForHLTService.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationForHLTSimService.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationOfflineService.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationOfflineSimService.h"
#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationService.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include "DataFormats/DetId/interface/DetId.h"


using namespace std;

SiPixelGainsDBReader::SiPixelGainsDBReader(const edm::ParameterSet& conf): conf_(conf) {

  std::string payloadType = conf_.getParameter<std::string>("payloadType");
  cout<<" type = "<<payloadType<<" sim/reco = "<<  conf_.getParameter<bool>("useSimRcd") <<endl;

  if (strcmp(payloadType.c_str(), "HLT") == 0) { // HLT
    if( conf_.getParameter<bool>("useSimRcd") )  //Sim
      SiPixelGainCalibrationService_ = new  SiPixelGainCalibrationForHLTSimService(conf_);
    else
      SiPixelGainCalibrationService_ = new SiPixelGainCalibrationForHLTService(conf_);
  } else if (strcmp(payloadType.c_str(), "Offline") == 0) {  // Offline 
    if(conf_.getParameter<bool>("useSimRcd"))  //Sim
      SiPixelGainCalibrationService_ = new SiPixelGainCalibrationOfflineSimService(conf_);
    else
      SiPixelGainCalibrationService_ = new SiPixelGainCalibrationOfflineService(conf_);
  } else if (strcmp(payloadType.c_str(), "Full") == 0) { //Full, not used at the moment
    SiPixelGainCalibrationService_ = new SiPixelGainCalibrationService(conf);
  }
  
}

void
SiPixelGainsDBReader::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  

  //Create Subdirectories
  edm::Service<TFileService> fs;
  TFileDirectory subDirPed = fs->mkdir("Pedestals");
  TFileDirectory subDirGain = fs->mkdir("Gains");
  char name[128];

  unsigned int nmodules = 0;
  uint32_t nchannels = 0;
  uint32_t ndead=0;
  uint32_t nnoisy=0;
  
  // Get the calibration data
  SiPixelGainCalibrationService_->setESObjects(iSetup);
  edm::LogInfo("SiPixelGainsDBReader") << "[SiPixelGainsDBReader::beginJob] End Reading Gainsects" << std::endl;

  // Get the Geometry
  iSetup.get<TrackerDigiGeometryRecord>().get( tkgeom );     
  edm::LogInfo("SiPixelGainsDBReader") <<" There are "<<tkgeom->dets().size() <<" detectors"<<std::endl;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  iSetup.get<IdealGeometryRecord>().get(tTopo);

  // Get the list of DetId's
  std::vector<uint32_t> vdetId_ = SiPixelGainCalibrationService_->getDetIds();

  //Create histograms
  _TH1F_Dead_sum =  fs->make<TH1F>("Summary_dead","Dead pixel fraction (0=dead, 1=alive)",vdetId_.size()+1,0,vdetId_.size()+1);
  _TH1F_Dead_all =  fs->make<TH1F>("DeadAll","Dead pixel fraction (0=dead, 1=alive)",50,0.,conf_.getUntrackedParameter<double>("maxRangeDeadPixHist",0.001));
  _TH1F_Noisy_sum = fs->make<TH1F>("Summary_noisy","Noisy pixel fraction (0=noisy, 1=alive)",vdetId_.size()+1,0,vdetId_.size()+1);
  _TH1F_Noisy_all = fs->make<TH1F>("NoisyAll","Noisy pixel fraction (0=noisy, 1=alive)",50,0.,conf_.getUntrackedParameter<double>("maxRangeDeadPixHist",0.001));
  _TH1F_Gains_sum =  fs->make<TH1F>("Summary_Gain","Gain Summary", vdetId_.size()+1,0,vdetId_.size()+1);
  _TH1F_Pedestals_sum =  fs->make<TH1F>("Summary_Pedestal","Pedestal Summary", vdetId_.size()+1,0,vdetId_.size()+1);
  _TH1F_Pedestals_all = fs->make<TH1F>("PedestalsAll","all Pedestals",350,-100,250);
  _TH1F_Pedestals_bpix = fs->make<TH1F>("PedestalsBpix","bpix Pedestals",350,-100,250);
  _TH1F_Pedestals_fpix = fs->make<TH1F>("PedestalsFpix","fpix Pedestals",350,-100,250);
  _TH1F_Gains_all = fs->make<TH1F>("GainsAll","all Gains",100,0,10);
  _TH1F_Gains_bpix = fs->make<TH1F>("GainsBpix", "bpix Gains", 100, 0, 10);
  _TH1F_Gains_fpix = fs->make<TH1F>("GainsFpix", "fpix Gains", 100, 0, 10);

  TTree *tree = new TTree("tree","tree");
  uint32_t detid;
  double gainmeanfortree, gainrmsfortree, pedmeanfortree, pedrmsfortree;
  tree->Branch("detid",&detid,"detid/I");
  tree->Branch("ped_mean",&pedmeanfortree,"ped_mean/D");
  tree->Branch("ped_rms",&pedrmsfortree,"ped_rms/D");
  tree->Branch("gain_mean",&gainmeanfortree,"gain_mean/D");
  tree->Branch("gain_rms",&gainrmsfortree,"gain_rms/D");

  // Loop over DetId's
  const bool PRINT = true;
  int ibin = 1;
  for (std::vector<uint32_t>::const_iterator detid_iter=vdetId_.begin();detid_iter!=vdetId_.end();detid_iter++){
    bool select = false;
    detid = *detid_iter;

    sprintf(name,"Pedestals_%d",detid);
    _TH1F_Pedestals_m[detid] = subDirPed.make<TH1F>(name,name,350,-100.,250.);    
    sprintf(name,"Gains_%d",detid);
    _TH1F_Gains_m[detid] = subDirGain.make<TH1F>(name,name,100,0.,10.); 

    DetId detIdObject(detid);
    const PixelGeomDetUnit* _PixelGeomDetUnit = dynamic_cast<const PixelGeomDetUnit*>(tkgeom->idToDetUnit(DetId(detid)));
    if (_PixelGeomDetUnit==0){
      edm::LogError("SiPixelCondObjHLTDisplay")<<"[SiPixelCondObjHLTReader::beginJob] the detID "<<detid<<" doesn't seem to belong to Tracker"<<std::endl; 
      continue;
    }     
   
      
    _deadfrac_m[detid]=0.;
    _noisyfrac_m[detid]=0.;

    nmodules++;

    const GeomDetUnit      * geoUnit = tkgeom->idToDetUnit( detIdObject );
    const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
    const PixelTopology & topol = pixDet->specificTopology();       


    if ( detIdObject.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel) ) {

      // new indecies 
      // Barell layer = 1,2,3
      unsigned int layerC1 = tTopo->pxbLayer(detIdObject);
      // Barrel ladder id 1-20,32,44.
      unsigned int ladderC1 = tTopo->pxbLadder(detIdObject);
      // Barrel Z-index=1,8
      unsigned int zindex1 = tTopo->pxbModule(detIdObject);

      if(layerC1==1 && ladderC1==1 && zindex1==1) select=true;
      if(layerC1==2 && ladderC1==1 && zindex1==1) select=true;
      if(layerC1==3 && ladderC1==1 && zindex1==1) select=true;
      if(layerC1==4 && ladderC1==1 && zindex1==1) select=true;
      if(PRINT&&select) cout<<" Barrel layer, ladder, module "
		    <<layerC1<<" "<<ladderC1<<" "<<zindex1<<endl;

      // Online 
      //PixelBarrelName pbn(detIdObject); // use det-id
      //PixelBarrelName::Shell sh = pbn.shell(); //enum
      //int sector = pbn.sectorName();
      //int ladder = pbn.ladderName();
      //int layer  = pbn.layerName();
      //int module = pbn.moduleName();
      //bool half  = pbn.isHalfModule();
      //string name= pbn.name();
      //PixelModuleName::ModuleType moduleType = pbn.moduleType();

      // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
      //int shell = int(sh);
      // change the module sign for z<0
      //if(shell==1 || shell==2) module = -module;
      // change ladeer sign for Outer )x<0)
      //if(shell==1 || shell==3) ladder = -ladder;

      //if(layer==1 && ladder==1 && module==1) select=true;
      //if(PRINT&&select) cout<<" shell "<<sh<<"("<<shell<<") "<<sector<<" "<<layer<<" "<<ladder<<" "
      //            <<module<<" "<<half<<" "<<name<<" "<<moduleType<<endl;

    } else {  // endcaps

      // new ids 
      int disk=tTopo->pxfDisk(detIdObject); //1,2,3
      int blade=tTopo->pxfBlade(detIdObject); //1-24
      int plaq=tTopo->pxfModule(detIdObject); //
      int side=tTopo->pxfSide(detIdObject); //size=1 for -z, 2 for +z
      int panel=tTopo->pxfPanel(detIdObject); //panel=1

      if(disk==1 && blade==1 && side==1 && panel==1 && plaq==1 ) select=true;
      if(disk==2 && blade==1 && side==1 && panel==1 && plaq==1 ) select=true;
      if(disk==3 && blade==1 && side==1 && panel==1 && plaq==1 ) select=true;
      if(PRINT&&select) cout<<"endcap, size "<<side<<" disk "<<disk<<", blade "
		    <<blade<<", panel "
		    <<panel<<", plaq "<<plaq<<endl;
 

      // Convert to online 
      //PixelEndcapName pen(detIdObject); // use det-id
      //string nameF = pen.name();
      //int diskName = pen.diskName();
      //int bladeName = pen.bladeName();
      //int pannelName = pen.pannelName();
      //int plaquetteName = pen.plaquetteName();
      //PixelEndcapName::HalfCylinder part = pen.halfCylinder();
      //PixelModuleName::ModuleType moduleType = pen.moduleType();
      //if(PRINT&&select) cout<<nameF<<" "<<diskName<<" "<<bladeName<<" "<<pannelName<<" "
      //	    <<plaquetteName<<" "<<part<<" "<<moduleType<<endl;

    }

    // Get the module sizes.
    int nrows = topol.nrows();      // rows in x
    int ncols = topol.ncolumns();   // cols in y
    float nchannelspermod=0;

    for(int col_iter=0; col_iter<ncols; col_iter++) {
       for(int row_iter=0; row_iter<nrows; row_iter++) {
	 nchannelspermod++;
	 nchannels++;
	 
	 if(SiPixelGainCalibrationService_->isDead(detid,col_iter,row_iter)) {
	    //	    std::cout << "found dead pixel " << detid << " " <<col_iter << "," << row_iter << std::endl;
	   ndead++;
	   _deadfrac_m[detid]++;
	   continue;
	 } else if(SiPixelGainCalibrationService_->isNoisy(detid,col_iter,row_iter)) {
	    //	    std::cout << "found noisy pixel " << detid << " " <<col_iter << "," << row_iter << std::endl;
	   nnoisy++;
	   _noisyfrac_m[detid]++;
	   continue;
	 }


	 float gain  = SiPixelGainCalibrationService_->getGain(detid, col_iter, row_iter);
	 _TH1F_Gains_m[detid]->Fill( gain );
	 _TH1F_Gains_all->Fill(gain);

	 if (detIdObject.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel))
    	    _TH1F_Gains_bpix->Fill(gain);
	 if (detIdObject.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap))
	   _TH1F_Gains_fpix->Fill(gain);

	 float ped  = SiPixelGainCalibrationService_->getPedestal(detid, col_iter, row_iter);
	 _TH1F_Pedestals_m[detid]->Fill( ped );
       	 _TH1F_Pedestals_all->Fill(ped);

         if (detIdObject.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel))
	   _TH1F_Pedestals_bpix->Fill(ped);
	 if (detIdObject.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap))
	   _TH1F_Pedestals_fpix->Fill(ped);

	 if(PRINT && select && (col_iter%10==0) && (row_iter%20==0) ) 
	    std::cout <<" DetId "<<detid<<" Col "<<col_iter<<" Row "<<row_iter
	    <<" Ped "<<ped<<" Gain "<<gain<<std::endl;	 
       }
    }

    _deadfrac_m[detid]/=nchannelspermod;
    _noisyfrac_m[detid]/=nchannelspermod;
    _TH1F_Dead_sum->SetBinContent(ibin,_deadfrac_m[detid]);
    _TH1F_Dead_all->Fill(_deadfrac_m[detid]);
    _TH1F_Noisy_sum->SetBinContent(ibin,_noisyfrac_m[detid]);
    _TH1F_Noisy_all->Fill(_noisyfrac_m[detid]);
    _TH1F_Gains_sum->SetBinContent(ibin,_TH1F_Gains_m[detid]->GetMean());
    _TH1F_Gains_sum->SetBinError(ibin,_TH1F_Gains_m[detid]->GetRMS());
    _TH1F_Pedestals_sum->SetBinContent(ibin,_TH1F_Pedestals_m[detid]->GetMean());
    _TH1F_Pedestals_sum->SetBinError(ibin,_TH1F_Pedestals_m[detid]->GetRMS());

    gainmeanfortree = _TH1F_Gains_m[detid]->GetMean();
    gainrmsfortree  = _TH1F_Gains_m[detid]->GetRMS();
    pedmeanfortree  = _TH1F_Pedestals_m[detid]->GetMean();
    pedrmsfortree   = _TH1F_Pedestals_m[detid]->GetRMS();
    //std::cout<<"DetId "<<detid<<"       GainMean "<<gainmeanfortree<<" RMS "<<gainrmsfortree<<"      PedMean "<<pedmeanfortree<<" RMS "<<pedrmsfortree<<std::endl;
    tree->Fill();

    ibin++;
   
  }
  
  edm::LogInfo("SiPixelGainsDBReader") <<"[SiPixelGainsDBReader::analyze] ---> PIXEL Modules  " << nmodules  << std::endl;
  edm::LogInfo("SiPixelGainsDBReader") <<"[SiPixelGainsDBReader::analyze] ---> PIXEL Channels (i.e. Number of Columns)" << nchannels << std::endl;
  
  std::cout<<" ---> SUMMARY :"<<std::endl;
  std::cout<<"Encounted "<<ndead<<" dead pixels"<<std::endl;
  std::cout<<"Encounted "<<nnoisy<<" noisy pixels"<<std::endl;
  std::cout<<"The Gain Mean is "<<_TH1F_Gains_all->GetMean()<<" with rms "<<_TH1F_Gains_all->GetRMS()<<std::endl;
  std::cout<<"         in BPIX "<<_TH1F_Gains_bpix->GetMean()<<" with rms "<<_TH1F_Gains_bpix->GetRMS()<<std::endl;
  std::cout<<"         in FPIX "<<_TH1F_Gains_fpix->GetMean()<<" with rms "<<_TH1F_Gains_fpix->GetRMS()<<std::endl;
  std::cout<<"The Ped Mean is "<<_TH1F_Pedestals_all->GetMean()<<" with rms "<<_TH1F_Pedestals_all->GetRMS()<<std::endl;
  std::cout<<"         in BPIX "<<_TH1F_Pedestals_bpix->GetMean()<<" with rms "<<_TH1F_Pedestals_bpix->GetRMS()<<std::endl;
  std::cout<<"         in FPIX "<<_TH1F_Pedestals_fpix->GetMean()<<" with rms "<<_TH1F_Pedestals_fpix->GetRMS()<<std::endl;
    
}

// ------------ method called once each job just before starting event loop  ------------
void 
SiPixelGainsDBReader::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SiPixelGainsDBReader::endJob() {
  std::cout<<" ---> End job "<<std::endl;
}
//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelGainsDBReader);
