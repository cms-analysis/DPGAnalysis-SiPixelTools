
// SiPixelRecHitsValid.cc
// Description: see SiPixelRecHitsValid.h
// Author: Jason Shaev, JHU
// Created 6/7/06
//
// Make standalone, independent from the validation code. dk 3/14
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

//#define PIXEL_ASSOCIATOR // use my special pixel associator
#ifdef PIXEL_ASSOCIATOR
#include "DPGAnalysis-SiPixelTools/PixelHitAssociator/interface/PixelHitAssociator.h"
#else 
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#endif

//#define QUICK

using namespace std;
using namespace edm;


SiPixelRecHitsValid_pix::SiPixelRecHitsValid_pix(const ParameterSet& ps): 
  dbe_(0), 
  conf_(ps),
  src_( ps.getParameter<edm::InputTag>( "src" ) ) 
{
  outputFile_ = ps.getUntrackedParameter<string>("outputFile", "pixelrechitshisto.root");

#ifdef PIXEL_ASSOCIATOR
  cout<<" Compare Sim-Rec pixel hits, using pixel hit associator "<<endl;
#else
  cout<<" Compare Sim-Rec pixel hits, using tracker hit associator "<<endl;
#endif
  cout<<" source = "<<src_<<endl; //dk

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

  if(!quick) {

  heta = dbe_->book1D("heta","eta",50,-2.5,2.5);
  heta1 = dbe_->book1D("heta1","eta",50,-2.5,2.5);
  heta2 = dbe_->book1D("heta2","eta",50,-2.5,2.5);
  heta3 = dbe_->book1D("heta3","eta",50,-2.5,2.5);

  htest1 = dbe_->book1D("htest1","test1",20,-0.5,19.5);
  htest2 = dbe_->book1D("htest2","test2",200,0.,0.2);
  htest3 = dbe_->book1D("htest3","test3",200,0.,0.2);

  //Cluster y-size by module number for barrel
  for (int i=0; i<8; i++) {
    sprintf(histo, "Clust_y_size_Module%d", i+1);
    clustYSizeModule[i] = dbe_->book1D(histo,"Cluster y-size by Module", 20, 0.5, 20.5); 
  } // end for
  
  //Cluster x-size by layer for barrel
  for (int i=0; i<3; i++) {
    sprintf(histo, "Clust_x_size_Layer%d", i+1);
    clustXSizeLayer[i] = dbe_->book1D(histo,"Cluster x-size by Layer", 20, 0.5, 20.5);
  } // end for
  
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
    
    //Cluster y-size for Disk1 by Plaquette
    sprintf(histo, "Clust_y_size_Disk1_Plaquette%d", i+1);
    clustYSizeDisk1Plaquettes[i] = dbe_->book1D(histo, "Cluster Y-size for Disk1 by Plaquette", 20, 0.5, 20.5);
    
    //Cluster y-size for Disk2 by Plaquette
    sprintf(histo, "Clust_y_size_Disk2_Plaquette%d", i+1);
    clustYSizeDisk2Plaquettes[i] = dbe_->book1D(histo, "Cluster Y-size for Disk2 by Plaquette", 20, 0.5, 20.5);
    
    //Cluster charge for Disk1 by Plaquette
    sprintf(histo, "Clust_charge_Disk1_Plaquette%d", i+1);
    clustChargeDisk1Plaquettes[i] = dbe_->book1D(histo, "Cluster charge for Disk1 by Plaquette", 50, 0., 200000.);
    
    //Cluster charge for Disk2 by Plaquette
    sprintf(histo, "Clust_charge_Disk2_Plaquette%d", i+1);
    clustChargeDisk2Plaquettes[i] = dbe_->book1D(histo, "Cluster charge for Disk2 by Plaquette", 50, 0., 200000.);
  } // end for
  
  } // end if quick

  dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/recHitBPIX");

  //RecHit X resolution for flipped and unflipped ladders by layer for barrel
  for (int i=0; i<3; i++) {
    //RecHit X resolution per layer
    sprintf(histo, "RecHit_XRes_Layer%d", i+1);
    recHitXResLayers[i] = dbe_->book1D(histo, "RecHit XRes by Layer", 100, -200., 200.);

    //RecHit Y resolution per layer
    sprintf(histo, "RecHit_YRes_Layer%d", i+1);
    recHitYResLayers[i] = dbe_->book1D(histo, "RecHit YRes by Layer", 100, -200., 200.);

    if(!quick) {

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
  recHitXFullModules = dbe_->book1D("RecHit_x_FullModules", "RecHit X distribution for full modules", 100,-2., 2.);
  
  //RecHit X distribution for half modules for barrel
  recHitXHalfModules = dbe_->book1D("RecHit_x_HalfModules", "RecHit X distribution for half modules", 100, -1., 1.);
  
  //RecHit Y distribution all modules for barrel
  recHitYAllModules = dbe_->book1D("RecHit_y_AllModules", "RecHit Y distribution for all modules", 100, -4., 4.);
  
  // eta plots
  for (int i=0; i<25; i++) {
    //RecHit X resolution per layer
    sprintf(histo, "RecHit_XRes_Layer1_Eta%d", i+1);
    recHitXResLayer1Eta[i] = dbe_->book1D(histo, "RecHit XRes Layer1, eta", 100, -200., 200.);
    sprintf(histo, "RecHit_XRes_Layer2_Eta%d", i+1);
    recHitXResLayer2Eta[i] = dbe_->book1D(histo, "RecHit XRes Layer2, eta", 100, -200., 200.);
    sprintf(histo, "RecHit_XRes_Layer3_Eta%d", i+1);
    recHitXResLayer3Eta[i] = dbe_->book1D(histo, "RecHit XRes Layer3, eta", 100, -200., 200.);

    //RecHit Y resolution per layer
    sprintf(histo, "RecHit_YRes_Layer1_Eta%d", i+1);
    recHitYResLayer1Eta[i] = dbe_->book1D(histo, "RecHit YRes Layer1, eta", 100, -200., 200.);
    sprintf(histo, "RecHit_YRes_Layer2_Eta%d", i+1);
    recHitYResLayer2Eta[i] = dbe_->book1D(histo, "RecHit YRes Layer2, eta", 100, -200., 200.);
    sprintf(histo, "RecHit_YRes_Layer3_Eta%d", i+1);
    recHitYResLayer3Eta[i] = dbe_->book1D(histo, "RecHit YRes Layer3, eta", 100, -200., 200.);
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



    //RecHit X resolution for flipped ladders by layer
    sprintf(histo, "RecHit_XRes_FlippedLadder_Layer%d", i+1);
    recHitXResFlippedLadderLayers[i] = dbe_->book1D(histo, "RecHit XRes Flipped Ladders by Layer", 100, -200., 200.);
    
    //RecHit X resolution for unflipped ladders by layer
    sprintf(histo, "RecHit_XRes_UnFlippedLadder_Layer%d", i+1);
    recHitXResNonFlippedLadderLayers[i] = dbe_->book1D(histo, "RecHit XRes NonFlipped Ladders by Layer", 100, -200., 200.);
  } // end for
  
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
  } // end for
  
  } // end if quick 

  dbe_->setCurrentFolder("TrackerRecHitsV/TrackerRecHits/Pixel/recHitFPIX");
  //RecHit X resolution all plaquettes
  recHitXResAllF = dbe_->book1D("RecHit_xres_f_All", "RecHit X Res All in Forward", 100, -200., 200.);
  
  //RecHit Y resolution all plaquettes
  recHitYResAllF = dbe_->book1D("RecHit_yres_f_All", "RecHit Y Res All in Forward", 100, -200., 200.);
  

  if(!quick) {

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
  recHitXPlaquetteSize1 = dbe_->book1D("RecHit_x_Plaquette_xsize1", "RecHit X Distribution for plaquette x-size1", 100, -2., 2.);
  
  //RecHit X distribution for plaquette with x-size 2 in forward
  recHitXPlaquetteSize2 = dbe_->book1D("RecHit_x_Plaquette_xsize2", "RecHit X Distribution for plaquette x-size2", 100, -2., 2.);
  
  //RecHit Y distribution for plaquette with y-size 2 in forward
  recHitYPlaquetteSize2 = dbe_->book1D("RecHit_y_Plaquette_ysize2", "RecHit Y Distribution for plaquette y-size2", 100, -4., 4.);
  
  //RecHit Y distribution for plaquette with y-size 3 in forward
  recHitYPlaquetteSize3 = dbe_->book1D("RecHit_y_Plaquette_ysize3", "RecHit Y Distribution for plaquette y-size3", 100, -4., 4.);
  
  //RecHit Y distribution for plaquette with y-size 4 in forward
  recHitYPlaquetteSize4 = dbe_->book1D("RecHit_y_Plaquette_ysize4", "RecHit Y Distribution for plaquette y-size4", 100, -4., 4.);
  
  //RecHit Y distribution for plaquette with y-size 5 in forward
  recHitYPlaquetteSize5 = dbe_->book1D("RecHit_y_Plaquette_ysize5", "RecHit Y Distribution for plaquette y-size5", 100, -4., 4.);
  
  //X and Y resolutions for both disks by plaquette in forward
  for (int i=0; i<7; i++) {
    //X resolution for Disk1 by plaquette
    sprintf(histo, "RecHit_XRes_Disk1_Plaquette%d", i+1);
    recHitXResDisk1Plaquettes[i] = dbe_->book1D(histo, "RecHit XRes Disk1 by plaquette", 100, -200., 200.); 
    //X resolution for Disk2 by plaquette
    sprintf(histo, "RecHit_XRes_Disk2_Plaquette%d", i+1);
    recHitXResDisk2Plaquettes[i] = dbe_->book1D(histo, "RecHit XRes Disk2 by plaquette", 100, -200., 200.);  
    
    //Y resolution for Disk1 by plaquette
    sprintf(histo, "RecHit_YRes_Disk1_Plaquette%d", i+1);
    recHitYResDisk1Plaquettes[i] = dbe_->book1D(histo, "RecHit YRes Disk1 by plaquette", 100, -200., 200.);
    //Y resolution for Disk2 by plaquette
    sprintf(histo, "RecHit_YRes_Disk2_Plaquette%d", i+1);
    recHitYResDisk2Plaquettes[i] = dbe_->book1D(histo, "RecHit YRes Disk2 by plaquette", 100, -200., 200.);
    
  }

  } // end if quick 

  if(!quick) {

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

  for (int i=0; i<3; i++) 
    {
      sprintf(histo, "RecHit_XPull_FlippedLadder_Layer%d", i+1);
      recHitXPullFlippedLadderLayers[i] = dbe_->book1D(histo, "RecHit XPull Flipped Ladders by Layer", 100, -10.0, 10.0);
      
      sprintf(histo, "RecHit_XPull_UnFlippedLadder_Layer%d", i+1);
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
      
      sprintf(histo, "RecHit_YPull_Disk1_Plaquette%d", i+1);
      recHitYPullDisk1Plaquettes[i] = dbe_->book1D(histo, "RecHit YPull Disk1 by plaquette", 100, -10.0, 10.0);
      
      sprintf(histo, "RecHit_YPull_Disk2_Plaquette%d", i+1);
      recHitYPullDisk2Plaquettes[i] = dbe_->book1D(histo, "RecHit YPull Disk2 by plaquette", 100, -10.0, 10.0);
    }

  } // end if quick

}

