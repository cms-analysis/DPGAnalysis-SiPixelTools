//----------------------------------------
// From File
/*TH2F* Gain2d             = new TH2F();
TH2F* ErrorGain2d          = new TH2F();
TH2F* Pedestal2d           = new TH2F();
TH2F* ErrorPedestal2d      = new TH2F();
TH2F* GainSaturate2d       = new TH2F();
TH2F* GainDynamicRange2d   = new TH2F();
TH2F* GainFitResult2d      = new TH2F();
TH2F* GainChi2NDF2d        = new TH2F();
TH2F* GainChi2Prob2d       = new TH2F();
TH2F* GainHighPoint2d      = new TH2F();
TH2F* GainLowPoint2d       = new TH2F();
*/
TH1F* GainNPoints1d        = new TH1F();

// From Second File
/*TH2F* Gain2d_2           = new TH2F();
TH2F* ErrorGain2d_2        = new TH2F();
TH2F* Pedestal2d_2         = new TH2F();
TH2F* ErrorPedestal2d_2    = new TH2F();
TH2F* GainSaturate2d_2     = new TH2F();
TH2F* GainDynamicRange2d_2 = new TH2F();
TH2F* GainFitResult2d_2    = new TH2F();
TH2F* GainChi2NDF2d_2      = new TH2F();
TH2F* GainChi2Prob2d_2     = new TH2F();
TH2F* GainHighPoint2d_2    = new TH2F();
TH2F* GainLowPoint2d_2     = new TH2F();
*/
TH1F* GainNPoints1d_2      = new TH1F();

