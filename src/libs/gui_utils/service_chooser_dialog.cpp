
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

#include <core/exception.h>
#include <core/exceptions/software.h>
#include <netcomm/fawkes/client.h>
#include <gui_utils/service_chooser_dialog.h>

#include <cstring>

namespace fawkes {

/** @class ServiceChooserDialog <gui_utils/service_chooser_dialog.h>
 * Service chooser dialog.
 * Allows to choose a service discovered via Avahi. Use the run routine,
 * it returns 1 if a service was selected or 0 if no service was found or
 * the selection was cancelled. The dialog is always modal.
 * @author Tim Niemueller
 */

/** Constructor.
 * @param parent parent window
 * @param title title of the dialog
 * @param service service string
 */
ServiceChooserDialog::ServiceChooserDialog(Gtk::Window &parent,
					   Glib::ustring title,
					   const char *service)
  : Gtk::Dialog(title, parent, /* modal */ true, /* separator */ true),
    ServiceView(service),
    __parent(parent), __expander("Manual entry")
{
  ctor();
  __client = NULL;
}


/** Constructor.
 * @param parent parent window
 * @param client Fawkes network client to connect on run()
 * @param title title of the dialog
 * @param service service string
 */
ServiceChooserDialog::ServiceChooserDialog(Gtk::Window &parent,
					   FawkesNetworkClient *client,
					   Glib::ustring title,
					   const char *service)
  : Gtk::Dialog(title, parent, /* modal */ true, /* separator */ true),
    ServiceView(service),
    __parent(parent), __expander("Manual entry")
{
  ctor();
  __client = client;
}


void
ServiceChooserDialog::ctor()
{
  set_default_size(360, 240);

  __treeview.set_model(m_service_list);
  __treeview.append_column("Service", m_service_record.name);
  __scrollwin.add(__treeview);
  __scrollwin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  __treeview.show();
  __expander.add(__entry);
  __entry.show();
  __entry.set_activates_default(true);

  Gtk::VBox *vbox = get_vbox();
  vbox->pack_start(__scrollwin);
  vbox->pack_end(__expander, Gtk::PACK_SHRINK);
  __scrollwin.show();
  __expander.show();

  add_button(Gtk::Stock::CANCEL, 0);
  add_button(Gtk::Stock::OK, 1);

  set_default_response(1);

#ifdef GLIBMM_PROPERTIES_ENABLED
  __expander.property_expanded().signal_changed().connect(sigc::mem_fun(*this, &ServiceChooserDialog::on_expander_changed));
#endif
}

void
ServiceChooserDialog::initialize()
{
}


/** Get selected service.
 * If a service has been selected use this method to get the IP Address as
 * string of the host that has the service and the port.
 * @param name name of the service
 * @param hostname hostname of the host associated with the service
 * @param ipaddr upon successful return contains the IP address as string
 * @param port upon successful return contains the port
 * @exception Exception thrown if no service has been selected
 */
void
ServiceChooserDialog::get_selected_service(Glib::ustring &name,
					   Glib::ustring &hostname,
					   Glib::ustring &ipaddr,
					   unsigned short int &port)
{
  Glib::RefPtr<Gtk::TreeSelection> treesel = __treeview.get_selection();
  if (__expander.get_expanded()) {
    if ( __entry.get_text_length() > 0 ) {
      char *tmpvalue = strdup(__entry.get_text().c_str());

      if ( strchr(tmpvalue, ':') != NULL ) {
	char *save_ptr;
	hostname = strtok_r(tmpvalue, ":", &save_ptr);
	char *tmpport = strtok_r(NULL, "", &save_ptr);

	int port_num = atoi(tmpport);
	if ( (port_num < 0) || (port_num > 0xFFFF) ) {
	  throw OutOfBoundsException("Invalid port", port_num, 0, 0xFFFF);
	}
	port = port_num;
      } else {
	hostname = tmpvalue;
      }

      // evil, but Glib::Regex is only availabel in ver >= 2.14, n/a on maemo
      ipaddr = hostname;

      free(tmpvalue);
      return;
    }
  }

  TreeModel::iterator iter = treesel->get_selected();
  if (iter) {
    TreeModel::Row row = *iter;
    name     = row[m_service_record.name];
    hostname = row[m_service_record.hostname];
    ipaddr   = row[m_service_record.ipaddr];
    port     = row[m_service_record.port];

  } else {
    throw Exception("No host selected");
  }
}

/** Signal handler for expander event.
 * Called when expander is (de-)expanded. Only works with Glibmm properties
 * enabled, i.e. not on Maemo.
 */
void
ServiceChooserDialog::on_expander_changed()
{
  if (__expander.get_expanded()) {
    __entry.grab_focus();
  } else {
    __treeview.grab_focus();
  }
}


/** Run dialog and try to connect.
 * This runs the service chooser dialog and connects to the given service
 * with the attached FawkesNetworkClient. If the connection couldn't be established
 * an error dialog is shown. You should not rely on the connection to be
 * active after calling this method, rather you should use a ConnectionDispatcher
 * to get the "connected" signal.
 */
void
ServiceChooserDialog::run_and_connect()
{
  if (! __client)  throw NullPointerException("FawkesNetworkClient not set");
  if (__client->connected()) throw Exception("Client is already connected");

  if ( run() ) {
    try {
      Glib::ustring name;
      Glib::ustring hostname;
      Glib::ustring ipaddr;
      unsigned short int port;
      get_selected_service(name, hostname, ipaddr, port);
      if ( port == 0 )  port = 1910;

      __client->connect(ipaddr.c_str(), port);
    } catch (Exception &e) {
      Glib::ustring message = "Connection failed: ";
      message += *(e.begin());
      Gtk::MessageDialog md(__parent, message, /* markup */ false,
			    Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK,
			    /* modal */ true);
      md.run();
    }
  }
}

} // end of namespace fawkes