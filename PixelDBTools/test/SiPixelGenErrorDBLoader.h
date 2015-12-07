#ifndef CondTools_SiPixel_SiPixelGenErrorDBLoader_h
#define CondTools_SiPixel_SiPixelGenErrorDBLoader_h

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelGenErrorDBObject.h"

class SiPixelGenErrorDBLoader : public edm::EDAnalyzer {
   public:
      explicit SiPixelGenErrorDBLoader(const edm::ParameterSet&);
      ~SiPixelGenErrorDBLoader();

      typedef std::vector<std::string> vstring;

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
			
      vstring theGenErrorCalibrations ;
      std::string theGenErrorBaseString;
      float theVersion;
      float theMagField;
      std::vector<uint32_t> theDetIds;
      std::vector<uint32_t> theTemplIds;
		
};

#endif
