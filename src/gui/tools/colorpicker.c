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
#include "gui/tools/colorpicker.h"

static GtkWidget *_gui_tools_colorpicker_tool = NULL;

static void
_gui_tools_colorpicker_changed (GtkComboBox *widget, gpointer user_data)
{
  dt_conf_set_int ("ui_last/colorpicker_mean", gtk_combo_box_get_active (widget));
  dt_gui_tools_colorpicker_update ();
}



static void
_gui_tools_colorpicker_toggled (GtkToggleButton *button, gpointer p)
{
  if(darktable.gui->reset) return;
  
  /* find colorout module */
  /** \todo this should go into something like: dt_iop_find_module_by_name() */
  dt_iop_module_t *module = NULL;
  GList *modules = darktable.develop->iop;
  while(modules)
  {
    module = (dt_iop_module_t *)modules->data;
    if(!strcmp(module->op, "colorout")) break;
    modules = g_list_next(modules);
  }
  
  /* check if we got it */
  if(module)
  {
    dt_iop_request_focus(module);
    module->request_color_pick = gtk_toggle_button_get_active(button);
  }
  else
  {
    dt_iop_request_focus(NULL);
  }
  dt_control_gui_queue_draw();
}

GtkWidget * 
dt_gui_tools_colorpicker_get ()
{
  /* singelton object */
  if (!_gui_tools_colorpicker_tool)
  {
    GtkWidget *widget=NULL;
    
    _gui_tools_colorpicker_tool = gtk_hbox_new (FALSE,2);
    
    /* pick toggle button */
    widget = gtk_toggle_button_new_with_label (_("pick"));
      g_signal_connect (G_OBJECT (widget), "toggled",
                    G_CALLBACK (_gui_tools_colorpicker_toggled),
                    (gpointer)0);
    gtk_box_pack_start (GTK_BOX (_gui_tools_colorpicker_tool),widget,FALSE,FALSE,8);
   
    /* initialize the mean combo box*/
    widget = gtk_combo_box_new_text ();
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("mean"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("min"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (widget), _("max"));
    g_signal_connect (G_OBJECT (widget), "changed",
                    G_CALLBACK (_gui_tools_colorpicker_changed),
                    (gpointer)0);
    gtk_box_pack_start (GTK_BOX (_gui_tools_colorpicker_tool),widget,FALSE,FALSE,0);
 
    /* label */
    gtk_box_pack_start (GTK_BOX (_gui_tools_colorpicker_tool), gtk_label_new(_("before module")),FALSE,FALSE,8);
    
    /* module */
    gtk_box_pack_start (GTK_BOX (_gui_tools_colorpicker_tool), gtk_label_new(_("no module selected")),FALSE,FALSE,8);
 
    /* lab label*/
    gtk_box_pack_start (GTK_BOX (_gui_tools_colorpicker_tool), gtk_label_new(_("( --- )")),FALSE,FALSE,8);

    /* reference the widget so it's not destroyed upon remove from toolbar */
    g_object_ref (_gui_tools_colorpicker_tool);
  }  
  
  return _gui_tools_colorpicker_tool;
}

void 
dt_gui_tools_colorpicker_set_zoom (int zoom)
{
  GtkWidget *widget = g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (dt_gui_tools_colorpicker_get ())),1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), zoom);
}


void 
dt_gui_tools_colorpicker_restore_settings ()
{
  /* set the colorpicker  mean value*/
  GtkWidget *widget = g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (dt_gui_tools_colorpicker_get ())),1);
  gtk_combo_box_set_active(GTK_COMBO_BOX(widget), dt_conf_get_int("ui_last/colorpicker_mean"));
}


void dt_gui_tools_colorpicker_update ()
{
  /* find colorout module */
  /** \todo this should go into something like: dt_iop_find_module_by_name() */
  dt_iop_module_t *module = NULL;
  GList *modules = darktable.develop->iop;
  while (modules)
  {
    module = (dt_iop_module_t *)modules->data;
    if (!strcmp (module->op, "colorout")) break;
    modules = g_list_next (modules);
  }
  
  
  if(module)
  {
    /* update module label */
    char colstring[512];
    GtkWidget *w = GTK_WIDGET (g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (_gui_tools_colorpicker_tool)),3));
    snprintf(colstring, 512, C_("color picker module", "`%s'"), module->name());
    gtk_label_set_label(GTK_LABEL(w), colstring);
    
    /* update colorpicker toggle button */
    w = GTK_WIDGET (g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (_gui_tools_colorpicker_tool)),0));
    darktable.gui->reset = 1;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), module->request_color_pick);
    darktable.gui->reset = 0;

    // int input_color = 1; // Lab
    // always adjust picked color:
    int m = dt_conf_get_int("ui_last/colorpicker_mean");
    float *col;
    switch(m)
    {
      case 0: // mean
        //if(input_color == 1)
        col = module->picked_color_Lab;
        //else col = module->picked_color;
        break;
      case 1: //min
        //if(input_color == 1)
        col = module->picked_color_min_Lab;
        //else col = module->picked_color_min;
        break;
      default:
        //if(input_color == 1)
        col = module->picked_color_max_Lab;
        //else col = module->picked_color_max;
        break;
    }
  
    /* update output values label */
    w = GTK_WIDGET (g_list_nth_data (gtk_container_get_children (GTK_CONTAINER (_gui_tools_colorpicker_tool)),4));
    
    // switch(input_color)
    // {
    //   case 0: // linear rgb
    //     snprintf(colstring, 512, "%s: (%.03f, %.03f, %.03f)", _("linear rgb"), col[0], col[1], col[2]);
    //     break;
    //   case 1: // Lab
        snprintf(colstring, 512, "%s: (%.02f, %.02f, %.02f)", _("Lab"), col[0], col[1], col[2]);
    //     break;
    //   default: // output color profile
    //     snprintf(colstring, 512, "%s: (%.03f, %.03f, %.03f)", _("output profile"), col[0], col[1], col[2]);
    //     break;
    // }
    gtk_label_set_label(GTK_LABEL(w), colstring);
  }
}

