#ifndef PixelTree_h
#define PixelTree_h

/** \class PixelTree
 * ----------------------------------------------------------------------
 * PixelTree
 * ---------
 * Summary: The full pixel information, including tracks and cross references
 *          A lot has been copied from 
 *            DPGAnalysis/SiPixelTools/plugins/PixelNtuplizer_RealData.cc
 *            SiPixelMonitorTrack/src/SiPixelTrackResidualSource.cc
 *
 * ----------------------------------------------------------------------
 * Send all questions, wishes and complaints to the 
 *
 * Author:  Urs Langenegger (PSI)
 * ----------------------------------------------------------------------
 *
 *
 ************************************************************/

#include <map>

#include "CondFormats/SiPixelObjects/interface/SiPixelFrameConverter.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFedCablingMap.h"
#include "CondFormats/DataRecord/interface/SiPixelFedCablingMapRcd.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

#include "TrackingTools/TrackFitters/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "Alignment/OfflineValidation/interface/TrackerValidationVariables.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "TObject.h"

using namespace reco;

class TObject;
class TTree;
class TH1D;
class TFile;
class RectangularPixelTopology;
class DetId; 

class PixelTree : public edm::EDAnalyzer {
 public:
  
  explicit PixelTree(const edm::ParameterSet& ps);
  virtual ~PixelTree();
  virtual void beginJob();
  virtual void beginRun(const edm::Run &, const edm::EventSetup &);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  
 protected:
  void init();
  void fillEvent();
  void fillTracks();
  void fillRecHits();
  void fillVertex();
  void fillDigis();
  
  void bpixNames(const DetId &pID, int &DBlayer, int &DBladder, int &DBmodule);
  void fpixNames(const DetId &pID, int &DBdisk, int &DBblade, int &DBpanel, int &DBplaquette);
  
  void onlineRocColRow(const DetId &pID, int offlineRow, int offlineCol, int &roc, int &col, int &row);

  void isPixelTrack(const edm::Ref<std::vector<Trajectory> > &refTraj, bool &isBpixtrack, bool &isFpixtrack);


 private:
  int             fVerbose; 
  std::string     fRootFileName; 
  std::string     fGlobalTag, fType;
  int             fDumpAllEvents;
  edm::InputTag   fPrimaryVertexCollectionLabel;
  edm::InputTag   fMuonCollectionLabel, fTrackCollectionLabel, fTrajectoryInputLabel, fPixelClusterLabel, fPixelRecHitLabel;
  edm::InputTag   fLumiSummaryLabel;
  std::string     fHLTProcessName; 
  edm::InputTag   fL1GTReadoutRecordLabel, fL1GTmapLabel, fHLTResultsLabel, fL1MuGMTLabel; 
  edm::ESHandle<SiPixelFedCablingMap> fCablingMap;

  bool fAccessSimHitInfo;

  TFile *fFile; 
  TH1D  *fL1Thist, *fL1TThist, *fHLThist; 
  TTree *fTree;

  int                    fInit; 
  std::map<int, int>     fFEDID; 
  SiPixelFrameConverter *fPFC[40]; 

  // -- general stuff
  unsigned int fRun, fEvent, fLumiBlock; 
  int          fBX, fOrbit; 
  unsigned int fTimeLo, fTimeHi; 
  
  float fBz;
  int fFED1, fFED2; 
  float fLumi, fLumiInt; 

  unsigned int fL1T, fHLT; 
  unsigned int fL1TA[4], fL1TT[4], fHLTA[8];
  bool         fTtA[64], fL1A[128], fHlA[1024]; 

  // -- primary vertices
  static const int PVMAX = 100; 
  int fPvN; 
  float fPvX[PVMAX], fPvY[PVMAX],  fPvZ[PVMAX], fPvXe[PVMAX], fPvYe[PVMAX],  fPvZe[PVMAX],  fPvChi2[PVMAX], fPvNdof[PVMAX];
  int   fPvIsFake[PVMAX];


