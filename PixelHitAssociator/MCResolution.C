#include "common_functions.h"

void MCResolution() {
  gROOT->SetStyle("Plain");
  stat_height = 0.2;
  gStyle->SetOptStat(10);
  gStyle->SetOptFit(11);
  gStyle->SetStatFormat("2.1f");
  gStyle->SetFitFormat("2.1f");
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(1);
  
  remove_prelim_label = 2;
  formats = { ".png" };
  
  bool cpe_fix = 0;
  std::string dir = "DQMData/TrackerRecHitsV/TrackerRecHits/Pixel/";  
  std::vector<std::tuple<const char*, std::string, Color_t> > plots_to_draw;
  std::vector<std::tuple<std::string, std::string, std::string, std::string> > histos;
  std::string xres = "SimHit-RecHit X-residuals (#mum)", yres = "SimHit-RecHit Y-residuals (#mum)";
  std::string legend_prefix = "";
  
  // Phase 1
  const char* desi_gen = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_GenericReco_OnTrackHits_design_10000.root";
  const char* desi_tem = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_TemplateReco_OnTrackHits_design_10000.root";
  const char* real_gen = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_GenericReco_OnTrackHits_50000.root";
  const char* real_tem = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_TemplateReco_OnTrackHits_50000.root";
  const char* gen  = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_GenericReco_OnTrackHits_NewGeom_10k.root";
  const char* temp = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_TemplateReco_OnTrackHits_NewGeom_10k.root";
  //gen  = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_GenericReco_OnTrackHits_NewGeom_100.root";
  //temp = "/data/jkarancs/CMSSW/PhaseI/Latest/SimToRecHitHistos_TemplateReco_OnTrackHits_NewGeom_100.root";
  // Tanja's comparison
  //real_tem = "/afs/cern.ch/user/j/jkarancs/public/h_10000.root";
  //compare_two(real_gen, real_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side2_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_pZ_Pan2_AlgoComp", "Ring2 (+Z, Panel2)",  "Generic (Mine)","Generic (Tanja)", 601, 634);
  const char* tem_def  = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_default_10000.root";
  const char* tem_fix  = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_fix_1000.root"; // pre2 (300um, big pixels fix)
  const char* tem_fix2 = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_fix2_10000.root"; // pre3 (290 um + ?big pix?)
  //const char* tem_fix3 = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_fix3_10000.root"; // pre4+PR (290 um + big pix, old temp)
  const char* tem_fix3 = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_1k_ls0_la0_ge0_te0.root"; // pre4+PR (290 um + big pix, old temp)
  const char* tem_fix4 = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_fix4_3000.root"; // pre4+PR+New conditions
  const char* gen_def  = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_default_10000.root";
  const char* gen_fix2 = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_fix2_10000.root"; // pre3 (290 um + ?big pix?)
  const char* gen_fix4 = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_fix4_10000.root"; // pre4+PR+New conditions

  const char* gen_phase1_old_oldcond = "/data/jkarancs/CMSSW/PhaseI/test/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_oldcode_10k.root";
  const char* tem_phase1_old_oldcond = "/data/jkarancs/CMSSW/PhaseI/test/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_oldcode_10k.root";
  const char* gen_phase1_old_newcond = "/data/jkarancs/CMSSW/PhaseI/test/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_oldcode_newcond_10k.root";
  const char* tem_phase1_old_newcond = "/data/jkarancs/CMSSW/PhaseI/test/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_oldcode_newcond_10k.root";
  const char* gen_phase1_new_oldcond = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_newcode_oldcond_50k.root";
  const char* tem_phase1_new_oldcond = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_newcode_oldcond_50k.root";
  const char* gen_phase1_new_newcond = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_newcode_newcond_50k.root";
  const char* tem_phase1_new_newcond = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_newcode_newcond_50k.root";
  
  // Phase 0
  const char* gen_phase0_old_oldcond = "/data/jkarancs/CMSSW/PhaseI/test/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase0_oldcode_10k.root";
  const char* tem_phase0_old_oldcond = "/data/jkarancs/CMSSW/PhaseI/test/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase0_oldcode_10k.root";
  const char* gen_phase0_new_oldcond = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase0_newcode_10k.root";
  const char* tem_phase0_new_oldcond = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase0_newcode_10k.root";

  // Phase 1
  // MC
  // 0T
  const char* gen_phase1_mc_0T       = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_newcode_newcond_50k.root";
  const char* tem_phase1_mc_0T       = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_newcode_newcond_50k.root";
  const char* gen_phase1_mc_38T      = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_91X_MC0T_4k.root";
  const char* tem_phase1_mc_38T      = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre4/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_91X_MC0T_4k.root";
  // Data 2017
  const char* gen_phase1_data_0T  = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre6/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_91X_Data0T_10k.root";
  const char* tem_phase1_data_0T  = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre6/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_91X_Data0T_10k.root";
  const char* gen_phase1_data_38T = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre6/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/gen_phase1_91X_Data38T_4k.root";
  const char* tem_phase1_data_38T = "/data/jkarancs/CMSSW/PhaseI/CMSSW_9_0_0_pre6/src/DPGAnalysis-SiPixelTools/PixelHitAssociator/tem_phase1_91X_Data38T_4k.root";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2016_11_16/";
  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2016_11_18/";
  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2016_11_22/50k/";
  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_02_08/";
  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_02_17/";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_02_17/old_cond_comp/";
  //plots_to_draw.push_back(std::make_tuple(gen_fix4, "Generic" , kBlue+1));
  //plots_to_draw.push_back(std::make_tuple(tem_fix4, "Template", kRed+2));
    
  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase0/2017_02_22/CodeComp_Generic/";
  //plots_to_draw.push_back(std::make_tuple(gen_phase0_old_oldcond, "Old Code" , kBlue+1));
  //plots_to_draw.push_back(std::make_tuple(gen_phase0_new_oldcond, "New Code",  kRed+2));
  //legend_prefix = "Phase 0, Generic";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase0/2017_02_22/CodeComp_Template/";
  //plots_to_draw.push_back(std::make_tuple(tem_phase0_old_oldcond, "Old Code" , kBlue+1));
  //plots_to_draw.push_back(std::make_tuple(tem_phase0_new_oldcond, "New Code",  kRed+2));
  //legend_prefix = "Phase 0, Template";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_03_01/CodeAndCondComp_Generic/";
  ////plots_to_draw.push_back(std::make_tuple(gen_phase1_old_oldcond, "Old Code - Old Cond" , kGreen+1));
  //plots_to_draw.push_back(std::make_tuple(gen_phase1_new_oldcond, "New Code - Old Cond",  kBlue+2));
  //plots_to_draw.push_back(std::make_tuple(gen_phase1_new_newcond, "New Code - New Cond",  kRed+2));
  //legend_prefix = "Generic";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_03_01/CodeAndCondComp_Template/";
  ////plots_to_draw.push_back(std::make_tuple(tem_phase1_old_oldcond, "Old Code - Old Cond" , kGreen+1));
  //plots_to_draw.push_back(std::make_tuple(tem_phase1_new_oldcond, "New Code - Old Cond",  kBlue+2));
  //plots_to_draw.push_back(std::make_tuple(tem_phase1_new_newcond, "New Code - New Cond",  kRed+2));
  //legend_prefix = "Template";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_03_21/DataPayloadsInMC_38T/";
  //plots_to_draw.push_back(std::make_tuple(gen_phase1_data_38T, "Generic",   kBlue+2));
  //plots_to_draw.push_back(std::make_tuple(tem_phase1_data_38T, "Template",  kRed+2));
  //legend_prefix = "3.8T Startup";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_03_21/DataPayloadsInMC_0T/";
  //plots_to_draw.push_back(std::make_tuple(gen_phase1_data_0T, "Generic",   kBlue+2));
  //plots_to_draw.push_back(std::make_tuple(tem_phase1_data_0T, "Template",  kRed+2));
  //legend_prefix = "0T Startup";

  //save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_04_05/MC_0T/";
  //plots_to_draw.push_back(std::make_tuple(tem_phase1_mc_0T, "Template",  kRed+2));
  //plots_to_draw.push_back(std::make_tuple(gen_phase1_mc_0T, "Generic",   kBlue+2));
  //legend_prefix = "0T MC";

  save_dir = "/afs/cern.ch/user/j/jkarancs/public/Resolution/2017MC/Phase1/2017_04_05/DataPayloadsInMC_38T/";
  plots_to_draw.push_back(std::make_tuple(gen_phase1_data_38T, "Generic",   kBlue+2));
  plots_to_draw.push_back(std::make_tuple(tem_phase1_data_38T, "Template",  kRed+2));
  legend_prefix = "3.8T Startup (New)";

  save = 1;
  int detail  = 1;
  bool bpix_x = 1;
  bool bpix_y = 1;
  bool fpix_x = 1;
  bool fpix_y = 1;

  // Specify x-axis title, histogram name, output filename and legend title
  if (bpix_x) {
    if (detail>=0) {
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_xres_b_All",                      "BPixX",                    "BPix"));
    }
    if (detail>0) {
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_Layer1",                     "BPixX_L1",                 "Layer 1"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_Layer2",                     "BPixX_L2",                 "Layer 2"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_Layer3",                     "BPixX_L3",                 "Layer 3"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_Layer4",                     "BPixX_L4",                 "Layer 4"));
    }
    if (detail>1) {
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1",    "BPixX_L1_NonFillped",      "Layer 1 (flipped)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2",    "BPixX_L2_NonFillped",      "Layer 2 (flipped)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3",    "BPixX_L3_NonFillped",      "Layer 3 (flipped)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4",    "BPixX_L4_NonFillped",      "Layer 4 (flipped)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer1",       "BPixX_L1_Flipped",         "Layer 1 (unflipped)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer2",       "BPixX_L2_Flipped",         "Layer 2 (unflipped)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer3",       "BPixX_L3_Flipped",         "Layer 3 (unflipped)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer4",       "BPixX_L4_Flipped",         "Layer 4 (unflipped)"));
    }
    if (detail>2) {
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_mZ", "BPixX_L1_NonFlipped_mZ",   "Layer 1 (unflipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_mZ", "BPixX_L2_NonFlipped_mZ",   "Layer 2 (unflipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_mZ", "BPixX_L3_NonFlipped_mZ",   "Layer 3 (unflipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_mZ", "BPixX_L4_NonFlipped_mZ",   "Layer 4 (unflipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_pZ", "BPixX_L1_NonFlipped_pZ",   "Layer 1 (unflipped, +Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_pZ", "BPixX_L2_NonFlipped_pZ",   "Layer 2 (unflipped, +Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_pZ", "BPixX_L3_NonFlipped_pZ",   "Layer 3 (unflipped, +Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_pZ", "BPixX_L4_NonFlipped_pZ",   "Layer 4 (unflipped, +Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_mZ",    "BPixX_L1_Flipped_mZ",      "Layer 1 (flipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_mZ",    "BPixX_L2_Flipped_mZ",      "Layer 2 (flipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_mZ",    "BPixX_L3_Flipped_mZ",      "Layer 3 (flipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_mZ",    "BPixX_L4_Flipped_mZ",      "Layer 4 (flipped, -Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_pZ",    "BPixX_L1_Flipped_pZ",      "Layer 1 (flipped, +Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_pZ",    "BPixX_L2_Flipped_pZ",      "Layer 2 (flipped, +Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_pZ",    "BPixX_L3_Flipped_pZ",      "Layer 3 (flipped, +Z)"));
      histos.push_back(std::make_tuple(xres, "recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_pZ",    "BPixX_L4_Flipped_pZ",      "Layer 4 (flipped, +Z)"));
    }
  }
  if (bpix_y) {
    if (detail>=0) {
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_yres_b_All",                      "BPixY",                    "BPix"));
    }
    if (detail>0) {
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1",                     "BPixY_L1",                 "Layer 1"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2",                     "BPixY_L2",                 "Layer 2"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3",                     "BPixY_L3",                 "Layer 3"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4",                     "BPixY_L4",                 "Layer 4"));
    }
    if (detail>2) {
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module1",             "BPixY_L1_Mm4",             "Layer 1, Mod -4"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module2",             "BPixY_L1_Mm3",             "Layer 1, Mod -3"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module3",             "BPixY_L1_Mm2",             "Layer 1, Mod -2"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module4",             "BPixY_L1_Mm1",             "Layer 1, Mod -1"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module5",             "BPixY_L1_Mp1",             "Layer 1, Mod +1"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module6",             "BPixY_L1_Mp2",             "Layer 1, Mod +2"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module7",             "BPixY_L1_Mp3",             "Layer 1, Mod +3"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer1_Module8",             "BPixY_L1_Mp4",             "Layer 1, Mod +4"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module1",             "BPixY_L2_Mm4",             "Layer 2, Mod -4"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module2",             "BPixY_L2_Mm3",             "Layer 2, Mod -3"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module3",             "BPixY_L2_Mm2",             "Layer 2, Mod -2"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module4",             "BPixY_L2_Mm1",             "Layer 2, Mod -1"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module5",             "BPixY_L2_Mp1",             "Layer 2, Mod +1"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module6",             "BPixY_L2_Mp2",             "Layer 2, Mod +2"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module7",             "BPixY_L2_Mp3",             "Layer 2, Mod +3"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer2_Module8",             "BPixY_L2_Mp4",             "Layer 2, Mod +4"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module1",             "BPixY_L3_Mm4",             "Layer 3, Mod -4"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module2",             "BPixY_L3_Mm3",             "Layer 3, Mod -3"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module3",             "BPixY_L3_Mm2",             "Layer 3, Mod -2"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module4",             "BPixY_L3_Mm1",             "Layer 3, Mod -1"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module5",             "BPixY_L3_Mp1",             "Layer 3, Mod +1"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module6",             "BPixY_L3_Mp2",             "Layer 3, Mod +2"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module7",             "BPixY_L3_Mp3",             "Layer 3, Mod +3"));
      histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer3_Module8",             "BPixY_L3_Mp4",             "Layer 3, Mod +4"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module1",             "BPixY_L4_Mm4",             "Layer 4, Mod -4"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module2",             "BPixY_L4_Mm3",             "Layer 4, Mod -3"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module3",             "BPixY_L4_Mm2",             "Layer 4, Mod -2"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module4",             "BPixY_L4_Mm1",             "Layer 4, Mod -1"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module5",             "BPixY_L4_Mp1",             "Layer 4, Mod +1"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module6",             "BPixY_L4_Mp2",             "Layer 4, Mod +2"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module7",             "BPixY_L4_Mp3",             "Layer 4, Mod +3"));
      //histos.push_back(std::make_tuple(yres, "recHitBPIX/RecHit_YRes_Layer4_Module8",             "BPixY_L4_Mp4",             "Layer 4, Mod +4"));
    }
  }
  if (fpix_x) {
    if (detail>=0) {
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_xres_f_All",                      "FPixX",               "FPix"));
    }
    if (detail>0) {
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring1_Side1_Panel1",         "FPixX_Ring1_mZ_Pan1", "Ring1 (-Z, Panel1)"));
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring1_Side1_Panel2",         "FPixX_Ring1_mZ_Pan2", "Ring1 (-Z, Panel2)"));
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring1_Side2_Panel1",         "FPixX_Ring1_pZ_Pan1", "Ring1 (+Z, Panel1)"));
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring1_Side2_Panel2",         "FPixX_Ring1_pZ_Pan2", "Ring1 (+Z, Panel2)"));
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring2_Side1_Panel1",         "FPixX_Ring2_mZ_Pan1", "Ring2 (-Z, Panel1)"));
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring2_Side1_Panel2",         "FPixX_Ring2_mZ_Pan2", "Ring2 (-Z, Panel2)"));
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring2_Side2_Panel1",         "FPixX_Ring2_pZ_Pan1", "Ring2 (+Z, Panel1)"));
      histos.push_back(std::make_tuple(xres, "recHitFPIX/RecHit_XRes_Ring2_Side2_Panel2",         "FPixX_Ring2_pZ_Pan2", "Ring2 (+Z, Panel2)"));
    }
  }
  if (fpix_y) {
    if (detail>=0) {
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_yres_f_All",                      "FPixY",               "FPix"));
    }
    if (detail>0) {
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring1_Side1_Panel1",         "FPixY_Ring1_mZ_Pan1", "Ring1 (-Z, Panel1)"));
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring1_Side1_Panel2",         "FPixY_Ring1_mZ_Pan2", "Ring1 (-Z, Panel2)"));
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring1_Side2_Panel1",         "FPixY_Ring1_pZ_Pan1", "Ring1 (+Z, Panel1)"));
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring1_Side2_Panel2",         "FPixY_Ring1_pZ_Pan2", "Ring1 (+Z, Panel2)"));
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring2_Side1_Panel1",         "FPixY_Ring2_mZ_Pan1", "Ring2 (-Z, Panel1)"));
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring2_Side1_Panel2",         "FPixY_Ring2_mZ_Pan2", "Ring2 (-Z, Panel2)"));
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring2_Side2_Panel1",         "FPixY_Ring2_pZ_Pan1", "Ring2 (+Z, Panel1)"));
      histos.push_back(std::make_tuple(yres, "recHitFPIX/RecHit_YRes_Ring2_Side2_Panel2",         "FPixY_Ring2_pZ_Pan2", "Ring2 (+Z, Panel2)"));
    }
  }

  if (plots_to_draw.size()==2) {
    for (const auto& item : histos) {
      std::string hname, canname, legtitle, xtitle, leg1, leg2;
      const char *file1, *file2;
      Color_t col1, col2;
      std::tie(xtitle, hname, canname, legtitle) = item;
      std::tie(file1, leg1, col1) = plots_to_draw[0];
      std::tie(file2, leg2, col2) = plots_to_draw[1];
      if (legend_prefix.size()) legtitle = legend_prefix+", "+legtitle;
      compare_two(file1, file2, dir+hname, xtitle, canname, legtitle, leg1,leg2, col1, col2);
    }
  } else if (plots_to_draw.size()==3) {
    for (const auto& item : histos) {
      std::string hname, canname, legtitle, xtitle, leg1, leg2, leg3;
      const char *file1, *file2, *file3;
      Color_t col1, col2, col3;
      std::tie(xtitle, hname, canname, legtitle) = item;
      std::tie(file1, leg1, col1) = plots_to_draw[0];
      std::tie(file2, leg2, col2) = plots_to_draw[1];
      std::tie(file3, leg3, col3) = plots_to_draw[2];
      if (legend_prefix.size()) legtitle = legend_prefix+", "+legtitle;
      compare_three(file1, file2, file3, dir+hname, xtitle, canname, legtitle, leg1,leg2, leg3, col1, col2, col3);
    }
  }
    
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1", xres, "XResiduals_L1_Gen_FlipComp", "L1 (Generic)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2", xres, "XResiduals_L2_Gen_FlipComp", "L2 (Generic)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3", xres, "XResiduals_L3_Gen_FlipComp", "L3 (Generic)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4", xres, "XResiduals_L4_Gen_FlipComp", "L4 (Generic)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1", xres, "XResiduals_L1_Tem_FlipComp", "L1 (Template)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2", xres, "XResiduals_L2_Tem_FlipComp", "L2 (Template)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3", xres, "XResiduals_L3_Tem_FlipComp", "L3 (Template)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4", xres, "XResiduals_L4_Tem_FlipComp", "L4 (Template)", "Flipped","Non-flipped", 619,418);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_pZ", xres, "XResiduals_L1_Gen_NonFlip_ZComp", "L1 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_pZ", xres, "XResiduals_L2_Gen_NonFlip_ZComp", "L2 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_pZ", xres, "XResiduals_L3_Gen_NonFlip_ZComp", "L3 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_pZ", xres, "XResiduals_L4_Gen_NonFlip_ZComp", "L4 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_pZ", xres, "XResiduals_L1_Gen_Flip_ZComp", "L1 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_pZ", xres, "XResiduals_L2_Gen_Flip_ZComp", "L2 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_pZ", xres, "XResiduals_L3_Gen_Flip_ZComp", "L3 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_pZ", xres, "XResiduals_L4_Gen_Flip_ZComp", "L4 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_pZ", xres, "XResiduals_L1_Gen_NonFlip_ZComp", "L1 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_pZ", xres, "XResiduals_L2_Gen_NonFlip_ZComp", "L2 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_pZ", xres, "XResiduals_L3_Gen_NonFlip_ZComp", "L3 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_pZ", xres, "XResiduals_L4_Gen_NonFlip_ZComp", "L4 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_pZ", xres, "XResiduals_L1_Gen_Flip_ZComp", "L1 (Flipped, Template)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_pZ", xres, "XResiduals_L2_Gen_Flip_ZComp", "L2 (Flipped, Template)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_pZ", xres, "XResiduals_L3_Gen_Flip_ZComp", "L3 (Flipped, Template)", "-Z","+Z", 601,634);
  //++  compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_pZ", xres, "XResiduals_L4_Gen_Flip_ZComp", "L4 (Flipped, Template)", "-Z","+Z", 601,634);
    
  //++  if (bpix_x) {
  //++    // ___________________________________________________
  //++    //                      BARREL - X
  //++    // GTComp (OK)
  //++    //compare_two(desi_tem, real_tem, dir+"recHitBPIX/RecHit_xres_b_All",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_BPix_Temp_GTComp",   "BPix (Templates)", "Design","Realistic", 619, 634);
  //++    //compare_two(desi_gen, real_gen, dir+"recHitBPIX/RecHit_xres_b_All",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_BPix_Gene_GTComp",   "BPix (Generic)",   "Design","Realistic", 418, 601);
  //++    /*
  //++    // AlgoComp
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_xres_b_All",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_BPix_AlgoComp", "BPix",    "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_Layer1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_AlgoComp",   "Layer 1", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_Layer2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_AlgoComp",   "Layer 2", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_Layer3", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_AlgoComp",   "Layer 3", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_Layer4", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_AlgoComp",   "Layer 4", "Generic","Template", 601, 634);
  //++    
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_AlgoComp",   "Layer 1 (flipped)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_AlgoComp",   "Layer 2 (flipped)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_AlgoComp",   "Layer 3 (flipped)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_AlgoComp",   "Layer 4 (flipped)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_AlgoComp",   "Layer 1 (unflipped)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_AlgoComp",   "Layer 2 (unflipped)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_AlgoComp",   "Layer 3 (unflipped)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_AlgoComp",   "Layer 4 (unflipped)", "Generic","Template", 601, 634);
  //++    */
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_NonFlipped_mZ_AlgoComp",   "Layer 1 (unflipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_NonFlipped_mZ_AlgoComp",   "Layer 2 (unflipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_NonFlipped_mZ_AlgoComp",   "Layer 3 (unflipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_NonFlipped_mZ_AlgoComp",   "Layer 4 (unflipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_NonFlipped_pZ_AlgoComp",   "Layer 1 (unflipped, +Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_NonFlipped_pZ_AlgoComp",   "Layer 2 (unflipped, +Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_NonFlipped_pZ_AlgoComp",   "Layer 3 (unflipped, +Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_NonFlipped_pZ_AlgoComp",   "Layer 4 (unflipped, +Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Flipped_mZ_AlgoComp",   "Layer 1 (flipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Flipped_mZ_AlgoComp",   "Layer 2 (flipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Flipped_mZ_AlgoComp",   "Layer 3 (flipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_mZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Flipped_mZ_AlgoComp",   "Layer 4 (flipped, -Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Flipped_pZ_AlgoComp",   "Layer 1 (flipped, +Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Flipped_pZ_AlgoComp",   "Layer 2 (flipped, +Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Flipped_pZ_AlgoComp",   "Layer 3 (flipped, +Z)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Flipped_pZ_AlgoComp",   "Layer 4 (flipped, +Z)", "Generic","Template", 601, 634);
  //++    
  //++    /*
  //++    // L1-4 - FlipComp
  //++    // Generic (OK)
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Gen_FlipComp", "L1 (Generic)", "Flipped","Non-flipped", 619,418);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Gen_FlipComp", "L2 (Generic)", "Flipped","Non-flipped", 619,418);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Gen_FlipComp", "L3 (Generic)", "Flipped","Non-flipped", 619,418);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Gen_FlipComp", "L4 (Generic)", "Flipped","Non-flipped", 619,418);
  //++    // Template (OK)
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Tem_FlipComp", "L1 (Template)", "Flipped","Non-flipped", 619,418);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Tem_FlipComp", "L2 (Template)", "Flipped","Non-flipped", 619,418);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Tem_FlipComp", "L3 (Template)", "Flipped","Non-flipped", 619,418);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Tem_FlipComp", "L4 (Template)", "Flipped","Non-flipped", 619,418);
  //++  
  //++    // L1-4 - +-Z Comp
  //++    // Generic
  //++    // NonFlipped
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Gen_NonFlip_ZComp", "L1 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Gen_NonFlip_ZComp", "L2 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Gen_NonFlip_ZComp", "L3 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Gen_NonFlip_ZComp", "L4 (Non-flipped, Generic)", "-Z","+Z", 601,634);
  //++    // Flipped
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Gen_Flip_ZComp", "L1 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Gen_Flip_ZComp", "L2 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Gen_Flip_ZComp", "L3 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++    compare_two(real_gen, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Gen_Flip_ZComp", "L4 (Flipped, Generic)", "-Z","+Z", 601,634);
  //++    // Template
  //++    // NonFlipped
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer1_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Gen_NonFlip_ZComp", "L1 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer2_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Gen_NonFlip_ZComp", "L2 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer3_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Gen_NonFlip_ZComp", "L3 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_NonFlippedLadder_Layer4_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Gen_NonFlip_ZComp", "L4 (Non-flipped, Template)", "-Z","+Z", 601,634);
  //++    // Flipped
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer1_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_Gen_Flip_ZComp", "L1 (Flipped, Template)", "-Z","+Z", 601,634);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer2_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_Gen_Flip_ZComp", "L2 (Flipped, Template)", "-Z","+Z", 601,634);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer3_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_Gen_Flip_ZComp", "L3 (Flipped, Template)", "-Z","+Z", 601,634);
  //++    compare_two(real_tem, dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_mZ", dir+"recHitBPIX/RecHit_XRes_FlippedLadder_Layer4_pZ", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_Gen_Flip_ZComp", "L4 (Flipped, Template)", "-Z","+Z", 601,634);
  //++    */
  //++  }
  //++  
  //++  if (bpix_y) {
  //++    // ___________________________________________________
  //++    //                      BARREL - Y
  //++  
  //++    // GTComp (OK)
  //++    //compare_two(desi_tem, real_tem, dir+"recHitBPIX/RecHit_yres_b_All",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_BPix_Temp_GTComp",   "BPix (Templates)", "Design","Realistic", 619, 634); 
  //++    //compare_two(desi_gen, real_gen, dir+"recHitBPIX/RecHit_yres_b_All",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_BPix_Gene_GTComp",   "BPix (Generic)",   "Design","Realistic", 418, 601);
  //++    
  //++    // AlgoComp
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_yres_b_All",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_BPix_AlgoComp", "BPix",    "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_AlgoComp",   "Layer 1", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_AlgoComp",   "Layer 2", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_AlgoComp",   "Layer 3", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_AlgoComp",   "Layer 4", "Generic","Template", 601, 634);
  //++    
  //++    // Module 1-8
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mm4_AlgoComp",   "Layer 1, Mod -4", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mm3_AlgoComp",   "Layer 1, Mod -3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module3", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mm2_AlgoComp",   "Layer 1, Mod -2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module4", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mm1_AlgoComp",   "Layer 1, Mod -1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module5", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mp1_AlgoComp",   "Layer 1, Mod +1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module6", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mp2_AlgoComp",   "Layer 1, Mod +2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module7", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mp3_AlgoComp",   "Layer 1, Mod +3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer1_Module8", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_Mp4_AlgoComp",   "Layer 1, Mod +4", "Generic","Template", 601, 634);
  //++    //
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mm4_AlgoComp",   "Layer 2, Mod -4", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mm3_AlgoComp",   "Layer 2, Mod -3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module3", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mm2_AlgoComp",   "Layer 2, Mod -2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module4", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mm1_AlgoComp",   "Layer 2, Mod -1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module5", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mp1_AlgoComp",   "Layer 2, Mod +1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module6", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mp2_AlgoComp",   "Layer 2, Mod +2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module7", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mp3_AlgoComp",   "Layer 2, Mod +3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer2_Module8", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_Mp4_AlgoComp",   "Layer 2, Mod +4", "Generic","Template", 601, 634);
  //++    //
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mm4_AlgoComp",   "Layer 3, Mod -4", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mm3_AlgoComp",   "Layer 3, Mod -3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module3", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mm2_AlgoComp",   "Layer 3, Mod -2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module4", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mm1_AlgoComp",   "Layer 3, Mod -1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module5", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mp1_AlgoComp",   "Layer 3, Mod +1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module6", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mp2_AlgoComp",   "Layer 3, Mod +2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module7", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mp3_AlgoComp",   "Layer 3, Mod +3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer3_Module8", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_Mp4_AlgoComp",   "Layer 3, Mod +4", "Generic","Template", 601, 634);
  //++    //
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mm4_AlgoComp",   "Layer 4, Mod -4", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mm3_AlgoComp",   "Layer 4, Mod -3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module3", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mm2_AlgoComp",   "Layer 4, Mod -2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module4", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mm1_AlgoComp",   "Layer 4, Mod -1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module5", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mp1_AlgoComp",   "Layer 4, Mod +1", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module6", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mp2_AlgoComp",   "Layer 4, Mod +2", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module7", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mp3_AlgoComp",   "Layer 4, Mod +3", "Generic","Template", 601, 634);
  //++    //compare_two(algo_gen, algo_tem, dir+"recHitBPIX/RecHit_YRes_Layer4_Module8", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_Mp4_AlgoComp",   "Layer 4, Mod +4", "Generic","Template", 601, 634);
  //++  
  //++  }
  //++  
  //++  if (fpix_x) {
  //++  
  //++    // ___________________________________________________
  //++    //                      FORWARD - X
  //++  
  //++    // AlgoComp
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Odd",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Odd_AlgoComp",  "Ring1 (-Z, Odd Blade)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Even", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Even_AlgoComp", "Ring1 (-Z, Even Blade)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side2_Odd",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_pZ_Odd_AlgoComp",  "Ring1 (+Z, Odd Blade)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side2_Even", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_pZ_Even_AlgoComp", "Ring1 (+Z, Even Blade)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan1_AlgoComp", "Ring2 (-Z, Panel1)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan2_AlgoComp", "Ring2 (-Z, Panel2)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side2_Panel1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_pZ_Pan1_AlgoComp", "Ring2 (+Z, Panel1)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side2_Panel2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_pZ_Pan2_AlgoComp", "Ring2 (+Z, Panel2)",  "Generic","Template", 601, 634);
  //++  
  //++    // Design GT
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Odd",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Odd_Desi_AlgoComp",  "Ring1 (-Z, Odd Blade, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Even", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Even_Desi_AlgoComp", "Ring1 (-Z, Even Blade, Design)", "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side2_Odd",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_pZ_Odd_Desi_AlgoComp",  "Ring1 (+Z, Odd Blade, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes__Ring1_Side2_Even", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_pZ_Even_Desi_AlgoComp", "Ring1 (+Z, Even Blade, Design)", "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan1_Desi_AlgoComp", "Ring2 (-Z, Panel1, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan2_Desi_AlgoComp", "Ring2 (-Z, Panel2, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side2_Panel1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_pZ_Pan1_Desi_AlgoComp", "Ring2 (+Z, Panel1, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_XRes_Ring2_Side2_Panel2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_pZ_Pan2_Desi_AlgoComp", "Ring2 (+Z, Panel2, Design)",  "Generic","Template", 601, 634);
  //++  }
  //++  
  //++  if (fpix_y) {
  //++    // ___________________________________________________
  //++    //                      FORWARD - Y
  //++  
  //++    // AlgoComp
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Odd",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Odd_AlgoComp",  "Ring1 (-Z, Odd Blade)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Even", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Even_AlgoComp", "Ring1 (-Z, Even Blade)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side2_Odd",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_pZ_Odd_AlgoComp",  "Ring1 (+Z, Odd Blade)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side2_Even", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_pZ_Even_AlgoComp", "Ring1 (+Z, Even Blade)", "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan1_AlgoComp", "Ring2 (-Z, Panel1)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan2_AlgoComp", "Ring2 (-Z, Panel2)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side2_Panel1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_pZ_Pan1_AlgoComp", "Ring2 (+Z, Panel1)",  "Generic","Template", 601, 634);
  //++    compare_two(algo_gen, algo_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side2_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_pZ_Pan2_AlgoComp", "Ring2 (+Z, Panel2)",  "Generic","Template", 601, 634);
  //++  
  //++    // Design GT
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Odd",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Odd_Desi_AlgoComp",  "Ring1 (-Z, Odd Blade, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Even", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Even_Desi_AlgoComp", "Ring1 (-Z, Even Blade, Design)", "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side2_Odd",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_pZ_Odd_Desi_AlgoComp",  "Ring1 (+Z, Odd Blade, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes__Ring1_Side2_Even", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_pZ_Even_Desi_AlgoComp", "Ring1 (+Z, Even Blade, Design)", "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan1_Desi_AlgoComp", "Ring2 (-Z, Panel1, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan2_Desi_AlgoComp", "Ring2 (-Z, Panel2, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side2_Panel1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_pZ_Pan1_Desi_AlgoComp", "Ring2 (+Z, Panel1, Design)",  "Generic","Template", 601, 634);
  //++    //compare_two(desi_gen, desi_tem, dir+"recHitFPIX/RecHit_YRes_Ring2_Side2_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_pZ_Pan2_Desi_AlgoComp", "Ring2 (+Z, Panel2, Design)",  "Generic","Template", 601, 634);
  //++  
  //++  }
  //++  
  //++  if (cpe_fix) {
  //++    // Generic reco
  //++    compare_two(gen_def, gen_fix2, dir+"recHitBPIX/RecHit_xres_b_All",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_BPix_GenComp", "BPix", "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitBPIX/RecHit_yres_b_All",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_BPix_GenComp", "BPix", "Generic","Generic + big pixels fix", 634, 601);
  //++    
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Odd",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Odd_GenComp",  "Ring1 (-Z, Odd Blade)",  "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Even", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Even_GenComp", "Ring1 (-Z, Even Blade)", "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan1_GenComp", "Ring2 (-Z, Panel1)",  "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan2_GenComp", "Ring2 (-Z, Panel2)",  "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Odd",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Odd_GenComp",  "Ring1 (-Z, Odd Blade)",  "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Even", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Even_GenComp", "Ring1 (-Z, Even Blade)", "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan1_GenComp", "Ring2 (-Z, Panel1)",  "Generic","Generic + big pixels fix", 634, 601);
  //++    compare_two(gen_def, gen_fix2, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan2_GenComp", "Ring2 (-Z, Panel2)",  "Generic","Generic + big pixels fix", 634, 601);
  //++    
  //++    // Template reco
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_xres_b_All",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_BPix_TempComp", "BPix",    "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_yres_b_All",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_BPix_TempComp", "BPix",    "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_XRes_Layer1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L1_TempComp",   "Layer 1", "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_XRes_Layer2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L2_TempComp",   "Layer 2", "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_XRes_Layer3", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L3_TempComp",   "Layer 3", "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_XRes_Layer4", "SimHit-RecHit X-residuals (#mum)", "XResiduals_L4_TempComp",   "Layer 4", "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_YRes_Layer1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L1_TempComp",   "Layer 1", "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_YRes_Layer2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L2_TempComp",   "Layer 2", "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_YRes_Layer3", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L3_TempComp",   "Layer 3", "Default (pre2)","Big pixels fix", 634, 418);
  //++    compare_two(tem_def, tem_fix3, dir+"recHitBPIX/RecHit_YRes_Layer4", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_L4_TempComp",   "Layer 4", "Default (pre2)","Big pixels fix", 634, 418);
  //++    
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Odd",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Odd_TempComp",  "Ring1 (-Z, Odd Blade)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes__Ring1_Side1_Even", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_mZ_Even_TempComp", "Ring1 (-Z, Even Blade)", "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan1_TempComp", "Ring2 (-Z, Panel1)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes_Ring2_Side1_Panel2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_mZ_Pan2_TempComp", "Ring2 (-Z, Panel2)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes__Ring1_Side2_Odd",  "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_pZ_Odd_TempComp",  "Ring1 (+Z, Odd Blade)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes__Ring1_Side2_Even", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring1_pZ_Even_TempComp", "Ring1 (+Z, Even Blade)", "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes_Ring2_Side2_Panel1", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_pZ_Pan1_TempComp", "Ring2 (+Z, Panel1)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_XRes_Ring2_Side2_Panel2", "SimHit-RecHit X-residuals (#mum)", "XResiduals_FPix_Ring2_pZ_Pan2_TempComp", "Ring2 (+Z, Panel2)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Odd",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Odd_TempComp",  "Ring1 (-Z, Odd Blade)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes__Ring1_Side1_Even", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_mZ_Even_TempComp", "Ring1 (-Z, Even Blade)", "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan1_TempComp", "Ring2 (-Z, Panel1)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes_Ring2_Side1_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_mZ_Pan2_TempComp", "Ring2 (-Z, Panel2)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes__Ring1_Side2_Odd",  "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_pZ_Odd_TempComp",  "Ring1 (+Z, Odd Blade)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes__Ring1_Side2_Even", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring1_pZ_Even_TempComp", "Ring1 (+Z, Even Blade)", "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes_Ring2_Side2_Panel1", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_pZ_Pan1_TempComp", "Ring2 (+Z, Panel1)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++    compare_three(tem_def, tem_fix2, tem_fix3, dir+"recHitFPIX/RecHit_YRes_Ring2_Side2_Panel2", "SimHit-RecHit Y-residuals (#mum)", "YResiduals_FPix_Ring2_pZ_Pan2_TempComp", "Ring2 (+Z, Panel2)",  "Default (pre2)","Big pixels fix + 290 um", "Big pixels + 290 um + new CPE", 634, 601, 418);
  //++  }

  if (save) gApplication->Terminate();
}
