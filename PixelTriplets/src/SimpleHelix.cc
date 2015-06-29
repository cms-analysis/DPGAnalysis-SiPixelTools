//
//  SimpleHelix.cc  Version 3.0
//
//  A simple helix class with a two-point+beam axis or three-point constructor
//  9 Feb 2008 - allow one of the three points to have a 2-D axial strip hit (x and y coordinates only).
//  2 May 2015 - add method to calculate the intersecton with an arbitrary plane
//  6 May 2015 - add two point+radius constructor, and parameter getter
//
//  six-vectors are 3 positions [x,y,z] followed by 3 direction cosines [cx,cy,cz]
//
// Created by Morris Swartz on 7/22/07.
// Copyright 2007 __TheJohnsHopkinsUniversity__. All rights reserved.
//
//  9 Feb 2008 - allow one of the three points to have a 2-D axial strip hit (x and y coordinates only).
//
//

#include <math.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include "SimpleHelix.h"

// ******************************************************************
//! Simple Constructor
// ******************************************************************

SimpleHelix::SimpleHelix(double radius, double xcenter, double ycenter, double dzdphi, double z0) {
	
	 radius_ = radius; xcenter_ = xcenter; ycenter_ = ycenter; dzdphi_ = dzdphi; z0_ = z0;
	 phi1_ = 0.; phi2_ = 0.; phi3_ = 0.; phi_poca_ = 0.;
	 
   }


// ******************************************************************
//! Constructor for two input points and the beam axis (x,y) = (0,0)
// ******************************************************************

SimpleHelix::SimpleHelix(std::vector<double> p1, std::vector<double> p2) {

   const double pi= 3.14159265358979;  const double twopi= 6.28318530717958; 
		 
// Check sizes of input vectors
    
   assert(p1.size() == 3 && p2.size() == 3);
   
// Calculate the center and radius of the x-y circle   
   
   double x1 = p1[0]; double x2 = p2[0]; double x3 = 0.;
   double y1 = p1[1]; double y2 = p2[1]; double y3 = 0.;
   
   double deny = y1*(x3-x2)+y2*(x1-x3)+y3*(x2-x1);   
   assert(fabs(deny) > 1.e-15);
   ycenter_ = 0.5*(x1*(y2*y2-y3*y3+x2*x2-x3*x3)+x2*(y3*y3-y1*y1+x3*x3-x1*x1)+x3*(y1*y1-y2*y2+x1*x1-x2*x2))/deny;
   
   double denx = x1*(y3-y2)+x2*(y1-y3)+x3*(y2-y1);   
   assert(fabs(denx) > 1.e-15);
   xcenter_ = 0.5*(y1*(y2*y2-y3*y3+x2*x2-x3*x3)+y2*(y3*y3-y1*y1+x3*x3-x1*x1)+y3*(y1*y1-y2*y2+x1*x1-x2*x2))/denx;

   radius_ = sqrt((x1-xcenter_)*(x1-xcenter_)+(y1-ycenter_)*(y1-ycenter_));
   
// Now calculate the azimuthal angles of the input points and the closest approach to the beam axis

   phi_poca_ = atan2(-ycenter_, -xcenter_);   

   phi1_ = atan2(y1-ycenter_, x1-xcenter_);      
   if(phi1_-phi_poca_> pi) { phi1_ -= twopi;} if(phi1_-phi_poca_< -pi) { phi1_ += twopi;}

   phi2_ = atan2(y2-ycenter_, x2-xcenter_);      
   if(phi2_-phi_poca_> pi) { phi2_ -= twopi;} if(phi2_-phi_poca_< -pi) { phi2_ += twopi;}

   phi3_ = atan2(y3-ycenter_, x3-xcenter_);   
   if(phi3_-phi_poca_> pi) { phi3_ -= twopi;} if(phi3_-phi_poca_< -pi) { phi3_ += twopi;}
    
// Finally, calculate the z - phi parameters
   
   double z1 = p1[2]; double z2 = p2[2];
   
   if(fabs(phi2_ - phi1_) > 1.e-15) {
     
      dzdphi_ = (z2 - z1)/(phi2_ - phi1_);
	  z0_ = z1 - phi1_ * dzdphi_;
   } else {
   
      dzdphi_ = 0.;
	  z0_ = 0.;
   }
   
   double r1 = sqrt(x1*x1+y1*y1);  double r2 = sqrt(x2*x2+y2*y2);
   if(r2 > r1) {
      if(phi2_ > phi1_) {dsdphi_ = sqrt(dzdphi_*dzdphi_ + radius_*radius_);} else {dsdphi_ = -sqrt(dzdphi_*dzdphi_ + radius_*radius_);}
   } else {
      if(phi2_ > phi1_) {dsdphi_ = -sqrt(dzdphi_*dzdphi_ + radius_*radius_);} else {dsdphi_ = sqrt(dzdphi_*dzdphi_ + radius_*radius_);}
   }  
}


