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

#ifndef DT_GUI_TOOLS_GLOBAL_H
#define DT_GUI_TOOLS_GLOBAL_H

/** Globals toolbox is top right and is always the same thru all views
	it does include tools that are shared among the views...
*/



/** get the widget for the global tool, works like a singelton */
GtkWidget* dt_gui_tools_global_get ();

#endif
