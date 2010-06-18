#define anaPixelTree_cxx
#include "anaPixelTree.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void anaPixelTree::Loop(int nevt) {
//   In a ROOT session, you can do:
//      Root > .L anaPixelTree.cc++
//      Root > anaPixelTree t("/afs/cern.ch/user/u/ursl/scratch0/pixel-test.root", 0)
//      Root > t.Loop(20);      // Loop on two entries
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();

   int step(100000), maxEvents(nentries); 
   if (nevt > 0 && nevt < nentries) maxEvents = nevt; 
   if (maxEvents < 1000000) step = 50000; 
   if (maxEvents < 100000)  step = 5000; 
   if (maxEvents < 10000)   step = 500; 
   if (maxEvents < 1000)    step = 100; 

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if ((nevt > -1) && (jentry > nevt)) break;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      cout << "new event: " << jentry << "/  " << event 
	   << " ClN: " << ClN << " TkN: " << TkN 
	   << endl;

      for (int ic = 0; ic < ClN; ++ic) {
	cout << Form("%04d", ic) << " detId: " << ClDetId[ic] 
	     << " r/c: " << ClRow[ic] << "/" << ClCol[ic] 
	     << " typ: " << ClType[ic]
	     << " tkI: "; 

	for (int i = 0; i < ClTkN[ic]; ++i) {
	  cout << ClTkI[ic][i] << "  "; 
	}
	cout << endl;
      }
   }
}
