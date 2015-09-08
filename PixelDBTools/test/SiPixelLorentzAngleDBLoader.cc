#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
//#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
//#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
//#include "Geometry/Records/interface/TrackerTopologyRcd.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
#include "DataFormats/GeometrySurface/interface/GloballyPositioned.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

#include "SiPixelLorentzAngleDBLoader.h"

using namespace std;
using namespace edm;

  //Constructor

SiPixelLorentzAngleDBLoader::SiPixelLorentzAngleDBLoader(edm::ParameterSet const& conf) : 
  conf_(conf) {
  recordName_ = conf_.getUntrackedParameter<std::string>("record","SiPixelLorentzAngleRcd");
  bPixLorentzAnglePerTesla_ = (float)conf_.getParameter<double>("bPixLorentzAnglePerTesla");
  fPixLorentzAnglePerTesla_ = (float)conf_.getParameter<double>("fPixLorentzAnglePerTesla");
  useFile_ = conf_.getParameter<bool>("useFile");		
  fileName_ = conf_.getParameter<string>("fileName");
  //phase1_ = conf_.getUntractedParameter<bool>("phase","false");		
  
  BPixParameters_ = conf_.getUntrackedParameter<Parameters>("BPixParameters");
  FPixParameters_ = conf_.getUntrackedParameter<Parameters>("FPixParameters");
}

  //BeginJob

void SiPixelLorentzAngleDBLoader::beginJob(){
  
}
// Virtual destructor needed.

SiPixelLorentzAngleDBLoader::~SiPixelLorentzAngleDBLoader() {  

}  

// Analyzer: Functions that gets called by framework every event

