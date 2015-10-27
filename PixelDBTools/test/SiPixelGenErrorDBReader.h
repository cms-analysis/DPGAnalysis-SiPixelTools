#ifndef CondTools_SiPixel_SiPixelGenErrorDBReader_h
#define CondTools_SiPixel_SiPixelGenErrorDBReader_h

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelGenErrorDBObject.h"
#include "CondFormats/DataRecord/interface/SiPixelGenErrorDBObjectRcd.h"
#include "CalibTracker/Records/interface/SiPixelGenErrorDBObjectESProducerRcd.h"


class SiPixelGenErrorDBReader : public edm::EDAnalyzer {
   public:
      explicit SiPixelGenErrorDBReader(const edm::ParameterSet&);
      ~SiPixelGenErrorDBReader();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      void printObject() ;

      std::string theGenErrorCalibrationLocation;
      bool theDetailedGenErrorDBErrorOutput;
      bool theFullGenErrorDBOutput;
      SiPixelGenErrorDBObject dbobject;
      bool phase1_; // to signal phase1 geometry
};
#endif