// ***********************************************************************************************
//! Constructor for thee input points
//! If one of the 3 input points has only 2 components, use that (x,y) line in z as a constraint
// ***********************************************************************************************

SimpleHelix::SimpleHelix(std::vector<double> p1, std::vector<double> p2, std::vector<double> p3) {

   const double pi= 3.14159265358979;  const double twopi= 6.28318530717958; 
	 
// Check sizes of input vectors

   int n3p = 0; int n2p = 0; 
   std::vector<double> *p3p1 = 0, *p3p2 = 0; 
   double *phip1=0, *phip2=0;
   
   if(p1.size() == 3) {
      ++n3p; p3p1 = &p1; phip1 = &phi1_;
   } else {
      if(p1.size() == 2) {++n2p;}
   }
       
   if(p2.size() == 3) {
      ++n3p; if(n3p == 1) {p3p1 = &p2; phip1 = &phi2_;} else {p3p2 = &p2; phip2 = &phi2_;}
   } else {
      if(p2.size() == 2) {++n2p;}
   }
       
   if(p3.size() == 3) {
      ++n3p; if(n3p == 2) {p3p2 = &p3; phip2 = &phi3_;}
   } else {
      if(p3.size() == 2) {++n2p;}
   }
   
   assert(n3p == 3 || (n3p == 2 && n2p == 1));
   
// Calculate the center and radius of the x-y circle   
   
   double x1 = p1[0]; double x2 = p2[0]; double x3 = p3[0];
   double y1 = p1[1]; double y2 = p2[1]; double y3 = p3[1];
   
   double deny = y1*(x3-x2)+y2*(x1-x3)+y3*(x2-x1);   
   assert(fabs(deny) > 1.e-15);
   ycenter_ = 0.5*(x1*(y2*y2-y3*y3+x2*x2-x3*x3)+x2*(y3*y3-y1*y1+x3*x3-x1*x1)+x3*(y1*y1-y2*y2+x1*x1-x2*x2))/deny;
   
   double denx = x1*(y3-y2)+x2*(y1-y3)+x3*(y2-y1);   
   assert(fabs(denx) > 1.e-15);
   xcenter_ = 0.5*(y1*(y2*y2-y3*y3+x2*x2-x3*x3)+y2*(y3*y3-y1*y1+x3*x3-x1*x1)+y3*(y1*y1-y2*y2+x1*x1-x2*x2))/denx;

   radius_ = sqrt((x1-xcenter_)*(x1-xcenter_)+(y1-ycenter_)*(y1-ycenter_));
   
// Now calculate the azimuthal angles of the input points and the closest approach to the beam axis

   phi_poca_ = atan2(-ycenter_, -xcenter_);   

   phi1_ = atan2(y1-ycenter_, x1-xcenter_);      
   if(phi1_-phi_poca_> pi) { phi1_ -= twopi;} if(phi1_-phi_poca_< -pi) { phi1_ += twopi;}

   phi2_ = atan2(y2-ycenter_, x2-xcenter_);      
   if(phi2_-phi_poca_> pi) { phi2_ -= twopi;} if(phi2_-phi_poca_< -pi) { phi2_ += twopi;}

   phi3_ = atan2(y3-ycenter_, x3-xcenter_);   
   if(phi3_-phi_poca_> pi) { phi3_ -= twopi;} if(phi3_-phi_poca_< -pi) { phi3_ += twopi;}
   
// Finally, calculate the z - phi parameters
   
   if(n3p == 3) {
   
// If 3 3-d points, fit for best z-parameters 
   
      double z1 = p1[2]; double z2 = p2[2]; double z3 = p3[2];
   
      double sumz = z1 + z2 + z3;
      double sumphi = phi1_ + phi2_ +phi3_;
      double sumphi2 = phi1_*phi1_ + phi2_*phi2_ +phi3_*phi3_;
      double sumphiz = phi1_*z1 + phi2_*z2 +phi3_*z3;
      double denz = 3.*sumphi2 - sumphi*sumphi;
      if(fabs(denz) > 1.e-15) {
     
         dzdphi_ = (3.*sumphiz - sumphi * sumz)/denz;
	     z0_ = (sumphi2*sumz - sumphi*sumphiz)/denz;
      } else {
   
         dzdphi_ = 0.;
	     z0_ = 0.;
      }
   
   } else {
   
// If only 2 3-d points, connect the z-dots
     if( p3p1->size()<3) std::cout<<" Error: p3p1 too small "<<std::endl; 
     if( p3p2->size()<3) std::cout<<" Error: p3p2 too small "<<std::endl; 
     double z1 = (*p3p1)[2]; double z2 = (*p3p2)[2];
   
      if(fabs((*phip2) - (*phip1)) > 1.e-15) {
     
         dzdphi_ = (z2 - z1)/((*phip2) - (*phip1));
	     z0_ = z1 - (*phip1) * dzdphi_;
      } else {
   
         dzdphi_ = 0.;
	     z0_ = 0.;
      }
	  
   }

   double r1 = sqrt(x1*x1+y1*y1);  double r2 = sqrt(x2*x2+y2*y2);
   if(r2 > r1) {
      if(phi2_ > phi1_) {dsdphi_ = sqrt(dzdphi_*dzdphi_ + radius_*radius_);} else {dsdphi_ = -sqrt(dzdphi_*dzdphi_ + radius_*radius_);}
   } else {
      if(phi2_ > phi1_) {dsdphi_ = -sqrt(dzdphi_*dzdphi_ + radius_*radius_);} else {dsdphi_ = sqrt(dzdphi_*dzdphi_ + radius_*radius_);}
   }
}

