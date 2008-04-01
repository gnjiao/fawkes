
/***************************************************************************
 *  firestation.cpp - Firestation
 *
 *  Created: Wed Oct 10 14:19:30 2007
 *  Copyright  2007  Daniel Beck
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
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
 */

#include <tools/firestation/firestation.h>
#include <tools/firestation/mirror_calib.h>
#include <tools/firestation/color_train_widget.h>
#include <tools/firestation/fuse_transfer_widget.h>

#include <cams/fileloader.h>
#include <cams/shmem.h>
#include <cams/net.h>

#include <fvutils/ipc/shm_image.h>
#include <fvutils/color/conversions.h>
#include <fvutils/color/yuv.h>
#include <fvutils/scalers/lossy.h>
#include <fvutils/system/camargp.h>
#include <fvutils/writers/jpeg.h>
#include <fvutils/writers/fvraw.h>

#include <core/exception.h>

#include <gdkmm/pixbuf.h>

#include <arpa/inet.h>

#include <iostream>

/** @class Firestation tools/firestation/firestation.h
 * Control GUI for vision related stuff.
 */

using namespace std;

/** Constructor.
 * @param ref_xml reference pointer to the glade file
 */
Firestation::Firestation(Glib::RefPtr<Gnome::Glade::Xml> ref_xml)
{
  m_yuv_orig_buffer = 0;
  m_yuv_draw_buffer = 0;
  m_yuv_scaled_buffer = 0;
  m_rgb_scaled_buffer = 0;
  m_img_writer = 0;

  m_calib_tool = new MirrorCalibTool();

  m_camera = 0;

  m_img_src = SRC_NONE;
  m_op_mode = MODE_VIEWER;

  m_update_img.connect( sigc::mem_fun(*this, &Firestation::draw_image) );

  // --- main window ------------------------------------------------
  ref_xml->get_widget("wndMain", m_wnd_main);
  if ( !m_wnd_main )
    {
      throw std::runtime_error("Couldn't find wndMain.");
    }

  ref_xml->get_widget("imgImage", m_img_image);
  if ( !m_img_image )
    {
      throw std::runtime_error("Couldn't find imgImage.");
    }
  m_img_image->signal_size_allocate().connect( sigc::mem_fun(*this, &Firestation::resize_image) );
    
  ref_xml->get_widget("evtImageEventBox", m_evt_image);
  if ( !m_evt_image )
    {
      throw std::runtime_error("Couldn't find evtImageEventBox.");
    }
  m_evt_image->signal_button_press_event().connect( sigc::mem_fun(*this, &Firestation::image_click) );


  ref_xml->get_widget("trvShmImageIds", m_trv_shm_image_ids);
  if ( !m_trv_shm_image_ids )
    {
      throw std::runtime_error("Couldn't find trvShmImageIds.");
    }

  ref_xml->get_widget("stbStatus", m_stb_status);
  if ( !m_stb_status )
    {
      throw std::runtime_error("Couldn't find stbStatus.");
    }

  ref_xml->get_widget("ckbContTrans", m_ckb_cont_trans);
  if ( !m_ckb_cont_trans )
    {
      throw std::runtime_error("Couldn't find ckbContTrans.");
    }
  m_ckb_cont_trans->signal_toggled().connect( sigc::mem_fun(*this, &Firestation::update_image) );
  // ----------------------------------------------------------------


  // --- toolbar widgets --------------------------------------------
  ref_xml->get_widget("tbtnExit", m_tbtn_exit);
  if ( !m_tbtn_exit )
    {
      throw std::runtime_error("Couldn't find tbtmExit.");
    }
  m_tbtn_exit->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::exit) );
  
  ref_xml->get_widget("tbtnUpdate", m_tbtn_update);
  if ( !m_tbtn_update )
    {
      throw std::runtime_error("Couldn't find tbtnUpdate.");
    }
  m_tbtn_update->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::update_image) );

  ref_xml->get_widget("tbtnSave", m_tbtn_save);
  if ( !m_tbtn_save )
    {
      throw std::runtime_error("Couldn't find tbtnSave.");
    }
  m_tbtn_save->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::save_image) );
  
  ref_xml->get_widget("tbtnOpenFile", m_tbtn_open_file);
  if ( !m_tbtn_open_file )
    {
      throw std::runtime_error("Couldn't find tbtnOpenFile.");
    }
  m_tbtn_open_file->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::open_file) );
  
  ref_xml->get_widget("tbtnOpenFolder", m_tbtn_open_folder);
  if ( !m_tbtn_open_folder )
    {
      throw std::runtime_error("Couldn't find tbtnOpenFolder.");
    }
  m_tbtn_open_folder->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::open_folder) );
  
  ref_xml->get_widget("tbtnOpenShm", m_tbtn_open_shm);
  if ( !m_tbtn_open_shm )
    {
      throw std::runtime_error("Couldn't find tbtnOpenShm.");
    }
  m_tbtn_open_shm->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::open_shm) );

  ref_xml->get_widget("tbtnOpenFuse", m_tbtn_open_fuse);
  if ( !m_tbtn_open_fuse )
    {
      throw std::runtime_error("Couldn't find tbtnOpenFuse.");
    }
  m_tbtn_open_fuse->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::open_fuse) );
  // ----------------------------------------------------------------


  // --- dialogs ----------------------------------------------------
  ref_xml->get_widget("fcdOpenImage", m_fcd_open_image);
  if ( !m_fcd_open_image )
    {
      throw std::runtime_error("Couldn't find fcdOpenImage.");
    }

  Gtk::FileFilter* filter_jpg = new Gtk::FileFilter();
  filter_jpg->set_name("JPEG");
  filter_jpg->add_pattern("*.jpg");
  filter_jpg->add_pattern("*.jpeg");
  m_fcd_open_image->add_filter(*filter_jpg);

  Gtk::FileFilter* filter_fvraw = new Gtk::FileFilter();
  filter_fvraw->set_name("FVRaw");
  filter_fvraw->add_pattern("*.raw");
  filter_fvraw->add_pattern("*.fvraw");
  m_fcd_open_image->add_filter(*filter_fvraw);

  ref_xml->get_widget("fcdSaveImage", m_fcd_save_image);
  if ( !m_fcd_save_image )
    {
      throw std::runtime_error("Couldn't find fcdSaveImage.");
    }
  m_fcd_save_image->add_filter(*filter_jpg);
  m_fcd_save_image->add_filter(*filter_fvraw);

  ref_xml->get_widget("dlgOpenShm", m_dlg_open_shm);
  if (!m_dlg_open_shm)
    {
      throw std::runtime_error("Couldn't find dlgOpenShm.");
    }

  ref_xml->get_widget("trvShmImageIds", m_trv_shm_image_ids);
  if ( !m_trv_shm_image_ids )
    {
      throw std::runtime_error("Couldn't find trvShmImageIds.");
    }
  m_shm_list_store = Gtk::ListStore::create(m_shm_columns);
  m_trv_shm_image_ids->set_model(m_shm_list_store);
  m_trv_shm_image_ids->append_column("#", m_shm_columns.m_id);
  m_trv_shm_image_ids->append_column("Name", m_shm_columns.m_name);


  ref_xml->get_widget("dlgOpenFuse", m_dlg_open_fuse);
  if (!m_dlg_open_fuse)
    {
      throw std::runtime_error("Couldn't find dlgOpenFuse.");
    }

  ref_xml->get_widget("ckbFuseJpeg", m_ckb_fuse_jpeg);
  if (! m_ckb_fuse_jpeg )
    {
      throw std::runtime_error("Couldn't find ckbFuseJpeg.");
    }

  ref_xml->get_widget("trvFuseServices", m_trv_fuse_services);
  if ( !m_trv_fuse_services )
    {
      throw std::runtime_error("Couldn't find trvFuseServices.");
    }
  m_fuse_tree_store = Gtk::TreeStore::create(m_fuse_columns);
  m_trv_fuse_services->set_model(m_fuse_tree_store);
  //  m_trv_fuse_services->append_column("#", m_fuse_columns.m_id);
  m_trv_fuse_services->append_column("Name", m_fuse_columns.m_name);
  // ----------------------------------------------------------------


  // --- color train widget -----------------------------------------
  m_rbt_ct_ball = dynamic_cast<Gtk::RadioButton*>( get_widget(ref_xml, "rbtCtBall") );
  m_rbt_ct_field = dynamic_cast<Gtk::RadioButton*>( get_widget(ref_xml, "rbtCtField") );
  m_rbt_ct_lines = dynamic_cast<Gtk::RadioButton*>( get_widget(ref_xml, "rbtCtLines") );
  m_rbt_ct_ball->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::ct_object_changed) );
  m_rbt_ct_field->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::ct_object_changed) );
  m_rbt_ct_lines->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::ct_object_changed) );

  m_btn_ct_start = dynamic_cast<Gtk::Button*>( get_widget(ref_xml, "btnCtStart") );
  m_btn_ct_start->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::ct_start) );
  
  m_ctw = new ColorTrainWidget(this);
  m_ctw->set_update_img_signal(&m_update_img);

  Gtk::Button* btn;
  btn = dynamic_cast<Gtk::Button*>( get_widget(ref_xml, "btnCtUnselect") );
  m_ctw->set_reset_selection_btn(btn);

  btn = dynamic_cast<Gtk::Button*>( get_widget(ref_xml, "btnCtAdd") );
  m_ctw->set_add_to_lut_btn(btn);

  btn = dynamic_cast<Gtk::Button*>( get_widget(ref_xml, "btnCtSaveHistos") );
  m_ctw->set_save_histos_btn(btn);

  btn = dynamic_cast<Gtk::Button*>( get_widget(ref_xml, "btnCtSaveColormap") );
  m_ctw->set_save_lut_btn(btn);

  btn = dynamic_cast<Gtk::Button*>( get_widget(ref_xml, "btnCtLoadColormap") );
  m_ctw->set_load_lut_btn(btn);

  Gtk::Scale* scl;
  scl = dynamic_cast<Gtk::Scale*>( get_widget(ref_xml, "sclCtThreshold") );
  m_ctw->set_threshold_scl(scl);

  scl = dynamic_cast<Gtk::Scale*>( get_widget(ref_xml, "sclCtMinProb") );
  m_ctw->set_min_prob_scl(scl);

  Gtk::Image* img;
  img = dynamic_cast<Gtk::Image*>( get_widget(ref_xml, "imgCtSegmentation") );
  m_ctw->set_segmentation_img(img);

  img = dynamic_cast<Gtk::Image*>( get_widget(ref_xml, "imgCtColormap") );
  m_ctw->set_lut_img(img);

  Gtk::FileChooserDialog* fcd;
  fcd = dynamic_cast<Gtk::FileChooserDialog*>( get_widget(ref_xml, "fcdCtFilechooser") );
  m_ctw->set_filechooser_dlg(fcd);

  // ----------------------------------------------------------------


  // --- mirror calibration -----------------------------------------
  ref_xml->get_widget("btnMcStart", m_btn_mc_start);
  if ( !m_btn_mc_start )
    {
      throw std::runtime_error("Couldn't find btnMcStart.");
    }
  m_btn_mc_start->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::mc_start) );

  ref_xml->get_widget("entCalibDist", m_ent_mc_dist);
  if ( ! m_ent_mc_dist ) 
    {
      throw std::runtime_error("Couldn't find entCalibDist.");
    }

  ref_xml->get_widget("entCalibOri", m_ent_mc_ori);
  if ( ! m_ent_mc_ori ) 
    {
      throw std::runtime_error("Couldn't find entCalibOri.");
    }

  ref_xml->get_widget("btnCalibLoad", m_btn_mc_load);
  if ( ! m_btn_mc_load )
    {
      throw std::runtime_error("Couldn't find btnCalibLoad.");
    }
  m_btn_mc_load->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::mc_load) );

  ref_xml->get_widget("btnCalibSave", m_btn_mc_save);
  if ( ! m_btn_mc_save )
    {
      throw std::runtime_error("Couldn't find btnCalibSave.");
    }
  m_btn_mc_save->signal_clicked().connect( sigc::mem_fun(*this, &Firestation::mc_save) );

  ref_xml->get_widget("fcdCalibSave", m_fcd_mc_save);
  if ( ! m_fcd_mc_save )
    {
      throw std::runtime_error("Couldn't find fcdCalibSave.");
    }
  
  ref_xml->get_widget("fcdCalibLoad", m_fcd_mc_load);
  if ( ! m_fcd_mc_load )
    {
      throw std::runtime_error("Couldn't find fcdCalibLoad.");
    }
  // ----------------------------------------------------------------

  // --- fuse transfer widget ---------------------------------------
  m_ftw = new FuseTransferWidget();
  m_ftw->set_current_lut( m_ctw->get_colormap() );

  Gtk::TreeView* trv = dynamic_cast<Gtk::TreeView*>( get_widget(ref_xml, "trvFuseRemoteLuts") );
  m_ftw->set_remote_lut_list_trv(trv);
  trv = dynamic_cast<Gtk::TreeView*>( get_widget(ref_xml, "trvFuseLocalLuts") );
  m_ftw->set_local_lut_list_trv(trv);
  img = dynamic_cast<Gtk::Image*>( get_widget(ref_xml, "imgFuseLocal") );
  m_ftw->set_local_img(img);
  img = dynamic_cast<Gtk::Image*>( get_widget(ref_xml, "imgFuseRemote") );
  m_ftw->set_remote_img(img);
  // ----------------------------------------------------------------

  m_signal_update_image.connect( sigc::mem_fun(*this, &Firestation::update_image) );

  m_scaled_img_width = m_evt_image->get_width();
  m_scaled_img_height = m_evt_image->get_height();

  m_avahi_thread = new AvahiThread();
  m_avahi_thread->watch_service("_fountain._tcp", this);
  m_avahi_thread->start();
}

