//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Dec  3 10:33:55 2009 by ROOT version 5.22/00d
// from TTree pixelTree/pixelTree
// found on file: /afs/cern.ch/user/u/ursl/scratch0/pixel-test.root
//////////////////////////////////////////////////////////

#ifndef anaPixelTree_h
#define anaPixelTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class anaPixelTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Int_t           lumiblock;
   Int_t           event;
   Int_t           bx;
   Int_t           orbit;
   Float_t         bz;
   UInt_t          tlo;
   UInt_t          thi;
   UInt_t          fed1;
   UInt_t          fed2;
   UInt_t          l1t;
   UInt_t          l1ta0;
   UInt_t          l1ta1;
   UInt_t          l1ta2;
   UInt_t          l1ta3;
   UInt_t          hlt;
   UInt_t          hlta0;
   UInt_t          hlta1;
   UInt_t          hlta2;
   UInt_t          hlta3;
   UInt_t          hltr0;
   UInt_t          hltr1;
   UInt_t          hltr2;
   UInt_t          hltr3;
   UInt_t          hlte0;
   UInt_t          hlte1;
   UInt_t          hlte2;
   UInt_t          hlte3;
   Int_t           PvN;
   Float_t         PvX[100];   //[PvN]
   Float_t         PvY[100];   //[PvN]
   Float_t         PvZ[100];   //[PvN]
   Float_t         PvChi2[100];   //[PvN]
   Float_t         PvNdof[100];   //[PvN]
   Int_t           PvIsFake[100];   //[PvN]
   Int_t           MuN;
   Int_t           MuType[100];   //[MuN]
   Int_t           MuTkI[100];   //[MuN]
   Float_t         MuPt[100];   //[MuN]
   Float_t         MuTheta[100];   //[MuN]
   Float_t         MuPhi[100];   //[MuN]
   Float_t         MuT[100];   //[MuN]
   Float_t         MuTcorr[100];   //[MuN]
   Float_t         MuTerr[100];   //[MuN]
   Float_t         MuTmean;
   Int_t           TkN;
   Int_t           TkCharge[10000];   //[TkN]
   Float_t         TkChi2[10000];   //[TkN]
   Float_t         TkNdof[10000];   //[TkN]
   Float_t         TkPt[10000];   //[TkN]
   Float_t         TkTheta[10000];   //[TkN]
   Float_t         TkPhi[10000];   //[TkN]
   Float_t         TkD0[10000];   //[TkN]
   Float_t         TkDz[10000];   //[TkN]
   Float_t         TkVx[10000];   //[TkN]
   Float_t         TkVy[10000];   //[TkN]
   Float_t         TkVz[10000];   //[TkN]
   Float_t         TkAlpha[10000][20];   //[TkN]
   Float_t         TkBeta[10000][20];   //[TkN]
   Float_t         TkResX[10000][20];   //[TkN]
   Float_t         TkResY[10000][20];   //[TkN]
   Float_t         TkResXe[10000][20];   //[TkN]
   Float_t         TkResYe[10000][20];   //[TkN]
   Float_t         TkRes2X[10000][20];   //[TkN]
   Float_t         TkRes2Xe[10000][20];   //[TkN]
   Int_t           TkClN[10000];   //[TkN]
   Int_t           TkClI[10000][20];   //[TkN]
   Int_t           TkType[10000];   //[TkN]
   Int_t           TkMuI[10000];   //[TkN]
   Int_t           ClN;
   Float_t         ClRow[100000];   //[ClN]
   Float_t         ClCol[100000];   //[ClN]
   Float_t         ClLx[100000];   //[ClN]
   Float_t         ClLxE[100000];   //[ClN]
   Float_t         ClLy[100000];   //[ClN]
   Float_t         ClLyE[100000];   //[ClN]
   Float_t         ClGx[100000];   //[ClN]
   Float_t         ClGy[100000];   //[ClN]
   Float_t         ClGz[100000];   //[ClN]
   Int_t           ClSize[100000];   //[ClN]
   Int_t           ClSizeX[100000];   //[ClN]
   Int_t           ClSizeY[100000];   //[ClN]
   Int_t           ClFlipped[100000];   //[ClN]
   Int_t           ClLayer[100000];   //[ClN]
   Int_t           ClLadder[100000];   //[ClN]
   Int_t           ClModule[100000];   //[ClN]
   Int_t           ClDisk[100000];   //[ClN]
   Int_t           ClBlade[100000];   //[ClN]
   Int_t           ClPanel[100000];   //[ClN]
   Int_t           ClPlaquette[100000];   //[ClN]
   Int_t           ClDetId[100000];   //[ClN]
   Float_t         ClCharge[100000];   //[ClN]
   Float_t         ClChargeCorr[100000];   //[ClN]
   Int_t           ClType[100000];   //[ClN]
   Int_t           ClTkN[100000];   //[ClN]
   Int_t           ClTkI[100000][100];   //[ClN]
   Int_t           ClDgN[100000];   //[ClN]
   Int_t           ClDgI[100000][100];   //[ClN]
   Int_t           DgN;
   Int_t           DgRow[1000000];   //[DgN]
   Int_t           DgCol[1000000];   //[DgN]
   Int_t           DgDetid[1000000];   //[DgN]
   Int_t           DgRoc[1000000];   //[DgN]
   Int_t           DgRocR[1000000];   //[DgN]
   Int_t           DgRocC[1000000];   //[DgN]
   Float_t         DgLx[1000000];   //[DgN]
   Float_t         DgLy[1000000];   //[DgN]
   Float_t         DgGx[1000000];   //[DgN]
   Float_t         DgGy[1000000];   //[DgN]
   Float_t         DgGz[1000000];   //[DgN]
   Float_t         DgAdc[1000000];   //[DgN]
   Float_t         DgCharge[1000000];   //[DgN]
   Int_t           DgClI[1000000];   //[DgN]

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumiblock;   //!
   TBranch        *b_event;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_bz;   //!
   TBranch        *b_tlo;   //!
   TBranch        *b_thi;   //!
   TBranch        *b_fed1;   //!
   TBranch        *b_fed2;   //!
   TBranch        *b_l1t;   //!
   TBranch        *b_l1ta0;   //!
   TBranch        *b_l1ta1;   //!
   TBranch        *b_l1ta2;   //!
   TBranch        *b_l1ta3;   //!
   TBranch        *b_hlt;   //!
   TBranch        *b_hlta0;   //!
   TBranch        *b_hlta1;   //!
   TBranch        *b_hlta2;   //!
   TBranch        *b_hlta3;   //!
   TBranch        *b_hltr0;   //!
   TBranch        *b_hltr1;   //!
   TBranch        *b_hltr2;   //!
   TBranch        *b_hltr3;   //!
   TBranch        *b_hlte0;   //!
   TBranch        *b_hlte1;   //!
   TBranch        *b_hlte2;   //!
   TBranch        *b_hlte3;   //!
   TBranch        *b_PvN;   //!
   TBranch        *b_PvX;   //!
   TBranch        *b_PvY;   //!
   TBranch        *b_PvZ;   //!
   TBranch        *b_PvChi2;   //!
   TBranch        *b_PvNdof;   //!
   TBranch        *b_PvIsFake;   //!
   TBranch        *b_MuN;   //!
   TBranch        *b_MuType;   //!
   TBranch        *b_MuTkI;   //!
   TBranch        *b_MuPt;   //!
   TBranch        *b_MuTheta;   //!
   TBranch        *b_MuPhi;   //!
   TBranch        *b_MuT;   //!
   TBranch        *b_MuTcorr;   //!
   TBranch        *b_MuTerr;   //!
   TBranch        *b_MuTmean;   //!
   TBranch        *b_TkN;   //!
   TBranch        *b_TkCharge;   //!
   TBranch        *b_TkChi2;   //!
   TBranch        *b_TkNdof;   //!
   TBranch        *b_TkPt;   //!
   TBranch        *b_TkTheta;   //!
   TBranch        *b_TkPhi;   //!
   TBranch        *b_TkD0;   //!
   TBranch        *b_TkDz;   //!
   TBranch        *b_TkVx;   //!
   TBranch        *b_TkVy;   //!
   TBranch        *b_TkVz;   //!
   TBranch        *b_TkAlpha;   //!
   TBranch        *b_TkBeta;   //!
   TBranch        *b_TkResX;   //!
   TBranch        *b_TkResY;   //!
   TBranch        *b_TkResXe;   //!
   TBranch        *b_TkResYe;   //!
   TBranch        *b_TkRes2X;   //!
   TBranch        *b_TkRes2Xe;   //!
   TBranch        *b_TkClN;   //!
   TBranch        *b_TkClI;   //!
   TBranch        *b_TkType;   //!
   TBranch        *b_TkMuI;   //!
   TBranch        *b_ClN;   //!
   TBranch        *b_ClRow;   //!
   TBranch        *b_ClCol;   //!
   TBranch        *b_ClLx;   //!
   TBranch        *b_ClLxE;   //!
   TBranch        *b_ClLy;   //!
   TBranch        *b_ClLyE;   //!
   TBranch        *b_ClGx;   //!
   TBranch        *b_ClGy;   //!
   TBranch        *b_ClGz;   //!
   TBranch        *b_ClSize;   //!
   TBranch        *b_ClSizeX;   //!
   TBranch        *b_ClSizeY;   //!
   TBranch        *b_ClFlipped;   //!
   TBranch        *b_ClLayer;   //!
   TBranch        *b_ClLadder;   //!
   TBranch        *b_ClModule;   //!
   TBranch        *b_ClDisk;   //!
   TBranch        *b_ClBlade;   //!
   TBranch        *b_ClPanel;   //!
   TBranch        *b_ClPlaquette;   //!
   TBranch        *b_ClDetId;   //!
   TBranch        *b_ClCharge;   //!
   TBranch        *b_ClChargeCorr;   //!
   TBranch        *b_ClType;   //!
   TBranch        *b_ClTkN;   //!
   TBranch        *b_ClTkI;   //!
   TBranch        *b_ClDgN;   //!
   TBranch        *b_ClDgI;   //!
   TBranch        *b_DgN;   //!
   TBranch        *b_DgRow;   //!
   TBranch        *b_DgCol;   //!
   TBranch        *b_DgDetid;   //!
   TBranch        *b_DgRoc;   //!
   TBranch        *b_DgRocR;   //!
   TBranch        *b_DgRocC;   //!
   TBranch        *b_DgLx;   //!
   TBranch        *b_DgLy;   //!
   TBranch        *b_DgGx;   //!
   TBranch        *b_DgGy;   //!
   TBranch        *b_DgGz;   //!
   TBranch        *b_DgAdc;   //!
   TBranch        *b_DgCharge;   //!
   TBranch        *b_DgClI;   //!

   anaPixelTree(const char *dir, int ischain);
   virtual ~anaPixelTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int nevt = -1);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef anaPixelTree_cxx
