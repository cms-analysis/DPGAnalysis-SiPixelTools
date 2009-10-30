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
#include <vector>
#include <bitset>

#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/Provenance/interface/Timestamp.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"

#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h>
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "PixelTree.h"

#include <TROOT.h>
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
  fTrajectoryInputLabel(iConfig.getUntrackedParameter<string>("trajectoryInputLabel", string("ctfRefitter"))),
  fMuonCollectionLabel(iConfig.getUntrackedParameter<string>("muonCollectionLabel", string("muons"))),
  fTrackCollectionLabel(iConfig.getUntrackedParameter<string>("trackCollectionLabel", string("ctfWithMaterialTracksP5"))),
  fPixelClusterLabel(iConfig.getUntrackedParameter<string>("pixelClusterLabel", string("siPixelClusters"))), 
  fL1GTReadoutRecordLabel(iConfig.getUntrackedParameter<std::string> ("L1GTReadoutRecordLabel")),
  fL1GTmapLabel(iConfig.getUntrackedParameter<InputTag> ("hltL1GtObjectMap")),
  fHLTResultsLabel(iConfig.getUntrackedParameter<InputTag> ("HLTResultsLabel"))
{
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- PixelTree constructor" << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  cout << "---  rootFileName:                    " << fRootFileName << endl;
  cout << "---  trajectoryInputLabel:            " << fTrajectoryInputLabel.c_str() << endl;
  cout << "---  muonCollectionLabel:             " << fMuonCollectionLabel << endl;
  cout << "---  trackCollectionLabel:            " << fTrackCollectionLabel << endl;
  cout << "---  pixelClusterLabel:               " << fPixelClusterLabel << endl;
  cout << "---  L1GTReadoutRecordLabel:          " << fL1GTReadoutRecordLabel << endl;
  cout << "---  hltL1GtObjectMap:                " << fL1GTmapLabel << endl;
  cout << "---  HLTResultsLabel:                 " << fHLTResultsLabel << endl;
  cout << "----------------------------------------------------------------------" << endl;
}

// ----------------------------------------------------------------------
PixelTree::~PixelTree() { }  

// ----------------------------------------------------------------------
void PixelTree::endJob() { 
  fFile->cd();

  fTree->Write();
  //   fL1Thist->Write(); 
  //   fHLThist->Write(); 

  fFile->Write();
  fFile->Close();
  delete fFile;
}

