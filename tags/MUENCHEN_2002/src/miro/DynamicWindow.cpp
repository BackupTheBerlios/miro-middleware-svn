// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DynamicWindow.h"
#include <iostream>
#include <complex>
#include <cmath>

namespace Miro
{
  using std::min;
  using std::max;


  // constructor
  DynamicWindow::DynamicWindow(Vector2d _velocity, int _maxPosAccel, int _maxNegAccel)
  {

    // clear all entrys of the velocity space
    for(int left = 0; left < VEL_SPACE_LEFT; left++) {
      for(int right = 0; right < VEL_SPACE_RIGHT; right++) {
	velocitySpace_[left][right] = 0;
      }
    }

    // set the maximum positive and negative acceleration
    maxPosAccel_ = _maxPosAccel;
    maxNegAccel_ = _maxNegAccel;

    // set the new velocity and calculate the size
    // of the new dynamicWindow
    setNewDynamicWindow(_velocity);

  }

  // destructor
  DynamicWindow::~DynamicWindow() {

  }

  // set the new velocity and calculate the size
  // of the new dynamicWindow
  void DynamicWindow::setNewDynamicWindow(Vector2d _velocity) {
	
    // set the actual velocity
    velocity_ = _velocity;
	
    // calculate the new boundaries of the dynamic window
    minRight_ = std::max((int)(std::imag(velocity_)  - (maxNegAccel_ / (10 * PACE))), -100);
    maxRight_ = std::min(100, (int)(std::imag(velocity_) + (maxPosAccel_ / (10 * PACE))));
    minLeft_ = std::max((int)(std::real(velocity_) - (maxNegAccel_ / (10 * PACE))), -100);
    maxLeft_ = std::min(100, (int)(std::real(velocity_) + (maxPosAccel_ / (10 * PACE))));

  }


  // calculate the new velocity using the content
  // of the actual dynamic window
  Vector2d DynamicWindow::calcNewVelocity() {

    // local variables and objects
    const double ANGLE_STEP = 2.;
    const int STEP_SIZE = 2;

    Vector2d bestLineVelocity(0, 0), bestVelocity(0, 0);

    double angle, cosAngle, sinAngle;   // actual angel, its cosinus and its sinus
    double fLeft, fRight;   // actual point of interest
    int lineSize;   // size of the total actual line
    int stepCount;   // linb-step-count in the actual dynamic window
    int sumOfLineValues;   // sum of line-values in the actual dynamic window
    int biggestLineValue;   //
    double actualLineValueAverage;   // average of the line-values of the actual line
    double biggestLineValueAverage;  // biggest of all actualLineValueAverages

    bestVelocity  = velocity_;
    biggestLineValueAverage = 0.;

    // florain: warum nicht gleich in rad?
    // #inlucde <miro/Angle.h>, using Miro::deg2Rad;
    // for (angle = deg2Rad(0); angle < deg2Rad(360); angle += deg2Rad(ANGE_STEP)
    for(angle = 0.; angle < 360.; angle += ANGLE_STEP) {
      // calculate sinus and cosinus of the angle

      // florian: pi = M_PI
      sinAngle = sin(angle * 3.1415 / 180.);
      cosAngle = cos(angle * 3.1415 / 180.);
      // start in the middle of the velocity space
      fLeft = 0.; fRight = 0.;
		
      // florian: bitte nur ein statement pro zeile - danke
      sumOfLineValues = 0; stepCount = 0; biggestLineValue = 0;
      bestLineVelocity = std::complex<double>(std::min(maxLeft_, std::max(0,minLeft_)),
					      std::min(maxRight_, std::max(0,minRight_)));

      // florian: optimierungspotential?!
      for(lineSize = 0; lineSize < 150; lineSize += STEP_SIZE) {
	fLeft += cosAngle * STEP_SIZE;
	fRight += sinAngle * STEP_SIZE;
	if( (fLeft>=minLeft_) && (fLeft<=maxLeft_) && (fRight>=minRight_) && (fRight<=maxRight_) ) {
	  sumOfLineValues += velocitySpace_[100 + (int)fLeft][100 + (int)fRight] *
	    velocitySpace_[100 + (int)fLeft][100 + (int)fRight];
	  stepCount++;
	  if(velocitySpace_[100 + (int)fLeft][100 + (int)fRight] > biggestLineValue) {
	    biggestLineValue = velocitySpace_[100 + (int)fLeft][100 + (int)fRight];
	    bestLineVelocity = std::complex<double>((int)fLeft, (int)fRight);
	  }
	}
      }
		    
      if(stepCount!=0) {
	actualLineValueAverage = (double)sumOfLineValues / (double)stepCount;
	if(actualLineValueAverage >= biggestLineValueAverage) {
	  biggestLineValueAverage = actualLineValueAverage;
	  bestVelocity = bestLineVelocity;
	}
      } 
 
    }  
    return bestVelocity;
  }
  