SiPixelRecHitsValid_pix::~SiPixelRecHitsValid_pix() {
}

void SiPixelRecHitsValid_pix::beginJob() {
  
}

void SiPixelRecHitsValid_pix::endJob() {
  if ( outputFile_.size() != 0 && dbe_ ) dbe_->save(outputFile_);
}

void SiPixelRecHitsValid_pix::analyze(const edm::Event& e, const edm::EventSetup& es) 
{

  //cout<<" analyse "<<endl;

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoHand;
  es.get<IdealGeometryRecord>().get(tTopoHand);
  const TrackerTopology *tTopo=tTopoHand.product();

  LogInfo("EventInfo") << " Run = " << e.id().run() << " Event = " << e.id().event();
  if ( (int) e.id().event() % 1000 == 0 )
    cout << " Run = " << e.id().run() << " Event = " << e.id().event() << endl;
  
  //Get RecHits
  edm::Handle<SiPixelRecHitCollection> recHitColl;
  e.getByLabel( src_, recHitColl);
  
  //Get event setup
  edm::ESHandle<TrackerGeometry> geom;
  es.get<TrackerDigiGeometryRecord>().get(geom); 
  const TrackerGeometry& theTracker(*geom);
  
  //cout<<" Call associator "<<endl;
#ifdef PIXEL_ASSOCIATOR
  PixelHitAssociator associate( e); 
  //PixelHitAssociator associate( e, conf_ ); 
#else
  //TrackerHitAssociator associate( e); 
  TrackerHitAssociator associate( e, conf_); 
#endif
  
  //iterate over detunits
  for (TrackerGeometry::DetContainer::const_iterator it = geom->dets().begin(); it != geom->dets().end(); it++) 
    {
      DetId detId = ((*it)->geographicalId());
      unsigned int subid=detId.subdetId();
      
      if (! ((subid==1) || (subid==2))) continue;
      
      const PixelGeomDetUnit * theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTracker.idToDet(detId) );
      
      SiPixelRecHitCollection::const_iterator pixeldet = recHitColl->find(detId);
      if (pixeldet == recHitColl->end()) continue;
      SiPixelRecHitCollection::DetSet pixelrechitRange = *pixeldet;
      SiPixelRecHitCollection::DetSet::const_iterator pixelrechitRangeIteratorBegin = pixelrechitRange.begin();
      SiPixelRecHitCollection::DetSet::const_iterator pixelrechitRangeIteratorEnd   = pixelrechitRange.end();
      SiPixelRecHitCollection::DetSet::const_iterator pixeliter = pixelrechitRangeIteratorBegin;
      std::vector<PSimHit> matched;
      
      //cout<<" pixel det "<<pixeldet->size()<<endl;
      //----Loop over rechits for this detId
      for ( ; pixeliter != pixelrechitRangeIteratorEnd; pixeliter++) 
	{
	  matched.clear();
	  matched = associate.associateHit(*pixeliter); // get the matched simhits
	  //cout<<" rechit "<<pixeliter->localPosition().x()<<" "<<matched.size()<<endl;
	  htest1->Fill(float(matched.size()));

	  if ( !matched.empty() ) {
	      float closest = 9999.9;
	      std::vector<PSimHit>::const_iterator closestit = matched.begin();
	      LocalPoint lp = pixeliter->localPosition();
	      float rechit_x = lp.x();
	      float rechit_y = lp.y();
	      

	      //loop over sim hits and fill closet
	      for (std::vector<PSimHit>::const_iterator m = matched.begin(); m<matched.end(); m++) 
		{
		  float sim_x1 = (*m).entryPoint().x();
		  float sim_x2 = (*m).exitPoint().x();
		  float sim_xpos = 0.5*(sim_x1+sim_x2);

		  float sim_y1 = (*m).entryPoint().y();
		  float sim_y2 = (*m).exitPoint().y();
		  float sim_ypos = 0.5*(sim_y1+sim_y2);
		  
		  float x_res = fabs(sim_xpos - rechit_x);
		  float y_res = fabs(sim_ypos - rechit_y);
		  
		  float dist = sqrt(x_res*x_res + y_res*y_res); // in cm
		  htest2->Fill(dist);

		  if ( dist < closest ) {
		    //closest = x_res;
		    closest = dist;
		    closestit = m;
		    
		    // std::cout<<" simhit "
		    //       <<(*m).pabs()<<" "
		    //       <<(*m).thetaAtEntry()<<" "
		    //       <<(*m).phiAtEntry()<<" "
		    //       <<(*m).particleType()<<" "
		    //       <<(*m).trackId()<<" "
		    //       <<(*m).momentumAtEntry()
		    //       <<std::endl;
		    
		  }
		} // end sim hit loop
	      
	      htest3->Fill(closest);
	      //cout<<" closest "<<closest<<endl;

	      if (subid==1) 
		{ //<----------barrel
		  fillBarrel(*pixeliter, *closestit, detId, theGeomDet,tTopo);	
		} // end barrel
	      if (subid==2) 
		{ // <-------forward
		  fillForward(*pixeliter, *closestit, detId, theGeomDet,tTopo);
		}
	      
	    } // end matched emtpy
	} // <-----end rechit loop 
    } // <------ end detunit loop
}