// ----------------------------------------------------------------------
void PixelTree::beginJob(const edm::EventSetup& es) {
  
  // -- a 'feature' of TFileService is that it does not like to add trees until a histogram is also created.
  //  edm::Service<TFileService> fs;
  //  fH1 = fs->make<TH1D>("nevt","nevt", 1, 0., 1.);

  // -- switch to direct TFile as a *** glibc detected *** corrupted double-linked list: 0x13389518 ***
  //    leaves the TFileService empty...
  fFile = new TFile(fRootFileName.c_str(), "RECREATE");
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
  fTree->Branch("l1ta0",        &fL1T0,         "l1ta0/i");
  fTree->Branch("l1ta1",        &fL1T1,         "l1ta1/i");
  fTree->Branch("l1ta2",        &fL1T2,         "l1ta2/i");
  fTree->Branch("l1ta3",        &fL1T3,         "l1ta3/i");

  fTree->Branch("hlt",          &fHLT,          "hlt/i");
  fTree->Branch("hlta0",        &fHLTa0,        "hlta0/i");
  fTree->Branch("hlta1",        &fHLTa1,        "hlta1/i");
  fTree->Branch("hlta2",        &fHLTa2,        "hlta2/i");
  fTree->Branch("hlta3",        &fHLTa3,        "hlta3/i");
  fTree->Branch("hltr0",        &fHLTr0,        "hltr0/i");
  fTree->Branch("hltr1",        &fHLTr1,        "hltr1/i");
  fTree->Branch("hltr2",        &fHLTr2,        "hltr2/i");
  fTree->Branch("hltr3",        &fHLTr3,        "hltr3/i");
  fTree->Branch("hlte0",        &fHLTe0,        "hlte0/i");
  fTree->Branch("hlte1",        &fHLTe1,        "hlte1/i");
  fTree->Branch("hlte2",        &fHLTe2,        "hlte2/i");
  fTree->Branch("hlte3",        &fHLTe3,        "hlte3/i");

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

  fTree->Branch("TkN",          &fTkN,          "TkN/I");
  fTree->Branch("TkCharge",     fTkCharge,      "TkCharge[TkN]/I");
  fTree->Branch("TkChi2",       fTkChi2,        "TkChi2[TkN]/F");
  fTree->Branch("TkNdof",       fTkNdof,        "TkNdof[TkN]/F");
  fTree->Branch("TkPt",         fTkPt,          "TkPt[TkN]/F");
  fTree->Branch("TkTheta",      fTkTheta,       "TkTheta[TkN]/F");
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
  fTree->Branch("TkClI",        fTkClI,         "TkClI[TkN][20]/I");    //FIXME: This should be variable?
  fTree->Branch("TkType",       fTkType,        "TkType[TkN]/I");
  fTree->Branch("TkMuI",        fTkMuI,         "TkMuI[TkN]/I");

  fTree->Branch("ClN",          &fClN,          "ClN/I");
  fTree->Branch("ClRow",        fClRow,         "ClRow[ClN]/F");
  fTree->Branch("ClCol",        fClCol,         "ClCol[ClN]/F");
  fTree->Branch("ClLx",         fClLx,          "ClLx[ClN]/F");
  fTree->Branch("ClLxE",        fClLxe,         "ClLxE[ClN]/F");
  fTree->Branch("ClLy",         fClLy,          "ClLy[ClN]/F");
  fTree->Branch("ClLyE",        fClLye,         "ClLyE[ClN]/F");
  fTree->Branch("ClGx",         fClGx,          "ClGx[ClN]/F");
  fTree->Branch("ClGy",         fClGy,          "ClGy[ClN]/F");
  fTree->Branch("ClSize",       fClSize,        "ClSize[ClN]/F");
  fTree->Branch("ClSizeX",      fClSizeX,       "ClSizeX[ClN]/F");
  fTree->Branch("ClSizeY",      fClSizeY,       "ClSizeY[ClN]/F");
  fTree->Branch("ClGz",         fClGz,          "ClGz[ClN]/F");
  fTree->Branch("ClFlipped",    fClFlipped,     "ClFlipped[ClN]/I");
  fTree->Branch("ClLayer",      fClLayer,       "ClLayer[ClN]/I");
  fTree->Branch("ClLadder",     fClLadder,      "ClLadder[ClN]/I");
  fTree->Branch("ClModule",     fClModule,      "ClModules[ClN]/I");
  fTree->Branch("ClDisk",       fClDisk,        "ClDisk[ClN]/I");
  fTree->Branch("ClBlade",      fClBlade,       "ClBlade[ClN]/I");
  fTree->Branch("ClPanel",      fClPanel,       "ClPanel[ClN]/I");
  fTree->Branch("ClPlaquette",  fClPlaquette,   "ClPlaquette[ClN]/I");
  fTree->Branch("ClCharge",     fClCharge,      "ClCharge[ClN]/F");
  fTree->Branch("ClChargeCorr", fClChargeCorr,  "ClChargeCorr[ClN]/F");
  fTree->Branch("ClType",       fClType,        "ClType[ClN]/I");
  fTree->Branch("ClTkI",        fClTkI,         "ClTkI[ClN]/I");
  fTree->Branch("ClDgI",        fClDgI,         "ClDgI[TkN][20]/I");    //FIXME: This should be variable?

  fTree->Branch("DgN",          &fDgN,          "DgN/I");
  fTree->Branch("DgRow",        fDgRow,         "DgRow[DgN]/I");
  fTree->Branch("DgCol",        fDgCol,         "DgCol[DgN]/I");
  fTree->Branch("DgLx",         fDgLx,          "DgLx[DgN]/F");
  fTree->Branch("DgLy",         fDgLy,          "DgLy[DgN]/F");
  fTree->Branch("DgGx",         fDgGx,          "DgGx[DgN]/F");
  fTree->Branch("DgGy",         fDgGy,          "DgGy[DgN]/F");
  fTree->Branch("DgGz",         fDgGz,          "DgGz[DgN]/F");
  fTree->Branch("DgAdc",        fDgAdc,         "DgAdc[DgN]/F");
  fTree->Branch("DgCharge",     fDgCharge,      "DgCharge[DgN]/F");
  fTree->Branch("DgClI",        fDgClI,         "DgClI[DgN]/I");      //FIXME: This should be an array?

  readOffsets();

}


