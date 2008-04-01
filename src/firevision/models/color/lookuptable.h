
/**************************************************************************
 *  lookuptable.h - This header defines a lookup table color model
 *
 *  Generated: Fri Jun 10 14:16:52 2005
 *  Copyright  2005  Tim Niemueller  [www.niemueller.de]
 *
 *  $Id$
 *
 ***************************************************************************/

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

#ifndef __FIREVISION_MODELS_COLOR_LOOKUPTABLE_H_
#define __FIREVISION_MODELS_COLOR_LOOKUPTABLE_H_

#include <models/color/colormodel.h>

#include <fvutils/colormap/yuvcm.h>

class ColorModelLookupTable : public ColorModel
{
 public:

  ColorModelLookupTable(YuvColormap *colormap);
  ColorModelLookupTable(const char *file);
  ColorModelLookupTable(const char *file, const char *lut_id, bool destroy_on_free = false);
  ColorModelLookupTable(unsigned int depth, const char *lut_id, bool destroy_on_free);
  ColorModelLookupTable(const char *lut_id, bool destroy_on_free);

  virtual ~ColorModelLookupTable();

  virtual color_t determine(unsigned int y, unsigned int u, unsigned int v) const;

  const char *   get_name();
  YuvColormap *  get_colormap() const;

  void load(const char *filename);

 private:
  YuvColormap *__colormap;
};


inline color_t
ColorModelLookupTable::determine(unsigned int y, unsigned int u, unsigned int v) const
{
  return __colormap->determine(y, u, v);
}

#endif
