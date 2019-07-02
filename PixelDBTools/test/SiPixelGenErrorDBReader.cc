// This reader still does not work so well.
// The formatting of readout is wrong.
// The comparison only works if the ascii files are available.
// I do not know how to access the internal numbers in a usefull way?
#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
//#include "RecoLocalTracker/SiPixelRecHits/interface/SiPixelGenError.h"
#include "CondFormats/SiPixelTransient/interface/SiPixelGenError.h"
//#include "MagneticField/Engine/interface/MagneticField.h"
//#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"


#include "SiPixelGenErrorDBReader.h"

using namespace std;

SiPixelGenErrorDBReader::SiPixelGenErrorDBReader(const edm::ParameterSet& iConfig):
	theGenErrorCalibrationLocation( iConfig.getParameter<std::string>("siPixelGenErrorCalibrationLocation") ),
	theDetailedGenErrorDBErrorOutput( iConfig.getParameter<bool>("wantDetailedGenErrorDBErrorOutput") ),
	theFullGenErrorDBOutput( iConfig.getParameter<bool>("wantFullGenErrorDBOutput") ) {
  //phase1_ = iConfig.getUntrackedParameter<bool>("phase1","false");
  //if(phase1_) cout<<" Use phase-1 geometry"<<endl;
}

SiPixelGenErrorDBReader::~SiPixelGenErrorDBReader()
{
}

void 
SiPixelGenErrorDBReader::beginJob()
{
}

