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
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include "DataFormats/DetId/interface/DetId.h"

// for old pixel ids
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include "DetectorInformationPhase1.h"

#include "SiPixelPixels.h"

//#define NEW_NAMES
//#define CHECK_ORIENT

using namespace std;
using namespace edm;

//Constructor

SiPixelPixels::SiPixelPixels(edm::ParameterSet const& conf) : 
  conf_(conf), phase1_(false) {

  //phase1_ = conf_.getUntrackedParameter<bool>("phase1",false);		
  //BPixParameters_ = conf_.getUntrackedParameter<Parameters>("BPixParameters");
  //FPixParameters_ = conf_.getUntrackedParameter<Parameters>("FPixParameters");
}

//BeginJob

void SiPixelPixels::beginJob(){
  
}

// Virtual destructor needed.

SiPixelPixels::~SiPixelPixels() {  

}  

// Analyzer: Functions that gets called by framework every event

void SiPixelPixels::analyze(const edm::Event& e, const edm::EventSetup& es) {
  const bool PRINT =  false;
  //const bool PRINT_TABLE = true;
  //const bool doReversedTest = true;

 
  edm::ESHandle<TrackerGeometry> tkgeom;
  es.get<TrackerDigiGeometryRecord>().get( tkgeom );

  if(PRINT) cout<<" There are "<<tkgeom->detUnits().size() <<" detectors"<<std::endl;

  // Test new TrackerGeometry features
  cout << "Test of TrackerGeometry::isThere";
  cout  << " is there PixelBarrel: " << tkgeom->isThere(GeomDetEnumerators::PixelBarrel);
  cout  << " is there PixelEndcap: " << tkgeom->isThere(GeomDetEnumerators::PixelEndcap);
  cout  << " is there P1PXB: " << tkgeom->isThere(GeomDetEnumerators::P1PXB);
  cout  << " is there P1PXEC: " << tkgeom->isThere(GeomDetEnumerators::P1PXEC);
  cout  << endl;

  // switch on the phase1 
  if( (tkgeom->isThere(GeomDetEnumerators::P1PXB)) && 
      (tkgeom->isThere(GeomDetEnumerators::P1PXEC)) ) phase1_ = true;
  else phase1_ = false;

  if(phase1_) cout<<"This is for phase1 geometry "<<endl;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  //es.get<IdealGeometryRecord>().get(tTopo);
  es.get<TrackerTopologyRcd>().get(tTopo);
  const TrackerTopology* tt = tTopo.product();

  // Mag field (not realy needed)
  //edm::ESHandle<MagneticField> magfield;
  //es.get<IdealMagneticFieldRecord>().get(magfield);


  DetectorInformationPhase1 di(es);
  //di.dumpDetIds(es,tt); // dump Detunits 

  cout<<" loop over modules "<<endl;

  int offlineRow=65; 
  int offlineCol=150;
  int roc=-1, col=-1, row=-1;
  //for(TrackerGeometry::DetUnitContainer::const_iterator it = tkgeom->detUnits().begin(); 
  for(TrackerGeometry::DetContainer::const_iterator it = tkgeom->detUnits().begin(); 
       it != tkgeom->detUnits().end(); it++) {

    // Is it a pixel detetector
    if( dynamic_cast<PixelGeomDetUnit const*>((*it)) == 0) continue; // no, than skip
    
    DetId detId = (*it)->geographicalId();
    int rawid = detId.rawId();
    
    if(PRINT) 
      cout<<"Det raw: "<<detId.rawId()<<" "<<detId.null()<<" "<<detId.det()
	  <<" "<<detId.subdetId()<<endl;
    
    // print roc list per module 
    //cout<<" ROC list "<<endl;
    //di.ROClist(detId);
    //int layer=-1, ladder=-1, module=-1;
    //di.bpixNames(detId,layer,ladder,module,tt);
    //cout<<" layer "<<layer<<" "<<ladder<<" "<<module<<endl;
    
    if(rawid==303054856) {
      
      cout<<" Print det info "<<endl;
      printDet(detId, tt);
 	  
      cout<<" Print pixel conversion  "<<endl;
      offlineRow=49; 
      offlineCol=109;
      roc=-1; col=-1; row=-1;
      di.onlineRocColRow(detId, offlineRow, offlineCol, roc,col,row);
      cout<<" Module "<<rawid<<" for roc "<<roc<<" row/col "
	  <<offlineRow<<"/"<<offlineCol<<" roc row/col "<<row<<"/"<<col<<endl;

    } else if(rawid==303046684) {
      
      cout<<" Print det info "<<endl;
      printDet(detId, tt);
      
      // print roc list per module 
      //cout<<" ROC list "<<endl;
      //di.ROClist(detId);
      //di.bpixNames(detId,layer,ladder,module);
      
      cout<<" Print pixel conversion  "<<endl;
      offlineRow=147; //65; 
      offlineCol=241; // 150;
      roc=-1; col=-1; row=-1;
      di.onlineRocColRow(detId, offlineRow, offlineCol, roc,col,row);
      cout<<" Module "<<rawid<<" for roc "<<roc<<" row/col "
	  <<offlineRow<<"/"<<offlineCol<<" roc row/col "<<row<<"/"<<col<<endl;

    } else if(rawid==303042580) {
      
      cout<<" Print det info "<<endl;
      printDet(detId, tt);
      
      // print roc list per module 
      //cout<<" ROC list "<<endl;
      //di.ROClist(detId);
      //di.bpixNames(detId,layer,ladder,module);
      
      cout<<" Print pixel conversion  "<<endl;
      offlineRow=137; 
      offlineCol=94;
      roc=-1; col=-1; row=-1;
      di.onlineRocColRow(detId, offlineRow, offlineCol, roc,col,row);
      cout<<" Module "<<rawid<<" for roc "<<roc<<" row/col "
	  <<offlineRow<<"/"<<offlineCol<<" roc row/col "<<row<<"/"<<col<<endl;

    } // end if rawid

   }

    
}


