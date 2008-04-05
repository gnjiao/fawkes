
/***************************************************************************
 *  vector.cpp - (Homogenous) vector
 *
 *  Created: Wed Sep 26 17:14:08 2007
 *  Copyright  2007  Daniel Beck
 *
 *  $Id$
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You can read the full text in the LICENSE file in the doc directory. 
 */

#include <geometry/hom_vector.h>
#include <geometry/hom_transform.h>
#include <core/exception.h>

#include <math.h>

using namespace std;



/** @class HomVector libs/geometry/hom_vector.h
 * A (homogeneous) vector.
 */

/** @var HomVector::mLength
 * The length of the vector.
 */

/** @var HomVector::mChanged
 * Indicates whether the vector was changed and the length possibly
 * needs to be recomputed.
 */


/**Constructor.
 * @param xval the x-coordinate
 * @param yval the y-coordinate
 * @param zval the z-coordinate
 */
HomVector::HomVector(float xval, float yval, float zval)
  : GeomPrim(xval, yval, zval)
{
  mElements(4,1) = 0.0;

  mChanged = true;
  mLength = length();
}


/**Constructor.
 * @param m a Matrix
 */
HomVector::HomVector(const Matrix& m)
  : GeomPrim(m)
{
  if (mElements(4,1) != 0)
    {
      cout << "Can't construct a HomVector from the given Matrix object since the 4th element is not equal to 0.0." << endl;
      Exception e("Vector::ctor(...)");
      throw e;
    }

  mChanged = true;
  mLength = length();
}


/**Constructor.
 * @param g a GeomPrim
 */
HomVector::HomVector(const GeomPrim& g)
  : GeomPrim(g)
{
  if (mElements(4,1) != 0)
    {
      cout << "Can't construct a HomVector from the given Matrix object since the 4th element is not equal to 0.0." << endl;
      Exception e("Vector::ctor(...)");
      throw e;
    }

  mChanged = true;
  mLength = length();
}

/**Destructor */
HomVector::~HomVector()
{
}


/**Calculates the length of the vector
 * @return the length
 */
float
HomVector::length()
{
  if (mChanged)
    {
      float t;
      t = x() * x();
      t += y() * y();
      t += z() * z();

      mLength = sqrt(t);
    }

  return mLength;
}


/**RO-getter for x.
 * @return the value
 */
float
HomVector::x() const
{
  return GeomPrim::_x();
}


/**RW-getter for x.
 * @return a reference to the x-element
 */
float&
HomVector::x()
{
  return GeomPrim::_x();
}


/**Setter function for x.
 * @param x the new x value
 */
void
HomVector::x(float x)
{
  GeomPrim::_x(x);
}


/**RO-getter for y.
 * @return the value
 */
float
HomVector::y() const
{
  return GeomPrim::_y();
}


/**RW-getter for y.
 * @return a reference to the y-element
 */
float&
HomVector::y()
{
  return GeomPrim::_y();
}


/**Setter function for y.
 * @param y the new y value
 */
void
HomVector::y(float y)
{
  GeomPrim::_y(y);
}


/**RO-getter for z.
 * @return the value
 */
float
HomVector::z() const
{
  return GeomPrim::_z();
}


/**RW-getter for z.
 * @return a reference to the z-element
 */
float&
HomVector::z()
{
  return GeomPrim::_z();
}


/**Setter function for z.
 * @param z the new z value
 */
void
HomVector::z(float z)
{
  GeomPrim::_z(z);
}


/**Apply a transform to the vector.
 * @param t a reference to the transform
 */
void
HomVector::apply_transform(const HomTransform& t)
{
  _apply_transform(t);
  mChanged = true;
}


/**Convenience function to rotate the vector around the x-axis.
 * @param angle the angle
 */
void
HomVector::rotate_x(float angle)
{
  _rotate_x(angle);
}


/**Convenience function to rotate the vector around the y-axis.
 * @param angle the angle
 */
