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

#include "VelocitySpace.h"
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
  VelocitySpace::VelocitySpace(Vector2d _velocity, int _maxPosAccel, int _maxNegAccel) {

    for(int left = 0; left < VEL_SPACE_LEFT; left++) {
      for(int right = 0; right < VEL_SPACE_RIGHT; right++) {
        velocitySpace_[left][right] = 0;
      }
    }

    maxPosAccel_ = _maxPosAccel;
    maxNegAccel_ = _maxNegAccel;

    setNewVelocitySpace(_velocity);

  }


  // destructor
  //
  VelocitySpace::~VelocitySpace() {

  }

  // copy function
  void VelocitySpace::getCopy(Miro::VelocitySpace *_VelocitySpace) {

    for(int left = 0; left < VEL_SPACE_LEFT; left++) {
      for(int right = 0; right < VEL_SPACE_RIGHT; right++) {
        _VelocitySpace->velocitySpace_[left][right] = velocitySpace_[left][right];
      }
    }

  }


  // add evaluation for preferred direction
  //
  void VelocitySpace::addEvalForPreferredDirection(double _prefDir) {

    double value_left, value_right;

    for(int left = minLeft_; left <= maxLeft_; left++) {
      for(int right = minRight_; right <= maxRight_; right++) {

	value_left = cos(_prefDir - M_PI / 4.) * left - sin(_prefDir - M_PI / 4.) * right;
	value_right = sin(_prefDir - M_PI / 4.) * left + cos(_prefDir - M_PI / 4.) * right;

	velocitySpace_[left+100][right+100] = (int)(255. * (1. + atan((double)value_left / fabs((double)value_right)) / (M_PI / 2.)) / 2.);

      }
    }
  }


  // add evaluation for obstacle
  //

  void VelocitySpace::addEvalForObstacle(std::vector<Vector2d> &_robot, std::vector<Vector2d> &_obstacle) {

    const int CURV_CNT = 6; // count (6)
    const int CURV_RES = 6; // count (6)
    const int CURV_LEN = 700; // mm
    const int CURV_SMOOTH_LEN = 10; // mm
    const int CURV_DELAY = 6; // count
    const double WHEEL_DISTANCE  = 330.; // mm
    const double BREAK_ACCEL = 2000.; // in mm/sec2

    int CURV[2*CURV_CNT+1][2*CURV_RES+1]; // curvature space

    int count, seg, left, right, curv, target, front, back;
    bool frontObstacle, backObstacle;
    double fLeft, fRight, offset, angle, rel, break_dist, left_break, right_break;

    std::FILE *logFile1;

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
	rotateMountedPolygon(_robot, Vector2d(offset, 0.), angle * (CURV_RES + 1));

	// rotate stepwise forwards and check for collisions
	for(seg = 0; seg < 2*CURV_RES+1; seg++) {
	  rotateMountedPolygon(_robot, Vector2d(offset, 0.), -angle);
          if(getDistanceBetweenPolygonAndPolygon(_robot, _obstacle) == 0) {
	    CURV[count][seg] = 0;
	  }
          else {
	    CURV[count][seg] = 250;
	  }
	}

	// rotate backwards to middle position
	rotateMountedPolygon(_robot, Vector2d(offset, 0.), angle * CURV_RES);

      }
      else { // left==right ==> robot moves straight forward/backward
	for(seg = 0; seg < 2*CURV_RES+1 ; seg++) {
	  CURV[count][seg] = 0;
	}
      }

      // CLEANING AND SMOOTHING CURVATURE SPACE
      //

      // cleaning curvature space
      front = CURV_RES;
      frontObstacle = false;
      while(front < 2*CURV_RES+1) {
	if(CURV[count][front] == 0) {
	  frontObstacle = true;
	}
	if(frontObstacle == true) {
	  CURV[count][front] = 0;
	}
	front++;
      }

      back = CURV_RES;
      backObstacle = false;
      while(back > -1) {
	if(CURV[count][back] == 0) {
	  backObstacle = true;
	}
	if(backObstacle == true) {
	  CURV[count][back] = 0;
	}
	back--;
      }


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
      for(right = minRight_; right <= maxRight_; right++) {

	// calculating actual curvature
	curv = (int)((atan((double)left / (double)right) + (M_PI / 2.)) * 2 * CURV_CNT / M_PI);
	// calculating breaking distance
	left_break = ((10. * fabs((double)left)) / BREAK_ACCEL) * 10. * (double)left;
	right_break = ((10. * fabs((double)right)) / BREAK_ACCEL) * 10. * (double)right;
	if(fabs(left_break) >= fabs(right_break))
	  break_dist = left_break;
	else
	  break_dist = right_break;

	if((left+right)>0)
	  target = CURV_RES  + CURV_DELAY + (int)(break_dist  * (double)CURV_RES / (double)CURV_LEN);
	else
	  target = CURV_RES - CURV_DELAY + (int)(break_dist * (double)CURV_RES / (double)CURV_LEN);

	velocitySpace_[left+100][right+100] = (int)((double)CURV[std::max(0,std::min(2*CURV_CNT,curv))][std::max(0,std::min(2*CURV_RES,target))] * (double)velocitySpace_[left+100][right+100] / 250.);

      }
    }

    // create logFiles
    logFile1 = std::fopen("curvature.log","a");
    for(curv = 0; curv <= 2*CURV_CNT; curv++) {
      for(seg = 0; seg <= 2*CURV_RES; seg++) {
	fprintf(logFile1,"%d\n",CURV[curv][seg]);
      }
    }
    fclose(logFile1);

  }


  // calc new velocity
  //
  Vector2d VelocitySpace::applyObjectiveFunctionToEval() {

    int biggestVelocitySpaceValue = 10;
    double biggestVelocitySpaceRadius = 0.;
    Vector2d biggestVelocitySpaceValueVelocity(0,0);
    bool found = false;
    int temp_left, temp_right;
    double angle;
    const double WHEEL_DISTANCE = 330.;

    for(int left = minLeft_; left < maxLeft_; left++) {
      for(int right = minRight_; right < maxRight_; right++) {

	angle = fabs((180. * (double)(left - right)) / (WHEEL_DISTANCE * M_PI));

        if((angle < 45.) && ((velocitySpace_[left+100][right+100] > biggestVelocitySpaceValue) ||
	   ((velocitySpace_[left+100][right+100] == biggestVelocitySpaceValue) && ((left * left) + (right * right)) > biggestVelocitySpaceRadius) ))
        {
          biggestVelocitySpaceValue = velocitySpace_[left+100][right+100];
	  biggestVelocitySpaceRadius = (left * left) + (right * right);
	  biggestVelocitySpaceValueVelocity = Vector2d(left,right);
	  found = true;

        }
      }
    }

    if(found==false) {
      if(minLeft_ < 0) {
	if(maxLeft_ < 0) {
	  temp_left = maxLeft_;
	}
	else {
	  temp_left = 0;
	}
      }
      else {
	temp_left = minLeft_;
      }
      if(minRight_ < 0) {
	if(maxRight_ < 0) {
	  temp_right = maxRight_;
	}
	else {
	  temp_right = 0;
	}
      }
      else {
	temp_right = minRight_;
      }

      biggestVelocitySpaceValueVelocity = Vector2d(temp_left, temp_right);

    }

    setNewVelocitySpace(biggestVelocitySpaceValueVelocity);

    return biggestVelocitySpaceValueVelocity;
  }


  // private methods
  //
  //


  // set new VelocitySpace
  //
  void VelocitySpace::setNewVelocitySpace(Vector2d _velocity) {


    velocity_ = _velocity;

    velocity_ = std::complex<double>(std::max(std::min(40., velocity_.real()),-40.), std::max(std::min(40.,velocity_.imag()),-40.));

    minRight_ = std::max((int)(std::imag(velocity_)  - (maxNegAccel_ / (10 * PACE))), -100);
    maxRight_ = std::min(100, (int)(std::imag(velocity_) + (maxPosAccel_ / (10 * PACE))));
    minLeft_ = std::max((int)(std::real(velocity_) - (maxNegAccel_ / (10 * PACE))), -100);
    maxLeft_ = std::min(100, (int)(std::real(velocity_) + (maxPosAccel_ / (10 * PACE))));

  }


  // get signed distance between point and line
  //
  double VelocitySpace::getSignedDistanceBetweenPointAndLine(Vector2d _p1, Vector2d _l1, Vector2d _l2) {

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
  double VelocitySpace::getDistanceBetweenLineAndLine(Vector2d _l1, Vector2d _l2, Vector2d _l3, Vector2d _l4) {

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
  double VelocitySpace::getDistanceBetweenPolygonAndPolygon(std::vector<Vector2d> &_polygon1, std::vector<Vector2d> &_polygon2) {

    std::vector<Vector2d>::iterator a1, a2, b1, b2;
    double distance, minDistance = 0.0;

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
  void VelocitySpace::rotateMountedPolygon(std::vector<Vector2d> &_polygon, Vector2d _point, double _angle) {

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
  void VelocitySpace::moveMountedPolygon(std::vector<Vector2d> &_polygon, Vector2d _distance) {

    std::vector<Vector2d>::iterator i;

    for(i = _polygon.begin() + 1; i < _polygon.end(); i++) {
      *i = Vector2d(i->real() + _distance.real(), i->imag() + _distance.imag());
    }

  }


  // rotate polygon around offset by given angle
  //
  void VelocitySpace::rotatePolygon(std::vector<Vector2d> &_polygon, Vector2d _point, double _angle) {

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
  void VelocitySpace::movePolygon(std::vector<Vector2d> &_polygon, Vector2d _distance) {

    std::vector<Vector2d>::iterator i;

    for(i = _polygon.begin(); i < _polygon.end(); i++) {
      *i = Vector2d(i->real() + _distance.real(), i->imag() + _distance.imag());
    }

  }


};