void SiPixelLorentzAngleDBLoader::analyze(const edm::Event& e, const edm::EventSetup& es) {
  cout<<" Load LA "<<endl;

  SiPixelLorentzAngle* LorentzAngle = new SiPixelLorentzAngle();
	   
  edm::ESHandle<TrackerGeometry> pDD;
  es.get<TrackerDigiGeometryRecord>().get( pDD );


  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  es.get<IdealGeometryRecord>().get(tTopo);
  //es.get<TrackerTopologyRcd>().get(tTopo);
  //const TrackerTopology* const tTopo = tTopoHandle.product();

  std::cout<<"SiPixelLorentzAngle"<<" There are "<<pDD->detUnits().size() <<" detectors"<<std::endl;
	

  for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); 
      it != pDD->detUnits().end(); it++) {
    
    if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0) {
      DetId detid=(*it)->geographicalId();
      
      // fill bpix values for LA 
      if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
	
	//PXBDetId pxdetid = PXBDetId(detid);
	unsigned int layerC = tTopo->pxbLayer(detid);
	// Barrel ladder id 1-20,32,44.
	unsigned int ladderC = tTopo->pxbLadder(detid);
	// Barrel Z-index=1,8
	unsigned int moduleC = tTopo->pxbModule(detid);

	//cout << "barrel:" << "  layer=" << pxdetid.layer() << "  ladder=" << pxdetid.ladder() << "  module=" << pxdetid.module() << endl;
	
	if(!useFile_) { // do not read from file 

	  if(bPixLorentzAnglePerTesla_ != -9999.) {  // use common value for all 

	    if ( ! LorentzAngle->putLorentzAngle(detid.rawId(),bPixLorentzAnglePerTesla_) )
	      cout<<"[SiPixelLorentzAngleDB::analyze] detid already exists"<<std::endl;

	    cout << "barrel:" << "  layer=" << layerC << "  ladder=" << ladderC 
		 << "  module=" << moduleC << " value "<<  bPixLorentzAnglePerTesla_ <<endl;
	
	  } else {  // use module specific parameters

	    for(Parameters::iterator it = BPixParameters_.begin(); it != BPixParameters_.end(); ++it) {
	      if( it->getParameter<unsigned int>("module") == moduleC && 
		  it->getParameter<unsigned int>("layer") == layerC ) { // select the correct la
		float lorentzangle = (float)it->getParameter<double>("angle"); // load
		LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		cout <<"Set: layer = " << layerC << " module "<<moduleC
		     <<" la "<<lorentzangle<<endl;
	      } // end if 
	    } // for det loop
	  } // single or individual values 

	} else { //  read from file 
	  cout << "method for reading file not implemented yet" << endl;
	  //edm::LogError("SiPixelLorentzAngleDBLoader")<<"[SiPixelLorentzAngleDBLoader::analyze] method for reading file not implemented yet" << std::endl;
	}
	
      // fill fpix values for LA 
      } else if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
	
	//hp
	//PXFDetId pxdetid = PXFDetId(detid);
	unsigned int diskC=tTopo->pxfDisk(detid);   //1,2,3
	unsigned int bladeC=tTopo->pxfBlade(detid); //1-24
	unsigned int sideC=tTopo->pxfSide(detid);   //sizd=1 for -z, 2 for +z
	unsigned int panelC=tTopo->pxfPanel(detid);   //sizd=1 for -z, 2 for +z
	unsigned int plaqC=tTopo->pxfModule(detid); //
	//cout << "endcap:" << "  side=" << pxdetid.side() << "  disk=" << pxdetid.disk() << "  blade=" << pxdetid.blade() << "  panel=" << pxdetid.panel() << "  module=" << pxdetid.module() << endl;
	/*hp
	  if ( ! LorentzAngle->putLorentzAngle(detid.rawId(),fPixLorentzAnglePerTesla_) )  edm::LogError("SiPixelLorentzAngleDBLoader")<<"[SiPixelLorentzAngleDBLoader::analyze] detid already exists"<<std::endl;
	*/
	
	if(fPixLorentzAnglePerTesla_ != -9999.) {  // use common value for all 
	  
	  if ( ! LorentzAngle->putLorentzAngle(detid.rawId(),fPixLorentzAnglePerTesla_) )  
	    edm::LogError("SiPixelLorentzAngleDB")<<"[SiPixelLorentzAngleDB::analyze] detid already exists"<<std::endl;
	  cout << " endcap:" << "  side=" << sideC << "  disk=" << diskC 
	       << "  blade=" << bladeC 
	    //<< "  panel=" << pxdetid.panel() 
	    // << "  module=" << pxdetid.module() 
	       << " value = "<< fPixLorentzAnglePerTesla_ << endl;
	  
	} else { // individual values 

	  for(Parameters::iterator it = FPixParameters_.begin(); it != FPixParameters_.end(); ++it) {
	    //cout << " PSet: " << *it << ", module = " << it->getParameter<unsigned int>("module") << endl;
	    if( it->getParameter<unsigned int>("side") == sideC && 
		it->getParameter<unsigned int>("disk") == diskC && 
		it->getParameter<unsigned int>("HVgroup") == HVgroup(panelC,plaqC) ) {
	      float lorentzangle = (float)it->getParameter<double>("angle");
	      LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
	      cout << "endcap:" << "  side=" << sideC << "  disk=" << diskC 
		   << "  blade=" << bladeC 
		//<< "  panel=" << pxdetid.panel() << "  module=" 
		// << pxdetid.module() 
		   << " la "<<lorentzangle<<endl;

	    } // end if
	  } // end for det loop 
	} // if single or individual

      } else { // b/fpix
	std::cout<<"[SiPixelLorentzAngleDBLoader::analyze] detid is Pixel but neither bpix nor fpix"<<std::endl;
      } // b/fpix
      
    } // pixel det
    
  } // end for det loop     
  
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if( mydbservice.isAvailable() ){
    try{
      if( mydbservice->isNewTagRequest(recordName_) ){
	mydbservice->createNewIOV<SiPixelLorentzAngle>(LorentzAngle,
						       mydbservice->beginOfTime(),
						       mydbservice->endOfTime(),
						       recordName_);
      } else {
	mydbservice->appendSinceTime<SiPixelLorentzAngle>(LorentzAngle,
							  mydbservice->currentTime(),
							  recordName_);
      }
    }catch(const cond::Exception& er){
      edm::LogError("SiPixelLorentzAngleDBLoader")<<er.what()<<std::endl;
      std::cout<<"SiPixelLorentzAngleDBLoader"<<er.what()<<std::endl;
    }catch(const std::exception& er){
      edm::LogError("SiPixelLorentzAngleDBLoader")<<"caught std::exception "<<er.what()<<std::endl;     
      std::cout<<"SiPixelLorentzAngleDBLoader"<<"caught std::exception "<<er.what()<<std::endl;
    } catch(...) {
      edm::LogError("SiPixelLorentzAngleDBLoader")<<"Funny error"<<std::endl;
      std::cout<<"SiPixelLorentzAngleDBLoader"<<"Funny error"<<std::endl;
    }
  }else{
    edm::LogError("SiPixelLorentzAngleDBLoader")<<"Service is unavailable"<<std::endl;
    std::cout<<"SiPixelLorentzAngleDBLoader"<<"Service is unavailable"<<std::endl;
  }
  
  
}


unsigned int SiPixelLorentzAngleDBLoader::HVgroup(unsigned int panel, unsigned int module){
  
  if( 1 == panel && ( 1 == module || 2 == module ))  {
      return 1;
  } else if( 1 == panel && ( 3 == module || 4 == module ))  {
    return 2;
  } else if( 2 == panel && 1 == module )  {
    return 1;
  } else if( 2 == panel && ( 2 == module || 3 == module ))  {
    return 2;
  } else {
    cout << " *** error *** in SiPixelLorentzAngleDBLoader::HVgroup(...), panel = " << panel << ", module = " << module << endl;
    return 0;
  }
   
}

void SiPixelLorentzAngleDBLoader::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelLorentzAngleDBLoader);
