#ifndef CalibTracker_SiPixelDynamicInefficiencyDBLoader_SiPixelDynamicInefficiencyDB_h
#define CalibTracker_SiPixelDynamicInefficiencyDBLoader_SiPixelDynamicInefficiencyDB_h

#include <map>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/DetId/interface/DetId.h"

class SiPixelDynamicInefficiencyDBLoader : public edm::EDAnalyzer
{
 public:
  
  explicit SiPixelDynamicInefficiencyDBLoader(const edm::ParameterSet& conf);
  
  virtual ~SiPixelDynamicInefficiencyDBLoader();
  
  virtual void beginJob();
  
  virtual void endJob(); 
  
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  
  
 private:

  edm::ParameterSet conf_;
  std::string recordName_;

  typedef std::vector< edm::ParameterSet > Parameters;
  Parameters thePixelGeomFactors_;
  Parameters theColGeomFactors_;
  Parameters theChipGeomFactors_;
  Parameters thePUEfficiency_;
  double theInstLumiScaleFactor_;
};


#endif
