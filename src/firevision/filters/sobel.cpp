
/***************************************************************************
 *  sobel.cpp - Implementation of a Sobel filter
 *
 *  Generated: Thu May 12 13:20:43 2005
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

#include <filters/sobel.h>

#include <ippi.h>
#include <cstddef>


/** @class FilterSobel <filters/sobel.h>
 * Sobel filter.
 */

/** Constructor.
 * @param ori edge orientation
 */
FilterSobel::FilterSobel(orientation_t ori)
{
  src = dst = NULL;
  src_roi = dst_roi = NULL;
  orientation = ori;
}


void
FilterSobel::setSrcBuffer(unsigned char *buf, ROI *roi, orientation_t ori, unsigned int buffer_num)
{
  src = buf;
  src_roi = roi;
  orientation = ori;
}


void
FilterSobel::setSrcBuffer(unsigned char *buf, ROI *roi, unsigned int buffer_num)
{
  src = buf;
  src_roi = roi;
}


void
FilterSobel::setDstBuffer(unsigned char *buf, ROI *roi, orientation_t ori)
{
  dst = buf;
  dst_roi = roi;
  orientation = ori;
}


void
FilterSobel::setOrientation(orientation_t ori)
{
  orientation = ori;
}


const char *
FilterSobel::getName()
{
  return "FilterSobel";
}


/** Generate a sobel kernel for the given orientation.
 * @param k matrix for the kernel of size 3x3, contains three
 * lines concatenated into an one dimensional array.
 * @param ori requested orientation of the filter
 */
void
FilterSobel::generateKernel(int *k, orientation_t ori)
{
  // k is the kernel
  switch (ori) {
  case ORI_DEG_0:
  case ORI_DEG_360:
    k[0] =  1;    k[1] =  2;    k[2] =  1;
    k[3] =  0;    k[4] =  0;    k[5] =  0;
    k[6] = -1;    k[7] = -2;    k[8] = -1;
    break;
  case ORI_DEG_45:
    k[0] =  2;    k[1] =  1;    k[2] =  0;
    k[3] =  1;    k[4] =  0;    k[5] = -1;
    k[6] =  0;    k[7] = -1;    k[8] = -2;
    break;
  case ORI_DEG_90:
    k[0] =  1;    k[1] =  0;    k[2] = -1;
    k[3] =  2;    k[4] =  0;    k[5] = -2;
    k[6] =  1;    k[7] =  0;    k[8] = -1;
    break;
  case ORI_DEG_135:
    k[0] =  0;    k[1] = -1;    k[2] = -2;
    k[3] =  1;    k[4] =  0;    k[5] = -1;
    k[6] =  2;    k[7] =  1;    k[8] =  0;
    break;
  case ORI_DEG_180:
    k[0] = -1;    k[1] = -2;    k[2] = -1;
    k[3] =  0;    k[4] =  0;    k[5] =  0;
    k[6] =  1;    k[7] =  2;    k[8] =  1;
    break;
  case ORI_DEG_225:
    k[0] = -2;    k[1] = -1;    k[2] =  0;
    k[3] = -1;    k[4] =  0;    k[5] =  1;
    k[6] =  0;    k[7] =  1;    k[8] =  2;
    break;
  case ORI_DEG_270:
    k[0] = -1;    k[1] =  0;    k[2] =  1;
    k[3] = -2;    k[4] =  0;    k[5] =  2;
    k[6] = -1;    k[7] =  0;    k[8] =  1;
    break;
  case ORI_DEG_315:
    k[0] =  0;    k[1] =  1;    k[2] =  2;
    k[3] = -1;    k[4] =  0;    k[5] =  1;
    k[6] = -2;    k[7] = -1;    k[8] =  0;
    break;
  default:
    // cout << "FilterSobel: Cannote generate kernel for the given orientation." << endl;
    break;
  }
}


void
FilterSobel::apply()
{
  shrinkRegion(src_roi, 3);
  shrinkRegion(dst_roi, 3);

  IppiSize size;
  size.width = src_roi->width;
  size.height = src_roi->height;

  IppStatus status;

  if (orientation == ORI_HORIZONTAL) {
    //                                    base + number of bytes to line y              + pixel bytes
    status = ippiFilterSobelHoriz_8u_C1R( src + (src_roi->start.y * src_roi->line_step) + (src_roi->start.x * src_roi->pixel_step), src_roi->line_step,
					  dst + (dst_roi->start.y * dst_roi->line_step) + (dst_roi->start.x * dst_roi->pixel_step), dst_roi->line_step,
					  size );
  } else if (orientation == ORI_VERTICAL) {
    status = ippiFilterSobelHoriz_8u_C1R( src + (src_roi->start.y * src_roi->line_step) + (src_roi->start.x * src_roi->pixel_step), src_roi->line_step,
					  dst + (dst_roi->start.y * dst_roi->line_step) + (dst_roi->start.x * dst_roi->pixel_step), dst_roi->line_step,
					  size );

  } else if ( (orientation == ORI_DEG_0) ||
	      (orientation == ORI_DEG_45) ||
	      (orientation == ORI_DEG_90) ||
	      (orientation == ORI_DEG_135) ||
	      (orientation == ORI_DEG_180) ||
	      (orientation == ORI_DEG_225) ||
	      (orientation == ORI_DEG_270) ||
	      (orientation == ORI_DEG_315) ||
	      (orientation == ORI_DEG_360)
	      ) {

    Ipp32s kernel[9];
    generateKernel(kernel, orientation);

    IppiSize kernel_size;
    kernel_size.width = kernel_size.height = 3;

    IppiPoint anchor;
    anchor.x = anchor.y = 1;

    status = ippiFilter_8u_C1R( src + (src_roi->start.y * src_roi->line_step) + (src_roi->start.x * src_roi->pixel_step), src_roi->line_step,
				dst + (dst_roi->start.y * dst_roi->line_step) + (dst_roi->start.x * dst_roi->pixel_step), dst_roi->line_step,
				size,
				kernel, kernel_size,
				anchor,
				/* divisor */ 1 );
    
  } else {
    // cout << "FilterSobel: Unsupported direction" << endl;
    status = ippStsNullPtrErr;
  }


  /*
  cout << "FilterSobel: ippiFilterSobel exit code: " << flush;
  switch (status) {
  case ippStsNoErr:
    cout << "ippStsNoErr";
    break;
  case ippStsNullPtrErr:
    cout << "ippStsNullPtrErr";
    break;
  case ippStsSizeErr:
    cout << "ippStsSizeErr";
    break;
  case ippStsStepErr:
    cout << "ippStsStepErr";
    break;
  case ippStsMaskSizeErr:
    cout << "ippStsMaskSizeErr";
    break;
  default:
    cout << "Unknown status";
  }
  cout << endl;
  */
}
