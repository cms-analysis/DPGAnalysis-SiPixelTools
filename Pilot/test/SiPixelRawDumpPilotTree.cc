/** \class SiPixelRawDumperPilotTree_H
 *  Plug-in module that dump raw data file 
 *  for pixel subdetector
 *  Added class to interpret the data d.k. 30/10/08
 *  Add histograms. Add pix 0 detection.
 *  Works with v7x, comment out the digis access.
 *  Adopted for Phase1.
 *  Add simple error vs fed num histos.
 *  Specific to the pilot detector.
 *  Modified to make trees, instead of histograms y.t. 5/8/2016
 *  (also incorporated time stamp information)
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "EventFilter/SiPixelRawToDigi/interface/PixelDataFormatter.h"
#include "SiPixelRawDump.h"

// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// For ROOT
//#include <TROOT.h>
#include <TFile.h>
//#include <TF1.h>
//#include <TH2F.h>
#include <TH1D.h>
//#include <TProfile.h>
//#include <TProfile2D.h>
#include <TTree.h>


#include <iostream>
//#include <fstream>

using namespace std;

//#define PHASE1    // For pilot blade keep this commented out

namespace {
  bool printErrors  = false;
  bool printData    = false;
  bool printHeaders = false;
  const bool printBX = false;
  const bool CHECK_PIXELS = true;
  const bool PRINT_BASELINE = false;
  // to store the previous pixel 
  //  int fed0 = -1, chan0 = -1, roc0 = -1, dcol0 = -1, pix0 =-1, count0=-1;
  int countDecodeErrors1=0, countDecodeErrors2=0;
#ifdef PHASE1
  const int n_of_FEDs = 56;
  const int n_of_Channels = 96;
  const int fedIdBpixMax = 40;
  const bool phase1 = true;
#else
  const int n_of_FEDs = 41;
  const int n_of_Channels = 36;
  const int fedIdBpixMax = 32;
  const bool phase1 = false;
#endif
}

// Include the helper decoding class (how did I mange to avoid linking conflicts?)
/////////////////////////////////////////////////////////////////////////////
//class Decode {
//public:
//  Decode() {}
//  ~Decode() {}
//  static int error(int error,int & fedChannel, int fed, int & stat1, int & stat2, bool print=false);
//  int data(int error, int & fedChannel, int fed, int & stat1, int & stat2, bool print=false);
//  static int header(unsigned long long word64, int fed, bool print, unsigned int & bx);
//  static int trailer(unsigned long long word64, int fed, bool print);
//  static int convertToCol(int dcol,int pix);
//  static int convertToRow(int pix);
//  static int checkLayerLink(int fed, int chan);
//  int get_adc(void) {return adc_;}
//  int get_roc(void) {return roc_;}
//  int get_dcol(void) {return dcol_;}
//  int get_pix(void) {return pix_;}
//  int get_col(void) {return col_;}
//  int get_row(void) {return row_;}
//  int get_channel(void) {return channel_;}
//private:
//  int channel_, adc_, roc_, dcol_, pix_, col_, row_;
//};
/////////////////////////////////////////////////////////////////////////////
//Returns 1,2,3 for layer 1,2,3 full modules, 11,12,13 for 1/2 modules
// 0 for fpix
// needs fedid 0-31, and channel 1-36.
//int Decode::checkLayerLink(int fed, int chan) {
//  int layer = 0;
//  if(fed<0 || fed>31) return layer;  // return 0 for invalid of fpix
//
//  if( chan>24) {   // layer 3
//
//    if(fed==0 || fed==8 )  {  // Type A
//
//      if(chan==28 ||chan==34 ||chan==35 ||chan==36 ) layer=13;  // 1/2 module
//      else layer = 3;
//
//    } else if(fed==23 || fed==31 )  {  // Type A
//
//      if(chan==27 ||chan==31 ||chan==32 ||chan==33 ) layer=13;  // 1/2 module
//      else layer = 3;
//  
//    } else if(fed==7 || fed==15 || fed==16 || fed==24 )  { // Type D
//
//      if(chan==25 ||chan==26 ||chan==29 ||chan==30 ) layer=13;
//      else layer = 3;
//
//    }  else { layer = 3;}
//
//    return layer; // layer 3
//
//  } else if( (chan>=13 && chan<=19) || chan==24 ) {
//
//    return 2; //layer 2
//
//  } else {
//
//    if(fed==0 || fed==8 || fed==16 || fed==24 )  {  // Type A   WRONG AFTER FIBER SWAP
//
//      if(chan==5 ||chan==6 ||chan==22 ||chan==23 ) layer=12; // 1/2 module 
//      else if(chan==4 ||chan==10 ||chan==11 ||chan==12 ) layer=11; // 1/2 module 
//      else layer = 1;
//  
//    } else if(fed==7 || fed==15 || fed==23 || fed==31 )  { // Type D
//
//      if(chan==1 ||chan==2 ||chan==20 ||chan==21 ) layer=12; // 1/2
//      else if(chan==3 ||chan==7 ||chan==8 ||chan==9 ) layer=11; // 1/2 
//      else layer = 1;
//
//    } else if(
//              fed==1  || fed==2  || fed==3  ||   
//              fed==9  || fed==10 || fed==11 ||   
//              fed==17 || fed==18 || fed==19 ||   
//              fed==25 || fed==26 || fed==27  )  { // Type B
//
//      if( (chan>=4 && chan<=6) || (chan>=10 && chan<=12) || (chan>=22 && chan<=23) ) layer=2;
//      else layer = 1;
//
//    } else if(
//              fed==4  || fed==5  || fed==6  ||   
//              fed==12 || fed==13 || fed==14 ||   
//              fed==20 || fed==21 || fed==22 ||   
//              fed==28 || fed==29 || fed==30  )  { // Type C
//
//      if( (chan>=1 && chan<=3) || (chan>=7 && chan<=9) || (chan>=20 && chan<=21) ) layer=2;
//      else layer = 1;
//
//    } else {
//      cout<<"unknown fed "<<fed<<endl;
//    } // if fed 
//
//    return layer;
//
//  }  // if chan
//}

//int Decode::convertToCol(int dcol, int pix) {
//  // First find if we are in the first or 2nd col of a dcol.
//  int colEvenOdd = pix%2;  // module(2), 0-1st sol, 1-2nd col.
//  // Transform
//  return (dcol * 2 + colEvenOdd); // col address, starts from 0
//
//}

//int Decode::convertToRow(int pix) {
//  return abs( int(pix/2) - 80); // row addres, starts from 0
//}

//int Decode::header(unsigned long long word64, int fed, bool print, unsigned int & bx) {
//  int fed_id=(word64>>8)&0xfff;
//  int event_id=(word64>>32)&0xffffff;
//  bx =(word64>>20)&0xfff;
////   if(bx!=101) {
////     cout<<" Header "<<" for FED "
//// 	<<fed_id<<" event "<<event_id<<" bx "<<bx<<endl;
////     int dummy=0;
////     cout<<" : ";
////     cin>>dummy;
////   }
//  if(print) cout<<"Header "<<" for FED "
//		<<fed_id<<" event "<<event_id<<" bx "<<bx<<endl;
//  fed0=-1; // reset the previous hit fed id
//  return event_id;
//}
////
//int Decode::trailer(unsigned long long word64, int fed, bool print) {
//  int slinkLength = int( (word64>>32) & 0xffffff );
//  int crc         = int( (word64&0xffff0000)>>16 );
//  int tts         = int( (word64&0xf0)>>4);
//  int slinkError  = int( (word64&0xf00)>>8);
//  if(print) cout<<"Trailer "<<" len "<<slinkLength
//		<<" tts "<<tts<<" error "<<slinkError<<" crc "<<hex<<crc<<dec<<endl;
//  return slinkLength;
//}
////
//// Decode error FIFO
//// Works for both, the error FIFO and the SLink error words. d.k. 25/04/07
//int Decode::error(int word, int & fedChannel, int fed, int & stat1, int & stat2, bool print) {
//  int status = -1;
//  print = print || printErrors;
//
//  const unsigned int  errorMask      = 0x3e00000;
//  const unsigned int  dummyMask      = 0x03600000;
//  const unsigned int  gapMask        = 0x03400000;
//  const unsigned int  timeOut        = 0x3a00000;
//  const unsigned int  timeOut2       = 0x3b00000;
//  const unsigned int  eventNumError  = 0x3e00000;
//  const unsigned int  trailError     = 0x3c00000;
//  const unsigned int  fifoError      = 0x3800000;
//
////  const unsigned int  timeOutChannelMask = 0x1f;  // channel mask for timeouts
//  //const unsigned int  eventNumMask = 0x1fe000; // event number mask
//  const unsigned int  channelMask = 0xfc000000; // channel num mask
//  const unsigned int  tbmEventMask = 0xff;    // tbm event num mask
//  const unsigned int  overflowMask = 0x100;   // data overflow
//  const unsigned int  tbmStatusMask = 0xff;   //TBM trailer info
//  const unsigned int  BlkNumMask = 0x700;   //pointer to error fifo #
//  const unsigned int  FsmErrMask = 0x600;   //pointer to FSM errors
//  const unsigned int  RocErrMask = 0x800;   //pointer to #Roc errors
//  const unsigned int  ChnFifMask = 0x1f;   //channel mask for fifo error
//  const unsigned int  Fif2NFMask = 0x40;   //mask for fifo2 NF
//  const unsigned int  TrigNFMask = 0x80;   //mask for trigger fifo NF
//  
//  //masks for diagnostic words from piggy pilot FED
//  const unsigned int  NORMask = 0x1f0000;  //number of ROCs 
//  const unsigned int  AutoResetMask = 0x80; //autoreset detected
//  const unsigned int  PKAMResetMask = 0x40; //PKAM reset detected
//  const unsigned int  TBMStackCntMask =0x1f; //TBM Stack Count
//  const unsigned int  TimeStampMask = 0xFFFFF; //Time Stamp for first ROC header & TBM trailer
//
//  const int offsets[8] = {0,4,9,13,18,22,27,31};
//  unsigned int channel = 0;
//
//  //cout<<"error word "<<hex<<word<<dec<<endl;
//  
//  if( (word&errorMask) == dummyMask ) { // DUMMY WORD
//    //cout<<" Dummy word";
//
//    if( (word&channelMask) != 0 ) { //pilot piggy tbm trailer bits
//      unsigned int chan = (word&channelMask)>>26;
//      unsigned int nor = (word&NORMask)>>16;
//      unsigned int autoreset = (word&AutoResetMask) >> 7;
//      unsigned int pkamreset = (word&PKAMResetMask) >> 6;
//      unsigned int stackCnt = (word&TBMStackCntMask);
//      if(print) 
//        cout<<" Diag ch="<<chan<<" #Rocs=" <<nor<<" resets? Auto/PKAM=" << autoreset << "/"<< pkamreset 
//	    << " TBMStackCnt=" << stackCnt << endl;
//    }
//
//    return 0;
//
//  } else if( (word&errorMask) == gapMask ) { // GAP WORD
//    //cout<<" Gap word";
//
//    if( (word&channelMask)!=0 ) { // pilot piggy timestamps first ROC header TBM trailer
//      unsigned int chan = (word&channelMask)>>26;
//      unsigned int bit20 = (word & 0x100000)>>20; // on=ROC header off=TBM trailer
//      unsigned int time = (word&TimeStampMask);
//      if(print) {
//        cout << " Chan=" << chan;
//        if(bit20==1) cout << " 1st ROC H timestamp= " ;
//        else cout << " TBM T timestamp= ";
//        cout << time << endl;
//      } //print
//    } // chan !=0
//
//    return 0;
//    
//  } else if( ((word&errorMask)==timeOut) || ((word&errorMask)==timeOut2) ) { // TIMEOUT
//
//    unsigned int bit20 =      (word & 0x100000)>>20; // works only for slink format
//
//    if(bit20 == 0) { // 2nd word 
//
//      unsigned int timeoutCnt = (word &  0x7f800)>>11; // only for slink
//      // unsigned int timeoutCnt = ((word&0xfc000000)>>24) + ((word&0x1800)>>11); // only for fifo
//      // More than 1 channel within a group can have a timeout error
//
//      unsigned int index = (word & 0x1F);  // index within a group of 4/5
//      unsigned int chip = (word& BlkNumMask)>>8;
//      int offset = offsets[chip];
//      if(print) cout<<"Timeout Error- channel: ";
//      //cout<<"Timeout Error- channel: ";
//      for(int i=0;i<5;i++) {
//	if( (index & 0x1) != 0) {
//	  channel = offset + i + 1;
//	  if(print) cout<<channel<<" ";
//	  //cout<<channel<<" ";
//	}
//	index = index >> 1;
//      }
//
//      if(print) cout << " TimeoutCount: " << timeoutCnt;
//      //cout << " TimeoutCount: " << timeoutCnt<<endl;;
//     
//     //if(print) cout<<" for Fed "<<fed<<endl;
//     status = -10;
//     fedChannel = channel;
//     //end of timeout  chip and channel decoding
//
//    } else {  // this is the 1st timout word with the baseline correction 
//   
//      int baselineCorr = 0;
//      if(word&0x200){
//	baselineCorr = -(((~word)&0x1ff) + 1);
//      } else {
//	baselineCorr = (word&0x1ff);
//      }
//
//      if(PRINT_BASELINE && print) cout<<"Timeout BaselineCorr: "<<baselineCorr<<endl;
//      //cout<<"Timeout BaselineCorr: "<<baselineCorr<<endl;
//      status = 0;
//    }
//
//
//  } else if( (word&errorMask) == eventNumError ) { // EVENT NUMBER ERROR
//    channel =  (word & channelMask) >>26;
//    unsigned int tbm_event   =  (word & tbmEventMask);
//    
//    if(print) cout<<" Event Number Error- channel: "<<channel<<" tbm event nr. "
//		  <<tbm_event<<" ";
//     status = -11;
//     fedChannel = channel;
//    
//  } else if( ((word&errorMask) == trailError)) {  // TRAILER 
//    channel =  (word & channelMask) >>26;
//    unsigned int tbm_status   =  (word & tbmStatusMask);
//    
//
//    if(tbm_status!=0) {
//      if(print) cout<<" Trailer Error- "<<"channel: "<<channel<<" TBM status:0x"
//			  <<hex<<tbm_status<<dec<<" "; // <<endl;
//     status = -15;
//     // implement the resync/reset 17
//    }
//
//    if(word & RocErrMask) {
//      if(print) cout<<"Number of Rocs Error- "<<"channel: "<<channel<<" "; // <<endl;
//     status = -12;
//    }
//
//    if(word & overflowMask) {
//      if(print) cout<<"Overflow Error- "<<"channel: "<<channel<<" "; // <<endl;
//     status = -14;
//    }
//
//    if(word & FsmErrMask) {
//      if(print) cout<<"Finite State Machine Error- "<<"channel: "<<channel
//			  <<" Error status:0x"<<hex<< ((word & FsmErrMask)>>9)<<dec<<" "; // <<endl;
//     status = -13;
//    }
//
//
//    fedChannel = channel;
//
//  } else if((word&errorMask)==fifoError) {  // FIFO
//    if(print) { 
//      if(word & Fif2NFMask) cout<<"A fifo 2 is Nearly full- ";
//      if(word & TrigNFMask) cout<<"The trigger fifo is nearly Full - ";
//      if(word & ChnFifMask) cout<<"fifo-1 is nearly full for channel"<<(word & ChnFifMask);
//      //cout<<endl;
//      status = -16;
//    }
//
//  } else {
//    if( (word == 0) && phase1 ) { 
//      // for phase1 simulations there are sometimes 0 words (fillers?)  
//      status = 0; // report OK 
//    } else {
//      cout<<" Unknown error?"<<" : ";
//      cout<<" for FED "<<fed<<" Word "<<hex<<word<<dec<<endl;
//    }
//  }
//
//  if(print && status <0) cout<<" FED "<<fed<<" status "<<status<<endl;
//  return status;
//}


/////////////////////////////////////////////////////////////////////////////
//int Decode::data(int word, int & fedChannel, int fed, int & stat1, int & stat2, bool print) {
//
//  //const int ROCMAX = 24;
//  const unsigned int plsmsk = 0xff;   // pulse height
//  const unsigned int pxlmsk = 0xff00; // pixel index
//  const unsigned int dclmsk = 0x1f0000;
//#ifdef PHASE1
//  const unsigned int rocmsk  =  0x1e00000;  // 4 bits 
//  const unsigned int chnlmsk = 0xfe000000; // 7 bits 
//  const unsigned int rocshift = 21;
//  const unsigned int linkshift = 25;
//#else
//  const unsigned int rocmsk =   0x3e00000;   // 5 bits
//  const unsigned int chnlmsk = 0xfc000000; // 6 bits 
//  const unsigned int rocshift = 21;
//  const unsigned int linkshift = 26;
//#endif
//
//  int status = 0;
//
//  roc_ = ((word&rocmsk)>>rocshift); // rocs start from 1
//  // Check for embeded special words
//  if(roc_>0 && roc_<25) {  // valid ROCs go from 1-24
//    //if(print) cout<<"data "<<hex<<word<<dec;
//    channel_ = ((word&chnlmsk)>>linkshift);
//
//    if(channel_>0 && channel_ <= n_of_Channels) {  // valid channels 1-36
//      //cout<<hex<<word<<dec;
//      dcol_=(word&dclmsk)>>16;
//      pix_=(word&pxlmsk)>>8;
//      adc_=(word&plsmsk);
//      fedChannel = channel_;
//
//      col_ = convertToCol(dcol_,pix_);
//      row_ = convertToRow(pix_);
//
//      // print the roc number according to the online 0-15 scheme
//      if(print) cout<<" Fed "<<fed<<" Channel- "<<channel_<<" ROC- "<<(roc_-1)<<" DCOL- "<<dcol_<<" Pixel- "
//		    <<pix_<<" ("<<col_<<","<<row_<<") ADC- "<<adc_<<endl;
//      status++;
//
//      if(CHECK_PIXELS) {
//
//	// Check invalid ROC numbers
//	if( ((fed>31) && (roc_>24)) || ((fed<=31) && (roc_>16))  ) {  //inv ROC
//          //if(printErrors) 
//	  cout<<" Fed "<<fed<<" wrong roc number chan/roc/dcol/pix/adc = "<<channel_<<"/"
//			      <<roc_-1<<"/"<<dcol_<<"/"<<pix_<<"/"<<adc_<<endl;
//	  status = -4;
//
//	 
//	  // protect for rerouted signals
//	} else if( fed<=31 && channel_<=24 && roc_>8 ) {
//	  if( !( (fed==6 && channel_==1) ||(fed==9 && channel_==16) ||(fed==23 && channel_==15)
//		 || (fed==31 && channel_==10) ||(fed==27 && channel_==15) )  ) {
//	    //if(printErrors) 
//	    cout<<" Fed "<<fed<<" wrong channel number, chan/roc/dcol/pix/adc = "<<channel_<<"/"
//				<<roc_-1<<"/"<<dcol_<<"/"<<pix_<<"/"<<adc_<<endl;
//	    status = -4;
//	  }
//	}
//
//
//	// Check pixels
//        if(pix_==0) {  // PIX=0
//	  // Detect pixel 0 events
//          if(printErrors) 
//	    cout<<" Fed "<<fed
//		<<" pix=0 chan/roc/dcol/pix/adc = "<<channel_<<"/"<<roc_-1<<"/"<<dcol_<<"/"
//		<<pix_<<"/"<<adc_<<" ("<<col_<<","<<row_<<")"<<endl;
//	  count0++;
//	  stat1 = roc_-1;
//	  stat2 = count0;
//	  status = -5;
//
//	} else if( fed==fed0 && channel_==chan0 && roc_==roc0 && dcol_==dcol0 && pix_==pix0 ) {
//	  // detect multiple pixels 
//
//	  count0++;
//          if(printErrors) cout<<" Fed "<<fed
//	    //cout<<" Fed "<<fed
//	      <<" double pixel  chan/roc/dcol/pix/adc = "<<channel_<<"/"<<roc_-1<<"/"<<dcol_<<"/"
//	      <<pix_<<"/"<<adc_<<" ("<<col_<<","<<row_<<") "<<count0<<endl;
//	  stat1 = roc_-1;
//	  stat2 = count0;
//	  status = -6;
//
//	} else {  // normal
//
//	  count0=0;
//
//	  fed0 = fed; chan0 =channel_; roc0 =roc_; dcol0 =dcol_; pix0=pix_;
//
//	  // Decode errors
//	  if(pix_<2 || pix_>161) {  // inv PIX
//	    if(printErrors)cout<<" Fed "<<fed<<" wrong pix number chan/roc/dcol/pix/adc = "<<channel_<<"/"
//			       <<roc_-1<<"/"<<dcol_<<"/"<<pix_<<"/"<<adc_<<" ("<<col_<<","<<row_<<")"<<endl;
//	    status = -3;
//	  }
//	  
//	  if(dcol_<0 || dcol_>25) {  // inv DCOL
//	    if(printErrors) cout<<" Fed "<<fed<<" wrong dcol number chan/roc/dcol/pix/adc = "<<channel_<<"/"
//				<<roc_-1<<"/"<<dcol_<<"/"<<pix_<<"/"<<adc_<<" ("<<col_<<","<<row_<<")"<<endl;
//	    status = -3;
//	  }
//
//	} // check pixels
//
//	  // Summary error count (for testing only)
//	if(pix_<2 || pix_>161 || dcol_<0 || dcol_>25) {
//	  countDecodeErrors2++;  // count pixels with errors 
//	  if(pix_<2 || pix_>161)  countDecodeErrors1++; // count errors
//	  if(dcol_<0 || dcol_>25) countDecodeErrors1++; // count errors
//	  //if(fed==6 && channel==35 ) cout<<" Fed "<<fed<<" wrong dcol number chan/roc/dcol/pix/adc = "<<channel<<"/"
//	  //			 <<roc-1<<"/"<<dcol<<"/"<<pix<<"/"<<adc<<" ("<<col<<","<<row<<")"<<endl;
//	}
//	
//	
//
//      }  // if CHECK_PIXELS
//
//    } else { // channel
//
//      cout<<" Wrong channel "<<channel_<<" : "
//	  <<" for FED "<<fed<<" Word "<<hex<<word<<dec<<endl;
//      return -2;
//
//    }
//
//  } else if(roc_==25) {  // ROC? 
//    unsigned int chan = ((word&chnlmsk)>>26);
//    cout<<"Wrong roc 25 "<<" in fed/chan "<<fed<<"/"<<chan<<endl;
//    status=-4;
//
//  } else {  // error word
//
//    //cout<<"error word "<<hex<<word<<dec;
//    status=error(word, fedChannel, fed, stat1, stat2, print);
//
//  }
//
//  return status;
//}
////////////////////////////////////////////////////////////////////////////

class SiPixelRawDumpPilotTree : public edm::EDAnalyzer {
public:

  /// ctor
  explicit SiPixelRawDumpPilotTree( const edm::ParameterSet& cfg);

  //explicit SiPixelRawDumpPilotTree( const edm::ParameterSet& cfg) : theConfig(cfg) {
  //consumes<FEDRawDataCollection>(theConfig.getUntrackedParameter<std::string>("InputLabel","source"));} 

  /// dtor
  virtual ~SiPixelRawDumpPilotTree() {}

  void beginJob();

  //void beginRun( const edm::EventSetup& ) {}

  // end of job 
  void endJob();

  /// get data, convert to digis attach againe to Event
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

private:
  edm::ParameterSet theConfig;
  edm::EDGetTokenT<FEDRawDataCollection> rawData;

  int printLocal;
  double printThreshold;
  int countEvents, countAllEvents;
  int countTotErrors;
  float sumPixels, sumFedSize, sumFedPixels[n_of_FEDs];
  int decodeErrors[n_of_FEDs][n_of_Channels];
  int decodeErrors000[n_of_FEDs][n_of_Channels];  // pix 0 problem
  int decodeErrorsDouble[n_of_FEDs][n_of_Channels];  // double pix  problem
  int errorType[20];

  TFile* file;

  TTree* success_tree;
  Int_t success_adc;
  Int_t success_layer;
  Int_t success_channel;
  Int_t success_fed_id;
  Int_t success_fed_id0;
  Int_t success_event;

  TTree* event_tree;
  Int_t event_run;
  Int_t event_event;
  Int_t event_lumi;
  Int_t event_bx;
  Int_t event_orbit;
  Int_t event_countPixels;
  Int_t event_countErrors;
  Int_t event_isWrongBX;
  Float_t event_aveFedSize;
  
  TTree* error_tree;
  Int_t error_status;
  Int_t error_fed_channel;
  Int_t error_fed_id;
  Int_t error_fed_id0;
  Int_t error_lumi; 
  Int_t error_layer;
  Int_t error_bxid;
  Int_t error_event;

  TTree* fed_tree;
  Float_t fed_buffersize;
  Int_t fed_id;
  Int_t fed_id0;
  Int_t fed_id_bpixmax;
  Int_t fed_ncounts;
  Int_t fed_nerrors;
  Int_t fed_bx;
  Int_t fed_bxid;
  Float_t fed_lumi;

  TH1D *hpixels4;

  Decode decode;
};
//----------------------------------------------------------------------------------
SiPixelRawDumpPilotTree::SiPixelRawDumpPilotTree( const edm::ParameterSet& cfg) : theConfig(cfg) {
  string label = theConfig.getUntrackedParameter<std::string>("InputLabel","source");
  // For the ByToken method
  rawData = consumes<FEDRawDataCollection>(label);
} 
//----------------------------------------------------------------------------------------
void SiPixelRawDumpPilotTree::endJob() {
  string errorName[18] = {" "," ","wrong channel","wrong pix or dcol","wrong roc","pix=0",
			  " double-pix"," "," "," ","timeout","ENE","NOR","FSM","overflow",
			  "trailer","fifo","reset/resync"};

// 2 - wrong channel
// 3 - wrong pix or dcol 
// 4 - wrong roc
// 5 - pix=0
// 6 - double pixel 
  // 10 - timeout ()
  // 11 - ene ()
  // 12 - mum pf rocs error ()
  // 13 - fsm ()
  // 14 - overflow ()
  // 15 - trailer ()
  // 16 - fifo  (30)
  // 17 - reset/resync NOT INCLUDED YET

  double tmp = sumPixels;
  if(countEvents>0) {
    sumPixels /= float(countEvents);
    sumFedSize /= float(countAllEvents);
    for(int i = 0; i < n_of_FEDs; ++i) {
      sumFedPixels[i] /= float(countEvents);
      hpixels4->Fill(float(i),sumFedPixels[i]); //pixels only 
    }
  }
    
  cout<<" Total/non-empty events " <<countAllEvents<<" / "<<countEvents<<" average number of pixels "<<sumPixels<<" ("<<tmp<<")"<<endl;

  cout<<" Average Fed size per event for all events (in 4-words) "<< (sumFedSize*2./static_cast<float>(n_of_FEDs)) 
      <<" total for all feds "<<(sumFedSize*2.) <<endl;

  cout<<" Size for ech FED per event in units of hit pixels:" <<endl;

  for(int i = 0; i < n_of_FEDs; ++i) cout<< sumFedPixels[i]<<" ";
  cout<<endl;

  cout<<" Total number of errors "<<countTotErrors<<" print threshold "<< int(countEvents*printThreshold) << " total errors per fed channel"<<endl;
  cout<<" FED errors "<<endl<<"Fed Channel Tot-Errors ENE-Errors Time-Errors Over-Errors"<<endl;

  cout<<" Decode errors "<<endl<<"Fed Channel Errors Pix_000 Double_Pix"<<endl;
  for(int i = 0; i < n_of_FEDs; ++i) {
    for(int j=0;j<n_of_Channels;++j) {
      int tmp = decodeErrors[i][j] + decodeErrors000[i][j] + decodeErrorsDouble[i][j];
      if(tmp>10) 
	cout<<" "<<i<<" -  "<<(j+1)<<"   -  "
	    <<decodeErrors[i][j]<<"  -    "
	    <<decodeErrors000[i][j]<<"  -   "
	    <<decodeErrorsDouble[i][j]<<endl;
    }
  }

  cout<<" Total errors for all feds "<<endl<<" Type Name Num-Of-Errors"<<endl;
  for(int i=0;i<20;++i) {
    if( errorType[i]>0 ) cout<<"   "<<i<<" - "<<errorName[i]<<" - "<<errorType[i]<<endl;
  }

  cout<<" Test decode errors "<<countDecodeErrors1<<" "<<countDecodeErrors2<<endl;


  file->Write();
  file->Close();


}
//----------------------------------------------------------------------
void SiPixelRawDumpPilotTree::beginJob() {

  printLocal = theConfig.getUntrackedParameter<int>("Verbosity",1);
  printThreshold = theConfig.getUntrackedParameter<double>("PrintThreshold",0.001); // threshold per event for printing errors
  cout<<" beginjob "<<printLocal<<" "<<printThreshold<<endl;  

  if(printLocal>0) printErrors  = true;
  else printErrors = false;
  if(printLocal>1) printData  = true;
  else printData = false;
  if(printLocal>2) printHeaders  = true;
  else printHeaders = false;

  countEvents=0;
  countAllEvents=0;
  countTotErrors=0;
  sumPixels=0.;
  sumFedSize=0;
  for(int i = 0; i < n_of_FEDs; ++i) {
    sumFedPixels[i]=0;
    for(int j=0;j<n_of_Channels;++j) {decodeErrors[i][j]=0; decodeErrors000[i][j]=0; decodeErrorsDouble[i][j]=0;}
  }
  for(int i=0;i<20;++i) errorType[i]=0;

  edm::Service<TFileService> fs;


  file = new TFile("Myroot.root","recreate");

  success_tree = new TTree("success_tree","success_tree");
  success_tree->Branch("success_adc",&success_adc,"success_adc/I"); 
  success_tree->Branch("success_layer",&success_layer,"success_layer/I"); 
  success_tree->Branch("success_channel",&success_channel,"success_channel/I"); 
  success_tree->Branch("success_fed_id",&success_fed_id,"success_fed_id/I"); 
  success_tree->Branch("success_fed_id0",&success_fed_id0,"success_fed_id0/I"); 
  success_tree->Branch("success_event",&success_event,"success_event/I"); 

  error_tree = new TTree("error_tree","error_tree");
  error_tree->Branch("error_status",&error_status,"error_status/I"); 
  error_tree->Branch("error_fed_channel",&error_fed_channel,"error_fed_channel/I"); 
  error_tree->Branch("error_fed_id",&error_fed_id,"error_fed_id/I"); 
  error_tree->Branch("error_fed_id0",&error_fed_id0,"error_fed_id0/I"); 
  error_tree->Branch("error_lumi",&error_lumi,"error_lumi/I"); 
  error_tree->Branch("error_layer",&error_layer,"error_layer/I"); 
  error_tree->Branch("error_bxid",&error_bxid,"error_bxid/I"); 
  error_tree->Branch("error_event",&error_event,"error_event/I"); 

  fed_tree = new TTree("fed_tree","fed_tree");
  fed_tree->Branch("fed_buffersize",&fed_buffersize,"fed_buffersize/F"); 
  fed_tree->Branch("fed_id",&fed_id,"fed_id/I"); 
  fed_tree->Branch("fed_id0",&fed_id0,"fed_id0/I"); 
  fed_tree->Branch("fed_id_bpixmax",&fed_id_bpixmax,"fed_id_bpixmax/I"); 
  fed_tree->Branch("fed_ncounts",&fed_ncounts,"fed_ncounts/I"); 
  fed_tree->Branch("fed_nerrors",&fed_nerrors,"fed_nerrors/I"); 
  fed_tree->Branch("fed_lumi",&fed_lumi,"fed_lumi/F"); 
  fed_tree->Branch("fed_bx",&fed_bx,"fed_bx/I"); 
  fed_tree->Branch("fed_bxid",&fed_bxid,"fed_bxid/I"); 
  
  event_tree = new TTree("event_tree","event_tree");
  event_tree->Branch("event_run",&event_run,"event_run/I"); 
  event_tree->Branch("event_event",&event_event,"event_event/I"); 
  event_tree->Branch("event_lumi",&event_lumi,"event_lumi/I"); 
  event_tree->Branch("event_bx",&event_bx,"event_bx/I"); 
  event_tree->Branch("event_orbit",&event_orbit,"event_orbit/I"); 
  event_tree->Branch("event_countPixels",&event_countPixels,"event_countPixels/I"); 
  event_tree->Branch("event_countErrors",&event_countErrors,"event_countErrors/I"); 
  event_tree->Branch("event_isWrongBX",&event_isWrongBX,"event_isWrongBX/I"); 
  event_tree->Branch("event_aveFedSize",&event_aveFedSize,"event_aveFedSize/F"); 


  hpixels4 = fs->make<TH1D>( "hpixels4", "pixels per each FED", n_of_FEDs, -0.5, static_cast<float>(n_of_FEDs) - 0.5);

}
//-----------------------------------------------------------------------
void SiPixelRawDumpPilotTree::analyze(const  edm::Event& ev, const edm::EventSetup& es) {
  const bool printEventInfo = false;

  // Access event information
  int run       = ev.id().run();
  int event     = ev.id().event();
  int lumiBlock = ev.luminosityBlock();
  int bx        = ev.bunchCrossing();
  int orbit     = ev.orbitNumber();
  static int oldEvent=0; // ,LS=0;
  
  countAllEvents++;

  if(printEventInfo) {
    // if(countAllEvents<100 || (countAllEvents&1000)==0 )
    //   cout<<" Run "<<run<<" LS "<<lumiBlock<<" event "<<event<<" bx "<<bx
    // 	  <<" all "<<countAllEvents<<endl;
    //if( lumiBlock != LS) {
      cout<<" Run "<<run<<" LS "<<lumiBlock<<" event "<<event<<" bx "<<bx
	  <<" all "<<countAllEvents<<endl;
      //LS=lumiBlock;
      //}
      //if(event<oldEvent) 
      cout<<"   Lower event number: Run "<<run<<" LS "<<lumiBlock<<" event "<<event<<" bx "<<bx
	  <<" all "<<countAllEvents<<" "<<oldEvent<<endl;
    //oldEvent=event;
    //return; // skip the rest 
  }

  if(printHeaders) 
    cout<<"Event = "<<countAllEvents<<" Event number "<<event<<" Run "<<run
	<<" LS "<<lumiBlock<<endl;
  

  // Get lumi info (does not work for raw)
//  edm::LuminosityBlock const& iLumi = ev.getLuminosityBlock();
//  edm::Handle<LumiSummary> lumi;
//  iLumi.getByLabel("lumiProducer", lumi);
//  edm::Handle<edm::ConditionsInLumiBlock> cond;
//  float intlumi = 0, instlumi=0;
//  int beamint1=0, beamint2=0;
//  iLumi.getByLabel("conditionsInEdm", cond);
//  // This will only work when running on RECO until (if) they fix it in the FW
//  // When running on RAW and reconstructing, the LumiSummary will not appear
//  // in the event before reaching endLuminosityBlock(). Therefore, it is not
//  // possible to get this info in the event
//  if (lumi.isValid()) {
//    intlumi =(lumi->intgRecLumi())/1000.; // integrated lumi per LS in -pb
//    instlumi=(lumi->avgInsDelLumi())/1000.; //ave. inst lumi per LS in -pb
//    beamint1=(cond->totalIntensityBeam1)/1000;
//    beamint2=(cond->totalIntensityBeam2)/1000;
//  } else {
//    std::cout << "** ERROR: Event does not get lumi info\n";
//  }
//  cout<<instlumi<<" "<<intlumi<<" "<<lumiBlock<<endl;



  edm::Handle<FEDRawDataCollection> buffers;
  //static std::string label = theConfig.getUntrackedParameter<std::string>("InputLabel","source");
  //static std::string instance = theConfig.getUntrackedParameter<std::string>("InputInstance","");  
  //ev.getByLabel( label, instance, buffers);
  ev.getByToken(rawData , buffers);  // the new bytoken 

#ifdef PHASE1
  std::pair<int,int> fedIds(1200,1255); // phase 1
  const int fedId0=1200;
#else // phase0
  std::pair<int,int> fedIds(FEDNumbering::MINSiPixelFEDID, FEDNumbering::MAXSiPixelFEDID); //0
  const int fedId0=0;
#endif


  //PixelDataFormatter formatter(0);  // only for digis
  //bool dummyErrorBool;

  //typedef unsigned int Word32;
  //typedef long long Word64;
  typedef uint32_t Word32;
  typedef uint64_t Word64;
  int status=0;
  int countPixels=0;
  int eventId = -1;
  int countErrorsPerEvent=0;
  double aveFedSize = 0.;
  int stat1=-1, stat2=-1;
  int fedchannelsize[n_of_Channels];
  bool wrongBX=false;


  // Loop over FEDs
  for (int fedId = fedIds.first; fedId <= fedIds.second; fedId++) {

    //edm::DetSetVector<PixelDigi> collection;
    PixelDataFormatter::Errors errors;

    //get event data for this fed
    const FEDRawData& rawData = buffers->FEDData( fedId );

    if(rawData.size()==0) continue;  // skip if not data for this fed

    if(printHeaders) cout<<"Get data For FED = "<<fedId<<" size in bytes "<<rawData.size()<<endl;

    for(int i=0;i<n_of_Channels;++i) fedchannelsize[i]=0;

    int nWords = rawData.size()/sizeof(Word64);
    //cout<<" size "<<nWords<<endl;

    sumFedSize += float(nWords);    
    if(fedId<32) aveFedSize += double(2.*nWords);


    // check headers
    const Word64* header = reinterpret_cast<const Word64* >(rawData.data()); 
    //cout<<hex<<*header<<dec<<endl;

    unsigned int bxid = 0;
    eventId = Decode::header(*header, fedId, printHeaders, bxid);
    //if(fedId = fedIds.first) 

    //    std::cout << "BX : " << bx << " " << bxid << std::endl;
    if(bx != int(bxid) ) { 
      wrongBX=true;
      if(printErrors && printBX && !phase1) 
	cout<<" Inconsistent BX: for event "<<event<<" (fed-header event "<<eventId<<") for LS "<<lumiBlock
	    <<" for run "<<run<<" for bx "<<bx<<" fed bx "<<bxid<<endl;
    }

    const Word64* trailer = reinterpret_cast<const Word64* >(rawData.data())+(nWords-1);
    //cout<<hex<<*trailer<<dec<<endl;
    status = Decode::trailer(*trailer,fedId, printHeaders);

    int countPixelsInFed=0;
    int countErrorsInFed=0;
    int fedChannel = 0;
    int num=0;

    // Loop over payload words
    for (const Word64* word = header+1; word != trailer; word++) {
      static const Word64 WORD32_mask  = 0xffffffff;

      for(int ipart=0;ipart<2;++ipart) {
	Word32 w = 0;
	if(ipart==0) {
	  w =  *word       & WORD32_mask;  // 1st word

	} else if(ipart==1) {
	  w =  *word >> 32 & WORD32_mask;  // 2nd word
	}

	num++;
	if(printLocal>3) cout<<" "<<num<<" "<<hex<<w<<dec<<endl;

	//status = Decode::data(w,fedChannel, fedId, stat1, stat2, printData);
	status = decode.data(w,fedChannel, fedId, stat1, stat2, printData);
	int layer = Decode::checkLayerLink(fedId, fedChannel); // get bpix layer, works only for phase0 
	if(phase1) layer=0;
	if(layer>10) layer = layer-10; // ignore 1/2 modules 


	if(status>0) {  // data
	  countPixels++;
	  countPixelsInFed++;
	  fedchannelsize[fedChannel-1]++;
	  
	  int adc = decode.get_adc();

	  success_adc = adc;
	  success_layer = layer;
	  success_channel = fedChannel;
	  success_fed_id = fedId;
	  success_fed_id0 = fedId0;
	  success_event = event;
	  success_tree->Fill();


	} else if(status<0) {  // error word
	  countErrorsInFed++;
	  //if( status == -6 || status == -5) 
	  if(printErrors) cout<<"    Bad stats for FED "<<fedId<<" Event "<<eventId<<"/"<<(eventId%256)
			      <<" count "<<countAllEvents<<" chan "<<fedChannel<<" status "<<status<<endl;
	  status=abs(status);
	  // 2 - wrong channel
	  // 3 - wrong pix or dcol 
	  // 4 - wrong roc
	  // 5 - pix=0
	  // 6 - double pixel
	  // 10 - timeout ()
	  // 11 - ene ()
	  // 12 - mum pf rocs error ()
	  // 13 - fsm ()
	  // 14 - overflow ()
	  // 15 - trailer ()
	  // 16 - fifo  (30)
	  // 17 - reset/resync NOT INCLUDED YET
	  
	  error_status = status; 
	  error_fed_channel = fedChannel;
	  error_fed_id = fedId;
	  error_fed_id0 = fedId0;
	  error_lumi = lumiBlock;
	  error_layer = layer;
	  error_bxid = bxid;
	  error_event = event;
	  error_tree->Fill();
  
	  if(status<20) errorType[status]++;

	  
	}
      } // for  1/2 word

    } // loop over longlong  words

    countTotErrors += countErrorsInFed;
    countErrorsPerEvent += countErrorsInFed;

    //convert data to digi (dummy for the moment)
    //formatter.interpretRawData( dummyErrorBool, fedId, rawData, collection, errors);
    //cout<<dummyErrorBool<<" "<<digis.size()<<" "<<errors.size()<<endl;

    if(countPixelsInFed>0)  {
      sumFedPixels[fedId-fedId0] += countPixelsInFed;
    }

    

    // Fill in to the trees
    fed_buffersize = 2*nWords;
    fed_id = fedId;
    fed_id0 = fedId0;
    fed_id_bpixmax = fedIdBpixMax;
    fed_lumi = lumiBlock;
    fed_ncounts = countPixelsInFed;
    fed_nerrors = countErrorsInFed;
    fed_bx = bx;
    fed_bxid = bxid;
    fed_tree->Fill();

  } // loop over feds



  if(wrongBX && printBX && !phase1) {
    cout<<" Inconsistent BX: for event "<<event<<" (fed-header event "<<eventId<<") for LS "<<lumiBlock
	<<" for run "<<run<<" for bx "<<bx<<" pix= "<<countPixels<<endl;
  }


  aveFedSize /= 32.;

  if(countPixels>0) {
    sumPixels += countPixels;
    countEvents++;
  } 



  event_run = run;
  event_event = event;
  event_lumi = lumiBlock;
  event_bx = bx;
  event_orbit = orbit;
  event_countPixels = countPixels;
  event_countErrors = countErrorsPerEvent;
  event_isWrongBX = (Int_t)wrongBX;
  event_aveFedSize = aveFedSize;
  event_tree->Fill();


 
} // end analyze

// 2 - wrong channel
// 4 - wrong roc
// 3 - wrong pix or dcol 
// 5 - pix=0
// 6 - double pix
  // 10 - timeout ()
  // 11 - ene ()
  // 12 - mum pf rocs error ()
  // 13 - fsm ()
  // 14 - overflow ()
  // 15 - trailer ()
  // 16 - fifo  (30)
  // 17 - reset/resync NOT INCLUDED YET

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SiPixelRawDumpPilotTree);
