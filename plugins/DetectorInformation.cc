// ----------------------------------------------------------------------
// DetectorInformation
// -------------------
//
// ----------------------------------------------------------------------
// Send all questions, wishes and complaints to the 
//
// Author:  Urs Langenegger (PSI)
// ----------------------------------------------------------------------

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#include "DetectorInformation.h"

#include "CondFormats/Alignment/interface/Definitions.h"
#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "CondFormats/SiPixelObjects/interface/DetectorIndex.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/Provenance/interface/Timestamp.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>

#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

using namespace std;
using namespace edm;
using namespace reco;

// ----------------------------------------------------------------------
DetectorInformation::DetectorInformation(edm::ParameterSet const& iConfig): 
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fRootFileName(iConfig.getUntrackedParameter<string>("rootFileName", string("pixelTree.root"))),
  fInit(0)
{
  string rcsid = string("$Id: DetectorInformation.cc,v 1.2 2010/09/21 08:26:18 ursl Exp $");
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- DetectorInformation constructor" << endl;
  cout << "---  version:                         " << rcsid << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  cout << "---  rootFileName:                    " << fRootFileName << endl;
  cout << "----------------------------------------------------------------------" << endl;

  init();

}

// ----------------------------------------------------------------------
DetectorInformation::~DetectorInformation() { }  

// ----------------------------------------------------------------------
void DetectorInformation::endJob() { 
  fFile->cd();

  fFile->Write();
  fFile->Close();
  delete fFile;

  for (int i = 0; i < 40; ++i) {
    delete fPFC[i];
  }

  cout << "==>DetectorInformation> Succesfully gracefully ended job" << endl;

}

// ----------------------------------------------------------------------
void DetectorInformation::beginJob() {
  
  // -- a 'feature' of TFileService is that it does not like to add trees until a histogram is also created.
  //  edm::Service<TFileService> fs;
  //  fH1 = fs->make<TH1D>("nevt","nevt", 1, 0., 1.);

  // -- switch to direct TFile as a *** glibc detected *** corrupted double-linked list: 0x13389518 ***
  //    leaves the TFileService empty...
  fFile = TFile::Open(fRootFileName.c_str(), "RECREATE");
  fFile->cd();

}


// ----------------------------------------------------------------------
void DetectorInformation::analyze(const edm::Event& iEvent, 
			const edm::EventSetup& iSetup) {

  if (0 == fInit) {
    fInit = 1; 
    // -- Setup cabling map and its map to detIDs
    iSetup.get<SiPixelFedCablingMapRcd>().get(fCablingMap);
    for (int i = 0; i < 40; ++i) {
      fPFC[i] = new SiPixelFrameConverter(fCablingMap.product(), i);
    }
    
    
    edm::ESHandle<TrackerGeometry> pDD;
    iSetup.get<TrackerDigiGeometryRecord>().get(pDD);

    for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){
      if(dynamic_cast<PixelGeomDetUnit*>((*it))!=0){
	DetId detId = (*it)->geographicalId();
	uint32_t newDetId = detId;
	
	for (int fedid = 0; fedid < 40; ++fedid) {
	  if (fPFC[fedid]->hasDetUnit(newDetId)) {
	    fFEDID.insert(make_pair(newDetId, fedid)); 
	    break;
	  }
	}
      }
    }

  }

  dumpDetIds(iSetup); 

} 


// ----------------------------------------------------------------------
void DetectorInformation::init() {
    
}


