// simple analysis macro. This is for the PixNtuple tree, same will work for the TrackNtuple tree.
// added to package on Feb 22, 2009 by fblekman

void rootmacro_example(void){
	
	TChain *ch1 = new TChain("PixNtuple");
	// add your files here. Root takes wildcards (but not when reading from castor!)
	ch1->Add("*.root");
	ch1->MakeClass("PixNtuple");
	gSystem->CompileMacro("PixNtuple.C","k");
	
	// make a histogram
	TH1D *charge = new TH1D("charge","charge",100,0,100);
	
	// create an analysis object from your chain
	PixNtuple an(ch1);
	int nruns=0;
	Int_t nbytes;
	// loop over all entries in the chain
	for(int ientry=0; ientry<an.fChain->GetEntries(); ientry++){
		nbytes = an.LoadTree(ientry);
		nbytes=an.GetEntry(ientry);
		
		if(ientry%1000==0)
			std::cout << ientry <<"/" << an.fChain->GetEntries() << std::endl;
		// take a look in PixNtuple.h (automatically generated if this script has been run) to see what variables are available
		// fill a histogram
		charge->Fill(an.Cluster_charge);
		
	}
	// plot the histogram
	new TCanvas();
	charge->Draw();
}