// ******************************************************************
//! Constructor for two input points and the helix radius
// ******************************************************************

SimpleHelix::SimpleHelix(std::vector<double> p1, std::vector<double> p2, double radius, int *Pass) {
   
   const double pi= 3.14159265358979;  const double twopi= 6.28318530717958;
   
   double xc1, yc1, phi_poca1, x_poca1, y_poca1, r2_poca1;
   double xc2, yc2, phi_poca2, x_poca2, y_poca2, r2_poca2;
   bool success = true;
		 
   // Check sizes of input vectors
   
   assert(p1.size() == 3 && p2.size() == 3);
   
   // Calculate the center and radius of the x-y circle
   
   double x1 = p1[0]; double x2 = p2[0]; double x3 = 0.;
   double y1 = p1[1]; double y2 = p2[1]; double y3 = 0.;
   
// Check to see if there are solutions [radius must be large enough]
   
   double dx = x2 - x1;
   double dy = y2 - y1;
   
//  Use the solution that has the largest denominators (most stable)
   
   if(fabs(dy) > fabs(dx)) {
      
      double dxdy = dx/dy;
      double dx2y2dy2 = (x2*x2-x1*x1+y2*y2-y1*y1)/(2.*dy);
      double dx2dy2 = (x2*x2-x1*x1+dy*dy)/(2.*dy);
   
      double a = 1. + dxdy*dxdy;
      double b = -2.*(x1 + dxdy*dx2dy2);
      double c = x1*x1 +dx2dy2*dx2dy2 - radius*radius;
      double arg = b*b-4.*a*c;
   
      if(arg >= 0.) {
      
//  There are (two) solutions
      
         radius_ = radius;
      
         xc1 = (-b+sqrt(arg))/(2.*a);
         yc1 =  dx2y2dy2 -  xc1*dxdy;
         phi_poca1 = atan2(-yc1, -xc1);
         x_poca1 = xc1+radius*cos(phi_poca1);
         y_poca1 = yc1+radius*sin(phi_poca1);
         r2_poca1 = x_poca1*x_poca1 + y_poca1*y_poca1;
     
         xc2 = (-b-sqrt(arg))/(2.*a);
         yc2 = dx2y2dy2 - xc2*dxdy;
         phi_poca2 = atan2(-yc2, -xc2);
         x_poca2 = xc2+radius*cos(phi_poca2);
         y_poca2 = yc2+radius*sin(phi_poca2);
         r2_poca2 = x_poca2*x_poca2 + y_poca2*y_poca2;
         
      } else { success = false;}
      
   } else {
      
      double dydx = dy/dx;
      double dx2y2dx2 = (x2*x2-x1*x1+y2*y2-y1*y1)/(2.*dx);
      double dy2dx2 = (y2*y2-y1*y1+dx*dx)/(2.*dx);
      
      double a = 1. + dydx*dydx;
      double b = -2.*(y1 + dydx*dy2dx2);
      double c = y1*y1 +dy2dx2*dy2dx2 - radius*radius;
      double arg = b*b-4.*a*c;
      
      if(arg >= 0.) {
         
         //  There are (two) solutions
         
         radius_ = radius;
         
         yc1 = (-b+sqrt(arg))/(2.*a);
         xc1 = dx2y2dx2 - yc1*dydx;
         phi_poca1 = atan2(-yc1, -xc1);
         x_poca1 = xc1+radius*cos(phi_poca1);
         y_poca1 = yc1+radius*sin(phi_poca1);
         r2_poca1 = x_poca1*x_poca1 + y_poca1*y_poca1;
         
         yc2 = (-b-sqrt(arg))/(2.*a);
         xc2 = dx2y2dx2 - yc2*dydx;
         phi_poca2 = atan2(-yc2, -xc2);
         x_poca2 = xc2+radius*cos(phi_poca2);
         y_poca2 = yc2+radius*sin(phi_poca2);
         r2_poca2 = x_poca2*x_poca2 + y_poca2*y_poca2;
         
      } else { success = false;}

   }
   *Pass = success;
   if (success) {
      
//  Select the solution that has the closest approach to the beam axis
      
      if(r2_poca1 < r2_poca2) {
         xcenter_ = xc1;
         ycenter_ = yc1;
         phi_poca_ = phi_poca1;
      } else {
         xcenter_ = xc2;
         ycenter_ = yc2;
         phi_poca_ = phi_poca2;
      }

      
   } else {
      
//  There are no solutions, use beam axis as a third point

   
      double deny = y1*(x3-x2)+y2*(x1-x3)+y3*(x2-x1);
      assert(fabs(deny) > 1.e-15);
      ycenter_ = 0.5*(x1*(y2*y2-y3*y3+x2*x2-x3*x3)+x2*(y3*y3-y1*y1+x3*x3-x1*x1)+x3*(y1*y1-y2*y2+x1*x1-x2*x2))/deny;
   
      double denx = x1*(y3-y2)+x2*(y1-y3)+x3*(y2-y1);
      assert(fabs(denx) > 1.e-15);
      xcenter_ = 0.5*(y1*(y2*y2-y3*y3+x2*x2-x3*x3)+y2*(y3*y3-y1*y1+x3*x3-x1*x1)+y3*(y1*y1-y2*y2+x1*x1-x2*x2))/denx;
   
      radius_ = sqrt((x1-xcenter_)*(x1-xcenter_)+(y1-ycenter_)*(y1-ycenter_));
   
   // Now calculate the azimuthal angles of the input points and the closest approach to the beam axis
   
      phi_poca_ = atan2(-ycenter_, -xcenter_);
      
   }
   
   phi1_ = atan2(y1-ycenter_, x1-xcenter_);
   if(phi1_-phi_poca_> pi) { phi1_ -= twopi;} if(phi1_-phi_poca_< -pi) { phi1_ += twopi;}
   
   phi2_ = atan2(y2-ycenter_, x2-xcenter_);
   if(phi2_-phi_poca_> pi) { phi2_ -= twopi;} if(phi2_-phi_poca_< -pi) { phi2_ += twopi;}
   
   phi3_ = atan2(y3-ycenter_, x3-xcenter_);
   if(phi3_-phi_poca_> pi) { phi3_ -= twopi;} if(phi3_-phi_poca_< -pi) { phi3_ += twopi;}
   
   // Finally, calculate the z - phi parameters
   
   double z1 = p1[2]; double z2 = p2[2];
   
   if(fabs(phi2_ - phi1_) > 1.e-15) {
      
      dzdphi_ = (z2 - z1)/(phi2_ - phi1_);
      z0_ = z1 - phi1_ * dzdphi_;
   } else {
      
      dzdphi_ = 0.;
      z0_ = 0.;
   }
   
   double r1 = sqrt(x1*x1+y1*y1);  double r2 = sqrt(x2*x2+y2*y2);
   if(r2 > r1) {
      if(phi2_ > phi1_) {dsdphi_ = sqrt(dzdphi_*dzdphi_ + radius_*radius_);} else {dsdphi_ = -sqrt(dzdphi_*dzdphi_ + radius_*radius_);}
   } else {
      if(phi2_ > phi1_) {dsdphi_ = -sqrt(dzdphi_*dzdphi_ + radius_*radius_);} else {dsdphi_ = sqrt(dzdphi_*dzdphi_ + radius_*radius_);}
   }  
}