// ----------------------------------------------------------------------
void DetectorInformation::ROClist(const DetId &pID) {

  DetId detId = pID;
  int realfedID = -1;
  uint32_t newDetId = detId;

  set<int> roclist;

  for (int irow = 0; irow < 600; irow += 50) {
    for (int icol = 0; icol < 600; icol += 50) {
  
      sipixelobjects::ElectronicIndex cabling; 
      sipixelobjects::DetectorIndex detector = {detId, irow, icol};      
      
      realfedID = fFEDID[newDetId];
      int OK = fPFC[realfedID]->toCabling(cabling, detector);
      
      if (1 == OK) continue;
      
      sipixelobjects::CablingPathToDetUnit path = {realfedID, cabling.link, cabling.roc};  
      const sipixelobjects::PixelROC *theRoc = fCablingMap->findItem(path);
      
      int roc = theRoc->idInDetUnit();
      uint32_t detSubId = detId.subdetId();
      if (detSubId == 1) {
	PixelBarrelName nameworker(detId);
	std::string outputname = nameworker.name();
	bool HalfModule = nameworker.isHalfModule();
	if ((outputname.find("mO") != string::npos || outputname.find("mI") != string::npos) && (HalfModule)) {
	  roc = theRoc->idInDetUnit() + 8;
	}
      }
      
      roclist.insert(roc);
    }
  }

  for (unsigned int i= 0; i < 16; ++i) {
    if (roclist.count(i) > 0) cout << i << " " ; 
  }
  cout << endl; 
}


// ----------------------------------------------------------------------
void DetectorInformation::dumpDetIds(const edm::EventSetup& iSetup) {

  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );

  cout << "**********************************************************************" << endl;
  cout << " *** Geometry node for TrackerGeom is  "<<&(*pDD)<<std::endl;
  cout << " *** I have " << pDD->dets().size() <<" detectors"<<std::endl;
  cout << " *** I have " << pDD->detTypes().size() <<" types"<<std::endl;
  
  for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){
    
    if(dynamic_cast<PixelGeomDetUnit*>((*it))!=0){
      DetId detId = (*it)->geographicalId();
      if (detId.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {

	PixelBarrelName::Shell DBshell = PixelBarrelName::PixelBarrelName(detId).shell();
	int DBlayer  = PixelBarrelName::PixelBarrelName(detId).layerName();
	int DBladder = PixelBarrelName::PixelBarrelName(detId).ladderName();
	int DBmodule = PixelBarrelName::PixelBarrelName(detId).moduleName();
	int DBsector = PixelBarrelName::PixelBarrelName(detId).sectorName();

	string quad; 
	if (DBshell == PixelBarrelName::mO) {
	  quad = string("BmO");
	}
	if (DBshell == PixelBarrelName::pO) {
	  quad = string("BpO");
	}
	if (DBshell == PixelBarrelName::mI) {
	  quad = string("BmI");
	}
	if (DBshell == PixelBarrelName::pI) {
	  quad = string("BpI");
	}
	
	cout << "BPix_" << quad << "_SEC" << DBsector << "_LYR" << DBlayer << "_LDR" << DBladder << "_MOD" << DBmodule 
	     << "  " << detId.rawId() 
	     << " with ROCs: ";

	ROClist(detId); 

	
	if (DBshell == PixelBarrelName::mO) {
	  DBladder *= -1;
	  DBmodule *= -1;
	}
	
	if (DBshell == PixelBarrelName::mI) {
	  DBmodule *= -1;
	}
	
	if (DBshell == PixelBarrelName::pO) {
	  DBladder *= -1;
	}

      }	else if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
	uint32_t id = detId();
       
        PixelEndcapName::HalfCylinder side = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).halfCylinder();
        int disk   = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).diskName();
        int blade  = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).bladeName();
        int panel  = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).pannelName();
        int module = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).plaquetteName();


	string quad; 
	if (side == PixelEndcapName::mO) {
	  quad = string("BmO");
	}
	if (side == PixelEndcapName::pO) {
	  quad = string("BpO");
	}
	if (side == PixelEndcapName::mI) {
	  quad = string("BmI");
	}
	if (side == PixelEndcapName::pI) {
	  quad = string("BpI");
	}
	
	cout << "FPix_" << quad << "_D" << disk << "_BLD" << blade << "_PNL" << panel << "_MOD" << module
	     << "  " << detId.rawId() 
	     << " with ROCs: ";

	ROClist(detId); 

