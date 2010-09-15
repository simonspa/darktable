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

#include <glade/glade.h>
#include "common/collection.h"
#include "common/darktable.h"
#include "control/control.h"
#include "control/conf.h"
#include "gui/gtk.h"
#include "gui/tools/collection.h"

static GtkWidget *_gui_tools_lighttable_layout_tool = NULL;

static void
_gui_tools_lighttable_zoom_changed (GtkSpinButton *widget, gpointer user_data)
{
  const int i = gtk_spin_button_get_value(widget);
  dt_conf_set_int("plugins/lighttable/images_in_row", i);
  dt_control_gui_queue_draw();
}

static void
_gui_tools_lighttable_layout_changed (GtkComboBox *widget, gpointer user_data)
{
  const int i = gtk_combo_box_get_active(widget);
  dt_conf_set_int("plugins/lighttable/layout", i);
  dt_control_gui_queue_draw();
}

GtkWidget * 
dt_gui_tools_lighttable_layout_get ()
{
  /* singelton object */
  if (!_gui_tools_lighttable_layout_tool)
  {
    GtkWidget *widget=NULL;
    
    _gui_tools_lighttable_layout_tool = gtk_hbox_new (FALSE,2);
    
    /* initialize the mode combo box*/
    widget = gtk_combo_box_new_text ();
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("zoomable light table"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("file manager"));
    g_signal_connect (G_OBJECT (widget), "changed",
                    G_CALLBACK (_gui_tools_lighttable_layout_changed),
                    (gpointer)0);
    gtk_box_pack_start (GTK_BOX (_gui_tools_lighttable_layout_tool),widget,FALSE,FALSE,0);
    
    /* initialize the image on row spinbutton */
    widget = gtk_spin_button_new_with_range (1,26,1);
    gtk_spin_button_set_digits( GTK_SPIN_BUTTON (widget) ,0);
    g_signal_connect (G_OBJECT (widget), "value-changed",
                    G_CALLBACK (_gui_tools_lighttable_zoom_changed),
                    (gpointer)0);
    gtk_box_pack_start (GTK_BOX (_gui_tools_lighttable_layout_tool), widget,FALSE,FALSE,0);

 
    /* reference the widget so it's not destroyed upon remove from toolbar */
    g_object_ref (_gui_tools_lighttable_layout_tool);
  }  
  
  return _gui_tools_lighttable_layout_tool;
}

void 
dt_gui_tools_lighttable_layout_set_zoom (int zoom)
{
  GtkWidget *widget = g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (dt_gui_tools_lighttable_layout_get ())),1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), zoom);
}


void 
dt_gui_tools_lighttable_layout_restore_settings ()
{
  GtkWidget *widget = g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (dt_gui_tools_lighttable_layout_get ())),0);
  gtk_combo_box_set_active (GTK_COMBO_BOX (widget), dt_conf_get_int ("plugins/lighttable/layout"));

  widget = g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (dt_gui_tools_lighttable_layout_get ())),1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (widget), dt_conf_get_int ("plugins/lighttable/images_in_row"));

}