// ******************************************************************
//! Destructor
// ******************************************************************

SimpleHelix::~SimpleHelix()
{
  // &&& delete any allocated memory
}


// ******************************************************************
//! Get the 5 helix parameters
// ******************************************************************

void SimpleHelix::parameters(double &radius, double &xcenter, double &ycenter, double &dzdphi, double &z0)
{
   
   radius = radius_; xcenter = xcenter_; ycenter = ycenter_; dzdphi = dzdphi_; z0 = z0_;

   return;
}


// ************************************************************************************************************ 
//! Return the direction cosines for each of the 2 or 3 points used to construct the helix
//! \param d1 - (output) 3-vector for track direction at point p1
//! \param d2 - (output) 3-vector for track direction at point p2
//! \param d3 - (output) 3-vector for track direction at point p3 (on beam axis if only two contructor points)
// ************************************************************************************************************ 

void SimpleHelix::directions(std::vector<double> &d1, std::vector<double> &d2, std::vector<double> &d3)

{

// Directions are given by dx/ds, dy/ds, dz/ds:

   d1.resize(3); d2.resize(3); d3.resize(3);
   
   d1[0] = -radius_*sin(phi1_)/dsdphi_; d1[1] = radius_*cos(phi1_)/dsdphi_; d1[2] = dzdphi_/dsdphi_;
   
   d2[0] = -radius_*sin(phi2_)/dsdphi_; d2[1] = radius_*cos(phi2_)/dsdphi_; d2[2] = dzdphi_/dsdphi_;
   
   d3[0] = -radius_*sin(phi3_)/dsdphi_; d3[1] = radius_*cos(phi3_)/dsdphi_; d3[2] = dzdphi_/dsdphi_;
   
   return;
   
}


