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

#ifndef VelocitySpace_h
#define VelocitySpace_h

#include <vector>
#include <complex>

// florian: const static member variables?
#define VEL_SPACE_LEFT 201
#define VEL_SPACE_RIGHT 201
#define PACE 5

namespace Miro
{
  typedef std::complex<double> Vector2d;

  class VelocitySpace
  {
  public:
    // constructor
    VelocitySpace(Vector2d, int, int);
    // destructor
    ~VelocitySpace();
    // copy function
    void getCopy(Miro::VelocitySpace *);

    // add evaluations for given, preferred direction to velocity space
    void addEvalForPreferredDirection(double);
    // add evaluations for given obstacle to velocity space
    void addEvalForObstacle(std::vector<Vector2d>&, std::vector<Vector2d>&);
    // obtain new velocity, by applying objective function to evaluations in velocity space
    Vector2d applyObjectiveFunctionToEval();

    // the velocity space
    int velocitySpace_[VEL_SPACE_LEFT][VEL_SPACE_RIGHT];
    // size of the dynamic window
    int minLeft_;
    int maxLeft_;
    int minRight_;
    int maxRight_;
    // maximum positive and negative acceleration
    int maxPosAccel_;
    int maxNegAccel_;
    // actual velocity
    Vector2d velocity_;


  private:
    // set new VelocitySpace
    void setNewVelocitySpace(Vector2d);
    // get signed distance between point and line
    double getSignedDistanceBetweenPointAndLine(Vector2d, Vector2d, Vector2d);
    // get distance between two lines
    double getDistanceBetweenLineAndLine(Vector2d, Vector2d, Vector2d, Vector2d);
    // get distance between two mounted polygons
    double getDistanceBetweenPolygonAndPolygon(std::vector<Vector2d>&, std::vector<Vector2d>&);
    // rotate mounted polygon around offset by angle
    void rotateMountedPolygon(std::vector<Vector2d>&, Vector2d, double);
    // move mounted polygon by distance
    void moveMountedPolygon(std::vector<Vector2d>&, Vector2d);
    // rotate polygon around offset by angle
    void rotatePolygon(std::vector<Vector2d>&, Vector2d, double);
    // move polygon by distance
    void movePolygon(std::vector<Vector2d>&, Vector2d);
    
  };
};
#endif

