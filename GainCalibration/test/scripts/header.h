#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>


#include "TKey.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TLegend.h"
#include "TList.h"
#include "TStyle.h"
#include "TObject.h"
#include "TMean.cc"

#include "PixelNameTranslator.cc"

using namespace std;

TString RunNumber = "_Run";
bool PrintImages = true;
bool PrintAllFormats = false;
bool printSummary = true;
bool printTexSummary = true;

double gain_max = 20;
double ped_max = 250;

// ******** Declaration of fonctions ********
vector<TString> makedirlist(TFile*,TString);
void write(TH1F* , Bool_t = 0 , Bool_t = 0 , TString = "rmou"); // logY , largePad , optstatoption
void write(TH2F* , Bool_t = 0);                                 // logZ
void writeAllLevels(TH1F*,TH1F*,TH1F*,TString);
int rocId(int, int);
void getBarrelPos(TString& , int& , int& , int& );
int  getDetId(TString);

#include "functions.C"// => easier to read

//Making PixNameTranslator object available from everywhere.
PixelNameTranslator pix_trans; //used in containers.h too
  
//class map_module_level & pix_val
#include "containers.h"   // => easier to read


