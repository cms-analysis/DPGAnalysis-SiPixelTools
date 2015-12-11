#ifndef SiPixelTools_SiPixelFolderOrganizerGC_h
#define SiPixelTools_SiPixelFolderOrganizerGC_h
// -*- C++ -*-
//
// Package:     SiPixelCommon
// Class  :     SiPixelFolderOrganizer
// 
/**\class SiPixelFolderOrganizer SiPixelFolderOrganizer.h DQM/SiPixelCommon/interface/SiPixelFolderOrganizer.h
   
Description: <Organizes the folders for the monitoring elements of the Pixel detector. Its methods return strings with names of folders to be created and used.>

Usage:
<usage>

*/
//
// Original Author:  chiochia
//         Created:  Thu Jan 26 23:49:46 CET 2006

#include <boost/cstdint.hpp>
#include <string>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "TFile.h"
#include "TDirectory.h"
#include "TSystem.h"
class SiPixelFolderOrganizerGC {
  
 public:

  /// Constructor
  SiPixelFolderOrganizerGC();

  /// Destructor
  virtual ~SiPixelFolderOrganizerGC();
  
  /// Set folder name for a module or plaquette
  //type is: BPIX  mod=0, lad=1, lay=2, phi=3, 
  //         FPIX  mod=0, blade=4, disc=5, ring=6
  bool setModuleFolder( std::map<std::string, TFileDirectory>  *myTFileDirMap, const uint32_t& rawdetid=0, int type=0, bool isUpgrade=false  );
  void getModuleFolder(const uint32_t& rawdetid, std::string& path, bool isUpgrade);

  /// Set folder name for a FED (used in the case of errors without detId)
  bool setFedFolder( std::map< std::string, TFileDirectory>  *myTFileDirMap, const uint32_t FedId );

  std::string setModuleFolderPath( const uint32_t& rawdetid=0, int type=0, bool isUpgrade=false  );

  bool dirExists( const char *path);
  std::vector<std::string>  split(const std::string &s, char delim);
  std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);
  TFileDirectory createAndSetDir( std::map<std::string, TFileDirectory>  *myTFileDirMap,   const  std::string  path, bool  isfirst ,  const  std::string alreadyDone);
  TFileDirectory createAndSetDir(  std::map<std::string, TFileDirectory>  *myTFileDirMap, TFileDirectory  dir,  const  std::string  path, bool  isfirst ,  const  std::string alreadyDone );

 private:

  std::string rootFolder;
  std::string slash;
  /* DQMStore* dbe_; */
  TFileService* file_;
  // std::map<std::string, TFileDirectory*>  myTFileDirMap;

};
#endif
