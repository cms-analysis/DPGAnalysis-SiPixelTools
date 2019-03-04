#ifndef StudyRecHitResolution_h
#define StudyRecHitResolution_h

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
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

//DWM histogram services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>

// Hit Associator
//#define PIXEL_ASSOCIATOR // use my special pixel associator

#ifdef PIXEL_ASSOCIATOR
#include "DPGAnalysis-SiPixelTools/PixelHitAssociator/interface/PixelHitAssociator.h"
#else 
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#endif

class TrackerTopology;

class StudyRecHitResolution : public edm::EDAnalyzer {

   public:
	//Constructor
	StudyRecHitResolution(const edm::ParameterSet& conf);

	//Destructor
	~StudyRecHitResolution();

   protected:

	virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
	void beginJob();
	void endJob();

   private:
	DQMStore* dbe_;
	std::string outputFile_;
	bool verbose_;
	edm::EDGetTokenT<edmNew::DetSetVector<SiPixelRecHit>> tPixelRecHit;
        edm::EDGetTokenT<TrajTrackAssociationCollection> tTracks;
	edm::ParameterSet conf_;

	TrackerHitAssociator::Config trackerHitAssociatorConfig_;

	void fillBarrel(const TrackingRecHit*,const PSimHit &, DetId, const PixelGeomDetUnit *,	
			 const TrackerTopology *,double,double,double);
	void fillForward(const TrackingRecHit*, const PSimHit &, DetId, const PixelGeomDetUnit *,
			 const TrackerTopology *,double,double,double);
#ifdef PIXEL_ASSOCIATOR
        void matchToSimHits(const PixelHitAssociator&, const TrackingRecHit *,
			    DetId, const PixelGeomDetUnit*, const TrackerTopology *,double,double,double);
#else
        void matchToSimHits(const TrackerHitAssociator&, const TrackingRecHit *, 
			    DetId, const PixelGeomDetUnit*, const TrackerTopology *,double,double,double);
#endif
        int PhaseIBladeOfflineToOnline(const int&);

	//Clusters BPIX
	MonitorElement* clustYSizeModule[8];
	//MonitorElement* clustXSizeLayer[4];
	MonitorElement* clustChargeLayer1Modules[8];
	MonitorElement* clustChargeLayer2Modules[8];
	MonitorElement* clustChargeLayer3Modules[8];
	MonitorElement* clustChargeLayer4Modules[8];

	//Cluster FPIX
	MonitorElement* clustXSizeDisk1Plaquettes[7];
	MonitorElement* clustXSizeDisk2Plaquettes[7];
	MonitorElement* clustXSizeDisk3Plaquettes[7];
	MonitorElement* clustYSizeDisk1Plaquettes[7];
	MonitorElement* clustYSizeDisk2Plaquettes[7];
	MonitorElement* clustYSizeDisk3Plaquettes[7];
	MonitorElement* clustChargeDisk1Plaquettes[7];
	MonitorElement* clustChargeDisk2Plaquettes[7];
	MonitorElement* clustChargeDisk3Plaquettes[7];

	//RecHits BPIX
	MonitorElement* recHitXResAllB;
	MonitorElement* recHitYResAllB;
	//MonitorElement* recHitXFullModules;
	//MonitorElement* recHitXHalfModules;
	MonitorElement* recHitYAllModules;
	MonitorElement* recHitXResFlippedLadderLayers[4];
	MonitorElement* recHitXResNonFlippedLadderLayers[4];
        MonitorElement *recHitXResFlippedLadderLayersSide[4][2];
        MonitorElement *recHitXResNonFlippedLadderLayersSide[4][2];

	MonitorElement *recHitL1XResSize1,*recHitL1XResSize2,*recHitL1XResSize3;   
	MonitorElement *recHitL2XResSize1,*recHitL2XResSize2,*recHitL2XResSize3;   
	MonitorElement *recHitL3XResSize1,*recHitL3XResSize2,*recHitL3XResSize3;   
	MonitorElement *recHitL4XResSize1,*recHitL4XResSize2,*recHitL4XResSize3;   

	MonitorElement* recHitYResLayer1Modules[8];
	MonitorElement* recHitYResLayer2Modules[8];
	MonitorElement* recHitYResLayer3Modules[8];
	MonitorElement* recHitYResLayer4Modules[8];

	MonitorElement* recHitXResLayers[4];
	MonitorElement* recHitYResLayers[4];
	MonitorElement* recHitXResLayersP[4];
	MonitorElement* recHitYResLayersP[4];
	MonitorElement* recHitXResLayersP1[4];
	MonitorElement* recHitXResLayersP2[4];
	MonitorElement* recHitXResLayersP3[4];
	MonitorElement* recHitYResLayersP1[4];
	MonitorElement* recHitYResLayersP2[4];
	MonitorElement* recHitYResLayersP3[4];
	MonitorElement* recHitYResLayersP4[4];

	MonitorElement* recHitXResVsPhiP[4];
	MonitorElement* recHitXResVsPhiP1[4];
	MonitorElement* recHitXResVsPhiP2[4];
	MonitorElement* recHitXResVsPhiP3[4];

