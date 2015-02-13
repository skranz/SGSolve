// Classes for storing data for stochastic games.
// BAB 9-25-2012
#ifndef _SGPOINT_HPP
#define _SGPOINT_HPP

#include "sgcommon.hpp"

//! A vector in \f$\mathbb{R}^2\f$
/*! A simple two-dimensional vector that supports arithmetic operations. */
class SGPoint
{
protected:
  vector<double> xy; /*!< Two dimensional vector of doubles. */

public:
  //! Default constructor that sets vector equal to zero.
  SGPoint():  xy(2,0.0) {}
  //! Sets both elements of the vector equal to x.
  SGPoint(double x): xy(2,x) {}
  //! Creates an SGPoint from the two-vector _xy. 
  SGPoint(vector<double> _xy)
  {
    assert(_xy.size()==2);
    xy = _xy;
  }
  //! Creates an SGPoint with elements x and y.
  SGPoint(double x, double y): xy(2,0.0)
  { xy[0] = x; xy[1] = y;  }

  ~SGPoint(){}

  //! Returns the counter-clockwise normal vector.
  SGPoint getNormal() const;
  //! Returns the Euclidean norm.
  double norm() const {return std::sqrt((*this)*(*this));}
  //! Returns the angle in radians relative to base.
  double angle(const SGPoint& base) const;
  //! Calculates distance between p0 and p1 in the sup-norm.
  static double distance(const SGPoint& p0,
			 const SGPoint& p1);

  //! Rounds off significant digits smaller than tol.
  void roundPoint(double tol);

  //! Takes the pointwise minimum with another SGPoint
  void max(const SGPoint & p);
  //! Takes the pointwise minimum with another SGPoint
  void min(const SGPoint & p);

  // Operators
  //! Access operator.
  double& operator[](int player);
  //! Constant access operator.
  const double& operator[](int player) const;
  //! Assignment operator
  SGPoint& operator=(const SGPoint & rhs);
  //! Sets both coordinates equal to d.
  SGPoint& operator=(double d);
  //! Augmented addition
  SGPoint& operator+=(const SGPoint & rhs);
  //! Augmented subtraction
  SGPoint& operator-=(const SGPoint & rhs);
  //! Augmented scalar multiplication
  SGPoint& operator*=(double d);
  //! Augmented scalar division
  SGPoint& operator/=(double d);
  //! Vector addition
  const SGPoint operator+(const SGPoint & rhs) const;
  //! Vector subtraction
  const SGPoint operator-(const SGPoint & rhs) const;
  //! Left-scalar multiplication
  friend SGPoint operator*(double d,const SGPoint & point);
  //! Right-scalar multiplication
  friend SGPoint operator*(const SGPoint & point,double d);
  //! Right scalar division.
  friend SGPoint operator/(const SGPoint & point,double d);
  //! Dot product.
  double operator*(const SGPoint & rhs) const; // dot product
  //! Equality
  bool operator==(const SGPoint & rhs) const;
  //! Not equls
  bool operator!=(const SGPoint & rhs) const;
  //! Greater than or equal
  bool operator>=(const SGPoint & rhs) const;
  //! Strictly greater
  bool operator>(const SGPoint & rhs) const;
  //! Less than or equal
  bool operator<=(const SGPoint & rhs) const;
  //! Strictly less
  bool operator<(const SGPoint & rhs) const;
  //! Greater than or equal to a scalar
  bool operator>=(double rhs) const;
  //! Strictly greater than a scalar
  bool operator>(double rhs) const;
  //! Less than or equal to a scalar
  bool operator<=(double rhs) const;
  //! Strictly less than a scalar
  bool operator<(double rhs) const;
  //! Output SGPoint to ostream
  friend ostream& operator<<(ostream& out, const SGPoint& rhs);

  
  static double invertSystem(SGPoint& x, const SGPoint&b,
			     const SGPoint& a0, const SGPoint& a1);
  static double intersectRay(SGPoint& intersection,
			     SGPoint& weights,
			     const SGPoint& pivot,
			     const SGPoint& direction,
			     const SGPoint& t0,
			     const SGPoint& t1);
  static double signedArea(const SGPoint& p0,
			   const SGPoint& p1,
			   const SGPoint& p2);

  //! Serialize an SGPoint
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & xy;
  }

  friend class boost::serialization::access;
  friend class SGTuple;
}; // SGPoint

#endif