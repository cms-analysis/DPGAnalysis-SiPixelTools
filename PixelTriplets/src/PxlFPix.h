// -*- C++ -*-
//
// Package:  PxlFPix
// Class:    PxlFPix
//
// my/PxlFPix/src/PxlFPix.cc
//
// Pixel (and strip) triplet residuals
//
// Original Author:  Daniel Pitzl, DESY
//         Created:  Sat Feb 12 12:12:42 CET 2011
// $Id$
//

// system include files:
#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>

// ROOT:
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include "TFile.h"
#include "TTree.h"


// CMS and user include files:
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
//#include <FWCore/Framework/interface/EventSetup.h>
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <DataFormats/BeamSpot/interface/BeamSpot.h>

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <DataFormats/TrackReco/interface/HitPattern.h>

// To convert detId to subdet/layer number:
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
//#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/Topology.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
//#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include <TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include <MagneticField/Engine/interface/MagneticField.h>

#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"

#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajectoryBuilder.h"

#include "TrackingTools/TrackFitters/interface/TrajectoryFitter.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

#include "TrackingTools/DetLayers/interface/DetLayer.h"

//#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleRcd.h"

using std::vector;
using namespace edm;
using namespace reco;
using namespace std;

class PxlFPix : public edm::EDAnalyzer {

public:
      explicit PxlFPix(const edm::ParameterSet&);
      ~PxlFPix();

private:
      virtual void beginJob() ;
      virtual void beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      edm::InputTag _triggerSrc;
      std::string _ttrhBuilder;
      HLTConfigProvider HLTConfig;

  edm::EDGetTokenT<reco::BeamSpot>  t_offlineBeamSpot_;
  edm::EDGetTokenT<reco::VertexCollection> t_offlinePrimaryVertices_ ;
  edm::EDGetTokenT <edm::TriggerResults> t_triggerSrc_ ;
  edm::EDGetTokenT <reco::TrackCollection>  t_generalTracks_;
  edm::EDGetTokenT< edm::View<reco::PFMET>> t_pfMet_;

  // ----------member data ---------------------------
  std::string proccessName_;
  TTree * newtree;

  double xpx1_g,xpy1_g,xpz1_g;
  double xpx1_l,xpy1_l,xpz1_l; 

  double xpx2_g,xpy2_g,xpz2_g;
  double xpx2_l,xpy2_l,xpz2_l; 

  double xblade;
  double xpanel;
  double xmodule;


  double  residue;

  double  residue_c;


  double resid_disk1;
  double resid_disk2;

  double  residue_refit;

  double final_pt;
  double final_phi;
  int run_num;
  int lumi_block;
};
