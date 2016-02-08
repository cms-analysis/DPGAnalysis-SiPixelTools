/// DQM and Framework services
#include "SiPixelFolderOrganizerGC.h"
//#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
/// Data Formats
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelNameUpgrade.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapNameUpgrade.h"
#include <sstream>
#include <cstdio>



/// Constructor
SiPixelFolderOrganizerGC::SiPixelFolderOrganizerGC() :
  rootFolder("Pixel"),
  slash("/"),
  // dbe_(edm::Service<DQMStore>().operator->())
  file_(edm::Service<TFileService>().operator->())
{  
}

SiPixelFolderOrganizerGC::~SiPixelFolderOrganizerGC() {}

bool SiPixelFolderOrganizerGC::setModuleFolder( std::map<std::string, TFileDirectory> *myTFileDirMap, const uint32_t& rawdetid, int type, bool isUpgrade  ) {
  
  bool flag = false;
  
  if(rawdetid == 0) {
    // file_->setCurrentFolder(rootFolder);

    TFileDirectory dir;

    // std::cout <<" rawdetid=0 & rootFolder.c_str() = " <<rootFolder.c_str() <<std::endl;
    if (dirExists(rootFolder.c_str()))   { 
    }
    else dir = createAndSetDir(myTFileDirMap, rootFolder.c_str(),1,"");
    
    flag = true;
  }
  ///
  /// Pixel Barrel
  ///
  else if(DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {

     if (!isUpgrade) {
     //for endcap types there is nothing to do: 
     if(type>3 && type!=7) return true;

     std::string subDetectorFolder = "Barrel";
     PixelBarrelName::Shell DBshell = PixelBarrelName(DetId(rawdetid)).shell();
     int DBlayer  = PixelBarrelName(DetId(rawdetid)).layerName();
     int DBladder = PixelBarrelName(DetId(rawdetid)).ladderName();
     int DBmodule = PixelBarrelName(DetId(rawdetid)).moduleName();
     
     char slayer[80];  sprintf(slayer, "Layer_%i",   DBlayer);
     char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
     char smodule[80]; sprintf(smodule,"Module_%i",  DBmodule);
     
     std::ostringstream sfolder;
     
     sfolder << rootFolder << "/" << subDetectorFolder; 
     if(type<4){
     sfolder << "/Shell_" <<DBshell
	     << "/" << slayer;
     }
     if(type<2){
       sfolder << "/" << sladder;
       if ( PixelBarrelName(DetId(rawdetid)).isHalfModule() ) sfolder <<"H"; 
       else sfolder <<"F";
     }
     if(type==0) sfolder << "/" <<smodule;
     //if(type==3) sfolder << "/all_" << smodule;
     
     //std::cout<<"set barrel folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
     
     
     TFileDirectory dir;
    
     if (dirExists(sfolder.str().c_str()))    { //dir=file_->getBareDirectory(sfolder.str().c_str());dir->cd();
     }
     else  dir = createAndSetDir( myTFileDirMap, sfolder.str().c_str(),1,"");
     flag = true;
     } else if (isUpgrade) {
       //for endcap types there is nothing to do: 
       if(type>3 && type!=7) return true;
       
       std::string subDetectorFolder = "Barrel";
       PixelBarrelNameUpgrade::Shell DBshell = PixelBarrelNameUpgrade(DetId(rawdetid)).shell();
       int DBlayer  = PixelBarrelNameUpgrade(DetId(rawdetid)).layerName();
       int DBladder = PixelBarrelNameUpgrade(DetId(rawdetid)).ladderName();
       int DBmodule = PixelBarrelNameUpgrade(DetId(rawdetid)).moduleName();
       
       char slayer[80];  sprintf(slayer, "Layer_%i",   DBlayer);
       char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
       char smodule[80]; sprintf(smodule,"Module_%i",  DBmodule);
       
       std::ostringstream sfolder;
       
       sfolder << rootFolder << "/" << subDetectorFolder; 
       if(type<4){
       sfolder << "/Shell_" <<DBshell
               << "/" << slayer;
   } 
       if(type<2){
         sfolder << "/" << sladder;
         if ( PixelBarrelNameUpgrade(DetId(rawdetid)).isHalfModule() ) sfolder <<"H"; 
         else sfolder <<"F";
       }
       if(type==0) sfolder << "/" <<smodule;
       //if(type==3) sfolder << "/all_" << smodule;
       
       //std::cout<<"set barrel folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
       
       // file_->setCurrentFolder(sfolder.str().c_str());
       // if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
       // file_->cd(sfolder.str().c_str());
       // std::cout <<"2  sfolder.str().c_str() = " <<sfolder.str().c_str() <<std::endl;
       TFileDirectory dir;
       if (dirExists(sfolder.str().c_str()))   { // dir=file_->getBareDirectory(sfolder.str().c_str()); dir->cd();
       }
       else dir = createAndSetDir(   myTFileDirMap, sfolder.str().c_str(),1,"");
       flag = true;
     }//endif(isUpgrade)
   } 
   
   ///
   /// Pixel Endcap
   ///
   else if(DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {

     if (!isUpgrade) {
     //for barrel types there is nothing to do: 
     if(type>0 && type < 4) return true;

     std::string subDetectorFolder = "Endcap";
     PixelEndcapName::HalfCylinder side = PixelEndcapName(DetId(rawdetid)).halfCylinder();
      int disk   = PixelEndcapName(DetId(rawdetid)).diskName();
      int blade  = PixelEndcapName(DetId(rawdetid)).bladeName();
      int panel  = PixelEndcapName(DetId(rawdetid)).pannelName();
      int module = PixelEndcapName(DetId(rawdetid)).plaquetteName();

      char sdisk[80];  sprintf(sdisk,  "Disk_%i",disk);
      char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
      char spanel[80]; sprintf(spanel, "Panel_%i",panel);
      char smodule[80];sprintf(smodule,"Module_%i",module);

      std::ostringstream sfolder;

      sfolder <<rootFolder <<"/" << subDetectorFolder << 
	"/HalfCylinder_" << side << "/" << sdisk; 
      if(type==0 || type ==4){
	sfolder << "/" << sblade; 
      }
      if(type==0){
	sfolder << "/" << spanel << "/" << smodule;
      }
//       if(type==6){
// 	sfolder << "/" << spanel << "_all_" << smodule;
//       }
     
     //std::cout<<"set endcap folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
     
     // file_->setCurrentFolder(sfolder.str().c_str());
      // if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
      // file_->cd(sfolder.str().c_str());
     
      // std::cout <<"3  sfolder.str().c_str() = " <<sfolder.str().c_str() <<std::endl;

      TFileDirectory dir;
      if (dirExists(sfolder.str().c_str()))   {// dir=file_->getBareDirectory(sfolder.str().c_str()); dir->cd(); 
      }
      else dir = createAndSetDir( myTFileDirMap, sfolder.str().c_str(),1,"");
      flag = true;

     } else if (isUpgrade) {
       //for barrel types there is nothing to do: 
       if(type>0 && type < 4) return true;
       
       std::string subDetectorFolder = "Endcap";
       PixelEndcapNameUpgrade::HalfCylinder side = PixelEndcapNameUpgrade(DetId(rawdetid)).halfCylinder();
        int disk   = PixelEndcapNameUpgrade(DetId(rawdetid)).diskName();
        int blade  = PixelEndcapNameUpgrade(DetId(rawdetid)).bladeName();
        int panel  = PixelEndcapNameUpgrade(DetId(rawdetid)).pannelName();
        int module = PixelEndcapNameUpgrade(DetId(rawdetid)).plaquetteName();
        
        
        char sdisk[80];  sprintf(sdisk,  "Disk_%i",disk);
        char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
        char spanel[80]; sprintf(spanel, "Panel_%i",panel);
        char smodule[80];sprintf(smodule,"Module_%i",module);
        
        std::ostringstream sfolder;
        
        sfolder <<rootFolder <<"/" << subDetectorFolder << 
          "/HalfCylinder_" << side << "/" << sdisk; 
        if(type==0 || type ==4){
          sfolder << "/" << sblade; 
        }
        if(type==0){
          sfolder << "/" << spanel << "/" << smodule;
        }
//        if(type==6){
//          sfolder << "/" << spanel << "_all_" << smodule;
//        }
       
       //std::cout<<"set endcap folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
       
	// file_->setCurrentFolder(sfolder.str().c_str());
	// if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
	// file_->cd(sfolder.str().c_str());
	// std::cout <<"4  sfolder.str().c_str() = " <<sfolder.str().c_str() <<std::endl;
	TFileDirectory dir;
	if (dirExists(sfolder.str().c_str()))  {  // dir=file_->getBareDirectory(sfolder.str().c_str()); dir->cd();
	}
	else dir = createAndSetDir(myTFileDirMap, sfolder.str().c_str(),1,"");
	
	flag = true;
     }//endifendcap&&isUpgrade
   } else throw cms::Exception("LogicError")
     << "[SiPixelFolderOrganizerGC::setModuleFolder] Not a Pixel detector DetId ";
   
   return flag;

}

bool SiPixelFolderOrganizerGC::setFedFolder(std::map<std::string, TFileDirectory>  *myTFileDirMap, const uint32_t FedId ) {

  std::string subDetectorFolder = "AdditionalPixelErrors";
  char sFed[80];  sprintf(sFed,  "FED_%i",FedId);
  std::ostringstream sfolder;
  
  sfolder << rootFolder << "/" << subDetectorFolder << "/" << sFed;
  //file_->setCurrentFolder(sfolder.str().c_str());
  // if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
  // file_->cd(sfolder.str().c_str());
 
  TFileDirectory dir;
  if (dirExists(sfolder.str().c_str()))   { // dir=file_->getBareDirectory(sfolder.str().c_str());   dir->cd();
  }
  else dir = createAndSetDir(myTFileDirMap,  sfolder.str().c_str(),1,"");
  return true;

}

void SiPixelFolderOrganizerGC::getModuleFolder(const uint32_t& rawdetid, 
                                             std::string& path,
                                             bool isUpgrade) {

  path = rootFolder;
  if(rawdetid == 0) {
    return;
  }else if( (DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) && (!isUpgrade) ) {
    std::string subDetectorFolder = "Barrel";
    PixelBarrelName::Shell DBshell = PixelBarrelName(DetId(rawdetid)).shell();
    int DBlayer  = PixelBarrelName(DetId(rawdetid)).layerName();
    int DBladder = PixelBarrelName(DetId(rawdetid)).ladderName();
    int DBmodule = PixelBarrelName(DetId(rawdetid)).moduleName();
    
    //char sshell[80];  sprintf(sshell, "Shell_%i",   DBshell);
    char slayer[80];  sprintf(slayer, "Layer_%i",   DBlayer);
    char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
    char smodule[80]; sprintf(smodule,"Module_%i",  DBmodule);
    
    std::ostringstream sfolder;
    sfolder << rootFolder << "/" << subDetectorFolder << "/Shell_" <<DBshell << "/" << slayer << "/" << sladder;
    if ( PixelBarrelName(DetId(rawdetid)).isHalfModule() ) sfolder <<"H"; 
    else sfolder <<"F";
    sfolder << "/" <<smodule;
    path = sfolder.str().c_str();
   
    //path = path + "/" + subDetectorFolder + "/" + sshell + "/" + slayer + "/" + sladder;
    //if(PixelBarrelName(DetId(rawdetid)).isHalfModule() )
    //  path = path + "H"; 
    //else path = path + "F";
    //path = path + "/" + smodule;

  }else if( (DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) && (isUpgrade) ) {
    std::string subDetectorFolder = "Barrel";
    PixelBarrelNameUpgrade::Shell DBshell = PixelBarrelNameUpgrade(DetId(rawdetid)).shell();
    int DBlayer  = PixelBarrelNameUpgrade(DetId(rawdetid)).layerName();
    int DBladder = PixelBarrelNameUpgrade(DetId(rawdetid)).ladderName();
    int DBmodule = PixelBarrelNameUpgrade(DetId(rawdetid)).moduleName();
    
    //char sshell[80];  sprintf(sshell, "Shell_%i",   DBshell);
    char slayer[80];  sprintf(slayer, "Layer_%i",   DBlayer);
    char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
    char smodule[80]; sprintf(smodule,"Module_%i",  DBmodule);
    
    std::ostringstream sfolder;
    sfolder << rootFolder << "/" << subDetectorFolder << "/Shell_" <<DBshell << "/" << slayer << "/" << sladder;
    if ( PixelBarrelNameUpgrade(DetId(rawdetid)).isHalfModule() ) sfolder <<"H"; 
    else sfolder <<"F";
    sfolder << "/" <<smodule;
    path = sfolder.str().c_str();
   
    //path = path + "/" + subDetectorFolder + "/" + sshell + "/" + slayer + "/" + sladder;
    //if(PixelBarrelNameUpgrade(DetId(rawdetid)).isHalfModule() )
    //  path = path + "H"; 
    //else path = path + "F";
    //path = path + "/" + smodule;

  } else if( (DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) && (!isUpgrade) ) {
    std::string subDetectorFolder = "Endcap";
    PixelEndcapName::HalfCylinder side = PixelEndcapName(DetId(rawdetid)).halfCylinder();
    int disk   = PixelEndcapName(DetId(rawdetid)).diskName();
    int blade  = PixelEndcapName(DetId(rawdetid)).bladeName();
    int panel  = PixelEndcapName(DetId(rawdetid)).pannelName();
    int module = PixelEndcapName(DetId(rawdetid)).plaquetteName();

    //char shc[80];  sprintf(shc,  "HalfCylinder_%i",side);
    char sdisk[80];  sprintf(sdisk,  "Disk_%i",disk);
    char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
    char spanel[80]; sprintf(spanel, "Panel_%i",panel);
    char smodule[80];sprintf(smodule,"Module_%i",module);

    std::ostringstream sfolder;
    sfolder <<rootFolder <<"/" << subDetectorFolder << "/HalfCylinder_" << side << "/" << sdisk << "/" << sblade << "/" << spanel << "/" << smodule;
    path = sfolder.str().c_str();
    
    //path = path + "/" + subDetectorFolder + "/" + shc + "/" + sdisk + "/" + sblade + "/" + spanel + "/" + smodule;

  } else if( (DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) && (isUpgrade) ) {
    std::string subDetectorFolder = "Endcap";
    PixelEndcapNameUpgrade::HalfCylinder side = PixelEndcapNameUpgrade(DetId(rawdetid)).halfCylinder();
    int disk   = PixelEndcapNameUpgrade(DetId(rawdetid)).diskName();
    int blade  = PixelEndcapNameUpgrade(DetId(rawdetid)).bladeName();
    int panel  = PixelEndcapNameUpgrade(DetId(rawdetid)).pannelName();
    int module = PixelEndcapNameUpgrade(DetId(rawdetid)).plaquetteName();

    //char shc[80];  sprintf(shc,  "HalfCylinder_%i",side);
    char sdisk[80];  sprintf(sdisk,  "Disk_%i",disk);
    char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
    char spanel[80]; sprintf(spanel, "Panel_%i",panel);
    char smodule[80];sprintf(smodule,"Module_%i",module);

    std::ostringstream sfolder;
    sfolder <<rootFolder <<"/" << subDetectorFolder << "/HalfCylinder_" << side << "/" << sdisk << "/" << sblade << "/" << spanel << "/" << smodule;
    path = sfolder.str().c_str();
    
    //path = path + "/" + subDetectorFolder + "/" + shc + "/" + sdisk + "/" + sblade + "/" + spanel + "/" + smodule;

  }else throw cms::Exception("LogicError")
     << "[SiPixelFolderOrganizerGC::getModuleFolder] Not a Pixel detector DetId ";
     
  //std::cout<<"resulting final path name: "<<path<<std::endl;   
     
  return;
}


bool  SiPixelFolderOrganizerGC::dirExists( const char *path){
   struct stat info;
   if (stat (path, &info )!=0)
     return 0;
   else if (info.st_mode & S_IFDIR)
     return 1;
   else return 0;
}
TFileDirectory SiPixelFolderOrganizerGC::createAndSetDir(  std::map<std::string, TFileDirectory>  *myTFileDirMap, const  std::string  path, bool  isfirst =1,  const  std::string alreadyDone ="" ){
  TFileDirectory  dir;
  /////Create the first dir in the directory, write it into the map and call the other function to create the other subdir////
  
  if (isfirst){
    std::vector<std::string>  pieces;
    pieces = split(path, '/');
    
   
    if (pieces.size()<=1) { 
      // std::cout <<"Prima chiamata scrivo :  pieces.size() "  << pieces[0].c_str() <<std::endl;
      if  ((*myTFileDirMap).find(pieces[0].c_str())== (*myTFileDirMap).end()){
	dir = file_->mkdir(pieces[0].c_str(),pieces[0].c_str());
	(*myTFileDirMap)[pieces[0].c_str()]= dir;
	return dir;}
      else return (*myTFileDirMap)[pieces[0].c_str()];
    }
    else{     
      int beginIdx = path.find('/');
      std::string subdir = path.substr(beginIdx + 1);
      if  ((*myTFileDirMap).find(pieces[0].c_str())== (*myTFileDirMap).end())   {
	dir = file_->mkdir(pieces[0].c_str(),pieces[0].c_str());
	(*myTFileDirMap)[pieces[0].c_str()] = dir;
	// std::cout <<"Prima chiamata scrivo :  pieces.size() "  << pieces[0].c_str() << " e poi creo  subdir"<<  subdir <<std::endl;
      }
      else 	dir =(*myTFileDirMap)[pieces[0].c_str()];
      createAndSetDir( myTFileDirMap, dir, subdir,0, pieces[0].c_str() );
      
    }
  }
  return dir;
}

TFileDirectory SiPixelFolderOrganizerGC::createAndSetDir(  std::map<std::string, TFileDirectory>  *myTFileDirMap, TFileDirectory present , const  std::string  path, bool  isfirst =0,  const  std::string alreadyDone ="" ){


  TFileDirectory  dir;
  // std::cout << "Gia' chiamata prima:  path = " << path <<  " alreadyDone "<<  alreadyDone <<  std::endl;
  std::vector<std::string>  pieces;
  pieces = split(path, '/');
  // std::cout <<" isfirst " << isfirst  <<" pieces.size() "  << pieces.size() <<std::endl;
  
  if (pieces.size()<=1) { 
    // std::cout <<" The last piece II :  pieces[0].c_str()  " <<  pieces[0].c_str() <<" written ! in path  alreadyDone+ Clemens       + pieces[0].c_str()  "<< alreadyDone<< " Clemens "<< pieces[0].c_str()<< std::endl; 
    if  ((*myTFileDirMap).find(alreadyDone+'/'+pieces[0].c_str())== (*myTFileDirMap).end()){
       dir = present.mkdir(pieces[0].c_str(),pieces[0].c_str());
       (*myTFileDirMap)[alreadyDone+'/'+ pieces[0].c_str()]= dir;
      
       // std::cout << " Leavig createAndSetDir(): dir  " << alreadyDone+'/'+ pieces[0].c_str() << " habs been created ; and I put an Histo pt=1"  <<std::endl;
       return dir;    }
    else return (*myTFileDirMap)[pieces[0].c_str()];
     
  }    else{    
    int beginIdx = path.find('/');
    std::string subdir = path.substr(beginIdx + 1);
    
    if  ((*myTFileDirMap).find(alreadyDone+'/'+pieces[0].c_str())== (*myTFileDirMap).end()){
      dir = present.mkdir(pieces[0].c_str(),pieces[0].c_str());   
      (*myTFileDirMap)[alreadyDone+'/'+pieces[0].c_str()]= dir;
      // std::cout <<" Sto scrivendo : pieces[0].c_str() " << pieces[0].c_str() << " e ora vado a scrivere subdir "  << subdir <<" e metto come already done "<< alreadyDone+'/'+pieces[0].c_str() << std::endl;
    }
    
    else  dir=(*myTFileDirMap)[alreadyDone+'/'+pieces[0].c_str()];
    createAndSetDir(myTFileDirMap, dir,  subdir, 0 , alreadyDone+'/'+pieces[0].c_str() );
  }
  
  
  return dir;
}


std::vector<std::string> SiPixelFolderOrganizerGC:: split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}


std::vector<std::string>  SiPixelFolderOrganizerGC::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string SiPixelFolderOrganizerGC::setModuleFolderPath( const uint32_t& rawdetid, int type, bool isUpgrade  ) {
  ///// Return the path of the directory for the map ////
 
  
  if(rawdetid == 0) {
    
   
    return rootFolder.c_str();
    
 
  }
  ///
  /// Pixel Barrel
  ///
  else if(DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {

     if (!isUpgrade) {
     //for endcap types there is nothing to do: 

     std::string subDetectorFolder = "Barrel";
     PixelBarrelName::Shell DBshell = PixelBarrelName(DetId(rawdetid)).shell();
     int DBlayer  = PixelBarrelName(DetId(rawdetid)).layerName();
     int DBladder = PixelBarrelName(DetId(rawdetid)).ladderName();
     int DBmodule = PixelBarrelName(DetId(rawdetid)).moduleName();
     
     char slayer[80];  sprintf(slayer, "Layer_%i",   DBlayer);
     char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
     char smodule[80]; sprintf(smodule,"Module_%i",  DBmodule);
     
     std::ostringstream sfolder;
     
     sfolder << rootFolder << "/" << subDetectorFolder; 
     if(type<4){
     sfolder << "/Shell_" <<DBshell
	     << "/" << slayer;
     }
     if(type<2){
       sfolder << "/" << sladder;
       if ( PixelBarrelName(DetId(rawdetid)).isHalfModule() ) sfolder <<"H"; 
       else sfolder <<"F";
     }
     if(type==0) sfolder << "/" <<smodule;
     //if(type==3) sfolder << "/all_" << smodule;
     
     //std::cout<<"set barrel folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
     
     //file_->setCurrentFolder(sfolder.str().c_str());
     // if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
     // file_->cd(sfolder.str().c_str());
     
     // std::cout<<"Prima pwd gDirectory->pwd()" <<gDirectory->pwd()<<std:: endl;
   
     return sfolder.str().c_str();
     
   
     } else if (isUpgrade) {
       //for endcap types there is nothing to do: 
       
       std::string subDetectorFolder = "Barrel";
       PixelBarrelNameUpgrade::Shell DBshell = PixelBarrelNameUpgrade(DetId(rawdetid)).shell();
       int DBlayer  = PixelBarrelNameUpgrade(DetId(rawdetid)).layerName();
       int DBladder = PixelBarrelNameUpgrade(DetId(rawdetid)).ladderName();
       int DBmodule = PixelBarrelNameUpgrade(DetId(rawdetid)).moduleName();
       
       char slayer[80];  sprintf(slayer, "Layer_%i",   DBlayer);
       char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
       char smodule[80]; sprintf(smodule,"Module_%i",  DBmodule);
       
       std::ostringstream sfolder;
       
       sfolder << rootFolder << "/" << subDetectorFolder; 
       if(type<4){
	 sfolder << "/Shell_" <<DBshell
		 << "/" << slayer;
       } 
       if(type<2){
         sfolder << "/" << sladder;
         if ( PixelBarrelNameUpgrade(DetId(rawdetid)).isHalfModule() ) sfolder <<"H"; 
         else sfolder <<"F";
       }
       if(type==0) sfolder << "/" <<smodule;
       //if(type==3) sfolder << "/all_" << smodule;
       
       //std::cout<<"set barrel folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
       
       // file_->setCurrentFolder(sfolder.str().c_str());
       // if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
       // file_->cd(sfolder.str().c_str());
       return sfolder.str().c_str();
     
     }//endif(isUpgrade)
  } 
  
  ///
  /// Pixel Endcap
   ///
   else if(DetId(rawdetid).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {

     if (!isUpgrade) {
     //for barrel types there is nothing to do: 
     

     std::string subDetectorFolder = "Endcap";
     PixelEndcapName::HalfCylinder side = PixelEndcapName(DetId(rawdetid)).halfCylinder();
      int disk   = PixelEndcapName(DetId(rawdetid)).diskName();
      int blade  = PixelEndcapName(DetId(rawdetid)).bladeName();
      int panel  = PixelEndcapName(DetId(rawdetid)).pannelName();
      int module = PixelEndcapName(DetId(rawdetid)).plaquetteName();

      char sdisk[80];  sprintf(sdisk,  "Disk_%i",disk);
      char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
      char spanel[80]; sprintf(spanel, "Panel_%i",panel);
      char smodule[80];sprintf(smodule,"Module_%i",module);

      std::ostringstream sfolder;

      sfolder <<rootFolder <<"/" << subDetectorFolder << 
	"/HalfCylinder_" << side << "/" << sdisk; 
      if(type==0 || type ==4){
	sfolder << "/" << sblade; 
      }
      if(type==0){
	sfolder << "/" << spanel << "/" << smodule;
      }
//       if(type==6){
// 	sfolder << "/" << spanel << "_all_" << smodule;
//       }
     
     //std::cout<<"set endcap folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
     
     // file_->setCurrentFolder(sfolder.str().c_str());
      // if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
      // file_->cd(sfolder.str().c_str());
     
      return sfolder.str().c_str();



     } else if (isUpgrade) {
       //for barrel types there is nothing to do: 
    
       
       std::string subDetectorFolder = "Endcap";
       PixelEndcapNameUpgrade::HalfCylinder side = PixelEndcapNameUpgrade(DetId(rawdetid)).halfCylinder();
        int disk   = PixelEndcapNameUpgrade(DetId(rawdetid)).diskName();
        int blade  = PixelEndcapNameUpgrade(DetId(rawdetid)).bladeName();
        int panel  = PixelEndcapNameUpgrade(DetId(rawdetid)).pannelName();
        int module = PixelEndcapNameUpgrade(DetId(rawdetid)).plaquetteName();
        
        
        char sdisk[80];  sprintf(sdisk,  "Disk_%i",disk);
        char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
        char spanel[80]; sprintf(spanel, "Panel_%i",panel);
        char smodule[80];sprintf(smodule,"Module_%i",module);
        
        std::ostringstream sfolder;
        
        sfolder <<rootFolder <<"/" << subDetectorFolder << 
          "/HalfCylinder_" << side << "/" << sdisk; 
        if(type==0 || type ==4){
          sfolder << "/" << sblade; 
        }
        if(type==0){
          sfolder << "/" << spanel << "/" << smodule;
        }
//        if(type==6){
//          sfolder << "/" << spanel << "_all_" << smodule;
//        }
       
       //std::cout<<"set endcap folder: "<<rawdetid<<" : "<<sfolder.str().c_str()<<std::endl;
       
	// file_->setCurrentFolder(sfolder.str().c_str());
	// if (!dirExists(sfolder.str().c_str()))  file_->mkdir(sfolder.str().c_str());
	// file_->cd(sfolder.str().c_str());
	return  sfolder.str().c_str();
	

     }//endifendcap&&isUpgrade
   } else throw cms::Exception("LogicError")
	    << "[SiPixelFolderOrganizerGC::setModuleFolder] Not a Pixel detector DetId ";
   
  return "";
  
}
