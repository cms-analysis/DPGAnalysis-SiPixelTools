//#include <memory>

#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CondFormats/DataRecord/interface/SiPixelFedCablingMapRcd.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFedCablingMap.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFedCablingTree.h"


using namespace std;
using namespace edm;
using namespace sipixelobjects;

// class declaration
class SiPixelFedCablingMapDBReader : public edm::EDAnalyzer {
   public:
      explicit SiPixelFedCablingMapDBReader( const edm::ParameterSet& ) {}
      ~SiPixelFedCablingMapDBReader();
      virtual void analyze( const edm::Event&, const edm::EventSetup& );
   private:
};


SiPixelFedCablingMapDBReader::~SiPixelFedCablingMapDBReader(){}

void SiPixelFedCablingMapDBReader::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {

   std::cout << "====== SiPixelFedCablingMapDBReader" << std::endl;

   edm::ESHandle<SiPixelFedCablingMap> map;
   iSetup.get<SiPixelFedCablingMapRcd>().get(map);

   LogWarning(" got map, version: ") << map->version();
   auto tree = map->cablingTree();
   LogWarning("PRINT MAP:")<<tree->print(100);
   LogWarning("PRINT MAP, end:");

}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelFedCablingMapDBReader);
