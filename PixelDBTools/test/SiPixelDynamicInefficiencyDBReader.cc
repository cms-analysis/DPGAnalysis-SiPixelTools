#include "CondFormats/SiPixelObjects/interface/SiPixelDynamicInefficiency.h"
#include "CondFormats/DataRecord/interface/SiPixelDynamicInefficiencyRcd.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDetEnumerators.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include <iostream>
#include <stdio.h>
#include <sys/time.h>

#include "SiPixelDynamicInefficiencyDBReader.h"


using namespace cms;

SiPixelDynamicInefficiencyDBReader::SiPixelDynamicInefficiencyDBReader( const edm::ParameterSet& iConfig ):
  Phase_(iConfig.getParameter<int>("Phase"))
{
  if (Phase_==0) {
    nBarrelLayers_ = 3;
    nForwardDisks_ = 2;
  } else if (Phase_==1) {
    nBarrelLayers_ = 4;
    nForwardDisks_ = 3;
  } else {
    throw cms::Exception("Configuration") << "Only Phase 0 and 1 is implemented now";
  }

  //Load factors from config file, copied from SimTracker/SiPixelDigitizer/plugins/SiPixelDigitizerAlgorithm.cc
  // 2016.03.01 - Added Phase I config parameters similar to Phase 0
  // BPix
  int i=0;
  thePixelColEfficiency[i++] = iConfig.getParameter<double>("thePixelColEfficiency_BPix1");
  thePixelColEfficiency[i++] = iConfig.getParameter<double>("thePixelColEfficiency_BPix2");
  thePixelColEfficiency[i++] = iConfig.getParameter<double>("thePixelColEfficiency_BPix3");
  if (Phase_==1)
    thePixelColEfficiency[i++] = iConfig.getParameter<double>("thePixelColEfficiency_BPix4");
  i=0;
  thePixelEfficiency[i++] = iConfig.getParameter<double>("thePixelEfficiency_BPix1");
  thePixelEfficiency[i++] = iConfig.getParameter<double>("thePixelEfficiency_BPix2");
  thePixelEfficiency[i++] = iConfig.getParameter<double>("thePixelEfficiency_BPix3");
  if (Phase_==1)
    thePixelEfficiency[i++] = iConfig.getParameter<double>("thePixelEfficiency_BPix4");
  i=0;
  thePixelChipEfficiency[i++] = iConfig.getParameter<double>("thePixelChipEfficiency_BPix1");
  thePixelChipEfficiency[i++] = iConfig.getParameter<double>("thePixelChipEfficiency_BPix2");
  thePixelChipEfficiency[i++] = iConfig.getParameter<double>("thePixelChipEfficiency_BPix3");
  if (Phase_==1)
    thePixelChipEfficiency[i++] = iConfig.getParameter<double>("thePixelChipEfficiency_BPix4");
  
  // Old Ladder/Module configuration parameters only valid for Phase 0
  if ( Phase_ == 0 ) {
    i=0;
    theLadderEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theLadderEfficiency_BPix1");
    theLadderEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theLadderEfficiency_BPix2");
    theLadderEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theLadderEfficiency_BPix3");
    if ( (theLadderEfficiency_BPix[0].size()!=20) || (theLadderEfficiency_BPix[1].size()!=32) ||
	 (theLadderEfficiency_BPix[2].size()!=44) )  
      throw cms::Exception("Configuration") << "Wrong ladder number in efficiency config!";
    
    i=0;
    theModuleEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theModuleEfficiency_BPix1");
    theModuleEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theModuleEfficiency_BPix2");
    theModuleEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theModuleEfficiency_BPix3");
    if ((theModuleEfficiency_BPix[0].size()!=4) || (theModuleEfficiency_BPix[1].size()!=4) ||
	(theModuleEfficiency_BPix[2].size()!=4))  
      throw cms::Exception("Configuration") << "Wrong module number in efficiency config!";
  }
  
  i=0;         
  thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_BPix1");
  thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_BPix2");
  thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_BPix3");
  if (Phase_==1)
    thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_BPix4");

  // FPix
  i=nBarrelLayers_;
  thePixelColEfficiency[i++]   = iConfig.getParameter<double>("thePixelColEfficiency_FPix1");
  thePixelColEfficiency[i++]   = iConfig.getParameter<double>("thePixelColEfficiency_FPix2");
  if (Phase_==1)
    thePixelColEfficiency[i++]   = iConfig.getParameter<double>("thePixelColEfficiency_FPix3");
  i=nBarrelLayers_;
  thePixelEfficiency[i++]      = iConfig.getParameter<double>("thePixelEfficiency_FPix1");
  thePixelEfficiency[i++]      = iConfig.getParameter<double>("thePixelEfficiency_FPix2");
  if (Phase_==1)
    thePixelEfficiency[i++]      = iConfig.getParameter<double>("thePixelEfficiency_FPix3");
  i=nBarrelLayers_;
  thePixelChipEfficiency[i++]  = iConfig.getParameter<double>("thePixelChipEfficiency_FPix1");
  thePixelChipEfficiency[i++]  = iConfig.getParameter<double>("thePixelChipEfficiency_FPix2");
  if (Phase_==1)
    thePixelChipEfficiency[i++]  = iConfig.getParameter<double>("thePixelChipEfficiency_FPix3");
  i=nBarrelLayers_;
  theInnerEfficiency_FPix[i++] = iConfig.getParameter<double>("theInnerEfficiency_FPix1");
  theInnerEfficiency_FPix[i++] = iConfig.getParameter<double>("theInnerEfficiency_FPix2");
  if (Phase_==1)
    theInnerEfficiency_FPix[i++] = iConfig.getParameter<double>("theInnerEfficiency_FPix3");
  i=nBarrelLayers_;
  theOuterEfficiency_FPix[i++] = iConfig.getParameter<double>("theOuterEfficiency_FPix1");
  theOuterEfficiency_FPix[i++] = iConfig.getParameter<double>("theOuterEfficiency_FPix2");
  if (Phase_==1)
    theOuterEfficiency_FPix[i++] = iConfig.getParameter<double>("theOuterEfficiency_FPix3");
  i=nBarrelLayers_;
  if (Phase_==0) {
    thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_FPix_Inner");
    thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_FPix_Outer");
  } else if (Phase_==1) {
    thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_FPix1");
    thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_FPix2");
    thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_FPix3");
  }
  theInstLumiScaleFactor = iConfig.getParameter<double>("theInstLumiScaleFactor");
}