// ----------------------------------------------------------------------
void PixelTree::analyze(const edm::Event& iEvent, 
			const edm::EventSetup& iSetup) {

  static int nevt(0); 
  static unsigned int oldRun(0); 

  if (oldRun == 0) {
    fL1Thist = new TH1D(Form("L1T_%d", fRun), Form("L1T names for run %d", fRun), 128, 0., 128.); fL1Thist->SetDirectory(fFile); 
    fHLThist = new TH1D(Form("HLT_%d", fRun), Form("HLT names for run %d", fRun), 128, 0., 128.); fHLThist->SetDirectory(fFile); 
    oldRun = 1; 
  }

  // -- produce list of detID and modules
  if (0) {
    if (0 == nevt) dumpDetIds(iSetup); 
  }

  ++nevt; 
  init();

  // -- Does this belong into beginJob()?
  ESHandle<TrackerGeometry> TG;
  iSetup.get<TrackerDigiGeometryRecord>().get(TG);
  const TrackerGeometry* theTrackerGeometry = TG.product();
 

  // ----------------------------------------------------------------------
  // -- Event information
  // ----------------------------------------------------------------------
  fRun       = iEvent.id().run();
  fLumiBlock = iEvent.luminosityBlock();
  fEvent     = iEvent.id().event();

  if (fRun != oldRun) {
    fL1Thist = new TH1D(Form("L1T_%d", fRun), Form("L1T names for run %d", fRun), 128, 0., 128.); fL1Thist->SetDirectory(fFile); 
    fHLThist = new TH1D(Form("HLT_%d", fRun), Form("HLT names for run %d", fRun), 128, 0., 128.); fHLThist->SetDirectory(fFile); 
    oldRun = fRun; 
  }

  const edm::Timestamp timeStamp = iEvent.time();
  unsigned int high = timeStamp.value() >> 32;       // seconds
  unsigned int low = 0xffffffff & timeStamp.value(); // microseconds  
  //   Caveat: bitset() ctor only uses the lower 32 bits for initialization!
  //   unsigned long long ltime = timeStamp.value(); 
  //   cout << "timeStamp: " << std::bitset<64>(timeStamp.value()) << " " << timeStamp.value() << endl;
  //   cout << "ltime:     "; for (int i = 63; i >= 0; --i) cout << (ltime & (0x1LL <<i)? '1': '0'); cout << endl;
  //   cout << "high:      " << std::bitset<32>(high) << " " << high << endl;
  //   cout << "low:       " << std::bitset<32>(low) << " " << low << endl;

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
    fFED2 = (0x1 < 12) ; 
  } else {

    edm::ESHandle<RunInfo> runInfoHandle;
    iSetup.get<RunInfoRcd>().get(runInfoHandle);
    const RunInfo *runInfo = runInfoHandle.product();
    unsigned int totbpixfeds(32), totfpixfeds(8); 
    
    std::vector<int> feds = runInfo->m_fed_in;
    uint32_t ifed(0); 
    for (size_t fedindex = 0; fedindex < feds.size(); fedindex++) {
      // cout << "FED " << fedindex << ": " << (uint32_t)feds[fedindex] << endl;
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
    // 	 << " fFED1 = " << std::bitset<32>(fFED1)
    // 	 << " fFED2 = " << std::bitset<32>(fFED2)
    // 	 << endl;
  } 

  // ----------------------------------------------------------------------
  // -- Trigger information
  // ----------------------------------------------------------------------

  // -- L1
  fL1T = fL1T0 = fL1T1 = fL1T2 = fL1T3 = 0; 
  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  iEvent.getByLabel(fL1GTReadoutRecordLabel,L1GTRR);
  Handle<L1GlobalTriggerObjectMapRecord> hL1GTmap; 
  iEvent.getByLabel("hltL1GtObjectMap", hL1GTmap);

  edm::ESHandle<L1GtTriggerMenu> hL1GtMenu;
  iSetup.get<L1GtTriggerMenuRcd>().get(hL1GtMenu);
  const L1GtTriggerMenu* l1GtMenu = hL1GtMenu.product();

  if (L1GTRR.isValid()) {
    fL1T = (L1GTRR->decision()? 1: 0);

    const AlgorithmMap& algorithmMap = l1GtMenu->gtAlgorithmMap();
    for (CItAlgo itAlgo = algorithmMap.begin(); itAlgo != algorithmMap.end(); itAlgo++) {
      std::string aName = itAlgo->first;
      int algBitNumber = (itAlgo->second).algoBitNumber();
      if (fVerbose > 2) cout << "i = " << algBitNumber << " -> " << aName << endl;
      fL1Thist->GetXaxis()->SetBinLabel(algBitNumber+1, aName.c_str());
    }

    int itrig(0); 
    for (unsigned int iTrig = 0; iTrig < L1GTRR->decisionWord().size(); ++iTrig) {
      int l1flag = L1GTRR->decisionWord()[iTrig]; 
      itrig = iTrig%32; 
      if (iTrig < 32) {
	if (l1flag) fL1T0 |= (0x1 << itrig);
      } else if (iTrig < 64) {
	if (l1flag) fL1T1 |= (0x1 << itrig);
      } else if (iTrig < 96) {
	if (l1flag) fL1T2 |= (0x1 << itrig);
      } else if (iTrig < 128) {
	if (l1flag) fL1T3 |= (0x1 << itrig);
      }
    }

    //  const  DecisionWord& gtDecisionWordBeforeMask = gtReadoutRecord->decisionWord();
    //  bool l1SingleEG15 = menu->gtAlgorithmResult( "L1_SingleEG15", gtDecisionWordBeforeMask);
    //    bool algResult = l1AlgorithmResult(iEvent, evSetup, algoName);

    if (fVerbose > 1) {
      cout << "L1 trigger accept: " << fL1T << endl;
      cout << " 3         2         1         0" << endl;
      cout << "10987654321098765432109876543210" << endl;
      cout << "--------------------------------" << endl;
      cout << std::bitset<32>(fL1T0) << endl
	   << std::bitset<32>(fL1T1) << endl
	   << std::bitset<32>(fL1T2) << endl
	   << std::bitset<32>(fL1T3) << endl;
    }

  } 


  // -- HLT: see http://cmslxr.fnal.gov/lxr/source/HLTrigger/HLTanalyzers/src/HLTrigReport.cc
  fHLT = fHLTa0 = fHLTa1 = fHLTa2 = fHLTa3 
    = fHLTr0 = fHLTr1 = fHLTr2 = fHLTr3 
    = fHLTe0 = fHLTe1 = fHLTe2 = fHLTe3 
    = 0; 
  
  Handle<TriggerResults> hHLTresults;
  bool hltF = true;
  try {
    iEvent.getByLabel(fHLTResultsLabel, hHLTresults);
  } catch (cms::Exception &ex) {
    //    cout << ex.explainSelf() << endl;
    cout << "==>PixelTree> Triggerresults  " << fHLTResultsLabel.encode() << " not found " << endl;
    hltF = false;
  }

  if (hltF && hHLTresults.isValid()) {
    TriggerNames triggerNames;
    triggerNames.init(*hHLTresults);
    fHLT = hHLTresults->accept();
    if (fVerbose > 1) cout << "hHLTresults->size() = " << hHLTresults->size() << " and HLT accept = " << fHLT << endl;

    vector<string>  hlNames;
    hlNames = triggerNames.triggerNames();

    int hltacc(0), hltrun(0), hlterr(0), itrig(0); 
    for (unsigned int iTrig = 0; iTrig < hlNames.size(); ++iTrig) {
      hltacc = hHLTresults->accept(iTrig); 
      hltrun  = hHLTresults->wasrun(iTrig); 
      hlterr  = hHLTresults->error(iTrig); 
      itrig = iTrig%32; 

      if (fVerbose > 2) cout << iTrig << " " << triggerNames.triggerName(iTrig) 
			     << " -> " << hlNames[iTrig] << "  " 
			     << " at " << triggerNames.triggerIndex(hlNames[iTrig]) << ",  " 
			     << iTrig << "%32=" << itrig 
			     << endl;

      fHLThist->GetXaxis()->SetBinLabel(iTrig+1, hlNames[iTrig].c_str()); 

      if (iTrig < 32) {
	if (hltacc) fHLTa0 |= (0x1 << itrig);
	if (hltrun) fHLTr0 |= (0x1 << itrig);
	if (hlterr) fHLTe0 |= (0x1 << itrig);
      } else if (iTrig < 64) {
	if (hltacc) fHLTa1 |= (0x1 << itrig);
	if (hltrun) fHLTr1 |= (0x1 << itrig);
	if (hlterr) fHLTe1 |= (0x1 << itrig);
      } else if (iTrig < 96) {
	if (hltacc) fHLTa2 |= (0x1 << itrig);
	if (hltrun) fHLTr2 |= (0x1 << itrig);
	if (hlterr) fHLTe2 |= (0x1 << itrig);
      } else if (iTrig < 128) {
	if (hltacc) fHLTa3 |= (0x1 << itrig);
	if (hltrun) fHLTr3 |= (0x1 << itrig);
	if (hlterr) fHLTe3 |= (0x1 << itrig);
      }
    }

    if (fVerbose > 1)  {
      cout << "HLT trigger accept/run: " << fHLT<< endl;
      cout << " 3         2         1         0" << endl;
      cout << "10987654321098765432109876543210" << endl;
      cout << "--------------------------------" << endl;
      cout << std::bitset<32>(fHLTa0) << endl
	   << std::bitset<32>(fHLTr0) << endl << endl
	   << std::bitset<32>(fHLTa1) << endl
	   << std::bitset<32>(fHLTr1) << endl<< endl
	   << std::bitset<32>(fHLTa2) << endl
	   << std::bitset<32>(fHLTr2) << endl << endl
	   << std::bitset<32>(fHLTa3) << endl
	   << std::bitset<32>(fHLTr3) << endl;
    }   
  }



  // ----------------------------------------------------------------------
  // -- Fill muons
  // ----------------------------------------------------------------------
   edm::Handle<MuonCollection> hMuonCollection;
   bool goodMuons(false);
   try{ 
     iEvent.getByLabel(fMuonCollectionLabel, hMuonCollection);
     goodMuons = true; 
   } catch (cms::Exception &ex) {
     cout << "No Muon collection with label " << fMuonCollectionLabel << endl;
   }
  
  float meanTime(0.), meanTimeW2(0.);
  vector<int> muTkRef;
  if (hMuonCollection.isValid()) {
    int idx(0); 
    for (MuonCollection::const_iterator it = hMuonCollection->begin(), itEnd = hMuonCollection->end(); it != itEnd; ++it){

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
      
      int type = 0; 
      if (it->isGlobalMuon()) type &= 0x1; 
      if (it->isStandAloneMuon()) type &= 0x2; 
      if (it->isTrackerMuon()) type &= 0x4; 
      if (it->isTimeValid()) type &= 0x8; 
      fMuType[fMuN]  = type;
      fMuT[fMuN]     = it->time().timeAtIpInOut;
      fMuTcorr[fMuN] = correctedTime(*it);
      fMuTerr[fMuN]  = it->time().timeAtIpInOutErr;

      fMuPt[fMuN]    =  it->globalTrack()->pt();
      fMuPhi[fMuN]   =  it->globalTrack()->phi();
      fMuTheta[fMuN] =  it->globalTrack()->theta();

      if (it->isTimeValid() && (fMuTerr[fMuN] < 15) && (fMuTerr[fMuN] > 0.)) {
	meanTime   += fMuTcorr[fMuN]/(fMuTerr[fMuN]*fMuTerr[fMuN]);
	meanTimeW2 += 1./(fMuTerr[fMuN]*fMuTerr[fMuN]);
      }
      ++fMuN;
    }
    fMuTmean = meanTime/meanTimeW2;
  }


  // ----------------------------------------------------------------------
  // -- Fill tracks
  // ----------------------------------------------------------------------

  // -- Tracks
  edm::Handle<std::vector<reco::Track> > hTrackCollection;
  try {
    iEvent.getByLabel(fTrackCollectionLabel, hTrackCollection);
  } catch (cms::Exception &ex) {
    cout << "No Track collection with label " << fTrackCollectionLabel << endl;
  }
  if (hTrackCollection.isValid()) {
    const std::vector<reco::Track> trackColl = *(hTrackCollection.product());
  } else {
    cout << "--> No valid track collection" << endl;
  }

  // -- Track trajectory association map
  edm::Handle<TrajTrackAssociationCollection> hTTAC;
  iEvent.getByLabel(fTrackCollectionLabel, hTTAC);

  // -- Pixel cluster
  edm::Handle< edmNew::DetSetVector<SiPixelCluster> > hClusterColl;
  iEvent.getByLabel(fPixelClusterLabel, hClusterColl );
  const edmNew::DetSetVector<SiPixelCluster> clustColl = *(hClusterColl.product());

  std::set<SiPixelCluster> clusterSet;
  TrajectoryStateCombiner tsoscomb;
  if (hTTAC.isValid()) {
    const TrajTrackAssociationCollection ttac = *(hTTAC.product());
    for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {
      const edm::Ref<std::vector<Trajectory> > refTraj = it->key;  
      reco::TrackRef trackref = it->val;

      // -- Check whether it is a pixel track
      bool isBpixTrack(false), isFpixTrack(false); 
      isPixelTrack(refTraj, isBpixTrack, isFpixTrack);
      if (!isBpixTrack && !isFpixTrack) {
	continue; 
      }

      fTkCharge[fTkN] = trackref->charge();
      fTkChi2[fTkN]   = trackref->chi2();
      fTkNdof[fTkN]   = trackref->ndof();
      fTkPt[fTkN]     = trackref->pt();
      fTkTheta[fTkN]  = trackref->theta();
      fTkPhi[fTkN]    = trackref->phi();
      fTkD0[fTkN]     = trackref->d0();
      fTkDz[fTkN]     = trackref->dz();
      fTkVx[fTkN]     = trackref->vx();
      fTkVy[fTkN]     = trackref->vy();
      fTkVz[fTkN]     = trackref->vz();
      fTkType[fTkN]   = 1;
      fTkMuI[fTkN]    = -1;

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
	  cout << "-- PixelTree> Did not find hit->detUnit()" << endl;
	  continue;
	}

	bool barrel = DetId::DetId(hit->geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
	bool endcap = DetId::DetId(hit->geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
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

	int DBlayer, DBladder, DBmodule, DBdisk, DBblade, DBpanel; 
	if (barrel) {
	  bpixNames(DetId::DetId((*hit).geographicalId()), DBlayer, DBladder, DBmodule); 
	} else {
	  fpixNames(DetId::DetId((*hit).geographicalId()), DBdisk, DBblade, DBpanel, DBmodule); 
	}

	const DetId &hit_detId = hit->geographicalId();
	uint IntSubDetID = (hit_detId.subdetId());
	if (IntSubDetID == 0) continue;
	const TrackerGeometry &theTracker(*theTrackerGeometry);
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
	  const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
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

	  fClType[fClN]    = 0; 
	  fClTkI[fClN]     = fTkN;  // ?? FIXME: A cluster can belong to more than one track

	  fTkAlpha[fTkN][iCluster] = alpha;
	  fTkBeta[fTkN][iCluster] = beta;

	  ++fClN;
	  ++iCluster;
	  continue;
	}

	const TrackingRecHit *persistentHit = hit->hit();
	if (persistentHit != 0) {
	  if (typeid(*persistentHit) != typeid(SiPixelRecHit))  continue;
	}
	
	const SiPixelRecHit *pixhit = dynamic_cast<const SiPixelRecHit*>(hit->hit());
	edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = (*pixhit).cluster();
	if (clust.isNonnull()) {
	  clusterSet.insert(*clust);

	  float clCharge = (clust->charge())/1000.0; // convert electrons to kilo-electrons
	  float clChargeCorr = clust->charge() * sqrt( 1.0 / ( 1.0/pow( tan(alpha), 2 ) + 
							       1.0/pow( tan(beta ), 2 ) + 
							       1.0 )
						       )/1000.;
	  
	  float xcenter = clust->x();
	  float ycenter = clust->y();
	  const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
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

	  fClFlipped[fClN] = isFlipped;

	  fClCharge[fClN]     = clCharge;
	  fClChargeCorr[fClN] = clChargeCorr;

	  fClType[fClN]    = 1; 
	  fClTkI[fClN]     = fTkN;  // ?? FIXME: A cluster can belong to more than one track
	  
	  if (iCluster < CLPERTRACKMAX) {
	    align::LocalVector res = tsos.localPosition() - hit->localPosition();
	    LocalError err1 = tsos.localError().positionError();
	    LocalError err2 = hit->localPositionError();
	    float errX = std::sqrt( err1.xx() + err2.xx() );
	    float errY = std::sqrt( err1.yy() + err2.yy() );

	    fTkResX[fTkN][iCluster]   = res.x();
	    fTkResXe[fTkN][iCluster]  = errX;
	    fTkRes2X[fTkN][iCluster]  = (res.x())*phiorientation;
	    fTkRes2Xe[fTkN][iCluster] = errX;

	    float resy(0.);
	    double dZ = gROrZDirection.z() - gPModule.z();
	    if (dZ > -999.) resy = res.y() * (dZ >=0.? +1 : -1) ;
	    else resy = res.y();
	    fTkResY[fTkN][iCluster]  = resy;
	    fTkResYe[fTkN][iCluster]  = errY;
	    
	    fTkAlpha[fTkN][iCluster] = alpha;
	    fTkBeta[fTkN][iCluster] = beta;
	    fTkClI[fTkN][iCluster] = fClN;
	  } else {
	    fTkAlpha[fTkN][CLPERTRACKMAX-1] = -98.;
	    fTkBeta[fTkN][CLPERTRACKMAX-1]  = -98.;
	    fTkClI[fTkN][CLPERTRACKMAX-1]   = -98;
	  }

	  // -- Get digis of this cluster
	  const std::vector<SiPixelCluster::Pixel>& pixvector = clust->pixels();
	  //	  cout << "  Found " << pixvector.size() << " pixels for this cluster " << endl;
	  for (unsigned int i = 0; i < pixvector.size(); ++i) {
	    SiPixelCluster::Pixel holdpix = pixvector[i];
	    
	    fDgRow[fDgN]    = holdpix.x;
	    fDgCol[fDgN]    = holdpix.y;
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
	    } else {
	      fClDgI[fClN][DGPERCLMAX-1] = -98;
	    }

	    ++fDgN;

	  }

	  ++fClN;
	  ++iCluster;
	}

      }

      ++fTkN;
    }
  } else {
    cout << "--> No valid track trajectory association" << endl;
  }


  // ----------------------------------------------------------------------
  // -- Clusters without tracks
  for (TrackerGeometry::DetContainer::const_iterator it = TG->dets().begin(); it != TG->dets().end(); it++){
    if (dynamic_cast<PixelGeomDetUnit*>((*it)) != 0){ 
      DetId detId = (*it)->geographicalId();

      edmNew::DetSetVector<SiPixelCluster>::const_iterator isearch = clustColl.find(detId);
      if (isearch != clustColl.end()) {  // Not an empty iterator
	edmNew::DetSet<SiPixelCluster>::const_iterator  di;
	for (di=isearch->begin(); di!=isearch->end(); di++) {
	  unsigned int temp = clusterSet.size();
	  clusterSet.insert(*di);
	  if (clusterSet.size() > temp) {
  
	    const TrackerGeometry& theTracker(*theTrackerGeometry);
	    const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
	    if (theGeomDet == 0) {
	      cout << "NO THEGEOMDET" << endl;
	      continue;
	    }
	    const RectangularPixelTopology *topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	    
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
	    
	    // -- Get digis of this cluster
	    const std::vector<SiPixelCluster::Pixel>& pixvector = di->pixels();
	    for (unsigned int i = 0; i < pixvector.size(); ++i) {
	      SiPixelCluster::Pixel holdpix = pixvector[i];
	      
	      fDgRow[fDgN]    = holdpix.x;
	      fDgCol[fDgN]    = holdpix.y;
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
	      } else {
		fClDgI[fClN][DGPERCLMAX-1] = -98;
	      }
	      
	      ++fDgN;

	    }

	    ++fClN;

	  }
	}
      }
    }
  }

  // -- That's it
  if (fVerbose > 0) {
    cout << nevt << "(" << fRun << "/" << fEvent << ")" 
	 << " MuN: " << fMuN << " TkN: " << fTkN << " ClN: " << fClN << " DgN: " << fDgN 
	 << " FEDs: " << nbpixfeds << "/" << nfpixfeds
	 << endl;
  }

  fTree->Fill();
} 


