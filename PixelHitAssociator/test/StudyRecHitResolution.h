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

//#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
//#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetType.h"

#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

//DWM histogram services
//#include "DQMServices/Core/interface/DQMStore.h"
//#include "DQMServices/Core/interface/MonitorElement.h"

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
	//DQMStore* dbe_;
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
        float matchToSims(const PixelHitAssociator&, const TrackingRecHit *,
			    DetId, const PixelGeomDetUnit*, const TrackerTopology *,double,double,double);
#else
        float matchToSims(const TrackerHitAssociator&, const TrackingRecHit *, 
			    DetId, const PixelGeomDetUnit*, const TrackerTopology *,double,double,double);
#endif
        int PhaseIBladeOfflineToOnline(const int&);


	TH1D*hptTrack, *hphiTrack, *hetaTrack;

	//RecHits BPIX
	TH1D* recHitXResAllB;
	TH1D* recHitYResAllB;
	TH1D* recHitXPullAllB;
	TH1D* recHitYPullAllB;
	TH1D* recHitXErrorAllB;
	TH1D* recHitYErrorAllB;

	//RecHits FPIX
	TH1D* recHitXResAllF;
	TH1D* recHitYResAllF;
	TH1D* recHitXPullAllF;
	TH1D* recHitYPullAllF;
	TH1D* recHitXErrorAllF;
	TH1D* recHitYErrorAllF;
	TH1D* recHitXResPosZF;
	TH1D* recHitXResNegZF;
	TH1D* recHitYResPosZF;
	TH1D* recHitYResNegZF;

	// Unused 
	//TH1D*hdist1, *hdist2, *hdist3, *hdist4;
	//TH1D*hcount1,*hcount2,*hcount3,*hcount4,*hcount5,*hcount6,*hcount7,*hcount8,*hcount9;
	//TH1D*hParticleType1,*hTrackId1,*hProcessType1,
	//*hParticleType2,*hTrackId2,*hProcessType2,
	//*hParticleType3,*hTrackId3,*hProcessType3,
	//*hParticleType4,*hTrackId4,*hProcessType4,
	//*hParticleType5,*hTrackId5,*hProcessType5;

	TH1D* recHitXResLayer[4];
	TH1D* recHitYResLayer[4];
	TH1D* recHitXResLayerP[4];
	TH1D* recHitYResLayerP[4];
	TH1D* recHitXResLayerP1[4];
	TH1D* recHitXResLayerP2[4];
	TH1D* recHitXResLayerP3[4];
	TH1D* recHitYResLayerP1[4];
	TH1D* recHitYResLayerP2[4];
	TH1D* recHitYResLayerP3[4];
	TH1D* recHitYResLayerP4[4];

	TH1D* recHitXResVsPhiP[4];
	TH1D* recHitXResVsPhiP1[4];
	TH1D* recHitXResVsPhiP2[4];
	TH1D* recHitXResVsPhiP3[4];

	TH1D* recHitXResFlippedLadderLayers[4];
	TH1D* recHitXResNonFlippedLadderLayers[4];
        TH1D*recHitXResFlippedLadderLayersSide[4][2];
        TH1D*recHitXResNonFlippedLadderLayersSide[4][2];

	TH1D* recHitYResLayer1Modules[8];
	TH1D* recHitYResLayer2Modules[8];
	TH1D* recHitYResLayer3Modules[8];
	TH1D* recHitYResLayer4Modules[8];

	// cluster size vs phi
	TH1D* clusizeXVsX[4];
	TH1D* clusizeXVsPhi[4];
	TH1D* clusizeX1VsPhi[4];
	TH1D* clusizeX2VsPhi[4];
	TH1D* clusizeX3VsPhi[4];
	TH1D*cluSizeXVsPhi1; // large scale ,*clusizeX22VsPhi,*clusizeX23VsPhi,*clusizeX24VsPhi ;

	TH1D*recHitL1XResSize1,*recHitL1XResSize2,*recHitL1XResSize3;   
	TH1D*recHitL2XResSize1,*recHitL2XResSize2,*recHitL2XResSize3;   
	TH1D*recHitL3XResSize1,*recHitL3XResSize2,*recHitL3XResSize3;   
	TH1D*recHitL4XResSize1,*recHitL4XResSize2,*recHitL4XResSize3;   

	TH1D* recHitXResLayer1Eta[25];
	TH1D* recHitXResLayer2Eta[25];
	TH1D* recHitXResLayer3Eta[25];
	TH1D* recHitXResLayer4Eta[25];
	TH1D* recHitYResLayer1Eta[25];
	TH1D* recHitYResLayer2Eta[25];
	TH1D* recHitYResLayer3Eta[25];
	TH1D* recHitYResLayer4Eta[25];

	//TH1D*size1_mz_f,*size1_mz_nf,*size1_pz_f,*size1_pz_nf;
	//TH1D*size2_mz_f,*size2_mz_nf,*size2_pz_f,*size2_pz_nf;
	//TH1D*size3_mz_f,*size3_mz_nf,*size3_pz_f,*size3_pz_nf;

#ifdef NOT
	//Clusters BPIX
	TH1D* clustYSizeModule[8];
	//TH1D* clustXSizeLayer[4];
	TH1D* clustChargeLayer1Modules[8];
	TH1D* clustChargeLayer2Modules[8];
	TH1D* clustChargeLayer3Modules[8];
	TH1D* clustChargeLayer4Modules[8];

	TH1D*hz1, *hz1_1, *hz1_2, *hz1_3, *hz1_4, *hz1_5;
	TH1D*hz1_11, *hz1_12, *hz1_13,*hz1_14,*hz1_15,*hz1_16,*hz1_17;	

	TH1D*phiPerDet1,*phiPerDet2,*phiPerDet3,*phiPerDet4 ;
	TH1D*cluXPerDet1,*cluXPerDet2,*cluXPerDet3,*cluXPerDet4;
	TH1D*cluYPerDet1,*cluYPerDet2,*cluYPerDet3,*cluYPerDet4;

#endif // NOT




        edm::InputTag src_;
        bool useTracks_;
        edm::InputTag tracks_;
        bool phase_;
	bool quick_;
	bool muOnly_;
	double ptCut_;
	int count1, count2, count3, count4, count5, count6, count9;

};

#endif
