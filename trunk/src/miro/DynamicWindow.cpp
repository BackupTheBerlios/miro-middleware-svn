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
#include <cstdio>
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

    double value_left, value_right;

    for(int left = minLeft_; left <= maxLeft_; left++) {
      for(int right = minRight_; right <= maxRight_; right++) {

	value_left = cos(_prefDir - M_PI / 4.) * left - sin(_prefDir - M_PI / 4.) * right;
	value_right = sin(_prefDir - M_PI / 4.) * left + cos(_prefDir - M_PI / 4.) * right;

	velocitySpace_[left+100][right+100] = (int)(255. * (1. + atan((double)value_left / fabs((double)value_right)) / (M_PI / 2.)) / 2.);

      }
    }
  }


  // check for collisions DELUXE
  //

  void DynamicWindow::collisionCheckDeluxe(std::vector<Vector2d> &_robot, std::vector<Vector2d> &_obstacle) {

    const int CURV_CNT = 6; // count
    const int CURV_RES = 6; // count
    const int CURV_LEN = 600; // mm
    const int CURV_SMOOTH_LEN = 300; // mm
    const double WHEEL_DISTANCE  = 390.; // mm
    const double BREAK_ACCEL = 2000.; // in mm/sec2

    int CURV[2*CURV_CNT+1][2*CURV_RES+1]; // curvature space
    
    int count, seg, left, right, curv, target, front, back;
    bool frontObstacle, backObstacle;
    double fLeft, fRight, offset, angle, rel, break_dist, left_break, right_break;

    std::FILE *logFile1;
    logFile1 = std::fopen("curvature.log","a");
    
    for(count = 0; count < 2*CURV_CNT+1; count++) {

      // CALCULATING CURVATURE SPACE
      //
      
      // relation between left and right wheel (-inf...0...+inf)
      rel = tan( (M_PI * count / (2*CURV_CNT)) - (M_PI/2) );
      
      // calculate speed for left and right wheel using 'rel'
      if(fabs(rel) >= 1) {
    	fLeft = (double)CURV_LEN / (double)CURV_RES;
        fRight = fLeft / rel;
      }
      else {
	fRight = (double)CURV_LEN / (double)CURV_RES;
	fLeft = fRight * rel;
      }

      // check for collisions along the curvature given by left and right speed   
      if(rel != 1) {  // aka left!=right ==> real curvature
	
	// calculate offset and angle for rotation
        offset = (WHEEL_DISTANCE / 2.) * ((fLeft + fRight) / (fLeft - fRight));
        angle = (180. * (fLeft - fRight)) / (WHEEL_DISTANCE * M_PI);
	
	// rotate completely backwards
	rotateMountedPolygon(_robot, Vector2d(-offset, 0.), -angle * (CURV_RES + 1));
	
	// rotate stepwise forwards and check for collisions
	for(seg = 0; seg < 2*CURV_RES+1; seg++) {
	  rotateMountedPolygon(_robot, Vector2d(-offset, 0.), angle);
          if(getDistanceBetweenPolygonAndPolygon(_robot, _obstacle) == 0) {
	    CURV[count][seg] = 0;
	  }
          else {
	    CURV[count][seg] = 250;
	  }
          fprintf(logFile1,"%d\n",CURV[count][seg]);
	}
	
	// rotate backwards to middle position
	rotateMountedPolygon(_robot, Vector2d(-offset, 0.), -angle * CURV_RES);
	
      }
      else { // left==right ==> robot moves straight forward/backward
	for(seg = 0; seg < 2*CURV_RES+1 ; seg++) {
	  fprintf(logFile1,"%d\n",0);	// INCOMPLETE!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
      }
     
      // SMOOTHING CURVATURE SPACE
      //
    
      // search curvature space for obstacles (obstacle = 0)
      front = CURV_RES;
      while((front < 2*CURV_RES+1) && (CURV[count][front] != 0)) {
	front++;
      }
      back = CURV_RES;
      while((back > 0) && (CURV[count][back] != 0)) {
        back--;
      }
      (front == 2*CURV_RES+1) ? frontObstacle = false : frontObstacle = true; // true = obstacle found
      (back == -1) ? backObstacle = false : backObstacle = true; // true = obstacle found
      backObstacle = false;
      
      while((frontObstacle && (front > 0)) || (backObstacle && (back < 2*CURV_RES+1))) {
	if(frontObstacle && (front > 0)) {
	  CURV[count][front-1] = std::min(CURV[count][front-1],std::min(250, CURV[count][front] + (int)(250 * CURV_LEN / (CURV_SMOOTH_LEN * CURV_RES))));
	  front--;
	} 
	if(backObstacle && (back < 2*CURV_RES+1)) {
	  CURV[count][back+1] = std::min(CURV[count][back+1],std::min(250, CURV[count][back] + (int)(250 * CURV_LEN / (CURV_SMOOTH_LEN * CURV_RES))));
	  back++;
	}
	
      }
      
      
    }
    
    // CALCULATING VELOCITY SPACE (BASED ON CURVATURE SPACE)
    //
    
    for(left = minLeft_; left <= maxLeft_; left++) {
      for(right = minRight_ ; right <= maxRight_; right++) {
	
	// calculating actual curvature
	curv = (int)((atan((double)left / (double)right) + (M_PI / 2.)) * 2 * CURV_CNT / M_PI);
	// calculating breaking distance
	left_break = ((10. * fabs((double)left)) / BREAK_ACCEL) * 10. * (double)left;	
	right_break = ((10. * fabs((double)right)) / BREAK_ACCEL) * 10. * (double)right;
	if(fabs(left_break) >= fabs(right_break))
	  break_dist = left_break;
	else
	  break_dist = right_break;
	
	target = CURV_RES  + (int)(break_dist  * (double)CURV_RES / (double)CURV_LEN);
	
	velocitySpace_[left+100][right+100] = (double)CURV[std::max(0,std::min(2*CURV_CNT,curv))][std::max(0,std::min(2*CURV_RES,target))] * (double)velocitySpace_[left+100][right+100] / 250.;
	// velocitySpace_[left+100][right+100] = std::min(CURV[std::max(0,std::min(2*CURV_CNT,curv))][std::max(0,std::min(2*CURV_RES,target))], velocitySpace_[left+100][right+100]);
	
      }
    }
    
    fclose(logFile1);
    
  }


  // check for collisions
  //
  void DynamicWindow::collisionCheck(std::vector<Vector2d> &_robot, std::vector<Vector2d> &_obstacle) {

    const double WHEEL_DISTANCE  = 390.;  // in mm
    const double MAX_POLYGON_DISTANCE = 500.;  // in mm
    const double BREAK_ACCELERATION = 2000.;  // in mm/sec2
    const double MIN_DISTANCE = 100.;
    const int RESOLUTION = 4;

    double offset, angle, pointValue, fLeft, fRight;

    for(int left = minLeft_; left <= maxLeft_; left += RESOLUTION) {
      for(int right = minRight_ ; right <= maxRight_; right += RESOLUTION ) {

	if(left == right) {
	  fLeft = 100. * (double)(abs(left) * left) / BREAK_ACCELERATION;
	  fRight = 100. * (double)(abs(right) * right) / BREAK_ACCELERATION;
      	}
       	else {
	  if(abs(left) > abs(right)) {
	    fLeft = 100. * (double)(abs(left) * left) / BREAK_ACCELERATION;
	    fRight = ((double)(right) / double(left)) * fLeft;
	  }
	  else if(abs(left) < abs(right)) {
	    fRight = 100. * (double)(abs(right) * right) / BREAK_ACCELERATION;
	    fLeft = ((double)(left) / double(right)) * fRight;
	    }
	}

        if(left != right) {

          offset = (WHEEL_DISTANCE / 2.) * ((fLeft + fRight) / (fLeft - fRight));
          angle = (180. * (fLeft - fRight)) / (WHEEL_DISTANCE * M_PI);
          rotateMountedPolygon(_robot, Vector2d(-offset, 0.), angle);

	  if(getDistanceBetweenPolygonAndPolygon(_robot, _obstacle) < MIN_DISTANCE) {
	    pointValue = 0;
	  }
	  else {
	    pointValue = std::min(1., getFrontDistanceBetweenPolygonAndPolygon(_robot, _obstacle) / MAX_POLYGON_DISTANCE);
	  }

          for(int x = 0; (x < RESOLUTION) && (x+left <= maxLeft_); x++) {
            for(int y = 0; (y < RESOLUTION) && (y+right <= maxRight_); y++) {
	      velocitySpace_[left+100+x][right+100+y] = (int)(velocitySpace_[left+100+x][right+100+y] * pointValue);
	    }
	  }

          rotateMountedPolygon(_robot, Vector2d(-offset, 0.), -angle);

        }
        else {  // (left == right)

          moveMountedPolygon(_robot, Vector2d(0., (fLeft + fRight) / 2.));

          if(getDistanceBetweenPolygonAndPolygon(_robot, _obstacle) < MIN_DISTANCE) {
	    pointValue = 0;
	  }
	  else {
	    pointValue = std::min(1., getDistanceBetweenPolygonAndPolygon(_robot, _obstacle) / MAX_POLYGON_DISTANCE);
	  }

          for(int x = 0; (x < RESOLUTION) && (x+left <= maxLeft_); x++) {
            for(int y = 0; (y < RESOLUTION) && (y+right <= maxRight_); y++) {
	      velocitySpace_[left+100+x][right+100+y] =	(int)(velocitySpace_[left+100+x][right+100+y] * pointValue);
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

    int biggestVelocitySpaceValue = 0;
    double biggestVelocitySpaceRadius = 0.;
    Vector2d biggestVelocitySpaceValueVelocity(0,0);
    

    for(int left = minLeft_; left < maxLeft_; left++) {
      for(int right = minRight_; right < maxRight_; right++) {

        if((velocitySpace_[left+100][right+100] > biggestVelocitySpaceValue) || 
	   ((velocitySpace_[left+100][right+100] == biggestVelocitySpaceValue) && ((left * left) + (right * right)) > biggestVelocitySpaceRadius) )
        {
          biggestVelocitySpaceValue = velocitySpace_[left+100][right+100];
	  biggestVelocitySpaceRadius = (left * left) + (right * right);
	  biggestVelocitySpaceValueVelocity = Vector2d(left,right);
	  
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
      if(l1_l2.imag() * l1_p1.real() + -l1_l2.real() * l1_p1.imag() < 0)
	return sqrt(l1_p1.real() * l1_p1.real() + l1_p1.imag() * l1_p1.imag());
      else
	return -sqrt(l1_p1.real() * l1_p1.real() + l1_p1.imag() * l1_p1.imag());
    }

    l2_p1 = _p1 - _l2;

    if(-l1_l2.real() * l2_p1.real() + -l1_l2.imag() * l2_p1.imag() < 0) {
      if(l1_l2.imag() * l2_p1.real() + -l1_l2.real() * l2_p1.imag() < 0)
	return sqrt(l2_p1.real() * l2_p1.real() + l2_p1.imag() * l2_p1.imag());
      else
	return -sqrt(l2_p1.real() * l2_p1.real() + l2_p1.imag() * l2_p1.imag());
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

    return fabs(std::min(std::min(l1_to_l3_l4, l2_to_l3_l4), std::min(l3_to_l1_l2, l4_to_l1_l2)));

  }


  // get distance between two mounted polygons
  //
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


  // get front distance between robot and obstacle
  //
  double DynamicWindow::getFrontDistanceBetweenPolygonAndPolygon(std::vector<Vector2d> &_polygon1, std::vector<Vector2d> &_polygon2) {

    std::vector<Vector2d>::iterator a1, a2, b1, b2;
    double distance, minDistance;

    for(a1 = _polygon1.begin() + 2, a2 = _polygon1.begin() + 3; a2 < _polygon1.begin() + 5; a1++, a2++) {
      for(b1 = _polygon2.begin() + 4, b2 = _polygon2.begin() + 5; b2 < _polygon2.begin() + 7; b1++, b2++) {

        distance = getDistanceBetweenLineAndLine(*a1, *a2, *b1, *b2);

        if(((a1 == _polygon1.begin() + 2) && (b1 == _polygon2.begin() + 4)) || (distance < minDistance)) {
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


  // move the given mounted polygon by given distance
  //
  void DynamicWindow::moveMountedPolygon(std::vector<Vector2d> &_polygon, Vector2d _distance) {

    std::vector<Vector2d>::iterator i;

    for(i = _polygon.begin() + 1; i < _polygon.end(); i++) {
      *i = Vector2d(i->real() + _distance.real(), i->imag() + _distance.imag());
    }

  }


  // rotate polygon around offset by given angle
  //
  void DynamicWindow::rotatePolygon(std::vector<Vector2d> &_polygon, Vector2d _point, double _angle) {

    std::vector<Vector2d>::iterator i;

    double cosAngle = cos(M_PI * _angle / 180.);
    double sinAngle = sin(M_PI * _angle / 180.);

    for(i = _polygon.begin(); i < _polygon.end(); i++) {
      double temp_x, temp_y;
      temp_x = (i->real() - _point.real()) * cosAngle + (i->imag() - _point.imag()) * -sinAngle;
      temp_y = (i->real() - _point.real()) * sinAngle + (i->imag() - _point.imag()) * cosAngle;
      *i = Vector2d(temp_x + _point.real(), temp_y + _point.imag());
    }

  }


  // move the given polygon by given distance
  //
  void DynamicWindow::movePolygon(std::vector<Vector2d> &_polygon, Vector2d _distance) {

    std::vector<Vector2d>::iterator i;

    for(i = _polygon.begin(); i < _polygon.end(); i++) {
      *i = Vector2d(i->real() + _distance.real(), i->imag() + _distance.imag());
    }

  }


};