void
SiPixelGenErrorDBReader::analyze(const edm::Event& iEvent, const edm::EventSetup& setup) {

  edm::ESHandle<SiPixelGenErrorDBObject> generrorH;
  setup.get<SiPixelGenErrorDBObjectRcd>().get(generrorH);
  dbobject = *generrorH.product();
  const SiPixelGenErrorDBObject * db = generrorH.product();

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopo;
  setup.get<TrackerTopologyRcd>().get(tTopo);

  // these seem to be the only variables I can get directly from the object class
  cout<<" DBObject version "<<dbobject.version()<<" index "<<dbobject.index()<<" max "<<dbobject.maxIndex()
      <<" fail "<<dbobject.fail()<<" numOfTeml "<<dbobject.numOfTempl()<<endl;

  std::cout << "Map info" << std::endl;
  std::vector<short> tempMapId;
  std::map<unsigned int,short> templMap=dbobject.getGenErrorIDs();
  for(std::map<unsigned int,short>::const_iterator it=templMap.begin(); it!=templMap.end();++it) {

    if(tempMapId.size()==0) tempMapId.push_back(it->second);

    for(unsigned int i=0; i<tempMapId.size();++i) {
      if( tempMapId[i] == it->second ) break;
      else if(i==tempMapId.size()-1) {
	tempMapId.push_back(it->second);
	break;
      }
    }
    
    if(theFullGenErrorDBOutput) { 
      // Find first where we are 
      auto detid = it->first;  // raw det id 
      auto subdet   = DetId(detid).subdetId(); // bpix/fpix 
      
      std::cout<< "DetId: "<<detid<<" GenErrorID: "<< it->second;  

      if(subdet == static_cast<int>(PixelSubdetector::PixelBarrel)) {  // BPix
      
	unsigned int layerC = tTopo->pxbLayer(detid);
	// Barrel ladder id 1-20,32,44.
	unsigned int ladderC = tTopo->pxbLadder(detid);
	// Barrel Z-index=1,8
	unsigned int moduleC = tTopo->pxbModule(detid);

	cout<<" bpix "<<layerC<<" "<<ladderC<<" "<<moduleC<<endl;

      } else if(subdet == static_cast<int>(PixelSubdetector::PixelEndcap)){

	unsigned int disk=tTopo->pxfDisk(detid);   //1,2,3
	unsigned int blade=tTopo->pxfBlade(detid); //1-24
	unsigned int side=tTopo->pxfSide(detid);   //sizd=1 for -z, 2 for +z
	unsigned int panel=tTopo->pxfPanel(detid);   //sizd=1 for -z, 2 for +z
	unsigned int moduleF=tTopo->pxfModule(detid); //
	
	cout<<" fpix "<<disk<<" "<<blade<<" "<<side<<" "<<panel<<" "<<moduleF<<endl;
      
      } // subdet 

    } // if FullOut  

  } // map
    
  std::cout << "\nMap stored GenError Id(s), size =  "<<tempMapId.size()<<endl;
  for(unsigned int vindex=0; vindex < tempMapId.size(); ++ vindex)
    std::cout << tempMapId[vindex] << " ";
  std::cout << std::endl;

  // Try to interpret the object 
  vector<SiPixelGenErrorStore> thePixelGenError;
  //const SiPixelGenErrorDBObject * ge = &dbobject;
  bool status = SiPixelGenError::pushfile(*db, thePixelGenError);
  cout<<" status = "<<status<<" size = "<<thePixelGenError.size()<<endl;
  
  SiPixelGenError genError(thePixelGenError);
  // these are all 0 because qbin() was not run.
  cout<<" some values "
      <<genError.lorxwidth()<<" "<<genError.lorywidth()<<" "
      <<endl;

  // Print the full object, I think it does not work, the print is for templates.
  printObject();

 
  // if the dircetory is an empty string ignore file comparison 
  if(theGenErrorCalibrationLocation == "") {

    cout<<" no file for camparison defined, comparison will be skipped "<<endl;

  } else {  // do the file comparision 

    //if(compareWithFile) {
    bool error=false;
    char c;
    int numOfTempl = dbobject.numOfTempl();
    int index = 0;
    float tempnum=0,diff=0;
    float tol = 1.0E-23;
    bool givenErrorMsg=false;
    
    std::cout << "\nChecking GenError DB object version " << dbobject.version() << " containing " << numOfTempl << " calibration(s) at " << dbobject.sVector()[index+22] <<"T\n";
    
    for(int i=0; i<numOfTempl;++i) {
      
      //Removes header in db object from diff
      index+=20;
      
      //Tell the person viewing the output what the GenError ID and version are -- note that version is only valid for >=13
      // Does not work correctly for data 
      std::cout << "Calibration " << i+1 << " of " << numOfTempl << ", with GenError ID " << dbobject.sVector()[index]
		<< "\tand Version " << dbobject.sVector()[index+1] <<"\t--------  "<<endl;
      
      
      //Opening the text-based GenError calibration
      std::ostringstream tout;
      tout << theGenErrorCalibrationLocation.c_str() << "generror_summary_zp"
	   << std::setw(4) << std::setfill('0') << std::right << dbobject.sVector()[index] << ".out" << std::ends;
      //edm::FileInPath file( tout.str());
      //tempfile = (file.fullPath()).c_str();
      
      string temp = tout.str();
      std::ifstream in_file(temp.c_str(), std::ios::in);
      
      cout<<" open file "<<tout.str()<<" "<<in_file.is_open()<<endl;
      
      if(in_file.is_open()) {	
	//Removes header in textfile from diff
	for(int header=0; (c=in_file.get()) != '\n'; ++header) {}
	
	//First read in from the text file -- this will be compared with index = 20
	in_file >> tempnum;
	
	//Read until the end of the current text file
	while(!in_file.eof()) {
	  //Calculate the difference between the text file and the db object
	  diff = std::abs(tempnum - dbobject.sVector()[index]);
	  
	  //Is there a difference?
	  if(diff > tol) {
	    //We have the if statement to output the message only once
	    if(!givenErrorMsg) std::cout << "does NOT match\n";
	    //If there is an error we want to display a message upon completion
	    error = true;
	    givenErrorMsg = true;
	    //Do we want more detailed output?
	    if(theDetailedGenErrorDBErrorOutput) {
	      std::cout << "from file = " << tempnum << "\t from dbobject = " << dbobject.sVector()[index]
			<< "\tdiff = "    << diff    << "\t db index = "      << index << std::endl;
	    }
	  }
	  //Go to the next entries
	  in_file >> tempnum;
	  ++index;
	}
	//There were no errors, the two files match.
	if(!givenErrorMsg) std::cout << "MATCHES\n";
      } else { //end current file
	cout<<" ERROR: cannot open file, comparison will be stopped"<<endl;
	break;
      }
      in_file.close();
      givenErrorMsg = false;

    } //end loop over all files

    if(error && !theDetailedGenErrorDBErrorOutput)
      cout << "\nThe were differences found between the files and the database.\n"
	   << "If you would like more detailed information please set\n"
	   << "wantDetailedOutput = True in the cfg file. If you would like a\n"
	   << "full output of the contents of the database file please set\n"
	   << "wantFullOutput = True. Make sure that you pipe the output to a\n"
	   << "log file. This could take a few minutes.\n\n";

  } // if compare 
  

}