/** Destructor. */
Firestation::~Firestation()
{
  free(m_yuv_orig_buffer);
  free(m_yuv_draw_buffer);
  free(m_yuv_scaled_buffer);
  free(m_rgb_scaled_buffer);
  
  delete m_camera;
  delete m_img_writer;

  delete m_calib_tool;
  delete m_ctw;
  delete m_ftw;
  delete m_avahi_thread;
}

Gtk::Widget*
Firestation::get_widget(Glib::RefPtr<Gnome::Glade::Xml> ref_xml,
			const char* widget_name) const
{
  Gtk::Widget* widget;
  ref_xml->get_widget(widget_name, widget);
  if ( !widget )
    { 
      std::string err_str = "Couldn't find widget ";
      err_str += std::string(widget_name);
      err_str += ".";
      throw runtime_error(err_str);
    }

  return widget;
}

/** Returns reference to main window.
 * @return reference to main window
 */
Gtk::Window&
Firestation::get_window() const
{
  return *m_wnd_main;
}

/** Exit routine. */
void
Firestation::exit()
{
  cout << "Terminating avhai thread ... " << flush;
  m_avahi_thread->cancel();
  m_avahi_thread->join();
  cout << "done" << endl;

  if (SRC_NONE != m_img_src)
    { 
      cout << "Closing camera ... " << flush;
      m_camera->close();
      cout << "done" << endl;
    }

  m_wnd_main->hide();
}

