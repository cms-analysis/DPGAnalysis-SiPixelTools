/** \class PixelFedErrorDumper
 *  Plug-in module that dump fed error data 
 *  for pixel subdetector
 * Works for CMSSW7X (bytoken)d.k.
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "EventFilter/SiPixelRawToDigi/interface/PixelDataFormatter.h"

// for detids 
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/DetId/interface/DetId.h"

// geometry and topology
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"


// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// For ROOT
#include <TROOT.h>
//#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>


#include <iostream>
using namespace std;


// Include the helper decoding class
/////////////////////////////////////////////////////////////////////////////
class MyDecode2 {
public:
  MyDecode2() {}
  ~MyDecode2() {}
  static int error(int error, int & fedChannel, int fed, bool print=false);
  static int data(int error, int & fedChannel, int fed, int layer, bool print=false);
  static int header(unsigned long long word64, int fed, bool print);
  static int trailer(unsigned long long word64, int fed, bool print);
private:
};
/////////////////////////////////////////////////////////////////////////////
int MyDecode2::header(unsigned long long word64, int fed, bool print) {
  int fed_id=(word64>>8)&0xfff;
  int event_id=(word64>>32)&0xffffff;
  unsigned int bx_id=(word64>>20)&0xfff;
//   if(bx_id!=101) {
//     cout<<" Header "<<" for FED "
// 	<<fed_id<<" event "<<event_id<<" bx "<<bx_id<<endl;
//     int dummy=0;
//     cout<<" : ";
//     cin>>dummy;
//   }
  if(print) cout<<" Header "<<" for FED "
		<<fed_id<<" event "<<event_id<<" bx "<<bx_id<<endl;

  return event_id;
}
//
int MyDecode2::trailer(unsigned long long word64, int fed, bool print) {
  int slinkLength = int( (word64>>32) & 0xffffff );
  int crc         = int( (word64&0xffff0000)>>16 );
  int tts         = int( (word64&0xf0)>>4);
  int slinkError  = int( (word64&0xf00)>>8);
  if(print) cout<<" Trailer "<<" len "<<slinkLength
		<<" tts "<<tts<<" error "<<slinkError<<" crc "<<hex<<crc<<dec<<endl;
  return slinkLength;
}
//
// Decode error FIFO
// Works for both, the error FIFO and the SLink error words. d.k. for phase1 6/17
int MyDecode2::error(int word, int & fedChannel, int fed, bool print) {
  int status = -1;
  bool PRINT_BASELINE = false;

  //print = print || printErrors;

  const unsigned int  errorMask      = 0x3e00000;
  const unsigned int  dummyMask      = 0x03600000;
  const unsigned int  gapMask        = 0x03400000;
  const unsigned int  timeOut        = 0x3a00000;
  const unsigned int  timeOut2       = 0x3b00000;
  const unsigned int  eventNumError  = 0x3e00000;
  const unsigned int  trailError     = 0x3c00000;
  const unsigned int  fifoError      = 0x3800000;

//  const unsigned int  timeOutChannelMask = 0x1f;  // channel mask for timeouts
  //const unsigned int  eventNumMask = 0x1fe000; // event number mask
  const unsigned int  channelMask = 0xfc000000; // channel num mask
  const unsigned int  tbmEventMask = 0xff;    // tbm event num mask
  const unsigned int  overflowMask = 0x100;   // data overflow
  const unsigned int  tbmStatusMask = 0xff;   //TBM trailer info
  const unsigned int  BlkNumMask = 0x700;   //pointer to error fifo #
  const unsigned int  FsmErrMask = 0x600;   //pointer to FSM errors
  const unsigned int  RocErrMask = 0x800;   //pointer to #Roc errors
  const unsigned int  ChnFifMask = 0x1f;   //channel mask for fifo error
  const unsigned int  Fif2NFMask = 0x40;   //mask for fifo2 NF
  const unsigned int  TrigNFMask = 0x80;   //mask for trigger fifo NF
  
  const int offsets[8] = {0,4,9,13,18,22,27,31};
  unsigned int channel = 0;

  //cout<<"error word "<<hex<<word<<dec<<endl;
  
  if( (word&errorMask) == dummyMask ) { // DUMMY WORD
    //cout<<" Dummy word";
    return 0;

  } else if( (word&errorMask) == gapMask ) { // GAP WORD
    //cout<<" Gap word";
    return 0;
    
  } else if( ((word&errorMask)==timeOut) || ((word&errorMask)==timeOut2) ) { // TIMEOUT

    unsigned int bit20 =      (word & 0x100000)>>20; // works only for slink format

    if(bit20 == 0) { // 2nd word 

      unsigned int timeoutCnt = (word &  0x7f800)>>11; // only for slink
      // unsigned int timeoutCnt = ((word&0xfc000000)>>24) + ((word&0x1800)>>11); // only for fifo
      // More than 1 channel within a group can have a timeout error

      unsigned int index = (word & 0x1F);  // index within a group of 4/5
      unsigned int chip = (word& BlkNumMask)>>8;
      int offset = offsets[chip];
      if(print) cout<<"Timeout Error- channel: ";
      //cout<<"Timeout Error- channel: ";
      for(int i=0;i<5;i++) {
	if( (index & 0x1) != 0) {
	  channel = offset + i + 1;
	  if(print) cout<<channel<<" ";
	  //cout<<channel<<" ";
	}
	index = index >> 1;
      }

      if(print) cout << " TimeoutCount: " << timeoutCnt;
      //cout << " TimeoutCount: " << timeoutCnt<<endl;;
     
     //if(print) cout<<" for Fed "<<fed<<endl;
     status = -10;
     fedChannel = channel;
     //end of timeout  chip and channel decoding

    } else {  // this is the 1st timout word with the baseline correction 
   
      int baselineCorr = 0;
      if(word&0x200){
	baselineCorr = -(((~word)&0x1ff) + 1);
      } else {
	baselineCorr = (word&0x1ff);
      }

      if(PRINT_BASELINE && print) cout<<"Timeout BaselineCorr: "<<baselineCorr<<endl;
      //cout<<"Timeout BaselineCorr: "<<baselineCorr<<endl;
      status = 0;
    }


  } else if( (word&errorMask) == eventNumError ) { // EVENT NUMBER ERROR
    channel =  (word & channelMask) >>26;
    unsigned int tbm_event   =  (word & tbmEventMask);
    
    if(print) cout<<" Event Number Error- channel: "<<channel<<" tbm event nr. "
		  <<tbm_event<<" ";
     status = -11;
     fedChannel = channel;
    
  } else if( ((word&errorMask) == trailError)) {  // TRAILER 
    channel =  (word & channelMask) >>26;
    unsigned int tbm_status   =  (word & tbmStatusMask);
    

    if(tbm_status!=0) {
      if(print) cout<<" Trailer Error- "<<"channel: "<<channel<<" TBM status:0x"
			  <<hex<<tbm_status<<dec<<" "; // <<endl;
     status = -15;
     // implement the resync/reset 17
    }

    if(word & RocErrMask) {
      if(print) cout<<"Number of Rocs Error- "<<"channel: "<<channel<<" "; // <<endl;
     status = -12;
    }

    if(word & overflowMask) {
      if(print) cout<<"Overflow Error- "<<"channel: "<<channel<<" "; // <<endl;
     status = -14;
    }

    if(word & FsmErrMask) {
      if(print) cout<<"Finite State Machine Error- "<<"channel: "<<channel
			  <<" Error status:0x"<<hex<< ((word & FsmErrMask)>>9)<<dec<<" "; // <<endl;
     status = -13;
    }


    fedChannel = channel;

  } else if((word&errorMask)==fifoError) {  // FIFO
    if(print) { 
      if(word & Fif2NFMask) cout<<"A fifo 2 is Nearly full- ";
      if(word & TrigNFMask) cout<<"The trigger fifo is nearly Full - ";
      if(word & ChnFifMask) cout<<"fifo-1 is nearly full for channel"<<(word & ChnFifMask);
      //cout<<endl;
      status = -16;
    }

  } else {
    if( (word == 0) ) { 
      // for phase1 simulations there are sometimes 0 words (fillers?)  
      status = 0; // report OK 
    } else {
      cout<<" Unknown error?"<<" : ";
      cout<<" for FED "<<fed<<" Word "<<hex<<word<<dec<<endl;
    }
  }

  if(print && status <0) cout<<" FED "<<fed<<" status "<<status<<endl;
  return status;
}

///////////////////////////////////////////////////////////////////////////
int MyDecode2::data(int word, int & fedChannel, int fed, int layer, bool print) {
  const bool CHECK_PIXELS = true;
  const int ROCMAX = 24;
  const unsigned int plsmsk = 0xff;   // pulse height
  const unsigned int pxlmsk = 0xff00; // pixel index
  const unsigned int dclmsk = 0x1f0000;
  const unsigned int rocmsk = 0x3e00000;
  const unsigned int chnlmsk = 0xfc000000;
  const unsigned int rocshift = 21;
  const unsigned int linkshift = 26;

  // for layer-1
  const unsigned int rowmsk = 0x7f00; 
  const unsigned int colmsk = 0x1f8000;

  int status = 0;

  int roc = ((word&rocmsk)>>rocshift); // rocs start from 1
  // Check for embeded special words
  if(roc>0 && roc<=ROCMAX) {  // valid ROCs go from 1-24
    //if(print) cout<<"data "<<hex<<word<<dec;
    int channel = ((word&chnlmsk)>>linkshift);
    fedChannel = channel;

    if(channel>0 && channel<=48) {  // valid channels 1-48
      //cout<<hex<<word<<dec;
      int dcol=(word&dclmsk)>>16;
      int pix=(word&pxlmsk)>>8;
      int adc=(word&plsmsk);
      //int col = convertToCol(dcol,pix);
      //int row  = convertToRow(pix);

      // for L1 
      int col1=(word&colmsk)>>15;
      int row1=(word&rowmsk)>>8;
      
      // test the unfold, make sure that we can get the l1 col/row
      int tmp = (dcol<<16) + (pix<<8);
      int col11 = (tmp&colmsk)>>15;
      int row11 = (tmp&rowmsk)>>8;
      if(col1!=col11) cout<<" col not equal "<<col1<<" "<<col11<<endl;
      if(row1!=row11) cout<<" row not equal "<<row1<<" "<<row11<<endl;

      // will need here a loopup table which can distuinguish layer 1 links
      // to decide if we read dcol/pis or col/row
      if(print) 
	cout<<" Channel- "<<channel<<" ROC- "<<(roc-1)
	    <<" DCOL- "<<dcol<<" Pixel- "<<pix
	    <<" (OR for L1 col-"<<col1<<" row-"<<row1
	    <<") ADC- "<<adc<<endl;
      

      if(CHECK_PIXELS) {

	if(layer==1) {
	  //cout<<" this is layer 1"<<endl;

	  if(col1<0 || col1>51) {
	    if(print) cout<<" Fed "<<fed<<", for layer1 wrong col, chan/roc/col/row/adc = "
			  <<channel<<"/"<<roc<<"/"<<col1<<"/"<<row1<<"/"<<adc<<endl;
	    status = -3;
	  }
	  
	  if(row1<0 || row1>79) {
	    if(print) 
	      cout<<" Fed "<<fed<<", for layer1 wrong row number chan/roc/col/row/adc = "
		  <<channel<<"/"<<roc<<"/"<<col1<<"/"<<row1<<"/"<<adc<<endl;
	    status = -3;
	  }


	} else { // all other layers 

	  if(dcol<0 || dcol>25) {
	    if(print) cout<<" Fed "<<fed<<", wrong dcol number chan/roc/dcol/pix/adc = "<<channel<<"/"<<roc<<"/"<<dcol<<"/"<<pix<<"/"<<adc<<endl;
	    status = -3;
	  }
	  
	  if(pix<2 || pix>181) {
	    if(print) 
	      cout<<" Fed "<<fed<<", wrong pix number chan/roc/dcol/pix/adc = "
		  <<channel<<"/"<<roc<<"/"<<dcol<<"/"<<pix<<"/"<<adc<<endl;
	    status = -3;
	  }
	  
	} // if layer 


	if(roc>7) {	  
	  if(print) 
	    cout<<" Fed "<<fed<<" wrong roc number chan/roc/dcol/pix/adc = "<<channel<<"/"<<roc<<"/"
		<<dcol<<"/"<<pix<<"/"<<adc<<endl;
	  status = -4;
	}

        if(pix==0) {
          if(print) cout<<" Fed "<<fed<<" pix=0 chan/roc/dcol/pix/adc = "<<channel<<"/"
                        <<roc<<"/"<<dcol<<"/"<<pix<<"/"<<adc<<endl;
	  status = -5;
	}
      } // IF CHECK ERRORS

      status++;	

    } else { // channel

      cout<<" Wrong channel "<<channel<<" : "
	  <<" Word "<<hex<<word<<dec<<endl;
      return -2;

    }

} else if(roc==25) {  // ROC? 
    unsigned int chan = ((word&chnlmsk)>>26);
    cout<<"Wrong roc 25 "<<" in chan "<<"/"<<chan<<endl;
    status=-4;

 } else {  // error word

  //cout<<"error word "<<hex<<word<<dec;
  status=error(word, fedChannel, fed,print);
  
 }

return status;
}

////////////////////////////////////////////////////////////////////////////

class PixelFedErrorDumper : public edm::EDAnalyzer {
public:

  /// ctor
  explicit PixelFedErrorDumper( const edm::ParameterSet& cfg); 

  /// dtor
  virtual ~PixelFedErrorDumper() {}

  void beginJob();

  //void beginRun( const edm::EventSetup& ) {}

  // end of job 
  void endJob();

  /// get data, convert to digis attach againe to Event
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  int analyzeFedErrors(int errorType, int FedId, int fedChannel, uint32_t word32);
  int analyzeConversionErrors(int errorType, int FedId, int fedChannel, uint32_t word32);

private:
  edm::ParameterSet theConfig;
  edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError> > fedErrorContainer;
  bool PRINT;
  //int countEvents, countAllEvents;
  int fedErrors, moduleErrors, spareCounts;
  //float sumPixels, sumFedSize, sumFedPixels[40];
  //int fedErrors[40][36];
  //int decodeErrors[40][36];
  //int decodeErrors000[40][36];  // pix 0 problem
  int countErrors[40], countErrors2[40];
  bool phase1_;

  TH1F *hfeds, *hfedsF, *hfedsSlink, *hfedsCRC, *hfedsUnknown;
  TH1F *herrors, *herrorsF, *htype;
  TH1F *hmode;
  TH1F *htbm;
  TH2F *hfedErrors0,*hfedErrors1,*hfedErrors2,*hfedErrors3,*hfedErrors4,*hfedErrors5,
    *hfedErrors6,*hfedErrors7,*hfedErrors8,*hfedErrors9;
  TH2F *hfedErrors0F, *hfed2d,*hfed2d0;

  TH1F *hlumi, *hbx;

};
//----------------------------------
PixelFedErrorDumper::PixelFedErrorDumper( const edm::ParameterSet& cfg) : theConfig(cfg) {

  //std::string src_ = theConfig.getUntrackedParameter<std::string>("InputLabel","source");
  std::string src = theConfig.getUntrackedParameter<std::string>("InputLabel","siPixelDigis");

  // For the ByToken method
  fedErrorContainer = consumes<edm::DetSetVector<SiPixelRawDataError> >(src);
  phase1_ = true;
} 

//---------------------------------
void PixelFedErrorDumper::endJob() {
  cout<<" fed-errors "<<fedErrors<<" module-Errors "<<moduleErrors<<" "<<spareCounts<<endl;
  for(int i=0;i<40;++i) {
    if(countErrors[i]>0 || countErrors2[i]>0 ) cout<<i<<" "<<countErrors[i]<<" "<<countErrors2[i]<<endl;
  }

}

void PixelFedErrorDumper::beginJob() {
  //countEvents=0;
  //countAllEvents=0;
  //countErrors=0;
  fedErrors=0;
  moduleErrors=0;
  spareCounts=0.;
  //sumFedSize=0;  

  PRINT = theConfig.getUntrackedParameter<bool>("Verbosity",false);
  for(int i=0;i<40;++i) {
    countErrors[i]=0;
    countErrors2[i]=0;
  }

//   for(int i=0;i<40;++i) {
//     sumFedPixels[i]=0;
//     for(int j=0;j<36;++j) fedErrors[i][j]=0;
//     for(int j=0;j<36;++j) decodeErrors[i][j]=0;
//     for(int j=0;j<36;++j) decodeErrors000[i][j]=0;
//   }
//   for(int i=0;i<20;++i) errorType[i]=0;

  edm::Service<TFileService> fs;

  htbm  = fs->make<TH1F>("htbm", "tbm errors ",256, -0.5, 255.5);
  //htbmF = fs->make<TH1F>("htbmF","tbm errors ",256, -0.5, 255.5);

  hmode  = fs->make<TH1F>( "hmode", "mode",16, -0.5, 15.5);
  //hmodeF  = fs->make<TH1F>("hmodeF","mode",16, -0.5, 15.5);

  int num_feds=140;
  float minFed=1200.;
  float maxFed=1340.;


  hfeds  = fs->make<TH1F>( "hfeds", "fed errors per FED",         num_feds,minFed,maxFed);
  hfedsF = fs->make<TH1F>( "hfedsF", "channel errors per FED",    num_feds,minFed,maxFed);
  hfedsSlink = fs->make<TH1F>( "hfedsSlink", "errors per FED",    num_feds,minFed,maxFed);
  hfedsCRC = fs->make<TH1F>( "hfedsCRC", "errors per FED",        num_feds,minFed,maxFed);
  hfedsUnknown = fs->make<TH1F>( "hfedsUnknown", "errors per FED",num_feds,minFed,maxFed);

  htype   = fs->make<TH1F>( "htype", "derived error type ",   50, -0.5, 49.5);
  herrors = fs->make<TH1F>( "herrors", "basic fed error type ", 50, -0.5, 49.5);
  herrorsF = fs->make<TH1F>( "herrorsF", "basic chanel error type",50, -0.5, 49.5);

  hfedErrors0 = fs->make<TH2F>( "hfedErrors0", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // ALL
  hfedErrors0F= fs->make<TH2F>( "hfedErrors0F","errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // ALL
  hfedErrors1 = fs->make<TH2F>( "hfedErrors1", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // Timeout
  hfedErrors2 = fs->make<TH2F>( "hfedErrors2", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // Overflow
  hfedErrors3 = fs->make<TH2F>( "hfedErrors3", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // ENE
  hfedErrors4 = fs->make<TH2F>( "hfedErrors4", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // FIFO
  hfedErrors5 = fs->make<TH2F>( "hfedErrors5", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // NOR
  hfedErrors6 = fs->make<TH2F>( "hfedErrors6", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // TRAILER
  hfedErrors7 = fs->make<TH2F>( "hfedErrors7", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // FSM 
  hfedErrors8 = fs->make<TH2F>( "hfedErrors8", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // Invalid ROC
  hfedErrors9 = fs->make<TH2F>( "hfedErrors9", "errors", num_feds,minFed,maxFed,49, -0.5, 48.5); // Invalid DCOL-PIX

  hfed2d = fs->make<TH2F>( "hfed2d", "errors", num_feds,minFed,maxFed,20, 19.5, 39.5); // ALL
  hfed2d0 = fs->make<TH2F>("hfed2d0","errors", num_feds,minFed,maxFed,20, 19.5, 39.5); // ALL

  hlumi  = fs->make<TH1F>("hlumi", "lumi", 4000,0,4000.);
  hbx    = fs->make<TH1F>("hbx",   "bx",   4000,0,4000.);  
  
}

//--------------------------------------------------------------------------------
int PixelFedErrorDumper::analyzeConversionErrors(int errorType, int FedId, int fedChannel, uint32_t word32) {

  if(fedChannel<1||fedChannel>48) {
    cout<<" Cannot get a valid  fed channel number of a -fed- error "<<fedChannel<<endl;
    return -1; // skip this error
  }
  int errorTypeMod = errorType;

  if( (errorType==25) || (errorType>=36&&errorType<=37) ) {
    
    hfedErrors0->Fill(float(FedId),float(fedChannel));
    hfed2d0->Fill(float(FedId),float(errorTypeMod));
    htype->Fill(float(errorTypeMod));
    
    switch(errorType) {
    case(25) : { // inv ROC
      hfedErrors8->Fill(float(FedId),float(fedChannel));
      break;}
    case(36) : { // inv ROC
      hfedErrors8->Fill(float(FedId),float(fedChannel));
      break;}
    case(37) : { // inv DCOL&PIX
      hfedErrors9->Fill(float(FedId),float(fedChannel));
      //spareCounts++;
      //cout<<" DCOL&PIX error: "<<errorType <<" "<<FedId<<" "<<fedChannel<<endl;
      //if(subid==1) cout<<" BPix: layer "<<layer<<" ladder "<<ladder<<" module "<<module<<endl;
      //else         cout<<" FPix: disk "<<disk<<" "<<blade<<" "<<zindex<<endl; 
      
      break;}
    }

  } else {
    cout<<" unknown error "<<errorType<<endl;
    hfedsUnknown->Fill(float(FedId));
    errorTypeMod=24;
    hfed2d0->Fill(float(FedId),float(errorTypeMod));
    htype->Fill(float(errorTypeMod));
  }

  hfed2d->Fill(float(FedId),float(errorTypeMod));
  if(errorTypeMod>=0 && errorTypeMod<40) countErrors2[errorTypeMod]++;

  return 0;
}

//--------------------------------------------------------------------------------
int PixelFedErrorDumper::analyzeFedErrors(int errorType, int FedId, int fedChannel, uint32_t word32) {

  if(fedChannel<1||fedChannel>48) {
    cout<<" Cannot get a valid  fed channel number of a -fed- error "<<fedChannel<<endl;
    return -1; // skip this error
  }
  int errorTypeMod = errorType;
  
  hfedErrors0->Fill(float(FedId),float(fedChannel));
  hfedErrors0F->Fill(float(FedId),float(fedChannel));
  
  if(errorType>=28 && errorType<=31) {
    
    switch(errorType) {
      
    case(28) : { // FIFO
      hfedErrors4->Fill(float(FedId),float(fedChannel));
      hfed2d0->Fill(float(FedId),float(errorTypeMod));
      htype->Fill(float(errorTypeMod));
      break;}
      
    case(29) : {  // TIMEOUT 
      hfedErrors1->Fill(float(FedId),float(fedChannel));
      hfed2d0->Fill(float(FedId),float(errorTypeMod));
      htype->Fill(float(errorTypeMod));
      break;}
      
    case(30) : {  // TRAILER
      
      int tbm = (word32& 0xff);
      int mode = (word32>>8) & 0xf;
      //hmodeF->Fill(float(mode));
      hmode->Fill(float(mode));
      //cout<<" error 30 "<<hex<<mode<<dec<<endl;
      countErrors[30]++;
      
      if( (mode&0x6) != 0) { // fsm
	hfedErrors7->Fill(float(FedId),float(fedChannel));
	errorTypeMod=23;
	hfed2d0->Fill(float(FedId),float(errorTypeMod));
	htype->Fill(float(errorTypeMod));
      } 

      if( tbm != 0) { // trailer
	hfedErrors6->Fill(float(FedId),float(fedChannel));
	htbm->Fill(float(tbm));
	errorTypeMod=20;
	hfed2d0->Fill(float(FedId),float(errorTypeMod));
	htype->Fill(float(errorTypeMod));
      }
      
      if( (mode&0x8) != 0) { // nor
	//cout<<" NOR "<<endl;
	hfedErrors5->Fill(float(FedId),float(fedChannel));
	errorTypeMod=21;
	hfed2d0->Fill(float(FedId),float(errorTypeMod));
	htype->Fill(float(errorTypeMod));
      }
      
      
      if( (mode&0x1) != 0) { // overflow
	hfedErrors2->Fill(float(FedId),float(fedChannel));
	errorTypeMod=22;
	hfed2d0->Fill(float(FedId),float(errorTypeMod));
	htype->Fill(float(errorTypeMod));
      } 
      
      
      break;}
      
    case(31) : {  // EVE
      hfedErrors3->Fill(float(FedId),float(fedChannel));
      hfed2d0->Fill(float(FedId),float(errorTypeMod));
      htype->Fill(float(errorTypeMod));
      break;}
    } // end switch
    
  } else if (errorType>=32 &&errorType<=34) {
    cout<<" Slink error "<<endl;
    
    hfedsSlink->Fill(float(FedId));
    hfed2d0->Fill(float(FedId),float(errorTypeMod));
    htype->Fill(float(errorTypeMod));
    
  } else if ( errorType==39) { // CRC 
    cout<<" CRC error "<<endl;
    hfedsCRC->Fill(float(FedId));
    hfed2d0->Fill(float(FedId),float(errorTypeMod));
    htype->Fill(float(errorTypeMod));

  } else {
    cout<<" unknown error "<<errorType<<endl;
    hfedsUnknown->Fill(float(FedId));
    errorTypeMod=24;	  
    hfed2d0->Fill(float(FedId),float(errorTypeMod));
    htype->Fill(float(errorTypeMod));
  }
  
  hfed2d->Fill(float(FedId),float(errorTypeMod));
  
  if(errorTypeMod>=0 && errorTypeMod<40) countErrors[errorTypeMod]++;

  return 0;
}

//--------------------------------------------------------------------------------
void PixelFedErrorDumper::analyze(const  edm::Event& ev, const edm::EventSetup& es) {
  //const bool PRINT = false;

  // Get event setup 
  //edm::ESHandle<TrackerGeometry> geom;
  //es.get<TrackerDigiGeometryRecord>().get( geom );
  //const TrackerGeometry& theTracker(*geom);

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoH;
  es.get<TrackerTopologyRcd>().get(tTopoH);
  const TrackerTopology *tTopo=tTopoH.product();

  // Access event information
  int run       = ev.id().run();
  int event     = ev.id().event();
  int lumiBlock = ev.luminosityBlock();
  int bx        = ev.bunchCrossing();
  //int orbit     = ev.orbitNumber(); // unused 

  hlumi->Fill(float(lumiBlock));
  hbx->Fill(float(bx));

  edm::Handle< edm::DetSetVector<SiPixelRawDataError> >  input;
  //static std::string src_ = theConfig.getUntrackedParameter<std::string>("InputLabel","source");
  //static std::string src_ = theConfig.getUntrackedParameter<std::string>("InputLabel","siPixelDigis");
  //static std::string instance = theConfig.getUntrackedParameter<std::string>("InputInstance","");  
  ev.getByToken(fedErrorContainer , input);  // the new bytoken

  if (!input.isValid()) {cout<<" Container not found "<<endl; return;}

  if(PRINT) cout<<" Container found "<<run<<" "<<event<<" "<<lumiBlock<<" "<<bx<<endl;

  // Iterate on detector units
  edm::DetSetVector<SiPixelRawDataError>::const_iterator DSViter;
  for(DSViter = input->begin(); DSViter != input->end(); DSViter++) {
    //bool valid = false;
    unsigned int detid = DSViter->id; // = rawid
    //cout<<hex<<detid<<dec<<endl;

    if(detid==0xffffffff) { // whole fed

      if(PRINT) cout<<" FED errors "<<DSViter->data.size()<<endl;
      // Look at FED errors now	
      edm::DetSet<SiPixelRawDataError>::const_iterator  di;

      //   iterate over errors 
      for(di = DSViter->data.begin(); di != DSViter->data.end(); di++) {
	int FedId = di->getFedId();                  // FED the error came from
	int errorType = di->getType();               // type of error
	uint32_t word32 = di->getWord32();
	uint64_t word64 = di->getWord64();

	fedErrors++;

 	herrors->Fill(float(errorType));
	//herrorsF->Fill(float(errorType));
	hfeds->Fill(float(FedId));

	if(PRINT) 
	  cout<<" fed " <<FedId<<" type "<<errorType<<" "<<hex<<word32<<" "<<word64<<dec<<" "<<di->getMessage()<<endl;

	int status=0;
	int fedChannel = -1;
	if (errorType==25 || (errorType>=35 && errorType<=38) ) { // conversion error
	  cout<<" Should never happen, a -fed- conversion error?  "<<endl;

	} else {  // fed error	
	  status = MyDecode2::error(word32, fedChannel, FedId, PRINT);
	  int ret = analyzeFedErrors(errorType, FedId, fedChannel, word32);
	  if(PRINT) cout<<" FED error: status "<<status<<" "<<ret<<endl;

	} // end if error    
      } // for errors
      
    } else { // module errors 

      DetId detId(detid);
      //const GeomDetUnit      * geoUnit = geom->idToDetUnit( detId );
      //const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
      //unsigned int detType=detId.det(); // det type, tracker=1
      unsigned int subid=detId.subdetId(); //subdetector type, barrel=1
      
      if(PRINT) {
	cout<<" Module errors: "<<DSViter->data.size()<<endl;
	cout<<"Det: "<<detId.rawId()<<":";
        //cout<<"Det: "<<detId.rawId()<<" "<<detId.null()<<" "<<detType<<" "<<subid<<endl;
      } // end PRINT

      int layer=0; //  ladder=0, module=0, disk=0, blade=0;
      if(subid==1) { // bpix

	//int layerC=tTopo->pxbLayer(detid);
	//int ladderC=tTopo->pxbLadder(detid);
	//int zindex=tTopo->pxbModule(detid);
	PixelBarrelName pbn(detid,tTopo,phase1_);
	//int sector = pbn.sectorName();
	//ladder = pbn.ladderName();
	layer  = pbn.layerName();
	//module = pbn.moduleName();
	//int half  = pbn.isHalfModule();
	//PixelBarrelName::Shell shell = pbn.shell();

	if(PRINT) 
	  cout<<" BPix:"<<pbn.name()<<endl;
	//cout<<" BPix: layer "<<layer<<" ladder "<<ladder<<" module "<<module<<" shell "<<shell<<" sector "<<sector<<" "
	//    <<layerC<<" "<<ladderC<<" "<<zindex<<" "<<half<<" "<<detType<<" "
	//    <<pbn.name()<<endl;

      } else if(subid==2) {  // fpix

	int disk=tTopo->pxfDisk(detid); //1,2,3
	int blade=tTopo->pxfBlade(detid); //1-24
	//int zindex=tTopo->pxfModule(detid); //
	int side=tTopo->pxfSide(detid); //size=1 for -z, 2 for +z
	int panel=tTopo->pxfPanel(detid); //panel=1
	PixelEndcapName pen(detid,tTopo,phase1_);
	
	if(PRINT) cout<<" FPix: disk "<<disk<<" "<<blade<<" "<<side<<" "<<panel<<endl; 

      } // bpix/fpix

      // Look at FED errors in this module
      edm::DetSet<SiPixelRawDataError>::const_iterator  di;
      for(di = DSViter->data.begin(); di != DSViter->data.end(); di++) {
	int FedId = di->getFedId();                  // FED the error came from
	int errorType = di->getType();               // type of error
	uint32_t word32 = di->getWord32();
	//uint64_t word64 = di->getWord64();  // unused 
	
	herrorsF->Fill(float(errorType));
	hfedsF->Fill(float(FedId));
	moduleErrors++;

	if(PRINT) cout<<" Fed " <<FedId<<" error type "<<errorType<<endl;

	int status=0;
	int fedChannel = 0;

	if( (errorType>=26 && errorType<=34) || (errorType==39) ) {  //  error from fed	

	  status = MyDecode2::error(word32, fedChannel, FedId, PRINT);
	  int ret = analyzeFedErrors(errorType, FedId, fedChannel, word32);
	  if(PRINT) cout<<" FED error: status "<<status<<" "<<ret<<endl;

	} else { // conversion error

	  //cout<<" 25-38 "<<errorType<<endl;

	  status = MyDecode2::data(word32, fedChannel, FedId, layer, PRINT);
	  int ret = analyzeConversionErrors(errorType, FedId, fedChannel, word32);
	  
	  if(PRINT) 
	    cout<<" Conversion error: fed "<<FedId<<" "<<"Channel "<<fedChannel<<" status  "<<status<<" "<<ret<<endl;


	}


      } // for

    } // if fed/module 

  } // end det loop 

}

//* 20 trailer
//* 21 NOR
//* 22 Overflow
//* 23 FSM
//* 24
//* 25 indicates an invalid ROC of 25
//* 26 indicates a gap word (ignored, not stored as error information)
//* 27 indicates a dummy word (ignored, not stored as error information)
//* 28 indicates a FIFO full error
//* 29 indicates a timeout error
//* 30 indicates a TBM error trailer
//* 31 indicates an event number error (TBM and FED event number mismatch)
//* 32 indicates an incorrectly formatted Slink Header
//* 33 indicates an incorrectly formatted Slink Trailer
//* 34 indicates the event size encoded in the Slink Trailer is different than the size found at raw to digi conversion
//* 35 indicates an invalid FED channel number
//* 36 indicates an invalid ROC value
//* 37 indicates an invalid dcol or pixel value
//* 38 indicates the pixels on a ROC weren't read out from lowest to highest row and dcol value
//* 39 indicates a CRC error, which indicates a corrupt data buffer was not translated
//* 40 indicates an overflow error


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PixelFedErrorDumper);
