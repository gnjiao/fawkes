
/***************************************************************************
 *  ht_lines.h - Header of lines shape model
 *               using Hough Transform
 *
 *  Created: Fri Jan 13 12:40:57 2006
 *  Copyright  2005-2006  Tim Niemueller [www.niemueller.de]
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

#ifndef _FIREVISION_MODELS_SHAPE_HT_LINE_H_
#define _FIREVISION_MODELS_SHAPE_HT_LINE_H_

#include <vector>
#include <iostream>
#include <cmath>

#include <fvutils/base/types.h>
#include <fvmodels/shape/line.h>
#include <fvmodels/shape/accumulators/ht_accum.h>

namespace firevision {

class ROI;

class HtLinesModel: public ShapeModel
{
 private:
  std::vector<LineShape> m_Lines;
  RhtAccumulator accumulator;

 public:
  /** Creates a new HtLinesModel instance
   * @param nr_candidates the nr of candidates that is considered per pixel (the hole angle
   *                      range is devided in this many parts/lines
   * @param angle_from The angle to start the candidates from, given in rad, 0 is straight up
   * @param angle_range the angle range the candidates are taken from starting at angle_from,
   *                    given in rad, can be used for example to only search for horizontal lines
   * @param r_scale This can be done to reduce the size of the hough space and to map more lines
   *                to one line
   * @param min_votes_ratio The minimum ratio num_votes_per_line/total_num_votes that we have to
   *                        have before a point in the hough space is considered to be a line,
   *                        this may actually be higher if you use min_votes and set it to a higher
   *                        number (set min_votes to 0 to only use min_votes_ration)
   * @param min_votes the minimum number of votes a point in the hough space has to have before it
   *                  is considered to be a line. The number may actually be higher if min_votes_ratio
   *                  is set too high (set min_votes_ration to 0 to use only min_votes)
   */
  HtLinesModel(unsigned int nr_candidates = 40, float angle_from = 0, float angle_range= 2 * M_PI, int r_scale = 1, float min_votes_ratio = 0.2f, int min_votes = -1);
  virtual ~HtLinesModel(void);

  std::string	getName(void) const {return std::string("RhtLinesModel");}
  int		parseImage(unsigned char* buffer, ROI *roi);
  int		getShapeCount(void) const;
  LineShape*    getShape(int id) const;
  LineShape*	getMostLikelyShape(void) const;
  std::vector< LineShape > * getShapes();
  
 private:

  unsigned int  RHT_NR_CANDIDATES;
  float         RHT_ANGLE_INCREMENT;
  float         RHT_ANGLE_FROM;
  float         RHT_ANGLE_RANGE;
  
  // The following constants are used for RHT accumulator precision
  int           RHT_R_SCALE;
  //const int             RHT_PHI_SCALE   = 8;

  int           RHT_MIN_VOTES;
  float         RHT_MIN_VOTES_RATIO;

  unsigned int  roi_width;
  unsigned int  roi_height;

};

} // end namespace firevision

#endif // FIREVISION_MODELS_SHAPE_HT_LINES_H__