// 	cout << "=FPix_" << quad << "_D" << disk << "_BLD" << blade << "_PNL" << panel << "_MOD" << module << "  " 
// 	     << detId.rawId() 
// 	     << endl;


      }
    }
  }


}


// ----------------------------------------------------------------------
void DetectorInformation::bpixNames(const DetId &pID, int &DBlayer, int &DBladder, int &DBmodule) {
  PixelBarrelName::Shell DBshell = PixelBarrelName::PixelBarrelName(pID).shell();
  DBlayer  = PixelBarrelName::PixelBarrelName(pID).layerName();
  DBladder = PixelBarrelName::PixelBarrelName(pID).ladderName();
  DBmodule = PixelBarrelName::PixelBarrelName(pID).moduleName();
  
  if (DBshell == PixelBarrelName::mO) {
    DBladder *= -1;
    DBmodule *= -1;
  }
  
  if (DBshell == PixelBarrelName::mI) {
    DBmodule *= -1;
  }
  
  if (DBshell == PixelBarrelName::pO) {
    DBladder *= -1;
  }

}

// ----------------------------------------------------------------------
void DetectorInformation::fpixNames(const DetId &pID, int &DBdisk, int &DBblade, int &DBpanel, int &DBmodule) {

  PixelEndcapName::HalfCylinder DBside = PixelEndcapName::PixelEndcapName(pID).halfCylinder();
  DBdisk   = PixelEndcapName::PixelEndcapName(pID).diskName();
  DBblade  = PixelEndcapName::PixelEndcapName(pID).bladeName();
  DBpanel  = PixelEndcapName::PixelEndcapName(pID).pannelName();
  DBmodule = PixelEndcapName::PixelEndcapName(pID).plaquetteName();
  
  if (DBside == PixelEndcapName::mO) {
    DBdisk   *= -1; 
    DBblade  *= -1;
    DBpanel  *= -1;
    DBmodule *= -1;
  }
  
  if (DBside == PixelEndcapName::mI) {
    DBdisk   *= -1; 
  }
  
  if (DBside == PixelEndcapName::pO) {
    DBblade  *= -1;
    DBpanel  *= -1;
    DBmodule *= -1;
  }

}


// ----------------------------------------------------------------------
// copied from DQM/SiPixelMonitorClient/src/SiPixelInformationExtractor.cc
void DetectorInformation::onlineRocColRow(const DetId &pID, int offlineRow, int offlineCol, int &roc, int &col, int &row) {
  int realfedID = -1;
  uint32_t newDetId = pID;

  //  SiPixelFrameConverter formatter(fCablingMap.product(), realfedID);
  sipixelobjects::ElectronicIndex cabling; 
  sipixelobjects::DetectorIndex detector = {pID, offlineRow, offlineCol};      

  realfedID = fFEDID[newDetId];
  fPFC[realfedID]->toCabling(cabling, detector);
  // cabling should now contain cabling.roc and cabling.dcol  and cabling.pxid
  // however, the coordinates now need to be converted from dcl,pxid to the row,col coordinates used in the calibration info 
  sipixelobjects::LocalPixel::DcolPxid loc;
  loc.dcol = cabling.dcol;
  loc.pxid = cabling.pxid;

  sipixelobjects::LocalPixel locpixel(loc);
  sipixelobjects::CablingPathToDetUnit path = {realfedID, cabling.link, cabling.roc};  
  const sipixelobjects::PixelROC *theRoc = fCablingMap->findItem(path);

  roc = theRoc->idInDetUnit();
  uint32_t detSubId = pID.subdetId();
  if (detSubId == 1) {
    PixelBarrelName nameworker(pID);
    std::string outputname = nameworker.name();
    bool HalfModule = nameworker.isHalfModule();
    if ((outputname.find("mO") != string::npos || outputname.find("mI") != string::npos) && (HalfModule)) {
      roc = theRoc->idInDetUnit() + 8;
    }
  }
  
  col = locpixel.rocCol();
  row = locpixel.rocRow();

}

// define this as a plug-in
DEFINE_FWK_MODULE(DetectorInformation);
