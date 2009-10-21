// ----------------------------------------------------------------------
// PixelTree
// ---------
// Summary: The full pixel information, including tracks and cross references
//          A lot has been copied from 
//            DPGAnalysis/SiPixelTools/plugins/PixelNtuplizer_RealData.cc
//            SiPixelMonitorTrack/src/SiPixelTrackResidualSource.cc
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

#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include <Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h>
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"



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
  fRootFileName(iConfig.getUntrackedParameter<string>("rootFileName", string("pixelTree.root"))),
  fTrajectoryInputLabel(iConfig.getUntrackedParameter<string>("trajectoryInputLabel", string("ctfRefitter"))),
  fMuonCollectionLabel(iConfig.getUntrackedParameter<string>("muonCollectionLabel", string("muons"))),
  fTrackCollectionLabel(iConfig.getUntrackedParameter<string>("trackCollectionLabel", string("ctfWithMaterialTracksP5"))),
  fPixelClusterLabel(iConfig.getUntrackedParameter<string>("pixelClusterLabel", string("siPixelClusters")))
{
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- PixelTree constructor" << endl;
  cout << "---  fMuonCollectionLabel:            " << fMuonCollectionLabel << endl;
  cout << "---  fTrajectoryInputLabel:           " << fTrajectoryInputLabel.c_str() << endl;
  cout << "---  fTrackCollectionLabel:           " << fTrackCollectionLabel << endl;
  cout << "---  fPixelClusterLabel:              " << fPixelClusterLabel << endl;
  cout << "----------------------------------------------------------------------" << endl;
}

// ----------------------------------------------------------------------
PixelTree::~PixelTree() { }  