// ************************************************************************************************************ 
//! Return coordinates and track direction at point of closest approach to the beam axis (0,0)
//! \param pd - (output) 6-vector for position and track direction at poca to beam axis
// ************************************************************************************************************ 

void SimpleHelix::poca(std::vector<double> &pd)

{

   pd.resize(6);
 
// Use definitions of quantities to calculate the coordinates of the poca and the track direction there
	  
   pd[0] = xcenter_+radius_*cos(phi_poca_); pd[1] = ycenter_+radius_*sin(phi_poca_); pd[2] = phi_poca_ * dzdphi_ + z0_;
   
   pd[3] = -radius_*sin(phi_poca_)/dsdphi_; pd[4] = radius_*cos(phi_poca_)/dsdphi_; pd[5] = dzdphi_/dsdphi_;
      
   return;
   
}   

// *************************************************************************************************************************** 
//! Extrapolate the helix to the plane defined by z=zex (returns the number of full loops needed to reach zex or -1 (failure)
//! \param zex - (input) z coordinate of plane defining the helix extrapolation 
//! \param pd - (output) 6-vector for position and track direction at z=zex
// *************************************************************************************************************************** 

int SimpleHelix::zposdir(double zex, std::vector<double> &pd)

{

   const double twopi= 6.28318530717958; 

// return an integer indicating the number of full helical circles (should be 0 most of the time)

   int ncircle;

   pd.resize(6);
   
   if(fabs(dzdphi_) < 1.e-6) {
   
      for(int i=0; i<6; ++i) { pd[i] = 0.;}
	  ncircle = -1;
	  
   } else {
   
      double phi = (zex-z0_)/dzdphi_; ncircle = (int) fabs(phi/twopi);
	  
	  pd[0] = xcenter_+radius_*cos(phi); pd[1] = ycenter_+radius_*sin(phi); pd[2] = zex;
   
      pd[3] = -radius_*sin(phi)/dsdphi_; pd[4] = radius_*cos(phi)/dsdphi_; pd[5] = dzdphi_/dsdphi_;
   }
   
   return ncircle;
   
}   