void SiPixelGenErrorDBReader::endJob() { 

}


// I think this was written for templates and not for genErrors
// so it does not work correctly.
//std::ostream& operator<<(std::ostream& s, const SiPixelGenErrorDBObject& dbobject){
void SiPixelGenErrorDBReader::printObject() {
  const bool FullPrint = false;
  //!-index to keep track of where we are in the object
  int index = 0;
  //!-these are modifiable parameters for the extended GenErrors
  //int txsize[4] = { 7,13,0,0};
  //int tysize[4] = {21,21,0,0};
  //!-entries takes the number of entries in By,Bx,Fy,Fx from the object
  //int entries[4] = {0};
  //!-local indicies for loops
  //int i,j,k,l,m,n,entry_it;
  //!-changes the size of the GenErrors based on the version
  int generrorVersion=0;
  
  std::cout << "\n\nDBobject version: " << dbobject.version() << std::endl;
  
  for(int m=0; m < dbobject.numOfTempl(); ++m)  {
    //To change the size of the output based on which GenError version we are using"
    generrorVersion = (int) dbobject.sVector()[index+21];
    cout<<" GenError version "<<generrorVersion<<" "<<m<<endl;

    std::cout << "************************************************************************************" << std::endl;
    std::cout << "***********           Reading GenError ID " << dbobject.sVector()[index+20]
	      <<" "<< m+1 << "/" << dbobject.numOfTempl() <<")                 ************* " << std::endl;
    std::cout << "************************************************************************************" << std::endl;
    
    //Header Title
    cout<<" Header Title"<<endl;
    SiPixelGenErrorDBObject::char2float temp;
    for (int n=0; n < 20; ++n) {
      temp.f = dbobject.sVector()[index];
      cout << temp.c[0] << temp.c[1] << temp.c[2] << temp.c[3];
      ++index;
    }
    
    //entries[0] = (int) dbobject.sVector()[index+3];                               // Y
    //entries[1] = (int)(dbobject.sVector()[index+4]*dbobject.sVector()[index+5]);   // X
    
    //Header
    cout<<" Header "<<endl;
    cout <<" ID "<< dbobject.sVector()[index]
	 <<" version "<< dbobject.sVector()[index+1]  
	 <<" Bfiled"<< dbobject.sVector()[index+2]
	 <<" NTy "<< dbobject.sVector()[index+3]  
	 <<" NTyx "<< dbobject.sVector()[index+4]  
	 <<" NTxx "<< dbobject.sVector()[index+5]
	 <<endl  
	 <<" Dtype "<< dbobject.sVector()[index+6]
	 <<" Vbias "<< dbobject.sVector()[index+7]  
	 <<" Temperature "<< dbobject.sVector()[index+8]  
	 <<" Fluence "<< dbobject.sVector()[index+9]
	 <<" Qscale "<< dbobject.sVector()[index+10]  
      //<<" s50 "<< dbobject.sVector()[index+11]  
	 <<endl
	 <<" LAYWidth "<< dbobject.sVector()[index+12]
	 <<" LAXWidth "<< dbobject.sVector()[index+13]  
	 <<" YSize "<< dbobject.sVector()[index+14] 
	 <<" XSize "<< dbobject.sVector()[index+15]
	 <<" ZSize "<< dbobject.sVector()[index+16]
      //<<" ss50 "<< dbobject.sVector()[index+17]
	 <<" LAYBias "<< dbobject.sVector()[index+18]
	 <<" LAXBias "<< dbobject.sVector()[index+19]
      //<<" FBin "<< dbobject.sVector()[index+20] <<" "<< dbobject.sVector()[index+21] <<" "
      //<< dbobject.sVector()[index+22]
	 <<endl;
    
    int NTy = dbobject.sVector()[index+3];  
    int NTyx = dbobject.sVector()[index+4];  
    int NTxx = dbobject.sVector()[index+5];  
    index += 23;
    
    if(FullPrint) cout<<"  "<<endl;
    for(int entry_it=0;entry_it<NTy;++entry_it) {
      if(FullPrint) 
	cout <<" run "<<dbobject.sVector()[index]    
	     <<" costrk "<<dbobject.sVector()[index+1]<<" "<< dbobject.sVector()[index+2]<<" "<< dbobject.sVector()[index+3]  
	     << " qavg " << dbobject.sVector()[index+4]  
	     << " pixmax " << dbobject.sVector()[index+5]
	     << " dyone " << dbobject.sVector()[index+6]  
	     << " syone " << dbobject.sVector()[index+7]  
	     << " dxone " << dbobject.sVector()[index+8]
	     << " sxone " << dbobject.sVector()[index+9]  
	     << " dytwo " << dbobject.sVector()[index+10] 
	     << " sytwo " << dbobject.sVector()[index+11]
	     << " dxtwo " << dbobject.sVector()[index+12] 
	     << " sxtwo " << dbobject.sVector()[index+13] 
	     << " qmin " << dbobject.sVector()[index+14]
	     << " qmin2 " << dbobject.sVector()[index+15] 
	     << endl;
      index+=16;

      for(int i=0;i<4;++i) {
	for(int j=0;j<4;++j) {
	  if(FullPrint) cout << dbobject.sVector()[index] <<" ";
	  ++index;
	}
      }
      if(FullPrint) cout << std::endl;
      
    } // NTy


    for(int entry_it=0;entry_it<NTyx;++entry_it) {
      for(int it=0;it<NTxx;++it) {

	if(FullPrint) 
	  cout <<" run "<<dbobject.sVector()[index]    
	       <<" costrk "<<dbobject.sVector()[index+1]<<" "<< dbobject.sVector()[index+2]<<" "<< dbobject.sVector()[index+3]  
	       << " qavg " << dbobject.sVector()[index+4]  
	       << " pixmax " << dbobject.sVector()[index+5]
	       << " dyone " << dbobject.sVector()[index+6]  
	       << " syone " << dbobject.sVector()[index+7]  
	       << " dxone " << dbobject.sVector()[index+8]
	       << " sxone " << dbobject.sVector()[index+9]  
	       << " dytwo " << dbobject.sVector()[index+10] 
	       << " sytwo " << dbobject.sVector()[index+11]
	       << " dxtwo " << dbobject.sVector()[index+12] 
	       << " sxtwo " << dbobject.sVector()[index+13] 
	       << " qmin " << dbobject.sVector()[index+14]
	       << " qmin2 " << dbobject.sVector()[index+15] 
	       << endl;
	index+=16;
	
	for(int i=0;i<4;++i) {
	  for(int j=0;j<4;++j) {
	    if(FullPrint) cout << dbobject.sVector()[index] <<" ";
	    ++index;
	  }
	}
	if(FullPrint) cout << std::endl;

      } // NTxx
    } // NTyx 

  }  // num objects 

  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelGenErrorDBReader);
