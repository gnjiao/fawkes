
/***************************************************************************
 *  imagecompressor.cpp - image compressor interface
 *
 *  Generated: Thu Mar 29 01:39:05 2007
 *  Copyright  2005-2007  Tim Niemueller [www.niemueller.de]
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

#include <fvutils/compression/imagecompressor.h>

/** @class ImageCompressor <fvutils/compression/imagecompressor.h>
 * Image compressor interface.
 *
 *
 * @fn void ImageCompressor::set_image_dimensions(unsigned int width, unsigned int height)
 * Set dimensions of image to compress.
 * @param width width of image in pixels
 * @param height height of image in pixels
 *
 * @fn void ImageCompressor::set_image_buffer(colorspace_t cspace, unsigned char *buffer)
 * Set image buffer to compress
 * @param cspace colorspace of image
 * @param buffer buffer that contains the image
 *
 * @fn void ImageCompressor::set_destination_buffer(unsigned char *buf, unsigned int buf_size)
 * Set destination buffer (if compressing to memory).
 * @param buf buffer
 * @param buf_size buffer size
 *
 * @fn unsigned int  ImageCompressor::compressed_size()
 * Get compressed size.
 * @return compressed size
 *
 * @fn void ImageCompressor::set_filename(const char *filename)
 * Set file name.
 * @param filename file name (if compressing to file)
 *
 * @fn void ImageCompressor::set_compression_destination(CompressionDestination cd)
 * Set compression destination.
 * @param cd compression destination
 *
 * @fn bool ImageCompressor::supports_compression_destination(CompressionDestination cd)
 * Check if compressor supports desired compression destination.
 * @param cd compression destination
 *
 * @fn unsigned int ImageCompressor::recommended_compressed_buffer_size()
 * Get the recommended size for the compressed buffer. It is not guaranteed
 * that this size is really enough for compression but in most cases this should
 * be suitable.
 * @return recommended buffer size
 *
 * @fn void ImageCompressor::compress()
 * Compress image.
 */

/** Virtual empty destructor. */
ImageCompressor::~ImageCompressor()
{
}