void
HomVector::rotate_y(float angle)
{
  _rotate_y(angle);
}


/**Convenience function to rotate the vector around the z-axis.
 * @param angle the angle
 */
void
HomVector::rotate_z(float angle)
{
  _rotate_z(angle);
}


/**Convenience function to scale the vector along the axes of the
 * coordinate system.
 * @param scale_x scale factor along the x-axis
 * @param scale_y scale factor along the y-axis
 * @param scale_z scale factor along the z-axis
 */
void
HomVector::scale(float scale_x, float scale_y, float scale_z)
{
  x() *= scale_x;
  y() *= scale_y;
  z() *= scale_z;
}


/**Convenience function to scale the length of the vector.
 * @param l the scaling factor
 */
void
HomVector::scale_length(float l)
{
  scale(l, l, l);
}


/**Brings the vector to unit-length.
 * @return a reference to itself
 */
HomVector&
HomVector::unit()
{
  float len = length();

  x() /= len;
  y() /= len;
  z() /= len;

  return *this;
}


/**Assignment operator.
 * @param v the rhs vector
 * @return a reference of the lhs vector (this)
 */
HomVector&
HomVector::operator=(const HomVector& v)
{
  GeomPrim::operator=(v);
  mLength = v.mLength;
  mChanged = v.mChanged;

  return *this;
}


/**Addition operator.
 * Adds two vectors.
 * @param v the rhs vector
 * @return the resulting vector
 */
HomVector
HomVector::operator+(const HomVector& v) const
{
  HomVector result;

  result.x() = x() + v.x();
  result.y() = y() + v.y();
  result.z() = z() + v.z();

  return result;
}


/**Add-assign operator.
 * @param v the lhs vector
 * @return a reference to the resulting vector (this)
 */
HomVector&
HomVector::operator+=(const HomVector& v)
{
  *this = *this + v;

  return *this;
}


/** Substraction operator.
 * @param v the lhs vector
 * @return the resulting vector
 */
HomVector
HomVector::operator-(const HomVector& v) const
{
  HomVector result;

  result.x() = x() - v.x();
  result.y() = y() - v.y();
  result.z() = z() - v.z();

  return result;
}


/** Sub-assign operator.
 * @param v the lhs vector
 * @return a reference to the resulting vector (this)
 */
HomVector&
HomVector::operator-=(const HomVector& v)
{
  *this = *this - v;

  return *this;
}


/**Multiplikation with a scalar.
 * @param f the scalar
 * @return the resulting vector
 */
HomVector
HomVector::operator*(const float& f) const
{
  HomVector result;

  result.x() = x() * f;
  result.y() = y() * f;
  result.z() = z() * f;

  return result;
}


/**In-place multiplikation with a scalar.
 * @param f the scalar
 * @return a reference to thre resulting vector (this)
 */
HomVector&
HomVector::operator*=(const float& f)
{
  *this = *this * f;

  return *this;
}

/**Scalar product.
 * @param v the the other vector
 * @return float the scalar product
 */
float
HomVector::operator*(const HomVector& v) const
{
  float result;

  result  = x() * v.x();
  result += y() * v.y();
  result += z() + v.z();

  return result;
}

/**Cross-product.
 * @param v the lhs vector
 * @return the cross-product
 */
HomVector
HomVector::operator%(const HomVector& v) const
{
  HomVector result;

  result.x() = y() * v.z() - z() * v.y(); 
  result.y() = z() * v.x() - x() * v.z();
  result.z() = x() * v.y() - y() * v.x();

  return result;
}


/**In-place cross-product.
 * @param v the lhs vector
 * @return a reference to the resulting vector (this)
 */
HomVector&
HomVector::operator%=(const HomVector& v)
{
  *this = *this % v;

  return *this;
}


/**Angle between two vectors.
 * @param v the other vector
 * @return the angle in radians
 */
float
HomVector::operator<(HomVector& v)
{
  return acos( (*this) * v / (length() * v.length()) );
}