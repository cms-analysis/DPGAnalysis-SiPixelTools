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

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DQM/TrackerMonitorTrack/interface/MonitorTrackResiduals.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "Alignment/OfflineValidation/interface/TrackerValidationVariables.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
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
  virtual void beginJob(const edm::EventSetup& iSetup);
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


  void readOffsets();
  void sectorAndWheel(const reco::Muon &muon0, int &w0, int &s0);
  float correctedTime(const reco::Muon &muon0);
  void dumpDetIds(const edm::EventSetup& iSetup);

  void isPixelTrack(const edm::Ref<std::vector<Trajectory> > &refTraj, bool &isBpixtrack, bool &isFpixtrack);


 private:
  int             fVerbose; 
  std::string     fRootFileName, fTrajectoryInputLabel;
  edm::InputTag   fMuonCollectionLabel, fTrackCollectionLabel, fPixelClusterLabel;
  std::string     fL1GTReadoutRecordLabel; 
  edm::InputTag   fL1GTmapLabel;
  edm::InputTag   fHLTResultsLabel;

  TFile *fFile; 
  TH1D  *fL1Thist, *fHLThist; 
  TTree *fTree;


  // -- general stuff
  unsigned int fRun, fEvent, fLumiBlock, fBX, fOrbit; 
  unsigned int fTimeLo, fTimeHi; 
  
  float fBz;
  int fFED1, fFED2; 

  unsigned int fL1T, fL1T0,  fL1T1,  fL1T2,  fL1T3; 
  unsigned int fHLT, fHLTa0, fHLTa1, fHLTa2, fHLTa3, 
    fHLTr0, fHLTr1, fHLTr2, fHLTr3, 
    fHLTe0, fHLTe1, fHLTe2, fHLTe3; 

  // -- muons
  static const int MUMAX = 100; 
  int fMuN; 
  int fMuType[MUMAX], fMuTkI[MUMAX];
  float fMuPt[MUMAX], fMuTheta[MUMAX], fMuPhi[MUMAX], 
    fMuT[MUMAX], fMuTcorr[MUMAX], fMuTerr[MUMAX], fMuTmean;

  // -- tracks
  static const int TRACKMAX = 10000; 
  static const int CLPERTRACKMAX = 20; 
  int   fTkN; 
  int   fTkCharge[TRACKMAX];
  float fTkChi2[TRACKMAX], fTkNdof[TRACKMAX];
  float fTkPt[TRACKMAX], fTkTheta[TRACKMAX], fTkPhi[TRACKMAX];
  float fTkD0[TRACKMAX], fTkDz[TRACKMAX];
  float fTkVx[TRACKMAX], fTkVy[TRACKMAX],    fTkVz[TRACKMAX];
  float fTkAlpha[TRACKMAX][CLPERTRACKMAX],   fTkBeta[TRACKMAX][CLPERTRACKMAX]; 
  float fTkResX[TRACKMAX][CLPERTRACKMAX],    fTkResY[TRACKMAX][CLPERTRACKMAX];
  float fTkResXe[TRACKMAX][CLPERTRACKMAX],   fTkResYe[TRACKMAX][CLPERTRACKMAX];
  float fTkRes2X[TRACKMAX][CLPERTRACKMAX];
  float fTkRes2Xe[TRACKMAX][CLPERTRACKMAX];
  int   fTkType[TRACKMAX], fTkMuI[TRACKMAX], fTkClI[TRACKMAX][CLPERTRACKMAX]; 

  // -- clusters
  static const int CLUSTERMAX = 100000; 
  static const int DGPERCLMAX = 50;  
  int   fClN;
  int   fClSize[CLUSTERMAX], fClSizeX[CLUSTERMAX], fClSizeY[CLUSTERMAX]; 
  float fClRow[CLUSTERMAX], fClCol[CLUSTERMAX];  //??
  float fClLx[CLUSTERMAX], fClLy[CLUSTERMAX], fClLxe[CLUSTERMAX], fClLye[CLUSTERMAX];
  float fClGx[CLUSTERMAX], fClGy[CLUSTERMAX], fClGz[CLUSTERMAX];
  float fClCharge[CLUSTERMAX],  fClChargeCorr[CLUSTERMAX];
  int   fClType[CLUSTERMAX], fClTkI[CLUSTERMAX], fClDgI[CLUSTERMAX][DGPERCLMAX]; 
  int   fClLayer[CLUSTERMAX],  fClLadder[CLUSTERMAX], fClModule[CLUSTERMAX];
  int   fClFlipped[CLUSTERMAX], fClDisk[CLUSTERMAX],  fClBlade[CLUSTERMAX], fClPanel[CLUSTERMAX], fClPlaquette[CLUSTERMAX];

  // -- digis
  static const int DIGIMAX = 1000000; 
  int fDgN; 
  int   fDgRow[DIGIMAX], fDgCol[DIGIMAX];
  float fDgLx[DIGIMAX], fDgLy[DIGIMAX];
  float fDgGx[DIGIMAX], fDgGy[DIGIMAX], fDgGz[DIGIMAX];
  float fDgAdc[DIGIMAX], fDgCharge[DIGIMAX];
  int   fDgClI[DIGIMAX];

  // -- muon auxiliary variables
  float fsbias[5][15];
  float fsrms[5][15];
  float fspoints[5][15];
  

 
};

#endif
