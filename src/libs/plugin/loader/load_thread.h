
/***************************************************************************
 *  load_thread.cpp - Plugin load thread
 *
 *  Created: Thu May 31 15:07:18 2007
 *  Copyright  2006-2008  Tim Niemueller [www.niemueller.de]
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

#ifndef __PLUGIN_LOADER_LOAD_THREAD_H_
#define __PLUGIN_LOADER_LOAD_THREAD_H_

#include <core/threading/thread.h>
#include <plugin/loader/plugin_loader.h>

namespace fawkes {

class ModuleManager;
class Plugin;
class Module;
class Configuration;

class PluginLoadThread : public Thread
{
 public:
  PluginLoadThread(ModuleManager *mm, const char *plugin_name,
		   Configuration *config);
  virtual ~PluginLoadThread();

  Plugin *  plugin();
  Module *  module();
  void      load_blocking();
  bool      finished();

  virtual void loop();

 private:
  void      load();

  char          *_module_name;
  bool           _finished;
  ModuleManager *_mm;
  Plugin        *_plugin;
  Module        *_module;
  Configuration *_config;
  PluginLoadException ple;
};


} // end namespace fawkes

#endif