  // -- muons
  static const int MUMAX = 100; 
  int fMuN; 
  int fMuType[MUMAX], fMuTkI[MUMAX];
  float fMuPt[MUMAX], fMuTheta[MUMAX], fMuPhi[MUMAX], 
    fMuT[MUMAX], fMuTcorr[MUMAX], fMuTerr[MUMAX], fMuTmean;
  int fMuTrigger; 

  // -- HF
  float fEtPlus, fEtMinus, fTplus, fTminus; 


  // -- tracks
  static const int TRACKMAX = 10000; 
  static const int CLPERTRACKMAX = 20; 
  int   fTkN; 
  int   fTkQuality[TRACKMAX], fTkCharge[TRACKMAX];
  float fTkNHits[TRACKMAX], fTkLHits[TRACKMAX];
  float fTkLHitsI[TRACKMAX], fTkLHitsO[TRACKMAX], fTkNHitFr[TRACKMAX];
  float fTkChi2[TRACKMAX], fTkNdof[TRACKMAX];
  float fTkPt[TRACKMAX], fTkTheta[TRACKMAX], fTkEta[TRACKMAX], fTkPhi[TRACKMAX];
  float fTkD0[TRACKMAX], fTkDz[TRACKMAX];
  float fTkVx[TRACKMAX], fTkVy[TRACKMAX],    fTkVz[TRACKMAX];
  float fTkAlpha[TRACKMAX][CLPERTRACKMAX],   fTkBeta[TRACKMAX][CLPERTRACKMAX]; 
  float fTkResX[TRACKMAX][CLPERTRACKMAX],    fTkResY[TRACKMAX][CLPERTRACKMAX];
  float fTkResXe[TRACKMAX][CLPERTRACKMAX],   fTkResYe[TRACKMAX][CLPERTRACKMAX];
  float fTkRes2X[TRACKMAX][CLPERTRACKMAX];
  float fTkRes2Xe[TRACKMAX][CLPERTRACKMAX];
  int   fTkType[TRACKMAX], fTkMuI[TRACKMAX], fTkClN[TRACKMAX], fTkClI[TRACKMAX][CLPERTRACKMAX]; 

  // -- clusters
  static const int CLUSTERMAX = 100000; 
  static const int DGPERCLMAX = 100;  
  static const int TKPERCLMAX = 100;  
  int          fClN;
  int          fClSize[CLUSTERMAX], fClSizeX[CLUSTERMAX], fClSizeY[CLUSTERMAX]; 
  float        fClRow[CLUSTERMAX], fClCol[CLUSTERMAX];  //??
  float        fClLx[CLUSTERMAX], fClLy[CLUSTERMAX], fClLxe[CLUSTERMAX], fClLye[CLUSTERMAX];
  float        fClGx[CLUSTERMAX], fClGy[CLUSTERMAX], fClGz[CLUSTERMAX];
  float        fClCharge[CLUSTERMAX],  fClChargeCorr[CLUSTERMAX];
  int          fClType[CLUSTERMAX], fClTkN[CLUSTERMAX], fClTkI[CLUSTERMAX][TKPERCLMAX], fClDgN[CLUSTERMAX], fClDgI[CLUSTERMAX][DGPERCLMAX]; 
  int          fClDetId[CLUSTERMAX];
  int          fClLayer[CLUSTERMAX],  fClLadder[CLUSTERMAX], fClModule[CLUSTERMAX];
  int          fClFlipped[CLUSTERMAX], fClDisk[CLUSTERMAX],  fClBlade[CLUSTERMAX], fClPanel[CLUSTERMAX], fClPlaquette[CLUSTERMAX];
  float        fClRhLx[CLUSTERMAX], fClRhLy[CLUSTERMAX], fClRhLxE[CLUSTERMAX], fClRhLyE[CLUSTERMAX];
  float        fClRhGx[CLUSTERMAX], fClRhGy[CLUSTERMAX], fClRhGz[CLUSTERMAX];
  float        fClRhProb[CLUSTERMAX], fClRhProbX[CLUSTERMAX], fClRhProbY[CLUSTERMAX];
  unsigned int fClRhQualWord[CLUSTERMAX]; 
  int          fClRhqBin[CLUSTERMAX], fClRhSpansTwoROCs[CLUSTERMAX], fClRhIsOnEdge[CLUSTERMAX], fClRhHasBadPixels[CLUSTERMAX]; 




