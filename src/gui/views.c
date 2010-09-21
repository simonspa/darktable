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

#include "gui/views.h"
#include "gui/gtk.h"
#include "views/view.h"
#include "dtgtk/togglebutton.h"
#include <glade/glade.h>

#define HEADER_LABEL_NAME "header_label"
static GtkBox *_gui_views_box=NULL;

static void _gui_views_add_separator (gboolean sep)
{
  GtkWidget *widget = gtk_label_new( sep?"|":"");
  gtk_widget_set_name (widget,HEADER_LABEL_NAME);
  gtk_box_pack_start (_gui_views_box,widget,TRUE,FALSE,8);
  gtk_misc_set_alignment(GTK_MISC(widget), 0.5, 1.0);
}

void 
dt_gui_views_init () 
{
  _gui_views_box = GTK_BOX (gtk_hbox_new(FALSE,4));
  gtk_container_set_border_width( GTK_CONTAINER (_gui_views_box),4);
  
  GtkBox *box = GTK_BOX (glade_xml_get_widget (darktable.gui->main_window, "header"));
  gtk_box_pack_end (box,GTK_WIDGET (_gui_views_box),FALSE,FALSE,0);
  _gui_views_add_separator (FALSE);
  
  
  gtk_widget_show_all (GTK_WIDGET (box));
}

typedef struct _gui_view_label_item_t {
  GtkWidget *widget;
  gchar *name;
  int view;
} _gui_view_label_item_t;

static void _gui_views_switched(GObject *obj,gpointer user_data) 
{
  _gui_view_label_item_t *item = user_data;
  
  const gchar *name = dt_view_manager_name (darktable.view_manager);
  
  gtk_widget_set_state (item->widget,
    (strcmp(name,gtk_label_get_text (GTK_LABEL (item->widget)))==0)?GTK_STATE_ACTIVE:GTK_STATE_NORMAL
  );
}

static gboolean
_gui_views_toggled_callback (GtkWidget *label,gpointer user_data) 
{
  dt_gui_switch_view ((long int)user_data); 
  return TRUE;
}


void dt_gui_views_add ( char *name, int view)
{
  if (g_list_length (gtk_container_get_children (GTK_CONTAINER (_gui_views_box)))>1)
    _gui_views_add_separator(TRUE);
 
  /* create the view label */
  GtkWidget *widget = gtk_label_new(name);
  g_signal_connect (G_OBJECT (widget),"button-press-event", G_CALLBACK (_gui_views_toggled_callback), (gpointer)(long int)view);
  gtk_widget_set_name (widget,HEADER_LABEL_NAME);
  gtk_box_pack_start (_gui_views_box,widget,TRUE,FALSE,0);
  gtk_misc_set_alignment(GTK_MISC(widget), 0.5, 1.0);
  
  /* connect switch view manager event */
  _gui_view_label_item_t *item = g_malloc (sizeof(_gui_view_label_item_t));
  item->name = g_strdup (name);
  item->view = view;
  item->widget = widget;
  
  g_signal_connect (darktable.view_manager->events,"view-manager-switched-mode", G_CALLBACK (_gui_views_switched), item);

}

