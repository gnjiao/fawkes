
/***************************************************************************
 *  segment_scanline.cpp - Implementation of scanline segmentation filter
 *                         This filter can be used to draw the segmentation for
 *                         all objects into a colored YUV422_PLANAR buffer
 *                         but only on the scanline model points
 *
 *  Generated: Thu Jul 14 15:04:23 2005
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

#include <filters/segment_scanline.h>

#include <models/color/colormodel.h>
#include <models/scanlines/scanlinemodel.h>

#include <fvutils/color/yuv.h>
#include <cstddef>


/** @class FilterScanlineSegmentation <filters/segment_scanline.h>
 * Segmentation filter.
 * Visually marks pixels depending of their classification determined by the
 * supplied color model to make the segmentation visible - but only the pixels
 * at scanline points.
 * The pixels are marked with the color matching the segmentation with an
 * appropriate place holder color.
 */

/** Constructor.
 * @param cm color model to use
 * @param slm scanline model to use
 */
FilterScanlineSegmentation::FilterScanlineSegmentation(ColorModel *cm, ScanlineModel *slm)
{
  src = dst = NULL;
  src_roi = dst_roi = NULL;
  this->cm = cm;
  this->slm = slm;
}


void
FilterScanlineSegmentation::setSrcBuffer(unsigned char *buf, ROI *roi, orientation_t ori, unsigned int buffer_num)
{
  src = buf;
  src_roi = roi;
}


void
FilterScanlineSegmentation::setSrcBuffer(unsigned char *buf, ROI *roi, unsigned int buffer_num)
{
  src = buf;
  src_roi = roi;
}

void
FilterScanlineSegmentation::setDstBuffer(unsigned char *buf, ROI *roi, orientation_t ori)
{
 dst = buf;
  dst_roi = roi;
}

void
FilterScanlineSegmentation::setOrientation(orientation_t ori)
{
}


const char *
FilterScanlineSegmentation::getName()
{
  return "FilterScanlineSegmentation";
}


void
FilterScanlineSegmentation::apply()
{

  unsigned int  x = 0, y = 0;
  unsigned char   py = 0, pu = 0, pv = 0;
  register unsigned char *dyp, *dup, *dvp;
  color_t c;


  slm->reset();
  while (! slm->finished()) {

    x = (*slm)->x;
    y = (*slm)->y;


    // Get source pixel values
    YUV422_PLANAR_YUV(src, src_roi->image_width, src_roi->image_height, x, y,  py,  pu,  pv);

    // destination y-plane
    dyp  = dst + (y * dst_roi->line_step) + (x * dst_roi->pixel_step);
    // destination u-plane
    dup  = YUV422_PLANAR_U_PLANE(dst, dst_roi->image_width, dst_roi->image_height)
                                   + (((y * dst_roi->line_step) + (x * dst_roi->pixel_step)) / 2) ;
    // destination v-plane
    dvp  = YUV422_PLANAR_V_PLANE(dst, dst_roi->image_width, dst_roi->image_height)
                                   + (((y * dst_roi->line_step) + (x * dst_roi->pixel_step)) / 2);

    c = cm->determine(py, pu, pv);

    switch (c) {
    case C_ORANGE:
      *dyp++ = 128;
      *dyp++ = 128;
      *dup++ = 0;
      *dvp++ = 255;
      break;
    case C_MAGENTA:
      *dyp++ = 128;
      *dyp++ = 128;
      *dup++ = 128;
      *dvp++ = 255;
      break;
    case C_CYAN:
      *dyp++ = 128;
      *dyp++ = 128;
      *dup++ = 255;
      *dvp++ = 0;
      break;
    case C_BLUE:
      *dyp++ = 128;
      *dyp++ = 128;
      *dup++ = 255;
      *dvp++ = 128;
      break;
    case C_YELLOW:
      *dyp++ = 255;
      *dyp++ = 255;
      *dup++ = 0;
      *dvp++ = 128;
      break;
    case C_GREEN:
      *dyp++ = 128;
      *dyp++ = 128;
      *dup++ = 0;
      *dvp++ = 0;
      break;
    case C_WHITE:
      *dyp++ = 255;
      *dyp++ = 255;
      *dup++ = 128;
      *dvp++ = 128;
      break;
    case C_RED:
      *dyp++ = 196;
      *dyp++ = 196;
      *dup++ = 0;
      *dvp++ = 255;
      break;
    default:
      *dyp++ = 0;
      *dyp++ = 0;
      *dup++ = 128;
      *dvp++ = 128;
      break;
    }
    ++(*slm);
  }
}
