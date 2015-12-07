#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Common/interface/Handle.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include "DataFormats/DetId/interface/DetId.h"

// for old pixel ids
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include "SiPixelDets.h"

#define NEW_NAMES
//#define OLD_NAMES

using namespace std;
using namespace edm;

//Constructor

SiPixelDets::SiPixelDets(edm::ParameterSet const& conf) : 
  conf_(conf), phase1_(false) {

  phase1_ = conf_.getUntrackedParameter<bool>("phase1",false);		
  //BPixParameters_ = conf_.getUntrackedParameter<Parameters>("BPixParameters");
  //FPixParameters_ = conf_.getUntrackedParameter<Parameters>("FPixParameters");
}

//BeginJob

void SiPixelDets::beginJob(){
  
}

// Virtual destructor needed.

SiPixelDets::~SiPixelDets() {  

}  

// Analyzer: Functions that gets called by framework every event

void SiPixelDets::analyze(const edm::Event& e, const edm::EventSetup& es) {
  const bool PRINT = false;
  const bool PRINT_TABLE = true;
  const bool doReversedTest = true;

  if(phase1_) cout<<"This is for phase1 geometry "<<endl;
 
  edm::ESHandle<TrackerGeometry> tkgeom;
  es.get<TrackerDigiGeometryRecord>().get( tkgeom );

  if(PRINT) cout<<" There are "<<tkgeom->detUnits().size() <<" detectors"<<std::endl;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  //es.get<IdealGeometryRecord>().get(tTopo);
  es.get<TrackerTopologyRcd>().get(tTopo);
#ifdef NEW_NAMES
  const TrackerTopology* tt = tTopo.product();
#endif

  for(TrackerGeometry::DetUnitContainer::const_iterator it = tkgeom->detUnits().begin(); 
      it != tkgeom->detUnits().end(); it++) {

    // Is it a pixel detetector
    if( dynamic_cast<PixelGeomDetUnit const*>((*it)) == 0) continue; // no, than skip

    DetId detId = (*it)->geographicalId();
    
    const GeomDetUnit      * geoUnit = tkgeom->idToDetUnit( detId );
    const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
    const PixelTopology & topol = pixDet->specificTopology();       

    // Get the module sizes.
    int nrows = topol.nrows();      // rows in x
    int ncols = topol.ncolumns();   // cols in y
    double detThick = pixDet->specificSurface().bounds().thickness();
    double detZ = pixDet->surface().position().z();
    double detR = pixDet->surface().position().perp();
    double detPhi = pixDet->surface().position().phi();
    GlobalVector  normVect = pixDet->surface().normalVector();
    // add the module direction, so we know the E field 

    if(PRINT) cout<<"Position "<<detR<<" "<<detZ<<" "<<detPhi<<" "
		  <<detThick<<" "<<nrows<<" "<<ncols<<endl;

    // bpix  
    if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {	
      //continue;

      unsigned int rawId = detId.rawId();
      if(PRINT) 
	cout<<"Det raw: "<<rawId<<" "<<detId.null()<<" "<<detId.det()<<" "<<detId.subdetId()<<endl;




#ifdef NEW_NAMES 
      //cout<<" print det "<< tTopo->print(detId) <<endl; // does not seem to work in 752

     // Use new indecies 
      // Barell layer = 1,2,3
      unsigned int layerC = tTopo->pxbLayer(detId);
      // Barrel ladder id 1-20,32,44.
      unsigned int ladderC = tTopo->pxbLadder(detId);
      // Barrel Z-index=1,8
      unsigned int zindex = tTopo->pxbModule(detId);

      if(PRINT) cout<<" Barrel layer, ladder, module "
	  <<layerC<<" "<<ladderC<<" "<<zindex<<endl;

      //printDet(detId, tt);

      // Convert to online 
      PixelBarrelName pbn(detId, tt, phase1_); // use det-id
      //PixelBarrelName pbn(detId, tt, false); // use det-id, select phase0
      //PixelBarrelName pbn(detId, tt, true); // use det-id, select phase1
      //PixelBarrelName pbn(pxdetid); // or pixel det id 
      PixelBarrelName::Shell sh = pbn.shell(); //enum
      int sector = pbn.sectorName();
      int ladder = pbn.ladderName();
      int layer  = pbn.layerName();
      int module = pbn.moduleName();
      bool half  = pbn.isHalfModule();
      string name= pbn.name();
      PixelModuleName::ModuleType moduleType = pbn.moduleType();
      DetId det=pbn.getDetId(tt);

      // Sometimes we also use the additional sign convention 
      int ladderSigned=ladder;
      int moduleSigned=module;
      // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
      int shell = int(sh);
      // change the module sign for z<0
      if(shell==1 || shell==2) moduleSigned = -module;
      // change ladeer sign for Outer )x<0)
      if(shell==1 || shell==3) ladderSigned = -ladder;

      if(PRINT) cout<<" shell "<<sh<<"("<<shell<<") "<<sector<<" "<<layer<<" "<<ladderSigned<<" "
		    <<moduleSigned<<" "<<half<<" "<<name<<" "<<moduleType<<" "<<det.rawId()<<" "
		    <<endl;

      if(PRINT_TABLE) 
	cout<<detId.rawId()<<" "<<name<<" r/phi/z = "<<detR<<"/"<<detPhi<<"/"<<detZ
	    <<" cmssw layer/ladder/module "<<layerC<<"/"<<ladderC<<"/"<<zindex
	    <<" E-phi "<<normVect.phi()    // <<" "<<normVect.barePhi() same
	    <<endl;

      // Now check if teh id is the same
      if(rawId != det.rawId() )
	 cout<<" Wrong bpix ID, old = "<<rawId<<" new = "<<det.rawId() <<endl;

      if(!phase1_) {
	PXBDetId pxbdet=pbn.getDetId();
	if(rawId != pxbdet.rawId() ) 
	  cout<<" Wrong bpix (old) ID, old = "<<rawId<<" new = "<<pxbdet.rawId() <<endl;
      }


      // Check the revers transformation (special test)
      if(doReversedTest) {
	PixelBarrelName pbn2(name, phase1_); // use name to get detId
	PixelBarrelName::Shell sh2 = pbn2.shell(); //enum
	int sector2 = pbn2.sectorName();
	int ladder2 = pbn2.ladderName();
	int layer2  = pbn2.layerName();
	int module2 = pbn2.moduleName();
	string name2= pbn2.name();
	PixelModuleName::ModuleType moduleType2 = pbn2.moduleType();
	DetId det2=pbn2.getDetId(tt);
	if(name !=name2)  cout<<" wrong  name "<<endl;
	if(shell !=sh2)  cout<<" wrong shell "<<endl;
	if(sector !=sector2)  cout<<" wrong sector "<<endl;
	if(layer !=layer2)  cout<<" wrong layer "<<endl;
	if(ladder!=ladder2) cout<<" wrong ladder "<<endl;
	if(module!=module2) cout<<" wrong module "<<endl;
	if(moduleType!=moduleType2) cout<<" wrong module type "<<endl;
	if(det2.rawId() != det.rawId()) cout<<" wrong rawid "<<endl;
      }

#endif // NEW_ANMES
	
#ifdef OLD_NAMES
      PXBDetId pxdetid = PXBDetId(detId);
      if(PRINT) cout << "barrel:" << "  layer=" << pxdetid.layer() << "  ladder=" << pxdetid.ladder() 
		     << "  module=" << pxdetid.module() << " "<<rawId<<endl;
      // OLD indices 
      // Barell layer = 1,2,3
      unsigned int layerC1 = pxdetid.layer();
      // Barrel ladder id 1-20,32,44.
      unsigned int ladderC1 = pxdetid.ladder();
      // Barrel Z-index=1,8
      unsigned int zindex1 = pxdetid.module();

      if(PRINT) cout<<" Barrel layer, ladder, module "
		    <<layerC1<<" "<<ladderC1<<" "<<zindex1<<endl;


      // Convert to online 
      PixelBarrelName pbn1(detId); // use det-id
      //PixelBarrelName pbn(pxdetid); // or pixel det id 
      PixelBarrelName::Shell sh1 = pbn1.shell(); //enum
      int sector1 = pbn1.sectorName();
      int ladder1 = pbn1.ladderName();
      int layer1  = pbn1.layerName();
      int module1 = pbn1.moduleName();
      bool half1  = pbn1.isHalfModule();
      string name1= pbn1.name();
      PXBDetId pxbdet1=pbn1.getDetId();

      PixelModuleName::ModuleType moduleType1 = pbn1.moduleType();

      // Sometimes we also use the additional sign convention 
      int ladderSigned1=ladder1;
      int moduleSigned1=module1;
      // Shell { mO = 1, mI = 2 , pO =3 , pI =4 };
      int shell1 = int(sh1);
      // change the module sign for z<0
      if(shell1==1 || shell1==2) moduleSigned1 = -module1;
      // change ladeer sign for Outer )x<0)
      if(shell1==1 || shell1==3) ladderSigned1 = -ladder1;

      if(PRINT) cout<<" shell1 "<<sh1<<"("<<shell1<<") "<<sector1<<" "<<layer1<<" "
		    <<ladderSigned1<<" "<<moduleSigned1
		    <<" "<<half1<<" "<<name1<<" "<<moduleType1<<" "<<pxbdet1.rawId()<<endl;

#endif // OLD_NAMES


      // Compare OLD with NEW if needed 
#if defined(OLD_NAMES) && defined(NEW_NAMES) 
      if(layerC!=layerC1) cout<<" wrong layer "<<endl;
      if(ladderC!=ladderC1) cout<<" wrong ladder "<<endl;
      if(zindex!=zindex1) cout<<" wrong module "<<endl;

      if(name !=name1)  cout<<" wrong  name "<<endl;
      if(shell !=shell1)  cout<<" wrong shell "<<endl;
      if(sector !=sector1)  cout<<" wrong sector "<<endl;
      if(layer !=layer1)  cout<<" wrong layer "<<endl;
      if(ladder!=ladder1) cout<<" wrong ladder "<<endl;
      if(module!=module1) cout<<" wrong module "<<endl;
      if(pxbdet.rawId() != pxbdet1.rawId()) cout<<" wrong rawid "<<endl;
      if(pxbdet.rawId() != det.rawId()) cout<<" wrong rawid "<<endl;
      if(rawId != det.rawId()) cout<<" wrong rawid "<<endl;
#endif //NEW & OLD_NAMES
	
      // FPIX 
    } else if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {

      // Now the fpix values  
      //continue;

      unsigned int rawId = detId.rawId();

      if(PRINT) 
	cout<<"Det: "<<rawId<<" "<<detId.null()<<" "<<detId.det()<<" "<<detId.subdetId()<<endl;


#ifdef NEW_NAMES
      // new ids 
      int disk=tTopo->pxfDisk(detId);   //1,2,3
      int blade=tTopo->pxfBlade(detId); //1-24
      int plaq=tTopo->pxfModule(detId); //
      int side=tTopo->pxfSide(detId);   //sizd=1 for -z, 2 for +z
      int panel=tTopo->pxfPanel(detId); //panel=1

      if(PRINT) cout<<"endcap, side="<<side<<" disk="<<disk<<", blade="<<blade
		    <<", panel="<<panel<<", plaq="<<plaq<<endl;
 

      // Convert to online 
      PixelEndcapName pen(detId,tt,phase1_); // use det-id phase0
      //PixelEndcapName pen(detId,tt,false); // use det-id phase0
      //PixelEndcapName pen(detId,tt,true); // use det-id phase1
      PixelEndcapName::HalfCylinder sh = pen.halfCylinder(); //enum
      string nameF = pen.name();
      int diskName = pen.diskName();
      int bladeName = pen.bladeName();
      int pannelName = pen.pannelName();
      int plaquetteName = pen.plaquetteName();
      DetId det=pen.getDetId(tt);
      //PixelEndcapName::HalfCylinder part = pen.halfCylinder();
      PixelModuleName::ModuleType moduleType = pen.moduleType();
      if(PRINT) cout<<sh<<" "<<nameF<<" "<<diskName<<" "<<bladeName<<" "<<pannelName<<" "
		    <<plaquetteName<<" "<<moduleType<<" "<<det.rawId()
		    <<endl;

      if(PRINT_TABLE) 
	cout<<rawId<<" "<<nameF<<" r/phi/z = "<<detR<<"/"<<detPhi<<"/"<<detZ
	    <<" cmssw side/disk/blade/pannel/plaq="
	    <<side<<"/"<<disk<<"/"<<blade<<"/"<<panel<<"/"<<plaq
	    <<" E-z "<<normVect.z()
	    <<endl;

      // Now check if teh id is the same
      if(rawId != det.rawId() )
	 cout<<" Wrong fpix ID, old = "<<rawId<<" new = "<<det.rawId() <<endl;

      if(!phase1_) {
	PXFDetId pxfdet=pen.getDetId();   
	if(rawId != pxfdet.rawId() ) 
	  cout<<" Wrong fpix (old) ID, old = "<<rawId<<" new = "<<pxfdet.rawId() <<endl;
      }

      // Reverse the transformation to check the name translation 
      if(doReversedTest) { 
	PixelEndcapName pen2(nameF,phase1_); // get id from name 

	PixelEndcapName::HalfCylinder sh2 = pen2.halfCylinder(); //enum
	string nameF2 = pen2.name();
	int diskName2 = pen2.diskName();
	int bladeName2 = pen2.bladeName();
	int pannelName2 = pen2.pannelName();
	int plaquetteName2 = pen2.plaquetteName();
	DetId det2=pen2.getDetId(tt);
	PixelModuleName::ModuleType moduleType2 = pen2.moduleType();
	if(nameF != nameF2)  cout<<" wrong name "<<endl;
	if(sh !=sh2)  cout<<" wrong shell "<<endl;
	if(diskName  != diskName2)  cout<<" wrong disk "<<endl;
	if(bladeName != bladeName2) cout<<" wrong blade "<<endl;
	if(pannelName != pannelName2) cout<<" wrong panel "<<endl;
	if(plaquetteName != plaquetteName2) cout<<" wrong plaquette "<<endl;
	if(moduleType != moduleType2) cout<<" wrong module type "<<endl;
	if(det.rawId() != det2.rawId()) cout<<" wrong rawid "<<endl;
      }

#endif // NEW_NAMES 

#ifdef OLD_NAMES
      // old ids, does not work for phase1
      if(!phase1_) {
	PXFDetId pxdetid = PXFDetId(detId);
	if(PRINT) 
	  cout << "endcap:" << " side=" << pxdetid.side() << "  disk=" << pxdetid.disk() 
	       << "  blade=" << pxdetid.blade() << "  panel=" << pxdetid.panel() 
	       << "  plaq=" << pxdetid.module() << " "<<rawId<<endl;
      }

      // Convert to online 
      PixelEndcapName pen1(detId); // use det-id
      PixelEndcapName::HalfCylinder sh1 = pen1.halfCylinder(); //enum
      string nameF1 = pen1.name();
      int diskName1 = pen1.diskName();
      int bladeName1 = pen1.bladeName();
      int pannelName1 = pen1.pannelName();
      int plaquetteName1 = pen1.plaquetteName();
      PXFDetId pxfdet1=pen1.getDetId();
      //PixelEndcapName::HalfCylinder part1 = pen1.halfCylinder();
      PixelModuleName::ModuleType moduleType1 = pen1.moduleType();
      if(PRINT) 
	cout<<nameF1<<" "<<diskName1<<" "<<bladeName1<<" "<<pannelName1<<" "
	    <<plaquetteName1<<" "<<moduleType1<<" "
	    <<pxfdet1.rawId()<<" "<<sh1<<endl;
#endif // OLD_NAMES

      // Compare OLD with NEW if needed 
#if defined(OLD_NAMES) && defined(NEW_NAMES) 
      if(nameF != nameF1)  cout<<" wrong name "<<endl;
      if(sh !=sh1)  cout<<" wrong shell "<<endl;
      if(diskName  != diskName1)  cout<<" wrong disk "<<endl;
      if(bladeName != bladeName1) cout<<" wrong blade "<<endl;
      if(pannelName != pannelName1) cout<<" wrong panel "<<endl;
      if(pxfdet.rawId() != pxfdet1.rawId()) cout<<" wrong rawid "<<endl;
      if(pxfdet.rawId() != det.rawId()) cout<<" wrong rawid "<<endl;
      if(rawId != det.rawId()) cout<<" wrong rawid "<<endl;
#endif


    } else { // b/fpix

      cout<<"detid is Pixel but neither bpix nor fpix"<<endl;

    } // b/fpix
      
  } // end for det loop     
    
}


void SiPixelDets::endJob() {

}

void SiPixelDets::printDet(DetId detid, const TrackerTopology* tt) {
  // new indecies 
  // Barell layer = 1,2,3
  unsigned int layer = tt->pxbLayer(detid);
  // Barrel ladder id 1-20,32,44.
  unsigned int ladder = tt->pxbLadder(detid);
  // Barrel Z-index=1,8
  unsigned int zindex = tt->pxbModule(detid);

  cout<<" Barrel layer, ladder, module "
      <<layer<<" "<<ladder<<" "<<zindex<<endl;


} 


//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelDets);
