
/***************************************************************************
 *  retriever_thread.cpp - FireVision Retriever Thread
 *
 *  Created: Tue Jun 26 17:39:11 2007
 *  Copyright  2006-2007  Tim Niemueller [www.niemueller.de]
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

#include <apps/retriever/retriever_thread.h>

#include <cams/camera.h>
#include <fvutils/ipc/shm_image.h>

/** @class FvRetrieverThread <apps/retriever/retriever_thread.h>
 * FireVision retriever thread.
 * This implements the functionality of the FvRetrieverPlugin.
 * @author Tim Niemueller
 */

/** Constructor. */
FvRetrieverThread::FvRetrieverThread()
  : Thread("FvRetrieverThread", Thread::OPMODE_WAITFORWAKEUP),
    VisionAspect(VisionAspect::CYCLIC)
{
}


/** Destructor. */
FvRetrieverThread::~FvRetrieverThread()
{
}


void
FvRetrieverThread::init()
{
  logger->log_debug(name(), "Registering for camera");
  try {
    cam = vision_master->register_for_camera(config->get_string("firevision", "/retriever/camera").c_str(), this);
  } catch (Exception &e) {
    e.append("FvRetrieverThread::init() failed");
    throw;
  }
  logger->log_error(name(), "Registered for camera");
  try {
    shm = new SharedMemoryImageBuffer("retriever", cam->colorspace(),
				      cam->pixel_width(), cam->pixel_height());
  } catch (Exception &e) {
    vision_master->unregister_thread(this);
    delete cam;
    cam = NULL;
    throw;
  }
}


void
FvRetrieverThread::finalize()
{
  logger->log_debug(name(), "Unregistering from vision master");
  vision_master->unregister_thread(this);
  delete cam;
  delete shm;
}


/** Thread loop. */
void
FvRetrieverThread::loop()
{
  /*
  logger->log_debug(name(), "Capturing frame from camera into shared memory buffer"
		    " (cam buffer size: %lu, shm buffer size: %lu)",
		    cam->buffer_size(), shm->data_size());
  */
  cam->capture();
  memcpy(shm->buffer(), cam->buffer(), cam->buffer_size()-1);
  cam->dispose_buffer();
  logger->log_debug(name(), "DONE");
}
