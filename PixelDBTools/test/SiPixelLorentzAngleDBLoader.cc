#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h" // new from 75X
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" // 

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
#include "DataFormats/GeometrySurface/interface/GloballyPositioned.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"


#include "SiPixelLorentzAngleDBLoader.h"

using namespace std;
using namespace edm;

  //Constructor

SiPixelLorentzAngleDBLoader::SiPixelLorentzAngleDBLoader(edm::ParameterSet const& conf) : 
  conf_(conf){
  //magneticField_ = conf_.getParameter<double>("magneticField");
  recordName_ = conf_.getUntrackedParameter<std::string>("record","SiPixelLorentzAngleRcd");
  useFile_ = conf_.getParameter<bool>("useFile");		
  fileName_ = conf_.getParameter<string>("fileName");
  BPixParameters_   = conf_.getUntrackedParameter<Parameters>("BPixParameters");
  FPixParameters_   = conf_.getUntrackedParameter<Parameters>("FPixParameters");
  ModuleParameters_ = conf_.getUntrackedParameter<Parameters>("ModuleParameters");
  bPixLorentzAnglePerTesla_ = (float)conf_.getUntrackedParameter<double>("bPixLorentzAnglePerTesla",
								-9999.);
  fPixLorentzAnglePerTesla_ = (float)conf_.getUntrackedParameter<double>("fPixLorentzAnglePerTesla",
								-9999.);
}

//BeginJob

void SiPixelLorentzAngleDBLoader::beginJob(){
  
}

// Virtual destructor needed.

SiPixelLorentzAngleDBLoader::~SiPixelLorentzAngleDBLoader() {  

}  

// Analyzer: Functions that gets called by framework every event