// ----------------------------------------------------------------------
void PixelTree::init() {

  fMuN = 0; 
  fMuTmean = -9999.;
  for (int i = 0; i < MUMAX; ++i) {
    fMuPt[i]   = fMuPhi[i] = fMuTheta[i] = -9999.;
    fMuT[i] = fMuTcorr[i] = fMuTerr[i] = -9999.;
    fMuType[i] = fMuTkI[i] = -1; 
  }

  fTkN = 0; 
  for (int i = 0; i < TRACKMAX; ++i) {
    fTkCharge[i] = -9999; 
    fTkChi2[i] = fTkNdof[i] = -9999.;
    fTkPt[i] = fTkTheta[i] = fTkPhi[i] = -9999.;
    fTkD0[i] = fTkDz[i] = -9999.;
    fTkVx[i] = fTkVy[i] = fTkVz[i] = -9999.;
    fTkType[i] = fTkMuI[i] = -9999;
    for (int j = 0; j < CLPERTRACKMAX; ++j) {
      fTkClI[i][j] = -9999;
      fTkAlpha[i][j] = fTkBeta[i][j] = -9999.;
      fTkResX[i][j]  = fTkResY[i][j] = -9999.;
      fTkResXe[i][j] = fTkResYe[i][j] = -9999.;
      fTkRes2X[i][j] = fTkRes2Xe[i][j] = -9999.;
    }
  }

  fClN = 0; 
  for (int i = 0; i < CLUSTERMAX; ++i) {
    fClSize[i] = fClSizeX[i] = fClSizeY[i] = -9999; 
    fClRow[i] = fClCol[i] = -9999; 
    fClLx[i] = fClLy[i] = -9999.;
    fClLxe[i] = fClLye[i] = -9999.;
    fClGx[i] = fClGy[i] = fClGz[i] = -9999.;
    fClCharge[i] = fClChargeCorr[i] = -9999.;
    fClType[i] = fClTkI[i] = -9999;
    for (int j = 0; j < DGPERCLMAX; ++j) {
      fClDgI[i][j] = -9999;
    }

    fClLayer[i] = fClLadder[i] = fClModule[i] = fClFlipped[i] = -9999;
    fClDisk[i] = fClBlade[i] = fClPanel[i] = fClPlaquette[i] = -9999;
  }

  fDgN = 0; 
  for (int i = 0; i < DIGIMAX; ++i) {
    fDgRow[i] = fDgCol[i] = -9999;
    fDgLx[i] = fDgLy[i] = fDgGx[i] = fDgGy[i] = fDgGz[i] = -9999.;
    fDgAdc[i] = fDgCharge[i] = -9999.;
    fDgClI[i] = -9999;
  }
    
}


