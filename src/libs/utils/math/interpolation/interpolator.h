
/***************************************************************************
 *  interpolator.h - Interpolator
 *
 *  Created: Tue Nov 18 10:38:25 2008
 *  Copyright  2008  Tim Niemueller [www.niemueller.de]
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

#ifndef _UTILS_MATH_INTERPOLATION_INTERPOLATOR_H_
#define _UTILS_MATH_INTERPOLATION_INTERPOLATOR_H_

namespace fawkes {

class Interpolator
{
 public:
  virtual ~Interpolator();

  virtual float interpolate(float t_current, float t_end, float t_step,
			    float v_start, float v_end)                   = 0;
};


} // end namespace fawkes

#endif
