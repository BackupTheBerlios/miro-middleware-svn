// dynamicWindow.h
#ifndef DynamicWindow_h
#define DynamicWindow_h

#include <vector>
#include <complex>

#define VEL_SPACE_LEFT 201
#define VEL_SPACE_RIGHT 201
#define PACE 10

typedef std::complex<double> Vector2d;

namespace Miro
{
  class DynamicWindow 
  {
  public:
    // constructor
    DynamicWindow(Vector2d, int, int);
    // destructor
    ~DynamicWindow();
    // check for collisions for every reachable velocity
    // and update the content of the actual dynamic window
    void collisionCheck(std::vector<Vector2d>&, std::vector<Vector2d>&);
    // calculate the new velocity using the content
    // of the actual dynamic window
    Vector2d calcNewVelocity();
    // set the new velocity and calculate the size
    // of the new dynamicWindow
    void setNewDynamicWindow(Vector2d);
    // set a prefferedDirection and prefferedTranslation
    // and calculate the new content of the velocitySpace
    void setPreferredDirection(double);
    // get distance between two given polygons
    double getPolygonDistance(std::vector<Vector2d>&, std::vector<Vector2d>&);
    // rotate the given polygon around the given point by the given angle
    void rotateMountedPolygon(std::vector<Vector2d>&, Vector2d, double);
    // move the given polygon by the given distance
    void moveMountedPolygon(std::vector<Vector2d>&, Vector2d);
    // get mathematical directed angle between two vectors (0..2*pi)
    double getAngleBetweenVectors(Vector2d, Vector2d);
    // get distance between point and line
    double getDistanceBetweenPointAndLine(Vector2d, Vector2d, Vector2d);	
    
    // 		protected:
    // uncommented for debugging only
    
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
  };
};
#endif

