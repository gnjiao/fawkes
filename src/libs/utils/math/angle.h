
/***************************************************************************
 *  angle.h - angle related math helper functions
 *
 *  Generated: Wed Jul 13 16:51:46 2005 (from FireVision)
 *  Copyright  2005  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE.GPL_WRE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL_WRE file in the doc directory.
 */

#ifndef __UTILS_MATH_ANGLE_H_
#define __UTILS_MATH_ANGLE_H_

#include <cmath>

namespace fawkes {


/** Convert an angle given in degrees to radians.
 * @param deg original value in degrees
 * @return converted value in radians
 */
inline float
deg2rad(float deg)
{
  return (deg * M_PI / 180.f);
}


/** Convert an angle given in radians to degrees.
 * @param rad original value in radians
 * @return converted value in degrees
 */
inline float
rad2deg(float rad)
{
  return (rad * 180.f / M_PI);
}


/** Get distance between two 2D cartesian coordinates.
 * @param x1 X coordinate of first point
 * @param y1 Y coordinate of first point
 * @param x2 X coordinate of second point
 * @param y2 Y coordinate of second point
 */
inline float
distance(float x1, float y1, float x2, float y2)
{
  return sqrt( (x2-x1) * (x2-x1) + (y2-y1) * (y2-y1) );
}

/** Normalize angle in radian between -PI and PI.
 * The given angle in radians is taken as an angle on the unit circle.
 * It is then normalized into the range -PI and PI, such that it is the
 * exact same angle on the unit circle but in the usual angle range.
 * @param angle_rad original value
 * @return normalized angle
 */
inline float 
normalize_mirror_rad(float angle_rad)
{
  if ( (angle_rad < -M_PI) || (angle_rad > M_PI) ) {
    return ( angle_rad - 2 * M_PI * round(angle_rad / (2 * M_PI)) );
  } else {
    return angle_rad;
  }
}

/** Normalize angle in radian between 0 and 2*PI.
 * The given angle in radians is taken as an angle on the unit circle.
 * It is then normalized into the range 0 and 2*PI, such that it is the
 * exact same angle on the unit circle but in the usual angle range.
 * @param angle_rad original value
 * @return normalized angle
 */
inline float
normalize_rad(float angle_rad)
{
  if ( (angle_rad < 0) || (angle_rad > 2 * M_PI) ) {
    return angle_rad - 2 * M_PI * floor(angle_rad / (M_PI * 2));
  } else {
    return angle_rad;
  }
}


inline void
cart2polar2d(float cart_x, float cart_y,
	     float *polar_phi, float *polar_dist)
{
  *polar_phi  = atan2f(cart_y, cart_x);
  *polar_dist = sqrtf(cart_x * cart_x + cart_y * cart_y);
}


inline void
polar2cart2d(float polar_phi, float polar_dist,
	     float *cart_x, float *cart_y)
{
  *cart_x = polar_dist * cosf(polar_phi);
  *cart_y = polar_dist * sinf(polar_phi);
}


} // end namespace fawkes

#endif
