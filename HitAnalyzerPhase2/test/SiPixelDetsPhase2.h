#ifndef SiPixelDetsPhase2_h
#define SiPixelDetsPhase2_h

#include <map>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

class SiPixelDetsPhase2 : public edm::EDAnalyzer
{
 public:
  
  explicit SiPixelDetsPhase2(const edm::ParameterSet& conf);
  
  virtual ~SiPixelDetsPhase2();
  
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
