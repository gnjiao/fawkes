
/***************************************************************************
 *  reader.h - Reader interface
 *
 *  Generated: Sun Jun 04 23:14:43 2006
 *  Copyright  2005-2006  Tim Niemueller [www.niemueller.de]
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

#ifndef __FIREVISION_UTILS_READERS_READER_H_
#define __FIREVISION_UTILS_READERS_READER_H_

#include "fvutils/color/colorspaces.h"

class Reader {

 public:
  virtual ~Reader();

  virtual void             set_buffer(unsigned char *yuv422planar_buffer)  = 0;
  virtual colorspace_t     colorspace()                                    = 0;
  virtual unsigned int     pixel_width()                                   = 0;
  virtual unsigned int     pixel_height()                                  = 0;
  virtual void             read()                                          = 0;

};



#endif
