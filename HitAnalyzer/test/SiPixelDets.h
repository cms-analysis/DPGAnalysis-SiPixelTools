#ifndef SiPixelDets_h
#define SiPixelDets_h

#include <map>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "DataFormats/Common/interface/Handle.h"
//#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

class SiPixelDets : public edm::EDAnalyzer
{
 public:
  
  explicit SiPixelDets(const edm::ParameterSet& conf);
  
  virtual ~SiPixelDets();
  
  //  virtual void beginJob(const edm::EventSetup& c);

  virtual void beginJob();
  
  virtual void endJob(); 
  
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  
  void printDet(DetId detid, const TrackerTopology* tt);

 private:

  edm::ParameterSet conf_;
  bool phase1_;
  //typedef std::vector< edm::ParameterSet > Parameters;
  //Parameters BPixParameters_;
  //Parameters FPixParameters_;

};


#endif
