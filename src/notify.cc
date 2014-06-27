/***
Copyright (C) 2013 Aniket Deole <aniket.deole@gmail.com>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License version 2.1, as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranties of
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <iostream>

#include "notify.hh"

Notify::Notify() {
	/** Window Related Properties **/
	set_default_size (960, 540);
    mainToolbar = new MainToolbar ();
    mainToolbar->setApp (this);

    set_titlebar(*mainToolbar);

    windowBody = Gtk::manage (new WindowBody (false, 0, Gtk::PACK_SHRINK));

	set_title("Notes For Linux");

	add(*windowBody);
	show_all ();
//	gdk_window_set_decorations (gtk_widget_get_window ((GtkWidget*) gobj ()), GDK_DECOR_BORDER);

    dbm = new DatabaseManager (this);

    windowBody->setApp (this);
    windowBody->setDatabaseManager (dbm);
}

Notify::~Notify () {}

void Notify::on_button_clicked () {
	std::cout << "HelloWorld!" << std::endl;
}

