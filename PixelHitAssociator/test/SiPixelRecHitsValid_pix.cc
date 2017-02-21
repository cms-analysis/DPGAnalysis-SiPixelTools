
// SiPixelRecHitsValid.cc
// Description: see SiPixelRecHitsValid.h
// Author: Jason Shaev, JHU
// Created 6/7/06
//
// Make standalone, independent from the validation code. dk 3/14
// Add option for on-track hits 29/Oct/2016 Janos Karancsi
//--------------------------------

#include "SiPixelRecHitsValid_pix.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include <math.h>

#include "DQMServices/Core/interface/DQMStore.h"

//#define QUICK

using namespace std;
using namespace edm;


SiPixelRecHitsValid_pix::SiPixelRecHitsValid_pix(const ParameterSet& ps): 
  dbe_(0), 
  conf_(ps),
  trackerHitAssociatorConfig_(ps, consumesCollector() ),
  src_( ps.getParameter<edm::InputTag>( "src" ) ),
  useTracks_( ps.getUntrackedParameter<bool>( "useTracks", false ) ),
  tracks_( ps.getUntrackedParameter<edm::InputTag>( "tracks", edm::InputTag("generalTracks") ) )  {
  if (useTracks_) {
    tTracks = consumes<TrajTrackAssociationCollection>( tracks_ );
  } else {
    tPixelRecHit = consumes<edmNew::DetSetVector<SiPixelRecHit>>( src_ );
  }

  outputFile_ = ps.getUntrackedParameter<string>("outputFile", "pixelrechitshisto.root");
  verbose_ = ps.getUntrackedParameter<bool>("verbose", false);

#ifdef PIXEL_ASSOCIATOR
  cout<<" Compare Sim-Rec pixel hits, using pixel hit associator "<<endl;
#else
  cout<<" Compare Sim-Rec pixel hits, using tracker hit associator "<<endl;
#endif
  cout<<" source = "<<(useTracks_? tracks_ : src_)<<endl; //dk

  dbe_ = Service<DQMStore>().operator->();
  //dbe_->showDirStructure();
  dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/clustBPIX");  
  Char_t histo[200];
#ifdef QUICK
  const bool quick=true;
#else
  const bool quick=false;
#endif

  // ---------------------------------------------------------------
  // All histograms that depend on plaquette number have 7 indexes.
  // The first 4 (0-3) correspond to Panel 1 plaquettes 1-4.
  // The last 3 (4-6) correspond to Panel 2 plaquettes 1-3.
  // ---------------------------------------------------------------
  const int NumLayers=4;
  //const int NumDisks=3;


  if(!quick) {

    //heta = dbe_->book1D("heta","eta1",50,-2.5,2.5);
    hphi1 = dbe_->book1D("hphi1","phi1",70,-3.5,3.5);
    htheta1 = dbe_->book1D("htheta1","theta1",70,-3.5,3.5);
    hbeta1  = dbe_->book1D("beta1","beta1",70,-3.5,3.5);

    hphi2 = dbe_->book1D("hphi2","phi2",70,-3.5,3.5);
    htheta2 = dbe_->book1D("htheta2","theta2",70,-3.5,3.5);
    hbeta2  = dbe_->book1D("beta2","beta2",70,-3.5,3.5);

    heta1 = dbe_->book1D("heta1","eta",50,-2.5,2.5);
    heta2 = dbe_->book1D("heta2","eta",50,-2.5,2.5);
    heta3 = dbe_->book1D("heta3","eta",50,-2.5,2.5);
    
    htest1 = dbe_->book2D("htest1","test1",50,0.,2.5,70,0.,3.5);
    htest2 = dbe_->book2D("htest2","test2",50,0.,2.5,70,0.,3.5);
    
    //Cluster y-size by module number for barrel
    for (int i=0; i<8; i++) {
      sprintf(histo, "Clust_y_size_Module%d", i+1);
      clustYSizeModule[i] = dbe_->book1D(histo,"Cluster y-size by Module", 20, 0.5, 20.5); 
    } // end for
    
    //Cluster x-size by layer for barrel
    //for (int i=0; i<NumLayers; i++) {
    //sprintf(histo, "Clust_x_size_Layer%d", i+1);
    //clustXSizeLayer[i] = dbe_->book1D(histo,"Cluster x-size by Layer", 20, 0.5, 20.5);
    //} // end for
    
    //Cluster charge by module for 3 layers of barrel
    for (int i=0; i<8; i++) {
      //Cluster charge by module for Layer1
      sprintf(histo, "Clust_charge_Layer1_Module%d", i+1);
      clustChargeLayer1Modules[i] = dbe_->book1D(histo, "Cluster charge Layer 1 by Module", 50, 0., 200000.);
      
      //Cluster charge by module for Layer2
      sprintf(histo, "Clust_charge_Layer2_Module%d", i+1);
      clustChargeLayer2Modules[i] = dbe_->book1D(histo, "Cluster charge Layer 2 by Module", 50, 0., 200000.);
      
      //Cluster charge by module for Layer3
      sprintf(histo, "Clust_charge_Layer3_Module%d", i+1);
      clustChargeLayer3Modules[i] = dbe_->book1D(histo, "Cluster charge Layer 3 by Module",50, 0., 200000.);	
    } // end for
    
    dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/clustFPIX");
    //Cluster x-size, y-size, and charge by plaquette for Disks in Forward
    for (int i=0; i<7; i++) {
      //Cluster x-size for Disk1 by Plaquette
      sprintf(histo, "Clust_x_size_Disk1_Plaquette%d", i+1);
      clustXSizeDisk1Plaquettes[i] = dbe_->book1D(histo, "Cluster X-size for Disk1 by Plaquette", 20, 0.5, 20.5);
      
      //Cluster x-size for Disk2 by Plaquette
      sprintf(histo, "Clust_x_size_Disk2_Plaquette%d", i+1);
      clustXSizeDisk2Plaquettes[i] = dbe_->book1D(histo, "Cluster X-size for Disk2 by Plaquette", 20, 0.5, 20.5);
      
      //Cluster x-size for Disk3 by Plaquette
      sprintf(histo, "Clust_x_size_Disk3_Plaquette%d", i+1);
      clustXSizeDisk3Plaquettes[i] = dbe_->book1D(histo, "Cluster X-size for Disk3 by Plaquette", 20, 0.5, 20.5);
      
      //Cluster y-size for Disk1 by Plaquette
      sprintf(histo, "Clust_y_size_Disk1_Plaquette%d", i+1);
      clustYSizeDisk1Plaquettes[i] = dbe_->book1D(histo, "Cluster Y-size for Disk1 by Plaquette", 20, 0.5, 20.5);
      
      //Cluster y-size for Disk2 by Plaquette
      sprintf(histo, "Clust_y_size_Disk2_Plaquette%d", i+1);
      clustYSizeDisk2Plaquettes[i] = dbe_->book1D(histo, "Cluster Y-size for Disk2 by Plaquette", 20, 0.5, 20.5);
      
      //Cluster y-size for Disk3 by Plaquette
      sprintf(histo, "Clust_y_size_Disk3_Plaquette%d", i+1);
      clustYSizeDisk3Plaquettes[i] = dbe_->book1D(histo, "Cluster Y-size for Disk3 by Plaquette", 20, 0.5, 20.5);
      
      //Cluster charge for Disk1 by Plaquette
      sprintf(histo, "Clust_charge_Disk1_Plaquette%d", i+1);
      clustChargeDisk1Plaquettes[i] = dbe_->book1D(histo, "Cluster charge for Disk1 by Plaquette", 50, 0., 200000.);
      
      //Cluster charge for Disk2 by Plaquette
      sprintf(histo, "Clust_charge_Disk2_Plaquette%d", i+1);
      clustChargeDisk2Plaquettes[i] = dbe_->book1D(histo, "Cluster charge for Disk2 by Plaquette", 50, 0., 200000.);
      
      //Cluster charge for Disk3 by Plaquette
      sprintf(histo, "Clust_charge_Disk3_Plaquette%d", i+1);
      clustChargeDisk3Plaquettes[i] = dbe_->book1D(histo, "Cluster charge for Disk3 by Plaquette", 50, 0., 200000.);
    } // end for
    
  } // end if quick
  
  dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/recHitBPIX");

  //RecHit X resolution for flipped and unflipped ladders by layer for barrel
  for (int i=0; i<NumLayers; i++) {
    //RecHit X resolution per layer
    sprintf(histo, "RecHit_XRes_Layer%d", i+1);
    recHitXResLayers[i] = dbe_->book1D(histo, "RecHit XRes by Layer", 100, -200., 200.);
    
    //RecHit Y resolution per layer
    sprintf(histo, "RecHit_YRes_Layer%d", i+1);
    recHitYResLayers[i] = dbe_->book1D(histo, "RecHit YRes by Layer", 100, -200., 200.);
    
    if(!quick) {

      // special histos for layer 1
      recHitL1XResSize1    = dbe_->book1D("recHitL1XSize1",      "XRes size 1 L1", 100, -200., 200.);   
      recHitL1XResSize2    = dbe_->book1D("recHitL1XSize2",      "XRes size 2 L1", 100, -200., 200.);   
      recHitL1XResSize3    = dbe_->book1D("recHitL1XSize3",      "XRes size 3 L1", 100, -200., 200.);   
      recHitL2XResSize1    = dbe_->book1D("recHitL2XSize1",      "XRes size 1 L2", 100, -200., 200.);   
      recHitL2XResSize2    = dbe_->book1D("recHitL2XSize2",      "XRes size 2 L2", 100, -200., 200.);   
      recHitL2XResSize3    = dbe_->book1D("recHitL2XSize3",      "XRes size 3 L2", 100, -200., 200.);   

      // alignment errors
      recHitXAlignError1 = 
	dbe_->book1D("RecHitXAlignError1","RecHit X  Alignment errors bpix 1", 100, 0., 100.);
      recHitXAlignError2 = 
	dbe_->book1D("RecHitXAlignError2","RecHit X  Alignment errors bpix 2", 100, 0., 100.);
      recHitXAlignError3 = 
	dbe_->book1D("RecHitXAlignError3","RecHit X  Alignment errors bpix 3", 100, 0., 100.);
      recHitYAlignError1 = 
	dbe_->book1D("RecHitYAlignError1","RecHit Y  Alignment errors bpix 1", 100, 0., 100.);
      recHitYAlignError2 = 
	dbe_->book1D("RecHitYAlignError2","RecHit Y  Alignment errors bpix 2", 100, 0., 100.);
      recHitYAlignError3 = 
	dbe_->book1D("RecHitYAlignError3","RecHit Y  Alignment errors bpix 3", 100, 0., 100.);

      //RecHit X Resolution all barrel hits
      recHitXResAllB = dbe_->book1D("RecHit_xres_b_All","RecHit X Res All Modules in Barrel", 100, -200., 200.);
      
      //RecHit Y Resolution all barrel hits
      recHitYResAllB = dbe_->book1D("RecHit_yres_b_All","RecHit Y Res All Modules in Barrel", 100, -200., 200.);
      
      //RecHit X distribution for full modules for barrel
      //recHitXFullModules = dbe_->book1D("RecHit_x_FullModules", "RecHit X distribution for full modules", 100,-2., 2.);
      
      //RecHit X distribution for half modules for barrel
      //recHitXHalfModules = dbe_->book1D("RecHit_x_HalfModules", "RecHit X distribution for half modules", 100, -1., 1.);
      
      //RecHit Y distribution all modules for barrel
      recHitYAllModules = dbe_->book1D("RecHit_y_AllModules", "RecHit Y distribution for all modules", 100, -4., 4.);
      
      // eta plots
      for (int j=0; j<25; j++) {
	//RecHit X resolution per layer
	sprintf(histo, "RecHit_XRes_Layer1_Eta%d", j+1);
	recHitXResLayer1Eta[j] = dbe_->book1D(histo, "RecHit XRes Layer1, eta", 100, -200., 200.);
	sprintf(histo, "RecHit_XRes_Layer2_Eta%d", j+1);
	recHitXResLayer2Eta[j] = dbe_->book1D(histo, "RecHit XRes Layer2, eta", 100, -200., 200.);
	sprintf(histo, "RecHit_XRes_Layer3_Eta%d", j+1);
	recHitXResLayer3Eta[j] = dbe_->book1D(histo, "RecHit XRes Layer3, eta", 100, -200., 200.);
	
	//RecHit Y resolution per layer
	sprintf(histo, "RecHit_YRes_Layer1_Eta%d", j+1);
	recHitYResLayer1Eta[j] = dbe_->book1D(histo, "RecHit YRes Layer1, eta", 100, -200., 200.);
	sprintf(histo, "RecHit_YRes_Layer2_Eta%d", j+1);
	recHitYResLayer2Eta[j] = dbe_->book1D(histo, "RecHit YRes Layer2, eta", 100, -200., 200.);
	sprintf(histo, "RecHit_YRes_Layer3_Eta%d", j+1);
	recHitYResLayer3Eta[j] = dbe_->book1D(histo, "RecHit YRes Layer3, eta", 100, -200., 200.);
      }
      


      //RecHit X resolution per layer Profile 
      sprintf(histo, "RecHit_XRes_LayerP%d", i+1);
      recHitXResLayersP[i] = dbe_->bookProfile(histo, "RecHit XRes by Layer", 25, 0., 2.5,0.,1000.," ");
      sprintf(histo, "RecHit_XRes_LayerP1%d", i+1);
      recHitXResLayersP1[i] = dbe_->bookProfile(histo, "RecHit XRes by Layer, size 1", 25, 0., 2.5,0.,1000.," ");
      sprintf(histo, "RecHit_XRes_LayerP2%d", i+1);
      recHitXResLayersP2[i] = dbe_->bookProfile(histo, "RecHit XRes by Layer, size 2", 25, 0., 2.5,0.,1000.," ");
      sprintf(histo, "RecHit_XRes_LayerP3%d", i+1);
      recHitXResLayersP3[i] = dbe_->bookProfile(histo, "RecHit XRes by Layer, size 3", 25, 0., 2.5,0.,1000.," ");
      
      //RecHit Y resolution per layer Profile 
      sprintf(histo, "RecHit_YRes_LayerP%d", i+1);
      recHitYResLayersP[i] = dbe_->bookProfile(histo, "RecHit YRes by Layer", 25, 0., 2.5,0.,1000.," ");
      sprintf(histo, "RecHit_YRes_LayerP1%d", i+1);
      recHitYResLayersP1[i] = dbe_->bookProfile(histo, "RecHit YRes by Layer, size 1", 25, 0., 2.5,0.,1000.," ");
      sprintf(histo, "RecHit_YRes_LayerP2%d", i+1);
      recHitYResLayersP2[i] = dbe_->bookProfile(histo, "RecHit YRes by Layer, size 2", 25, 0., 2.5,0.,1000.," ");
      sprintf(histo, "RecHit_YRes_LayerP3%d", i+1);
      recHitYResLayersP3[i] = dbe_->bookProfile(histo, "RecHit YRes by Layer, size 3", 25, 0., 2.5,0.,1000.," ");
      sprintf(histo, "RecHit_YRes_LayerP4%d", i+1);
      recHitYResLayersP4[i] = dbe_->bookProfile(histo, "RecHit YRes by Layer, size 4", 25, 0., 2.5,0.,1000.," ");
      
      recHitX11 = dbe_->bookProfile("recHitX11", "RecHit", 100, 0.5, 2.5,0.,100.," ");
      recHitX12 = dbe_->bookProfile("recHitX12", "RecHit", 100, 0.5, 2.5,0.,100.," ");
      recHitX21 = dbe_->bookProfile("recHitX21", "RecHit", 100, 0.5, 2.5,0.,100.," ");
      recHitX22 = dbe_->bookProfile("recHitX22", "RecHit", 100, 0.5, 2.5,0.,100.," ");
      
      //RecHit X resolution for flipped ladders by layer
      sprintf(histo, "RecHit_XRes_FlippedLadder_Layer%d", i+1);
      recHitXResFlippedLadderLayers[i] = dbe_->book1D(histo, "RecHit XRes Flipped Ladders by Layer", 100, -200., 200.);
      
      //RecHit X resolution for unflipped ladders by layer
      sprintf(histo, "RecHit_XRes_NonFlippedLadder_Layer%d", i+1);
      recHitXResNonFlippedLadderLayers[i] = dbe_->book1D(histo, "RecHit XRes NonFlipped Ladders by Layer", 100, -200., 200.);

      // Same plots for -Z/+Z
      sprintf(histo, "RecHit_XRes_FlippedLadder_Layer%d_mZ", i+1);
      recHitXResFlippedLadderLayersSide[i][0] = dbe_->book1D(histo, "RecHit XRes Flipped Ladders by Layer, -Z", 100, -200., 200.);
      sprintf(histo, "RecHit_XRes_NonFlippedLadder_Layer%d_mZ", i+1);
      recHitXResNonFlippedLadderLayersSide[i][0] = dbe_->book1D(histo, "RecHit XRes NonFlipped Ladders by Layer, -Z", 100, -200., 200.);
      sprintf(histo, "RecHit_XRes_FlippedLadder_Layer%d_pZ", i+1);
      recHitXResFlippedLadderLayersSide[i][1] = dbe_->book1D(histo, "RecHit XRes Flipped Ladders by Layer, +Z", 100, -200., 200.);
      sprintf(histo, "RecHit_XRes_NonFlippedLadder_Layer%d_pZ", i+1);
      recHitXResNonFlippedLadderLayersSide[i][1] = dbe_->book1D(histo, "RecHit XRes NonFlipped Ladders by Layer, +Z", 100, -200., 200.);

      
      //RecHit Y resolutions for layers by module for barrel
      for (int i=0; i<8; i++) {
	//Rec Hit Y resolution by module for Layer1
	sprintf(histo, "RecHit_YRes_Layer1_Module%d", i+1);
	recHitYResLayer1Modules[i] = dbe_->book1D(histo, "RecHit YRes Layer1 by module", 100, -200., 200.);
	
	//RecHit Y resolution by module for Layer2
	sprintf(histo, "RecHit_YRes_Layer2_Module%d", i+1);
	recHitYResLayer2Modules[i] = dbe_->book1D(histo, "RecHit YRes Layer2 by module", 100, -200., 200.);
	
	//RecHit Y resolution by module for Layer3
	sprintf(histo, "RecHit_YRes_Layer3_Module%d", i+1);
	recHitYResLayer3Modules[i] = dbe_->book1D(histo, "RecHit YRes Layer3 by module", 100, -200., 200.); 
	
	//RecHit Y resolution by module for Layer4
	sprintf(histo, "RecHit_YRes_Layer4_Module%d", i+1);
	recHitYResLayer4Modules[i] = dbe_->book1D(histo, "RecHit YRes Layer4 by module", 100, -200., 200.); 
      } // end for
      
    } // end if quick
  } // end for layers 

  dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/recHitFPIX");
  //RecHit X resolution all plaquettes
  recHitXResAllF = dbe_->book1D("RecHit_xres_f_All", "RecHit X Res All in Forward", 100, -200., 200.);
  recHitXResPosZF = dbe_->book1D("RecHit_xres_f_posZ", "RecHit X Res All in Forward Disks +Z", 100, -200., 200.);
  recHitXResNegZF = dbe_->book1D("RecHit_xres_f_negZ", "RecHit X Res All in Forward Disks -Z", 100, -200., 200.);
  
  //RecHit Y resolution all plaquettes
  recHitYResAllF = dbe_->book1D("RecHit_yres_f_All", "RecHit Y Res All in Forward", 100, -200., 200.);
  recHitYResPosZF = dbe_->book1D("RecHit_yres_f_posZ", "RecHit Y Res All in Forward Disks +Z", 100, -200., 200.);
  recHitYResNegZF = dbe_->book1D("RecHit_yres_f_negZ", "RecHit Y Res All in Forward Disks -Z", 100, -200., 200.);

  // histograms per disk, side, panel, ring  
  for (int i=0; i<7; i++) { // 0 all, 1 -z, 2 +z, 3 panel 1, 4 panel 2, 5 ring 1, 6 ring 2
    //X resolution for Disk1 
    sprintf(histo, "RecHit_XRes_Disk1%d", i+1);
    recHitXResDisk1[i] = dbe_->book1D(histo, "RecHit XRes Disk1", 100, -200., 200.); 
    //X resolution for Disk2 
    sprintf(histo, "RecHit_XRes_Disk2%d", i+1);
    recHitXResDisk2[i] = dbe_->book1D(histo, "RecHit XRes Disk2", 100, -200., 200.);  
    //X resolution for Disk=3 
    sprintf(histo, "RecHit_XRes_Disk3%d", i+1);
    recHitXResDisk3[i] = dbe_->book1D(histo, "RecHit XRes Disk3", 100, -200., 200.);  
    
    //Y resolution for Disk1
    sprintf(histo, "RecHit_YRes_Disk1%d", i+1);
    recHitYResDisk1[i] = dbe_->book1D(histo, "RecHit YRes Disk1", 100, -200., 200.);
    //Y resolution for Disk2 
    sprintf(histo, "RecHit_YRes_Disk2%d", i+1);
    recHitYResDisk2[i] = dbe_->book1D(histo, "RecHit YRes Disk2", 100, -200., 200.);
    //Y resolution for Disk3 
    sprintf(histo, "RecHit_YRes_Disk3%d", i+1);
    recHitYResDisk3[i] = dbe_->book1D(histo, "RecHit YRes Disk3", 100, -200., 200.);
  }

  if(!quick) {
    // jk 18 Feb 2017
    for (int ring=0; ring<2; ring++) {
      for (int side=0; side<2; side++) for (int panel=0; panel<2; panel++) {
	sprintf(histo, "RecHit_XRes_Ring%d_Side%d_Panel%d", ring+1, side+1, panel+1);
	recHitXResRingSidePanel[ring][side][panel] = dbe_->book1D(histo, histo, 100, -200., 200.); 
	sprintf(histo, "RecHit_YRes_Ring%d_Side%d_Panel%d", ring+1, side+1, panel+1);
	recHitYResRingSidePanel[ring][side][panel] = dbe_->book1D(histo, histo, 100, -200., 200.);
      }
      for (int side=0; side<2; side++) {
	sprintf(histo, "RecHit_XRes_Ring%d_Side%d", ring+1, side+1);
	recHitXResRingSide[ring][side] = dbe_->book1D(histo, histo, 100, -200., 200.); 
      }
      for (int panel=0; panel<2; panel++) {
	sprintf(histo, "RecHit_YRes_Ring%d_Panel%d", ring+1, panel+1);
	recHitYResRingPanel[ring][panel] = dbe_->book1D(histo, histo, 100, -200., 200.);
      }
    }

    recHitXAlignError4 = 
      dbe_->book1D("RecHitXAlignError4","RecHit X  Alignment errors fpix 1", 100, 0., 100.);
    recHitXAlignError5 = 
      dbe_->book1D("RecHitXAlignError5","RecHit X  Alignment errors fpix 2", 100, 0., 100.);
    recHitYAlignError4 = 
      dbe_->book1D("RecHitYAlignError4","RecHit Y  Alignment errors fpix 1", 100, 0., 100.);
    recHitYAlignError5 = 
      dbe_->book1D("RecHitYAlignError5","RecHit Y  Alignment errors fpix 2", 100, 0., 100.);
    recHitXAlignError6 = 
      dbe_->book1D("RecHitXAlignError6","RecHit X  Alignment errors fpix 1", 100, 0., 100.);
    recHitXAlignError7 = 
      dbe_->book1D("RecHitXAlignError7","RecHit X  Alignment errors fpix 2", 100, 0., 100.);
    recHitYAlignError6 = 
      dbe_->book1D("RecHitYAlignError6","RecHit Y  Alignment errors fpix 1", 100, 0., 100.);
    recHitYAlignError7 = 
      dbe_->book1D("RecHitYAlignError7","RecHit Y  Alignment errors fpix 2", 100, 0., 100.);

    test = 
      dbe_->book2D("test","Alignment errors fpix", 100, 0., 100.,20,12.,32.);
    
  //RecHit X distribution for plaquette with x-size 1 in forward
  // recHitXPlaquetteSize1 = dbe_->book1D("RecHit_x_Plaquette_xsize1", "RecHit X Distribution for plaquette x-size1", 100, -2., 2.);
  
  // //RecHit X distribution for plaquette with x-size 2 in forward
  // recHitXPlaquetteSize2 = dbe_->book1D("RecHit_x_Plaquette_xsize2", "RecHit X Distribution for plaquette x-size2", 100, -2., 2.);
  
  // //RecHit Y distribution for plaquette with y-size 2 in forward
  // recHitYPlaquetteSize2 = dbe_->book1D("RecHit_y_Plaquette_ysize2", "RecHit Y Distribution for plaquette y-size2", 100, -4., 4.);
  
  // //RecHit Y distribution for plaquette with y-size 3 in forward
  // recHitYPlaquetteSize3 = dbe_->book1D("RecHit_y_Plaquette_ysize3", "RecHit Y Distribution for plaquette y-size3", 100, -4., 4.);
  
  // //RecHit Y distribution for plaquette with y-size 4 in forward
  // recHitYPlaquetteSize4 = dbe_->book1D("RecHit_y_Plaquette_ysize4", "RecHit Y Distribution for plaquette y-size4", 100, -4., 4.);
  
  // //RecHit Y distribution for plaquette with y-size 5 in forward
  // recHitYPlaquetteSize5 = dbe_->book1D("RecHit_y_Plaquette_ysize5", "RecHit Y Distribution for plaquette y-size5", 100, -4., 4.);
  
  //X and Y resolutions for both disks by plaquette in forward

  } // end if quick 

  if(!quick) {
    // PULLS
  dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/recHitPullsBPIX");
  recHitXPullAllB = dbe_->book1D("RecHit_xpull_b_All"       , 
				 "RecHit X Pull All Modules in Barrel"        , 100, -10.0, 10.0);
  recHitYPullAllB = dbe_->book1D("RecHit_ypull_b_All"       , 
				 "RecHit Y Pull All Modules in Barrel"        , 100, -10.0, 10.0);
  //recHitXErrorAllB = dbe_->book1D("RecHit_xerr_b_All"       , 
  //				 "RecHit X Error All Modules in Barrel"        , 100, 0, 0.1);
  //recHitYErrorAllB = dbe_->book1D("RecHit_yerr_b_All"       , 
  //				 "RecHit Y Error All Modules in Barrel"        , 100, 0, 0.1);
  recHitXError1B = dbe_->book1D("recHitXError1B"       , 
				 "RecHit X Error in Barrel Layer 1"        , 100, 0, 0.1);
  recHitYError1B = dbe_->book1D("recHitYError1B"       , 
				 "RecHit Y Error in Barrel Layer 1"        , 100, 0, 0.1);
  recHitXError2B = dbe_->book1D("recHitXError2B"       , 
				 "RecHit X Error in Barrel Layer 2"        , 100, 0, 0.1);
  recHitYError2B = dbe_->book1D("recHitYError2B"       , 
				 "RecHit Y Error in Barrel Layer 2"        , 100, 0, 0.1);
  recHitXError3B = dbe_->book1D("recHitXError3B"       , 
				 "RecHit X Error in Barrel Layer 3"        , 100, 0, 0.1);
  recHitYError3B = dbe_->book1D("recHitYError3B"       , 
				 "RecHit Y Error in Barrel Layer 3"        , 100, 0, 0.1);

  for (int i=0; i<NumLayers; i++) 
    {
      sprintf(histo, "RecHit_XPull_FlippedLadder_Layer%d", i+1);
      recHitXPullFlippedLadderLayers[i] = dbe_->book1D(histo, "RecHit XPull Flipped Ladders by Layer", 100, -10.0, 10.0);
      
      sprintf(histo, "RecHit_XPull_NonFlippedLadder_Layer%d", i+1);
      recHitXPullNonFlippedLadderLayers[i] = dbe_->book1D(histo, "RecHit XPull NonFlipped Ladders by Layer", 100, -10.0, 10.0);
    }
  
  for (int i=0; i<8; i++) 
    {
      sprintf(histo, "RecHit_YPull_Layer1_Module%d", i+1);
      recHitYPullLayer1Modules[i] = dbe_->book1D(histo, "RecHit YPull Layer1 by module", 100, -10.0, 10.0);
      
      sprintf(histo, "RecHit_YPull_Layer2_Module%d", i+1);
      recHitYPullLayer2Modules[i] = dbe_->book1D(histo, "RecHit YPull Layer2 by module", 100, -10.0, 10.0);
      
      sprintf(histo, "RecHit_YPull_Layer3_Module%d", i+1);
      recHitYPullLayer3Modules[i] = dbe_->book1D(histo, "RecHit YPull Layer3 by module", 100, -10.0, 10.0); 

      sprintf(histo, "RecHit_YPull_Layer4_Module%d", i+1);
      recHitYPullLayer4Modules[i] = dbe_->book1D(histo, "RecHit YPull Layer4 by module", 100, -10.0, 10.0); 
    }
  
  dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/recHitPullsFPIX");
  recHitXPullAllF = dbe_->book1D("RecHit_XPull_f_All", "RecHit X Pull All in Forward", 100, -10.0, 10.0);  
  recHitYPullAllF = dbe_->book1D("RecHit_YPull_f_All", "RecHit Y Pull All in Forward", 100, -10.0, 10.0);
  recHitXErrorAllF = dbe_->book1D("RecHit_XError_f_All", "RecHit X Error All in Forward", 100, 0.0, 0.1);  
  recHitYErrorAllF = dbe_->book1D("RecHit_YError_f_All", "RecHit Y Error All in Forward", 100, 0.0, 0.1);
  
  for (int i=0; i<7; i++) 
    {
      sprintf(histo, "RecHit_XPull_Disk1_Plaquette%d", i+1);
      recHitXPullDisk1Plaquettes[i] = dbe_->book1D(histo, "RecHit XPull Disk1 by plaquette", 100, -10.0, 10.0); 
      sprintf(histo, "RecHit_XPull_Disk2_Plaquette%d", i+1);
      recHitXPullDisk2Plaquettes[i] = dbe_->book1D(histo, "RecHit XPull Disk2 by plaquette", 100, -10.0, 10.0);  
      sprintf(histo, "RecHit_XPull_Disk3_Plaquette%d", i+1);
      recHitXPullDisk3Plaquettes[i] = dbe_->book1D(histo, "RecHit XPull Disk3 by plaquette", 100, -10.0, 10.0);  
      
      sprintf(histo, "RecHit_YPull_Disk1_Plaquette%d", i+1);
      recHitYPullDisk1Plaquettes[i] = dbe_->book1D(histo, "RecHit YPull Disk1 by plaquette", 100, -10.0, 10.0);      
      sprintf(histo, "RecHit_YPull_Disk2_Plaquette%d", i+1);
      recHitYPullDisk2Plaquettes[i] = dbe_->book1D(histo, "RecHit YPull Disk2 by plaquette", 100, -10.0, 10.0);
      sprintf(histo, "RecHit_YPull_Disk3_Plaquette%d", i+1);
      recHitYPullDisk3Plaquettes[i] = dbe_->book1D(histo, "RecHit YPull Disk3 by plaquette", 100, -10.0, 10.0);
    }

  } // end if quick

}

