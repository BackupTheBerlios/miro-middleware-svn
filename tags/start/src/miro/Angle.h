// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroAngle_hh
#define miroAngle_hh

#include <cmath>
#include <iostream>

namespace Miro
{
  // forward declarations
  class Angle;

  // global helper functions declarations

  double rad2Deg(double deg);
  double deg2Rad(double rad);


  // Calculation operators

  /**
   * Add @ref Angle beta to @ref Angle alpha.
   *
   * @return The resulting @ref Angle gamma, which is normalized to -PI < x <= PI.
   */
  Angle operator+(Angle alpha, Angle beta);
  /**
   * Subtract @ref Angle beta from @ref Angle alpha.
   *
   * @return The resulting @ref Angle gamma, which is normalized to -PI < x <= PI.
   */
  Angle operator-(Angle alpha, Angle beta);
  /**
   * Multiply @ref Angle alpha by the scalar val.
   *
   * @return The resulting @ref Angle gamma, which is normalized to -PI < x <= PI.
   */
  Angle operator*(Angle alpha, double val);
  /**
   * Divide @ref Angle alpha by the scalar val.
   *
   * @return The resulting @ref Angle gamma, which is normalized to -PI < x <= PI.
   */
  Angle operator/(Angle alpha, double val);

  // Logical operators

  /**
   * Test whether @ref Angle alpha is equal to @ref Angle beta.
   */
  bool operator==(Angle alpha, Angle beta);
  /**
   * Test whether @ref Angle alpha is not equal to @ref Angle beta.
   */
  bool operator!=(Angle alpha, Angle beta);
  /**
   * Test whether @ref Angle alpha greater than to @ref Angle beta.
   */

  // IO friend functions

  /**
   * Ostream operator.
   *
   * Print the @ref Angle alpha on the stream ostr.
   * The value is printed as a double representing the radiant form.
   *
   * @return A reference to the ostream.
   */
  std::ostream& operator<<(std::ostream& ostr, Angle alpha);
  /**
   * Istream operator.
   *
   * Read the @ref Angle alpha from the stream istr.
   * The value is expected to be a double representing the radiant form.
   *
   * @return A reference to the istream.
   */
  std::istream& operator>>(std::istream& istr,  Angle& alpha);

  /**
   * Class to simplify mathematics with angles.
   *
   * An Angle is asserted to be always within the range of
   * -PI < Angle <= PI.
   * The internal representation of the data type is a double, 
   * storing the angle in the radiant representation.
   */
  class Angle
  {
    typedef Angle self;

  public:
    // Constructors

    /**
     * Default constructor.()
     *
     * Initializes the angle to 0.
     */
    Angle() : 
      angle(0.) 
    {}
    /**
     * Initializing constructor: radiant.
     *
     * Initializes the angle to the in radiant specified value.
     */
    explicit Angle(double alpha)  : 
      angle(alpha) 
    { normalize(); }
    /**
     * Copy constructor.
     *
     * Initializes the angle by the given Angle alpha.
     */
    Angle(const Angle& alpha) : 
      angle(alpha) 
    {}

    // Access methods

    /**
     * Get the Angle in radiant.
     */
    double rad() const { 
      return angle; 
    }
    /**
     * Get the Angle in degree.
     */
    double deg() const { 
      return (180. / M_PI * angle); 
    }
    /**
     * Set the Angle in radiant.
     */
    void setRad(double alpha) { 
      angle = alpha; normalize(); 
    }
    /**
     * Set the Angle in degree.
     */
    void setDeg(double alpha) { 
      angle = (M_PI / 180.  * alpha); normalize(); 
    }

    // Type conversion

    /**
     * Conversion from double: radiant.
     *
     * Converts an Angle to a double of the in radiant specified value.
     */ 
    operator double () const { 
      return angle; 
    }

    // Assignment operators

    /**
     * Self assignment.
     *
     * You can assign an Angle to another Angle. Wow!
     *
     * @return Reference to self.
     */ 
    Angle& operator = (Angle alpha) { 
      angle = alpha.angle; 
      return *this; 
    }
    /**
     * Assignment from double: radiant.
     *
     * Converts a double to an angle of the in radiant specified value
     * and assigns it to the Angle.
     *
     * @return Reference to self.
     */ 
    Angle& operator = (double alpha) { 
      angle = alpha; 
      normalize();
      return *this; 
    }

    // Unary operators

    /**
     * The positive Angle.
     *
     * Actually this method does nothing, but is presented for
     * mathematical completeness.
     *
     * @return Self.
     */
    Angle operator+() const { 
      return Angle(angle); 
    }
    /**
     * The negative Angle.
     *
     * Vied in geometrical terms, the Angle is mirrored a the x axis.
     */
    Angle operator-() const { 
      return Angle(-angle); 
    }
    