/** Saves the current image. */
void
Firestation::save_image()
{
  if (m_img_src == SRC_NONE)
    { return; }

  m_fcd_save_image->set_transient_for(*this);

  int result = m_fcd_save_image->run();

  switch(result) 
    {
    case(Gtk::RESPONSE_OK):
      {
	delete m_img_writer;

	Glib::ustring filter_name = m_fcd_save_image->get_filter()->get_name();
	if ( Glib::ustring("JPEG") == filter_name )
	  {
	    m_img_writer = new JpegWriter();
	  }
	else if( Glib::ustring("FVRaw") == filter_name )
	  {
	    m_img_writer = new FvRawWriter();
	  }
	else
	  {
	    cout << "save_file(): unknown file format" << endl;
	    break;
	  }

	std::string filename = m_fcd_save_image->get_filename();
	m_img_writer->set_filename( filename.c_str() );
	m_img_writer->set_dimensions(m_img_width, m_img_height);
	m_img_writer->set_buffer(m_img_cs, m_yuv_orig_buffer);
	m_img_writer->write();
	
	std::cout << "Save file: " <<  filename << std::endl;
	break;
      }

    case(Gtk::RESPONSE_CANCEL):
      break;

    default:
      break;
    }
  
  m_fcd_save_image->hide();
}

