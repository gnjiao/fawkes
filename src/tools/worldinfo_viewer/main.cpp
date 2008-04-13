
/***************************************************************************
 *  main.cpp - World Info Viewer
 *
 *  Created: Wed April 09 20:04:46 2008
 *  Copyright  2008  Daniel Beck
 *
 *  $Id$
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

#include <tools/worldinfo_viewer/worldinfo_viewer.h>
#include <tools/worldinfo_viewer/backend_thread.h>
#include <tools/worldinfo_viewer/data_container.h>
#include <utils/system/hostinfo.h>
#include <config/sqlite.h>

#include <libglademm/xml.h>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
  Thread::init_main();
  
  HostInfo* host_info = new HostInfo();
  Configuration* config = new SQLiteConfiguration(CONFDIR);
  config->load(host_info->short_name(), "default.db");
  delete host_info;

  std::string addr;
  unsigned int port;
  std::string key;
  std::string iv;
  try 
    {
      addr = config->get_string("/worldinfo/multicast_addr");
      port = config->get_uint("/worldinfo/udp_port");
      key = config->get_string("/worldinfo/encryption_key");
      iv  = config->get_string("/worldinfo/encryption_iv");
    } 
  catch (Exception &e) 
    {
      delete config;
      e.append("Could not get required configuration data for world info viewer");
      e.print_trace();
      throw;
    }
  
  delete config;

  WorldInfoDataContainer* data_container = new WorldInfoDataContainer;
  WorldInfoViewerBackendThread* backend_thread = new WorldInfoViewerBackendThread(data_container,
										  addr.c_str(), 
										  port,
										  key.c_str(), 
										  iv.c_str());

  backend_thread->start();

  try
    {
      Gtk::Main kit(argc, argv);
      Glib::RefPtr<Gnome::Glade::Xml> ref_xml = Gnome::Glade::Xml::create(RESDIR"/glade/worldinfo_viewer/worldinfo_viewer.glade");
      WorldInfoViewer viewer(ref_xml, data_container);
      backend_thread->new_data().connect( sigc::mem_fun(viewer, &WorldInfoViewer::redraw_field ) );
      backend_thread->new_gamestate_data().connect( sigc::mem_fun(viewer, &WorldInfoViewer::gamestate_changed ) );

      kit.run( viewer.get_window() );
    }
  catch (std::exception const& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
    }

  backend_thread->cancel();
  backend_thread->join();
  delete backend_thread;

  delete data_container;

  Thread::destroy_main();

  return 0;
}
