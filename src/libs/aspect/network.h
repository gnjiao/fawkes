
/***************************************************************************
 *  network.h - Network aspect for Fawkes
 *
 *  Created: Fri Jun 29 14:23:02 2007 (on flight to RoboCup 2007, Atlanta)
 *  Copyright  2006-2007  Tim Niemueller [www.niemueller.de]
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

#ifndef __ASPECT_NETWORK_H_
#define __ASPECT_NETWORK_H_

#include <netcomm/service_discovery/service_publisher.h>
#include <netcomm/service_discovery/service_browser.h>
#include <netcomm/utils/resolver.h>

namespace fawkes {

class NetworkAspect
{
 public:
  virtual ~NetworkAspect();

  void init_NetworkAspect(NetworkNameResolver *resolver,
			  ServicePublisher *service_publisher,
			  ServiceBrowser *service_browser);

 protected:
  NetworkNameResolver *nnresolver;
  ServicePublisher    *service_publisher;
  ServiceBrowser      *service_browser;
};

} // end namespace fawkes

#endif
