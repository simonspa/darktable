/*
    This file is part of darktable,
    copyright (c) 2010 Henrik Andersson.

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

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "common/darktable.h"
#include "common/colorlabels.h"
#include "control/control.h"
#include "gui/gtk.h"
#include "dtgtk/button.h"


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

static GtkWidget *_gui_tools_colorlabels_tool = NULL;

char *_gui_tools_colorlabels_tooltips[4];



static void
_gui_tools_colorlabels_button_clicked(GtkWidget *widget, gpointer user_data)
{
  dt_colorlabels_key_accel_callback(user_data);
  dt_control_queue_draw_all();
}

GtkWidget * 
dt_gui_tools_colorlabels_get ()
{
  /* singelton object */
  if (!_gui_tools_colorlabels_tool)
  {
    _gui_tools_colorlabels_tooltips[0] = _("toggle red label\nof selected images (f1)");
    _gui_tools_colorlabels_tooltips[1] = _("toggle yellow label\nof selected images (f2)");
    _gui_tools_colorlabels_tooltips[2] = _("toggle green label\nof selected images (f3)");
    _gui_tools_colorlabels_tooltips[3] = _("clear all labels of selected images");
    
    GtkWidget *widget=NULL;
    
    _gui_tools_colorlabels_tool = gtk_hbox_new (FALSE,2);
    for (int i=0;i<4;i++)
    {
      widget = dtgtk_button_new(dtgtk_cairo_paint_label, (i|8));
      gtk_object_set(GTK_OBJECT(widget), "tooltip-text", _gui_tools_colorlabels_tooltips[i], NULL);
      gtk_box_pack_start (GTK_BOX (_gui_tools_colorlabels_tool),widget,FALSE,FALSE,0);
        g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(_gui_tools_colorlabels_button_clicked), (gpointer)i);
    }

    /* reference the widget so it's not destroyed upon remove from toolbar */
    g_object_ref (_gui_tools_colorlabels_tool);
  }  
  
  return _gui_tools_colorlabels_tool;
}




