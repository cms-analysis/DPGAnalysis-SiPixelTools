#ifndef StudyRecHitMatching_h
#define StudyRecHitMatching_h

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

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

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
#include <vector>

// Hit Associator
#define PIXEL_ASSOCIATOR // use my special pixel associator

#ifdef PIXEL_ASSOCIATOR
#include "DPGAnalysis-SiPixelTools/PixelHitAssociator/interface/PixelHitAssociator.h"
#else 
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
//#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#endif

class TrackerTopology;

class StudyRecHitMatching : public edm::EDAnalyzer {

   public:
	//Constructor
	StudyRecHitMatching(const edm::ParameterSet& conf);

	//Destructor
	~StudyRecHitMatching();

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
	edm::EDGetTokenT<edm::PSimHitContainer> tPixelSimHits;

	edm::ParameterSet conf_;


	void fillBarrel(const TrackingRecHit*,const PSimHit &, DetId, const PixelGeomDetUnit *,	
			const TrackerTopology *,double,double,double,int);
	void fillForward(const TrackingRecHit*, const PSimHit &, DetId, const PixelGeomDetUnit *,
			 const TrackerTopology *,double,double,double,int);
#ifdef PIXEL_ASSOCIATOR
	std::vector<PSimHit> associateHit(const std::vector<PSimHit>&,DetId);
	//PixelHitAssociator::Config trackerHitAssociatorConfig_;
        //float matchToSims(PixelHitAssociator&, const TrackingRecHit *,
	//		    DetId, const PixelGeomDetUnit*, const TrackerTopology *,double,double,double);
        float matchToSims(const std::vector<PSimHit>&,const TrackingRecHit *,
			    DetId, const PixelGeomDetUnit*, const TrackerTopology *,double,double,double);
#else
	TrackerHitAssociator::Config trackerHitAssociatorConfig_;
        float matchToSims(const TrackerHitAssociator&, const TrackingRecHit *, 
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
	MonitorElement *hz1, *hz1_1, *hz1_2, *hz1_3, *hz1_4, *hz1_5;
	MonitorElement *hz1_11, *hz1_12, *hz1_13,*hz1_14,*hz1_15,*hz1_16,*hz1_17;	
	MonitorElement *hz1_21, *hz1_22, *hz1_23,*hz1_24,*hz1_25,*hz1_26;	
	MonitorElement *recHitXResB1,*recHitXResB2,*recHitXResB3,*recHitXResB4,*recHitXResB5,*recHitXResB6;

	//MonitorElement *recHitX11, *recHitX12; // *recHitX21, *recHitX22;
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
	MonitorElement* clusizeXVsX[4];
	MonitorElement* clusizeXVsPhi[4];
	MonitorElement* clusizeX1VsPhi[4];
	MonitorElement* clusizeX2VsPhi[4];
	MonitorElement* clusizeX3VsPhi[4];

	MonitorElement *cluSizeXVsPhi1; // large scale ,*clusizeX22VsPhi,*clusizeX23VsPhi,*clusizeX24VsPhi ;
	MonitorElement *size1_mz_f,*size1_mz_nf,*size1_pz_f,*size1_pz_nf;
	MonitorElement *size2_mz_f,*size2_mz_nf,*size2_pz_f,*size2_pz_nf;
	MonitorElement *size3_mz_f,*size3_mz_nf,*size3_pz_f,*size3_pz_nf;

	MonitorElement *hdist1,*hdist2,*hdist3,*hdist4,*hdist5;
	MonitorElement *hcount1,*hcount2,*hcount3,*hcount4,*hcount5,*hcount6,*hcount7,*hcount8,*hcount9;
	MonitorElement *test;
	MonitorElement *htest1, *htest2, *htest3, *htest4, *htest5, *htest6;
	MonitorElement *hParticleType1,*hTrackId1,*hProcessType1,
	  *hParticleType2,*hTrackId2,*hProcessType2,
	  *hParticleType3,*hTrackId3,*hProcessType3,
	  *hParticleType4,*hTrackId4,*hProcessType4,
	  *hParticleType5,*hTrackId5,*hProcessType5;

	MonitorElement *phiPerDet1,*phiPerDet2,*phiPerDet3,*phiPerDet4 ;
	MonitorElement *cluXPerDet1,*cluXPerDet2,*cluXPerDet3,*cluXPerDet4;
	MonitorElement *cluYPerDet1,*cluYPerDet2,*cluYPerDet3,*cluYPerDet4;
	//MonitorElement *simsXPerDet1,*simsXPerDet2,*simsXPerDet3,*simsXPerDet4;
	//MonitorElement *simsYPerDet1,*simsYPerDet2,*simsYPerDet3,*simsYPerDet4;

        edm::InputTag src_;
        bool useTracks_;
        edm::InputTag tracks_;
        bool phase_;
	bool quick_;
	bool muOnly_;
	bool useSimHits_;
	double ptCut_;
	int count1, count2, count3, count4, count5, count6, count9;

};

#endif