/** Reads in a new image for the current image source. */
void
Firestation::update_image()
{
  if (m_img_src == SRC_NONE)
    { return; }

  m_camera->capture();
  memcpy(m_yuv_orig_buffer, m_camera->buffer(), m_img_size);
  memcpy(m_yuv_draw_buffer, m_camera->buffer(), m_img_size);
  m_camera->dispose_buffer();
  
  scale_image();
  draw_image();

  m_ctw->draw_segmentation_result();
}

/** Reads in an image from a file. */
void
Firestation::open_file()
{
  m_fcd_open_image->set_action(Gtk::FILE_CHOOSER_ACTION_OPEN);
  m_fcd_open_image->set_transient_for(*this);

  int result = m_fcd_open_image->run();
	
  switch(result)
    {
    case Gtk::RESPONSE_OK:
      {
	pre_open_img_src();
	
	std::string filename = m_fcd_open_image->get_filename();
	
	m_camera = new FileLoader( filename.c_str() );
	m_img_src = SRC_FILE;
	post_open_img_src();
	
	break;
      }
      
    case Gtk::RESPONSE_CANCEL:
      {
	break;
      }
      
    default:
      {
	break;
      }
    }

  m_fcd_open_image->hide();  
}

/** Reads in images from a directory. */
void
Firestation::open_folder()
{
  m_fcd_open_image->set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  m_fcd_open_image->set_transient_for(*this);

  int result = m_fcd_open_image->run();
	
  switch(result)
    {
    case Gtk::RESPONSE_OK:
      {
	pre_open_img_src();
	
	std::string extension;
	Glib::ustring filter_name = m_fcd_save_image->get_filter()->get_name();
	if ( Glib::ustring("JPEG") == filter_name )
	  { extension = "jpg"; }
	else if ( Glib::ustring("FVRaw") == filter_name )
	  { extension = "raw"; }

	std::string folder = m_fcd_open_image->get_current_folder();
	char* as;
	asprintf(&as, "file:file:dir=%s:ext=%s", folder.c_str(), extension.c_str());
	CameraArgumentParser cap(as);
	m_camera = new FileLoader( &cap );
	m_img_src = SRC_FILE;
	post_open_img_src();
	free(as);

	break;
      }
      
    case Gtk::RESPONSE_CANCEL:
      {
	break;
      }
      
    default:
      {
	break;
      }
    }

  m_fcd_open_image->hide();  
}