  void DynamicWindow::collisionCheck(std::vector<Vector2d> &_robot, std::vector<Vector2d> &_obstacle) {
		
    // muss parameterisierbar sein, für andere Roboter...
    const double WHEEL_DISTANCE  = 390.;  // in mm
    const double SCALE_ANGLE = 1.0;
    const double MAX_POL_DIST = 500.; // maximum polgon distance = 50cm
    const int RES = 3;
    int hack;

    double offset, angle, pointValue, maxPointValue = 0.;
    
    for(int left = minLeft_; left <= maxLeft_; left+=RES) {
      for(int right = minRight_ + 1; right <= maxRight_; right+=RES ) {
	
	if((left - right) == 0)
	  (right<100)?(hack=1):(hack=-1);
	else
	  hack = 0;
	
	double fLeft = 10. * (double)left;
	double fRight = 10. * (double)right;
	
	offset = (WHEEL_DISTANCE / 2.) * ((fLeft + (fRight + hack)) / (fLeft - (fRight + hack)));
	angle = (180. * (fLeft - (fRight + hack))) / (390. * M_PI);
	
	rotateMountedPolygon(_robot, Vector2d(-offset, 0.), angle * SCALE_ANGLE);
	pointValue = min(MAX_POL_DIST, getDistanceBetweenPolygonAndPolygon(_robot, _obstacle));
	if(pointValue < 10.) {
	  for(int x = 0; x < RES; x++) {
	    for(int y = 0; y < RES; y++) {
	      if ((left+100+x < VEL_SPACE_LEFT) && (right+100+y < VEL_SPACE_RIGHT)) {
		velocitySpace_[left+100+x][right+100+y] = 0;
	      }
	    }
	  }
	}
	else {
	  for(int x = 0; x < RES; x++) {
	    for(int y = 0; y < RES; y++) {
	      if ((left+100+x < VEL_SPACE_LEFT) && (right+100+y < VEL_SPACE_RIGHT)) {
		velocitySpace_[left+100+x][right+100+y] =
		  min(255, (int)(velocitySpace_[left+100+x][right+100+y] * pointValue / MAX_POL_DIST));
	      }
	    }
	  }
	}
	if(maxPointValue < pointValue)
	  maxPointValue = pointValue;
	rotateMountedPolygon(_robot, Vector2d(-offset, 0.), -angle * SCALE_ANGLE);
      }  
    }
    std::cout << "MAX POINT VALUE :::: " << maxPointValue << std::endl;
    
  }
  
  
  // set a preferred direction and calculate the new
  // content of the velocity space
  void DynamicWindow::setPreferredDirection(double _prefDir) {

    const double WHEEL_DISTANCE = 39.;	// in cm
    double trans, rot, rotDist, posTrans;
	
    for(int left = minLeft_; left <= maxLeft_; left++) {
      for(int right = minRight_; right <= maxRight_; right++) {
	
	// calculate translation (-100..100) and rotation (-294..294)
	trans = (left + right) / 2.;
	rot = (left - right) * 180. / (WHEEL_DISTANCE * M_PI);
	
	// calculate the difference between the preferred direction and
	// and the actual direction and deliver the weight betwwen 0 and 100
	rotDist = 100. * (1. - ((fabs(_prefDir - rot)) / (fabs(_prefDir) + 294.)));
	
	// positive translation weight (0..100)
	posTrans = (trans + 100.) / 2.;
	
	// set the corresponding value in the velocitySpace
	velocitySpace_[left+100][right+100] = (int)(1.0 *  rotDist + 1.5 * posTrans);
	
      }
    }	
    
  } 
  
