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
#include "control/conf.h"
#include "gui/gtk.h"
#include "gui/tools/collection.h"

static GtkWidget *_gui_tools_collection_tool = NULL;


static void
_gui_tools_collection_update_query()
{
  /* updates query */
  dt_collection_update (darktable.collection);
  
  /* updates visual */
  GtkWidget *win = glade_xml_get_widget (darktable.gui->main_window, "center");
  gtk_widget_queue_draw (win);
}

static void
_gui_tools_collection_image_filter_changed (GtkComboBox *widget, gpointer user_data)
{
  // image_filter
  int i = gtk_combo_box_get_active (widget);
  if     (i == 0)  dt_conf_set_int ("ui_last/combo_filter",     DT_LIB_FILTER_ALL);
  else if(i == 1)  dt_conf_set_int ("ui_last/combo_filter",     DT_LIB_FILTER_STAR_NO);
  else if(i == 2)  dt_conf_set_int ("ui_last/combo_filter",     DT_LIB_FILTER_STAR_1);
  else if(i == 3)  dt_conf_set_int ("ui_last/combo_filter",     DT_LIB_FILTER_STAR_2);
  else if(i == 4)  dt_conf_set_int ("ui_last/combo_filter",     DT_LIB_FILTER_STAR_3);
  else if(i == 5)  dt_conf_set_int ("ui_last/combo_filter",     DT_LIB_FILTER_STAR_4);

  /* update collection star filter flags */
  if         (i == 0) dt_collection_set_filter_flags (darktable.collection, dt_collection_get_filter_flags (darktable.collection) & ~(COLLECTION_FILTER_ATLEAST_RATING|COLLECTION_FILTER_EQUAL_RATING));
  else if (i == 1) dt_collection_set_filter_flags (darktable.collection, (dt_collection_get_filter_flags (darktable.collection) | COLLECTION_FILTER_EQUAL_RATING) & ~COLLECTION_FILTER_ATLEAST_RATING);
  else dt_collection_set_filter_flags (darktable.collection, dt_collection_get_filter_flags (darktable.collection) | COLLECTION_FILTER_ATLEAST_RATING );
  
  /* set the star filter in collection */
  dt_collection_set_rating (darktable.collection, i-1);		
  
  _gui_tools_collection_update_query ();
}


static void
_gui_tools_collection_image_sort_changed (GtkComboBox *widget, gpointer user_data)
{
  // image_sort
  int i = gtk_combo_box_get_active (widget);
  if     (i == 0)  dt_conf_set_int ("ui_last/combo_sort",     DT_LIB_SORT_FILENAME);
  else if (i == 1)  dt_conf_set_int ("ui_last/combo_sort",     DT_LIB_SORT_DATETIME);
  else if (i == 2)  dt_conf_set_int ("ui_last/combo_sort",     DT_LIB_SORT_RATING);
  else if (i == 3)  dt_conf_set_int ("ui_last/combo_sort",     DT_LIB_SORT_ID);
  else if (i == 4)  dt_conf_set_int ("ui_last/combo_sort",     DT_LIB_SORT_COLOR);

  _gui_tools_collection_update_query ();
}



GtkWidget * 
dt_gui_tools_collection_get ()
{
  /* singelton object */
  if (!_gui_tools_collection_tool)
  {
    GtkWidget *widget=NULL;
    
    _gui_tools_collection_tool = gtk_hbox_new (FALSE,2);
    

    /* initialize the filter */
    gtk_box_pack_start (GTK_BOX (_gui_tools_collection_tool), gtk_label_new (_("list")),FALSE,FALSE,0);

    widget = gtk_combo_box_new_text ();
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("all"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("no star"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("1 star"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("2 star"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("3 star"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("4 star"));
    g_signal_connect (G_OBJECT (widget), "changed",
                    G_CALLBACK (_gui_tools_collection_image_filter_changed),
                    (gpointer)0);
    gtk_box_pack_start (GTK_BOX (_gui_tools_collection_tool),widget,FALSE,FALSE,0);

    
    /* initialize the sort */
    gtk_box_pack_start (GTK_BOX (_gui_tools_collection_tool), gtk_label_new (_("images sorted by")),FALSE,FALSE,0);

    widget = gtk_combo_box_new_text ();
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("filename"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("time"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("rating"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("id"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("color labels"));
    g_signal_connect (G_OBJECT (widget), "changed",
                      G_CALLBACK (_gui_tools_collection_image_sort_changed),
                      (gpointer)0);
                      
    gtk_box_pack_start (GTK_BOX (_gui_tools_collection_tool),widget,FALSE,FALSE,0);
    
    /* reference the widget so it's not destroyed upon remove from toolbar */
    g_object_ref (_gui_tools_collection_tool);
  }  
  
  return _gui_tools_collection_tool;
}

void 
dt_gui_tools_collection_restore_settings ()
{
  GtkWidget *widget = g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (dt_gui_tools_collection_get ())),1);
  dt_lib_filter_t filter = dt_conf_get_int ("ui_last/combo_filter");
  gtk_combo_box_set_active (GTK_COMBO_BOX (widget), (int)filter);

  widget = g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (dt_gui_tools_collection_get ())),3);
  dt_lib_sort_t sort = dt_conf_get_int ("ui_last/combo_sort");
  gtk_combo_box_set_active (GTK_COMBO_BOX (widget), (int)sort);
}