anaPixelTree::anaPixelTree(const char *dir, int ischain)
{
  TChain *chain = new TChain("pixelTree","");
  if (1 == ischain) {
    chain->Add(Form("%s/*", dir));
  } else {
    chain->Add(dir); 
  }
  Init(chain);

}

anaPixelTree::~anaPixelTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t anaPixelTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t anaPixelTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void anaPixelTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumiblock", &lumiblock, &b_lumiblock);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("bx", &bx, &b_bx);
   fChain->SetBranchAddress("orbit", &orbit, &b_orbit);
   fChain->SetBranchAddress("bz", &bz, &b_bz);
   fChain->SetBranchAddress("tlo", &tlo, &b_tlo);
   fChain->SetBranchAddress("thi", &thi, &b_thi);
   fChain->SetBranchAddress("fed1", &fed1, &b_fed1);
   fChain->SetBranchAddress("fed2", &fed2, &b_fed2);
   fChain->SetBranchAddress("l1t", &l1t, &b_l1t);
   fChain->SetBranchAddress("l1ta0", &l1ta0, &b_l1ta0);
   fChain->SetBranchAddress("l1ta1", &l1ta1, &b_l1ta1);
   fChain->SetBranchAddress("l1ta2", &l1ta2, &b_l1ta2);
   fChain->SetBranchAddress("l1ta3", &l1ta3, &b_l1ta3);
   fChain->SetBranchAddress("hlt", &hlt, &b_hlt);
   fChain->SetBranchAddress("hlta0", &hlta0, &b_hlta0);
   fChain->SetBranchAddress("hlta1", &hlta1, &b_hlta1);
   fChain->SetBranchAddress("hlta2", &hlta2, &b_hlta2);
   fChain->SetBranchAddress("hlta3", &hlta3, &b_hlta3);
   fChain->SetBranchAddress("hltr0", &hltr0, &b_hltr0);
   fChain->SetBranchAddress("hltr1", &hltr1, &b_hltr1);
   fChain->SetBranchAddress("hltr2", &hltr2, &b_hltr2);
   fChain->SetBranchAddress("hltr3", &hltr3, &b_hltr3);
   fChain->SetBranchAddress("hlte0", &hlte0, &b_hlte0);
   fChain->SetBranchAddress("hlte1", &hlte1, &b_hlte1);
   fChain->SetBranchAddress("hlte2", &hlte2, &b_hlte2);
   fChain->SetBranchAddress("hlte3", &hlte3, &b_hlte3);
   fChain->SetBranchAddress("PvN", &PvN, &b_PvN);
   fChain->SetBranchAddress("PvX", PvX, &b_PvX);
   fChain->SetBranchAddress("PvY", PvY, &b_PvY);
   fChain->SetBranchAddress("PvZ", PvZ, &b_PvZ);
   fChain->SetBranchAddress("PvChi2", PvChi2, &b_PvChi2);
   fChain->SetBranchAddress("PvNdof", PvNdof, &b_PvNdof);
   fChain->SetBranchAddress("PvIsFake", PvIsFake, &b_PvIsFake);
   fChain->SetBranchAddress("MuN", &MuN, &b_MuN);
   fChain->SetBranchAddress("MuType", &MuType, &b_MuType);
   fChain->SetBranchAddress("MuTkI", &MuTkI, &b_MuTkI);
   fChain->SetBranchAddress("MuPt", &MuPt, &b_MuPt);
   fChain->SetBranchAddress("MuTheta", &MuTheta, &b_MuTheta);
   fChain->SetBranchAddress("MuPhi", &MuPhi, &b_MuPhi);
   fChain->SetBranchAddress("MuT", &MuT, &b_MuT);
   fChain->SetBranchAddress("MuTcorr", &MuTcorr, &b_MuTcorr);
   fChain->SetBranchAddress("MuTerr", &MuTerr, &b_MuTerr);
   fChain->SetBranchAddress("MuTmean", &MuTmean, &b_MuTmean);
   fChain->SetBranchAddress("TkN", &TkN, &b_TkN);
   fChain->SetBranchAddress("TkCharge", TkCharge, &b_TkCharge);
   fChain->SetBranchAddress("TkChi2", TkChi2, &b_TkChi2);
   fChain->SetBranchAddress("TkNdof", TkNdof, &b_TkNdof);
   fChain->SetBranchAddress("TkPt", TkPt, &b_TkPt);
   fChain->SetBranchAddress("TkTheta", TkTheta, &b_TkTheta);
   fChain->SetBranchAddress("TkPhi", TkPhi, &b_TkPhi);
   fChain->SetBranchAddress("TkD0", TkD0, &b_TkD0);
   fChain->SetBranchAddress("TkDz", TkDz, &b_TkDz);
   fChain->SetBranchAddress("TkVx", TkVx, &b_TkVx);
   fChain->SetBranchAddress("TkVy", TkVy, &b_TkVy);
   fChain->SetBranchAddress("TkVz", TkVz, &b_TkVz);
   fChain->SetBranchAddress("TkAlpha", TkAlpha, &b_TkAlpha);
   fChain->SetBranchAddress("TkBeta", TkBeta, &b_TkBeta);
   fChain->SetBranchAddress("TkResX", TkResX, &b_TkResX);
   fChain->SetBranchAddress("TkResY", TkResY, &b_TkResY);
   fChain->SetBranchAddress("TkResXe", TkResXe, &b_TkResXe);
   fChain->SetBranchAddress("TkResYe", TkResYe, &b_TkResYe);
   fChain->SetBranchAddress("TkRes2X", TkRes2X, &b_TkRes2X);
   fChain->SetBranchAddress("TkRes2Xe", TkRes2Xe, &b_TkRes2Xe);
   fChain->SetBranchAddress("TkClN", TkClN, &b_TkClN);
   fChain->SetBranchAddress("TkClI", TkClI, &b_TkClI);
   fChain->SetBranchAddress("TkType", TkType, &b_TkType);
   fChain->SetBranchAddress("TkMuI", TkMuI, &b_TkMuI);
   fChain->SetBranchAddress("ClN", &ClN, &b_ClN);
   fChain->SetBranchAddress("ClRow", ClRow, &b_ClRow);
   fChain->SetBranchAddress("ClCol", ClCol, &b_ClCol);
   fChain->SetBranchAddress("ClLx", ClLx, &b_ClLx);
   fChain->SetBranchAddress("ClLxE", ClLxE, &b_ClLxE);
   fChain->SetBranchAddress("ClLy", ClLy, &b_ClLy);
   fChain->SetBranchAddress("ClLyE", ClLyE, &b_ClLyE);
   fChain->SetBranchAddress("ClGx", ClGx, &b_ClGx);
   fChain->SetBranchAddress("ClGy", ClGy, &b_ClGy);
   fChain->SetBranchAddress("ClGz", ClGz, &b_ClGz);
   fChain->SetBranchAddress("ClSize", ClSize, &b_ClSize);
   fChain->SetBranchAddress("ClSizeX", ClSizeX, &b_ClSizeX);
   fChain->SetBranchAddress("ClSizeY", ClSizeY, &b_ClSizeY);
   fChain->SetBranchAddress("ClFlipped", ClFlipped, &b_ClFlipped);
   fChain->SetBranchAddress("ClLayer", ClLayer, &b_ClLayer);
   fChain->SetBranchAddress("ClLadder", ClLadder, &b_ClLadder);
   fChain->SetBranchAddress("ClModule", ClModule, &b_ClModule);
   fChain->SetBranchAddress("ClDisk", ClDisk, &b_ClDisk);
   fChain->SetBranchAddress("ClBlade", ClBlade, &b_ClBlade);
   fChain->SetBranchAddress("ClPanel", ClPanel, &b_ClPanel);
   fChain->SetBranchAddress("ClPlaquette", ClPlaquette, &b_ClPlaquette);
   fChain->SetBranchAddress("ClDetId", ClDetId, &b_ClDetId);
   fChain->SetBranchAddress("ClCharge", ClCharge, &b_ClCharge);
   fChain->SetBranchAddress("ClChargeCorr", ClChargeCorr, &b_ClChargeCorr);
   fChain->SetBranchAddress("ClType", ClType, &b_ClType);
   fChain->SetBranchAddress("ClTkN", ClTkN, &b_ClTkN);
   fChain->SetBranchAddress("ClTkI", ClTkI, &b_ClTkI);
   fChain->SetBranchAddress("ClDgN", ClDgN, &b_ClDgN);
   fChain->SetBranchAddress("ClDgI", ClDgI, &b_ClDgI);
   fChain->SetBranchAddress("DgN", &DgN, &b_DgN);
   fChain->SetBranchAddress("DgRow", DgRow, &b_DgRow);
   fChain->SetBranchAddress("DgCol", DgCol, &b_DgCol);
   fChain->SetBranchAddress("DgDetid", DgDetid, &b_DgDetid);
   fChain->SetBranchAddress("DgRoc", DgRoc, &b_DgRoc);
   fChain->SetBranchAddress("DgRocR", DgRocR, &b_DgRocR);
   fChain->SetBranchAddress("DgRocC", DgRocC, &b_DgRocC);
   fChain->SetBranchAddress("DgLx", DgLx, &b_DgLx);
   fChain->SetBranchAddress("DgLy", DgLy, &b_DgLy);
   fChain->SetBranchAddress("DgGx", DgGx, &b_DgGx);
   fChain->SetBranchAddress("DgGy", DgGy, &b_DgGy);
   fChain->SetBranchAddress("DgGz", DgGz, &b_DgGz);
   fChain->SetBranchAddress("DgAdc", DgAdc, &b_DgAdc);
   fChain->SetBranchAddress("DgCharge", DgCharge, &b_DgCharge);
   fChain->SetBranchAddress("DgClI", DgClI, &b_DgClI);
   Notify();
}

Bool_t anaPixelTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void anaPixelTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t anaPixelTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef anaPixelTree_cxx
