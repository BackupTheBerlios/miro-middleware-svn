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
#ifndef VelocitySpace_h
#define VelocitySpace_h

#include <vector>
#include <complex>

namespace Miro
{
  // forward declarations
  class DifferentialMotionParameters;
  class ConstraintArbiterViewer;

  //! A 2d vector, represented as complex number.
  typedef std::complex<double> Vector2d;
  //! A poligon, represented as vector of 2d vectors.
  typedef std::vector<Vector2d> Polygon;

  //! Class for calcuations on the velocity space.
  class VelocitySpace
  {
  public:
    //! Constructor.
    VelocitySpace();
    //! Copy constructor.
    VelocitySpace(VelocitySpace const& _rhs);
    //! Destructor cleaning up the velocity space.
    ~VelocitySpace();

    void setWheelBase(double _wheelBase);
    void init(int _maxVelocity, int _spaceResolution,
	      int _maxAccel, int _maxDecel, int _pace);

    /**
     * add evaluations for given, preferred direction with given,
     * maximum speed to velocity space
     */
    void addEvalForPreferredDirection(double, double);

    void addEvalForStraightVelocity(double, double);

    // add an evaluation for a preferred velocity
    void addEvalForVelocity(Vector2d const& _velocity);
    // clear all evaluations
    void clearAllEvals(void);
    // add evaluations for given obstacle to velocity space
    void addEvalForObstacle(Polygon&, Polygon&);
    // obtain new velocity, by applying objective function to evaluations in velocity space
    Vector2d applyObjectiveFunctionToEval();

  protected:
    // set new VelocitySpace
    void setNewVelocity(Vector2d const& _velocity);
    // get signed distance between point and line
    double getSignedDistanceBetweenPointAndLine(Vector2d const& _p1, 
						Vector2d const& _l1,
						Vector2d const& _l2);
    // get distance between two lines
    double getDistanceBetweenLineAndLine(Vector2d const& _l1, 
					 Vector2d const& _l2, 
					 Vector2d const& _l3, 
					 Vector2d const& _l4);
    // get distance between two mounted polygons
    double getDistanceBetweenPolygonAndPolygon(Polygon const& _polygon1,
					       Polygon const& _polygon2);
    // rotate mounted polygon around offset by angle
    void rotateMountedPolygon(Polygon&, Vector2d const&, double);
    // move mounted polygon by distance
    void moveMountedPolygon(Polygon&, Vector2d const&);
    // rotate polygon around offset by angle
    void rotatePolygon(Polygon&, Vector2d const&, double);
    // move polygon by distance
    void movePolygon(Polygon&, Vector2d const&);
    // get index of velocity space array by velocity
    int getIndexByVelocity(double);
    // get velocity by index of velocity space array
    double getVelocityByIndex(int);

    void lr2velocity(double left, double right, Vector2d& velocity);
    void velocity2lr(Vector2d const& velocity, double& left, double& right);

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

    //! The space between the two wheels.
    double wheelBase_;
    //! Raw velocity space.
    int * space_;
    //! The velocity space column index.
    int** velocitySpace_;

    //! My really good friend the graphical monitoring frontend.
    friend class ConstraintArbiterViewer;
  };

  // calculate index of velocity space array for given velocity
  //
  inline
  int
  VelocitySpace::getIndexByVelocity(double _velocity) {
    return (std::max(0,
		     std::min(2 * maxVelocity_, 
			      maxVelocity_ + (int)_velocity)) / spaceResolution_);
  }

	
  // calculate velocity for given index of velocity space array
  //
  inline
  double 
  VelocitySpace::getVelocityByIndex(int _index) {
    return (std::max(-maxVelocity_,
		     std::min(maxVelocity_, 
			      (_index * spaceResolution_) - maxVelocity_)));
  }

  inline
  void
  VelocitySpace::lr2velocity(double _left, double _right, Vector2d& _velocity)
  {
    _velocity = Vector2d((_left + _right) / 2., 
			 (_right - _left) / wheelBase_);
  }

  inline
  void
  VelocitySpace::velocity2lr(Vector2d const& _velocity, 
			     double& _left, double& _right)
  {
    _left = _velocity.real();
    _right = _velocity.real();

    double delta = _velocity.imag() * wheelBase_ * .5;

    _left -= delta;
    _right += delta;
  }
}
#endif

