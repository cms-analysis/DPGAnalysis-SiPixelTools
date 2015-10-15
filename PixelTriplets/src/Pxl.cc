// -*- C++ -*-
//
// Package:  Pxl
// Class:    Pxl
//
// my/Pxl/src/Pxl.cc
//
// Pixel (and strip) triplet residuals
//
// Original Author:  Daniel Pitzl, DESY
//         Created:  Sat Feb 12 12:12:42 CET 2011
// $Id$
//

// system include files:
#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>

// ROOT:
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

// CMS and user include files:
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
//#include <FWCore/Framework/interface/EventSetup.h>
#include "FWCore/Framework/interface/MakerMacros.h"

#include <DataFormats/BeamSpot/interface/BeamSpot.h>

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <DataFormats/TrackReco/interface/HitPattern.h>

#include <MagneticField/Engine/interface/MagneticField.h>

// To convert detId to subdet/layer number:
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
//#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/Topology.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"



#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
//#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
//#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"


#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajectoryBuilder.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include <TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

#include "DataFormats/TrackerRecHit2D/interface/TkCloner.h"

#include "TrackingTools/DetLayers/interface/DetLayer.h"

//#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"
#include "CondFormats/DataRecord/interface/SiPixelLorentzAngleRcd.h"

// Flag for new tracking rechis, has to be ON for pre7 and later
#define NEW_TRACKINGRECHITS  // For V71X_pre7 and later

//
// class declaration:
//
class myCounters{
public:
  static int neve;
  static unsigned int prevrun;
};

int myCounters::neve = 0;
unsigned int myCounters::prevrun = 0;
//
//
//

struct Histos{
  TH1D     *h000, *h001, *h002, *h003, *h004, *h005, *h006, *h007, *h008, *h009;
  TH1D     *h010, *h011, *h012, *h013, *h014, *h015, *h016, *h017, *h018, *h019;
  TH1D     *h020, *h021, *h022, *h023, *h024, *h025, *h026, *h027, *h028, *h029;
  TH1D     *h030, *h031, *h032, *h033, *h034, *h035, *h036, *h037, *h038, *h039;
  TH1D     *h040, *h041, *h042;
  TProfile                      *h043, *h044, *h045, *h046, *h047, *h048;
  TH2D                                                                    *h049;
  TH1D     *h050, *h051, *h052, *h053, *h054, *h055, *h056, *h057, *h058, *h059;
  TH1D     *h060, *h061, *h062, *h063, *h064, *h065;
  TH2D                                               *h066;
  TH1D                                                      *h067, *h068, *h069;
  TH1D     *h070, *h071, *h072, *h073;
  TProfile                             *h074, *h075;
  TH1D                                               *h076, *h077, *h078, *h079;
  TH1D     *h080, *h081, *h082;
  TProfile                      *h083, *h084, *h085;
  TH1D                                               *h086, *h087, *h088, *h089;
  TH1D     *h090, *h091, *h092, *h093, *h094, *h095, *h096, *h097, *h098, *h099;

  TH1D     *h100, *h101, *h102, *h103, *h104, *h105,               *h108;
  TH2D                                               *h106, *h107,        *h109;
  TH1D     *h110,        *h112, *h113, *h114, *h115, *h116,        *h118, *h119;
  TH2D            *h111,                                    *h117;
  TProfile        *h121, *h122, *h123, *h124, *h125, *h126, *h127, *h128, *h129;
  TProfile *h130, *h131, *h132, *h133, *h134, *h135, *h136, *h137, *h138, *h139;

  TH1D     *h200, *h201, *h202, *h203, *h204, *h205,               *h208;
  TH2D                                               *h206, *h207,        *h209;
  TProfile *h210, *h211, *h212, *h213, *h214, *h215, *h216, *h217, *h218, *h219;
  TProfile *h220, *h221, *h222, *h223, *h224, *h225, *h226;
  TH1D                                                      *h227, *h228, *h229;
  TProfile *h230, *h231, *h232, *h233, *h234;
  TH1D                                        *h235, *h236, *h237, *h238, *h239;
  TProfile *h240, *h241, *h242, *h243, *h244, *h245, *h246, *h247, *h248;
  TH1D                                                                    *h249;
  TH1D     *h250, *h251, *h252, *h253, *h254, *h255, *h256, *h257, *h258, *h259;
  TH1D     *h260;
  TProfile        *h261, *h262, *h263, *h264, *h265, *h266, *h267, *h268;
  TH1D                                                                    *h269;
  TH1D     *h270, *h271, *h272, *h273, *h274, *h275, *h276, *h277, *h278, *h279;
  TH1D     *h280, *h281, *h282;
  TProfile                      *h283;
  TH1D                                 *h284, *h285;
  TProfile                                           *h286, *h287, *h288, *h289;
  TH1D     *h290, *h291, *h292, *h293, *h294, *h295, *h296, *h297, *h298, *h299;

  TH1D     *h300, *h301, *h302, *h303, *h304, *h305,               *h308;
  TH2D                                               *h306, *h307,        *h309;
  TH1D     *h310, *h311, *h312, *h313, *h314, *h315, *h316, *h317, *h318;
  TProfile                                                                *h319;
  TProfile *h320, *h321, *h322,                             *h327;
  TH2D                          *h323, *h324;
  TProfile *h330, *h331;
  TH1D                   *h332;
  TProfile                             *h334;
  TProfile                      *h343;

  TH1D                                 *h374, *h375, *h376, *h377,        *h379;
  TH1D     *h380, *h381, *h382, *h383, *h384, *h385, *h386, *h387, *h388, *h389;
  TH1D     *h390, *h391, *h392, *h393, *h394, *h395, *h396, *h397, *h398, *h399;

  TH1D     *h400, *h401, *h402, *h403, *h404, *h405, *h406, *h407, *h408;
  TProfile                                                                *h409;
  TH1D     *h410, *h411;
  TProfile               *h412, *h412_out_zplus, *h412_out_zminus, *h412_in_zplus, *h412_in_zminus, *h413, *h414, *h415, *h416, *h417, *h418, *h419;
  TH1D     *h420, *h420_out_zplus, *h420_out_zminus, *h420_in_zplus, *h420_in_zminus, *h421; 
  TH1D     *h420_mod1, *h420_mod2, *h420_mod3, *h420_mod4, *h420_mod5, *h420_mod6, *h420_mod7, *h420_mod8; 
  TH1D     *h421_mod1, *h421_mod2, *h421_mod3, *h421_mod4, *h421_mod5, *h421_mod6, *h421_mod7, *h421_mod8;
  TH1D     *h420_1, *h420_2, *h420_3;
  TProfile               *h422, *h423, *h424, *h425, *h426, *h427, *h428, *h429, *h429_eta;
  TH1D     *h430, *h431, *h432,                      *h436,        *h438, *h439;
  TProfile                      *h433, *h434, *h435,        *h437;
  TH1D     *h440, *h441, *h442, *h443, *h444, *h445, *h446, *h447, *h448, *h449;

  TH1D     *h450, *h451, *h452, *h453, *h454, *h455, *h456, *h457, *h458;
  TProfile                                                                *h459;
  TH1D     *h460, *h461;
  TProfile               *h462, *h463, *h464, *h465, *h466, *h467, *h468, *h469;
  TH1D     *h470, *h471;
  TH1D     *h470_1, *h470_2, *h470_3;
  TProfile               *h472, *h473, *h474, *h475, *h476, *h477, *h478, *h479;
  TH1D     *h480, *h481, *h482,                      *h486, *h487, *h488, *h489;
  TProfile                      *h483, *h484, *h485;
  TH1D     *h490, *h491, *h492, *h493, *h494, *h495, *h496, *h497, *h498, *h499;

  TH1D     *h500, *h501, *h502, *h503, *h504, *h505, *h506, *h507, *h508;
  TProfile                                                                *h509;
  TH1D     *h510, *h511;
  TProfile               *h512, *h513, *h514, *h515, *h516, *h517, *h518, *h519;
  TH1D     *h520, *h521;
  TH1D     *h520_1, *h520_2, *h520_3, *h520_4, *h520_5;
  TProfile               *h522, *h523, *h524, *h525, *h526, *h527, *h528, *h529;
  TH1D     *h530, *h531, *h532,                      *h536, *h537, *h538, *h539;
  TProfile                      *h533, *h534, *h535;
  TH1D     *h540, *h541, *h542, *h543, *h544, *h545, *h546, *h547, *h548, *h549;

  TH1D                                                      *h557, *h558, *h559;
  TH1D     *h560, *h561;
  TProfile               *h562, *h563, *h564, *h565;

  TH1D     *h570, *h571;
  TProfile               *h572, *h573, *h574, *h575, *h576, *h577;
  TH1D     *h580, *h581;
  TProfile               *h582, *h583, *h584, *h585, *h586, *h587;
  TH1D     *h590, *h591;
  TProfile               *h592, *h593, *h594, *h595, *h596, *h597;

  TH1D     *h600, *h601, *h602, *h603, *h604, *h605, *h606, *h607, *h608;
  TProfile                                                                *h609;
  TH1D     *h610, *h611;
  TProfile               *h612, *h612_out_zplus, *h612_out_zminus, *h612_in_zplus, *h612_in_zminus, *h613, *h614, *h615, *h616, *h617, *h618, *h619;
  TH1D     *h620, *h620_out_zplus, *h620_out_zminus, *h620_in_zplus, *h620_in_zminus, *h621;
  TProfile               *h622, *h623, *h624, *h625, *h626, *h627, *h628, *h629;
  TH1D     *h630, *h631, *h632,                      *h636, *h637, *h638, *h639;
  TProfile                      *h633, *h634, *h635;
  TH1D     *h640, *h641;
  TProfile               *h642, *h643, *h644, *h645, *h646, *h647, *h648, *h649;
  TH1D     *h650, *h651, *h652, *h653, *h654, *h655, *h656, *h657, *h658, *h659;

  TH1D                                                      *h667, *h668, *h669;
  TH1D     *h670, *h671;
  TProfile               *h672, *h673, *h674, *h675;

  TH1D     *h700, *h701, *h702, *h703, *h704, *h705, *h706, *h707, *h708;
  TProfile                                                                *h709;
  TH1D     *h710, *h711;
  TProfile               *h712, *h713, *h714, *h715, *h716, *h717, *h718, *h719;
  TH1D     *h720, *h721;
  TProfile               *h722, *h723, *h724, *h725, *h726, *h727, *h728, *h729;
  TH1D     *h730, *h731, *h732,                      *h736, *h737, *h738, *h739;
  TProfile                      *h733, *h734, *h735;
  TH1D     *h740, *h741;

  TH1D     *h750, *h751, *h752, *h753, *h754, *h755, *h756, *h757, *h758;
  TProfile                                                                *h759;
  TH1D     *h760, *h761;
  TProfile               *h762, *h763, *h764, *h765, *h766, *h767, *h768, *h769;
  TH1D     *h770, *h771;
  TProfile               *h772, *h773, *h774, *h775, *h776, *h777, *h778, *h779;
  TH1D     *h780, *h781, *h782,               *h785, *h786, *h787, *h788, *h789;
  TProfile                      *h783, *h784;
  TH1D     *h790, *h791;

  TH1D     *h800, *h801, *h802, *h803, *h804, *h805, *h806, *h807, *h808;
  TProfile                                                                *h809;
  TH1D     *h810, *h811;
  TProfile               *h812, *h813, *h814, *h815, *h816, *h817, *h818, *h819;
  TH1D     *h820, *h821;
  TProfile               *h822, *h823, *h824, *h825, *h826, *h827, *h828, *h829;
  TH1D     *h830, *h831, *h832,                      *h836, *h837, *h838, *h839;
  TProfile                      *h833, *h834, *h835;
  TH1D     *h840, *h841;

  TH1D     *h850, *h851, *h852, *h853, *h854, *h855, *h856, *h857, *h858;
  TProfile                                                                *h859;
  TH1D     *h860, *h861;
  TProfile               *h862, *h863, *h864, *h865, *h866, *h867, *h868, *h869;
  TH1D     *h870, *h871;
  TProfile               *h872, *h873, *h874, *h875, *h876, *h877, *h878, *h879;
  TH1D     *h880, *h881, *h882,               *h885, *h886, *h887, *h888, *h889;
  TProfile                      *h883, *h884;
  TH1D     *h890, *h891;

  TH1D     *h900, *h901, *h902, *h903, *h904,        *h906, *h907, *h908, *h909;
  TProfile                                    *h905;
  TH1D     *h910, *h911,        *h913,        *h915,        *h917;
  TProfile               *h912,        *h914,        *h916,        *h918, *h919;
  TH1D     *h920, *h921;
  TProfile               *h922,        *h924, *h925, *h926,        *h928, *h929;
  TH1D     *h930, *h931, *h932, *h933, *h934, *h935, *h936, *h937, *h938, *h939;

  TH1D     *h960, *h961, *h962, *h963, *h964, *h965, *h966, *h967, *h968;
  TH1D     *h970, *h971,        *h973,        *h975,        *h977,        *h979;
  TH2D            *h981;
  TH1D                          *h983;

  TH1D     *i500, *i501, *i502, *i503, *i504, *i505, *i506, *i507;
  TProfile                                                         *i508, *i509;
  TH1D     *i510, *i511;
  TProfile               *i512, *i512_out_zplus, *i512_out_zminus, *i512_in_zplus, *i512_in_zminus, *i513, *i514, *i515, *i516, *i517, *i518, *i519;
  TH1D     *i520, *i520_out_zplus, *i520_out_zminus, *i520_in_zplus, *i520_in_zminus, *i521;
  TH1D     *i520_1, *i520_2, *i520_3, *i520_4, *i520_5;
  TProfile               *i522, *i523, *i524, *i525, *i526, *i527, *i528, *i529;
  TH1D     *i530, *i531, *i532,                      *i536, *i537, *i538, *i539;
  TProfile                      *i533, *i534, *i535;
  TH1D     *i540, *i541, *i542, *i543, *i544, *i545, *i546, *i547, *i548, *i549;

  // unbiased resids:

  TH1D     *k100, *k101;
  TProfile               *k102;
  TH1D                          *k103, *k104;
  TProfile                                    *k105, *k106, *k107, *k108;
  TH1D                                                                    *k109;
  TProfile *k110;
  TH1D            *k111;
  TProfile               *k112, *k113, *k114, *k115, *k116, *k117, *k118;

  TH1D     *k120, *k121;
  TProfile               *k122;
  TH1D                          *k123, *k124;
  TProfile                                    *k125, *k126, *k127, *k128;
  TH1D                                                                    *k129;
  TProfile *k130;
  TH1D            *k131;
  TProfile               *k132, *k133, *k134, *k135, *k136, *k137, *k138;

  TH1D     *k140, *k141;
  TProfile               *k142;
  TH1D                          *k143, *k144;
  TProfile                                    *k145, *k146, *k147, *k148;
  TH1D                                                                    *k149;
  TProfile *k150;
  TH1D            *k151;
  TProfile               *k152, *k153, *k154, *k155, *k156, *k157, *k158;

  TH1D     *k160, *k161;

  TH1D     *k170;
  TProfile        *k171, *k172, *k173, *k174, *k175, *k176;
  TH1D     *k180;
  TProfile        *k181, *k182, *k183, *k184, *k185, *k186;
  TH1D     *k190;
  TProfile        *k191, *k192, *k193, *k194, *k195, *k196;

  TH1D     *k300, *k301, *k302, *k303,                             *k308;
  TProfile                             *k304, *k305, *k306, *k307;

  TProfile *k310;
  TH1D            *k311;
  TProfile               *k312, *k313, *k314, *k315, *k316, *k317, *k318;
  TH1D                                                                    *k319;

  TProfile *k320;
  TH1D            *k321;
  TProfile               *k322, *k323, *k324, *k325, *k326, *k327, *k328;
  TH1D                                                                    *k329;

  TProfile *k330;
  TH1D            *k331;
  TProfile               *k332, *k333, *k334, *k335, *k336, *k337, *k338;
  TH1D                                                                    *k339;

  TProfile *k340;
  TH1D            *k341;
  TProfile               *k342, *k343, *k344, *k345, *k346, *k347, *k348;
  TH1D                                                                    *k349;

  TProfile *k350;
  TH1D            *k351;
  TProfile               *k352, *k353, *k354, *k355, *k356, *k357, *k358;
  TH1D                                                                    *k359;

  TProfile *k360;
  TH1D            *k361;
  TProfile               *k362, *k363, *k364, *k365, *k366, *k367, *k368;
  TH1D                                                                    *k369;

  TH1D     *k400, *k401, *k402, *k403, *k404;
  TProfile                                    *k405, *k406;

  TProfile *k410;
  TH1D            *k411, *k412;
  TProfile                      *k413, *k414, *k415;

  TProfile *k420;
  TH1D            *k421, *k422;
  TProfile                      *k423, *k424, *k425;

  TProfile *k430;
  TH1D            *k431, *k432;
  TProfile                      *k433, *k434, *k435;

  TH1D     *k500, *k501, *k502, *k503,                             *k508;
  TProfile                             *k504, *k505, *k506, *k507;

  TProfile *k510;
  TH1D            *k511;
  TProfile               *k512, *k513, *k514, *k515, *k516, *k517, *k518;
  TH1D                                                                    *k519;

  TProfile *k520;
  TH1D            *k521;
  TProfile               *k522, *k523, *k524, *k525, *k526, *k527, *k528;
  TH1D                                                                    *k529;

  TProfile *k530;
  TH1D            *k531;
  TProfile               *k532, *k533, *k534, *k535, *k536, *k537, *k538;
  TH1D                                                                    *k539;

  TProfile *k540;
  TH1D            *k541;
  TProfile               *k542, *k543, *k544, *k545, *k546, *k547, *k548;
  TH1D                                                                    *k549;

  TProfile *k550;
  TH1D            *k551;
  TProfile               *k552, *k553, *k554, *k555, *k556, *k557, *k558;
  TH1D                                                                    *k559;

  TProfile *k560;
  TH1D            *k561;
  TProfile               *k562, *k563, *k564, *k565, *k566, *k567, *k568;
  TH1D                                                                    *k569;

  TProfile *k570;
  TH1D            *k571;
  TProfile               *k572, *k573, *k574, *k575, *k576, *k577, *k578;
  TH1D                                                                    *k579;

  TProfile *k580;
  TH1D            *k581;
  TProfile               *k582, *k583, *k584, *k585, *k586, *k587, *k588;
  TH1D                                                                    *k589;

  TH1D     *k600, *k601, *k602, *k603, *k604;
  TProfile                                    *k605, *k606, *k607;
  TH2D                                                             *k608, *k609;
  TProfile *k610;
  TH1D            *k611, *k612;
  TProfile                      *k613, *k614;
  TH1D                                        *k615;

  TProfile *k620;
  TH1D            *k621, *k622;
  TProfile                      *k623, *k624;
  TH1D                                        *k625;

  TProfile *k630;
  TH1D            *k631, *k632;
  TProfile                      *k633, *k634;
  TH1D                                        *k635;

  TProfile *k640;
  TH1D            *k641, *k642;
  TProfile                      *k643, *k644;
  TH1D                                        *k645;

  TProfile *k650;
  TH1D            *k651, *k652;
  TProfile                      *k653, *k654;
  TH1D                                        *k655;

  TProfile *k660;
  TH1D            *k661, *k662;
  TProfile                      *k663, *k664;
  TH1D                                        *k665;

  TProfile *k670;
  TH1D            *k671, *k672;
  TProfile                      *k673, *k674;
  TH1D                                        *k675;

  TH1D            *k681, *k682, *k683, *k684, *k685, *k686, *k687, *k688, *k689;
  //
  // pixel ray resids:
  //
  TH1D     *r300, *r301;
  TProfile               *r302, *r303, *r304, *r305, *r306, *r307, *r308, *r309;
  TH1D     *r310, *r311,               *r314, *r315, *r316, *r317, *r318, *r319;

  TH1D     *r410, *r411,               *r414, *r415, *r416, *r417, *r418, *r419;
  TH2D                   *r412, *r413;
  TH1D     *r420, *r421;
  TProfile               *r422, *r423, *r424, *r425, *r426, *r427, *r428, *r429;

  TH1D     *r610, *r611,               *r614, *r615, *r616, *r617, *r618, *r619;
  TH2D                   *r612, *r613;
  TH1D     *r620, *r621;
  TProfile               *r622, *r623, *r624, *r625, *r626, *r627, *r628, *r629;
  //
  // unbiased refit resids:
  //
  TH1D *u110, *u111;
  TH1D *u120, *u121;
  TH1D *u130, *u131;

  TH1D *u310, *u311;
  TH1D *u320, *u321;
  TH1D *u330, *u331;
  TH1D *u340, *u341;
  TH1D *u350, *u351;
  TH1D *u360, *u361;

  TH1D *u410, *u411, *u412, *u413, *u414, *u415, *u416, *u417, *u418, *u419;
  TH1D *u420, *u421, *u422, *u423, *u424, *u425, *u426, *u427, *u428, *u429;
  TH1D *u430, *u431, *u432, *u433, *u434, *u435, *u436, *u437, *u438, *u439;
  TH1D *u440, *u441, *u442, *u443, *u444, *u445, *u446, *u447, *u448, *u449;
  TH1D *u450, *u451, *u452, *u453, *u454, *u455, *u456, *u457, *u458, *u459;

  TH1D *u510, *u511;
  TH1D *u520, *u521;
  TH1D *u530, *u531;
  TH1D *u540, *u541;
  TH1D *u550, *u551;
  TH1D *u560, *u561;
  TH1D *u570, *u571;
  TH1D *u580, *u581;

  TH1D *u610, *u611, *u612, *u613, *u614, *u615, *u616, *u617, *u618, *u619;
  TH1D *u620, *u621, *u622, *u623, *u624, *u625, *u626, *u627, *u628, *u629;
  TH1D *u630, *u631, *u632, *u633, *u634, *u635, *u636, *u637, *u638, *u639;
  TH1D *u640, *u641, *u642, *u643, *u644, *u645, *u646, *u647, *u648, *u649;
  TH1D *u650, *u651, *u652, *u653, *u654, *u655, *u656, *u657, *u658, *u659;
  TH1D *u660, *u661, *u662, *u663, *u664, *u665, *u666, *u667, *u668, *u669;
  TH1D *u670, *u671, *u672, *u673, *u674, *u675, *u676, *u677, *u678, *u679;
  TH1D *u680, *u681, *u682, *u683, *u684, *u685, *u686, *u687, *u688, *u689;

  void init(TFileDirectory* fs);
};

class Pxl : public edm::EDAnalyzer, public Histos {
  
public:
  explicit Pxl(const edm::ParameterSet&);
  ~Pxl();

private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  edm::InputTag _triggerSrc;
  std::string _ttrhBuilder;
  HLTConfigProvider HLTConfig;
  bool singleParticleMC;

  // ----------member data:
  std::map<int, Histos> runmap;
};

//
// constants, enums and typedefs:
//

//
// static data member definitions:
//

//
// constructor:
//
Pxl::Pxl(const edm::ParameterSet& iConfig)
{
  std::cout << "Pxl constructed\n";
  _triggerSrc = iConfig.getParameter<edm::InputTag>("triggerSource");
  _ttrhBuilder = iConfig.getParameter<std::string>("ttrhBuilder");
  singleParticleMC  = iConfig.getUntrackedParameter<bool>("singleParticleMC",false);
  std::cout<<_triggerSrc<<" "<<_triggerSrc.label()<<" "<<_triggerSrc.process()<<" "
	   <<_triggerSrc.instance()<<" "<<std::endl;
}
//
// destructor:
//
Pxl::~Pxl()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

void Histos::init(TFileDirectory* fs)
{
  h000 = fs->make<TH1D>( "h000", "#beta star;#beta star [cm]", 100, 0, 500 );
  h001 = fs->make<TH1D>( "h001", "emittance;emittance_{x} [cm]", 100, 0, 1e-5 );
  h002 = fs->make<TH1D>( "h002", "beam width x;beam width x [#mum]", 100, 0, 200 );
  h003 = fs->make<TH1D>( "h003", "beam width y;beam width y [#mum]", 100, 0, 200 );
  h004 = fs->make<TH1D>( "h004", "beam spot #sigma_{z};beam spot #sigma_{z} [cm]", 100, 0, 20 );
  h005 = fs->make<TH1D>( "h005", "beam spot x;beam spot x [cm]", 100, -1, 1 );
  h006 = fs->make<TH1D>( "h006", "beam spot y;beam spot y [cm]", 100, -1, 1 );
  h007 = fs->make<TH1D>( "h007", "beam spot z;beam spot z [cm]", 100, -5, 5 );
  h008 = fs->make<TH1D>( "h008", "beam slope dx/dz;beam slope dx/dz [mrad]", 100, -5, 5 );
  h009 = fs->make<TH1D>( "h009", "beam slope dy/dz;beam slope dy/dz [mrad]", 100, -5, 5 );

  h010 = fs->make<TH1D>( "h010", "number of primary vertices;vertices;events", 31, -0.5, 30.5 );
  h011 = fs->make<TH1D>( "h011", "invalid z-vertex;z [cm]", 100, -50, 50 );
  h012 = fs->make<TH1D>( "h012", "fake z-vertex;z [cm]", 100, -50, 50 );
  h013 = fs->make<TH1D>( "h013", "non-fake z-vertex;z [cm]", 100, -50, 50 );
  h014 = fs->make<TH1D>( "h014", "vertex x;x [cm]", 100, -0.5, 0.5 );
  h015 = fs->make<TH1D>( "h015", "vertex y;y [cm]", 100, -0.5, 0.5 );
  h016 = fs->make<TH1D>( "h016", "tracks per vertex;tracks;vertices", 101, -0.5, 100.5 );
  h017 = fs->make<TH1D>( "h017", "tracks per vertex;tracks;vertices", 100, 5, 505 );
  h018 = fs->make<TH1D>( "h018", "z-vertex with refitted tracks;z [cm]", 100, -50, 50 );
  h019 = fs->make<TH1D>( "h019", "z-vertex without refitted tracks;z [cm]", 100, -50, 50 );

  h021 = fs->make<TH1D>( "h021", "vertex sum p_{t};sum p_{t} [GeV]", 100, 0, 100 );
  h022 = fs->make<TH1D>( "h022", "vertex max sum p_{t};max sum p_{t} [GeV]", 100, 0, 100 );

  h023 = fs->make<TH1D>( "h023", "best vertex x;x [cm]", 100, -0.25, 0.25 );
  h024 = fs->make<TH1D>( "h024", "best vertex y;y [cm]", 100, -0.25, 0.25 );
  h025 = fs->make<TH1D>( "h025", "best vertex z;z [cm]", 100, -25, 25 );

  h026 = fs->make<TH1D>( "h026", "Sum Et;Sum Et [GeV]", 100, 0, 3000 );
  h027 = fs->make<TH1D>( "h027", "MET;MET [GeV]", 100, 0, 200 );

  h028 = fs->make<TH1D>( "h028", "sum track p_{t};sum track P_{T} [GeV]", 100, 0, 500 );
  h029 = fs->make<TH1D>( "h029", "sum primary track charge;sum track charge", 41, -20.5, 20.5 );

  h030 = fs->make<TH1D>( "h030", "number of tracks;tracks", 200, 0, 2000 );
  h031 = fs->make<TH1D>( "h031", "track charge;charge", 11, -5.5, 5.5 );
  h032 = fs->make<TH1D>( "h032", "p_{t};p_{t} [GeV]", 100, 0, 5 );
  h033 = fs->make<TH1D>( "h033", "p_{t} use logy, p_{t} [GeV]", 100, 0, 100 );

  h034 = fs->make<TH1D>( "h034", "number of rec hits;hits;tracks", 41, -0.5, 40.5 );
  h035 = fs->make<TH1D>( "h035", "valid tracker hits;tracker hits;tracks", 41, -0.5, 40.5 );
  h036 = fs->make<TH1D>( "h036", "valid pixel barrel hits;valid pixel barrel hits;tracks", 7, -0.5, 6.5 );
  h037 = fs->make<TH1D>( "h037", "tracker layers;tracker layers;tracks", 31, -0.5, 30.5 );
  h038 = fs->make<TH1D>( "h038", "pixel barrel layers;pixel barrel layers;tracks", 6, -0.5, 5.5 );

  h040 = fs->make<TH1D>( "h040", "b_{xy}, p_{t} > 8, PXB1;b_{xy} [#mum];tracks", 100, -250, 250 );
  h041 = fs->make<TH1D>( "h041", "#sigma(d_{xy}), p_{t} > 8, PXB1;#sigma(d_{xy}) [#mum];tracks", 100, 0, 100 );
  h042 = fs->make<TH1D>( "h042", "b_{xy}/#sigma, p_{t} > 8, PXB1;b_{xy}/#sigma;tracks", 100, -10, 10 );

  h043 = fs->make<TProfile>( "h043", "#sigma(b_{xy}) vs logp_{t} with PXB1;log(p_{t} [GeV]);<#sigma(b_{xy})> [#mum]", 40, 0, 2, 0, 5000 );
  h044 = fs->make<TProfile>( "h044", "rms(b_{xy}) vs logp_{t} with PXB1;log(p_{t} [GeV]);rms(b_{xy}) [#mum]", 40, 0, 2, 0, 5000 );
  h045 = fs->make<TProfile>( "h045", "rms(b_{xy}/#sigma) vs logp_{t} with PXB1;log(p_{t} [GeV]);rms(b_{xy}/#sigma) [#mum]", 40, 0, 2, 0, 10 );

  h049 = fs->make<TH2D>( "h049", "barrel pixels;col;row;hits", 416, -0.5, 415.5, 160, -0.5, 159.5 );

  h051 = fs->make<TH1D>( "h051", "kap-kap;#Delta#kappa;tracks", 100, -0.01, 0.01 );
  h052 = fs->make<TH1D>( "h052", "phi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h053 = fs->make<TH1D>( "h053", "dca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );
  h054 = fs->make<TH1D>( "h054", "dip-dip;#Deltadip;tracks", 100, -0.1, 0.1 );
  h055 = fs->make<TH1D>( "h055", "z0-z0;#Deltaz_{0};tracks", 100, -0.1, 0.1 );

  h057 = fs->make<TH1D>( "h057", "tscp ref x;x [cm];hits", 100, -1, 1 );
  h058 = fs->make<TH1D>( "h058", "tscp ref y;y [cm];hits", 100, -1, 1 );
  h059 = fs->make<TH1D>( "h059", "tscp ref z;z [cm];hits", 100, -10, 10 );

  h060 = fs->make<TH1D>( "h060", "rec hit tracker subdet;subdet ID;tracks", 11, -0.5, 10.5 );
  h061 = fs->make<TH1D>( "h061", "rec hits local x;x [cm];hits", 120, -6, 6 );
  h062 = fs->make<TH1D>( "h062", "rec hits local y;y [cm];hits",  80, -4, 4 );

  h063 = fs->make<TH1D>( "h063", "rec hits global R;R [cm];hits", 120, 0, 120 );
  h064 = fs->make<TH1D>( "h064", "rec hits global phi;#phi [deg];hits", 180, -180, 180 );
  h065 = fs->make<TH1D>( "h065", "rec hits global z;z [cm];hits", 300, -300, 300 );

  h066 = fs->make<TH2D>( "h066", "rec hits barrel x-y;x [cm];y [cm]", 240, -120, 120, 240, -120, 120 );

  h067 = fs->make<TH1D>( "h067", "PXB row;PXB row;hits", 160, -0.5, 159.5 );
  h068 = fs->make<TH1D>( "h068", "PXB row;PXB row;ADC [ke]", 160, -0.5, 159.5 );
  h069 = fs->make<TH1D>( "h069", "PXB y pull ", 100, -10., 10. );

  h070 = fs->make<TH1D>( "h070", "PXB cluster charge;PXB cluster charge [ke];hits", 100, 0, 200 );
  h071 = fs->make<TH1D>( "h071", "PXB cluster charge*cos(dip);PXB cluster charge*cos(dip) [ke];hits", 100, 0, 100 );
  h072 = fs->make<TH1D>( "h072", "PXB cluster cols;PXB cluster cols;hits", 31, -0.5, 30.5 );
  h073 = fs->make<TH1D>( "h073", "PXB cluster rows;PXB cluster rows;hits", 21, -0.5, 20.5 );
  h074 = fs->make<TProfile>( "h074", "PXB cluster cols vs dip;dip angle [deg];<PXB cluster cols>", 80, -80, 80, 0, 30 );
  h075 = fs->make<TProfile>( "h075", "PXB cluster rows vs dip;dip angle [deg];<PXB cluster rows>", 80, -80, 80, 0, 10 );
  h076 = fs->make<TH1D>( "h076", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h077 = fs->make<TH1D>( "h077", "PXB x error ", 100, 0., 100. );
  h078 = fs->make<TH1D>( "h078", "PXB y error ", 100, 0., 100. );
  h079 = fs->make<TH1D>( "h079", "PXB x pull ", 100, -10., 10. );

  h080 = fs->make<TH1D>( "h080", "PXB x in pixel;x in pixel [#mum];hits", 100, 00, 100 );
  h081 = fs->make<TH1D>( "h081", "PXB x in pixel, =2 rows;x in pixel [#mum];hits", 100, 0, 100 );
  h082 = fs->make<TH1D>( "h082", "PXB x in pixel <>2 rows;x in pixel [#mum];hits", 100, 0, 100 );
  h083 = fs->make<TProfile>( "h083", "PXB eta vs x in pixel;x in pixel [#mum];<eta x>", 100, 0, 100, -2, 2 );
  h084 = fs->make<TProfile>( "h084", "PXB eta vs x in pixel outward;x in pixel [#mum];<eta x>", 100, 0, 100, -2, 2 );
  h085 = fs->make<TProfile>( "h085", "PXB eta vs x in pixel inward ;x in pixel [#mum];<eta x>", 100, 0, 100, -2, 2 );

  h086 = fs->make<TH1D>( "h086", "PXB x_{loc}, row = 1;x_{loc} [cm];hits", 164, -0.82, 0.82 );
  h087 = fs->make<TH1D>( "h087", "PXB x_{loc}, row = 2;x_{loc} [cm];hits", 164, -0.82, 0.82 );
  h088 = fs->make<TH1D>( "h088", "Align x error ", 100, 0., 100. );
  h089 = fs->make<TH1D>( "h089", "Align y error ", 100, 0., 100. );

  h090 = fs->make<TH1D>( "h090", "dxy ", 100, -1000., 1000.);
  h091 = fs->make<TH1D>( "h091", "dxy error", 100, 0., 100.);

  h100 = fs->make<TH1D>( "h100", "hits on tracks PXB layer;PXB layer;hits", 6, -0.5, 5.5 );

  h101 = fs->make<TH1D>( "h101", "hits on tracks PXB1 ladder;ladder;hits", 22, -0.5, 21.5 );
  h102 = fs->make<TH1D>( "h102", "hits on tracks PXB1 module;module;hits", 10, -0.5, 9.5 );
  h103 = fs->make<TH1D>( "h103", "hits on tracks PXB1 R;R [cm];hits", 150, 0, 15 );
  h104 = fs->make<TH1D>( "h104", "hits on tracks PXB1 #phi;#phi [deg];hits", 180, -180, 180 );
  h105 = fs->make<TH1D>( "h105", "hits on tracks PXB1 z;z [cm];hits", 600, -30, 30 );
  h106 = fs->make<TH2D>( "h106", "hits on tracks PXB1 #phi-z;#phi [deg];z [cm]", 180, -180, 180, 600, -30, 30 );
  h107 = fs->make<TH2D>( "h107", "hits local x-y PXB1;x [cm];y [cm]", 180, -0.9, 0.9, 440, -3.3, 3.3 );

  h111 = fs->make<TH2D>( "h111", "hits on tracks PXB1 x-y;x [cm];y [cm]", 240, -6, 6, 240, -6, 6 );
  h112 = fs->make<TH1D>( "h112", "residuals PXB1 #DeltaU;#DeltaU [#mum];hits", 100, -250, 250 );
  h113 = fs->make<TH1D>( "h113", "residuals PXB1 #Deltaz;#Deltaz [#mum];hits", 100, -250, 250 );
  h114 = fs->make<TH1D>( "h114", "residuals PXB1 #DeltaU;#DeltaU [#mum];hits", 100, -50, 50 );
  h115 = fs->make<TH1D>( "h115", "residuals PXB1 #Deltaz;#Deltaz [#mum];hits", 100, -250, 250 );

  h121 = fs->make<TProfile>( "h121", "rms(b_{xy}) vs x_{loc} PXB1, p_{t} > 8;x(PXB1) [cm];rms(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );

  h122 = fs->make<TProfile>( "h122", "rms(b_{xy}) vs x_{loc} PXB1, 0.8 < p_{t} < 1.2;x(PXB1) [cm];rms(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h123 = fs->make<TProfile>( "h123", "#sigma(b_{xy}) vs x_{loc} PXB1, 0.8 < p_{t} < 1.2;x(PXB1) [cm];#sigma(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h124 = fs->make<TProfile>( "h124", "rms(b_{xy}/#sigma) vs x_{loc} PXB1, 0.8 < p_{t} < 1.2;x(PXB1) [cm];rms(b_{xy}/#sigma)", 160, -0.8, 0.8, 0, 10 );

  h125 = fs->make<TProfile>( "h125", "rms(b_{xy}) vs x_{loc} PXB1, 1.2 < p_{t} < 1.6;x(PXB1) [cm];rms(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h126 = fs->make<TProfile>( "h126", "#sigma(b_{xy}) vs x_{loc} PXB1, 1.2 < p_{t} < 1.6;x(PXB1) [cm];#sigma(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h127 = fs->make<TProfile>( "h127", "rms(b_{xy}/#sigma) vs x_{loc} PXB1, 1.2 < p_{t} < 1.6;x(PXB1) [cm];rms(b_{xy}/#sigma)", 160, -0.8, 0.8, 0, 10 );

  h128 = fs->make<TProfile>( "h128", "rms(b_{xy}) vs x_{loc} PXB1, 1.6 < p_{t} < 2.2;x(PXB1) [cm];rms(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h129 = fs->make<TProfile>( "h129", "#sigma(b_{xy}) vs x_{loc} PXB1, 1.6 < p_{t} < 2.2;x(PXB1) [cm];#sigma(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h130 = fs->make<TProfile>( "h130", "rms(b_{xy}/#sigma) vs x_{loc} PXB1, 1.6 < p_{t} < 2.2;x(PXB1) [cm];rms(b_{xy}/#sigma)", 160, -0.8, 0.8, 0, 10 );

  h131 = fs->make<TProfile>( "h131", "rms(b_{xy}) vs x_{loc} PXB1, 2.2 < p_{t} < 4.0;x(PXB1) [cm];rms(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h132 = fs->make<TProfile>( "h132", "#sigma(b_{xy}) vs x_{loc} PXB1, 2.2 < p_{t} < 4.0;x(PXB1) [cm];#sigma(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h133 = fs->make<TProfile>( "h133", "rms(b_{xy}/#sigma) vs x_{loc} PXB1, 2.2 < p_{t} < 4.0;x(PXB1) [cm];rms(b_{xy}/#sigma)", 160, -0.8, 0.8, 0, 10 );

  h134 = fs->make<TProfile>( "h134", "rms(b_{xy}) vs x_{loc} PXB1, 4 < p_{t} < 10;x(PXB1) [cm];rms(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h135 = fs->make<TProfile>( "h135", "#sigma(b_{xy}) vs x_{loc} PXB1, 4 < p_{t} < 10;x(PXB1) [cm];#sigma(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h136 = fs->make<TProfile>( "h136", "rms(b_{xy}/#sigma) vs x_{loc} PXB1, 4 < p_{t} < 10;x(PXB1) [cm];rms(b_{xy}/#sigma)", 160, -0.8, 0.8, 0, 10 );

  h137 = fs->make<TProfile>( "h137", "rms(b_{xy}) vs x_{loc} PXB1, p_{t} > 10;x(PXB1) [cm];rms(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h138 = fs->make<TProfile>( "h138", "#sigma(b_{xy}) vs x_{loc} PXB1, p_{t} > 10;x(PXB1) [cm];#sigma(b_{xy}) [#mum]", 160, -0.8, 0.8, 0, 5000 );
  h139 = fs->make<TProfile>( "h139", "rms(b_{xy}/#sigma) vs x_{loc} PXB1, p_{t} > 10;x(PXB1) [cm];rms(b_{xy}/#sigma)", 160, -0.8, 0.8, 0, 10 );

  h201 = fs->make<TH1D>( "h201", "hits on tracks PXB2 ladder;ladder;hits", 34, -0.5, 33.5 );
  h202 = fs->make<TH1D>( "h202", "hits on tracks PXB2 module;module;hits", 10, -0.5, 9.5 );
  h203 = fs->make<TH1D>( "h203", "hits on tracks PXB2 R;R [cm];hits", 150, 0, 15 );
  h204 = fs->make<TH1D>( "h204", "hits on tracks PXB2 #phi;#phi [deg];hits", 180, -180, 180 );
  h205 = fs->make<TH1D>( "h205", "hits on tracks PXB2 z;z [cm];hits", 600, -30, 30 );
  h206 = fs->make<TH2D>( "h206", "hits on tracks PXB2 #phi-z;#phi [deg];z [cm]", 180, -180, 180, 600, -30, 30 );
  h207 = fs->make<TH2D>( "h207", "hits local x-y PXB2;x [cm];y [cm]", 180, -0.9, 0.9, 440, -3.3, 3.3 );

  // PXB2 triplet residuals:

  h210 = fs->make<TProfile>( "h210", "PXB2 #sigma_{x} vs x_{loc}, 0.8-1.2 GeV;x_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 160, -0.8, 0.8, 0, 499 );
  h211 = fs->make<TProfile>( "h211", "PXB2 #sigma_{x} vs x_{loc}, 1.2-1.6 GeV;x_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 160, -0.8, 0.8, 0, 499 );
  h212 = fs->make<TProfile>( "h212", "PXB2 #sigma_{x} vs x_{loc}, 1.6-2.2 GeV;x_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 160, -0.8, 0.8, 0, 499 );
  h213 = fs->make<TProfile>( "h213", "PXB2 #sigma_{x} vs x_{loc}, 2.2-4.0 GeV;x_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 160, -0.8, 0.8, 0, 499 );
  h214 = fs->make<TProfile>( "h214", "PXB2 #sigma_{x} vs x_{loc}, p_{t} > 4 GeV ;x_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 160, -0.8, 0.8, 0, 499 );

  h215 = fs->make<TProfile>( "h215", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h216 = fs->make<TProfile>( "h216", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h217 = fs->make<TProfile>( "h217", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h218 = fs->make<TProfile>( "h218", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h219 = fs->make<TProfile>( "h219", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );

  h220 = fs->make<TProfile>( "h220", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h221 = fs->make<TProfile>( "h221", "PXB2 #sigma_{x} vs z_{loc}, 1.2-1.6 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h222 = fs->make<TProfile>( "h222", "PXB2 #sigma_{x} vs z_{loc}, 1.6-2.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h223 = fs->make<TProfile>( "h223", "PXB2 #sigma_{x} vs z_{loc}, 2.2-4.0 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h224 = fs->make<TProfile>( "h224", "PXB2 #sigma_{x} vs z_{loc}, p_{t} > 4 GeV ;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );

  h225 = fs->make<TProfile>( "h225", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h226 = fs->make<TProfile>( "h226", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );

  h227 = fs->make<TH1D>( "h227", "PXB2 x_{pix}, nrow = 1;x_{cluster} mod 100 [#mum];hits", 100, 0, 100 );
  h228 = fs->make<TH1D>( "h228", "PXB2 x_{pix}, nrow = 2;x_{cluster} mod 100 [#mum];hits", 100, 0, 100 );
  h229 = fs->make<TH1D>( "h229", "PXB2 x_{pix}, nrow = 3;x_{cluster} mod 100 [#mum];hits", 100, 0, 100 );

  h230 = fs->make<TProfile>( "h230", "PXB2 #sigma_{z} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltaz) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h231 = fs->make<TProfile>( "h231", "PXB2 #sigma_{z} vs z_{loc}, 1.2-1.6 GeV;z_{loc} [cm];PXB2 rms(#Deltaz) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h232 = fs->make<TProfile>( "h232", "PXB2 #sigma_{z} vs z_{loc}, 1.6-2.2 GeV;z_{loc} [cm];PXB2 rms(#Deltaz) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h233 = fs->make<TProfile>( "h233", "PXB2 #sigma_{z} vs z_{loc}, 2.2-4.0 GeV;z_{loc} [cm];PXB2 rms(#Deltaz) [#mum]", 432, -3.24, 3.24, 0, 499 );
  h234 = fs->make<TProfile>( "h234", "PXB2 #sigma_{z} vs z_{loc}, p_{t} > 4 GeV ;z_{loc} [cm];PXB2 rms(#Deltaz) [#mum]", 432, -3.24, 3.24, 0, 499 );

  h235 = fs->make<TH1D>( "h235", "PXB2 x_{pix};x_{cluster} mod 100 [#mum];hits", 100, 0, 100 );
  h236 = fs->make<TH1D>( "h236", "PXB2 v_{pix};v_{pix} [#mum];hits", 100, 0, 100 );
  h237 = fs->make<TH1D>( "h237", "PXB2 v_{pix}, nrow = 1;v_{pix} [#mum];hits", 100, 0, 100 );
  h238 = fs->make<TH1D>( "h238", "PXB2 v_{pix}, nrow = 2;v_{pix} [#mum];hits", 100, 0, 100 );
  h239 = fs->make<TH1D>( "h239", "PXB2 v_{pix}, nrow = 3;v_{pix} [#mum];hits", 100, 0, 100 );

  h240 = fs->make<TProfile>( "h240", "PXB2 rows vs t_{pix};x_{track} mod 200 [#mum];PXB2 <rows>", 100, 0, 200, 0, 10 );
  h241 = fs->make<TProfile>( "h241", "PXB2 #Deltax vs x_{pix};x_{cluster} mod 100 [#mum];<PXB2 #Deltax> [#mum]", 100, 0, 100, -99, 99 );
  h242 = fs->make<TProfile>( "h242", "PXB2 #sigma_{x} vs x_{pix};x_{cluster} mod 200 [#mum];PXB2 rms(#Deltax) [#mum]", 100, 0, 200, 0, 99 );
  h243 = fs->make<TProfile>( "h243", "PXB2 #Deltax vs l_{pix};x_{track} mod 100 [#mum];<PXB2 #Deltax> [#mum]", 100, 0, 100, -99, 99 );
  h244 = fs->make<TProfile>( "h244", "PXB2 #sigma_{x} vs t_{pix};x_{track} mod 200 [#mum];PXB2 rms(#Deltax) [#mum]", 100, 0, 200, 0, 99 );

  h245 = fs->make<TProfile>( "h245", "PXB2 eta vs t_{pix};x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h246 = fs->make<TProfile>( "h246", "PXB2 eta vs t_{pix} outward;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h247 = fs->make<TProfile>( "h247", "PXB2 eta vs t_{pix} inward ;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h248 = fs->make<TProfile>( "h248", "PXB2 eta vs x_{pix};x_{cluster} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );

  h249 = fs->make<TH1D>( "h249", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h250 = fs->make<TH1D>( "h250", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h251 = fs->make<TH1D>( "h251", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h252 = fs->make<TH1D>( "h252", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h253 = fs->make<TH1D>( "h253", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h254 = fs->make<TH1D>( "h254", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h255 = fs->make<TH1D>( "h255", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h256 = fs->make<TH1D>( "h256", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h257 = fs->make<TH1D>( "h257", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h258 = fs->make<TH1D>( "h258", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h259 = fs->make<TH1D>( "h259", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );

  h260 = fs->make<TH1D>( "h260", "PXB inc ang;inc ang [deg];hits", 80, -20, 20 );
  h261 = fs->make<TProfile>( "h261", "PXB2 eta vs t_{pix},       inc < -10;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h262 = fs->make<TProfile>( "h262", "PXB2 eta vs t_{pix}, -10 < inc <  -5;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h263 = fs->make<TProfile>( "h263", "PXB2 eta vs t_{pix},  -5 < inc <   0;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h264 = fs->make<TProfile>( "h264", "PXB2 eta vs t_{pix},   0 < inc <   5;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h265 = fs->make<TProfile>( "h265", "PXB2 eta vs t_{pix},   5 < inc <  10;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );
  h266 = fs->make<TProfile>( "h266", "PXB2 eta vs t_{pix},  10 < inc      ;x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );

  h267 = fs->make<TProfile>( "h267", "PXB2 #sigma_{x} vs t_{pix};x_{track} mod 200 [#mum];PXB2 rms(#Deltax) [#mum]", 100, 0, 200, 0, 99 );
  h268 = fs->make<TProfile>( "h268", "PXB2 #sigma_{x} vs l_{pix};x_{track} mod 100 [#mum];PXB2 rms(#Deltax) [#mum]", 100, 0, 100, 0, 99 );

  h269 = fs->make<TH1D>( "h269", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h270 = fs->make<TH1D>( "h270", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h271 = fs->make<TH1D>( "h271", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h272 = fs->make<TH1D>( "h272", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h273 = fs->make<TH1D>( "h273", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h274 = fs->make<TH1D>( "h274", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h275 = fs->make<TH1D>( "h275", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h276 = fs->make<TH1D>( "h276", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h277 = fs->make<TH1D>( "h277", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h278 = fs->make<TH1D>( "h278", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );
  h279 = fs->make<TH1D>( "h279", "PXB cluster etaX;PXB cluster etaX;hits", 100, -1, 1 );

  h280 = fs->make<TH1D>( "h280", "PXB2 x_{pix}, eta < -0.3;x_{cluster} mod 100 [#mum];hits", 100, 0, 100 );
  h281 = fs->make<TH1D>( "h281", "PXB2 x_{pix}, -0.3 eta < 0.3;x_{cluster} mod 100 [#mum];hits", 100, 0, 100 );
  h282 = fs->make<TH1D>( "h282", "PXB2 x_{pix}, eta > 0.3;x_{cluster} mod 100 [#mum];hits", 100, 0, 100 );

  h283 = fs->make<TProfile>( "h283", "PXB2 x_{com} vs t_{pix}, p_{t} > 4 GeV ;x_{track} mod 200 [#mum];PXB2 rms(x_{com}-xtrp) [#mum]", 100, 0, 200, 0, 499 );
  h284 = fs->make<TH1D>( "h284", "PXB2 residuals dcx, p_{t} > 12;PXB2 dcx [#mum];hits", 100, -150, 150 );

  h285 = fs->make<TH1D>( "h285", "PXB2 m_{pix};m_{pix} [#mum];hits", 100, 0, 100 );
  h286 = fs->make<TProfile>( "h286", "PXB2 x_{my} vs x_{loc}, p_{t} > 4 GeV ;x_{loc} [cm];PXB2 <x_{my}-x_{loc}> [#mum]", 160, -0.8, 0.8, -99, 99 );
  h287 = fs->make<TProfile>( "h287", "PXB2 x_{my} vs xtrp, p_{t} > 4 GeV ;xtrp [cm];PXB2 <x_{my}-xtrp> [#mum]", 160, -0.8, 0.8, -99, 99 );
  h288 = fs->make<TProfile>( "h288", "PXB2 x_{my} vs l_{pix}, p_{t} > 4 GeV ;x_{track} mod 100 [#mum];PXB2 <x_{my}-xtrp> [#mum]", 100, 0, 100, -99, 99 );
  h289 = fs->make<TProfile>( "h289", "PXB2 x_{my} vs t_{pix}, p_{t} > 4 GeV ;x_{track} mod 200 [#mum];PXB2 rms(x_{my}-xtrp) [#mum]", 100, 0, 200, 0, 499 );

  h290 = fs->make<TH1D>( "h290", "PXB2 residuals #Deltax, p_{t} > 12, 1-row;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h291 = fs->make<TH1D>( "h291", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h292 = fs->make<TH1D>( "h292", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h293 = fs->make<TH1D>( "h293", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h294 = fs->make<TH1D>( "h294", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h295 = fs->make<TH1D>( "h295", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h296 = fs->make<TH1D>( "h296", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h297 = fs->make<TH1D>( "h297", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h298 = fs->make<TH1D>( "h298", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h299 = fs->make<TH1D>( "h299", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h300 = fs->make<TH1D>( "h300", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  // PXB3 hits:

  h301 = fs->make<TH1D>( "h301", "hits on tracks PXB3 ladder;ladder;hits", 46, -0.5, 45.5 );
  h302 = fs->make<TH1D>( "h302", "hits on tracks PXB3 module;module;hits", 10, -0.5, 9.5 );
  h303 = fs->make<TH1D>( "h303", "hits on tracks PXB3 R;R [cm];hits", 150, 0, 15 );
  h304 = fs->make<TH1D>( "h304", "hits on tracks PXB3 #phi;#phi [deg];hits", 180, -180, 180 );
  h305 = fs->make<TH1D>( "h305", "hits on tracks PXB3 z;z [cm];hits", 600, -30, 30 );
  h306 = fs->make<TH2D>( "h306", "hits on tracks PXB3 #phi-z;#phi [deg];z [cm]", 180, -180, 180, 600, -30, 30 );
  h307 = fs->make<TH2D>( "h307", "hits local x-y PXB3;x [cm];y [cm]", 180, -0.9, 0.9, 440, -3.3, 3.3 );

  h308 = fs->make<TH1D>( "h308", "PXB2 lever arm;extrapolation factor to PXB2;tracks", 100, 0, 1 );

  // pile up study:

  h310 = fs->make<TH1D>( "h310", "PXB2 #Deltax 1-2 vtx;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h311 = fs->make<TH1D>( "h311", "PXB2 #Deltaz 1-2 vtx;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h312 = fs->make<TH1D>( "h312", "PXB2 #Deltax 3-5 vtx;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h313 = fs->make<TH1D>( "h313", "PXB2 #Deltaz 3-5 vtx;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h314 = fs->make<TH1D>( "h314", "PXB2 #Deltax 6-9 vtx;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h315 = fs->make<TH1D>( "h315", "PXB2 #Deltaz 6-9 vtx;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h316 = fs->make<TH1D>( "h316", "PXB2 #Deltax > 9 vtx;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h317 = fs->make<TH1D>( "h317", "PXB2 #Deltaz > 9 vtx;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h318 = fs->make<TH1D>( "h318", "vertcies > 9 vtx;vertcies;events", 31, -0.5, 30.5 );

  h319 = fs->make<TProfile>( "h319", "PXB2 #sigma_{x} vs inc;#phi_{inc2} [deg];PXB2 rms(#Deltax) [#mum]", 40, -10, 10, 0, 99 );

  h320 = fs->make<TProfile>( "h320", "PXB2 #sigma_{x} vs z_{pix}, p_{t} > 4 GeV;z_{track} mod 150 [#mum];PXB2 rms(#Deltax) [#mum]",  75, 0, 150, 0, 499 );
  h321 = fs->make<TProfile>( "h321", "PXB2 #sigma_{x} vs s_{pix}, p_{t} > 4 GeV;x_{cluster} mod 200 [#mum];PXB2 rms(#Deltax) [#mum]", 150, 0, 300, 0, 499 );

  h322 = fs->make<TProfile>( "h322", "PXB2 charge vs t_{pix};x_{track} mod 200 [#mum];<charge*cos(dip)> [ke]", 100, 0, 200, 0, 199 );
  h323 = fs->make<TH2D>( "h323", "PXB2 t_{pix} vs s_{pix};x_{cluster} mod 200 [#mum];x_{track} mod 200 [#mum]", 150, 0, 300, 100, 0, 200 );
  h324 = fs->make<TH2D>( "h324", "PXB2 t_{pix} vs s_{pix} dip < 10;x_{cluster} mod 200 [#mum];x_{track} mod 200 [#mum]", 150, 0, 300, 100, 0, 200 );

  h327 = fs->make<TProfile>( "h327", "PXB2 #sigma_{x} vs z_{loc}, 0.8-1.2 GeV;z_{loc} [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );

  h330 = fs->make<TProfile>( "h330", "PXB2 eta vs t_{pix};x_{track} mod 200 [#mum];PXB2 <eta>", 100, 0, 200, -2, 2 );

  h331 = fs->make<TProfile>( "h331", "PXB2 hit error vs t_{pix};x_{track} mod 200 [#mum];PXB2 hit <error x> [#mum]", 100, 0, 200, 0, 99 );
  h332 = fs->make<TH1D>( "h332", "PXB2 hit error;PXB2 hit error x [#mum]", 100, 0, 100 );

  h334 = fs->make<TProfile>( "h334", "PXB2 COG #sigma_{x} vs inc;#phi_{inc2} [deg];PXB2 COG rms(#Deltax) [#mum]", 40, -10, 10, 0, 99 );

  h343 = fs->make<TProfile>( "h343", "PXB2 #Deltax vs l_{pix}, nrow = 2;x_{track} mod 100 [#mum];<PXB2 #Deltax> [#mum]", 100, 0, 100, -99, 99 );

  h374 = fs->make<TH1D>( "h374", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h375 = fs->make<TH1D>( "h375", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h376 = fs->make<TH1D>( "h376", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h377 = fs->make<TH1D>( "h377", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  h379 = fs->make<TH1D>( "h379", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h380 = fs->make<TH1D>( "h380", "PXB2 residuals COG #Deltax, p_{t} > 12;PXB2 COG #Deltax [#mum];hits", 100, -150, 150 );
  h381 = fs->make<TH1D>( "h381", "PXB2 residuals COG #Deltax, p_{t} > 12;PXB2 COG #Deltax [#mum];hits", 100, -150, 150 );
  h382 = fs->make<TH1D>( "h382", "PXB2 residuals COG #Deltax, p_{t} > 12;PXB2 COG #Deltax [#mum];hits", 100, -150, 150 );
  h383 = fs->make<TH1D>( "h383", "PXB2 residuals COG #Deltax, p_{t} > 12;PXB2 COG #Deltax [#mum];hits", 100, -150, 150 );

  h390 = fs->make<TH1D>( "h390", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h391 = fs->make<TH1D>( "h391", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h392 = fs->make<TH1D>( "h392", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h393 = fs->make<TH1D>( "h393", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h394 = fs->make<TH1D>( "h394", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h395 = fs->make<TH1D>( "h395", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h396 = fs->make<TH1D>( "h396", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h397 = fs->make<TH1D>( "h397", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h398 = fs->make<TH1D>( "h398", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h399 = fs->make<TH1D>( "h399", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  // triplets 3+1 -> 2:

  h401 = fs->make<TH1D>( "h401", "PXB2 triplets z2;z [cm];hits", 600, -30, 30 );
  h402 = fs->make<TH1D>( "h402", "PXB2 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h403 = fs->make<TH1D>( "h403", "PXB2 udca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );
  h404 = fs->make<TH1D>( "h404", "PXB2 udip-dip;#Deltadip;tracks", 100, -0.1, 0.1 );
  h405 = fs->make<TH1D>( "h405", "PXB2 uz0-z0;#Deltaz_{0};tracks", 100, -0.1, 0.1 );

  h406 = fs->make<TH1D>( "h406", "valid tracker hits;tracker hits;tracks", 51, -0.5, 50.5 );
  h407 = fs->make<TH1D>( "h407", "valid pixel barrel hits;valid pixel barrel hits;tracks", 6, -0.5, 5.5 );
  h408 = fs->make<TH1D>( "h408", "tracker layers;tracker layers;tracks", 31, -0.5, 30.5 );

  h409 = fs->make<TProfile>( "h409", "PXB2 angle of incidence;PXB2 #phi [deg];PXB2 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h410 = fs->make<TH1D>( "h410", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h411 = fs->make<TH1D>( "h411", "PXB2 residuals #Deltaz;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );

  // mean resid profiles:

  h412 = fs->make<TProfile>( "h412", "PXB2 #Deltax vs #phi;#phi_{2} [deg];PXB2 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h412_out_zplus = fs->make<TProfile>( "h412_out_zplus", "PXB2 #Deltax vs #phi, outward-facing modules, z+;#phi_{2} [deg];PXB2 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h412_out_zminus = fs->make<TProfile>( "h412_out_zminus", "PXB2 #Deltax vs #phi, outward-facing modules, z-;#phi_{2} [deg];PXB2 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h412_in_zplus = fs->make<TProfile>( "h412_in_zplus", "PXB2 #Deltax vs #phi, inward-facing modules, z+;#phi_{2} [deg];PXB2 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h412_in_zminus = fs->make<TProfile>( "h412_in_zminus", "PXB2 #Deltax vs #phi, inward-facing modules, z-;#phi_{2} [deg];PXB2 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h413 = fs->make<TProfile>( "h413", "PXB2 #Deltaz vs #phi;#phi_{2} [deg];PXB2 <#Deltaz> [#mum]", 180, -180, 180, -199, 199 );

  h414 = fs->make<TProfile>( "h414", "PXB2 #Deltax vs z;z2 [cm];PXB2 <#Deltax> [#mum]", 80, -20, 20, -99, 99 );
  h415 = fs->make<TProfile>( "h415", "PXB2 #Deltaz vs z;z2 [cm];PXB2 <#Deltaz> [#mum]", 80, -20, 20, -199, 199 );

  h416 = fs->make<TProfile>( "h416", "PXB2 #Deltax vs p_{t};log(p_{t} [GeV]);PXB2 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );
  h417 = fs->make<TProfile>( "h417", "PXB2 #Deltaz vs p_{t};log(p_{t} [GeV]);PXB2 <#Deltaz> [#mum]", 20, 0, 2, -199, 199 );

  h418 = fs->make<TProfile>( "h418", "PXB2 #Deltax vs p_{t} +;log(p_{t} [GeV]);PXB2 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );
  h419 = fs->make<TProfile>( "h419", "PXB2 #Deltax vs p_{t} -;log(p_{t} [GeV]);PXB2 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );

  h420 = fs->make<TH1D>( "h420", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h420_out_zplus = fs->make<TH1D>( "h420_out_zplus", "PXB2 residuals #Deltax, p_{t} > 12, outward-facing modules, z+;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h420_out_zminus = fs->make<TH1D>( "h420_out_zminus", "PXB2 residuals #Deltax, p_{t} > 12, outward-facing modules, z-;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h420_in_zplus = fs->make<TH1D>( "h420_in_zplus", "PXB2 residuals #Deltax, p_{t} > 12, inward-facing modules, z+;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h420_in_zminus = fs->make<TH1D>( "h420_in_zminus", "PXB2 residuals #Deltax, p_{t} > 12, inward-facing modules, z-;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h421 = fs->make<TH1D>( "h421", "PXB2 residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );

  h420_1 = fs->make<TH1D>( "h420_1", "PXB2 residuals #Deltax, p_{t} > 12, lever 1;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h420_2 = fs->make<TH1D>( "h420_2", "PXB2 residuals #Deltax, p_{t} > 12, lever 2;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h420_3 = fs->make<TH1D>( "h420_3", "PXB2 residuals #Deltax, p_{t} > 12, lever 3;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  h420_mod1 = fs->make<TH1D>( "h420_mod1", "PXB2 Module 1 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  h420_mod2 = fs->make<TH1D>( "h420_mod2", "PXB2 Module 2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  h420_mod3 = fs->make<TH1D>( "h420_mod3", "PXB2 Module 3 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  h420_mod4 = fs->make<TH1D>( "h420_mod4", "PXB2 Module 4 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  h420_mod5 = fs->make<TH1D>( "h420_mod5", "PXB2 Module 5 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  h420_mod6 = fs->make<TH1D>( "h420_mod6", "PXB2 Module 6 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  h420_mod7 = fs->make<TH1D>( "h420_mod7", "PXB2 Module 7 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  h420_mod8 = fs->make<TH1D>( "h420_mod8", "PXB2 Module 8 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits ", 100, -150, 150 );
  
  h421_mod1 = fs->make<TH1D>( "h421_mod1", "PXB2 Module 1  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h421_mod2 = fs->make<TH1D>( "h421_mod2", "PXB2 Module 2  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h421_mod3 = fs->make<TH1D>( "h421_mod3", "PXB2 Module 3  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h421_mod4 = fs->make<TH1D>( "h421_mod4", "PXB2 Module 4  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h421_mod5 = fs->make<TH1D>( "h421_mod5", "PXB2 Module 5  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h421_mod6 = fs->make<TH1D>( "h421_mod6", "PXB2 Module 6  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h421_mod7 = fs->make<TH1D>( "h421_mod7", "PXB2 Module 7  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h421_mod8 = fs->make<TH1D>( "h421_mod8", "PXB2 Module 8  residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  // width profiles:

  h422 = fs->make<TProfile>( "h422", "PXB2 #sigma_{x} vs #phi;#phi_{2} [deg];PXB2 rms(#Deltax) [#mum]", 360, -180, 180, 0, 99 );
  h423 = fs->make<TProfile>( "h423", "PXB2 #sigma_{z} vs #phi;#phi_{2} [deg];PXB2 rms(#Deltaz) [#mum]", 360, -180, 180, 0, 199 );

  h424 = fs->make<TProfile>( "h424", "PXB2 #sigma_{x} vs z;z2 [cm];PXB2 rms(#Deltax) [#mum]", 80, -20, 20, 0, 99 );
  h425 = fs->make<TProfile>( "h425", "PXB2 #sigma_{z} vs z;z2 [cm];PXB2 rms(#Deltaz) [#mum]", 80, -20, 20, 0, 199 );

  h426 = fs->make<TProfile>( "h426", "PXB2 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB2 rms(#Deltax) [#mum]", 20, 0, 2, 0, 99 );
  h427 = fs->make<TProfile>( "h427", "PXB2 #sigma_{z} vs p_{t};log(p_{t} [GeV]);PXB2 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  h428 = fs->make<TProfile>( "h428", "PXB2 #sigma_{x} vs dip;dip [deg];PXB2 rms(#Deltax) [#mum]", 70, -70, 70, 0, 99 );
  h429 = fs->make<TProfile>( "h429", "PXB2 #sigma_{z} vs dip;dip [deg];PXB2 rms(#Deltaz) [#mum]", 70, -70, 70, 0, 199 );
  h429_eta = fs->make<TProfile>( "h429_eta", "PXB2 #sigma_{z} vs #eta;Track #eta;PXB2 rms(#Deltaz) [#mum]", 70, -1.5, 1.5, 0, 199 );

  h430 = fs->make<TH1D>( "h430", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h431 = fs->make<TH1D>( "h431", "PXB2 residuals #Deltaz;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
				      
  h432 = fs->make<TH1D>( "h432", "PXB2 residuals #Deltaz, 18 < |dip| < 50;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h433 = fs->make<TProfile>( "h433", "PXB2 #sigma_{z} vs p_{t}, best dip;log(p_{t} [GeV]);PXB2 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  h434 = fs->make<TProfile>( "h434", "PXB2 #sigma_{x} vs inc;#phi_{inc2} [deg];PXB2 rms(#Deltax) [#mum]", 40, -10, 10, 0, 99 );

  h435 = fs->make<TProfile>( "h435", "PXB2 #sigma_{x} vs #phi at 1 GeV;#phi_{2} [deg];PXB2 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h436 = fs->make<TProfile>( "h436", "PXB2 #sigma_{x} vs inc;#phi_{inc2} [deg];PXB2 rms(#Deltax) [#mum]", 40, -10, 10, 0, 99 );
  h437 = fs->make<TProfile>( "h437", "PXB2 #sigma_{x} vs inc;#phi_{inc2} [deg];PXB2 rms(#Deltax) [#mum]", 40, -10, 10, 0, 99 );

  h438 = fs->make<TH1D>( "h438", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h439 = fs->make<TH1D>( "h439", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  h440 = fs->make<TH1D>( "h440", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h441 = fs->make<TH1D>( "h441", "PXB2 residuals #Deltaz;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
				      
  h442 = fs->make<TH1D>( "h442", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h443 = fs->make<TH1D>( "h443", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h444 = fs->make<TH1D>( "h444", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h445 = fs->make<TH1D>( "h445", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h446 = fs->make<TH1D>( "h446", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h447 = fs->make<TH1D>( "h447", "PXB2 cluster rows;PXB2 cluster rows;hits", 10, 0.5, 10.5 );

  h448 = fs->make<TH1D>( "h448", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h449 = fs->make<TH1D>( "h449", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  // triplets 3+1 -> 2 with refitted kappa:

  h451 = fs->make<TH1D>( "h451", "PXB2 triplets z2;z [cm];hits", 600, -30, 30 );
  h452 = fs->make<TH1D>( "h452", "PXB2 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h453 = fs->make<TH1D>( "h453", "PXB2 udca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );
  h454 = fs->make<TH1D>( "h454", "PXB2 udip-dip;#Deltadip;tracks", 100, -0.1, 0.1 );
  h455 = fs->make<TH1D>( "h455", "PXB2 uz0-z0;#Deltaz_{0};tracks", 100, -0.1, 0.1 );
  h456 = fs->make<TH1D>( "h456", "PXB2 (kap2-kap)/kap;(#kappa_{2}-#kappa)/#kappa;tracks", 100, -0.02, 0.02 );

  h459 = fs->make<TProfile>( "h459", "PXB2 angle of incidence;PXB2 #phi [deg];PXB2 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h460 = fs->make<TH1D>( "h460", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h461 = fs->make<TH1D>( "h461", "PXB2 residuals #Deltaz;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );

  // mean resid profiles:

  h462 = fs->make<TProfile>( "h462", "PXB2 #Deltax vs #phi;#phi_{2} [deg];PXB2 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h463 = fs->make<TProfile>( "h463", "PXB2 #Deltaz vs #phi;#phi_{2} [deg];PXB2 <#Deltaz> [#mum]", 180, -180, 180, -199, 199 );

  h464 = fs->make<TProfile>( "h464", "PXB2 #Deltax vs z;z2 [cm];PXB2 <#Deltax> [#mum]", 80, -20, 20, -99, 99 );
  h465 = fs->make<TProfile>( "h465", "PXB2 #Deltaz vs z;z2 [cm];PXB2 <#Deltaz> [#mum]", 80, -20, 20, -199, 199 );

  h466 = fs->make<TProfile>( "h466", "PXB2 #Deltax vs p_{t};log(p_{t} [GeV]);PXB2 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );
  h467 = fs->make<TProfile>( "h467", "PXB2 #Deltaz vs p_{t};log(p_{t} [GeV]);PXB2 <#Deltaz> [#mum]", 20, 0, 2, -199, 199 );

  h468 = fs->make<TProfile>( "h468", "PXB2 #Deltax vs p_{t} +;log(p_{t} [GeV]);PXB2 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );
  h469 = fs->make<TProfile>( "h469", "PXB2 #Deltax vs p_{t} -;log(p_{t} [GeV]);PXB2 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );

  h470 = fs->make<TH1D>( "h470", "PXB2 residuals #Deltax, p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h471 = fs->make<TH1D>( "h471", "PXB2 residuals #Deltaz, p_{t} > 12;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );

  h470_1 = fs->make<TH1D>( "h470_1", "PXB2 residuals #Deltax, p_{t} > 12, lever1;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h470_2 = fs->make<TH1D>( "h470_2", "PXB2 residuals #Deltax, p_{t} > 12, lever2;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h470_3 = fs->make<TH1D>( "h470_3", "PXB2 residuals #Deltax, p_{t} > 12, lever3;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  // width profiles:

  h472 = fs->make<TProfile>( "h472", "PXB2 #sigma_{x} vs #phi;#phi_{2} [deg];PXB2 rms(#Deltax) [#mum]", 360, -180, 180, 0, 99 );
  h473 = fs->make<TProfile>( "h473", "PXB2 #sigma_{z} vs #phi;#phi_{2} [deg];PXB2 rms(#Deltaz) [#mum]", 360, -180, 180, 0, 199 );

  h474 = fs->make<TProfile>( "h474", "PXB2 #sigma_{x} vs z;z2 [cm];PXB2 rms(#Deltax) [#mum]", 80, -20, 20, 0, 99 );
  h475 = fs->make<TProfile>( "h475", "PXB2 #sigma_{z} vs z;z2 [cm];PXB2 rms(#Deltaz) [#mum]", 80, -20, 20, 0, 199 );

  h476 = fs->make<TProfile>( "h476", "PXB2 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB2 rms(#Deltax) [#mum]", 20, 0, 2, 0, 99 );
  h477 = fs->make<TProfile>( "h477", "PXB2 #sigma_{z} vs p_{t};log(p_{t} [GeV]);PXB2 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  h478 = fs->make<TProfile>( "h478", "PXB2 #sigma_{x} vs dip;dip [deg];PXB2 rms(#Deltax) [#mum]", 70, -70, 70, 0, 99 );
  h479 = fs->make<TProfile>( "h479", "PXB2 #sigma_{z} vs dip;dip [deg];PXB2 rms(#Deltaz) [#mum]", 70, -70, 70, 0, 199 );

  h480 = fs->make<TH1D>( "h480", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h481 = fs->make<TH1D>( "h481", "PXB2 residuals #Deltaz;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
				      
  h482 = fs->make<TH1D>( "h482", "PXB2 residuals #Deltaz, 18 < |dip| < 50;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
  h483 = fs->make<TProfile>( "h483", "PXB2 #sigma_{z} vs p_{t}, best dip;log(p_{t} [GeV]);PXB2 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  h484 = fs->make<TProfile>( "h484", "PXB2 #sigma_{x} vs inc;#phi_{inc2} [deg];PXB2 rms(#Deltax) [#mum]", 40, -10, 10, 0, 99 );

  h485 = fs->make<TProfile>( "h485", "PXB2 #sigma_{x} vs #phi at 1 GeV;#phi_{2} [deg];PXB2 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h490 = fs->make<TH1D>( "h490", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h491 = fs->make<TH1D>( "h491", "PXB2 residuals #Deltaz;PXB2 #Deltaz [#mum];hits", 100, -300, 300 );
				      
  h492 = fs->make<TH1D>( "h492", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h493 = fs->make<TH1D>( "h493", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h494 = fs->make<TH1D>( "h494", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h495 = fs->make<TH1D>( "h495", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  h496 = fs->make<TH1D>( "h496", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );

  // 2+3 -> 1 triplets:

  h501 = fs->make<TH1D>( "h501", "PXB1 triplets z1;PXB1 z [cm];hits", 600, -30, 30 );
  h502 = fs->make<TH1D>( "h502", "PXB1 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h503 = fs->make<TH1D>( "h503", "PXB1 udca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );
  h504 = fs->make<TH1D>( "h504", "PXB1 udip-dip;#Deltadip;tracks", 100, -0.1, 0.1 );
  h505 = fs->make<TH1D>( "h505", "PXB1 uz0-z0;#Deltaz_{0};tracks", 100, -0.1, 0.1 );

  h508 = fs->make<TH1D>( "h508", "PXB1 lever arm;extrapolation factor to PXB1;tracks", 100, 1, 2 );

  h509 = fs->make<TProfile>( "h509", "PXB1 angle of incidence;PXB1 #phi [deg];PXB1 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h510 = fs->make<TH1D>( "h510", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h511 = fs->make<TH1D>( "h511", "PXB1 residuals #Deltaz;PXB1 #Deltaz [#mum];hits", 100, -300, 300 );

  // mean resid profiles:

  h512 = fs->make<TProfile>( "h512", "PXB1 #Deltax vs #phi;PXB1 #phi [deg];PXB1 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  h513 = fs->make<TProfile>( "h513", "PXB1 #Deltaz vs #phi;PXB1 #phi [deg];PXB1 <#Deltaz> [#mum]", 180, -180, 180, -199, 199 );

  h514 = fs->make<TProfile>( "h514", "PXB1 #Deltax vs z;PXB1 z [cm];PXB1 <#Deltax> [#mum]", 80, -20, 20, -199, 199 );
  h515 = fs->make<TProfile>( "h515", "PXB1 #Deltaz vs z;PXB1 z [cm];PXB1 <#Deltaz>  [#mum]", 80, -20, 20, -199, 199 );

  h516 = fs->make<TProfile>( "h516", "PXB1 #Deltax vs p_{t};log(p_{t} [GeV]);PXB1 <#Deltax> [#mum]", 20, 0, 2, -199, 199 );
  h517 = fs->make<TProfile>( "h517", "PXB1 #Deltaz vs p_{t};log(p_{t} [GeV]);PXB1 <#Deltaz>  [#mum]", 20, 0, 2, -199, 199 );

  h518 = fs->make<TProfile>( "h518", "PXB1 #Deltax vs p_{t} +;log(p_{t} [GeV]);PXB1 <#Deltax> [#mum]", 20, 0, 2, -199, 199 );
  h519 = fs->make<TProfile>( "h519", "PXB1 #Deltax vs p_{t} -;log(p_{t} [GeV]);PXB1 <#Deltax> [#mum]", 20, 0, 2, -199, 199 );

  h520 = fs->make<TH1D>( "h520", "PXB1 residuals #Deltax, p_{t} > 12;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h521 = fs->make<TH1D>( "h521", "PXB1 residuals #Deltaz, p_{t} > 12;PXB1 #Deltaz [#mum];hits", 100, -300, 300 );

  h520_1 = fs->make<TH1D>( "h520_1", "PXB1 residuals #Deltax, p_{t} > 12, lever 1;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h520_2 = fs->make<TH1D>( "h520_2", "PXB1 residuals #Deltax, p_{t} > 12, lever 2;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h520_3 = fs->make<TH1D>( "h520_3", "PXB1 residuals #Deltax, p_{t} > 12, lever 3;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h520_4 = fs->make<TH1D>( "h520_4", "PXB1 residuals #Deltax, p_{t} > 12, lever 4;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h520_5 = fs->make<TH1D>( "h520_5", "PXB1 residuals #Deltax, p_{t} > 12, lever 5;PXB1 #Deltax [#mum];hits", 100, -150, 150 );

  // width profiles:

  h522 = fs->make<TProfile>( "h522", "PXB1 #sigma_{x} vs #phi;PXB1 #phi [deg];PXB1 rms(#Deltax) [#mum]", 360, -180, 180, 0, 199 );
  h523 = fs->make<TProfile>( "h523", "PXB1 #sigma_{z} vs #phi;PXB1 #phi [deg];PXB1 rms(#Deltaz)  [#mum]", 360, -180, 180, 0, 199 );

  h524 = fs->make<TProfile>( "h524", "PXB1 #sigma_{x} vs z;PXB1 z [cm];PXB1 rms(#Deltax) [#mum]", 80, -20, 20, 0, 199 );
  h525 = fs->make<TProfile>( "h525", "PXB1 #sigma_{z} vs z;PXB1 z [cm];PXB1 rms(#Deltaz) [#mum]", 80, -20, 20, 0, 199 );

  h526 = fs->make<TProfile>( "h526", "PXB1 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB1 rms(#Deltax) [#mum]", 20, 0, 2, 0, 199 );
  h527 = fs->make<TProfile>( "h527", "PXB1 #sigma_{z} vs p_{t};log(p_{t} [GeV]);PXB1 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  h528 = fs->make<TProfile>( "h528", "PXB1 #sigma_{x} vs dip;dip [deg];PXB1 rms(#Deltax) [#mum]", 70, -70, 70, 0, 199 );
  h529 = fs->make<TProfile>( "h529", "PXB1 #sigma_{z} vs dip;dip [deg];PXB1 rms(#Deltaz) [#mum]", 70, -70, 70, 0, 199 );

  h530 = fs->make<TH1D>( "h530", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h531 = fs->make<TH1D>( "h531", "PXB1 residuals #Deltaz;PXB1 #Deltaz [#mum];hits", 100, -300, 300 );

  h532 = fs->make<TH1D>( "h532", "PXB1 residuals #Deltaz, 18 < |dip| < 50;PXB1 #Deltaz [#mum];hits", 100, -300, 300 );
  h533 = fs->make<TProfile>( "h533", "PXB1 #sigma_{z} vs p_{t}, best dip;log(p_{t} [GeV]);PXB1 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  h534 = fs->make<TProfile>( "h534", "PXB1 #sigma_{x} vs inc;PXB1 #phi_{inc} [deg];PXB1 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );

  h535 = fs->make<TProfile>( "h535", "PXB1 #sigma_{x} vs #phi at 1 GeV;PXB1 #phi [deg];PXB1 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h540 = fs->make<TH1D>( "h540", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h541 = fs->make<TH1D>( "h541", "PXB1 residuals #Deltaz;PXB1 #Deltaz [#mum];hits", 100, -300, 300 );

  h542 = fs->make<TH1D>( "h542", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h543 = fs->make<TH1D>( "h543", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h544 = fs->make<TH1D>( "h544", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h545 = fs->make<TH1D>( "h545", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h546 = fs->make<TH1D>( "h546", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );

  // pixel track Karimaki:

  h557 = fs->make<TH1D>( "h557", "ukap-kap;#Delta#kappa;tracks", 100, -0.01, 0.01 );
  h558 = fs->make<TH1D>( "h558", "uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h559 = fs->make<TH1D>( "h559", "udca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );

  h560 = fs->make<TH1D>( "h560", "pixel track dcap, p_{t} > 4;dcap [#mum];hits", 100, -1000, 1000 );
  h561 = fs->make<TH1D>( "h561", "pixel track dcap, p_{t} > 8;dcap [#mum];hits", 100, -1000, 1000 );

  h562 = fs->make<TProfile>( "h562", "pixel track  dcap vs #phi;#phi_{2} [deg];<dcap> [#mum]", 180, -180, 180, -500, 499 );
  h563 = fs->make<TProfile>( "h563", "pixel tracks  dcap vs p_{t};log(p_{t} [GeV]);<dcap> [#mum]", 20, 0, 2, -500, 499 );

  h564 = fs->make<TProfile>( "h564", "pixel track #sigma(dcap) vs #phi;#phi_{2} [deg];#sigma(dcap) [#mum]", 180, -180, 180, 0, 499 );
  h565 = fs->make<TProfile>( "h565", "pixel tracks #sigma(dcap) vs p_{t};log(p_{t} [GeV]);#sigma(dcap) [#mum]", 20, 0, 2, 0, 499 );

  h570 = fs->make<TH1D>( "h570", "pixel track check dca1;PXB1 dca [um];hits", 100, -50, 50 );
  h571 = fs->make<TH1D>( "h571", "PXB1 #phi_{inc};PXB1 #phi_{inc} [deg];tracks", 80, -40, 40 );
  h572 = fs->make<TProfile>( "h572", "PXB1 p_{t} vs inc;PXB1 #phi_{inc} [deg];<p_{t}> [GeV]", 80, -40, 40, 0, 99 );
  h573 = fs->make<TProfile>( "h573", "PXB1 track sign vs inc;PXB1 #phi_{inc} [deg];<track charge>", 80, -40, 40, -2, 2 );
  h574 = fs->make<TProfile>( "h574", "PXB1 rows vs inc;PXB1 #phi_{inc} [deg];<rows/cluster>", 80, -40, 40, 0, 9 );
  h575 = fs->make<TProfile>( "h575", "PXB1 rows vs tan inc;PXB1 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  h576 = fs->make<TProfile>( "h576", "PXB1 rows vs tan inc;PXB1 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  h577 = fs->make<TProfile>( "h577", "PXB1 rows vs tan inc;PXB1 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );

  h580 = fs->make<TH1D>( "h580", "pixel track check dca2;PXB2 dca [um];hits", 100, -50, 50 );
  h581 = fs->make<TH1D>( "h581", "PXB2 #phi_{inc};PXB2 #phi_{inc} [deg];tracks", 80, -40, 40 );
  h582 = fs->make<TProfile>( "h582", "PXB2 p_{t} vs inc;PXB2 #phi_{inc} [deg];<p_{t}> [GeV]", 80, -40, 40, 0, 99 );
  h583 = fs->make<TProfile>( "h583", "PXB2 track sign vs inc;PXB2 #phi_{inc} [deg];<track charge>", 80, -40, 40, -2, 2 );
  h584 = fs->make<TProfile>( "h584", "PXB2 rows vs inc;PXB2 #phi_{inc} [deg];<rows/cluster>", 80, -40, 40, 0, 9 );
  h585 = fs->make<TProfile>( "h585", "PXB2 rows vs tan inc;PXB2 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  h586 = fs->make<TProfile>( "h586", "PXB2 rows vs tan inc;PXB2 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  h587 = fs->make<TProfile>( "h587", "PXB2 rows vs tan inc;PXB2 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );

  h590 = fs->make<TH1D>( "h590", "pixel track check dca3;PXB3 dca [um];hits", 100, -50, 50 );
  h591 = fs->make<TH1D>( "h591", "PXB3 #phi_{inc};PXB3 #phi_{inc} [deg];tracks", 80, -40, 40 );
  h592 = fs->make<TProfile>( "h592", "PXB3 p_{t} vs inc;PXB3 #phi_{inc} [deg];<p_{t}> [GeV]", 80, -40, 40, 0, 99 );
  h593 = fs->make<TProfile>( "h593", "PXB3 track sign vs inc;PXB3 #phi_{inc} [deg];<track charge>", 80, -40, 40, -2, 2 );
  h594 = fs->make<TProfile>( "h594", "PXB3 rows vs inc;PXB3 #phi_{inc} [deg];<rows/cluster>", 80, -40, 40, 0, 9 );
  h595 = fs->make<TProfile>( "h595", "PXB3 rows vs tan inc;PXB3 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  h596 = fs->make<TProfile>( "h596", "PXB3 rows vs tan inc;PXB3 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  h597 = fs->make<TProfile>( "h597", "PXB3 rows vs tan inc;PXB3 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );

  // triplet beam + 2 -> 1:

  h601 = fs->make<TH1D>( "h601", "PXB1 triplets z1;PXB1 z [cm];hits", 600, -30, 30 );
  h602 = fs->make<TH1D>( "h602", "PXB1 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h603 = fs->make<TH1D>( "h603", "PXB1 udca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );

  h607 = fs->make<TH1D>( "h607", "PXB1 lever arm;extrapolation factor to PXB1;tracks", 100, 0, 1 );
  h608 = fs->make<TH1D>( "h608", "PXB1 angle of incidence;PXB1 #phi_{inc} [deg];tracks", 60, -30, 30 );
  h609 = fs->make<TProfile>( "h609", "PXB1 angle of incidence;PXB1 #phi [deg];PXB1 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h610 = fs->make<TH1D>( "h610", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );

  // mean resid profiles:

  h612 = fs->make<TProfile>( "h612", "PXB1 #Deltax vs #phi;PXB1 #phi [deg];PXB1 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h612_out_zplus = fs->make<TProfile>( "h612_out_zplus", "PXB1 #Deltax vs #phi, outward-facing modules, z+;PXB1 #phi [deg];PXB1 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h612_out_zminus = fs->make<TProfile>( "h612_out_zminus", "PXB1 #Deltax vs #phi, outward-facing modules, z-;PXB1 #phi [deg];PXB1 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h612_in_zplus = fs->make<TProfile>( "h612_in_zplus", "PXB1 #Deltax vs #phi, inward-facing modules, z+;PXB1 #phi [deg];PXB1 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );
  h612_in_zminus = fs->make<TProfile>( "h612_in_zminus", "PXB1 #Deltax vs #phi, inward-facing modules, z-;PXB1 #phi [deg];PXB1 <#Deltax> [#mum]", 180, -180, 180, -99, 99 );

  h614 = fs->make<TProfile>( "h614", "PXB1 #Deltax vs z;PXB1 z [cm];PXB1 <#Deltax> [#mum]", 80, -20, 20, -99, 99 );

  h616 = fs->make<TProfile>( "h616", "PXB1 #Deltax vs p_{t};log(p_{t} [GeV]);PXB1 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );

  h618 = fs->make<TProfile>( "h618", "PXB1 #Deltax vs p_{t} +;log(p_{t} [GeV]);PXB1 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );
  h619 = fs->make<TProfile>( "h619", "PXB1 #Deltax vs p_{t} -;log(p_{t} [GeV]);PXB1 <#Deltax> [#mum]", 20, 0, 2, -99, 99 );

  h620 = fs->make<TH1D>( "h620", "PXB1 residuals #Deltax, p_{t} > 12;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h620_out_zplus = fs->make<TH1D>( "h620_out_zplus", "PXB1 residuals #Deltax, p_{t} > 12, outward-facing modules, z+;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h620_out_zminus = fs->make<TH1D>( "h620_out_zminus", "PXB1 residuals #Deltax, p_{t} > 12, outward-facing modules, z-;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h620_in_zplus = fs->make<TH1D>( "h620_in_zplus", "PXB1 residuals #Deltax, p_{t} > 12, inward-facing modules, z+;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h620_in_zminus = fs->make<TH1D>( "h620_in_zminus", "PXB1 residuals #Deltax, p_{t} > 12, inward-facing modules, z-;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h621 = fs->make<TH1D>( "h621", "PXB1 residuals #Deltax, p_{t} > 25;PXB1 #Deltax [#mum];hits", 100, -150, 150 );

  // width profiles:

  h622 = fs->make<TProfile>( "h622", "PXB1 #sigma_{x} vs #phi;PXB1 #phi [deg];PXB1 rms(#Deltax) [#mum]", 360, -180, 180, 0, 99 );

  h624 = fs->make<TProfile>( "h624", "PXB1 #sigma_{x} vs z;PXB1 z [cm];PXB1 rms(#Deltax) [#mum]", 80, -20, 20, 0, 99 );

  h626 = fs->make<TProfile>( "h626", "PXB1 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB1 rms(#Deltax) [#mum]", 20, 0, 2, 0, 99 );

  h628 = fs->make<TProfile>( "h628", "PXB1 #sigma_{x} vs dip;dip [deg];PXB1 rms(#Deltax) [#mum]", 70, -70, 70, 0, 99 );

  h630 = fs->make<TH1D>( "h630", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );

  h634 = fs->make<TProfile>( "h634", "PXB1 #sigma_{x} vs inc;PXB1 #phi_{inc} [deg];PXB1 rms(#Deltax) [#mum]", 60, -15, 15, 0, 99 );

  h635 = fs->make<TProfile>( "h635", "PXB1 #sigma_{x} vs #phi at 1 GeV;PXB1 #phi [deg];PXB1 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h650 = fs->make<TH1D>( "h650", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
				      
  h652 = fs->make<TH1D>( "h652", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h653 = fs->make<TH1D>( "h653", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h654 = fs->make<TH1D>( "h654", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h655 = fs->make<TH1D>( "h655", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  h656 = fs->make<TH1D>( "h656", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );

  // pixel track Karimaki with beam:

  h667 = fs->make<TH1D>( "h667", "ukap-kap;#Delta#kappa;tracks", 100, -0.01, 0.01 );
  h668 = fs->make<TH1D>( "h668", "uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h669 = fs->make<TH1D>( "h669", "udca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );

  h670 = fs->make<TH1D>( "h670", "pixel track dcap, p_{t} > 4;dcap [#mum];hits", 100, -50, 50 );
  h671 = fs->make<TH1D>( "h671", "pixel track dcap, p_{t} > 8;dcap [#mum];hits", 100, -50, 50 );

  h672 = fs->make<TProfile>( "h672", "pixel track  dcap vs #phi;#phi_{2} [deg];<dcap> [#mum]", 180, -180, 180, -500, 499 );
  h673 = fs->make<TProfile>( "h673", "pixel tracks  dcap vs p_{t};log(p_{t} [GeV]);<dcap> [#mum]", 20, 0, 2, -500, 499 );

  h674 = fs->make<TProfile>( "h674", "pixel track #sigma(dcap) vs #phi;#phi_{2} [deg];#sigma(dcap) [#mum]", 180, -180, 180, 0, 499 );
  h675 = fs->make<TProfile>( "h675", "pixel tracks #sigma(dcap) vs p_{t};log(p_{t} [GeV]);#sigma(dcap) [#mum]", 20, 0, 2, 0, 499 );

  // TIB triplet 1+3 -> 2:

  h701 = fs->make<TH1D>( "h701", "TIB2 triplets z2;TIB2 z [cm];hits", 600, -60, 60 );
  h702 = fs->make<TH1D>( "h702", "TIB2 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h703 = fs->make<TH1D>( "h703", "TIB2 udca-dca;#Deltadca [cm];tracks", 100, -0.5, 0.5 );

  h708 = fs->make<TH1D>( "h708", "TIB2 lever arm;extrapolation factor to TIB2;tracks", 100, 0, 1 );
  h709 = fs->make<TProfile>( "h709", "TIB2 angle of incidence;TIB2 #phi [deg];TIB2 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h710 = fs->make<TH1D>( "h710", "TIB2 residuals #Deltax;TIB2 #Deltax [#mum];hits", 100, -250, 250 );

  // mean resid profiles:

  h712 = fs->make<TProfile>( "h712", "TIB2 #Deltax vs #phi;TIB2 #phi [deg];TIB2 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  h714 = fs->make<TProfile>( "h714", "TIB2 #Deltax vs z;TIB2 z [cm];TIB2 <#Deltax> [#mum]", 100, -50, 50, -499, 499 );

  h716 = fs->make<TProfile>( "h716", "TIB2 #Deltax vs p_{t};log(p_{t} [GeV]);TIB2 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h718 = fs->make<TProfile>( "h718", "TIB2 #Deltax vs p_{t} +;log(p_{t} [GeV]);TIB2 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );
  h719 = fs->make<TProfile>( "h719", "TIB2 #Deltax vs p_{t} -;log(p_{t} [GeV]);TIB2 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h720 = fs->make<TH1D>( "h720", "TIB2 residuals #Deltax, p_{t} > 12;TIB2 #Deltax [#mum];hits", 100, -250, 250 );

  // width profiles:

  h722 = fs->make<TProfile>( "h722", "TIB2 #sigma_{x} vs #phi;TIB2 #phi [deg];TIB2 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h724 = fs->make<TProfile>( "h724", "TIB2 #sigma_{x} vs z;TIB2 z [cm];TIB2 rms(#Deltax) [#mum]", 100, -50, 50, 0, 499 );

  h726 = fs->make<TProfile>( "h726", "TIB2 #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB2 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );

  h728 = fs->make<TProfile>( "h728", "TIB2 #sigma_{x} vs dip;dip [deg];TIB2 rms(#Deltax) [#mum]", 70, -70, 70, 0, 499 );

  h730 = fs->make<TH1D>( "h730", "TIB2 residuals #Deltax;TIB2 #Deltax [#mum];hits", 100, -250, 250 );

  h734 = fs->make<TProfile>( "h734", "TIB2 #sigma_{x} vs inc;TIB2 #phi_{inc} [deg];TIB2 rms(#Deltax) [#mum]", 30, -15, 0, 0, 499 );

  h735 = fs->make<TProfile>( "h735", "TIB2 #sigma_{x} vs #phi at 1 GeV;TIB2 #phi [deg];TIB2 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h740 = fs->make<TH1D>( "h740", "TIB2 residuals #Deltax;TIB2 #Deltax [#mum];hits", 100, -250, 250 );

  // TIB triplet 2+4 -> 3:

  h751 = fs->make<TH1D>( "h751", "TIB3 triplets z3;TIB z3 [cm];hits", 600, -60, 60 );
  h752 = fs->make<TH1D>( "h752", "TIB3 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h753 = fs->make<TH1D>( "h753", "TIB3 udca-dca;#Deltadca [cm];tracks", 100, -0.5, 0.5 );

  h758 = fs->make<TH1D>( "h758", "TIB3 lever arm;extrapolation factor to TIB3;tracks", 100, 0, 1 );
  h759 = fs->make<TProfile>( "h759", "TIB3 angle of incidence;TIB3 #phi [deg];TIB3 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h760 = fs->make<TH1D>( "h760", "TIB3 residuals #Deltax;TIB #Deltax [#mum];hits", 100, -250, 250 );

  // mean resid profiles:

  h762 = fs->make<TProfile>( "h762", "TIB3 #Deltax vs #phi;TIB3 #phi [deg];TIB3 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  h764 = fs->make<TProfile>( "h764", "TIB3 #Deltax vs z;TIB3 z [cm];TIB3 <#Deltax> [#mum]", 100, -50, 50, -499, 499 );

  h766 = fs->make<TProfile>( "h766", "TIB3 #Deltax vs p_{t};log(p_{t} [GeV]);TIB3 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h768 = fs->make<TProfile>( "h768", "TIB3 #Deltax vs p_{t} +;log(p_{t} [GeV]);TIB3 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );
  h769 = fs->make<TProfile>( "h769", "TIB3 #Deltax vs p_{t} -;log(p_{t} [GeV]);TIB3 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h770 = fs->make<TH1D>( "h770", "TIB3 residuals #Deltax, p_{t} > 12;TIB3 #Deltax [#mum];hits", 100, -250, 250 );

  // width profiles:

  h772 = fs->make<TProfile>( "h772", "TIB3 #sigma_{x} vs #phi;TIB3 #phi [deg];TIB3 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h774 = fs->make<TProfile>( "h774", "TIB3 #sigma_{x} vs z;TIB3 z [cm];TIB3 rms(#Deltax) [#mum]", 100, -50, 50, 0, 499 );

  h776 = fs->make<TProfile>( "h776", "TIB3 #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB3 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );

  h778 = fs->make<TProfile>( "h778", "TIB3 #sigma_{x} vs dip;dip [deg];TIB3 rms(#Deltax) [#mum]", 70, -70, 70, 0, 499 );

  h780 = fs->make<TH1D>( "h780", "residuals TIB3 #Deltax;TIB3 #Deltax [#mum];hits", 100, -250, 250 );

  h784 = fs->make<TProfile>( "h784", "TIB3 #sigma_{x} vs inc;TIB3 #phi_{inc} [deg];TIB3 rms(#Deltax) [#mum]", 30, -15, 0, 0, 499 );

  h785 = fs->make<TProfile>( "h785", "TIB3 #sigma_{x} vs #phi at 1 GeV;TIB3 #phi [deg];TIB3 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h790 = fs->make<TH1D>( "h790", "residuals TIB3 #Deltax;TIB3 #Deltax [#mum];hits", 100, -250, 250 );

  // TOB triplet 3+5 -> 4:

  h801 = fs->make<TH1D>( "h801", "TOB4 triplets z2;TOB4 z [cm];hits", 200, -100, 100 );
  h802 = fs->make<TH1D>( "h802", "TOB4 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h803 = fs->make<TH1D>( "h803", "TOB4 udca-dca;#Deltadca [cm];tracks", 100, -1, 1 );

  h808 = fs->make<TH1D>( "h808", "TOB4 lever arm;extrapolation factor to TOB4;tracks", 100, 0, 1 );

  h809 = fs->make<TProfile>( "h809", "TOB4 angle of incidence;TOB4 #phi [deg];TOB4 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h810 = fs->make<TH1D>( "h810", "TOB4 residuals #Deltax;TOB4 #Deltax [#mum];hits", 100, -400, 400 );

  // mean resid profiles:

  h812 = fs->make<TProfile>( "h812", "TOB4 #Deltax vs #phi;TOB4 #phi [deg];TOB4 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  h814 = fs->make<TProfile>( "h814", "TOB4 #Deltax vs z;TOB4 z [cm];TOB4 <#Deltax> [#mum]", 100, -100, 100, -499, 499 );

  h816 = fs->make<TProfile>( "h816", "TOB4 #Deltax vs p_{t};log(p_{t} [GeV]);TOB4 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h818 = fs->make<TProfile>( "h818", "TOB4 #Deltax vs p_{t} +;log(p_{t} [GeV]);TOB4 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );
  h819 = fs->make<TProfile>( "h819", "TOB4 #Deltax vs p_{t} -;log(p_{t} [GeV]);TOB4 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h820 = fs->make<TH1D>( "h820", "TOB4 residuals #Deltax, p_{t} > 12;TOB4 #Deltax [#mum];hits", 100, -400, 400 );

  // width profiles:

  h822 = fs->make<TProfile>( "h822", "TOB4 #sigma_{x} vs #phi;TOB4 #phi [deg];TOB4 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h824 = fs->make<TProfile>( "h824", "TOB4 #sigma_{x} vs z;TOB4 z [cm];TOB4 rms(#Deltax) [#mum]", 100, -100, 100, 0, 499 );

  h826 = fs->make<TProfile>( "h826", "TOB4 #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB4 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );

  h828 = fs->make<TProfile>( "h828", "TOB4 #sigma_{x} vs dip;dip [deg];TOB4 rms(#Deltax) [#mum]", 70, -70, 70, 0, 499 );

  h830 = fs->make<TH1D>( "h830", "TOB4 residuals #Deltax;TOB4 #Deltax [#mum];hits", 100, -400, 400 );

  h834 = fs->make<TProfile>( "h834", "TOB4 #sigma_{x} vs inc;TOB4 #phi_{inc} [deg];TOB4 rms(#Deltax) [#mum]", 30, -15, 0, 0, 499 );

  h835 = fs->make<TProfile>( "h835", "TOB4 #sigma_{x} vs #phi at 1 GeV;TOB4 #phi [deg];TOB4 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h840 = fs->make<TH1D>( "h840", "TOB4 residuals #Deltax;TOB4 #Deltax [#mum];hits", 100, -400, 400 );

  // TOB triplet 4+6 -> 5:

  h851 = fs->make<TH1D>( "h851", "TOB5 triplets z3;TOB z3 [cm];hits", 200, -100, 100 );
  h852 = fs->make<TH1D>( "h852", "TOB5 uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h853 = fs->make<TH1D>( "h853", "TOB5 udca-dca;#Deltadca [cm];tracks", 100, -1, 1 );

  h858 = fs->make<TH1D>( "h858", "TOB5 lever arm;extrapolation factor to TOB5;tracks", 100, 0, 1 );
  h859 = fs->make<TProfile>( "h859", "TOB5 angle of incidence;TOB5 #phi [deg];TOB5 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  h860 = fs->make<TH1D>( "h860", "TOB5 residuals #Deltax;TOB #Deltax [#mum];hits", 100, -250, 250 );

  // mean resid profiles:

  h862 = fs->make<TProfile>( "h862", "TOB5 #Deltax vs #phi;TOB5 #phi [deg];TOB5 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  h864 = fs->make<TProfile>( "h864", "TOB5 #Deltax vs z;TOB5 z [cm];TOB5 <#Deltax> [#mum]", 100, -100, 100, -499, 499 );

  h866 = fs->make<TProfile>( "h866", "TOB5 #Deltax vs p_{t};log(p_{t} [GeV]);TOB5 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h868 = fs->make<TProfile>( "h868", "TOB5 #Deltax vs p_{t} +;log(p_{t} [GeV]);TOB5 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );
  h869 = fs->make<TProfile>( "h869", "TOB5 #Deltax vs p_{t} -;log(p_{t} [GeV]);TOB5 <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h870 = fs->make<TH1D>( "h870", "TOB5 residuals #Deltax, p_{t} > 12;TOB5 #Deltax [#mum];hits", 100, -250, 250 );

  // width profiles:

  h872 = fs->make<TProfile>( "h872", "TOB5 #sigma_{x} vs #phi;TOB5 #phi [deg];TOB5 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h874 = fs->make<TProfile>( "h874", "TOB5 #sigma_{x} vs z;TOB5 z [cm];TOB5 rms(#Deltax) [#mum]", 100, -100, 100, 0, 499 );

  h876 = fs->make<TProfile>( "h876", "TOB5 #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB5 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );

  h878 = fs->make<TProfile>( "h878", "TOB5 #sigma_{x} vs dip;dip [deg];TOB5 rms(#Deltax) [#mum]", 70, -70, 70, 0, 499 );

  h880 = fs->make<TH1D>( "h880", "residuals TOB5 #Deltax;TOB5 #Deltax [#mum];hits", 100, -250, 250 );

  h884 = fs->make<TProfile>( "h884", "TOB5 #sigma_{x} vs inc;TOB5 #phi_{inc} [deg];TOB5 rms(#Deltax) [#mum]", 30, -15, 0, 0, 499 );

  h885 = fs->make<TProfile>( "h885", "TOB5 #sigma_{x} vs #phi at 1 GeV;TOB5 #phi [deg];TOB5 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  h890 = fs->make<TH1D>( "h890", "residuals TOB5 #Deltax;TOB5 #Deltax [#mum];hits", 100, -250, 250 );

  // TEC triplets:

  h901 = fs->make<TH1D>( "h901", "TEC triplets R;TEC R [cm];hits", 100, 10, 110 );
  h902 = fs->make<TH1D>( "h902", "TEC uphi-phi;#Delta#phi [rad];tracks", 100, -0.1, 0.1 );
  h903 = fs->make<TH1D>( "h903", "TEC udca-dca;#Deltadca [cm];tracks", 100, -1, 1 );
  h904 = fs->make<TH1D>( "h904", "TEC pitch;TEC local pitch [#mum];tracks", 130, 80, 210 );
  h905 = fs->make<TProfile>( "h905", "TEC pitch vs R;TEC R [cm];TEC local pitch [#mum]", 80, 25, 105, 0, 250 );
  h906 = fs->make<TH1D>( "h906", "TEC triplets z;TEC z [cm];hits", 600, -300, 300 );

  h908 = fs->make<TH1D>( "h908", "TEC lever arm;extrapolation factor in TEC;tracks", 100, 0, 1 );
  h909 = fs->make<TH1D>( "h909", "TEC base length in z;TEC z base length [cm];tracks", 100, -50, 50 );

  h910 = fs->make<TH1D>( "h910", "TEC residuals #Deltax;TEC #Deltax [#mum];hits", 100, -400, 400 );

  // mean resid profiles:

  h912 = fs->make<TProfile>( "h912", "TEC #Deltax vs #phi;TEC #phi [deg];TEC <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  h914 = fs->make<TProfile>( "h914", "TEC #Deltax vs R;TEC R [cm];TEC <#Deltax> [#mum]", 100, 10, 110, -499, 499 );

  h916 = fs->make<TProfile>( "h916", "TEC #Deltax vs p_{t};log(p_{t} [GeV]);TEC <#Deltax> [#mum]", 20, 0, 2, -999, 999 );

  h918 = fs->make<TProfile>( "h918", "TEC #Deltax vs p_{t} +;log(p_{t} [GeV]);TEC <#Deltax> [#mum]", 20, 0, 2, -499, 499 );
  h919 = fs->make<TProfile>( "h919", "TEC #Deltax vs p_{t} -;log(p_{t} [GeV]);TEC <#Deltax> [#mum]", 20, 0, 2, -499, 499 );

  h920 = fs->make<TH1D>( "h920", "TEC residuals #Deltax, p_{t} > 12;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h921 = fs->make<TH1D>( "h921", "TEC residuals #Deltax, p_{t} > 12, long;TEC #Deltax [#mum];hits", 100, -400, 400 );

  // width profiles:

  h922 = fs->make<TProfile>( "h922", "TEC #sigma_{x} vs #phi;TEC #phi [deg];TEC rms(#Deltax) [#mum]", 360, -180, 180, 0, 500 );

  h924 = fs->make<TProfile>( "h924", "TEC #sigma_{x} vs R;TEC R [cm];TEC rms(#Deltax) [#mum]", 80, 25, 105, 0, 500 );
  h925 = fs->make<TProfile>( "h925", "TEC #sigma_{x} vs pitch;TEC pitch [#mum];TEC rms(#Deltax) [#mum]",  55, 90, 200, 0, 499 );

  h926 = fs->make<TProfile>( "h926", "TEC #sigma_{x} vs p_{t};log(p_{t} [GeV]);TEC rms(#Deltax) [#mum]", 20, 0, 2, 0, 1000 );

  h928 = fs->make<TProfile>( "h928", "TEC #sigma_{x} vs dip;dip [deg];TEC rms(#Deltax) [#mum]", 80, -80, 80, 0, 500 );

  h929 = fs->make<TProfile>( "h929", "TEC #sigma_{x} vs #phi at 1 GeV;TEC #phi [deg];TEC rms(#Deltax) [#mum]", 360, -180, 180, 0, 1000 );

  h930 = fs->make<TH1D>( "h930", "TEC residuals #Deltax, p > 12;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h931 = fs->make<TH1D>( "h931", "TEC residuals #Deltax, p > 12, 80 < p_{t}ch < 91;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h932 = fs->make<TH1D>( "h932", "TEC residuals #Deltax, p > 12, 91 < p_{t}ch < 102;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h933 = fs->make<TH1D>( "h933", "TEC residuals #Deltax, p > 12, 102 < p_{t}ch < 113;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h934 = fs->make<TH1D>( "h934", "TEC residuals #Deltax, p > 12, 113 < p_{t}ch < 123;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h935 = fs->make<TH1D>( "h935", "TEC residuals #Deltax, p > 12, 123 < p_{t}ch < 133;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h936 = fs->make<TH1D>( "h936", "TEC residuals #Deltax, p > 12, 133 < p_{t}ch < 144;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h937 = fs->make<TH1D>( "h937", "TEC residuals #Deltax, p > 12, 144 < p_{t}ch < 158;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h938 = fs->make<TH1D>( "h938", "TEC residuals #Deltax, p > 12, 163 < p_{t}ch < 184;TEC #Deltax [#mum];hits", 100, -400, 400 );
  h939 = fs->make<TH1D>( "h939", "TEC residuals #Deltax, p > 12, 184 < p_{t}ch < 205;TEC #Deltax [#mum];hits", 100, -400, 400 );

  // 1+2 -> 3 triplets:

  i501 = fs->make<TH1D>( "i501", "PXB3 triplets z3;PXB3 z [cm];hits", 600, -30, 30 );
  i502 = fs->make<TH1D>( "i502", "PXB3 uphi-phi;#delta#phi [rad];tracks", 100, -0.1, 0.1 );
  i503 = fs->make<TH1D>( "i503", "PXB3 udca-dca;#Deltadca [cm];tracks", 100, -0.1, 0.1 );
  i504 = fs->make<TH1D>( "i504", "PXB3 udip-dip;ddip;tracks", 100, -0.1, 0.1 );
  i505 = fs->make<TH1D>( "i505", "PXB3 uz0-z0;#Deltaz0;tracks", 100, -0.1, 0.1 );

  i507 = fs->make<TH1D>( "i507", "PXB3 lever arm;extrapolation factor to PXB3;tracks", 100, 1, 2 );
  i508 = fs->make<TProfile>( "i508", "PXB3 lever arm vs #phi;PXB3 #phi [deg];PXB3 extrapolation factor", 180, -180, 180, 0, 5 );

  i509 = fs->make<TProfile>( "i509", "PXB3 angle of incidence;PXB3 #phi [deg];PXB3 #phi_{inc} [deg]", 180, -180, 180, -90, 90 );

  i510 = fs->make<TH1D>( "i510", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i511 = fs->make<TH1D>( "i511", "PXB3 residuals #Deltaz;PXB3 #Deltaz [#mum];hits", 100, -300, 300 );

  // mean resid profiles:

  i512 = fs->make<TProfile>( "i512", "PXB3 #Deltax vs #phi;PXB3 #phi [deg];PXB3 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  i512_out_zplus = fs->make<TProfile>( "i512_out_zplus", "PXB3 #Deltax vs #phi, outward-facing modules, z+;PXB3 #phi [deg];PXB3 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  i512_out_zminus = fs->make<TProfile>( "i512_out_zminus", "PXB3 #Deltax vs #phi, outward-facing modules, z-;PXB3 #phi [deg];PXB3 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  i512_in_zplus = fs->make<TProfile>( "i512_in_zplus", "PXB3 #Deltax vs #phi, inward-facing modules, z+;PXB3 #phi [deg];PXB3 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  i512_in_zminus = fs->make<TProfile>( "i512_in_zminus", "PXB3 #Deltax vs #phi, inward-facing modules, z-;PXB3 #phi [deg];PXB3 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  i513 = fs->make<TProfile>( "i513", "PXB3 #Deltaz vs #phi;PXB3 #phi [deg];PXB3 <#Deltaz> [#mum]", 180, -180, 180, -199, 199 );

  i514 = fs->make<TProfile>( "i514", "PXB3 #Deltax vs z;PXB3 z [cm];PXB3 <#Deltax> [#mum]", 104, -26, 26, -199, 199 );
  i515 = fs->make<TProfile>( "i515", "PXB3 #Deltaz vs z;PXB3 z [cm];PXB3 <#Deltaz>  [#mum]", 104, -26, 26, -199, 199 );

  i516 = fs->make<TProfile>( "i516", "PXB3 #Deltax vs p_{t};log(p_{t} [GeV]);PXB3 <#Deltax> [#mum]", 20, 0, 2, -199, 199 );
  i517 = fs->make<TProfile>( "i517", "PXB3 #Deltaz vs p_{t};log(p_{t} [GeV]);PXB3 <#Deltaz>  [#mum]", 20, 0, 2, -199, 199 );

  i518 = fs->make<TProfile>( "i518", "PXB3 #Deltax vs p_{t} +;log(p_{t} [GeV]);PXB3 <#Deltax> [#mum]", 20, 0, 2, -199, 199 );
  i519 = fs->make<TProfile>( "i519", "PXB3 #Deltax vs p_{t} -;log(p_{t} [GeV]);PXB3 <#Deltax> [#mum]", 20, 0, 2, -199, 199 );

  i520 = fs->make<TH1D>( "i520", "PXB3 residuals #Deltax, p_{t} > 12;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_out_zplus = fs->make<TH1D>( "i520_out_zplus", "PXB3 residuals #Deltax, p_{t} > 12, outward-facing modules, z+;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_out_zminus = fs->make<TH1D>( "i520_out_zminus", "PXB3 residuals #Deltax, p_{t} > 12, outward-facing modules, z-;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_in_zplus = fs->make<TH1D>( "i520_in_zplus", "PXB3 residuals #Deltax, p_{t} > 12, inward-facing modules, z+;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_in_zminus = fs->make<TH1D>( "i520_in_zminus", "PXB3 residuals #Deltax, p_{t} > 12, inward-facing modules, z-;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i521 = fs->make<TH1D>( "i521", "PXB3 residuals #Deltaz, p_{t} > 12;PXB3 #Deltaz [#mum];hits", 100, -300, 300 );

  i520_1 = fs->make<TH1D>( "i520_1", "PXB3 residuals #Deltax, p_{t} > 12, lever 1;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_2 = fs->make<TH1D>( "i520_2", "PXB3 residuals #Deltax, p_{t} > 12, lever 2;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_3 = fs->make<TH1D>( "i520_3", "PXB3 residuals #Deltax, p_{t} > 12, lever 3;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_4 = fs->make<TH1D>( "i520_4", "PXB3 residuals #Deltax, p_{t} > 12, lever 4;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i520_5 = fs->make<TH1D>( "i520_5", "PXB3 residuals #Deltax, p_{t} > 12, lever 5;PXB3 #Deltax [#mum];hits", 100, -150, 150 );

  // width profiles:

  i522 = fs->make<TProfile>( "i522", "PXB3 #sigma_{x} vs #phi;PXB3 #phi [deg];PXB3 rms(#Deltax) [#mum]", 360, -180, 180, 0, 199 );
  i523 = fs->make<TProfile>( "i523", "PXB3 #sigma_{z} vs #phi;PXB3 #phi [deg];PXB3 rms(#Deltaz)  [#mum]", 360, -180, 180, 0, 199 );

  i524 = fs->make<TProfile>( "i524", "PXB3 #sigma_{x} vs z;PXB3 z [cm];PXB3 rms(#Deltax) [#mum]", 104, -26, 26, -199, 199 );
  i525 = fs->make<TProfile>( "i525", "PXB3 #sigma_{z} vs z;PXB3 z [cm];PXB3 rms(#Deltaz) [#mum]", 104, -26, 26, 0, 199 );

  i526 = fs->make<TProfile>( "i526", "PXB3 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB3 rms(#Deltax) [#mum]", 20, 0, 2, 0, 199 );
  i527 = fs->make<TProfile>( "i527", "PXB3 #sigma_{z} vs p_{t};log(p_{t} [GeV]);PXB3 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  i528 = fs->make<TProfile>( "i528", "PXB3 #sigma_{x} vs dip;dip [deg];PXB3 rms(#Deltax) [#mum]", 70, -70, 70, 0, 199 );
  i529 = fs->make<TProfile>( "i529", "PXB3 #sigma_{z} vs dip;dip [deg];PXB3 rms(#Deltaz) [#mum]", 70, -70, 70, 0, 199 );

  i530 = fs->make<TH1D>( "i530", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i531 = fs->make<TH1D>( "i531", "PXB3 residuals #Deltaz;PXB3 #Deltaz [#mum];hits", 100, -300, 300 );

  i532 = fs->make<TH1D>( "i532", "PXB3 residuals #Deltaz, 18 < |dip| < 50;PXB3 #Deltaz [#mum];hits", 100, -300, 300 );
  i533 = fs->make<TProfile>( "i533", "PXB3 #sigma_{z} vs p_{t}, best dip;log(p_{t} [GeV]);PXB3 rms(#Deltaz) [#mum]", 20, 0, 2, 0, 199 );

  i534 = fs->make<TProfile>( "i534", "PXB3 #sigma_{x} vs inc;PXB3 #phi_{inc} [deg];PXB3 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );

  i535 = fs->make<TProfile>( "i535", "PXB3 #sigma_{x} vs #phi at 1 GeV;PXB3 #phi [deg];PXB3 rms(#Deltax) [#mum]", 360, -180, 180, 0, 499 );

  i540 = fs->make<TH1D>( "i540", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i541 = fs->make<TH1D>( "i541", "PXB3 residuals #Deltaz;PXB3 #Deltaz [#mum];hits", 100, -300, 300 );

  i542 = fs->make<TH1D>( "i542", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i543 = fs->make<TH1D>( "i543", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i544 = fs->make<TH1D>( "i544", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i545 = fs->make<TH1D>( "i545", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  i546 = fs->make<TH1D>( "i546", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );

  // resids:

  k100 = fs->make<TH1D>( "k100", "PXB1 residuals #Deltax p_{t} >  4;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  k101 = fs->make<TH1D>( "k101", "PXB1 residuals #Deltax p_{t} > 12;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  k102 = fs->make<TProfile>( "k102", "PXB1 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB1 rms(#Deltax) [#mum]", 20, 0, 2, 0, 199 );
  k103 = fs->make<TH1D>( "k103", "PXB1 #alpha_{inc};PXB1 #alpha_{inc} [deg];hits", 180, -180, 180 );
  k104 = fs->make<TH1D>( "k104", "PXB1 #beta_{inc};PXB1 #beta_{inc} [deg];hits", 180, -90, 90 );
  k105 = fs->make<TProfile>( "k105", "PXB1 #sigma_{x} vs inc;#phi_{inc} [deg];PXB1 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );
  k106 = fs->make<TProfile>( "k106", "PXB1 #sigma_{x} vs inc;#phi_{inc} [deg];PXB1 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );
  k107 = fs->make<TProfile>( "k107", "PXB1 #sigma_{x} vs tet;dip inc [deg];PXB1 rms(#Deltax) [#mum]", 80, -80, 80, 0, 199 );
  k108 = fs->make<TProfile>( "k108", "PXB1 #sigma_{z} vs tet;dip inc [deg];PXB1 rms(dy) [#mum]", 80, -80, 80, 0, 399 );
  k109 = fs->make<TH1D>( "k109", "PXB1 local x;local x [cm];tracks", 200, -1, 1 );
  k110 = fs->make<TProfile>( "k110", "PXB1 #sigma_{x} vs x;local x [cm];PXB1 rms(#Deltax) [#mum]", 164, -0.82, 0.82, 0, 199 );
  k111 = fs->make<TH1D>( "k111", "PXB1 local y;local y [cm];tracks", 440, -3.3, 3.3 );//150 um bins
  k112 = fs->make<TProfile>( "k112", "PXB1 #sigma_{x} vs y;local y [cm];PXB1 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 199 );
  k113 = fs->make<TProfile>( "k113", "PXB1 #sigma_{z} vs y;local y [cm];PXB1 rms(dy) [#mum]", 432, -3.24, 3.24, 0, 399 );
  k114 = fs->make<TProfile>( "k114", "PXB1 #sigma_{x} vs x at 1 GeV;local x [cm];PXB1 rms(#Deltax) [#mum]", 164, -0.82, 0.82, 0, 499 );
  k115 = fs->make<TProfile>( "k115", "PXB1 #sigma_{x} vs z at 1 GeV;local y [cm];PXB1 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  k116 = fs->make<TProfile>( "k116", "PXB1 #Deltax vs F;#phi [deg];PXB1 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  k117 = fs->make<TProfile>( "k117", "PXB1 #Deltax vs Z;z [cm];PXB1 <#Deltax> [#mum]", 250, -25, 25, -199, 199 );
  k118 = fs->make<TProfile>( "k118", "PXB1 #Deltaz vs Z;z [cm];PXB1 <#Deltaz> [#mum]", 250, -25, 25, -399, 399 );

  k120 = fs->make<TH1D>( "k120", "PXB2 residuals #Deltax p_{t} >  4;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  k121 = fs->make<TH1D>( "k121", "PXB2 residuals #Deltax p_{t} > 12;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  k122 = fs->make<TProfile>( "k122", "PXB2 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB2 rms(#Deltax) [#mum]", 20, 0, 2, 0, 199 );
  k123 = fs->make<TH1D>( "k123", "PXB2 #alpha_{inc};PXB2 #alpha_{inc} [deg];hits", 180, -180, 180 );
  k124 = fs->make<TH1D>( "k124", "PXB2 #beta_{inc};PXB2 #beta_{inc} [deg];hits", 180, -180, 180 );
  k125 = fs->make<TProfile>( "k125", "PXB2 #sigma_{x} vs inc;#phi_{inc} [deg];PXB2 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );
  k126 = fs->make<TProfile>( "k126", "PXB2 #sigma_{x} vs inc;#phi_{inc} [deg];PXB2 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );
  k127 = fs->make<TProfile>( "k127", "PXB2 #sigma_{x} vs tet;dip inc [deg];PXB2 rms(#Deltax) [#mum]", 80, -80, 80, 0, 199 );
  k128 = fs->make<TProfile>( "k128", "PXB2 #sigma_{z} vs tet;dip inc [deg];PXB2 rms(dy) [#mum]", 80, -80, 80, 0, 399 );
  k129 = fs->make<TH1D>( "k129", "PXB2 local x;local x [cm];tracks", 200, -1, 1 );
  k130 = fs->make<TProfile>( "k130", "PXB2 #sigma_{x} vs x;local x [cm];PXB2 rms(#Deltax) [#mum]", 164, -0.82, 0.82, 0, 199 );
  k131 = fs->make<TH1D>( "k131", "PXB2 local y;local y [cm];tracks", 440, -3.3, 3.3 );//150 um bins
  k132 = fs->make<TProfile>( "k132", "PXB2 #sigma_{x} vs y;local y [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 199 );
  k133 = fs->make<TProfile>( "k133", "PXB2 #sigma_{z} vs y;local y [cm];PXB2 rms(dy) [#mum]", 432, -3.24, 3.24, 0, 399 );
  k134 = fs->make<TProfile>( "k134", "PXB2 #sigma_{x} vs x at 1 GeV;local x [cm];PXB2 rms(#Deltax) [#mum]", 164, -0.82, 0.82, 0, 499 );
  k135 = fs->make<TProfile>( "k135", "PXB2 #sigma_{x} vs z at 1 GeV;local y [cm];PXB2 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  k136 = fs->make<TProfile>( "k136", "PXB2 #Deltax vs F;#phi [deg];PXB2 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  k137 = fs->make<TProfile>( "k137", "PXB2 #Deltax vs Z;z [cm];PXB2 <#Deltax> [#mum]", 250, -25, 25, -199, 199 );
  k138 = fs->make<TProfile>( "k138", "PXB2 #Deltaz vs Z;z [cm];PXB2 <#Deltaz> [#mum]", 250, -25, 25, -399, 399 );

  k140 = fs->make<TH1D>( "k140", "PXB3 residuals #Deltax p_{t} >  4;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  k141 = fs->make<TH1D>( "k141", "PXB3 residuals #Deltax p_{t} > 12;PXB3 #Deltax [#mum];hits", 100, -150, 150 );
  k142 = fs->make<TProfile>( "k142", "PXB3 #sigma_{x} vs p_{t};log(p_{t} [GeV]);PXB3 rms(#Deltax) [#mum]", 20, 0, 2, 0, 199 );
  k143 = fs->make<TH1D>( "k143", "PXB3 #alpha_{inc};PXB3 #alpha_{inc} [deg];hits", 180, -180, 180 );
  k144 = fs->make<TH1D>( "k144", "PXB3 #beta_{inc};PXB3 #beta_{inc} [deg];hits", 180, -180, 180 );
  k145 = fs->make<TProfile>( "k145", "PXB3 #sigma_{x} vs inc;#phi_{inc} [deg];PXB3 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );
  k146 = fs->make<TProfile>( "k146", "PXB3 #sigma_{x} vs inc;#phi_{inc} [deg];PXB3 rms(#Deltax) [#mum]", 60, -15, 15, 0, 199 );
  k147 = fs->make<TProfile>( "k147", "PXB3 #sigma_{x} vs tet;dip inc [deg];PXB3 rms(#Deltax) [#mum]", 80, -80, 80, 0, 199 );
  k148 = fs->make<TProfile>( "k148", "PXB3 #sigma_{z} vs tet;dip inc [deg];PXB3 rms(dy) [#mum]", 80, -80, 80, 0, 399 );
  k149 = fs->make<TH1D>( "k149", "PXB3 local x;local x [cm];tracks", 200, -1, 1 );
  k150 = fs->make<TProfile>( "k150", "PXB3 #sigma_{x} vs x;local x [cm];PXB3 rms(#Deltax) [#mum]", 164, -0.82, 0.82, 0, 199 );
  k151 = fs->make<TH1D>( "k151", "PXB3 local y;local y [cm];tracks", 440, -3.3, 3.3 );//150 um bins
  k152 = fs->make<TProfile>( "k152", "PXB3 #sigma_{x} vs y;local y [cm];PXB3 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 199 );
  k153 = fs->make<TProfile>( "k153", "PXB3 #sigma_{z} vs y;local y [cm];PXB3 rms(dy) [#mum]", 432, -3.24, 3.24, 0, 399 );
  k154 = fs->make<TProfile>( "k154", "PXB3 #sigma_{x} vs x at 1 GeV;local x [cm];PXB3 rms(#Deltax) [#mum]", 164, -0.82, 0.82, 0, 499 );
  k155 = fs->make<TProfile>( "k155", "PXB3 #sigma_{x} vs z at 1 GeV;local y [cm];PXB3 rms(#Deltax) [#mum]", 432, -3.24, 3.24, 0, 499 );
  k156 = fs->make<TProfile>( "k156", "PXB3 #Deltax vs F;#phi [deg];PXB3 <#Deltax> [#mum]", 180, -180, 180, -199, 199 );
  k157 = fs->make<TProfile>( "k157", "PXB3 #Deltax vs Z;z [cm];PXB3 <#Deltax> [#mum]", 250, -25, 25, -199, 199 );
  k158 = fs->make<TProfile>( "k158", "PXB3 #Deltaz vs Z;z [cm];PXB3 <#Deltaz> [#mum]", 250, -25, 25, -399, 399 );

  k160 = fs->make<TH1D>( "k160", "PXB2 hit error x;PXB2 hit error x [#mum];hits", 100, 0, 50 );
  k161 = fs->make<TH1D>( "k161", "PXB2 track error x p_{t} > 4;PXB2 track error x [#mum];hits", 100, 0, 50 );

  // incident angle:

  k170 = fs->make<TH1D>( "k170", "PXB1 #phi_{inc};PXB1 #phi_{inc} [deg];tracks", 80, -40, 40 );
  k171 = fs->make<TProfile>( "k171", "PXB1 p_{t} vs inc;PXB1 #phi_{inc} [deg];<p_{t}> [GeV]", 80, -40, 40, 0, 99 );
  k172 = fs->make<TProfile>( "k172", "PXB1 track sign vs inc;PXB1 #phi_{inc} [deg];<track charge>", 80, -40, 40, -2, 2 );
  k173 = fs->make<TProfile>( "k173", "PXB1 rows vs inc;PXB1 #phi_{inc} [deg];<rows/cluster>", 80, -40, 40, 0, 9 );
  k174 = fs->make<TProfile>( "k174", "PXB1 rows vs tan inc;PXB1 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  k175 = fs->make<TProfile>( "k175", "PXB1 rows vs tan inc;PXB1 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  k176 = fs->make<TProfile>( "k176", "PXB1 rows vs tan inc;PXB1 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );

  k180 = fs->make<TH1D>( "k180", "PXB2 #phi_{inc};PXB2 #phi_{inc} [deg];tracks", 80, -40, 40 );
  k181 = fs->make<TProfile>( "k181", "PXB2 p_{t} vs inc;PXB2 #phi_{inc} [deg];<p_{t}> [GeV]", 80, -40, 40, 0, 99 );
  k182 = fs->make<TProfile>( "k182", "PXB2 track sign vs inc;PXB2 #phi_{inc} [deg];<track charge>", 80, -40, 40, -2, 2 );
  k183 = fs->make<TProfile>( "k183", "PXB2 rows vs inc;PXB2 #phi_{inc} [deg];<rows/cluster>", 80, -40, 40, 0, 9 );
  k184 = fs->make<TProfile>( "k184", "PXB2 rows vs tan inc;PXB2 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  k185 = fs->make<TProfile>( "k185", "PXB2 rows vs tan inc;PXB2 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  k186 = fs->make<TProfile>( "k186", "PXB2 rows vs tan inc;PXB2 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );

  k190 = fs->make<TH1D>( "k190", "PXB3 #phi_{inc};PXB3 #phi_{inc} [deg];tracks", 80, -40, 40 );
  k191 = fs->make<TProfile>( "k191", "PXB3 p_{t} vs inc;PXB3 #phi_{inc} [deg];<p_{t}> [GeV]", 80, -40, 40, 0, 99 );
  k192 = fs->make<TProfile>( "k192", "PXB3 track sign vs inc;PXB3 #phi_{inc} [deg];<track charge>", 80, -40, 40, -2, 2 );
  k193 = fs->make<TProfile>( "k193", "PXB3 rows vs inc;PXB3 #phi_{inc} [deg];<rows/cluster>", 80, -40, 40, 0, 9 );
  k194 = fs->make<TProfile>( "k194", "PXB3 rows vs tan inc;PXB3 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  k195 = fs->make<TProfile>( "k195", "PXB3 rows vs tan inc;PXB3 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );
  k196 = fs->make<TProfile>( "k196", "PXB3 rows vs tan inc;PXB3 tan(inc);<rows/cluster>", 100, -0.5, 0.5, 0, 9 );

  k301 = fs->make<TH1D>( "k301", "TIB R;TIB R [cm];tracks", 200, 20, 60 );
  k302 = fs->make<TH1D>( "k302", "TIB F;TIB F [deg];tracks", 180, -180, 180 );
  k303 = fs->make<TH1D>( "k303", "TIB Z;TIB Z [cm];tracks", 140, -70, 70 );
  k304 = fs->make<TProfile>( "k304", "TIB #sigma_{x} vs F at 1 GeV;#phi [deg];TIB rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k305 = fs->make<TProfile>( "k305", "TIB #sigma_{x} vs Z at 1 GeV;Z [cm];TIB rms(#Deltax) [#mum]", 120, -60, 60, 0, 499 );
  k306 = fs->make<TProfile>( "k306", "TIB #sigma_{x} vs x at 1 GeV;local x [cm];TIB rms(#Deltax) [#mum]",  80, -4, 4, 0, 499 );
  k307 = fs->make<TProfile>( "k307", "TIB #sigma_{x} vs z at 1 GeV;local y [cm];TIB rms(#Deltax) [#mum]", 124, -6.2, 6.2, 0, 499 );
  k308 = fs->make<TH1D>( "k308", "TIB #alpha_{inc};TIB #alpha_{inc} [deg];hits", 180, -180, 180 );

  k310 = fs->make<TProfile>( "k310", "TIB1a #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB1a rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k311 = fs->make<TH1D>( "k311", "TIB1a residuals #Deltax p_{t} > 4;TIB1a #Deltax [#mum];hits", 100, -250, 250 );
  k312 = fs->make<TProfile>( "k312", "TIB1a #sigma_{x} vs F p_{t} > 4;#phi [deg];TIB1a rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k313 = fs->make<TProfile>( "k313", "TIB1a #sigma_{x} vs Z p_{t} > 4;Z [cm];TIB1a rms(#Deltax) [#mum]", 120, -60, 60, 0, 499 );
  k314 = fs->make<TProfile>( "k314", "TIB1a #sigma_{x} vs x p_{t} > 4;local x [cm];TIB1a rms(#Deltax) [#mum]",  80, -4, 4, 0, 499 );
  k315 = fs->make<TProfile>( "k315", "TIB1a #sigma_{x} vs z p_{t} > 4;local y [cm];TIB1a rms(#Deltax) [#mum]", 124, -6.2, 6.2, 0, 499 );
  k316 = fs->make<TProfile>( "k316", "TIB1a #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TIB1a rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k317 = fs->make<TProfile>( "k317", "TIB1a #Deltax vs F;#phi [deg];TIB1a <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k318 = fs->make<TProfile>( "k318", "TIB1a #Deltax vs Z;z [cm];TIB1a <#Deltax> [#mum]", 200, -50, 50, -499, 499 );
  k319 = fs->make<TH1D>( "k319", "TIB1a residuals #Deltax p_{t} > 12;TIB1a #Deltax [#mum];hits", 100, -250, 250 );

  k320 = fs->make<TProfile>( "k320", "TIB1s #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB1s rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k321 = fs->make<TH1D>( "k321", "TIB1s residuals #Deltax p_{t} > 4;TIB1s #Deltax [#mum];hits", 100, -250, 250 );
  k322 = fs->make<TProfile>( "k322", "TIB1s #sigma_{x} vs F p_{t} > 4;#phi [deg];TIB1s rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k323 = fs->make<TProfile>( "k323", "TIB1s #sigma_{x} vs Z p_{t} > 4;Z [cm];TIB1s rms(#Deltax) [#mum]", 120, -60, 60, 0, 499 );
  k324 = fs->make<TProfile>( "k324", "TIB1s #sigma_{x} vs x p_{t} > 4;local x [cm];TIB1s rms(#Deltax) [#mum]",  80, -4, 4, 0, 499 );
  k325 = fs->make<TProfile>( "k325", "TIB1s #sigma_{x} vs z p_{t} > 4;local y [cm];TIB1s rms(#Deltax) [#mum]", 124, -6.2, 6.2, 0, 499 );
  k326 = fs->make<TProfile>( "k326", "TIB1s #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TIB1s rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k327 = fs->make<TProfile>( "k327", "TIB1s #Deltax vs F;#phi [deg];TIB1s <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k328 = fs->make<TProfile>( "k328", "TIB1s #Deltax vs Z;z [cm];TIB1s <#Deltax> [#mum]", 200, -50, 50, -499, 499 );
  k329 = fs->make<TH1D>( "k329", "TIB1s residuals #Deltax p_{t} > 12;TIB1s #Deltax [#mum];hits", 100, -250, 250 );

  k330 = fs->make<TProfile>( "k330", "TIB2a #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB2a rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k331 = fs->make<TH1D>( "k331", "TIB2a residuals #Deltax p_{t} > 4;TIB2a #Deltax [#mum];hits", 100, -250, 250 );
  k332 = fs->make<TProfile>( "k332", "TIB2a #sigma_{x} vs F p_{t} > 4;#phi [deg];TIB2a rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k333 = fs->make<TProfile>( "k333", "TIB2a #sigma_{x} vs Z p_{t} > 4;Z [cm];TIB2a rms(#Deltax) [#mum]", 120, -60, 60, 0, 499 );
  k334 = fs->make<TProfile>( "k334", "TIB2a #sigma_{x} vs x p_{t} > 4;local x [cm];TIB2a rms(#Deltax) [#mum]",  80, -4, 4, 0, 499 );
  k335 = fs->make<TProfile>( "k335", "TIB2a #sigma_{x} vs z p_{t} > 4;local y [cm];TIB2a rms(#Deltax) [#mum]", 124, -6.2, 6.2, 0, 499 );
  k336 = fs->make<TProfile>( "k336", "TIB2a #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TIB2a rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k337 = fs->make<TProfile>( "k337", "TIB2a #Deltax vs F;#phi [deg];TIB2a <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k338 = fs->make<TProfile>( "k338", "TIB2a #Deltax vs Z;z [cm];TIB2a <#Deltax> [#mum]", 200, -50, 50, -499, 499 );
  k339 = fs->make<TH1D>( "k339", "TIB2a residuals #Deltax p_{t} > 12;TIB2a #Deltax [#mum];hits", 100, -250, 250 );

  k340 = fs->make<TProfile>( "k340", "TIB2s #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB2s rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k341 = fs->make<TH1D>( "k341", "TIB2s residuals #Deltax p_{t} > 4;TIB2s #Deltax [#mum];hits", 100, -250, 250 );
  k342 = fs->make<TProfile>( "k342", "TIB2s #sigma_{x} vs F p_{t} > 4;#phi [deg];TIB2s rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k343 = fs->make<TProfile>( "k343", "TIB2s #sigma_{x} vs Z p_{t} > 4;Z [cm];TIB2s rms(#Deltax) [#mum]", 120, -60, 60, 0, 499 );
  k344 = fs->make<TProfile>( "k344", "TIB2s #sigma_{x} vs x p_{t} > 4;local x [cm];TIB2s rms(#Deltax) [#mum]",  80, -4, 4, 0, 499 );
  k345 = fs->make<TProfile>( "k345", "TIB2s #sigma_{x} vs z p_{t} > 4;local y [cm];TIB2s rms(#Deltax) [#mum]", 124, -6.2, 6.2, 0, 499 );
  k346 = fs->make<TProfile>( "k346", "TIB2s #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TIB2s rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k347 = fs->make<TProfile>( "k347", "TIB2s #Deltax vs F;#phi [deg];TIB2s <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k348 = fs->make<TProfile>( "k348", "TIB2s #Deltax vs Z;z [cm];TIB2s <#Deltax> [#mum]", 200, -50, 50, -499, 499 );
  k349 = fs->make<TH1D>( "k349", "TIB2s residuals #Deltax p_{t} > 12;TIB2s #Deltax [#mum];hits", 100, -250, 250 );

  k350 = fs->make<TProfile>( "k350", "TIB3a #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB3a rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k351 = fs->make<TH1D>( "k351", "TIB3a residuals #Deltax p_{t} > 4;TIB3a #Deltax [#mum];hits", 100, -250, 250 );
  k352 = fs->make<TProfile>( "k352", "TIB3a #sigma_{x} vs F p_{t} > 4;#phi [deg];TIB3a rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k353 = fs->make<TProfile>( "k353", "TIB3a #sigma_{x} vs Z p_{t} > 4;Z [cm];TIB3a rms(#Deltax) [#mum]", 120, -60, 60, 0, 499 );
  k354 = fs->make<TProfile>( "k354", "TIB3a #sigma_{x} vs x p_{t} > 4;local x [cm];TIB3a rms(#Deltax) [#mum]",  80, -4, 4, 0, 499 );
  k355 = fs->make<TProfile>( "k355", "TIB3a #sigma_{x} vs z p_{t} > 4;local y [cm];TIB3a rms(#Deltax) [#mum]", 124, -6.2, 6.2, 0, 499 );
  k356 = fs->make<TProfile>( "k356", "TIB3a #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TIB3a rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k357 = fs->make<TProfile>( "k357", "TIB3a #Deltax vs F;#phi [deg];TIB3a <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k358 = fs->make<TProfile>( "k358", "TIB3a #Deltax vs Z;z [cm];TIB3a <#Deltax> [#mum]", 200, -50, 50, -499, 499 );
  k359 = fs->make<TH1D>( "k359", "TIB3a residuals #Deltax p_{t} > 12;TIB3a #Deltax [#mum];hits", 100, -250, 250 );

  k360 = fs->make<TProfile>( "k360", "TIB4a #sigma_{x} vs p_{t};log(p_{t} [GeV]);TIB4a rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k361 = fs->make<TH1D>( "k361", "TIB4a residuals #Deltax p_{t} > 4;TIB4a #Deltax [#mum];hits", 100, -250, 250 );
  k362 = fs->make<TProfile>( "k362", "TIB4a #sigma_{x} vs F p_{t} > 4;#phi [deg];TIB4a rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k363 = fs->make<TProfile>( "k363", "TIB4a #sigma_{x} vs Z p_{t} > 4;Z [cm];TIB4a rms(#Deltax) [#mum]", 120, -60, 60, 0, 499 );
  k364 = fs->make<TProfile>( "k364", "TIB4a #sigma_{x} vs x p_{t} > 4;local x [cm];TIB4a rms(#Deltax) [#mum]",  80, -4, 4, 0, 499 );
  k365 = fs->make<TProfile>( "k365", "TIB4a #sigma_{x} vs z p_{t} > 4;local y [cm];TIB4a rms(#Deltax) [#mum]", 124, -6.2, 6.2, 0, 499 );
  k366 = fs->make<TProfile>( "k366", "TIB4a #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TIB4a rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k367 = fs->make<TProfile>( "k367", "TIB4a #Deltax vs F;#phi [deg];TIB4a <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k368 = fs->make<TProfile>( "k368", "TIB4a #Deltax vs Z;z [cm];TIB4a <#Deltax> [#mum]", 200, -50, 50, -499, 499 );
  k369 = fs->make<TH1D>( "k369", "TIB4a residuals #Deltax p_{t} > 12;TIB4a #Deltax [#mum];hits", 100, -250, 250 );

  k401 = fs->make<TH1D>( "k401", "TID R;TID R [cm];tracks", 200, 20, 60 );
  k402 = fs->make<TH1D>( "k402", "TID F;TID F [deg];tracks", 180, -180, 180 );
  k403 = fs->make<TH1D>( "k403", "TID Z;TID Z [cm];tracks", 220, -110, 110 );
  k404 = fs->make<TH1D>( "k404", "TID residuals #Deltax;TID #Deltax [#mum];hits", 100, -250, 250 );
  k405 = fs->make<TProfile>( "k405", "TID residuals #Deltax vs R;TID R [cm];TID rms(#Deltax) [#mum]", 30, 25, 55, 0, 499 );
  k406 = fs->make<TProfile>( "k406", "TID residuals #Deltax vs p;TID pitch [#mum];TID rms(#Deltax) [#mum]", 100, 50, 250, 0, 499 );

  k410 = fs->make<TProfile>( "k412", "TIDr1 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TIDr1 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k411 = fs->make<TH1D>( "k410", "TIDr1a residuals #Deltax;TIDr1a #Deltax [#mum];hits", 100, -250, 250 );
  k412 = fs->make<TH1D>( "k411", "TIDr1s residuals #Deltax;TIDr1s #Deltax [#mum];hits", 100, -250, 250 );
  k413 = fs->make<TProfile>( "k413", "TIDr1 residuals #sigma_{x} vs F;TID F [deg];TIDr1 rms(#Deltax) [#mum]", 64, -180, 180, 0, 499 );
  k414 = fs->make<TProfile>( "k414", "TIDr1 residuals #Deltax vs F;TID F [deg];TIDr1 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  k420 = fs->make<TProfile>( "k422", "TIDr2 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TIDr2 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k421 = fs->make<TH1D>( "k420", "TIDr2a residuals #Deltax;TIDr2a #Deltax [#mum];hits", 100, -250, 250 );
  k422 = fs->make<TH1D>( "k421", "TIDr2s residuals #Deltax;TIDr2s #Deltax [#mum];hits", 100, -250, 250 );
  k423 = fs->make<TProfile>( "k423", "TIDr2 residuals #sigma_{x} vs F;TID F [deg];TIDr2 rms(#Deltax) [#mum]", 64, -180, 180, 0, 499 );
  k424 = fs->make<TProfile>( "k424", "TIDr2 residuals #Deltax vs F;TID F [deg];TIDr2 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  k430 = fs->make<TProfile>( "k432", "TIDr3 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TIDr3 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k431 = fs->make<TH1D>( "k430", "TIDr3a residuals #Deltax;TIDr3a #Deltax [#mum];hits", 100, -250, 250 );
  k432 = fs->make<TH1D>( "k431", "TIDr3s residuals #Deltax;TIDr3s #Deltax [#mum];hits", 100, -250, 250 );
  k433 = fs->make<TProfile>( "k433", "TIDr3 residuals #sigma_{x} vs F;TID F [deg];TIDr3 rms(#Deltax) [#mum]", 64, -180, 180, 0, 499 );
  k434 = fs->make<TProfile>( "k434", "TIDr3 residuals #Deltax vs F;TID F [deg];TIDr3 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );

  k501 = fs->make<TH1D>( "k501", "TOB R;TOB R [cm];tracks", 240, 55, 115 );
  k502 = fs->make<TH1D>( "k502", "TOB F;TOB F [deg];tracks", 180, -180, 180 );
  k503 = fs->make<TH1D>( "k503", "TOB Z;TOB Z [cm];tracks", 220, -110, 110 );
  k504 = fs->make<TProfile>( "k504", "TOB #sigma_{x} vs F at 1 GeV;#phi [deg];TOB rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k505 = fs->make<TProfile>( "k505", "TOB #sigma_{x} vs Z at 1 GeV;Z [cm];TOB rms(#Deltax) [#mum]", 220, -110, 110, 0, 499 );
  k506 = fs->make<TProfile>( "k506", "TOB #sigma_{x} vs x at 1 GeV;local x [cm];TOB rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k507 = fs->make<TProfile>( "k507", "TOB #sigma_{x} vs z at 1 GeV;local y [cm];TOB rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k508 = fs->make<TH1D>( "k508", "TOB #alpha_{inc};TOB #alpha_{inc} [deg];hits", 180, -180, 180 );

  k510 = fs->make<TProfile>( "k510", "TOB1-ax #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB1-ax rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k511 = fs->make<TH1D>( "k511", "TOB1-ax residuals #Deltax p_{t} > 4;TOB1-ax #Deltax [#mum];hits", 100, -400, 400 );
  k512 = fs->make<TProfile>( "k512", "TOB1-ax #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB1-ax rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k513 = fs->make<TProfile>( "k513", "TOB1-ax #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB1-ax rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k514 = fs->make<TProfile>( "k514", "TOB1-ax #sigma_{x} vs x p_{t} > 4;local x [cm];TOB1-ax rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k515 = fs->make<TProfile>( "k515", "TOB1-ax #sigma_{x} vs z p_{t} > 4;local y [cm];TOB1-ax rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k516 = fs->make<TProfile>( "k516", "TOB1-ax #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB1-ax rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k517 = fs->make<TProfile>( "k517", "TOB1-ax #Deltax vs F;#phi [deg];TOB1-ax <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k518 = fs->make<TProfile>( "k518", "TOB1-ax #Deltax vs Z;z [cm];TOB1-ax <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k519 = fs->make<TH1D>( "k519", "TOB1-ax residuals #Deltax p_{t} > 12;TOB1-ax #Deltax [#mum];hits", 100, -400, 400 );

  k520 = fs->make<TProfile>( "k520", "TOB1s #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB1s rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k521 = fs->make<TH1D>( "k521", "TOB1s residuals #Deltax p_{t} > 4;TOB1s #Deltax [#mum];hits", 100, -400, 400 );
  k522 = fs->make<TProfile>( "k522", "TOB1s #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB1s rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k523 = fs->make<TProfile>( "k523", "TOB1s #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB1s rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k524 = fs->make<TProfile>( "k524", "TOB1s #sigma_{x} vs x p_{t} > 4;local x [cm];TOB1s rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k525 = fs->make<TProfile>( "k525", "TOB1s #sigma_{x} vs z p_{t} > 4;local y [cm];TOB1s rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k526 = fs->make<TProfile>( "k526", "TOB1s #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB1s rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k527 = fs->make<TProfile>( "k527", "TOB1s #Deltax vs F;#phi [deg];TOB1s <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k528 = fs->make<TProfile>( "k528", "TOB1s #Deltax vs Z;z [cm];TOB1s <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k529 = fs->make<TH1D>( "k529", "TOB1s residuals #Deltax p_{t} > 12;TOB1s #Deltax [#mum];hits", 100, -400, 400 );

  k530 = fs->make<TProfile>( "k530", "TOB2-ax #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB2-ax rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k531 = fs->make<TH1D>( "k531", "TOB2-ax residuals #Deltax p_{t} > 4;TOB2-ax #Deltax [#mum];hits", 100, -400, 400 );
  k532 = fs->make<TProfile>( "k532", "TOB2-ax #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB2-ax rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k533 = fs->make<TProfile>( "k533", "TOB2-ax #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB2-ax rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k534 = fs->make<TProfile>( "k534", "TOB2-ax #sigma_{x} vs x p_{t} > 4;local x [cm];TOB2-ax rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k535 = fs->make<TProfile>( "k535", "TOB2-ax #sigma_{x} vs z p_{t} > 4;local y [cm];TOB2-ax rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k536 = fs->make<TProfile>( "k536", "TOB2-ax #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB2-ax rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k537 = fs->make<TProfile>( "k537", "TOB2-ax #Deltax vs F;#phi [deg];TOB2-ax <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k538 = fs->make<TProfile>( "k538", "TOB2-ax #Deltax vs Z;z [cm];TOB2-ax <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k539 = fs->make<TH1D>( "k539", "TOB2-ax residuals #Deltax p_{t} > 12;TOB2-ax #Deltax [#mum];hits", 100, -400, 400 );

  k540 = fs->make<TProfile>( "k540", "TOB2s #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB2s rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k541 = fs->make<TH1D>( "k541", "TOB2s residuals #Deltax p_{t} > 4;TOB2s #Deltax [#mum];hits", 100, -400, 400 );
  k542 = fs->make<TProfile>( "k542", "TOB2s #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB2s rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k543 = fs->make<TProfile>( "k543", "TOB2s #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB2s rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k544 = fs->make<TProfile>( "k544", "TOB2s #sigma_{x} vs x p_{t} > 4;local x [cm];TOB2s rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k545 = fs->make<TProfile>( "k545", "TOB2s #sigma_{x} vs z p_{t} > 4;local y [cm];TOB2s rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k546 = fs->make<TProfile>( "k546", "TOB2s #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB2s rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k547 = fs->make<TProfile>( "k547", "TOB2s #Deltax vs F;#phi [deg];TOB2s <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k548 = fs->make<TProfile>( "k548", "TOB2s #Deltax vs Z;z [cm];TOB2s <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k549 = fs->make<TH1D>( "k549", "TOB2s residuals #Deltax p_{t} > 12;TOB2s #Deltax [#mum];hits", 100, -400, 400 );

  k550 = fs->make<TProfile>( "k550", "TOB3-ax #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB3-ax rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k551 = fs->make<TH1D>( "k551", "TOB3-ax residuals #Deltax p_{t} > 4;TOB3-ax #Deltax [#mum];hits", 100, -400, 400 );
  k552 = fs->make<TProfile>( "k552", "TOB3-ax #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB3-ax rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k553 = fs->make<TProfile>( "k553", "TOB3-ax #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB3-ax rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k554 = fs->make<TProfile>( "k554", "TOB3-ax #sigma_{x} vs x p_{t} > 4;local x [cm];TOB3-ax rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k555 = fs->make<TProfile>( "k555", "TOB3-ax #sigma_{x} vs z p_{t} > 4;local y [cm];TOB3-ax rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k556 = fs->make<TProfile>( "k556", "TOB3-ax #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB3-ax rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k557 = fs->make<TProfile>( "k557", "TOB3-ax #Deltax vs F;#phi [deg];TOB3-ax <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k558 = fs->make<TProfile>( "k558", "TOB3-ax #Deltax vs Z;z [cm];TOB3-ax <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k559 = fs->make<TH1D>( "k559", "TOB3-ax residuals #Deltax p_{t} > 12;TOB3-ax #Deltax [#mum];hits", 100, -400, 400 );

  k560 = fs->make<TProfile>( "k560", "TOB4-ax #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB4-ax rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k561 = fs->make<TH1D>( "k561", "TOB4-ax residuals #Deltax p_{t} > 4;TOB4-ax #Deltax [#mum];hits", 100, -400, 400 );
  k562 = fs->make<TProfile>( "k562", "TOB4-ax #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB4-ax rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k563 = fs->make<TProfile>( "k563", "TOB4-ax #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB4-ax rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k564 = fs->make<TProfile>( "k564", "TOB4-ax #sigma_{x} vs x p_{t} > 4;local x [cm];TOB4-ax rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k565 = fs->make<TProfile>( "k565", "TOB4-ax #sigma_{x} vs z p_{t} > 4;local y [cm];TOB4-ax rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k566 = fs->make<TProfile>( "k566", "TOB4-ax #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB4-ax rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k567 = fs->make<TProfile>( "k567", "TOB4-ax #Deltax vs F;#phi [deg];TOB4-ax <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k568 = fs->make<TProfile>( "k568", "TOB4-ax #Deltax vs Z;z [cm];TOB4-ax <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k569 = fs->make<TH1D>( "k569", "TOB4-ax residuals #Deltax p_{t} > 12;TOB4-ax #Deltax [#mum];hits", 100, -400, 400 );

  k570 = fs->make<TProfile>( "k570", "TOB5-ax #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB5-ax rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k571 = fs->make<TH1D>( "k571", "TOB5-ax residuals #Deltax p_{t} > 4;TOB5-ax #Deltax [#mum];hits", 100, -400, 400 );
  k572 = fs->make<TProfile>( "k572", "TOB5-ax #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB5-ax rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k573 = fs->make<TProfile>( "k573", "TOB5-ax #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB5-ax rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k574 = fs->make<TProfile>( "k574", "TOB5-ax #sigma_{x} vs x p_{t} > 4;local x [cm];TOB5-ax rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k575 = fs->make<TProfile>( "k575", "TOB5-ax #sigma_{x} vs z p_{t} > 4;local y [cm];TOB5-ax rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k576 = fs->make<TProfile>( "k576", "TOB5-ax #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB5-ax rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k577 = fs->make<TProfile>( "k577", "TOB5-ax #Deltax vs F;#phi [deg];TOB5-ax <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k578 = fs->make<TProfile>( "k578", "TOB5-ax #Deltax vs Z;z [cm];TOB5-ax <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k579 = fs->make<TH1D>( "k579", "TOB5-ax residuals #Deltax p_{t} > 12;TOB5-ax #Deltax [#mum];hits", 100, -400, 400 );

  k580 = fs->make<TProfile>( "k580", "TOB6-ax #sigma_{x} vs p_{t};log(p_{t} [GeV]);TOB6-ax rms(#Deltax) [#mum]", 20, 0, 2, 0, 399 );
  k581 = fs->make<TH1D>( "k581", "TOB6-ax residuals #Deltax p_{t} > 4;TOB6-ax #Deltax [#mum];hits", 100, -400, 400 );
  k582 = fs->make<TProfile>( "k582", "TOB6-ax #sigma_{x} vs F p_{t} > 4;#phi [deg];TOB6-ax rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k583 = fs->make<TProfile>( "k583", "TOB6-ax #sigma_{x} vs Z p_{t} > 4;Z [cm];TOB6-ax rms(#Deltax) [#mum]", 110, -110, 110, 0, 499 );
  k584 = fs->make<TProfile>( "k584", "TOB6-ax #sigma_{x} vs x p_{t} > 4;local x [cm];TOB6-ax rms(#Deltax) [#mum]", 100, -5, 5, 0, 499 );
  k585 = fs->make<TProfile>( "k585", "TOB6-ax #sigma_{x} vs z p_{t} > 4;local y [cm];TOB6-ax rms(#Deltax) [#mum]", 200, -10, 10, 0, 499 );
  k586 = fs->make<TProfile>( "k586", "TOB6-ax #sigma_{x} vs inc p_{t} > 4;#phi_{inc} [deg];TOB6-ax rms(#Deltax) [#mum]", 80, -20, 20, 0, 499 );
  k587 = fs->make<TProfile>( "k587", "TOB6-ax #Deltax vs F;#phi [deg];TOB6-ax <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k588 = fs->make<TProfile>( "k588", "TOB6-ax #Deltax vs Z;z [cm];TOB6-ax <#Deltax> [#mum]", 200, -100, 100, -499, 499 );
  k589 = fs->make<TH1D>( "k589", "TOB6-ax residuals #Deltax p_{t} > 12;TOB6-ax #Deltax [#mum];hits", 100, -400, 400 );

  k601 = fs->make<TH1D>( "k601", "TEC R;TEC R [cm];tracks", 200, 20, 120 );
  k602 = fs->make<TH1D>( "k602", "TEC F;TEC F [deg];tracks", 180, -180, 180 );
  k603 = fs->make<TH1D>( "k603", "TEC Z;TEC Z [cm];tracks", 600, -300, 300 );
  k604 = fs->make<TH1D>( "k604", "TEC residuals #Deltax;TEC #Deltax [#mum];hits", 100, -250, 250 );
  k605 = fs->make<TProfile>( "k605", "TEC residuals #Deltax vs R;TEC R [cm];TEC rms(#Deltax) [#mum]", 90, 25, 115, 0, 499 );
  k606 = fs->make<TProfile>( "k606", "TEC pitch vs R;TEC R [cm];TEC local pitch [#mum]", 90, 25, 115, 0, 250 );
  k607 = fs->make<TProfile>( "k607", "TEC residuals #Deltax vs p;TEC pitch [#mum];TID rms(#Deltax) [#mum]", 100, 50, 250, 0, 499 );
  k608 = fs->make<TH2D>( "k608", "TEC back petal;TEC x [cm];TEC y [cm]", 200, -100, 100, 200, -100, 100 );
  k609 = fs->make<TH2D>( "k609", "TEC front petal;TEC x [cm];TEC y [cm]", 200, -100, 100, 200, -100, 100 );

  k610 = fs->make<TProfile>( "k610", "TECr1 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TECr1 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k611 = fs->make<TH1D>( "k611", "TECr1-ax residuals #Deltax p_{t} > 4;TECr1-ax #Deltax [#mum];hits", 100, -250, 250 );
  k612 = fs->make<TH1D>( "k612", "TECr1s residuals #Deltax p_{t} > 4;TECr1s #Deltax [#mum];hits", 100, -250, 250 );
  k613 = fs->make<TProfile>( "k613", "TECr1 residuals #Deltax vs F;TEC F [deg];TECr1 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k614 = fs->make<TProfile>( "k614", "TECr1 residuals #sigma_{x} vs F;TEC F [deg];TECr1 rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k615 = fs->make<TH1D>( "k615", "TECr1 residuals #Deltax p_{t} > 12;TECr1 #Deltax [#mum];hits", 100, -250, 250 );

  k620 = fs->make<TProfile>( "k620", "TECr2 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TECr2 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k621 = fs->make<TH1D>( "k621", "TECr2-ax residuals #Deltax p_{t} > 4;TECr2-ax #Deltax [#mum];hits", 100, -250, 250 );
  k622 = fs->make<TH1D>( "k622", "TECr2s residuals #Deltax p_{t} > 4;TECr2s #Deltax [#mum];hits", 100, -250, 250 );
  k623 = fs->make<TProfile>( "k623", "TECr2 residuals #Deltax vs F;TEC F [deg];TECr2 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k624 = fs->make<TProfile>( "k624", "TECr2 residuals #sigma_{x} vs F;TEC F [deg];TECr2 rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k625 = fs->make<TH1D>( "k625", "TECr2 residuals #Deltax p_{t} > 12;TECr2 #Deltax [#mum];hits", 100, -250, 250 );

  k630 = fs->make<TProfile>( "k630", "TECr3 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TECr3 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k631 = fs->make<TH1D>( "k631", "TECr3-ax residuals #Deltax p_{t} > 4;TECr3-ax #Deltax [#mum];hits", 100, -250, 250 );
  k632 = fs->make<TH1D>( "k632", "TECr3s residuals #Deltax p_{t} > 4;TECr3s #Deltax [#mum];hits", 100, -250, 250 );
  k633 = fs->make<TProfile>( "k633", "TECr3 residuals #Deltax vs F;TEC F [deg];TECr3 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k634 = fs->make<TProfile>( "k634", "TECr3 residuals #sigma_{x} vs F;TEC F [deg];TECr3 rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k635 = fs->make<TH1D>( "k635", "TECr3 residuals #Deltax p_{t} > 12;TECr3 #Deltax [#mum];hits", 100, -250, 250 );

  k640 = fs->make<TProfile>( "k640", "TECr4 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TECr4 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k641 = fs->make<TH1D>( "k641", "TECr4-ax residuals #Deltax p_{t} > 4;TECr4-ax #Deltax [#mum];hits", 100, -250, 250 );
  k642 = fs->make<TH1D>( "k642", "TECr4s residuals #Deltax p_{t} > 4;TECr4s #Deltax [#mum];hits", 100, -250, 250 );
  k643 = fs->make<TProfile>( "k643", "TECr4 residuals #Deltax vs F;TEC F [deg];TECr4 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k644 = fs->make<TProfile>( "k644", "TECr4 residuals #sigma_{x} vs F;TEC F [deg];TECr4 rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k645 = fs->make<TH1D>( "k645", "TECr4 residuals #Deltax p_{t} > 12;TECr4 #Deltax [#mum];hits", 100, -250, 250 );

  k650 = fs->make<TProfile>( "k650", "TECr5 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TECr5 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k651 = fs->make<TH1D>( "k651", "TECr5-ax residuals #Deltax p_{t} > 4;TECr5-ax #Deltax [#mum];hits", 100, -250, 250 );
  k652 = fs->make<TH1D>( "k652", "TECr5s residuals #Deltax p_{t} > 4;TECr5s #Deltax [#mum];hits", 100, -250, 250 );
  k653 = fs->make<TProfile>( "k653", "TECr5 residuals #Deltax vs F;TEC F [deg];TECr5 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k654 = fs->make<TProfile>( "k654", "TECr5 residuals #sigma_{x} vs F;TEC F [deg];TECr5 rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k655 = fs->make<TH1D>( "k655", "TECr5 residuals #Deltax p_{t} > 12;TECr5 #Deltax [#mum];hits", 100, -250, 250 );

  k660 = fs->make<TProfile>( "k660", "TECr6 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TECr6 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k661 = fs->make<TH1D>( "k661", "TECr6-ax residuals #Deltax p_{t} > 4;TECr6-ax #Deltax [#mum];hits", 100, -250, 250 );
  k662 = fs->make<TH1D>( "k662", "TECr6s residuals #Deltax p_{t} > 4;TECr6s #Deltax [#mum];hits", 100, -250, 250 );
  k663 = fs->make<TProfile>( "k663", "TECr6 residuals #Deltax vs F;TEC F [deg];TECr6 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k664 = fs->make<TProfile>( "k664", "TECr6 residuals #sigma_{x} vs F;TEC F [deg];TECr6 rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k665 = fs->make<TH1D>( "k665", "TECr6 residuals #Deltax p_{t} > 12;TECr6 #Deltax [#mum];hits", 100, -250, 250 );

  k670 = fs->make<TProfile>( "k670", "TECr7 residuals #Deltax vs p_{t};log(p_{t} [GeV]);TECr7 rms(#Deltax) [#mum]", 20, 0, 2, 0, 499 );
  k671 = fs->make<TH1D>( "k671", "TECr7-ax residuals #Deltax p_{t} > 4;TECr7-ax #Deltax [#mum];hits", 100, -250, 250 );
  k672 = fs->make<TH1D>( "k672", "TECr7s residuals #Deltax p_{t} > 4;TECr7s #Deltax [#mum];hits", 100, -250, 250 );
  k673 = fs->make<TProfile>( "k673", "TECr7 residuals #Deltax vs F;TEC F [deg];TECr7 <#Deltax> [#mum]", 180, -180, 180, -499, 499 );
  k674 = fs->make<TProfile>( "k674", "TECr7 residuals #sigma_{x} vs F;TEC F [deg];TECr7 rms(#Deltax) [#mum]", 180, -180, 180, 0, 499 );
  k675 = fs->make<TH1D>( "k675", "TECr7 residuals #Deltax p_{t} > 12;TECr7 #Deltax [#mum];hits", 100, -250, 250 );

  k681 = fs->make<TH1D>( "k681", "TECw1 residuals #Deltax p_{t} > 4;TECw1 #Deltax [#mum];hits", 100, -250, 250 );
  k682 = fs->make<TH1D>( "k682", "TECw2 residuals #Deltax p_{t} > 4;TECw2 #Deltax [#mum];hits", 100, -250, 250 );
  k683 = fs->make<TH1D>( "k683", "TECw3 residuals #Deltax p_{t} > 4;TECw3 #Deltax [#mum];hits", 100, -250, 250 );
  k684 = fs->make<TH1D>( "k684", "TECw4 residuals #Deltax p_{t} > 4;TECw4 #Deltax [#mum];hits", 100, -250, 250 );
  k685 = fs->make<TH1D>( "k685", "TECw5 residuals #Deltax p_{t} > 4;TECw5 #Deltax [#mum];hits", 100, -250, 250 );
  k686 = fs->make<TH1D>( "k686", "TECw6 residuals #Deltax p_{t} > 4;TECw6 #Deltax [#mum];hits", 100, -250, 250 );
  k687 = fs->make<TH1D>( "k687", "TECw7 residuals #Deltax p_{t} > 4;TECw7 #Deltax [#mum];hits", 100, -250, 250 );
  k688 = fs->make<TH1D>( "k688", "TECw8 residuals #Deltax p_{t} > 4;TECw8 #Deltax [#mum];hits", 100, -250, 250 );
  k689 = fs->make<TH1D>( "k689", "TECw9 residuals #Deltax p_{t} > 4;TECw9 #Deltax [#mum];hits", 100, -250, 250 );

  // pixel rays:

  r301 = fs->make<TH1D>( "r301", "TIB-pxt residual;resid [mm];hits", 100, -5, 5 );
  r302 = fs->make<TProfile>( "r302", "TIB-pxt residual vs #phi;#phi [deg];<resid> [mm]", 90, -180, 180, -50, 50 );
  r303 = fs->make<TProfile>( "r303", "TIB-pxt residual vs #phi;#phi [deg];rms(resid) [mm]", 90, -180, 180, 0, 50 );
  r304 = fs->make<TProfile>( "r304", "TIB-pxt residual vs R;R [cm];<resid> [mm]", 35, 20, 55, -50, 50 );
  r305 = fs->make<TProfile>( "r305", "TIB-pxt residual vs R;R [cm];rms(resid) [mm]", 35, 20, 55, 0, 50 );
  r306 = fs->make<TProfile>( "r306", "TIB-pxt residual vs p_{t};log(p_{t} [deg]);<resid> [mm]", 20, 0, 2, -50, 50 );
  r307 = fs->make<TProfile>( "r307", "TIB-pxt residual vs p_{t};log(p_{t} [deg]);rms(resid) [mm]", 20, 0, 2, 0, 50 );

  r411 = fs->make<TH1D>( "r411", "TID-pxt arc length;s [cm];tracks", 100, 0, 100 );
  r412 = fs->make<TH2D>( "r412", "TID-pxt track y-x;x [cm];y [cm]", 100, -50, 50, 100, -50, 50 );
  r413 = fs->make<TH2D>( "r413", "TID-pxt det y-x;x [cm];y [cm]", 100, -50, 50, 100, -50, 50 );
  r414 = fs->make<TH1D>( "r414", "TID-pxt track-module;#Deltax [cm];hits", 100, -5, 5 );
  r415 = fs->make<TH1D>( "r415", "TID-pxt track-module;dy [cm];hits", 100, -5, 5 );

  r421 = fs->make<TH1D>( "r421", "TID-pxt residual;resid [mm];hits", 100, -10, 10 );
  r422 = fs->make<TProfile>( "r422", "TID-pxt residual vs #phi;#phi [deg];<resid> [mm]", 90, -180, 180, -10, 10 );
  r423 = fs->make<TProfile>( "r423", "TID-pxt residual vs #phi;#phi [deg];rms(resid) [mm]", 90, -180, 180, 0, 10 );
  r424 = fs->make<TProfile>( "r424", "TID-pxt residual vs R;R [cm];<resid> [mm]", 35, 20, 55, -10, 10 );
  r425 = fs->make<TProfile>( "r425", "TID-pxt residual vs R;R [cm];rms(resid) [mm]", 35, 20, 55, 0, 10 );
  r426 = fs->make<TProfile>( "r426", "TID-pxt residual vs p_{t};log(p_{t} [deg]);<resid> [mm]", 20, 0, 2, -10, 10 );
  r427 = fs->make<TProfile>( "r427", "TID-pxt residual vs p_{t};log(p_{t} [deg]);rms(resid) [mm]", 20, 0, 2, 0, 10 );

  r611 = fs->make<TH1D>( "r611", "TEC-pxt arc length;s [cm];tracks", 150, 0, 150 );
  r612 = fs->make<TH2D>( "r612", "TEC-pxt track y-x;x [cm];y [cm]", 110, -110, 110, 110, -110, 110 );
  r613 = fs->make<TH2D>( "r613", "TEC-pxt det y-x;x [cm];y [cm]", 110, -110, 110, 110, -110, 110 );
  r614 = fs->make<TH1D>( "r614", "TEC-pxt track-module;#Deltax [cm];hits", 100, -5, 5 );
  r615 = fs->make<TH1D>( "r615", "TEC-pxt track-module;dy [cm];hits", 100, -5, 5 );

  r620 = fs->make<TH1D>( "r620", "TEC-pxt residual;resid [cm];hits", 100, -5, 5 );
  r621 = fs->make<TH1D>( "r621", "TEC-pxt residual;resid [mm];hits", 100, -25, 25 );
  r622 = fs->make<TProfile>( "r622", "TEC-pxt residual vs #phi;#phi [deg];<resid> [mm]", 90, -180, 180, -50, 50 );
  r623 = fs->make<TProfile>( "r623", "TEC-pxt residual vs #phi;#phi [deg];rms(resid) [mm]", 90, -180, 180, 0, 50 );
  r624 = fs->make<TProfile>( "r624", "TEC-pxt residual vs R;R [cm];<resid> [mm]", 90, 20, 110, -50, 50 );
  r625 = fs->make<TProfile>( "r625", "TEC-pxt residual vs R;R [cm];rms(resid) [mm]", 90, 20, 110, 0, 50 );
  r626 = fs->make<TProfile>( "r626", "TEC-pxt residual vs p_{t};log(p_{t} [deg]);<resid> [mm]", 20, 0, 2, -50, 50 );
  r627 = fs->make<TProfile>( "r627", "TEC-pxt residual vs p_{t};log(p_{t} [deg]);rms(resid) [mm]", 20, 0, 2, 0, 50 );

  // unbiased resids:

  u110 = fs->make<TH1D>( "u110", "PXB1 residuals #Deltax;PXB1 #Deltax [#mum];hits", 100, -150, 150 );
  u120 = fs->make<TH1D>( "u120", "PXB2 residuals #Deltax;PXB2 #Deltax [#mum];hits", 100, -150, 150 );
  u130 = fs->make<TH1D>( "u130", "PXB3 residuals #Deltax;PXB3 #Deltax [#mum];hits", 100, -150, 150 );

  u310 = fs->make<TH1D>( "u310", "TIB1-ax residuals #Deltax;TIB1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u320 = fs->make<TH1D>( "u320", "TIB1-st residuals #Deltax;TIB1-st #Deltax [#mum];hits", 100, -250, 250 );
  u330 = fs->make<TH1D>( "u330", "TIB2-ax residuals #Deltax;TIB2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u340 = fs->make<TH1D>( "u340", "TIB2-st residuals #Deltax;TIB2-st #Deltax [#mum];hits", 100, -250, 250 );
  u350 = fs->make<TH1D>( "u350", "TIB3  residuals #Deltax;TIB3  #Deltax [#mum];hits", 100, -250, 250 );
  u360 = fs->make<TH1D>( "u360", "TIB4  residuals #Deltax;TIB4  #Deltax [#mum];hits", 100, -250, 250 );

  u410 = fs->make<TH1D>( "u410", "TID-s1-d1-ax residuals #Deltax;TID-s1-d1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u411 = fs->make<TH1D>( "u411", "TID-s1-d1-st residuals #Deltax;TID-s1-d1-st #Deltax [#mum];hits", 100, -250, 250 );
  u412 = fs->make<TH1D>( "u412", "TID-s1-d2-ax residuals #Deltax;TID-s1-d2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u413 = fs->make<TH1D>( "u413", "TID-s1-d2-st residuals #Deltax;TID-s1-d2-st #Deltax [#mum];hits", 100, -250, 250 );
  u414 = fs->make<TH1D>( "u414", "TID-s1-d3-ax residuals #Deltax;TID-s1-d3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u415 = fs->make<TH1D>( "u415", "TID-s1-d3-st residuals #Deltax;TID-s1-d3-st #Deltax [#mum];hits", 100, -250, 250 );

  u420 = fs->make<TH1D>( "u420", "TID-s2-d1-ax residuals #Deltax;TID-s2-d1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u421 = fs->make<TH1D>( "u421", "TID-s2-d1-st residuals #Deltax;TID-s2-d1-st #Deltax [#mum];hits", 100, -250, 250 );
  u422 = fs->make<TH1D>( "u422", "TID-s2-d2-ax residuals #Deltax;TID-s2-d2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u423 = fs->make<TH1D>( "u423", "TID-s2-d2-st residuals #Deltax;TID-s2-d2-st #Deltax [#mum];hits", 100, -250, 250 );
  u424 = fs->make<TH1D>( "u424", "TID-s2-d3-ax residuals #Deltax;TID-s2-d3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u425 = fs->make<TH1D>( "u425", "TID-s2-d3-st residuals #Deltax;TID-s2-d3-st #Deltax [#mum];hits", 100, -250, 250 );

  u430 = fs->make<TH1D>( "u430", "TID-s1-r1-ax residuals #Deltax;TID-s1-r1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u431 = fs->make<TH1D>( "u431", "TID-s1-r1-st residuals #Deltax;TID-s1-r1-st #Deltax [#mum];hits", 100, -250, 250 );
  u432 = fs->make<TH1D>( "u432", "TID-s1-r2-ax residuals #Deltax;TID-s1-r2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u433 = fs->make<TH1D>( "u433", "TID-s1-r2-st residuals #Deltax;TID-s1-r2-st #Deltax [#mum];hits", 100, -250, 250 );
  u434 = fs->make<TH1D>( "u434", "TID-s1-r3-ax residuals #Deltax;TID-s1-r3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u435 = fs->make<TH1D>( "u435", "TID-s1-r3-st residuals #Deltax;TID-s1-r3-st #Deltax [#mum];hits", 100, -250, 250 );

  u440 = fs->make<TH1D>( "u440", "TID-s2-r1-ax residuals #Deltax;TID-s2-r1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u441 = fs->make<TH1D>( "u441", "TID-s2-r1-st residuals #Deltax;TID-s2-r1-st #Deltax [#mum];hits", 100, -250, 250 );
  u442 = fs->make<TH1D>( "u442", "TID-s2-r2-ax residuals #Deltax;TID-s2-r2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u443 = fs->make<TH1D>( "u443", "TID-s2-r2-st residuals #Deltax;TID-s2-r2-st #Deltax [#mum];hits", 100, -250, 250 );
  u444 = fs->make<TH1D>( "u444", "TID-s2-r3-ax residuals #Deltax;TID-s2-r3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u445 = fs->make<TH1D>( "u445", "TID-s2-r3-st residuals #Deltax;TID-s2-r3-st #Deltax [#mum];hits", 100, -250, 250 );

  u510 = fs->make<TH1D>( "u510", "TOB1-ax residuals #Deltax;TOB1-ax #Deltax [#mum];hits", 100, -400, 400 );
  u520 = fs->make<TH1D>( "u520", "TOB1-st residuals #Deltax;TOB1-st #Deltax [#mum];hits", 100, -400, 400 );
  u530 = fs->make<TH1D>( "u530", "TOB2-ax residuals #Deltax;TOB2-ax #Deltax [#mum];hits", 100, -400, 400 );
  u540 = fs->make<TH1D>( "u540", "TOB2-st residuals #Deltax;TOB2-st #Deltax [#mum];hits", 100, -400, 400 );
  u550 = fs->make<TH1D>( "u550", "TOB3  residuals #Deltax;TOB3  #Deltax [#mum];hits", 100, -400, 400 );
  u560 = fs->make<TH1D>( "u560", "TOB4  residuals #Deltax;TOB4  #Deltax [#mum];hits", 100, -400, 400 );
  u570 = fs->make<TH1D>( "u570", "TOB5  residuals #Deltax;TOB5  #Deltax [#mum];hits", 100, -400, 400 );
  u580 = fs->make<TH1D>( "u580", "TOB6  residuals #Deltax;TOB6  #Deltax [#mum];hits", 100, -400, 400 );

  u610 = fs->make<TH1D>( "u610", "TEC-s1-w1-ax residuals #Deltax;TEC-s1-w1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u611 = fs->make<TH1D>( "u611", "TEC-s1-w1-st residuals #Deltax;TEC-s1-w1-st #Deltax [#mum];hits", 100, -250, 250 );
  u612 = fs->make<TH1D>( "u612", "TEC-s1-w2-ax residuals #Deltax;TEC-s1-w2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u613 = fs->make<TH1D>( "u613", "TEC-s1-w2-st residuals #Deltax;TEC-s1-w2-st #Deltax [#mum];hits", 100, -250, 250 );
  u614 = fs->make<TH1D>( "u614", "TEC-s1-w3-ax residuals #Deltax;TEC-s1-w3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u615 = fs->make<TH1D>( "u615", "TEC-s1-w3-st residuals #Deltax;TEC-s1-w3-st #Deltax [#mum];hits", 100, -250, 250 );
  u616 = fs->make<TH1D>( "u616", "TEC-s1-w4-ax residuals #Deltax;TEC-s1-w4-ax #Deltax [#mum];hits", 100, -250, 250 );
  u617 = fs->make<TH1D>( "u617", "TEC-s1-w4-st residuals #Deltax;TEC-s1-w4-st #Deltax [#mum];hits", 100, -250, 250 );
  u618 = fs->make<TH1D>( "u618", "TEC-s1-w5-ax residuals #Deltax;TEC-s1-w5-ax #Deltax [#mum];hits", 100, -250, 250 );
  u619 = fs->make<TH1D>( "u619", "TEC-s1-w5-st residuals #Deltax;TEC-s1-w5-st #Deltax [#mum];hits", 100, -250, 250 );
  u620 = fs->make<TH1D>( "u620", "TEC-s1-w6-ax residuals #Deltax;TEC-s1-w6-ax #Deltax [#mum];hits", 100, -250, 250 );
  u621 = fs->make<TH1D>( "u621", "TEC-s1-w6-st residuals #Deltax;TEC-s1-w6-st #Deltax [#mum];hits", 100, -250, 250 );
  u622 = fs->make<TH1D>( "u622", "TEC-s1-w7-ax residuals #Deltax;TEC-s1-w7-ax #Deltax [#mum];hits", 100, -250, 250 );
  u623 = fs->make<TH1D>( "u623", "TEC-s1-w7-st residuals #Deltax;TEC-s1-w7-st #Deltax [#mum];hits", 100, -250, 250 );
  u624 = fs->make<TH1D>( "u624", "TEC-s1-w8-ax residuals #Deltax;TEC-s1-w8-ax #Deltax [#mum];hits", 100, -250, 250 );
  u625 = fs->make<TH1D>( "u625", "TEC-s1-w8-st residuals #Deltax;TEC-s1-w8-st #Deltax [#mum];hits", 100, -250, 250 );
  u626 = fs->make<TH1D>( "u626", "TEC-s1-w9-ax residuals #Deltax;TEC-s1-w9-ax #Deltax [#mum];hits", 100, -250, 250 );
  u627 = fs->make<TH1D>( "u627", "TEC-s1-w9-st residuals #Deltax;TEC-s1-w9-st #Deltax [#mum];hits", 100, -250, 250 );

  u630 = fs->make<TH1D>( "u630", "TEC-s1-r1-ax residuals #Deltax;TEC-s1-r1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u631 = fs->make<TH1D>( "u631", "TEC-s1-r1-st residuals #Deltax;TEC-s1-r1-st #Deltax [#mum];hits", 100, -250, 250 );
  u632 = fs->make<TH1D>( "u632", "TEC-s1-r2-ax residuals #Deltax;TEC-s1-r2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u633 = fs->make<TH1D>( "u633", "TEC-s1-r2-st residuals #Deltax;TEC-s1-r2-st #Deltax [#mum];hits", 100, -250, 250 );
  u634 = fs->make<TH1D>( "u634", "TEC-s1-r3-ax residuals #Deltax;TEC-s1-r3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u635 = fs->make<TH1D>( "u635", "TEC-s1-r3-st residuals #Deltax;TEC-s1-r3-st #Deltax [#mum];hits", 100, -250, 250 );
  u636 = fs->make<TH1D>( "u636", "TEC-s1-r4-ax residuals #Deltax;TEC-s1-r4-ax #Deltax [#mum];hits", 100, -250, 250 );
  u637 = fs->make<TH1D>( "u637", "TEC-s1-r4-st residuals #Deltax;TEC-s1-r4-st #Deltax [#mum];hits", 100, -250, 250 );
  u638 = fs->make<TH1D>( "u638", "TEC-s1-r5-ax residuals #Deltax;TEC-s1-r5-ax #Deltax [#mum];hits", 100, -250, 250 );
  u639 = fs->make<TH1D>( "u639", "TEC-s1-r5-st residuals #Deltax;TEC-s1-r5-st #Deltax [#mum];hits", 100, -250, 250 );
  u640 = fs->make<TH1D>( "u640", "TEC-s1-r6-ax residuals #Deltax;TEC-s1-r6-ax #Deltax [#mum];hits", 100, -250, 250 );
  u641 = fs->make<TH1D>( "u641", "TEC-s1-r6-st residuals #Deltax;TEC-s1-r6-st #Deltax [#mum];hits", 100, -250, 250 );
  u642 = fs->make<TH1D>( "u642", "TEC-s1-r7-ax residuals #Deltax;TEC-s1-r7-ax #Deltax [#mum];hits", 100, -250, 250 );
  u643 = fs->make<TH1D>( "u643", "TEC-s1-r7-st residuals #Deltax;TEC-s1-r7-st #Deltax [#mum];hits", 100, -250, 250 );

  u650 = fs->make<TH1D>( "u650", "TEC-s2-w1-ax residuals #Deltax;TEC-s2-w1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u651 = fs->make<TH1D>( "u651", "TEC-s2-w1-st residuals #Deltax;TEC-s2-w1-st #Deltax [#mum];hits", 100, -250, 250 );
  u652 = fs->make<TH1D>( "u652", "TEC-s2-w2-ax residuals #Deltax;TEC-s2-w2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u653 = fs->make<TH1D>( "u653", "TEC-s2-w2-st residuals #Deltax;TEC-s2-w2-st #Deltax [#mum];hits", 100, -250, 250 );
  u654 = fs->make<TH1D>( "u654", "TEC-s2-w3-ax residuals #Deltax;TEC-s2-w3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u655 = fs->make<TH1D>( "u655", "TEC-s2-w3-st residuals #Deltax;TEC-s2-w3-st #Deltax [#mum];hits", 100, -250, 250 );
  u656 = fs->make<TH1D>( "u656", "TEC-s2-w4-ax residuals #Deltax;TEC-s2-w4-ax #Deltax [#mum];hits", 100, -250, 250 );
  u657 = fs->make<TH1D>( "u657", "TEC-s2-w4-st residuals #Deltax;TEC-s2-w4-st #Deltax [#mum];hits", 100, -250, 250 );
  u658 = fs->make<TH1D>( "u658", "TEC-s2-w5-ax residuals #Deltax;TEC-s2-w5-ax #Deltax [#mum];hits", 100, -250, 250 );
  u659 = fs->make<TH1D>( "u659", "TEC-s2-w5-st residuals #Deltax;TEC-s2-w5-st #Deltax [#mum];hits", 100, -250, 250 );
  u660 = fs->make<TH1D>( "u660", "TEC-s2-w6-ax residuals #Deltax;TEC-s2-w6-ax #Deltax [#mum];hits", 100, -250, 250 );
  u661 = fs->make<TH1D>( "u661", "TEC-s2-w6-st residuals #Deltax;TEC-s2-w6-st #Deltax [#mum];hits", 100, -250, 250 );
  u662 = fs->make<TH1D>( "u662", "TEC-s2-w7-ax residuals #Deltax;TEC-s2-w7-ax #Deltax [#mum];hits", 100, -250, 250 );
  u663 = fs->make<TH1D>( "u663", "TEC-s2-w7-st residuals #Deltax;TEC-s2-w7-st #Deltax [#mum];hits", 100, -250, 250 );
  u664 = fs->make<TH1D>( "u664", "TEC-s2-w8-ax residuals #Deltax;TEC-s2-w8-ax #Deltax [#mum];hits", 100, -250, 250 );
  u665 = fs->make<TH1D>( "u665", "TEC-s2-w8-st residuals #Deltax;TEC-s2-w8-st #Deltax [#mum];hits", 100, -250, 250 );
  u666 = fs->make<TH1D>( "u666", "TEC-s2-w9-ax residuals #Deltax;TEC-s2-w9-ax #Deltax [#mum];hits", 100, -250, 250 );
  u667 = fs->make<TH1D>( "u667", "TEC-s2-w9-st residuals #Deltax;TEC-s2-w9-st #Deltax [#mum];hits", 100, -250, 250 );

  u670 = fs->make<TH1D>( "u670", "TEC-s2-r1-ax residuals #Deltax;TEC-s2-r1-ax #Deltax [#mum];hits", 100, -250, 250 );
  u671 = fs->make<TH1D>( "u671", "TEC-s2-r1-st residuals #Deltax;TEC-s2-r1-st #Deltax [#mum];hits", 100, -250, 250 );
  u672 = fs->make<TH1D>( "u672", "TEC-s2-r2-ax residuals #Deltax;TEC-s2-r2-ax #Deltax [#mum];hits", 100, -250, 250 );
  u673 = fs->make<TH1D>( "u673", "TEC-s2-r2-st residuals #Deltax;TEC-s2-r2-st #Deltax [#mum];hits", 100, -250, 250 );
  u674 = fs->make<TH1D>( "u674", "TEC-s2-r3-ax residuals #Deltax;TEC-s2-r3-ax #Deltax [#mum];hits", 100, -250, 250 );
  u675 = fs->make<TH1D>( "u675", "TEC-s2-r3-st residuals #Deltax;TEC-s2-r3-st #Deltax [#mum];hits", 100, -250, 250 );
  u676 = fs->make<TH1D>( "u676", "TEC-s2-r4-ax residuals #Deltax;TEC-s2-r4-ax #Deltax [#mum];hits", 100, -250, 250 );
  u677 = fs->make<TH1D>( "u677", "TEC-s2-r4-st residuals #Deltax;TEC-s2-r4-st #Deltax [#mum];hits", 100, -250, 250 );
  u678 = fs->make<TH1D>( "u678", "TEC-s2-r5-ax residuals #Deltax;TEC-s2-r5-ax #Deltax [#mum];hits", 100, -250, 250 );
  u679 = fs->make<TH1D>( "u679", "TEC-s2-r5-st residuals #Deltax;TEC-s2-r5-st #Deltax [#mum];hits", 100, -250, 250 );
  u680 = fs->make<TH1D>( "u680", "TEC-s2-r6-ax residuals #Deltax;TEC-s2-r6-ax #Deltax [#mum];hits", 100, -250, 250 );
  u681 = fs->make<TH1D>( "u681", "TEC-s2-r6-st residuals #Deltax;TEC-s2-r6-st #Deltax [#mum];hits", 100, -250, 250 );
  u682 = fs->make<TH1D>( "u682", "TEC-s2-r7-ax residuals #Deltax;TEC-s2-r7-ax #Deltax [#mum];hits", 100, -250, 250 );
  u683 = fs->make<TH1D>( "u683", "TEC-s2-r7-st residuals #Deltax;TEC-s2-r7-st #Deltax [#mum];hits", 100, -250, 250 );
}

//
// member functions:
// method called once each job just before starting event loop
//
void Pxl::beginJob()
{
}

//----------------------------------------------------------------------
// method called for each event:

void Pxl::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup)
{
	const int run = iRun.run();

	std::map<int, Histos>::iterator iter = runmap.find(run);
	if(iter != runmap.end())
	{
	  static_cast<Histos&>(*this) = iter->second;
	}
	else
	{
	  edm::Service<TFileService> fs;
	  
	  std::stringstream runstr;
	  runstr << "Run" << run;
	  TFileDirectory subdir = fs->mkdir(runstr.str().c_str());
	  
	  runmap[run].init(&subdir);
	  static_cast<Histos&>(*this) = runmap[run];
	}	

	bool hltSetupChanged = false;
	std::cout<<_triggerSrc<<" "<<_triggerSrc.label()<<" "<<_triggerSrc.process()<<" "
		 <<_triggerSrc.instance()<<" "<<std::endl;

	//const bool showTrigger = false;

	if(_triggerSrc.label()!="") { 
	  if(!HLTConfig.init(iRun, iSetup, _triggerSrc.process(), hltSetupChanged))
		throw cms::Exception("Failed to init HLT config");
	}
}

void Pxl::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace math;
  
  const double pi = 4*atan(1);
  const double wt = 180/pi;
  const double twopi = 2*pi;
  const double pihalf = 2*atan(1);
  const double sqrtpihalf = sqrt(pihalf);

  myCounters::neve++;
  if( myCounters::prevrun != iEvent.run() ){

    time_t unixZeit = iEvent.time().unixTime();
    cout << "new run " << iEvent.run();
    cout << ", LumiBlock " << iEvent.luminosityBlock();
    cout << " taken " << ctime(&unixZeit); // ctime has endline

    myCounters::prevrun = iEvent.run();

  }// new run

  int idbg = 0;  // printout for the first few events
  if( myCounters::neve < 2 ) idbg = 1;

  int jdbg = 0; // special printout

  if( idbg ) {
    cout << endl;
    cout << "run " << iEvent.run();
    cout << ", LumiBlock " << iEvent.luminosityBlock();
    cout << ", event " << iEvent.eventAuxiliary().event();
    time_t unixZeit = iEvent.time().unixTime();
    cout << ", taken " << ctime(&unixZeit); // ctime has endline
  }


  //----------------- HLT -------------------------------------------

  // Trigger information, do only if the container is defined
  if(_triggerSrc.label()!="" && idbg==1 ) { 
    edm::Handle<edm::TriggerResults> triggerResults;
    iEvent.getByLabel(_triggerSrc, triggerResults);
    assert(triggerResults->size() == HLTConfig.size());

    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults);
    for(unsigned int i = 0; i < triggerResults->size(); ++i)
    {
      std::string triggerName = triggerNames.triggerName(i);
      // this does not work in 74X
      //std::pair<int, int> prescale = HLTConfig.prescaleValues(iEvent, iSetup, triggerName);
      // this compiles&runs but I am not sure how to use it?
      std::pair<std::vector<std::pair<std::basic_string<char>, int> >, int> 
	prescale = HLTConfig.prescaleValuesInDetail(iEvent, iSetup, triggerName);
      std::cout << i << ": " << triggerName << ", " << prescale.second << std::endl;
    }
  }

  //--------------------------------------------------------------------
  // beam spot:

  edm::Handle<reco::BeamSpot> rbs;
  iEvent.getByLabel( "offlineBeamSpot", rbs );

  XYZPoint bsP = XYZPoint(0,0,0);
  int ibs = 0;

  if( rbs.failedToGet() ) return;
  if( ! rbs.isValid() ) return;

  ibs = 1;
  h000->Fill( rbs->betaStar() );
  h001->Fill( rbs->emittanceX() );
  h002->Fill( rbs->BeamWidthX()*1e4 );
  h003->Fill( rbs->BeamWidthY()*1e4 );
  h004->Fill( rbs->sigmaZ() );
  h005->Fill( rbs->x0() );
  h006->Fill( rbs->y0() );
  h007->Fill( rbs->z0() );
  h008->Fill( rbs->dxdz()*1e3 );
  h009->Fill( rbs->dydz()*1e3 );
  bsP = XYZPoint( rbs->x0(), rbs->y0(), rbs->z0() );

  double bx = rbs->BeamWidthX();
  double by = rbs->BeamWidthY();

  if( idbg ){
    cout << "beam spot x " << rbs->x0();
    cout << ", y " << rbs->y0();
    cout << ", z " << rbs->z0();
    cout << endl;
  }

  //--------------------------------------------------------------------
  // primary vertices:

  Handle<VertexCollection> vertices;
  iEvent.getByLabel( "offlinePrimaryVertices", vertices );

  if( vertices.failedToGet() ) return;
  if( !vertices.isValid() ) return;

  int nvertex = vertices->size();
  h010->Fill( nvertex );

  // need vertex global point for tracks
  // from #include "DataFormats/GeometryVector/interface/GlobalPoint.h"
  // Global points are three-dimensional by default
  // typedef Global3DPoint  GlobalPoint;
  // typedef Point3DBase< float, GlobalTag> Global3DPoint;

  XYZPoint vtxN = XYZPoint(0,0,0);
  XYZPoint vtxP = XYZPoint(0,0,0);

  double bestNdof = 0;
  double maxSumPt = 0;
  Vertex bestPvx;

  for( VertexCollection::const_iterator iVertex = vertices->begin();
      iVertex != vertices->end(); ++iVertex ) {

    if( ! iVertex->isValid() )
      h011->Fill( iVertex->z() );

    else {

      if( iVertex->isFake() ) 
	h012->Fill( iVertex->z() );

      else{
	h013->Fill( iVertex->z() );
	h014->Fill( iVertex->x() );
	h015->Fill( iVertex->y() );
	h016->Fill( iVertex->ndof() );
	h017->Fill( iVertex->ndof() );

	if( idbg ){
	  cout << "vertex";
	  cout << ": x " << iVertex->x();
	  cout << ", y " << iVertex->y();
	  cout << ", z " << iVertex->z();
	  cout << ", ndof " << iVertex->ndof();
	  cout << ", sumpt " << iVertex->p4().pt();
	  cout << endl;
	}

	if( iVertex->hasRefittedTracks() )
	  h018->Fill( iVertex->z() ); // empty in Zmumu sample
	else
	  h019->Fill( iVertex->z() ); // all here: why?

	if( iVertex->ndof() > bestNdof ) {
	  bestNdof = iVertex->ndof();
	  vtxN = XYZPoint( iVertex->x(), iVertex->y(), iVertex->z() );
	}

	h021->Fill( iVertex->p4().pt() );

	if( iVertex->p4().pt() > maxSumPt ) {
	  maxSumPt = iVertex->p4().pt();
	  vtxP = XYZPoint( iVertex->x(), iVertex->y(), iVertex->z() );
	  bestPvx = *iVertex;
	}
      }// non-fake
    }//valid
  } // loop over vertices

  h022->Fill( maxSumPt );

  //if( maxSumPt < 1 ) return;
  if(!singleParticleMC && maxSumPt < 1 ) return;

  if( maxSumPt < 1 ) vtxP = vtxN;

  h023->Fill( vtxP.x() );
  h024->Fill( vtxP.y() );
  h025->Fill( vtxP.z() );

  double xBS = 0;
  double yBS = 0;
  if( ibs ) {
    xBS = bsP.x();
    yBS = bsP.y();
  }
  else {
    xBS = vtxP.x();
    yBS = vtxP.y();
  }

  //--------------------------------------------------------------------
  // MET:
  if(0) {
    edm::Handle< edm::View<reco::PFMET> > pfMEThandle;
    iEvent.getByLabel( "pfMet", pfMEThandle );
    
    if( !pfMEThandle.failedToGet() && pfMEThandle.isValid()){
      
      h026->Fill( pfMEThandle->front().sumEt() );
      h027->Fill( pfMEThandle->front().et() );
    }
  } // if MET


  //--------------------------------------------------------------------
  // get a fitter to refit TrackCandidates, the same fitter as used in standard reconstruction:
  // Fitter = cms.string('KFFittingSmootherWithOutliersRejectionAndRK'),
  // KalmanFilter
  // RungeKutta


#ifdef NEW_TRACKINGRECHITS

  // Fitter
  edm::ESHandle<TrajectoryFitter> aFitter;
  iSetup.get<TrajectoryFitter::Record>().get("KFFittingSmootherWithOutliersRejectionAndRK",aFitter);
  std::unique_ptr<TrajectoryFitter> theFitter = aFitter->clone();
  
  //----------------------------------------------------------------------------
  // Transient Rechit Builders
  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", theB );

  // Transient rec hits:
  ESHandle<TransientTrackingRecHitBuilder> hitBuilder;
  iSetup.get<TransientRecHitRecord>().get( _ttrhBuilder, hitBuilder );

  // Cloner, New from 71Xpre7
  const TkTransientTrackingRecHitBuilder * builder = 
    static_cast<TkTransientTrackingRecHitBuilder const *>(hitBuilder.product());
  //dynamic_cast<TkTransientTrackingRecHitBuilder const *>(hitBuilder.product());
  auto hitCloner = builder->cloner();
  //static_cast<TkTransientTrackingRecHitBuilder const *>(hitBuilder.product())->cloner();
  
  theFitter->setHitCloner(&hitCloner);

#else 

  // old
  ESHandle<TrajectoryFitter> TF;
  iSetup.get<TrajectoryFitter::Record>().get( "KFFittingSmootherWithOutliersRejectionAndRK", TF );
  const TrajectoryFitter* theFitter = TF.product();

  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", theB );

  // transient rec hits:
  ESHandle<TransientTrackingRecHitBuilder> hitBuilder;
  iSetup.get<TransientRecHitRecord>().get( _ttrhBuilder, hitBuilder );

#endif

  //--------------------------------------------------------------------
  // TrackPropagator:
  edm::ESHandle<Propagator> prop;
  iSetup.get<TrackingComponentsRecord>().get( "PropagatorWithMaterial", prop );
  const Propagator* thePropagator = prop.product();


  //--------------------------------------------------------------------
  // tracks:
  Handle<TrackCollection> tracks;

  iEvent.getByLabel( "generalTracks", tracks );

  if( tracks.failedToGet() ) return;
  if( !tracks.isValid() ) return;

  if( idbg ){
    cout << "  tracks " << tracks->size();
    cout << endl;
  }

  h030->Fill( tracks->size() );

  //----------------------------------------------------------------------------
  // get tracker geometry:

  edm::ESHandle<TrackerGeometry> pTG;
  iSetup.get<TrackerDigiGeometryRecord>().get( pTG );

  if( ! pTG.isValid() ) {
    cout << "Unable to find TrackerDigiGeometry. Return\n";
    return;
  }

  // loop over tracker detectors:
  // This is just to test some parameters 
#if 0
  for( TrackerGeometry::DetContainer::const_iterator idet = pTG->dets().begin();
       idet != pTG->dets().end(); ++idet ) {

    DetId mydetId = (*idet)->geographicalId();
    uint32_t mysubDet = mydetId.subdetId();

    if( mysubDet == PixelSubdetector::PixelBarrel || 
	mysubDet == PixelSubdetector::PixelEndcap ) {

    //TEST
    const GeomDet * myGeomDet = pTG->idToDet( mydetId );
    //Surface::GlobalPoint gp = myGeomDet->toGlobal( lp );
    cout<<myGeomDet<<" ? ";
    if(myGeomDet != NULL) {
      LocalError lape = myGeomDet->localAlignmentError();
      cout<< lape.valid() <<" ";
      if (lape.valid()) {
	float tmp11= 0.;
	if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
	float tmp13= 0.;
	if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;

	if( mysubDet == PixelSubdetector::PixelBarrel ) {
	  cout << ": PXB layer " << PXBDetId(mydetId).layer();
	  cout << ", ladder " << PXBDetId(mydetId).ladder();
	  cout << ", module " << PXBDetId(mydetId).module()<<" ";
	} else if( mysubDet == PixelSubdetector::PixelEndcap ) {
	  cout << ": PXD side " << PXFDetId(mydetId).side();
	  cout << ", disk " << PXFDetId(mydetId).disk();
	  cout << ", blade " << PXFDetId(mydetId).blade()<<" ";
	}
	cout<<tmp11<<" "<<tmp13;
	//h088->Fill( tmp11 );
	//h089->Fill( tmp13 );
      }
    }
    cout<<endl;

    //    if( myCounters::neve == 1 ){
    if( myCounters::neve == 0 ){

      //cout << "Det " << mydetId.det();
      //cout << ", subDet " << mydetId.subdetId();

      if( mysubDet == PixelSubdetector::PixelBarrel ) {

	cout << ": PXB layer " << PXBDetId(mydetId).layer();
	cout << ", ladder " << PXBDetId(mydetId).ladder();
	cout << ", module " << PXBDetId(mydetId).module();
	cout << ", at R " << (*idet)->position().perp();
	cout << ", F " << (*idet)->position().barePhi()*wt;
	cout << ", z " << (*idet)->position().z();
	cout << endl;
	cout << "rot x";
	cout << "\t" << (*idet)->rotation().xx();
	cout << "\t" << (*idet)->rotation().xy();
	cout << "\t" << (*idet)->rotation().xz();
	cout << endl;
	cout << "rot y";
	cout << "\t" << (*idet)->rotation().yx();
	cout << "\t" << (*idet)->rotation().yy();
	cout << "\t" << (*idet)->rotation().yz();
	cout << endl;
	cout << "rot z";
	cout << "\t" << (*idet)->rotation().zx();
	cout << "\t" << (*idet)->rotation().zy();
	cout << "\t" << (*idet)->rotation().zz();
	cout << endl;

	// normal vector: includes alignment (varies from module to module along z on one ladder)
	// neighbouring ladders alternate with inward/outward orientation

	cout << "normal";
	cout << ": x " << (*idet)->surface().normalVector().x();
	cout << ", y " << (*idet)->surface().normalVector().y();
	cout << ", z " << (*idet)->surface().normalVector().z();
	cout << ", f " << (*idet)->surface().normalVector().barePhi()*wt;
	cout << endl;

      }//PXB

      if( mysubDet == PixelSubdetector::PixelEndcap ) {

	cout << ": PXD side " << PXFDetId(mydetId).side();
	cout << ", disk " << PXFDetId(mydetId).disk();
	cout << ", blade " << PXFDetId(mydetId).blade();
	cout << ", panel " << PXFDetId(mydetId).panel();
	cout << ", module " << PXFDetId(mydetId).module();
	cout << ", at R " << (*idet)->position().perp();
	cout << ", F " << (*idet)->position().barePhi()*wt;
	cout << ", z " << (*idet)->position().z();
	cout << endl;
	cout << "rot x";
	cout << "\t" << (*idet)->rotation().xx();
	cout << "\t" << (*idet)->rotation().xy();
	cout << "\t" << (*idet)->rotation().xz();
	cout << endl;
	cout << "rot y";
	cout << "\t" << (*idet)->rotation().yx();
	cout << "\t" << (*idet)->rotation().yy();
	cout << "\t" << (*idet)->rotation().yz();
	cout << endl;
	cout << "rot z";
	cout << "\t" << (*idet)->rotation().zx();
	cout << "\t" << (*idet)->rotation().zy();
	cout << "\t" << (*idet)->rotation().zz();
	cout << endl;
	cout << "normal";
	cout << ": x " << (*idet)->surface().normalVector().x();
	cout << ", y " << (*idet)->surface().normalVector().y();
	cout << ", z " << (*idet)->surface().normalVector().z();
	cout << ", f " << (*idet)->surface().normalVector().barePhi()*wt;
	cout << endl;

      }//PXD

    }//idbg

    } // if pixels
  }//idet
#endif

#if 0
  edm::ESHandle<SiPixelLorentzAngle> SiPixelLorentzAngle_; 
  iSetup.get<SiPixelLorentzAngleRcd>().get(SiPixelLorentzAngle_);
  std::map<unsigned int,float> detid_la= SiPixelLorentzAngle_->getLorentzAngles();
  std::map<unsigned int,float>::const_iterator it;
  for (it=detid_la.begin();it!=detid_la.end();it++)
  {
    unsigned int subdet   = DetId(it->first).subdetId();
    if(subdet == static_cast<int>(PixelSubdetector::PixelBarrel))
      std::cout << it->first << ": " << it->second << std::endl;
  }
#endif

  //----------------------------------------------------------------------------
  // Tracks:

  double sumpt = 0;
  double sumq = 0;
  Surface::GlobalPoint origin = Surface::GlobalPoint(0,0,0);
  const TrackerGeomDet * geomDet2 = NULL;
  for( TrackCollection::const_iterator iTrack = tracks->begin();
      iTrack != tracks->end(); ++iTrack ) {

    // cpt = cqRB = 0.3*R[m]*B[T] = 1.14*R[m] for B=3.8T
    // D = 2R = 2*pt/1.14
    // calo: D = 1.3 m => pt = 0.74 GeV/c

    double pt = iTrack->pt();
    double pp = iTrack->p();

    //if( pt < 0.75 ) continue;// curls up
    //if( pt < 1.75 ) continue;// want sharper image

    //float tmp = abs(iTrack->dxy(vtxP))/iTrack->dxyError();
    //cout<<pt<<" "<<abs(iTrack->dxy(vtxP))<<" "<<iTrack->dxyError()<<" "<<tmp<<endl;

    h090->Fill( (iTrack->dxy(vtxP))*1E4 );
    h091->Fill( (iTrack->dxyError())*1E4 );

    if(!singleParticleMC && 
       (abs( iTrack->dxy(vtxP) ) > 5*iTrack->dxyError()) ) continue; // not prompt


    sumpt += pt;
    sumq += iTrack->charge();

    double logpt = log(pt) / log(10);

    h031->Fill( iTrack->charge() );
    h032->Fill( pt );
    h033->Fill( pt );

    const reco::HitPattern& hp = iTrack->hitPattern();

    h035->Fill( hp.numberOfValidTrackerHits() );
    h036->Fill( hp.numberOfValidPixelBarrelHits() );
    h037->Fill( hp.trackerLayersWithMeasurement() );
    h038->Fill( hp.pixelBarrelLayersWithMeasurement() );

    if( idbg ) {
      cout << endl;
      cout << "Track " << distance( tracks->begin(), iTrack );
      cout << ": pt " << iTrack->pt();
      cout << ", eta " << iTrack->eta();
      cout << ", phi " << iTrack->phi()*wt;
      cout << setprecision(1);
      cout << ", dxyv " << iTrack->dxy(vtxP)*1E4 << " um";
      cout << ", dzv " << iTrack->dz(vtxP)*1E1 << " mm";
      cout << setprecision(4);
      cout << ", hits " << hp.numberOfHits(HitPattern::TRACK_HITS);/// assuming HitCategory =TRACK_HITS = 0
      cout << ", valid " << hp.numberOfValidTrackerHits();
      cout << endl;
    }

    double phi = iTrack->phi();
    double dca = iTrack->d0(); // w.r.t. origin                               
    //double dca = -iTrack->dxy(); // dxy = -d0                               
    double dip = iTrack->lambda();
    double z0  = iTrack->dz();
    double tet = pihalf - dip;
    //double eta = iTrack->eta();

    // beam line at z of track, taking beam tilt into account

    double zBeam = iTrack->dz(bsP);//z0p of track along beam line w.r.t. beam z center
    double xBeam = rbs->x0() + rbs->dxdz() * zBeam;//beam at z of track
    double yBeam = rbs->y0() + rbs->dydz() * zBeam;
    double z0p =  zBeam + bsP.z(); // z0p of track along beam line w.r.t. CMS z = 0
    XYZPoint blP = XYZPoint( xBeam, yBeam, z0p );//point on beam line at z of track

    xBS = xBeam;//improve
    yBS = yBeam;//beam tilt taken into account

    double bcap = iTrack->dxy(blP);//impact parameter to beam
    double edca = iTrack->dxyError();
    double ebca = sqrt( edca*edca + bx*by );//round beam
    double sbca = bcap / ebca;//impact parameter significance

    if( hp.hasValidHitInFirstPixelBarrel() &&
	hp.trackerLayersWithMeasurement() > 7 ) {

      if( pt > 8 ) {

	h040->Fill( bcap*1E4 );//26 um in Oct 2011, 21 um Apr 2012
	h041->Fill( edca*1E4 );
	h042->Fill( sbca );//1.02 in 2011 reReco

      }

      // vs logpt:
      // profile of abs(dca) gives mean abs(dca):
      // mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
      // => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)

      h043->Fill( logpt, ebca*1E4 );
      if( abs(sbca) < 5 ) {
	h044->Fill( logpt, sqrtpihalf*abs(bcap)*1E4 );
	h045->Fill( logpt, sqrtpihalf*abs(sbca) );
      }

    }//long tracks

    // transient track:
    TransientTrack tTrack = theB->build(*iTrack);

    TrajectoryStateOnSurface initialTSOS = tTrack.innermostMeasurementState();

    double kap = tTrack.initialFreeState().transverseCurvature();

    TrajectoryStateClosestToPoint tscp = tTrack.trajectoryStateClosestToPoint( origin );

    if( tscp.isValid() ) {

      h057->Fill( tscp.referencePoint().x() ); // 0.0
      h058->Fill( tscp.referencePoint().y() ); // 0.0
      h059->Fill( tscp.referencePoint().z() ); // 0.0
      kap = tscp.perigeeParameters().transverseCurvature();
      phi = tscp.perigeeParameters().phi();
      dca = tscp.perigeeParameters().transverseImpactParameter();
      tet = tscp.perigeeParameters().theta();
      z0  = tscp.perigeeParameters().longitudinalImpactParameter();
      dip = pihalf - tet;

      h051->Fill( kap - tTrack.initialFreeState().transverseCurvature() ); // identical
      h052->Fill( phi - iTrack->phi() ); // identical
      h053->Fill( dca - iTrack->d0() ); // identical
      h054->Fill( dip - iTrack->lambda() ); // identical
      h055->Fill( z0  - iTrack->dz() ); // identical

    }//tscp valid

    double cf = cos(phi);
    double sf = sin(phi);
    //double xdca =  dca * sf;
    //double ydca = -dca * cf;

    //double tt = tan(tet);

    double rinv = -kap; // Karimaki                                           
    double rho = 1/kap;
    double erd = 1.0 - kap*dca;
    double drd = dca * ( 0.5*kap*dca - 1.0 ); // 0.5 * kap * dca**2 - dca;
    double hkk = 0.5*kap*kap;

    // track w.r.t. beam (cirmov):

    double dp = -xBS*sf + yBS*cf + dca;
    double dl = -xBS*cf - yBS*sf;
    double sa = 2*dp + rinv * ( dp*dp + dl*dl );
    double dcap = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to beam
    double ud = 1 + rinv*dca;
    double phip = atan2( -rinv*xBS + ud*sf, rinv*yBS + ud*cf );//direction

    // track at R(PXB1), from FUNPHI, FUNLEN:

    double R1 = 4.4; // PXB1

    double s1 = 0;
    double fpos1 = phi - pihalf;

    if( R1 >= abs(dca) ) {

      // sin(delta phi):

      double sindp = ( 0.5*kap * (R1*R1 + dca*dca) - dca ) / (R1*erd);
      fpos1 = phi + asin(sindp); // phi position at R1

      // sin(alpha):

      double sina = R1*kap * sqrt( 1.0 - sindp*sindp );

      // s = alpha / kappa:

      if( sina >= 1.0 )
	s1 = pi / kap;
      else{
	if( sina <= -1.0 )
	  s1 = -pi / kap;
	else
	  s1 = asin(sina) / kap;//always positive
      }

      // Check direction: limit to half-turn

      if( hkk * ( R1*R1 - dca*dca ) > erd ) s1 = pi/abs(kap) - s1; // always positive

    }// R1 > dca

    if( fpos1 > pi ) fpos1 -= twopi;
    else if( fpos1 < -pi ) fpos1 += twopi;

    double zR1 = z0 + s1*tan(dip); // z at R1

    //--------------------------------------------------------------------------
    // loop over tracker detectors:

    double xcrss[99];
    double ycrss[99];
    double zcrss[99];
    int ncrss = 0;

    for( TrackerGeometry::DetContainer::const_iterator idet = pTG->dets().begin();
	 idet != pTG->dets().end(); ++idet ) {

      DetId mydetId = (*idet)->geographicalId();
      uint32_t mysubDet = mydetId.subdetId();

      if( mysubDet != PixelSubdetector::PixelBarrel ) continue;

      /*
	cout << ": PXB layer " << PXBDetId(mydetId).layer();
	cout << ", ladder " << PXBDetId(mydetId).ladder();
	cout << ", module " << PXBDetId(mydetId).module();
	cout << ", at R1 " << (*idet)->position().perp();
	cout << ", F " << (*idet)->position().barePhi()*wt;
	cout << ", z " << (*idet)->position().z();
	cout << endl;
      */

      if( PXBDetId(mydetId).layer() == 1 ) {

	double dz = zR1 - (*idet)->position().z();

	if( abs(dz) > 4.0 ) continue;

	double df = fpos1 - (*idet)->position().barePhi();//track phi at R1 vs ladder phi

	if( df > pi ) df -= twopi;
	else if( df < -pi ) df += twopi;

	if( abs(df)*wt > 36.0 ) continue;//coarse matching track to ladder

	// normal vector: includes alignment (varies from module to module along z on one ladder)
	// neighbouring ladders alternate with inward/outward orientation

	/*
	  cout << "normal";
	  cout << ": x " << (*idet)->surface().normalVector().x();
	  cout << ", y " << (*idet)->surface().normalVector().y();
	  cout << ", z " << (*idet)->surface().normalVector().z();
	  cout << ", f " << (*idet)->surface().normalVector().barePhi()*wt;
	  cout << endl;
	*/

	double phiN = (*idet)->surface().normalVector().barePhi();//normal vector

	double phidet = phiN - pihalf;// orientation of sensor plane in x-y
	double ux = cos(phidet);// vector in sensor plane
	double uy = sin(phidet);
	double x = (*idet)->position().x();
	double y = (*idet)->position().y();

	// intersect helix with line: FUNRXY (in FUNPHI) from V. Blobel
	// factor f for intersect point (x + f*ux, y + f*uy)

	double a =                                 kap * ( ux*ux + uy*uy ) * 0.5;
	double b =       erd * ( ux*sf - uy*cf ) + kap * ( ux*x + uy*y );
	double c = drd + erd * (  x*sf -  y*cf ) + kap * (  x*x +  y*y ) * 0.5;
	double dis = b*b - 4.0*a*c;
	double f = 0;

	if( dis > 0 ) {

	  dis = sqrt(dis);
	  double f1 = 0;
	  double f2 = 0;

	  if( b < 0 ) {
            f1 = 0.5 * ( dis - b ) / a;
	    f2 = 2.0 * c / ( dis - b );
	  }
	  else{
            f1 = -0.5 * ( dis + b ) / a;
            f2 = -2.0 * c / ( dis + b );
	  }

	  f = f1;
	  if( abs(f2) < abs(f1) ) f = f2;

	}//dis

	xcrss[ncrss] = x + f*ux;
	ycrss[ncrss] = y + f*uy;
	double r = sqrt( xcrss[ncrss]*xcrss[ncrss] + ycrss[ncrss]*ycrss[ncrss] );

	double s = 0;

	if( r >= abs(dca) ) {
	  double sindp = ( 0.5*kap * ( r*r + dca*dca ) - dca ) / (r*erd);
	  double sina = r*kap * sqrt( 1.0 - sindp*sindp );
	  if( sina >= 1.0 )
	    s = pi / kap;
	  else{
	    if( sina <= -1.0 )
	      s = -pi / kap;
	    else
	      s = asin(sina) / kap;
	  }
	  if( hkk * ( r*r - dca*dca ) > erd ) s = pi/abs(kap) - s;
	}

	zcrss[ncrss] = z0 + s*tan(dip); // z at r

	ncrss++;

      }//PXB1

    }//idet

    //--------------------------------------------------------------------------
    // rec hits from track extra:

    if( iTrack->extra().isNull() ) continue;//next track
    if( ! iTrack->extra().isAvailable() ) continue;//next track

    h034->Fill( tTrack.recHitsSize() ); // tTrack

    double rmin = 99.9;
    uint32_t innerDetId = 0;
    double xPXB1 = 0;
    double yPXB1 = 0;
    double zPXB1 = 0;
    double uPXB1 = 0;
    //double vPXB1 = 0;
    //double ePXB1 = 0;
    //double fPXB1 = 0;

    double xPXB2 = 0;
    double yPXB2 = 0;
    double zPXB2 = 0;
    double uPXB2 = 0;
    double vPXB2 = 0;
    double ePXB2 = 0;
    double fPXB2 = 0;

    double xPXB3 = 0;
    double yPXB3 = 0;
    double zPXB3 = 0;
    double uPXB3 = 0;


    //double vPXB3 = 0;
    //double ePXB3 = 0;
    //double fPXB3 = 0;

    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    double phiN1 = 0;
    double phiN2 = 0;
    double phiN3 = 0;
    //double clch1 = 0;
    double clch2 = 0;
    //double clch3 = 0;
    //int ncol1 = 0;
    //int ncol2 = 0;
    //int ncol3 = 0;
    int nrow1 = 0;
    int nrow2 = 0;
    int nrow3 = 0;
    //double etaX1 = 0;
    double etaX2 = 0;
    //double etaX3 = 0;
    //double cogp1 = 0;
    double cogp2 = 0;
    //double cogp3 = 0;
    double xmid2 = 0;
    double ymid2 = 0;
    const GeomDet * det2 = NULL;
    int ilad2 = 0;
    int xmin2 = 0;
    int xmax2 = 0;
    int zmin2 = 0;
    int zmax2 = 0;

    int nTIB1 = 0;
    double xTIB1 = 0;
    double yTIB1 = 0;
    //double zTIB1 = 0;
    //double phiNTIB1 = 0;

    int nTIB2 = 0;
    double xTIB2 = 0;
    double yTIB2 = 0;
    double zTIB2 = 0;
    double phiNTIB2 = 0;

    int nTIB3 = 0;
    double xTIB3 = 0;
    double yTIB3 = 0;
    double zTIB3 = 0;
    double phiNTIB3 = 0;

    int nTIB4 = 0;
    double xTIB4 = 0;
    double yTIB4 = 0;
    //double zTIB4 = 0;
    //double phiNTIB4 = 0;

    int nTOB1 = 0;
    //double xTOB1 = 0;
    //double yTOB1 = 0;
    //double zTOB1 = 0;
    //double phiNTOB1 = 0;

    int nTOB2 = 0;
    //double xTOB2 = 0;
    //double yTOB2 = 0;
    //double zTOB2 = 0;
    //double phiNTOB2 = 0;

    int nTOB3 = 0;
    double xTOB3 = 0;
    double yTOB3 = 0;
    //double zTOB3 = 0;
    //double phiNTOB3 = 0;

    int nTOB4 = 0;
    double xTOB4 = 0;
    double yTOB4 = 0;
    double zTOB4 = 0;
    double phiNTOB4 = 0;

    int nTOB5 = 0;
    double xTOB5 = 0;
    double yTOB5 = 0;
    double zTOB5 = 0;
    double phiNTOB5 = 0;

    int nTOB6 = 0;
    double xTOB6 = 0;
    double yTOB6 = 0;
    //double zTOB6 = 0;
    //double phiNTOB6 = 0;

    int imod = 0;
    // std::cout << " imod 0 "<< imod<< std::endl;

    int nTEC[3][10];// initialized to 0 by default
    for( int is = 0; is < 3; ++is ){
      for( int iw = 0; iw < 10; ++iw ){
	nTEC[is][iw] = 0;
      }
    }
    double xTEC[3][10];
    double yTEC[3][10];
    double zTEC[3][10];
    //double phiNTEC[3][10];
    DetId detTEC[3][10];

    edm::OwnVector<TrackingRecHit> recHitVector; // for seed

    std::vector<TransientTrackingRecHit::RecHitPointer> myTTRHvec;

    Trajectory::RecHitContainer coTTRHvec; // for fit, constant

    // loop over recHits on this track:

    for( trackingRecHit_iterator irecHit = iTrack->recHitsBegin();
	 irecHit != iTrack->recHitsEnd(); ++irecHit ) {

      DetId detId = (*irecHit)->geographicalId();
      uint32_t subDet = detId.subdetId();

      // enum Detector { Tracker=1, Muon=2, Ecal=3, Hcal=4, Calo=5 };
      if( detId.det() != 1 ){
	cout << "rec hit ID = " << detId.det() << " not in tracker!?!?\n";
	continue;
      }

      recHitVector.push_back( (*irecHit)->clone() );

      int xmin = 0;
      int xmax = 0;
      int ymin = 0;
      int ymax = 0;

      double cogp = 0;

      //int icol = 0;
      //int irow = 0;
      int ncol = 0;
      int nrow = 0;
      double clch = 0;

      bool halfmod = 0;

      double Q_f_X = 0.0;//first
      double Q_l_X = 0.0;//last
      double Q_m_X = 0.0;//middle
      double etaX = 0;

      double Q_f_Y = 0.0;//first
      double Q_l_Y = 0.0;//last
      double Q_m_Y = 0.0;//middle

      if( (*irecHit)->isValid() ) {

	// enum SubDetector{ PixelBarrel=1, PixelEndcap=2 };
	// enum SubDetector{ TIB=3, TID=4, TOB=5, TEC=6 };

	//if( idbg ) cout << "  hit in " << subDet << endl;

	h060->Fill( subDet );

	// cast to SiPixelRecHit:
	// TrackingRecHit -> RecHit2DLocalPos -> BaseSiTrackerRecHit2DLocalPos -> SiPixelRecHit

	// Examine the detector
	if( subDet == 1 ){ // PXB

	  int ilay = PXBDetId(detId).layer();
	  int ilad = PXBDetId(detId).ladder();
	  imod = PXBDetId(detId).module();
	 // std::cout << " imod 1 "<< imod<< std::endl;

	  if( ilay == 1 ){
	    if(      ilad ==  5 ) halfmod = 1;
	    else if( ilad ==  6 ) halfmod = 1;
	    else if( ilad == 15 ) halfmod = 1;
	    else if( ilad == 16 ) halfmod = 1;
	  }

	  if( ilay == 2 ){
	    if(      ilad ==  8 ) halfmod = 1;
	    else if( ilad ==  9 ) halfmod = 1;
	    else if( ilad == 24 ) halfmod = 1;
	    else if( ilad == 25 ) halfmod = 1;
	  }

	  if( ilay == 3 ){
	    if( ilad == 11 ) halfmod = 1;
	    if( ilad == 12 ) halfmod = 1;
	    if( ilad == 33 ) halfmod = 1;
	    if( ilad == 34 ) halfmod = 1;
	  }

	  if( idbg ) {
	    cout << "  layer  " << PXBDetId(detId).layer();
	    cout << ", ladder " << PXBDetId(detId).ladder();
	    cout << ", module " << PXBDetId(detId).module();
	    cout << endl;
	  }

	  const SiPixelRecHit *pixhit = dynamic_cast<const SiPixelRecHit*>( &*(*irecHit) );

	  if( pixhit->hasFilledProb() ){
	    float clusProb = pixhit->clusterProbability(0);
	    if( idbg ) cout << "  cluster prob " << clusProb << endl;
	  }

	  // pixel cluster:
	  // TrackingRecHit -> RecHit2DLocalPos -> BaseSiTrackerRecHit2DLocalPos -> SiPixelRecHit -> SiPixelCluster
	  edm::Ref<edmNew::DetSetVector<SiPixelCluster>, SiPixelCluster> const & clust = pixhit->cluster();

	  if( clust.isNonnull() ) {

	    if( idbg ) {
	      cout << setprecision(0);
	      cout << "  charge " << clust->charge();
	      cout << setprecision(4);
	      cout << ", cols " << clust->minPixelCol() << " - " << clust->maxPixelCol(); //0..416 = 8*52
	      cout << " = " << clust->sizeY();
	      cout << ", rows " << clust->minPixelRow() << " - " << clust->maxPixelRow();//0..159 left and right
	      cout << " = " << clust->sizeX();
	      cout << endl;
	    }

	    // Fetch the pixels vector from the cluster:
	    const vector<SiPixelCluster::Pixel> & pixelsVec = clust->pixels();

	    // Obtain boundaries in index units:
	    xmin = clust->minPixelRow();
	    xmax = clust->maxPixelRow();
	    ymin = clust->minPixelCol();
	    ymax = clust->maxPixelCol();

	    // cluster matrix:

	    int QQ[9][99];
	    for( int ix = 0; ix < 9; ++ix ){
	      for( int jz = 0; jz < 99; ++jz ){
		QQ[ix][jz] = 0;
	      }
	    }
	    double xsum = 0;
	    double qsum = 0;

	    // loop over the pixels:
	    int isize = pixelsVec.size();
	    for( int i = 0;  i < isize; ++i ) {

	      int ix = pixelsVec[i].x - xmin;
	      int jz = pixelsVec[i].y - ymin;
	      if( ix > 8 ) ix = 8;
	      if( jz > 98 ) jz = 98;
	      QQ[ix][jz] = pixelsVec[i].adc;

	      double pix_adc = pixelsVec[i].adc;
	      qsum += pix_adc;
	      xsum += pix_adc * pixelsVec[i].x;

	      // X projection:
	      if( pixelsVec[i].x == xmin )
		Q_f_X += pix_adc;
	      else{
		if( pixelsVec[i].x == xmax ) 
		  Q_l_X += pix_adc;
		else
		  Q_m_X += pix_adc;
	      }

	      if( ! halfmod ) {
		h049->Fill( pixelsVec[i].y, pixelsVec[i].x );
		h067->Fill( pixelsVec[i].x );//0..159
		h068->Fill( pixelsVec[i].x, pix_adc*1E-3 );//weighted
	      }

	      // Y projection:
	      if( pixelsVec[i].y == ymin ) 
		Q_f_Y += pix_adc;
	      else{
		if( pixelsVec[i].y == ymax ) 
		  Q_l_Y += pix_adc;
		else 
		  Q_m_Y += pix_adc;
	      }

	    }//loop over pixels

	    etaX = ( Q_f_X - Q_l_X ) / ( Q_f_X + Q_l_X + Q_m_X );

	    cogp = xsum / qsum;

	    clch = clust->charge();//electrons
	    /*icol = clust->minPixelCol();
	    irow = clust->minPixelRow();*/
	    ncol = clust->sizeY();
	    nrow = clust->sizeX();

	    if( ncol > 5 && idbg ){
	      cout << setprecision(1);
	      cout.setf(ios::showpoint);
	      cout.setf(ios::uppercase);
	      //cout.setf(ios::scientific);
	      cout.setf(ios::fixed);
	      cout << "  dip " << setw(5) << dip*wt;
	      cout << setprecision(4);
	      cout << ", layer  " << PXBDetId(detId).layer();
	      cout << ", ladder " << PXBDetId(detId).ladder();
	      cout << ", module " << PXBDetId(detId).module();
	      cout << ", x " << xmin << " - " << xmax;
	      cout << ", z " << ymin << " - " << ymax;
	      cout << endl;
	      for( int ix = 0; ix < min( 9, nrow ); ++ix ){
		cout << "    row " << setw(3) << ix + xmin;
		for( int jz = 0; jz < min( 99, ncol ); ++jz ){
		  cout << setw(6) << QQ[ix][jz] / 100;
		}
		cout << endl;
	      }
	    }//long ncol
	    //
	    //dip -7.E+01, layer  1, ladder 18, module 2, x 148 - 149, z 141 - 148
	    //row 148     0     0     0     0     0     0    75    78
	    //row 149   191   164   111   166    98    96   150     0
	    //
	    //dip 8.E+01, layer  1, ladder 9, module 8, x 21 - 22, z 368 - 375
	    //row  21     0     0     0     0     0     0     0    84
	    //row  22   259   171   113   115   144   106   161     0
	    //
	    //dip -8.E+01, layer  1, ladder 8, module 1, x 8 - 10, z 279 - 290
	    //row   8     0     0     0     0     0     0     0     0     0     0    70    99
	    //row   9     0     0    91   129   131   112    97    97   146   134   107     0
	    //row  10    55   107    56     0     0     0     0     0     0     0     0     0
	    //
	    //dip 8.E+01, layer  1, ladder 2, module 7, x 139 - 140, z 135 - 144
	    //row 139    94   107   173    52     0     0     0     0     0     0
	    //row 140     0     0     0    70   121   128   116   132    99    91
	    //
	    //dip -7.E+01, layer  2, ladder 10, module 2, x 139 - 140, z 408 - 415
	    //row 139    50   397   107   100    71     0     0     0
	    //row 140     0     0     0     0    48   130   246   239
 
	    h070->Fill( clch/1E3 );
	    h071->Fill( clch/1E3*cos(dip) );
	    h072->Fill( ncol );
	    h073->Fill( nrow );
	    h074->Fill( dip*wt, ncol );
	    h075->Fill( dip*wt, nrow );
	    if( nrow == 2 ) h076->Fill( etaX );

	  }//clust nonNull

	}//PXB

      }//valid

      // build transient hit:
#ifdef NEW_TRACKINGRECHITS
      // for pre7
      auto tmprh = 
	(*irecHit)->cloneForFit(*builder->geometry()->idToDet((**irecHit).geographicalId()));
      auto transRecHit = 
	hitCloner.makeShared(tmprh, initialTSOS);
#else 

      TransientTrackingRecHit::RecHitPointer transRecHit = hitBuilder->build( &*(*irecHit), initialTSOS);

#endif

      myTTRHvec.push_back( transRecHit );
      coTTRHvec.push_back( transRecHit );

      if( ! (*irecHit)->isValid() ) continue;

      double xloc = transRecHit->localPosition().x();// 1st meas coord
      double yloc = transRecHit->localPosition().y();// 2nd meas coord or zero
      //double zloc = transRecHit->localPosition().z();// up, always zero, unused

      double vxloc = transRecHit->localPositionError().xx();//covariance
      double vyloc = transRecHit->localPositionError().yy();//covariance

      double gX = transRecHit->globalPosition().x();
      double gY = transRecHit->globalPosition().y();
      double gZ = transRecHit->globalPosition().z();

      if( transRecHit->canImproveWithTrack() ) {//use z from track to apply alignment

	//if( idbg ) cout << "  try to improve\n";

	TrajectoryStateOnSurface propTSOS = 
	  thePropagator->propagate( initialTSOS, transRecHit->det()->surface() );

	if( propTSOS.isValid() ){

	  //if( idbg ) cout << "  have propTSOS\n";

#ifdef NEW_TRACKINGRECHITS

	  auto preciseHit = hitCloner.makeShared(tmprh,propTSOS); //pre7

#else

	  TransientTrackingRecHit::RecHitPointer preciseHit = transRecHit->clone(propTSOS);

#endif
	  //if( idbg ) cout << "  have preciseHit\n";

	  xloc = preciseHit->localPosition().x();// 1st meas coord
	  yloc = preciseHit->localPosition().y();// 2nd meas coord or zero
	  // zloc = preciseHit->localPosition().z();// up, always zero

	  vxloc = preciseHit->localPositionError().xx();//covariance
	  //vyloc = preciseHit->localPositionError().yy();//covariance

	  if( idbg && subDet==1 ) {
	    cout << "  improved hit in lay " << PXBDetId(detId).layer();
	    cout << setprecision(4);
	    cout << ", xloc from " << transRecHit->localPosition().x();
	    cout << " to " << preciseHit->localPosition().x();
	    cout << ", yloc from " << transRecHit->localPosition().y();
	    cout << " to " << preciseHit->localPosition().y();
	    cout << endl;
	    cout<<sqrt(transRecHit->localPositionError().xx())*1E4<<" ";
	    cout<<sqrt(transRecHit->localPositionError().yy())*1E4<<" ";
	    cout<<sqrt(preciseHit->localPositionError().xx())*1E4<<" ";
	    cout<<sqrt(preciseHit->localPositionError().yy())*1E4<<" ";
	    cout << endl;
	  }

	  gX = preciseHit->globalPosition().x();
	  gY = preciseHit->globalPosition().y();
	  gZ = preciseHit->globalPosition().z();

	}//valid propTSOS
	else{
	  if( idbg ) cout << "  propTSOS not valid\n";
	}
      }//canImprove

      double gF = atan2( gY, gX );
      double gR = sqrt( gX*gX + gY*gY );

      h061->Fill( xloc );
      h062->Fill( yloc );
      h063->Fill( gR );
      h064->Fill( gF*wt );
      h065->Fill( gZ );

      if( subDet == PixelSubdetector::PixelBarrel ||
	  subDet == StripSubdetector::TIB ||
	  subDet == StripSubdetector::TOB ) { // barrel

	h066->Fill( gX, gY );
      }

      if( gR < rmin ) {
	rmin = gR;
	innerDetId = detId.rawId();
      }

      double phiN = transRecHit->det()->surface().normalVector().barePhi();//normal vector

      double xmid = transRecHit->det()->position().x();
      double ymid = transRecHit->det()->position().y();

      // PXB:

      if( subDet == PixelSubdetector::PixelBarrel ) {

	double xpix = fmod( xloc+0.82, 0.01 );// xpix = 0..0.01

	h080->Fill( xpix*1E4 );
	if( nrow == 2 )
	  h081->Fill( xpix*1E4 );
	else
	  h082->Fill( xpix*1E4 );

	if( nrow == 2 ) h083->Fill( xpix*1E4, etaX );

	double df = phiN - gF;//normal vector vs position vector: inwards or outwards

	// take care of cut at +180/-180:

	if( df > pi ) df -= twopi;
	else if( df < -pi ) df += twopi;

	// outward/inward have different Lorentz drift:

	if( nrow == 2 ) {
	  if( abs(df) < pihalf ) // outward
	    h084->Fill( xpix*1E4, etaX );
	  else // inward, identical
	    h085->Fill( xpix*1E4, etaX ); // identical
	}

	if( nrow == 1 ) h086->Fill( xloc );
	if( nrow == 2 ) h087->Fill( xloc );

	int ilay = PXBDetId(detId).layer();
	int ilad = PXBDetId(detId).ladder();
	int imod = PXBDetId(detId).module();
	// std::cout << " imod 2 "<< imod<< std::endl;

	if( idbg ) {
	  cout << "  xloc " << xloc;
	  cout << ", cogp " << cogp;
	  double cogx = (cogp + 0.5 - 80) * 0.01 - 0.0054;
	  if( cogp < 79 ) cogx -= 0.01; // big pix
	  if( cogp > 80 ) cogx += 0.01; // big pix
	  cout << ", cogx " << cogx;
	  cout << ", dx = " << cogx - xloc;
	  cout << endl;
	}

	h100->Fill( ilay ); // 1,2,3

	if( ilay == 1 ) {

	  n1++;
	  xPXB1 = gX;
	  yPXB1 = gY;
	  zPXB1 = gZ;
	  uPXB1 = xloc;
	  //vPXB1 = yloc;
	  //ePXB1 = sqrt( vxloc );
	  //fPXB1 = sqrt( vyloc );
	  phiN1 = phiN;
	  //clch1 = clch;
	  //ncol1 = ncol;
	  nrow1 = nrow;
	  //etaX1 = etaX;
	  //cogp1 = cogp;

	  h101->Fill( ilad );// 1..20
	  h102->Fill( imod );// 1..8

	  h103->Fill( gR ); // <R1> = 4.36 cm
	  h104->Fill( gF*wt );
	  h105->Fill( gZ );

	  h106->Fill( gF*wt, gZ ); // phi-z of hit

	  if(      ilad ==  5 ) halfmod = 1;
	  else if( ilad ==  6 ) halfmod = 1;
	  else if( ilad == 15 ) halfmod = 1;
	  else if( ilad == 16 ) halfmod = 1;

	  if( !halfmod ) h107->Fill( xloc, yloc ); // hit within one module

	  // track impact parameter to beam:

	  if( abs(sbca) < 5 &&
	      ! halfmod &&
	      hp.hasValidHitInFirstPixelBarrel() &&
	      hp.trackerLayersWithMeasurement() > 7 ) {

	    if( pt > 8 ) h121->Fill( xloc, sqrtpihalf*abs(bcap)*1E4 );

	    // pt bins:

	    if( pt < 1.2 ) {
	      h122->Fill( xloc, sqrtpihalf*abs(bcap)*1E4 );
	      h123->Fill( xloc, edca*1E4 );
	      h124->Fill( xloc, sqrtpihalf*abs(sbca) );
	    }

	    if( pt > 1.2 && pt < 1.6 ) {
	      h125->Fill( xloc, sqrtpihalf*abs(bcap)*1E4 );
	      h126->Fill( xloc, edca*1E4 );
	      h127->Fill( xloc, sqrtpihalf*abs(sbca) );
	    }

	    if( pt > 1.6 && pt < 2.2 ) {
	      h128->Fill( xloc, sqrtpihalf*abs(bcap)*1E4 );
	      h129->Fill( xloc, edca*1E4 );
	      h130->Fill( xloc, sqrtpihalf*abs(sbca) );
	    }

	    if( pt > 2.2 && pt < 4.0 ) {
	      h131->Fill( xloc, sqrtpihalf*abs(bcap)*1E4 );
	      h132->Fill( xloc, edca*1E4 );
	      h133->Fill( xloc, sqrtpihalf*abs(sbca) );
	    }

	    if( pt > 4.0 && pt < 9.9 ) {
	      h134->Fill( xloc, sqrtpihalf*abs(bcap)*1E4 );
	      h135->Fill( xloc, edca*1E4 );
	      h136->Fill( xloc, sqrtpihalf*abs(sbca) );
	    }

	    if( pt > 9.9 ) {
	      h137->Fill( xloc, sqrtpihalf*abs(bcap)*1E4 );
	      h138->Fill( xloc, edca*1E4 );
	      h139->Fill( xloc, sqrtpihalf*abs(sbca) );
	    }

	  }//hits

	  // my crossings:

	  for( int icrss = 0; icrss < ncrss; ++icrss ){

	    double fcrss = atan2( ycrss[icrss], xcrss[icrss] );
	    double df = gF - fcrss;
	    if( df > pi ) df -= twopi;
	    else if( df < -pi ) df += twopi;
	    double du = gR*df;
	    double dz = gZ - zcrss[icrss];

	    if( abs(du) < 0.01 && abs(dz) < 0.05 ) h111->Fill( gX, gY );
	    h112->Fill( du*1E4 );
	    h113->Fill( dz*1E4 );

	    if( abs(dz) < 0.02 ) h114->Fill( du*1E4 );
	    if( abs(du) < 0.01 ) h115->Fill( dz*1E4 );

	  }//crss

	}//PXB1

	if( ilay == 2 ){

	  n2++;
	  xPXB2 = gX; // precise hit in CMS global coordinates
	  yPXB2 = gY;
	  zPXB2 = gZ;
	  uPXB2 = xloc; // precise hit in local coordinates (w.r.t. sensor center)
	  vPXB2 = yloc;
	  phiN2 = phiN;
	  ePXB2 = sqrt( vxloc );
	  fPXB2 = sqrt( vyloc );
	  clch2 = clch; // cluster charge [e]
	  //ncol2 = ncol;
	  nrow2 = nrow;
	  etaX2 = etaX;
	  cogp2 = cogp;
	  xmid2 = xmid; // middle of sensor in global CMS coordinates
	  ymid2 = ymid;
	  ilad2 = ilad;
	  xmin2 = xmin;
	  xmax2 = xmax;
	  zmin2 = ymin;
	  zmax2 = ymax;

	  det2 = transRecHit->det();

	  h201->Fill( ilad );// 1..32
	  h202->Fill( imod );//1..8

	  h203->Fill( gR ); // <R2> = 7.28 cm
	  h204->Fill( gF*wt );
	  h205->Fill( gZ );

	  h206->Fill( gF*wt, gZ ); // phi-z of hit

	  if(      ilad ==  8 ) halfmod = 1;
	  else if( ilad ==  9 ) halfmod = 1;
	  else if( ilad == 24 ) halfmod = 1;
	  else if( ilad == 25 ) halfmod = 1;

	  if( !halfmod ){
	    h207->Fill( xloc, yloc ); // hit within one module
	  }

	}//PXB2

	if( ilay == 3 ){

	  n3++;
	  xPXB3 = gX;
	  yPXB3 = gY;
	  zPXB3 = gZ;
	  uPXB3 = xloc;
	  //vPXB3 = yloc;
	  //ePXB3 = sqrt( vxloc );
	  //fPXB3 = sqrt( vyloc );
	  phiN3 = phiN;
	  //clch3 = clch;
	  //ncol3 = ncol;
	  nrow3 = nrow;
	  //etaX3 = etaX;
	  //cogp3 = cogp;

	  h301->Fill( ilad );//1..44
	  h302->Fill( imod );//1..8

	  h303->Fill( gR ); // <R3> = 10.18 cm
	  h304->Fill( gF*wt );
	  h305->Fill( gZ );

	  h306->Fill( gF*wt, gZ ); // phi-z of hit

	  if( ilad == 11 ) halfmod = 1;
	  if( ilad == 12 ) halfmod = 1;
	  if( ilad == 33 ) halfmod = 1;
	  if( ilad == 34 ) halfmod = 1;

	  if( !halfmod ){
	    h307->Fill( xloc, yloc ); // hit within one module
	  }

	}//PXB3

      }//PXB

    }//loop rechits

    //if( pt < 0.75 ) continue;// curls up

    //------------------------------------------------------------------------
    // refit the track:

    //edm::RefToBase<TrajectorySeed> seed = iTrack->seedRef(); // not present in RECO

    //if( idbg ) cout << "  prepare refit\n";

    PTrajectoryStateOnDet PTraj = trajectoryStateTransform::persistentState( initialTSOS, innerDetId );
    const TrajectorySeed seed( PTraj, recHitVector, alongMomentum );

    //if( idbg ) cout << "  have seed\n";

    std::vector<Trajectory> refitTrajectoryCollection = theFitter->fit( seed, coTTRHvec, initialTSOS );

    if( refitTrajectoryCollection.size() > 0 ) { // should be either 0 or 1            

      const Trajectory& refitTrajectory = refitTrajectoryCollection.front();

      // Trajectory.measurements:

      Trajectory::DataContainer refitTMs = refitTrajectory.measurements();

      if( idbg ) {
	cout << "  refitTrajectory has " << refitTMs.size() <<" hits in subdet";
      }

      // hits in subDet:

      if( idbg ) {

	for( Trajectory::DataContainer::iterator iTM = refitTMs.begin();
	     iTM != refitTMs.end(); iTM++ ) {

	  TransientTrackingRecHit::ConstRecHitPointer iTTRH = iTM->recHit();
	  if( iTTRH->hit()->isValid() ){
	    cout << "  " << iTTRH->geographicalId().subdetId();
	  }
	}
	cout << endl;

	cout << "         pt " << refitTrajectory.geometricalInnermostState().globalMomentum().perp();
	cout << ", eta " << refitTrajectory.geometricalInnermostState().globalMomentum().eta();
	cout << ", phi " << refitTrajectory.geometricalInnermostState().globalMomentum().barePhi()*wt;
	cout << ", at R " << refitTrajectory.geometricalInnermostState().globalPosition().perp();
	cout << ", z " << refitTrajectory.geometricalInnermostState().globalPosition().z();
	cout << ", phi " << refitTrajectory.geometricalInnermostState().globalPosition().barePhi()*wt;
	cout << endl;

      }//dbg

      // trajectory residuals:

      for( Trajectory::DataContainer::iterator iTM = refitTMs.begin();
	   iTM != refitTMs.end(); iTM++ ) {

	if( ! iTM->recHit()->isValid() ) continue;

	DetId detId = iTM->recHit()->geographicalId();

	uint32_t subDet = detId.subdetId();

	// enum SubDetector{ PixelBarrel=1, PixelEndcap=2 };
	// enum SubDetector{ TIB=3, TID=4, TOB=5, TEC=6 };

	double xHit = iTM->recHit()->localPosition().x(); // primary measurement direction
	double yHit = iTM->recHit()->localPosition().y(); // always 0 in strips
        /*
	int ilay = 0;
	if( detId.subdetId() == 1 ){
	  ilay = PXBDetId( detId ).layer();
	}
	
	if( subDet == 1 && idbg ){//1=PXB
	  cout << "  PXB layer " << ilay << endl;
	}
	*/

	double dx = xHit - iTM->predictedState().localPosition().x();
	double dy = yHit - iTM->predictedState().localPosition().y();
	double vxh = iTM->recHit()->localPositionError().xx();//covariance
	double vxt = iTM->predictedState().localError().positionError().xx();//

	//if( subDet == 1 && idbg ){//1=PXB
	if( subDet == 4 && idbg ){//4=TID
	  cout << "  predictdStateResid = " << dx*1E4 << " um";
	  cout << ", eh = " << sqrt(vxh)*1E4 << " um";
	  cout << ", et = " << sqrt(vxt)*1E4 << " um";
	  cout << endl;
	}

	TrajectoryStateOnSurface combinedPredictedState =
	  TrajectoryStateCombiner().combine( iTM->forwardPredictedState(), iTM->backwardPredictedState() );

	if( ! combinedPredictedState.isValid() ) continue;//skip hit

	if( jdbg ) cout << "  have combinedPredictedState\n";

	double R = combinedPredictedState.globalPosition().perp();
	double F = combinedPredictedState.globalPosition().barePhi();
	double Z = combinedPredictedState.globalPosition().z();

	double xptch;
	double yptch;	

	if( subDet <  3 ){//1,2=pixel
	  PixelTopology & pixelTopol = (PixelTopology&) iTM->recHit()->detUnit()->topology();
	  xptch = pixelTopol.pitch().first;
	  yptch = pixelTopol.pitch().second;
	}
	else {//strip
	  StripTopology & stripTopol = (StripTopology&) iTM->recHit()->detUnit()->topology();
	  xptch = stripTopol.localPitch( combinedPredictedState.localPosition() );
	  yptch = stripTopol.localStripLength( combinedPredictedState.localPosition() );
	}

	if( subDet == 4 && idbg ){//4=TID
	  cout << "  TID side " << TIDDetId(detId).side();
	  cout << ", disk " << TIDDetId(detId).diskNumber();
	  cout << ", ring " << TIDDetId(detId).ringNumber();
	  cout << ", stereo " << TIDDetId(detId).isStereo();
	  cout << endl;
	  cout << setprecision(4);
	  cout << "  xHit " << xHit;
	  cout << ", yHit " << yHit;
	  cout << setprecision(1);
	  cout << ", pitch " << xptch*1E4 << " um";
	  cout << setprecision(4);
	  cout << endl;
	}

	dx = xHit - combinedPredictedState.localPosition().x(); //x = primary measurement
	dy = yHit - combinedPredictedState.localPosition().y(); //
	vxh = iTM->recHit()->localPositionError().xx();//covariance
	vxt = combinedPredictedState.localError().positionError().xx();//

	// angles of incidence:
	// local z = upwards = normal vector
	// local x = primary measurement direction
	// local y = secondary measurement direction

	double alf_inc = atan2( combinedPredictedState.localDirection().x(), combinedPredictedState.localDirection().z() );
	double bet_inc = atan2( combinedPredictedState.localDirection().y(), combinedPredictedState.localDirection().z() );

	double phiinc = alf_inc;
	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	if( bet_inc > pihalf ) bet_inc -= pi;
	else if( bet_inc < -pihalf ) bet_inc += pi;

	//if( subDet == 1 && idbg ){//1=PXB
	if( subDet == 4 && idbg ){//4=TID

	  cout << setprecision(1);
	  cout << "  combinedStateResid = " << dx*1E4 << " um";
	  cout << ", eh = " << sqrt(vxh)*1E4 << " um";
	  cout << ", et = " << sqrt(vxt)*1E4 << " um";
	  cout << ", dy = " << dy*1E4 << " um";
	  cout << setprecision(4);
	  cout << ", track at x " << combinedPredictedState.localPosition().x();
	  cout << ", y " << combinedPredictedState.localPosition().y();
	  cout << endl;
	}

	// use Topology. no effect in PXB, essential in TID, TEC

	const Topology* theTopology = &(iTM->recHit()->detUnit()->topology() );

	// MeasurementPoint [pitch] (like channel number)

        // TODO: Use the measurementPosition(point, trackdir) version of this function in order to take bows into account!
	MeasurementPoint hitMeasurement = theTopology->measurementPosition( iTM->recHit()->localPosition() );

	// TID and TEC have trapezoidal detectors:
	// translation from channel number into local x depends on local y
	// track prediction has local x,y => can convert into proper channel number MeasurementPoint:

        // TODO: Use the measurementPosition(point, trackdir) version of this function in order to take bows into account!
	MeasurementPoint combinedPredictedMeasurement = theTopology->measurementPosition( combinedPredictedState.localPosition() );

	dx = hitMeasurement.x() - combinedPredictedMeasurement.x(); //in units of pitch
	dy = hitMeasurement.y() - combinedPredictedMeasurement.y(); //in units of pitch
	dx = dx * xptch;//convert back into [cm] using local pitch
	dy = dy * yptch;//[cm]

	if( jdbg ) cout << "  have combinedPredictedMeasurement\n";

	//if( subDet == 1 && idbg ){//1=PXB
	if( subDet == 4 && idbg ){//4=TID

	  cout << setprecision(1);
	  cout << "  topologyStateResid = " << dx*1E4 << " um";
	  cout << setprecision(4);
	  cout << ", hit at x " << hitMeasurement.x();
	  cout << ", y " << hitMeasurement.y();
	  cout << ", track at x " << combinedPredictedMeasurement.x();
	  cout << ", y " << combinedPredictedMeasurement.y();
	  cout << endl;
	}

	// plot resids:

	if( subDet == PixelSubdetector::PixelBarrel ) {

	  if( PXBDetId( detId ).layer() == 1 ) {

	    // resolution: biased reasiduals in PXB1

	    if( pt >  4 ) k100->Fill( dx*1E4 );
	    if( pt > 12 ) k101->Fill( dx*1E4 );

	    k102->Fill( logpt, abs(dx)*1E4 ); // increasing: seeding bias

	    if( pt > 4 ) {

	      k103->Fill( alf_inc*wt ); //at +-90 deg
	      k104->Fill( bet_inc*wt ); //-80 to + 80 deg
	      if( abs(alf_inc) < pihalf )
		k105->Fill( phiinc*wt, abs(dx)*1E4 );
	      else
		k106->Fill( phiinc*wt, abs(dx)*1E4 );
	      k107->Fill( bet_inc*wt, abs(dx)*1E4 );
	      k108->Fill( bet_inc*wt, abs(dy)*1E4 );//local y = z in PXB

	      k109->Fill( combinedPredictedState.localPosition().x() );//
	      k110->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//

	      k111->Fill( combinedPredictedState.localPosition().y() );//
	      k112->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k113->Fill( combinedPredictedState.localPosition().y(), abs(dy)*1E4 );//

	      // alignment check:

	      k116->Fill( F*wt, dx*1E4 );//
	      k117->Fill( Z, dx*1E4 );//
	      k118->Fill( Z, dy*1E4 );//

	    }//pt

	    if( pt > 0.8 && pt < 1.2 ) { // low pt
	      k114->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k115->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	    }

	    // phiinc:

	    k170->Fill( phiinc*wt );
	    k171->Fill( phiinc*wt, pt );
	    k172->Fill( phiinc*wt, iTrack->charge() );
	    k173->Fill( phiinc*wt, nrow1 );
	    k174->Fill( tan(phiinc), nrow1 );
	    if( abs(alf_inc) < pihalf )
	      k175->Fill( tan(phiinc), nrow1 );
	    else
	      k176->Fill( tan(phiinc), nrow1 );

	  }//layer

	  if( PXBDetId( detId ).layer() == 2 ) {

	    if( pt >  4 ) k120->Fill( dx*1E4 );
	    if( pt > 12 ) k121->Fill( dx*1E4 );

	    k122->Fill( logpt, abs(dx)*1E4 );//decreasing, as expected

	    if( pt > 4 ) {

	      k123->Fill( alf_inc*wt ); //at +-90 deg
	      k124->Fill( bet_inc*wt ); //
	      if( abs(alf_inc) < pihalf )
		k125->Fill( phiinc*wt, abs(dx)*1E4 );
	      else
		k126->Fill( phiinc*wt, abs(dx)*1E4 );
	      k127->Fill( bet_inc*wt, abs(dx)*1E4 );
	      k128->Fill( bet_inc*wt, abs(dy)*1E4 );//local y = z in PXB

	      k129->Fill( combinedPredictedState.localPosition().x() );//
	      k130->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//

	      k131->Fill( combinedPredictedState.localPosition().y() );//
	      k132->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k133->Fill( combinedPredictedState.localPosition().y(), abs(dy)*1E4 );//

	      // alignment check:

	      k136->Fill( F*wt, dx*1E4 );//
	      k137->Fill( Z, dx*1E4 );//
	      k138->Fill( Z, dy*1E4 );//

	      k160->Fill( sqrt(vxh)*1E4 );
	      k161->Fill( sqrt(vxt)*1E4 );

	    }//pt

	    if( pt > 0.8 && pt < 1.2 ) { // low pt
	      k134->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k135->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	    }

	    k180->Fill( phiinc*wt );
	    k181->Fill( phiinc*wt, pt );
	    k182->Fill( phiinc*wt, iTrack->charge() );
	    k183->Fill( phiinc*wt, nrow2 );
	    k184->Fill( tan(phiinc), nrow2 );
	    if( abs(alf_inc) < pihalf )
	      k185->Fill( tan(phiinc), nrow2 );
	    else
	      k186->Fill( tan(phiinc), nrow2 );

	  }//layer

	  if( PXBDetId( detId ).layer() == 3 ) {

	    if( pt >  4 ) k140->Fill( dx*1E4 );
	    if( pt > 12 ) k141->Fill( dx*1E4 );

	    k142->Fill( logpt, abs(dx)*1E4 );

	    if( pt > 4 ) {

	      k143->Fill( alf_inc*wt ); //at +-90 deg
	      k144->Fill( bet_inc*wt ); //
	      if( abs(alf_inc) < pihalf )
		k145->Fill( phiinc*wt, abs(dx)*1E4 );
	      else
		k146->Fill( phiinc*wt, abs(dx)*1E4 );
	      k147->Fill( bet_inc*wt, abs(dx)*1E4 );
	      k148->Fill( bet_inc*wt, abs(dy)*1E4 );//local y = z in PXB

	      k149->Fill( combinedPredictedState.localPosition().x() );//
	      k150->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//

	      k151->Fill( combinedPredictedState.localPosition().y() );//
	      k152->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k153->Fill( combinedPredictedState.localPosition().y(), abs(dy)*1E4 );//

	      // alignment check:

	      k156->Fill( F*wt, dx*1E4 );//
	      k157->Fill( Z, dx*1E4 );//
	      k158->Fill( Z, dy*1E4 );//

	    }//pt

	    if( pt > 0.8 && pt < 1.2 ) { // low pt
	      k154->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k155->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	    }

	    k190->Fill( phiinc*wt );
	    k191->Fill( phiinc*wt, pt );
	    k192->Fill( phiinc*wt, iTrack->charge() );
	    k193->Fill( phiinc*wt, nrow3 );
	    k194->Fill( tan(phiinc), nrow3 );
	    if( abs(alf_inc) < pihalf )
	      k195->Fill( tan(phiinc), nrow3 );
	    else
	      k196->Fill( tan(phiinc), nrow3 );

	  }//layer

	}//PXB

	if( subDet == StripSubdetector::TIB ) {

	  k301->Fill( R );
	  k302->Fill( F*wt );
	  k303->Fill( Z );
	  if( pt > 0.8 && pt < 1.2 ) { // low pt
	    k304->Fill( F*wt, abs(dx)*1E4 );
	    k305->Fill( Z, abs(dx)*1E4 );
	    k306->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	    k307->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	  }//pt
	  if( pt > 4 ) k308->Fill( alf_inc*wt );

	  if( TIBDetId( detId ).layer() == 1 && TIBDetId(detId).isRPhi() ) {
	    k310->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k311->Fill( dx*1E4 );
	      k312->Fill( F*wt, abs(dx)*1E4 );
	      k313->Fill( Z, abs(dx)*1E4 );
	      k314->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k315->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k316->Fill( phiinc*wt, abs(dx)*1E4 );
	      k317->Fill( F*wt, dx*1E4 );
	      k318->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k319->Fill( dx*1E4 );
	  }//layer

	  if( TIBDetId( detId ).layer() == 1 && TIBDetId(detId).isStereo() ) {
	    k320->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k321->Fill( dx*1E4 );
	      k322->Fill( F*wt, abs(dx)*1E4 );
	      k323->Fill( Z, abs(dx)*1E4 );
	      k324->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k325->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k326->Fill( phiinc*wt, abs(dx)*1E4 );
	      k327->Fill( F*wt, dx*1E4 );
	      k328->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k329->Fill( dx*1E4 );
	  }//layer

	  if( TIBDetId( detId ).layer() == 2 && TIBDetId(detId).isRPhi() ) {
	    k330->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k331->Fill( dx*1E4 );
	      k332->Fill( F*wt, abs(dx)*1E4 );
	      k333->Fill( Z, abs(dx)*1E4 );
	      k334->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k335->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k336->Fill( phiinc*wt, abs(dx)*1E4 );
	      k337->Fill( F*wt, dx*1E4 );
	      k338->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k339->Fill( dx*1E4 );
	  }//layer

	  if( TIBDetId( detId ).layer() == 2 && TIBDetId(detId).isStereo() ) {
	    k340->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k341->Fill( dx*1E4 );
	      k342->Fill( F*wt, abs(dx)*1E4 );
	      k343->Fill( Z, abs(dx)*1E4 );
	      k344->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k345->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k346->Fill( phiinc*wt, abs(dx)*1E4 );
	      k347->Fill( F*wt, dx*1E4 );
	      k348->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k349->Fill( dx*1E4 );
	  }//layer

	  if( TIBDetId( detId ).layer() == 3 ) {
	    k350->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k351->Fill( dx*1E4 );
	      k352->Fill( F*wt, abs(dx)*1E4 );
	      k353->Fill( Z, abs(dx)*1E4 );
	      k354->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k355->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k356->Fill( phiinc*wt, abs(dx)*1E4 );
	      k357->Fill( F*wt, dx*1E4 );
	      k358->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k359->Fill( dx*1E4 );
	  }//layer

	  if( TIBDetId( detId ).layer() == 4 ) {
	    k360->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k361->Fill( dx*1E4 );
	      k362->Fill( F*wt, abs(dx)*1E4 );
	      k363->Fill( Z, abs(dx)*1E4 );
	      k364->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k365->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k366->Fill( phiinc*wt, abs(dx)*1E4 );
	      k367->Fill( F*wt, dx*1E4 );
	      k368->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k369->Fill( dx*1E4 );
	  }//layer

	}//TIB

	if( subDet == StripSubdetector::TID ) {

	  //StripTopology & stripTopol = (StripTopology&) iTM->recHit()->detUnit()->topology();

	  k401->Fill( R );
	  k402->Fill( F*wt );
	  k403->Fill( Z );

	  if( pt > 4 ) {

	    k404->Fill( dx*1E4 );
	    k405->Fill( R, abs(dx)*1E4 );
	    k406->Fill( xptch*1E4, abs(dx)*1E4 );
	  }

	  if( TIDDetId(detId).ringNumber() == 1 ) {
	    k410->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TIDDetId(detId).isRPhi() )
		k411->Fill( dx*1E4 );
	      else
		k412->Fill( dx*1E4 );
	      k413->Fill( F*wt, abs(dx)*1E4 );
	      k414->Fill( F*wt, dx*1E4 );
	    }
	  }

	  if( TIDDetId(detId).ringNumber() == 2 ) {
	    k420->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TIDDetId(detId).isRPhi() )
		k421->Fill( dx*1E4 );
	      else
		k422->Fill( dx*1E4 );
	      k423->Fill( F*wt, abs(dx)*1E4 );
	      k424->Fill( F*wt, dx*1E4 );
	    }
	  }

	  if( TIDDetId(detId).ringNumber() == 3 ) {
	    k430->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TIDDetId(detId).isRPhi() )
		k431->Fill( dx*1E4 );
	      else
		k432->Fill( dx*1E4 );
	      k433->Fill( F*wt, abs(dx)*1E4 );
	      k434->Fill( F*wt, dx*1E4 );
	    }
	  }

	}//TID

	if( subDet == StripSubdetector::TOB ) {

	  k501->Fill( R );
	  k502->Fill( F*wt );
	  k503->Fill( Z );
	  if( pt > 0.8 && pt < 1.2 ) { // low pt
	    k504->Fill( F*wt, abs(dx)*1E4 );
	    k505->Fill( Z, abs(dx)*1E4 );
	    k506->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	    k507->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	  }//pt
	  if( pt > 4 ) k508->Fill( alf_inc*wt ); //at 0, +-180

	  if( TOBDetId( detId ).layer() == 1 && TOBDetId(detId).isRPhi() ) {
	    k510->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k511->Fill( dx*1E4 );
	      k512->Fill( F*wt, abs(dx)*1E4 );
	      k513->Fill( Z, abs(dx)*1E4 );
	      k514->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k515->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k516->Fill( phiinc*wt, abs(dx)*1E4 );
	      k517->Fill( F*wt, dx*1E4 );
	      k518->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k519->Fill( dx*1E4 );
	  }//layer

	  if( TOBDetId( detId ).layer() == 1 && TOBDetId(detId).isStereo() ) {
	    k520->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k521->Fill( dx*1E4 );
	      k522->Fill( F*wt, abs(dx)*1E4 );
	      k523->Fill( Z, abs(dx)*1E4 );
	      k524->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k525->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k526->Fill( phiinc*wt, abs(dx)*1E4 );
	      k527->Fill( F*wt, dx*1E4 );
	      k528->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k529->Fill( dx*1E4 );
	  }//layer

	  if( TOBDetId( detId ).layer() == 2 && TOBDetId(detId).isRPhi() ) {
	    k530->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k531->Fill( dx*1E4 );
	      k532->Fill( F*wt, abs(dx)*1E4 );
	      k533->Fill( Z, abs(dx)*1E4 );
	      k534->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k535->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k536->Fill( phiinc*wt, abs(dx)*1E4 );
	      k537->Fill( F*wt, dx*1E4 );
	      k538->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k539->Fill( dx*1E4 );
	  }//layer

	  if( TOBDetId( detId ).layer() == 2 && TOBDetId(detId).isStereo() ) {
	    k540->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k541->Fill( dx*1E4 );
	      k542->Fill( F*wt, abs(dx)*1E4 );
	      k543->Fill( Z, abs(dx)*1E4 );
	      k544->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k545->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k546->Fill( phiinc*wt, abs(dx)*1E4 );
	      k547->Fill( F*wt, dx*1E4 );
	      k548->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k549->Fill( dx*1E4 );
	  }//layer

	  if( TOBDetId( detId ).layer() == 3 ) {
	    k550->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k551->Fill( dx*1E4 );
	      k552->Fill( F*wt, abs(dx)*1E4 );
	      k553->Fill( Z, abs(dx)*1E4 );
	      k554->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k555->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k556->Fill( phiinc*wt, abs(dx)*1E4 );
	      k557->Fill( F*wt, dx*1E4 );
	      k558->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k559->Fill( dx*1E4 );
	  }//layer

	  if( TOBDetId( detId ).layer() == 4 ) {
	    k560->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k561->Fill( dx*1E4 );
	      k562->Fill( F*wt, abs(dx)*1E4 );
	      k563->Fill( Z, abs(dx)*1E4 );
	      k564->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k565->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k566->Fill( phiinc*wt, abs(dx)*1E4 );
	      k567->Fill( F*wt, dx*1E4 );
	      k568->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k569->Fill( dx*1E4 );
	  }//layer

	  if( TOBDetId( detId ).layer() == 5 ) {
	    k570->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k571->Fill( dx*1E4 );
	      k572->Fill( F*wt, abs(dx)*1E4 );
	      k573->Fill( Z, abs(dx)*1E4 );
	      k574->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k575->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k576->Fill( phiinc*wt, abs(dx)*1E4 );
	      k577->Fill( F*wt, dx*1E4 );
	      k578->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k579->Fill( dx*1E4 );
	  }//layer

	  if( TOBDetId( detId ).layer() == 6 ) {
	    k580->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      k581->Fill( dx*1E4 );
	      k582->Fill( F*wt, abs(dx)*1E4 );
	      k583->Fill( Z, abs(dx)*1E4 );
	      k584->Fill( combinedPredictedState.localPosition().x(), abs(dx)*1E4 );//
	      k585->Fill( combinedPredictedState.localPosition().y(), abs(dx)*1E4 );//
	      k586->Fill( phiinc*wt, abs(dx)*1E4 );
	      k587->Fill( F*wt, dx*1E4 );
	      k588->Fill( Z, dx*1E4 );
	    }//pt
	    if( pt > 12 ) k589->Fill( dx*1E4 );
	  }//layer

	}//TOB

	if( subDet == StripSubdetector::TEC ) {

	  k601->Fill( R );
	  k602->Fill( F*wt );
	  k603->Fill( Z );

	  if( pt > 4 ) {

	    k604->Fill( dx*1E4 );
	    k605->Fill( R, abs(dx)*1E4 );
	    k606->Fill( R, xptch*1E4 );
	    if( TECDetId(detId).ringNumber() < 7 )
	      k607->Fill( xptch*1E4, abs(dx)*1E4 );
	  }//pt

	  if( TECDetId(detId).isBackPetal() )
	    k608->Fill( combinedPredictedState.globalPosition().x(), combinedPredictedState.globalPosition().y() );
	  else
	    k609->Fill( combinedPredictedState.globalPosition().x(), combinedPredictedState.globalPosition().y() );

	  if( TECDetId(detId).ringNumber() == 1 ) {
	    k610->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TECDetId(detId).isRPhi() )
		k611->Fill( dx*1E4 );
	      else
		k612->Fill( dx*1E4 );
	      k613->Fill( F*wt, dx*1E4 );
	      k614->Fill( F*wt, abs(dx)*1E4 );
	    }//pt
	    if( pt > 12 ) k615->Fill( dx*1E4 );
	  }//ring

	  if( TECDetId(detId).ringNumber() == 2 ) {
	    k620->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TECDetId(detId).isRPhi() )
		k621->Fill( dx*1E4 );
	      else
		k622->Fill( dx*1E4 );
	      k623->Fill( F*wt, dx*1E4 );
	      k624->Fill( F*wt, abs(dx)*1E4 );
	    }//pt
	    if( pt > 12 ) k625->Fill( dx*1E4 );
	  }//ring

	  if( TECDetId(detId).ringNumber() == 3 ) {
	    k630->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TECDetId(detId).isRPhi() )
		k631->Fill( dx*1E4 );
	      else
		k632->Fill( dx*1E4 );
	      k633->Fill( F*wt, dx*1E4 );
	      k634->Fill( F*wt, abs(dx)*1E4 );
	    }//pt
	    if( pt > 12 ) k635->Fill( dx*1E4 );
	  }//ring

	  if( TECDetId(detId).ringNumber() == 4 ) {
	    k640->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TECDetId(detId).isRPhi() )
		k641->Fill( dx*1E4 );
	      else
		k642->Fill( dx*1E4 );
	      k643->Fill( F*wt, dx*1E4 );
	      k644->Fill( F*wt, abs(dx)*1E4 );
	    }//pt
	    if( pt > 12 ) k645->Fill( dx*1E4 );
	  }//ring

	  if( TECDetId(detId).ringNumber() == 5 ) {
	    k650->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TECDetId(detId).isRPhi() )
		k651->Fill( dx*1E4 );
	      else
		k652->Fill( dx*1E4 );
	      k653->Fill( F*wt, dx*1E4 );
	      k654->Fill( F*wt, abs(dx)*1E4 );
	    }//pt
	    if( pt > 12 ) k655->Fill( dx*1E4 );
	  }//ring

	  if( TECDetId(detId).ringNumber() == 6 ) {
	    k660->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TECDetId(detId).isRPhi() )
		k661->Fill( dx*1E4 );
	      else
		k662->Fill( dx*1E4 );
	      k663->Fill( F*wt, dx*1E4 );
	      k664->Fill( F*wt, abs(dx)*1E4 );
	    }//pt
	    if( pt > 12 ) k665->Fill( dx*1E4 );
	  }//ring

	  if( TECDetId(detId).ringNumber() == 7 ) {
	    k670->Fill( logpt, abs(dx)*1E4 );
	    if( pt > 4 ) {
	      if( TECDetId(detId).isRPhi() )
		k671->Fill( dx*1E4 );
	      else
		k672->Fill( dx*1E4 );
	      k673->Fill( F*wt, dx*1E4 );
	      k674->Fill( F*wt, abs(dx)*1E4 );
	    }//pt
	    if( pt > 12 ) k675->Fill( dx*1E4 );
	  }//ring

	  if( pt > 4 && TECDetId(detId).ringNumber() < 7 ) {

	    if( TECDetId(detId).wheelNumber() == 1 ) k681->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 2 ) k682->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 3 ) k683->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 4 ) k684->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 5 ) k685->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 6 ) k686->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 7 ) k687->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 8 ) k688->Fill( dx*1E4 );
	    if( TECDetId(detId).wheelNumber() == 9 ) k689->Fill( dx*1E4 );
	  }

	}//TEC

	//2012 if( subDet < 3 ) continue;//don't mess with pixel hits. use stripTopol below

	/*
	double gX = iTM->recHit()->globalPosition().x();// gives poor resolution
	double gY = iTM->recHit()->globalPosition().y();
	double gZ = iTM->recHit()->globalPosition().z();// spiky in TIB: sensor mid points
	*/

	// construct local point:
	// strip trackers measure in x (=phi)
	// take y from track (=z in TIB/TOB, = R in TID/TEC)
	// local z is zero (what about bowing = surface deformation?)

	/*
	const Local3DPoint lp3( iTM->recHit()->localPosition().x(), // measurement direction
				iTM->predictedState().localPosition().y(), // track in z (or R)
				iTM->predictedState().localPosition().z() ); // track in 3rd D

	const GeomDet * myGeomDet = pTG->idToDet( detId );
	Surface::GlobalPoint gp = myGeomDet->toGlobal( lp3 );

	double gX = gp.x();
	double gY = gp.y();
	double gZ = gp.z();
	*/

	// use topology: needed for TEC

	double xx = hitMeasurement.x();
	double yy;
	if( subDet < 3 ) // pixel is 2D
	  yy = hitMeasurement.y();
	else // strips are 1D
	  yy = combinedPredictedMeasurement.y();

	MeasurementPoint mp( xx, yy );

	//2012 StripTopology & stripTopol = (StripTopology&) iTM->recHit()->detUnit()->topology();

	Surface::LocalPoint lp = theTopology->localPosition( mp );
	//2012 Surface::LocalPoint lp = stripTopol.localPosition( mp ); // makes no difference in TEC

	//const GeomDet * myGeomDet = pTG->idToDet( detId );
	const GeomDet * myGeomDet = iTM->recHit()->det(); // makes no difference in TEC
	Surface::GlobalPoint gp = myGeomDet->toGlobal( lp );

	double gX = gp.x();
	double gY = gp.y();
	double gZ = gp.z();

 	//double phiN = myGeomDet->surface().normalVector().barePhi();//normal vector
 	double phiN = iTM->recHit()->det()->surface().normalVector().barePhi();//normal vector

	if( idbg && subDet == StripSubdetector::TEC ) {
	  cout << "  TEC hit ";
	  cout << ", side " << TECDetId(detId).side();
	  cout << ", wheel " << TECDetId(detId).wheel();
	  cout << ", ring " << TECDetId(detId).ring();
	  cout << ", petal " << TECDetId(detId).petalNumber();
	  cout << ", order " << TECDetId(detId).order();//1=back or 2=front
	  cout << ", module " << TECDetId(detId).module();
	  cout << ", stereo " << TECDetId(detId).isStereo();
	  cout << ", comp " << myGeomDet->components().size(); //always 0
	  cout << endl;
	}

	//2012: overwrite PXB global coordinates once more, using topology:

	if( subDet == PixelSubdetector::PixelBarrel ) {

	  int ilay = PXBDetId(detId).layer();
  
	  if( ilay == 1 ) {
	    xPXB1 = gX;
	    yPXB1 = gY;
	    zPXB1 = gZ;
	  }

	  else if( ilay == 2 ) {
	    xPXB2 = gX;
	    yPXB2 = gY;
	    zPXB2 = gZ;
	    geomDet2 = (TrackerGeomDet*) myGeomDet;
	  }

	  else if( ilay == 3 ) {
	    xPXB3 = gX;
	    yPXB3 = gY;
	    zPXB3 = gZ;
	  }

	}//PXB

	if( subDet == StripSubdetector::TIB ) {

	  if( TIBDetId(detId).isRPhi() ){

	    if( TIBDetId(detId).layer() == 1 ) {

	      nTIB1++;
	      xTIB1 = gX;
	      yTIB1 = gY;
	      //zTIB1 = gZ;
	      //phiNTIB1 = phiN;
	    }

	    if( TIBDetId(detId).layer() == 2 ) {

	      nTIB2++;
	      xTIB2 = gX;
	      yTIB2 = gY;
	      zTIB2 = gZ;
	      phiNTIB2 = phiN;
	    }

	    if( TIBDetId(detId).layer() == 3 ) {

	      nTIB3++;
	      xTIB3 = gX;
	      yTIB3 = gY;
	      zTIB3 = gZ;
	      phiNTIB3 = phiN;
	    }

	    if( TIBDetId(detId).layer() == 4 ) {

	      nTIB4++;
	      xTIB4 = gX;
	      yTIB4 = gY;
	      //zTIB4 = gZ;
	      //phiNTIB4 = phiN;
	    }

	  }// rphi

	}//TIB

	if( subDet == StripSubdetector::TOB ) {

	  if( TOBDetId(detId).isRPhi() ){

	    if( TOBDetId(detId).layer() == 1 ) {

	      nTOB1++;
	      //xTOB1 = gX;
	      //yTOB1 = gY;
	      //zTOB1 = gZ;
	      //phiNTOB1 = phiN;
	    }

	    if( TOBDetId(detId).layer() == 2 ) {

	      nTOB2++;
	      //xTOB2 = gX;
	      //yTOB2 = gY;
	      //zTOB2 = gZ;
	      //phiNTOB2 = phiN;
	    }

	    if( TOBDetId(detId).layer() == 3 ) {

	      nTOB3++;
	      xTOB3 = gX;
	      yTOB3 = gY;
	      //zTOB3 = gZ;
	      //phiNTOB3 = phiN;
	    }

	    if( TOBDetId(detId).layer() == 4 ) {

	      nTOB4++;
	      xTOB4 = gX;
	      yTOB4 = gY;
	      zTOB4 = gZ;
	      phiNTOB4 = phiN;
	    }

	    if( TOBDetId(detId).layer() == 5 ) {

	      nTOB5++;
	      xTOB5 = gX;
	      yTOB5 = gY;
	      zTOB5 = gZ;
	      phiNTOB5 = phiN;
	    }

	    if( TOBDetId(detId).layer() == 6 ) {

	      nTOB6++;
	      xTOB6 = gX;
	      yTOB6 = gY;
	      //zTOB6 = gZ;
	      //phiNTOB6 = phiN;
	    }

	  }// rphi

	}//TOB

	if( subDet == StripSubdetector::TEC ) {

	  if( TECDetId(detId).isRPhi() ){

	    int is = TECDetId(detId).side();//1..2
	    int iw = TECDetId(detId).wheel();//1..9

	    if( nTEC[is][iw] == 0 ) {//take first hit per wheel
	      nTEC[is][iw]++;
	      xTEC[is][iw] = gX;
	      yTEC[is][iw] = gY;
	      zTEC[is][iw] = gZ;
	      //phiNTEC[is][iw] = phiN;
	      detTEC[is][iw] = detId;
	    }
	  }// rphi

	}//TEC

      }//loop iTM

    }//refitted trajectory

    //if( pt < 0.75 ) continue;// curls up

    //------------------------------------------------------------------------
    // refit once more, leaving out hit in 2nd PXB:

    double kap2 = 0;
    bool refit2valid = 0;

    if( n2 > 0 ){

      Trajectory::RecHitContainer nyTTRHvec; // for fit

      for( vector<TransientTrackingRecHit::RecHitPointer>::iterator iTTRH = myTTRHvec.begin();
	   iTTRH != myTTRHvec.end(); ++iTTRH ) {

	if( idbg == 9 ) {
	  cout << "  hit " << distance( (vector<TransientTrackingRecHit::RecHitPointer>::iterator)myTTRHvec.begin(), iTTRH );
	  if( (*iTTRH)->hit()->isValid() ){
	    cout << ": subdet " << (*iTTRH)->hit()->geographicalId().subdetId();
	    cout << ", weight " << (*iTTRH)->weight(); // default weight is 1
	    cout << endl;
	  }
	  else cout << " not valid\n";
	}

	int iuse = 1;
	if( (*iTTRH)->hit()->isValid() ){
	  if( (*iTTRH)->hit()->geographicalId().subdetId() == 1 ){ //PXB
	    if( PXBDetId( (*iTTRH)->hit()->geographicalId() ).layer() == 2 ) iuse = 0; // skip PXB2: unbiased track
	  }
	}
	if( iuse ) nyTTRHvec.push_back( *iTTRH );
      }//copy

      if( idbg ) {
	cout << "  all hits " << myTTRHvec.size();
	cout << ", without PXB2 " << nyTTRHvec.size();
	cout << endl;
      }

      // re-fit without PXB2:

      std::vector<Trajectory> refitTrajectoryVec2 = theFitter->fit( seed, nyTTRHvec, initialTSOS );

      if( refitTrajectoryVec2.size() > 0 ) { // should be either 0 or 1            

	const Trajectory& refitTrajectory2 = refitTrajectoryVec2.front();

	// Trajectory.measurements:

	Trajectory::DataContainer refitTMvec2 = refitTrajectory2.measurements();

	if( idbg ) {
	  cout << "  refitTrajectory2 has " << refitTMvec2.size() <<" hits in subdet";
	}

	for( Trajectory::DataContainer::iterator iTM = refitTMvec2.begin();
	     iTM != refitTMvec2.end(); iTM++ ) {

	  TransientTrackingRecHit::ConstRecHitPointer iTTRH = iTM->recHit();
	  if( iTTRH->hit()->isValid() ){
	    if( idbg ) cout << "  " << iTTRH->geographicalId().subdetId();
	  }
	}
	if( idbg ) cout << endl;

	if( idbg ) {
	  cout << "  ndof " << refitTrajectory2.ndof();
	  cout << ", found " << refitTrajectory2.foundHits();
	  cout << ", missing " << refitTrajectory2.lostHits();
	  cout << ", chi2 " << refitTrajectory2.chiSquared();
	  cout << ", /ndof " << refitTrajectory2.chiSquared() / refitTrajectory2.ndof();
	  cout << endl;
	}

	if( idbg ) {
	  cout << "         pt " << refitTrajectory2.geometricalInnermostState().globalMomentum().perp();
	  cout << ", eta " << refitTrajectory2.geometricalInnermostState().globalMomentum().eta();
	  cout << ", phi " << refitTrajectory2.geometricalInnermostState().globalMomentum().barePhi()*wt;
	  cout << ", at R " << refitTrajectory2.geometricalInnermostState().globalPosition().perp();
	  cout << ", z " << refitTrajectory2.geometricalInnermostState().globalPosition().z();
	  cout << ", phi " << refitTrajectory2.geometricalInnermostState().globalPosition().barePhi()*wt;
	  cout << endl;
	}
	refit2valid = 1;
	kap2 = refitTrajectory2.geometricalInnermostState().transverseCurvature();

	// now use it!

      }//refit2
      else 
	if( idbg ) cout << "  no refit\n";

    }// n2 > 0

    //------------------------------------------------------------------------
    // 1-2-3 pixel triplet:

    if( n1*n2*n3 > 0 ) {

      { // let's open a scope, so we can redefine the variables further down

	if( jdbg ) cout << "  triplet 1+3 -> 2\n";

	double f2 = atan2( yPXB2, xPXB2 );//position angle in layer 2

	double ax = xPXB3 - xPXB1;
	double ay = yPXB3 - yPXB1;
	double aa = sqrt( ax*ax + ay*ay ); // from 1 to 3

	double xmid = 0.5 * ( xPXB1 + xPXB3 );
	double ymid = 0.5 * ( yPXB1 + yPXB3 );
	double bx = xPXB2 - xmid;
	double by = yPXB2 - ymid;
	double bb = sqrt( bx*bx + by*by ); // from mid point to point 2

	h406->Fill( hp.numberOfValidTrackerHits() );
	h407->Fill( hp.numberOfValidPixelBarrelHits() );
	h408->Fill( hp.trackerLayersWithMeasurement() );

	// Author: Johannes Gassner (15.11.1996)
	// Make track from 2 space points and kappa (cmz98/ftn/csmktr.f)
	// Definition of the Helix :

	// x( t ) = X0 + KAPPA^-1 * SIN( PHI0 + t )
	// y( t ) = Y0 - KAPPA^-1 * COS( PHI0 + t )          t > 0
	// z( t ) = Z0 + KAPPA^-1 * TAN( DIP ) * t

	// Center of the helix in the xy-projection:

	// X0 = + ( DCA - KAPPA^-1 ) * SIN( PHI0 )
	// Y0 = - ( DCA - KAPPA^-1 ) * COS( PHI0 )

	// Point 1 must be in the inner layer, 3 in the outer:

	double r1 = sqrt( xPXB1*xPXB1 + yPXB1*yPXB1 );
	double r3 = sqrt( xPXB3*xPXB3 + yPXB3*yPXB3 );

	if( r3-r1 < 2.0 ) cout << "warn r1 = " << r1 << ", r3 = " << r3 << endl;

	// Calculate the centre of the helix in xy-projection that
	// transverses the two spacepoints. The points with the same
	// distance from the two points are lying on a line.
	// LAMBDA is the distance between the point in the middle of
	// the two spacepoints and the centre of the helix.

	// we already have kap and rho = 1/kap

	double lam = sqrt( -0.25 + 
			   rho*rho / ( ( xPXB1 - xPXB3 )*( xPXB1 - xPXB3 ) + ( yPXB1 - yPXB3 )*( yPXB1 - yPXB3 ) ) );

	// There are two solutions, the sign of kap gives the information
	// which of them is correct:

	if( kap > 0 ) lam = -lam;

	// ( X0, Y0 ) is the centre of the circle
	// that describes the helix in xy-projection:

	double x0 =  0.5*( xPXB1 + xPXB3 ) + lam * ( -yPXB1 + yPXB3 );
	double y0 =  0.5*( yPXB1 + yPXB3 ) + lam * (  xPXB1 - xPXB3 );

	// Calculate theta:

	double num = ( yPXB3 - y0 ) * ( xPXB1 - x0 ) - ( xPXB3 - x0 ) * ( yPXB1 - y0 );
	double den = ( xPXB1 - x0 ) * ( xPXB3 - x0 ) + ( yPXB1 - y0 ) * ( yPXB3 - y0 );
	double tandip = kap * ( zPXB3 - zPXB1 ) / atan( num / den );
	double udip = atan(tandip);
	//double utet = pihalf - udip;

	// To get phi0 in the right interval one must distinguish
	// two cases with positve and negative kap:

	double uphi;
	if( kap > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );

	// The distance of the closest approach DCA depends on the sign
	// of kappa:

	double udca;
	if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
	else          udca = rho + sqrt( x0*x0 + y0*y0 );

	// angle from first hit to dca point:

	double dphi = atan( ( ( xPXB1 - x0 ) * y0 - ( yPXB1 - y0 ) * x0 )
			  / ( ( xPXB1 - x0 ) * x0 + ( yPXB1 - y0 ) * y0 ) );

	double uz0 = zPXB1 + tandip * dphi * rho;

	h401->Fill( zPXB2 );
	h402->Fill( uphi - iTrack->phi() );
	h403->Fill( udca - iTrack->d0() );
	h404->Fill( udip - iTrack->lambda() );
	h405->Fill( uz0  - iTrack->dz() );

	// interpolate to middle hit:
	// cirmov
	// we already have rinv = -kap

	double cosphi = cos(uphi);
	double sinphi = sin(uphi);
	double dp = -xPXB2*sinphi + yPXB2*cosphi + udca;
	double dl = -xPXB2*cosphi - yPXB2*sinphi;
	double sa = 2*dp + rinv * ( dp*dp + dl*dl );
	double dca2 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
	double ud = 1 + rinv*udca;
	double phi2 = atan2( -rinv*xPXB2 + ud*sinphi, rinv*yPXB2 + ud*cosphi );//direction

	double phiinc = phi2 - phiN2;//angle of incidence in rphi w.r.t. normal vector

	// phiN alternates inward/outward
	// reduce phiinc:

	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	// arc length:

	double xx = xPXB2 + dca2 * sin(phi2); // point on track
	double yy = yPXB2 - dca2 * cos(phi2);

	double vx = xx - xmid2;//from module center
	double vy = yy - ymid2;
	double vv = sqrt( vx*vx + vy*vy );

	double f0 = uphi;//
	double kx = kap*xx;
	double ky = kap*yy;
	double kd = kap*udca;

	// Solve track equation for s:

	double dx = kx - (kd-1)*sin(f0);
	double dy = ky + (kd-1)*cos(f0);
	double ks = atan2( dx, -dy ) - f0;// turn angle

	// Limit to half-turn:

	if(      ks >  pi ) ks = ks - twopi;
	else if( ks < -pi ) ks = ks + twopi;

	double s = ks*rho; // signed
	double uz2 = uz0 + s*tandip; // track z at R2
	double dz2 = zPXB2 - uz2;

	Surface::GlobalPoint gp2( xx, yy, uz2 );
	Surface::LocalPoint lp2 = det2->toLocal( gp2 );

	// local x = phi
	// local y = z in barrel
	// local z = radial in barrel (thickness)

	double xpix = fmod( uPXB2 + 0.82, 0.01 ); // xpix = 0..0.01 reconstructed
	double xpx2 = fmod( uPXB2 + 0.82, 0.02 ); // xpix = 0..0.02 reconstructed
	double xpx1 = fmod( uPXB1 + 0.82, 0.01 ); // xpix = 0..0.01 reconstructed
	double xpx3 = fmod( uPXB3 + 0.82, 0.01 ); // xpix = 0..0.01 reconstructed

	//double dpix = fmod( uPXB2 + dca2 + 0.82, 0.01 ); // dpix = 0..0.01 predicted

	double vpix = fmod( vv, 0.01 ); // vpix = 0..0.01 predicted
	if( uPXB2 < 0 ) vpix = -vpix; // vv is unsigned distance from module center

	double lpix = fmod( lp2.x() + 0.82, 0.01 ); // lpix = 0..0.01 predicted
	double tpix = fmod( lp2.x() + 0.82, 0.02 ); // tpix = 0..0.02 predicted

	double zpix = fmod( lp2.y() + 3.24, 0.015 ); // zpix = 0..0.015 predicted
	double spix = fmod( lp2.y() + 3.24, 0.03  ); // spix = 0..0.03  predicted

	int smin = zmin2%52; // 0..51 column along z
	int smax = zmax2%52; // 0..51 column along z

	double cogx = (cogp2 + 0.5 - 80) * 0.01 - 0.0054; // Lorentz shift
	if( cogp2 < 79 ) cogx -= 0.01; // big pix
	if( cogp2 > 80 ) cogx += 0.01; // big pix

	double mpix = fmod( cogx + 0.82, 0.01 ); // mpix = 0..0.01 from cluster COG
	double cogdx = cogx - lp2.x(); // residual

	// hybrid method:

	double hybx = uPXB2; // template
	if( mpix*1E4 < 20 ) hybx = cogx; // COG
	if( mpix*1E4 > 75 ) hybx = cogx;
	//double hpix = fmod( hybx + 0.82, 0.01 ); // hpix = 0..0.01 from cluster hybrid method
	double hybdx = hybx - lp2.x(); // residual

	bool halfmod = 0;
	if(      ilad2 ==  8 ) halfmod = 1;
	else if( ilad2 ==  9 ) halfmod = 1;
	else if( ilad2 == 24 ) halfmod = 1;
	else if( ilad2 == 25 ) halfmod = 1;

	if( pt > 4 ) {

	  h308->Fill( bb/aa ); // lever arm
	  h409->Fill( f2*wt, phiinc*wt );
	  h410->Fill( dca2*1E4 );
	  h411->Fill( dz2*1E4 );

	  // pile up study Summer 2011:

	  if( nvertex < 3 ) {
	    h310->Fill( dca2*1E4 );
	    h311->Fill( dz2*1E4 );
	  }
	  else if( nvertex < 6 ) {
	    h312->Fill( dca2*1E4 );
	    h313->Fill( dz2*1E4 );
	  }
	  else if( nvertex < 10 ) {
	    h314->Fill( dca2*1E4 );
	    h315->Fill( dz2*1E4 );
	  }
	  else {
	    h316->Fill( dca2*1E4 );
	    h317->Fill( dz2*1E4 );
	    h318->Fill( nvertex );
	  } // pile up

	} // pt > 4

	if( pt > 12 ) {

	  h420->Fill( dca2*1E4 ); // 12.7 um
	  h421->Fill( dz2*1E4 );

	  // Add errors and pulls
          h077->Fill( ePXB2*1E4 );
          h078->Fill( fPXB2*1E4 );
          if(idbg) cout<<" residuals "<<dca2*1E4<<" "<<ePXB2*1E4<<" "<<dz2*1E4
		       <<" "<<fPXB2*1E4<<endl;
          double pulx=0., puly=0.;
          if(ePXB2!=0.0) pulx = dca2/ePXB2;
          if(fPXB2!=0.0) puly = dz2/fPXB2;
          h079->Fill( pulx );
          h069->Fill( puly );

	  // Alignment errors 
	  //LocalError lape = det2->localAlignmentError();
	  //cout<<geomDet2<<endl;
	  if(geomDet2 != NULL) {
	    LocalError lape = geomDet2->localAlignmentError();
	    //cout<< lape.valid() <<endl;
	    if (lape.valid()) {
	      float tmp11= 0.;
	      if(lape.xx()>0.) tmp11= sqrt(lape.xx())*1E4;
	      float tmp13= 0.;
	      if(lape.yy()>0.) tmp13= sqrt(lape.yy())*1E4;
	      //cout<<" layer 2 "<<tmp11<<" "<<tmp13<<endl;
	      h088->Fill( tmp11 );
	      h089->Fill( tmp13 );
	    } else {   if( myCounters::neve < 10 ) cout<<" lape invalid?"<<endl;}
	  }

          if(bb/aa < 0.015) h420_1->Fill(dca2*1E4);
          else if(bb/aa >= 0.015 && bb/aa < 0.065) h420_2->Fill(dca2*1E4);
          else h420_3->Fill(dca2*1E4);

	  //Flipped/non-flipped
	  if( abs( phi2 - phiN2 ) < pihalf ) // outward facing module
            if(zPXB2 > 0)
	      h420_out_zplus->Fill( dca2*1E4 );
            else
	      h420_out_zminus->Fill( dca2*1E4 );
	  else
            if(zPXB2 > 0)
	      h420_in_zplus->Fill( dca2*1E4 );
            else
	      h420_in_zminus->Fill( dca2*1E4 );
	  
	  // //////// Modules:1-8
	
	  if( imod==1  ){// innermost  module
	    h420_mod1->Fill( dca2*1E4 );
	    h421_mod1->Fill( dz2*1E4 );
	  }
	  if( imod==2  ){
	    h420_mod2->Fill( dca2*1E4 );
	    h421_mod2->Fill( dz2*1E4 );
	  }
	  if( imod==3  ){
	    h420_mod3->Fill( dca2*1E4 );
	    h421_mod3->Fill( dz2*1E4 );
	  }
	  if( imod==4  ){
	    h420_mod4->Fill( dca2*1E4 );
	     h421_mod4->Fill( dz2*1E4 );
	  }
	  if( imod==5  ){
	    h420_mod5->Fill( dca2*1E4 );
	     h421_mod5->Fill( dz2*1E4 );
	  }
	  if( imod==6 ){
	    h420_mod6->Fill( dca2*1E4 );
	    h421_mod6->Fill( dz2*1E4 );
	  }	  
	  if( imod==7  ){
	    h420_mod7->Fill( dca2*1E4 );
	    h421_mod7->Fill( dz2*1E4 );
	  }
	  if( imod==8  ){// outermost  module
	    h420_mod8->Fill( dca2*1E4 );     
	     h421_mod8->Fill( dz2*1E4 );
	  }
	  
	  
	  // versus number o ftracker hits
	  if( hp.trackerLayersWithMeasurement() > 8 ) {
	    h430->Fill( dca2*1E4 );
	    h431->Fill( dz2*1E4 );
	  }
	  //
	  if( phiinc*wt > -1 && phiinc*wt < 7 ){
	    h440->Fill( dca2*1E4 ); // 11.4 um
	    h441->Fill( dz2*1E4 );
	  }
	  // Versus cluster size in x
	  if(      nrow2 == 1 ) h442->Fill( dca2*1E4 ); // 13.6
	  else if( nrow2 == 2 ) h443->Fill( dca2*1E4 ); // 12.7
	  else if( nrow2 == 3 ) h444->Fill( dca2*1E4 ); // 19.7 um
	  else                  h445->Fill( dca2*1E4 ); // two peaks

	  if( nrow1 == 2 && nrow2 == 2 && nrow3 == 2 ) h446->Fill( dca2*1E4 ); // 12.2 um

	  h447->Fill( nrow2 );

	} // pt > 12

	// residual profiles: alignment check

	if( pt > 4 ) {
	  h412->Fill( f2*wt, dca2*1E4 );
	  h413->Fill( f2*wt, dz2*1E4 );

	  if( abs( phi2 - phiN2 ) < pihalf ) // outward facing module
            if(zPXB2 > 0)
	      h412_out_zplus->Fill( f2*wt, dca2*1E4 );
            else
	      h412_out_zminus->Fill( f2*wt, dca2*1E4 );
	  else
            if(zPXB2 > 0)
	      h412_in_zplus->Fill( f2*wt, dca2*1E4 );
            else
	      h412_in_zminus->Fill( f2*wt, dca2*1E4 );

	  h414->Fill( zPXB2, dca2*1E4 );
	  h415->Fill( zPXB2, dz2*1E4 );
	}
	h416->Fill( logpt, dca2*1E4 );
	h417->Fill( logpt, dz2*1E4 );
	if( iTrack->charge() > 0 ) h418->Fill( logpt, dca2*1E4 );
	else h419->Fill( logpt, dca2*1E4 );

	// profile of abs(dca) gives mean abs(dca):
	// mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
	// => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
	// point resolution = 1/sqrt(3/2) * triplet middle residual width
	// => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

	if( pt > 4 ) {

	  h422->Fill( f2*wt, abs(dca2)*1E4 );
	  h423->Fill( f2*wt, abs(dz2)*1E4 );

	  h424->Fill( zPXB2, abs(dca2)*1E4 );
	  h425->Fill( zPXB2, abs(dz2)*1E4 );

	  h428->Fill( udip*wt, abs(dca2)*1E4 );
	  h429->Fill( udip*wt, abs(dz2)*1E4 );
          h429_eta->Fill(iTrack->eta(), abs(dz2)*1E4);
	  if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) h432->Fill( dz2*1E4 );

	  h434->Fill( phiinc*wt, abs(dca2)*1E4 );
	  h334->Fill( phiinc*wt, abs(cogdx)*1E4 );//similar

	  if( abs( phi2 - phiN2 ) < pihalf ) // outward facing module
	    h436->Fill( phiinc*wt, abs(dca2)*1E4 );
	  else
	    h437->Fill( phiinc*wt, abs(dca2)*1E4 );//similar

	  if(  lpix*1E4 > 30 && lpix*1E4 < 70 ) h319->Fill( phiinc*wt, abs(dca2)*1E4 );//flat

	} // pt > 4

	h426->Fill( logpt, abs(dca2)*1E4 );
	h427->Fill( logpt, abs(dz2)*1E4 );
	if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) h433->Fill( logpt, abs(dz2)*1E4 );

	// pt bins:
	// uPXB2 = local x
	// vPXB2 = local z
	// low pt: material

	if( pt > 0.8 && pt < 1.2 ) {
	  h435->Fill( f2*wt, abs(dca2)*1E4 );
	  if( ! halfmod ) {
	    h210->Fill( uPXB2, abs(dca2)*1E4 ); // hump at +-0.15
	    //search material along v=z in u=x slices:
	    if(      abs(uPXB2) < 0.1 ) h215->Fill( vPXB2, abs(dca2)*1E4 );
	    else if( abs(uPXB2) < 0.2 ) h216->Fill( vPXB2, abs(dca2)*1E4 );
	    else if( abs(uPXB2) < 0.3 ) h217->Fill( vPXB2, abs(dca2)*1E4 );
	    else if( abs(uPXB2) < 0.4 ) h218->Fill( vPXB2, abs(dca2)*1E4 );
	    else if( abs(uPXB2) < 0.5 ) h219->Fill( vPXB2, abs(dca2)*1E4 );
	    else if( abs(uPXB2) < 0.6 ) h225->Fill( vPXB2, abs(dca2)*1E4 );
	    else if( abs(uPXB2) < 0.7 ) h226->Fill( vPXB2, abs(dca2)*1E4 );
	    else                        h327->Fill( vPXB2, abs(dca2)*1E4 );
	  } // !halfmod
	  h220->Fill( vPXB2, abs(dca2)*1E4 );
	  h230->Fill( vPXB2, abs(dz2)*1E4 );
	} // pt

	if( pt > 1.2 && pt < 1.6 ) {
	  if( ! halfmod ) h211->Fill( uPXB2, abs(dca2)*1E4 );
	  h221->Fill( vPXB2, abs(dca2)*1E4 );
	  h231->Fill( vPXB2, abs(dz2)*1E4 );
	}

	if( pt > 1.6 && pt < 2.2 ) {
	  if( ! halfmod ) h212->Fill( uPXB2, abs(dca2)*1E4 );
	  h222->Fill( vPXB2, abs(dca2)*1E4 );
	  h232->Fill( vPXB2, abs(dz2)*1E4 );
	}

	if( pt > 2.2 && pt < 4.0 ) {
	  if( ! halfmod ) h213->Fill( uPXB2, abs(dca2)*1E4 );
	  h223->Fill( vPXB2, abs(dca2)*1E4 );
	  h233->Fill( vPXB2, abs(dz2)*1E4 );
	}

	if( pt > 4.0 ) {
	  if( ! halfmod ) h214->Fill( uPXB2, abs(dca2)*1E4 );
	  h224->Fill( vPXB2, abs(dca2)*1E4 );
	  h234->Fill( vPXB2, abs(dz2)*1E4 );
	}

	// full modules:

	if( pt > 4.0 &&
	    ! halfmod &&
	    xmin2 > 0 && xmax2 < 159 && // skip edges
	    xmin2 != 79 && xmax2 != 79 &&  // skip wide pixels
	    xmin2 != 80 && xmax2 != 80 ) { // skip wide pixels

	  h235->Fill( xpix*1E4 ); // from cluster
	  h236->Fill( lpix*1E4 ); // from track: flat

	  h285->Fill( mpix*1E4 ); // from cluster mixed
	  h286->Fill( uPXB2, (cogx - uPXB2)*1E4 ); // flat

	  if( nrow2 == 1 ) h237->Fill( lpix*1E4 );
	  if( nrow2 == 2 ) h238->Fill( lpix*1E4 );
	  if( nrow2 == 3 ) h239->Fill( lpix*1E4 );

	  if( nrow2 == 1 ) h227->Fill( xpix*1E4 );
	  if( nrow2 == 2 ) h228->Fill( xpix*1E4 );
	  if( nrow2 == 3 ) h229->Fill( xpix*1E4 );

	  h240->Fill( tpix*1E4, nrow2 );
	  h322->Fill( tpix*1E4, clch2*1E-3*cos(dip) );
	  if( nrow2 < 3 ) h330->Fill( tpix*1E4, etaX2 );
	  h331->Fill( tpix*1E4, ePXB2*1E4 );//hit error
	  h332->Fill( ePXB2*1E4 );//hit error

	  h241->Fill( xpix*1E4, dca2*1E4 ); // flat at zero

	  h242->Fill( xpx2*1E4, abs(dca2)*1E4 ); // xpix = from cluster

	  h243->Fill( lpix*1E4, dca2*1E4 ); // no bias
	  h244->Fill( tpix*1E4, abs(dca2)*1E4 ); // tpix = from track, sine

	  // dca w.r.t. cluster COG cogx:

	  h287->Fill( lp2.x(), cogdx*1E4 ); // zero
	  h288->Fill( lpix*1E4, cogdx*1E4 ); // +-2 um
	  h289->Fill( tpix*1E4, abs(cogdx)*1E4 ); // cogx = COG

	  // hybrid method:

	  h283->Fill( tpix*1E4, abs(hybdx)*1E4 ); // similar to dca2

	  if( nrow2 == 2 ) {
	    h343->Fill( lpix*1E4, dca2*1E4 ); // bias? no.
	    h267->Fill( tpix*1E4, abs(dca2)*1E4 ); // U-shaped
	  }

	  h268->Fill( lpix*1E4, abs(dca2)*1E4 ); // one-pixel wide resolution
	  h320->Fill( zpix*1E4, abs(dca2)*1E4 );//flat
	  h321->Fill( spix*1E4, abs(dca2)*1E4 );//two-pixel z, flat

	  // in-pixel map:

	  if( smin > 0 && smax < 51 ) { // skip long pixels in z
	    h323->Fill( spix*1E4, tpix*1E4 ); // 2x2 flat
	    if( abs(dip)*wt < 10 ) h324->Fill( spix*1E4, tpix*1E4 );
	  }

	  // highest pt:

	  if( pt > 12 ) {

	    if(      lpix*1E4 > 30 && lpix*1E4 < 70 ) h438->Fill( dca2*1E4 ); // 9.1 um
	    else if( lpix*1E4 < 20 || lpix*1E4 > 80 ) h439->Fill( dca2*1E4 ); // 17.0, two peaks
	    if( nrow2 == 1 ) h290->Fill( dca2*1E4 ); // single 13.1

	    h379->Fill( dca2 *1E4 ); // dca from templates
	    h380->Fill( cogdx*1E4 ); // dca from COG: 12.8
	    h284->Fill( hybdx*1E4 ); // hybrid 12.6

	    // nrow = 1 and nrow = 2 are related
	    // might expect shifts and biases if analyzed separately

	    if( nrow2 == 2 ) {

	      if( lpix*1E4 < 20 ) h291->Fill( dca2*1E4 ); // split
	      if( lpix*1E4 > 80 ) h292->Fill( dca2*1E4 ); // split

	      h381->Fill( cogdx*1E4 );

	      if( lpix*1E4 < 20 ) h382->Fill( cogdx*1E4 );//right
	      if( lpix*1E4 > 80 ) h383->Fill( cogdx*1E4 );//left

	      // study split:

	      if( lpix*1E4 < 20 || lpix*1E4 > 80 ){

		if( abs( phi2 - phiN2 ) < pihalf )
		  h293->Fill( dca2*1E4 ); // split
		else
		  h294->Fill( dca2*1E4 ); // split

		if( zPXB2 > 0 ) 
		  h295->Fill( dca2*1E4 ); // split
		else
		  h296->Fill( dca2*1E4 ); // split

		if( iTrack->charge() > 0 )
		  h297->Fill( dca2*1E4 ); // split
		else
		  h298->Fill( dca2*1E4 ); // split

		if( phiinc > 0 )
		  h299->Fill( dca2*1E4 ); // split
		else
		  h300->Fill( dca2*1E4 ); // split

		if( uPXB2 > 0 )
		  h390->Fill( dca2*1E4 ); // split
		else
		  h391->Fill( dca2*1E4 ); // split

		if( clch2 < 40E3 )
		  h392->Fill( dca2*1E4 ); // split
		else
		  h393->Fill( dca2*1E4 );//

	      }//pixel edges

	      // 4-Felder test (Claus Kleinwort):

	      if( lpix*1E4 < 20 ) {
		if( abs( phi2 - phiN2 ) < pihalf )
		  h394->Fill( dca2*1E4 );//left
		else
		  h395->Fill( dca2*1E4 );//right
	      }
	      if( lpix*1E4 > 80 ) {
		if( abs( phi2 - phiN2 ) < pihalf )
		  h396->Fill( dca2*1E4 );//right
		else
		  h397->Fill( dca2*1E4 );//left
	      }

	    }//nrow=2

	    // study split for all nrow:
	    // 4-Felder test (Claus Kleinwort):

	    if( lpix*1E4 < 20 ) {
	      if( abs( phi2 - phiN2 ) < pihalf )
		h374->Fill( dca2*1E4 ); // left
	      else
		h375->Fill( dca2*1E4 ); // right
	    }
	    if( lpix*1E4 > 80 ) {
	      if( abs( phi2 - phiN2 ) < pihalf )
		h376->Fill( dca2*1E4 ); // right
	      else
		h377->Fill( dca2*1E4 ); // left
	    }

	    if( lpix*1E4 > 30 &&
		lpix*1E4 < 70 &&
		xpx1*1E4 > 25 &&
		xpx1*1E4 < 75 &&
		xpx3*1E4 > 25 &&
		xpx3*1E4 < 75 )
	      h448->Fill( dca2*1E4 ); // 9.1 um

	  }//pt > 12

	  // study eta:

	  //if( nrow2 == 2 ) {
	  if( nrow2 < 3 ) {

	    if( pt > 12 && lpix*1E4 > 30 && lpix*1E4 < 70 ) h449->Fill( dca2*1E4 );//9.2

	    h245->Fill( tpix*1E4, etaX2 ); // x from track

	    if( abs( phi2 - phiN2 ) < pihalf ) // outward module
	      h246->Fill( tpix*1E4, etaX2 ); // identical
	    else
	      h247->Fill( tpix*1E4, etaX2 ); // identical

	    h248->Fill( xpx2*1E4, etaX2 ); // x from cluster

	    if(      lpix < 0.0000 ) h249->Fill( etaX2 );
	    else if( lpix < 0.0010 ) h250->Fill( etaX2 );
	    else if( lpix < 0.0020 ) h251->Fill( etaX2 );
	    else if( lpix < 0.0030 ) h252->Fill( etaX2 );
	    else if( lpix < 0.0040 ) h253->Fill( etaX2 );
	    else if( lpix < 0.0050 ) h254->Fill( etaX2 );
	    else if( lpix < 0.0060 ) h255->Fill( etaX2 );
	    else if( lpix < 0.0070 ) h256->Fill( etaX2 );
	    else if( lpix < 0.0080 ) h257->Fill( etaX2 );
	    else if( lpix < 0.0090 ) h258->Fill( etaX2 );
	    else if( lpix < 0.0100 ) h259->Fill( etaX2 );

	    // angle of incidence:

	    h260->Fill( phiinc*wt ); // +-10 deg

	    if(      phiinc*wt  < -10 )
	      h261->Fill( tpix*1E4, etaX2 );
	    else if( phiinc*wt  <  -5 )
	      h262->Fill( tpix*1E4, etaX2 );
	    else if( phiinc*wt  <   0 )
	      h263->Fill( tpix*1E4, etaX2 );
	    else if( phiinc*wt  <   5 )
	      h264->Fill( tpix*1E4, etaX2 );
	    else if( phiinc*wt  <  10 )
	      h265->Fill( tpix*1E4, etaX2 );
	    else
	      h266->Fill( tpix*1E4, etaX2 );

	    // understand xpix algorithm:

	    if(      xpix < 0.0000 ) h269->Fill( etaX2 );
	    else if( xpix < 0.0010 ) h270->Fill( etaX2 );
	    else if( xpix < 0.0020 ) h271->Fill( etaX2 );
	    else if( xpix < 0.0030 ) h272->Fill( etaX2 );
	    else if( xpix < 0.0040 ) h273->Fill( etaX2 );
	    else if( xpix < 0.0050 ) h274->Fill( etaX2 );
	    else if( xpix < 0.0060 ) h275->Fill( etaX2 );
	    else if( xpix < 0.0070 ) h276->Fill( etaX2 );
	    else if( xpix < 0.0080 ) h277->Fill( etaX2 );
	    else if( xpix < 0.0090 ) h278->Fill( etaX2 );
	    else if( xpix < 0.0100 ) h279->Fill( etaX2 );

	    if(      etaX2 < -0.3 ) h280->Fill( xpix*1E4 );
	    else if( etaX2 <  0.3 ) h281->Fill( xpix*1E4 );
	    else                    h282->Fill( xpix*1E4 );

	  }//nrow = 2

	}//pt > 4, ! halfmod

      }//triplet 1+3 -> 2

      //------------------------------------------------------------------------
      // Triplet 1+3 -> 2 with refitted track:

      if( refit2valid ){

	if( jdbg ) cout << "  triplet 1+3 -> 2 refitted\n";

	double f2 = atan2( yPXB2, xPXB2 );//position angle in layer 2

	double ax = xPXB3 - xPXB1;
	double ay = yPXB3 - yPXB1;
	double aa = sqrt( ax*ax + ay*ay ); // from 1 to 3

	double xmid = 0.5 * ( xPXB1 + xPXB3 );
	double ymid = 0.5 * ( yPXB1 + yPXB3 );
	double bx = xPXB2 - xmid;
	double by = yPXB2 - ymid;
	double bb = sqrt( bx*bx + by*by ); // from mid point to point 2

	double rho2 = 1/kap2;

	// Calculate the centre of the helix in xy-projection that
	// transverses the two spacepoints. The points with the same
	// distance from the two points are lying on a line.
	// LAMBDA is the distance between the point in the middle of
	// the two spacepoints and the centre of the helix.

	double lam = sqrt( -0.25 + 
			   rho2*rho2 / ( ( xPXB1 - xPXB3 )*( xPXB1 - xPXB3 ) + ( yPXB1 - yPXB3 )*( yPXB1 - yPXB3 ) ) );

	// There are two solutions, the sign of kap gives the information
	// which of them is correct:

	if( kap2 > 0 ) lam = -lam;

	// ( X0, Y0 ) is the centre of the circle
	// that describes the helix in xy-projection:

	double x0 =  0.5*( xPXB1 + xPXB3 ) + lam * ( -yPXB1 + yPXB3 );
	double y0 =  0.5*( yPXB1 + yPXB3 ) + lam * (  xPXB1 - xPXB3 );

	// Calculate theta:

	double num = ( yPXB3 - y0 ) * ( xPXB1 - x0 ) - ( xPXB3 - x0 ) * ( yPXB1 - y0 );
	double den = ( xPXB1 - x0 ) * ( xPXB3 - x0 ) + ( yPXB1 - y0 ) * ( yPXB3 - y0 );
	double tandip = kap2 * ( zPXB3 - zPXB1 ) / atan( num / den );
	double udip = atan(tandip);
	//double utet = pihalf - udip;

	// To get phi0 in the right interval one must distinguish
	// two cases with positve and negative kap:

	double uphi;
	if( kap2 > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );

	// The distance of the closest approach DCA depends on the sign
	// of kappa:

	double udca;
	if( kap2 > 0 ) udca = rho2 - sqrt( x0*x0 + y0*y0 );
	else          udca = rho2 + sqrt( x0*x0 + y0*y0 );

	// angle from first hit to dca point:

	double dphi = atan( ( ( xPXB1 - x0 ) * y0 - ( yPXB1 - y0 ) * x0 )
			  / ( ( xPXB1 - x0 ) * x0 + ( yPXB1 - y0 ) * y0 ) );

	double uz0 = zPXB1 + tandip * dphi * rho2;

	h451->Fill( zPXB2 );
	h452->Fill( uphi - iTrack->phi() );
	h453->Fill( udca - iTrack->d0() );
	h454->Fill( udip - iTrack->lambda() );
	h455->Fill( uz0  - iTrack->dz() );

	// interpolate to middle hit:
	// cirmov

	double rinv2 = -kap2;
	double cosphi = cos(uphi);
	double sinphi = sin(uphi);
	double dp = -xPXB2*sinphi + yPXB2*cosphi + udca;
	double dl = -xPXB2*cosphi - yPXB2*sinphi;
	double sa = 2*dp + rinv2 * ( dp*dp + dl*dl );
	double dca2 = sa / ( 1 + sqrt(1 + rinv2*sa) );// distance to hit 2
	double ud = 1 + rinv2*udca;
	double phi2 = atan2( -rinv2*xPXB2 + ud*sinphi, rinv2*yPXB2 + ud*cosphi );//direction

	double phiinc = phi2 - phiN2;//angle of incidence in rphi w.r.t. normal vector

	// phiN alternates inward/outward
	// reduce phiinc:

	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	// arc length:

	double xx = xPXB2 + dca2 * sin(phi2); // point on track
	double yy = yPXB2 - dca2 * cos(phi2);

	double f0 = uphi;//
	double kx = kap2*xx;
	double ky = kap2*yy;
	double kd = kap2*udca;

	// Solve track equation for s:

	double dx = kx - (kd-1)*sin(f0);
	double dy = ky + (kd-1)*cos(f0);
	double ks = atan2( dx, -dy ) - f0;// turn angle

	//---  Limit to half-turn:

	if(      ks >  pi ) ks = ks - twopi;
	else if( ks < -pi ) ks = ks + twopi;

	double s = ks*rho2;// signed
	double uz2 = uz0 + s*tandip; //track z at R2
	double dz2 = zPXB2 - uz2;

	if( pt > 4 ) {
	  h456->Fill( (kap2 - kap) / kap );
	  h459->Fill( f2*wt, phiinc*wt );
	  h460->Fill( dca2*1E4 );
	  h461->Fill( dz2*1E4 );
	}

	if( pt > 12 ) {

	  h470->Fill( dca2*1E4 );
	  h471->Fill( dz2*1E4 );

          if(bb/aa < 0.015) h470_1->Fill(dca2*1E4);
          else if(bb/aa >= 0.015 && bb/aa < 0.065) h470_2->Fill(dca2*1E4);
          else h470_3->Fill(dca2*1E4);

	  if( hp.trackerLayersWithMeasurement() > 8 ) {
	    h480->Fill( dca2*1E4 );
	    h481->Fill( dz2*1E4 );
	  }

	  if( phiinc*wt > -1 && phiinc*wt < 7 ){
	    h490->Fill( dca2*1E4 );
	    h491->Fill( dz2*1E4 );
	  }

	  if( nrow2 == 1 ) h492->Fill( dca2*1E4 );
	  else {
	    if( nrow2 == 2 ) h493->Fill( dca2*1E4 );
	    else {
	      if( nrow2 == 3 ) h494->Fill( dca2*1E4 );
	      else h495->Fill( dca2*1E4 );
	    }
	  }

	  if( nrow1 == 2 && nrow2 == 2 && nrow3 == 2 ) h496->Fill( dca2*1E4 );

	}//pt>12

	// residual profiles: alignment check

	if( pt > 4 ) {
	  h462->Fill( f2*wt, dca2*1E4 );
	  h463->Fill( f2*wt, dz2*1E4 );
	  
	  h464->Fill( zPXB2, dca2*1E4 );
	  h465->Fill( zPXB2, dz2*1E4 );
	}
	h466->Fill( logpt, dca2*1E4 );
	h467->Fill( logpt, dz2*1E4 );
	if( iTrack->charge() > 0 ) h468->Fill( logpt, dca2*1E4 );
	else h469->Fill( logpt, dca2*1E4 );

	// profile of abs(dca) gives mean abs(dca):
	// mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
	// => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
	// point resolution = 1/sqrt(3/2) * triplet middle residual width
	// => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

	if( pt > 4 ) {

	  h472->Fill( f2*wt, abs(dca2)*1E4 );
	  h473->Fill( f2*wt, abs(dz2)*1E4 );

	  h474->Fill( zPXB2, abs(dca2)*1E4 );
	  h475->Fill( zPXB2, abs(dz2)*1E4 );

	  h478->Fill( udip*wt, abs(dca2)*1E4 );
	  h479->Fill( udip*wt, abs(dz2)*1E4 );
	  if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) h482->Fill( dz2*1E4 );

	  h484->Fill( phiinc*wt, abs(dca2)*1E4 );

	}//pt

	if( pt > 0.8 && pt < 1.2 ) { // low pt
	  h485->Fill( f2*wt, abs(dca2)*1E4 );
	}
	h476->Fill( logpt, abs(dca2)*1E4 );
	h477->Fill( logpt, abs(dz2)*1E4 );
	if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) h483->Fill( logpt, abs(dz2)*1E4 );

      }//triplet 1+3 -> 2 refitted

      //------------------------------------------------------------------------
      // triplet 3+2 -> 1:

      {
	if( jdbg ) cout << "  triplet 3+2 -> 1\n";

	double f1 = atan2( yPXB1, xPXB1 );//position angle in layer 1

	double ax = xPXB3 - xPXB2;
	double ay = yPXB3 - yPXB2;
	double aa = sqrt( ax*ax + ay*ay ); // from 2 to 3

	double xmid = 0.5 * ( xPXB2 + xPXB3 );
	double ymid = 0.5 * ( yPXB2 + yPXB3 );
	double bx = xPXB1 - xmid;
	double by = yPXB1 - ymid;
	double bb = sqrt( bx*bx + by*by ); // from mid point to point 1

	// Calculate the centre of the helix in xy-projection that
	// transverses the two spacepoints. The points with the same
	// distance from the two points are lying on a line.
	// LAMBDA is the distance between the point in the middle of
	// the two spacepoints and the centre of the helix.

	// we already have kap and rho = 1/kap

	double lam = sqrt( -0.25 + 
			   rho*rho / ( ( xPXB2 - xPXB3 )*( xPXB2 - xPXB3 ) + ( yPXB2 - yPXB3 )*( yPXB2 - yPXB3 ) ) );

	// There are two solutions, the sign of kap gives the information
	// which of them is correct:

	if( kap > 0 ) lam = -lam;

	// ( X0, Y0 ) is the centre of the circle
	// that describes the helix in xy-projection:

	double x0 =  0.5*( xPXB2 + xPXB3 ) + lam * ( -yPXB2 + yPXB3 );
	double y0 =  0.5*( yPXB2 + yPXB3 ) + lam * (  xPXB2 - xPXB3 );

	// Calculate theta:

	double num = ( yPXB3 - y0 ) * ( xPXB2 - x0 ) - ( xPXB3 - x0 ) * ( yPXB2 - y0 );
	double den = ( xPXB2 - x0 ) * ( xPXB3 - x0 ) + ( yPXB2 - y0 ) * ( yPXB3 - y0 );
	double tandip = kap * ( zPXB3 - zPXB2 ) / atan( num / den );
	double udip = atan(tandip);
	//double utet = pihalf - udip;

	// To get phi0 in the right interval one must distinguish
	// two cases with positve and negative kap:

	double uphi;
	if( kap > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );

	// The distance of the closest approach DCA depends on the sign
	// of kappa:

	double udca;
	if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
	else          udca = rho + sqrt( x0*x0 + y0*y0 );

	// angle from first hit to dca point:

	double dphi = atan( ( ( xPXB2 - x0 ) * y0 - ( yPXB2 - y0 ) * x0 )
			  / ( ( xPXB2 - x0 ) * x0 + ( yPXB2 - y0 ) * y0 ) );

	double uz0 = zPXB2 + tandip * dphi * rho;

	h501->Fill( zPXB1 );
	h502->Fill( uphi - iTrack->phi() );
	h503->Fill( udca - iTrack->d0() );
	h504->Fill( udip - iTrack->lambda() );
	h505->Fill( uz0  - iTrack->dz() );

	// extrapolate to inner hit:
	// cirmov
	// we already have rinv = -kap

	double cosphi = cos(uphi);
	double sinphi = sin(uphi);
	double dp = -xPXB1*sinphi + yPXB1*cosphi + udca;
	double dl = -xPXB1*cosphi - yPXB1*sinphi;
	double sa = 2*dp + rinv * ( dp*dp + dl*dl );
	double dca1 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 1
	double ud = 1 + rinv*udca;
	double phi1 = atan2( -rinv*xPXB1 + ud*sinphi, rinv*yPXB1 + ud*cosphi );//direction

	double phiinc = phi1 - phiN1;//angle of incidence in rphi w.r.t. normal vector

	// phiN alternates inward/outward
	// reduce phiinc:

	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	// arc length:

	double xx = xPXB1 + dca1 * sin(phi1); // point on track
	double yy = yPXB1 - dca1 * cos(phi1);

	double f0 = uphi;//
	double kx = kap*xx;
	double ky = kap*yy;
	double kd = kap*udca;

	// Solve track equation for s:

	double dx = kx - (kd-1)*sin(f0);
	double dy = ky + (kd-1)*cos(f0);
	double ks = atan2( dx, -dy ) - f0;// turn angle

	//---  Limit to half-turn:

	if(      ks >  pi ) ks = ks - twopi;
	else if( ks < -pi ) ks = ks + twopi;

	double s = ks*rho;// signed
	double uz1 = uz0 + s*tandip; //track z at R1
	double dz1 = zPXB1 - uz1;

	if( pt > 4 ) {
	  h508->Fill( bb/aa );//lever arm
	  h509->Fill( f1*wt, phiinc*wt );
	  h510->Fill( dca1*1E4 );
	  h511->Fill( dz1*1E4 );
	}

	if( pt > 12 ) {

	  h520->Fill( dca1*1E4 );
	  h521->Fill( dz1*1E4 );

          if(bb/aa >= 1.10 && bb/aa < 1.27) h520_1->Fill(dca1*1E4);
          else if(bb/aa >= 1.27 && bb/aa < 1.43) h520_2->Fill(dca1*1E4);
          else if(bb/aa >= 1.43 && bb/aa < 1.57) h520_3->Fill(dca1*1E4);
          else if(bb/aa >= 1.57 && bb/aa < 1.80) h520_4->Fill(dca1*1E4);
          else if(bb/aa >= 1.80 && bb/aa < 2.00) h520_5->Fill(dca1*1E4);

	  if( hp.trackerLayersWithMeasurement() > 8 ) {
	    h530->Fill( dca1*1E4 );
	    h531->Fill( dz1*1E4 );
	  }

	  if( phiinc*wt > -1 && phiinc*wt < 7 ){
	    h540->Fill( dca1*1E4 );
	    h541->Fill( dz1*1E4 );
	  }

	  if( nrow1 == 1 ) h542->Fill( dca1*1E4 );
	  else {
	    if( nrow1 == 2 ) h543->Fill( dca1*1E4 );
	    else {
	      if( nrow1 == 3 ) h544->Fill( dca1*1E4 );
	      else h545->Fill( dca1*1E4 );
	    }
	  }

	  if( nrow1 == 2 && nrow2 == 2 && nrow3 == 2 ) h546->Fill( dca1*1E4 );

	}//pt>12

	// residual profiles: alignment check

	if( pt > 4 ) {
	  h512->Fill( f1*wt, dca1*1E4 );
	  h513->Fill( f1*wt, dz1*1E4 );

	  h514->Fill( zPXB1, dca1*1E4 );
	  h515->Fill( zPXB1, dz1*1E4 );
	}
	h516->Fill( logpt, dca1*1E4 );
	h517->Fill( logpt, dz1*1E4 );
	if( iTrack->charge() > 0 ) h518->Fill( logpt, dca1*1E4 );
	else h519->Fill( logpt, dca1*1E4 );

	// profile of abs(dca) gives mean abs(dca):
	// mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
	// => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
	// point resolution = 1/sqrt(3/2) * triplet middle residual width
	// => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

	if( pt > 4 ) {

	  h522->Fill( f1*wt, abs(dca1)*1E4 );
	  h523->Fill( f1*wt, abs(dz1)*1E4 );

	  h524->Fill( zPXB1, abs(dca1)*1E4 );
	  h525->Fill( zPXB1, abs(dz1)*1E4 );

	  h528->Fill( udip*wt, abs(dca1)*1E4 );
	  h529->Fill( udip*wt, abs(dz1)*1E4 );
	  if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) h532->Fill( dz1*1E4 );

	  h534->Fill( phiinc*wt, abs(dca1)*1E4 );

	}//pt

	if( pt > 0.8 && pt < 1.2 ) { // low pt
	  h535->Fill( f1*wt, abs(dca1)*1E4 );
	}
	h526->Fill( logpt, abs(dca1)*1E4 );
	h527->Fill( logpt, abs(dz1)*1E4 );
	if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) h533->Fill( logpt, abs(dz1)*1E4 );
      }

      //------------------------------------------------------------------------
      // triplet 1+2 -> 3:

      {
	if( jdbg ) cout << "  triplet 1+2 -> 3\n";

	double f3 = atan2( yPXB3, xPXB3 );//position angle in layer 3

	double ax = xPXB2 - xPXB1;
	double ay = yPXB2 - yPXB1;
	double aa = sqrt( ax*ax + ay*ay ); // from 1 to 2

	double xmid = 0.5 * ( xPXB1 + xPXB2 );
	double ymid = 0.5 * ( yPXB1 + yPXB2 );
	double bx = xPXB3 - xmid;
	double by = yPXB3 - ymid;
	double bb = sqrt( bx*bx + by*by ); // from mid point to point 3

	// Calculate the centre of the helix in xy-projection that
	// transverses the two spacepoints. The points with the same
	// distance from the two points are lying on a line.
	// LAMBDA is the distance between the point in the middle of
	// the two spacepoints and the centre of the helix.

	// we already have kap and rho = 1/kap

	double lam = sqrt( -0.25 + 
			   rho*rho / ( ( xPXB1 - xPXB2 )*( xPXB1 - xPXB2 ) + ( yPXB1 - yPXB2 )*( yPXB1 - yPXB2 ) ) );

	// There are two solutions, the sign of kap gives the information
	// which of them is correct:

	if( kap > 0 ) lam = -lam;

	// ( X0, Y0 ) is the centre of the circle
	// that describes the helix in xy-projection:

	double x0 =  0.5*( xPXB1 + xPXB2 ) + lam * ( -yPXB1 + yPXB2 );
	double y0 =  0.5*( yPXB1 + yPXB2 ) + lam * (  xPXB1 - xPXB2 );

	// Calculate theta:

	double num = ( yPXB2 - y0 ) * ( xPXB1 - x0 ) - ( xPXB2 - x0 ) * ( yPXB1 - y0 );
	double den = ( xPXB1 - x0 ) * ( xPXB2 - x0 ) + ( yPXB1 - y0 ) * ( yPXB2 - y0 );
	double tandip = kap * ( zPXB2 - zPXB1 ) / atan( num / den );
	double udip = atan(tandip);
	//double utet = pihalf - udip;

	// To get phi0 in the right interval one must distinguish
	// two cases with positve and negative kap:

	double uphi;
	if( kap > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );

	// The distance of the closest approach DCA depends on the sign
	// of kappa:

	double udca;
	if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
	else          udca = rho + sqrt( x0*x0 + y0*y0 );

	// angle from first hit to dca point:

	double dphi = atan( ( ( xPXB1 - x0 ) * y0 - ( yPXB1 - y0 ) * x0 )
			  / ( ( xPXB1 - x0 ) * x0 + ( yPXB1 - y0 ) * y0 ) );

	double uz0 = zPXB1 + tandip * dphi * rho;

	i501->Fill( zPXB3 );
	i502->Fill( uphi - iTrack->phi() );
	i503->Fill( udca - iTrack->d0() );
	i504->Fill( udip - iTrack->lambda() );
	i505->Fill( uz0  - iTrack->dz() );

	// extrapolate to outer hit:
	// cirmov
	// we already have rinv = -kap

	double cosphi = cos(uphi);
	double sinphi = sin(uphi);
	double dp = -xPXB3*sinphi + yPXB3*cosphi + udca;
	double dl = -xPXB3*cosphi - yPXB3*sinphi;
	double sa = 2*dp + rinv * ( dp*dp + dl*dl );
	double dca3 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 3
	double ud = 1 + rinv*udca;
	double phi3 = atan2( -rinv*xPXB3 + ud*sinphi, rinv*yPXB3 + ud*cosphi );//track direction

	double phiinc = phi3 - phiN3;//angle of incidence in rphi w.r.t. normal vector

	// phiN alternates inward/outward
	// reduce phiinc:

	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	// arc length:

	double xx = xPXB3 + dca3 * sin(phi3); // point on track
	double yy = yPXB3 - dca3 * cos(phi3);

	double f0 = uphi;//
	double kx = kap*xx;
	double ky = kap*yy;
	double kd = kap*udca;

	// Solve track equation for s:

	double dx = kx - (kd-1)*sin(f0);
	double dy = ky + (kd-1)*cos(f0);
	double ks = atan2( dx, -dy ) - f0;// turn angle

	//---  Limit to half-turn:

	if(      ks >  pi ) ks = ks - twopi;
	else if( ks < -pi ) ks = ks + twopi;

	double s = ks*rho;// signed
	double uz3 = uz0 + s*tandip; //track z at R3
	double dz3 = zPXB3 - uz3;

	if( pt > 4 ) {
	  i507->Fill( bb/aa );//lever arm
	  i508->Fill( f3*wt, bb/aa );//lever arm
	  i509->Fill( f3*wt, phiinc*wt );
	  i510->Fill( dca3*1E4 );
	  i511->Fill( dz3*1E4 );
	}

	if( pt > 12 ) {

	  i520->Fill( dca3*1E4 );
	  i521->Fill( dz3*1E4 );

          if(bb/aa >= 1.10 && bb/aa < 1.27) i520_1->Fill(dca3*1E4);
          else if(bb/aa >= 1.27 && bb/aa < 1.43) i520_2->Fill(dca3*1E4);
          else if(bb/aa >= 1.43 && bb/aa < 1.57) i520_3->Fill(dca3*1E4);
          else if(bb/aa >= 1.57 && bb/aa < 1.80) i520_4->Fill(dca3*1E4);
          else if(bb/aa >= 1.80 && bb/aa < 2.00) i520_5->Fill(dca3*1E4);

          if( abs( phi3 - phiN3 ) < pihalf ) // outward facing module
            if(zPXB3 > 0)
              i520_out_zplus->Fill(dca3*1E4);
            else
              i520_out_zminus->Fill(dca3*1E4);
          else
            if(zPXB3 > 0)
              i520_in_zplus->Fill(dca3*1E4);
            else
              i520_in_zminus->Fill(dca3*1E4);

	  if( hp.trackerLayersWithMeasurement() > 8 ) {
	    i530->Fill( dca3*1E4 );
	    i531->Fill( dz3*1E4 );
	  }

	  if( phiinc*wt > -1 && phiinc*wt < 7 ){
	    i540->Fill( dca3*1E4 );
	    i541->Fill( dz3*1E4 );
	  }

	  if( nrow1 == 1 ) i542->Fill( dca3*1E4 );
	  else {
	    if( nrow1 == 2 ) i543->Fill( dca3*1E4 );
	    else {
	      if( nrow1 == 3 ) i544->Fill( dca3*1E4 );
	      else i545->Fill( dca3*1E4 );
	    }
	  }

	  if( nrow1 == 2 && nrow2 == 2 && nrow3 == 2 ) i546->Fill( dca3*1E4 );

	}//pt>12

	// residual profiles: alignment check

	if( pt > 4 ) {
	  i512->Fill( f3*wt, dca3*1E4 );
	  i513->Fill( f3*wt, dz3*1E4 );

          if( abs( phi3 - phiN3 ) < pihalf ) // outward facing module
            if(zPXB3 > 0)
	      i512_out_zplus->Fill( f3*wt, dca3*1E4 );
            else
	      i512_out_zminus->Fill( f3*wt, dca3*1E4 );
          else
            if(zPXB3 > 0)
  	      i512_in_zplus->Fill( f3*wt, dca3*1E4 );
            else
  	      i512_in_zminus->Fill( f3*wt, dca3*1E4 );

	  i514->Fill( zPXB3, dca3*1E4 );
	  i515->Fill( zPXB3, dz3*1E4 );
	}
	i516->Fill( logpt, dca3*1E4 );
	i517->Fill( logpt, dz3*1E4 );
	if( iTrack->charge() > 0 ) i518->Fill( logpt, dca3*1E4 );
	else i519->Fill( logpt, dca3*1E4 );

	// profile of abs(dca) gives mean abs(dca):
	// mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
	// => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
	// point resolution = 1/sqrt(3/2) * triplet middle residual width
	// => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

	if( pt > 4 ) {

	  i522->Fill( f3*wt, abs(dca3)*1E4 );
	  i523->Fill( f3*wt, abs(dz3)*1E4 );
	  
	  i524->Fill( zPXB3, abs(dca3)*1E4 );
	  i525->Fill( zPXB3, abs(dz3)*1E4 );

	  i528->Fill( udip*wt, abs(dca3)*1E4 );
	  i529->Fill( udip*wt, abs(dz3)*1E4 );
	  if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) i532->Fill( dz3*1E4 );

	  i534->Fill( phiinc*wt, abs(dca3)*1E4 );

	}//pt

	if( pt > 0.8 && pt < 1.2 ) { // low pt
	  i535->Fill( f3*wt, abs(dca3)*1E4 );
	}
	i526->Fill( logpt, abs(dca3)*1E4 );
	i527->Fill( logpt, abs(dz3)*1E4 );
	if( abs(dip)*wt > 18 && abs(dip)*wt < 50 ) i533->Fill( logpt, abs(dz3)*1E4 );
      }

      //------------------------------------------------------------------------
      // Karimaki circle fit through 3 points:

      if( jdbg ) cout << "  Karimaki\n";

      double SW = 0;
      double SX = 0;
      double SY = 0;
      double SXX = 0;
      double SXY = 0;
      double SYY = 0;
      double SXR2 = 0;
      double SYR2 = 0;
      double SR2 = 0;
      double SR4 = 0;

      for( int ii = 1; ii <= 3; ++ii ){

	double XI, YI;

	if(      ii == 1 ){
	  XI = xPXB1;
	  YI = yPXB1;
	}
	else if( ii == 2 ){
	  XI = xPXB2;
	  YI = yPXB2;
	}
	else if( ii == 3 ){
	  XI = xPXB3;
	  YI = yPXB3;
	}

	XI -= xBS; // ref = beam
	YI -= yBS;

	double R2 = XI*XI + YI*YI;
	double WI = 1; // should be 1/sigma**2

	SW   = SW   + WI;
	SX   = SX   + WI * XI;
	SY   = SY   + WI * YI;
	SXX  = SXX  + WI * XI*XI;
	SXY  = SXY  + WI * XI*YI;
	SYY  = SYY  + WI * YI*YI;
	SR2  = SR2  + WI * R2;
	SXR2 = SXR2 + WI * XI*R2;
	SYR2 = SYR2 + WI * YI*R2;
	SR4  = SR4  + WI * R2*R2;

      }//loop ii

      double AX   = SX / SW;
      double AY   = SY / SW;
      double AXX  = SXX / SW;
      double AXY  = SXY / SW;
      double AYY  = SYY / SW;
      double AR2  = SR2 / SW;
      double AXR2 = SXR2 / SW;
      double AYR2 = SYR2 / SW;
      double AR4  = SR4 / SW;

      double CXX  = AXX - AX*AX;
      double CXY  = AXY - AX*AY;
      double CYY  = AYY - AY*AY;
      double CXR2 = AXR2 - AX*AR2;
      double CYR2 = AYR2 - AY*AR2;
      double CR4  = AR4 - AR2*AR2;

      double Q1 = CR4*CXY - CXR2*CYR2;
      double Q2 = CR4*(CXX-CYY) - CXR2*CXR2 + CYR2*CYR2;
      double PHI = 0.5 * atan2( 2*Q1, Q2 ); // track direction at ref. point (beam)
      double SINPHI = sin(PHI);
      double COSPHI = cos(PHI);
      if( COSPHI*AX + SINPHI*AY < 0 ) {
	if( PHI < 0 ) PHI = PHI + pi;
	else          PHI = PHI - pi;
	SINPHI = sin(PHI);
	COSPHI = cos(PHI);
      }

      double FKA = ( SINPHI*CXR2 - COSPHI*CYR2 ) / CR4;
      double DLT = -FKA*AR2 + SINPHI*AX - COSPHI*AY;
      double SQT = sqrt( 1 - 4*DLT*FKA );
      double RNV = 2*FKA / SQT; // inverse radius = -kappa
      double DCA = 2*DLT / ( 1 + SQT ); // dca to reference point (beam)
      double KAP = -RNV; // CMS sign convention
      double ERD = 1.0 - KAP*DCA;
      double RHO = 1/KAP;

      h557->Fill( KAP - kap );
      double df = PHI - phip;
      if( df > pi ) df -= twopi;
      else if( df < -pi ) df += twopi;
      h558->Fill( df );
      h559->Fill( DCA - dcap );

      double f2 = atan2( yPXB2, xPXB2 );//position angle in layer 2

      if( pt > 4 ) {
	h560->Fill( DCA*1E4 );
	if( pt > 8 ) h561->Fill( DCA*1E4 );
	h562->Fill( f2*wt, DCA*1E4 );
	h564->Fill( f2*wt, abs(DCA)*1E4 );
      }
      h563->Fill( logpt, DCA*1E4 );
      h565->Fill( logpt, abs(DCA)*1E4 );

      // angle of incidence:

      {
	double dx = xPXB1 - xBS;
	double dy = yPXB1 - yBS;
	double dp = -dx*SINPHI + dy*COSPHI + DCA;
	double dl = -dx*COSPHI - dy*SINPHI;
	double sa = 2*dp + RNV * ( dp*dp + dl*dl );
	double dca = sa / ( 1 + sqrt(1 + RNV*sa) );// distance to hit, should be zero
	double ud = 1 + RNV*DCA;
	double phi = atan2( -RNV*dx + ud*SINPHI, RNV*dy + ud*COSPHI );//track direction

	double phiinc = phi - phiN1;//angle of incidence in rphi w.r.t. normal vector

	// phiN alternates inward/outward
	// reduce phiinc:

	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	h570->Fill( dca*1E4 );//should be 0.0, is zero
	h571->Fill( phiinc*wt );
	h572->Fill( phiinc*wt, pt );
	h573->Fill( phiinc*wt, iTrack->charge() );
	h574->Fill( phiinc*wt, nrow1 );
	h575->Fill( tan(phiinc), nrow1 );
	if( abs(phi - phiN1) < pihalf )
	  h576->Fill( tan(phiinc), nrow1 );
	else
	  h577->Fill( tan(phiinc), nrow1 );

      }//PXB1

      {
	double dx = xPXB2 - xBS;
	double dy = yPXB2 - yBS;
	double dp = -dx*SINPHI + dy*COSPHI + DCA;
	double dl = -dx*COSPHI - dy*SINPHI;
	double sa = 2*dp + RNV * ( dp*dp + dl*dl );
	double dca = sa / ( 1 + sqrt(1 + RNV*sa) );// distance to hit, should be zero
	double ud = 1 + RNV*DCA;
	double phi = atan2( -RNV*dx + ud*SINPHI, RNV*dy + ud*COSPHI );//track direction

	double phiinc = phi - phiN2;//angle of incidence in rphi w.r.t. normal vector

	// phiN alternates inward/outward
	// reduce phiinc:

	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	h580->Fill( dca*1E4 );//should be 0.0
	h581->Fill( phiinc*wt );
	h582->Fill( phiinc*wt, pt );
	h583->Fill( phiinc*wt, iTrack->charge() );
	h584->Fill( phiinc*wt, nrow2 );
	h585->Fill( tan(phiinc), nrow2 );
	if( abs(phi - phiN2) < pihalf )
	  h586->Fill( tan(phiinc), nrow2 );
	else
	  h587->Fill( tan(phiinc), nrow2 );

      }//PXB2

      {
	double dx = xPXB3 - xBS;
	double dy = yPXB3 - yBS;
	double dp = -dx*SINPHI + dy*COSPHI + DCA;
	double dl = -dx*COSPHI - dy*SINPHI;
	double sa = 2*dp + RNV * ( dp*dp + dl*dl );
	double dca = sa / ( 1 + sqrt(1 + RNV*sa) );// distance to hit, should be zero
	double ud = 1 + RNV*DCA;
	double phi = atan2( -RNV*dx + ud*SINPHI, RNV*dy + ud*COSPHI );//track direction

	double phiinc = phi - phiN3;//angle of incidence in rphi w.r.t. normal vector

	// phiN alternates inward/outward
	// reduce phiinc:

	if( phiinc > pihalf ) phiinc -= pi;
	else if( phiinc < -pihalf ) phiinc += pi;

	h590->Fill( dca*1E4 );//should be 0.0
	h591->Fill( phiinc*wt );
	h592->Fill( phiinc*wt, pt );
	h593->Fill( phiinc*wt, iTrack->charge() );
	h594->Fill( phiinc*wt, nrow3 );
	h595->Fill( tan(phiinc), nrow3 );
	if( abs(phi - phiN3) < pihalf )
	  h596->Fill( tan(phiinc), nrow3 );
	else
	  h597->Fill( tan(phiinc), nrow3 );

      }//PXB3

      //------------------------------------------------------------------------
      // add beam spot as measurement:

      SW   = SW   + 0.5; // half the weight of a pixel hit

      AX   = SX / SW;
      AY   = SY / SW;
      AXX  = SXX / SW;
      AXY  = SXY / SW;
      AYY  = SYY / SW;
      AR2  = SR2 / SW;
      AXR2 = SXR2 / SW;
      AYR2 = SYR2 / SW;
      AR4  = SR4 / SW;

      CXX  = AXX - AX*AX;
      CXY  = AXY - AX*AY;
      CYY  = AYY - AY*AY;
      CXR2 = AXR2 - AX*AR2;
      CYR2 = AYR2 - AY*AR2;
      CR4  = AR4 - AR2*AR2;

      Q1 = CR4*CXY - CXR2*CYR2;
      Q2 = CR4*(CXX-CYY) - CXR2*CXR2 + CYR2*CYR2;
      PHI = 0.5 * atan2( 2*Q1, Q2 ); // track direction at ref. point
      SINPHI = sin(PHI);
      COSPHI = cos(PHI);
      if( COSPHI*AX + SINPHI*AY < 0 ) {
	if( PHI < 0 ) PHI = PHI + pi;
	else          PHI = PHI - pi;
	SINPHI = sin(PHI);
	COSPHI = cos(PHI);
      }

      FKA = ( SINPHI*CXR2 - COSPHI*CYR2 ) / CR4;
      DLT = -FKA*AR2 + SINPHI*AX - COSPHI*AY;
      SQT = sqrt( 1 - 4*DLT*FKA );
      RNV = 2*FKA / SQT; // inverse radius = -kappa
      DCA = 2*DLT / ( 1 + SQT ); // dca to reference point
      KAP = -RNV; //CMS sign convention
      ERD = 1.0 - KAP*DCA;
      RHO = 1/KAP;

      h667->Fill( -RNV - kap );
      df = PHI - phip;
      if( df > pi ) df -= twopi;
      else if( df < -pi ) df += twopi;
      h668->Fill( df );
      h669->Fill( DCA - dcap );

      f2 = atan2( yPXB2, xPXB2 );//position angle in layer 2

      if( pt > 4 ) {
	h670->Fill( DCA*1E4 );
	if( pt > 8 ) h671->Fill( DCA*1E4 );
	h672->Fill( f2*wt, DCA*1E4 );
	h674->Fill( f2*wt, abs(DCA)*1E4 );
      }
      h673->Fill( logpt, DCA*1E4 );
      h675->Fill( logpt, abs(DCA)*1E4 );

      // arc length from beam to pixel:

      double dx1 = xPXB1 - xBS;
      double dy1 = yPXB1 - yBS;
      double R1 = sqrt( dx1*dx1 + dy1*dy1 );//from origin
      double sindp1 = ( 0.5*KAP * (R1*R1 + DCA*DCA) - DCA ) / (R1*ERD);
      double sina1 = R1*KAP * sqrt( 1.0 - sindp1*sindp1 );
      double s1 = asin(sina1) / KAP;//always positive

      double dx3 = xPXB3 - xBS;
      double dy3 = yPXB3 - yBS;
      double R3 = sqrt( dx3*dx3 + dy3*dy3 );//from origin
      double sindp3 = ( 0.5*KAP * (R3*R3 + DCA*DCA) - DCA ) / (R3*ERD);
      double sina3 = R3*KAP * sqrt( 1.0 - sindp3*sindp3 );
      double s3 = asin(sina3) / KAP;//always positive

      double tandip = ( zPXB3 - zPXB1 ) / ( s3 - s1 );//pixel dip angle

      double Z0p = zPXB1 - s1*tandip; //z at beam

      //------------------------------------------------------------------------
      // extrapolate Pixel track to strip tracker:

      for( vector<TransientTrackingRecHit::RecHitPointer>::iterator iTTRH = myTTRHvec.begin();
	   iTTRH != myTTRHvec.end(); ++iTTRH ) {

	if( ! (*iTTRH)->hit()->isValid() ) continue;

	DetId detId = (*iTTRH)->hit()->geographicalId();
	uint32_t subDet = detId.subdetId();

	// enum SubDetector{ TIB=3, TID=4, TOB=5, TEC=6 };

	if( subDet < 3 ) continue; // skip PXB, PXD

	if( jdbg ) cout << "  extrapolate pixel track to " << subDet << endl;

	const Topology* theTopology = &( (*iTTRH)->detUnit()->topology() );

	//const GeomDet * myGeomDet = pTG->idToDet( detId );
	const GeomDet * myGeomDet = (*iTTRH)->det();

	double xDet = myGeomDet->position().x();
	double yDet = myGeomDet->position().y();
	double zDet = myGeomDet->position().z();

	// barrel: track to R

	if( subDet == 3 || subDet == 5 ) { //TIB,TOB

	  double phiN = myGeomDet->surface().normalVector().barePhi();//normal vector

	  double phidet = phiN - pihalf;// orientation of sensor plane in x-y
	  double ux = cos(phidet);// vector in sensor plane
	  double uy = sin(phidet);

	  double x = myGeomDet->position().x() - xBS;
	  double y = myGeomDet->position().y() - yBS;

	  // intersect helix with line: FUNRXY (in FUNPHI) from V. Blobel
	  // factor f for intersect point (x + f*ux, y + f*uy)

	  double cf = cos(PHI);
	  double sf = sin(PHI);
	  double drd = DCA * ( 0.5*KAP*DCA - 1.0 ); // 0.5 * kap * dca**2 - dca;
	  double a =                                 KAP * ( ux*ux + uy*uy ) * 0.5;
	  double b =       ERD * ( ux*sf - uy*cf ) + KAP * ( ux*x + uy*y );
	  double c = drd + ERD * (  x*sf -  y*cf ) + KAP * (  x*x +  y*y ) * 0.5;
	  double dis = b*b - 4.0*a*c;

	  double f = 0;

	  if( dis > 0 ) {

	    dis = sqrt(dis);
	    double f1 = 0;
	    double f2 = 0;

	    if( b < 0 ) {
	      f1 = 0.5 * ( dis - b ) / a;
	      f2 = 2.0 * c / ( dis - b );
	    }
	    else{
	      f1 = -0.5 * ( dis + b ) / a;
	      f2 = -2.0 * c / ( dis + b );
	    }

	    f = f1;
	    if( abs(f2) < abs(f1) ) f = f2;

	  }//dis

	  double xcrss = x + f*ux;//from beam
	  double ycrss = y + f*uy;
	  double r = sqrt( xcrss*xcrss + ycrss*ycrss );

	  double s = 0;

	  if( r >= abs(DCA) ) {
	    double sindp = ( 0.5*KAP * ( r*r + DCA*DCA ) - DCA ) / (r*ERD);
	    double sina = r*KAP * sqrt( 1.0 - sindp*sindp );
	    if( sina >= 1.0 )
	      s = pi / KAP;
	    else{
	      if( sina <= -1.0 )
		s = -pi / KAP;
	      else
		s = asin(sina) / KAP;
	    }
	    double hkk = 0.5*KAP*KAP;
	    if( hkk * ( r*r - DCA*DCA ) > ERD ) s = pi/abs(KAP) - s;
	  }

	  double Z = Z0p + s*tandip; // z at r
	  double X = xcrss + xBS;
	  double Y = ycrss + yBS;
	  double F = atan2( Y, X );
	  double R = sqrt( X*X + Y*Y );

	  Surface::GlobalPoint gp( X, Y, Z );

	  Surface::LocalPoint lp = myGeomDet->toLocal( gp );

	  // MeasurementPoint [pitch] (like channel number)

	  MeasurementPoint hitMeasurement = theTopology->measurementPosition( (*iTTRH)->localPosition() );

	  // TID and TEC have trapezoidal detectors:
	  // translation from channel number into local x depends on local y
	  // track prediction has local x,y => can convert into proper channel number MeasurementPoint:

	  MeasurementPoint predictedPosition = theTopology->measurementPosition( lp );

	  double resid = hitMeasurement.x() - predictedPosition.x();//[pitch]

	  double xptch;
	  //double yptch;	

	  if( subDet <  3 ){//1,2=pixel
	    PixelTopology & pixelTopol = (PixelTopology&) (*iTTRH)->detUnit()->topology();
	    xptch = pixelTopol.pitch().first;
	    //yptch = pixelTopol.pitch().second;
	  }
	  else {//strip
	    StripTopology & stripTopol = (StripTopology&) (*iTTRH)->detUnit()->topology();
	    xptch = stripTopol.localPitch( lp );
	    //yptch = stripTopol.localStripLength( lp );
	  }

	  resid = resid * xptch;//[cm]

	  if( subDet == 3 ){ //3=TIB

	    if( pt > 4 ){
	      r301->Fill( resid*1E1 );
	      r302->Fill( F*wt, resid*1E1 );
	      r303->Fill( F*wt, abs(resid)*1E1 );
	      r304->Fill( R, resid*1E1 );
	      r305->Fill( R, abs(resid)*1E1 );
	    }
	    r306->Fill( logpt, resid*1E1 );
	    r307->Fill( logpt, abs(resid)*1E1 );

	  }//TIB

	}//TIB,TOB

	if( subDet == 4 || subDet == 6 ) { // 4=TID disk, 6=TEC wheel

	  if( tandip*zDet < 0 ) continue; // wrong z side

	  // disk: track at zDet

	  double DZ = zDet - Z0p;//beam to Det
	  double S = DZ/tandip;//arc length beam to Det
	  double PSI = KAP*S;//Abrollwinkel from beam to Det
	  double C0 = cos(PHI);
	  double S0 = sin(PHI);
	  double SP = sin(PHI+PSI);
	  double CP = cos(PHI+PSI);
	  double X = RHO*(-S0+SP) + xBS;
	  double Y = RHO*( C0-CP) + yBS;
	  double F = atan2( Y, X );
	  double R = sqrt( X*X + Y*Y );

	  double dx = X - xDet; //Det center
	  double dy = Y - yDet;

	  Surface::GlobalPoint gp( X, Y, zDet );

	  Surface::LocalPoint lp = myGeomDet->toLocal( gp );

	  // MeasurementPoint [pitch] (like channel number)

	  MeasurementPoint hitMeasurement = theTopology->measurementPosition( (*iTTRH)->localPosition() );

	  // TID and TEC have trapezoidal detectors:
	  // translation from channel number into local x depends on local y
	  // track prediction has local x,y => can convert into proper channel number MeasurementPoint:

	  MeasurementPoint predictedPosition = theTopology->measurementPosition( lp );

	  double resid = hitMeasurement.x() - predictedPosition.x();//[pitch]

	  double xptch;
	  //double yptch;	

	  if( subDet <  3 ){//1,2=pixel
	    PixelTopology & pixelTopol = (PixelTopology&) (*iTTRH)->detUnit()->topology();
	    xptch = pixelTopol.pitch().first;
	    //yptch = pixelTopol.pitch().second;
	  }
	  else {//strip
	    StripTopology & stripTopol = (StripTopology&) (*iTTRH)->detUnit()->topology();
	    xptch = stripTopol.localPitch( lp );
	    //yptch = stripTopol.localStripLength( lp );
	  }

	  resid = resid * xptch;//[cm]

	  if( subDet == 4 ){ //4=TID

	    r411->Fill( S );
	    r412->Fill( X, Y );
	    r413->Fill( xDet, yDet );
	    r414->Fill( dx );
	    r415->Fill( dy );

	    if( pt > 4 ){
	      r421->Fill( resid*1E1 );
	      r422->Fill( F*wt, resid*1E1 );
	      r423->Fill( F*wt, abs(resid)*1E1 );
	      r424->Fill( R, resid*1E1 );
	      r425->Fill( R, abs(resid)*1E1 );
	    }
	    r426->Fill( logpt, resid*1E1 );
	    r427->Fill( logpt, abs(resid)*1E1 );

	  }//TID

	  if( subDet == 6 ){//6=TEC

	    r611->Fill( S );
	    r612->Fill( X, Y );
	    r613->Fill( xDet, yDet );
	    r614->Fill( dx );
	    r615->Fill( dy );
	    if( pt > 4 ){
	      r620->Fill( resid );
	      r621->Fill( resid*1E1 );
	      r622->Fill( F*wt, resid );
	      r623->Fill( F*wt, abs(resid) );
	      r624->Fill( R, resid );
	      r625->Fill( R, abs(resid) );
	    }
	    r626->Fill( logpt, resid );
	    r627->Fill( logpt, abs(resid) );

	  }//TEC

	}//disk

      }//loop over hits

    }//3 PXB layers

    if( pt < 0.75 ) continue;// curls up

    //------------------------------------------------------------------------
    // triplet beam + 2 -> 1

    if( ibs && n1 > 0 && n2 > 0 ) {

      if( jdbg ) cout << "  triplet 2+B -> 1\n";

      double xb = xBS;
      double yb = yBS;

      double f1 = atan2( yPXB1, xPXB1 );//position angle in layer 1

      double ax = xPXB2 - xBS;
      double ay = yPXB2 - yBS;
      double aa = sqrt( ax*ax + ay*ay ); // from 1 to 2

      double xmid = 0.5 * ( xBS + xPXB2 );
      double ymid = 0.5 * ( yBS + yPXB2 );
      double bx = xPXB1 - xmid;
      double by = yPXB1 - ymid;
      double bb = sqrt( bx*bx + by*by ); // from mid point to point 1

      // Calculate the centre of the helix in xy-projection that
      // transverses the two spacepoints. The points with the same
      // distance from the two points are lying on a line.
      // LAMBDA is the distance between the point in the middle of
      // the two spacepoints and the centre of the helix.

      // we already have kap and rho = 1/kap

      double lam = sqrt( -0.25 + 
			 rho*rho / ( ( xb - xPXB2 )*( xb - xPXB2 ) + ( yb - yPXB2 )*( yb - yPXB2 ) ) );

      // There are two solutions, the sign of kap gives the information
      // which of them is correct:

      if( kap > 0 ) lam = -lam;

      // ( X0, Y0 ) is the centre of the circle
      // that describes the helix in xy-projection:

      double x0 =  0.5*( xb + xPXB2 ) + lam * ( -yb + yPXB2 );
      double y0 =  0.5*( yb + yPXB2 ) + lam * (  xb - xPXB2 );

      // To get phi0 in the right interval one must distinguish
      // two cases with positve and negative kap:

      double uphi;
      if( kap > 0 ) uphi = atan2( -x0,  y0 );
      else          uphi = atan2(  x0, -y0 );

      // The distance of the closest approach DCA depends on the sign
      // of kappa:

      double udca;
      if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
      else          udca = rho + sqrt( x0*x0 + y0*y0 );

      h601->Fill( zPXB1 );
      h602->Fill( uphi - iTrack->phi() );
      h603->Fill( udca - iTrack->d0() );

      // interpolate to hit 1:
      // cirmov
      // we already have rinv = -kap

      double cosphi = cos(uphi);
      double sinphi = sin(uphi);
      double dp = -xPXB1*sinphi + yPXB1*cosphi + udca;
      double dl = -xPXB1*cosphi - yPXB1*sinphi;
      double sa = 2*dp + rinv * ( dp*dp + dl*dl );
      double dca1 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 1
      double ud = 1 + rinv*udca;
      double phi1 = atan2( -rinv*xPXB1 + ud*sinphi, rinv*yPXB1 + ud*cosphi );//direction

      double phiinc = phi1 - phiN1;//angle of incidence in rphi w.r.t. normal vector

      // phiN alternates inward/outward
      // reduce phiinc:

      if( phiinc > pihalf ) phiinc -= pi;
      else if( phiinc < -pihalf ) phiinc += pi;

      if( pt > 4 ) {
	h607->Fill( bb/aa );//lever arm
	h608->Fill( phiinc*wt );
	h609->Fill( f1*wt, phiinc*wt );
	h610->Fill( dca1*1E4 );
      }

      if( pt > 12 ) {

	h620->Fill( dca1*1E4 );
        if( abs( phi1 - phiN1 ) < pihalf ) // outward facing module
          if(zPXB1 > 0)
	    h620_out_zplus->Fill( dca1*1E4 );
          else
	    h620_out_zminus->Fill( dca1*1E4 );
        else
          if(zPXB1 > 0)
	    h620_in_zplus->Fill( dca1*1E4 );
          else
	    h620_in_zminus->Fill( dca1*1E4 );

	if( hp.trackerLayersWithMeasurement() > 8 ) {
	  h630->Fill( dca1*1E4 );
	}

	if( phiinc*wt > -1 && phiinc*wt < 7 ){
	  h650->Fill( dca1*1E4 );
	}

	if( nrow1 == 1 ) h652->Fill( dca1*1E4 );
	else {
	  if( nrow1 == 2 ) h653->Fill( dca1*1E4 );
	  else {
	    if( nrow1 == 3 ) h654->Fill( dca1*1E4 );
	    else h655->Fill( dca1*1E4 );
	  }
	}

	if( nrow1 == 2 && nrow2 == 2 && nrow3 == 2 ) h656->Fill( dca1*1E4 );

      }//pt>12

      if( pt > 25 ) {
	h621->Fill( dca1*1E4 );
      }

      // residual profiles: alignment check

      if( pt > 4 ) {
	h612->Fill( f1*wt, dca1*1E4 );
	h614->Fill( zPXB1, dca1*1E4 );

        if( abs( phi1 - phiN1 ) < pihalf ) // outward facing module
          if(zPXB1 > 0)
	    h612_out_zplus->Fill( f1*wt, dca1*1E4 );
          else
	    h612_out_zminus->Fill( f1*wt, dca1*1E4 );
        else
          if(zPXB1 > 0)
	    h612_in_zplus->Fill( f1*wt, dca1*1E4 );
          else
	    h612_in_zminus->Fill( f1*wt, dca1*1E4 );
      }
      h616->Fill( logpt, dca1*1E4 );
      if( iTrack->charge() > 0 ) h618->Fill( logpt, dca1*1E4 );
      else h619->Fill( logpt, dca1*1E4 );

      // profile of abs(dca) gives mean abs(dca):
      // mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
      // => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
      // point resolution = 1/sqrt(3/2) * triplet middle residual width
      // => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

      if( pt > 4 ) {

	h622->Fill( f1*wt, abs(dca1)*1E4 );

	h624->Fill( zPXB1, abs(dca1)*1E4 );

	h628->Fill( dip*wt, abs(dca1)*1E4 );

	h634->Fill( phiinc*wt, abs(dca1)*1E4 );

      }//pt

      if( pt > 0.8 && pt < 1.2 ) { // low pt
	h635->Fill( f1*wt, abs(dca1)*1E4 );
      }
      h626->Fill( logpt, abs(dca1)*1E4 );

    }//beam+1+2

    //------------------------------------------------------------------------
    // TIB triplet 1+3 -> 2:

    if( nTIB1*nTIB2*nTIB3 > 0 ) {

      if( jdbg ) cout << "  TIB triplet 3+1 -> 2\n";

      double f2 = atan2( yTIB2, xTIB2 );//position angle

      double ax = xTIB3 - xTIB1;
      double ay = yTIB3 - yTIB1;
      double aa = sqrt( ax*ax + ay*ay ); // from 1 to 3

      double xmid = 0.5 * ( xTIB1 + xTIB3 );
      double ymid = 0.5 * ( yTIB1 + yTIB3 );
      double bx = xTIB2 - xmid;
      double by = yTIB2 - ymid;
      double bb = sqrt( bx*bx + by*by ); // from mid point to point 2

      // Calculate the centre of the helix in xy-projection that
      // transverses the two spacepoints. The points with the same
      // distance from the two points are lying on a line.
      // LAMBDA is the distance between the point in the middle of
      // the two spacepoints and the centre of the helix.

      // we already have kap and rho = 1/kap

      double lam = sqrt( -0.25 + 
			 rho*rho / ( ( xTIB1 - xTIB3 )*( xTIB1 - xTIB3 ) + ( yTIB1 - yTIB3 )*( yTIB1 - yTIB3 ) ) );

      // There are two solutions, the sign of kap gives the information
      // which of them is correct:

      if( kap > 0 ) lam = -lam;

      // ( X0, Y0 ) is the centre of the circle
      // that describes the helix in xy-projection:

      double x0 =  0.5*( xTIB1 + xTIB3 ) + lam * ( -yTIB1 + yTIB3 );
      double y0 =  0.5*( yTIB1 + yTIB3 ) + lam * (  xTIB1 - xTIB3 );

      // To get phi0 in the right interval one must distinguish
      // two cases with positve and negative kap:

      double uphi;
      if( kap > 0 ) uphi = atan2( -x0,  y0 );
      else          uphi = atan2(  x0, -y0 );

      // The distance of the closest approach DCA depends on the sign
      // of kappa:

      double udca;
      if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
      else          udca = rho + sqrt( x0*x0 + y0*y0 );

      h701->Fill( zTIB2 );
      h702->Fill( uphi - iTrack->phi() );
      h703->Fill( udca - iTrack->d0() );

      // interpolate to hit 2:
      // cirmov
      // we already have rinv = -kap

      double cosphi = cos(uphi);
      double sinphi = sin(uphi);
      double dp = -xTIB2*sinphi + yTIB2*cosphi + udca;
      double dl = -xTIB2*cosphi - yTIB2*sinphi;
      double sa = 2*dp + rinv * ( dp*dp + dl*dl );
      double dca2 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
      double ud = 1 + rinv*udca;
      double phi2 = atan2( -rinv*xTIB2 + ud*sinphi, rinv*yTIB2 + ud*cosphi );//direction

      double phiinc = phi2 - phiNTIB2;//angle of incidence in rphi w.r.t. normal vector

      // phiN alternates inward/outward
      // reduce phiinc

      if( phiinc > pihalf ) phiinc -= pi;
      else if( phiinc < -pihalf ) phiinc += pi;

      if( pt > 4 ) {
	h708->Fill( bb/aa );//lever arm
	h709->Fill( f2*wt, phiinc*wt );
	h710->Fill( dca2*1E4 );
      }

      if( pt > 12 ) {

	h720->Fill( dca2*1E4 );

	// 2010B: sigma(resid) = 25.6 in TIB2, 33.6 in TIB3
	// => sigma(TIB2) = 18.9
	// => sigma(TIB3) = 28.8

	if( hp.trackerLayersWithMeasurement() > 8 ) {
	  h730->Fill( dca2*1E4 );
	}

	if( phiinc*wt > -1 && phiinc*wt < 7 ){
	  h740->Fill( dca2*1E4 );
	}

      }//pt>12

      // residual profiles: alignment check

      if( pt > 4 ) {
	h712->Fill( f2*wt, dca2*1E4 );
	h714->Fill( zTIB2, dca2*1E4 );
      }
      h716->Fill( logpt, dca2*1E4 );
      if( iTrack->charge() > 0 ) h718->Fill( logpt, dca2*1E4 );
      else h719->Fill( logpt, dca2*1E4 );

      // profile of abs(dca) gives mean abs(dca):
      // mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
      // => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
      // point resolution = 1/sqrt(3/2) * triplet middle residual width
      // => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

      if( pt > 4 ) {

	h722->Fill( f2*wt, abs(dca2)*1E4 );

	h724->Fill( zTIB2, abs(dca2)*1E4 );

	h728->Fill( dip*wt, abs(dca2)*1E4 );

	h734->Fill( phiinc*wt, abs(dca2)*1E4 );

      }//pt

      if( pt > 0.8 && pt < 1.2 ) { // low pt
	h735->Fill( f2*wt, abs(dca2)*1E4 );
      }
      h726->Fill( logpt, abs(dca2)*1E4 );

    }//TIB 1,2,3

    //------------------------------------------------------------------------
    // TIB triplet 2+4 -> 3:

    if( nTIB2*nTIB3*nTIB4 > 0 ) {

      if( jdbg ) cout << "  TIB triplet 4+2 -> 3\n";

      double f3 = atan2( yTIB3, xTIB3 );//position angle

      double ax = xTIB4 - xTIB2;
      double ay = yTIB4 - yTIB2;
      double aa = sqrt( ax*ax + ay*ay ); // from 2 to 4

      double xmid = 0.5 * ( xTIB2 + xTIB4 );
      double ymid = 0.5 * ( yTIB2 + yTIB4 );
      double bx = xTIB3 - xmid;
      double by = yTIB3 - ymid;
      double bb = sqrt( bx*bx + by*by ); // from mid point to point 3

      // Calculate the centre of the helix in xy-projection that
      // transverses the two spacepoints. The points with the same
      // distance from the two points are lying on a line.
      // LAMBDA is the distance between the point in the middle of
      // the two spacepoints and the centre of the helix.

      // we already have kap and rho = 1/kap

      double lam = sqrt( -0.25 + 
			 rho*rho / ( ( xTIB2 - xTIB4 )*( xTIB2 - xTIB4 ) + ( yTIB2 - yTIB4 )*( yTIB2 - yTIB4 ) ) );

      // There are two solutions, the sign of kap gives the information
      // which of them is correct:

      if( kap > 0 ) lam = -lam;

      // ( X0, Y0 ) is the centre of the circle
      // that describes the helix in xy-projection:

      double x0 =  0.5*( xTIB2 + xTIB4 ) + lam * ( -yTIB2 + yTIB4 );
      double y0 =  0.5*( yTIB2 + yTIB4 ) + lam * (  xTIB2 - xTIB4 );

      // To get phi0 in the right interval one must distinguish
      // two cases with positve and negative kap:

      double uphi;
      if( kap > 0 ) uphi = atan2( -x0,  y0 );
      else          uphi = atan2(  x0, -y0 );

      // The distance of the closest approach DCA depends on the sign
      // of kappa:

      double udca;
      if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
      else          udca = rho + sqrt( x0*x0 + y0*y0 );

      h751->Fill( zTIB2 );
      h752->Fill( uphi - iTrack->phi() );
      h753->Fill( udca - iTrack->d0() );

      // interpolate to hit 2:
      // cirmov
      // we already have rinv = -kap

      double cosphi = cos(uphi);
      double sinphi = sin(uphi);
      double dp = -xTIB3*sinphi + yTIB3*cosphi + udca;
      double dl = -xTIB3*cosphi - yTIB3*sinphi;
      double sa = 2*dp + rinv * ( dp*dp + dl*dl );
      double dca3 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
      double ud = 1 + rinv*udca;
      double phi3 = atan2( -rinv*xTIB3 + ud*sinphi, rinv*yTIB3 + ud*cosphi );//direction

      double phiinc = phi3 - phiNTIB3;//angle of incidence in rphi w.r.t. normal vector

      // phiN alternates inward/outward
      // reduce phiinc

      if( phiinc > pihalf ) phiinc -= pi;
      else if( phiinc < -pihalf ) phiinc += pi;

      if( pt > 4 ) {
	h758->Fill( bb/aa );//lever arm
	h759->Fill( f3*wt, phiinc*wt );
	h760->Fill( dca3*1E4 );
      }

      if( pt > 12 ) {

	h770->Fill( dca3*1E4 );

	if( hp.trackerLayersWithMeasurement() > 8 ) {
	  h780->Fill( dca3*1E4 );
	}

	if( phiinc*wt > -1 && phiinc*wt < 7 ){
	  h790->Fill( dca3*1E4 );
	}

      }//pt>12

      // residual profiles: alignment check

      if( pt > 4 ) {
	h762->Fill( f3*wt, dca3*1E4 );
	h764->Fill( zTIB3, dca3*1E4 );
      }
      h766->Fill( logpt, dca3*1E4 );
      if( iTrack->charge() > 0 ) h768->Fill( logpt, dca3*1E4 );
      else h769->Fill( logpt, dca3*1E4 );

      // profile of abs(dca) gives mean abs(dca):
      // mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
      // => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
      // point resolution = 1/sqrt(3/2) * triplet middle residual width
      // => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

      if( pt > 4 ) {

	h772->Fill( f3*wt, abs(dca3)*1E4 );

	h774->Fill( zTIB3, abs(dca3)*1E4 );

	h778->Fill( dip*wt, abs(dca3)*1E4 );

	h784->Fill( phiinc*wt, abs(dca3)*1E4 );

      }//pt

      if( pt > 0.8 && pt < 1.2 ) { // low pt
	h785->Fill( f3*wt, abs(dca3)*1E4 );
      }
      h776->Fill( logpt, abs(dca3)*1E4 );

    }//TIB 2,3,4

    //------------------------------------------------------------------------
    // TOB triplet 3+5 -> 4:

    if( nTOB3*nTOB4*nTOB5 > 0 ) {

      if( jdbg ) cout << "  TOB triplet 5+3 -> 4\n";

      double f4 = atan2( yTOB4, xTOB4 );//position angle

      double ax = xTOB5 - xTOB3;
      double ay = yTOB5 - yTOB3;
      double aa = sqrt( ax*ax + ay*ay ); // from 3 to 5

      double xmid = 0.5 * ( xTOB3 + xTOB5 );
      double ymid = 0.5 * ( yTOB3 + yTOB5 );
      double bx = xTOB4 - xmid;
      double by = yTOB4 - ymid;
      double bb = sqrt( bx*bx + by*by ); // from mid point to point 4

      // Calculate the centre of the helix in xy-projection that
      // transverses the two spacepoints. The points with the same
      // distance from the two points are lying on a line.
      // LAMBDA is the distance between the point in the middle of
      // the two spacepoints and the centre of the helix.

      // we already have kap and rho = 1/kap

      double lam = sqrt( -0.25 + 
			 rho*rho / ( ( xTOB3 - xTOB5 )*( xTOB3 - xTOB5 ) + ( yTOB3 - yTOB5 )*( yTOB3 - yTOB5 ) ) );

      // There are two solutions, the sign of kap gives the information
      // which of them is correct:

      if( kap > 0 ) lam = -lam;

      // ( X0, Y0 ) is the centre of the circle
      // that describes the helix in xy-projection:

      double x0 =  0.5*( xTOB3 + xTOB5 ) + lam * ( -yTOB3 + yTOB5 );
      double y0 =  0.5*( yTOB3 + yTOB5 ) + lam * (  xTOB3 - xTOB5 );

      // To get phi0 in the right interval one must distinguish
      // two cases with positve and negative kap:

      double uphi;
      if( kap > 0 ) uphi = atan2( -x0,  y0 );
      else          uphi = atan2(  x0, -y0 );

      // The distance of the closest approach DCA depends on the sign
      // of kappa:

      double udca;
      if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
      else          udca = rho + sqrt( x0*x0 + y0*y0 );

      h801->Fill( zTOB4 );
      h802->Fill( uphi - iTrack->phi() );
      h803->Fill( udca - iTrack->d0() );

      // interpolate to hit 2:
      // cirmov
      // we already have rinv = -kap

      double cosphi = cos(uphi);
      double sinphi = sin(uphi);
      double dp = -xTOB4*sinphi + yTOB4*cosphi + udca;
      double dl = -xTOB4*cosphi - yTOB4*sinphi;
      double sa = 2*dp + rinv * ( dp*dp + dl*dl );
      double dca2 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
      double ud = 1 + rinv*udca;
      double phi2 = atan2( -rinv*xTOB4 + ud*sinphi, rinv*yTOB4 + ud*cosphi );//direction

      double phiinc = phi2 - phiNTOB4;//angle of incidence in rphi w.r.t. normal vector

      // phiN alternates inward/outward
      // reduce phiinc

      if( phiinc > pihalf ) phiinc -= pi;
      else if( phiinc < -pihalf ) phiinc += pi;

      if( pt > 4 ) {
	h808->Fill( bb/aa );//lever arm
	h809->Fill( f4*wt, phiinc*wt );
	h810->Fill( dca2*1E4 );
      }

      if( pt > 12 ) {

	h820->Fill( dca2*1E4 );

	// 2010B: sigma(resid) = 50.3 in TOB4, 39.0 in TOB5
	// => sigma(TOB4) = 43.0
	// => sigma(TOB4) = 29.1

	if( hp.trackerLayersWithMeasurement() > 8 ) {
	  h830->Fill( dca2*1E4 );
	}

	if( phiinc*wt > -1 && phiinc*wt < 7 ){
	  h840->Fill( dca2*1E4 );
	}

      }//pt>12

      // residual profiles: alignment check

      if( pt > 4 ) {
	h812->Fill( f4*wt, dca2*1E4 );
	h814->Fill( zTOB4, dca2*1E4 );
      }
      h816->Fill( logpt, dca2*1E4 );
      if( iTrack->charge() > 0 ) h818->Fill( logpt, dca2*1E4 );
      else h819->Fill( logpt, dca2*1E4 );

      // profile of abs(dca) gives mean abs(dca):
      // mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
      // => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
      // point resolution = 1/sqrt(3/2) * triplet middle residual width
      // => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

      if( pt > 4 ) {

	h822->Fill( f4*wt, abs(dca2)*1E4 );

	h824->Fill( zTOB4, abs(dca2)*1E4 );

	h828->Fill( dip*wt, abs(dca2)*1E4 );

	h834->Fill( phiinc*wt, abs(dca2)*1E4 );

      }//pt

      if( pt > 0.8 && pt < 1.2 ) { // low pt
	h835->Fill( f4*wt, abs(dca2)*1E4 );
      }
      h826->Fill( logpt, abs(dca2)*1E4 );

    }//TOB 1,2,3

    //------------------------------------------------------------------------
    // TOB triplet 4+6 -> 5:

    if( nTOB4*nTOB5*nTOB6 > 0 ) {

      if( jdbg ) cout << "  TOB triplet 6+4 -> 5\n";

      double f5 = atan2( yTOB5, xTOB5 );//position angle

      double ax = xTOB6 - xTOB4;
      double ay = yTOB6 - yTOB4;
      double aa = sqrt( ax*ax + ay*ay ); // from 4 to 6

      double xmid = 0.5 * ( xTOB4 + xTOB6 );
      double ymid = 0.5 * ( yTOB4 + yTOB6 );
      double bx = xTOB5 - xmid;
      double by = yTOB5 - ymid;
      double bb = sqrt( bx*bx + by*by ); // from mid point to point 5

      // Calculate the centre of the helix in xy-projection that
      // transverses the two spacepoints. The points with the same
      // distance from the two points are lying on a line.
      // LAMBDA is the distance between the point in the middle of
      // the two spacepoints and the centre of the helix.

      // we already have kap and rho = 1/kap

      double lam = sqrt( -0.25 + 
			 rho*rho / ( ( xTOB4 - xTOB6 )*( xTOB4 - xTOB6 ) + ( yTOB4 - yTOB6 )*( yTOB4 - yTOB6 ) ) );

      // There are two solutions, the sign of kap gives the information
      // which of them is correct:

      if( kap > 0 ) lam = -lam;

      // ( X0, Y0 ) is the centre of the circle
      // that describes the helix in xy-projection:

      double x0 =  0.5*( xTOB4 + xTOB6 ) + lam * ( -yTOB4 + yTOB6 );
      double y0 =  0.5*( yTOB4 + yTOB6 ) + lam * (  xTOB4 - xTOB6 );

      // To get phi0 in the right interval one must distinguish
      // two cases with positve and negative kap:

      double uphi;
      if( kap > 0 ) uphi = atan2( -x0,  y0 );
      else          uphi = atan2(  x0, -y0 );

      // The distance of the closest approach DCA depends on the sign
      // of kappa:

      double udca;
      if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
      else          udca = rho + sqrt( x0*x0 + y0*y0 );

      h851->Fill( zTOB4 );
      h852->Fill( uphi - iTrack->phi() );
      h853->Fill( udca - iTrack->d0() );

      // interpolate to hit 2:
      // cirmov
      // we already have rinv = -kap

      double cosphi = cos(uphi);
      double sinphi = sin(uphi);
      double dp = -xTOB5*sinphi + yTOB5*cosphi + udca;
      double dl = -xTOB5*cosphi - yTOB5*sinphi;
      double sa = 2*dp + rinv * ( dp*dp + dl*dl );
      double dca3 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
      double ud = 1 + rinv*udca;
      double phi3 = atan2( -rinv*xTOB5 + ud*sinphi, rinv*yTOB5 + ud*cosphi );//direction

      double phiinc = phi3 - phiNTOB5;//angle of incidence in rphi w.r.t. normal vector

      // phiN alternates inward/outward
      // reduce phiinc:

      if( phiinc > pihalf ) phiinc -= pi;
      else if( phiinc < -pihalf ) phiinc += pi;

      if( pt > 4 ) {
	h858->Fill( bb/aa );//lever arm
	h859->Fill( f5*wt, phiinc*wt );
	h860->Fill( dca3*1E4 );
      }

      if( pt > 12 ) {

	h870->Fill( dca3*1E4 );

	if( hp.trackerLayersWithMeasurement() > 8 ) {
	  h880->Fill( dca3*1E4 );
	}

	if( phiinc*wt > -1 && phiinc*wt < 7 ){
	  h890->Fill( dca3*1E4 );
	}

      }//pt>12

      // residual profiles: alignment check

      if( pt > 4 ) {
	h862->Fill( f5*wt, dca3*1E4 );
	h864->Fill( zTOB5, dca3*1E4 );
      }
      h866->Fill( logpt, dca3*1E4 );
      if( iTrack->charge() > 0 ) h868->Fill( logpt, dca3*1E4 );
      else h869->Fill( logpt, dca3*1E4 );

      // profile of abs(dca) gives mean abs(dca):
      // mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
      // => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
      // point resolution = 1/sqrt(3/2) * triplet middle residual width
      // => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

      if( pt > 4 ) {

	h872->Fill( f5*wt, abs(dca3)*1E4 );

	h874->Fill( zTOB5, abs(dca3)*1E4 );

	h878->Fill( dip*wt, abs(dca3)*1E4 );

	h884->Fill( phiinc*wt, abs(dca3)*1E4 );

      }//pt

      if( pt > 0.8 && pt < 1.2 ) { // low pt
	h885->Fill( f5*wt, abs(dca3)*1E4 );
      }
      h876->Fill( logpt, abs(dca3)*1E4 );

    }//TOB 2,3,4

    //------------------------------------------------------------------------
    // TEC: 2 sides, 9 wheels

    for( int is = 1; is < 3; ++is ){

      for( int i2 = 2; i2 < 9; ++i2 ){//middle plane = wheels 2..8

	if( nTEC[is][i2] == 0 ) continue;

	int i1 = i2 - 1;
	if( nTEC[is][i1] == 0 ) continue;

	int i3 = i2 + 1;
	if( nTEC[is][i3] == 0 ) continue;

	if( jdbg ) cout << "  TEC side " << is << " triplet " << i2 << endl;

	double x1 = xTEC[is][i1];
	double y1 = yTEC[is][i1];
	double z1 = zTEC[is][i1];
	//DetId detId1 = detTEC[is][i1];

	double x2 = xTEC[is][i2];
	double y2 = yTEC[is][i2];
	double z2 = zTEC[is][i2];
	DetId detId2 = detTEC[is][i2];

	const GeomDet * myGeomDet = pTG->idToDet( detId2 );
	Surface::GlobalPoint gp = Surface::GlobalPoint(x2,y2,z2);
	Surface::LocalPoint lp = myGeomDet->toLocal( gp );

	const GeomDetUnit * myGeomDetUnit = pTG->idToDetUnit( detId2 );
	StripTopology & stripTopol = (StripTopology&) myGeomDetUnit->topology();
	double xptch = stripTopol.localPitch( lp );

	double x3 = xTEC[is][i3];
	double y3 = yTEC[is][i3];
	double z3 = zTEC[is][i3];
	//DetId detId3 = detTEC[is][i3];

	double f2 = atan2( y2, x2 );//position angle
	double R2 = sqrt( x2*x2 + y2*y2 );

	double ax = x3 - x1;
	double ay = y3 - y1;
	double aa = sqrt( ax*ax + ay*ay ); // from 1 to 3
	double az = z3 - z1;

	double xmid = 0.5 * ( x1 + x3 );
	double ymid = 0.5 * ( y1 + y3 );
	double bx = x2 - xmid;
	double by = y2 - ymid;
	double bb = sqrt( bx*bx + by*by ); // from mid point to point 2

	// Calculate the centre of the helix in xy-projection that
	// transverses the two spacepoints. The points with the same
	// distance from the two points are lying on a line.
	// LAMBDA is the distance between the point in the middle of
	// the two spacepoints and the centre of the helix.

	// we already have kap and rho = 1/kap

	double lam = sqrt( -0.25 + 
			   rho*rho / ( ( x1 - x3 )*( x1 - x3 ) + ( y1 - y3 )*( y1 - y3 ) ) );

	// There are two solutions, the sign of kap gives the information
	// which of them is correct:

	if( kap > 0 ) lam = -lam;

	// ( X0, Y0 ) is the centre of the circle
	// that describes the helix in xy-projection:

	double x0 =  0.5*( x1 + x3 ) + lam * ( -y1 + y3 );
	double y0 =  0.5*( y1 + y3 ) + lam * (  x1 - x3 );

	// To get phi0 in the right interval one must distinguish
	// two cases with positve and negative kap:

	double uphi;
	if( kap > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );

	// The distance of the closest approach DCA depends on the sign
	// of kappa:

	double udca;
	if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
	else          udca = rho + sqrt( x0*x0 + y0*y0 );

	h901->Fill( R2 );
	h902->Fill( uphi - iTrack->phi() );
	h903->Fill( udca - iTrack->d0() );
	h904->Fill( xptch*1E4 );
	h905->Fill( R2, xptch*1E4 );
	h906->Fill( z2 );

	// interpolate to hit 2:
	// cirmov
	// we already have rinv = -kap

	double cosphi = cos(uphi);
	double sinphi = sin(uphi);
	double dp = -x2*sinphi + y2*cosphi + udca;
	double dl = -x2*cosphi - y2*sinphi;
	double sa = 2*dp + rinv * ( dp*dp + dl*dl );
	double dca2 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
	//double ud = 1 + rinv*udca;
	//double phi2 = atan2( -rinv*x2 + ud*sinphi, rinv*y2 + ud*cosphi );//direction

	if( pt > 4 ) {

	  h908->Fill( bb/aa );//lever arm
	  h909->Fill( az );//base length in z
	  h910->Fill( dca2*1E4 );

	}//pt

	if( pt > 12 ) {

	  h920->Fill( dca2*1E4 );

	  if( hp.trackerLayersWithMeasurement() > 8 ) {
	    h921->Fill( dca2*1E4 );
	  }

	}//pt>12

	if( pp > 12 ) {

	  h930->Fill( dca2*1E4 );

	  if(      xptch < 0.0091 ) 
	    h931->Fill( dca2*1E4 );
	  else if( xptch < 0.0102 ) 
	    h932->Fill( dca2*1E4 );
	  else if( xptch < 0.0113 ) 
	    h933->Fill( dca2*1E4 );
	  else if( xptch < 0.0123 ) 
	    h934->Fill( dca2*1E4 );
	  else if( xptch < 0.0133 ) 
	    h935->Fill( dca2*1E4 );
	  else if( xptch < 0.0144 ) 
	    h936->Fill( dca2*1E4 );
	  else if( xptch < 0.0158 ) 
	    h937->Fill( dca2*1E4 );
	  else if( xptch < 0.0184 ) 
	    h938->Fill( dca2*1E4 );
	  else
	    h939->Fill( dca2*1E4 );
	}// p > 12

	// residual profiles: alignment check

	if( pt > 4 ) {
	  h912->Fill( f2*wt, dca2*1E4 );
	  h914->Fill( R2, dca2*1E4 );
	}
	h916->Fill( logpt, dca2*1E4 );
	if( iTrack->charge() > 0 ) h918->Fill( logpt, dca2*1E4 );
	else h919->Fill( logpt, dca2*1E4 );

	// profile of abs(dca) gives mean abs(dca):
	// mean of abs(Gauss) = 0.7979 * RMS = 1/sqrt(pi/2) 
	// => rms = sqrt(pi/2) * mean of abs (sqrt(pi/2) = 1.2533)
	// point resolution = 1/sqrt(3/2) * triplet middle residual width
	// => sqrt(pi/2)*sqrt(2/3) = sqrt(pi/3) = 1.0233, almost one

	if( pt > 4 ) {

	  h922->Fill( f2*wt, abs(dca2)*1E4 );

	  h924->Fill( R2, abs(dca2)*1E4 );
	  h925->Fill( xptch*1E4, abs(dca2)*1E4 );

	  h928->Fill( dip*wt, abs(dca2)*1E4 );

	}//pt

	if( pt > 0.8 && pt < 1.2 ) { // low pt
	  h929->Fill( f2*wt, abs(dca2)*1E4 );
	}
	h926->Fill( logpt, abs(dca2)*1E4 );

      }//TEC wheels

    }//TEC sides

    //------------------------------------------------------------------------
    // unbiased residuals:

    if( pt > 4 ) {

      // loop over hits on this track:

      for( vector<TransientTrackingRecHit::RecHitPointer>::iterator jTTRH = myTTRHvec.begin();
	   jTTRH != myTTRHvec.end(); ++jTTRH ) {

	if( ! (*jTTRH)->hit()->isValid() ) continue;

	Surface::GlobalPoint gp = (*jTTRH)->globalPosition();

	Trajectory::RecHitContainer nyTTRHvec; // for fit

	for( vector<TransientTrackingRecHit::RecHitPointer>::iterator iTTRH = myTTRHvec.begin();
	     iTTRH != myTTRHvec.end(); ++iTTRH ) {

	  if( iTTRH == jTTRH ) continue;//skip hit i

	  nyTTRHvec.push_back( *iTTRH );//copy all others

	}//copy

	// re-fit without hit i:

	std::vector<Trajectory> refitTrajectoryVec2 = theFitter->fit( seed, nyTTRHvec, initialTSOS );

	if( refitTrajectoryVec2.size() > 0 ) { // should be either 0 or 1            

	  const Trajectory& refitTrajectory2 = refitTrajectoryVec2.front();

	  // Trajectory.measurements:

	  const TrajectoryMeasurement closestTM = refitTrajectory2.closestMeasurement( gp );

	  TrajectoryStateOnSurface closestTSOS = closestTM.updatedState();

	  TrajectoryStateOnSurface propTSOS = thePropagator->propagate( closestTSOS, (*jTTRH)->det()->surface() );

	  if( propTSOS.isValid() ){

	    const Topology* theTopology = &( (*jTTRH)->detUnit()->topology() );

	    // MeasurementPoint [pitch] (like channel number)

	    MeasurementPoint hitMeasurement = theTopology->measurementPosition( (*jTTRH)->localPosition() );

	    // TID and TEC have trapezoidal detectors:
	    // translation from channel number into local x depends on local y
	    // track prediction has local x,y => can convert into proper channel number MeasurementPoint:

	    MeasurementPoint predictedPosition = theTopology->measurementPosition( propTSOS.localPosition() );

	    double resid = hitMeasurement.x() - predictedPosition.x();//[pitch]

	    DetId detId = (*jTTRH)->hit()->geographicalId();
	    uint32_t subDet = detId.subdetId();

	    double xptch;
	    //double yptch;

	    if( subDet <  3 ){//1,2=pixel
	      PixelTopology & pixelTopol = (PixelTopology&) (*jTTRH)->detUnit()->topology();
	      xptch = pixelTopol.pitch().first;
	      //yptch = pixelTopol.pitch().second;
	    }
	    else {//strip
	      StripTopology & stripTopol = (StripTopology&) (*jTTRH)->detUnit()->topology();
	      xptch = stripTopol.localPitch( propTSOS.localPosition() );
	      //yptch = stripTopol.localStripLength( propTSOS.localPosition() );
	    }

	    resid = resid * xptch;//[cm]

	    if( subDet == PixelSubdetector::PixelBarrel ) {

	      if( PXBDetId( detId ).layer() == 1 ) {
		u110->Fill( resid*1E4 );
	      }//layer

	      if( PXBDetId( detId ).layer() == 2 ) {
		u120->Fill( resid*1E4 );
	      }//layer

	      if( PXBDetId( detId ).layer() == 3 ) {
		u130->Fill( resid*1E4 );
	      }//layer

	    }//PXB

	    if( subDet == StripSubdetector::TIB ) {

	      if( TIBDetId( detId ).layer() == 1 && TIBDetId(detId).isRPhi() ) {
		u310->Fill( resid*1E4 );
	      }//layer

	      if( TIBDetId( detId ).layer() == 1 && TIBDetId(detId).isStereo() ) {
		u320->Fill( resid*1E4 );
	      }//layer

	      if( TIBDetId( detId ).layer() == 2 && TIBDetId(detId).isRPhi() ) {
		u330->Fill( resid*1E4 );
	      }//layer

	      if( TIBDetId( detId ).layer() == 2 && TIBDetId(detId).isStereo() ) {
		u340->Fill( resid*1E4 );
	      }//layer

	      if( TIBDetId( detId ).layer() == 3 ) {
		u350->Fill( resid*1E4 );
	      }//layer

	      if( TIBDetId( detId ).layer() == 4 ) {
		u360->Fill( resid*1E4 );
	      }//layer

	    }//TIB

	    if( subDet == StripSubdetector::TID ) {

	      if( TIDDetId(detId).side() == 1 ){

		if( TIDDetId(detId).diskNumber() == 1 ){
		  if( TIDDetId(detId).isRPhi() ) {
		    u410->Fill( resid*1E4 );
		  }
		  else {
		    u411->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).diskNumber() == 2 ){
		  if( TIDDetId(detId).isRPhi() ) {
		    u412->Fill( resid*1E4 );
		  }
		  else {
		    u413->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).diskNumber() == 3 ){
		  if( TIDDetId(detId).isRPhi() ) {
		    u414->Fill( resid*1E4 );
		  }
		  else {
		    u415->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).ringNumber() == 1 ) {
		  if( TIDDetId(detId).isRPhi() ) {
		    u430->Fill( resid*1E4 );
		  }
		  else {
		    u431->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).ringNumber() == 2 ) {
		  if( TIDDetId(detId).isRPhi() ) {
		    u432->Fill( resid*1E4 );
		  }
		  else {
		    u433->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).ringNumber() == 3 ) {
		  if( TIDDetId(detId).isRPhi() ) {
		    u434->Fill( resid*1E4 );
		  }
		  else {
		    u435->Fill( resid*1E4 );
		  }
		}

	      }//z side 1
	      else { // other z side

		if( TIDDetId(detId).diskNumber() == 1 ){
		  if( TIDDetId(detId).isRPhi() ) {
		    u420->Fill( resid*1E4 );
		  }
		  else {
		    u421->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).diskNumber() == 2 ){
		  if( TIDDetId(detId).isRPhi() ) {
		    u422->Fill( resid*1E4 );
		  }
		  else {
		    u423->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).diskNumber() == 3 ){
		  if( TIDDetId(detId).isRPhi() ) {
		    u424->Fill( resid*1E4 );
		  }
		  else {
		    u425->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).ringNumber() == 1 ) {
		  if( TIDDetId(detId).isRPhi() ) {
		    u440->Fill( resid*1E4 );
		  }
		  else {
		    u441->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).ringNumber() == 2 ) {
		  if( TIDDetId(detId).isRPhi() ) {
		    u442->Fill( resid*1E4 );
		  }
		  else {
		    u443->Fill( resid*1E4 );
		  }
		}

		if( TIDDetId(detId).ringNumber() == 3 ) {
		  if( TIDDetId(detId).isRPhi() ) {
		    u444->Fill( resid*1E4 );
		  }
		  else {
		    u445->Fill( resid*1E4 );
		  }
		}
	      }// z side
	    }//TID

	    if( subDet == StripSubdetector::TOB ) {

	      if( TOBDetId( detId ).layer() == 1 && TOBDetId(detId).isRPhi() ) {
		u510->Fill( resid*1E4 );
	      }//layer

	      if( TOBDetId( detId ).layer() == 1 && TOBDetId(detId).isStereo() ) {
		u520->Fill( resid*1E4 );
	      }//layer

	      if( TOBDetId( detId ).layer() == 2 && TOBDetId(detId).isRPhi() ) {
		u530->Fill( resid*1E4 );
	      }//layer

	      if( TOBDetId( detId ).layer() == 2 && TOBDetId(detId).isStereo() ) {
		u540->Fill( resid*1E4 );
	      }//layer

	      if( TOBDetId( detId ).layer() == 3 ) {
		u550->Fill( resid*1E4 );
	      }//layer

	      if( TOBDetId( detId ).layer() == 4 ) {
		u560->Fill( resid*1E4 );
	      }//layer

	      if( TOBDetId( detId ).layer() == 5 ) {
		u570->Fill( resid*1E4 );
	      }//layer

	      if( TOBDetId( detId ).layer() == 6 ) {
		u580->Fill( resid*1E4 );
	      }//layer

	    }//TOB

	    if( subDet == StripSubdetector::TEC ) {

	      if( TECDetId(detId).side() == 1 ){

		if( TECDetId(detId).wheelNumber() == 1 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u610->Fill( resid*1E4 );
		  }
		  else {
		    u611->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 2 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u612->Fill( resid*1E4 );
		  }
		  else {
		    u613->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 3 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u614->Fill( resid*1E4 );
		  }
		  else {
		    u615->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 4 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u616->Fill( resid*1E4 );
		  }
		  else {
		    u617->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 5 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u618->Fill( resid*1E4 );
		  }
		  else {
		    u619->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 6 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u620->Fill( resid*1E4 );
		  }
		  else {
		    u621->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 7 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u622->Fill( resid*1E4 );
		  }
		  else {
		    u623->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 8 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u624->Fill( resid*1E4 );
		  }
		  else {
		    u625->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 9 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u626->Fill( resid*1E4 );
		  }
		  else {
		    u627->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 1 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u630->Fill( resid*1E4 );
		  }
		  else {
		    u631->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 2 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u632->Fill( resid*1E4 );
		  }
		  else {
		    u633->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 3 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u634->Fill( resid*1E4 );
		  }
		  else {
		    u635->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 4 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u636->Fill( resid*1E4 );
		  }
		  else {
		    u637->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 5 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u638->Fill( resid*1E4 );
		  }
		  else {
		    u639->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 6 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u640->Fill( resid*1E4 );
		  }
		  else {
		    u641->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 7 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u642->Fill( resid*1E4 );
		  }
		  else {
		    u643->Fill( resid*1E4 );
		  }
		}

	      } // z side 1

	      else { // other z side

		if( TECDetId(detId).wheelNumber() == 1 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u650->Fill( resid*1E4 );
		  }
		  else {
		    u651->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 2 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u652->Fill( resid*1E4 );
		  }
		  else {
		    u653->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 3 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u654->Fill( resid*1E4 );
		  }
		  else {
		    u655->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 4 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u656->Fill( resid*1E4 );
		  }
		  else {
		    u657->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 5 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u658->Fill( resid*1E4 );
		  }
		  else {
		    u659->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 6 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u660->Fill( resid*1E4 );
		  }
		  else {
		    u661->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 7 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u662->Fill( resid*1E4 );
		  }
		  else {
		    u663->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 8 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u664->Fill( resid*1E4 );
		  }
		  else {
		    u665->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).wheelNumber() == 9 ){
		  if( TECDetId(detId).isRPhi() ) {
		    u666->Fill( resid*1E4 );
		  }
		  else {
		    u667->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 1 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u670->Fill( resid*1E4 );
		  }
		  else {
		    u671->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 2 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u672->Fill( resid*1E4 );
		  }
		  else {
		    u673->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 3 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u674->Fill( resid*1E4 );
		  }
		  else {
		    u675->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 4 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u676->Fill( resid*1E4 );
		  }
		  else {
		    u677->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 5 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u678->Fill( resid*1E4 );
		  }
		  else {
		    u679->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 6 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u680->Fill( resid*1E4 );
		  }
		  else {
		    u681->Fill( resid*1E4 );
		  }
		}

		if( TECDetId(detId).ringNumber() == 7 ) {
		  if( TECDetId(detId).isRPhi() ) {
		    u682->Fill( resid*1E4 );
		  }
		  else {
		    u683->Fill( resid*1E4 );
		  }
		}

	      }//side

	    }//TEC

	  }//valid propTSOS

	}//refit2 OK

      }//loop hit i left out in refit for unbiased residual

    }//pt cut

  }// loop over tracks

  h028->Fill( sumpt );
  h029->Fill( sumq );

}//event
//----------------------------------------------------------------------
// method called just after ending the event loop:
//
void Pxl::endJob() {

  std::cout << "end of job after " << myCounters::neve << " events.\n";

}

//define this as a plug-in
DEFINE_FWK_MODULE(Pxl);