void SiPixelRecHitsValid_pix::fillBarrel(const SiPixelRecHit& recHit, const PSimHit& simHit, 
				     DetId detId, const PixelGeomDetUnit* theGeomDet,
				     const TrackerTopology *tTopo) 
{
  const float cmtomicron = 10000.0; 
  const float PI = 3.1416;
  const bool PRINT = false; //dk
#ifdef QUICK
  const bool quick = true; // fill only essential histos
#else
  const bool quick = false; // fill only essential histos
#endif

  //float phi = simHit.phiAtEntry();
  float theta = simHit.thetaAtEntry(); // theta with respect to module coordinates 
  float beta = 0;  // beta is roughly like real theta
  if(theta<PI/2.) beta=(PI/2.) - theta; //  
  else            beta=theta - (PI/2.);
  float eta = -log(tan(beta/2.));  // this is an approximation to the tracks eta, only positive
  //float eta = std::abs((simHit.localDirection()).eta());
  float p  = simHit.pabs();
  int pid = simHit.particleType();

  // skip secendaries
  if(pid!=13 || p<10. ) return;

  // std::cout<<" simhit "
  // 	   <<p<<" "
  // 	   <<eta<<" "
  //   //<<phi<<" "
  // 	   <<pid<<" "
  //   //	   <<simHit.trackId()<<" "
  // 	   <<std::endl;

  if(!quick) {
    heta->Fill(eta);
    //hphi->Fill(phi);
  }
  LocalPoint lp = recHit.localPosition();
  float lp_y = lp.y();  
  float lp_x = lp.x();

  LocalError lerr = recHit.localPositionError();
  float lerr_x = sqrt(lerr.xx());
  float lerr_y = sqrt(lerr.yy());
  //cout<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<endl;

  if(!quick) recHitYAllModules->Fill(lp_y);
  
  float sim_x1 = simHit.entryPoint().x();
  float sim_x2 = simHit.exitPoint().x();
  float sim_xpos = 0.5*(sim_x1 + sim_x2);
  float res_x = (lp.x() - sim_xpos)*cmtomicron;
  
  if(!quick) recHitXResAllB->Fill(res_x);
  
  float sim_y1 = simHit.entryPoint().y();
  float sim_y2 = simHit.exitPoint().y();
  float sim_ypos = 0.5*(sim_y1 + sim_y2);
  float res_y = (lp.y() - sim_ypos)*cmtomicron;
  
  if(PRINT) // dk 
    cout<<detId.rawId()<<" "<<lp_x<<" "<<lp_y<<" "<<lerr_x<<" "<<lerr_y<<" "
	<<sim_xpos<<" "<<sim_ypos<<" "<<res_x<<" "<<res_y
	<<endl;

  if(!quick) recHitYResAllB->Fill(res_y);
  
  float pull_x = ( lp_x - sim_xpos ) / lerr_x;
  float pull_y = ( lp_y - sim_ypos ) / lerr_y;

  if(!quick) {
    recHitXPullAllB->Fill(pull_x);  
    recHitYPullAllB->Fill(pull_y);
    
    if( (tTopo->pxbLayer(detId)) == 1) 
      {recHitXError1B->Fill(lerr_x); recHitYError1B->Fill(lerr_y);}
    else if( (tTopo->pxbLayer(detId)) == 2) 
      {recHitXError2B->Fill(lerr_x); recHitYError2B->Fill(lerr_y);}
    else if( (tTopo->pxbLayer(detId)) == 3) 
      {recHitXError3B->Fill(lerr_x); recHitYError3B->Fill(lerr_y);}
    
    int rows = theGeomDet->specificTopology().nrows(); 
    if (rows == 160) recHitXFullModules->Fill(lp_x);
    else if (rows == 80) recHitXHalfModules->Fill(lp_x);
    
    float tmp1 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,0.)).perp();
    float tmp2 = theGeomDet->surface().toGlobal(Local3DPoint(0.,0.,1.)).perp();
    
    LocalError lape = theGeomDet->localAlignmentError();
    if (lape.valid()) {
      float tmp11= 0.;
      if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
      //float tmp12= sqrt(lape.xy())*1E4;
      float tmp13= 0.;
      if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
      //bool tmp14=tmp2<tmp1;
      if( (tTopo->pxbLayer(detId)) == 1) 
	{recHitXAlignError1->Fill(tmp11); recHitYAlignError1->Fill(tmp13);}
      else if( (tTopo->pxbLayer(detId)) == 2) 
	{recHitXAlignError2->Fill(tmp11); recHitYAlignError2->Fill(tmp13);}
      else if( (tTopo->pxbLayer(detId)) == 3) 
	{recHitXAlignError3->Fill(tmp11); recHitYAlignError3->Fill(tmp13);}
      else {cout<<" unknown layer "<< tTopo->pxbLayer(detId)<<endl;}
      //cout<<tTopo->pxbLayer(detId)<<" "<<tTopo->pxbModule(detId)<<" "<<rows<<" "<<tmp14<<" "
      //  <<tmp11<<" "<<tmp13<<endl;
    }  // if lape

    if (tmp2<tmp1) { // flipped
      for (unsigned int i=0; i<3; i++)  {
	if (tTopo->pxbLayer(detId) == i+1) {
	  recHitXResFlippedLadderLayers[i]->Fill(res_x);
	  recHitXPullFlippedLadderLayers[i]->Fill(pull_x);
	}
      }
    } else {
      for (unsigned int i=0; i<3; i++) {
	if (tTopo->pxbLayer(detId) == i+1) {
	  recHitXResNonFlippedLadderLayers[i]->Fill(res_x);
	  recHitXPullNonFlippedLadderLayers[i]->Fill(pull_x);
	}
      }
    }
  } // end if quick 
  

  //get cluster
  SiPixelRecHit::ClusterRef const& clust = recHit.cluster();

  // as a function of layer
  for (unsigned int i=0; i<3; i++) {
    if (tTopo->pxbLayer(detId) == i+1)  { // select layer

      recHitXResLayers[i]->Fill(res_x);  //QUICK
      recHitYResLayers[i]->Fill(res_y);  //QUICK
      if(!quick) {
	recHitXResLayersP[i]->Fill(eta,std::abs(res_x));
	recHitYResLayersP[i]->Fill(eta,std::abs(res_y));
	int sizeX = (*clust).sizeX();
	if(sizeX==1)      recHitXResLayersP1[i]->Fill(eta,std::abs(res_x));
	else if(sizeX==2) recHitXResLayersP2[i]->Fill(eta,std::abs(res_x));
	else if(sizeX>=3) recHitXResLayersP3[i]->Fill(eta,std::abs(res_x));
	int sizeY = (*clust).sizeY();
	if(sizeY==1)      recHitYResLayersP1[i]->Fill(eta,std::abs(res_y));
	else if(sizeY==2) recHitYResLayersP2[i]->Fill(eta,std::abs(res_y));
	else if(sizeY==3) recHitYResLayersP3[i]->Fill(eta,std::abs(res_y));
	else if(sizeY>=4) recHitYResLayersP4[i]->Fill(eta,std::abs(res_y));
      } // end if quick
    }
  }


  if(quick) return; // skip the rest 

  // as a function of eta
  for (unsigned int i=0; i<25; i++) {
    float eta1=float(i)*0.1;
    float eta2=eta1+0.1;
    if(eta>=eta1 && eta<eta2) {
 
      if (tTopo->pxbLayer(detId) == 1) 
	{heta1->Fill(eta);recHitXResLayer1Eta[i]->Fill(res_x);recHitYResLayer1Eta[i]->Fill(res_y);}
      else if (tTopo->pxbLayer(detId) == 2) 
	{heta2->Fill(eta);recHitXResLayer2Eta[i]->Fill(res_x);recHitYResLayer2Eta[i]->Fill(res_y);}
      else if (tTopo->pxbLayer(detId) == 3) 
	{heta3->Fill(eta);recHitXResLayer3Eta[i]->Fill(res_x);recHitYResLayer3Eta[i]->Fill(res_y);}
      
    } // eta 
  } // i (eta)
  
  // fill module dependent info
  for (unsigned int i=0; i<8; i++) 
    {
      if (tTopo->pxbModule(detId) == i+1) 
	{
	  int sizeY = (*clust).sizeY();
	  clustYSizeModule[i]->Fill(sizeY);
	  
	  if (tTopo->pxbLayer(detId) == 1) 
	    {
	      float charge = (*clust).charge();
	      clustChargeLayer1Modules[i]->Fill(charge);
	      recHitYResLayer1Modules[i]->Fill(res_y);
	      recHitYPullLayer1Modules[i]->Fill(pull_y);
	    }
	  else if (tTopo->pxbLayer(detId) == 2) 
	    {
	      float charge = (*clust).charge();
	      clustChargeLayer2Modules[i]->Fill(charge);
	      recHitYResLayer2Modules[i]->Fill(res_y);
	      recHitYPullLayer2Modules[i]->Fill(pull_y);
	    }
	  else if (tTopo->pxbLayer(detId) == 3) 
	    {
	      float charge = (*clust).charge();
	      clustChargeLayer3Modules[i]->Fill(charge);
	      recHitYResLayer3Modules[i]->Fill(res_y);
	      recHitYPullLayer3Modules[i]->Fill(pull_y);
	    }
	}
    }
  int sizeX = (*clust).sizeX();
  if (tTopo->pxbLayer(detId) == 1) clustXSizeLayer[0]->Fill(sizeX);
  if (tTopo->pxbLayer(detId) == 2) clustXSizeLayer[1]->Fill(sizeX);
  if (tTopo->pxbLayer(detId) == 3) clustXSizeLayer[2]->Fill(sizeX);
}


