// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
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
#include "Angle.h"

namespace Miro
{

  // public methods
  //
  //


  // constructor
  //
  VelocitySpace::VelocitySpace() :
    maxVelocity_(0),		// in mm/sec
    spaceResolution_(0),	// in mm
    maxAccel_(0),		// in mm/sec2
    maxDecel_(0),		// in mm/sec2
    pace_(0),			// in times/sec
    wheelBase_(0),
    space_(NULL),
    velocitySpace_(NULL)
  {
  }

  VelocitySpace::VelocitySpace(VelocitySpace const& _rhs) :
    maxVelocity_(_rhs.maxVelocity_),
    spaceResolution_(_rhs.spaceResolution_),
    maxAccel_(_rhs.maxAccel_),
    maxDecel_(_rhs.maxDecel_),
    pace_(_rhs.pace_),
    wheelBase_(_rhs.wheelBase_),
    space_(NULL),
    velocitySpace_(NULL)
  {
    init(maxVelocity_, spaceResolution_,
	 maxAccel_, maxDecel_, pace_);
  }

  VelocitySpace::VelocitySpace(int _maxVelocity,
			       int _spaceResolution,
			       int _maxAccel,
			       int _maxDecel,
			       int _pace) :
    maxVelocity_(_maxVelocity),		// in mm/sec
    spaceResolution_(_spaceResolution),	// in mm
    maxAccel_(_maxAccel),		// in mm/sec2
    maxDecel_(_maxDecel),		// in mm/sec2
    pace_(_pace)			// in times/sec
  {
    // init member variables for given parameters


    // set up velocity space
    int size = 2 * (maxVelocity_/spaceResolution_) + 1;
    space_ = new int[size * size];
    velocitySpace_ = new int*[size];
    for(int i = 0; i < size; i++) {
      velocitySpace_[i] = space_ + i * size;
    }

    // set initial dynamic window
    setNewVelocity(Vector2d(0., 0.));
  }



  // destructor
  //
  VelocitySpace::~VelocitySpace()
  {
    delete[] space_;
    delete[] velocitySpace_;
  }

  void
  VelocitySpace::setWheelBase(double _wheelBase)
  {
    wheelBase_ = _wheelBase;
  }

  void
  VelocitySpace::init(int _maxVelocity, int _spaceResolution,
		      int _maxAccel, int _maxDecel, int _pace)
  {
    // clean up old variables
    delete[] space_;
    delete[] velocitySpace_;

    // set member variables
    maxVelocity_ = _maxVelocity;
    spaceResolution_ = _spaceResolution;
    maxAccel_ = _maxAccel;
    maxDecel_ = _maxDecel;
    pace_ = _pace;

    // set up velocity space
    int size = 2 * (maxVelocity_/spaceResolution_) + 1;
    space_ = new int[size * size];
    velocitySpace_ = new int*[size];
    for(int i = 0; i < size; i++) {
      velocitySpace_[i] = space_ + i * size;
    }

    // set initial dynamic window
    setNewVelocity(Vector2d(0., 0.));
  }

  // add evaluation for preferred direction
  //
  void
  VelocitySpace::addEvalForPreferredDirection(double _prefDir, double _maxSpeed)
  {
    double l_value, r_value, left, right;
    if(fabs(_prefDir) <= M_PI_2)
	_prefDir = -_prefDir;
    std::cout << "addEvalForPrefferredDirection" << std::endl;
    for(int l_index = minDynWinLeft_; l_index <= maxDynWinLeft_; l_index++) {
      for(int r_index = minDynWinRight_; r_index <= maxDynWinRight_; r_index++) {

	left = getVelocityByIndex(l_index);
	right = getVelocityByIndex(r_index);
        if(fabs(left) < spaceResolution_ && fabs(right) < spaceResolution_){
           left = 1.0;
           right = 1.0;
        }

        l_value =
	  cos(_prefDir - M_PI_4) * left -
	  sin(_prefDir - M_PI_4) * right;
	r_value =
	  sin(_prefDir - M_PI_4) * left +
	  cos(_prefDir - M_PI_4) * right;

	if (fabs(left) < _maxSpeed && fabs(right) < _maxSpeed){
            velocitySpace_[l_index][r_index] =
	    (int)rint(255. * ((1. +
			      atan2(l_value, fabs(r_value)) / M_PI_2) / 2.));

        }
	else
	  velocitySpace_[l_index][r_index] = 0;
      }
    }
    std::cout << "addEvalForPrefferedDingens end" << std::endl;
  }

