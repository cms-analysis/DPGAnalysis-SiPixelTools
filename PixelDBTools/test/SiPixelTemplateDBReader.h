#ifndef CondTools_SiPixel_SiPixelTemplateDBReader_h
#define CondTools_SiPixel_SiPixelTemplateDBReader_h

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelTemplateDBObject.h"
#include "CondFormats/DataRecord/interface/SiPixelTemplateDBObjectRcd.h"
#include "CalibTracker/Records/interface/SiPixelTemplateDBObjectESProducerRcd.h"


class SiPixelTemplateDBReader : public edm::EDAnalyzer {
   public:
      explicit SiPixelTemplateDBReader(const edm::ParameterSet&);
      ~SiPixelTemplateDBReader();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      void printObject() ;
		
      //edm::ESWatcher<SiPixelTemplateDBObjectESProducerRcd>  SiPixTemplDBObjectWatcher_;
      //edm::ESWatcher<SiPixelTemplateDBObjectRcd>  SiPixTemplDBObjWatcher_;
			
      std::string theTemplateCalibrationLocation;
      bool theDetailedTemplateDBErrorOutput;
      bool theFullTemplateDBOutput;
      bool testGlobalTag;
      SiPixelTemplateDBObject dbobject;
      //bool hasTriggeredWatcher;			

};

#endif