// ************************************************************************************************************ 
//! Extrapolate the helix to the cylinder defined by r=rex (returns true for success or false for failure).
//! \param rex - (input) r coordinate of the cylinder defining the helix extrapolation 
//! \param pd - (output) 6-vector for position and track direction at r=rex
// ************************************************************************************************************ 

bool SimpleHelix::rposdir(double rex, std::vector<double> &pd)

{

// return an integer indicating the number of full helical circles (should be 0 most of the time)

   bool success; double phi;

   pd.resize(6);

// rotate system to eliminate the y-coordinate of the helix axis
   
   double xc_prime = xcenter_ * cos(phi_poca_) + ycenter_ * sin(phi_poca_);
   
   double cosdphi = (rex*rex - radius_*radius_ - xc_prime*xc_prime)/(2.*xc_prime*radius_);
   
   if(fabs(cosdphi) <= 1.) {
   
// Take maximum or minimum angle solution depending upon track propagation direction 
   
      if(dsdphi_ > 0.) {phi = phi_poca_ + acos(cosdphi);} else {phi = phi_poca_ - acos(cosdphi);}
	  
	  success = true;
	  
	  pd[0] = xcenter_+radius_*cos(phi); pd[1] = ycenter_+radius_*sin(phi); pd[2] = dzdphi_*phi + z0_;
   
      pd[3] = -radius_*sin(phi)/dsdphi_; pd[4] = radius_*cos(phi)/dsdphi_; pd[5] = dzdphi_/dsdphi_;
	  
   } else {
   
      for(int i=0; i<6; ++i) { pd[i] = 0.;}
	  success = false;
	  
   }
   
   
   return success;
   
}

// ***************************************************************************************************************************
//! Extrapolate the helix to the plane defined by the 6-vector pp (returns the number of full loops needed to reach the
//! intersection or -1 (failure)
//! \param pp - (input) 6-vector that defines the plane.  Estimates the intersection nearest to pp[3] (1st guess z-position)
//! \param pd - (output) 6-vector for position and track direction at the intersection
// ***************************************************************************************************************************


int SimpleHelix::pposdir(std::vector<double> &pp, std::vector<double> &pd)

{
   
   const double twopi= 6.28318530717958;
   double z1, phi, fz, fpz, deltaz;
   
   // return an integer indicating the number of full helical circles (should be 0 most of time)
   
   int ncircle=-99;
   
   pd.resize(6);
   
   double cost2 = pp[5]*pp[5]/(pp[3]*pp[3]+pp[4]*pp[4]+pp[5]*pp[5]);
   
   if(fabs(dzdphi_) < 1.e-6 && cost2 > 0.99) {
      
      for(int i=0; i<6; ++i) { pd[i] = 0.;}
      ncircle = -1;
      
   } else {
      
// Use Newton's Method to find the intersection
      
// Take z-position of plane vector as the first guess
      
      z1 = pp[2];
      
      int niter = 0;
      
      deltaz = 1.;
      
      while(fabs(deltaz) > 1.e-8 && niter < 100) {
         
         phi = (z1-z0_)/dzdphi_;
         
         fz = pp[3]*(xcenter_ - pp[0] + radius_*cos(phi))
         + pp[4]*(ycenter_ - pp[1] + radius_*sin(phi))
         + pp[5]*(z1 - pp[2]);
         
         fpz = -pp[3]*radius_*sin(phi)/dzdphi_+pp[4]*radius_*cos(phi)/dzdphi_+pp[5];
         
         deltaz = -fz/fpz;
         
         z1 += deltaz;
         
         ++niter;
         
      }
      
      phi = (z1-z0_)/dzdphi_; ncircle = (int) fabs(phi/twopi);
      
      pd[0] = xcenter_+radius_*cos(phi); pd[1] = ycenter_+radius_*sin(phi); pd[2] = z1;
      
      pd[3] = -radius_*sin(phi)/dsdphi_; pd[4] = radius_*cos(phi)/dsdphi_; pd[5] = dzdphi_/dsdphi_;
   }
   
   return ncircle;
   
}   


