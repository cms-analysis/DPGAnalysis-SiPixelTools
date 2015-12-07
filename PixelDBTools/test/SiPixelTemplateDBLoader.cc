#include <fstream>

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "CondFormats/DataRecord/interface/SiPixelTemplateDBObjectRcd.h"

#include "SiPixelTemplateDBLoader.h"
using namespace std;

SiPixelTemplateDBLoader::SiPixelTemplateDBLoader(const edm::ParameterSet& iConfig):
	theTemplateCalibrations( iConfig.getParameter<vstring>("siPixelTemplateCalibrations") ),
	//theTemplateBaseString( iConfig.getParameter<std::string>("theTemplateBaseString") ),
	theVersion( iConfig.getParameter<double>("Version") ), // external version num 
	//theMagField( iConfig.getParameter<double>("MagField") ),
	theDetIds( iConfig.getParameter<std::vector<uint32_t> >("detIds") ), // det groups
	theTemplIds( iConfig.getParameter<std::vector<uint32_t> >("templateIds") ) //templates
{
}


SiPixelTemplateDBLoader::~SiPixelTemplateDBLoader() {
}

void SiPixelTemplateDBLoader::beginJob() {
}

void SiPixelTemplateDBLoader::analyze(const edm::Event& iEvent, const edm::EventSetup& es) {
  // Local variables 
  const char *tempfile;
  const bool PRINT = true;

  //--- Make the POOL-ORA object to store the database object
  SiPixelTemplateDBObject* obj = new SiPixelTemplateDBObject;
  	
  // Set the number of templates to be passed to the dbobject
  obj->setNumOfTempl(theTemplateCalibrations.size());
  
  // Set the version of the template dbobject - this is an external parameter
  obj->setVersion(theVersion);
  
  if(PRINT) cout<<" templates size "<< theTemplateCalibrations.size() 
		<<" version(external)  "<<theVersion<<endl;

  // Open the template file(s) 
  for(int m=0; m< obj->numOfTempl(); ++m){

    edm::FileInPath file( theTemplateCalibrations[m].c_str() );
    tempfile = (file.fullPath()).c_str();
    std::ifstream in_file(tempfile, std::ios::in);
    
    if(in_file.is_open()){
      cout << "Opened Template File: " << file.fullPath().c_str()<<" "<<m<<endl;
      
      // Local variables 
      char title_char[80], c;
      SiPixelTemplateDBObject::char2float temp;
      float tempstore;
      int iter,j;
      
      // Templates contain a header char - we must be clever about storing this
      for (iter = 0; (c=in_file.get()) != '\n'; ++iter) {
	if(iter < 79) {title_char[iter] = c;}
      }
      if(iter > 78) {iter=78;}
      title_char[iter+1] ='\n';

      if(PRINT) cout<<title_char<<endl;
      
      for(j=0; j<80; j+=4) {
	temp.c[0] = title_char[j];
	temp.c[1] = title_char[j+1];
	temp.c[2] = title_char[j+2];
	temp.c[3] = title_char[j+3];
	obj->push_back(temp.f);
	obj->setMaxIndex(obj->maxIndex()+1);
      }
      
      // Fill the dbobject
      in_file >> tempstore;
      while(!in_file.eof()) {
	obj->setMaxIndex(obj->maxIndex()+1);
	obj->push_back(tempstore);
	in_file >> tempstore;
      }
      
      in_file.close();
    } else {
      // If file didn't open, report this
      cout << "Error opening File" << tempfile<<endl;
    }
  }
  
  // Get from geometry all modules 
  edm::ESHandle<TrackerGeometry> pDD;
  es.get<TrackerDigiGeometryRecord>().get( pDD );
  
  if(PRINT) cout<<" There are "<<pDD->detUnits().size()<<" detectors"<<endl;

  for(unsigned int i=0; i<theDetIds.size(); ++i) { // loop over module groups 
    short s_detid = (short) theDetIds[i];
    short templid = (short) theTemplIds[i];

    DetId theDetid(s_detid);
    if(s_detid!=0 && s_detid!=1 && s_detid!=2) {  // check it is a valid pixel selection  
      if ( ! (*obj).putTemplateID( theDetid.rawId(),templid ) ) {
	cout << " Could not fill specified det unit: " << theDetid<<endl;
      }
    }

    if(PRINT) cout<<" load template "<<templid<<" for modules "<<s_detid<<endl;

    // Now loop over all modules 
    for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); 
	it != pDD->detUnits().end(); it++){
      
      //cout<< "Detid  "<<((*it)->geographicalId()).rawId()<<" "
      //  <<((*it)->geographicalId()).subdetId()<<endl;

      // select on pixel modules 
      if( dynamic_cast<PixelGeomDetUnit const*>((*it))!=0) { // skip non-pixels
	DetId detid=(*it)->geographicalId();
	
	if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel) &&
	   (detid.subdetId() == s_detid || s_detid == 0) ) {
 	  if(PRINT) cout<<" bpix module "<<detid.rawId()<<endl;
	  if ( ! (*obj).putTemplateID( detid.rawId(),templid ) )
	    cout<< " Could not fill barrel det unit"<<detid.rawId()<<endl;
	}

	if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap) &&
	   (detid.subdetId() == s_detid || s_detid == 0) ) {
 	  if(PRINT) cout<<" fpix module "<<" "<<detid.rawId()<<endl;
	  if ( ! (*obj).putTemplateID( detid.rawId(),templid ) )
	    cout << " Could not fill endcap det unit"<<detid.rawId()<<endl;
	} // if endcap
      } // if pixel 
    } // det loop 
  } // det template map loop 
  
  // Uncomment to output the contents of the db object at the end of the job
  //	std::cout << *obj << std::endl;
  //std::map<unsigned int,short> templMap=(*obj).getTemplateIDs();
  //for(std::map<unsigned int,short>::const_iterator it=templMap.begin(); it!=templMap.end();++it)
  //std::cout<< "Map:\n"<< "DetId: "<< it->first << " TemplateID: "<< it->second <<"\n";
  
  //--- Create a new IOV
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  if( !poolDbService.isAvailable() ) { // Die if not available
    throw cms::Exception("NotAvailable") << "PoolDBOutputService not available";
  }

  if(poolDbService->isNewTagRequest("SiPixelTemplateDBObjectRcd")) {
    cout<<" New tag request "<<endl;
    poolDbService->writeOne(obj, poolDbService->beginOfTime(),"SiPixelTemplateDBObjectRcd");
  } else {
    cout<<" Old tag request "<<endl;
    poolDbService->writeOne(obj, poolDbService->currentTime(),"SiPixelTemplateDBObjectRcd");
  }
}

void SiPixelTemplateDBLoader::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelTemplateDBLoader);