void SiPixelPixels::endJob() {

}

void SiPixelPixels::printDet(DetId detId, const TrackerTopology* tt) {
  const bool PRINT = true;


//     const GeomDetUnit      * geoUnit = tkgeom->idToDetUnit( detId );
//     const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
//     const PixelTopology & topol = pixDet->specificTopology();       

//     // Get the module sizes.
//     int nrows = topol.nrows();      // rows in x
//     int ncols = topol.ncolumns();   // cols in y
//     double detThick = pixDet->specificSurface().bounds().thickness();
//     double detZ = pixDet->surface().position().z();
//     double detR = pixDet->surface().position().perp();
//     double detPhi = pixDet->surface().position().phi();
//     GlobalVector  normVect = pixDet->surface().normalVector();
//     // add the module direction, so we know the E field 

//     if(PRINT) cout<<"Position "<<detR<<" "<<detZ<<" "<<detPhi<<" "
// 		  <<detThick<<" "<<nrows<<" "<<ncols<<endl;


  unsigned int rawId = detId.rawId();
  if(PRINT) 
    cout<<"Det raw: "<<rawId<<" "<<detId.null()<<" "<<detId.det()
	<<" "<<detId.subdetId()<<endl;

  // bpix  
  if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {	


    // Barell layer = 1,2,3,4
    unsigned int layerC = tt->pxbLayer(detId);
    // Barrel ladder id 1-20,32,44.
    unsigned int ladderC = tt->pxbLadder(detId);
    // Barrel Z-index=1,8
    unsigned int zindex = tt->pxbModule(detId);

    if(PRINT) cout<<" Barrel layer, ladder, module "
		  <<layerC<<" "<<ladderC<<" "<<zindex<<endl;


    // Convert to online 
    PixelBarrelName pbn(detId, tt, phase1_); // use det-id
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
      // FPIX 
  } else if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {

      // new ids 
      int disk=tt->pxfDisk(detId);   //1,2,3
      int blade=tt->pxfBlade(detId); //1-24
      int plaq=tt->pxfModule(detId); //
      int side=tt->pxfSide(detId);   //sizd=1 for -z, 2 for +z
      int panel=tt->pxfPanel(detId); //panel=1

      if(PRINT) cout<<"endcap, side="<<side<<" disk="<<disk<<", blade="<<blade
		    <<", panel="<<panel<<", plaq="<<plaq<<endl;
 

      // Convert to online 
      PixelEndcapName pen(detId,tt,phase1_); // use det-id phase0
      PixelEndcapName::HalfCylinder sh = pen.halfCylinder(); //enum
      string nameF = pen.name();
      int diskName = pen.diskName();
      int bladeName = pen.bladeName();
      int pannelName = pen.pannelName();
      int plaquetteName = pen.plaquetteName();
      DetId det=pen.getDetId(tt);
      PixelModuleName::ModuleType moduleType = pen.moduleType();
      if(PRINT) cout<<"FPIX "<<sh<<" "<<nameF<<" "<<diskName<<" "<<bladeName<<" "<<pannelName<<" "
		    <<plaquetteName<<" "<<moduleType<<" "<<det.rawId()
		    <<endl;

  } else { // b/fpix

    cout<<"detid is Pixel but neither bpix nor fpix"<<endl;

  } // b/fpix

} 


//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelPixels);
