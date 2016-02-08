#ifndef __FILES_FUNCS__
#define __FILES_FUNCS__

#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TChainElement.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;


TFile* getFile(TString , bool = true , bool = true);
template <class T> T* getHist(TFile& , TString , bool = true , bool = true);
vector<TString>* getListOfFiles(TString strfiles);




//---------------------------------------------------------------

TFile* getFile(TString str , bool verbose , bool exitIfVoid){

  TFile* f = TFile::Open(str , "READ");
  if( f == 0){
    cout << "Error !! Can't open the file " << str << endl;
    if(exitIfVoid){
      cout << "Exiting ..." << endl;
      exit(0);
    }
    return 0;
  }
  else{
    if(verbose)
      cout << "File : " << str << endl;
    return f;
  }
}



//---------------------------------------------------------------

template <class T>
T* getHist(TFile& f , TString str , bool verbose , bool exitIfVoid){

  T* h = 0;
  if(str.Contains("/"))
    h = (T*) f.Get(str);
  else
    h = (T*) f.FindObjectAny(str);
  
  if( h == 0){
    cout << "Error !! Can't find the hist " << str << " in the given file." << endl;
    if(exitIfVoid){
      cout << "Exitiing ..." << endl;
      exit(0);
    }
    return 0;
  }
  else{
    if(verbose)
      cout << h->ClassName() << " : " << str << endl;
    return h;
  }
}



//---------------------------------------------------------------

vector<TString>* getListOfFiles(TString strfiles){

  vector<TString>* vfiles = new vector<TString>;

  if(strfiles.Contains(".root")){
    TChain chain("evt","");
    chain.Add(strfiles);
    TObjArray* fileElements=chain.GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chEl=0;
    while (( chEl=(TChainElement*)next() ))
      vfiles->push_back(TString(chEl->GetTitle()));
  }
  else if(strfiles.Contains(".txt")){
    ifstream txtfile;
    txtfile.open(strfiles);
    if(!txtfile) {
      cout<<"Unable to read the txt file where the rootfiles are." << endl ;
      cout << strfiles << " doesn't exist." << endl << "Aborting ...";
      exit(0);
    }
    string filename;
    while(txtfile>>filename && filename!="EOF")
      vfiles->push_back(TString(filename));
    txtfile.close();
  }
  else {
    cout << "Unknown type of input to get files. Must contain either .root or .txt extension." << endl << "Aborting ..." << endl;
    exit(0);
  }
  cout << "[getListOfFiles] Will run on " << vfiles->size() << " files" << endl;
  return vfiles;
}

#endif