// ----------------------------------------------------------------------
void PixelTree::readOffsets() {
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 16; j++){
      fspoints[i][j] =  -10000;
      fsbias[i][j] =  -10000;
      fsrms[i][j] =  -10000;
    }
  }

  ifstream f1("muons_input-singleOffset.txt");
  while(!f1.eof())  {
   int w0,s0;
   float b,r,p;

   f1 >>  w0 >> s0 >> p >>  b >> r;
   if (!f1.good()) break;

   fspoints[w0][s0] = p;
   fsbias[w0][s0] = b;
   fsrms[w0][s0] = r;
   cout << " muon offsets " << w0 << " " << s0 << " " << b << " " << p << " " << r <<  endl;
  }
}


// ----------------------------------------------------------------------
void PixelTree::sectorAndWheel(const reco::Muon & muon0,int &w0, int &s0) {
 w0=0;
 s0=0;
 for (trackingRecHit_iterator match = muon0.bestTrack()->recHitsBegin(); match != muon0.bestTrack()->recHitsEnd(); ++match) {
  DetId did=(*match)->geographicalId() ;
  if(did.det() == 2 && did.subdetId() == MuonSubdetId::DT)  {
    DTChamberId * id =  new DTChamberId(did);
    w0=id->wheel();
    s0=id->sector();
 
    delete id;
    break;
  }
 }
}

