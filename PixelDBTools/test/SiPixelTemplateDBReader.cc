#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>

#include "SiPixelTemplateDBReader.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "MagneticField/Engine/interface/MagneticField.h"

using namespace std;

SiPixelTemplateDBReader::SiPixelTemplateDBReader(const edm::ParameterSet& iConfig):
	theTemplateCalibrationLocation( iConfig.getParameter<std::string>("siPixelTemplateCalibrationLocation") ),
	theDetailedTemplateDBErrorOutput( iConfig.getParameter<bool>("wantDetailedTemplateDBErrorOutput") ),
	theFullTemplateDBOutput( iConfig.getParameter<bool>("wantFullTemplateDBOutput") ),
	useBFieldFromGlobalTag( iConfig.getParameter<bool>("useBFieldFromGlobalTag") ) {
}

SiPixelTemplateDBReader::~SiPixelTemplateDBReader() {
}

void SiPixelTemplateDBReader::beginJob() {
}

void SiPixelTemplateDBReader::analyze(const edm::Event& iEvent, const edm::EventSetup& setup) {
  std::string label = "";

  if(useBFieldFromGlobalTag) {
    edm::ESHandle<MagneticField> magfield;
    setup.get<IdealMagneticFieldRecord>().get(magfield);
    GlobalPoint center(0.0, 0.0, 0.0);
    float theMagField = magfield.product()->inTesla(center).mag();    
    std::cout << " Testing global tag at magfield = " << theMagField << std::endl;
    
    if(     theMagField>=-0.1 && theMagField<1.0 ) label = "0T";
    else if(theMagField>=1.0  && theMagField<2.5 ) label = "2T";
    else if(theMagField>=2.5  && theMagField<3.25) label = "3T";
    else if(theMagField>=3.25 && theMagField<3.65) label = "35T";
    else if(theMagField>=3.9  && theMagField<4.1 ) label = "4T";
    if(label != " ") cout << " use label "<<label<<" to get templates "<<endl;

  } else {
    std::cout << " Loading from file " << std::endl;
  }

  edm::ESHandle<SiPixelTemplateDBObject> templateH;
  setup.get<SiPixelTemplateDBObjectRcd>().get(label,templateH);
  dbobject = *templateH.product();

  //if(hasTriggeredWatcher) {
    std::vector<short> tempMapId;
    
    if(theFullTemplateDBOutput) std::cout << "Map info" << std::endl;
    std::map<unsigned int,short> templMap=dbobject.getTemplateIDs();
    for(std::map<unsigned int,short>::const_iterator it=templMap.begin(); it!=templMap.end();++it) {
      if(tempMapId.size()==0) tempMapId.push_back(it->second);
      for(unsigned int i=0; i<tempMapId.size();++i) {
	if(tempMapId[i]==it->second) continue;
	else if(i==tempMapId.size()-1) {
	  tempMapId.push_back(it->second);
	  break;
	}
      }
      if(0 && theFullTemplateDBOutput) // disable for the moment
	std::cout<< "DetId: "<< it->first<<" TemplateID: "<< it->second<<"\n";
    }
    
    std::cout << "\nMap stores template Id(s): ";
    for(unsigned int vindex=0; vindex < tempMapId.size(); ++ vindex)
      std::cout << tempMapId[vindex] << " ";
    std::cout << std::endl;
    
    // print it
    if(theFullTemplateDBOutput) {
      //std::cout << dbobject << std::endl;
      printObject();
    }

    int numOfTempl = dbobject.numOfTempl();
    std::cout << "\nChecking Template DB object version " << dbobject.version() << " containing " 
	      << numOfTempl << " calibration(s) at " << dbobject.sVector()[22] <<endl;

    return;

    // compare with the ascii file (not available most of the time)

    //local variables
    const char * tempfile;
    char c;
    int index = 0;
    float tempnum=0,diff=0;
    float tol = 1.0E-23;
    bool error=false,givenErrorMsg=false;;
    for(int i=0; i<numOfTempl;++i) {
      
      //Removes header in db object from diff
      index+=20;
      
      //Tell the person viewing the output what the template ID and version are -- note that version is only valid for >=13
      std::cout << "Calibration " << i+1 << " of " << numOfTempl << ", with Template ID " 
		<< dbobject.sVector()[index]<< "\tand Version " << dbobject.sVector()[index+1] <<endl;
      
      //Opening the text-based template calibration
      std::ostringstream tout;
      tout << theTemplateCalibrationLocation.c_str() << "/data/template_summary_zp"
	   << std::setw(4) << std::setfill('0') << std::right << dbobject.sVector()[index] << ".out" << std::ends;
      
      edm::FileInPath file( tout.str());
      tempfile = (file.fullPath()).c_str();
      std::ifstream in_file(tempfile, std::ios::in);
      
      if(in_file.is_open()) {
	
	//Removes header in textfile from diff
	for(int header=0; (c=in_file.get()) != '\n'; ++header) {}
	
	//First read in from the text file -- this will be compared with index = 20
	in_file >> tempnum;
	
	//Read until the end of the current text file
	while(!in_file.eof())
	  {
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
	      if(theDetailedTemplateDBErrorOutput) {
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
      }//end current file
      in_file.close();
      givenErrorMsg = false;
    }//end loop over all files
    
    if(error && !theDetailedTemplateDBErrorOutput)
      std::cout << "\nThe were differences found between the files and the database.\n"
		<< "If you would like more detailed information please set\n"
		<< "wantDetailedOutput = True in the cfg file. If you would like a\n"
		<< "full output of the contents of the database file please set\n"
		<< "wantFullOutput = True. Make sure that you pipe the output to a\n"
		<< "log file. This could take a few minutes.\n\n";
    
    //}
}


void SiPixelTemplateDBReader::endJob() {
}

void SiPixelTemplateDBReader::printObject() {
  const bool FullPrint = false;
  //!-index to keep track of where we are in the object
  int index = 0;
  //!-these are modifiable parameters for the extended templates
  int txsize[4] = { 7,13,0,0};
  int tysize[4] = {21,21,0,0};
  //!-entries takes the number of entries in By,Bx,Fy,Fx from the object
  int entries[4] = {0};
  //!-local indicies for loops
  int i,j,k,l,m,n,entry_it;
  //!-changes the size of the templates based on the version
  int sizeSetter=0,templateVersion=0;
  
  std::cout << " (printObject) DBobject version: " << dbobject.version() << std::endl;
  
  for(m=0; m < dbobject.numOfTempl(); ++m)  {
      //To change the size of the output based on which template version we are using"
      templateVersion = (int) dbobject.sVector()[index+21];
      //templateVersion = (int) dbobject.sVector_[index+21];
      cout<< m<<" template version "<<templateVersion <<endl;

      if(templateVersion<=10) {
	cout<<"*****WARNING***** Use size 21 & 7 for template "<<templateVersion<<endl;
	sizeSetter=0;
      } else {
	sizeSetter=1;
	cout<<"*****WARNING***** Use size 21 & 13 for template "<<templateVersion<<endl;
      }
      
      std::cout << "************************************************************" << endl;
      std::cout << "***************  Reading Template ID " << dbobject.sVector()[index+20]
		<< "\t(" << m+1 << "/" << dbobject.numOfTempl() <<")  *******"<<endl;
      std::cout << "*************************************************************" <<endl;
      
      //Header Title
      SiPixelTemplateDBObject::char2float temp;
      for (n=0; n < 20; ++n) {
	temp.f = dbobject.sVector()[index];
	cout << temp.c[0] << temp.c[1] << temp.c[2] << temp.c[3];
	++index;
      }
      
      //continue;
      
      entries[0] = (int) dbobject.sVector()[index+3];               // Y
      entries[1] = (int)(dbobject.sVector()[index+4]*dbobject.sVector()[index+5]);   // X
    
      //Header
      int templ_version = dbobject.sVector()[index+1];  
      cout <<" id "     << dbobject.sVector()[index]   
	   <<" version "<< dbobject.sVector()[index+1]  
	   <<" B "      << dbobject.sVector()[index+2]
	   << endl
	   <<" NTy "    << dbobject.sVector()[index+3]  
	   <<" NTyx "   << dbobject.sVector()[index+4]  
	   <<" NTxx "   << dbobject.sVector()[index+5]
	   <<" Type "   << dbobject.sVector()[index+6]
	   << endl  
	   <<" Bias "   << dbobject.sVector()[index+7]  
	   <<" Temparature "<< dbobject.sVector()[index+8]
	   <<" Fluence "<< dbobject.sVector()[index+9]  
	   <<" Qscale " << dbobject.sVector()[index+10] 
	   <<" s50 "    << dbobject.sVector()[index+11]
	   << endl
	   <<" LoYWid " << dbobject.sVector()[index+12] 
	   <<" LoXWid " << dbobject.sVector()[index+13] 
	   <<" Ysize "  << dbobject.sVector()[index+14]
	   <<" Xsize "  << dbobject.sVector()[index+15] 
	   <<" Zsize "  << dbobject.sVector()[index+16] 
	   << endl;
      index += 17;
      
      if(templ_version>17) { // read additional things 
	cout <<" ss50 "     << dbobject.sVector()[index]   
	     <<" LoYBias " << dbobject.sVector()[index+1]  
	     <<" LoXBias " << dbobject.sVector()[index+2]
	     <<" FBin " << dbobject.sVector()[index+3]
	     <<" "      << dbobject.sVector()[index+4]
	     <<" "      << dbobject.sVector()[index+5]
	     <<endl;
	index += 6;
      }

      //Loop over By,Bx,Fy,Fx
      for(entry_it=0;entry_it<4;++entry_it) {
	//Run,costrk,qavg,...,clslenx
	for(i=0;i < entries[entry_it];++i) {
	  if(FullPrint) 
	    cout << dbobject.sVector()[index]  //run    
		 << "\t" << dbobject.sVector()[index+1] // costrk 
		 << "\t" << dbobject.sVector()[index+2] // costrk
		 << "\t" << dbobject.sVector()[index+3] // costrk

		 << "\n" << dbobject.sVector()[index+4] // Qav
		 << "\t" << dbobject.sVector()[index+5] // pixmax
		 << "\t" << dbobject.sVector()[index+6] //symax
		 << "\t" << dbobject.sVector()[index+7] // dyone  
		 << "\t" << dbobject.sVector()[index+8] // syone
		 << "\t" << dbobject.sVector()[index+9]    //sxmax
		 << "\t" << dbobject.sVector()[index+10] // dzone
		 << "\t" << dbobject.sVector()[index+11] //sxone

		 << "\n" << dbobject.sVector()[index+12] //dytwo
		 << "\t" << dbobject.sVector()[index+13] //sytwo
		 << "\t" << dbobject.sVector()[index+14] //dxtwo
		 << "\t" << dbobject.sVector()[index+15] //sxtwo
		 << "\t" << dbobject.sVector()[index+16] //qmin
		 << "\t" << dbobject.sVector()[index+17] // clsleny
		 << "\t" << dbobject.sVector()[index+18] // clslenx
		 << std::endl;
	  index+=19;
	  //YPar
	  for(j=0;j<2;++j) {
	    for(k=0;k<5;++k) {
	      if(FullPrint) cout << dbobject.sVector()[index] << "\t";
	      ++index;
	    }
	    if(FullPrint) cout << std::endl;
	  }
	  //YTemp
	  for(j=0;j<9;++j) {
	    for(k=0;k<tysize[sizeSetter];++k) {
	      if(FullPrint) cout << dbobject.sVector()[index] << "\t";
	      ++index;
	    }
	    if(FullPrint) cout << std::endl;
	  }
	  //XPar
	  for(j=0;j<2;++j) {
	    for(k=0;k<5;++k) {
	      if(FullPrint) cout << dbobject.sVector()[index] << "\t";
	      ++index;
	    }
	    if(FullPrint) cout << std::endl;
	  }
	  //XTemp
	  for(j=0;j<9;++j)
	    {
	      for(k=0;k<txsize[sizeSetter];++k)
		{
		  if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		  ++index;
		}
	      if(FullPrint) cout << std::endl;
	    }
	  //Y average reco params  4*4 yavg, yrms,...
	  for(j=0;j<4;++j)
	    {
	      for(k=0;k<4;++k)
		{
		  if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		  ++index;
		}
	      if(FullPrint) cout << std::endl;
	    }
	  //Yflpar  4*6 yflpar 
	  for(j=0;j<4;++j)
	    {
	      for(k=0;k<6;++k)
		{
		  if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		  ++index;
		}
	      if(FullPrint) cout << std::endl;
	    }
	  //X average reco params
	  for(j=0;j<4;++j)  // 4*4 xavg
	    {
	      for(k=0;k<4;++k)
		{
		  if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		  ++index;
		}
	      if(FullPrint) cout << std::endl;
	    }
	  //Xflpar  4*6 xflpar
	  for(j=0;j<4;++j)
	    {
	      for(k=0;k<6;++k)
		{
		  if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		  ++index;
		}
	      if(FullPrint) cout << std::endl;
	    }
	  //Chi2X,Y  4*4
	  for(j=0;j<4;++j) {
	      for(k=0;k<2;++k) {
		  for(l=0;l<2;++l) {
		    if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		    ++index;
		  }
	      }
	      if(FullPrint) cout << std::endl;
	  }
	  //Y average Chi2 params 4*4
	  for(j=0;j<4;++j) {
	      for(k=0;k<4;++k) {
		if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		++index;
	      }
	      if(FullPrint) cout << std::endl;
	  }
	  //X average Chi2 params 4*4
	  for(j=0;j<4;++j) {
	      for(k=0;k<4;++k) {
		if(FullPrint) cout << dbobject.sVector()[index] << "\t";
		++index;
	      }
	      if(FullPrint) cout << std::endl;
	  }
	  //Y average reco paramif(FullPrint) cout for CPE Generic 4*4
	  for(j=0;j<4;++j) {
	    for(k=0;k<4;++k) {
	      if(FullPrint) cout << dbobject.sVector()[index] << "\t";
	      ++index;
	    }
	    if(FullPrint) cout << std::endl;
	  }
	  //X average reco paramif(FullPrint) cout for CPE Generic 4*4
	  for(j=0;j<4;++j) {
	    for(k=0;k<4;++k) {
	      if(FullPrint) cout << dbobject.sVector()[index] << "\t";
	      ++index;
	    }
	    if(FullPrint) cout << std::endl;
	  }
	  //SpareX,Y  2 * 10
	  for(j=0;j<20;++j) {
	    if(FullPrint)  cout << dbobject.sVector()[index] << "\t";
	    ++index; 
	    if(j==9 || j==19) if(FullPrint) cout << std::endl;
	  } 
	} 
      }
  }
}

//--------------------------------------------------

std::ostream& operator<<(std::ostream& s, const SiPixelTemplateDBObject& dbobject){
  //!-index to keep track of where we are in the object
  int index = 0;
  //!-these are modifiable parameters for the extended templates
  int txsize[4] = { 7,13,0,0};
  int tysize[4] = {21,21,0,0};
  //!-entries takes the number of entries in By,Bx,Fy,Fx from the object
  int entries[4] = {0};
  //!-local indicies for loops
  int i,j,k,l,m,n,entry_it;
  //!-changes the size of the templates based on the version
  int sizeSetter=0,templateVersion=0;
  
  std::cout << " (<<) DBobject version: " << dbobject.version() << std::endl;
  
  for(m=0; m < dbobject.numOfTempl(); ++m)  {
      //To change the size of the output based on which template version we are using"
      templateVersion = (int) dbobject.sVector_[index+21];
      cout<< m<<" template version "<<templateVersion <<endl;
      if(templateVersion<=10) {
	std::cout << "*****WARNING***** This code will not format this template version properly *****WARNING*****\n";
	sizeSetter=0;
      }
      else if(templateVersion<=16) sizeSetter=1;
      else std::cout << "*****WARNING***** This code has not been tested at formatting this version *****WARNING*****\n";
      
      std::cout << "\n\n*********************************************************************************************" << std::endl;
      std::cout << "***************                  Reading Template ID " << dbobject.sVector_[index+20]
		<< "\t(" << m+1 << "/" << dbobject.numOfTempl_ <<")                 ***************" << std::endl;
      std::cout << "*********************************************************************************************\n\n" << std::endl;
      

      //Header Title
      SiPixelTemplateDBObject::char2float temp;
      for (n=0; n < 20; ++n) {
	temp.f = dbobject.sVector_[index];
	s << temp.c[0] << temp.c[1] << temp.c[2] << temp.c[3];
	++index;
      }
      
      continue;

      entries[0] = (int) dbobject.sVector_[index+3];                               // Y
      entries[1] = (int)(dbobject.sVector_[index+4]*dbobject.sVector_[index+5]);   // X
      
      //Header
      s        << dbobject.sVector_[index]   <<"\t"<< dbobject.sVector_[index+1]  <<"\t"<< dbobject.sVector_[index+2]
	       <<"\t"<< dbobject.sVector_[index+3]  <<"\t"<< dbobject.sVector_[index+4]  <<"\t"<< dbobject.sVector_[index+5]
	       <<"\t"<< dbobject.sVector_[index+6]  <<"\t"<< dbobject.sVector_[index+7]  <<"\t"<< dbobject.sVector_[index+8]
	       <<"\t"<< dbobject.sVector_[index+9]  <<"\t"<< dbobject.sVector_[index+10] <<"\t"<< dbobject.sVector_[index+11]
	       <<"\t"<< dbobject.sVector_[index+12] <<"\t"<< dbobject.sVector_[index+13] <<"\t"<< dbobject.sVector_[index+14]
	       <<"\t"<< dbobject.sVector_[index+15] <<"\t"<< dbobject.sVector_[index+16] << std::endl;
      index += 17;
      
      //Loop over By,Bx,Fy,Fx
      for(entry_it=0;entry_it<4;++entry_it) {
	//Run,costrk,qavg,...,clslenx
	for(i=0;i < entries[entry_it];++i)
	  {
	    s         << dbobject.sVector_[index]    << "\t" << dbobject.sVector_[index+1]  << "\t" << dbobject.sVector_[index+2]
		      << "\t" << dbobject.sVector_[index+3]  << "\n" << dbobject.sVector_[index+4]  << "\t" << dbobject.sVector_[index+5]
		      << "\t" << dbobject.sVector_[index+6]  << "\t" << dbobject.sVector_[index+7]  << "\t" << dbobject.sVector_[index+8]
		      << "\t" << dbobject.sVector_[index+9]  << "\t" << dbobject.sVector_[index+10] << "\t" << dbobject.sVector_[index+11]
		      << "\n" << dbobject.sVector_[index+12] << "\t" << dbobject.sVector_[index+13] << "\t" << dbobject.sVector_[index+14]
		      << "\t" << dbobject.sVector_[index+15] << "\t" << dbobject.sVector_[index+16] << "\t" << dbobject.sVector_[index+17]
		      << "\t" << dbobject.sVector_[index+18] << std::endl;
	    index+=19;
	    //YPar
	    for(j=0;j<2;++j)
	      {
		for(k=0;k<5;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //YTemp
	    for(j=0;j<9;++j)
	      {
		for(k=0;k<tysize[sizeSetter];++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //XPar
	    for(j=0;j<2;++j)
	      {
		for(k=0;k<5;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //XTemp
	    for(j=0;j<9;++j)
	      {
		for(k=0;k<txsize[sizeSetter];++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //Y average reco params
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<4;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //Yflpar
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<6;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //X average reco params
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<4;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //Xflpar
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<6;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //Chi2X,Y
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<2;++k)
		  {
		    for(l=0;l<2;++l)
		      {
			s << dbobject.sVector_[index] << "\t";
			++index;
		      }
		  }
		s << std::endl;
	      }
	    //Y average Chi2 params
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<4;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //X average Chi2 params
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<4;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //Y average reco params for CPE Generic
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<4;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //X average reco params for CPE Generic
	    for(j=0;j<4;++j)
	      {
		for(k=0;k<4;++k)
		  {
		    s << dbobject.sVector_[index] << "\t";
		    ++index;
		  }
		s << std::endl;
	      }
	    //SpareX,Y
	    for(j=0;j<20;++j)
	      {
		s << dbobject.sVector_[index] << "\t";
		++index;
		if(j==9 ||j==19) s << std::endl;
	      }
	  }
      }
    }
  return s;
}
//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelTemplateDBReader);
