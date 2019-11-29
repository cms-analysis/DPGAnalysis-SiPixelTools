#ifndef SiPixelRecHitsValid_pix_h
#define SiPixelRecHitsValid_pix_h

/** \class SiPixelRecHitsValid
 * File: SiPixelRecHitsValid.h
 * \author Jason Shaev, JHU
 * Created: 6/7/06
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"

//Simhit stuff
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "Geometry/CommonTopologies/interface/PixelTopology.h"
//#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
//#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

// For ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TVector3.h>

#include <string>

// Hit Associator
//#define PIXEL_ASSOCIATOR // use my special pixel associator

#ifdef PIXEL_ASSOCIATOR
#include "DPGAnalysis-SiPixelTools/PixelHitAssociator/interface/PixelHitAssociator.h"
#else 
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#endif

//#define SPLIT_INTO_ETA  // make seperate eta histos 
//#define SPLIT_INTO_Z // make seperate ihstos for module z
//#define LOOK_AT_ERRORS // look at alignment errors 
//#define SPLIT_INTO_PANELS // split forwatd disks in oanels 

class TrackerTopology;

class SiPixelRecHitsValid_pix : public edm::EDAnalyzer {

   public:
	//Constructor
	SiPixelRecHitsValid_pix(const edm::ParameterSet& conf);

	//Destructor
	~SiPixelRecHitsValid_pix();

   protected:

	virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
	void beginJob();
	void endJob();

   private:

	std::string outputFile_;
	bool verbose_;
	bool muOnly_;
	bool ptCut_;
	edm::EDGetTokenT<edmNew::DetSetVector<SiPixelRecHit>> tPixelRecHit;
        edm::EDGetTokenT<TrajTrackAssociationCollection> tTracks;
	edm::ParameterSet conf_;

	TrackerHitAssociator::Config trackerHitAssociatorConfig_;

	void fillBarrel(const TrackingRecHit*,const PSimHit &, DetId, const PixelGeomDetUnit *,	
			 const TrackerTopology *);
	void fillForward(const TrackingRecHit*, const PSimHit &, DetId, const PixelGeomDetUnit *,
			 const TrackerTopology *);
#ifdef PIXEL_ASSOCIATOR
        void matchToSimHits(const PixelHitAssociator&, const TrackingRecHit *,
			    DetId, const PixelGeomDetUnit*, const TrackerTopology *);
#else
        void matchToSimHits(const TrackerHitAssociator&, const TrackingRecHit *, 
			    DetId, const PixelGeomDetUnit*, const TrackerTopology *);
#endif
        int PhaseIBladeOfflineToOnline(const int&);

        TH1D* htheta1,*hbeta1,*hphi1, *heta1;

	//RecHits BPIX
	TH1D* recHitXResAllB;
	TH1D* recHitYResAllB;
	// Pull distributions
	TH1D* recHitXPullAllB;
	TH1D* recHitYPullAllB;

	TH1D* recHitXResLayer[4]; // versus layers 
	TH1D* recHitYResLayer[4];
	TH1D* recHitXResFlippedL[4];
	TH1D* recHitXResNonFlippedL[4];
        TH1D *recHitXResFlippedLpz[4],*recHitXResFlippedLmz[4];
        TH1D *recHitXResNonFlippedLpz[4],*recHitXResNonFlippedLmz[4];
	TH1D *recHitXErrorB[4], *recHitYErrorB[4];

	TProfile* recHitXResLayerP[4];
	TProfile* recHitYResLayerP[4];
	TProfile* recHitXResLayerP1[4];
	TProfile* recHitXResLayerP2[4];
	TProfile* recHitXResLayerP3[4];
	TProfile* recHitYResLayerP1[4];
	TProfile* recHitYResLayerP2[4];
	TProfile* recHitYResLayerP3[4];
	TProfile* recHitYResLayerP4[4];

#ifdef SPLIT_INTO_ETA
	TH1D* recHitXResLayer1Eta[25];
	TH1D* recHitXResLayer2Eta[25];
	TH1D* recHitXResLayer3Eta[25];
	TH1D* recHitXResLayer4Eta[25];
	TH1D* recHitYResLayer1Eta[25];
	TH1D* recHitYResLayer2Eta[25];
	TH1D* recHitYResLayer3Eta[25];
	TH1D* recHitYResLayer4Eta[25];
#endif // SPLIT_INTO_ETA

	// for the moment keep commened out
	//TH1D *recHitL1XResSize1,*recHitL1XResSize2,*recHitL1XResSize3; // layer 1
	//TH1D *recHitL2XResSize1,*recHitL2XResSize2,*recHitL2XResSize3; // layer 2

#ifdef SPLIT_INTO_Z
	TH1D* recHitYResLayer1Modules[8];
	TH1D* recHitYResLayer2Modules[8];
	TH1D* recHitYResLayer3Modules[8];
	TH1D* recHitYResLayer4Modules[8];
#endif
	// skip the pulls
	//TH1D* recHitXPullFlippedLadderLayers[4];
	//TH1D* recHitXPullNonFlippedLadderLayers[4];
	//TH1D* recHitYPullLayer1Modules[8];
	//TH1D* recHitYPullLayer2Modules[8];
	//TH1D* recHitYPullLayer3Modules[8];
	//TH1D* recHitYPullLayer4Modules[8];
 
	//RecHits FPIX
	TH1D* recHitXResAllF;
	TH1D* recHitXResPosZF;
	TH1D* recHitXResNegZF;
	TH1D* recHitYResAllF;
	TH1D* recHitYResPosZF;
	TH1D* recHitYResNegZF;
	TH1D* recHitXPullAllF;
	TH1D* recHitYPullAllF;
	TH1D* recHitXErrorAllF;
	TH1D* recHitYErrorAllF;


	TH1D* recHitXResDisk1[7];
	TH1D* recHitYResDisk1[7];
	TH1D* recHitXResDisk2[7];
	TH1D* recHitYResDisk2[7];
	TH1D* recHitXResDisk3[7];
	TH1D* recHitYResDisk3[7];

	TH1D* recHitXResRingSide[2][2];
	TH1D* recHitYResRingPanel[2][2];

#ifdef SPLIT_INTO_PANELS
	TH1D* recHitXResRingSidePanel[2][2][2];
	TH1D* recHitYResRingSidePanel[2][2][2];
#endif

#ifdef LOOK_AT_ERRORS
	// Alignment errors 
	TH1D *recHitXAlignError1, *recHitXAlignError2, *recHitXAlignError3;
	TH1D *recHitXAlignError4, *recHitXAlignError5, *recHitXAlignError6, *recHitXAlignError7;
	TH1D *recHitYAlignError1, *recHitYAlignError2, *recHitYAlignError3;
	TH1D *recHitYAlignError4, *recHitYAlignError5, *recHitYAlignError6, *recHitYAlignError7;
#endif

        edm::InputTag src_;
        bool useTracks_;
        edm::InputTag tracks_;
        bool phase_;
};

#endif