  // gavril@jhu.edu: Start: Add sim hit and sim track info ===========================================================

  static const int SIMHITPERCLMAX = 10;             // max number of simhits associated with a cluster/rechit
  int   fClSimHitN[CLUSTERMAX];                     // actual number of simhits associated with a cluster/rechit
  int   fClSimHitPID[CLUSTERMAX][SIMHITPERCLMAX];   // PID of the particle that produced the simHit associated with the cluster/rechit
  int   fClSimHitPRC[CLUSTERMAX][SIMHITPERCLMAX];   // procces type ( 2 = primary, ... )
  float fClSimHitLx[CLUSTERMAX][SIMHITPERCLMAX];    // X local position of simhit 
  float fClSimHitLy[CLUSTERMAX][SIMHITPERCLMAX];    // Y local position of simhit
  int   fClSimHitTrkID[CLUSTERMAX][SIMHITPERCLMAX]; // track ID
  float fClSimHitThe[CLUSTERMAX][SIMHITPERCLMAX];   // thetaAtEntry
  float fClSimHitPhi[CLUSTERMAX][SIMHITPERCLMAX];   // phiAtEntry
  float fClSimHitMom[CLUSTERMAX][SIMHITPERCLMAX];   // momentum at entry

  static const int SIMTRKPERCLMAX = 10;             // max number of sim tracks associated with a cluster
  int   fClSimTrN[CLUSTERMAX];                      // actual number of sim tracks associated with a cluster
  int   fClSimTrID[CLUSTERMAX][SIMTRKPERCLMAX];     // ID of the sim track
  float fClSimTrFr[CLUSTERMAX][SIMTRKPERCLMAX];     // fraction of cluster churge due to this sim track ???
  
  int   fClSimTrID2[CLUSTERMAX][SIMTRKPERCLMAX]    ; // same as fClSimTrID ???
  int   fClSimTrType[CLUSTERMAX][SIMTRKPERCLMAX]   ;
  int   fClSimTrQ[CLUSTERMAX][SIMTRKPERCLMAX]      ;
  float fClSimTrPx[CLUSTERMAX][SIMTRKPERCLMAX]     ;
  float fClSimTrPy[CLUSTERMAX][SIMTRKPERCLMAX]     ;
  float fClSimTrPz[CLUSTERMAX][SIMTRKPERCLMAX]     ;
  float fClSimTrEn[CLUSTERMAX][SIMTRKPERCLMAX]     ;
  
  float fClSimTrEta[CLUSTERMAX][SIMTRKPERCLMAX]    ;
  float fClSimTrPhi[CLUSTERMAX][SIMTRKPERCLMAX]    ;
  float fClSimTrPt[CLUSTERMAX][SIMTRKPERCLMAX]     ;
  
  float fClSimTrVx[CLUSTERMAX][SIMTRKPERCLMAX]     ;
  float fClSimTrVy[CLUSTERMAX][SIMTRKPERCLMAX]     ;
  float fClSimTrVz[CLUSTERMAX][SIMTRKPERCLMAX]     ;

  // gavril: End: Add sim hit and sim track info ===========================================================


  // -- digis
  static const int DIGIMAX = 1000000; 
  int fDgN; 
  int   fDgRow[DIGIMAX], fDgCol[DIGIMAX];
  int   fDgDetId[DIGIMAX];
  int   fDgRoc[DIGIMAX], fDgRocR[DIGIMAX], fDgRocC[DIGIMAX];
  float fDgLx[DIGIMAX], fDgLy[DIGIMAX];
  float fDgGx[DIGIMAX], fDgGy[DIGIMAX], fDgGz[DIGIMAX];
  float fDgAdc[DIGIMAX], fDgCharge[DIGIMAX];
  int   fDgClI[DIGIMAX];

  // -- muon auxiliary variables
  float fsbias[5][15];
  float fsrms[5][15];
  float fspoints[5][15];


  HLTConfigProvider fHltConfig;  
  bool fValidHLTConfig;
};

#endif