    // Combined assignment - calculation operators

    /**
     * In place addition.
     *
     * Add Angle alpha to this Angle.
     *
     * @return Reference to self.
     */
    Angle& operator+=(Angle alpha) { 
      angle += alpha.angle; 
      normalize1(); 
      return *this; 
    }
    /**
     * In place subtraction.
     *
     * Subtract Angle alpha from this Angle.
     *
     * @return Reference to self.
     */
    Angle& operator-=(Angle alpha) { 
      angle -= alpha.angle;
      normalize1(); 
      return *this; 
    }
    /**
     * In place addition.
     *
     * Add the scalar val to this Angle. 
     *
     * @return Reference to self.
     */
    Angle& operator+=(double val) { 
      angle += val;
      normalize(); 
      return *this; 
    }
    /**
     * In place subtraction.
     *
     * Subtract the scalar val to this Angle. 
     *
     * @return Reference to self.
     */
    Angle& operator-=(double val) {
      angle -= val;
      normalize();
      return *this; 
    }
    /**
     * In place scalar multiplication.
     *
     * Multiply the Angle by the scalar val.
     *
     * @return Reference to self.
     */
    Angle& operator*=(double val) {
      angle *= val; 
      normalize();
      return *this; 
    }
    /**
     * In place scalar division.
     *
     * Divide the Angle by the scalar val.
     *
     * @return Reference to self.
     */
    Angle& operator/=(double val) { 
      angle /= val; 
      normalize(); 
      return *this; 
    }

    // Calculation operators
    friend Angle    operator +  (Angle alpha, Angle beta);
    friend Angle    operator +  (Angle alpha, double beta);
    friend Angle    operator +  (double alpha, Angle beta);
    friend Angle    operator -  (Angle alpha, Angle beta);
    friend Angle    operator -  (Angle alpha, double beta);
    friend Angle    operator -  (double alpha, Angle beta);
    friend Angle    operator *  (Angle alpha, double val);
    friend Angle    operator /  (Angle alpha, double val);

  // Logical operators
    friend bool     operator == (Angle alpha, Angle beta);
    friend bool     operator != (Angle alpha, Angle beta);
    friend bool     operator <  (Angle alpha, Angle beta);

  // IO friend functions
    friend std::ostream& operator << (class std::ostream& ostr, Angle alpha);
    friend std::istream& operator >> (class std::istream& istr,  Angle& alpha);
 
  protected:
    void normalize();
    void normalize1();

  private:
    double angle;
  };

  inline
  void 
  Angle::normalize() { 
    while (angle > M_PI) angle -= 2. * M_PI;
    while (angle <= -M_PI) angle += 2. * M_PI;
  }  

  inline
  void 
  Angle::normalize1() { 
    if (angle > M_PI) 
      angle -= 2. * M_PI;
    else if (angle <= -M_PI) 
      angle += 2. * M_PI;
  }  

  // helper functions
  inline
  double deg2Rad(double deg) {
    return deg * M_PI / 180;
  }
  inline
  double rad2Deg(double rad) {
    return rad * 180 * M_1_PI;
  }

  // Calculation operators
  inline
  Angle
  operator + (Angle alpha, Angle beta) {
    return Angle(alpha.angle + beta.angle);
  }
  inline
  Angle
  operator + (Angle alpha, double beta) {
    return Angle(alpha.angle + beta);
  }
  inline
  Angle
  operator + (double alpha, Angle beta) {
    return Angle(alpha + beta.angle);
  }
  inline
  Angle
  operator - (Angle alpha, Angle beta) {
    return Angle(alpha.angle - beta.angle);
  }
  inline
  Angle
  operator - (Angle alpha, double beta) {
    return Angle(alpha.angle - beta);
  }
  inline
  Angle
  operator - (double alpha, Angle beta) {
    return Angle(alpha - beta.angle);
  }
  inline
  Angle
  operator * (Angle alpha, double val) {
    return Angle(alpha.angle * val);
  }
  inline
  Angle
  operator / (Angle alpha, double val) {
    return Angle(alpha.angle / val);
  }

  // Logical operators
  inline
  bool 
  operator == (Angle alpha, Angle beta) {
    return alpha.angle == beta.angle;
  }
  inline
  bool 
  operator != (Angle alpha, Angle beta) {
    return alpha.angle != beta.angle;
  }
  inline
  bool
  operator < (Angle alpha, Angle beta) {
    return alpha.angle < beta.angle;
  }
};

#endif




