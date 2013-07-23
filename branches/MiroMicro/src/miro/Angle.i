// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

namespace Miro
{

  inline
  void
  Angle::normalize(double& _alpha)
  {
    if (_alpha < 10. && _alpha > -10.) {
      while (_alpha > M_PI)
        _alpha -= 2. * M_PI;
      while (_alpha <= -M_PI)
        _alpha += 2. * M_PI;
      return;
    }

    _alpha = asin(sin(_alpha));
  }

  inline
  void
  Angle::normalize1(double& _alpha)
  {
    if (_alpha > M_PI)
      _alpha -= 2. * M_PI;
    else if (_alpha <= -M_PI)
      _alpha += 2. * M_PI;
  }

  inline
  void
  Angle::normalize()
  {
    if (angle_ < 10. && angle_ > -10) {
      while (angle_ > M_PI)
        angle_ -= 2. * M_PI;
      while (angle_ <= -M_PI)
        angle_ += 2. * M_PI;

      return;
    }

    angle_ = asin(sin(angle_));
  }

  inline
  void
  Angle::normalize1()
  {
    if (angle_ > M_PI)
      angle_ -= 2. * M_PI;
    else if (angle_ <= -M_PI)
      angle_ += 2. * M_PI;
  }

  // helper functions
  inline
  double deg2Rad(double _deg)
  {
    return _deg * M_PI / 180.;
  }
  inline
  double rad2Deg(double _rad)
  {
    return _rad * 180. * M_1_PI;
  }

  // Calculation operators
  inline
  Angle
  operator + (Angle alpha, Angle beta)
  {
    return Angle(alpha.angle_ + beta.angle_);
  }
  inline
  Angle
  operator + (Angle alpha, double beta)
  {
    return Angle(alpha.angle_ + beta);
  }
  inline
  Angle
  operator + (double alpha, Angle beta)
  {
    return Angle(alpha + beta.angle_);
  }
  inline
  Angle
  operator - (Angle alpha, Angle beta)
  {
    return Angle(alpha.angle_ - beta.angle_);
  }
  inline
  Angle
  operator - (Angle alpha, double beta)
  {
    return Angle(alpha.angle_ - beta);
  }
  inline
  Angle
  operator - (double alpha, Angle beta)
  {
    return Angle(alpha - beta.angle_);
  }
  inline
  Angle
  operator * (Angle alpha, double val)
  {
    return Angle(alpha.angle_ * val);
  }
  inline
  Angle
  operator / (Angle alpha, double val)
  {
    return Angle(alpha.angle_ / val);
  }

  // Logical operators
  inline
  bool
  operator == (Angle alpha, Angle beta)
  {
    return alpha.angle_ == beta.angle_;
  }
  inline
  bool
  operator != (Angle alpha, Angle beta)
  {
    return alpha.angle_ != beta.angle_;
  }
  inline
  bool
  operator < (Angle alpha, Angle beta)
  {
    return alpha.angle_ < beta.angle_;
  }
}
