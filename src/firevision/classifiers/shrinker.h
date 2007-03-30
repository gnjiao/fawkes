
/***************************************************************************
 *  shrinker.h - Header for Shrinker
 *
 *  Generated: Wed Aug 31 21:25:14 2005
 *  Copyright  2005  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __FIREVISION_CLASSIFIER_SHRINKER_H_
#define __FIREVISION_CLASSIFIER_SHRINKER_H_

class ROI;

class Shrinker
{
 public:

  Shrinker();
  virtual ~Shrinker();

  virtual void setFilteredBuffer(unsigned char *yuv422planar_buffer);
  virtual void shrink( ROI *roi );

 protected:
  /** Source image buffer. */
  unsigned char *src;

};

#endif