// ----------------------------------------------------------------------
void PixelTree::endJob() { 
  fFile->cd();
  fTree->Write();
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
  fTree->Branch("event",        &fEvent,        "event/I");
  fTree->Branch("bz",           &fBz,           "bz/F");
  fTree->Branch("fed1",         &fFED1,         "fed1/I");
  fTree->Branch("fed2",         &fFED2,         "fed2/I");

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
  fTree->Branch("ClRow",        fClRow,         "ClRow[ClN]/I");
  fTree->Branch("ClCol",        fClCol,         "ClCol[ClN]/I");
  fTree->Branch("ClLx",         fClLx,          "ClLx[ClN]/F");
  fTree->Branch("ClLy",         fClLy,          "ClLy[ClN]/F");
  fTree->Branch("ClGx",         fClGx,          "ClGx[ClN]/F");
  fTree->Branch("ClGy",         fClGy,          "ClGy[ClN]/F");
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

  // -- produce list of detID and modules
  if (0) {
    if (0 == nevt) dumpDetIds(iSetup); 
  }

  ++nevt; 
  //  fH1->Fill(0.5);
  //  cout << "PixelTree event: " << nevt << endl;
  init();

  // -- Does this belong into beginJob()?
  ESHandle<TrackerGeometry> TG;
  iSetup.get<TrackerDigiGeometryRecord>().get(TG);
  const TrackerGeometry* theTrackerGeometry = TG.product();
 

  // ----------------------------------------------------------------------
  // -- Event information
  // ----------------------------------------------------------------------
  fRun   = iEvent.id().run();
  fEvent = iEvent.id().event();

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
  // -- Fill tracks/trajectories?
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

      // -- The measurements associated with a track
      std::vector<TrajectoryMeasurement> tmeasColl =refTraj->measurements();
      //      cout << " Found " << tmeasColl.size() << " measurements for this track " << endl;
      int iCluster(0); 
      for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = tmeasColl.begin(); tmeasIt!=tmeasColl.end(); tmeasIt++){   
	if (!tmeasIt->updatedState().isValid()) continue; 
	TrajectoryStateOnSurface tsos = tsoscomb(tmeasIt->forwardPredictedState(), tmeasIt->backwardPredictedState());
	TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt->recHit();
	if (!hit->isValid() || hit->geographicalId().det() != DetId::Tracker) {
	  continue; 
	} 

	const DetId &hit_detId = hit->geographicalId();
	uint IntSubDetID = (hit_detId.subdetId());
	if (IntSubDetID == 0) continue;
	const TrackingRecHit *persistentHit = hit->hit();
	if (persistentHit != 0) {
	  if (typeid(*persistentHit) != typeid(SiPixelRecHit))  continue;
	}
	
        align::LocalVector res = tsos.localPosition() - hit->localPosition();
        LocalError err1 = tsos.localError().positionError();
        LocalError err2 = hit->localPositionError();
        float errX = std::sqrt( err1.xx() + err2.xx() );
        float errY = std::sqrt( err1.yy() + err2.yy() );
        
	const SiPixelRecHit *pixhit = dynamic_cast<const SiPixelRecHit*>(hit->hit());
	edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const& clust = (*pixhit).cluster();
	if (clust.isNonnull()) {
	  clusterSet.insert(*clust);
	  const TrackerGeometry &theTracker(*theTrackerGeometry);
	  const PixelGeomDetUnit *theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(hit_detId) );
	  if (theGeomDet == 0) {
	    cout << "NO THEGEOMDET" << endl;
	    continue;
	  }


	  //const GeomDetUnit* detUnit = hit->detUnit();
	  const Surface &surface = hit->detUnit()->surface();
          LocalPoint lPModule(0.,0.,0.), lPhiDirection(1.,0.,0.), lROrZDirection(0.,1.,0.);
          GlobalPoint gPModule       = surface.toGlobal(lPModule),
            gPhiDirection  = surface.toGlobal(lPhiDirection),
            gROrZDirection = surface.toGlobal(lROrZDirection);
	  float dZ(-999.);
          float phiorientation = deltaPhi(gPhiDirection.phi(), gPModule.phi()) >= 0 ? +1. : -1.;
	    
	  const RectangularPixelTopology * topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	  
	  LocalTrajectoryParameters ltp = tsos.localParameters();
	  LocalVector localDir = ltp.momentum()/ltp.momentum().mag();
	  
	  float alpha = atan2(localDir.z(), localDir.x());
	  float beta = atan2(localDir.z(), localDir.y());
	  float clCharge = (clust->charge())/1000.0; // convert electrons to kilo-electrons
	  float clChargeCorr = clust->charge() * sqrt( 1.0 / ( 1.0/pow( tan(alpha), 2 ) + 
							     1.0/pow( tan(beta ), 2 ) + 
							     1.0 )
						     )/1000.;
  
	  //find cluster global position (rphi, z)
	  // get cluster center of gravity (of charge)
	  float xcenter = clust->x();
	  float ycenter = clust->y();
	  // get the cluster position in local coordinates (cm) 
	  LocalPoint clustlp = topol->localPosition( MeasurementPoint(xcenter, ycenter) );
	  // get the cluster position in global coordinates (cm)
	  GlobalPoint clustgp = theGeomDet->surface().toGlobal( clustlp );
	  
	  //find location of hit (barrel or endcap, same for cluster)
	  bool barrel = DetId::DetId((*hit).geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
	  //bool endcap = DetId::DetId((*hit).geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
	  
	  //uint32_t DBlayer = PixelBarrelName::PixelBarrelName(DetId::DetId((*hit).geographicalId())).layerName();
	  //float phi = clustgp.phi(); 
	  //float z = clustgp.z();

	  fClSize[fClN]   = clust->size();
	  fClSizeX[fClN]  = clust->sizeX();
	  fClSizeY[fClN]  = clust->sizeY();
	  fClRow[fClN]    = clust->x();
	  fClCol[fClN]    = clust->y();
	  
	  fClLx[fClN]      = clustlp.x();
	  fClLy[fClN]      = clustlp.y();
	  fClLxe[fClN]     = sqrt(err2.xx());  // FIXME: check this
	  fClLye[fClN]     = sqrt(err2.yy());
	  
	  fClGx[fClN] = clustgp.x();
	  fClGy[fClN] = clustgp.y();
	  fClGz[fClN] = clustgp.z();

	  if (barrel) {
	    
	    PixelBarrelName::Shell DBshell = PixelBarrelName::PixelBarrelName(DetId::DetId((*hit).geographicalId())).shell();
	    int DBlayer  = PixelBarrelName::PixelBarrelName(DetId::DetId((*hit).geographicalId())).layerName();
	    int DBladder = PixelBarrelName::PixelBarrelName(DetId::DetId((*hit).geographicalId())).ladderName();
	    int DBmodule = PixelBarrelName::PixelBarrelName(DetId::DetId((*hit).geographicalId())).moduleName();

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

	    fClLayer[fClN] = DBlayer;
	    fClLadder[fClN] = DBladder; 
	    fClModule[fClN] = DBmodule;

	  } else {

	    PixelEndcapName::HalfCylinder DBside = PixelEndcapName::PixelEndcapName(DetId::DetId((*hit).geographicalId())).halfCylinder();
	    int DBdisk   = PixelEndcapName::PixelEndcapName(DetId::DetId((*hit).geographicalId())).diskName();
	    int DBblade  = PixelEndcapName::PixelEndcapName(DetId::DetId((*hit).geographicalId())).bladeName();
	    int DBpanel  = PixelEndcapName::PixelEndcapName(DetId::DetId((*hit).geographicalId())).pannelName();
	    int DBmodule = PixelEndcapName::PixelEndcapName(DetId::DetId((*hit).geographicalId())).plaquetteName();

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

	    fClDisk[fClN]      =  DBdisk;
	    fClBlade[fClN]     =  DBblade;
	    fClPanel[fClN]     =  DBpanel;
	    fClPlaquette[fClN] =  DBmodule;
	  }

	  // Is flipped ?
	  float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
	  float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
	  int isFlipped(0); 
	  if (tmp2 < tmp1) isFlipped = 1;
	  else isFlipped = 0;

	  fClFlipped[fClN] = isFlipped;

	  fClCharge[fClN]     = clCharge;
	  fClChargeCorr[fClN] = clChargeCorr;

	  fClType[fClN]    = 1; 
	  fClTkI[fClN]     = fTkN;  // ?? FIXME: A cluster can belong to more than one track
	  
	  if (iCluster < CLPERTRACKMAX) {
	    fTkResX[fTkN][iCluster]   = res.x();
	    fTkResXe[fTkN][iCluster]  = errX;
	    fTkRes2X[fTkN][iCluster]  = (res.x())*phiorientation;
	    fTkRes2Xe[fTkN][iCluster] = errX;

	    // FIXME: Understand the following: 
	    float resy(0.);
	    dZ = gROrZDirection.z() - gPModule.z();
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



  // -- Clusters without tracks
  for (TrackerGeometry::DetContainer::const_iterator it = TG->dets().begin(); it != TG->dets().end(); it++){
    if (dynamic_cast<PixelGeomDetUnit*>((*it)) != 0){ 
      DetId detId = (*it)->geographicalId();

      //int nofclOnTrack = 0, nofclOffTrack=0; 
      uint32_t DBlayer=10, DBdisk=10; 
      //float z=0.; 
      //set layer/disk
      if (DetId::DetId(detId).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
	DBlayer = PixelBarrelName::PixelBarrelName(DetId::DetId(detId)).layerName();
      }
      if (DetId::DetId(detId).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
	DBdisk = PixelEndcapName::PixelEndcapName(DetId::DetId(detId )).diskName();
      }
      edmNew::DetSetVector<SiPixelCluster>::const_iterator isearch = clustColl.find(detId);
      if (isearch != clustColl.end()) {  // Not an empty iterator
	edmNew::DetSet<SiPixelCluster>::const_iterator  di;
	for (di=isearch->begin(); di!=isearch->end(); di++) {
	  unsigned int temp = clusterSet.size();
	  clusterSet.insert(*di);
	  //check if cluster is off track
	  if (clusterSet.size()>temp) {
	    // 	    cout << "Found a cluster that is not on a track" << endl;

	    //const DetId &hit_detId = di->geographicalId();
	    //uint IntSubDetID = (hit_detId.subdetId());

	    /////////////////////////////////////////////////
	    //find cluster global position (rphi, z) get cluster
	    //define tracker and pixel geometry and topology
	    const TrackerGeometry& theTracker(*theTrackerGeometry);
	    const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*> (theTracker.idToDet(detId) );
	    //test if PixelGeomDetUnit exists
	    if (theGeomDet == 0) {
	      cout << "NO THEGEOMDET" << endl;
	      continue;
	    }
	    const RectangularPixelTopology *topol = dynamic_cast<const RectangularPixelTopology*>(&(theGeomDet->specificTopology()));
	    
	    // -- find location of hit (barrel or endcap, same for cluster)
	    bool barrel = DetId::DetId((*di).geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
	    //bool endcap = DetId::DetId((*di).geographicalId()).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);

	   
	    //center of gravity (of charge)
	    float xcenter = di->x();
	    float ycenter = di->y();
	    // get the cluster position in local coordinates (cm) 
	    LocalPoint clustlp = topol->localPosition( MeasurementPoint(xcenter, ycenter) );
	    // get the cluster position in global coordinates (cm)
	    GlobalPoint clustgp = theGeomDet->surface().toGlobal( clustlp );

	    /////////////////////////////////////////////////

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

	    if (barrel) {
	      PixelBarrelName::Shell DBshell = PixelBarrelName::PixelBarrelName(DetId::DetId((*di).geographicalId())).shell();
	      int DBlayer  = PixelBarrelName::PixelBarrelName(DetId::DetId((*di).geographicalId())).layerName();
	      int DBladder = PixelBarrelName::PixelBarrelName(DetId::DetId((*di).geographicalId())).ladderName();
	      int DBmodule = PixelBarrelName::PixelBarrelName(DetId::DetId((*di).geographicalId())).moduleName();
	      
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

	      fClLayer[fClN] = DBlayer;
	      fClLadder[fClN] = DBladder; 
	      fClModule[fClN] = DBmodule;
	    } else {
	      PixelEndcapName::HalfCylinder DBside = PixelEndcapName::PixelEndcapName(DetId::DetId((*di).geographicalId())).halfCylinder();
	      int DBdisk   = PixelEndcapName::PixelEndcapName(DetId::DetId((*di).geographicalId())).diskName();
	      int DBblade  = PixelEndcapName::PixelEndcapName(DetId::DetId((*di).geographicalId())).bladeName();
	      int DBpanel  = PixelEndcapName::PixelEndcapName(DetId::DetId((*di).geographicalId())).pannelName();
	      int DBmodule = PixelEndcapName::PixelEndcapName(DetId::DetId((*di).geographicalId())).plaquetteName();
	      
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
	      
	      fClDisk[fClN]      =  DBdisk;
	      fClBlade[fClN]     =  DBblade;
	      fClPanel[fClN]     =  DBpanel;
	      fClPlaquette[fClN] =  DBmodule;
	    }
	    
	    // Is flipped ?
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
	    // 	    cout << "  Found " << pixvector.size() << " pixels for this off-track cluster " << endl;
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
	    

	    // 	    cout << "Cluster[" << fClN << "] digi indices: "; 
	    // 	    for (int iq = 0; iq < 20; ++iq) {
	    // 	      if (fClDgI[fClN][iq] < -97) break;
	    // 	      cout << fClDgI[fClN][iq] << "(" << fDgClI[fClDgI[fClN][iq]] << ") ";
	    // 	    }
	    // 	    cout << endl;

	    ++fClN;

	  }
	}
      }
    }
  }

  // -- That's it
  cout << nevt << "(" << fRun << "/" << fEvent << ")" 
       << " MuN: " << fMuN << " TkN: " << fTkN << " ClN: " << fClN << " DgN: " << fDgN 
       << " FEDs: " << nbpixfeds << "/" << nfpixfeds
       << endl;
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
      //const GeomDetUnit      * geoUnit = pDD->idToDetUnit( detId );
      //const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
      //int nrows = (pixDet->specificTopology()).nrows();
      //int ncols = (pixDet->specificTopology()).ncolumns();

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


// define this as a plug-in
DEFINE_FWK_MODULE(PixelTree);
