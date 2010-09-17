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

#ifndef DT_GUI_TOOLBARS_H
#define DT_GUI_TOOLBARS_H

#include <gtk/gtk.h>
typedef enum dt_gui_toolbar_t {
	TopLeftToolbar,
	TopCenterToolbar,
	TopRightToolbar,
	BottomLeftToolbar,
	BottomCenterToolbar,
	BottomRightToolbar
} dt_gui_toolbar_t;


/**  initializes the toolbars */
void dt_gui_toolbars_init ();
/** add tool to toolbar */
void dt_gui_toolbars_add_tool (dt_gui_toolbar_t toolbar,GtkWidget *); 
/** set tool of toolbar, discards currently available tools. */
void dt_gui_toolbars_set_tool (dt_gui_toolbar_t toolbar,GtkWidget *); 
/** add a seperator to toolbar, just some spacing... */
void dt_gui_toolbars_add_seperator (dt_gui_toolbar_t toolbar); 

/** removes all tools added to toolbar */
void dt_gui_toolbars_clear (dt_gui_toolbar_t toolbar);

/** creates a new toolbox for use in modules. */
GtkWidget *dt_gui_toolbars_toolbox_new ();

#endif