// ----------------------------------------------------------------------
float PixelTree::correctedTime(const  reco::Muon & aMuon) {
  int s0,w0;
  sectorAndWheel(aMuon,w0,s0);
  float time = -10000;
  if(fspoints[w0+2][s0]>20) time = aMuon.time().timeAtIpInOut-fsbias[w0+2][s0];
  return time;
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
  }

}


// ----------------------------------------------------------------------
// this has nothing to do with the PixelTree...
void PixelTree::dumpDetIds(const edm::EventSetup& iSetup) {

  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );

  cout << "**********************************************************************" << endl;
  cout << " *** Geometry node for TrackerGeom is  "<<&(*pDD)<<std::endl;
  cout << " *** I have " << pDD->dets().size() <<" detectors"<<std::endl;
  cout << " *** I have " << pDD->detTypes().size() <<" types"<<std::endl;
  
  for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){
    
    if(dynamic_cast<PixelGeomDetUnit*>((*it))!=0){
      DetId detId = (*it)->geographicalId();
      if (detId.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {

	PixelBarrelName::Shell DBshell = PixelBarrelName::PixelBarrelName(detId).shell();
	int DBlayer  = PixelBarrelName::PixelBarrelName(detId).layerName();
	int DBladder = PixelBarrelName::PixelBarrelName(detId).ladderName();
	int DBmodule = PixelBarrelName::PixelBarrelName(detId).moduleName();
	int DBsector = PixelBarrelName::PixelBarrelName(detId).sectorName();

	string quad; 
	if (DBshell == PixelBarrelName::mO) {
	  quad = string("BmO");
	}
	if (DBshell == PixelBarrelName::pO) {
	  quad = string("BpO");
	}
	if (DBshell == PixelBarrelName::mI) {
	  quad = string("BmI");
	}
	if (DBshell == PixelBarrelName::pI) {
	  quad = string("BpI");
	}
	
	cout << "BPix_" << quad << "_SEC" << DBsector << "_LYR" << DBlayer << "_LDR" << DBladder << "_MOD" << DBmodule << "  " << detId.rawId() << endl;
	
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

      }	else if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
	uint32_t id = detId();
       
        PixelEndcapName::HalfCylinder side = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).halfCylinder();
        int disk   = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).diskName();
        int blade  = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).bladeName();
        int panel  = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).pannelName();
        int module = PixelEndcapName::PixelEndcapName(DetId::DetId(id)).plaquetteName();


	string quad; 
	if (side == PixelEndcapName::mO) {
	  quad = string("BmO");
	}
	if (side == PixelEndcapName::pO) {
	  quad = string("BpO");
	}
	if (side == PixelEndcapName::mI) {
	  quad = string("BmI");
	}
	if (side == PixelEndcapName::pI) {
	  quad = string("BpI");
	}
	

	cout << "FPix_" << quad << "_D" << disk << "_BLD" << blade << "_PNL" << panel << "  " << detId.rawId() << endl;
	cout << "=FPix_" << quad << "_D" << disk << "_BLD" << blade << "_PNL" << panel << "_MOD" << module << "  " << detId.rawId() << endl;


	// 	cout << " ---> Adding Endcap Module " <<  detId.rawId() 
	// 	     << " at hc = " << side << " disk = " << disk << " blade = " << blade << " panel = " << panel << " module = " << module
	// 	     << endl;
	
        char sside[80];  sprintf(sside,  "HalfCylinder_%i",side);
        char sdisk[80];  sprintf(sdisk,  "Disk_%i",disk);
        char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
        char spanel[80]; sprintf(spanel, "Panel_%i",panel);
        char smodule[80];sprintf(smodule,"Module_%i",module);
        std::string side_str = sside;
	std::string disk_str = sdisk;
      }
    }
  }


}


// ----------------------------------------------------------------------
void PixelTree::bpixNames(const DetId &pID, int &DBlayer, int &DBladder, int &DBmodule) {
  PixelBarrelName::Shell DBshell = PixelBarrelName::PixelBarrelName(pID).shell();
  DBlayer  = PixelBarrelName::PixelBarrelName(pID).layerName();
  DBladder = PixelBarrelName::PixelBarrelName(pID).ladderName();
  DBmodule = PixelBarrelName::PixelBarrelName(pID).moduleName();
  
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

  PixelEndcapName::HalfCylinder DBside = PixelEndcapName::PixelEndcapName(pID).halfCylinder();
  DBdisk   = PixelEndcapName::PixelEndcapName(pID).diskName();
  DBblade  = PixelEndcapName::PixelEndcapName(pID).bladeName();
  DBpanel  = PixelEndcapName::PixelEndcapName(pID).pannelName();
  DBmodule = PixelEndcapName::PixelEndcapName(pID).plaquetteName();
  
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



// define this as a plug-in
DEFINE_FWK_MODULE(PixelTree);
