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

namespace Miro
{
  typedef std::complex<double> Vector2d;

  class VelocitySpace
  {
  public:
    // constructor
    VelocitySpace(int, int, int, int, int);
    // destructor
    ~VelocitySpace();
    // copy function
    void getCopy(Miro::VelocitySpace *);

    // add evaluations for given, preferred direction with given, maximum speed to velocity space
    void addEvalForPreferredDirection(double, double);
    // add evaluations for given obstacle to velocity space
    void addEvalForObstacle(std::vector<Vector2d>&, std::vector<Vector2d>&);
    // obtain new velocity, by applying objective function to evaluations in velocity space
    Vector2d applyObjectiveFunctionToEval();

    // the velocity space
    int** velocitySpace_;
    // maximum speed, speed resolution, maximum acceleration and maximum deceleretion
    int maxVelocity_;
    int spaceResolution_;
    int maxAccel_;
    int maxDecel_;
    int pace_;
    // size of the dynamic window
    int minDynWinLeft_;
    int maxDynWinLeft_;
    int minDynWinRight_;
    int maxDynWinRight_;
    // actual velocity
    Vector2d velocity_;


  private:
    // set new VelocitySpace
    void setNewVelocity(Vector2d);
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
    // get index of velocity space array by velocity
    int getIndexByVelocity(double);
    // get velocity by index of velocity space array
    double getVelocityByIndex(int);
    
  };
};
#endif