  void VelocitySpace::addEvalForStraightVelocity(double _prefDir, double _maxSpeed)
  {
    double l_value, r_value, left, right;
    double axis_direction;
    double axis_value;
    double v_dist;

    if(fabs(_prefDir) <= M_PI_2)
	_prefDir = -_prefDir;

    for(int l_index = minDynWinLeft_; l_index <= maxDynWinLeft_; l_index++) {
      for(int r_index = minDynWinRight_; r_index <= maxDynWinRight_; r_index++) {

	left = getVelocityByIndex(l_index);
	right = getVelocityByIndex(r_index);
        v_dist = sqrt(left*left + right*right);

        if(fabs(left) < spaceResolution_ && fabs(right) < spaceResolution_){
           left = 1.0;
           right = 1.0;
        }

        l_value =
	  cos(_prefDir - M_PI_4) * left -
	  sin(_prefDir - M_PI_4) * right;
	r_value =
	  sin(_prefDir - M_PI_4) * left +
	  cos(_prefDir - M_PI_4) * right;

//        double lrAngle = atan2(right, left) - M_PI_4;
//        Miro::Angle::normalize(lrAngle);
//        if((fabs(lrAngle) < M_PI_2 && lrAngle > _curvature1 && lrAngle < _curvature2)||
//	   (fabs(lrAngle) >= M_PI_2 && (lrAngle > _curvature1+M_PI || lrAngle < _curvature2-M_PI))){

	   if (fabs(left) < _maxSpeed && fabs(right) < _maxSpeed){//v_dist <= _maxSpeed){
               axis_direction = (fabs(atan2(r_value, l_value)) > M_PI_2)?-1.0:1.0;
               axis_value = ((axis_direction*v_dist)/(abs(maxVelocity_)/sqrt(2.0)) + 1.0)/2.0;
               velocitySpace_[l_index][r_index] =
	       (int)rint(255. * (((1. +
			      atan2(fabs(l_value), fabs(r_value)) / M_PI_2) / 2.)*axis_value));

           }
	   else
	     velocitySpace_[l_index][r_index] = 0;
 //       }
 //       else
 //          velocitySpace_[l_index][r_index] = 0;
      }
    }
  }

  /**
   * velocity.real() = translation, velocity.imag() = rotation
   */  
  void
  VelocitySpace::addEvalForVelocity(Vector2d const& _velocity) 
  {
#ifdef ASDF
    for(int l_index = minDynWinLeft_; l_index <= maxDynWinLeft_; l_index++) {
      for(int r_index = minDynWinRight_; r_index <= maxDynWinRight_; r_index++) {

	double left = getVelocityByIndex(l_index);
	double right = getVelocityByIndex(r_index);
	double l_value = 
	  cos(_prefDir - M_PI_4) * left -
	  sin(_prefDir - M_PI_4) * right;
	double r_value =
	  sin(_prefDir - M_PI_4) * left +
	  cos(_prefDir - M_PI_4) * right;

        double v_dist = sqrt(left*left + right*right);
	if (v_dist <= _maxSpeed){
            axis_direction = (fabs(atan2(l_value, r_value)) > M_PI_2)?-1.0:1.0;
            axis_value = ((axis_direction*v_dist)/abs(maxVelocity_) + 1.0)/2.0;
            velocitySpace_[l_index][r_index] =
	    (int)rint(255. * (((1. +
			      atan(fabs(l_value /r_value)) / M_PI_2) / 2.)*axis_value));

        }
	else
	  velocitySpace_[l_index][r_index] = 0;
      }
    }
#endif
  }

  // clear all evaluations
  //
  void VelocitySpace::clearAllEvals(void) {
    for(int l_index = 0; l_index < 2*(maxVelocity_/spaceResolution_)+1; l_index++) {
      for(int r_index = 0; r_index < 2*(maxVelocity_/spaceResolution_)+1; r_index++) {
        velocitySpace_[l_index][r_index] = 0;
      }
    }
  }


  // add evaluation for obstacle
  //

