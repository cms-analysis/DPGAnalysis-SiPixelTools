// ----------------------------------------------------------------------
// -- Validation script for the trigger information 
// -- contained in the PixelTree
// -- 
// -- Compare this output to the printout of     
// -- *process.hltrep*process.l1trep
// --
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
void l1(int runNumber = 138921) {

  string l1Names[256];
  int    l1Counts[256];

  TH1D *hl1 = (TH1D*)gFile->Get(Form("L1T_%d", runNumber)); 

  if (0 == hl1) {
    cout << "Histogram " << Form("L1T_%d", runNumber) << " not found, returning" << endl;
    return; 
  }

  for (int i = 0; i < hl1->GetNbinsX(); ++i) {
    l1Names[i] = string(hl1->GetXaxis()->GetBinLabel(i+1));
    if (l1Names[i] > "") {
      cout << Form("%3d ", i) << l1Names[i] << endl;
    }
  }
    

  vector<int> tCnts;
  for (int i = 0; i < 128; ++i) {
    tCnts.push_back(0); 
  }
  //  cout << tCnts[1] << endl;
  // -- start tree reading
  TTree *t = (TTree*)gFile->Get("pixelTree"); 

  bool l1A[128];
  t->SetBranchAddress("l1A", &l1A);

  Int_t nentries = Int_t(t->GetEntries());

  Int_t nb(0), nbytes(0), nevents(0);
  //  for (Int_t jentry = 0; jentry < 200; jentry++) {
  for (Int_t jentry = 0; jentry < nentries; jentry++) {
    nb = t->GetEntry(jentry);   nbytes += nb;
    ++nevents;
    for (int i = 0; i < 128; ++i) {
      if (l1A[i]) {
	tCnts[i] = tCnts[i] + 1; 
	//	cout << "Found trigger bit " << l1Names[i] << endl;
      }
	
    }

  }


  // -- Print results
  for (int i = 0; i < 128; ++i) {
    if (l1Names[i] > "") {
      cout << Form("%40s %5d %4.3f", l1Names[i].c_str(), tCnts[i], float(tCnts[i])/float(nevents)) << endl;
    }
  }
  cout << "Nevents: " << nevents << endl;
}



// ----------------------------------------------------------------------
void hlt(int runNumber = 138921) {

  string hltNames[256];
  int    hltCounts[256];

  TH1D *hHLT = (TH1D*)gFile->Get(Form("HLT_%d", runNumber)); 

  if (0 == hHLT) {
    cout << "Histogram " << Form("HLT_%d", runNumber) << " not found, returning" << endl;
    return; 
  }

  for (int i = 0; i < hHLT->GetNbinsX(); ++i) {
    hltNames[i] = string(hHLT->GetXaxis()->GetBinLabel(i+1));
    if (hltNames[i] > "") {
      cout << Form("%3d ", i) << hltNames[i] << endl;
    }
  }
    
  vector<int> tCnts;
  for (int i = 0; i < 256; ++i) {
    tCnts.push_back(0); 
  }
  //  cout << tCnts[1] << endl;
  // -- start tree reading
  TTree *t = (TTree*)gFile->Get("pixelTree"); 

  bool l1A[256];
  t->SetBranchAddress("hlA", &l1A);

  Int_t nentries = Int_t(t->GetEntries());

  Int_t nb(0), nbytes(0), nevents(0);
  //  for (Int_t jentry = 0; jentry < 200; jentry++) {
  for (Int_t jentry = 0; jentry < nentries; jentry++) {
    nb = t->GetEntry(jentry);   nbytes += nb;
    ++nevents;
    for (int i = 0; i < 128; ++i) {
      if (l1A[i]) {
	tCnts[i] = tCnts[i] + 1; 
	//	cout << "Found trigger bit " << l1Names[i] << endl;
      }
	
    }

  }


  // -- Print results
  for (int i = 0; i < 128; ++i) {
    if (hltNames[i] > "") {
      cout << Form("%40s %5d %4.3f", hltNames[i].c_str(), tCnts[i], float(tCnts[i])/float(nevents)) << endl;
    }
  }
  cout << "Nevents: " << nevents << endl;
}