/** Opens a SHM image. */
void
Firestation::open_shm()
{
  unsigned int num_buffers = 0;
  SharedMemory::SharedMemoryIterator shmit;
  SharedMemoryImageBufferHeader* h = new SharedMemoryImageBufferHeader;
  shmit = SharedMemory::find(FIREVISION_SHM_IMAGE_MAGIC_TOKEN, h);
  
  if (shmit == SharedMemory::end())
    {
      m_stb_status->push("No SHM images found");
      return;
    }
  else
    {
      m_shm_list_store->clear();
      
      while ( shmit != SharedMemory::end() )
	{
	  ++num_buffers;
	  Gtk::TreeModel::Row row = *(m_shm_list_store->append());
	  row[m_shm_columns.m_id] = num_buffers;
	  const SharedMemoryImageBufferHeader* h = (SharedMemoryImageBufferHeader*)*shmit;
	  row[m_shm_columns.m_name] = h->image_id();
	  shmit++;
	}
    }
  
  m_dlg_open_shm->set_transient_for(*this);
  
  int result = m_dlg_open_shm->run();
  
  switch(result)
    {
    case Gtk::RESPONSE_OK:
      {
	delete m_shm_buffer;
	
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column;
	m_trv_shm_image_ids->get_cursor(path, column);
	
	Gtk::TreeModel::iterator iter = m_shm_list_store->get_iter(path);
	
	if (iter)
	  {
	    Gtk::TreeModel::Row row = *iter;
	    if (row)
	      {
		Glib::ustring name = row[m_shm_columns.m_name];
		pre_open_img_src();
		
		try
		  {
		    m_camera = new SharedMemoryCamera( name.c_str() );
		  }
		catch (Exception& e)
		  {
		    e.print_trace();
		  }
		
		m_img_src = SRC_SHM;
		
		post_open_img_src();
	      }
	  }
	else
	  {
	    std::cout << "invalid iter" << std::endl;
	  }
	
	break;
      }
      
    case Gtk::RESPONSE_CANCEL:
      break;
      
    default:
      break;
    }
  
  m_dlg_open_shm->hide();
}

/** Connects to a FUSE server. */
void
Firestation::open_fuse()
{
  Gtk::TreeModel::Children children = m_fuse_tree_store->children();
  if ( 0 == children.size() )
    {
      m_stb_status->push("No FUSE services found");
      return;
    }
  
  m_trv_fuse_services->expand_all();
  m_dlg_open_fuse->set_transient_for(*this);
  
  int result = m_dlg_open_fuse->run();
  
  switch(result)
    {
    case Gtk::RESPONSE_OK:
      {
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column;
	m_trv_fuse_services->get_cursor(path, column);
	
	Gtk::TreeModel::iterator iter = m_fuse_tree_store->get_iter(path);
	
	if (iter)
	  {
	    Gtk::TreeModel::Row row = *iter;
	    if (row)
	      {
 		Glib::ustring hostname = row[m_fuse_columns.m_service_hostname];
 		unsigned short int port = row[m_fuse_columns.m_service_port];
 		Glib::ustring image_id = row[m_fuse_columns.m_image_id];
 		bool jpeg = m_ckb_fuse_jpeg->get_active();

		pre_open_img_src();
		
		try
		  {
 		    m_camera = new NetworkCamera(hostname.c_str(), port, image_id.c_str(), jpeg);
 		    m_img_src = SRC_FUSE;
 		    post_open_img_src();
		  }
		catch (Exception& e)
		  {
		    m_img_src = SRC_NONE;
		    e.print_trace();
		  }
	      }
	  }
	else
	  {
	    std::cout << "invalid iter" << std::endl;
	  }
	
	break;
      }
      
    case Gtk::RESPONSE_CANCEL:
      break;
      
    default:
      break;
    }
  
  m_dlg_open_fuse->hide();
}

void
Firestation::pre_open_img_src()
{  
  if (SRC_NONE != m_img_src)
    {
      m_camera->stop();
      m_camera->close();
      
      delete m_camera;
      m_camera = 0;

      m_img_src = SRC_NONE;
    }
}

/** Stuff that is executed after an image source has been opened
 * successfully.
 */ 