//----------------------------------------
// Summaries
double nmod = 1416;
TH1F* SUM_Gain2d             = new TH1F("SUM_Gain2d","SUM_Gain2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_ErrorGain2d        = new TH1F("SUM_ErrorGain2d","SUM_ErrorGain2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_Pedestal2d         = new TH1F("SUM_Pedestal2d","SUM_Pedestal2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_ErrorPedestal2d    = new TH1F("SUM_ErrorPedestal2d","SUM_ErrorPedestal2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainSaturate2d     = new TH1F("SUM_GainSaturate2d","SUM_GainSaturate2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainDynamicRange2d = new TH1F("SUM_GainDynamicRange2d","SUM_GainDynamicRange2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainFitResult2d    = new TH1F("SUM_GainFitResult2d","SUM_GainFitResult2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainChi2NDF2d      = new TH1F("SUM_GainChi2NDF2d","SUM_GainChi2NDF2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainChi2Prob2d     = new TH1F("SUM_GainChi2Prob2d","SUM_GainChi2Prob2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainHighPoint2d    = new TH1F("SUM_GainHighPoint2d","SUM_GainHighPoint2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainLowPoint2d     = new TH1F("SUM_GainLowPoint2d","SUM_GainLowPoint2d",(int)nmod,0.5,nmod+0.5);
TH1F* SUM_GainNPoints1d      = new TH1F("SUM_GainNPoints1d","SUM_GainNPoints1d",(int)nmod,0.5,nmod+0.5);

//----------------------------------------
// Binning
int nbin_gain    = 400; double xmin_gain    = 0,   xmax_gain    = 8*50; // Run 2: 8; Run 3: 8*50
int nbin_errgain = 400; double xmin_errgain = 0,   xmax_errgain = 0.01*50;
int nbin_ped     = 350; double xmin_ped     = -50, xmax_ped     = 250;
int nbin_errped  = 350; double xmin_errped  = -1,  xmax_errped  = 1;
if(diff){
  nbin_gain    = 400; xmin_gain    = -6*50; xmax_gain    = 6*50; // Run 2: -6, 6; Run 3: -6*50, 6*50
  nbin_errgain = 400; xmin_errgain = -0.1;  xmax_errgain = 0.1;
  nbin_ped     = 350; xmin_ped     = -50;   xmax_ped     = 50;
  nbin_errped  = 350; xmin_errped  = -10;   xmax_errped  = 10;
}

//----------------------------------------
// Maps
map_module_level MAP_Gain("Gain", 50, xmin_gain, xmax_gain);
map_module_level MAP_ErrorGain("ErrorGain", 50, xmin_errgain, xmax_errgain);
map_module_level MAP_Pedestal("Pedestal", 50, xmin_ped, xmax_ped);
map_module_level MAP_ErrorPedestal("ErrorPedestal", 50, xmin_errped, xmax_errped);
map_module_level MAP_GainSaturate("GainSaturate", 50, -10, 10);
map_module_level MAP_GainDynamicRange("GainDynamicRange", 50, -25, 25);
map_module_level MAP_GainFitResult("GainFitResult", 50, -1, 1);
map_module_level MAP_GainChi2NDF("GainChi2NDF", 50, -10, 10);
map_module_level MAP_GainChi2Prob("GainChi2Prob", 50, -0.1, 0.1);
map_module_level MAP_GainHighPoint("GainHighPoint", 50, -100, 100);
map_module_level MAP_GainLowPoint("GainLowPoint", 50, -10, 10);
map_module_level MAP_GainNPoints("GainNPoints", 50, -2, 2);
  
map_module_level MAP_frac_gain_high("frac_gain_high", 100, 0, 1);
map_module_level MAP_frac_bad_fit("frac_bad_fit", 100, 0, 1);

//----------------------------------------
//Pixel Level
TH1F* TOT_Gain = new TH1F("TOT_Gain","TOT_Gain_PixelLevel;Gain;# pixels",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_ErrorGain = new TH1F("TOT_ErrorGain","TOT_ErrorGain_PixelLevel;Error on 1/Gain;#pixels",nbin_errgain,xmin_errgain,xmax_errgain);
TH1F* TOT_Pedestal = new TH1F("TOT_Pedestal","TOT_Pedestal_PixelLevel;Pedestal;# pixels",nbin_ped,xmin_ped,xmax_ped);
TH1F* TOT_ErrorPedestal = new TH1F("TOT_ErrorPedestal","TOT_ErrorPedestal_PixelLevel;Error on Pedestal;# pixels",nbin_errped,xmin_errped,xmax_errped);

TH1F* TOT_GainBPix = new TH1F("TOT_GainBPix","TOT_GainBPix_PixelLevel;Gain;# pixels",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_ErrorGainBPix = new TH1F("TOT_ErrorGainBPix","TOT_ErrorGainBPix_PixelLevel;Error on 1/Gain;# pixels",nbin_errgain,xmin_errgain,xmax_errgain);
TH1F* TOT_PedestalBPix = new TH1F("TOT_PedestalBPix","TOT_PedestalBPix_PixelLevel;Pedestal;# pixels",nbin_ped,xmin_ped,xmax_ped);
TH1F* TOT_ErrorPedestalBPix = new TH1F("TOT_ErrorPedestalBPix","TOT_ErrorPedestalBPix_PixelLevel;Error on Pedestal;# pixels",nbin_errped,xmin_errped,xmax_errped);
TH1F* TOT_GainFPix = new TH1F("TOT_GainFPix","TOT_GainFPix_PixelLevel;Gain;# pixels",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_ErrorGainFPix = new TH1F("TOT_ErrorGainFPix","TOT_ErrorGainFPix_PixelLevel;Error on 1/Gain;# pixels",nbin_errgain,xmin_errgain,xmax_errgain);
TH1F* TOT_PedestalFPix = new TH1F("TOT_PedestalFPix","TOT_PedestalFPix_PixelLevel;Pedestal;# pixels",nbin_ped,xmin_ped,xmax_ped);
TH1F* TOT_ErrorPedestalFPix = new TH1F("TOT_ErrorPedestalFPix","TOT_ErrorPedestalFPix_PixelLevel;Error on Pedestal;# pixels",nbin_errped,xmin_errped,xmax_errped);

//----------------------------------------
//Column Level
TH1F* TOT_GainPerCol = new TH1F("TOT_GainPerCol","TOT_Gain_ColumnLevel;Gain;# columns",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_PedestalPerCol = new TH1F("TOT_PedestalPerCol","TOT_Pedestal_ColumnLevel;Gain;# columns",nbin_ped,xmin_ped,xmax_ped);

TH1F* TOT_GainPerColBPix = new TH1F("TOT_GainPerColBPix","TOT_GainBPix_ColumnLevel;Gain;# columns",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_ErrorGainPerColBPix = new TH1F("TOT_ErrorGainPerColBPix","TOT_ErrorGainBPix_ColumnLevel;Error on 1/Gain;# columns",nbin_errgain,xmin_errgain,xmax_errgain);
TH1F* TOT_PedestalPerColBPix = new TH1F("TOT_PedestalPerColBPix","TOT_PedestalBPix_ColumnLevel;Pedestal;# columns",nbin_ped,xmin_ped,xmax_ped);
TH1F* TOT_ErrorPedestalPerColBPix = new TH1F("TOT_ErrorPedestalPerColBPix","TOT_ErrorPedestalBPix_ColumnLevel;Error on Pedestal;# columns",nbin_errped,xmin_errped,xmax_errped);
TH1F* TOT_GainPerColFPix = new TH1F("TOT_GainPerColFPix","TOT_GainFPix_ColumnLevel;Gain;# columns",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_ErrorGainPerColFPix = new TH1F("TOT_ErrorGainPerColFPix","TOT_ErrorGainFPix_ColumnLevel;Error on 1/Gain;# columns",nbin_errgain,xmin_errgain,xmax_errgain);
TH1F* TOT_PedestalPerColFPix = new TH1F("TOT_PedestalPerColFPix","TOT_PedestalFPix_ColumnLevel;Pedestal;# columns",nbin_ped,xmin_ped,xmax_ped);
TH1F* TOT_ErrorPedestalPerColFPix = new TH1F("TOT_ErrorPedestalPerColFPix","TOT_ErrorPedestalFPix_ColumnLevel;Error on Pedestal;# columns",nbin_errped,xmin_errped,xmax_errped);

//----------------------------------------
//ROC Level
TH1F* TOT_GainPerROC = new TH1F("TOT_GainPerROC","TOT_Gain_ROCLevel;Gain;# ROC",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_PedestalPerROC = new TH1F("TOT_PedestalPerROC","TOT_Pedestal_ROCLevel;Gain;# ROC",nbin_ped,xmin_ped,xmax_ped);

TH1F* TOT_GainPerROCBPix = new TH1F("TOT_GainPerROCBPix","TOT_GainBPix_ROCLevel;Gain;# ROC",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_ErrorGainPerROCBPix = new TH1F("TOT_ErrorGainPerROCBPix","TOT_ErrorGainBPix_ROCLevel;Error on 1/Gain;# ROC",nbin_errgain,xmin_errgain,xmax_errgain);
TH1F* TOT_PedestalPerROCBPix = new TH1F("TOT_PedestalPerROCBPix","TOT_PedestalBPix_ROCLevel;Pedestal;# ROC",nbin_ped,xmin_ped,xmax_ped);
TH1F* TOT_ErrorPedestalPerROCBPix = new TH1F("TOT_ErrorPedestalPerROCBPix","TOT_ErrorPedestalBPix_ROCLevel;Error on Pedestal;# ROC",nbin_errped,xmin_errped,xmax_errped);
TH1F* TOT_GainPerROCFPix = new TH1F("TOT_GainPerROCFPix","TOT_GainFPix_ROCLevel;Gain;# ROC",nbin_gain,xmin_gain,xmax_gain);
TH1F* TOT_ErrorGainPerROCFPix = new TH1F("TOT_ErrorGainPerROCFPix","TOT_ErrorGainFPix_ROCLevel;Error on 1/Gain;# ROC",nbin_errgain,xmin_errgain,xmax_errgain);
TH1F* TOT_PedestalPerROCFPix = new TH1F("TOT_PedestalPerROCFPix","TOT_PedestalFPix_ROCLevel;Pedestal;# ROC",nbin_ped,xmin_ped,xmax_ped);
TH1F* TOT_ErrorPedestalPerROCFPix = new TH1F("TOT_ErrorPedestalPerROCFPix","TOT_ErrorPedestalFPix_ROCLevel;Error on Pedestal;# ROC",nbin_errped,xmin_errped,xmax_errped);

TH2F* CorrelationGainPed = new TH2F("CorrelationGainPed","CorrelationGainPed;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationGainPedBPix = new TH2F("CorrelationGainPedBPix","CorrelationGainPedBPix;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationGainPedFPix = new TH2F("CorrelationGainPedFPix","CorrelationGainPedFPix;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);


// ******** ERRORS **********

//Pixel Level 
TH2F* CorrelationError = new TH2F("CorrelationError","CorrelationError;Error on 1/gain;Error on pedestal", nbin_errgain, xmin_errgain, xmax_errgain, nbin_errped, xmin_errped, xmax_errped);
TH2F* ErrorVsGain = new TH2F("ErrorVsGain","ErrorVsGain;Gain;Error on 1/gain", nbin_gain, xmin_gain, xmax_gain, nbin_errgain, xmin_errgain, xmax_errgain);
TH2F* ErrorVsPedestal = new TH2F("ErrorVsPedestal","ErrorVsPedestal;Pedestal;Error on pedestal", nbin_ped, xmin_ped, xmax_ped, nbin_errped, xmin_errped, xmax_errped);
TH2F* CorrelationErrorBPix = new TH2F("CorrelationErrorBPix","CorrelationErrorBPix;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainBPix = new TH2F("ErrorVsGainBPix","ErrorVsGainBPix;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalBPix = new TH2F("ErrorVsPedestalBPix","ErrorVsPedestalBPix;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);
TH2F* CorrelationErrorFPix = new TH2F("CorrelationErrorFPix","CorrelationErrorFPix;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainFPix = new TH2F("ErrorVsGainFPix","ErrorVsGainFPix;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalFPix = new TH2F("ErrorVsPedestalFPix","ErrorVsPedestalFPix;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);

//Column Level
TH2F* CorrelationGainPedPerCol = new TH2F("CorrelationGainPedPerCol","CorrelationGainPedPerCol;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationErrorPerCol = new TH2F("CorrelationErrorPerCol","CorrelationErrorPerColPerCol;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainPerCol = new TH2F("ErrorVsGainPerCol","ErrorVsGainPerCol;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalPerCol = new TH2F("ErrorVsPedestalPerCol","ErrorVsPedestalPerCol;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);
TH2F* CorrelationGainPedPerColBPix = new TH2F("CorrelationGainPedPerColBPix","CorrelationGainPedPerColBPix;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationErrorPerColBPix = new TH2F("CorrelationErrorPerColBPix","CorrelationErrorPerColBPix;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainPerColBPix = new TH2F("ErrorVsGainPerColBPix","ErrorVsGainPerColBPix;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalPerColBPix = new TH2F("ErrorVsPedestalPerColBPix","ErrorVsPedestalPerColBPix;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);
TH2F* CorrelationGainPedPerColFPix = new TH2F("CorrelationGainPedPerColFPix","CorrelationGainPedPerColFPix;gain;pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationErrorPerColFPix = new TH2F("CorrelationErrorPerColFPix","CorrelationErrorPerColFPix;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainPerColFPix = new TH2F("ErrorVsGainPerColFPix","ErrorVsGainPerColFPix;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalPerColFPix = new TH2F("ErrorVsPedestalPerColFPix","ErrorVsPedestalPerColFPix;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);

//ROC Level
TH2F* CorrelationGainPedPerROC = new TH2F("CorrelationGainPedPerROC","CorrelationGainPedPerROC;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationErrorPerROC = new TH2F("CorrelationErrorPerROC","CorrelationErrorPerROC;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainPerROC = new TH2F("ErrorVsGainPerROC","ErrorVsGainPerROC;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalPerROC = new TH2F("ErrorVsPedestalPerROC","ErrorVsPedestalPerROC;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);
TH2F* CorrelationGainPedPerROCBPix = new TH2F("CorrelationGainPedPerROCBPix","CorrelationGainPedPerROCBPix;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationErrorPerROCBPix = new TH2F("CorrelationErrorPerROCBPix","CorrelationErrorPerROCBPix;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainPerROCBPix = new TH2F("ErrorVsGainPerROCBPix","ErrorVsGainPerROCBPix;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalPerROCBPix = new TH2F("ErrorVsPedestalPerROCBPix","ErrorVsPedestalPerROCBPix;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);
TH2F* CorrelationGainPedPerROCFPix = new TH2F("CorrelationGainPedPerROCFPix","CorrelationGainPedPerROCFPix;Gain;Pedestal",nbin_gain,xmin_gain,xmax_gain,nbin_ped,xmin_ped,xmax_ped);
TH2F* CorrelationErrorPerROCFPix = new TH2F("CorrelationErrorPerROCFPix","CorrelationErrorPerROCFPix;Error on 1/gain;Error on pedestal",100,0,1,200,0,100);
TH2F* ErrorVsGainPerROCFPix = new TH2F("ErrorVsGainPerROCFPix","ErrorVsGainPerROCFPix;Gain;Error on 1/gain",nbin_gain,xmin_gain,xmax_gain,100,0,1);
TH2F* ErrorVsPedestalPerROCFPix = new TH2F("ErrorVsPedestalPerROCFPix","ErrorVsPedestalPerROCFPix;Pedestal;Error on pedestal",nbin_ped,xmin_ped,xmax_ped,200,0,100);

//Counting overflows for mean/RMS
TOT_GainBPix->StatOverflows(kTRUE);
TOT_ErrorGainBPix->StatOverflows(kTRUE);
TOT_PedestalBPix->StatOverflows(kTRUE);
TOT_ErrorPedestalBPix->StatOverflows(kTRUE);
TOT_GainFPix->StatOverflows(kTRUE);
TOT_ErrorGainFPix->StatOverflows(kTRUE);
TOT_PedestalFPix->StatOverflows(kTRUE);
TOT_ErrorPedestalFPix->StatOverflows(kTRUE);
