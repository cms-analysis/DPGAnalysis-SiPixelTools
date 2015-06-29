//
//  SimpleHelix.h (v3.0)
//
//  A simple helix class with a two-point+beam axis or three-point constructor
//
//  9 Feb 2008 - allow one of the three points to have a 2-D axial strip hit (x and y coordinates only).
//  2 May 2015 - add method to calculate the intersecton with an arbitrary plane
//  6 May 2015 - add two point+radius constructor and parameter getter
//
// Created by Morris Swartz on 7/22/07.
// Copyright 2007 __TheJohnsHopkinsUniversity__. All rights reserved.
//
//
 
#ifndef SimpleHelix_h
#define SimpleHelix_h 1

#include<vector>
#include<math.h>
#include<assert.h>
// ******************************************************************************************
//! \class SimpleHelix 
//!
//!  
//!  Creates helixes from 3 space points or 2 space points + beam axis constraint, or
//!  2 space points and a radius.
//!  If one of the 3 input points has only 2 components, use that (x,y) line in z as a 
//!  constraint (allows 2 pixel hits and 1 axial strip hit to define the helix).
//!  Methods to evaluate the track direction at each constructor point, to calculate the
//!  coordinates and track direction at the point of closest approach to the beam axis,
//!  and to extrapolate the helical track (and direction) to fixed planes and cylinders
//!  are implemented.
//! 
// ******************************************************************************************
class SimpleHelix {

 public:
 
    SimpleHelix() {radius_ = 100.; xcenter_ = -100.; ycenter_ = 0.; dzdphi_ = 0.; z0_ = 0.;} //!< Default constructor
  
    SimpleHelix(double radius, double xcenter, double ycenter, double dzdphi, double z0); //!< Simple constructor
  
    SimpleHelix(std::vector<double> p1, std::vector<double> p2); //!< Constructor for two points plus beam axis (0,0)
  
    SimpleHelix(std::vector<double> p1, std::vector<double> p2, std::vector<double> p3); //!< Constructor for three points 
   
    SimpleHelix(std::vector<double> p1, std::vector<double> p2, double radius, int *Pass); //!< Constructor for two points and radius
  
    ~SimpleHelix();  //!< Destructor
   
    void parameters(double &radius, double &xcenter, double &ycenter, double &dzdphi, double &z0); //!< Simple Getter
  
    void directions(std::vector<double> &d1, std::vector<double> &d2, std::vector<double> &d3); //! return track direction cosines at the input points (3-vectors)
  
    void poca(std::vector<double> &pd);  //! return coordinates and track direction at the point of closest approach to the beam axis (6-vector)
  
    int zposdir(double zex, std::vector<double> &pd);  //! extrapolate the helix to position z=zex, return coordinates and directions (6-vector)
  
    bool rposdir(double rex, std::vector<double> &pd);  //! extrapolate the helix to radius r=rex, return coordinates and directions (6-vector)
   
    int pposdir(std::vector<double> &pp, std::vector<double> &pd);  //! extrapolate the helix to plane defined by 6-vector pp, return coordinates and directions (6-vector)
  
 private:
  
  // Basic Helix Parameters	
  
    mutable double radius_;      //!< radius of helix
    mutable double xcenter_;     //!< center of helical axis
    mutable double ycenter_;     //!< center of helical axis
    mutable double dzdphi_;      //!< change in z with phi
    mutable double z0_;          //!< z of the helix at phi = 0
    mutable double dsdphi_;      //!< signed change in arc length with phi for outwardly propagating track
    mutable double phi1_;        //!< phi (about helix axis ... not global phi) corresponding to input point 1
    mutable double phi2_;        //!< phi (about helix axis ... not global phi) corresponding to input point 2
    mutable double phi3_;        //!< phi (about helix axis ... not global phi) corresponding to input point 3
    mutable double phi_poca_;    //!< phi (about helix axis ... not global phi) corresponding to closest approach to the beam axis
} ;


#endif