	MonitorElement* recHitXResLayer1Eta[25];
	MonitorElement* recHitXResLayer2Eta[25];
	MonitorElement* recHitXResLayer3Eta[25];
	MonitorElement* recHitXResLayer4Eta[25];
	MonitorElement* recHitYResLayer1Eta[25];
	MonitorElement* recHitYResLayer2Eta[25];
	MonitorElement* recHitYResLayer3Eta[25];
	MonitorElement* recHitYResLayer4Eta[25];

  	MonitorElement *htheta1,*hbeta1,*hphi1;
  	MonitorElement *htheta2,*hbeta2,*hphi2;
	MonitorElement *heta1, *heta2, *heta3, *heta4;
	MonitorElement *htest1, *htest2;
	MonitorElement *recHitX11, *recHitX12; // *recHitX21, *recHitX22;
	MonitorElement *hptTrack, *hphiTrack, *hetaTrack;

	//RecHits FPIX
	MonitorElement* recHitXResAllF;
	MonitorElement* recHitXResPosZF;
	MonitorElement* recHitXResNegZF;
	MonitorElement* recHitYResAllF;
	MonitorElement* recHitYResPosZF;
	MonitorElement* recHitYResNegZF;
	//MonitorElement* recHitXPlaquetteSize1;
	//MonitorElement* recHitXPlaquetteSize2;
	//MonitorElement* recHitYPlaquetteSize2;
	//MonitorElement* recHitYPlaquetteSize3;
	//MonitorElement* recHitYPlaquetteSize4;
	//MonitorElement* recHitYPlaquetteSize5;
	MonitorElement* recHitXResDisk1[7];
	MonitorElement* recHitYResDisk1[7];
	MonitorElement* recHitXResDisk2[7];
	MonitorElement* recHitYResDisk2[7];
	MonitorElement* recHitXResDisk3[7];
	MonitorElement* recHitYResDisk3[7];
	MonitorElement* recHitXResRingSidePanel[2][2][2];
	MonitorElement* recHitYResRingSidePanel[2][2][2];
	MonitorElement* recHitXResRingSide[2][2];
	MonitorElement* recHitYResRingPanel[2][2];

	// Pull distributions
	//RecHits BPIX
	MonitorElement* recHitXPullAllB;
	MonitorElement* recHitYPullAllB;
	MonitorElement *recHitXError1B,*recHitXError2B,*recHitXError3B,*recHitXError4B;
	MonitorElement *recHitYError1B,*recHitYError2B,*recHitYError3B,*recHitYError4B;

	MonitorElement* recHitXPullFlippedLadderLayers[4];
	MonitorElement* recHitXPullNonFlippedLadderLayers[4];
	MonitorElement* recHitYPullLayer1Modules[8];
	MonitorElement* recHitYPullLayer2Modules[8];
	MonitorElement* recHitYPullLayer3Modules[8];
	MonitorElement* recHitYPullLayer4Modules[8];

	//RecHits FPIX
	MonitorElement* recHitXPullAllF;
	MonitorElement* recHitYPullAllF;
	MonitorElement* recHitXErrorAllF;
	MonitorElement* recHitYErrorAllF;

	MonitorElement* recHitXPullDisk1Plaquettes[7];
	MonitorElement* recHitYPullDisk1Plaquettes[7];
	MonitorElement* recHitXPullDisk2Plaquettes[7];
	MonitorElement* recHitYPullDisk2Plaquettes[7];
	MonitorElement* recHitXPullDisk3Plaquettes[7];
	MonitorElement* recHitYPullDisk3Plaquettes[7];

	// Alignment errors 
	MonitorElement *recHitXAlignError1, *recHitXAlignError2, *recHitXAlignError3;
	MonitorElement *recHitXAlignError4, *recHitXAlignError5, *recHitXAlignError6, *recHitXAlignError7;
	MonitorElement *recHitYAlignError1, *recHitYAlignError2, *recHitYAlignError3;
	MonitorElement *recHitYAlignError4, *recHitYAlignError5, *recHitYAlignError6, *recHitYAlignError7;

	// cluster size vs phi
	MonitorElement* clusizeXVsPhi[4];
	MonitorElement* clusizeX1VsPhi[4];
	MonitorElement* clusizeX2VsPhi[4];
	MonitorElement* clusizeX3VsPhi[4];

	MonitorElement *clusizeX21VsPhi,*clusizeX22VsPhi,*clusizeX23VsPhi,*clusizeX24VsPhi ;
	MonitorElement *size1_mz_f,*size1_mz_nf,*size1_pz_f,*size1_pz_nf;
	MonitorElement *size2_mz_f,*size2_mz_nf,*size2_pz_f,*size2_pz_nf;
	MonitorElement *size3_mz_f,*size3_mz_nf,*size3_pz_f,*size3_pz_nf;


	MonitorElement* test;





        edm::InputTag src_;
        bool useTracks_;
        edm::InputTag tracks_;
        bool phase_;
	bool quick_;
	bool muOnly_;
	double ptCut_;
};

#endif
