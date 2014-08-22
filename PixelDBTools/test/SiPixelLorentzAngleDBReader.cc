#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleRcd.h"
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleSimRcd.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SiPixelLorentzAngleDBReader.h"

#include <iostream>
#include <stdio.h>
#include <sys/time.h>


using namespace cms;

SiPixelLorentzAngleDBReader::SiPixelLorentzAngleDBReader( const edm::ParameterSet& iConfig ):
  printdebug_(iConfig.getUntrackedParameter<bool>("printDebug",false)),
  useSimRcd_( iConfig.getParameter<bool>("useSimRcd") )
{
}

SiPixelLorentzAngleDBReader::~SiPixelLorentzAngleDBReader(){}

void SiPixelLorentzAngleDBReader::analyze( const edm::Event& e, const edm::EventSetup& iSetup) {

 edm::ESHandle<SiPixelLorentzAngle> SiPixelLorentzAngle_; 

 if(useSimRcd_ == true) {
   iSetup.get<SiPixelLorentzAngleSimRcd>().get(SiPixelLorentzAngle_);
   edm::LogInfo("SiPixelLorentzAngleReader") <<" Show LA for reconstruction "<<std::endl;
   std::cout<<" Show LA for simulations "<<std::endl;
   
 } else {
   //edm::LogInfo("SiPixelLorentzAngleReader") <<" Show LA for reconstruction "<<std::endl;
   std::cout<<" Show LA for reconstruction "<<std::endl;
   iSetup.get<SiPixelLorentzAngleRcd>().get(SiPixelLorentzAngle_);
   // get the payloads with labels
   //iSetup.get<SiPixelLorentzAngleRcd>().get("fromAlignment",SiPixelLorentzAngle_);
   //iSetup.get<SiPixelLorentzAngleRcd>().get("forWidth",SiPixelLorentzAngle_);
 }

 //edm::LogInfo("SiPixelLorentzAngleDBReader") << "[SiPixelLorentzAngleDBReader::analyze] End Reading SiPixelLorentzAngle" << std::endl;
  edm::Service<TFileService> fs;

  LorentzAngleBarrel_ = fs->make<TH1F>("LorentzAngleBarrelPixel","LorentzAngleBarrelPixel",150,0,0.15);
  LorentzAngleForward_= fs->make<TH1F>("LorentzAngleForwardPixel","LorentzAngleForwardPixel",150,0,0.15);

  LABPixL1_[0] = fs->make<TH1F>("LABPixL1Z1","LorentzAngleBPix Lay1 Z1",150,0,0.15);
  LABPixL1_[1] = fs->make<TH1F>("LABPixL1Z2","LorentzAngleBPix Lay1 Z2",150,0,0.15);
  LABPixL1_[2] = fs->make<TH1F>("LABPixL1Z3","LorentzAngleBPix Lay1 Z3",150,0,0.15);
  LABPixL1_[3] = fs->make<TH1F>("LABPixL1Z4","LorentzAngleBPix Lay1 Z4",150,0,0.15);
  LABPixL1_[4] = fs->make<TH1F>("LABPixL1Z5","LorentzAngleBPix Lay1 Z5",150,0,0.15);
  LABPixL1_[5] = fs->make<TH1F>("LABPixL1Z6","LorentzAngleBPix Lay1 Z6",150,0,0.15);
  LABPixL1_[6] = fs->make<TH1F>("LABPixL1Z7","LorentzAngleBPix Lay1 Z7",150,0,0.15);
  LABPixL1_[7] = fs->make<TH1F>("LABPixL1Z8","LorentzAngleBPix Lay1 Z8",150,0,0.15);

  LABPixL2_[0] = fs->make<TH1F>("LABPixL2Z1","LorentzAngleBPix Lay2 Z1",150,0,0.15);
  LABPixL2_[1] = fs->make<TH1F>("LABPixL2Z2","LorentzAngleBPix Lay2 Z2",150,0,0.15);
  LABPixL2_[2] = fs->make<TH1F>("LABPixL2Z3","LorentzAngleBPix Lay2 Z3",150,0,0.15);
  LABPixL2_[3] = fs->make<TH1F>("LABPixL2Z4","LorentzAngleBPix Lay2 Z4",150,0,0.15);
  LABPixL2_[4] = fs->make<TH1F>("LABPixL2Z5","LorentzAngleBPix Lay2 Z5",150,0,0.15);
  LABPixL2_[5] = fs->make<TH1F>("LABPixL2Z6","LorentzAngleBPix Lay2 Z6",150,0,0.15);
  LABPixL2_[6] = fs->make<TH1F>("LABPixL2Z7","LorentzAngleBPix Lay2 Z7",150,0,0.15);
  LABPixL2_[7] = fs->make<TH1F>("LABPixL2Z8","LorentzAngleBPix Lay2 Z8",150,0,0.15);

  LABPixL3_[0] = fs->make<TH1F>("LABPixL3Z1","LorentzAngleBPix Lay3 Z1",150,0,0.15);
  LABPixL3_[1] = fs->make<TH1F>("LABPixL3Z2","LorentzAngleBPix Lay3 Z2",150,0,0.15);
  LABPixL3_[2] = fs->make<TH1F>("LABPixL3Z3","LorentzAngleBPix Lay3 Z3",150,0,0.15);
  LABPixL3_[3] = fs->make<TH1F>("LABPixL3Z4","LorentzAngleBPix Lay3 Z4",150,0,0.15);
  LABPixL3_[4] = fs->make<TH1F>("LABPixL3Z5","LorentzAngleBPix Lay3 Z5",150,0,0.15);
  LABPixL3_[5] = fs->make<TH1F>("LABPixL3Z6","LorentzAngleBPix Lay3 Z6",150,0,0.15);
  LABPixL3_[6] = fs->make<TH1F>("LABPixL3Z7","LorentzAngleBPix Lay3 Z7",150,0,0.15);
  LABPixL3_[7] = fs->make<TH1F>("LABPixL3Z8","LorentzAngleBPix Lay3 Z8",150,0,0.15);


  std::map<unsigned int,float> detid_la= SiPixelLorentzAngle_->getLorentzAngles();
  std::map<unsigned int,float>::const_iterator it;

  double la_old=1.;
  for (it=detid_la.begin();it!=detid_la.end();it++) {
    auto detid = it->first;
    auto la = it->second;

    //if(printdebug_)
    //std::cout  << "detid " << detid<< " \t" << " Lorentz angle  " << la  << std::endl;
    //edm::LogInfo("SiPixelLorentzAngleDBReader")  << "detid " << it->first << " \t" << " Lorentz angle  " << it->second;

    auto subdet   = DetId(detid).subdetId();
    if(subdet == static_cast<int>(PixelSubdetector::PixelBarrel)){
      LorentzAngleBarrel_->Fill(la);

      //if(printdebug_) std::cout<<" bpix detid "<<detid<< " Lorentz angle "<< la<<std::endl;
      //edm::LogInfo("SiPixelLorentzAngleReader")  << " bpix detid " << it->first << " \t" << " Lorentz angle  " << it->second;

      PXBDetId pdetId = PXBDetId(detid);
      //unsigned int detTypeP=pdetId.det();
      //unsigned int subidP=pdetId.subdetId();
      // Barell layer = 1,2,3
      int layerC=pdetId.layer();
      // Barrel ladder id 1-20,32,44.
      int ladderC=pdetId.ladder();
      // Barrel Z-index=1,8
      int zindex=pdetId.module();
      
      if     (layerC==1) LABPixL1_[zindex-1]->Fill(la);
      else if(layerC==2) LABPixL2_[zindex-1]->Fill(la);
      else if(layerC==3) LABPixL3_[zindex-1]->Fill(la);

      if(printdebug_) {
	// print only one ladder, so once per ring
	if(ladderC==1) std::cout<<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<zindex
				<<" Lorentz angle  "<<la<< std::endl;
	//edm::LogInfo("SiPixelLorentzAngleReader")  <<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<zindex<< " Lorentz angle  " << it->second

      } else {

	if(ladderC==1) { // print once per ring 
	  if(la != la_old) {
	    std::cout<<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<zindex<< " Lorentz angle  " <<la << std::endl;
	    //edm::LogInfo("SiPixelLorentzAngleReader")  <<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<zindex<< " Lorentz angle  " << it->second;
	  } // else {std::cout<<"same"<<std::endl;}

	  la_old = la;
	} // ladder=1

      } // if print 

    } else if(subdet == static_cast<int>(PixelSubdetector::PixelEndcap)){
      LorentzAngleForward_->Fill(la);

      PXFDetId pdetId = PXFDetId(detid);       
      int disk=pdetId.disk(); //1,2,3
      int blade=pdetId.blade(); //1-24
      int moduleF=pdetId.module(); //
      int side=pdetId.side(); //size=1 for -z, 2 for +z
      int panel=pdetId.panel(); //panel=1

      if(blade==1 && moduleF==1 && side==1 && panel==1) { // print once per disk 
	std::cout<<"FPix - disk "<<disk<< " Lorentz angle  " << it->second  << std::endl;
	//edm::LogInfo("SiPixelLorentzAngleReader")  <<"FPix - disk "<<disk<< " Lorentz angle  " << it->second;
      }

    } // end b/fpix if 

  }  // end for loop 

}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelLorentzAngleDBReader);
