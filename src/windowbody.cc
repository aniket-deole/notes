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

#include <gtkmm/box.h>
#include <gtkmm/widget.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/styleprovider.h>

#include <windowbody.hh>

WindowBody::WindowBody (bool homogeneous, int spacing, Gtk::PackOptions options, int padding, Notify* a, DatabaseManager* d) 
    : Gtk::Box (Gtk::ORIENTATION_VERTICAL, padding) {
    dbm = d;
    app = a;
	set_orientation (Gtk::ORIENTATION_VERTICAL);

//	add (*mainToolbar);

    Gtk::Box* body = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 0));

    Gtk::Paned* paneOne = Gtk::manage (new Gtk::Paned (Gtk::ORIENTATION_HORIZONTAL));
    addCss (paneOne, "paneOne", ".paneOne{ -GtkPaned-handle-size: 1px;}");

    npv = new NotePaneView (false, 0, Gtk::PACK_SHRINK, 0, app, dbm);
    app->npv = npv;
    nlpv = new NoteListPaneView (false, 0, Gtk::PACK_SHRINK, 0, app, dbm);
    app->nlpv = nlpv;
    lpv = new LeftPaneView (false, 0, Gtk::PACK_SHRINK,0, app, dbm);
    app->lpv = lpv;
    paneOne->pack1 (*lpv, false, false);

    Gtk::Box* rightFrameOfPaneOne = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 0));
    Gtk::Paned* paneTwo = Gtk::manage (new Gtk::Paned (Gtk::ORIENTATION_HORIZONTAL));

    paneTwo->pack1 (*nlpv, false, false);

    paneTwo->pack2 (*npv, true, false);

    rightFrameOfPaneOne->pack_start (*paneTwo, true, true, 0);

    paneOne->pack2 (*rightFrameOfPaneOne, true, false);

    body->pack_start (*paneOne, true, true, 0);

    pack_end (*body, Gtk::PACK_EXPAND_WIDGET, 0);
  	show_all ();

}

WindowBody::~WindowBody () {

}