  void
  VelocitySpace::addEvalForObstacle(Polygon &_robot,
				    Polygon &_obstacle)
  {

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
	rotateMountedPolygon(_robot, Vector2d(0., offset), angle * (CURV_RES + 1));

	// rotate stepwise forwards and check for collisions
	for(seg = 0; seg < 2*CURV_RES+1; seg++) {
	  rotateMountedPolygon(_robot, Vector2d(0., offset), -angle);
          if(getDistanceBetweenPolygonAndPolygon(_robot, _obstacle) == 0) {
	    CURV[count][seg] = 0;
	  }
          else {
	    CURV[count][seg] = 250;
	  }
	}

	// rotate backwards to middle position
	rotateMountedPolygon(_robot, Vector2d(0., offset), angle * CURV_RES);

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

    for(int l_index = minDynWinLeft_; l_index <= maxDynWinLeft_; l_index++) {
      for(int r_index = minDynWinRight_; r_index <= maxDynWinRight_; r_index++) {

	// get left/right velocity from array indices
	left = getVelocityByIndex(l_index);
	right = getVelocityByIndex(r_index);
	// calculating actual curvature
	curv = (int)((atan((double)left / (double)right) + (M_PI / 2.)) * 2 * CURV_CNT / M_PI);
	// calculating breaking distance
	left_break = ((fabs((double)left)) / BREAK_ACCEL) * (double)left;
	right_break = ((fabs((double)right)) / BREAK_ACCEL) * (double)right;
	if(fabs(left_break) >= fabs(right_break))
	  break_dist = left_break;
	else
	  break_dist = right_break;

	if((left+right)>0)
	  target = CURV_RES  + CURV_DELAY + (int)(break_dist  * (double)CURV_RES / (double)CURV_LEN);
	else
	  target = CURV_RES - CURV_DELAY + (int)(break_dist * (double)CURV_RES / (double)CURV_LEN);

	velocitySpace_[l_index][r_index] = (int)((double)CURV[std::max(0,std::min(2*CURV_CNT,curv))][std::max(0,std::min(2*CURV_RES,target))]
	    * (double)velocitySpace_[l_index][r_index] / 250.);

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
  Vector2d
  VelocitySpace::applyObjectiveFunctionToEval()
  {
    int biggestEval = 10;		// start with a threshold of 10
    double biggestRad = 0.;
    Vector2d biggestValueVelocity(0., 0.);
    bool found = false;
    double left, right;
    double temp_left, temp_right;

    // destinate best entry
    for(int l_index = minDynWinLeft_; l_index <= maxDynWinLeft_; l_index++) {
      for(int r_index = minDynWinRight_; r_index <= maxDynWinRight_; r_index++) {

	left = getVelocityByIndex(l_index);
	right = getVelocityByIndex(r_index);
	//double	rel = atan(left/right);

        if(fabs(left) < spaceResolution_ && fabs(right) < spaceResolution_){
           left = 0.5;
           right = 0.5;
        }

        double lrAngle = atan2(right, left) - M_PI_4;
        Miro::Angle::normalize(lrAngle);
        if(!((lrAngle > posCurvConstrB_ && lrAngle < posCurvConstrE_)||
             (lrAngle < negCurvConstrB_ && lrAngle > negCurvConstrE_))){


	   if(    (velocitySpace_[l_index][r_index] > biggestEval)
	       || (velocitySpace_[l_index][r_index] == biggestEval
	          && (left*left)+(right*right) > biggestRad )    )
	   {
             biggestEval = velocitySpace_[l_index][r_index];
	     biggestRad = (left * left) + (right * right);
	     biggestValueVelocity = Vector2d(left,right);
	     found = true;
           }
         }
       }
    }

    // if no suitable entry found, slow down until stop
    if(found==false) {
      if(velocity_.real() < 0.) {
	temp_left = std::min(0., velocity_.real() + maxAccel_ / pace_);
      }
      else {
        temp_left = std::max(0., velocity_.real() - maxDecel_ / pace_);
      }
      if(velocity_.imag() < 0.) {
        temp_right = std::min(0., velocity_.imag() + maxAccel_ / pace_);
      }
      else {
        temp_right = std::max(0., velocity_.imag() - maxDecel_ / pace_);
      }
      biggestValueVelocity = Vector2d(temp_left, temp_right);
    }

    // set new velocity
    setNewVelocity(biggestValueVelocity);

    return biggestValueVelocity;
  }


  // private methods
  //


  // set new velocity
  //
  void
  VelocitySpace::setNewVelocity(Vector2d const& _velocity)
  {
    velocity_ = _velocity;
    minDynWinLeft_ = getIndexByVelocity((int)velocity_.real() - (maxDecel_ / pace_));
    minDynWinRight_ = getIndexByVelocity((int)velocity_.imag() - (maxDecel_ / pace_));
    maxDynWinLeft_ = getIndexByVelocity((int)velocity_.real() + (maxAccel_ / pace_));
    maxDynWinRight_ = getIndexByVelocity((int)velocity_.imag() + (maxAccel_ / pace_));
  }


  // get signed distance between point and line
  //
  double
  VelocitySpace::getSignedDistanceBetweenPointAndLine(Vector2d const& _p1,
						      Vector2d const& _l1,
						      Vector2d const& _l2)
  {
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
  double
  VelocitySpace::getDistanceBetweenLineAndLine(Vector2d const& _l1,
					       Vector2d const& _l2,
					       Vector2d const& _l3,
					       Vector2d const& _l4)
  {
    double l1_to_l3_l4 = getSignedDistanceBetweenPointAndLine(_l1, _l3, _l4);
    double l2_to_l3_l4 = getSignedDistanceBetweenPointAndLine(_l2, _l3, _l4);
    double l3_to_l1_l2 = getSignedDistanceBetweenPointAndLine(_l3, _l1, _l2);
    double l4_to_l1_l2 = getSignedDistanceBetweenPointAndLine(_l4, _l1, _l2);

    if( ((l1_to_l3_l4 * l2_to_l3_l4) < 0) &&
	((l3_to_l1_l2 * l4_to_l1_l2) < 0) )
      return 0;

    return fabs(std::min(std::min(l1_to_l3_l4, l2_to_l3_l4),
			 std::min(l3_to_l1_l2, l4_to_l1_l2)));

  }


  // get distance between two mounted polygons
  //
  double
  VelocitySpace::getDistanceBetweenPolygonAndPolygon(Polygon const& _polygon1,
						     Polygon const& _polygon2)
  {
    Polygon::const_iterator a1, a2, b1, b2;
    double distance, minDistance = 0.0;

    for(a1 = _polygon1.begin(), a2 = _polygon1.begin() + 1; a2 < _polygon1.end(); a1++, a2++) {
      for(b1 = _polygon2.begin(), b2 = _polygon2.begin() + 1; b2 < _polygon2.end(); b1++, b2++) {

        distance = getDistanceBetweenLineAndLine(*a1, *a2, *b1, *b2);

        if(( (a1 == _polygon1.begin()) &&
	     (b1 == _polygon2.begin()) ) ||
	   (distance < minDistance)) {
          minDistance = distance;
        }
      }
    }

    return minDistance;
  }


  // rotate mounted polygon around offset by given angle
  //
  void
  VelocitySpace::rotateMountedPolygon(Polygon &_polygon,
				      Vector2d const& _point, double _angle)
  {
    Polygon::iterator i;

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
  void 
  VelocitySpace::moveMountedPolygon(Polygon &_polygon, Vector2d const& _distance) 
  {
    Polygon::iterator i;

    for(i = _polygon.begin() + 1; i < _polygon.end(); i++) {
      *i += _distance;
    }
  }


  // rotate polygon around offset by given angle
  //
  void VelocitySpace::rotatePolygon(Polygon &_polygon, 
				    Vector2d const&_point, double _angle) 
  {
    Polygon::iterator i;

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
  void 
  VelocitySpace::movePolygon(Polygon &_polygon, Vector2d const& _distance)
  {
    Polygon::iterator i;

    for(i = _polygon.begin(); i < _polygon.end(); i++) {
      *i += _distance;
    }
  }



  void VelocitySpace::clearCurvatureConstraints()
  {
     posCurvConstrB_ = 1.5*M_PI;
     posCurvConstrE_ = -1.5*M_PI;
     negCurvConstrB_ = -1.5*M_PI;
     negCurvConstrE_ = 1.5*M_PI;

  }

  void VelocitySpace::addPositiveCurvatureConstraint(double _constrB, double _constrE)
  {

     posCurvConstrB_ = _constrB;
     posCurvConstrE_ = _constrE;


  }

  void VelocitySpace::addNegativeCurvatureConstraint(double _constrB, double _constrE)
  {

     negCurvConstrB_ = _constrB;
     negCurvConstrE_ = _constrE;

  }


}