void SiPixelRecHitsValid_pix::fillForward(const SiPixelRecHit & recHit, const PSimHit & simHit, 
				      DetId detId,const PixelGeomDetUnit * theGeomDet,
				      const TrackerTopology *tTopo) 
{
  const float cmtomicron = 10000.0;
#ifdef QUICK
  const bool quick = true;
#else
  const bool quick = false;
#endif

  int rows = theGeomDet->specificTopology().nrows();
  int cols = theGeomDet->specificTopology().ncolumns();
  
  LocalPoint lp = recHit.localPosition();
  float lp_x = lp.x();
  float lp_y = lp.y();
  
  LocalError lerr = recHit.localPositionError();
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
  recHitXResAllF->Fill(res_x);
  recHitYResAllF->Fill(res_y);  

  if(quick) return; // skip the rest

  recHitXErrorAllF->Fill(lerr_x);
  recHitYErrorAllF->Fill(lerr_y);
  recHitXPullAllF->Fill(pull_x);
  recHitYPullAllF->Fill(pull_y);

  if (rows == 80)       recHitXPlaquetteSize1->Fill(lp_x);
  else if (rows == 160) recHitXPlaquetteSize2->Fill(lp_x);
   
  
  if (cols == 104) 
    {
      recHitYPlaquetteSize2->Fill(lp_y);
    } 
  else if (cols == 156) 
    {
      recHitYPlaquetteSize3->Fill(lp_y);
    } 
  else if (cols == 208) 
    {
      recHitYPlaquetteSize4->Fill(lp_y);
    } 
  else if (cols == 260) 
    {
      recHitYPlaquetteSize5->Fill(lp_y);
    }
  
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
    
  // get cluster
  SiPixelRecHit::ClusterRef const& clust = recHit.cluster();
  
  // fill plaquette dependent info
  for (unsigned int i=0; i<7; i++) 
    {
      if (tTopo->pxfModule(detId) == i+1) 
	{
	  if (tTopo->pxfDisk(detId) == 1) 
	    {
	      int sizeX = (*clust).sizeX();
	      clustXSizeDisk1Plaquettes[i]->Fill(sizeX);
	      
	      int sizeY = (*clust).sizeY();
	      clustYSizeDisk1Plaquettes[i]->Fill(sizeY);
	      
	      float charge = (*clust).charge();
	      clustChargeDisk1Plaquettes[i]->Fill(charge);
	      
	      recHitXResDisk1Plaquettes[i]->Fill(res_x);
	      recHitYResDisk1Plaquettes[i]->Fill(res_y);

	      recHitXPullDisk1Plaquettes[i]->Fill(pull_x);
	      recHitYPullDisk1Plaquettes[i]->Fill(pull_y);
	    }
	  else 
	    {
	      int sizeX = (*clust).sizeX();
	      clustXSizeDisk2Plaquettes[i]->Fill(sizeX);
	      
	      int sizeY = (*clust).sizeY();
	      clustYSizeDisk2Plaquettes[i]->Fill(sizeY);
	      
	      float charge = (*clust).charge();
	      clustChargeDisk2Plaquettes[i]->Fill(charge);
	      
	      recHitXResDisk2Plaquettes[i]->Fill(res_x);
	      recHitYResDisk2Plaquettes[i]->Fill(res_y);

	      recHitXPullDisk2Plaquettes[i]->Fill(pull_x);
	      recHitYPullDisk2Plaquettes[i]->Fill(pull_y);
	      
	    } // end else
	} // end if module
      else if (tTopo->pxfPanel(detId) == 2 && (tTopo->pxfModule(detId)+4) == i+1) 
	{
	  if (tTopo->pxfDisk(detId) == 1) 
	    {
	      int sizeX = (*clust).sizeX();
	      clustXSizeDisk1Plaquettes[i]->Fill(sizeX);
	      
	      int sizeY = (*clust).sizeY();
	      clustYSizeDisk1Plaquettes[i]->Fill(sizeY);
	      
	      float charge = (*clust).charge();
	      clustChargeDisk1Plaquettes[i]->Fill(charge);
	      
	      recHitXResDisk1Plaquettes[i]->Fill(res_x);
	      recHitYResDisk1Plaquettes[i]->Fill(res_y);

	      recHitXPullDisk1Plaquettes[i]->Fill(pull_x);
	      recHitYPullDisk1Plaquettes[i]->Fill(pull_y);
	    }
	  else 
	    {
	      int sizeX = (*clust).sizeX();
	      clustXSizeDisk2Plaquettes[i]->Fill(sizeX);
	      
	      int sizeY = (*clust).sizeY();
	      clustYSizeDisk2Plaquettes[i]->Fill(sizeY);
	      
	      float charge = (*clust).charge();
	      clustChargeDisk2Plaquettes[i]->Fill(charge);
	      
	      recHitXResDisk2Plaquettes[i]->Fill(res_x);
	      recHitYResDisk2Plaquettes[i]->Fill(res_y);

	      recHitXPullDisk2Plaquettes[i]->Fill(pull_x);
	      recHitYPullDisk2Plaquettes[i]->Fill(pull_y);

	    } // end else
        } // end else
    } // end for
}
//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelRecHitsValid_pix);