SiPixelRecHitsValid_pix::~SiPixelRecHitsValid_pix() {
}

void SiPixelRecHitsValid_pix::beginJob() {
  
}

void SiPixelRecHitsValid_pix::endJob() {
  if ( outputFile_.size() != 0 && dbe_ ) {
    cout<<" Save file "<<endl;
    dbe_->save(outputFile_);
  } else {
    cout<<" not saved "<<outputFile_.size()<<endl;
  }
}

void SiPixelRecHitsValid_pix::analyze(const edm::Event& e, const edm::EventSetup& es) {

  //#ifdef QUICK
  //const bool quick = true; // fill only essential histos
  //#else
  //const bool quick = false; // fill only essential histos
  //#endif

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoHand;
  es.get<TrackerTopologyRcd>().get(tTopoHand);
  const TrackerTopology *tTopo=tTopoHand.product();

  // Check which phase we are in
  edm::ESHandle<TrackerGeometry> trackerGeometryHandle;
  es.get<TrackerDigiGeometryRecord>().get(trackerGeometryHandle);
  auto trackerGeometry = trackerGeometryHandle.product();
  phase_ = 
    trackerGeometry -> isThere(GeomDetEnumerators::P1PXB) &&
    trackerGeometry -> isThere(GeomDetEnumerators::P1PXEC);

  if ( ((int) e.id().event() % 1000 == 0) || verbose_ )
    cout << " Run = " << e.id().run() << " Event = " << e.id().event() << endl;
  
  //Get RecHits
  edm::Handle<SiPixelRecHitCollection> recHitColl;

  // Get tracks
  edm::Handle<TrajTrackAssociationCollection> hTTAC;
  if (useTracks_) {
    e.getByToken(tTracks, hTTAC);
  } else {
    //e.getByLabel( src_, recHitColl);
    e.getByToken(tPixelRecHit , recHitColl);
  }
  
  //Get event setup
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get(geom); 
  const TrackerGeometry& theTracker(*geom);
  
  if(verbose_) cout<<" Call associator "<<endl;
#ifdef PIXEL_ASSOCIATOR
  PixelHitAssociator associate( e); 
  //PixelHitAssociator associate( e, conf_ ); 
#else
  //TrackerHitAssociator associate( e);
  TrackerHitAssociator associate( e, trackerHitAssociatorConfig_); 
#endif

  //iterate over detunits
  for (TrackerGeometry::DetContainer::const_iterator it = geom->dets().begin(); it != geom->dets().end(); it++) {
    DetId detId = ((*it)->geographicalId());
    
    if (!( (detId.subdetId() == PixelSubdetector::PixelBarrel)||
	   (detId.subdetId() == PixelSubdetector::PixelEndcap) )) continue;
    
    const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTracker.idToDet(detId) );
    
    if (useTracks_) {
      if (hTTAC.isValid()) {
	const TrajTrackAssociationCollection ttac = *(hTTAC.product());
	if (verbose_) cout << "   hTTAC.isValid()" << endl;
	// Loop on traj-track pairs
	for (TrajTrackAssociationCollection::const_iterator it = ttac.begin(); it !=  ttac.end(); ++it) {
	  
	  if (verbose_) cout << "      TracjTrackAssociationCollection iterating" << endl;
	  reco::TrackRef trackref = it->val;
	  
	  for(trackingRecHit_iterator irecHit = trackref->recHitsBegin();
	      irecHit != trackref->recHitsEnd(); ++irecHit)
	    if (detId == (*irecHit)->geographicalId())
	      matchToSimHits(associate, (*irecHit), detId, theGeomDet,tTopo);
	  
	  //const edm::Ref<std::vector<Trajectory> > refTraj = it->key;
	  //std::vector<TrajectoryMeasurement> tmeasColl =refTraj->measurements();
	  //for (std::vector<TrajectoryMeasurement>::const_iterator tmeasIt = refTraj->measurements().begin(); 
	  //     tmeasIt!=refTraj->measurements().end(); tmeasIt++) {   
	  //  if (!tmeasIt->updatedState().isValid()) continue; 
	  //  TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt->recHit();
	  //  if (detId == hit->geographicalId()) matchToSimHits(associate, &(*hit), detId, theGeomDet,tTopo);
	  //}
	}
      }
    } else {
      
      SiPixelRecHitCollection::const_iterator pixeldet = recHitColl->find(detId);
      if (pixeldet == recHitColl->end()) continue;
      if(verbose_) cout<<" pixel det "<<pixeldet->size()<<endl;
      //----Loop over rechits for this detId
      SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixeldet->begin();
      for (; pixeliter != pixeldet->end(); pixeliter++) matchToSimHits(associate, &(*pixeliter), detId, theGeomDet,tTopo);
    }
  } // <------ end detunit loop
}