void
Firestation::post_open_img_src()
{
  if (m_img_src == SRC_NONE) { return; }

  m_camera->open();
  m_camera->start();
  m_camera->capture();

  m_img_width = m_camera->pixel_width();
  m_img_height = m_camera->pixel_height();
  m_img_cs = m_camera->colorspace();
  
  m_img_size = colorspace_buffer_size( m_img_cs, 
				       m_img_width, 
				       m_img_height );
  
  m_yuv_orig_buffer = (unsigned char*) malloc(m_img_size);
  m_yuv_draw_buffer = (unsigned char*) malloc(m_img_size);
  memcpy(m_yuv_orig_buffer, m_camera->buffer(), m_img_size);
  memcpy(m_yuv_draw_buffer, m_camera->buffer(), m_img_size);

  m_camera->dispose_buffer();
  
  m_tbtn_update->set_sensitive(true);
  m_tbtn_save->set_sensitive(true);

  scale_image();
  draw_image();
	
  m_ctw->set_src_buffer(m_yuv_orig_buffer, m_img_width, m_img_height);
  m_ctw->set_draw_buffer(m_yuv_draw_buffer);
  m_ctw->draw_segmentation_result();
}

/** Handles the scaling of the displayed image.
 * @return true if redraw is necessary
 */
bool
Firestation::scale_image()
{
  if (m_img_src != SRC_NONE)
    {
      float scale_factor_width = m_scaled_img_width / (float) m_img_width;
      float scale_factor_height = m_scaled_img_height / (float) m_img_height;
      
      float new_scale_factor;
      new_scale_factor = (scale_factor_width < scale_factor_height) ? scale_factor_width : scale_factor_height;
      if ( new_scale_factor != m_scale_factor )
	{
	  m_scale_factor = new_scale_factor;
	  m_scaled_img_width = (unsigned int) floor(m_img_width * m_scale_factor);
	  m_scaled_img_height = (unsigned int) floor(m_img_height * m_scale_factor);
	  m_img_image->set_size_request(m_scaled_img_width, m_scaled_img_height);
	  m_evt_image->set_size_request(m_scaled_img_width, m_scaled_img_height);
	  
	  return true;
	}
    }
  
  return false;
}

/** Draws the image. */
void
Firestation::draw_image()
{
  if ( m_img_src == SRC_NONE ) { return; }

  LossyScaler scaler;
  scaler.set_original_buffer( m_yuv_draw_buffer );
  scaler.set_original_dimensions(m_img_width, m_img_height);
  scaler.set_scale_factor(m_scale_factor);
  scaler.set_scaled_dimensions(m_scaled_img_width, m_scaled_img_height);
  m_scaled_img_width = scaler.needed_scaled_width();
  m_scaled_img_height = scaler.needed_scaled_height();
  free(m_rgb_scaled_buffer);
  free(m_yuv_scaled_buffer);
  m_yuv_scaled_buffer = (unsigned char*) malloc( colorspace_buffer_size( m_img_cs,
									 m_scaled_img_width,
									 m_scaled_img_height ) );
  scaler.set_scaled_buffer(m_yuv_scaled_buffer);
  scaler.scale();
  
  m_rgb_scaled_buffer = (unsigned char*) malloc( colorspace_buffer_size( RGB,
									 m_scaled_img_width,
									 m_scaled_img_height ) );
  
  convert( m_img_cs, RGB,
	   m_yuv_scaled_buffer, m_rgb_scaled_buffer,
	   m_scaled_img_width, m_scaled_img_height );
  
  Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create_from_data( m_rgb_scaled_buffer,
								   Gdk::COLORSPACE_RGB,
								   false,
								   8,
								   m_scaled_img_width,
								   m_scaled_img_height,
								   3 * m_scaled_img_width );
  
  m_img_image->set(image);
}

/** Signal handler that is called whenever the window size is changed. 
 * @param allocation a Gtk allocation
 */
void
Firestation::resize_image(Gtk::Allocation& allocation)
{
  m_scaled_img_width = (unsigned int) allocation.get_width();
  m_scaled_img_height = (unsigned int) allocation.get_height();
  if ( scale_image() )
    {
      draw_image();
    } 
}

/** Handles mouse clicks in the image area.
 * @param event a Gtk event
 * @return true if signal was handled
 */