  double DynamicWindow::getDistanceBetweenPolygonAndPolygon(std::vector<Vector2d> &_polygon1, std::vector<Vector2d> &_polygon2) {
    
    std::vector<Vector2d>::iterator a1, a2, b1, b2;
    double distance, minDistance;
    
    for(a1 = _polygon1.begin(), a2 = _polygon1.begin() + 1; a2 < _polygon1.end(); a1++, a2++) {
      for(b1 = _polygon2.begin(), b2 = _polygon2.begin() + 1; b2 < _polygon2.end(); b1++, b2++) {
	
	distance = getDistanceBetweenLineAndLine(*a1, *a2, *b1, *b2);
	
	if(((a1 == _polygon1.begin()) && (b1 == _polygon2.begin())) || (distance < minDistance)) {
	  minDistance = distance;
	}
	
      }
    }
    return minDistance;
  }  
  
  void DynamicWindow::rotateMountedPolygon(std::vector<Vector2d> &_polygon, Vector2d _point, double _angle) {
    
    std::vector<Vector2d>::iterator i;
    
    double cosAngle = cos(M_PI * _angle / 180.);
    double sinAngle = sin(M_PI * _angle / 180.);
    
    for(i = _polygon.begin() + 1; i < _polygon.end(); i++) {
      double temp_x, temp_y;
      temp_x = (i->real() - _point.real()) * cosAngle + (i->imag() - _point.imag()) * -sinAngle;
      temp_y = (i->real() - _point.real()) * sinAngle + (i->imag() - _point.imag()) * cosAngle;  
      *i = Vector2d(temp_x + _point.real(), temp_y + _point.imag());
    }
    
  }
  
  void DynamicWindow::moveMountedPolygon(std::vector<Vector2d> &_polygon, Vector2d _distance) {
    
    std::vector<Vector2d>::iterator i;
    
    for(i = _polygon.begin() + 1; i < _polygon.end(); i++) {
      *i = Vector2d(i->real() + _distance.real(), i->imag() + _distance.imag());
    }
    
  }

  double DynamicWindow::getAngleBetweenVectors(Vector2d _v1, Vector2d _v2) {

    double a1 = atan(_v1.imag() / _v1.real());

    if(_v1.real() < 0)
      a1 += M_PI;
    if(_v1.real() >= 0 && _v1.imag() < 0)
      a1 += 2 * M_PI;
    
    double a2 = atan(_v2.imag() / _v2.real());
    if(_v2.real() < 0)
      a2 += M_PI;
    if(_v2.real() >= 0 && _v2.imag() < 0)
      a2 += 2 * M_PI;
    
    if(a2 < a1)
      a2 += 2 * M_PI;
    
    return  a2 - a1;
    
  }
  
  double DynamicWindow::getSignedDistanceBetweenPointAndLine(Vector2d _p1, Vector2d _l1, Vector2d _l2) {

    Vector2d l1_l2, l1_p1, l2_p1;

    l1_l2 = _l2 - _l1;
    l1_p1 = _p1 - _l1;

    if(l1_l2.real() * l1_p1.real() + l1_l2.imag() * l1_p1.imag() < 0) {
      return sqrt(l1_p1.real() * l1_p1.real() + l1_p1.imag() * l1_p1.imag());
    }

    l2_p1 = _p1 - _l2;

    if(-l1_l2.real() * l2_p1.real() + -l1_l2.imag() * l2_p1.imag() < 0) {
      return sqrt(l2_p1.real() * l2_p1.real() + l2_p1.imag() * l2_p1.imag());
    }

    return (l1_l2.real() * l1_p1.imag() - l1_l2.imag() * l1_p1.real())
      / sqrt(l1_l2.real() * l1_l2.real() + l1_l2.imag() * l1_l2.imag());

  }

  double DynamicWindow::getDistanceBetweenLineAndLine(Vector2d _l1, Vector2d _l2, Vector2d _l3, Vector2d _l4) {

    double l1_to_l3_l4, l2_to_l3_l4, l3_to_l1_l2, l4_to_l1_l2;
    
    l1_to_l3_l4 = getSignedDistanceBetweenPointAndLine(_l1, _l3, _l4);
    l2_to_l3_l4 = getSignedDistanceBetweenPointAndLine(_l2, _l3, _l4);
    l3_to_l1_l2 = getSignedDistanceBetweenPointAndLine(_l3, _l1, _l2);
    l4_to_l1_l2 = getSignedDistanceBetweenPointAndLine(_l4, _l1, _l2);

    if( ((l1_to_l3_l4 * l2_to_l3_l4) < 0) && ((l3_to_l1_l2 * l4_to_l1_l2) < 0) )
      return 0;

    return fabs(min(min(l1_to_l3_l4, l2_to_l3_l4), min(l3_to_l1_l2, l4_to_l1_l2)));

  }
  
};
