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

  // public methods
  //
  //


  // constructor
  //
  DynamicWindow::DynamicWindow(Vector2d _velocity, int _maxPosAccel, int _maxNegAccel) {

    for(int left = 0; left < VEL_SPACE_LEFT; left++) {
      for(int right = 0; right < VEL_SPACE_RIGHT; right++) {
        velocitySpace_[left][right] = 0;
      }
    }

    maxPosAccel_ = _maxPosAccel;
    maxNegAccel_ = _maxNegAccel;

    setNewDynamicWindow(_velocity);

  }


  // destructor
  //
  DynamicWindow::~DynamicWindow() {

  }
  

  // set preferred direction
  //  
  void DynamicWindow::setPreferredDirection(double _prefDir) {
	
    // const double WHEEL_DISTANCE = 390.;  // in mm
    const double MAX_SPEED = 100.;  // in cm 
		
    double trans, rot;

    for(int left = minLeft_; left <= maxLeft_; left++) {
      for(int right = minRight_; right <= maxRight_; right++) {

      	trans = exp(-1 * (((MAX_SPEED - ((double)(left + right) / 2.)) / (2 * MAX_SPEED)) *
			  ((MAX_SPEED - ((double)(left + right) / 2.)) / (2 * MAX_SPEED))) / .3);
      	rot = (0.5 + (_prefDir / 2.) + atan((double)left / (double)right) / (M_PI / 2.));

	if(rot > 1) {
	  rot = fabs(rot - 2);
	}
	else {
	  if(rot < -1) {
	    rot = fabs(rot + 2);
	  }
	  else {
	    rot = fabs(rot);
	  }
	}

	velocitySpace_[left+100][right+100] = (int)(64. * (1.0 * rot + 1.5 * trans));
      }
    }
  }	
  

  // check for collisions
  //
  void DynamicWindow::collisionCheck(std::vector<Vector2d> &_robot, std::vector<Vector2d> &_obstacle) {
    
    const double WHEEL_DISTANCE  = 390.;  // in mm
    const double MAX_POLYGON_DISTANCE = 500.;  // in mm
    const double BREAK_ACCELERATION = 1000.;  // in mm/sec2
    const int RESOLUTION = 3;

    double offset, angle, pointValue, fLeft, fRight;
    
    for(int left = minLeft_; left <= maxLeft_; left += RESOLUTION) {
      for(int right = minRight_ ; right <= maxRight_; right += RESOLUTION ) {
  
	if(left == right) {
	  fLeft = 10. * (double)(left * left) / BREAK_ACCELERATION;
	  fRight = 10. * (double)(right * right) / BREAK_ACCELERATION;
      	}
      	else {
	  if(left > right) {
	    fLeft = 10. * (double)(left * left) / BREAK_ACCELERATION;
	    fRight = ((double)(right) / double(left)) * fLeft;
	  }
	  else if(left < right) {
	    fRight = 10. * (double)(right * right) / BREAK_ACCELERATION;
	    fLeft = ((double)(left) / double(right)) * fRight;
	  }
      	}
  
        if(left != right) {
                      
          offset = (WHEEL_DISTANCE / 2.) * ((fLeft + fRight) / (fLeft - fRight));
          angle = (180. * (fLeft - fRight)) / (WHEEL_DISTANCE * M_PI);
          rotateMountedPolygon(_robot, Vector2d(-offset, 0.), angle);       
          pointValue = min(1., getDistanceBetweenMountedPolygonAndMountedPolygon(_robot, _obstacle) / MAX_POLYGON_DISTANCE);
          
          for(int x = 0; x < RESOLUTION; x++) {
            for(int y = 0; y < RESOLUTION; y++) {
              if ((left+100+x < VEL_SPACE_LEFT) && (right+100+y < VEL_SPACE_RIGHT)) {
                velocitySpace_[left+100+x][right+100+y] = 
		  (int)(velocitySpace_[left+100+x][right+100+y] * pointValue);
              }
            }
          }
          
          rotateMountedPolygon(_robot, Vector2d(-offset, 0.), -angle);
          
        }
        else {  // (left - right == 0)
          
          moveMountedPolygon(_robot, Vector2d(0., (fLeft + fRight) / 2.));
          pointValue = min(1., getDistanceBetweenMountedPolygonAndMountedPolygon(_robot, _obstacle) / MAX_POLYGON_DISTANCE);

          for(int x = 0; x < RESOLUTION; x++) {
            for(int y = 0; y < RESOLUTION; y++) {
              if ((left+100+x < VEL_SPACE_LEFT) && (right+100+y < VEL_SPACE_RIGHT)) {
                velocitySpace_[left+100+x][right+100+y] = 
		  (int)(velocitySpace_[left+100+x][right+100+y] * pointValue);
              }
            }
          }
          
          moveMountedPolygon(_robot, Vector2d(0., -1 * (fLeft + fRight) / 2.)); 
          
        }
        
      } 
    }  
    
  } 


  // calc new velocity
  //
  Vector2d DynamicWindow::calcNewVelocity() {

    int biggestVelocitySpaceValue = 10;
    Vector2d biggestVelocitySpaceValueVelocity(0, 0);
    
    for(int left = minLeft_; left < maxLeft_; left++) {
      for(int right = minRight_; right < maxRight_; right++) {
      
        if(velocitySpace_[left+100][right+100] > biggestVelocitySpaceValue)
        {
          biggestVelocitySpaceValue = velocitySpace_[left+100][right+100];
          biggestVelocitySpaceValueVelocity = Vector2d(left, right);
        }
        
      }
    }

    setNewDynamicWindow(biggestVelocitySpaceValueVelocity);

    return biggestVelocitySpaceValueVelocity;
  }
  

  // private methods
  //
  //
	

  // set new DynamicWindow
  // 
  void DynamicWindow::setNewDynamicWindow(Vector2d _velocity) {


    velocity_ = _velocity;

    velocity_ = std::complex<double>(std::max(std::min(40., velocity_.real()),-40.), std::max(std::min(40.,velocity_.imag()),-40.));
        
    minRight_ = std::max((int)(std::imag(velocity_)  - (maxNegAccel_ / (10 * PACE))), -100);
    maxRight_ = std::min(100, (int)(std::imag(velocity_) + (maxPosAccel_ / (10 * PACE))));
    minLeft_ = std::max((int)(std::real(velocity_) - (maxNegAccel_ / (10 * PACE))), -100);
    maxLeft_ = std::min(100, (int)(std::real(velocity_) + (maxPosAccel_ / (10 * PACE))));

  }
	

  // get signed distance between point and line
  //  
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
  
	
  // get distance between two lines
  //
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
  
  
  // get distance between two mounted polygons
  // 
  double DynamicWindow::getDistanceBetweenMountedPolygonAndMountedPolygon(std::vector<Vector2d> &_polygon1, std::vector<Vector2d> &_polygon2) {
    
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

  
  // rotate mounted polygon around offset by given angle
  // 
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
  
  
  // move the given polygon by given distance
  // 
  void DynamicWindow::moveMountedPolygon(std::vector<Vector2d> &_polygon, Vector2d _distance) {
    
    std::vector<Vector2d>::iterator i;
    
    for(i = _polygon.begin() + 1; i < _polygon.end(); i++) {
      *i = Vector2d(i->real() + _distance.real(), i->imag() + _distance.imag());
    }
    
  }

    
};
