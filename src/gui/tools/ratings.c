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
#include "common/colorlabels.h"
#include "common/darktable.h"
#include "control/control.h"
#include "control/conf.h"
#include "dtgtk/togglebutton.h"
#include "gui/gtk.h"
#include "gui/tools/ratings.h"

static GtkWidget *_gui_tools_ratings_tool = NULL;


static void
_gui_tools_ratings_button_clicked(GtkWidget *widget, gpointer user_data)
{
  
  
}

GtkWidget * 
dt_gui_tools_ratings_get ()
{
  /* singelton object */
  if (!_gui_tools_ratings_tool)
  {
    GtkWidget *widget=NULL;
    
    _gui_tools_ratings_tool = gtk_hbox_new (FALSE,2);
    gtk_container_set_border_width (GTK_CONTAINER (_gui_tools_ratings_tool),2);
    for (int i=0;i<4;i++)
    {
      widget = dtgtk_togglebutton_new(dtgtk_cairo_paint_star, 0);
      gtk_box_pack_start (GTK_BOX (_gui_tools_ratings_tool),widget,FALSE,FALSE,0);
      g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(_gui_tools_ratings_button_clicked), (gpointer)i);
    }
    
    
    /* reference the widget so it's not destroyed upon remove from toolbar */
    g_object_ref (_gui_tools_ratings_tool);
  }  
  
  return _gui_tools_ratings_tool;
}




