#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleRcd.h"
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleSimRcd.h"
#include "Geometry/CommonTopologies/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" //???

//#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
//#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
//#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h" //???
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

#include "SiPixelLorentzAngleDBReader.h"

#include <iostream>
#include <stdio.h>
#include <sys/time.h>


using namespace std;
using namespace cms;

SiPixelLorentzAngleDBReader::SiPixelLorentzAngleDBReader( const edm::ParameterSet& iConfig ):
  printdebug_(iConfig.getUntrackedParameter<bool>("printDebug",false)),
  useSimRcd_( iConfig.getParameter<bool>("useSimRcd") ),
  tagLabel_( iConfig.getUntrackedParameter<std::string>("label"," ")) {

  cout<<" LA Reader "<<printdebug_<<endl;

}

SiPixelLorentzAngleDBReader::~SiPixelLorentzAngleDBReader(){}

void SiPixelLorentzAngleDBReader::analyze( const edm::Event& e, const edm::EventSetup& iSetup) {

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  //iSetup.get<IdealGeometryRecord>().get(tTopo);
  iSetup.get<TrackerTopologyRcd>().get(tTopo);
  //const TrackerTopology* tt = tTopo.product()

 edm::ESHandle<SiPixelLorentzAngle> SiPixelLorentzAngle_; 

 if(useSimRcd_ == true) {
   iSetup.get<SiPixelLorentzAngleSimRcd>().get(SiPixelLorentzAngle_);
   edm::LogInfo("SiPixelLorentzAngleReader") <<" Show LA for reconstruction "<<std::endl;
   std::cout<<" Show LA for simulations "<<std::endl;
   
 } else {
   //edm::LogInfo("SiPixelLorentzAngleReader") <<" Show LA for reconstruction "<<std::endl;
   std::cout<<" Show LA for reconstruction, use label = "<<tagLabel_<<std::endl;
   // get the payloads with labels
   if(tagLabel_=="fromAlignment") {
     iSetup.get<SiPixelLorentzAngleRcd>().get("fromAlignment",SiPixelLorentzAngle_);
   } else if(tagLabel_=="forWidth") {
     iSetup.get<SiPixelLorentzAngleRcd>().get("forWidth",SiPixelLorentzAngle_);
   } else {  // the nomral no label one 
     iSetup.get<SiPixelLorentzAngleRcd>().get(SiPixelLorentzAngle_);
   }
 }

  edm::Service<TFileService> fs;
  LorentzAngleBarrel_ = fs->make<TH1F>("LorentzAngleBarrelPixel","LorentzAngleBarrelPixel",150,0,0.15);
  LorentzAngleForward_= fs->make<TH1F>("LorentzAngleForwardPixel","LorentzAngleForwardPixel",150,0,0.15);

  // LABPixL1_[0] = fs->make<TH1F>("LABPixL1Z1","LorentzAngleBPix Lay1 Z1",150,0,0.15);
  // LABPixL1_[1] = fs->make<TH1F>("LABPixL1Z2","LorentzAngleBPix Lay1 Z2",150,0,0.15);
  // LABPixL1_[2] = fs->make<TH1F>("LABPixL1Z3","LorentzAngleBPix Lay1 Z3",150,0,0.15);
  // LABPixL1_[3] = fs->make<TH1F>("LABPixL1Z4","LorentzAngleBPix Lay1 Z4",150,0,0.15);
  // LABPixL1_[4] = fs->make<TH1F>("LABPixL1Z5","LorentzAngleBPix Lay1 Z5",150,0,0.15);
  // LABPixL1_[5] = fs->make<TH1F>("LABPixL1Z6","LorentzAngleBPix Lay1 Z6",150,0,0.15);
  // LABPixL1_[6] = fs->make<TH1F>("LABPixL1Z7","LorentzAngleBPix Lay1 Z7",150,0,0.15);
  // LABPixL1_[7] = fs->make<TH1F>("LABPixL1Z8","LorentzAngleBPix Lay1 Z8",150,0,0.15);

  // LABPixL2_[0] = fs->make<TH1F>("LABPixL2Z1","LorentzAngleBPix Lay2 Z1",150,0,0.15);
  // LABPixL2_[1] = fs->make<TH1F>("LABPixL2Z2","LorentzAngleBPix Lay2 Z2",150,0,0.15);
  // LABPixL2_[2] = fs->make<TH1F>("LABPixL2Z3","LorentzAngleBPix Lay2 Z3",150,0,0.15);
  // LABPixL2_[3] = fs->make<TH1F>("LABPixL2Z4","LorentzAngleBPix Lay2 Z4",150,0,0.15);
  // LABPixL2_[4] = fs->make<TH1F>("LABPixL2Z5","LorentzAngleBPix Lay2 Z5",150,0,0.15);
  // LABPixL2_[5] = fs->make<TH1F>("LABPixL2Z6","LorentzAngleBPix Lay2 Z6",150,0,0.15);
  // LABPixL2_[6] = fs->make<TH1F>("LABPixL2Z7","LorentzAngleBPix Lay2 Z7",150,0,0.15);
  // LABPixL2_[7] = fs->make<TH1F>("LABPixL2Z8","LorentzAngleBPix Lay2 Z8",150,0,0.15);

  // LABPixL3_[0] = fs->make<TH1F>("LABPixL3Z1","LorentzAngleBPix Lay3 Z1",150,0,0.15);
  // LABPixL3_[1] = fs->make<TH1F>("LABPixL3Z2","LorentzAngleBPix Lay3 Z2",150,0,0.15);
  // LABPixL3_[2] = fs->make<TH1F>("LABPixL3Z3","LorentzAngleBPix Lay3 Z3",150,0,0.15);
  // LABPixL3_[3] = fs->make<TH1F>("LABPixL3Z4","LorentzAngleBPix Lay3 Z4",150,0,0.15);
  // LABPixL3_[4] = fs->make<TH1F>("LABPixL3Z5","LorentzAngleBPix Lay3 Z5",150,0,0.15);
  // LABPixL3_[5] = fs->make<TH1F>("LABPixL3Z6","LorentzAngleBPix Lay3 Z6",150,0,0.15);
  // LABPixL3_[6] = fs->make<TH1F>("LABPixL3Z7","LorentzAngleBPix Lay3 Z7",150,0,0.15);
  // LABPixL3_[7] = fs->make<TH1F>("LABPixL3Z8","LorentzAngleBPix Lay3 Z8",150,0,0.15);

  // LABPixL4_[0] = fs->make<TH1F>("LABPixL4Z1","LorentzAngleBPix Lay4 Z1",150,0,0.15);
  // LABPixL4_[1] = fs->make<TH1F>("LABPixL4Z2","LorentzAngleBPix Lay4 Z2",150,0,0.15);
  // LABPixL4_[2] = fs->make<TH1F>("LABPixL4Z3","LorentzAngleBPix Lay4 Z3",150,0,0.15);
  // LABPixL4_[3] = fs->make<TH1F>("LABPixL4Z4","LorentzAngleBPix Lay4 Z4",150,0,0.15);
  // LABPixL4_[4] = fs->make<TH1F>("LABPixL4Z5","LorentzAngleBPix Lay4 Z5",150,0,0.15);
  // LABPixL4_[5] = fs->make<TH1F>("LABPixL4Z6","LorentzAngleBPix Lay4 Z6",150,0,0.15);
  // LABPixL4_[6] = fs->make<TH1F>("LABPixL4Z7","LorentzAngleBPix Lay4 Z7",150,0,0.15);
  // LABPixL4_[7] = fs->make<TH1F>("LABPixL4Z8","LorentzAngleBPix Lay4 Z8",150,0,0.15);


  std::map<unsigned int,float> detid_la= SiPixelLorentzAngle_->getLorentzAngles();
  std::map<unsigned int,float>::const_iterator it;
  int countBPix=0, countFPix=0, countEPix=0;

  //double la1New=-1. , la2New=-1. , la3New=-1.;
  bool l1z[8]={false,false,false,false,false,false,false,false};
  bool l2z[8]={false,false,false,false,false,false,false,false};
  bool l3z[8]={false,false,false,false,false,false,false,false};
  bool l4z[8]={false,false,false,false,false,false,false,false};

  for (it=detid_la.begin();it!=detid_la.end();it++) {
    auto detid = it->first;
    auto la = it->second;

    //if(printdebug_)
    //std::cout  << "detid " << detid<< " \t" << " Lorentz angle  " << la  << std::endl;

    auto subdet   = DetId(detid).subdetId();

    if(subdet == static_cast<int>(PixelSubdetector::PixelBarrel)) {  // BPix
      countBPix++;
      LorentzAngleBarrel_->Fill(la);
      
      unsigned int layerC = tTopo->pxbLayer(detid);
      // Barrel ladder id 1-20,32,44.
      unsigned int ladderC = tTopo->pxbLadder(detid);
      // Barrel Z-index=1,8
      unsigned int moduleC = tTopo->pxbModule(detid);

      //if(printdebug_) std::cout<<" bpix detid "<<detid<< " Lorentz angle "<< la<<std::endl;
      //edm::LogInfo("SiPixelLorentzAngleReader")  << " bpix detid " << it->first << " \t" << " Lorentz angle  " << it->second;      
      //if     (layerC==1) LABPixL1_[moduleC-1]->Fill(la);
      //else if(layerC==2) LABPixL2_[moduleC-1]->Fill(la);
      //else if(layerC==3) LABPixL3_[moduleC-1]->Fill(la);
      //else if(layerC==4) LABPixL4_[moduleC-1]->Fill(la);

      if(printdebug_) {

	std::cout<<detid<<" BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<moduleC
		 << " Lorentz angle  " <<la << std::endl;

      } else {

	// print only one ladder, so once per ring, seperate new modules 
	if     (layerC==1) {
	  if(!l1z[moduleC-1])  
	    std::cout<<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<moduleC
		     <<" Lorentz angle  "<<la<< std::endl;
	  l1z[moduleC-1]=true;
	} else if(layerC==2) {
	  if(!l2z[moduleC-1])  
	    std::cout<<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<moduleC
		     <<" Lorentz angle  "<<la<< std::endl;
	  l2z[moduleC-1]=true;
	  
	} else if(layerC==3) {
	  if(!l3z[moduleC-1])  
	    std::cout<<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<moduleC
		     <<" Lorentz angle  "<<la<< std::endl;
	  l3z[moduleC-1]=true;
	  
	} else if(layerC==4) {
	  if(!l4z[moduleC-1])  
	    std::cout<<"BPix - layer "<<layerC<<" ladder "<<ladderC<<" ring "<<moduleC
		     <<" Lorentz angle  "<<la<< std::endl;
	  l4z[moduleC-1]=true;
	}

      } // end print if

    } else if(subdet == static_cast<int>(PixelSubdetector::PixelEndcap)){
      
      LorentzAngleForward_->Fill(la);

      unsigned int disk=tTopo->pxfDisk(detid);   //1,2,3
      unsigned int blade=tTopo->pxfBlade(detid); //1-24
      unsigned int side=tTopo->pxfSide(detid);   //sizd=1 for -z, 2 for +z
      unsigned int panel=tTopo->pxfPanel(detid);   //sizd=1 for -z, 2 for +z
      unsigned int moduleF=tTopo->pxfModule(detid); //
      
      if(disk<=8) countFPix++;
      else countEPix++;

      if(printdebug_) {
	std::cout<<detid<<" FPix - disk "<<disk<<" blade "<<blade<<" side "<<side
		 <<" pand; "<<panel<<" module "<<moduleF<< " Lorentz angle  " <<la << std::endl;
      } else {

	if(blade==1 && side==1 && panel==1) { // print for 2 disks, 4 plaquettes
	  std::cout<<"FPix - disk "<<disk<< " Plq "<<moduleF<<" Lorentz angle  " << it->second  << std::endl;
	}
      }


    } else {
      //if(printdebug_)
      //std::cout  << "Unknown det - detid " << detid<<" "<< " Lorentz angle  " << la  << std::endl;
    } // end b/fpix if 

  }  // end for loop 

  std::cout<<" Modules "<<countBPix<<" TBPX "<<countFPix<<" TFPX "<<countEPix<<" TEPX "<<std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelLorentzAngleDBReader);