bool
Firestation::image_click(GdkEventButton* event)
{
  unsigned int image_x;
  unsigned int image_y;

  image_x = (unsigned int)rint(event->x / m_scale_factor);
  image_y = (unsigned int)rint(event->y / m_scale_factor);

  switch (m_op_mode)
    {
    case MODE_VIEWER:
      if (m_img_src != SRC_NONE)
	{
	  switch( m_img_cs )
	    {
	    case YUV422_PLANAR:
	      register unsigned char y;
	      register unsigned char u;
	      register unsigned char v;
	      YUV422_PLANAR_YUV( m_yuv_orig_buffer,
				 m_img_width,
				 m_img_height,
				 (unsigned int)rint(event->x / m_scale_factor),
				 (unsigned int)rint(event->y / m_scale_factor),
				 y, u, v );
	      cout << "Y: " << (unsigned int)y 
		   << " U: " << (unsigned int)u 
		   << " V: " << (unsigned int)v << endl;
	      break;
	    default:
	  cout << "Unhandled colorspace" << endl;
	    }
	}
      break;

    case MODE_COLOR_TRAIN:
      m_ctw->click( (unsigned int) rint(event->x / m_scale_factor),
		    (unsigned int) rint(event->y / m_scale_factor) );
      draw_image();
     break;

    case MODE_MIRROR_CALIB:
      {
	m_calib_tool->step( (unsigned int) rint(event->x / m_scale_factor),
			    (unsigned int) rint(event->y / m_scale_factor) );

	bool show;
	float next_dist;
	float next_ori;
	show = m_calib_tool->get_next(&next_dist, &next_ori);

	if (show)
	  {
	    char* next_dist_char = (char*) malloc(10);
	    char* next_ori_char = (char*) malloc(10);
	    
	    sprintf(next_dist_char, "%2f", next_dist);
	    sprintf(next_ori_char, "%2f", next_ori);
	    m_ent_mc_dist->set_text(Glib::ustring(next_dist_char));
	    m_ent_mc_ori->set_text(Glib::ustring(next_ori_char));
	    
	    free(next_dist_char);
	    free(next_ori_char);
	  }
	else
	  {
	    m_ent_mc_dist->set_text("");
	    m_ent_mc_ori->set_text("");
	  }
	
	break;
      }

    case MODE_MIRROR_CALIB_EVAL:
      {
	float dist;
	float phi;
	m_calib_tool->eval(image_x, image_y, &dist, &phi);
	printf("Distance: %2f\t Phi: %2f\n", dist, phi);
	break;
      }

    default:
      break;
    }

  return true;
}

/** Starts the color training. */
void
Firestation::ct_start()
{
  if (m_op_mode == MODE_COLOR_TRAIN)
    {
      m_op_mode = MODE_VIEWER;
      m_stb_status->push("Leaving color training mode");
    }
  else
    {
      if (m_img_src != SRC_NONE)
	{
	  m_ctw->set_fg_object( ct_get_fg_object() );
	  
	  m_op_mode = MODE_COLOR_TRAIN;

	  m_stb_status->push("Entering color training mode");
	}
    }
}

hint_t
Firestation::ct_get_fg_object()
{
  if ( m_rbt_ct_ball->get_active() )
    {
      return H_BALL;
    }
  else if ( m_rbt_ct_field->get_active() )
    {
      return H_FIELD;
    }
  else if ( m_rbt_ct_lines->get_active() )
    {
      return H_LINE;
    }
  else
    {
      printf("ct_get_fg_object(): UNKNOWN\n");
      return H_UNKNOWN;
    }
}

void
Firestation::ct_object_changed()
{
  if ( m_rbt_ct_ball->get_active() )
    {
      m_ctw->set_fg_object(H_BALL);
    }
  else if ( m_rbt_ct_field->get_active() )
    {
      m_ctw->set_fg_object(H_FIELD);
    }
  else if ( m_rbt_ct_lines->get_active() )
    {
      m_ctw->set_fg_object(H_LINE);
    }
  else
    {
      printf("ct_object_changed(): UNKNOWN\n");
    }
}

/** Start the mirror calibration process. */
void
Firestation::mc_start()
{
  if (m_op_mode == MODE_MIRROR_CALIB)
    {
      m_op_mode = MODE_VIEWER;
      m_stb_status->push("Leaving mirror calibration mode");
    }
  else
    {
      if (m_img_src != SRC_NONE)
	{
	  m_calib_tool->set_img_dimensions(m_img_width, m_img_height);
	  m_calib_tool->start();

	  m_op_mode = MODE_MIRROR_CALIB;

	  bool show;
	  float next_dist;
	  float next_ori;
	  show = m_calib_tool->get_next(&next_dist, &next_ori);
	  
	  if (show)
	    {
	      char* next_dist_char = (char*) malloc(10);
	      char* next_ori_char = (char*) malloc(10);
	      
	      sprintf(next_dist_char, "%2f", next_dist);
	      sprintf(next_ori_char, "%2f", next_ori);
	      m_ent_mc_dist->set_text(Glib::ustring(next_dist_char));
	      m_ent_mc_ori->set_text(Glib::ustring(next_ori_char));
	      
	      free(next_dist_char);
	      free(next_ori_char);
	    }
	  else
	    {
	      m_ent_mc_dist->set_text("");
	      m_ent_mc_ori->set_text("");
	    }
	  
	  m_stb_status->push("Entering mirror calibration mode");
	}
    }
}

