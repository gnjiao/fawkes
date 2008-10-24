
/***************************************************************************
 *  service_chooser_dialog.cpp - Dialog for choosing a network service
 *
 *  Created: Sun Oct 12 17:06:06 2008 (split from lasergui_hildon.cpp)
 *  Copyright  2008  Tim Niemueller [www.niemueller.de]
 *
 *  $Id: lasergui.cpp 1432 2008-10-10 16:11:00Z tim $
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

#ifndef __LIBS_GUI_UTILS_SERVICE_CHOOSER_DIALOG_H_
#define __LIBS_GUI_UTILS_SERVICE_CHOOSER_DIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/treeview.h>
#include <gtkmm/entry.h>
#include <gtkmm/expander.h>
#include <gtkmm/scrolledwindow.h>
#include <gui_utils/service_view.h>

namespace fawkes {

class FawkesNetworkClient;

class ServiceChooserDialog
  : public Gtk::Dialog,
    public ServiceView
{
 public:
  ServiceChooserDialog(Gtk::Window &parent,
		       FawkesNetworkClient *client,
		       Glib::ustring title = "Select Service",
		       const char *service = "_fawkes._tcp");

  ServiceChooserDialog(Gtk::Window &parent,
		       Glib::ustring title = "Select Service",
		       const char *service = "_fawkes._tcp");

  virtual void initialize();

  void get_selected_service(Glib::ustring &name, Glib::ustring &hostname,
			    Glib::ustring &ipaddr, unsigned short int &port);

  void run_and_connect();

 protected:
  virtual void on_expander_changed();

 private:
  void ctor();
  fawkes::FawkesNetworkClient *__client;

  Gtk::Window         &__parent;
  Gtk::TreeView        __treeview;
  Gtk::Entry           __entry;
  Gtk::Expander        __expander;
  Gtk::ScrolledWindow  __scrollwin;
};

} // end of namespace fawkes

#endif