#ifdef PIXEL_ASSOCIATOR
void SiPixelRecHitsValid_pix::matchToSimHits(const PixelHitAssociator& associate, const TrackingRecHit* hit, 
					     DetId detId, const PixelGeomDetUnit* theGeomDet, const TrackerTopology *tTopo) {
#else
void SiPixelRecHitsValid_pix::matchToSimHits(const TrackerHitAssociator& associate, const TrackingRecHit* hit, 
					     DetId detId, const PixelGeomDetUnit* theGeomDet, const TrackerTopology *tTopo) {
#endif
  std::vector<PSimHit> matched = associate.associateHit(*hit); // get the matched simhits
  
  if(verbose_) 
    cout<<" rechit "<<hit->localPosition().x()<<" "<<matched.size()<<endl;
  
  if ( !matched.empty() ) {
    float closest = 9999.9;
    std::vector<PSimHit>::const_iterator closestit = matched.begin();
    LocalPoint lp = hit->localPosition();
    float rechit_x = lp.x();
    float rechit_y = lp.y();
    
    
    //loop over sim hits and fill closet
    for (std::vector<PSimHit>::const_iterator m = matched.begin(); m<matched.end(); m++) {
      float sim_x1 = (*m).entryPoint().x();
      float sim_x2 = (*m).exitPoint().x();
      float sim_xpos = 0.5*(sim_x1+sim_x2);
      
      float sim_y1 = (*m).entryPoint().y();
      float sim_y2 = (*m).exitPoint().y();
      float sim_ypos = 0.5*(sim_y1+sim_y2);
      
      float x_res = fabs(sim_xpos - rechit_x);
      float y_res = fabs(sim_ypos - rechit_y);
      
      float dist = sqrt(x_res*x_res + y_res*y_res); // in cm
      
      if ( dist < closest ) {
	//closest = x_res;
	closest = dist;
	closestit = m;
        
	if(verbose_) 
	  std::cout<<" simhit "
		   <<(*m).pabs()<<" "
		   <<(*m).thetaAtEntry()<<" "
		   <<(*m).phiAtEntry()<<" "
		   <<(*m).particleType()<<" "
		   <<(*m).trackId()<<" "
		   <<(*m).momentumAtEntry()
		   <<std::endl;
	
      }
    } // end sim hit loop
    
    if(verbose_) cout<<" closest "<<closest<<" "<<detId.subdetId()<<endl;
    
    if (detId.subdetId() == PixelSubdetector::PixelBarrel)
      fillBarrel(hit, *closestit, detId, theGeomDet,tTopo);
    else if (detId.subdetId() == PixelSubdetector::PixelEndcap)
      fillForward(hit, *closestit, detId, theGeomDet,tTopo);
  } // end matched emtpy
}


void SiPixelRecHitsValid_pix::fillBarrel(const TrackingRecHit* recHit, const PSimHit& simHit, 
				     DetId detId, const PixelGeomDetUnit* theGeomDet,
				     const TrackerTopology *tTopo) {
  const float cmtomicron = 10000.0; 
  const float PI = 3.1416;
  const int NumLayers = 4;
  bool PRINT = verbose_;
#ifdef QUICK
  const bool quick = true; // fill only essential histos
#else
  const bool quick = false; // fill only essential histos
#endif

  float phi = simHit.phiAtEntry();
  float theta = simHit.thetaAtEntry(); // theta with respect to module coordinates 
  float beta = 0;  // beta is roughly like real theta
  if(theta<PI/2.) beta=(PI/2.) - theta; //  
  else            beta=theta - (PI/2.);
  float eta = -log(tan(beta/2.));  // this is an approximation to the tracks eta, only positive

  //float phiLocal = simHit.localDirection().phi(); // same as phi
  //float thetaLocal = simHit.localDirection().theta(); // same as theta

  //float pt  = simHit.momentumAtEntry().perp(); // crap, not real pt
  float p  = simHit.pabs();
  int pid = simHit.particleType();

  // skip secondaries
  if( abs(pid) != 13 || p<10. ) {if(PRINT) cout<<" skip "<<p<<" "<<pid<<endl; return;}

  unsigned int layer = tTopo->pxbLayer(detId);
  unsigned int module = tTopo->pxbModule(detId);
  // The inner ladder (in the smaller radius) are flipped
  // unsigned int ladder = tTopo->pxbLadder(detId);
  // bool odd_lad = ladder % 2;
  // bool inner = (phase_ ? layer == 4 : layer % 2) ? !odd_lad : odd_lad;
  // Determining flipped ladders automatically using surface
  float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
  float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
  bool flipped = (tmp2<tmp1);
  //std::cout<<"Flipped (Surface): "<<(tmp2<tmp1)<<" Flipped (Inner): "<<inner<<std::endl;
  if(PRINT) cout<<" layer "<<layer<<" eta "<<eta<<" phi "<<phi<<endl;

    if(layer==1) {
      htest1->Fill(eta,phi);
    } else if(layer==2) {
      htest2->Fill(eta,phi);
    }

  // skip, for tests only
  //if( abs(eta)<0.1 || abs(eta)>1.0) return; // limit 
  //if( abs(phi)<1.3 || abs(phi)>1.9) return;  // limit to l2 acceptance

  if(PRINT) std::cout<<" simhit "
		     <<p<<" "
		     <<pid<<" "
		     <<phi<<" "
		     <<theta<<" "
		     <<simHit.trackId()<<" lay "
		     <<layer<<" mod "
		     <<module<<std::endl;

  if(!quick) {
    if(layer==1) {
      hphi1->Fill(phi);
      htheta1->Fill(theta);
      hbeta1->Fill(beta);
    } else if(layer==2) {
      hphi2->Fill(phi);
      htheta2->Fill(theta);
      hbeta2->Fill(beta);
    }
  }
  LocalPoint lp = recHit->localPosition();
  float lp_y = lp.y();  
  float lp_x = lp.x();

  LocalError lerr = recHit->localPositionError();
  float lerr_x = sqrt(lerr.xx());
  float lerr_y = sqrt(lerr.yy());
  //cout<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<endl;

  
  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  float res_x = (lp.x() - sim_xpos)*cmtomicron;
  
  
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  float res_y = (lp.y() - sim_ypos)*cmtomicron;
  
  if(PRINT) // dk 
    cout<<detId.rawId()<<" "<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<" "
	<<sim_xpos<<" "<<sim_ypos<<" "<<res_x<<" "<<res_y
	<<endl;

  
  float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  float pull_y = ( lp_y - sim_ypos ) / lerr_y;

  if(!quick) {

    //recHitYAllModules->Fill(lp_y);
    recHitXResAllB->Fill(res_x);
    recHitYResAllB->Fill(res_y);
    recHitXPullAllB->Fill(pull_x);  
    recHitYPullAllB->Fill(pull_y);

    if( layer == 1) {
      recHitXError1B->Fill(lerr_x); recHitYError1B->Fill(lerr_y);
      if(phi<0.) recHitX11->Fill(abs(phi),res_x); else recHitX12->Fill(abs(phi),res_x);
    } else if( layer == 2) { 
      recHitXError2B->Fill(lerr_x); recHitYError2B->Fill(lerr_y);
      if(phi<0.) recHitX21->Fill(abs(phi),res_x); else recHitX22->Fill(abs(phi),res_x);
    } else if( layer == 3) {
      recHitXError3B->Fill(lerr_x); recHitYError3B->Fill(lerr_y);
      }
    //int rows = theGeomDet->specificTopology().nrows(); 
    //if (rows == 160) recHitXFullModules->Fill(lp_x);
    //else if (rows == 80) recHitXHalfModules->Fill(lp_x);
    
    LocalError lape = theGeomDet->localAlignmentError();
    if (lape.valid()) {
      float tmp11= 0.;
      if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
      //float tmp12= sqrt(lape.xy())*1E4;
      float tmp13= 0.;
      if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
      //bool tmp14=tmp2<tmp1;
      if( (layer) == 1) 
	{recHitXAlignError1->Fill(tmp11); recHitYAlignError1->Fill(tmp13);}
      else if( (layer) == 2) 
	{recHitXAlignError2->Fill(tmp11); recHitYAlignError2->Fill(tmp13);}
      else if( (layer) == 3) 
	{recHitXAlignError3->Fill(tmp11); recHitYAlignError3->Fill(tmp13);}
      else {cout<<" unknown layer "<< layer<<endl;}
      //cout<<layer<<" "<<tTopo->pxbModule(detId)<<" "<<rows<<" "<<tmp14<<" "
      //  <<tmp11<<" "<<tmp13<<endl;
    }  // if lape

    //int module = tTopo->pxbModule(detId);
    if (flipped) {
      recHitXResFlippedLadderLayers[layer-1]->Fill(res_x);
      recHitXResFlippedLadderLayersSide[layer-1][module>4]->Fill(res_x);
      recHitXPullFlippedLadderLayers[layer-1]->Fill(pull_x);
    } else {
      recHitXResNonFlippedLadderLayers[layer-1]->Fill(res_x);
      recHitXResNonFlippedLadderLayersSide[layer-1][module>4]->Fill(res_x);
      recHitXPullNonFlippedLadderLayers[layer-1]->Fill(pull_x);
    } // end if

  } // end if quick 
  // as a function of layer
  recHitXResLayers[layer-1]->Fill(res_x);  //QUICK
  recHitYResLayers[layer-1]->Fill(res_y);  //QUICK
  if(!quick) {
    recHitXResLayersP[layer-1]->Fill(eta,std::abs(res_x));
    recHitYResLayersP[layer-1]->Fill(eta,std::abs(res_y));
  } // end if quick


  //get cluster
  SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)recHit)->cluster();

  // as a function of layer
  for (unsigned int i=0; i<NumLayers; i++) {
    if (layer == i+1)  { // select layer
      if(!quick) {
	int sizeX = (*clust).sizeX();
	if(sizeX==1) {
	  recHitXResLayersP1[i]->Fill(eta,std::abs(res_x));
	  if(i==0) recHitL1XResSize1->Fill(res_x);
	  else if(i==1) recHitL2XResSize1->Fill(res_x);
	} else if(sizeX==2) {
	  recHitXResLayersP2[i]->Fill(eta,std::abs(res_x));
	  if(i==0) recHitL1XResSize2->Fill(res_x);
	  else if(i==1) recHitL2XResSize2->Fill(res_x);
	} else if(sizeX>=3) {
	  recHitXResLayersP3[i]->Fill(eta,std::abs(res_x));
	  if(i==0) recHitL1XResSize3->Fill(res_x);
	  else if(i==1) recHitL2XResSize3->Fill(res_x);
	}
	
	int sizeY = (*clust).sizeY();
	if(sizeY==1)      recHitYResLayersP1[i]->Fill(eta,std::abs(res_y));
	else if(sizeY==2) recHitYResLayersP2[i]->Fill(eta,std::abs(res_y));
	else if(sizeY==3) recHitYResLayersP3[i]->Fill(eta,std::abs(res_y));
	else if(sizeY>=4) recHitYResLayersP4[i]->Fill(eta,std::abs(res_y));
      } // end if quick
    }
  }
  
  // fill module dependent info
  for (unsigned int i=0; i<8; i++) {
    if (tTopo->pxbModule(detId) == i+1) {
      int sizeY = (*clust).sizeY();
      clustYSizeModule[i]->Fill(sizeY);
      
      float charge = (*clust).charge();
      if (layer == 1) clustChargeLayer1Modules[i]->Fill(charge);
      else if (layer == 2) clustChargeLayer2Modules[i]->Fill(charge);
      else if (layer == 3) clustChargeLayer3Modules[i]->Fill(charge);
      } // end if
  } // end for

  if(quick) return; // skip the rest 

  // as a function of eta
  for (unsigned int i=0; i<25; i++) {
    float eta1=float(i)*0.1;
    float eta2=eta1+0.1;
    if(eta>=eta1 && eta<eta2) {
 
      if (layer == 1) 
	{heta1->Fill(eta);recHitXResLayer1Eta[i]->Fill(res_x);recHitYResLayer1Eta[i]->Fill(res_y);}
      else if (layer == 2) 
	{heta2->Fill(eta);recHitXResLayer2Eta[i]->Fill(res_x);recHitYResLayer2Eta[i]->Fill(res_y);}
      else if (layer == 3) 
	{heta3->Fill(eta);recHitXResLayer3Eta[i]->Fill(res_x);recHitYResLayer3Eta[i]->Fill(res_y);}
      
    } // eta 
  } // i (eta)
  
  // fill module dependent info
  for (unsigned int i=0; i<8; i++) {
    if (tTopo->pxbModule(detId) == i+1) {
      
      if (layer == 1) {
	recHitYResLayer1Modules[i]->Fill(res_y);
	recHitYPullLayer1Modules[i]->Fill(pull_y);
      } else if (layer == 2)  {
	recHitYResLayer2Modules[i]->Fill(res_y);
	recHitYPullLayer2Modules[i]->Fill(pull_y);
      } else if (layer == 3) {
	recHitYResLayer3Modules[i]->Fill(res_y);
	recHitYPullLayer3Modules[i]->Fill(pull_y);
      } // end if
    } // end if
  } // end for

  //int sizeX = (*clust).sizeX();
  //if (layer == 1) clustXSizeLayer[0]->Fill(sizeX);
  //if (layer == 2) clustXSizeLayer[1]->Fill(sizeX);
  //if (layer == 3) clustXSizeLayer[2]->Fill(sizeX);

}