/** Load mirror calibration data from a file. */
void
Firestation::mc_load()
{
  m_fcd_mc_load->set_transient_for(*this);

  Gtk::FileFilter filter_mirror;
  filter_mirror.set_name("Mirror Calibration");
  filter_mirror.add_pattern("*.mirror");
  filter_mirror.add_pattern("*.bulb");
  m_fcd_mc_load->add_filter(filter_mirror);
  
  int result = m_fcd_mc_load->run();

  switch(result)
    {
    case Gtk::RESPONSE_OK:
      {
	std::string filename = m_fcd_mc_load->get_filename();
	m_calib_tool->load( filename.c_str() );
	m_op_mode = MODE_MIRROR_CALIB_EVAL;
	break;
      }
    case Gtk::RESPONSE_CANCEL:
      break;
    default:
      break;
    }

  m_fcd_mc_load->hide();
}

/** Save calibration data to a file. */
void
Firestation::mc_save()
{
  m_fcd_mc_save->set_transient_for(*this);

  int result = m_fcd_mc_save->run();

  switch(result) 
    {
    case(Gtk::RESPONSE_OK):
      {
	std::string filename = m_fcd_mc_save->get_filename();

	m_calib_tool->save( filename.c_str() );
	break;
      }

    case(Gtk::RESPONSE_CANCEL):
      break;

    default:
      break;
    }
  
  m_fcd_mc_save->hide();

}

void
Firestation::all_for_now()
{
}

void
Firestation::cache_exhausted()
{
}

void
Firestation::browse_failed( const char* name,
			      const char* type,
			      const char* domain )
{
}

void
Firestation::service_added( const char* name,
			    const char* type,
			    const char* domain,
			    const char* host_name,
			    const struct sockaddr* addr,
			    const socklen_t addr_size,
			    uint16_t port,
			    std::list<std::string>& txt,
			    int flags )
{
  std::vector<FUSE_imageinfo_t> image_list;
  NetworkCamera cam(host_name, port);
  cam.open();
  image_list = cam.image_list();
 
  std::vector<FUSE_imageinfo_t>::iterator fit;

  Gtk::TreeModel::Children children = m_fuse_tree_store->children();
  Gtk::TreeModel::Row row = *(m_fuse_tree_store->append());
  row[m_fuse_columns.m_id] = children.size();
  row[m_fuse_columns.m_name] = Glib::ustring(name);
  row[m_fuse_columns.m_service_name] = Glib::ustring(name);
  row[m_fuse_columns.m_service_type] = Glib::ustring(type);
  row[m_fuse_columns.m_service_domain] = Glib::ustring(domain);
  row[m_fuse_columns.m_service_hostname] = Glib::ustring(host_name);
  row[m_fuse_columns.m_service_port] = port;

  for (fit = image_list.begin(); fit != image_list.end(); ++fit)
    {
      Gtk::TreeModel::Row childrow = *(m_fuse_tree_store->append(row.children()));
      childrow[m_fuse_columns.m_name] = Glib::ustring(fit->image_id);
      childrow[m_fuse_columns.m_service_name] = Glib::ustring(name);
      childrow[m_fuse_columns.m_service_type] = Glib::ustring(type);
      childrow[m_fuse_columns.m_service_domain] = Glib::ustring(domain);
      childrow[m_fuse_columns.m_service_hostname] = Glib::ustring(host_name);
      childrow[m_fuse_columns.m_service_port] = port;
      childrow[m_fuse_columns.m_image_id] = Glib::ustring(fit->image_id);
      childrow[m_fuse_columns.m_image_width] = fit->width;
      childrow[m_fuse_columns.m_image_height] = fit->height;
      childrow[m_fuse_columns.m_image_colorspace] = Glib::ustring( colorspace_to_string((colorspace_t) fit->colorspace) );
    }

  m_ftw->add_fountain_service(name, host_name, port);
}

void
Firestation::service_removed( const char* name,
			      const char* type,
			      const char* domain )
{
  Gtk::TreeModel::Children children = m_fuse_tree_store->children();
  Gtk::TreeModel::iterator rit;
  for (rit = children.begin(); rit != children.end(); ++rit)
    {
      Glib::ustring n = (*rit)[m_fuse_columns.m_service_name];
      Glib::ustring t = (*rit)[m_fuse_columns.m_service_type];
      Glib::ustring d = (*rit)[m_fuse_columns.m_service_domain];

      if ( strcmp( n.c_str(), name) == 0 &&
	   strcmp( t.c_str(), type) == 0 &&
	   strcmp( d.c_str(), domain) == 0 )
	{
	  m_fuse_tree_store->erase(rit);
	}
    }

  m_ftw->remove_fountain_service(name);
}
