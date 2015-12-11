#ifndef PIXELNAMETRANSLATOR_GC
#define PIXELNAMETRANSLATOR_GC

#include <map>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class PixelNameTranslator {

public:

  PixelNameTranslator();
  
  void init(); 
  void initDetID(); 
  void initPSINames();

  // -- XDAQ and offline world
  int    xdaq2detID(string xdaqName); 
  string detID2xdaq(int detid);
  int    detID2Bpix(int detid, int &layer, int &ladder, int &module);
  int    detID2Fpix(int detid, int &disk, int &blade, int &panel, int &module);

  // -- PSI module naming
  // string xdaq2psiname(string xdaqName);  FIXME
  // string psiname2xdaq(string xdaqName);  FIXME

private:
  map<int, string> fDetID2Xdaq; 

};

#endif