int SiPixelRecHitsValid_pix::PhaseIBladeOfflineToOnline(const int& blade)
{
  int blade_online = -999;
  if(1  <= blade && blade < 6)  blade_online = 6  - blade; // 5 on 1st quarter
  if(6  <= blade && blade < 17) blade_online = 5  - blade; // 11 on 2nd half
  if(17 <= blade && blade < 23) blade_online = 28 - blade; // 6 on 4th quarter
  if(23 <= blade && blade < 31) blade_online = 31 - blade; // 8 on 1st quarter
  if(31 <= blade && blade < 48) blade_online = 30 - blade; // 17 on 2nd half
  if(48 <= blade && blade < 57) blade_online = 65 - blade; // 9 on 4th quarter
  return blade_online;
}

// ------------------------------------------------------------------------------
void SiPixelRecHitsValid_pix::fillForward(const TrackingRecHit* recHit, const PSimHit & simHit, 
				      DetId detId,const PixelGeomDetUnit * theGeomDet,
				      const TrackerTopology *tTopo) {
  const float cmtomicron = 10000.0;
#ifdef QUICK
  const bool quick = true;
#else
  const bool quick = false;
#endif

  //int rows = theGeomDet->specificTopology().nrows();
  //int cols = theGeomDet->specificTopology().ncolumns();
  
  LocalPoint lp = recHit->localPosition();
  float lp_x = lp.x();
  float lp_y = lp.y();
  
  LocalError lerr = recHit->localPositionError();
  float lerr_x = sqrt(lerr.xx());
  float lerr_y = sqrt(lerr.yy());

  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  
  float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  float pull_y = ( lp_y - sim_ypos ) / lerr_y;


  float res_x = (lp.x() - sim_xpos)*cmtomicron;  
  float res_y = (lp.y() - sim_ypos)*cmtomicron;

  // integrated fpix results 
  recHitXResAllF->Fill(res_x);
  recHitYResAllF->Fill(res_y);  
  
  // spliy into disks etc.
  int disk = tTopo->pxfDisk(detId);
  int panel = tTopo->pxfPanel(detId);      // Phase 1: Forward 1, Backward 2
  int side = tTopo->pxfSide(detId);
  //int module = tTopo->pxfModule(detId);  // Phase 1: Always 1
  // Phase 1 specific
  int blade  = tTopo->pxfBlade(detId);     // Phase 1: Inner blades 1-22, Outer blades 23-56
  int ring = 1 + (blade>22);               // Phase 1: Inner: 1, Outer: 2
  //int phase1_online_blade = PhaseIBladeOfflineToOnline(blade); // Phase 1: Ring1 +-1-11, Ring2 +-1-17
  //int xside = 1 + (phase1_online_blade>0); // Phase 1: -X 1, +X 2

  if (!quick) {
    recHitXResRingSidePanel[ring-1][side-1][ring%2]->Fill(res_x);
    recHitYResRingSidePanel[ring-1][side-1][ring%2]->Fill(res_y);
    recHitXResRingSide[ring-1][side-1]->Fill(res_x);
    recHitYResRingPanel[ring-1][panel-1]->Fill(res_y);
  }

  if (side==1) {
    recHitXResNegZF->Fill(res_x);
    recHitYResNegZF->Fill(res_y);
  } else if (side==2) {
    recHitXResPosZF->Fill(res_x);
    recHitYResPosZF->Fill(res_y);  
  }

  // get cluster
  SiPixelRecHit::ClusterRef const& clust = ((SiPixelRecHit*)recHit)->cluster();
  int sizeX = (*clust).sizeX(), sizeY = (*clust).sizeY();
  float charge = (*clust).charge();
  if(disk==1) {
    if(!quick) {
      clustXSizeDisk1Plaquettes[0]->Fill(sizeX);
      clustYSizeDisk1Plaquettes[0]->Fill(sizeY);
      clustChargeDisk1Plaquettes[0]->Fill(charge);
    }
  } else if (disk==2) {
    if(!quick) {
      clustXSizeDisk2Plaquettes[0]->Fill(sizeX);	
      clustYSizeDisk2Plaquettes[0]->Fill(sizeY);
      clustChargeDisk2Plaquettes[0]->Fill(charge);
    }
  } else if (disk==3) {
    if(!quick) {
      clustXSizeDisk3Plaquettes[0]->Fill(sizeX);	
      clustYSizeDisk3Plaquettes[0]->Fill(sizeY);	
      clustChargeDisk3Plaquettes[0]->Fill(charge);
    } 
  }

  if(disk==1) {

    recHitXResDisk1[0]->Fill(res_x);
    recHitYResDisk1[0]->Fill(res_y);

    if(side==1) {  // -z 
      recHitXResDisk1[1]->Fill(res_x);
      recHitYResDisk1[1]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk1[3]->Fill(res_x);
	recHitYResDisk1[3]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk1[4]->Fill(res_x);
	recHitYResDisk1[4]->Fill(res_y);
      }

    } else { //+z
      recHitXResDisk1[2]->Fill(res_x);
      recHitYResDisk1[2]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk1[5]->Fill(res_x);
	recHitYResDisk1[5]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk1[6]->Fill(res_x);
	recHitYResDisk1[6]->Fill(res_y);
      }

    }

    if(!quick) {
      recHitXPullDisk1Plaquettes[0]->Fill(pull_x);
      recHitYPullDisk1Plaquettes[0]->Fill(pull_y);
    }
    

  } else if (disk==2) {

    recHitXResDisk2[0]->Fill(res_x);
    recHitYResDisk2[0]->Fill(res_y);
    if(side==1) {  // -z 
      recHitXResDisk2[1]->Fill(res_x);
      recHitYResDisk2[1]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk2[3]->Fill(res_x);
	recHitYResDisk2[3]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk2[4]->Fill(res_x);
	recHitYResDisk2[4]->Fill(res_y);
      }
    } else { //+z
      recHitXResDisk2[2]->Fill(res_x);
      recHitYResDisk2[2]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk2[5]->Fill(res_x);
	recHitYResDisk2[5]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk2[6]->Fill(res_x);
	recHitYResDisk2[6]->Fill(res_y);
      }
    }

    if(!quick) {
      recHitXPullDisk2Plaquettes[0]->Fill(pull_x);
      recHitYPullDisk2Plaquettes[0]->Fill(pull_y);
    }


  } else if (disk==3) {

    recHitXResDisk3[0]->Fill(res_x);
    recHitYResDisk3[0]->Fill(res_y);
    if(side==1) {  // -z 
      recHitXResDisk3[1]->Fill(res_x);
      recHitYResDisk3[1]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk3[3]->Fill(res_x);
	recHitYResDisk3[3]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk3[4]->Fill(res_x);
	recHitYResDisk3[4]->Fill(res_y);
      }
    } else { //+z
      recHitXResDisk3[2]->Fill(res_x);
      recHitYResDisk3[2]->Fill(res_y);

      if(panel==1) {  // panel 1 
	recHitXResDisk3[5]->Fill(res_x);
	recHitYResDisk3[5]->Fill(res_y);
      } else { // panel 2
	recHitXResDisk3[6]->Fill(res_x);
	recHitYResDisk3[6]->Fill(res_y);
      }
    }

    if(!quick) {
      recHitXPullDisk3Plaquettes[0]->Fill(pull_x);
      recHitYPullDisk3Plaquettes[0]->Fill(pull_y);
    }

  } // end disk 


  if(quick) return; // skip the rest

  recHitXErrorAllF->Fill(lerr_x);
  recHitYErrorAllF->Fill(lerr_y);
  recHitXPullAllF->Fill(pull_x);
  recHitYPullAllF->Fill(pull_y);

  // if (rows == 80)       recHitXPlaquetteSize1->Fill(lp_x);
  // else if (rows == 160) recHitXPlaquetteSize2->Fill(lp_x);  
  // if (cols == 104) 
  //   {
  //     recHitYPlaquetteSize2->Fill(lp_y);
  //   } 
  // else if (cols == 156) 
  //   {
  //     recHitYPlaquetteSize3->Fill(lp_y);
  //   } 
  // else if (cols == 208) 
  //   {
  //     recHitYPlaquetteSize4->Fill(lp_y);
  //   } 
  // else if (cols == 260) 
  //   {
  //     recHitYPlaquetteSize5->Fill(lp_y);
  //   }
  
  LocalError lape = theGeomDet->localAlignmentError();
  if (lape.valid()) {
    float tmp11= 0.;
    if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
    //float tmp12= sqrt(lape.xy())*1E4;
    float tmp13= 0.;
    if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
    if( (tTopo->pxfDisk(detId)) == 1)  { // disk 1
      if( (tTopo->pxfSide(detId)) == 1)  // -z
	{recHitXAlignError4->Fill(tmp11); recHitYAlignError4->Fill(tmp13);}
      else if( (tTopo->pxfSide(detId)) == 2)  // +z
	{recHitXAlignError5->Fill(tmp11); recHitYAlignError5->Fill(tmp13);}
    } else if( (tTopo->pxfDisk(detId)) == 2) {  // disk 2
      if( (tTopo->pxfSide(detId)) == 1)  
	{recHitXAlignError6->Fill(tmp11); recHitYAlignError6->Fill(tmp13);}
      else if( (tTopo->pxfSide(detId)) == 2)  
	{recHitXAlignError7->Fill(tmp11); recHitYAlignError7->Fill(tmp13);}
    }
    else {cout<<" unkown disk "<<tTopo->pxfDisk(detId)<<endl;}
    //if(tmp11>0.) cout<<tTopo->pxfDisk(detId)<<" "
    //		     <<tTopo->pxfSide(detId)<<" "<<tTopo->pxfBlade(detId)<<" "
    //		     <<tTopo->pxfPanel(detId)<<" "<<tTopo->pxfModule(detId)<<" "
    //		     <<rows<<" "<<cols<<" "
    //		     <<tmp11<<" "<<tmp13<<endl;

    float tmp14=float(tTopo->pxfBlade(detId));
    if( (tTopo->pxfDisk(detId)) == 2) tmp14 += 50.;
    if( (tTopo->pxfSide(detId)) == 2) tmp14 += 25.; 
    test->Fill(tmp14,tmp13);

  } // if lape 
    

}

//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelRecHitsValid_pix);
