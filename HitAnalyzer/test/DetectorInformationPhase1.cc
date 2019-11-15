// ----------------------------------------------------------------------
// DetectorInformation for Phase1
// -------------------
// Based on the similar code for phase0 written by Urs.
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

#include "DetectorInformationPhase1.h"

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

//#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"

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
DetectorInformationPhase1::DetectorInformationPhase1(const edm::EventSetup& iSetup) {


  fVerbose = 0;
  phase1_ = true;

  FEDMin=1200;
  FEDMax=1338;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- DetectorInformationPhase1 constructor" << endl;
  //cout << "---  version:                         " << rcsid << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  // cout << "---  rootFileName:                    " << fRootFileName << endl;
  cout << "----------------------------------------------------------------------" << endl;

  cout << " Cabling map initilised " << endl;
  fInit=0;
  if (0 == fInit) {
    fInit = 1; 
    // -- Setup cabling map and its map to detIDs
    iSetup.get<SiPixelFedCablingMapRcd>().get(fCablingMap);
    for (int i = FEDMin; i <= FEDMax; ++i) {
      if(fVerbose==1) cout<<" fed : "<<i<<endl;
      fPFC[i-FEDMin] = new SiPixelFrameConverter(fCablingMap.product(),(i));
    }
    
    
    edm::ESHandle<TrackerGeometry> pDD;
    iSetup.get<TrackerDigiGeometryRecord>().get(pDD);

    for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){
      if(dynamic_cast<const PixelGeomDetUnit*>((*it))!=0){
	DetId detId = (*it)->geographicalId();
	uint32_t newDetId = detId;
	
	for (int fedid = FEDMin; fedid <= FEDMax; ++fedid) {
	  if (fPFC[fedid-FEDMin]->hasDetUnit(newDetId)) {
	    fFEDID.insert(make_pair(newDetId, fedid)); 
	    if(fVerbose==1) cout<<" det "<<newDetId <<" "<<fedid<<endl;
	    break;
	  }
	}
      }
    }

  }


}

// ----------------------------------------------------------------------
DetectorInformationPhase1::~DetectorInformationPhase1() { 

  for (int i = FEDMin; i <= FEDMax; ++i) {
    delete fPFC[i-FEDMin]; // rescale to the range 0-140
  }
  

}  

// ----------------------------------------------------------------------
// might not work for fpix
void DetectorInformationPhase1::ROClist(const DetId &pID) {

  DetId detId = pID;
  int realfedID = -1;
  uint32_t newDetId = detId;

  set<int> roclist;

  //cout<<" do the roc list "<<endl;;

  for (int irow = 0; irow < 600; irow += 50) {
    for (int icol = 0; icol < 600; icol += 50) {
  
      sipixelobjects::ElectronicIndex cabling; 
      sipixelobjects::DetectorIndex detector = {detId, irow, icol};      
      
      realfedID = fFEDID[newDetId];
      int OK = fPFC[realfedID-FEDMin]->toCabling(cabling, detector);

      //cout<<newDetId<<" "<<realfedID<<" "<<OK<<" "<<irow<<" "<<icol<<" "<<endl;

      if (1 == OK) continue;
      
      sipixelobjects::CablingPathToDetUnit path = {(unsigned int) realfedID, (unsigned int)cabling.link, (unsigned int)cabling.roc};  
      const sipixelobjects::PixelROC *theRoc = fCablingMap->findItem(path);
      
      int roc = theRoc->idInDetUnit();

      // uint32_t detSubId = detId.subdetId();
      // if (detSubId == 1) {
      // 	PixelBarrelName nameworker(detId);
      // 	std::string outputname = nameworker.name();
      // 	bool HalfModule = nameworker.isHalfModule();
      // 	if ((outputname.find("mO") != string::npos || outputname.find("mI") != string::npos) && (HalfModule)) {
      // 	  roc = theRoc->idInDetUnit() + 8;
      // 	}
      // }
      
      roclist.insert(roc);
    }
  }

  for (unsigned int i= 0; i < 16; ++i) {
    if (roclist.count(i) > 0) cout << i << " " ; 
  }
  cout << endl; 
}


