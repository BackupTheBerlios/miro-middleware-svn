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

#ifndef DynamicWindow_h
#define DynamicWindow_h

#include <vector>
#include <complex>

// florian: const static member variables?
#define VEL_SPACE_LEFT 201
#define VEL_SPACE_RIGHT 201
#define PACE 10

namespace Miro
{
  typedef std::complex<double> Vector2d;

  class DynamicWindow 
  {
  public:
    // constructor
    DynamicWindow(Vector2d, int, int);
    // destructor
    ~DynamicWindow();

    // florian: const &    // florian: const vector , const ?
    void setPreferredDirection(double);

    // florain: const !
    void collisionCheck(std::vector<Vector2d>&, std::vector<Vector2d>&);
    // calculate the new velocity using the content
    // of the actual dynamic window

    // florian: const ?
    Vector2d calcNewVelocity();
    // set the new velocity and calculate the size
    // of the new dynamicWindow

		
    
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
    // set new DynamicWindow
    void setNewDynamicWindow(Vector2d);
    // get signed distance between point and line
    double getSignedDistanceBetweenPointAndLine(Vector2d, Vector2d, Vector2d);	
    // get distance between two lines
    double getDistanceBetweenLineAndLine(Vector2d, Vector2d, Vector2d, Vector2d);
    // get distance between two mounted polygons
    double getDistanceBetweenPolygonAndPolygon(std::vector<Vector2d>&, std::vector<Vector2d>&);
    // rotate mounted polygon around offset by given angle
    void rotateMountedPolygon(std::vector<Vector2d>&, Vector2d, double);
    // move the given polygon by the given distance
    void moveMountedPolygon(std::vector<Vector2d>&, Vector2d);
    
  };
};
#endif