void SiPixelLorentzAngleDBLoader::analyze(const edm::Event& e, const edm::EventSetup& es) {

	SiPixelLorentzAngle* LorentzAngle = new SiPixelLorentzAngle();

        //Retrieve tracker topology from geometry
	// In 75X
        edm::ESHandle<TrackerTopology> tTopoHandle;
        es.get<TrackerTopologyRcd>().get(tTopoHandle);
        const TrackerTopology* const tTopo = tTopoHandle.product();

	// In 74X
	//edm::ESHandle<TrackerTopology> tTopo;
	//es.get<IdealGeometryRecord>().get(tTopo);
	
        //Retrieve old style tracker geometry from geometry
	edm::ESHandle<TrackerGeometry> pDD;
	es.get<TrackerDigiGeometryRecord>().get( pDD );
	std::cout<<" There are "<<pDD->detUnits().size() <<" detectors (old)"<<std::endl;
	
	for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); 
	    it != pDD->detUnits().end(); it++){
	  
	  if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0){
	    DetId detid=(*it)->geographicalId();
	    const DetId detidc = (*it)->geographicalId();
	    
	    // fill bpix values for LA 
	    if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
	      
	      cout << " pixel barrel:" << "  layer=" << tTopo->pxbLayer(detidc.rawId()) << "  ladder=" << tTopo->pxbLadder(detidc.rawId()) << "  module=" << tTopo->pxbModule(detidc.rawId()) << "  rawId=" << detidc.rawId();

	      // use a commmon value (e.g. for MC)
	      if(bPixLorentzAnglePerTesla_ != -9999.) {  // use common value for all 
		cout<<" LA = "<< bPixLorentzAnglePerTesla_<<endl;
		if(!LorentzAngle->putLorentzAngle(detid.rawId(),bPixLorentzAnglePerTesla_))
		  cout<<"[SiPixelLorentzAngleDB::analyze] detid already exists"<<std::endl;
		
	      } else if(useFile_) {

		  cout<<"method for reading file not implemented yet" << std::endl;

	      } else {
		
		//first individuals are put
		for(Parameters::iterator it = ModuleParameters_.begin(); 
		    it != ModuleParameters_.end(); ++it) {
		  if( it->getParameter<unsigned int>("rawid") == detidc.rawId() ) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		    cout << " LA= " << lorentzangle 
			 << " individual value " << detid.rawId() << endl;
		  }
		}
		
		//modules already put are automatically skipped
		for(Parameters::iterator it = BPixParameters_.begin(); 
		    it != BPixParameters_.end(); ++it) {
		  if( it->getParameter<unsigned int>("module") == tTopo->pxbModule(detidc.rawId()) 
		      && it->getParameter<unsigned int>("layer") == tTopo->pxbLayer(detidc.rawId()) ) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		    cout << " LA= " << lorentzangle << endl;
		  }
		}
		
	      }
	      
	      // fill fpix values for LA 
	    } else if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
	      
	      cout << " pixel endcap:" << "  side=" << tTopo->pxfSide(detidc.rawId()) << "  disk=" << tTopo->pxfDisk(detidc.rawId()) << "  blade=" << tTopo->pxfBlade(detidc.rawId()) << "  panel=" << tTopo->pxfPanel(detidc.rawId()) << "  module=" << tTopo->pxfModule(detidc.rawId()) << "  rawId=" << detidc.rawId();
	      
	      // use a commmon value (e.g. for MC)
	      if(fPixLorentzAnglePerTesla_ != -9999.) {  // use common value for all 
		cout<<" LA = "<< fPixLorentzAnglePerTesla_<<endl;
		if(!LorentzAngle->putLorentzAngle(detid.rawId(),fPixLorentzAnglePerTesla_))
		  cout<<"[SiPixelLorentzAngleDB::analyze] detid already exists"<<std::endl;
		
	      } else if(useFile_) {

		  cout<<"method for reading file not implemented yet" << std::endl;

	      } else {
		
		//first individuals are put
		for(Parameters::iterator it = ModuleParameters_.begin(); 
		    it != ModuleParameters_.end(); ++it) {
		  if( it->getParameter<unsigned int>("rawid") == detidc.rawId() ) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		    cout << " LA= " << lorentzangle << " individual value " << detid.rawId() << endl;
		  } // if
		} // for 
		
		//modules already put are automatically skipped
		for(Parameters::iterator it = FPixParameters_.begin(); 
		    it != FPixParameters_.end(); ++it) {
		  if( it->getParameter<unsigned int>("side") == tTopo->pxfSide(detidc.rawId()) && it->getParameter<unsigned int>("disk") == tTopo->pxfDisk(detidc.rawId()) && it->getParameter<unsigned int>("HVgroup") == HVgroup( tTopo->pxfPanel(detidc.rawId()), tTopo->pxfModule(detidc.rawId()) ) ) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		    cout << " LA= " << lorentzangle << endl;
		  } // endif 
		} // for  
	
	      } // if 	
	    } else { // bpix/fpix 
	      cout<<"detid is Pixel but neither bpix nor fpix"<<std::endl;
	    } // bpix/fpix
	    
	  }
	  
	}      
  	
	
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
	    cout<<"SiPixelLorentzAngleDBLoader"<<er.what()<<std::endl;
	  }catch(const std::exception& er){
	    cout<<"SiPixelLorentzAngleDBLoader"<<"caught std::exception "<<er.what()<<std::endl;
	  }catch(...){
	    cout<<"Funny error"<<std::endl;
	  }
	}else{
	  cout<<"Service is unavailable"<<std::endl;
	}
	
}

unsigned int SiPixelLorentzAngleDBLoader::HVgroup(unsigned int panel, unsigned int module){

   if( 1 == panel && ( 1 == module || 2 == module ))  {
      return 1;
   }
   else if( 1 == panel && ( 3 == module || 4 == module ))  {
      return 2;
   }
   else if( 2 == panel && 1 == module )  {
      return 1;
   }
   else if( 2 == panel && ( 2 == module || 3 == module ))  {
      return 2;
   }
   else {
      cout << " *** error *** in SiPixelLorentzAngleDBLoader::HVgroup(...), panel = " << panel << ", module = " << module << endl;
      return 0;
   }
   
}


void SiPixelLorentzAngleDBLoader::endJob(){

}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelLorentzAngleDBLoader);
