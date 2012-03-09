// ----------------------------------------------------------------------
// PixelTree
// ---------
// Summary: The full pixel information, including tracks and cross references
//          A lot has been copied from 
//            DPGAnalysis/SiPixelTools/plugins/PixelNtuplizer_RealData.cc
//            DQM/SiPixelMonitorTrack/src/SiPixelTrackResidualSource.cc
//
// ----------------------------------------------------------------------
// Send all questions, wishes and complaints to the 
//
// Author:  Urs Langenegger (PSI)
// ----------------------------------------------------------------------

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#include "PixelTree.h"

#include "CondFormats/Alignment/interface/Definitions.h"
#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtFdlWord.h"

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "CondFormats/SiPixelObjects/interface/DetectorIndex.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/Provenance/interface/Timestamp.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"  
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h" 

#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"


#include <TROOT.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

using namespace std;
using namespace edm;
using namespace reco;

// ----------------------------------------------------------------------
PixelTree::PixelTree(edm::ParameterSet const& iConfig): 
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fRootFileName(iConfig.getUntrackedParameter<string>("rootFileName", string("pixelTree.root"))),
  fGlobalTag(iConfig.getParameter<string>("globalTag").c_str()),
  fType(iConfig.getUntrackedParameter<string>("type", string("somepin"))),
  fDumpAllEvents(iConfig.getUntrackedParameter<int>("dumpAllEvents", 0)),
  fPrimaryVertexCollectionLabel(iConfig.getUntrackedParameter<InputTag>("PrimaryVertexCollectionLabel", edm::InputTag("offlinePrimaryVertices"))),
  fMuonCollectionLabel(iConfig.getUntrackedParameter<InputTag>("muonCollectionLabel", edm::InputTag("muons"))),
  fTrackCollectionLabel(iConfig.getUntrackedParameter<InputTag>("trackCollectionLabel", edm::InputTag("generalTracks"))),
  fTrajectoryInputLabel(iConfig.getUntrackedParameter<InputTag>("trajectoryInputLabel", edm::InputTag("ctfRefitter"))),
  fPixelClusterLabel(iConfig.getUntrackedParameter<InputTag>("pixelClusterLabel", edm::InputTag("siPixelClusters"))), 
  fPixelRecHitLabel(iConfig.getUntrackedParameter<InputTag>("pixelRecHitLabel", edm::InputTag("siPixelRecHits"))), 
  fHLTProcessName(iConfig.getUntrackedParameter<string>("HLTProcessName")),
  fL1GTReadoutRecordLabel(iConfig.getUntrackedParameter<InputTag>("L1GTReadoutRecordLabel", edm::InputTag("gtDigis"))),
  fL1GTmapLabel(iConfig.getUntrackedParameter<InputTag>("hltL1GtObjectMap", edm::InputTag("hltL1GtObjectMap"))),
  fHLTResultsLabel(iConfig.getUntrackedParameter<InputTag>("HLTResultsLabel", edm::InputTag("TriggerResults::HLT"))),
  fL1MuGMTLabel(iConfig.getUntrackedParameter<InputTag>("l1muGmtLabel", edm::InputTag("gtDigis"))),
  fAccessSimHitInfo(iConfig.getUntrackedParameter<bool>( "accessSimHitInfo", false) ),
  fInit(0)
{
  string rcsid = string("$Id: PixelTree.cc,v 1.49 2012/03/07 16:30:06 ursl Exp $");
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- PixelTree constructor" << endl;
  cout << "---  version:                         " << rcsid << endl;
  cout << "---  type:                            " << fType << endl;
  cout << "---  global tag:                      " << fGlobalTag << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  cout << "---  dumpAllEvents:                   " << fDumpAllEvents << endl;
  cout << "---  rootFileName:                    " << fRootFileName << endl;
  cout << "---  trajectoryInputLabel:            " << fTrajectoryInputLabel << endl;
  cout << "---  muonCollectionLabel:             " << fMuonCollectionLabel << endl;
  cout << "---  trackCollectionLabel:            " << fTrackCollectionLabel << endl;
  cout << "---  pixelClusterLabel:               " << fPixelClusterLabel << endl;
  cout << "---  pixelRecHitLabel                 " << fPixelRecHitLabel << endl;
  cout << "---  HLT process name                 " << fHLTProcessName << endl;
  cout << "---  L1GTReadoutRecordLabel:          " << fL1GTReadoutRecordLabel << endl;
  cout << "---  hltL1GtObjectMap:                " << fL1GTmapLabel << endl;
  cout << "---  HLTResultsLabel:                 " << fHLTResultsLabel << endl;
  cout << "---  L1MuGMTLabel:                    " << fL1MuGMTLabel << endl;
  cout << "---  (int)fAccessSimHitInfo           " << (int)fAccessSimHitInfo << endl;
  cout << "----------------------------------------------------------------------" << endl;

  fPvN = PVMAX; 
  fMuN = MUMAX; 
  fTkN = TRACKMAX; 
  fClN = CLUSTERMAX; 
  fDgN = DIGIMAX; 
  init();

}

// ----------------------------------------------------------------------
PixelTree::~PixelTree() { }  

// ----------------------------------------------------------------------
void PixelTree::endJob() { 
  fFile->cd();

  // -- (kind of) from M. Verzetti
  TObjString SwRelease(Form("%s", (char*)gSystem->Getenv("CMSSW_VERSION")));
  SwRelease.Write("SwRelease");
  TObjString GlobalTag(fGlobalTag.c_str());
  GlobalTag.Write("GlobalTag");
  TObjString Type(fType.c_str());
  Type.Write("Type");
  TObjString rcsid("$Id: PixelTree.cc,v 1.49 2012/03/07 16:30:06 ursl Exp $");
  rcsid.Write("Rcsid");



  fTree->Write();
  //   fL1Thist->Write(); 
  //   fHLThist->Write(); 

  fFile->Write();
  fFile->Close();
  delete fFile;

  for (int i = 0; i < 40; ++i) {
    delete fPFC[i];
  }

  cout << "==>PixelTree> Succesfully gracefully ended job" << endl;

}

// ----------------------------------------------------------------------
void PixelTree::beginJob() {
  
  // -- a 'feature' of TFileService is that it does not like to add trees until a histogram is also created.
  //  edm::Service<TFileService> fs;
  //  fH1 = fs->make<TH1D>("nevt","nevt", 1, 0., 1.);

  // -- switch to direct TFile as a *** glibc detected *** corrupted double-linked list: 0x13389518 ***
  //    leaves the TFileService empty...
  fFile = TFile::Open(fRootFileName.c_str(), "RECREATE");
  fFile->cd();

  fTree = new TTree("pixelTree", "pixelTree");

  fTree->Branch("run",          &fRun,          "run/I");
  fTree->Branch("lumiblock",    &fLumiBlock,    "lumiblock/I");
  fTree->Branch("event",        &fEvent,        "event/I");
  fTree->Branch("bx",           &fBX,           "bx/I");
  fTree->Branch("orbit",        &fOrbit,        "orbit/I");
  fTree->Branch("bz",           &fBz,           "bz/F");
  fTree->Branch("tlo",          &fTimeLo,       "tlo/i");
  fTree->Branch("thi",          &fTimeHi,       "thi/i");

  fTree->Branch("fed1",         &fFED1,         "fed1/i");
  fTree->Branch("fed2",         &fFED2,         "fed2/i");

  fTree->Branch("l1t",          &fL1T,          "l1t/i");
  fTree->Branch("l1ta",         fL1TA,          "l1ta[4]/i");
  fTree->Branch("l1tt",         fL1TT,          "l1tt[4]/i");
  fTree->Branch("hlta",         fHLTA,          "hlta[8]/i");

  fTree->Branch("ttA",          fTtA,           "ttA[64]/O");
  fTree->Branch("l1A",          fL1A,           "l1A[128]/O");
  fTree->Branch("hlA",          fHlA,           "hlA[1024]/O");

  fTree->Branch("hlt",          &fHLT,          "hlt/i");

  fTree->Branch("PvN",          &fPvN,          "PvN/I");
  fTree->Branch("PvX",          fPvX,           "PvX[PvN]/F");
  fTree->Branch("PvY",          fPvY,           "PvY[PvN]/F");
  fTree->Branch("PvZ",          fPvZ,           "PvZ[PvN]/F");
  fTree->Branch("PvXe",         fPvXe,          "PvXe[PvN]/F");
  fTree->Branch("PvYe",         fPvYe,          "PvYe[PvN]/F");
  fTree->Branch("PvZe",         fPvZe,          "PvZe[PvN]/F");
  fTree->Branch("PvChi2",       fPvChi2,        "PvChi2[PvN]/F");
  fTree->Branch("PvNdof",       fPvNdof,        "PvNdof[PvN]/F");
  fTree->Branch("PvIsFake",     fPvIsFake,      "PvIsFake[PvN]/I");


  fTree->Branch("MuN",          &fMuN,          "MuN/I");
  fTree->Branch("MuType",       fMuType,        "MuType[MuN]/I");
  fTree->Branch("MuTkI",        fMuTkI,         "MuTkI[MuN]/I");
  fTree->Branch("MuPt",         fMuPt,          "MuPt[MuN]/F");
  fTree->Branch("MuTheta",      fMuTheta,       "MuTheta[MuN]/F");
  fTree->Branch("MuPhi",        fMuPhi,         "MuPhi[MuN]/F");
  fTree->Branch("MuT",          fMuT,           "MuT[MuN]/F");
  fTree->Branch("MuTcorr",      fMuTcorr,       "MuTcorr[MuN]/F");
  fTree->Branch("MuTerr",       fMuTerr,        "MuTerr[MuN]/F");
  fTree->Branch("MuTmean",      &fMuTmean,      "MuTmean/F");
  fTree->Branch("MuTrigger",    &fMuTrigger,    "MuTrigger/I");

  fTree->Branch("HfEplus",      &fEtPlus,       "HfEplus/F");
  fTree->Branch("HfEminus",     &fEtMinus,      "HfEminus/F");
  fTree->Branch("HfTplus",      &fTplus,        "HfTplus/F");
  fTree->Branch("HfTminus",     &fTminus,       "HfTminus/F");

  fTree->Branch("TkN",          &fTkN,          "TkN/I");
  fTree->Branch("TkQuality",    fTkQuality,     "TkQuality[TkN]/I");
  fTree->Branch("TkCharge",     fTkCharge,      "TkCharge[TkN]/I");
  fTree->Branch("TkChi2",       fTkChi2,        "TkChi2[TkN]/F");
  fTree->Branch("TkNdof",       fTkNdof,        "TkNdof[TkN]/F");
  fTree->Branch("TkPt",         fTkPt,          "TkPt[TkN]/F");
  fTree->Branch("TkTheta",      fTkTheta,       "TkTheta[TkN]/F");
  fTree->Branch("TkEta",        fTkEta,         "TkEta[TkN]/F");
  fTree->Branch("TkPhi",        fTkPhi,         "TkPhi[TkN]/F");
  fTree->Branch("TkD0",         fTkD0,          "TkD0[TkN]/F");
  fTree->Branch("TkDz",         fTkDz,          "TkDz[TkN]/F");
  fTree->Branch("TkVx",         fTkVx,          "TkVx[TkN]/F");
  fTree->Branch("TkVy",         fTkVy,          "TkVy[TkN]/F");
  fTree->Branch("TkVz",         fTkVz,          "TkVz[TkN]/F");
  fTree->Branch("TkAlpha",      fTkAlpha,       "TkAlpha[TkN][20]/F");  //FIXME: This should be variable?
  fTree->Branch("TkBeta",       fTkBeta,        "TkBeta[TkN][20]/F");   //FIXME: This should be variable?
  fTree->Branch("TkResX",       fTkResX,        "TkResX[TkN][20]/F");   //FIXME: This should be variable?
  fTree->Branch("TkResY",       fTkResY,        "TkResY[TkN][20]/F");   //FIXME: This should be variable?
  fTree->Branch("TkResXe",      fTkResXe,       "TkResXe[TkN][20]/F");  //FIXME: This should be variable?
  fTree->Branch("TkResYe",      fTkResYe,       "TkResYe[TkN][20]/F");  //FIXME: This should be variable?
  fTree->Branch("TkRes2X",      fTkRes2X,       "TkRes2X[TkN][20]/F");  //FIXME: This should be variable?
  fTree->Branch("TkRes2Xe",     fTkRes2Xe,      "TkRes2Xe[TkN][20]/F"); //FIXME: This should be variable?
  fTree->Branch("TkClN",        fTkClN,         "TkClN[TkN]/I");
  fTree->Branch("TkClI",        fTkClI,         "TkClI[TkN][20]/I");    //FIXME: This should be variable?
  fTree->Branch("TkType",       fTkType,        "TkType[TkN]/I");
  fTree->Branch("TkNHits",      fTkNHits,       "TkNHits[TkN]/I");
  fTree->Branch("TkLHits",      fTkLHits,       "TkLHits[TkN]/I");
  fTree->Branch("TkLHitsI",     fTkLHitsI,      "TkLHitsI[TkN]/I");
  fTree->Branch("TkLHitsO",     fTkLHitsO,      "TkLHitsO[TkN]/I");
  fTree->Branch("TkNHitFr",     fTkNHitFr,      "TkNHitFr[TkN]/F");
  fTree->Branch("TkMuI",        fTkMuI,         "TkMuI[TkN]/I");

  fTree->Branch("ClN",              &fClN,            "ClN/I");
  fTree->Branch("ClRow",            fClRow,           "ClRow[ClN]/F");
  fTree->Branch("ClCol",            fClCol,           "ClCol[ClN]/F");
  fTree->Branch("ClLx",             fClLx,            "ClLx[ClN]/F");
  fTree->Branch("ClLxE",            fClLxe,           "ClLxE[ClN]/F");
  fTree->Branch("ClLy",             fClLy,            "ClLy[ClN]/F");
  fTree->Branch("ClLyE",            fClLye,           "ClLyE[ClN]/F");
  fTree->Branch("ClGx",             fClGx,            "ClGx[ClN]/F");
  fTree->Branch("ClGy",             fClGy,            "ClGy[ClN]/F");
  fTree->Branch("ClGz",             fClGz,            "ClGz[ClN]/F");
  fTree->Branch("ClSize",           fClSize,          "ClSize[ClN]/I");
  fTree->Branch("ClSizeX",          fClSizeX,         "ClSizeX[ClN]/I");
  fTree->Branch("ClSizeY",          fClSizeY,         "ClSizeY[ClN]/I");
  fTree->Branch("ClFlipped",        fClFlipped,       "ClFlipped[ClN]/I");
  fTree->Branch("ClLayer",          fClLayer,         "ClLayer[ClN]/I");
  fTree->Branch("ClLadder",         fClLadder,        "ClLadder[ClN]/I");
  fTree->Branch("ClModule",         fClModule,        "ClModules[ClN]/I");
  fTree->Branch("ClDisk",           fClDisk,          "ClDisk[ClN]/I");
  fTree->Branch("ClBlade",          fClBlade,         "ClBlade[ClN]/I");
  fTree->Branch("ClPanel",          fClPanel,         "ClPanel[ClN]/I");
  fTree->Branch("ClPlaquette",      fClPlaquette,     "ClPlaquette[ClN]/I");
  fTree->Branch("ClDetId",          fClDetId,         "ClDetId[ClN]/I");
  fTree->Branch("ClCharge",         fClCharge,        "ClCharge[ClN]/F");
  fTree->Branch("ClChargeCorr",     fClChargeCorr,    "ClChargeCorr[ClN]/F");
  fTree->Branch("ClType",           fClType,          "ClType[ClN]/I");
  fTree->Branch("ClRhLx",           fClRhLx,          "ClRhLx[ClN]/F");    
  fTree->Branch("ClRhLxE",          fClRhLxE,         "ClRhLxE[ClN]/F");    
  fTree->Branch("ClRhLy",           fClRhLy,          "ClRhLy[ClN]/F");  
  fTree->Branch("ClRhLyE",          fClRhLyE,         "ClRhLyE[ClN]/F");  
  fTree->Branch("ClRhGx",           fClRhGx,          "ClRhGx[ClN]/F");    
  fTree->Branch("ClRhGy",           fClRhGy,          "ClRhGy[ClN]/F");  
  fTree->Branch("ClRhGz",           fClRhGz,          "ClRhGz[ClN]/F");  
  fTree->Branch("ClRhProb",         fClRhProb,        "ClRhProb[ClN]/F");
  fTree->Branch("ClRhProbX",        fClRhProbX,       "ClRhProbX[ClN]/F");
  fTree->Branch("ClRhProbY",        fClRhProbY,       "ClRhProbY[ClN]/F");
  fTree->Branch("ClRhQualWord",     fClRhQualWord,    "ClRhQualWord[ClN]/i");
  fTree->Branch("ClRhqBin",         fClRhqBin,        "ClRhqBin[ClN]/I");
  fTree->Branch("ClRhSpansTwoROCs", fClRhSpansTwoROCs,"ClRhSpansTwoROCs[ClN]/I");
  fTree->Branch("ClRhIsOnEdge"    , fClRhIsOnEdge,    "ClRhIsOnEdge[ClN]/I");
  fTree->Branch("ClRhHasBadPixels", fClRhHasBadPixels,"ClRhHasBadPixels[ClN]/I");
  fTree->Branch("ClTkN",            fClTkN,           "ClTkN[ClN]/I");
  fTree->Branch("ClTkI",            fClTkI,           "ClTkI[ClN][100]/I");
  fTree->Branch("ClDgN",            fClDgN,           "ClDgN[ClN]/I");
  fTree->Branch("ClDgI",            fClDgI,           "ClDgI[ClN][100]/I");    //FIXME: This should be variable?

  // Sim hit info start ==============================================================

  fTree->Branch("ClSimHitN",        fClSimHitN,       "ClSimHitN[ClN]/I");
  fTree->Branch("ClSimHitPID",      fClSimHitPID,     "ClSimHitPID[ClN][10]/I");
  fTree->Branch("ClSimHitPRC",      fClSimHitPRC,     "ClSimHitPRC[ClN][10]/I");
  fTree->Branch("ClSimHitTrkID",    fClSimHitTrkID,   "ClSimHitTrkID[ClN][10]/I");
  fTree->Branch("ClSimHitLx",       fClSimHitLx,      "ClSimHitLx[ClN][10]/F");
  fTree->Branch("ClSimHitLy",       fClSimHitLy,      "ClSimHitLy[ClN][10]/F");
  fTree->Branch("ClSimHitThe",      fClSimHitThe,     "ClSimHitThe[ClN][10]/F");
  fTree->Branch("ClSimHitPhi",      fClSimHitPhi,     "ClSimHitPhi[ClN][10]/F");
  fTree->Branch("ClSimHitMom",      fClSimHitMom,     "ClSimHitMom[ClN][10]/F");

  fTree->Branch("ClSimTrN",        fClSimTrN,       "ClSimTrN[ClN]/I");
  fTree->Branch("ClSimTrID",       fClSimTrID,      "ClSimTrID[ClN][10]/I");
  fTree->Branch("ClSimTrFr",       fClSimTrFr,      "ClSimTrFr[ClN][10]/F");

  fTree->Branch("ClSimTrID2",       fClSimTrID2,      "ClSimTrID2[ClN][10]/I");
  fTree->Branch("ClSimTrType",      fClSimTrType,     "ClSimTrType[ClN][10]/I");
  fTree->Branch("ClSimTrQ",         fClSimTrQ,        "ClSimTrQ[ClN][10]/I");  
  fTree->Branch("ClSimTrPx",        fClSimTrPx,       "ClSimTrPx[ClN][10]/F");  
  fTree->Branch("ClSimTrPy",        fClSimTrPy,       "ClSimTrPy[ClN][10]/F");
  fTree->Branch("ClSimTrPz",        fClSimTrPz,       "ClSimTrPz[ClN][10]/F"); 
  fTree->Branch("ClSimTrEn",        fClSimTrEn,       "ClSimTrEn[ClN][10]/F"); 
  
  fTree->Branch("ClSimTrEta",       fClSimTrEta,      "ClSimTrEta[ClN][10]/F");
  fTree->Branch("ClSimTrPhi",       fClSimTrPhi,      "ClSimTrPhi[ClN][10]/F");
  fTree->Branch("ClSimTrPt",        fClSimTrPt,       "ClSimTrPt[ClN][10]/F");
  
  fTree->Branch("ClSimTrVx",        fClSimTrVx,       "ClSimTrVx[ClN][10]/F");
  fTree->Branch("ClSimTrVy",        fClSimTrVy,       "ClSimTrVy[ClN][10]/F");
  fTree->Branch("ClSimTrVz",        fClSimTrVz,       "ClSimTrVz[ClN][10]/F");

  // Sim hit info end ==============================================================

  fTree->Branch("DgN",          &fDgN,          "DgN/I");
  fTree->Branch("DgRow",        fDgRow,         "DgRow[DgN]/I");
  fTree->Branch("DgCol",        fDgCol,         "DgCol[DgN]/I");
  fTree->Branch("DgDetid",      fDgDetId,       "DgDetId[DgN]/I");
  fTree->Branch("DgRoc",        fDgRoc,         "DgRoc[DgN]/I");
  fTree->Branch("DgRocR",       fDgRocR,        "DgRocR[DgN]/I");
  fTree->Branch("DgRocC",       fDgRocC,        "DgRocC[DgN]/I");
  fTree->Branch("DgLx",         fDgLx,          "DgLx[DgN]/F");
  fTree->Branch("DgLy",         fDgLy,          "DgLy[DgN]/F");
  fTree->Branch("DgGx",         fDgGx,          "DgGx[DgN]/F");
  fTree->Branch("DgGy",         fDgGy,          "DgGy[DgN]/F");
  fTree->Branch("DgGz",         fDgGz,          "DgGz[DgN]/F");
  fTree->Branch("DgAdc",        fDgAdc,         "DgAdc[DgN]/F");
  fTree->Branch("DgCharge",     fDgCharge,      "DgCharge[DgN]/F");
  fTree->Branch("DgClI",        fDgClI,         "DgClI[DgN]/I");      //FIXME: This should be an array?

}


// ----------------------------------------------------------------------
void  PixelTree::beginRun(const Run &run, const EventSetup &iSetup) {
  bool hasChanged;
  fValidHLTConfig = fHltConfig.init(run,iSetup,fHLTProcessName,hasChanged);
}

// ----------------------------------------------------------------------
void PixelTree::endRun(Run const&run, EventSetup const&iSetup) {
  fValidHLTConfig = false;
} 



// ----------------------------------------------------------------------
void PixelTree::analyze(const edm::Event& iEvent, 
			const edm::EventSetup& iSetup)  {

  std::vector<PSimHit> vec_simhits_assoc;
  TrackerHitAssociator *associate(0);
  if (fAccessSimHitInfo) {
    associate = new TrackerHitAssociator(iEvent);
  }
  
  edm::Handle< edm::DetSetVector<PixelDigiSimLink> > pixeldigisimlink;
  if (fAccessSimHitInfo) {
    iEvent.getByLabel("simSiPixelDigis", pixeldigisimlink);
  }

  edm::Handle<SimTrackContainer> simTrackCollection;
  SimTrackContainer simTC;
  if (fAccessSimHitInfo) {
    iEvent.getByLabel("g4SimHits", simTrackCollection);
    simTC = *(simTrackCollection.product());
  }
  
  edm::Handle<SimVertexContainer> simVertexCollection;
  SimVertexContainer simVC;
  if (fAccessSimHitInfo) {
    iEvent.getByLabel("g4SimHits", simVertexCollection);
    simVC = *(simVertexCollection.product());
  }


  static int nevt(0); 
  static unsigned int oldRun(0); 

  int nCl0(0), nCl1(0), nCl2(0), nTk(0); 

  if (oldRun == 0) {
    fL1Thist = new TH1D(Form("L1T_%d", fRun), Form("L1T names for run %d", fRun), 128, 0., 128.); fL1Thist->SetDirectory(fFile); 
    fL1TThist= new TH1D(Form("L1TT_%d",fRun), Form("L1TT names for run %d", fRun), 128, 0., 128.); fL1TThist->SetDirectory(fFile); 
    fHLThist = new TH1D(Form("HLT_%d", fRun), Form("HLT names for run %d", fRun), 10*128, 0., 10*128.); fHLThist->SetDirectory(fFile); 
    oldRun = 1; 
  }

  if (0 == fInit) {
    fInit = 1; 
    // -- Setup cabling map and its map to detIDs
    iSetup.get<SiPixelFedCablingMapRcd>().get(fCablingMap);
    for (int i = 0; i < 40; ++i) {
      fPFC[i] = new SiPixelFrameConverter(fCablingMap.product(), i);
    }
    
    
    edm::ESHandle<TrackerGeometry> pDD;
    iSetup.get<TrackerDigiGeometryRecord>().get(pDD);

    for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){
      if(dynamic_cast<PixelGeomDetUnit*>((*it))!=0){
	DetId detId = (*it)->geographicalId();
	uint32_t newDetId = detId;
	
	for (int fedid = 0; fedid < 40; ++fedid) {
	  if (fPFC[fedid]->hasDetUnit(newDetId)) {
	    fFEDID.insert(make_pair(newDetId, fedid)); 
	    break;
	  }
	}
      }
    }

  }

  ++nevt; 
  init();

  // -- Does this belong into beginJob()?
  ESHandle<TrackerGeometry> TG;
  iSetup.get<TrackerDigiGeometryRecord>().get(TG);
  const TrackerGeometry* theTrackerGeometry = TG.product();
  const TrackerGeometry& theTracker(*theTrackerGeometry);

  // ----------------------------------------------------------------------
  // -- Event information
  // ----------------------------------------------------------------------
  fRun       = iEvent.id().run();
  fLumiBlock = iEvent.luminosityBlock();
  fEvent     = iEvent.id().event();

  if (fRun != oldRun) {
    fL1Thist = new TH1D(Form("L1T_%d", fRun), Form("L1T names for run %d", fRun), 2*128, 0., 2*128.); fL1Thist->SetDirectory(fFile); 
    fL1TThist= new TH1D(Form("L1TT_%d",fRun), Form("L1TT names for run %d", fRun), 2*128, 0., 2*128.); fL1TThist->SetDirectory(fFile); 
    fHLThist = new TH1D(Form("HLT_%d", fRun), Form("HLT names for run %d", fRun), 10*128, 0., 10*128.); fHLThist->SetDirectory(fFile); 
    oldRun = fRun; 
  }

  const edm::Timestamp timeStamp = iEvent.time();
  unsigned int high = timeStamp.value() >> 32;       // seconds
  unsigned int low = 0xffffffff & timeStamp.value(); // microseconds  

  fTimeLo    = low;
  fTimeHi    = high;

  fBX        = iEvent.bunchCrossing();
  fOrbit     = iEvent.orbitNumber();

  // -- Magnetic field
  ESHandle<MagneticField> MF;
  iSetup.get<IdealMagneticFieldRecord>().get(MF);
  const MagneticField* theMagneticField = MF.product();
  fBz    = fabs(theMagneticField->inTesla(GlobalPoint(0,0,0)).z());

  // -- FED
  edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("RunInfoRcd"));
  uint32_t nbpixfeds(0);
  uint32_t nfpixfeds(0);
  if (recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
    //record not found
    cout << "Record \"RunInfoRcd" << "\" does not exist " << endl;
    fFED1 = 0; 
    fFED2 = (0x1 << 12) ; 
  } else {
    edm::ESHandle<RunInfo> runInfoHandle;
    iSetup.get<RunInfoRcd>().get(runInfoHandle);
    const RunInfo *runInfo = runInfoHandle.product();
    unsigned int totbpixfeds(32), totfpixfeds(8); 
    
    std::vector<int> feds = runInfo->m_fed_in;
    uint32_t ifed(0); 
    for (size_t fedindex = 0; fedindex < feds.size(); fedindex++) {
      //      cout << "FED " << fedindex << ": " << (uint32_t)feds[fedindex] << endl;
      ifed = (uint32_t)feds[fedindex]; 
      if (ifed >= (totbpixfeds + totfpixfeds))	   continue;
      
      if (ifed < totbpixfeds) {
	++nbpixfeds;
	fFED1 |= (0x1 << ifed); 
      } else if (ifed < (totbpixfeds + totfpixfeds)) {
	++nfpixfeds;
	ifed -= 32; 
	fFED2 |= (0x1 << ifed); 
      }
    }

    if (nbpixfeds == totbpixfeds) fFED2 |= (0x1 << 10); 
    if (nfpixfeds == totfpixfeds) fFED2 |= (0x1 << 11); 

    //     cout << "number of feds in run: BPIX " << nbpixfeds << ", FPIX " << nfpixfeds 
    //      	 << " fFED1 = " << std::bitset<32>(fFED1)
    //        	 << " fFED2 = " << std::bitset<32>(fFED2)
    //        	 << endl;
  } 

  // ----------------------------------------------------------------------
  // -- Trigger information
  // ----------------------------------------------------------------------

  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  iEvent.getByLabel(fL1GTReadoutRecordLabel,L1GTRR);
  Handle<L1GlobalTriggerObjectMapRecord> hL1GTmap; 
  iEvent.getByLabel("hltL1GtObjectMap", hL1GTmap);

  L1GtUtils l1GtUtils;
  l1GtUtils.retrieveL1EventSetup(iSetup);
  static int first(1);
  if (first) {
    first = 0; 
    cout << "L1 trigger menu: ";
    cout << l1GtUtils.l1TriggerMenu() << endl;
  }

  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  string algoname; 
  int    algobit(-1); 
  bool   result(false); 
  int    iErrorCode(0);
  bool   unmaskedResult(false);

  for (CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
    algoname = (algo->second).algoName();
    algobit  = (algo->second).algoBitNumber();
    result   = l1GtUtils.decisionAfterMask(iEvent, algoname, iErrorCode);
    unmaskedResult = l1GtUtils.decisionBeforeMask(iEvent, algoname, iErrorCode);

    fL1Thist->GetXaxis()->SetBinLabel(algobit+1, algoname.c_str());
    fL1A[algobit]   = result;
    if(unmaskedResult) {
      int offset = algobit/32; 
      int index  = algobit%32; 
      if( offset<4 && index<32 ) fL1TA[offset] |= ( 0x1 << index );
      //cout<<algoname<<" "<<algobit<<" "<<result<<" "<<mask<<" "<<prescale<< " "<<unmaskedResult
      //  <<" "<<hex<<fL1TA[offset]<<dec<<" "<<offset<<" "<<index<<endl;
    }
  }

  for (CItAlgo algo = menu->gtTechnicalTriggerMap().begin(); algo != menu->gtTechnicalTriggerMap().end(); ++algo) {
    algoname = (algo->second).algoName();
    algobit  = (algo->second).algoBitNumber();
    result   = l1GtUtils.decisionAfterMask(iEvent, algoname, iErrorCode);
    unmaskedResult = l1GtUtils.decisionBeforeMask(iEvent, algoname, iErrorCode);
    fL1TThist->GetXaxis()->SetBinLabel(algobit+1, algoname.c_str());
    fTtA[algobit] = result; 
    if(unmaskedResult) {
      int offset = algobit/32; 
      int index  = algobit%32; 
      if( offset<4 && index<32 ) fL1TT[offset] |= ( 0x1 << index );
      //cout<<algoname<<" "<<algobit<<" "<<result<<" "<<mask<<" "<<prescale<< " "<<unmaskedResult
      //  <<" "<<hex<<fL1TT[offset]<<dec<<" "<<offset<<" "<<index<<endl;
    }
  }

  // -- Read HLT configuration and names
  vector<string> validTriggerNames;
  if (fValidHLTConfig) validTriggerNames = fHltConfig.triggerNames();
  else cout << "==> PixelTree: No valid Trigger configuration!!!" << endl;

  if (validTriggerNames.size() < 1) {
    cout << "==>PixelTree: NO valid trigger names returned by HLT config provided!!??" << endl;
  } else {

    
    Handle<TriggerResults> hHLTresults;
    bool hltF = true;
    try {
      iEvent.getByLabel(fHLTResultsLabel, hHLTresults);
    } catch (...) {
      if (fVerbose > 0) cout << "==>HFDumpTrigger> Triggerresults  " << fHLTResultsLabel.encode() << " not found " << endl;
      hltF = false;
    }
  
    if (hltF) {
      const TriggerNames &trigName = iEvent.triggerNames(*hHLTresults);
      unsigned int index(999); 
      bool result(false);
      for (unsigned int it = 0; it < validTriggerNames.size(); ++it) {
	index    = trigName.triggerIndex(validTriggerNames[it]); 
	result   = (index < validTriggerNames.size() && hHLTresults->accept(index));

	fHLThist->GetXaxis()->SetBinLabel(index+1, validTriggerNames[it].c_str()); 
	fHlA[index] = result; 
      }
    }

  }



  // ----------------------------------------------------------------------
  // -- Primary vertices
  // ----------------------------------------------------------------------
  edm::Handle<reco::VertexCollection> hVertices;
  bool goodVertices(false);
  if ("nada" != fPrimaryVertexCollectionLabel.label()) {
    try{ 
      iEvent.getByLabel(fPrimaryVertexCollectionLabel, hVertices);
      goodVertices = true; 
    } catch (...) {
      cout << "No Vertex collection with label " << fPrimaryVertexCollectionLabel << endl;
    }
  }

  if (goodVertices) {
    for (reco::VertexCollection::const_iterator iv = hVertices->begin(); iv != hVertices->end(); ++iv) {
      if (fPvN > PVMAX - 1) break;
      fPvX[fPvN] = iv->x();
      fPvY[fPvN] = iv->y();
      fPvZ[fPvN] = iv->z();
      
      fPvXe[fPvN] = iv->xError();
      fPvYe[fPvN] = iv->yError();
      fPvZe[fPvN] = iv->zError();
      
      fPvChi2[fPvN] = iv->chi2(); 
      fPvNdof[fPvN] = iv->ndof(); 
      
      fPvIsFake[fPvN] = (iv->isFake()?1:0); 
      
      ++fPvN; 
    }
  } else {
    cout << "No goodVertices with label " << fPrimaryVertexCollectionLabel << endl;
  }

  // ----------------------------------------------------------------------
  // -- Fill muons
  // ----------------------------------------------------------------------
  edm::Handle<MuonCollection> hMuonCollection;
  try{ 
    iEvent.getByLabel(fMuonCollectionLabel, hMuonCollection);
  } catch (cms::Exception &ex) {
    cout << "No Muon collection with label " << fMuonCollectionLabel << endl;
  }

  // -- muon trigger information (from DQM/HcalMonitorTasks/src/HcalDetDiagTimingMonitor.cc)
  static const int MAXRPC =20;
  static const int MAXDTBX=20;
  static const int MAXCSC =20;    
  static const int TRIG_DT =1;
  static const int TRIG_RPC=2;
  static const int TRIG_CSC=4;
  static const int TRIG_RPCF=8;
  
  edm::Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
  bool goodL1Muons(false);
  try{ 
    iEvent.getByLabel(fL1MuGMTLabel, gmtrc_handle);
    goodL1Muons = gmtrc_handle.isValid();
  } catch (cms::Exception &ex) {
    goodL1Muons = false;
    cout << " HELP caught exception, no L1MuGMTReadoutCollection with label " << fL1MuGMTLabel << " found " << endl;
  }

  fMuTrigger = 0; 
  if(goodL1Muons) {
    L1MuGMTReadoutCollection const* gmtrc = gmtrc_handle.product();
  
    int idt   =0;
    int icsc  =0;
    int irpcb =0;
    int irpcf =0;
    int ndt[5]   = {0,0,0,0,0};
    int ncsc[5]  = {0,0,0,0,0};
    int nrpcb[5] = {0,0,0,0,0};
    int nrpcf[5] = {0,0,0,0,0};
    int N;	
    std::vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
    std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
    N=0;
    int NN=0;
    int BXinEVENT=1,TRIGGER=0;
    for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {
      if(igmtrr->getBxInEvent()==0) BXinEVENT=NN;
      NN++;
      std::vector<L1MuRegionalCand>::const_iterator iter1;
      std::vector<L1MuRegionalCand> rmc;
      // DTBX Trigger
      rmc = igmtrr->getDTBXCands(); 
      for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
	if ( idt < MAXDTBX && !(*iter1).empty() ) {
	  idt++; 
	  if(N<5) ndt[N]++; 
	  
	} 	 
      }
      // CSC Trigger
      rmc = igmtrr->getCSCCands(); 
      for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
	if ( icsc < MAXCSC && !(*iter1).empty() ) {
	  icsc++; 
	  if(N<5) ncsc[N]++; 
	} 
      }
      // RPCb Trigger
      rmc = igmtrr->getBrlRPCCands();
      for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
	if ( irpcb < MAXRPC && !(*iter1).empty() ) {
	  irpcb++;
	  if(N<5) nrpcb[N]++;
	  
	}  
      }
      // RPCfwd Trigger
      rmc = igmtrr->getFwdRPCCands();
      for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
	if ( irpcf < MAXRPC && !(*iter1).empty() ) {
	  irpcf++;
	  if(N<5) nrpcf[N]++;
	  
	}  
      }
      
      N++;
    }
    if(ncsc[BXinEVENT]>0 ) {   TRIGGER=+TRIG_CSC;  }
    if(ndt[BXinEVENT]>0  ) {   TRIGGER=+TRIG_DT;   }
    if(nrpcb[BXinEVENT]>0) {   TRIGGER=+TRIG_RPC;  }
    if(nrpcf[BXinEVENT]>0) {   TRIGGER=+TRIG_RPCF; }

    fMuTrigger = TRIGGER;
  }
  


  vector<int> muTkRef, muMuRef;
  if (hMuonCollection.isValid()) {
    int idx(0); 
    for (MuonCollection::const_iterator it = hMuonCollection->begin(), itEnd = hMuonCollection->end(); it != itEnd; ++it){
      if (fMuN > MUMAX - 1) break;

      // -- Get reference to track
      if ((it->globalTrack()).isNull()) {
	//	cout << " bad muon globaltrack" << endl;
	continue;
      }
      if ((it->track()).isNull()) {
	//	cout << " bad muon track" << endl;
 	idx = -1;
      } else {
	idx = (it->track()).index();
      }
      muTkRef.push_back(idx);
      muMuRef.push_back(fMuN);
      
      int type = 0; 
      if (it->isGlobalMuon()) type &= 0x1; 
      if (it->isStandAloneMuon()) type &= 0x2; 
      if (it->isTrackerMuon()) type &= 0x4; 
      if (it->isTimeValid()) type &= 0x8; 
      fMuType[fMuN]  = type;
      fMuT[fMuN]     = it->time().timeAtIpInOut;
      fMuTcorr[fMuN] = 0; 
      fMuTerr[fMuN]  = it->time().timeAtIpInOutErr;

      fMuPt[fMuN]    =  it->globalTrack()->pt();
      fMuPhi[fMuN]   =  it->globalTrack()->phi();
      fMuTheta[fMuN] =  it->globalTrack()->theta();

      ++fMuN;
    }
    fMuTmean = 0;
  }


  //---------------------------------------------------------------------
  //---Fill HF information (code originally written  by D.Marlow)
  //---------------------------------------------------------------------
  if (0) {
    Handle<HFRecHitCollection> hHf;
    if (!iEvent.getByLabel("hfreco", hHf)) {
      if (fVerbose > 0) cout << "Could not get rec hits! Tried with label: hfreco" << endl;
    }
    
    ESHandle<CaloGeometry> caloGeometry ;
    iSetup.get<CaloGeometryRecord>().get(caloGeometry);
    
    int nHits = 0;
    double eSum = 0.;
    double ePlus = 0;
    double EtPlus = 0.;
    double tPlus = 0.;
    double eMinus = 0.;
    float EtMinus = 0.;
    double tMinus = 0.;
    
    if (hHf.isValid()) {
      for (unsigned int i = 0; i < hHf->size(); ++i) {
	double energy = (*hHf)[i].energy();
	double time = (*hHf)[i].time();
	HcalDetId cell((*hHf)[i].id());
	
	const CaloCellGeometry* cellGeometry = caloGeometry->getSubdetectorGeometry(cell)->getGeometry (cell);
	if (cellGeometry == 0) cout << "No cell geometry " << cell.rawId() << endl;
	double fEta = cellGeometry->getPosition().eta();
	double fTheta = cellGeometry->getPosition().theta();
	
	if (energy > 1.) { nHits++;}
	eSum += energy;
	if (fEta > 0.) {
	  if (energy > 0.) {
	    ePlus += energy;
	    tPlus += energy*time;
	  }
	  EtPlus += energy*sin(fTheta);
	} else {
	  if (energy > 0.) {
	    eMinus += energy;
	    tMinus += energy*time;
	  }
	  EtMinus += energy*sin(fTheta);
	}
      }
      if (ePlus > 0.) {tPlus = tPlus/ePlus;}
      if (eMinus > 0.) {tMinus = tMinus/eMinus;}
      fEtPlus  = EtPlus;
      fEtMinus = EtMinus;
      fTplus   = tPlus;
      fTminus  = tMinus;
    }
  }

  // ----------------------------------------------------------------------
  // -- Fill tracks
  // ----------------------------------------------------------------------


  // -- Pixel cluster
  edm::Handle< edmNew::DetSetVector<SiPixelCluster> > hClusterColl;
  iEvent.getByLabel(fPixelClusterLabel, hClusterColl);
  const edmNew::DetSetVector<SiPixelCluster> clustColl = *(hClusterColl.product());

  // -- Pixel RecHit
  edm::Handle<SiPixelRecHitCollection> hRecHitColl;
  iEvent.getByLabel(fPixelRecHitLabel, hRecHitColl); 
  //  cout << "pixel cluster collection size: " << clustColl.size() << endl;

  // -- Tracks
  edm::Handle<std::vector<reco::Track> > hTrackCollection;
  try {
    iEvent.getByLabel(fTrackCollectionLabel, hTrackCollection);
  } catch (cms::Exception &ex) {
    if (fVerbose > 1) cout << "No Track collection with label " << fTrackCollectionLabel << endl;
  }
  if (hTrackCollection.isValid()) {
    const std::vector<reco::Track> trackColl = *(hTrackCollection.product());
    nTk = trackColl.size();
    if (fVerbose > 1) cout << "--> Track collection size: " << nTk << endl;
  } else {
    if (fVerbose > 1) cout << "--> No valid track collection" << endl;
  }

  if (nTk > 0) {


    // -- Track trajectory association map
    edm::Handle<TrajTrackAssociationCollection> hTTAC;
    iEvent.getByLabel(fTrajectoryInputLabel, hTTAC);
    if (fVerbose > 1) cout << "===========> trajectory collection size: " << hTTAC->size() << endl;
    
    TrajectoryStateCombiner tsoscomb;
    if (hTTAC.isValid()) {
      const TrajTrackAssociationCollection ttac = *(hTTAC.product());
      if (fVerbose > 1) cout << "   hTTAC.isValid()" << endl;
      for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {
	if (fTkN > TRACKMAX - 1) break;
	
	if (fVerbose > 1) cout << "      TracjTrackAssociationCollection iterating" << endl;
	const edm::Ref<std::vector<Trajectory> > refTraj = it->key;  
	reco::TrackRef trackref = it->val;
	
	// -- Check whether it is a pixel track
	bool isBpixTrack(false), isFpixTrack(false); 
	isPixelTrack(refTraj, isBpixTrack, isFpixTrack);
	if (!isBpixTrack && !isFpixTrack) {
	  if (fVerbose > 1) cout << "  not a pixel track" << endl;
	  continue; 
	} 
	
	//      cout << "track index = " << trackref.index() << endl;
	
	fTkQuality[fTkN]= trackref->qualityMask(); // see: CMSSW/DataFormats/TrackReco/interface/TrackBase.h
	fTkCharge[fTkN] = trackref->charge();
	fTkChi2[fTkN]   = trackref->chi2();
	fTkNdof[fTkN]   = trackref->ndof();
	fTkPt[fTkN]     = trackref->pt();
	fTkTheta[fTkN]  = trackref->theta();
	fTkEta[fTkN]    = trackref->eta();
	fTkPhi[fTkN]    = trackref->phi();
	fTkD0[fTkN]     = trackref->d0();
	fTkDz[fTkN]     = trackref->dz();
	fTkVx[fTkN]     = trackref->vx();
	fTkVy[fTkN]     = trackref->vy();
	fTkVz[fTkN]     = trackref->vz();
	fTkNHits[fTkN]  = trackref->hitPattern().numberOfValidHits();
	fTkLHits[fTkN]  = trackref->hitPattern().numberOfLostHits();
	fTkLHitsI[fTkN] = trackref->trackerExpectedHitsInner().numberOfLostTrackerHits();
	fTkLHitsO[fTkN] = trackref->trackerExpectedHitsOuter().numberOfLostTrackerHits();
	fTkNHitFr[fTkN] = -1;
	fTkType[fTkN]   = 1;
	fTkMuI[fTkN]    = -1;
	if ((fTkNHits[fTkN] + fTkLHits[fTkN] + fTkLHitsI[fTkN] + fTkLHitsO[fTkN]) > 0) {
	  fTkNHitFr[fTkN] = fTkNHits[fTkN]/(fTkNHits[fTkN] + fTkLHits[fTkN] + fTkLHitsI[fTkN] + fTkLHitsO[fTkN]);
	}
	
	// ----------------------------------------------------------------------
	// -- Clusters associated with a track
	std::vector<TrajectoryMeasurement> tmeasColl =refTraj->measurements();
	int iCluster(0); 
	for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = tmeasColl.begin(); tmeasIt!=tmeasColl.end(); tmeasIt++){   
	  if (!tmeasIt->updatedState().isValid()) continue; 
	  TrajectoryStateOnSurface tsos = tsoscomb(tmeasIt->forwardPredictedState(), tmeasIt->backwardPredictedState());
	  TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt->recHit();
	  
	  if (hit->geographicalId().det() != DetId::Tracker) {
	    continue; 
	  } 
	  
	  const GeomDetUnit* detUnit = hit->detUnit();
	  if (0 == detUnit) {
	    if (fVerbose > 2) cout << "-- PixelTree> Did not find hit->detUnit()" << endl;
	    continue;
	  }
	  
	  bool barrel = DetId(hit->geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
	  bool endcap = DetId(hit->geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
	  if (!barrel && !endcap) {
	    continue;
	  }
	  
	  const Surface &surface = hit->detUnit()->surface();
	  LocalPoint lPModule(0.,0.,0.), lPhiDirection(1.,0.,0.), lROrZDirection(0.,1.,0.);
	  GlobalPoint gPModule       = surface.toGlobal(lPModule),
	    gPhiDirection  = surface.toGlobal(lPhiDirection),
	    gROrZDirection = surface.toGlobal(lROrZDirection);
	  
	  
	  float phiorientation = deltaPhi(gPhiDirection.phi(), gPModule.phi()) >= 0 ? +1. : -1.;
	  
	  LocalTrajectoryParameters ltp = tsos.localParameters();
	  LocalVector localDir = ltp.momentum()/ltp.momentum().mag();
	  
	  float alpha = atan2(localDir.z(), localDir.x());
	  float beta = atan2(localDir.z(), localDir.y());
	  
	  int DBlayer, DBladder, DBmodule, DBdisk, DBblade, DBpanel, DBdetid; 
	  DBdetid = hit->geographicalId().rawId();
	  if (barrel) {
	    bpixNames(DetId((*hit).geographicalId()), DBlayer, DBladder, DBmodule); 
	  } else {
	    fpixNames(DetId((*hit).geographicalId()), DBdisk, DBblade, DBpanel, DBmodule); 
	  }
	  
	  const DetId &hit_detId = hit->geographicalId();
	  uint IntSubDetID = (hit_detId.subdetId());
	  if (IntSubDetID == 0) continue;
	  //	const TrackerGeometry &theTracker(*theTrackerGeometry); //?????????????????????
	  const PixelGeomDetUnit *theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(hit_detId) );
	  if (theGeomDet == 0) {
	    continue; 	  // skip strip modules
	  }
	  
	  // Flipped module?
	  float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
	  float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
	  int isFlipped(0); 
	  if (tmp2 < tmp1) isFlipped = 1;
	  else isFlipped = 0;
	  
	  // -- Missing hits
	  if (hit->getType() == TrackingRecHit::missing) {
	    
	    LocalPoint mhPos = tsos.localPosition();
	    float xcenter = mhPos.x();
	    float ycenter = mhPos.y();
	    const PixelTopology * topol = &(theGeomDet->specificTopology());
	    LocalPoint clustlp = topol->localPosition( MeasurementPoint(xcenter, ycenter) );
	    GlobalPoint clustgp = theGeomDet->surface().toGlobal( clustlp );
	    
	    fClLx[fClN]      = clustlp.x();
	    fClLy[fClN]      = clustlp.y();
	    
	    fClGx[fClN]      = clustgp.x();
	    fClGy[fClN]      = clustgp.y();
	    fClGz[fClN]      = clustgp.z();
	    
	    fClFlipped[fClN] = isFlipped;
	    
	    fClSize[fClN]   = -99;
	    fClSizeX[fClN]  = -99;
	    fClSizeY[fClN]  = -99;
	    fClRow[fClN]    = -99;
	    fClCol[fClN]    = -99;
	    
	    fClCharge[fClN]     = -99.;
	    fClChargeCorr[fClN] = -99.;
	    
	    if (barrel) {
	      fClLayer[fClN]     = DBlayer;
	      fClLadder[fClN]    = DBladder; 
	      fClModule[fClN]    = DBmodule;
	      fClDisk[fClN]      = -99;
	      fClBlade[fClN]     = -99;
	      fClPanel[fClN]     = -99;
	      fClPlaquette[fClN] = -99;
	    } 
	    
	    if (endcap) {
	      fClLayer[fClN]     = -99;
	      fClLadder[fClN]    = -99; 
	      fClModule[fClN]    = -99;
	      fClDisk[fClN]      =  DBdisk;
	      fClBlade[fClN]     =  DBblade;
	      fClPanel[fClN]     =  DBpanel;
	      fClPlaquette[fClN] =  DBmodule;
	    }
	    
	    fClDetId[fClN]   = DBdetid;
	    fClType[fClN]    = 0; 
	    
	    fClRhLx[fClN]           = clustlp.x();
	    //	  fClRhLxE[fClN]          = TMath::Sqrt(hit->localPositionError().xx());
	    fClRhLy[fClN]           = clustlp.y();
	    //	  fClRhLyE[fClN]          = TMath::Sqrt(hit->localPositionError().yy());
	    fClRhGx[fClN]           = clustgp.x();
	    fClRhGy[fClN]           = clustgp.y();
	    fClRhGz[fClN]           = clustgp.z();
	    
	    fClRhProb[fClN]         = -9999;
	    fClRhProbX[fClN]        = -9999;
	    fClRhProbY[fClN]        = -9999;
	    fClRhQualWord[fClN]     = 9999;
	    fClRhqBin[fClN]         = -9999;
	    fClRhSpansTwoROCs[fClN] = -9999;
	    fClRhIsOnEdge[fClN]     = -9999;
	    fClRhHasBadPixels[fClN] = -9999;
	    
	    int it(0); 
	    while (fClTkI[fClN][it] > -1 && it < TKPERCLMAX) {
	      ++it;
	    }
	    fClTkI[fClN][it] = fTkN;
	    fClTkN[fClN]    += 1;
	    
	    fClDgN[fClN]     = 0;
	    
	    if (iCluster < CLPERTRACKMAX) {
	      fTkClI[fTkN][iCluster]  = fClN;
	      fTkClN[fTkN] = iCluster+1;
	    }  
	    
	    fTkAlpha[fTkN][iCluster] = alpha;
	    fTkBeta[fTkN][iCluster] = beta;
	    
	    ++nCl0;
	    
	    ++fClN;
	    ++iCluster;
	    continue;
	  } // -- end missing hits
	  
	  const TrackingRecHit *persistentHit = hit->hit();
	  if ( persistentHit != 0 ) 
	    {
	      if ( typeid(*persistentHit) != typeid(SiPixelRecHit) )  
		continue;
	    }
	  
	  const SiPixelRecHit *pixhit = dynamic_cast<const SiPixelRecHit*>(hit->hit());
	  
	  
	  
	  // gavril : begin associate and store simhit info
	  
	  if ( fAccessSimHitInfo )
	    {
	      vec_simhits_assoc.clear();
	      vec_simhits_assoc = associate->associateHit(*pixhit);
	      
	      fClSimHitN[fClN] = (int)vec_simhits_assoc.size();
	      int iSimHit = 0;
	      
	      for (std::vector<PSimHit>::const_iterator m = vec_simhits_assoc.begin(); 
		   m < vec_simhits_assoc.end() && iSimHit < SIMHITPERCLMAX; ++m) 
		{
		  
		  fClSimHitPID[fClN][iSimHit]   = (int)( m->particleType() );
		  fClSimHitPRC[fClN][iSimHit]   = (int)( m->processType()  );
		  fClSimHitTrkID[fClN][iSimHit] = (int)( m->trackId()      );
		  
		  fClSimHitLx[fClN][iSimHit]    = ( m->entryPoint().x() + m->exitPoint().x() ) / 2.0;
		  fClSimHitLy[fClN][iSimHit]    = ( m->entryPoint().y() + m->exitPoint().y() ) / 2.0;
		  
		  fClSimHitThe[fClN][iSimHit]   =  m->thetaAtEntry();
		  fClSimHitPhi[fClN][iSimHit]   =  m->phiAtEntry();
		  fClSimHitMom[fClN][iSimHit]   =  m->pabs();
		  
		  ++iSimHit;
		  
		} // end sim hit loop
	      
	    } // 	if ( fAccessSimHitInfo )
	  
	  // gavril : end: associate and store simhit info
	  
	  
	  edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = (*pixhit).cluster();
	  if ( clust.isNonnull() ) 
	    { 
	      
	      // gavril : begin : access sim tracks associated with current cluster
	      if ( fAccessSimHitInfo )
		{
		  int minPixelRow = (*clust).minPixelRow();
		  int maxPixelRow = (*clust).maxPixelRow();
		  int minPixelCol = (*clust).minPixelCol();
		  int maxPixelCol = (*clust).maxPixelCol();
		  
		  edm::DetSetVector<PixelDigiSimLink>::const_iterator isearch = pixeldigisimlink->find( DBdetid );
		  edm::DetSet<PixelDigiSimLink> digiLink = (*isearch);
		  
		  edm::DetSet<PixelDigiSimLink>::const_iterator linkiter = digiLink.data.begin();
		  //create a vector for the track ids in the digisimlinks
		  
		  std::vector<int>   simTrackIdV;      
		  std::vector<float> simTrackFrV;   
		  
		  std::vector<int>   simTrackId2V; 
		  std::vector<int>   simTrackTypeV;
		  std::vector<int>   simTrackQV;   
		  std::vector<float> simTrackPxV;  
		  std::vector<float> simTrackPyV;  
		  std::vector<float> simTrackPzV;  
		  std::vector<float> simTrackEnV;  
		  
		  std::vector<float> simTrackEtaV; 
		  std::vector<float> simTrackPhiV; 
		  std::vector<float> simTrackPtV;  
		  
		  std::vector<float> simTrackVxV;  
		  std::vector<float> simTrackVyV;  
		  std::vector<float> simTrackVzV;  
		  
		  simTrackIdV.clear();
		  simTrackFrV.clear();  
		  
		  simTrackId2V.clear(); 
		  simTrackTypeV.clear();
		  simTrackQV.clear();   
		  simTrackPxV.clear();  
		  simTrackPyV.clear();  
		  simTrackPzV.clear();  
		  simTrackEnV.clear();  
		  
		  simTrackEtaV.clear(); 
		  simTrackPhiV.clear(); 
		  simTrackPtV.clear();  
		  
		  simTrackVxV.clear();  
		  simTrackVyV.clear();  
		  simTrackVzV.clear();  
		  
		  for( ; linkiter != digiLink.data.end(); linkiter++) 
		    { // loop over all digisimlinks
		      
		      std::pair<int,int> pixel_coord = PixelDigi::channelToPixel(linkiter->channel());
		      
		      // is the digisimlink inside the cluster boundaries?
		      if ( pixel_coord.first  <= maxPixelRow &&
			   pixel_coord.first  >= minPixelRow &&
			   pixel_coord.second <= maxPixelCol &&
			   pixel_coord.second >= minPixelCol ) 
			{
			  
			  bool inStock(false); // did we see this simTrackId before? - We will check later
			  
			  std::vector<int>::const_iterator sTIter = simTrackIdV.begin();
			  for ( ; sTIter < simTrackIdV.end(); sTIter++) 
			    {
			      if ( (*sTIter) == (int)linkiter->SimTrackId() ) 
				{
				  inStock=true; // now we saw this id before
				}
			    }
			  
			  
			  if ( !inStock ) 
			    {
			      
			      int simtrkid = linkiter->SimTrackId();      			      
			      
			      for (SimTrackContainer::const_iterator simTrack=simTC.begin(); simTrack!=simTC.end(); simTrack++)
				{
				  
				  if ( (int)simTrack->trackId() == simtrkid )
				    {			
				      
				      simTrackIdV.push_back(   linkiter->SimTrackId()    ); // add the track id to the vector
				      simTrackFrV.push_back(   linkiter->fraction()      ); 
				      
				      simTrackId2V.push_back(  (int)simTrack->trackId()  );
				      simTrackTypeV.push_back( simTrack->type()          ); 
				      simTrackQV.push_back(    simTrack->charge()        );
				      simTrackPxV.push_back(   simTrack->momentum().x()  );
				      simTrackPyV.push_back(   simTrack->momentum().y()  );
				      simTrackPzV.push_back(   simTrack->momentum().z()  );
				      simTrackEnV.push_back(   simTrack->momentum().t()  );
				      
				      simTrackEtaV.push_back(  simTrack->momentum().eta()             );
				      simTrackPhiV.push_back(  simTrack->momentum().phi()             );
				      simTrackPtV.push_back(   simTrack->momentum().pt()              );
				      
				      simTrackVxV.push_back(   simVC[simTrack->vertIndex()].position().x() );
				      simTrackVyV.push_back(   simVC[simTrack->vertIndex()].position().y() );
				      simTrackVzV.push_back(   simVC[simTrack->vertIndex()].position().z() );
				      
				      break;
				      
				    } // if ( (int)simTrack->trackId() == simtrkid )
				  
				} // for (SimTrackContainer::const_iterator simTrack=simTC.begin(); simTrack!=simTC.end(); simTrack++)
			      
			      // gavril : check this !!!
			      //if ( !found_sim_trk )
			      //cout << "Din not find sim tracks with ID = " << simtrkid << "   !!!!!!!!!!!!!!!!!!!!!!!" << endl; 
			      
			    } // if ( !inStock )
			  
			} // if ( pixel_coord.first  <= maxPixelRow && ... )
		      
		    } // for( ; linkiter != digiLink.data.end(); linkiter++) 
		  
		  
		  fClSimTrN[fClN] = (int)simTrackIdV.size();
		  
		  for ( int iSimTrk = 0; iSimTrk < fClSimTrN[fClN]; iSimTrk++  )
		    {
		      fClSimTrID[fClN][iSimTrk] = simTrackIdV[iSimTrk]; 
		      fClSimTrFr[fClN][iSimTrk] = simTrackFrV[iSimTrk];
		      
		      fClSimTrID2[fClN][iSimTrk]       = simTrackId2V[iSimTrk];
		      fClSimTrType[fClN][iSimTrk]      = simTrackTypeV[iSimTrk];
		      fClSimTrQ[fClN][iSimTrk]         = simTrackQV[iSimTrk];
		      fClSimTrPx[fClN][iSimTrk]        = simTrackPxV[iSimTrk];
		      fClSimTrPy[fClN][iSimTrk]        = simTrackPyV[iSimTrk];
		      fClSimTrPz[fClN][iSimTrk]        = simTrackPzV[iSimTrk];
		      fClSimTrEn[fClN][iSimTrk]        = simTrackEnV[iSimTrk];
		      
		      fClSimTrEta[fClN][iSimTrk]       = simTrackEtaV[iSimTrk];
		      fClSimTrPhi[fClN][iSimTrk]       = simTrackPhiV[iSimTrk];
		      fClSimTrPt[fClN][iSimTrk]        = simTrackPtV[iSimTrk];
		      
		      fClSimTrVx[fClN][iSimTrk]   = simTrackVxV[iSimTrk];
		      fClSimTrVy[fClN][iSimTrk]   = simTrackVyV[iSimTrk];
		      fClSimTrVz[fClN][iSimTrk]   = simTrackVzV[iSimTrk];
		      
		    }
		  
		} //  if ( fAccessSimHitInfo )
	      
	      // gavril : end : access sim tracks associated with current cluster
	      
	      
	      
	      
	      align::LocalVector res = tsos.localPosition() - hit->localPosition();
	      LocalError err1 = tsos.localError().positionError();
	      LocalError err2 = hit->localPositionError();
	      float errX = std::sqrt( err1.xx() + err2.xx() );
	      float errY = std::sqrt( err1.yy() + err2.yy() );
	      float resy(0.);
	      double dZ = gROrZDirection.z() - gPModule.z();
	      if (dZ > -999.) resy = res.y() * (dZ >=0.? +1 : -1) ;
	      else resy = res.y();
	      
	      if (fVerbose > 1) cout << "cluster at "
				     << " detID = " << DBdetid
				     << " charge = " << clust->charge()
				     << " x/y = " << clust->x() << "/" << clust->y()
				     << endl;
	      
	      // -- If this cluster is already in ntuple, do not add it, but finish filling Tk* information
	      int alreadyAt(-1);
	      for (int ic = 0; ic < fClN; ++ic) {
		if ((fClDetId[ic] == DBdetid) 
		    && (static_cast<int>(clust->x()) == static_cast<int>(fClRow[ic]))
		    && (static_cast<int>(clust->y()) == static_cast<int>(fClCol[ic]))
		    ) {
		  alreadyAt = ic; 
		  break; 
		} 
	      }
	      
	      if (alreadyAt > -1) {
		int it(0); 
		while (fClTkI[alreadyAt][it] > -1 && it < TKPERCLMAX) {
		  ++it;
		}
		fClTkI[alreadyAt][it] = fTkN;
		fClTkN[alreadyAt]    += 1;
		
		if (iCluster < CLPERTRACKMAX) {
		  fTkResX[fTkN][iCluster]   = res.x();
		  fTkResXe[fTkN][iCluster]  = errX;
		  fTkRes2X[fTkN][iCluster]  = (res.x())*phiorientation;
		  fTkRes2Xe[fTkN][iCluster] = errX;
		  
		  fTkResY[fTkN][iCluster]  = resy;
		  fTkResYe[fTkN][iCluster]  = errY;
		  
		  fTkAlpha[fTkN][iCluster] = alpha;
		  fTkBeta[fTkN][iCluster] = beta;
		  fTkClN[fTkN] = iCluster+1;
		  fTkClI[fTkN][iCluster] = alreadyAt;
		} else {
		  fTkAlpha[fTkN][CLPERTRACKMAX-1] = -98.;
		  fTkBeta[fTkN][CLPERTRACKMAX-1]  = -98.;
		  fTkClN[fTkN] = CLPERTRACKMAX-1;
		  fTkClI[fTkN][CLPERTRACKMAX-1]   = -98;
		}
		
		++iCluster;
		continue; 
	      }
	      
	      
	      if (iCluster < CLPERTRACKMAX) {
		fTkResX[fTkN][iCluster]   = res.x();
		fTkResXe[fTkN][iCluster]  = errX;
		fTkRes2X[fTkN][iCluster]  = (res.x())*phiorientation;
		fTkRes2Xe[fTkN][iCluster] = errX;
		
		fTkResY[fTkN][iCluster]  = resy;
		fTkResYe[fTkN][iCluster]  = errY;
		
		fTkAlpha[fTkN][iCluster] = alpha;
		fTkBeta[fTkN][iCluster] = beta;
		fTkClN[fTkN] = iCluster+1;
		fTkClI[fTkN][iCluster] = fClN;
	      } else {
		fTkAlpha[fTkN][CLPERTRACKMAX-1] = -98.;
		fTkBeta[fTkN][CLPERTRACKMAX-1]  = -98.;
		fTkClN[fTkN] = CLPERTRACKMAX-1;
		fTkClI[fTkN][CLPERTRACKMAX-1]   = -98;
	      }
	      
	      
	      
	      float clCharge = (clust->charge())/1000.0; // convert electrons to kilo-electrons
	      float clChargeCorr = clust->charge() * sqrt( 1.0 / ( 1.0/pow( tan(alpha), 2 ) + 
								   1.0/pow( tan(beta ), 2 ) + 
								   1.0 )
							   )/1000.;
	      
	      float xcenter = clust->x();
	      float ycenter = clust->y();
	      const PixelTopology * topol = &(theGeomDet->specificTopology());
	      LocalPoint clustlp = topol->localPosition( MeasurementPoint(xcenter, ycenter) );
	      GlobalPoint clustgp = theGeomDet->surface().toGlobal( clustlp );
	      
	      fClSize[fClN]   = clust->size();
	      fClSizeX[fClN]  = clust->sizeX();
	      fClSizeY[fClN]  = clust->sizeY();
	      fClRow[fClN]    = clust->x();
	      fClCol[fClN]    = clust->y();
	      
	      fClLx[fClN]      = clustlp.x();
	      fClLy[fClN]      = clustlp.y();
	      fClLxe[fClN]     = sqrt(hit->localPositionError().xx());  // FIXME: check this
	      fClLye[fClN]     = sqrt(hit->localPositionError().yy());
	      
	      fClGx[fClN] = clustgp.x();
	      fClGy[fClN] = clustgp.y();
	      fClGz[fClN] = clustgp.z();
	      
	      fClRhLx[fClN]           = hit->localPosition().x();
	      fClRhLxE[fClN]          = TMath::Sqrt(hit->localPositionError().xx());
	      fClRhLy[fClN]           = hit->localPosition().y();
	      fClRhLyE[fClN]          = TMath::Sqrt(hit->localPositionError().yy());
	      fClRhGx[fClN]           = hit->globalPosition().x();
	      fClRhGy[fClN]           = hit->globalPosition().y();
	      fClRhGz[fClN]           = hit->globalPosition().z();
	      
	      fClRhQualWord[fClN]     = pixhit->rawQualityWord();
	      fClRhqBin[fClN]         = pixhit->qBin();;
	      
	      if (pixhit->hasFilledProb()) {
		fClRhProb[fClN]         = pixhit->clusterProbability(0);
		// 	    fClRhProbX[fClN]        = pixhit->probabilityX();
		// 	    fClRhProbY[fClN]        = pixhit->probabilityY();
		fClRhProbX[fClN]        = pixhit->probabilityXY();
		fClRhProbY[fClN]        = pixhit->probabilityQ();
	      } else {
		fClRhProb[fClN]         = -98;
		fClRhProbX[fClN]        = -98;
		fClRhProbY[fClN]        = -98;
	      }
	      if (pixhit->spansTwoROCs()) {
		fClRhSpansTwoROCs[fClN] = 1;
	      } else {
		fClRhSpansTwoROCs[fClN] = 0;
	      }
	      if (pixhit->isOnEdge()) {
		fClRhIsOnEdge[fClN]     = 1;
	      } else {
		fClRhIsOnEdge[fClN]     = 0;
	      }
	      if (pixhit->hasBadPixels()) {
		fClRhHasBadPixels[fClN] = 1;
	      } else {
		fClRhHasBadPixels[fClN] = 0;
	      }
	      
	      if (barrel) {
		fClLayer[fClN]     = DBlayer;
		fClLadder[fClN]    = DBladder; 
		fClModule[fClN]    = DBmodule;
		fClDisk[fClN]      = -99;
		fClBlade[fClN]     = -99;
		fClPanel[fClN]     = -99;
		fClPlaquette[fClN] = -99;
	      } 
	      
	      if (endcap) {
		fClLayer[fClN]     = -99;
		fClLadder[fClN]    = -99; 
		fClModule[fClN]    = -99;
		fClDisk[fClN]      =  DBdisk;
		fClBlade[fClN]     =  DBblade;
		fClPanel[fClN]     =  DBpanel;
		fClPlaquette[fClN] =  DBmodule;
	      }
	      fClDetId[fClN]   = DBdetid;
	      
	      fClFlipped[fClN] = isFlipped;
	      
	      fClCharge[fClN]     = clCharge;
	      fClChargeCorr[fClN] = clChargeCorr;
	      
	      fClType[fClN]    = 1; 
	      
	      int it(0); 
	      while (fClTkI[fClN][it] > -1 && it < TKPERCLMAX) {
		++it;
	      }
	      fClTkI[fClN][it] = fTkN;
	      fClTkN[fClN]    += 1;
	      
	      // -- Get digis of this cluster
	      const std::vector<SiPixelCluster::Pixel>& pixvector = clust->pixels();
	      if (fVerbose > 1) cout << "  Found " << pixvector.size() << " pixels for this cluster " << endl;
	      for (unsigned int i = 0; i < pixvector.size(); ++i) {
		if (fDgN > DIGIMAX - 1) break;
		SiPixelCluster::Pixel holdpix = pixvector[i];
		
		fDgRow[fDgN]    = holdpix.x;
		fDgCol[fDgN]    = holdpix.y;
		fDgDetId[fDgN]  = DBdetid;
		onlineRocColRow(DBdetid, fDgRow[fDgN], fDgCol[fDgN], fDgRoc[fDgN], fDgRocC[fDgN], fDgRocR[fDgN]);
		
		fDgAdc[fDgN]    = -99.;
		fDgCharge[fDgN] = holdpix.adc/1000.;
		
		LocalPoint lp = topol->localPosition(MeasurementPoint(holdpix.x, holdpix.y));
		fDgLx[fDgN] = lp.x();
		fDgLy[fDgN] = lp.y();
		
		GlobalPoint GP =  theGeomDet->surface().toGlobal(Local3DPoint(lp.x(),lp.y(),lp.z()));
		fDgGx[fDgN] = GP.x();      
		fDgGy[fDgN] = GP.y();
		fDgGz[fDgN] = GP.z();
		
		fDgClI[fDgN] = fClN;
		
		// -- fill pointer to this digi in cluster digi index array
		if ((signed)i < DGPERCLMAX) {
		  fClDgI[fClN][i] = fDgN;
		  fClDgN[fClN] += 1;
		} else {
		  fClDgI[fClN][DGPERCLMAX-1] = -98;
		  fClDgN[fClN] = 99;
		}
		
		++fDgN;
		
	      }
	      
	      ++nCl1;
	      
	      ++fClN;
	      ++iCluster;
	      
	    } // if ( clust.isNonnull() )
	  else 
	    cout << "Pixel rechits with no associated cluster ?!?!?!!!!!!!!!!!!!!!!!!!!!!! " << endl;
	  
	}  // for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = tmeasColl.begin(); ...
	
	++fTkN;
      }
    } 
    else 
      {
	cout << "--> No valid track trajectory association" << endl;
      }
  }

  // ----------------------------------------------------------------------
  // -- Clusters without tracks
  for (TrackerGeometry::DetContainer::const_iterator it = TG->dets().begin(); it != TG->dets().end(); it++){
    if (dynamic_cast<PixelGeomDetUnit*>((*it)) != 0){ 
      DetId detId = (*it)->geographicalId();

      // -- clusters on this det
      edmNew::DetSetVector<SiPixelCluster>::const_iterator isearch = clustColl.find(detId);
      // -- rechits on this det
      SiPixelRecHitCollection::const_iterator dsmatch = hRecHitColl->find(detId);
      SiPixelRecHitCollection::DetSet rhRange;
      if (dsmatch != hRecHitColl->end()) { 
	rhRange = *dsmatch;
      }
      SiPixelRecHitCollection::DetSet::const_iterator rhIteratorBegin = rhRange.begin();
      SiPixelRecHitCollection::DetSet::const_iterator rhIteratorEnd = rhRange.end();
      SiPixelRecHitCollection::DetSet::const_iterator iRh;

      if (isearch != clustColl.end()) {  // Not an empty iterator
	edmNew::DetSet<SiPixelCluster>::const_iterator  di;
	for (di = isearch->begin(); di != isearch->end(); ++di) {
	  if (fClN > CLUSTERMAX - 1) break;

	  // -- If this cluster is already in ntuple, do not add it
	  int alreadyAt(-1);
	  for (int ic = 0; ic < fClN; ++ic) {
	    if ((static_cast<int>(detId) == fClDetId[ic]) 
		&& (static_cast<int>(di->x()) == static_cast<int>(fClRow[ic]))
		&& (static_cast<int>(di->y()) == static_cast<int>(fClCol[ic]))
		) {
	      alreadyAt = ic; 
	      break; 
	    } 
	  }

	  if (-1 == alreadyAt) {
  	    const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
	    if (theGeomDet == 0) {
	      cout << "NO THEGEOMDET" << endl;
	      continue;
	    }
	    const PixelTopology * topol = &(theGeomDet->specificTopology());
	    
	    // -- find location of hit (barrel or endcap, same for cluster)
	    bool barrel = detId.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
	    bool endcap = detId.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);

	    if (!barrel && !endcap) {
	      continue;
	    }

	    float xcenter = di->x();
	    float ycenter = di->y();
	    LocalPoint clustlp = topol->localPosition( MeasurementPoint(xcenter, ycenter) );
	    GlobalPoint clustgp = theGeomDet->surface().toGlobal( clustlp );

	    fClSize[fClN]   = di->size();
	    fClSizeX[fClN]  = di->sizeX();
	    fClSizeY[fClN]  = di->sizeY();
	    fClRow[fClN]    = di->x();
	    fClCol[fClN]    = di->y();
	    
	    fClLx[fClN]      = clustlp.x();
	    fClLy[fClN]      = clustlp.y();
	    fClLxe[fClN]     = -9999.;
	    fClLye[fClN]     = -9999.;
	    
	    fClGx[fClN] = clustgp.x();
	    fClGy[fClN] = clustgp.y();
	    fClGz[fClN] = clustgp.z();
	    
	    int DBlayer, DBladder, DBmodule, DBdisk, DBblade, DBpanel; 

	    if (barrel) {
	      bpixNames(detId, DBlayer, DBladder, DBmodule); 
	      fClLayer[fClN]     = DBlayer;
	      fClLadder[fClN]    = DBladder; 
	      fClModule[fClN]    = DBmodule;
	      fClDisk[fClN]      = -99;
	      fClBlade[fClN]     = -99;
	      fClPanel[fClN]     = -99;
	      fClPlaquette[fClN] = -99;
	    } 

	    if (endcap) {
	      fpixNames(detId, DBdisk, DBblade, DBpanel, DBmodule); 
	      fClLayer[fClN]     = -99;
	      fClLadder[fClN]    = -99; 
	      fClModule[fClN]    = -99;
	      fClDisk[fClN]      =  DBdisk;
	      fClBlade[fClN]     =  DBblade;
	      fClPanel[fClN]     =  DBpanel;
	      fClPlaquette[fClN] =  DBmodule;
	    }
	    fClDetId[fClN]  = detId;
	    
	    // -- Flipped module?
	    float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
	    float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
	    int isFlipped(0); 
	    if (tmp2 < tmp1) isFlipped = 1;
	    else isFlipped = 0;

	    fClFlipped[fClN] = isFlipped;

	    fClCharge[fClN]     = (di->charge())/1000.;
	    fClChargeCorr[fClN] = (di->charge())/1000.;
	    
	    fClType[fClN]    = 2; 
	    
	    int rhx(0), rhy(0), foundRH(0); 
	    for (iRh = rhIteratorBegin; iRh != rhIteratorEnd; ++iRh) {
	      rhx = static_cast<int>(iRh->cluster()->x()); 
	      rhy = static_cast<int>(iRh->cluster()->y()); 
	      if ((rhx == static_cast<int>(di->x())) && (rhy == static_cast<int>(di->y()))) {
		foundRH = 1; 
		break;
	      }
	    }
	    
	    if ( 1 == foundRH ) 
	      {
		
		// gavril : begin : associate and store simhit info
		if ( fAccessSimHitInfo )
		  {
		    vec_simhits_assoc.clear();
		    vec_simhits_assoc = associate->associateHit(*iRh);
		    
		    fClSimHitN[fClN] = (int)vec_simhits_assoc.size();
		    int iSimHit = 0;
		    
		    for (std::vector<PSimHit>::const_iterator m = vec_simhits_assoc.begin(); 
			 m < vec_simhits_assoc.end() && iSimHit < SIMHITPERCLMAX; ++m) 
		      {
			//cout << "iSimHit = " << iSimHit << endl;
			
			fClSimHitPID[fClN][iSimHit]   = (int)( m->particleType() );
			fClSimHitPRC[fClN][iSimHit]   = (int)( m->processType()  );
			fClSimHitTrkID[fClN][iSimHit] = (int)( m->trackId()      );
			
			fClSimHitLx[fClN][iSimHit]    = ( m->entryPoint().x() + m->exitPoint().x() ) / 2.0;
			fClSimHitLy[fClN][iSimHit]    = ( m->entryPoint().y() + m->exitPoint().y() ) / 2.0;
			
			fClSimHitThe[fClN][iSimHit]   =  m->thetaAtEntry();
			fClSimHitPhi[fClN][iSimHit]   =  m->phiAtEntry();
			fClSimHitMom[fClN][iSimHit]   =  m->pabs();
			
			++iSimHit;
			
		      } // end sim hit loop
		    
		    int minPixelRow = (*di).minPixelRow();
		    int maxPixelRow = (*di).maxPixelRow();
		    int minPixelCol = (*di).minPixelCol();
		    int maxPixelCol = (*di).maxPixelCol();
		    
		    int DBdetid = iRh->geographicalId().rawId();
		    edm::DetSetVector<PixelDigiSimLink>::const_iterator ipdgl = pixeldigisimlink->find( DBdetid );
		    edm::DetSet<PixelDigiSimLink> digiLink = (*ipdgl);
		    
		    edm::DetSet<PixelDigiSimLink>::const_iterator linkiter = digiLink.data.begin();
		    //create a vector for the track ids in the digisimlinks
		    std::vector<int> simTrackIdV;    
		    std::vector<float> simTrackFrV;
		    
		    std::vector<int>   simTrackId2V; 
		    std::vector<int>   simTrackTypeV;
		    std::vector<int>   simTrackQV;   
		    std::vector<float> simTrackPxV;  
		    std::vector<float> simTrackPyV;  
		    std::vector<float> simTrackPzV;  
		    std::vector<float> simTrackEnV;  
		    
		    std::vector<float> simTrackEtaV; 
		    std::vector<float> simTrackPhiV; 
		    std::vector<float> simTrackPtV;  
		    
		    std::vector<float> simTrackVxV;  
		    std::vector<float> simTrackVyV;  
		    std::vector<float> simTrackVzV;  
		    
		    simTrackIdV.clear();    
		    simTrackFrV.clear();
		    
		    simTrackId2V.clear(); 
		    simTrackTypeV.clear();
		    simTrackQV.clear();   
		    simTrackPxV.clear();  
		    simTrackPyV.clear();  
		    simTrackPzV.clear();  
		    simTrackEnV.clear();  
		    
		    simTrackEtaV.clear(); 
		    simTrackPhiV.clear(); 
		    simTrackPtV.clear();  
		    
		    simTrackVxV.clear();  
		    simTrackVyV.clear();  
		    simTrackVzV.clear();
		    
		    
		    for( ; linkiter != digiLink.data.end(); linkiter++) 
		      { // loop over all digisimlinks
			
			std::pair<int,int> pixel_coord = PixelDigi::channelToPixel(linkiter->channel());
			
			// is the digisimlink inside the cluster boundaries?
			if ( pixel_coord.first  <= maxPixelRow &&
			     pixel_coord.first  >= minPixelRow &&
			     pixel_coord.second <= maxPixelCol &&
			     pixel_coord.second >= minPixelCol ) 
			  {
			    
			    bool inStock(false); // did we see this simTrackId before? - We will check later
			    
			    std::vector<int>::const_iterator sTIter =  simTrackIdV.begin();
			    for ( ; sTIter < simTrackIdV.end(); sTIter++) 
			      {
				if ( (*sTIter) == (int)linkiter->SimTrackId() ) 
				  {
				    inStock=true; // now we saw this id before
				  }
			      }
			    
			    if ( !inStock ) 
			      {
				
				int simtrkid = linkiter->SimTrackId();      			      
				
				for (SimTrackContainer::const_iterator simTrack=simTC.begin(); simTrack!=simTC.end(); simTrack++)
				  {
				    
				    if ( (int)simTrack->trackId() == simtrkid )
				      {
					
					simTrackIdV.push_back(   linkiter->SimTrackId()    ); // add the track id to the vector
					simTrackFrV.push_back(   linkiter->fraction()      ); 
					
					simTrackId2V.push_back(  (int)simTrack->trackId()  );
					simTrackTypeV.push_back( simTrack->type()          ); 
					simTrackQV.push_back(    simTrack->charge()        );
					simTrackPxV.push_back(   simTrack->momentum().x()  );
					simTrackPyV.push_back(   simTrack->momentum().y()  );
					simTrackPzV.push_back(   simTrack->momentum().z()  );
					simTrackEnV.push_back(   simTrack->momentum().t()  );
					
					simTrackEtaV.push_back(  simTrack->momentum().eta()             );
					simTrackPhiV.push_back(  simTrack->momentum().phi()             );
					simTrackPtV.push_back(   simTrack->momentum().pt()              );
					
					simTrackVxV.push_back(   simVC[simTrack->vertIndex()].position().x() );
					simTrackVyV.push_back(   simVC[simTrack->vertIndex()].position().y() );
					simTrackVzV.push_back(   simVC[simTrack->vertIndex()].position().z() );
					
					break;
					
				      } // if ( (int)simTrack->trackId() == simtrkid )
				    
				  } // for (SimTrackContainer::const_iterator simTrack=simTC.begin(); ...)
				
				// gavril : check this 
				//  if ( !found_sim_trk )
				//cout << "2 Din not find sim tracks with ID = " << simtrkid << "   !!!!!!!!!!!!!!!!!!!" << endl;
				
			      } // if ( !inStock )
			    
			  } // if ( pixel_coord.first  <= maxPixelRow && ... )
			
		      } // for( ; linkiter != digiLink.data.end(); linkiter++) 
		    
		    fClSimTrN[fClN] = (int)simTrackIdV.size();
		    
		    for ( int iSimTrk = 0; iSimTrk < fClSimTrN[fClN]; iSimTrk++  )
		      {
			fClSimTrID[fClN][iSimTrk] = simTrackIdV[iSimTrk]; 
			fClSimTrFr[fClN][iSimTrk] = simTrackFrV[iSimTrk];
			
			fClSimTrID2[fClN][iSimTrk]       = simTrackId2V[iSimTrk];
			fClSimTrType[fClN][iSimTrk]      = simTrackTypeV[iSimTrk];
			fClSimTrQ[fClN][iSimTrk]         = simTrackQV[iSimTrk];
			fClSimTrPx[fClN][iSimTrk]        = simTrackPxV[iSimTrk];
			fClSimTrPy[fClN][iSimTrk]        = simTrackPyV[iSimTrk];
			fClSimTrPz[fClN][iSimTrk]        = simTrackPzV[iSimTrk];
			fClSimTrEn[fClN][iSimTrk]        = simTrackEnV[iSimTrk];
			
			fClSimTrEta[fClN][iSimTrk]       = simTrackEtaV[iSimTrk];
			fClSimTrPhi[fClN][iSimTrk]       = simTrackPhiV[iSimTrk];
			fClSimTrPt[fClN][iSimTrk]        = simTrackPtV[iSimTrk];
			
			fClSimTrVx[fClN][iSimTrk]   = simTrackVxV[iSimTrk];
			fClSimTrVy[fClN][iSimTrk]   = simTrackVyV[iSimTrk];
			fClSimTrVz[fClN][iSimTrk]   = simTrackVzV[iSimTrk];
			
			
		      } // for ( int iSimTrk = 0; iSimTrk < fClSimTrN[fClN]; iSimTrk++  )
		
		    
		  } // 	if ( fAccessSimHitInfo ) 

		// gavril : end : associate and store simhit info
	       
		
		fClRhLx[fClN]           = iRh->localPosition().x();
		fClRhLxE[fClN]          = TMath::Sqrt(iRh->localPositionError().xx());
		fClRhLy[fClN]           = iRh->localPosition().y();
		fClRhLyE[fClN]          = TMath::Sqrt(iRh->localPositionError().yy());
		
		// 	    fClRhGx[fClN]           = iRh->globalPosition().x();
		// 	    fClRhGy[fClN]           = iRh->globalPosition().y();
		// 	    fClRhGz[fClN]           = iRh->globalPosition().z();
		
		fClRhQualWord[fClN]     = iRh->rawQualityWord();
		fClRhqBin[fClN]         = iRh->qBin();;
		
	      if (iRh->hasFilledProb()) {
		fClRhProb[fClN]         = iRh->clusterProbability(0);
		fClRhProbX[fClN]        = iRh->probabilityX();
		fClRhProbY[fClN]        = iRh->probabilityY();
	      } else {
		fClRhProb[fClN]         = -98;
		fClRhProbX[fClN]        = -98;
		fClRhProbY[fClN]        = -98;
	      }
	      if (iRh->spansTwoROCs()) {
		fClRhSpansTwoROCs[fClN] = 1;
	      } else {
		fClRhSpansTwoROCs[fClN] = 0;
	      }
	      if (iRh->isOnEdge()) {
		fClRhIsOnEdge[fClN]     = 1;
	      } else {
		fClRhIsOnEdge[fClN]     = 0;
	      }
	      if (iRh->hasBadPixels()) {
		fClRhHasBadPixels[fClN] = 1;
	      } else {
		fClRhHasBadPixels[fClN] = 0;
	      }
	    }

	    // -- Get digis of this cluster
	    const std::vector<SiPixelCluster::Pixel>& pixvector = di->pixels();
	    for (unsigned int i = 0; i < pixvector.size(); ++i) {
	      if (fDgN > DIGIMAX - 1) break;
	      SiPixelCluster::Pixel holdpix = pixvector[i];
	      
	      fDgRow[fDgN]    = holdpix.x;
	      fDgCol[fDgN]    = holdpix.y;
	      fDgDetId[fDgN]  = detId;

	      onlineRocColRow(detId, fDgRow[fDgN], fDgCol[fDgN], fDgRoc[fDgN], fDgRocC[fDgN], fDgRocR[fDgN]);
	      fDgAdc[fDgN]    = -99.;
	      fDgCharge[fDgN] = holdpix.adc/1000.;
	      
	      LocalPoint lp = topol->localPosition(MeasurementPoint(holdpix.x, holdpix.y));
	      fDgLx[fDgN] = lp.x();
	      fDgLy[fDgN] = lp.y();
	      
	      GlobalPoint GP =  theGeomDet->surface().toGlobal(Local3DPoint(lp.x(),lp.y(),lp.z()));
	      fDgGx[fDgN] = GP.x();      
	      fDgGy[fDgN] = GP.y();
	      fDgGz[fDgN] = GP.z();

	      fDgClI[fDgN] = fClN;
	      
	      // -- fill pointer to this digi in cluster digi index array
	      if ((signed)i < DGPERCLMAX) { 
		fClDgI[fClN][i] = fDgN;
		fClDgN[fClN] += 1;
	      } else {
		fClDgI[fClN][DGPERCLMAX-1] = -98;
		fClDgN[fClN] = 99;
	      }
	      
	      ++fDgN;

	    }

	    ++nCl2;
	    ++fClN;

	  }
	}
      }
    }
  }

  // -- That's it
  if (fVerbose > 0) {
    cout << nevt << " (" << fRun << "/" << fEvent << ")" 
	 << " lumiSec: " << fLumiBlock 
	 << " orb: " << fOrbit << " BX: " << fBX
        // << " time: " << fTimeHi << "/" << fTimeLo
	 << " TkN: " << fTkN 
	 << " reco: " << nTk
	 << " ClN: " << fClN 
	 << "(0: " << nCl0 << "/1: " << nCl1 << "/2: " << nCl2 << "/1+2: " << nCl1+nCl2 << ")"
      //	 << " reco: " << clusterSet.size()  
	 << " DgN: " << fDgN 
	 << " FEDs: " << nbpixfeds << "/" << nfpixfeds
	 << endl;
  }

  if (fDumpAllEvents > 0) {
    fTree->Fill();
  } else {
    if (fMuN > 0 || fTkN > 0 || fClN > 0 || fDgN > 0) {
      fTree->Fill();
    }
  }

} 


// ----------------------------------------------------------------------
void PixelTree::init() {

  for (int i = 0; i < 4; ++i) {
    fL1TA[i] = fL1TT[i] = 0;
  }

  for (int i = 0; i < 8; ++i) {
    fHLTA[i] = 0;
  }
  
  for (int i = 0; i < 64; ++i) {
    fTtA[i]    = false; 
    fL1A[i]    = false; 
    fL1A[i+64] = false; 

    fHlA[i]    = false; 
    fHlA[i+64] = false; 
    fHlA[i+128]= false; 
    fHlA[i+192]= false; 
    fHlA[i+256]= false; 
    fHlA[i+320]= false; 
    fHlA[i+384]= false; 
    fHlA[i+448]= false; 
    fHlA[i+512]= false; 
    fHlA[i+576]= false; 
    fHlA[i+640]= false; 
    fHlA[i+704]= false; 
    fHlA[i+768]= false; 
    fHlA[i+832]= false; 
    fHlA[i+896]= false; 
    fHlA[i+960]= false; 
  }

  for (int i = 0; i < fPvN; ++i) {
    fPvX[i] = fPvY[i] = fPvZ[i] =  fPvXe[i] = fPvYe[i] = fPvZe[i] = fPvChi2[i] = -9999.;
    fPvNdof[i] = fPvIsFake[i] = -9999; 
  }
  fPvN = 0; 

  fMuTmean = -9999.;
  for (int i = 0; i < fMuN; ++i) {
    fMuPt[i]   = fMuPhi[i] = fMuTheta[i] = -9999.;
    fMuT[i] = fMuTcorr[i] = fMuTerr[i] = -9999.;
    fMuType[i] = fMuTkI[i] = -1; 
  }
  fMuN = 0; 
  fMuTrigger = 0; 

  for (int i = 0; i < fTkN; ++i) {
    fTkQuality[i]= -9999; 
    fTkCharge[i] = -9999; 
    fTkNHits[i]  = fTkLHits[i] = 0;
    fTkLHitsI[i] = fTkLHitsO[i]= 0;
    fTkNHitFr[i] = 0.;
    fTkChi2[i]   = fTkNdof[i] = -9999.;
    fTkPt[i]     = fTkTheta[i] = fTkPhi[i] = -9999.;
    fTkD0[i]     = fTkDz[i] = -9999.;
    fTkVx[i]     = fTkVy[i] = fTkVz[i] = -9999.;
    fTkType[i]   = fTkMuI[i] = -9999;
    fTkClN[i]    = 0;
    for (int j = 0; j < CLPERTRACKMAX; ++j) {
      fTkClI[i][j] = -9999;
      fTkAlpha[i][j] = fTkBeta[i][j] = -9999.;
      fTkResX[i][j]  = fTkResY[i][j] = -9999.;
      fTkResXe[i][j] = fTkResYe[i][j] = -9999.;
      fTkRes2X[i][j] = fTkRes2Xe[i][j] = -9999.;
    }
  }
  fTkN = 0; 

  for (int i = 0; i < fClN; ++i) {
    fClSize[i] = fClSizeX[i] = fClSizeY[i] = -9999; 
    fClRow[i] = fClCol[i] = -9999; 
    fClDetId[i] = -9999; 
    fClLx[i] = fClLy[i] = -9999.;
    fClLxe[i] = fClLye[i] = -9999.;
    fClGx[i] = fClGy[i] = fClGz[i] = -9999.;
    fClCharge[i] = fClChargeCorr[i] = -9999.;
    fClType[i] = -9999;
    for (int j = 0; j < DGPERCLMAX; ++j) {
      fClDgI[i][j] = -9999;
    }
    for (int j = 0; j < TKPERCLMAX; ++j) {
      fClTkI[i][j] = -9999;
    }
    fClDgN[i] = 0;
    fClTkN[i] = 0;

    fClLayer[i] = fClLadder[i] = fClModule[i] = fClFlipped[i] = -9999;
    fClDisk[i] = fClBlade[i] = fClPanel[i] = fClPlaquette[i] = -9999;

    fClRhLx[i]  = -9999;
    fClRhLy[i]  = -9999;
    fClRhLxE[i] = -9999;
    fClRhLyE[i] = -9999;
    fClRhGx[i]  = -9999;
    fClRhGy[i]  = -9999;
    fClRhGz[i]  = -9999;
    
    fClRhProb[i]  = -9999;
    fClRhProbX[i]  = -9999;
    fClRhProbY[i]  = -9999;
    fClRhQualWord[i]  = 9999;
    fClRhqBin[i]  = -9999;
    fClRhSpansTwoROCs[i] = -9999;
    fClRhIsOnEdge[i] = -9999;
    fClRhHasBadPixels[i] = -9999;
  
    
    // gavril : sim hit info
    
    if ( fAccessSimHitInfo )
      {
	for (int j=0; j<SIMHITPERCLMAX; ++j)
	  {
	    fClSimHitPID[i][j]   = -9999;   // PID of the particle that produced the simHit associated with the cluster/rechit
	    fClSimHitPRC[i][j]   = -9999;   // procces type ( 2 = primary, ... )
	    fClSimHitLx[i][j]    = -9999;   // X local position of simhit 
	    fClSimHitLy[i][j]    = -9999;   // Y local position of simhit
	    fClSimHitTrkID[i][j] = -9999;   // 
	    fClSimHitThe[i][j]    = -9999; 
	    fClSimHitPhi[i][j]    = -9999; 
	    fClSimHitMom[i][j]    = -9999; 
	    
	    
	  }
	fClSimHitN[i] = 0;
	
	for (int j=0; j<SIMTRKPERCLMAX; ++j)
	  {
	    fClSimTrID[i][j]   = -9999;  
	    fClSimTrFr[i][j]   = -9999;  
	    
	    fClSimTrID2 [i][j]   = -9999;
	    fClSimTrType[i][j]   = -9999;
	    fClSimTrQ   [i][j]   = -9999;  
	    fClSimTrPx  [i][j]   = -9999;  
	    fClSimTrPy  [i][j]   = -9999;
	    fClSimTrPz  [i][j]   = -9999; 
	    fClSimTrEn  [i][j]   = -9999; 
	    
	    fClSimTrEta [i][j]   = -9999;
	    fClSimTrPhi [i][j]   = -9999;
	    fClSimTrPt  [i][j]   = -9999;
	    
	    fClSimTrVx  [i][j]   = -9999;
	    fClSimTrVy  [i][j]   = -9999;
	    fClSimTrVz  [i][j]   = -9999;
	    
	  }
	fClSimTrN[i] = 0;
	
      }
    
  }
  
  
  fClN = 0; 
  
  for (int i = 0; i < fDgN; ++i) {
    fDgRow[i] = fDgCol[i] = -9999;
    fDgLx[i] = fDgLy[i] = fDgGx[i] = fDgGy[i] = fDgGz[i] = -9999.;
    fDgAdc[i] = fDgCharge[i] = -9999.;
    fDgClI[i] = -9999;
    fDgRoc[i] = fDgRocR[i] = fDgRocC[i] = -9999;
  }
  fDgN = 0; 
  
}
  

// ----------------------------------------------------------------------
void PixelTree::isPixelTrack(const edm::Ref<std::vector<Trajectory> > &refTraj, bool &isBpixtrack, bool &isFpixtrack) {
  std::vector<TrajectoryMeasurement> tmeasColl = refTraj->measurements();
  std::vector<TrajectoryMeasurement>::const_iterator tmeasIt;
  for (tmeasIt = tmeasColl.begin(); tmeasIt != tmeasColl.end(); tmeasIt++) {
    if (!tmeasIt->updatedState().isValid()) continue; 
    TransientTrackingRecHit::ConstRecHitPointer testhit = tmeasIt->recHit();
    if (!testhit->isValid() || testhit->geographicalId().det() != DetId::Tracker) continue; 
    uint testSubDetID = (testhit->geographicalId().subdetId()); 
    if (testSubDetID==PixelSubdetector::PixelBarrel) isBpixtrack = true;
    if (testSubDetID==PixelSubdetector::PixelEndcap) isFpixtrack = true;
    if (isBpixtrack && isFpixtrack) break;
  }

}


// ----------------------------------------------------------------------
void PixelTree::bpixNames(const DetId &pID, int &DBlayer, int &DBladder, int &DBmodule) {
  PixelBarrelName::Shell DBshell = PixelBarrelName(pID).shell();
  DBlayer  = PixelBarrelName(pID).layerName();
  DBladder = PixelBarrelName(pID).ladderName();
  DBmodule = PixelBarrelName(pID).moduleName();
  
  if (DBshell == PixelBarrelName::mO) {
    DBladder *= -1;
    DBmodule *= -1;
  }
  
  if (DBshell == PixelBarrelName::mI) {
    DBmodule *= -1;
  }
  
  if (DBshell == PixelBarrelName::pO) {
    DBladder *= -1;
  }

}

// ----------------------------------------------------------------------
void PixelTree::fpixNames(const DetId &pID, int &DBdisk, int &DBblade, int &DBpanel, int &DBmodule) {

  PixelEndcapName::HalfCylinder DBside = PixelEndcapName(pID).halfCylinder();
  DBdisk   = PixelEndcapName(pID).diskName();
  DBblade  = PixelEndcapName(pID).bladeName();
  DBpanel  = PixelEndcapName(pID).pannelName();
  DBmodule = PixelEndcapName(pID).plaquetteName();
  
  if (DBside == PixelEndcapName::mO) {
    DBdisk   *= -1; 
    DBblade  *= -1;
    DBpanel  *= -1;
    DBmodule *= -1;
  }
  
  if (DBside == PixelEndcapName::mI) {
    DBdisk   *= -1; 
  }
  
  if (DBside == PixelEndcapName::pO) {
    DBblade  *= -1;
    DBpanel  *= -1;
    DBmodule *= -1;
  }

}


// ----------------------------------------------------------------------
// copied from DQM/SiPixelMonitorClient/src/SiPixelInformationExtractor.cc
void PixelTree::onlineRocColRow(const DetId &pID, int offlineRow, int offlineCol, int &roc, int &col, int &row) {
  int realfedID = -1;
  uint32_t newDetId = pID;

  //   for (int fedid = 0; fedid < 40; ++fedid){
  //     if (fPFC[fedid]->hasDetUnit(newDetId)){
  //       realfedID = fedid;
  //       break;   
  //     }    
  //   }
  //   if (realfedID != fFEDID[newDetId]) {
  //     cout << "========XXXXXXXXXXXXXX============ " << realfedID << " <-> " << fFEDID[newDetId] << endl;
  //   }

  //  SiPixelFrameConverter formatter(fCablingMap.product(), realfedID);
  sipixelobjects::ElectronicIndex cabling; 
  sipixelobjects::DetectorIndex detector = {pID, offlineRow, offlineCol};      

  realfedID = fFEDID[newDetId];
  fPFC[realfedID]->toCabling(cabling, detector);
  // cabling should now contain cabling.roc and cabling.dcol  and cabling.pxid
  // however, the coordinates now need to be converted from dcl,pxid to the row,col coordinates used in the calibration info 
  sipixelobjects::LocalPixel::DcolPxid loc;
  loc.dcol = cabling.dcol;
  loc.pxid = cabling.pxid;

  sipixelobjects::LocalPixel locpixel(loc);
  sipixelobjects::CablingPathToDetUnit path = {realfedID, cabling.link, cabling.roc};  
  const sipixelobjects::PixelROC *theRoc = fCablingMap->findItem(path);

  roc = theRoc->idInDetUnit();
  uint32_t detSubId = pID.subdetId();
  if (detSubId == 1) {
    PixelBarrelName nameworker(pID);
    std::string outputname = nameworker.name();
    bool HalfModule = nameworker.isHalfModule();
    if ((outputname.find("mO") != string::npos || outputname.find("mI") != string::npos) && (HalfModule)) {
      roc = theRoc->idInDetUnit() + 8;
    }
  }
  
  col = locpixel.rocCol();
  row = locpixel.rocRow();

}


// define this as a plug-in
DEFINE_FWK_MODULE(PixelTree);
