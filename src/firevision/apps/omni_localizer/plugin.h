
/***************************************************************************
 *  plugin.h - Omni localizer plugin
 *
 *  Created: ???
 *  Copyright  2008  Volker Krause <volker.krause@rwth-aachen.de>
 *
 *  $Id: pipeline_thread.cpp 1049 2008-04-24 22:40:12Z beck $
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL file in the doc directory.
 */

#ifndef __FIREVISION_APPS_OMNI_LOCALIZER_PLUGIN_H_
#define __FIREVISION_APPS_OMNI_LOCALIZER_PLUGIN_H_

#include <core/plugin.h>

class FvOmniLocalizerPlugin : public fawkes::Plugin
{
  public:
    FvOmniLocalizerPlugin();
};

#endif
