/*
    This file is part of darktable,
    copyright (c) 2009--2010 Henrik Andersson.

    darktable is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    darktable is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with darktable.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gdk/gdkkeysyms.h>
#include "common/collection.h"
#include "common/darktable.h"
#include "control/conf.h"
#include "views/view.h"
#include "gui/gtk.h"
#include "gui/tools/global.h"


static struct _gui_tools_global_tool_t 
{
  GtkWidget *container;
  
  GtkWidget *toggle_film_strip;
  GtkWidget *toggle_header;
  
}_gui_tools_global_tool = {0};


static void
_gui_tools_global_toggle_header (GtkWidget *widget,gpointer user_data)
{
  dt_conf_set_int("ui_last/header",gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget)));
  GtkWidget *header= glade_xml_get_widget (darktable.gui->main_window, "header");
  if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (widget)))
    gtk_widget_set_visible (header, TRUE);
  else
    gtk_widget_set_visible (header, FALSE);
  
  gtk_widget_queue_draw (header);
}

static gboolean 
_gui_tools_global_toggle_filmstrip (GtkWidget *widget,gpointer user_data)
{
  dt_view_manager_toggle_film_strip(darktable.view_manager);
  return TRUE;
}

static void _gui_tools_global_key_accelerator_toggle_filmstrip(void *data)
{
  _gui_tools_global_toggle_filmstrip(NULL,NULL);
}

static void _gui_tools_global_film_strip_toggled (GObject *object, gpointer user_data)
{
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (_gui_tools_global_tool.toggle_film_strip),(darktable.view_manager->film_strip_on)?TRUE:FALSE);
}

GtkWidget * 
dt_gui_tools_global_get ()
{
  /* singelton object */
  if (!_gui_tools_global_tool.container)
  {
    _gui_tools_global_tool.container = gtk_hbox_new (FALSE,2);
    
    
  
    /* add film strip toggle button*/
    _gui_tools_global_tool.toggle_film_strip = dtgtk_togglebutton_new(dtgtk_cairo_paint_filmstrip, 0);
    gtk_object_set(GTK_OBJECT(_gui_tools_global_tool.toggle_film_strip), "tooltip-text", _("toggle view of filmstrip (ctrl-f)"), NULL);
    gtk_box_pack_start (GTK_BOX (_gui_tools_global_tool.container), _gui_tools_global_tool.toggle_film_strip,FALSE,FALSE,0);
    if (dt_conf_get_bool("plugins/filmstrip/on"))
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (_gui_tools_global_tool.toggle_film_strip),TRUE);
    g_signal_connect(G_OBJECT(_gui_tools_global_tool.toggle_film_strip), "button-press-event", G_CALLBACK(_gui_tools_global_toggle_filmstrip),0);
    g_signal_connect(darktable.view_manager->events, "film-strip-view-toggled", G_CALLBACK(_gui_tools_global_film_strip_toggled),0);
    dt_gui_key_accel_register(GDK_CONTROL_MASK, GDK_f, _gui_tools_global_key_accelerator_toggle_filmstrip, 0);
    
    
    /* add header toggle */
    _gui_tools_global_tool.toggle_header = dtgtk_togglebutton_new(dtgtk_cairo_paint_header, 0);
    gtk_object_set(GTK_OBJECT(_gui_tools_global_tool.toggle_header), "tooltip-text", _("toggle view of header"), NULL);
    g_signal_connect(G_OBJECT(_gui_tools_global_tool.toggle_header), "toggled", G_CALLBACK(_gui_tools_global_toggle_header),0);
    if(dt_conf_get_int("ui_last/header"))
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (_gui_tools_global_tool.toggle_header),TRUE);
    else
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (_gui_tools_global_tool.toggle_header),FALSE);
      
    gtk_box_pack_start (GTK_BOX (_gui_tools_global_tool.container), _gui_tools_global_tool.toggle_header,FALSE,FALSE,0);
    
    /* reference the widget so it's not destroyed upon remove from toolbar */
    g_object_ref (_gui_tools_global_tool.container);
  }  
  
  return _gui_tools_global_tool.container;
}




