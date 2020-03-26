#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Geometry/CommonTopologies/interface/PixelGeomDetUnit.h"
//#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" // 
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
//#include "Geometry/CommonTopologies/interface/PixelTopology.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"

//#include "DQM/SiPixelPhase1Common/interface/SiPixelCoordinates.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
#include "DataFormats/GeometrySurface/interface/GloballyPositioned.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

#include "SiPixelLorentzAngleDBLoader.h"

using namespace std;
using namespace edm;

  //Constructor

SiPixelLorentzAngleDBLoader::SiPixelLorentzAngleDBLoader(edm::ParameterSet const& conf) : 
  conf_(conf) {
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

        // Initialize SiPixelCoordinates, used for the individual LA selection and printout
	//SiPixelCoordinates coord;
        //coord.init(es);
	
        //Retrieve old style tracker geometry from geometry
	edm::ESHandle<TrackerGeometry> pDD;
	es.get<TrackerDigiGeometryRecord>().get( pDD );
	std::cout<<" There are "<<pDD->detUnits().size() <<" detectors (old)"<<std::endl;

        //Retrieve tracker topology from geometry
        edm::ESHandle<TrackerTopology> tTopoH;
        es.get<TrackerTopologyRcd>().get(tTopoH);
        const TrackerTopology *tTopo=tTopoH.product();

	//for(TrackerGeometry::DetContainer::const_iterator it = pDD->detUnits().begin(); 
	for(auto it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
	  
	  // for phase2 this does not select the inner tracker module but all modules 
	  if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0) {
	    const DetId detid = (*it)->geographicalId();
	    //auto detType= detid.det(); // det type, tracker=1
	    auto  rawId = detid.rawId();
	    int found = 0;
	    int side = tTopo->side(detid);  // 1:-z 2:+z for fpix, for bpix gives 0
	    
	    // fill bpix values for LA 
	    if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
	
              int layer = tTopo->pxbLayer(detid);
              // Barrel ladder id 1-20,32,44.
              int ladder = tTopo->pxbLadder(detid);
              // Barrel Z-index=1,8
              int module = tTopo->pxbModule(detid);
	      if(module<5)side=1;
	      else        side=2;

	      cout <<" pixel barrel:" 
		   <<" layer="<<layer<<" ladder="<<ladder<<" module="<<module 
		   <<"  rawId=" << rawId<<" side "<<side;

	      // use a commmon value (e.g. for MC)
	      if(bPixLorentzAnglePerTesla_ != -9999.) {  // use common value for all 
		cout<<" LA = "<< bPixLorentzAnglePerTesla_<<" common for all bpix"<<endl;
		if(!LorentzAngle->putLorentzAngle(detid.rawId(),bPixLorentzAnglePerTesla_))
		  cout<<"[SiPixelLorentzAngleDB::analyze] ERROR!: detid already exists"<<std::endl;
		
	      } else if(useFile_) {

		  cout<<"method for reading file not implemented yet" << std::endl;

	      } else {
		
		//first individuals are put
		for(Parameters::iterator it = ModuleParameters_.begin(); 
		    it != ModuleParameters_.end(); ++it) {
		  if( it->getParameter<unsigned int>("rawid") == detid.rawId() ) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    if (!found) {
		      LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		      cout << " LA= " << lorentzangle 
			   << " individual value " << detid.rawId() << endl;
		      found = 1;
		    } else cout<<"[SiPixelLorentzAngleDB::analyze] ERROR!: detid already exists"<<std::endl;
		  }
		}
		
		//modules already put are automatically skipped
		for(Parameters::iterator it = BPixParameters_.begin(); 
		    it != BPixParameters_.end(); ++it) {
		  if (it->exists("layer"))  if (it->getParameter<int>("layer")  != layer)  continue;
		  if (it->exists("ladder")) if (it->getParameter<int>("ladder") != ladder) continue;
		  if (it->exists("module")) if (it->getParameter<int>("module") != module) continue;
		  if (it->exists("side"))   if (it->getParameter<int>("side")   != side)   continue;
		  if (!found) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		    cout << " LA= " << lorentzangle << endl;
		    found = 2;
		  } else if (found==1) {
		    cout<<"[SiPixelLorentzAngleDB::analyze] detid already given in ModuleParameters, skipping ..."<<std::endl;
		  } else cout<<"[SiPixelLorentzAngleDB::analyze] ERROR!: detid already exists"<<std::endl;
		}
		
	      }
	      
	      // fill fpix values for LA (for phase2 fpix & epix)
	    } else if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
	      
	      //int disk=tTopo->pxfDisk(detid); //1,2,3
	      //int blade=tTopo->pxfBlade(detid); //1-56
	       //int module=tTopo->pxfModule(detid); //
	       //int sidef=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
	       //int panel=tTopo->pxfPanel(detid); //panel=1

	       // For fpix we also need to find the ring number which is not available from topology

	       // Convert to online 
	       PixelEndcapName pen(detid,tTopo,true); // use det-id phaseq
	       //PixelEndcapName::HalfCylinder sh = pen.halfCylinder(); //enum
	       //string nameF = pen.name();
	       int disk = pen.diskName();
	       int blade = pen.bladeName();
	       int panel = pen.pannelName();
	       //int plaquetteName = pen.plaquetteName();
	       int ring = pen.ringName();

	      cout << " pixel endcap:" 
		   <<" side="<<side<<" disk=" <<disk<<" blade ="<<blade
		   <<" pannel="<< panel<< " ring=" << ring << "  rawId=" << rawId;
	      
	      // use a commmon value (e.g. for MC)
	      if(fPixLorentzAnglePerTesla_ != -9999.) {  // use common value for all 
		cout<<" LA = "<< fPixLorentzAnglePerTesla_<<" common for all fpix"<<endl;
		if(!LorentzAngle->putLorentzAngle(detid.rawId(),fPixLorentzAnglePerTesla_))
		  cout<<"[SiPixelLorentzAngleDB::analyze] detid already exists"<<std::endl;
		
	      } else if(useFile_) {

		  cout<<"method for reading file not implemented yet" << std::endl;

	      } else {
		
		//first individuals are put
		for(Parameters::iterator it = ModuleParameters_.begin(); 
		    it != ModuleParameters_.end(); ++it) {
		  if( it->getParameter<unsigned int>("rawid") == detid.rawId() ) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    if (!found) {
		      LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		      cout << " LA= " << lorentzangle 
			   << " individual value " << detid.rawId() << endl;
		      found = 1;
		    } else cout<<"[SiPixelLorentzAngleDB::analyze] ERROR!: detid already exists"<<std::endl;
		  } // if
		} // for 
		
		//modules already put are automatically skipped
		for(Parameters::iterator it = FPixParameters_.begin(); 
		    it != FPixParameters_.end(); ++it) {
		  if (it->exists("side"))    if (it->getParameter<int>("side")    != side)   continue;
		  if (it->exists("disk") )   if (it->getParameter<int>("disk")    != disk)   continue;
		  if (it->exists("ring") )   if (it->getParameter<int>("ring")    != ring)   continue;
		  if (it->exists("blade"))   if (it->getParameter<int>("blade")   != blade)  continue;
		  if (it->exists("panel"))   if (it->getParameter<int>("panel")   != panel)  continue;
		  //if (it->exists("plq"))     if (it->getParameter<int>("plq")     != module) continue;
		  if (it->exists("HVgroup")) if (it->getParameter<int>("HVgroup") != 
						 HVgroup(panel,ring))    continue;
		  if (!found) {
		    float lorentzangle = (float)it->getParameter<double>("angle");
		    LorentzAngle->putLorentzAngle(detid.rawId(),lorentzangle);
		    cout << " LA= " << lorentzangle << endl;
		    found = 2;
		  } else if (found==1) {
		    cout<<"[SiPixelLorentzAngleDB::analyze] detid already given in ModuleParameters, skipping ..."<<std::endl;
		  } else cout<<"[SiPixelLorentzAngleDB::analyze] ERROR!: detid already exists"<<std::endl;
		} // for  
	
	      } // if 	

	    } else { // bpix/fpix 
	      // other tracker modules 
	      //cout<<"detid is Pixel but neither bpix nor fpix, det type  "<<detType<<" subdet "<<detid.subdetId()<<std::endl;
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

int SiPixelLorentzAngleDBLoader::HVgroup(int panel, int module){

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