SiPixelDynamicInefficiencyDBReader::~SiPixelDynamicInefficiencyDBReader(){}


double 
SiPixelDynamicInefficiencyDBReader::getMatchingGeomFactor_(const DetId& detid,
							const std::map<unsigned int, double>& map_geomfactor,
							const std::vector<uint32_t>& detIdmasks) {
  double geomfactor_db = 1;
  for ( auto map_element : map_geomfactor ) {
    const DetId mapid = DetId(map_element.first);
    if (mapid.subdetId() != detid.subdetId()) continue;
    size_t __i=0;
    for (;__i<detIdmasks.size();__i++){
      DetId maskid = DetId(detIdmasks.at(__i));
      if (maskid.subdetId() != mapid.subdetId()) continue;
      if ((detid.rawId()&maskid.rawId()) != (mapid.rawId()&maskid.rawId()) && 
	  (mapid.rawId()&maskid.rawId()) != DetId(mapid.det(),mapid.subdetId()).rawId()) break;
    }
    if (__i != detIdmasks.size()) continue;
    geomfactor_db *= map_element.second;
  }
  return geomfactor_db;
}

std::vector<double>
SiPixelDynamicInefficiencyDBReader::getMatchingPUFactors_(const DetId& detid,
							 const std::map<unsigned int, std::vector<double> >& map_pufactory,
							 const std::vector<uint32_t>& detIdmasks) {
  std::vector<double> pufactors_db;
  for ( auto map_element : map_pufactory ) {
    const DetId mapid = DetId(map_element.first);
    if (mapid.subdetId() != detid.subdetId()) continue;
    size_t __i=0;
    for (;__i<detIdmasks.size();__i++){
      DetId maskid = DetId(detIdmasks.at(__i));
      if (maskid.subdetId() != mapid.subdetId()) continue;
      if ((detid.rawId()&maskid.rawId()) != (mapid.rawId()&maskid.rawId()) && 
	  (mapid.rawId()&maskid.rawId()) != DetId(mapid.det(),mapid.subdetId()).rawId()) break;
    }
    if (__i != detIdmasks.size()) continue;
    pufactors_db = map_element.second;
  }
  return pufactors_db;
}

void SiPixelDynamicInefficiencyDBReader::analyze( const edm::Event& e, const edm::EventSetup& iSetup){
  // Reading DB content
  edm::ESHandle<SiPixelDynamicInefficiency> SiPixelDynamicInefficiency_; 
  iSetup.get<SiPixelDynamicInefficiencyRcd>().get(SiPixelDynamicInefficiency_);
  edm::LogInfo("SiPixelDynamicInefficiencyDBReader") << "[SiPixelDynamicInefficiencyDBReader::analyze] End Reading SiPixelDynamicInefficiency" << std::endl;

  std::map<unsigned int, double> map_pixelgeomfactor = SiPixelDynamicInefficiency_->getPixelGeomFactors();
  std::map<unsigned int, double> map_colgeomfactor = SiPixelDynamicInefficiency_->getColGeomFactors();
  std::map<unsigned int, double> map_chipgeomfactor = SiPixelDynamicInefficiency_->getChipGeomFactors();
  std::map<unsigned int, std::vector<double> > map_pufactor = SiPixelDynamicInefficiency_->getPUFactors();
  std::vector<uint32_t> detIdmasks_db = SiPixelDynamicInefficiency_->getDetIdmasks();
  double theInstLumiScaleFactor_db = SiPixelDynamicInefficiency_->gettheInstLumiScaleFactor_();
  
  std::cout<<"-------------------------------------------------------"<<std::endl;
  std::cout<<"Printing out DB content:"<<std::endl<<std::endl;

  std::cout<<"  PixelGeomFactors:"<<std::endl;
  for ( auto pixel : map_pixelgeomfactor )
    std::cout<<"    MapID = "<<pixel.first<<"\tFactor = "<<pixel.second<<std::endl;
  std::cout<<std::endl;

  std::cout<<"  ColGeomFactors:"<<std::endl;
  for ( auto col : map_colgeomfactor )
    std::cout<<"    MapID = "<<col.first<<"\tFactor = "<<col.second<<std::endl;
  std::cout<<std::endl;

  std::cout<<"  ChipGeomFactors:"<<std::endl;
  for ( auto chip : map_chipgeomfactor )
    std::cout<<"    MapID = "<<chip.first<<"\tFactor = "<<chip.second<<std::endl;
  std::cout<<std::endl;

  std::cout<<"  PUFactors:"<<std::endl;
  for ( auto pu : map_pufactor ) {
    std::cout<<"    MapID = "<<pu.first<<"\t Factor"<<(pu.second.size()>1 ? "s" : "")<<" = ";
    for (size_t i=0, n= pu.second.size(); i<n; ++i)
      std::cout<<pu.second[i]<<( (i==n-1) ? "\n" : ", ");
  }
  std::cout<<std::endl;

  std::cout<<"  DetIdmasks:"<<std::endl;
  for ( auto mask : detIdmasks_db )
    std::cout<<"    MaskID = "<<mask<<std::endl;
  std::cout<<std::endl;

  std::cout<<"  theInstLumiScaleFactor = "<<theInstLumiScaleFactor_db<<std::endl;
  std::cout<<std::endl;

  std::cout<<"-------------------------------------------------------"<<std::endl;
  std::cout<<"Start matching detIdmasks from GT geometry"<<std::endl<<std::endl;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoHandle;
  iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);
  const TrackerTopology* const tTopo = tTopoHandle.product();

  // Check what masks we would get using the current geometry
  // It has to match what is in the db content!!
  std::vector<uint32_t> detIdmasks_geom;
  uint32_t max = std::numeric_limits<uint32_t>::max();
  detIdmasks_geom.push_back(tTopo->pxbDetId(max,0,0).rawId());
  detIdmasks_geom.push_back(tTopo->pxbDetId(0,max,0).rawId());
  detIdmasks_geom.push_back(tTopo->pxbDetId(0,0,max).rawId());
  detIdmasks_geom.push_back(tTopo->pxfDetId(max,0,0,0,0).rawId());
  detIdmasks_geom.push_back(tTopo->pxfDetId(0,max,0,0,0).rawId());
  detIdmasks_geom.push_back(tTopo->pxfDetId(0,0,max,0,0).rawId());
  detIdmasks_geom.push_back(tTopo->pxfDetId(0,0,0,max,0).rawId());
  detIdmasks_geom.push_back(tTopo->pxfDetId(0,0,0,0,max).rawId());

  bool all_ok = true;
  for (size_t i=0; i<detIdmasks_db.size(); ++i) {
    if (detIdmasks_db[i] != detIdmasks_geom[i]) {
      all_ok = 0;
      std::cout<<"!!  Mismatch !! - detIdmasks["<<i<<"] -  Current geometry: "<<detIdmasks_geom[i]<<", but DB content: "<<detIdmasks_db[i]<<std::endl;
    }
  }
  if (all_ok) std::cout<<"All detId masks matching what is in the DB\n\n";
  else std::cout<<"\nBad Geometry!! Are you using the right Geometry? Phase I db content should never be used for Phase 0 and vice versa!\n\n";

  std::cout<<"-------------------------------------------------------"<<std::endl;
  std::cout<<"Start matching DB content to config parameters"<<std::endl<<std::endl;

  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );
      
  std::vector<int> nmodule(nBarrelLayers_+nForwardDisks_, 0);
  std::vector<int> pixelgeom_match(nBarrelLayers_+nForwardDisks_, 0);
  std::vector<int> colgeom_match(nBarrelLayers_+nForwardDisks_, 0);
  std::vector<int> chipgeom_match(nBarrelLayers_+nForwardDisks_, 0);
  std::vector<int> pu_match(nBarrelLayers_+nForwardDisks_, 0);
  
  // Matching Geometry
  for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++) {
    if( dynamic_cast<PixelGeomDetUnit const*>((*it))==0) continue;

    // Geometry variables
    const DetId detid=(*it)->geographicalId();
    int layer = -9999, ladder = -9999, module = -9999, disk = -9999, panel = -9999;
    bool isBPix = (detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel));
    bool isFPix = (detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap));
    if(isBPix) {
      layer=tTopo->layer(detid);
      ladder=tTopo->pxbLadder(detid.rawId());
      module=tTopo->pxbModule(detid.rawId());
      if (module<=4) module=5-module;
      else module-=4;
    } else if (isFPix) {
      disk=tTopo->layer(detid);
      panel=tTopo->pxfPanel(detid);
      module=tTopo->pxfModule(detid);
    } else {
      throw cms::Exception("") << "Unknown pixle module";
    }

    int layDiskIndex = isBPix ? layer-1 : nBarrelLayers_+disk-1;
    ++nmodule[layDiskIndex];

    // ___________________________________________
    //          Geometry Scale factors

    // From Database
    double geomfactor_pixel_db = getMatchingGeomFactor_(detid, map_pixelgeomfactor, detIdmasks_db);
    double geomfactor_col_db   = getMatchingGeomFactor_(detid, map_colgeomfactor,   detIdmasks_db);
    double geomfactor_chip_db  = getMatchingGeomFactor_(detid, map_chipgeomfactor,  detIdmasks_db);

    // From Config
    double geomfactor_pixel_cfg = thePixelEfficiency[layDiskIndex];
    double geomfactor_col_cfg   = thePixelColEfficiency[layDiskIndex];
    double geomfactor_chip_cfg  = thePixelChipEfficiency[layDiskIndex];
    if (Phase_==0) {
      if(isBPix) {
	geomfactor_col_cfg *= theLadderEfficiency_BPix[layDiskIndex][ladder-1]*theModuleEfficiency_BPix[layDiskIndex][module-1];
      } else if (isFPix) {
	if ((panel==1 && (module==1 || module==2)) || (panel==2 && module==1)) {
	  // Inner 3 module
	  geomfactor_col_cfg *= theInnerEfficiency_FPix[layDiskIndex];
	} else {
	  // Outer 4 module
	  geomfactor_col_cfg *= theOuterEfficiency_FPix[layDiskIndex];
	}
      }
    }

    // Matching
    if (geomfactor_pixel_db == geomfactor_pixel_cfg) ++pixelgeom_match[layDiskIndex];
    if (geomfactor_col_db   == geomfactor_col_cfg)   ++colgeom_match[layDiskIndex];
    if (geomfactor_chip_db  == geomfactor_chip_cfg)  ++chipgeom_match[layDiskIndex];

    // ___________________________________________
    //             PU Scale factors

    std::vector<double> pufactors_db = getMatchingPUFactors_(detid, map_pufactor,  detIdmasks_db);
    std::vector<double> pufactors_cfg;
    if (Phase_==0&&isFPix) {
      if ((panel==1 && (module==1 || module==2)) || (panel==2 && module==1))
	pufactors_cfg = thePUEfficiency[3];
      else
	pufactors_cfg = thePUEfficiency[4];
    } else {
      pufactors_cfg = thePUEfficiency[layDiskIndex];      
    }

    bool match_pu = (pufactors_db.size()==pufactors_cfg.size());
    for (unsigned int i=0; i<pufactors_cfg.size()&&match_pu; i++)
      match_pu &= (pufactors_db[i] == pufactors_cfg[i]);
    if (match_pu) ++pu_match[layDiskIndex];
  }

  std::cout<<"  PixelGeomFactors:"<<std::endl;
  for (int i=0; i<nBarrelLayers_; ++i) {
    if (pixelgeom_match[i]==nmodule[i])
      std::cout<<"    Layer "<<i+1<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Layer "<<i+1<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  for (int i=nBarrelLayers_; i<nBarrelLayers_+nForwardDisks_; ++i) { 
    if (pixelgeom_match[i]==nmodule[i])
      std::cout<<"    Disk  "<<i+1-nBarrelLayers_<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Disk  "<<i+1-nBarrelLayers_<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  std::cout<<std::endl;

  std::cout<<"  ColGeomFactors:"<<std::endl;
  for (int i=0; i<nBarrelLayers_; ++i) { 
    if (colgeom_match[i]==nmodule[i])
      std::cout<<"    Layer "<<i+1<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Layer "<<i+1<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  for (int i=nBarrelLayers_; i<nBarrelLayers_+nForwardDisks_; ++i) { 
    if (colgeom_match[i]==nmodule[i])
      std::cout<<"    Disk  "<<i+1-nBarrelLayers_<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Disk  "<<i+1-nBarrelLayers_<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  std::cout<<std::endl;

  std::cout<<"  ChipGeomFactors:"<<std::endl;
  for (int i=0; i<nBarrelLayers_; ++i) { 
    if (chipgeom_match[i]==nmodule[i])
      std::cout<<"    Layer "<<i+1<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Layer "<<i+1<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  for (int i=nBarrelLayers_; i<nBarrelLayers_+nForwardDisks_; ++i) { 
    if (chipgeom_match[i]==nmodule[i])
      std::cout<<"    Disk  "<<i+1-nBarrelLayers_<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Disk  "<<i+1-nBarrelLayers_<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  std::cout<<std::endl;

  std::cout<<"  PUGeomFactors:"<<std::endl;
  for (int i=0; i<nBarrelLayers_; ++i) { 
    if (pu_match[i]==nmodule[i])
      std::cout<<"    Layer "<<i+1<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Layer "<<i+1<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  for (int i=nBarrelLayers_; i<nBarrelLayers_+nForwardDisks_; ++i) { 
    if (pu_match[i]==nmodule[i])
      std::cout<<"    Disk  "<<i+1-nBarrelLayers_<<" modules ("<<nmodule[i]<<") Match"<<std::endl;
    else {
      std::cout<<"!!  Mismatch on Disk  "<<i+1-nBarrelLayers_<<"  !!"<<std::endl;
      all_ok = false;
    }
  }
  std::cout<<std::endl;

  std::cout<<"  theInstLumiScaleFactor:"<<std::endl;
  if (theInstLumiScaleFactor==theInstLumiScaleFactor_db) {
    std::cout<<"    Match"<<std::endl;
  } else {
    std::cout<<"!!  Mismatch  !!"<<std::endl;
    all_ok = false;
  }
  std::cout<<std::endl;
  std::cout<<"-------------------------------------------------------"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"  RESULT:"<<std::endl;
  if (all_ok) {
    std::cout<<"    All OK! :) The DB content exactly matches the configuration."<<std::endl;
  } else {
    std::cout<<"    Not OK! :( The DB content doesn't exactly match the configuration."<<std::endl;
    std::cout<<"    Please revise them!"<<std::endl;
  }
  std::cout<<std::endl;
  std::cout<<"-------------------------------------------------------"<<std::endl;
}
DEFINE_FWK_MODULE(SiPixelDynamicInefficiencyDBReader);