// ----------------------------------------------------------------------
// might not work for fpix
void DetectorInformationPhase1::dumpDetIds(const edm::EventSetup& iSetup, 
					   const TrackerTopology* tt) {

  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );

  cout << "**********************************************************************" << endl;
  cout << " *** Geometry node for TrackerGeom is  "<<&(*pDD)<<std::endl;
  cout << " *** I have " << pDD->dets().size() <<" detectors"<<std::endl;
  cout << " *** I have " << pDD->detTypes().size() <<" types"<<std::endl;
  
  for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){
    
    if(dynamic_cast<const PixelGeomDetUnit*>((*it))!=0){
      DetId detId = (*it)->geographicalId();

      if (detId.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {

	//uint32_t id = detId();       
	//cout<<" bpix "<<detId.rawId()<<" "<<id<<endl;
	// NEEDS TOPOLOGY
	PixelBarrelName::Shell DBshell = PixelBarrelName(detId,tt,phase1_).shell();
	//cout<<detId.rawId()<<" "<<DBshell<<endl;
	int DBlayer  = PixelBarrelName(detId,tt,phase1_).layerName();
	//cout<<detId.rawId()<<" "<<DBlayer<<endl;
	int DBladder = PixelBarrelName(detId,tt,phase1_).ladderName();
	int DBmodule = PixelBarrelName(detId,tt,phase1_).moduleName();
	int DBsector = PixelBarrelName(detId,tt,phase1_).sectorName();

	//cout<<detId.rawId()<<" "<<DBlayer<<" "<<DBladder<<" "<<DBmodule<<endl;


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

	//cout<<detId.rawId()<<" "<<DBshell<<" "<<quad<<endl;
	
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
	cout<<" fpix "<<DetId(id).rawId()<<" "<<id<<" "<<detId.rawId()<<endl;

        PixelEndcapName::HalfCylinder side = PixelEndcapName(detId,tt,phase1_).halfCylinder();
        int disk   = PixelEndcapName(detId,tt,phase1_).diskName();
        int blade  = PixelEndcapName(detId,tt,phase1_).bladeName();
        int panel  = PixelEndcapName(detId,tt,phase1_).pannelName();
        int module = PixelEndcapName(detId,tt,phase1_).plaquetteName();

	cout<<" fpix "<<disk<<endl;

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

	cout<<endl;
	//ROClist(detId);   // does not work for some fpix 

// 	cout << "=FPix_" << quad << "_D" << disk << "_BLD" << blade << "_PNL" << panel << "_MOD" << module << "  " 
// 	     << detId.rawId() 
// 	     << endl;


      }
    }
  }

  cout<<" end "<<endl;

}


// ----------------------------------------------------------------------
// not tested 
void DetectorInformationPhase1::bpixNames(const DetId &pID, int &DBlayer, 
		int &DBladder, int &DBmodule,  const TrackerTopology* tt) {
  PixelBarrelName::Shell DBshell = PixelBarrelName(pID).shell();
  DBlayer  = PixelBarrelName(pID,tt,phase1_).layerName();
  DBladder = PixelBarrelName(pID,tt,phase1_).ladderName();
  DBmodule = PixelBarrelName(pID,tt,phase1_).moduleName();
  
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
// not tested 
void DetectorInformationPhase1::fpixNames(const DetId &pID, int &DBdisk, int &DBblade, int &DBpanel, int &DBmodule,  const TrackerTopology* tt) {

  PixelEndcapName::HalfCylinder DBside = PixelEndcapName(pID).halfCylinder();
  DBdisk   = PixelEndcapName(pID,tt,phase1_).diskName();
  DBblade  = PixelEndcapName(pID,tt,phase1_).bladeName();
  DBpanel  = PixelEndcapName(pID,tt,phase1_).pannelName();
  DBmodule = PixelEndcapName(pID,tt,phase1_).plaquetteName();
  
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
// does not work for fpix yet
void DetectorInformationPhase1::onlineRocColRow(const DetId &pID, 
     int offlineRow, int offlineCol, int &roc, int &col, int &row) {
  int realfedID = -1;
  uint32_t newDetId = pID;

  //  SiPixelFrameConverter formatter(fCablingMap.product(), realfedID);
  sipixelobjects::ElectronicIndex cabling; 
  sipixelobjects::DetectorIndex detector = {pID, offlineRow, offlineCol};      

  realfedID = fFEDID[newDetId];

  cout<<" fed "<<realfedID<<endl;

  fPFC[realfedID-FEDMin]->toCabling(cabling, detector);
  // cabling should now contain cabling.roc and cabling.dcol  and cabling.pxid
  // however, the coordinates now need to be converted from dcl,pxid to the row,col coordinates used in the calibration info 
  sipixelobjects::LocalPixel::DcolPxid loc;
  loc.dcol = cabling.dcol;
  loc.pxid = cabling.pxid;

  cout<<" dol/pxid "<<loc.dcol<<" "<<loc.pxid<<endl;

  sipixelobjects::LocalPixel locpixel(loc);
  sipixelobjects::CablingPathToDetUnit path = {(unsigned int) realfedID, (unsigned int)cabling.link, (unsigned int)cabling.roc};  
  const sipixelobjects::PixelROC *theRoc = fCablingMap->findItem(path);

  roc = theRoc->idInDetUnit();
  cout<<" roc "<<roc<<endl;

  //uint32_t detSubId = pID.subdetId();  / not needed for phase1
  // if (detSubId == 1) { // bpix 
  //   PixelBarrelName nameworker(pID,tt,phase1_);
  //   std::string outputname = nameworker.name();
  //   cout<<outputname<<endl;
  //   bool HalfModule = nameworker.isHalfModule();
  //   if ((outputname.find("mO") != string::npos || outputname.find("mI") != string::npos) && (HalfModule)) {
  //     cout<<" half module?? "<<endl;
  //     roc = theRoc->idInDetUnit() + 8;
  //   }
  // } else { // fpix 
  //   cout<<" fpix not yet implemented "<<endl;
  // }

  col = locpixel.rocCol();
  row = locpixel.rocRow();
  
}

// define this as a plug-in
//DEFINE_FWK_MODULE(DetectorInformationPhase1);
